/*
 *  iso_connection.c
 *
 *  Copyright 2013, 2014 Michael Zillgith
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
#include "byte_stream.h"
#include "buffer_chain.h"
#include "cotp.h"
#include "iso_session.h"
#include "iso_presentation.h"
#include "acse.h"
#include "iso_server.h"
#include "socket.h"
#include "thread.h"

#include "iso_server_private.h"

#ifndef DEBUG_ISO_SERVER
#ifdef DEBUG
#define DEBUG_ISO_SERVER 1
#else
#define DEBUG_ISO_SERVER 0
#endif /*DEBUG */
#endif /* DEBUG_ISO_SERVER */

#define RECEIVE_BUF_SIZE CONFIG_MMS_MAXIMUM_PDU_SIZE + 100
#define SEND_BUF_SIZE CONFIG_MMS_MAXIMUM_PDU_SIZE + 100

#define ISO_CON_STATE_RUNNING 1
#define ISO_CON_STATE_STOPPED 0

struct sIsoConnection
{
    uint8_t* receiveBuffer;
    uint8_t* sendBuffer;
    MessageReceivedHandler msgRcvdHandler;
    IsoServer isoServer;
    void* msgRcvdHandlerParameter;
    Socket socket;
    int state;
    IsoSession* session;
    IsoPresentation* presentation;
    CotpConnection* cotpConnection;
    char* clientAddress;
    Thread thread;
    Semaphore conMutex;


    void* securityToken;
};

