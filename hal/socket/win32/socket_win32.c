/*
 *  socket_win32.c
 *
 *  Copyright 2013-2024 Michael Zillgith
 *
 *  This file is part of Platform Abstraction Layer (libpal)
 *  for libiec61850, libmms, and lib60870.
 */

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <stdbool.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

#include "hal_socket.h"
#include "lib_memory.h"

#ifndef DEBUG_SOCKET
#define DEBUG_SOCKET 0
#endif

#ifndef __MINGW64_VERSION_MAJOR
struct tcp_keepalive
{
    u_long onoff;
    u_long keepalivetime;
    u_long keepaliveinterval;
};
#endif

#define SIO_KEEPALIVE_VALS _WSAIOW(IOC_VENDOR, 4)

struct sSocket
{
    SOCKET fd;
    uint32_t connectTimeout;
};

struct sServerSocket
{
    SOCKET fd;
    int backLog;
};

struct sHandleSet
{
    fd_set handles;
    SOCKET maxHandle;
};

struct sUdpSocket
{
    SOCKET fd;
    int ns; /* IPv4: AF_INET; IPv6: AF_INET6 */
};

HandleSet
Handleset_new(void)
{
    HandleSet result = (HandleSet)GLOBAL_MALLOC(sizeof(struct sHandleSet));

    if (result != NULL)
    {
        FD_ZERO(&result->handles);
        result->maxHandle = INVALID_SOCKET;
    }

    return result;
}

void
Handleset_reset(HandleSet self)
{
    FD_ZERO(&self->handles);
    self->maxHandle = INVALID_SOCKET;
}

void
Handleset_addSocket(HandleSet self, const Socket sock)
{
    if (self != NULL && sock != NULL && sock->fd != INVALID_SOCKET)
    {

        FD_SET(sock->fd, &self->handles);

        if ((sock->fd > self->maxHandle) || (self->maxHandle == INVALID_SOCKET))
            self->maxHandle = sock->fd;
    }
}

void
Handleset_removeSocket(HandleSet self, const Socket sock)
{
    if (self != NULL && sock != NULL && sock->fd != INVALID_SOCKET)
    {
        FD_CLR(sock->fd, &self->handles);
    }
}

int
Handleset_waitReady(HandleSet self, unsigned int timeoutMs)
{
    int result;

    if ((self != NULL) && (self->maxHandle != INVALID_SOCKET))
    {
        struct timeval timeout;

        timeout.tv_sec = timeoutMs / 1000;
        timeout.tv_usec = (timeoutMs % 1000) * 1000;

        fd_set handles;

        memcpy((void*)&handles, &(self->handles), sizeof(fd_set));

        result = select(self->maxHandle + 1, &handles, NULL, NULL, &timeout);
    }
    else
    {
        result = -1;
    }

    return result;
}

void
Handleset_destroy(HandleSet self)
{
    GLOBAL_FREEMEM(self);
}

static bool wsaStartupCalled = false;
static int socketCount = 0;

void
Socket_activateTcpKeepAlive(Socket self, int idleTime, int interval, int count)
{
    (void)count; /* not supported in windows socket API */

    struct tcp_keepalive keepalive;
    DWORD retVal = 0;

    keepalive.onoff = 1;
    keepalive.keepalivetime = idleTime * 1000;
    keepalive.keepaliveinterval = interval * 1000;

    if (WSAIoctl(self->fd, SIO_KEEPALIVE_VALS, &keepalive, sizeof(keepalive), NULL, 0, &retVal, NULL, NULL) ==
        SOCKET_ERROR)
    {
        if (DEBUG_SOCKET)
            printf("WIN32_SOCKET: WSAIotcl(SIO_KEEPALIVE_VALS) failed: %d\n", WSAGetLastError());
    }
}

static void
setSocketNonBlocking(Socket self)
{
    unsigned long mode = 1;
    if (ioctlsocket(self->fd, FIONBIO, &mode) != 0)
    {
        if (DEBUG_SOCKET)
            printf("WIN32_SOCKET: failed to set socket non-blocking!\n");
    }

    /* activate TCP_NODELAY */

    int tcpNoDelay = 1;

    setsockopt(self->fd, IPPROTO_TCP, TCP_NODELAY, (const char*)&tcpNoDelay, sizeof(int));
}

