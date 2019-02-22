/*
 *  iso_client_connection.c
 *
 *  Client side representation of the ISO stack (COTP, session, presentation, ACSE)
 *
 *  Copyright 2013 Michael Zillgith
 *
 *  This file is part of libIEC61850.
 *
 *  libIEC61850 is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  libIEC61850 is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with libIEC61850.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  See COPYING file for the complete license text.
 */

#include "libiec61850_platform_includes.h"

#include "stack_config.h"

#include "hal_socket.h"
#include "hal_thread.h"
#include "cotp.h"
#include "iso_session.h"
#include "iso_presentation.h"
#include "iso_client_connection.h"

#include "tls_config.h"
#include "acse.h"


#ifndef DEBUG_ISO_CLIENT
#ifdef DEBUG
#define DEBUG_ISO_CLIENT 1
#else
#define DEBUG_ISO_CLIENT 0
#endif /*DEBUG */
#endif /* DEBUG_ISO_SERVER */

#define STATE_IDLE 0
#define STATE_ASSOCIATED 1
#define STATE_ERROR 2

#define TPKT_RFC1006_HEADER_SIZE 4

#define ISO_CLIENT_BUFFER_SIZE CONFIG_MMS_MAXIMUM_PDU_SIZE + 100

struct sIsoClientConnection
{
    IsoIndicationCallback callback;
    void* callbackParameter;

    volatile int state;
    Semaphore stateMutex;

    Socket socket;

#if (CONFIG_MMS_SUPPORT_TLS == 1)
    TLSSocket tlsSocket;
#endif

    CotpConnection* cotpConnection;
    IsoPresentation* presentation;
    IsoSession* session;
    AcseConnection acseConnection;

    uint8_t* sendBuffer; /* ISO/MMS send buffer */
    uint8_t* receiveBuf; /* ISO/MMS receive buffer */
    ByteBuffer* receiveBuffer;

    ByteBuffer* transmitPayloadBuffer;
    Semaphore transmitBufferMutex;

    ByteBuffer* receivePayloadBuffer;
    Semaphore receiveBufferMutex;

    uint8_t* cotpReadBuf;
    uint8_t* cotpWriteBuf;

    ByteBuffer* cotpReadBuffer;
    ByteBuffer* cotpWriteBuffer;

    volatile bool handlingThreadRunning;
    volatile bool stopHandlingThread;
    volatile bool destroyHandlingThread;
    volatile bool startHandlingThread;

    Thread thread;
};

static void
setState(IsoClientConnection self, int newState)
{
    Semaphore_wait(self->stateMutex);
    self->state = newState;
    Semaphore_post(self->stateMutex);
}

static int
getState(IsoClientConnection self)
{
    int stateVal;

    Semaphore_wait(self->stateMutex);
    stateVal = self->state;
    Semaphore_post(self->stateMutex);

    return stateVal;
}

