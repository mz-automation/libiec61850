/*
 *  ise_session.h
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

#ifndef ISE_SESSION_H_
#define ISE_SESSION_H_

#include "byte_buffer.h"
#include "buffer_chain.h"
#include "iso_connection_parameters.h"

typedef struct {
	SSelector callingSessionSelector;
	SSelector calledSessionSelector;
	uint16_t sessionRequirement;
	uint8_t protocolOptions;
	ByteBuffer userData;
} IsoSession;

typedef enum {
	SESSION_OK,
	SESSION_ERROR,
	SESSION_CONNECT,
	SESSION_GIVE_TOKEN,
	SESSION_DATA,
	SESSION_ABORT,
	SESSION_FINISH,
	SESSION_DISCONNECT,
	SESSION_NOT_FINISHED
} IsoSessionIndication;

LIB61850_INTERNAL void
IsoSession_init(IsoSession* session);

LIB61850_INTERNAL ByteBuffer*
IsoSession_getUserData(IsoSession* session);

LIB61850_INTERNAL void
IsoSession_createConnectSpdu(IsoSession* self, IsoConnectionParameters isoParameters, BufferChain buffer, BufferChain payload);

LIB61850_INTERNAL IsoSessionIndication
IsoSession_parseMessage(IsoSession* self, ByteBuffer* message);

LIB61850_INTERNAL void
IsoSession_createDataSpdu(IsoSession* self, BufferChain buffer, BufferChain payload);

LIB61850_INTERNAL void
IsoSession_createAcceptSpdu(IsoSession* self, BufferChain buffer, BufferChain payload);

LIB61850_INTERNAL void
IsoSession_createAbortSpdu(IsoSession* self, BufferChain buffer, BufferChain payload);

LIB61850_INTERNAL void
IsoSession_createFinishSpdu(IsoSession* self, BufferChain buffer, BufferChain payload);

LIB61850_INTERNAL void
IsoSession_createDisconnectSpdu(IsoSession* self, BufferChain buffer, BufferChain payload);

#endif /* ISE_SESSION_H_ */
