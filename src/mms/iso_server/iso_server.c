/*
 *  iso_server.c
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

#ifndef DEBUG_ISO_SERVER
#ifdef DEBUG
#define DEBUG_ISO_SERVER 1
#else
#define DEBUG_ISO_SERVER 0
#endif /*DEBUG */
#endif /* DEBUG_ISO_SERVER */

#include "mms_server_connection.h"

#include "hal_thread.h"

#include "iso_server.h"

#include "iso_server_private.h"

#ifndef CONFIG_MAXIMUM_TCP_CLIENT_CONNECTIONS
#define CONFIG_MAXIMUM_TCP_CLIENT_CONNECTIONS 5
#endif

#define TCP_PORT 102
#define SECURE_TCP_PORT 3782
#define BACKLOG 10

struct sIsoServer {
    IsoServerState state;

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore stateLock;
#endif

    ConnectionIndicationHandler connectionHandler;
    void* connectionHandlerParameter;

    AcseAuthenticator authenticator;
    void* authenticatorParameter;

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Thread serverThread;
#endif

    HandleSet handleset;

    Socket serverSocket;
    int tcpPort;
    char* localIpAddress;

    TLSConfiguration tlsConfiguration;

#if (CONFIG_MMS_SERVER_CONFIG_SERVICES_AT_RUNTIME == 1)
    int maxConnections;
#endif

#if (CONFIG_MAXIMUM_TCP_CLIENT_CONNECTIONS == -1)
    LinkedList openClientConnections;
#else
    IsoConnection openClientConnections[CONFIG_MAXIMUM_TCP_CLIENT_CONNECTIONS];
#endif /* (CONFIG_MAXIMUM_TCP_CLIENT_CONNECTIONS == -1) */

#if (CONFIG_MMS_THREADLESS_STACK != 1) && (CONFIG_MMS_SINGLE_THREADED == 0)
    Semaphore openClientConnectionsMutex; /* mutex for openClientConnections list */
    Semaphore connectionCounterMutex;
#endif

    int connectionCounter;
};

static void
setState(IsoServer self, IsoServerState newState)
{
#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_wait(self->stateLock);
#endif
    self->state = newState;
#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_post(self->stateLock);
#endif
}

static IsoServerState
getState(IsoServer self)
{
    IsoServerState state;

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_wait(self->stateLock);
#endif
    state = self->state;
#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_post(self->stateLock);
#endif

    return state;
}

#if (CONFIG_MMS_THREADLESS_STACK != 1) && (CONFIG_MMS_SINGLE_THREADED == 0)
static inline void
lockClientConnections(IsoServer self)
{
    Semaphore_wait(self->openClientConnectionsMutex);
}

static inline void
unlockClientConnections(IsoServer self)
{
    Semaphore_post(self->openClientConnectionsMutex);
}
#endif /* (CONFIG_MAXIMUM_TCP_CLIENT_CONNECTIONS == -1) */

static void
addClientConnection(IsoServer self, IsoConnection connection)
{

#if (CONFIG_MMS_THREADLESS_STACK != 1) && (CONFIG_MMS_SINGLE_THREADED == 0)
    Semaphore_wait(self->connectionCounterMutex);
#endif

    self->connectionCounter++;
    if (DEBUG_ISO_SERVER)
        printf("ISO_SERVER: increase connection counter to %i!\n", self->connectionCounter);

#if (CONFIG_MMS_THREADLESS_STACK != 1) && (CONFIG_MMS_SINGLE_THREADED == 0)
    lockClientConnections(self);
#endif

#if (CONFIG_MAXIMUM_TCP_CLIENT_CONNECTIONS == -1)
    LinkedList_add(self->openClientConnections, connection);
#else
    int i;

    for (i = 0; i < CONFIG_MAXIMUM_TCP_CLIENT_CONNECTIONS; i++) {
        if (self->openClientConnections[i] == NULL) {
            self->openClientConnections[i] = connection;

            if (DEBUG_ISO_SERVER)
                printf("ISO_SERVER: added connection (%p) index:%i\n", connection, i);

            break;
        }
    }
#endif

#if (CONFIG_MMS_THREADLESS_STACK != 1) && (CONFIG_MMS_SINGLE_THREADED == 0)
    unlockClientConnections(self);
#endif

#if (CONFIG_MMS_THREADLESS_STACK != 1) && (CONFIG_MMS_SINGLE_THREADED == 0)
    Semaphore_post(self->connectionCounterMutex);
#endif

}