static bool
prepareAddress(const char* address, int port, struct sockaddr_in* sockaddr)
{
    memset((char*)sockaddr, 0, sizeof(struct sockaddr_in));

    if (address != NULL)
    {
        struct hostent* server;
        server = gethostbyname(address);

        if (server == NULL)
            return false;

        memcpy((char*)&sockaddr->sin_addr.s_addr, (char*)server->h_addr, server->h_length);
    }
    else
        sockaddr->sin_addr.s_addr = htonl(INADDR_ANY);

    sockaddr->sin_family = AF_INET;
    sockaddr->sin_port = htons(port);

    return true;
}

static bool
wsaStartUp(void)
{
    if (wsaStartupCalled == false)
    {
        int ec;
        WSADATA wsa;

        if ((ec = WSAStartup(MAKEWORD(2, 0), &wsa)) != 0)
        {
            if (DEBUG_SOCKET)
                printf("WIN32_SOCKET: winsock error: code %i\n", ec);
            return false;
        }
        else
        {
            wsaStartupCalled = true;
            return true;
        }
    }
    else
        return true;
}

static void
wsaShutdown(void)
{
    if (wsaStartupCalled)
    {
        if (socketCount == 0)
        {
            WSACleanup();
            wsaStartupCalled = false;
        }
    }
}

