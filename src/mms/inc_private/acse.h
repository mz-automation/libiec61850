/*
 *  acse.h
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

#include "libiec61850_platform_includes.h"
#include "stack_config.h"
#include "byte_buffer.h"
#include "buffer_chain.h"
#include "iso_connection_parameters.h"
#include "tls_socket.h"

#ifndef ACSE_H_
#define ACSE_H_

typedef enum
{
    idle, requestIndicated, connected
} AcseConnectionState;

typedef enum
{
    ACSE_ERROR,
    ACSE_ASSOCIATE,
    ACSE_ASSOCIATE_FAILED,
    ACSE_OK,
    ACSE_ABORT,
    ACSE_RELEASE_REQUEST,
    ACSE_RELEASE_RESPONSE
} AcseIndication;

typedef struct sAcseConnection
{
    AcseConnectionState state;
    long nextReference;
    uint8_t* userDataBuffer;
    int userDataBufferSize;
    AcseAuthenticationParameter authentication;
    AcseAuthenticator authenticator;
    IsoApplicationReference applicationReference;
    void* authenticatorParameter;
    void* securityToken;

#if (CONFIG_MMS_SUPPORT_TLS == 1)
    TLSSocket tlsSocket;
#endif
} AcseConnection;

#define ACSE_RESULT_ACCEPT 0
#define ACSE_RESULT_REJECT_PERMANENT 1
#define ACSE_RESULT_REJECT_TRANSIENT 2

LIB61850_INTERNAL void
AcseConnection_init(AcseConnection* self, AcseAuthenticator authenticator, void* parameter, TLSSocket tlsSocket);

LIB61850_INTERNAL AcseIndication
AcseConnection_parseMessage(AcseConnection* self, ByteBuffer* message);

LIB61850_INTERNAL void
AcseConnection_createAssociateFailedMessage(AcseConnection* connection, BufferChain writeBuffer);

LIB61850_INTERNAL void
AcseConnection_createAssociateResponseMessage(
        AcseConnection* self,
        uint8_t resultCode,
        BufferChain acseWriteBuffer,
        BufferChain payloadBuffer
        );

LIB61850_INTERNAL void
AcseConnection_createAssociateRequestMessage(AcseConnection* self,
        IsoConnectionParameters isoParameters,
        BufferChain writeBuffer,
        BufferChain payload,
        AcseAuthenticationParameter authParameter);

LIB61850_INTERNAL void
AcseConnection_createAbortMessage(AcseConnection* self, BufferChain writeBuffer, bool isProvider);

LIB61850_INTERNAL void
AcseConnection_createReleaseRequestMessage(AcseConnection* self, BufferChain writeBuffer);

LIB61850_INTERNAL void
AcseConnection_createReleaseResponseMessage(AcseConnection* self, BufferChain writeBuffer);

#endif /* ACSE_H_ */
