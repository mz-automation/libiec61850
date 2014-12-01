/*
 *  cotp.h
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

#ifndef COTP_H_
#define COTP_H_

#include "libiec61850_platform_includes.h"
#include "byte_buffer.h"
#include "byte_stream.h"
#include "buffer_chain.h"
#include "socket.h"
#include "iso_connection_parameters.h"

typedef struct {
    int32_t tsap_id_src;
    int32_t tsap_id_dst;
    uint8_t tpdu_size;
} CotpOptions;

typedef struct {
    int state;
    int srcRef;
    int dstRef;
    int protocolClass;
    Socket socket;
    CotpOptions options;
    bool isLastDataUnit;
    ByteBuffer* payload;
    ByteBuffer* writeBuffer;
} CotpConnection;

typedef enum {
    OK, ERROR, CONNECT_INDICATION, DATA_INDICATION, DISCONNECT_INDICATION
} CotpIndication;

int /* in byte */
CotpConnection_getTpduSize(CotpConnection* self);

void
CotpConnection_setTpduSize(CotpConnection* self, int tpduSize /* in byte */);

void
CotpConnection_init(CotpConnection* self, Socket socket, ByteBuffer* payloadBuffer);

void
CotpConnection_destroy(CotpConnection* self);

CotpIndication
CotpConnection_parseIncomingMessage(CotpConnection* self);

CotpIndication
CotpConnection_sendConnectionRequestMessage(CotpConnection* self, IsoConnectionParameters isoParameters);

CotpIndication
CotpConnection_sendConnectionResponseMessage(CotpConnection* self);

CotpIndication
CotpConnection_sendDataMessage(CotpConnection* self, BufferChain payload);

ByteBuffer*
CotpConnection_getPayload(CotpConnection* self);

int
CotpConnection_getSrcRef(CotpConnection* self);

int
CotpConnection_getDstRef(CotpConnection* self);

#endif /* COTP_H_ */
