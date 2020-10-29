/*
 *  mms_client_initiate.c
 *
 *  Copyright 2013-2020 Michael Zillgith
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
#include "mms_common.h"
#include "mms_common_internal.h"
#include "mms_client_connection.h"
#include "byte_buffer.h"

#include "stack_config.h"

#include "mms_client_internal.h"

#include "ber_encoder.h"

/* servicesSupported = {GetNameList} - required by initiate request, other services are required by some servers to work properly */
static uint8_t servicesSupported[] = { 0xee, 0x1c, 0x00, 0x00, 0x04, 0x08, 0x00, 0x00, 0x79, 0xef, 0x18 };

void
mmsClient_createInitiateRequest(MmsConnection self, ByteBuffer* message)
{
    int maxServerOutstandingCalling = DEFAULT_MAX_SERV_OUTSTANDING_CALLING;
    int maxServerOutstandingCalled = DEFAULT_MAX_SERV_OUTSTANDING_CALLED;
    int dataStructureNestingLevel = DEFAULT_DATA_STRUCTURE_NESTING_LEVEL;

    uint32_t localDetailSize =
            BerEncoder_UInt32determineEncodedSize(self->parameters.maxPduSize);

    uint32_t proposedMaxServerOutstandingCallingSize =
            BerEncoder_UInt32determineEncodedSize(maxServerOutstandingCalling);

    uint32_t proposedMaxServerOutstandingCalledSize =
            BerEncoder_UInt32determineEncodedSize(maxServerOutstandingCalled);

    uint32_t dataStructureNestingLevelSize =
            BerEncoder_UInt32determineEncodedSize(dataStructureNestingLevel);

    uint32_t initRequestDetailSize = 3 + 5 + 14;

    uint32_t initiateRequestPduSize = 2 + localDetailSize +
                             2 + proposedMaxServerOutstandingCallingSize +
                             2 + proposedMaxServerOutstandingCalledSize +
                             2 + dataStructureNestingLevelSize +
                             2 + initRequestDetailSize;

    /* encode message (InitiateRequestPdu) */

    int bufPos = 0;
    uint8_t* buffer = message->buffer;

    bufPos = BerEncoder_encodeTL(0xa8, initiateRequestPduSize, buffer, bufPos);

    /* localDetail */
    bufPos = BerEncoder_encodeTL(0x80, localDetailSize, buffer, bufPos);
    bufPos = BerEncoder_encodeUInt32(self->parameters.maxPduSize, buffer, bufPos);

    /* proposedMaxServerOutstandingCalling */
    bufPos = BerEncoder_encodeTL(0x81, proposedMaxServerOutstandingCallingSize, buffer, bufPos);
    bufPos = BerEncoder_encodeUInt32(maxServerOutstandingCalling, buffer, bufPos);

    /* proposedMaxServerOutstandingCalled */
    bufPos = BerEncoder_encodeTL(0x82, proposedMaxServerOutstandingCalledSize, buffer, bufPos);
    bufPos = BerEncoder_encodeUInt32(maxServerOutstandingCalled, buffer, bufPos);

    /* proposedDataStructureNestingLevel */
    bufPos = BerEncoder_encodeTL(0x83, dataStructureNestingLevelSize, buffer, bufPos);
    bufPos = BerEncoder_encodeUInt32(dataStructureNestingLevel, buffer, bufPos);

    /* initRequestDetail */
    bufPos = BerEncoder_encodeTL(0xa4, initRequestDetailSize, buffer, bufPos);

    /* proposedVersionNumber = 1 */
    buffer[bufPos++] = 0x80;
    buffer[bufPos++] = 0x01;
    buffer[bufPos++] = 0x01;

    /* proposedParameterCBC: fixed */
    buffer[bufPos++] = 0x81;
    buffer[bufPos++] = 0x03;
    buffer[bufPos++] = 0x05; /* padding */
    buffer[bufPos++] = 0xf1; /* str1, str2, vnam, vlis, valt  */
    buffer[bufPos++] = 0x00;

    /* servicesSupportedCalling */
    bufPos = BerEncoder_encodeBitString(0x82, 85, servicesSupported, buffer, bufPos);

    message->size = bufPos;
}

