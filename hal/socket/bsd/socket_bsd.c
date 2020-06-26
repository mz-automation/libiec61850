/*
 *  socket_bsd.c
 *
 *  Copyright 2013-2020 Michael Zillgith
 *  contributions by Michael Clausen (School of engineering Valais).
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

#include "hal_socket.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>

#include <netinet/tcp.h> // required for TCP keepalive

#include <signal.h>
#include <poll.h>

#include "linked_list.h"
#include "hal_thread.h"
#include "lib_memory.h"

#ifndef DEBUG_SOCKET
#define DEBUG_SOCKET 0
#endif

#ifndef MSG_NOSIGNAL
#define MSG_NOSIGNAL 0
#endif

struct sSocket {
    int fd;
    uint32_t connectTimeout;
};

struct sServerSocket {
    int fd;
    int backLog;
};

struct sHandleSet {
    LinkedList sockets;
    bool pollfdIsUpdated;
    struct pollfd* fds;
    int nfds;
};

HandleSet
Handleset_new(void)
{
    HandleSet self = (HandleSet) GLOBAL_MALLOC(sizeof(struct sHandleSet));

    if (self) {
        self->sockets = LinkedList_create();
        self->pollfdIsUpdated = false;
        self->fds = NULL;
        self->nfds = 0;
    }

    return self;
}

void
Handleset_reset(HandleSet self)
{
    if (self) {
        if (self->sockets) {
            LinkedList_destroyStatic(self->sockets);
            self->sockets = LinkedList_create();
            self->pollfdIsUpdated = false;
        }
    }
}

void
Handleset_addSocket(HandleSet self, const Socket sock)
{
    if (self != NULL && sock != NULL && sock->fd != -1) {
        LinkedList_add(self->sockets, sock);
        self->pollfdIsUpdated = false;
    }
}

void
Handleset_removeSocket(HandleSet self, const Socket sock)
{
    if (self && self->sockets && sock) {
        LinkedList_remove(self->sockets, sock);
        self->pollfdIsUpdated = false;
    }
}

int
Handleset_waitReady(HandleSet self, unsigned int timeoutMs)
{
    /* check if pollfd array is updated */
    if (self->pollfdIsUpdated == false) {
        if (self->fds) {
            GLOBAL_FREEMEM(self->fds);
            self->fds = NULL;
        }

        self->nfds = LinkedList_size(self->sockets);

        self->fds = GLOBAL_CALLOC(self->nfds, sizeof(struct pollfd));

        int i;

        for (i = 0; i < self->nfds; i++) {
            LinkedList sockElem = LinkedList_get(self->sockets, i);

            if (sockElem) {
                Socket sock = (Socket) LinkedList_getData(sockElem);

                if (sock) {
                    self->fds[i].fd = sock->fd;
                    self->fds[i].events = POLL_IN;
                }
            }
        }

        self->pollfdIsUpdated = true;
    }

    if (self->fds && self->nfds > 0) {
        int result = poll(self->fds, self->nfds, timeoutMs);

        if (result == -1) {
            if (DEBUG_SOCKET)
                printf("SOCKET: poll error (errno: %i)\n", errno);
        }

        return result;
    }
    else {
        /* there is no socket to wait for */
        return 0;
    }
}

void
Handleset_destroy(HandleSet self)
{
    if (self) {
        if (self->sockets)
            LinkedList_destroyStatic(self->sockets);

        if (self->fds)
            GLOBAL_FREEMEM(self->fds);

        GLOBAL_FREEMEM(self);
    }
}