static void
removeClientConnection(IsoServer self, IsoConnection connection)
{
#if (CONFIG_MAXIMUM_TCP_CLIENT_CONNECTIONS == -1)

    LinkedList_remove(self->openClientConnections, connection);

#if (CONFIG_MMS_SINGLE_THREADED == 1)
    IsoConnection_removeFromHandleSet(connection, self->handleset);
#endif


#else

    int i;

    for (i = 0; i < CONFIG_MAXIMUM_TCP_CLIENT_CONNECTIONS; i++) {
        if (self->openClientConnections[i] == connection) {

#if (CONFIG_MMS_SINGLE_THREADED == 1)
            IsoConnection_removeFromHandleSet(connection, self->handleset);
#endif

            if (DEBUG_ISO_SERVER)
                printf("ISO_SERVER: removed connection (%p) index:%i\n", connection, i);

            self->openClientConnections[i] = NULL;
            break;
        }
    }
#endif /* (CONFIG_MAXIMUM_TCP_CLIENT_CONNECTIONS == -1) */
}

static void
removeTerminatedConnections(IsoServer self, bool isSingleThread)
{
#if (CONFIG_MMS_THREADLESS_STACK != 1) && (CONFIG_MMS_SINGLE_THREADED == 0)
    lockClientConnections(self);
#endif

#if (CONFIG_MAXIMUM_TCP_CLIENT_CONNECTIONS == -1)

    LinkedList openConnection = LinkedList_getNext(self->openClientConnections);

    while (openConnection) {
        IsoConnection isoConnection = (IsoConnection) openConnection->data;

        if (isSingleThread) {
            if (IsoConnection_getState(isoConnection) == ISO_CON_STATE_STOPPED) {

                self->connectionHandler(ISO_CONNECTION_CLOSED, self->connectionHandlerParameter,
                        isoConnection);

                IsoConnection_close(isoConnection);
            }
        }

        if (IsoConnection_getState(isoConnection) == ISO_CON_STATE_TERMINATED) {
            removeClientConnection(self, isoConnection);
            IsoConnection_destroy(isoConnection);
            openConnection = LinkedList_getNext(self->openClientConnections);
        }
        else
            openConnection = LinkedList_getNext(openConnection);
    }

#else
    int i;

    for (i = 0; i < CONFIG_MAXIMUM_TCP_CLIENT_CONNECTIONS; i++) {
        if (self->openClientConnections[i] != NULL) {

            IsoConnection isoConnection = self->openClientConnections[i];

            if (isSingleThread) {
                if (IsoConnection_getState(isoConnection) == ISO_CON_STATE_STOPPED) {

                    self->connectionHandler(ISO_CONNECTION_CLOSED, self->connectionHandlerParameter,
                            isoConnection);

                    IsoConnection_close(isoConnection);

                    IsoConnection_removeFromHandleSet(isoConnection, self->handleset);
                }
            }

            if (IsoConnection_getState(isoConnection) == ISO_CON_STATE_TERMINATED) {
                removeClientConnection(self, isoConnection);
                IsoConnection_destroy(isoConnection);
            }
        }
    }
#endif /* (CONFIG_MAXIMUM_TCP_CLIENT_CONNECTIONS == -1) */

#if (CONFIG_MMS_THREADLESS_STACK != 1) && (CONFIG_MMS_SINGLE_THREADED == 0)
    unlockClientConnections(self);
#endif

}

static void
closeAllOpenClientConnections(IsoServer self)
{

#if (CONFIG_MMS_THREADLESS_STACK != 1) && (CONFIG_MMS_SINGLE_THREADED == 0)
    lockClientConnections(self);
#endif

#if (CONFIG_MAXIMUM_TCP_CLIENT_CONNECTIONS == -1)

    LinkedList openConnection = LinkedList_getNext(self->openClientConnections);
    while (openConnection != NULL) {
        IsoConnection isoConnection = (IsoConnection) openConnection->data;

        IsoConnection_close(isoConnection);
        IsoConnection_destroy(isoConnection);

        openConnection = LinkedList_getNext(openConnection);
    }

#if (CONFIG_MMS_SINGLE_THREADED == 1)
    LinkedList_destroyStatic(self->openClientConnections);
    self->openClientConnections = NULL;
#endif

#else
    int i;

    for (i = 0; i < CONFIG_MAXIMUM_TCP_CLIENT_CONNECTIONS; i++) {
        if (self->openClientConnections[i] != NULL) {
            IsoConnection_close(self->openClientConnections[i]);
            IsoConnection_destroy(self->openClientConnections[i]);
        }
    }
#endif /* (CONFIG_MAXIMUM_TCP_CLIENT_CONNECTIONS == -1) */

#if (CONFIG_MMS_THREADLESS_STACK != 1) && (CONFIG_MMS_SINGLE_THREADED == 0)
    unlockClientConnections(self);
#endif

}

