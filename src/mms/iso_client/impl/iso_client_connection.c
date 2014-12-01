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

#include "socket.h"
#include "thread.h"
#include "cotp.h"
#include "iso_session.h"
#include "iso_presentation.h"
#include "iso_client_connection.h"
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

#define ISO_CLIENT_BUFFER_SIZE CONFIG_MMS_MAXIMUM_PDU_SIZE + 100

struct sIsoClientConnection
{
    IsoIndicationCallback callback;
    void* callbackParameter;
    int state;
    Socket socket;
    CotpConnection* cotpConnection;
    IsoPresentation* presentation;
    IsoSession* session;
    AcseConnection acseConnection;

    uint8_t* sendBuffer; /* send buffer */
    uint8_t* cotpBuf; /* receive buffer */
    ByteBuffer* cotpBuffer;

    ByteBuffer* transmitPayloadBuffer;
    Semaphore transmitBufferMutex;

    ByteBuffer* receivePayloadBuffer;
    Semaphore receiveBufferMutex;

    Thread thread;
};

static void*
connectionHandlingThread(void* threadParameter)
{
    IsoClientConnection self = (IsoClientConnection) threadParameter;

    IsoSessionIndication sessionIndication;

    if (DEBUG_ISO_CLIENT)
        printf("ISO_CLIENT_CONNECTION: new connection\n");

    Semaphore_wait(self->receiveBufferMutex);

    while (CotpConnection_parseIncomingMessage(self->cotpConnection) == DATA_INDICATION) {

        sessionIndication =
                IsoSession_parseMessage(self->session,
                        CotpConnection_getPayload(self->cotpConnection));

        if (DEBUG_ISO_CLIENT)
            printf("ISO_CLIENT_CONNECTION: parse message\n");

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
    }

    self->callback(ISO_IND_CLOSED, self->callbackParameter, NULL);

    if (DEBUG_ISO_CLIENT)
        printf("ISO_CLIENT_CONNECTION: exit connection\n");

    return NULL;
}

IsoClientConnection
IsoClientConnection_create(IsoIndicationCallback callback, void* callbackParameter)
{
    IsoClientConnection self = (IsoClientConnection) calloc(1, sizeof(struct sIsoClientConnection));

    self->callback = callback;
    self->callbackParameter = callbackParameter;
    self->state = STATE_IDLE;

    self->sendBuffer = (uint8_t*) malloc(ISO_CLIENT_BUFFER_SIZE);

    self->transmitPayloadBuffer = (ByteBuffer*) calloc(1, sizeof(ByteBuffer));
    self->transmitPayloadBuffer->buffer = self->sendBuffer;
    self->transmitPayloadBuffer->maxSize = ISO_CLIENT_BUFFER_SIZE;

    self->receivePayloadBuffer = (ByteBuffer*) calloc(1, sizeof(ByteBuffer));

    self->transmitBufferMutex = Semaphore_create(1);

    self->receiveBufferMutex = Semaphore_create(1);

    return self;
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

    if (indication != OK)
        if (DEBUG_ISO_CLIENT)
            printf("ISO_CLIENT: IsoClientConnection_sendMessage: send message failed!\n");
}

void
IsoClientConnection_close(IsoClientConnection self)
{
    if (DEBUG_ISO_CLIENT)
        printf("ISO_CLIENT: IsoClientConnection_close\n");

    if (self->socket != NULL) {
        if (DEBUG_ISO_CLIENT)
            printf("ISO_CLIENT: call socket close\n");

        Socket_destroy(self->socket);
    }

    self->state = STATE_IDLE;
}

void
IsoClientConnection_destroy(IsoClientConnection self)
{
    if (DEBUG_ISO_CLIENT)
        printf("ISO_CLIENT: IsoClientConnection_destroy\n");

    if (self->state == STATE_ASSOCIATED) {

        if (DEBUG_ISO_CLIENT)
            printf("ISO_CLIENT: call IsoClientConnection_close\n");

        IsoClientConnection_close(self);
    }

    if (self->thread != NULL)
        Thread_destroy(self->thread);

    if (self->cotpBuf != NULL)
        free(self->cotpBuf);
    if (self->cotpBuffer != NULL)
        free(self->cotpBuffer);
    if (self->cotpConnection != NULL) {
        CotpConnection_destroy(self->cotpConnection);
        free(self->cotpConnection);
    }
    if (self->session != NULL)
        free(self->session);
    if (self->presentation != NULL)
        free(self->presentation);

    free(self->transmitPayloadBuffer);
    free(self->receivePayloadBuffer);

    Semaphore_destroy(self->receiveBufferMutex);
    Semaphore_destroy(self->transmitBufferMutex);

    free(self->sendBuffer);
    free(self);
}

