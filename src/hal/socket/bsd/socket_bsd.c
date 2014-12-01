/*
 *  socket_bsd.c
 *
 *  Copyright 2013 Michael Zillgith, contributed to the project by Michael Clausen (School of engineering Valais).
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

#include "socket.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>

#include <fcntl.h>

#include <netinet/tcp.h> // required for TCP keepalive

#include "thread.h"

#include "libiec61850_platform_includes.h"

#ifndef DEBUG_SOCKET
#define DEBUG_SOCKET 0
#endif

struct sSocket {
    int fd;
};

struct sServerSocket {
    int fd;
    int backLog;
};

static void
activateKeepAlive(int sd)
{
#if defined SO_KEEPALIVE
    int optval;
    socklen_t optlen = sizeof(optval);

    optval = CONFIG_TCP_KEEPALIVE_IDLE;
    setsockopt(sd, SOL_SOCKET, SO_KEEPALIVE, &optval, optlen);
    optval = 1;
    setsockopt(sd, SOL_SOCKET, SO_NOSIGPIPE, &optval, optlen);

#if defined TCP_KEEPCNT
    optval = CONFIG_TCP_KEEPALIVE_INTERVAL;
    setsockopt(sd, IPPROTO_TCP, TCP_KEEPINTVL, &optval, optlen);

    optval = CONFIG_TCP_KEEPALIVE_CNT;
    setsockopt(sd, IPPROTO_TCP, TCP_KEEPCNT, &optval, optlen);
#endif /* TCP_KEEPCNT */

#endif /* SO_KEEPALIVE */
}

static bool
prepareServerAddress(char* address, int port, struct sockaddr_in* sockaddr)
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

#if 0
static void
setSocketNonBlocking(Socket self)
{
    int flags = fcntl(self->fd, F_GETFL, 0);
    fcntl(self->fd, F_SETFL, flags | O_NONBLOCK);
}
#endif

ServerSocket
TcpServerSocket_create(char* address, int port)
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
            serverSocket = malloc(sizeof(struct sServerSocket));
            serverSocket->fd = fd;
            serverSocket->backLog = 0;
        }
        else {
            close(fd);
            return NULL ;
        }

#if CONFIG_ACTIVATE_TCP_KEEPALIVE == 1
        activateKeepAlive(fd);
#endif
    }

    return serverSocket;
}

void
ServerSocket_listen(ServerSocket self)
{
    listen(self->fd, self->backLog);
}

Socket
ServerSocket_accept(ServerSocket self)
{
    int fd;

    Socket conSocket = NULL;

    fd = accept(self->fd, NULL, NULL );

    if (fd >= 0) {
        conSocket = TcpSocket_create();
        conSocket->fd = fd;
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

        // shutdown is required to unblock read or accept in another thread!
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

    free(self);
}

Socket
TcpSocket_create()
{
    Socket self = malloc(sizeof(struct sSocket));

    self->fd = -1;

    return self;
}

int
Socket_connect(Socket self, char* address, int port)
{
    struct sockaddr_in serverAddress;

    if (DEBUG_SOCKET)
        printf("Socket_connect: %s:%i\n", address, port);

    if (!prepareServerAddress(address, port, &serverAddress))
        return 0;

    self->fd = socket(AF_INET, SOCK_STREAM, 0);

#if CONFIG_ACTIVATE_TCP_KEEPALIVE == 1
    activateKeepAlive(self->fd);
#endif

    if (connect(self->fd, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0)
        return 0;
    else
        return 1;
}

char*
Socket_getPeerAddress(Socket self)
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

    char* clientConnection = malloc(strlen(addrString) + 9);


    if (isIPv6)
        sprintf(clientConnection, "[%s]:%i", addrString, port);
    else
        sprintf(clientConnection, "%s:%i", addrString, port);

    return clientConnection;
}

int
Socket_read(Socket self, uint8_t* buf, int size)
{
    assert(self != NULL);

    if (self->fd == -1)
        return -1;

    int read_bytes = read(self->fd, buf, size);

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
    else  {
        return read_bytes;
    }
}

int
Socket_write(Socket self, uint8_t* buf, int size)
{
    if (self->fd == -1)
        return -1;

    // MSG_NOSIGNAL - prevent send to signal SIGPIPE when peer unexpectedly closed the socket
    return send(self->fd, buf, size, 0);
}

void
Socket_destroy(Socket self)
{
    int fd = self->fd;

    self->fd = -1;

    closeAndShutdownSocket(fd);

    Thread_sleep(10);

    free(self);
}