void
Socket_activateTcpKeepAlive(Socket self, int idleTime, int interval, int count)
{
#if defined SO_KEEPALIVE
    int optval;
    socklen_t optlen = sizeof(optval);

    optval = idleTime;
    setsockopt(self->fd, SOL_SOCKET, SO_KEEPALIVE, &optval, optlen);
    optval = 1;
    setsockopt(self->fd, SOL_SOCKET, SO_NOSIGPIPE, &optval, optlen);

#if defined TCP_KEEPCNT
    optval = interval;
    setsockopt(self->fd, IPPROTO_TCP, TCP_KEEPINTVL, &optval, optlen);

    optval = count;
    setsockopt(self->fd, IPPROTO_TCP, TCP_KEEPCNT, &optval, optlen);
#endif /* TCP_KEEPCNT */

#endif /* SO_KEEPALIVE */
}

static bool
prepareServerAddress(const char* address, int port, struct sockaddr_in* sockaddr)
{

	memset((char *) sockaddr , 0, sizeof(struct sockaddr_in));

	if (address != NULL) {
		struct hostent *server;
		server = gethostbyname(address);

		if (server == NULL) return false;

		memcpy((char *) &sockaddr->sin_addr.s_addr, (char *) server->h_addr, server->h_length);
	}
	else
		sockaddr->sin_addr.s_addr = htonl(INADDR_ANY);

    sockaddr->sin_family = AF_INET;
    sockaddr->sin_port = htons(port);

    return true;
}

static void
setSocketNonBlocking(Socket self)
{
    int flags = fcntl(self->fd, F_GETFL, 0);
    fcntl(self->fd, F_SETFL, flags | O_NONBLOCK);
}

static void
activateTcpNoDelay(Socket self)
{
    /* activate TCP_NODELAY option - packets will be sent immediately */
    int flag = 1;
    setsockopt(self->fd, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof(int));
}

ServerSocket
TcpServerSocket_create(const char* address, int port)
{
    ServerSocket serverSocket = NULL;

    int fd;

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) >= 0) {
        struct sockaddr_in serverAddress;

        if (!prepareServerAddress(address, port, &serverAddress)) {
            close(fd);
            return NULL;
        }

        int optionReuseAddr = 1;
        setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *) &optionReuseAddr, sizeof(int));

        if (bind(fd, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) >= 0) {
            serverSocket = (ServerSocket) GLOBAL_MALLOC(sizeof(struct sServerSocket));
            serverSocket->fd = fd;
            serverSocket->backLog = 0;

            setSocketNonBlocking((Socket) serverSocket);
        }
        else {
            close(fd);
            return NULL ;
        }
    }

    return serverSocket;
}

void
ServerSocket_listen(ServerSocket self)
{
    listen(self->fd, self->backLog);
}


/* CHANGED TO MAKE NON-BLOCKING --> RETURNS NULL IF NO CONNECTION IS PENDING */
Socket
ServerSocket_accept(ServerSocket self)
{
    int fd;

    Socket conSocket = NULL;

    fd = accept(self->fd, NULL, NULL );

    if (fd >= 0) {
        conSocket = (Socket) GLOBAL_CALLOC(1, sizeof(struct sSocket));

        if (conSocket) {
            conSocket->fd = fd;

            activateTcpNoDelay(conSocket);
        }
        else {
            /* out of memory */
            close(fd);

            if (DEBUG_SOCKET)
                printf("SOCKET: out of memory\n");
        }
    }

    return conSocket;
}

void
ServerSocket_setBacklog(ServerSocket self, int backlog)
{
    self->backLog = backlog;
}

static void
closeAndShutdownSocket(int socketFd)
{
    if (socketFd != -1) {

        if (DEBUG_SOCKET)
            printf("socket_linux.c: call shutdown for %i!\n", socketFd);

        /* shutdown is required to unblock read or accept in another thread! */
        shutdown(socketFd, SHUT_RDWR);

        close(socketFd);
    }
}

void
ServerSocket_destroy(ServerSocket self)
{
    int fd = self->fd;

    self->fd = -1;

    closeAndShutdownSocket(fd);

    Thread_sleep(10);

    GLOBAL_FREEMEM(self);
}

