/*
 *  ethernet.h
 *
 *  Copyright 2013 Michael Zillgith
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

#ifndef ETHERNET_H_
#define ETHERNET_H_

#include <stdint.h>

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
 * Opaque handle that represents an Ethernet "socket".
 */
typedef struct sEthernetSocket* EthernetSocket;

/**
 * Return the MAC address of an Ethernet interface.
 *
 * The result are the six bytes that make up the Ethernet MAC address.
 *
 * \param interfaceId the ID of the Ethernet interface
 * \param addr pointer to a buffer to store the MAC address
 */
void
Ethernet_getInterfaceMACAddress(char* interfaceId, uint8_t* addr);

/**
 * Create an Ethernet socket using the specified interface and
 * destination MAC address.
 *
 * \param interfaceId the ID of the Ethernet interface
 * \param destAddress byte array that contains the Ethernet MAC address
 */
EthernetSocket
Ethernet_createSocket(char* interfaceId, uint8_t* destAddress);

void
Ethernet_destroySocket(EthernetSocket ethSocket);

void
Ethernet_sendPacket(EthernetSocket ethSocket, uint8_t* buffer, int packetSize);

void
Ethernet_setProtocolFilter(EthernetSocket ethSocket, uint16_t etherType);

int
Ethernet_receivePacket(EthernetSocket self, uint8_t* buffer, int bufferSize);

/*! @} */

/*! @} */

#ifdef __cplusplus
}
#endif

#endif /* ETHERNET_H_ */