static void
connectionHandlingThread(IsoClientConnection self)
{
    IsoSessionIndication sessionIndication;

    self->handlingThreadRunning = true;

    if (DEBUG_ISO_CLIENT)
        printf("ISO_CLIENT_CONNECTION: new connection %p\n", self);

    /* Wait until lower layer association is finished */
    Semaphore_wait(self->receiveBufferMutex);

    CotpConnection_resetPayload(self->cotpConnection);

    while (true) {

        TpktState packetState;

        while ((packetState = CotpConnection_readToTpktBuffer(self->cotpConnection)) == TPKT_WAITING)
        {
            Thread_sleep(1);

            if (self->stopHandlingThread) {

                if (DEBUG_ISO_CLIENT)
                    printf("ISO_CLIENT_CONNECTION: stop thread requested\n");
                packetState = TPKT_ERROR;
                break;
            }
        }

        if (packetState == TPKT_ERROR)
            break;

        CotpIndication cotpIndication = CotpConnection_parseIncomingMessage(self->cotpConnection);

        if (cotpIndication == COTP_MORE_FRAGMENTS_FOLLOW)
            continue;

        if (cotpIndication != COTP_DATA_INDICATION)
            break;

        if (DEBUG_ISO_CLIENT)
            printf("ISO_CLIENT_CONNECTION: parse message\n");

        sessionIndication =
                IsoSession_parseMessage(self->session,
                        CotpConnection_getPayload(self->cotpConnection));

        if (sessionIndication != SESSION_DATA) {
            if (DEBUG_ISO_CLIENT)
                printf("ISO_CLIENT_CONNECTION: Invalid session message\n");
            break;
        }

        if (!IsoPresentation_parseUserData(self->presentation, IsoSession_getUserData(self->session))) {
            if (DEBUG_ISO_CLIENT)
                printf("ISO_CLIENT_CONNECTION: Invalid presentation message\n");
            break;
        }

        self->callback(ISO_IND_DATA, self->callbackParameter,
                &(self->presentation->nextPayload));

        /* wait for user to release the buffer */
        Semaphore_wait(self->receiveBufferMutex);

        CotpConnection_resetPayload(self->cotpConnection);
    }

    self->callback(ISO_IND_CLOSED, self->callbackParameter, NULL);;

    setState(self, STATE_IDLE);

#if (CONFIG_MMS_SUPPORT_TLS == 1)
    if (self->cotpConnection->tlsSocket)
        TLSSocket_close(self->cotpConnection->tlsSocket);
#endif

    Socket_destroy(self->socket);

    if (DEBUG_ISO_CLIENT)
        printf("ISO_CLIENT_CONNECTION: exit connection %p\n", self);

    /* release buffer to enable reuse of client connection */
    Semaphore_post(self->receiveBufferMutex);

    self->handlingThreadRunning = false;
}


static void*
connectionThreadFunction(void* parameter)
{
    IsoClientConnection self = (IsoClientConnection) parameter;

    while (self->destroyHandlingThread == false) {

        if (self->startHandlingThread) {
            self->startHandlingThread = false;
            connectionHandlingThread(self);
        }

        Thread_sleep(1);
    }

    self->destroyHandlingThread = false;

    return NULL;
}


IsoClientConnection
IsoClientConnection_create(IsoIndicationCallback callback, void* callbackParameter)
{
    IsoClientConnection self = (IsoClientConnection) GLOBAL_CALLOC(1, sizeof(struct sIsoClientConnection));

    if (self == NULL)
        return NULL;

    self->callback = callback;
    self->callbackParameter = callbackParameter;

    self->state = STATE_IDLE;
    self->stateMutex = Semaphore_create(1);

    self->sendBuffer = (uint8_t*) GLOBAL_MALLOC(ISO_CLIENT_BUFFER_SIZE);

    self->transmitPayloadBuffer = (ByteBuffer*) GLOBAL_CALLOC(1, sizeof(ByteBuffer));
    self->transmitPayloadBuffer->buffer = self->sendBuffer;
    self->transmitPayloadBuffer->maxSize = ISO_CLIENT_BUFFER_SIZE;

    self->receivePayloadBuffer = (ByteBuffer*) GLOBAL_CALLOC(1, sizeof(ByteBuffer));

    self->transmitBufferMutex = Semaphore_create(1);

    self->receiveBufferMutex = Semaphore_create(1);

    self->receiveBuf = (uint8_t*) GLOBAL_MALLOC(ISO_CLIENT_BUFFER_SIZE);
    self->receiveBuffer = (ByteBuffer*) GLOBAL_CALLOC(1, sizeof(ByteBuffer));
    ByteBuffer_wrap(self->receiveBuffer, self->receiveBuf, 0, ISO_CLIENT_BUFFER_SIZE);

    self->presentation = (IsoPresentation*) GLOBAL_CALLOC(1, sizeof(IsoPresentation));

    self->session = (IsoSession*) GLOBAL_CALLOC(1, sizeof(IsoSession));

    self->cotpReadBuf = (uint8_t*) GLOBAL_MALLOC(CONFIG_COTP_MAX_TPDU_SIZE + TPKT_RFC1006_HEADER_SIZE);
    self->cotpWriteBuf = (uint8_t*) GLOBAL_MALLOC(CONFIG_COTP_MAX_TPDU_SIZE + TPKT_RFC1006_HEADER_SIZE);

    self->cotpReadBuffer = (ByteBuffer*) GLOBAL_CALLOC(1, sizeof(ByteBuffer));
    ByteBuffer_wrap(self->cotpReadBuffer, self->cotpReadBuf, 0, CONFIG_COTP_MAX_TPDU_SIZE + TPKT_RFC1006_HEADER_SIZE);

    self->cotpWriteBuffer = (ByteBuffer*) GLOBAL_CALLOC(1, sizeof(ByteBuffer));
    ByteBuffer_wrap(self->cotpWriteBuffer, self->cotpWriteBuf, 0, CONFIG_COTP_MAX_TPDU_SIZE + TPKT_RFC1006_HEADER_SIZE);

    self->cotpConnection = (CotpConnection*) GLOBAL_CALLOC(1, sizeof(CotpConnection));

    self->handlingThreadRunning = false;

    self->stopHandlingThread = false;
    self->destroyHandlingThread = false;
    self->startHandlingThread = false;

    return self;
}

