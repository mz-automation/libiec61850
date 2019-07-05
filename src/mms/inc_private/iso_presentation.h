/*
 *  iso_presentation.h
 *
 *  Copyright 2013-2019 Michael Zillgith
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

#ifndef ISO_PRESENTATION_H_
#define ISO_PRESENTATION_H_

#include "byte_buffer.h"
#include "buffer_chain.h"
#include "iso_connection_parameters.h"

typedef struct {
	PSelector callingPresentationSelector;
	PSelector calledPresentationSelector;
	uint8_t nextContextId;
	uint8_t acseContextId;
	uint8_t mmsContextId;
	ByteBuffer nextPayload;
} IsoPresentation;

LIB61850_INTERNAL void
IsoPresentation_init(IsoPresentation* session);

LIB61850_INTERNAL int
IsoPresentation_parseUserData(IsoPresentation* session, ByteBuffer* message);

LIB61850_INTERNAL int
IsoPresentation_parseConnect(IsoPresentation* session, ByteBuffer* message);

LIB61850_INTERNAL void
IsoPresentation_createConnectPdu(IsoPresentation* self, IsoConnectionParameters parameters,
        BufferChain buffer, BufferChain payload);

LIB61850_INTERNAL void
IsoPresentation_createCpaMessage(IsoPresentation* self, BufferChain writeBuffer, BufferChain payload);

LIB61850_INTERNAL void
IsoPresentation_createUserData(IsoPresentation* self, BufferChain writeBuffer, BufferChain payload);

LIB61850_INTERNAL void
IsoPresentation_createUserDataACSE(IsoPresentation* self, BufferChain writeBuffer, BufferChain payload);

LIB61850_INTERNAL int
IsoPresentation_parseAcceptMessage(IsoPresentation* self, ByteBuffer* byteBuffer);

LIB61850_INTERNAL void
IsoPresentation_createAbortUserMessage(IsoPresentation* self, BufferChain writeBuffer, BufferChain payload);

#endif /* ISO_PRESENTATION_H_ */