Socket
TcpSocket_create()
{
    Socket self = NULL;

    int sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock != -1) {
        self = (Socket) GLOBAL_MALLOC(sizeof(struct sSocket));

        if (self) {
            self->fd = sock;
            self->connectTimeout = 5000;

#if 0
            int tcpUserTimeout = 10000;
            int result = setsockopt(sock, SOL_TCP,  TCP_USER_TIMEOUT, &tcpUserTimeout, sizeof(tcpUserTimeout));
#endif
        }
        else {
            /* out of memory */
            close(sock);

            if (DEBUG_SOCKET)
                printf("SOCKET: out of memory\n");
        }

    }
    else {
        if (DEBUG_SOCKET)
            printf("SOCKET: failed to create socket (errno=%i)\n", errno);
    }

    return self;
}

void
Socket_setConnectTimeout(Socket self, uint32_t timeoutInMs)
{
    self->connectTimeout = timeoutInMs;
}

bool
Socket_setLocalAddress(Socket self, const char* address, int port)
{
    struct sockaddr_in clientAddress;

    if (!prepareServerAddress(address, port, &clientAddress))
        return false;

    if (bind(self->fd, (struct sockaddr *) &clientAddress, sizeof(clientAddress)) != 0)
        return false;

    return true;
}

bool
Socket_connectAsync(Socket self, const char* address, int port)
{
    struct sockaddr_in serverAddress;

    if (DEBUG_SOCKET)
        printf("SOCKET: connect: %s:%i\n", address, port);

    if (!prepareServerAddress(address, port, &serverAddress))
        return false;

    fd_set fdSet;
    FD_ZERO(&fdSet);
    FD_SET(self->fd, &fdSet);

    activateTcpNoDelay(self);

    fcntl(self->fd, F_SETFL, O_NONBLOCK);

    if (connect(self->fd, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {

        if (errno != EINPROGRESS) {
            self->fd = -1;
            return false;
        }
    }

    return true; /* is connecting or already connected */
}

SocketState
Socket_checkAsyncConnectState(Socket self)
{
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    fd_set fdSet;
    FD_ZERO(&fdSet);
    FD_SET(self->fd, &fdSet);

    int selectVal = select(self->fd + 1, NULL, &fdSet , NULL, &timeout);

    if (selectVal == 1) {

        /* Check if connection is established */

        int so_error;
        socklen_t len = sizeof so_error;

        if (getsockopt(self->fd, SOL_SOCKET, SO_ERROR, &so_error, &len) >= 0) {

            if (so_error == 0)
                return SOCKET_STATE_CONNECTED;
        }

        return SOCKET_STATE_FAILED;
    }
    else if (selectVal == 0) {
        return SOCKET_STATE_CONNECTING;
    }
    else {
        return SOCKET_STATE_FAILED;
    }
}

bool
Socket_connect(Socket self, const char* address, int port)
{
    if (Socket_connectAsync(self, address, port) == false)
        return false;

    struct timeval timeout;
    timeout.tv_sec = self->connectTimeout / 1000;
    timeout.tv_usec = (self->connectTimeout % 1000) * 1000;

    fd_set fdSet;
    FD_ZERO(&fdSet);
    FD_SET(self->fd, &fdSet);

    if (select(self->fd + 1, NULL, &fdSet , NULL, &timeout) == 1) {

        /* Check if connection is established */

        int so_error;
        socklen_t len = sizeof so_error;

        if (getsockopt(self->fd, SOL_SOCKET, SO_ERROR, &so_error, &len) >= 0) {

            if (so_error == 0)
                return true;
        }
    }

    close (self->fd);
    self->fd = -1;

    return false;
}

static char*
convertAddressToStr(struct sockaddr_storage* addr)
{
    char addrString[INET6_ADDRSTRLEN + 7];
    int port;

    bool isIPv6;

    if (addr->ss_family == AF_INET) {
        struct sockaddr_in* ipv4Addr = (struct sockaddr_in*) addr;
        port = ntohs(ipv4Addr->sin_port);
        inet_ntop(AF_INET, &(ipv4Addr->sin_addr), addrString, INET_ADDRSTRLEN);
        isIPv6 = false;
    }
    else if (addr->ss_family == AF_INET6) {
        struct sockaddr_in6* ipv6Addr = (struct sockaddr_in6*) addr;
        port = ntohs(ipv6Addr->sin6_port);
        inet_ntop(AF_INET6, &(ipv6Addr->sin6_addr), addrString, INET6_ADDRSTRLEN);
        isIPv6 = true;
    }
    else
        return NULL ;

    char* clientConnection = (char*) GLOBAL_MALLOC(strlen(addrString) + 9);

    if (isIPv6)
        sprintf(clientConnection, "[%s]:%i", addrString, port);
    else
        sprintf(clientConnection, "%s:%i", addrString, port);

    return clientConnection;
}

char*
Socket_getPeerAddress(Socket self)
{
    struct sockaddr_storage addr;
    socklen_t addrLen = sizeof(addr);

    if (getpeername(self->fd, (struct sockaddr*) &addr, &addrLen) == 0) {
        return convertAddressToStr(&addr);
    }
    else
        return NULL;
}

char*
Socket_getLocalAddress(Socket self)
{
    struct sockaddr_storage addr;
    socklen_t addrLen = sizeof(addr);

    if (getsockname(self->fd, (struct sockaddr*) &addr, &addrLen) == 0) {
        return convertAddressToStr(&addr);
    }
    else
        return NULL;
}

char*
Socket_getPeerAddressStatic(Socket self, char* peerAddressString)
{
    struct sockaddr_storage addr;
    socklen_t addrLen = sizeof(addr);

    getpeername(self->fd, (struct sockaddr*) &addr, &addrLen);

    char addrString[INET6_ADDRSTRLEN + 7];
    int port;

    bool isIPv6;

    if (addr.ss_family == AF_INET) {
        struct sockaddr_in* ipv4Addr = (struct sockaddr_in*) &addr;
        port = ntohs(ipv4Addr->sin_port);
        inet_ntop(AF_INET, &(ipv4Addr->sin_addr), addrString, INET_ADDRSTRLEN);
        isIPv6 = false;
    }
    else if (addr.ss_family == AF_INET6) {
        struct sockaddr_in6* ipv6Addr = (struct sockaddr_in6*) &addr;
        port = ntohs(ipv6Addr->sin6_port);
        inet_ntop(AF_INET6, &(ipv6Addr->sin6_addr), addrString, INET6_ADDRSTRLEN);
        isIPv6 = true;
    }
    else
        return NULL ;

    if (isIPv6)
        sprintf(peerAddressString, "[%s]:%i", addrString, port);
    else
        sprintf(peerAddressString, "%s:%i", addrString, port);

    return peerAddressString;
}

int
Socket_read(Socket self, uint8_t* buf, int size)
{
    if (self->fd == -1)
        return -1;

    int read_bytes = recv(self->fd, buf, size, MSG_DONTWAIT);

    if (read_bytes == 0)
        return -1;

    if (read_bytes == -1) {
        int error = errno;

        switch (error) {

            case EAGAIN:
                return 0;
            case EBADF:
                return -1;

            default:
                return -1;
        }
    }

    return read_bytes;
}

int
Socket_write(Socket self, uint8_t* buf, int size)
{
    if (self->fd == -1)
        return -1;

    /* MSG_NOSIGNAL - prevent send to signal SIGPIPE when peer unexpectedly closed the socket */
    int retVal = send(self->fd, buf, size, MSG_NOSIGNAL | MSG_DONTWAIT);

    if ((retVal == -1) && (errno == EAGAIN))
        return 0;
    else
        return retVal;
}

void
Socket_destroy(Socket self)
{
    int fd = self->fd;

    self->fd = -1;

    closeAndShutdownSocket(fd);

    Thread_sleep(10);

    GLOBAL_FREEMEM(self);
}
