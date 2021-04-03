/*
 *  iso_client_connection.c
 *
 *  Client side representation of the ISO stack (COTP, session, presentation, ACSE)
 *
 *  Copyright 2013-2018 Michael Zillgith
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
#define STATE_CONNECTED 1
#define STATE_ERROR 2
#define STATE_CONNECTING 3

#define TPKT_RFC1006_HEADER_SIZE 4

#define ISO_CLIENT_BUFFER_SIZE CONFIG_MMS_MAXIMUM_PDU_SIZE + 100

typedef enum {
    INT_STATE_IDLE,
    INT_STATE_TCP_CONNECTING,
    INT_STATE_WAIT_FOR_COTP_CONNECT_RESP,
    INT_STATE_WAIT_FOR_ACSE_RESP,
    INT_STATE_WAIT_FOR_DATA_MSG,
    INT_STATE_CLOSING_CONNECTION,
    INT_STATE_CLOSE_ON_ERROR,
    INT_STATE_ERROR
} eIsoClientInternalState;

struct sIsoClientConnection
{
    IsoConnectionParameters parameters;

    IsoIndicationCallback callback;
    void* callbackParameter;

    volatile eIsoClientInternalState intState;
    volatile int state;
    Semaphore stateMutex;

    uint32_t readTimeoutInMs; /* read timeout in ms */
    uint64_t nextReadTimeout; /* timeout value for read and connect */

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

    Semaphore tickMutex;

    uint8_t* cotpReadBuf;
    uint8_t* cotpWriteBuf;

    ByteBuffer* cotpReadBuffer;
    ByteBuffer* cotpWriteBuffer;
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
setIntState(IsoClientConnection self, eIsoClientInternalState newState)
{
    self->intState = newState;
}

static eIsoClientInternalState
getIntState(IsoClientConnection self)
{
    return self->intState;
}

IsoClientConnection
IsoClientConnection_create(IsoConnectionParameters parameters, IsoIndicationCallback callback, void* callbackParameter)
{
    IsoClientConnection self = (IsoClientConnection) GLOBAL_CALLOC(1, sizeof(struct sIsoClientConnection));

    if (self) {
        self->parameters = parameters;
        self->callback = callback;
        self->callbackParameter = callbackParameter;

        self->intState = INT_STATE_IDLE;
        self->state = STATE_IDLE;
        self->stateMutex = Semaphore_create(1);

        self->sendBuffer = (uint8_t*) GLOBAL_MALLOC(ISO_CLIENT_BUFFER_SIZE);

        self->transmitPayloadBuffer = (ByteBuffer*) GLOBAL_CALLOC(1, sizeof(ByteBuffer));
        self->transmitPayloadBuffer->buffer = self->sendBuffer;
        self->transmitPayloadBuffer->maxSize = ISO_CLIENT_BUFFER_SIZE;

        self->receivePayloadBuffer = (ByteBuffer*) GLOBAL_CALLOC(1, sizeof(ByteBuffer));

        self->transmitBufferMutex = Semaphore_create(1);

        self->tickMutex = Semaphore_create(1);

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
    }

    return self;
}

static bool
sendConnectionRequestMessage(IsoClientConnection self)
{
    if (self->cotpConnection) {
        /* Destroy existing handle set when connection is reused */
        if (self->cotpConnection->handleSet)
            Handleset_destroy(self->cotpConnection->handleSet);
        self->cotpConnection->handleSet = NULL;
    }

    /* COTP (ISO transport) handshake */
    CotpConnection_init(self->cotpConnection, self->socket, self->receiveBuffer, self->cotpReadBuffer, self->cotpWriteBuffer);

#if (CONFIG_MMS_SUPPORT_TLS == 1)
    if (self->parameters->tlsConfiguration) {

        TLSConfiguration_setClientMode(self->parameters->tlsConfiguration);

        /* create TLSSocket and start TLS authentication */
        TLSSocket tlsSocket = TLSSocket_create(self->socket, self->parameters->tlsConfiguration, false);

        if (tlsSocket)
            self->cotpConnection->tlsSocket = tlsSocket;
        else {

            if (DEBUG_ISO_CLIENT)
                printf("ISO_CLIENT: TLS handshake failed!\n");

            return false;
        }
    }
#endif /* (CONFIG_MMS_SUPPORT_TLS == 1) */

    /* COTP (ISO transport) handshake */
    CotpIndication cotpIndication =
            CotpConnection_sendConnectionRequestMessage(self->cotpConnection, self->parameters);

    if (cotpIndication != COTP_OK)
        return false;
    else
        return true;
}