static void
callTickHandlerForClientConnections(IsoServer self)
{

#if (CONFIG_MMS_THREADLESS_STACK != 1) && (CONFIG_MMS_SINGLE_THREADED == 0)
    lockClientConnections(self);
#endif

#if (CONFIG_MAXIMUM_TCP_CLIENT_CONNECTIONS == -1)

    LinkedList openConnection = LinkedList_getNext(self->openClientConnections);

    while (openConnection != NULL) {
        IsoConnection isoConnection = (IsoConnection) openConnection->data;

        IsoConnection_callTickHandler(isoConnection);

        openConnection = LinkedList_getNext(openConnection);
    }

#else
    int i;

    for (i = 0; i < CONFIG_MAXIMUM_TCP_CLIENT_CONNECTIONS; i++) {
        if (self->openClientConnections[i] != NULL) {
            IsoConnection_callTickHandler(self->openClientConnections[i]);
        }
    }
#endif /* (CONFIG_MAXIMUM_TCP_CLIENT_CONNECTIONS == -1) */

#if (CONFIG_MMS_THREADLESS_STACK != 1) && (CONFIG_MMS_SINGLE_THREADED == 0)
    unlockClientConnections(self);
#endif

}

static void
handleClientConnections(IsoServer self)
{
#if (CONFIG_MAXIMUM_TCP_CLIENT_CONNECTIONS == -1)

#if (CONFIG_MMS_THREADLESS_STACK != 1) && (CONFIG_MMS_SINGLE_THREADED == 0)
    lockClientConnections(self);
#endif

    LinkedList openConnection = LinkedList_getNext(self->openClientConnections);

    while (openConnection != NULL) {
        IsoConnection isoConnection = (IsoConnection) openConnection->data;

        if (IsoConnection_isRunning(isoConnection))
            IsoConnection_handleTcpConnection(isoConnection, true);

        openConnection = LinkedList_getNext(openConnection);
    }

#if (CONFIG_MMS_THREADLESS_STACK != 1) && (CONFIG_MMS_SINGLE_THREADED == 0)
    unlockClientConnections(self);
#endif

    removeTerminatedConnections(self, true);

#else

#if (CONFIG_MMS_THREADLESS_STACK != 1) && (CONFIG_MMS_SINGLE_THREADED == 0)
    lockClientConnections(self);
#endif

    int i;

    for (i = 0; i < CONFIG_MAXIMUM_TCP_CLIENT_CONNECTIONS; i++) {
        if (self->openClientConnections[i] != NULL) {
            if (IsoConnection_isRunning(self->openClientConnections[i])) {

                IsoConnection_handleTcpConnection(self->openClientConnections[i], true);
            }
            else {
                IsoConnection_destroy(self->openClientConnections[i]);

                self->openClientConnections[i] = NULL;
            }

        }
    }

#if (CONFIG_MMS_THREADLESS_STACK != 1) && (CONFIG_MMS_SINGLE_THREADED == 0)
    unlockClientConnections(self);
#endif

    removeTerminatedConnections(self, true);

#endif /* (CONFIG_MAXIMUM_TCP_CLIENT_CONNECTIONS == -1) */
}

static bool
setupIsoServer(IsoServer self)
{
    bool success = true;

    self->serverSocket = (Socket) TcpServerSocket_create(self->localIpAddress, self->tcpPort);

    if (self->serverSocket == NULL) {
        setState(self, ISO_SVR_STATE_ERROR);
        success = false;

        goto exit_function;
    }

    self->handleset = Handleset_new();
    Handleset_addSocket(self->handleset, self->serverSocket);

#if (CONFIG_ACTIVATE_TCP_KEEPALIVE == 1)
    Socket_activateTcpKeepAlive(self->serverSocket,
            CONFIG_TCP_KEEPALIVE_IDLE,
            CONFIG_TCP_KEEPALIVE_INTERVAL,
            CONFIG_TCP_KEEPALIVE_CNT);
#endif

    ServerSocket_setBacklog((ServerSocket) self->serverSocket, BACKLOG);

    ServerSocket_listen((ServerSocket) self->serverSocket);

    setState(self, ISO_SVR_STATE_RUNNING);

exit_function:
    return success;
}


