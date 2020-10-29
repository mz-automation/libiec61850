/*
 *  iso_client_connection.h
 *
 *  This is an internal interface of MMS client connection that connects MMS to the ISO client
 *  protocol stack. It is used as an abstraction layer to isolate the MMS code from the lower
 *  protocol layers.
 *
 *  Copyright 2013-2018 Michael Zillgith
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

#ifndef ISO_CLIENT_CONNECTION_H_
#define ISO_CLIENT_CONNECTION_H_

#include "byte_buffer.h"
#include "iso_connection_parameters.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    ISO_IND_ASSOCIATION_SUCCESS,
    ISO_IND_ASSOCIATION_FAILED,
    ISO_IND_CLOSED,
    ISO_IND_DATA,
    ISO_IND_TICK
} IsoIndication;

typedef bool
(*IsoIndicationCallback)(IsoIndication indication, void* param, ByteBuffer* payload);

/**
 * opaque data structure to represent an ISO client connection.
 */
typedef struct sIsoClientConnection* IsoClientConnection;

LIB61850_INTERNAL IsoClientConnection
IsoClientConnection_create(IsoConnectionParameters parameters, IsoIndicationCallback callback, void* callbackParameter);

LIB61850_INTERNAL void
IsoClientConnection_destroy(IsoClientConnection self);

LIB61850_INTERNAL bool
IsoClientConnection_associateAsync(IsoClientConnection self, uint32_t connectTimeoutInMs, uint32_t readTimeoutInMs);

/**
 * called by tick function
 *
 * \return value indicates that connection is currently waiting and calling thread can be suspended
 */
LIB61850_INTERNAL bool
IsoClientConnection_handleConnection(IsoClientConnection self);

LIB61850_INTERNAL void
IsoClientConnection_associate(IsoClientConnection self, uint32_t connectTimeoutInMs);

/**
 * Send message and release the transmit buffer
 *
 * \param payload message to send
 */
LIB61850_INTERNAL void
IsoClientConnection_sendMessage(IsoClientConnection self, ByteBuffer* payload);

LIB61850_INTERNAL void
IsoClientConnection_release(IsoClientConnection self);

/**
 * \brief Send ACSE abort message and wait until connection is closed by server or timeout occurred
 */
LIB61850_INTERNAL void
IsoClientConnection_abortAsync(IsoClientConnection self);

LIB61850_INTERNAL void
IsoClientConnection_close(IsoClientConnection self);

/**
 * This function should be called by the API client (usually the MmsConnection) to reserve(allocate)
 * the payload buffer. This is used to prevent concurrent access to the send buffer of the IsoClientConnection
 */
LIB61850_INTERNAL ByteBuffer*
IsoClientConnection_allocateTransmitBuffer(IsoClientConnection self);

/**
 * This function is used to release the transmit buffer in case a formerly allocated transmit buffer cannot
 * be sent.
 */
LIB61850_INTERNAL void
IsoClientConnection_releaseTransmitBuffer(IsoClientConnection self);


#ifdef __cplusplus
}
#endif

#endif /* ISO_CLIENT_CONNECTION_H_ */