static void
sendAcseInitiateRequest(IsoClientConnection self)
{
    /* Upper layers handshake */
    struct sBufferChain sAcsePayload;
    BufferChain acsePayload = &sAcsePayload;
    acsePayload->buffer = self->transmitPayloadBuffer->buffer;
    acsePayload->partLength = self->transmitPayloadBuffer->size;
    acsePayload->length = self->transmitPayloadBuffer->size;
    acsePayload->nextPart = NULL;

    AcseConnection_init(&(self->acseConnection), NULL, NULL, NULL);

    AcseAuthenticationParameter authParameter = self->parameters->acseAuthParameter;

    struct sBufferChain sAcseBuffer;
    BufferChain acseBuffer = &sAcseBuffer;

    acseBuffer->buffer = self->sendBuffer + self->transmitPayloadBuffer->size;
    acseBuffer->partMaxLength = ISO_CLIENT_BUFFER_SIZE - acsePayload->length;

    AcseConnection_createAssociateRequestMessage(&(self->acseConnection), self->parameters, acseBuffer, acsePayload,
            authParameter);

    struct sBufferChain sPresentationBuffer;
    BufferChain presentationBuffer = &sPresentationBuffer;

    presentationBuffer->buffer = self->sendBuffer + acseBuffer->length;
    presentationBuffer->partMaxLength = ISO_CLIENT_BUFFER_SIZE - acseBuffer->length;

    IsoPresentation_init(self->presentation);
    IsoPresentation_createConnectPdu(self->presentation, self->parameters, presentationBuffer, acseBuffer);

    struct sBufferChain sSessionBuffer;
    BufferChain sessionBuffer = &sSessionBuffer;
    sessionBuffer->buffer = self->sendBuffer + presentationBuffer->length;

    IsoSession_init(self->session);
    IsoSession_createConnectSpdu(self->session, self->parameters, sessionBuffer,
            presentationBuffer);

    CotpConnection_sendDataMessage(self->cotpConnection, sessionBuffer);

    Semaphore_post(self->transmitBufferMutex);
}


static void
releaseSocket(IsoClientConnection self)
{
    if (self->socket) {

#if (CONFIG_MMS_SUPPORT_TLS == 1)
        if (self->cotpConnection->tlsSocket) {
            TLSSocket_close(self->cotpConnection->tlsSocket);
            self->cotpConnection->tlsSocket = NULL;
        }
#endif

        Socket_destroy(self->socket);
        self->socket = NULL;
    }
}

/*
 * Connection state machine
 *
 * called by tick function
 *
 * \return value indicates that connection is currently waiting and calling thread can be suspended
 */