ServerSocket
TcpServerSocket_create(const char* address, int port)
{
    ServerSocket serverSocket = NULL;
    int ec;
    SOCKET listen_socket = INVALID_SOCKET;

    if (wsaStartUp() == false)
        return NULL;

    struct sockaddr_in server_addr;

    if (!prepareAddress(address, port, &server_addr))
        return NULL;

    listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (listen_socket == INVALID_SOCKET)
    {
        if (DEBUG_SOCKET)
            printf("WIN32_SOCKET: socket failed with error: %i\n", WSAGetLastError());

        wsaShutdown();

        return NULL;
    }

    int optionReuseAddr = 1;
    setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&optionReuseAddr, sizeof(int));

    ec = bind(listen_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

    if (ec == SOCKET_ERROR)
    {
        if (DEBUG_SOCKET)
            printf("WIN32_SOCKET: bind failed with error:%i\n", WSAGetLastError());
        closesocket(listen_socket);

        wsaShutdown();

        return NULL;
    }

    serverSocket = (ServerSocket)GLOBAL_MALLOC(sizeof(struct sServerSocket));

    if (serverSocket)
    {
        serverSocket->fd = listen_socket;
        serverSocket->backLog = 10;

        setSocketNonBlocking((Socket)serverSocket);

        socketCount++;
    }
    else
    {
        closesocket(listen_socket);
        wsaShutdown();
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
    Socket conSocket = NULL;

    SOCKET fd = accept(self->fd, NULL, NULL);

    if (fd != INVALID_SOCKET)
    {
        conSocket = (Socket)GLOBAL_CALLOC(1, sizeof(struct sSocket));
        conSocket->fd = fd;

        socketCount++;

        setSocketNonBlocking(conSocket);

        if (DEBUG_SOCKET)
            printf("WIN32_SOCKET: connection accepted\n");
    }
    else
    {
        if (DEBUG_SOCKET)
            printf("WIN32_SOCKET: accept failed\n");
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
    if (self->fd != INVALID_SOCKET)
    {
        shutdown(self->fd, 2);
        closesocket(self->fd);
        socketCount--;
        self->fd = INVALID_SOCKET;
    }

    wsaShutdown();
    GLOBAL_FREEMEM(self);
}

Socket
TcpSocket_create()
{
    Socket self = NULL;

    if (wsaStartUp() == false)
        return NULL;

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock != INVALID_SOCKET)
    {
        self = (Socket)GLOBAL_MALLOC(sizeof(struct sSocket));

        if (self)
        {
            self->fd = sock;
            self->connectTimeout = 5000;

            socketCount++;
        }
        else
        {
            if (DEBUG_SOCKET)
                printf("SOCKET: failed to create socket - cannot allocate memory\n");

            closesocket(sock);
            wsaShutdown();
        }
    }
    else
    {
        if (DEBUG_SOCKET)
            printf("SOCKET: failed to create socket (error code=%i)\n", WSAGetLastError());
    }

    return self;
}

void
Socket_setConnectTimeout(Socket self, uint32_t timeoutInMs)
{
    self->connectTimeout = timeoutInMs;
}

bool
Socket_bind(Socket self, const char* srcAddress, int srcPort)
{
    struct sockaddr_in localAddress;

    if (!prepareAddress(srcAddress, srcPort, &localAddress))
        return false;

    int result = bind(self->fd, (struct sockaddr*)&localAddress, sizeof(localAddress));

    if (result == SOCKET_ERROR)
    {
        if (DEBUG_SOCKET)
            printf("SOCKET: failed to bind TCP socket (errno=%i)\n", WSAGetLastError());

        closesocket(self->fd);
        self->fd = -1;

        return false;
    }

    return true;
}

bool
Socket_connectAsync(Socket self, const char* address, int port)
{
    if (DEBUG_SOCKET)
        printf("WIN32_SOCKET: Socket_connect: %s:%i\n", address, port);

    struct sockaddr_in serverAddress;

    if (wsaStartUp() == false)
        return false;

    if (!prepareAddress(address, port, &serverAddress))
        return false;

    setSocketNonBlocking(self);

    if (connect(self->fd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
    {
        if (WSAGetLastError() != WSAEWOULDBLOCK)
        {
            closesocket(self->fd);
            self->fd = INVALID_SOCKET;
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

    int selectVal = select(self->fd + 1, NULL, &fdSet, NULL, &timeout);

    if (selectVal == 1)
    {

        /* Check if connection is established */

        int so_error;
        int len = sizeof so_error;

        if (getsockopt(self->fd, SOL_SOCKET, SO_ERROR, (char*)(&so_error), &len) != SOCKET_ERROR)
        {
            if (so_error == 0)
            {

                int recvRes = recv(self->fd, NULL, 0, 0);

                if (recvRes == SOCKET_ERROR)
                {
                    int wsaError = WSAGetLastError();

                    if (wsaError == WSAECONNRESET)
                        return SOCKET_STATE_FAILED;

                    if (wsaError == WSAECONNABORTED)
                        return SOCKET_STATE_FAILED;
                }

                return SOCKET_STATE_CONNECTED;
            }
        }

        return SOCKET_STATE_FAILED;
    }
    else if (selectVal == 0)
    {
        return SOCKET_STATE_CONNECTING;
    }
    else
    {
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

    if (select(self->fd + 1, NULL, &fdSet, NULL, &timeout) == 1)
    {

        /* Check if connection is established */

        int so_error;
        socklen_t len = sizeof so_error;

        if (getsockopt(self->fd, SOL_SOCKET, SO_ERROR, (char*)&so_error, &len) >= 0)
        {

            if (so_error == 0)
                return true;
        }
    }

    closesocket(self->fd);
    self->fd = INVALID_SOCKET;

    return false;
}

static char*
convertAddressToStr(struct sockaddr_storage* addr)
{
    char addrString[INET6_ADDRSTRLEN + 7];
    int addrStringLen = INET6_ADDRSTRLEN + 7;
    int port;

    bool isIPv6;

    if (addr->ss_family == AF_INET)
    {
        struct sockaddr_in* ipv4Addr = (struct sockaddr_in*)addr;
        port = ntohs(ipv4Addr->sin_port);
        ipv4Addr->sin_port = 0;
        WSAAddressToString((LPSOCKADDR)ipv4Addr, sizeof(struct sockaddr_storage), NULL, (LPSTR)addrString,
                           (LPDWORD)&addrStringLen);
        isIPv6 = false;
    }
    else if (addr->ss_family == AF_INET6)
    {
        struct sockaddr_in6* ipv6Addr = (struct sockaddr_in6*)addr;
        port = ntohs(ipv6Addr->sin6_port);
        ipv6Addr->sin6_port = 0;
        WSAAddressToString((LPSOCKADDR)ipv6Addr, sizeof(struct sockaddr_storage), NULL, (LPSTR)addrString,
                           (LPDWORD)&addrStringLen);
        isIPv6 = true;
    }
    else
        return NULL;

    char* clientConnection = (char*)GLOBAL_MALLOC(strlen(addrString) + 9);

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

    if (getpeername(self->fd, (struct sockaddr*)&addr, &addrLen) == 0)
    {
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

    if (getsockname(self->fd, (struct sockaddr*)&addr, &addrLen) == 0)
    {
        return convertAddressToStr(&addr);
    }
    else
        return NULL;
}

char*
Socket_getPeerAddressStatic(Socket self, char* peerAddressString)
{
    struct sockaddr_storage addr;
    int addrLen = sizeof(addr);

    getpeername(self->fd, (struct sockaddr*)&addr, &addrLen);

    char addrString[INET6_ADDRSTRLEN + 7];
    int addrStringLen = INET6_ADDRSTRLEN + 7;
    int port;

    bool isIPv6;

    if (addr.ss_family == AF_INET)
    {
        struct sockaddr_in* ipv4Addr = (struct sockaddr_in*)&addr;
        port = ntohs(ipv4Addr->sin_port);
        ipv4Addr->sin_port = 0;
        WSAAddressToString((LPSOCKADDR)ipv4Addr, sizeof(struct sockaddr_storage), NULL, (LPSTR)addrString,
                           (LPDWORD)&addrStringLen);
        isIPv6 = false;
    }
    else if (addr.ss_family == AF_INET6)
    {
        struct sockaddr_in6* ipv6Addr = (struct sockaddr_in6*)&addr;
        port = ntohs(ipv6Addr->sin6_port);
        ipv6Addr->sin6_port = 0;
        WSAAddressToString((LPSOCKADDR)ipv6Addr, sizeof(struct sockaddr_storage), NULL, (LPSTR)addrString,
                           (LPDWORD)&addrStringLen);
        isIPv6 = true;
    }
    else
        return NULL;

    if (isIPv6)
        sprintf(peerAddressString, "[%s]:%i", addrString, port);
    else
        sprintf(peerAddressString, "%s:%i", addrString, port);

    return peerAddressString;
}

int
Socket_read(Socket self, uint8_t* buf, int size)
{
    int bytes_read = recv(self->fd, (char*)buf, size, 0);

    if (bytes_read == 0) /* peer has closed socket */
        return -1;

    if (bytes_read == SOCKET_ERROR)
    {
        if (WSAGetLastError() == WSAEWOULDBLOCK)
            return 0;
        else
            return -1;
    }

    return bytes_read;
}

int
Socket_write(Socket self, uint8_t* buf, int size)
{
    int bytes_sent = send(self->fd, (char*)buf, size, 0);

    if (bytes_sent == SOCKET_ERROR)
    {
        int errorCode = WSAGetLastError();

        if (errorCode == WSAEWOULDBLOCK)
            bytes_sent = 0;
        else
            bytes_sent = -1;
    }

    return bytes_sent;
}

void
Socket_destroy(Socket self)
{
    if (self->fd != INVALID_SOCKET)
    {
        shutdown(self->fd, 2);
        closesocket(self->fd);

        self->fd = INVALID_SOCKET;

        socketCount--;
    }

    wsaShutdown();

    GLOBAL_FREEMEM(self);
}

static UdpSocket
UdpSocket_createUsingNamespace(int ns)
{
    if (wsaStartUp() == false)
        return NULL;

    UdpSocket self = NULL;

    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);

    if (sock != INVALID_SOCKET)
    {
        self = (UdpSocket)GLOBAL_MALLOC(sizeof(struct sSocket));

        if (self)
        {
            self->fd = sock;
            self->ns = ns;
        }
        else
        {
            if (DEBUG_SOCKET)
                printf("SOCKET: failed to allocate memory\n");

            closesocket(sock);
        }
    }
    else
    {
        if (DEBUG_SOCKET)
            printf("SOCKET: failed to create UDP socket (errno=%i)\n", WSAGetLastError());
    }

    return self;
}

UdpSocket
UdpSocket_create()
{
    return UdpSocket_createUsingNamespace(AF_INET);
}

UdpSocket
UdpSocket_createIpV6()
{
    return UdpSocket_createUsingNamespace(AF_INET6);
}

bool
UdpSocket_addGroupMembership(UdpSocket self, const char* multicastAddress)
{
    if (self->ns == AF_INET)
    {
        struct ip_mreq mreq;

        if (inet_pton(AF_INET, multicastAddress, &(mreq.imr_multiaddr)) < 1)
        {
            printf("SOCKET: Invalid IPv4 multicast address\n");
            return false;
        }
        else
        {
            mreq.imr_interface.s_addr = htonl(INADDR_ANY);

            if (setsockopt(self->fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (const char*)&mreq, sizeof(mreq)) == -1)
            {
                printf("SOCKET: failed to set IPv4 multicast group (errno: %i)\n", WSAGetLastError());
                return false;
            }
        }

        return true;
    }
    else if (self->ns == AF_INET6)
    {
        struct ipv6_mreq mreq;

        if (inet_pton(AF_INET6, multicastAddress, &(mreq.ipv6mr_multiaddr)) < 1)
        {
            printf("SOCKET: failed to set IPv6 multicast group (errno: %i)\n", WSAGetLastError());
            return false;
        }

        mreq.ipv6mr_interface = 0;

        if (setsockopt(self->fd, IPPROTO_IPV6, IPV6_ADD_MEMBERSHIP, (const char*)&mreq, sizeof(mreq)) == -1)
        {
            printf("SOCKET: failed to set IPv6 multicast group (errno: %i)\n", WSAGetLastError());
            return false;
        }

        return true;
    }

    return false;
}

bool
UdpSocket_setMulticastTtl(UdpSocket self, int ttl)
{
    if (self->ns == AF_INET)
    {
        if (setsockopt(self->fd, IPPROTO_IP, IP_MULTICAST_TTL, (const char*)&ttl, sizeof(ttl)) == -1)
        {
            printf("SOCKET: failed to set IPv4 multicast TTL (errno: %i)\n", WSAGetLastError());
            return false;
        }

        return true;
    }
    else if (self->ns == AF_INET6)
    {
        if (setsockopt(self->fd, IPPROTO_IPV6, IPV6_MULTICAST_HOPS, (const char*)&ttl, sizeof(ttl)) == -1)
        {
            printf("SOCKET: failed to set IPv6 multicast TTL(hops) (errno: %i)\n", WSAGetLastError());
            return false;
        }

        return true;
    }

    return false;
}

bool
UdpSocket_bind(UdpSocket self, const char* address, int port)
{
    // TODO add support for IPv6
    struct sockaddr_in localAddress;

    if (!prepareAddress(address, port, &localAddress))
    {
        closesocket(self->fd);
        self->fd = 0;
        return false;
    }

    int result = bind(self->fd, (struct sockaddr*)&localAddress, sizeof(localAddress));

    if (result == -1)
    {
        if (DEBUG_SOCKET)
            printf("SOCKET: failed to bind UDP socket (errno=%i)\n", errno);

        closesocket(self->fd);
        self->fd = 0;

        return false;
    }

    return true;
}

bool
UdpSocket_sendTo(UdpSocket self, const char* address, int port, uint8_t* msg, int msgSize)
{
    // TODO add support for IPv6
    struct sockaddr_in remoteAddress;

    if (!prepareAddress(address, port, &remoteAddress))
    {

        if (DEBUG_SOCKET)
            printf("SOCKET: failed to lookup remote address %s\n", address);

        return false;
    }

    int result =
        sendto(self->fd, (const char*)msg, msgSize, 0, (struct sockaddr*)&remoteAddress, sizeof(remoteAddress));

    if (result == msgSize)
    {
        return true;
    }
    else if (result == -1)
    {
        if (DEBUG_SOCKET)
            printf("SOCKET: failed to send UDP message (errno=%i)\n", errno);
    }
    else
    {
        if (DEBUG_SOCKET)
            printf("SOCKET: failed to send UDP message (insufficient data sent)\n");
    }

    return false;
}

int
UdpSocket_receiveFrom(UdpSocket self, char* address, int maxAddrSize, uint8_t* msg, int msgSize)
{
    // TODO add support for IPv6
    struct sockaddr_storage remoteAddress;
    memset(&remoteAddress, 0, sizeof(struct sockaddr_storage));
    socklen_t structSize = sizeof(struct sockaddr_storage);

    if (address)
        address[0] = 0;

    int result = recvfrom(self->fd, (char*)msg, msgSize, 0, (struct sockaddr*)&remoteAddress, &structSize);

    if (result == 0) /* peer has closed socket */
        return -1;

    if (result == SOCKET_ERROR)
    {
        if (WSAGetLastError() == WSAEWOULDBLOCK)
            return 0;
        else
            return -1;
    }

    if (address)
    {
        bool isIPv6;
        char addrString[INET6_ADDRSTRLEN + 7];
        int port;

        if (remoteAddress.ss_family == AF_INET)
        {
            struct sockaddr_in* ipv4Addr = (struct sockaddr_in*)&remoteAddress;
            port = ntohs(ipv4Addr->sin_port);
            inet_ntop(AF_INET, &(ipv4Addr->sin_addr), addrString, INET_ADDRSTRLEN);
            isIPv6 = false;
        }
        else if (remoteAddress.ss_family == AF_INET6)
        {
            struct sockaddr_in6* ipv6Addr = (struct sockaddr_in6*)&remoteAddress;
            port = ntohs(ipv6Addr->sin6_port);
            inet_ntop(AF_INET6, &(ipv6Addr->sin6_addr), addrString, INET6_ADDRSTRLEN);
            isIPv6 = true;
        }
        else
            return result;

        if (isIPv6)
            snprintf(address, maxAddrSize, "[%s]:%i", addrString, port);
        else
            snprintf(address, maxAddrSize, "%s:%i", addrString, port);
    }

    return result;
}
