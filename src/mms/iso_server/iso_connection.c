/*
 *  iso_connection.c
 *
 *  Copyright 2013-2020 Michael Zillgith
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
#include "buffer_chain.h"
#include "cotp.h"
#include "iso_session.h"
#include "iso_presentation.h"
#include "acse.h"
#include "iso_server.h"
#include "hal_socket.h"
#include "hal_thread.h"

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

#define TPKT_RFC1006_HEADER_SIZE 4

struct sIsoConnection
{
    uint8_t* receiveBuffer;
    ByteBuffer rcvBuffer;

    uint8_t* sendBuffer;

    uint8_t* cotpReadBuf;
    uint8_t* cotpWriteBuf;
    ByteBuffer cotpReadBuffer;
    ByteBuffer cotpWriteBuffer;

    MessageReceivedHandler msgRcvdHandler;
    UserLayerTickHandler tickHandler;
    void* handlerParameter; /* context parameter for msgRcvdHandler */

    IsoServer isoServer;

    Socket socket;

#if (CONFIG_MMS_SUPPORT_TLS == 1)
    TLSSocket tlsSocket;
#endif

    int state;
    IsoSession* session;
    IsoPresentation* presentation;
    CotpConnection* cotpConnection;

    AcseConnection* acseConnection;

    char* clientAddress;
    char* localAddress;

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Thread thread;
    Semaphore conMutex;
#endif

#if (CONFIG_MMS_SINGLE_THREADED != 1) || (CONFIG_MMS_THREADLESS_STACK == 1)
    HandleSet handleSet;
#endif
};

static void
finalizeIsoConnection(IsoConnection self)
{
    if (DEBUG_ISO_SERVER)
        printf("ISO_SERVER: finalizeIsoConnection (%p)--> close transport connection\n", self);

#if (CONFIG_MMS_SUPPORT_TLS == 1)
    if (self->tlsSocket)
        TLSSocket_close(self->tlsSocket);
#endif

#if (CONFIG_MMS_THREADLESS_STACK != 1)
#if (CONFIG_MMS_SINGLE_THREADED != 1)
    if (self->handleSet) {
        Handleset_destroy(self->handleSet);
        self->handleSet = NULL;
    }
#endif
#endif

    GLOBAL_FREEMEM(self->session);
    GLOBAL_FREEMEM(self->presentation);
    GLOBAL_FREEMEM(self->acseConnection);

    GLOBAL_FREEMEM(self->cotpReadBuf);
    GLOBAL_FREEMEM(self->cotpWriteBuf);

    if (self->cotpConnection) {
        if (self->cotpConnection->handleSet)
            Handleset_destroy(self->cotpConnection->handleSet);
    }

    GLOBAL_FREEMEM(self->cotpConnection);
    self->cotpConnection = NULL;

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_destroy(self->conMutex);
#endif

    GLOBAL_FREEMEM(self->receiveBuffer);
    GLOBAL_FREEMEM(self->sendBuffer);
    GLOBAL_FREEMEM(self->clientAddress);
    GLOBAL_FREEMEM(self->localAddress);
    IsoServer isoServer = self->isoServer;

    if (DEBUG_ISO_SERVER)
        printf("ISO_SERVER: connection %p closed\n", self);

    private_IsoServer_decreaseConnectionCounter(isoServer);
}

void
IsoConnection_addToHandleSet(const IsoConnection self, HandleSet handles)
{
   Handleset_addSocket(handles, self->socket);
}

void
IsoConnection_removeFromHandleSet(const IsoConnection self, HandleSet handles)
{
    Handleset_removeSocket(handles, self->socket);
}

void
IsoConnection_callTickHandler(IsoConnection self)
{
    if (self->tickHandler) {
        self->tickHandler(self->handlerParameter);
    }
}

