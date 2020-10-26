/*
 *  mms_client_status.c
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
#include "mms_common.h"
#include "mms_client_connection.h"
#include "byte_buffer.h"

#include "mms_client_internal.h"
#include "ber_encoder.h"
#include "ber_decode.h"

void
mmsClient_createStatusRequest(uint32_t invokeId, ByteBuffer* request, bool extendedDerivation)
{
    uint32_t invokeIdSize = BerEncoder_UInt32determineEncodedSize(invokeId);
    uint32_t confirmedRequestPduSize = 2 + 2 + 1 + invokeIdSize;

    int bufPos = 0;
    uint8_t* buffer = request->buffer;

    bufPos = BerEncoder_encodeTL(0xa0, confirmedRequestPduSize, buffer, bufPos);
    bufPos = BerEncoder_encodeTL(0x02, invokeIdSize, buffer, bufPos);
    bufPos = BerEncoder_encodeUInt32(invokeId, buffer, bufPos);

    bufPos = BerEncoder_encodeBoolean(0x80, extendedDerivation, buffer, bufPos);

    request->size = bufPos;
}

bool
mmsClient_parseStatusResponse(MmsConnection self, ByteBuffer* response, int bufPos, int* vmdLogicalStatus, int* vmdPhysicalStatus)
{
    (void)self;

    uint8_t* buffer = ByteBuffer_getBuffer(response);
    int maxBufPos = ByteBuffer_getSize(response);
    int length;

    uint8_t tag = buffer[bufPos++];

    if (tag != 0xa0) {
        if (DEBUG_MMS_CLIENT)
            printf("mmsClient_parseStatusResponse: unknown tag %02x\n", tag);
        goto exit_error;
    }

    bufPos = BerDecoder_decodeLength(buffer, &length, bufPos, maxBufPos);
    if (bufPos < 0) goto exit_error;

    int endPos = bufPos + length;

    bool hasPhysicalStatus = false;
    bool hasLogicalStatus = false;

    while (bufPos < endPos) {
        tag = buffer[bufPos++];
        bufPos = BerDecoder_decodeLength(buffer, &length, bufPos, maxBufPos);
        if (bufPos < 0) goto exit_error;

        switch (tag) {
        case 0x80: /* vmdLogicalStatus */
            if (vmdLogicalStatus != NULL)
                *vmdLogicalStatus = BerDecoder_decodeUint32(buffer, length, bufPos);

            hasLogicalStatus = true;
            bufPos += length;
            break;
        case 0x81: /* vmdPhysicalStatus */
            if (vmdPhysicalStatus != NULL)
                *vmdPhysicalStatus = BerDecoder_decodeUint32(buffer, length, bufPos);

            hasPhysicalStatus = true;
            bufPos += length;
            break;
        case 0x82: /* localDetail */
            bufPos += length;
            break;
        case 0x00: /* indefinite length end tag -> ignore */
            break;
        default:
            return false;
        }
    }

    if (hasPhysicalStatus && hasLogicalStatus)
        return true;

exit_error:
    return false;
}



