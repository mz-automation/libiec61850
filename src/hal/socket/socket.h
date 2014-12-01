/*
 *  socket.h
 *
 *  Copyright 2013 Michael Zillgith
 *
 *	This file is part of libIEC61850.
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

#ifndef SOCKET_H_
#define SOCKET_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup hal Hardware/OS abstraction layer
   *
   *  @{
   */

/**
 * @defgroup HAL_SOCKET Interface to the TCP/IP stack (abstract socket layer)
 *
 *  Thread and Socket abstraction layer. This functions have to be implemented to
 *  port libIEC61850 to a new hardware/OS platform.
 *
 * @{
 */

/** Opaque reference for a server socket instance */
typedef struct sServerSocket* ServerSocket;

/** Opaque reference for a client or connection socket instance */
typedef struct sSocket* Socket;

/**
 * Create a new TcpServerSocket instance
 *
 * \param address ip address or hostname to listen on
 * \param port the TCP port to listen on
 *
 * \return the newly create TcpServerSocket instance
 */
ServerSocket
TcpServerSocket_create(char* address, int port);


void
ServerSocket_listen(ServerSocket self);

Socket
ServerSocket_accept(ServerSocket self);

void
ServerSocket_setBacklog(ServerSocket self, int backlog);

void
ServerSocket_destroy(ServerSocket self);

Socket
TcpSocket_create(void);


int
Socket_connect(Socket self, char* address, int port);

int
Socket_read(Socket self, uint8_t* buf, int size);

int
Socket_write(Socket self, uint8_t* buf, int size);

char*
Socket_getPeerAddress(Socket self);

void
Socket_destroy(Socket self);

/*! @} */

/*! @} */

#ifdef __cplusplus
}
#endif

#endif /* SOCKET_H_ */