/** used by single and multi-threaded versions
 *
 * \param isSingleThread when true server is running in single thread or non-thread mode
 */
static void
handleIsoConnections(IsoServer self, bool isSingleThread)
{
    if (isSingleThread) {
        /*
         * NOTE: when running in multi thread mode the tick handler is called
         * by the connection thread.
         */
        callTickHandlerForClientConnections(self);
    }

    if (Handleset_waitReady(self->handleset, 1) < 1)
        return;

    Socket connectionSocket;

    if ((connectionSocket = ServerSocket_accept((ServerSocket) self->serverSocket)) != NULL) {

#if (CONFIG_MMS_SERVER_CONFIG_SERVICES_AT_RUNTIME == 1)
        if (self->maxConnections > -1) {
            if (private_IsoServer_getConnectionCounter(self) >= self->maxConnections) {
                if (DEBUG_ISO_SERVER)
                    printf("ISO_SERVER: maximum number of connections reached -> reject connection attempt.\n");

                Socket_destroy(connectionSocket);

                return;
            }
        }
#endif /* (CONFIG_MMS_SERVER_CONFIG_SERVICES_AT_RUNTIME == 1) */

#if (CONFIG_MAXIMUM_TCP_CLIENT_CONNECTIONS != -1)
        if (private_IsoServer_getConnectionCounter(self) >= CONFIG_MAXIMUM_TCP_CLIENT_CONNECTIONS) {
            if (DEBUG_ISO_SERVER)
                printf("ISO_SERVER: maximum number of connections reached -> reject connection attempt.\n");

            Socket_destroy(connectionSocket);

            if (isSingleThread)
                handleClientConnections(self);

            return;
        }
#endif

        IsoConnection isoConnection = IsoConnection_create(connectionSocket, self, isSingleThread);

        if (isoConnection) {
            addClientConnection(self, isoConnection);

            if (isSingleThread)
                IsoConnection_addToHandleSet(isoConnection, self->handleset);

            self->connectionHandler(ISO_CONNECTION_OPENED, self->connectionHandlerParameter,
                    isoConnection);

            if (isSingleThread == false)
                IsoConnection_start(isoConnection);
        }
        else {
            Socket_destroy(connectionSocket);
        }
    }

    if (isSingleThread)
        handleClientConnections(self);
}

#if (CONFIG_MMS_SINGLE_THREADED == 0) && (CONFIG_MMS_THREADLESS_STACK == 0)
/* only required for multi-threaded server! */
static void*
isoServerThread(void* isoServerParam)
{
    IsoServer self = (IsoServer) isoServerParam;

    if (!setupIsoServer(self)) {
        if (DEBUG_ISO_SERVER)
            printf("ISO_SERVER: starting server failed!\n");

        self->serverSocket = NULL;

        goto cleanUp;
    }

    if (DEBUG_ISO_SERVER)
        printf("ISO_SERVER: isoServerThread %p started\n", isoServerParam);

    while (self->state == ISO_SVR_STATE_RUNNING)
    {
        removeTerminatedConnections(self, false);

        handleIsoConnections(self, false);
    }

    self->state = ISO_SVR_STATE_STOPPED;

cleanUp:

    if (DEBUG_ISO_SERVER)
        printf("ISO_SERVER: isoServerThread %p stopped\n", isoServerParam);

    return NULL;
}
#endif

IsoServer
IsoServer_create(TLSConfiguration tlsConfiguration)
{
    IsoServer self = (IsoServer) GLOBAL_CALLOC(1, sizeof(struct sIsoServer));

    if (self) {
        self->state = ISO_SVR_STATE_IDLE;

        if (tlsConfiguration == NULL)
            self->tcpPort = TCP_PORT;
        else
            self->tcpPort = SECURE_TCP_PORT;

        self->tlsConfiguration = tlsConfiguration;

#if (CONFIG_MMS_THREADLESS_STACK != 1)
        self->stateLock = Semaphore_create(1);
#endif

#if (CONFIG_MAXIMUM_TCP_CLIENT_CONNECTIONS == -1)
        self->openClientConnections = LinkedList_create();
#endif

#if (CONFIG_MMS_SERVER_CONFIG_SERVICES_AT_RUNTIME == 1)
        self->maxConnections = CONFIG_MAXIMUM_TCP_CLIENT_CONNECTIONS;
#endif

#if (CONFIG_MMS_THREADLESS_STACK != 1) && (CONFIG_MMS_SINGLE_THREADED == 0)
        self->connectionCounterMutex = Semaphore_create(1);
        self->openClientConnectionsMutex = Semaphore_create(1);
#endif /* (CONFIG_MMS_THREADLESS_STACK != 1) */

        self->connectionCounter = 0;
    }

    return self;
}