int
mmsClient_createConcludeRequest(MmsConnection self, ByteBuffer* message)
{
    (void)self;

    if (message->maxSize > 1) {
        message->buffer[0] = 0x8b;
        message->buffer[1] = 0;
        message->size = 2;
        return 2;
    }
    else
        return -1;
}

static bool
parseInitResponseDetail(MmsConnection self, uint8_t* buffer, int bufPos, int maxBufPos)
{
    while (bufPos < maxBufPos) {
        uint8_t tag = buffer[bufPos++];
        int length;

        bufPos = BerDecoder_decodeLength(buffer, &length, bufPos, maxBufPos);

        if (bufPos < 0)
            return false;

        switch (tag) {

        case 0x80: /* negotiated protocol version */
            break;

        case 0x81: /* parameter CBB */
            break;

        case 0x82: /* services supported called */
            {
                int i;

                for (i = 0; i < 11; i++)
                     self->parameters.servicesSupported[i] = buffer[bufPos + i];
            }
            break;

        case 0x00: /* indefinite length end tag -> ignore */
            break;

        default:
            break;
        }

        bufPos += length;
    }

    return true;
}

bool
mmsClient_parseInitiateResponse(MmsConnection self, ByteBuffer* response)
{
    self->parameters.maxPduSize = CONFIG_MMS_MAXIMUM_PDU_SIZE;
    self->parameters.dataStructureNestingLevel = DEFAULT_DATA_STRUCTURE_NESTING_LEVEL;
    self->parameters.maxServOutstandingCalled = DEFAULT_MAX_SERV_OUTSTANDING_CALLED;
    self->parameters.maxServOutstandingCalling = DEFAULT_MAX_SERV_OUTSTANDING_CALLING;

    int bufPos = 1; /* ignore tag - already checked */

    int maxBufPos = ByteBuffer_getSize(response);
    uint8_t* buffer = ByteBuffer_getBuffer(response);

    int length;
    bufPos = BerDecoder_decodeLength(buffer, &length, bufPos, maxBufPos);

    if (bufPos < 0)
        return false;

    while (bufPos < maxBufPos) {
        uint8_t tag = buffer[bufPos++];

        bufPos = BerDecoder_decodeLength(buffer, &length, bufPos, maxBufPos);

        if (bufPos < 0)
            return false;

        switch (tag) {
        case 0x80: /* local-detail-calling */
            self->parameters.maxPduSize = BerDecoder_decodeUint32(buffer, length, bufPos);

            if (self->parameters.maxPduSize > CONFIG_MMS_MAXIMUM_PDU_SIZE)
                self->parameters.maxPduSize = CONFIG_MMS_MAXIMUM_PDU_SIZE;

            break;

        case 0x81:  /* proposed-max-serv-outstanding-calling */
            self->parameters.maxServOutstandingCalling = BerDecoder_decodeUint32(buffer, length, bufPos);

            if (self->parameters.maxServOutstandingCalling > DEFAULT_MAX_SERV_OUTSTANDING_CALLING)
                self->parameters.maxServOutstandingCalling = DEFAULT_MAX_SERV_OUTSTANDING_CALLING;

            break;

        case 0x82:  /* proposed-max-serv-outstanding-called */
            self->parameters.maxServOutstandingCalled = BerDecoder_decodeUint32(buffer, length, bufPos);

            if (self->parameters.maxServOutstandingCalled > DEFAULT_MAX_SERV_OUTSTANDING_CALLED)
                self->parameters.maxServOutstandingCalled = DEFAULT_MAX_SERV_OUTSTANDING_CALLED;

            break;
        case 0x83: /* proposed-data-structure-nesting-level */
            self->parameters.dataStructureNestingLevel = BerDecoder_decodeUint32(buffer, length, bufPos);
            break;

        case 0xa4: /* mms-init-request-detail */
            {
                if (parseInitResponseDetail(self, buffer, bufPos, bufPos + length) == false)
                    return false;
            }
            break;

        case 0x00: /* indefinite length end tag -> ignore */
            break;

        default:
            break; /* Ignore unknown tags */
        }

        bufPos += length;
    }


    return true;
}