bool
IsoClientConnection_handleConnection(IsoClientConnection self)
{
    Semaphore_wait(self->tickMutex);

    bool waits = false;

    eIsoClientInternalState currentState = getIntState(self);

    eIsoClientInternalState nextState = currentState;

    switch (currentState) {

    case INT_STATE_IDLE:
    case INT_STATE_ERROR:
        waits = true;
        break;

    case INT_STATE_TCP_CONNECTING:
        {
            SocketState socketState = Socket_checkAsyncConnectState(self->socket);

            if (socketState == SOCKET_STATE_CONNECTED) {
                if (sendConnectionRequestMessage(self)) {
                    self->nextReadTimeout = Hal_getTimeInMs() + self->readTimeoutInMs;
                    nextState = INT_STATE_WAIT_FOR_COTP_CONNECT_RESP;
                }
                else {
                    IsoClientConnection_releaseTransmitBuffer(self);
                    self->callback(ISO_IND_ASSOCIATION_FAILED, self->callbackParameter, NULL);
                    nextState = INT_STATE_CLOSE_ON_ERROR;
                }
            }
            else if (socketState == SOCKET_STATE_FAILED) {
                IsoClientConnection_releaseTransmitBuffer(self);
                self->callback(ISO_IND_ASSOCIATION_FAILED, self->callbackParameter, NULL);
                nextState = INT_STATE_CLOSE_ON_ERROR;
            }
            else {

                /* check connect timeout */

                uint64_t currentTime = Hal_getTimeInMs();

                if (currentTime > self->nextReadTimeout) {
                    IsoClientConnection_releaseTransmitBuffer(self);
                    self->callback(ISO_IND_ASSOCIATION_FAILED, self->callbackParameter, NULL);
                    nextState = INT_STATE_CLOSE_ON_ERROR;
                }
                else {
                    waits = true;
                }

            }

        }
        break;

    case INT_STATE_WAIT_FOR_COTP_CONNECT_RESP:
        {
            uint64_t currentTime = Hal_getTimeInMs();

            if (currentTime > self->nextReadTimeout) {

                if (DEBUG_ISO_CLIENT)
                    printf("ISO_CLIENT: Timeout waiting for COTP CR\n");

                IsoClientConnection_releaseTransmitBuffer(self);

                self->callback(ISO_IND_ASSOCIATION_FAILED, self->callbackParameter, NULL);

                nextState = INT_STATE_CLOSE_ON_ERROR;
            }
            else {

                TpktState packetState = CotpConnection_readToTpktBuffer(self->cotpConnection);

                if (packetState == TPKT_PACKET_COMPLETE) {

                    CotpIndication cotpIndication = CotpConnection_parseIncomingMessage(self->cotpConnection);

                    if (cotpIndication != COTP_CONNECT_INDICATION) {
                        if (DEBUG_ISO_CLIENT)
                            printf("ISO_CLIENT: Unexpected COTP state (%i)\n", cotpIndication);

                        IsoClientConnection_releaseTransmitBuffer(self);

                        self->callback(ISO_IND_ASSOCIATION_FAILED, self->callbackParameter, NULL);

                        nextState = INT_STATE_CLOSE_ON_ERROR;
                    }
                    else {
                        sendAcseInitiateRequest(self);

                        self->nextReadTimeout = Hal_getTimeInMs() + self->readTimeoutInMs;

                        nextState = INT_STATE_WAIT_FOR_ACSE_RESP;
                    }
                }
                else if (packetState == TPKT_ERROR) {
                    if (DEBUG_ISO_CLIENT)
                        printf("ISO_CLIENT: Error receiving COTP message\n");

                    IsoClientConnection_releaseTransmitBuffer(self);

                    self->callback(ISO_IND_ASSOCIATION_FAILED, self->callbackParameter, NULL);

                    nextState = INT_STATE_CLOSE_ON_ERROR;
                }
                else {
                    waits = true;
                }

            }
        }
        break;

    case INT_STATE_WAIT_FOR_ACSE_RESP:
        {
            uint64_t currentTime = Hal_getTimeInMs();

            if (currentTime > self->nextReadTimeout) {

                if (DEBUG_ISO_CLIENT)
                    printf("ISO_CLIENT: Timeout waiting for ACSE initiate response\n");

                self->callback(ISO_IND_ASSOCIATION_FAILED, self->callbackParameter, NULL);

                nextState = INT_STATE_CLOSE_ON_ERROR;
            }
            else {

                TpktState packetState = CotpConnection_readToTpktBuffer(self->cotpConnection);

                if (packetState == TPKT_PACKET_COMPLETE) {

                    CotpIndication cotpIndication = CotpConnection_parseIncomingMessage(self->cotpConnection);

                    if (cotpIndication != COTP_DATA_INDICATION) {
                        if (DEBUG_ISO_CLIENT)
                            printf("ISO_CLIENT: Unexpected COTP state (%i)\n", cotpIndication);

                        self->callback(ISO_IND_ASSOCIATION_FAILED, self->callbackParameter, NULL);

                        nextState = INT_STATE_CLOSE_ON_ERROR;
                    }
                    else {

                        /* parse ACSE response */

                       IsoSessionIndication sessionIndication;

                       sessionIndication =
                               IsoSession_parseMessage(self->session, CotpConnection_getPayload(self->cotpConnection));

                       if (sessionIndication != SESSION_CONNECT) {
                           if (DEBUG_ISO_CLIENT)
                               printf("ISO_CLIENT: IsoClientConnection_associate: no session connect indication\n");

                           self->callback(ISO_IND_ASSOCIATION_FAILED, self->callbackParameter, NULL);

                           nextState = INT_STATE_CLOSE_ON_ERROR;
                       }
                       else {

                           if (IsoPresentation_parseAcceptMessage(self->presentation, IsoSession_getUserData(self->session)) == false) {

                               if (DEBUG_ISO_CLIENT)
                                   printf("ISO_CLIENT: IsoClientConnection_associate: no presentation ok indication\n");

                               self->callback(ISO_IND_ASSOCIATION_FAILED, self->callbackParameter, NULL);

                               nextState = INT_STATE_CLOSE_ON_ERROR;
                           }
                           else {

                               AcseIndication acseIndication = AcseConnection_parseMessage(&(self->acseConnection), &self->presentation->nextPayload);

                               if (acseIndication != ACSE_ASSOCIATE) {
                                   if (DEBUG_ISO_CLIENT)
                                       printf("ISO_CLIENT: IsoClientConnection_associate: no ACSE_ASSOCIATE indication\n");

                                   self->callback(ISO_IND_ASSOCIATION_FAILED, self->callbackParameter, NULL);

                                   nextState = INT_STATE_CLOSE_ON_ERROR;
                               }
                               else {

                               }

                           }

                           ByteBuffer_wrap(self->receivePayloadBuffer, self->acseConnection.userDataBuffer,
                                   self->acseConnection.userDataBufferSize, self->acseConnection.userDataBufferSize);

                           setState(self, STATE_CONNECTED);
                           nextState = INT_STATE_WAIT_FOR_DATA_MSG;

                           if (self->callback(ISO_IND_ASSOCIATION_SUCCESS, self->callbackParameter, self->receivePayloadBuffer) == false) {
                               nextState = INT_STATE_CLOSE_ON_ERROR;
                           }

                           CotpConnection_resetPayload(self->cotpConnection);
                       }
                    }
                }
                else if (packetState == TPKT_ERROR) {
                    if (DEBUG_ISO_CLIENT)
                        printf("ISO_CLIENT: Error receiving COTP message\n");

                    self->callback(ISO_IND_ASSOCIATION_FAILED, self->callbackParameter, NULL);

                    nextState = INT_STATE_CLOSE_ON_ERROR;
                }
                else {
                    waits = true;
                }

            }
        }
        break;

    case INT_STATE_WAIT_FOR_DATA_MSG:
        {
            TpktState packetState = CotpConnection_readToTpktBuffer(self->cotpConnection);

            if (packetState == TPKT_ERROR) {
                nextState = INT_STATE_CLOSE_ON_ERROR;
            }
            else if (packetState == TPKT_PACKET_COMPLETE) {

                CotpIndication cotpIndication = CotpConnection_parseIncomingMessage(self->cotpConnection);

                switch (cotpIndication) {

                case COTP_MORE_FRAGMENTS_FOLLOW:
                    break;

                case COTP_DISCONNECT_INDICATION:
                    {
                        nextState = INT_STATE_CLOSING_CONNECTION;
                    }
                    break;

                case COTP_DATA_INDICATION:
                    {
                        if (DEBUG_ISO_CLIENT)
                            printf("ISO_CLIENT_CONNECTION: parse message\n");

                        IsoSessionIndication sessionIndication =
                                IsoSession_parseMessage(self->session,
                                        CotpConnection_getPayload(self->cotpConnection));

                        if (sessionIndication != SESSION_DATA) {
                            if (DEBUG_ISO_CLIENT)
                                printf("ISO_CLIENT_CONNECTION: Invalid session message\n");

                            nextState = INT_STATE_CLOSE_ON_ERROR;
                        }
                        else {

                            if (!IsoPresentation_parseUserData(self->presentation, IsoSession_getUserData(self->session))) {

                                if (DEBUG_ISO_CLIENT)
                                    printf("ISO_CLIENT_CONNECTION: Invalid presentation message\n");

                                nextState = INT_STATE_CLOSE_ON_ERROR;
                            }
                            else {

                                self->callback(ISO_IND_DATA, self->callbackParameter,
                                        &(self->presentation->nextPayload));

                                CotpConnection_resetPayload(self->cotpConnection);
                            }
                        }
                    }
                    break;

                default:
                    {
                        nextState = INT_STATE_CLOSE_ON_ERROR;
                    }
                    break;

                }
            }
            else {
                waits = true;
            }

        }
        break;

    case INT_STATE_CLOSE_ON_ERROR:
        {
            setState(self, STATE_ERROR);

            self->callback(ISO_IND_CLOSED, self->callbackParameter, NULL);;

            releaseSocket(self);

            nextState = INT_STATE_ERROR;
        }
        break;

    case INT_STATE_CLOSING_CONNECTION:
        {
            setState(self, STATE_IDLE);

            self->callback(ISO_IND_CLOSED, self->callbackParameter, NULL);;

            releaseSocket(self);

            nextState = INT_STATE_IDLE;
        }
        break;

    default:

        if (DEBUG_ISO_CLIENT)
            printf("ISO_CLIENT_CONNECTION: Illegal state\n");

        break;

    }

    self->callback(ISO_IND_TICK, self->callbackParameter, NULL);

    setIntState(self, nextState);

    Semaphore_post(self->tickMutex);

    return waits;
}