void
IsoClientConnection_associate(IsoClientConnection self, IsoConnectionParameters params,
        ByteBuffer* payload, uint32_t connectTimeoutInMs)
{
    self->socket = TcpSocket_create();

    if (self->socket == NULL)
        goto exit_error;

    Socket_setConnectTimeout(self->socket, connectTimeoutInMs);

    self->stopHandlingThread = false;
    self->destroyHandlingThread = false;

#if (CONFIG_ACTIVATE_TCP_KEEPALIVE == 1)
    Socket_activateTcpKeepAlive(self->socket,
            CONFIG_TCP_KEEPALIVE_IDLE,
            CONFIG_TCP_KEEPALIVE_INTERVAL,
            CONFIG_TCP_KEEPALIVE_CNT);
#endif

    if (!Socket_connect(self->socket, params->hostname, params->tcpPort))
        goto exit_error;

    /* COTP (ISO transport) handshake */
    CotpConnection_init(self->cotpConnection, self->socket, self->receiveBuffer, self->cotpReadBuffer, self->cotpWriteBuffer);

#if (CONFIG_MMS_SUPPORT_TLS == 1)
    if (params->tlsConfiguration) {

        /* create TLSSocket and start TLS authentication */
        TLSSocket tlsSocket = TLSSocket_create(self->socket, params->tlsConfiguration, false);

        if (tlsSocket)
            self->cotpConnection->tlsSocket = tlsSocket;
        else {

            if (DEBUG_ISO_CLIENT)
                printf("TLS handshake failed!\n");

            goto exit_error;
        }
    }
#endif /* (CONFIG_MMS_SUPPORT_TLS == 1) */


    /* COTP (ISO transport) handshake */
    CotpIndication cotpIndication =
            CotpConnection_sendConnectionRequestMessage(self->cotpConnection, params);

    TpktState packetState;

    uint64_t timeout = Hal_getTimeInMs() + CONFIG_TCP_READ_TIMEOUT_MS;

    while (((packetState = CotpConnection_readToTpktBuffer(self->cotpConnection)) == TPKT_WAITING)
        && (Hal_getTimeInMs() < timeout))
    {
        Thread_sleep(1);
    }

    if (packetState != TPKT_PACKET_COMPLETE)
        goto exit_error;

    cotpIndication = CotpConnection_parseIncomingMessage(self->cotpConnection);

    if (cotpIndication != COTP_CONNECT_INDICATION)
        goto exit_error;

    /* Upper layers handshake */
    struct sBufferChain sAcsePayload;
    BufferChain acsePayload = &sAcsePayload;
    acsePayload->buffer = payload->buffer;
    acsePayload->partLength = payload->size;
    acsePayload->length = payload->size;
    acsePayload->nextPart = NULL;

    AcseConnection_init(&(self->acseConnection), NULL, NULL, NULL);

    AcseAuthenticationParameter authParameter = params->acseAuthParameter;

    struct sBufferChain sAcseBuffer;
    BufferChain acseBuffer = &sAcseBuffer;

    acseBuffer->buffer = self->sendBuffer + payload->size;
    acseBuffer->partMaxLength = ISO_CLIENT_BUFFER_SIZE - acsePayload->length;

    AcseConnection_createAssociateRequestMessage(&(self->acseConnection), params, acseBuffer, acsePayload,
            authParameter);

    struct sBufferChain sPresentationBuffer;
    BufferChain presentationBuffer = &sPresentationBuffer;

    presentationBuffer->buffer = self->sendBuffer + acseBuffer->length;
    presentationBuffer->partMaxLength = ISO_CLIENT_BUFFER_SIZE - acseBuffer->length;

    IsoPresentation_init(self->presentation);
    IsoPresentation_createConnectPdu(self->presentation, params, presentationBuffer, acseBuffer);

    struct sBufferChain sSessionBuffer;
    BufferChain sessionBuffer = &sSessionBuffer;
    sessionBuffer->buffer = self->sendBuffer + presentationBuffer->length;

    IsoSession_init(self->session);
    IsoSession_createConnectSpdu(self->session, params, sessionBuffer,
            presentationBuffer);

    CotpConnection_sendDataMessage(self->cotpConnection, sessionBuffer);

    Semaphore_post(self->transmitBufferMutex);

    while ((packetState = CotpConnection_readToTpktBuffer(self->cotpConnection)) == TPKT_WAITING)
    {
        Thread_sleep(1);
    }

    cotpIndication = CotpConnection_parseIncomingMessage(self->cotpConnection);

    if (cotpIndication != COTP_DATA_INDICATION)
        goto exit_error;

    IsoSessionIndication sessionIndication;

    sessionIndication =
            IsoSession_parseMessage(self->session, CotpConnection_getPayload(self->cotpConnection));

    if (sessionIndication != SESSION_CONNECT) {
        if (DEBUG_ISO_CLIENT)
            printf("IsoClientConnection_associate: no session connect indication\n");
        goto exit_error;
    }

    if (!IsoPresentation_parseAcceptMessage(self->presentation, IsoSession_getUserData(self->session))) {
        if (DEBUG_ISO_CLIENT)
            printf("IsoClientConnection_associate: no presentation ok indication\n");
        goto exit_error;
    }

    AcseIndication acseIndication;

    acseIndication = AcseConnection_parseMessage(&(self->acseConnection), &self->presentation->nextPayload);

    if (acseIndication != ACSE_ASSOCIATE) {
        if (DEBUG_ISO_CLIENT)
            printf("IsoClientConnection_associate: no ACSE_ASSOCIATE indication\n");
        goto exit_error;
    }

    ByteBuffer_wrap(self->receivePayloadBuffer, self->acseConnection.userDataBuffer,
            self->acseConnection.userDataBufferSize, self->acseConnection.userDataBufferSize);

    Semaphore_wait(self->receiveBufferMutex);

    self->callback(ISO_IND_ASSOCIATION_SUCCESS, self->callbackParameter, self->receivePayloadBuffer);

    /* wait for upper layer to release buffer */
    Semaphore_wait(self->receiveBufferMutex);

    setState(self, STATE_ASSOCIATED);

    if (self->thread == NULL) {
        self->thread = Thread_create(connectionThreadFunction, self, false);
        Thread_start(self->thread);
    }

    self->startHandlingThread = true;

    while (self->handlingThreadRunning == false)
        Thread_sleep(1);

    return;

exit_error:
    self->callback(ISO_IND_ASSOCIATION_FAILED, self->callbackParameter, NULL);

    setState(self, STATE_ERROR);

    Socket_destroy(self->socket);
    self->socket = NULL;

    Semaphore_post(self->transmitBufferMutex);

    return;
}