#if (CONFIG_MMS_SERVER_CONFIG_SERVICES_AT_RUNTIME == 1)
void
IsoServer_setMaxConnections(IsoServer self, int maxConnections)
{
    self->maxConnections = maxConnections;
}
#endif /* (CONFIG_MMS_SERVER_CONFIG_SERVICES_AT_RUNTIME == 1) */

void
IsoServer_setTcpPort(IsoServer self, int port)
{
    self->tcpPort = port;
}

void
IsoServer_setLocalIpAddress(IsoServer self, const char* ipAddress)
{
    GLOBAL_FREEMEM(self->localIpAddress);

    if (ipAddress)
        self->localIpAddress = StringUtils_copyString(ipAddress);
    else
        self->localIpAddress = NULL;
}

IsoServerState
IsoServer_getState(IsoServer self)
{
    return getState(self);
}

void
IsoServer_setAuthenticator(IsoServer self, AcseAuthenticator authenticator, void* authenticatorParameter)
{
    self->authenticator = authenticator;
    self->authenticatorParameter = authenticatorParameter;
}

AcseAuthenticator
IsoServer_getAuthenticator(IsoServer self)
{
    return self->authenticator;
}

void*
IsoServer_getAuthenticatorParameter(IsoServer self)
{
    return self->authenticatorParameter;
}

TLSConfiguration
IsoServer_getTLSConfiguration(IsoServer self)
{
    return self->tlsConfiguration;
}

#if (CONFIG_MMS_THREADLESS_STACK != 1) && (CONFIG_MMS_SINGLE_THREADED != 1)
void
IsoServer_startListening(IsoServer self)
{
    if (self->state == ISO_SVR_STATE_RUNNING) {
        if (DEBUG_ISO_SERVER)
                printf("ISO_SERVER: server already in RUNNING state!\n");

        goto exit_function;
    }

    self->state = ISO_SVR_STATE_IDLE;

    self->serverThread = Thread_create((ThreadExecutionFunction) isoServerThread, self, false);

    Thread_start(self->serverThread);

    /* wait until server is up */
    while (self->state == ISO_SVR_STATE_IDLE)
        Thread_sleep(1);

    if (DEBUG_ISO_SERVER)
        printf("ISO_SERVER: new iso server thread started\n");

exit_function:
    return;
}
#endif /* (CONFIG_MMS_THREADLESS_STACK != 1) && (CONFIG_MMS_SINGLE_THREADED != 1) */

void
IsoServer_startListeningThreadless(IsoServer self)
{
    if (!setupIsoServer(self)) {
        if (DEBUG_ISO_SERVER)
            printf("ISO_SERVER: starting server failed!\n");

        self->serverSocket = NULL;
    }
    else {
        setState(self, ISO_SVR_STATE_RUNNING);

        if (DEBUG_ISO_SERVER)
            printf("ISO_SERVER: new iso server (threadless) started\n");
    }
}

int
IsoServer_waitReady(IsoServer self, unsigned int timeoutMs)
{
   int result = -1;

   if (getState(self) == ISO_SVR_STATE_RUNNING) {

       if (self->handleset) {
           result = Handleset_waitReady(self->handleset, timeoutMs);
       }
       else {
           if (DEBUG_ISO_SERVER)
               printf("ISO_SERVER: internal error - no handleset!\n");
       }

   }

   return result;
}

void
IsoServer_processIncomingMessages(IsoServer self)
{
    if (getState(self) == ISO_SVR_STATE_RUNNING)
        handleIsoConnections(self, true);
}

int
IsoServer_getConnectionCounter(IsoServer self)
{
    return private_IsoServer_getConnectionCounter(self);
}

