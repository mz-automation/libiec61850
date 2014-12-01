/*
 *  socket_win32.c
 *
 *  Copyright 2013 Michael Zillgith
 *
 *	This file is part of libIEC61850.
 *
 *	libIEC61850 is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	libIEC61850 is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with libIEC61850.  If not, see <http://www.gnu.org/licenses/>.
 *
 *	See COPYING file for the complete license text.
 */

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdbool.h>
#include <windows.h>

#pragma comment (lib, "Ws2_32.lib")

#include "socket.h"
#include "stack_config.h"


#ifndef __MINGW64_VERSION_MAJOR
struct tcp_keepalive {
    u_long  onoff;
    u_long  keepalivetime;
    u_long  keepaliveinterval;
};
#endif

#define SIO_KEEPALIVE_VALS    _WSAIOW(IOC_VENDOR,4)

struct sSocket {
	SOCKET fd;
};

struct sServerSocket {
	SOCKET fd;
	int backLog;
};

static void
activateKeepAlive(SOCKET s)
{
	struct tcp_keepalive keepalive;
	DWORD retVal=0;

	keepalive.onoff = 1;
	keepalive.keepalivetime = CONFIG_TCP_KEEPALIVE_IDLE * 1000;
	keepalive.keepaliveinterval = CONFIG_TCP_KEEPALIVE_INTERVAL * 1000;

	 if (WSAIoctl(s, SIO_KEEPALIVE_VALS, &keepalive, sizeof(keepalive),
	            NULL, 0, &retVal, NULL, NULL) == SOCKET_ERROR)
	 {
	        printf("WSAIotcl(SIO_KEEPALIVE_VALS) failed; %d\n",
	            WSAGetLastError());
	 }
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

ServerSocket
TcpServerSocket_create(char* address, int port)
{
	ServerSocket serverSocket = NULL;
	int ec;
	WSADATA wsa;
	SOCKET listen_socket = INVALID_SOCKET;

	if ((ec = WSAStartup(MAKEWORD(2,0), &wsa)) != 0) {
		printf("winsock error: code %i\n");
		return NULL;
	}

	struct sockaddr_in server_addr;

	if (!prepareServerAddress(address, port, &server_addr))
	    return NULL;

	listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

#if CONFIG_ACTIVATE_TCP_KEEPALIVE == 1
    activateKeepAlive(listen_socket);
#endif

	if (listen_socket == INVALID_SOCKET) {
		printf("socket failed with error: %i\n", WSAGetLastError());
		WSACleanup();
		return NULL;
	}

	int optionReuseAddr = 1;
	setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&optionReuseAddr, sizeof(int));

	ec = bind(listen_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

	if (ec == SOCKET_ERROR) {
		printf("bind failed with error:%i\n", WSAGetLastError());
		closesocket(listen_socket);
		WSACleanup();
		return NULL;
	}

	serverSocket = (ServerSocket) malloc(sizeof(struct sServerSocket));

	serverSocket->fd = listen_socket;
	serverSocket->backLog = 10;

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

	fd = accept(self->fd, NULL, NULL);

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

void
ServerSocket_destroy(ServerSocket self)
{
	closesocket(self->fd);
	WSACleanup();
	free(self);
}

Socket
TcpSocket_create()
{
	Socket self = (Socket) malloc(sizeof(struct sSocket));

	self->fd = -1;

	return self;
}

int
Socket_connect(Socket self, char* address, int port)
{
	struct hostent *server;
	struct sockaddr_in serverAddress;
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2,0), &wsa) != 0) {
		printf("winsock error: code %i\n");
		return 0;
	}

	if (!prepareServerAddress(address, port, &serverAddress))
	    return 0;

	self->fd = socket(AF_INET, SOCK_STREAM, 0);

#if CONFIG_ACTIVATE_TCP_KEEPALIVE == 1
    activateKeepAlive(self->fd);
#endif

	if (connect(self->fd, (struct sockaddr *) &serverAddress,sizeof(serverAddress)) < 0) {
		printf("Socket failed connecting!\n");
		return 0;
	}
	else
		return 1;
}

char*
Socket_getPeerAddress(Socket self)
{
	struct sockaddr_storage addr;
	int addrLen = sizeof(addr);

	getpeername(self->fd, (struct sockaddr*) &addr, &addrLen);

	char addrString[INET6_ADDRSTRLEN + 7];
	int addrStringLen = INET6_ADDRSTRLEN + 7;
	int port;

	bool isIPv6;

	if (addr.ss_family == AF_INET)  {
		struct sockaddr_in* ipv4Addr = (struct sockaddr_in*) &addr;
		port = ntohs(ipv4Addr->sin_port);
		ipv4Addr->sin_port = 0;
		WSAAddressToString((LPSOCKADDR) ipv4Addr, sizeof(struct sockaddr_storage), NULL, 
			(LPSTR) addrString, (LPDWORD) &addrStringLen);
		isIPv6 = false;
	}
	else if (addr.ss_family == AF_INET6){
		struct sockaddr_in6* ipv6Addr = (struct sockaddr_in6*) &addr;
		port = ntohs(ipv6Addr->sin6_port);
		ipv6Addr->sin6_port = 0;
		WSAAddressToString((LPSOCKADDR) ipv6Addr, sizeof(struct sockaddr_storage), NULL, 
			(LPSTR) addrString, (LPDWORD) &addrStringLen);
		isIPv6 = true;
	}
	else
		return NULL;

	char* clientConnection = (char*) malloc(strlen(addrString) + 9);

    if (isIPv6)
        sprintf(clientConnection, "[%s]:%i", addrString, port);
    else
        sprintf(clientConnection, "%s:%i", addrString, port);

	return clientConnection;
}

int
Socket_read(Socket self, uint8_t* buf, int size)
{
	return recv(self->fd, (char*) buf, size, 0);
}

int
Socket_write(Socket self, uint8_t* buf, int size)
{
	return send(self->fd, (char*) buf, size, 0);
}

void
Socket_destroy(Socket self)
{
	if (self->fd != -1) {
		closesocket(self->fd);
	}

	free(self);
}