void
IsoClientConnection_sendMessage(IsoClientConnection self, ByteBuffer* payloadBuffer)
{

    struct sBufferChain payloadBCMemory;
    BufferChain payload = &payloadBCMemory;

    BufferChain_init(payload, payloadBuffer->size, payloadBuffer->size, NULL, payloadBuffer->buffer);

    struct sBufferChain presentationBCMemory;
    BufferChain presentationBuffer = &presentationBCMemory;

    presentationBuffer->buffer = self->sendBuffer + payload->length;
    presentationBuffer->partMaxLength = ISO_CLIENT_BUFFER_SIZE;

    IsoPresentation_createUserData(self->presentation, presentationBuffer, payload);

    struct sBufferChain sessionBufferBCMemory;
    BufferChain sessionBuffer = &sessionBufferBCMemory;

    IsoSession_createDataSpdu(self->session, sessionBuffer, presentationBuffer);

    CotpIndication indication = CotpConnection_sendDataMessage(self->cotpConnection, sessionBuffer);

    /* release transmit buffer for use by API client */
    Semaphore_post(self->transmitBufferMutex);

    if (indication != COTP_OK)
        if (DEBUG_ISO_CLIENT)
            printf("ISO_CLIENT: IsoClientConnection_sendMessage: send message failed!\n");
}