static void
handleTcpConnection(IsoConnection self)
{
    if (DEBUG_ISO_SERVER)
        printf("ISO_SERVER: connection %p started\n", self);

    CotpIndication cotpIndication;

    IsoSessionIndication sIndication;

    AcseIndication aIndication;
    AcseConnection acseConnection;

    ByteBuffer receiveBuffer;

    self->cotpConnection = (CotpConnection*) calloc(1, sizeof(CotpConnection));
    CotpConnection_init(self->cotpConnection, self->socket, &receiveBuffer);

    self->session = (IsoSession*) calloc(1, sizeof(IsoSession));
    IsoSession_init(self->session);

    self->presentation = (IsoPresentation*) calloc(1, sizeof(IsoPresentation));
    IsoPresentation_init(self->presentation);

    AcseConnection_init(&acseConnection, IsoServer_getAuthenticator(self->isoServer),
            IsoServer_getAuthenticatorParameter(self->isoServer));

    while (self->msgRcvdHandlerParameter == NULL)
        Thread_sleep(1);

    if (DEBUG_ISO_SERVER)
        printf("ISO_SERVER: IsoConnection: Start to handle connection for client %s\n", self->clientAddress);

    while (self->state == ISO_CON_STATE_RUNNING) {
        ByteBuffer_wrap(&receiveBuffer, self->receiveBuffer, 0, RECEIVE_BUF_SIZE);

        cotpIndication = CotpConnection_parseIncomingMessage(self->cotpConnection);

        switch (cotpIndication) {
        case CONNECT_INDICATION:
            if (DEBUG_ISO_SERVER)
                printf("ISO_SERVER: COTP connection indication\n");

            Semaphore_wait(self->conMutex);

            CotpConnection_sendConnectionResponseMessage(self->cotpConnection);

            Semaphore_post(self->conMutex);

            break;
        case DATA_INDICATION:
            {
                if (DEBUG_ISO_SERVER)
                    printf("ISO_SERVER: COTP data indication\n");

                ByteBuffer* cotpPayload = CotpConnection_getPayload(self->cotpConnection);

                sIndication = IsoSession_parseMessage(self->session, cotpPayload);

                ByteBuffer* sessionUserData = IsoSession_getUserData(self->session);

                switch (sIndication) {
                case SESSION_CONNECT:
                    if (DEBUG_ISO_SERVER)
                        printf("ISO_SERVER: iso_connection: session connect indication\n");

                    if (IsoPresentation_parseConnect(self->presentation, sessionUserData)) {
                        if (DEBUG_ISO_SERVER)
                            printf("ISO_SERVER: iso_connection: presentation ok\n");

                        ByteBuffer* acseBuffer = &(self->presentation->nextPayload);

                        aIndication = AcseConnection_parseMessage(&acseConnection, acseBuffer);

                        self->securityToken = acseConnection.securityToken;

                        if (aIndication == ACSE_ASSOCIATE) {

                            Semaphore_wait(self->conMutex);

                            if (DEBUG_ISO_SERVER)
                                printf("ISO_SERVER: cotp_server: acse associate\n");

                            ByteBuffer mmsRequest;

                            ByteBuffer_wrap(&mmsRequest, acseConnection.userDataBuffer,
                                    acseConnection.userDataBufferSize, acseConnection.userDataBufferSize);
                            ByteBuffer mmsResponseBuffer; /* new */

                            ByteBuffer_wrap(&mmsResponseBuffer, self->sendBuffer, 0, SEND_BUF_SIZE);

                            self->msgRcvdHandler(self->msgRcvdHandlerParameter,
                                    &mmsRequest, &mmsResponseBuffer);

                            struct sBufferChain mmsBufferPartStruct;
                            BufferChain mmsBufferPart = &mmsBufferPartStruct;

                            BufferChain_init(mmsBufferPart, mmsResponseBuffer.size, mmsResponseBuffer.size, NULL,
                                    self->sendBuffer);

                            if (mmsResponseBuffer.size > 0) {
                                if (DEBUG_ISO_SERVER)
                                    printf("iso_connection: application payload size: %i\n",
                                            mmsResponseBuffer.size);

                                struct sBufferChain acseBufferPartStruct;
                                BufferChain acseBufferPart = &acseBufferPartStruct;

                                acseBufferPart->buffer = self->sendBuffer + mmsBufferPart->length;
                                acseBufferPart->partMaxLength = SEND_BUF_SIZE - mmsBufferPart->length;

                                AcseConnection_createAssociateResponseMessage(&acseConnection,
                                ACSE_RESULT_ACCEPT, acseBufferPart, mmsBufferPart);

                                struct sBufferChain presentationBufferPartStruct;
                                BufferChain presentationBufferPart = &presentationBufferPartStruct;

                                presentationBufferPart->buffer = self->sendBuffer + acseBufferPart->length;
                                presentationBufferPart->partMaxLength = SEND_BUF_SIZE - acseBufferPart->length;

                                IsoPresentation_createCpaMessage(self->presentation, presentationBufferPart,
                                        acseBufferPart);

                                struct sBufferChain sessionBufferPartStruct;
                                BufferChain sessionBufferPart = &sessionBufferPartStruct;
                                sessionBufferPart->buffer = self->sendBuffer + presentationBufferPart->length;
                                sessionBufferPart->partMaxLength = SEND_BUF_SIZE - presentationBufferPart->length;

                                IsoSession_createAcceptSpdu(self->session, sessionBufferPart, presentationBufferPart);

                                CotpConnection_sendDataMessage(self->cotpConnection, sessionBufferPart);
                            }
                            else {
                                if (DEBUG_ISO_SERVER)
                                    printf(
                                            "ISO_SERVER: iso_connection: association error. No response from application!\n");
                            }

                            Semaphore_post(self->conMutex);
                        }
                        else {
                            if (DEBUG_ISO_SERVER)
                                printf("ISO_SERVER: iso_connection: acse association failed\n");
                            self->state = ISO_CON_STATE_STOPPED;
                        }

                    }
                    break;
                case SESSION_DATA:
                    if (DEBUG_ISO_SERVER)
                        printf("ISO_SERVER: iso_connection: session data indication\n");

                    if (!IsoPresentation_parseUserData(self->presentation, sessionUserData)) {
                        if (DEBUG_ISO_SERVER)
                            printf("ISO_SERVER: cotp_server: presentation error\n");
                        self->state = ISO_CON_STATE_STOPPED;
                        break;
                    }

                    if (self->presentation->nextContextId == self->presentation->mmsContextId) {
                        if (DEBUG_ISO_SERVER)
                            printf("ISO_SERVER: iso_connection: mms message\n");

                        ByteBuffer* mmsRequest = &(self->presentation->nextPayload);

                        ByteBuffer mmsResponseBuffer;

                        IsoServer_userLock(self->isoServer);
                        Semaphore_wait(self->conMutex);

                        ByteBuffer_wrap(&mmsResponseBuffer, self->sendBuffer, 0, SEND_BUF_SIZE);

                        self->msgRcvdHandler(self->msgRcvdHandlerParameter,
                                mmsRequest, &mmsResponseBuffer);

                        if (mmsResponseBuffer.size > 0) {


                            struct sBufferChain mmsBufferPartStruct;
                            BufferChain mmsBufferPart = &mmsBufferPartStruct;

                            BufferChain_init(mmsBufferPart, mmsResponseBuffer.size,
                                    mmsResponseBuffer.size, NULL, self->sendBuffer);

                            struct sBufferChain presentationBufferPartStruct;
                            BufferChain presentationBufferPart = &presentationBufferPartStruct;
                            presentationBufferPart->buffer = self->sendBuffer + mmsBufferPart->length;
                            presentationBufferPart->partMaxLength = SEND_BUF_SIZE - mmsBufferPart->length;

                            IsoPresentation_createUserData(self->presentation,
                                    presentationBufferPart, mmsBufferPart);

                            struct sBufferChain sessionBufferPartStruct;
                            BufferChain sessionBufferPart = &sessionBufferPartStruct;
                            sessionBufferPart->buffer = self->sendBuffer + presentationBufferPart->length;
                            sessionBufferPart->partMaxLength = SEND_BUF_SIZE - presentationBufferPart->length;

                            IsoSession_createDataSpdu(self->session, sessionBufferPart, presentationBufferPart);

                            CotpConnection_sendDataMessage(self->cotpConnection, sessionBufferPart);
                        }

                        Semaphore_post(self->conMutex);
                        IsoServer_userUnlock(self->isoServer);
                    }
                    else {
                        if (DEBUG_ISO_SERVER)
                            printf("ISO_SERVER: iso_connection: unknown presentation layer context!");
                    }

                    break;

                case SESSION_FINISH:
                    if (DEBUG_ISO_SERVER)
                        printf("ISO_SERVER: iso_connection: session finish indication\n");

                    if (IsoPresentation_parseUserData(self->presentation, sessionUserData)) {
                        if (DEBUG_ISO_SERVER)
                            printf("ISO_SERVER: iso_connection: presentation ok\n");

                        struct sBufferChain acseBufferPartStruct;
                        BufferChain acseBufferPart = &acseBufferPartStruct;
                        acseBufferPart->buffer = self->sendBuffer;
                        acseBufferPart->partMaxLength = SEND_BUF_SIZE;

                        AcseConnection_createReleaseResponseMessage(&acseConnection, acseBufferPart);

                        struct sBufferChain presentationBufferPartStruct;
                        BufferChain presentationBufferPart = &presentationBufferPartStruct;
                        presentationBufferPart->buffer = self->sendBuffer + acseBufferPart->length;
                        presentationBufferPart->partMaxLength = SEND_BUF_SIZE - acseBufferPart->length;

                        IsoPresentation_createUserDataACSE(self->presentation, presentationBufferPart, acseBufferPart);

                        struct sBufferChain sessionBufferPartStruct;
                        BufferChain sessionBufferPart = &sessionBufferPartStruct;
                        sessionBufferPart->buffer = self->sendBuffer + presentationBufferPart->length;
                        sessionBufferPart->partMaxLength = SEND_BUF_SIZE - presentationBufferPart->length;

                        IsoSession_createDisconnectSpdu(self->session, sessionBufferPart, presentationBufferPart);

                        CotpConnection_sendDataMessage(self->cotpConnection, sessionBufferPart);
                    }

                    self->state = ISO_CON_STATE_STOPPED;

                    break;

                case SESSION_ABORT:
                    self->state = ISO_CON_STATE_STOPPED;
                    break;

                case SESSION_ERROR:
                    self->state = ISO_CON_STATE_STOPPED;
                    break;

                default: /* illegal state */
                    self->state = ISO_CON_STATE_STOPPED;
                    break;
                }
            }
            break;
        case ERROR:
            if (DEBUG_ISO_SERVER)
                printf("ISO_SERVER: Connection closed\n");
            self->state = ISO_CON_STATE_STOPPED;
            break;
        default:
            if (DEBUG_ISO_SERVER)
                printf("ISO_SERVER: COTP Unknown Indication: %i\n", cotpIndication);
            self->state = ISO_CON_STATE_STOPPED;
            break;
        }
    }

    if (DEBUG_ISO_SERVER)
        printf("ISO_SERVER: Connection handling loop finished --> close transport connection\n");

    IsoServer_closeConnection(self->isoServer, self);

    if (self->socket != NULL)
        Socket_destroy(self->socket);

    free(self->session);
    free(self->presentation);

    AcseConnection_destroy(&acseConnection);

    CotpConnection_destroy(self->cotpConnection);
    free(self->cotpConnection);

    Semaphore_destroy(self->conMutex);

    free(self->receiveBuffer);
    free(self->sendBuffer);
    free(self->clientAddress);

    IsoServer isoServer = self->isoServer;

    free(self);

    if (DEBUG_ISO_SERVER)
        printf("ISO_SERVER: connection %p closed\n", self);

    private_IsoServer_decreaseConnectionCounter(isoServer);
}