void
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

    CotpConnection_sendDataMessage(self->cotpConnection, sessionBuffer);

    Semaphore_post(self->transmitBufferMutex);
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

    CotpConnection_sendDataMessage(self->cotpConnection, sessionBuffer);

    Semaphore_post(self->transmitBufferMutex);
}

void
IsoClientConnection_associate(IsoClientConnection self, IsoConnectionParameters params,
        ByteBuffer* payload)
{
    Socket socket = TcpSocket_create();

    self->socket = socket;

    if (!Socket_connect(socket, params->hostname, params->tcpPort))
        goto returnError;

    self->cotpBuf = (uint8_t*) malloc(ISO_CLIENT_BUFFER_SIZE);
    self->cotpBuffer = (ByteBuffer*) calloc(1, sizeof(ByteBuffer));
    ByteBuffer_wrap(self->cotpBuffer, self->cotpBuf, 0, ISO_CLIENT_BUFFER_SIZE);

    self->cotpConnection = (CotpConnection*) calloc(1, sizeof(CotpConnection));
    CotpConnection_init(self->cotpConnection, socket, self->cotpBuffer);

    /* COTP (ISO transport) handshake */
    CotpIndication cotpIndication =
            CotpConnection_sendConnectionRequestMessage(self->cotpConnection, params);

    cotpIndication = CotpConnection_parseIncomingMessage(self->cotpConnection);

    if (cotpIndication != CONNECT_INDICATION)
        goto returnError;

    /* Upper layers handshake */
    struct sBufferChain sAcsePayload;
    BufferChain acsePayload = &sAcsePayload;
    acsePayload->buffer = payload->buffer;
    acsePayload->partLength = payload->size;
    acsePayload->length = payload->size;
    acsePayload->nextPart = NULL;

    AcseConnection_init(&(self->acseConnection), NULL, NULL);

    AcseAuthenticationParameter authParameter = NULL;

    if (params != NULL)
        authParameter = params->acseAuthParameter;

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

    self->presentation = (IsoPresentation*) calloc(1, sizeof(IsoPresentation));
    IsoPresentation_init(self->presentation);

    IsoPresentation_createConnectPdu(self->presentation, params, presentationBuffer, acseBuffer);

    struct sBufferChain sSessionBuffer;
    BufferChain sessionBuffer = &sSessionBuffer;
    sessionBuffer->buffer = self->sendBuffer + presentationBuffer->length;

    self->session = (IsoSession*) calloc(1, sizeof(IsoSession));
    IsoSession_init(self->session);

    IsoSession_createConnectSpdu(self->session, params, sessionBuffer,
            presentationBuffer);

    CotpConnection_sendDataMessage(self->cotpConnection, sessionBuffer);

    Semaphore_post(self->transmitBufferMutex);

    cotpIndication = CotpConnection_parseIncomingMessage(self->cotpConnection);

    if (cotpIndication != DATA_INDICATION)
        goto returnError;

    IsoSessionIndication sessionIndication;

    sessionIndication =
            IsoSession_parseMessage(self->session, CotpConnection_getPayload(self->cotpConnection));

    if (sessionIndication != SESSION_CONNECT) {
        if (DEBUG_ISO_CLIENT)
            printf("IsoClientConnection_associate: no session connect indication\n");
        goto returnError;
    }

    if (!IsoPresentation_parseAcceptMessage(self->presentation, IsoSession_getUserData(self->session))) {
        if (DEBUG_ISO_CLIENT)
            printf("IsoClientConnection_associate: no presentation ok indication\n");
        goto returnError;
    }

    AcseIndication acseIndication;

    acseIndication = AcseConnection_parseMessage(&(self->acseConnection), &self->presentation->nextPayload);

    if (acseIndication != ACSE_ASSOCIATE) {
        if (DEBUG_ISO_CLIENT)
            printf("IsoClientConnection_associate: no ACSE_ASSOCIATE indication\n");
        goto returnError;
    }


    ByteBuffer_wrap(self->receivePayloadBuffer, self->acseConnection.userDataBuffer,
            self->acseConnection.userDataBufferSize, self->acseConnection.userDataBufferSize);

    Semaphore_wait(self->receiveBufferMutex);

    self->callback(ISO_IND_ASSOCIATION_SUCCESS, self->callbackParameter, self->receivePayloadBuffer);

    self->state = STATE_ASSOCIATED;

    self->thread = Thread_create(connectionHandlingThread, self, false);
    Thread_start(self->thread);

    return;

    returnError:
    self->callback(ISO_IND_ASSOCIATION_FAILED, self->callbackParameter, NULL);

    self->state = STATE_ERROR;

    Socket_destroy(self->socket);
    self->socket = NULL;

    return;
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