static void
stopListening(IsoServer self)
{
    setState(self, ISO_SVR_STATE_STOPPED);

    if (self->serverSocket != NULL) {
        ServerSocket_destroy((ServerSocket) self->serverSocket);
        self->serverSocket = NULL;
    }
}

void
IsoServer_stopListeningThreadless(IsoServer self)
{
    stopListening(self);

    closeAllOpenClientConnections(self);

    if (DEBUG_ISO_SERVER)
        printf("ISO_SERVER: IsoServer_stopListeningThreadless finished!\n");
}

#if (CONFIG_MMS_THREADLESS_STACK != 1)
void
IsoServer_stopListening(IsoServer self)
{
    setState(self, ISO_SVR_STATE_STOPPED);

    if (self->serverThread != NULL)
        Thread_destroy(self->serverThread);

    if (self->serverSocket != NULL) {
        ServerSocket_destroy((ServerSocket) self->serverSocket);
        self->serverSocket = NULL;
    }

    closeAllOpenClientConnections(self);

    /* Wait for connection threads to finish */
    while (private_IsoServer_getConnectionCounter(self) > 0)
        Thread_sleep(10);

    if (DEBUG_ISO_SERVER)
        printf("ISO_SERVER: IsoServer_stopListening finished!\n");
}
#endif

void
IsoServer_closeConnection(IsoServer self, IsoConnection isoConnection)
{
    if (getState(self) != ISO_SVR_STATE_IDLE) {
        self->connectionHandler(ISO_CONNECTION_CLOSED, self->connectionHandlerParameter,
                isoConnection);
    }
}

void
IsoServer_setConnectionHandler(IsoServer self, ConnectionIndicationHandler handler,
        void* parameter)
{
    self->connectionHandler = handler;
    self->connectionHandlerParameter = parameter;
}

void
IsoServer_destroy(IsoServer self)
{
    if (self) {

#if (CONFIG_MMS_THREADLESS_STACK != 1)
        if (self->state == ISO_SVR_STATE_RUNNING)
            IsoServer_stopListening(self);
#endif

#if (CONFIG_MAXIMUM_TCP_CLIENT_CONNECTIONS == -1)

#if (CONFIG_MMS_SINGLE_THREADED == 1)
        if (self->openClientConnections != NULL)
            LinkedList_destroy(self->openClientConnections);
#else
        if (self->openClientConnections != NULL)
            LinkedList_destroyStatic(self->openClientConnections);
#endif /* (CONFIG_MMS_SINGLE_THREADED == 1) */

#if (CONFIG_MMS_THREADLESS_STACK != 1) && (CONFIG_MMS_SINGLE_THREADED == 0)
        lockClientConnections(self);
#endif

#endif /* (CONFIG_MAXIMUM_TCP_CLIENT_CONNECTIONS == -1) */

#if (CONFIG_MMS_THREADLESS_STACK != 1) && (CONFIG_MMS_SINGLE_THREADED == 0)
        Semaphore_destroy(self->connectionCounterMutex);
        Semaphore_destroy(self->openClientConnectionsMutex);
#endif

        if (self->handleset)
            Handleset_destroy(self->handleset);

#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_destroy(self->stateLock);
#endif

        GLOBAL_FREEMEM(self->localIpAddress);

        GLOBAL_FREEMEM(self);
    }
}

void
private_IsoServer_decreaseConnectionCounter(IsoServer self)
{
#if (CONFIG_MMS_THREADLESS_STACK != 1) && (CONFIG_MMS_SINGLE_THREADED == 0)
    Semaphore_wait(self->connectionCounterMutex);
#endif

    self->connectionCounter--;

    if (DEBUG_ISO_SERVER)
        printf("IsoServer: decrease connection counter to %i!\n", self->connectionCounter);

#if (CONFIG_MMS_THREADLESS_STACK != 1) && (CONFIG_MMS_SINGLE_THREADED == 0)
    Semaphore_post(self->connectionCounterMutex);
#endif
}

int
private_IsoServer_getConnectionCounter(IsoServer self)
{
    int connectionCounter;

#if (CONFIG_MMS_THREADLESS_STACK != 1) && (CONFIG_MMS_SINGLE_THREADED == 0)
    Semaphore_wait(self->connectionCounterMutex);
#endif

    connectionCounter = self->connectionCounter;

#if (CONFIG_MMS_THREADLESS_STACK != 1) && (CONFIG_MMS_SINGLE_THREADED == 0)
    Semaphore_post(self->connectionCounterMutex);
#endif

    return connectionCounter;
}