bool
IsoClientConnection_associateAsync(IsoClientConnection self, uint32_t connectTimeoutInMs, uint32_t readTimeoutInMs)
{
    Semaphore_wait(self->tickMutex);

    /* Create socket and start connect */

    self->socket = TcpSocket_create();

    if (self->socket == NULL) {
        Semaphore_post(self->tickMutex);
        return false;
    }

    bool success = true;

    setState(self, STATE_CONNECTING);
    setIntState(self, INT_STATE_TCP_CONNECTING);

#if (CONFIG_ACTIVATE_TCP_KEEPALIVE == 1)
    Socket_activateTcpKeepAlive(self->socket,
            CONFIG_TCP_KEEPALIVE_IDLE,
            CONFIG_TCP_KEEPALIVE_INTERVAL,
            CONFIG_TCP_KEEPALIVE_CNT);
#endif

    /* set read timeout */
    self->readTimeoutInMs = readTimeoutInMs;

    /* set timeout for connect */
    self->nextReadTimeout = Hal_getTimeInMs() + connectTimeoutInMs;

    if (Socket_connectAsync(self->socket, self->parameters->hostname, self->parameters->tcpPort) == false) {

        Socket_destroy(self->socket);
        self->socket = NULL;

        setIntState(self, INT_STATE_ERROR);
        setState(self, STATE_ERROR);

        IsoClientConnection_releaseTransmitBuffer(self);

        success = false;
    }

    Semaphore_post(self->tickMutex);

    return success;
}