IsoConnection
IsoConnection_create(Socket socket, IsoServer isoServer)
{
    IsoConnection self = (IsoConnection) calloc(1, sizeof(struct sIsoConnection));
    self->socket = socket;
    self->receiveBuffer = (uint8_t*) malloc(RECEIVE_BUF_SIZE);
    self->sendBuffer = (uint8_t*) malloc(SEND_BUF_SIZE);
    self->msgRcvdHandler = NULL;
    self->msgRcvdHandlerParameter = NULL;
    self->isoServer = isoServer;
    self->state = ISO_CON_STATE_RUNNING;
    self->clientAddress = Socket_getPeerAddress(self->socket);

    self->thread = Thread_create((ThreadExecutionFunction) handleTcpConnection, self, true);
    self->conMutex = Semaphore_create(1);

    Thread_start(self->thread);

    if (DEBUG_ISO_SERVER)
        printf("ISO_SERVER: new iso connection thread started\n");

    return self;
}

char*
IsoConnection_getPeerAddress(IsoConnection self)
{
    return self->clientAddress;
}

void
IsoConnection_sendMessage(IsoConnection self, ByteBuffer* message, bool handlerMode)
{
    if (self->state == ISO_CON_STATE_STOPPED) {
        if (DEBUG_ISO_SERVER)
            printf("DEBUG_ISO_SERVER: sendMessage: connection already stopped!\n");
        return;
    }

    if (!handlerMode)
        Semaphore_wait(self->conMutex);

    struct sBufferChain payloadBufferStruct;
    BufferChain payloadBuffer = &payloadBufferStruct;
    payloadBuffer->length = message->size;
    payloadBuffer->partLength = message->size;
    payloadBuffer->partMaxLength = message->size;
    payloadBuffer->buffer = message->buffer;
    payloadBuffer->nextPart = NULL;

    struct sBufferChain presentationBufferStruct;
    BufferChain presentationBuffer = &presentationBufferStruct;
    presentationBuffer->buffer = self->sendBuffer;
    presentationBuffer->partMaxLength = SEND_BUF_SIZE;

    IsoPresentation_createUserData(self->presentation,
            presentationBuffer, payloadBuffer);

    struct sBufferChain sessionBufferStruct;
    BufferChain sessionBuffer = &sessionBufferStruct;
    sessionBuffer->buffer = self->sendBuffer + presentationBuffer->partLength;

    IsoSession_createDataSpdu(self->session, sessionBuffer, presentationBuffer);

    CotpIndication indication;

    indication = CotpConnection_sendDataMessage(self->cotpConnection, sessionBuffer);

    if (DEBUG_ISO_SERVER) {
        if (indication != OK)
            printf("ISO_SERVER: IsoConnection_sendMessage failed!\n");
        else
            printf("ISO_SERVER: IsoConnection_sendMessage success!\n");
    }

    if (!handlerMode)
        Semaphore_post(self->conMutex);
}

void
IsoConnection_close(IsoConnection self)
{
    if (self->state != ISO_CON_STATE_STOPPED) {
        Socket socket = self->socket;
        self->state = ISO_CON_STATE_STOPPED;
        self->socket = NULL;

        Socket_destroy(socket);
    }
}

void
IsoConnection_installListener(IsoConnection self, MessageReceivedHandler handler,
        void* parameter)
{
    self->msgRcvdHandler = handler;
    self->msgRcvdHandlerParameter = parameter;
}

void*
IsoConnection_getSecurityToken(IsoConnection self)
{
    return self->securityToken;
}