void
IsoClientConnection_close(IsoClientConnection self)
{
    if (DEBUG_ISO_CLIENT)
        printf("ISO_CLIENT: IsoClientConnection_close\n");

    if (self->handlingThreadRunning) {
        self->stopHandlingThread = true;
        while (self->handlingThreadRunning)
            Thread_sleep(1);
    }

    setState(self, STATE_IDLE);
}


void
IsoClientConnection_destroy(IsoClientConnection self)
{
    if (DEBUG_ISO_CLIENT)
        printf("ISO_CLIENT: IsoClientConnection_destroy\n");

    if (getState(self) == STATE_ASSOCIATED) {

        if (DEBUG_ISO_CLIENT)
            printf("ISO_CLIENT: call IsoClientConnection_close\n");

        IsoClientConnection_close(self);
    }

    /* stop handling thread */
    self->destroyHandlingThread = true;

    if (self->thread != NULL) {
        while (self->destroyHandlingThread)
            Thread_sleep(1);

        Thread_destroy(self->thread);
    }

    if (self->receiveBuf != NULL)
        GLOBAL_FREEMEM(self->receiveBuf);
    if (self->receiveBuffer != NULL)
        GLOBAL_FREEMEM(self->receiveBuffer);
    if (self->cotpConnection != NULL)
        GLOBAL_FREEMEM(self->cotpConnection);

    if (self->cotpReadBuffer != NULL)
        GLOBAL_FREEMEM(self->cotpReadBuffer);

    if (self->cotpReadBuf != NULL)
        GLOBAL_FREEMEM(self->cotpReadBuf);

    if (self->cotpWriteBuffer != NULL)
        GLOBAL_FREEMEM(self->cotpWriteBuffer);

    if (self->cotpWriteBuf != NULL)
        GLOBAL_FREEMEM(self->cotpWriteBuf);

    if (self->session != NULL)
        GLOBAL_FREEMEM(self->session);
    if (self->presentation != NULL)
        GLOBAL_FREEMEM(self->presentation);

    GLOBAL_FREEMEM(self->transmitPayloadBuffer);
    GLOBAL_FREEMEM(self->receivePayloadBuffer);

    Semaphore_destroy(self->receiveBufferMutex);
    Semaphore_destroy(self->transmitBufferMutex);
    Semaphore_destroy(self->stateMutex);

    GLOBAL_FREEMEM(self->sendBuffer);
    GLOBAL_FREEMEM(self);
}