void
IsoClientConnection_sendMessage(IsoClientConnection self, ByteBuffer* payloadBuffer)
{
    if (getState(self) == STATE_CONNECTED) {
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

        if (indication != COTP_OK)
            if (DEBUG_ISO_CLIENT)
                printf("ISO_CLIENT: IsoClientConnection_sendMessage: send message failed!\n");
    }
    else {
        if (DEBUG_ISO_CLIENT)
            printf("ISO_CLIENT: Not connected --> cannot send message\n");
    }

    /* release transmit buffer for use by API client */
    Semaphore_post(self->transmitBufferMutex);
}

void
IsoClientConnection_close(IsoClientConnection self)
{
    if (DEBUG_ISO_CLIENT)
        printf("ISO_CLIENT: IsoClientConnection_close\n");

    Semaphore_wait(self->tickMutex);

    eIsoClientInternalState intState = getIntState(self);

    if ((intState != INT_STATE_IDLE) && (intState != INT_STATE_ERROR) && (intState != INT_STATE_CLOSE_ON_ERROR)) {
        setIntState(self, INT_STATE_CLOSING_CONNECTION);

        Semaphore_post(self->tickMutex);

        IsoClientConnection_handleConnection(self);
        setState(self, STATE_IDLE);
    }
    else {
        Semaphore_post(self->tickMutex);
    }
}


void
IsoClientConnection_destroy(IsoClientConnection self)
{
    if (DEBUG_ISO_CLIENT)
        printf("ISO_CLIENT: IsoClientConnection_destroy(%p)\n", self);

    int state = getState(self);

    if (state == STATE_CONNECTED) {

        if (DEBUG_ISO_CLIENT)
            printf("ISO_CLIENT: call IsoClientConnection_close\n");

        IsoClientConnection_close(self);
    }

    releaseSocket(self);

    if (self->receiveBuf != NULL)
        GLOBAL_FREEMEM(self->receiveBuf);
    if (self->receiveBuffer != NULL)
        GLOBAL_FREEMEM(self->receiveBuffer);
    if (self->cotpConnection != NULL) {
        if (self->cotpConnection->handleSet != NULL)
            Handleset_destroy(self->cotpConnection->handleSet);
        GLOBAL_FREEMEM(self->cotpConnection);
    }

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

    Semaphore_destroy(self->transmitBufferMutex);
    Semaphore_destroy(self->stateMutex);
    Semaphore_destroy(self->tickMutex);

    GLOBAL_FREEMEM(self->sendBuffer);
    GLOBAL_FREEMEM(self);
}

static void
sendAbortMessage(IsoClientConnection self)
{
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

    CotpConnection_sendDataMessage(self->cotpConnection, sessionBuffer);

    Semaphore_post(self->transmitBufferMutex);
}

void
IsoClientConnection_abortAsync(IsoClientConnection self)
{
    sendAbortMessage(self);
}

void
IsoClientConnection_release(IsoClientConnection self)
{
    /* TODO block other messages from being sent */
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
IsoClientConnection_releaseTransmitBuffer(IsoClientConnection self)
{
    Semaphore_post(self->transmitBufferMutex);
}
