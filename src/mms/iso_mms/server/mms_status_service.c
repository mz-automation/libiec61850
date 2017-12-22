/*
 *  mms_status_service.c
 *
 *  Copyright 2014 Michael Zillgith
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
#include "mms_server_internal.h"

#if (MMS_STATUS_SERVICE == 1)

void
mmsServer_handleStatusRequest(
        MmsServerConnection connection,
        uint8_t* requestBuffer,
        int bufPos,
        uint32_t invokeId,
        ByteBuffer* response)
{
    /* check for extended derivation */
    bool extendedDerivation = BerDecoder_decodeBoolean(requestBuffer, bufPos);

    if (DEBUG_MMS_SERVER)
        printf("mms_status_service.c: statusRequest (extendedDerivation: %i)\n", extendedDerivation);

    bufPos = 0;
    uint8_t* buffer = response->buffer;

    uint32_t invokeIdLength = BerEncoder_UInt32determineEncodedSize(invokeId);

    MmsServer mmsServer = connection->server;

    /* Invoke user provided callback */
    if (mmsServer->statusRequestListener != NULL)
        mmsServer->statusRequestListener(mmsServer->statusRequestListenerParameter, mmsServer, connection, extendedDerivation);

    uint32_t vmdPhysicalStatusLength = BerEncoder_UInt32determineEncodedSize((uint32_t) mmsServer->vmdPhysicalStatus);
    uint32_t vmdLogicalStatusLength = BerEncoder_UInt32determineEncodedSize((uint32_t) mmsServer->vmdLogicalStatus);

    uint32_t statusLength = 2 + vmdPhysicalStatusLength + 2 + vmdLogicalStatusLength;

    uint32_t statusResponseLength = invokeIdLength + 2 + 1 + BerEncoder_determineLengthSize(statusLength) + statusLength;

    /* Status response pdu */
    bufPos = BerEncoder_encodeTL(0xa1, statusResponseLength, buffer, bufPos);

    /* invokeId */
    bufPos = BerEncoder_encodeTL(0x02, invokeIdLength, buffer, bufPos);
    bufPos = BerEncoder_encodeUInt32(invokeId, buffer, bufPos);

    bufPos = BerEncoder_encodeTL(0xa0, statusLength, buffer, bufPos);
    bufPos = BerEncoder_encodeTL(0x80, vmdLogicalStatusLength, buffer, bufPos);
    bufPos = BerEncoder_encodeUInt32((uint32_t) mmsServer->vmdLogicalStatus, buffer, bufPos);
    bufPos = BerEncoder_encodeTL(0x81, vmdPhysicalStatusLength, buffer, bufPos);
    bufPos = BerEncoder_encodeUInt32((uint32_t) mmsServer->vmdPhysicalStatus, buffer, bufPos);

    response->size = bufPos;
}

void
MmsServer_setVMDStatus(MmsServer self, int vmdLogicalStatus, int vmdPhysicalStatus)
{
    self->vmdLogicalStatus = vmdLogicalStatus;
    self->vmdPhysicalStatus = vmdPhysicalStatus;
}

int
MmsServer_getVMDLogicalStatus(MmsServer self)
{
    return self->vmdLogicalStatus;
}

int
MmsServer_getVMDPhysicalStatus(MmsServer self)
{
    return self->vmdPhysicalStatus;
}

void
MmsServer_setStatusRequestListener(MmsServer self, MmsStatusRequestListener listener, void* parameter)
{
    self->statusRequestListener = listener;
    self->statusRequestListenerParameter = parameter;
}

#endif /* MMS_STATUS_SERVICE == 1 */
