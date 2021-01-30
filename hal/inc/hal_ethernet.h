/*
 *  ethernet_hal.h
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

#ifndef ETHERNET_HAL_H_
#define ETHERNET_HAL_H_

#include "hal_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup hal
   *
   *  @{
   */

/**
 * @defgroup HAL_ETHERNET Direct access to the ethernet layer (optional - required by GOOSE and Sampled Values)
 *
 * @{
 */


/**
 * \brief Opaque handle that represents an Ethernet "socket".
 */
typedef struct sEthernetSocket* EthernetSocket;

/** Opaque reference for a set of ethernet socket handles */
typedef struct sEthernetHandleSet* EthernetHandleSet;

/**
 * \brief Create a new connection handle set (EthernetHandleSet)
 *
 * \return new EthernetHandleSet instance
 */
PAL_API EthernetHandleSet
EthernetHandleSet_new(void);

/**
 * \brief add a socket to an existing handle set
 *
 * \param self the HandleSet instance
 * \param sock the socket to add
 */
PAL_API void
EthernetHandleSet_addSocket(EthernetHandleSet self, const EthernetSocket sock);

/**
 * \brief remove a socket from an existing handle set
 *
 * \param self the HandleSet instance
 * \param sock the socket to add
 */
PAL_API void
EthernetHandleSet_removeSocket(EthernetHandleSet self, const EthernetSocket sock);

/**
 * \brief wait for a socket to become ready
 *
 * This function is corresponding to the BSD socket select function.
 * The function will return after \p timeoutMs ms if no data is pending.
 *
 * \param self the HandleSet instance
 * \param timeoutMs in milliseconds (ms)
 * \return It returns the number of sockets on which data is pending
 *   or 0 if no data is pending on any of the monitored connections.
 *   The function shall return -1 if a socket error occures.
 */
PAL_API int
EthernetHandleSet_waitReady(EthernetHandleSet self, unsigned int timeoutMs);

/**
 * \brief destroy the EthernetHandleSet instance
 *
 * \param self the HandleSet instance to destroy
 */
PAL_API void
EthernetHandleSet_destroy(EthernetHandleSet self);

/**
 * \brief Return the MAC address of an Ethernet interface.
 *
 * The result are the six bytes that make up the Ethernet MAC address.
 *
 * \param interfaceId the ID of the Ethernet interface
 * \param addr pointer to a buffer to store the MAC address
 */
PAL_API void
Ethernet_getInterfaceMACAddress(const char* interfaceId, uint8_t* addr);

/**
 * \brief Create an Ethernet socket using the specified interface and
 * destination MAC address.
 *
 * \param interfaceId the ID of the Ethernet interface
 * \param destAddress byte array that contains the Ethernet MAC address
 */
PAL_API EthernetSocket
Ethernet_createSocket(const char* interfaceId, uint8_t* destAddress);

/**
 * \brief destroy the ethernet socket
 *
 * \param ethSocket the ethernet socket handle
 */
PAL_API void
Ethernet_destroySocket(EthernetSocket ethSocket);

PAL_API void
Ethernet_sendPacket(EthernetSocket ethSocket, uint8_t* buffer, int packetSize);

/*
 * \brief set a protocol filter for the specified etherType
 *
 * \param ethSocket the ethernet socket handle
 * \param etherType the ether type of messages to accept
 */
PAL_API void
Ethernet_setProtocolFilter(EthernetSocket ethSocket, uint16_t etherType);

/**
 * \brief receive an ethernet packet (non-blocking)
 *
 * \param ethSocket the ethernet socket handle
 * \param buffer the buffer to copy the message to
 * \param bufferSize the maximum size of the buffer
 *
 * \return size of message received in bytes
 */
PAL_API int
Ethernet_receivePacket(EthernetSocket ethSocket, uint8_t* buffer, int bufferSize);

/**
 * \brief Indicates if runtime provides support for direct Ethernet access
 *
 * \return true if Ethernet support is available, false otherwise
 */
PAL_API bool
Ethernet_isSupported(void);

/*! @} */

/*! @} */

#ifdef __cplusplus
}
#endif

#endif /* ETHERNET_HAL_H_ */