void
IsoConnection_handleTcpConnection(IsoConnection self, bool isSingleThread)
{
#if (CONFIG_MMS_SINGLE_THREADED != 1)
    if (isSingleThread == false) {

        /* call tick handler */
        if (self->tickHandler) {
            self->tickHandler(self->handlerParameter);
        }

        if (Handleset_waitReady(self->handleSet, 10) < 1)
            goto exit_function;
    }
#endif

    TpktState tpktState = CotpConnection_readToTpktBuffer(self->cotpConnection);

    if (tpktState == TPKT_ERROR)
        self->state = ISO_CON_STATE_STOPPED;

    if (tpktState != TPKT_PACKET_COMPLETE)
        goto exit_function;

    CotpIndication cotpIndication = CotpConnection_parseIncomingMessage(self->cotpConnection);

    switch (cotpIndication) {
    case COTP_MORE_FRAGMENTS_FOLLOW:
        goto exit_function;

    case COTP_CONNECT_INDICATION:
        if (DEBUG_ISO_SERVER)
            printf("ISO_SERVER: COTP connection indication\n");

#if (CONFIG_MMS_THREADLESS_STACK != 1)
        IsoConnection_lock(self);
#endif

        CotpConnection_sendConnectionResponseMessage(self->cotpConnection);

#if (CONFIG_MMS_THREADLESS_STACK != 1)
        IsoConnection_unlock(self);
#endif

        break;
    case COTP_DATA_INDICATION:
        {
            ByteBuffer* cotpPayload = CotpConnection_getPayload(self->cotpConnection);

            if (DEBUG_ISO_SERVER)
                printf("ISO_SERVER: COTP data indication (payload size = %i)\n", cotpPayload->size);

            IsoSessionIndication sIndication = IsoSession_parseMessage(self->session, cotpPayload);

            ByteBuffer* sessionUserData = IsoSession_getUserData(self->session);

            switch (sIndication) {
            case SESSION_CONNECT:
                if (DEBUG_ISO_SERVER)
                    printf("ISO_SERVER: iso_connection: session connect indication\n");

                if (IsoPresentation_parseConnect(self->presentation, sessionUserData)) {
                    if (DEBUG_ISO_SERVER)
                        printf("ISO_SERVER: iso_connection: presentation ok\n");

                    ByteBuffer* acseBuffer = &(self->presentation->nextPayload);

                    AcseIndication aIndication = AcseConnection_parseMessage(self->acseConnection, acseBuffer);

                    if (aIndication == ACSE_ASSOCIATE) {

#if (CONFIG_MMS_THREADLESS_STACK != 1)
                        IsoConnection_lock(self);
#endif

                        if (DEBUG_ISO_SERVER)
                            printf("ISO_SERVER: cotp_server: acse associate\n");

                        ByteBuffer mmsRequest;

                        ByteBuffer_wrap(&mmsRequest, self->acseConnection->userDataBuffer,
                                self->acseConnection->userDataBufferSize, self->acseConnection->userDataBufferSize);
                        ByteBuffer mmsResponseBuffer; /* new */

                        ByteBuffer_wrap(&mmsResponseBuffer, self->sendBuffer, 0, SEND_BUF_SIZE);

                        if (self->msgRcvdHandler != NULL) {
                            self->msgRcvdHandler(self->handlerParameter,
                                    &mmsRequest, &mmsResponseBuffer);
                        }

                        struct sBufferChain mmsBufferPartStruct;
                        BufferChain mmsBufferPart = &mmsBufferPartStruct;

                        BufferChain_init(mmsBufferPart, mmsResponseBuffer.size, mmsResponseBuffer.size, NULL,
                                self->sendBuffer);

                        if (mmsResponseBuffer.size > 0) {
                            if (DEBUG_ISO_SERVER)
                                printf("ISO_SERVER: iso_connection: application payload size: %i\n",
                                        mmsResponseBuffer.size);

                            struct sBufferChain acseBufferPartStruct;
                            BufferChain acseBufferPart = &acseBufferPartStruct;

                            acseBufferPart->buffer = self->sendBuffer + mmsBufferPart->length;
                            acseBufferPart->partMaxLength = SEND_BUF_SIZE - mmsBufferPart->length;

                            AcseConnection_createAssociateResponseMessage(self->acseConnection,
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
                                printf("ISO_SERVER: iso_connection: association error. No response from application!\n");
                        }

#if (CONFIG_MMS_THREADLESS_STACK != 1)
                        IsoConnection_unlock(self);
#endif
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
                        printf("ISO_SERVER: presentation layer error\n");
					self->state = ISO_CON_STATE_STOPPED;
                    break;
                }

                if (self->presentation->nextContextId == self->presentation->mmsContextId) {
                    if (DEBUG_ISO_SERVER)
                        printf("ISO_SERVER: iso_connection: mms message\n");

                    ByteBuffer* mmsRequest = &(self->presentation->nextPayload);

                    ByteBuffer mmsResponseBuffer;

#if (CONFIG_MMS_THREADLESS_STACK != 1)
                    IsoConnection_lock(self);
#endif

                    ByteBuffer_wrap(&mmsResponseBuffer, self->sendBuffer, 0, SEND_BUF_SIZE);

                    if (self->msgRcvdHandler != NULL) {

                        self->msgRcvdHandler(self->handlerParameter,
                                mmsRequest, &mmsResponseBuffer);
                    }

                    /* send a response if required */
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

#if (CONFIG_MMS_THREADLESS_STACK != 1)
                    IsoConnection_unlock(self);
#endif
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

#if (CONFIG_MMS_THREADLESS_STACK != 1)
                    IsoConnection_lock(self);
#endif

                    struct sBufferChain acseBufferPartStruct;
                    BufferChain acseBufferPart = &acseBufferPartStruct;
                    acseBufferPart->buffer = self->sendBuffer;
                    acseBufferPart->partMaxLength = SEND_BUF_SIZE;

                    AcseConnection_createReleaseResponseMessage(self->acseConnection, acseBufferPart);

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

#if (CONFIG_MMS_THREADLESS_STACK != 1)
                    IsoConnection_unlock(self);
#endif
                }

                self->state = ISO_CON_STATE_STOPPED;

                break;

            case SESSION_ABORT:
                if (DEBUG_ISO_SERVER)
                    printf("ISO_SERVER: iso_connection: session abort indication\n");
                self->state = ISO_CON_STATE_STOPPED;
                break;

            case SESSION_ERROR:
                if (DEBUG_ISO_SERVER)
                    printf("ISO_SERVER: iso_connection: session error indication\n");
                self->state = ISO_CON_STATE_STOPPED;
                break;

            default: /* illegal state */
                if (DEBUG_ISO_SERVER)
                    printf("ISO_SERVER: iso_connection: session illegal state\n");

                self->state = ISO_CON_STATE_STOPPED;
                break;
            }

            CotpConnection_resetPayload(self->cotpConnection);
        }
        break;
    case COTP_ERROR:
        if (DEBUG_ISO_SERVER)
            printf("ISO_SERVER: Connection closed\n");
        self->state = ISO_CON_STATE_STOPPED;
        break;
    default:
        if (DEBUG_ISO_SERVER)
            printf("ISO_SERVER: COTP unknown indication: %i\n", cotpIndication);
        self->state = ISO_CON_STATE_STOPPED;
        break;
    }

exit_function:
    return;
}

#if ((CONFIG_MMS_SINGLE_THREADED == 0) && (CONFIG_MMS_THREADLESS_STACK == 0))
/* only for multi-thread mode */
static void*
handleTcpConnection(void* parameter)
{
    IsoConnection self = (IsoConnection) parameter;

    while(self->state == ISO_CON_STATE_RUNNING)
        IsoConnection_handleTcpConnection(self, false);

    IsoServer_closeConnection(self->isoServer, self);

    finalizeIsoConnection(self);

    self->state = ISO_CON_STATE_TERMINATED;

    return NULL;
}
#endif /* (CONFIG_MMS_SINGLE_THREADED == 0) */

IsoConnection
IsoConnection_create(Socket socket, IsoServer isoServer, bool isSingleThread)
{
    IsoConnection self = (IsoConnection) GLOBAL_CALLOC(1, sizeof(struct sIsoConnection));

    if (self) {
        self->socket = socket;

#if (CONFIG_MMS_SUPPORT_TLS == 1)
        if (IsoServer_getTLSConfiguration(isoServer) != NULL) {
            self->tlsSocket = TLSSocket_create(socket, IsoServer_getTLSConfiguration(isoServer), true);

            if (self->tlsSocket == NULL) {
                if (DEBUG_ISO_SERVER)
                    printf("ISO_SERVER: IsoConnection - TLS initialization failed\n");

                GLOBAL_FREEMEM(self);

                return NULL;
            }
        }
#endif /* (CONFIG_MMS_SUPPORT_TLS == 1) */

        self->receiveBuffer = (uint8_t*) GLOBAL_MALLOC(RECEIVE_BUF_SIZE);
        self->sendBuffer = (uint8_t*) GLOBAL_MALLOC(SEND_BUF_SIZE);
        self->msgRcvdHandler = NULL;
        self->tickHandler = NULL;
        self->handlerParameter = NULL;
        self->isoServer = isoServer;
        self->state = ISO_CON_STATE_RUNNING;
        self->clientAddress = Socket_getPeerAddress(self->socket);
        self->localAddress = Socket_getLocalAddress(self->socket);

#if (CONFIG_MMS_THREADLESS_STACK != 1)
        self->conMutex = Semaphore_create(1);
#endif

        ByteBuffer_wrap(&(self->rcvBuffer), self->receiveBuffer, 0, RECEIVE_BUF_SIZE);

        self->cotpReadBuf = (uint8_t*) GLOBAL_MALLOC(CONFIG_COTP_MAX_TPDU_SIZE + TPKT_RFC1006_HEADER_SIZE);
        self->cotpWriteBuf = (uint8_t*) GLOBAL_MALLOC(CONFIG_COTP_MAX_TPDU_SIZE + TPKT_RFC1006_HEADER_SIZE);

        ByteBuffer_wrap(&(self->cotpReadBuffer), self->cotpReadBuf, 0, CONFIG_COTP_MAX_TPDU_SIZE + TPKT_RFC1006_HEADER_SIZE);
        ByteBuffer_wrap(&(self->cotpWriteBuffer), self->cotpWriteBuf, 0, CONFIG_COTP_MAX_TPDU_SIZE + TPKT_RFC1006_HEADER_SIZE);

        self->cotpConnection = (CotpConnection*) GLOBAL_CALLOC(1, sizeof(CotpConnection));
        CotpConnection_init(self->cotpConnection, self->socket, &(self->rcvBuffer), &(self->cotpReadBuffer), &(self->cotpWriteBuffer));

#if (CONFIG_MMS_SUPPORT_TLS == 1)
        if (self->tlsSocket)
            self->cotpConnection->tlsSocket = self->tlsSocket;
#endif /* (CONFIG_MMS_SUPPORT_TLS == 1) */

        self->session = (IsoSession*) GLOBAL_CALLOC(1, sizeof(IsoSession));
        IsoSession_init(self->session);

        self->presentation = (IsoPresentation*) GLOBAL_CALLOC(1, sizeof(IsoPresentation));
        IsoPresentation_init(self->presentation);

        self->acseConnection = (AcseConnection*) GLOBAL_CALLOC(1, sizeof(AcseConnection));

#if (CONFIG_MMS_SUPPORT_TLS == 1)
        AcseConnection_init(self->acseConnection, IsoServer_getAuthenticator(self->isoServer),
                IsoServer_getAuthenticatorParameter(self->isoServer), self->tlsSocket);
#else
        AcseConnection_init(self->acseConnection, IsoServer_getAuthenticator(self->isoServer),
                IsoServer_getAuthenticatorParameter(self->isoServer), NULL);
#endif

        if (DEBUG_ISO_SERVER)
            printf("ISO_SERVER: IsoConnection: Start to handle connection for client %s\n", self->clientAddress);

#if (CONFIG_MMS_SINGLE_THREADED == 0)
#if (CONFIG_MMS_THREADLESS_STACK == 0)
        if (isSingleThread == false) {
            self->handleSet = Handleset_new();
            Handleset_addSocket(self->handleSet, self->socket);
            self->thread = Thread_create((ThreadExecutionFunction) handleTcpConnection, self, false);
        }
    #endif
#endif
    }

    return self;
}

int
IsoConnection_getState(IsoConnection self)
{
    return self->state;
}

void
IsoConnection_start(IsoConnection self)
{
#if (CONFIG_MMS_SINGLE_THREADED == 0)
#if (CONFIG_MMS_THREADLESS_STACK == 0)
    Thread_start(self->thread);
#endif
#endif
}

void
IsoConnection_destroy(IsoConnection self)
{
#if (CONFIG_MMS_THREADLESS_STACK == 0) && (CONFIG_MMS_SINGLE_THREADED == 0)
    if (self->thread)
        Thread_destroy(self->thread);
#endif

    if (self->socket != NULL)
        Socket_destroy(self->socket);

#if (CONFIG_MMS_SINGLE_THREADED != 1) || (CONFIG_MMS_THREADLESS_STACK == 1)
    if (self->handleSet)
        Handleset_destroy(self->handleSet);
#endif

    if (self->cotpConnection) {
        if (self->cotpConnection->handleSet)
            Handleset_destroy(self->cotpConnection->handleSet);
    }

    GLOBAL_FREEMEM(self);
}

char*
IsoConnection_getPeerAddress(IsoConnection self)
{
    return self->clientAddress;
}

char*
IsoConnection_getLocalAddress(IsoConnection self)
{
    return self->localAddress;
}

void
IsoConnection_lock(IsoConnection self)
{
#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_wait(self->conMutex);
#endif
}

void
IsoConnection_unlock(IsoConnection self)
{
#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_post(self->conMutex);
#endif
}

void
IsoConnection_sendMessage(IsoConnection self, ByteBuffer* message)
{
    if (self->state == ISO_CON_STATE_STOPPED) {
        if (DEBUG_ISO_SERVER)
            printf("DEBUG_ISO_SERVER: sendMessage: connection already stopped!\n");
        goto exit_error;
    }

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
        if (indication != COTP_OK)
            printf("ISO_SERVER: IsoConnection_sendMessage failed!\n");
        else
            printf("ISO_SERVER: IsoConnection_sendMessage success!\n");
    }

exit_error:
    return;
}

void
IsoConnection_close(IsoConnection self)
{
    if (self->state != ISO_CON_STATE_TERMINATED) {
        self->state = ISO_CON_STATE_STOPPED;

#if (CONFIG_MMS_THREADLESS_STACK != 1) && (CONFIG_MMS_SINGLE_THREADED != 1)
        /* wait for connection thread to terminate */
        if (self->thread) {
            Thread_destroy(self->thread);
            self->thread = NULL;
        }
        else {
            finalizeIsoConnection(self);
            self->state = ISO_CON_STATE_TERMINATED;
        }
#else
        finalizeIsoConnection(self);
        self->state = ISO_CON_STATE_TERMINATED;
#endif
    }
}

void
IsoConnection_installListener(IsoConnection self, MessageReceivedHandler rcvdHandler,
        UserLayerTickHandler tickHandler,
        void* parameter)
{
    self->msgRcvdHandler = rcvdHandler;
    self->tickHandler = tickHandler;
    self->handlerParameter = parameter;
}

void*
IsoConnection_getSecurityToken(IsoConnection self)
{
    return self->acseConnection->securityToken;
}

bool
IsoConnection_isRunning(IsoConnection self)
{
    if (self->state == ISO_CON_STATE_RUNNING)
        return true;
    else
        return false;
}
