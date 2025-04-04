/*
 *  socket_hal.h
 *
 *  Copyright 2013-2024 Michael Zillgith
 *
 *  This file is part of Platform Abstraction Layer (libpal)
 *  for libiec61850, libmms, and lib60870.
 */

#ifndef SOCKET_HAL_H_
#define SOCKET_HAL_H_

#include "hal_base.h"

/**
 * \file hal_socket.h
 * \brief Abstraction layer TCP/IP sockets
 * Has to be implemented for CS 104 TCP/IP.
 */

#ifdef __cplusplus
extern "C" {
#endif

/*! \defgroup hal Platform (Hardware/OS) abstraction layer
   *
   *  Platform abstraction layer. These functions have to be implemented when the library is
   *  to be ported to new platforms. It might not be required to implement all interfaces
   *  depending on the required library features.
   *
   *  @{
   */

/**
 * @defgroup HAL_SOCKET Interface to the TCP/IP stack (abstract socket layer)
 *
 *  Thread and Socket abstraction layer. This functions have to be implemented to
 *  port lib60870 to a new hardware/OS platform when TCP/IP is required.
 *
 * @{
 */

/** Opaque reference for a server socket instance */
typedef struct sServerSocket* ServerSocket;

typedef struct sUdpSocket* UdpSocket;

/** Opaque reference for a client or connection socket instance */
typedef struct sSocket* Socket;

/** Opaque reference for a set of server and socket handles */
typedef struct sHandleSet* HandleSet;

/** State of an asynchronous connect */
typedef enum
{
    SOCKET_STATE_CONNECTING = 0,
    SOCKET_STATE_FAILED = 1,
    SOCKET_STATE_CONNECTED = 2
} SocketState;


/**
 * \brief Create a new connection handle set (HandleSet)
 *
 * \return new HandleSet instance
 */
PAL_API HandleSet
Handleset_new(void);

/**
 * \brief Reset the handle set for reuse
 */
PAL_API void
Handleset_reset(HandleSet self);

/**
 * \brief add a socket to an existing handle set
 *
 * \param self the HandleSet instance
 * \param sock the socket to add
 */
PAL_API void
Handleset_addSocket(HandleSet self, const Socket sock);

/**
 * \brief remove a socket from an existing handle set
 */
void
Handleset_removeSocket(HandleSet self, const Socket sock);

/**
 * \brief wait for a socket to become ready
 *
 * This function is corresponding to the BSD socket select function.
 * It returns the number of sockets on which data is pending or 0 if no data is pending
 * on any of the monitored connections. The function will return after "timeout" ms if no
 * data is pending.
 * The function shall return -1 if a socket error occures.
 *
 * \param self the HandleSet instance
 * \param timeout in milliseconds (ms)
 * \return It returns the number of sockets on which data is pending
 *   or 0 if no data is pending on any of the monitored connections.
 *   The function shall return -1 if a socket error occures.
 */
PAL_API int
Handleset_waitReady(HandleSet self, unsigned int timeoutMs);

/**
 * \brief destroy the HandleSet instance
 *
 * \param self the HandleSet instance to destroy
 */
PAL_API void
Handleset_destroy(HandleSet self);

/**
 * \brief Create a new TcpServerSocket instance
 *
 * Implementation of this function is MANDATORY if server functionality is required.
 *
 * \param address ip address or hostname to listen on
 * \param port the TCP port to listen on
 *
 * \return the newly create TcpServerSocket instance
 */
PAL_API ServerSocket
TcpServerSocket_create(const char* address, int port);

/**
 * \brief Create an IPv4 UDP socket instance
 *
 * \return new UDP socket instance
 */
PAL_API UdpSocket
UdpSocket_create(void);

/**
 * \brief Create an IPv6 UDP socket instance
 *
 * \return new UDP socket instance
 */
PAL_API UdpSocket
UdpSocket_createIpV6(void);

/**
 * \brief Add the socket to an IPv4 or IPv6 multicast group
 *
 * \param self UDP socket instance
 * \param multicastAddress IPv4 or IPv6 multicast address
 *
 * \return true on success, false otherwise
 */
PAL_API bool
UdpSocket_addGroupMembership(UdpSocket self, const char* multicastAddress);

/**
 * \brief Sets the multicast TTL (number of hops) for this UDP socket
 *
 * \param self UDP socket instance
 * \param ttl number of hops for multicast messages. Default is 1 (not routable!)
 *
 * \return true on success, false otherwise
 */
PAL_API bool
UdpSocket_setMulticastTtl(UdpSocket self, int ttl);

PAL_API bool
UdpSocket_bind(UdpSocket self, const char* address, int port);

PAL_API bool
UdpSocket_sendTo(UdpSocket self, const char* address, int port, uint8_t* msg, int msgSize);

/**
 * \brief Receive data from UDP socket (store data and (optionally) the IP address of the sender
 *
 * \param self UDP socket instance
 * \param address (optional) buffer to store the IP address as string
 * \param maxAddrSize (optional) size of the provided buffer to store the IP address
 * \param msg buffer to store the UDP message data
 * \param msgSize the maximum size of the message to receive
 *
 * \return number of received bytes or -1 in case of an error
 */
PAL_API int
UdpSocket_receiveFrom(UdpSocket self, char* address, int maxAddrSize, uint8_t* msg, int msgSize);


PAL_API void
ServerSocket_listen(ServerSocket self);

/**
 * \brief accept a new incoming connection (non-blocking)
 *
 * This function shall accept a new incoming connection. It is non-blocking and has to
 * return NULL if no new connection is pending.
 *
 * Implementation of this function is MANDATORY if server functionality is required.
 *
 * NOTE: The behaviour of this function changed with version 0.8!
 *
 * \param self server socket instance
 *
 * \return handle of the new connection socket or NULL if no new connection is available
 */
PAL_API Socket
ServerSocket_accept(ServerSocket self);

/**
 * \brief active TCP keep alive for socket and set keep alive parameters
 *
 * NOTE: implementation is mandatory for IEC 61850 MMS
 *
 * \param self server socket instance
 * \param idleTime time (in s) between last received message and first keep alive message
 * \param interval time (in s) between subsequent keep alive messages if no ACK received
 * \param count number of not missing keep alive ACKs until socket is considered dead
 */
PAL_API void
Socket_activateTcpKeepAlive(Socket self, int idleTime, int interval, int count);

/**
 * \brief set the maximum number of pending connections in the queue
 *
 * Implementation of this function is OPTIONAL.
 *
 * \param self the server socket instance
 * \param backlog the number of pending connections in the queue
 *
 */
PAL_API void
ServerSocket_setBacklog(ServerSocket self, int backlog);

/**
 * \brief destroy a server socket instance
 *
 * Free all resources allocated by this server socket instance.
 *
 * Implementation of this function is MANDATORY if server functionality is required.
 *
 * \param self server socket instance
 */
PAL_API void
ServerSocket_destroy(ServerSocket self);

/**
 * \brief create a TCP client socket
 *
 * Implementation of this function is MANDATORY if client functionality is required.
 *
 * \return a new client socket instance.
 */
PAL_API Socket
TcpSocket_create(void);

/**
 * \brief set the timeout to establish a new connection
 *
 * \param self the client socket instance
 * \param timeoutInMs the timeout in ms
 */
PAL_API void
Socket_setConnectTimeout(Socket self, uint32_t timeoutInMs);

/**
 * \brief bind a socket to a particular IP address and port (for TcpSocket)
 * 
 * NOTE: Don't use the socket when this functions returns false!
 * 
 * \param self the client socket instance
 * \param srcAddress the local IP address or hostname as C string
 * \param srcPort the local TCP port to use. When < 1 the OS will chose the TCP port to use.
 * 
 * \return true in case of success, false otherwise
 */ 
PAL_API bool
Socket_bind(Socket self, const char* srcAddress, int srcPort);

/**
 * \brief connect to a server
 *
 * Connect to a server application identified by the address and port parameter.
 *
 * The "address" parameter may either be a hostname or an IP address. The IP address
 * has to be provided as a C string (e.g. "10.0.2.1").
 *
 * Implementation of this function is MANDATORY if client functionality is required.
 *
 * NOTE: return type changed from int to bool with version 0.8
 *
 * \param self the client socket instance
 * \param address the IP address or hostname as C string
 * \param port the TCP port of the application to connect to
 *
 * \return true if the connection was established successfully, false otherwise
 */
PAL_API bool
Socket_connect(Socket self, const char* address, int port);

PAL_API bool
Socket_connectAsync(Socket self, const char* address, int port);

PAL_API SocketState
Socket_checkAsyncConnectState(Socket self);

/**
 * \brief read from socket to local buffer (non-blocking)
 *
 * The function shall return immediately if no data is available. In this case
 * the function returns 0. If an error happens the function shall return -1.
 *
 * Implementation of this function is MANDATORY
 *
 * NOTE: The behaviour of this function changed with version 0.8!
 *
 * \param self the client, connection or server socket instance
 * \param buf the buffer where the read bytes are copied to
 * \param size the maximum number of bytes to read (size of the provided buffer)
 *
 * \return the number of bytes read or -1 if an error occurred
 */
PAL_API int
Socket_read(Socket self, uint8_t* buf, int size);

/**
 * \brief send a message through the socket
 *
 * Implementation of this function is MANDATORY
 *
 * \param self client, connection or server socket instance
 *
 * \return number of bytes transmitted of -1 in case of an error
 */
PAL_API int
Socket_write(Socket self, uint8_t* buf, int size);

PAL_API char*
Socket_getLocalAddress(Socket self);

/**
 * \brief Get the address of the peer application (IP address and port number)
 *
 * The peer address has to be returned as null terminated string
 *
 * Implementation of this function is MANDATORY (libiec61850)
 *
 * \param self the client, connection or server socket instance
 *
 * \return the IP address and port number as strings separated by the ':' character.
 */
PAL_API char*
Socket_getPeerAddress(Socket self);

/**
 * \brief Get the address of the peer application (IP address and port number)
 *
 * The peer address has to be returned as null terminated string
 *
 * Implementation of this function is MANDATORY (lib60870 and libiec61850)
 *
 * \param self the client, connection or server socket instance
 * \param peerAddressString a string to store the peer address (the string should have space
 *        for at least 60 characters)
 *
 * \return the IP address and port number as strings separated by the ':' character. If the
 *         address is an IPv6 address the IP part is encapsulated in square brackets.
 */
PAL_API char*
Socket_getPeerAddressStatic(Socket self, char* peerAddressString);

/**
 * \brief destroy a socket (close the socket if a connection is established)
 *
 * This function shall close the connection (if one is established) and free all
 * resources allocated by the socket.
 *
 * Implementation of this function is MANDATORY
 *
 * \param self the client, connection or server socket instance
 */
PAL_API void
Socket_destroy(Socket self);

/*! @} */

/*! @} */

#ifdef __cplusplus
}
#endif

#endif /* SOCKET_HAL_H_ */