bool
IsoClientConnection_abort(IsoClientConnection self)
{
    //TODO block other messages from being sent
    IsoClientConnection_allocateTransmitBuffer(self);

    struct sBufferChain sAcseBuffer;
    BufferChain acseBuffer = &sAcseBuffer;
    acseBuffer->partMaxLength = ISO_CLIENT_BUFFER_SIZE;
    acseBuffer->buffer = self->sendBuffer;
    acseBuffer->nextPart = NULL;

    AcseConnection_createAbortMessage(NULL, acseBuffer, false);

    struct sBufferChain sPresentationBuffer;
    BufferChain presentationBuffer = &sPresentationBuffer;
    presentationBuffer->partMaxLength = ISO_CLIENT_BUFFER_SIZE - acseBuffer->length;
    presentationBuffer->buffer = self->sendBuffer + acseBuffer->length;
    presentationBuffer->nextPart = acseBuffer;

    IsoPresentation_createAbortUserMessage(self->presentation, presentationBuffer, acseBuffer);

    struct sBufferChain sSessionBuffer;
    BufferChain sessionBuffer = &sSessionBuffer;
    sessionBuffer->partMaxLength = ISO_CLIENT_BUFFER_SIZE - presentationBuffer->length;
    sessionBuffer->buffer = self->sendBuffer + presentationBuffer->length;
    sessionBuffer->nextPart = presentationBuffer;

    IsoSession_createAbortSpdu(self->session, sessionBuffer, presentationBuffer);

    if (DEBUG_ISO_CLIENT)
        printf("ISO_CLIENT: send abort message\n");

    CotpConnection_sendDataMessage(self->cotpConnection, sessionBuffer);

    Semaphore_post(self->transmitBufferMutex);

    uint64_t timeout = Hal_getTimeInMs() + CONFIG_TCP_READ_TIMEOUT_MS;

    while ((self->handlingThreadRunning == true) && (Hal_getTimeInMs() < timeout));

    if (self->handlingThreadRunning)
        return false;
    else
        return true;
}

void
IsoClientConnection_release(IsoClientConnection self)
{
    //TODO block other messages from being sent
    IsoClientConnection_allocateTransmitBuffer(self);

    struct sBufferChain sAcseBuffer;
    BufferChain acseBuffer = &sAcseBuffer;
    acseBuffer->partMaxLength = ISO_CLIENT_BUFFER_SIZE;
    acseBuffer->buffer = self->sendBuffer;
    acseBuffer->nextPart = NULL;

    AcseConnection_createReleaseRequestMessage(NULL, acseBuffer);

    struct sBufferChain sPresentationBuffer;
    BufferChain presentationBuffer = &sPresentationBuffer;
    presentationBuffer->partMaxLength = ISO_CLIENT_BUFFER_SIZE - acseBuffer->length;
    presentationBuffer->buffer = self->sendBuffer + acseBuffer->length;
    presentationBuffer->nextPart = acseBuffer;

    IsoPresentation_createUserDataACSE(self->presentation, presentationBuffer, acseBuffer);

    struct sBufferChain sSessionBuffer;
    BufferChain sessionBuffer = &sSessionBuffer;
    sessionBuffer->partMaxLength = ISO_CLIENT_BUFFER_SIZE - presentationBuffer->length;
    sessionBuffer->buffer = self->sendBuffer + presentationBuffer->length;
    sessionBuffer->nextPart = presentationBuffer;

    IsoSession_createFinishSpdu(NULL, sessionBuffer, presentationBuffer);

    if (DEBUG_ISO_CLIENT)
        printf("ISO_CLIENT: send release message\n");

    CotpConnection_sendDataMessage(self->cotpConnection, sessionBuffer);

    Semaphore_post(self->transmitBufferMutex);
}


ByteBuffer*
IsoClientConnection_allocateTransmitBuffer(IsoClientConnection self)
{
    Semaphore_wait(self->transmitBufferMutex);
    self->transmitPayloadBuffer->size = 0;
    self->transmitPayloadBuffer->maxSize = ISO_CLIENT_BUFFER_SIZE;
    return self->transmitPayloadBuffer;
}

void
IsoClientConnection_releaseReceiveBuffer(IsoClientConnection self)
{
    Semaphore_post(self->receiveBufferMutex);
}
