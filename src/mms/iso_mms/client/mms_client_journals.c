/*
 *  mms_client_journals.c
 *
 *  Copyright 2016 Michael Zillgith
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
#include "conversions.h"
#include "mms_value_internal.h"

void
mmsClient_createReadJournalRequest(uint32_t invokeId, ByteBuffer* request, const char* domainId, const char* itemId)
{
    /* calculate sizes */
    uint32_t invokeIdSize = BerEncoder_UInt32determineEncodedSize(invokeId);

    uint32_t domainIdStringSize = strlen(domainId);
    uint32_t domainIdSize = 1 + BerEncoder_determineLengthSize(domainIdStringSize) + domainIdStringSize;

    uint32_t itemIdStringSize = strlen(itemId);
    uint32_t itemIdSize = 1 + BerEncoder_determineLengthSize(itemIdStringSize) + itemIdStringSize;

    uint32_t objectIdSize = domainIdSize + itemIdSize;

    uint32_t journalNameSize =  1 + BerEncoder_determineLengthSize(objectIdSize) + (objectIdSize);

    uint32_t journalReadSize = 1 + BerEncoder_determineLengthSize(journalNameSize) + (journalNameSize);

    uint32_t confirmedRequestPduSize = 1 + 2 + 2  + invokeIdSize + journalReadSize;

    /* encode to buffer */
    int bufPos = 0;
    uint8_t* buffer = request->buffer;

    bufPos = BerEncoder_encodeTL(0xa0, confirmedRequestPduSize, buffer, bufPos);
    bufPos = BerEncoder_encodeTL(0x02, invokeIdSize, buffer, bufPos);
    bufPos = BerEncoder_encodeUInt32(invokeId, buffer, bufPos);

    /* Encode read journal tag (context | structured ) [65 = 41h] */
    buffer[bufPos++] = 0xbf;
    buffer[bufPos++] = 0x41;

    bufPos = BerEncoder_encodeLength(journalReadSize, buffer, bufPos);
    bufPos = BerEncoder_encodeTL(0xa0, journalNameSize, buffer, bufPos);

    bufPos = BerEncoder_encodeTL(0xa1, objectIdSize, buffer, bufPos);

    bufPos = BerEncoder_encodeOctetString(0x1a, (uint8_t*) domainId, domainIdStringSize, buffer, bufPos);
    bufPos = BerEncoder_encodeOctetString(0x1a, (uint8_t*) itemId, itemIdStringSize, buffer, bufPos);

    request->size = bufPos;
}

void
mmsClient_createReadJournalRequestWithTimeRange(uint32_t invokeId, ByteBuffer* request, const char* domainId, const char* itemId,
        MmsValue* startingTime, MmsValue* endingTime)
{
    /* calculate sizes */
    uint32_t invokeIdSize = BerEncoder_UInt32determineEncodedSize(invokeId);

    uint32_t domainIdStringSize = strlen(domainId);
    uint32_t domainIdSize = 1 + BerEncoder_determineLengthSize(domainIdStringSize) + domainIdStringSize;

    uint32_t itemIdStringSize = strlen(itemId);
    uint32_t itemIdSize = 1 + BerEncoder_determineLengthSize(itemIdStringSize) + itemIdStringSize;

    uint32_t objectIdSize = domainIdSize + itemIdSize;

    uint32_t journalNameSize =  1 + BerEncoder_determineLengthSize(objectIdSize) + (objectIdSize);

    uint32_t startingTimeSize = 2 + startingTime->value.binaryTime.size;

    uint32_t rangeStartSpecSize = 2 + startingTimeSize;

    uint32_t endingTimeSize = 2 + endingTime->value.binaryTime.size;

    uint32_t rangeEndSpecSize = 2 + endingTimeSize;

    uint32_t journalReadContentSize = journalNameSize + rangeStartSpecSize + rangeEndSpecSize;

    uint32_t journalReadSize = 1 + BerEncoder_determineLengthSize(journalReadContentSize) + (journalReadContentSize);

    uint32_t confirmedRequestPduSize = 1 + 2 + 2  + invokeIdSize + journalReadSize;

    /* encode to buffer */
    int bufPos = 0;
    uint8_t* buffer = request->buffer;

    bufPos = BerEncoder_encodeTL(0xa0, confirmedRequestPduSize, buffer, bufPos);
    bufPos = BerEncoder_encodeTL(0x02, invokeIdSize, buffer, bufPos);
    bufPos = BerEncoder_encodeUInt32(invokeId, buffer, bufPos);

    /* encode read journal tag (context | structured ) [65 = 41h] */
    buffer[bufPos++] = 0xbf;
    buffer[bufPos++] = 0x41;

    bufPos = BerEncoder_encodeLength(journalReadSize, buffer, bufPos);

    /* encode journalName */
    bufPos = BerEncoder_encodeTL(0xa0, journalNameSize, buffer, bufPos);

    bufPos = BerEncoder_encodeTL(0xa1, objectIdSize, buffer, bufPos);

    bufPos = BerEncoder_encodeOctetString(0x1a, (uint8_t*) domainId, domainIdStringSize, buffer, bufPos);
    bufPos = BerEncoder_encodeOctetString(0x1a, (uint8_t*) itemId, itemIdStringSize, buffer, bufPos);

    /* encode rangeStartSpecification|startingTime */
    bufPos = BerEncoder_encodeTL(0xa1, startingTimeSize, buffer, bufPos);
    bufPos = BerEncoder_encodeOctetString(0x80, startingTime->value.binaryTime.buf,
            startingTime->value.binaryTime.size, buffer, bufPos);

    /* encode rangeStopSpecification|endingTime */
    bufPos = BerEncoder_encodeTL(0xa2, endingTimeSize, buffer, bufPos);
    bufPos = BerEncoder_encodeOctetString(0x80, endingTime->value.binaryTime.buf,
            endingTime->value.binaryTime.size, buffer, bufPos);

    request->size = bufPos;
}

void
mmsClient_createReadJournalRequestStartAfter(uint32_t invokeId, ByteBuffer* request, const char* domainId, const char* itemId,
        MmsValue* timeSpecification, MmsValue* entrySpecification)
{
    /* calculate sizes */
    uint32_t invokeIdSize = BerEncoder_UInt32determineEncodedSize(invokeId);

    uint32_t domainIdStringSize = strlen(domainId);
    uint32_t domainIdSize = 1 + BerEncoder_determineLengthSize(domainIdStringSize) + domainIdStringSize;

    uint32_t itemIdStringSize = strlen(itemId);
    uint32_t itemIdSize = 1 + BerEncoder_determineLengthSize(itemIdStringSize) + itemIdStringSize;

    uint32_t objectIdSize = domainIdSize + itemIdSize;

    uint32_t journalNameSize =  1 + BerEncoder_determineLengthSize(objectIdSize) + (objectIdSize);

    uint32_t timeSpecificationSize = 2 + timeSpecification->value.binaryTime.size;

    uint32_t entrySpecificationSize = 2 + entrySpecification->value.octetString.size;

    uint32_t entryToStartAfterContentSize = timeSpecificationSize + entrySpecificationSize;

    uint32_t entryToStartAfterSize =  1 + BerEncoder_determineLengthSize(entryToStartAfterContentSize)
            + entryToStartAfterContentSize;

    uint32_t journalReadContentSize = journalNameSize + entryToStartAfterSize;

    uint32_t journalReadSize = 1 + BerEncoder_determineLengthSize(journalReadContentSize) + (journalReadContentSize);

    uint32_t confirmedRequestPduSize = 1 + 2 + 2  + invokeIdSize + journalReadSize;

    /* encode to buffer */
    int bufPos = 0;
    uint8_t* buffer = request->buffer;

    bufPos = BerEncoder_encodeTL(0xa0, confirmedRequestPduSize, buffer, bufPos);
    bufPos = BerEncoder_encodeTL(0x02, invokeIdSize, buffer, bufPos);
    bufPos = BerEncoder_encodeUInt32(invokeId, buffer, bufPos);

    /* encode read journal tag (context | structured ) [65 = 41h] */
    buffer[bufPos++] = 0xbf;
    buffer[bufPos++] = 0x41;

    bufPos = BerEncoder_encodeLength(journalReadSize, buffer, bufPos);

    /* encode journalName */
    bufPos = BerEncoder_encodeTL(0xa0, journalNameSize, buffer, bufPos);

    bufPos = BerEncoder_encodeTL(0xa1, objectIdSize, buffer, bufPos);

    bufPos = BerEncoder_encodeOctetString(0x1a, (uint8_t*) domainId, domainIdStringSize, buffer, bufPos);
    bufPos = BerEncoder_encodeOctetString(0x1a, (uint8_t*) itemId, itemIdStringSize, buffer, bufPos);

    /* encode entryToStartAfter */
    bufPos = BerEncoder_encodeTL(0xa5, entryToStartAfterContentSize, buffer, bufPos);

    /* encode entryToStartAfter|timeSpecification */
    bufPos = BerEncoder_encodeOctetString(0x80, timeSpecification->value.binaryTime.buf,
            timeSpecification->value.binaryTime.size, buffer, bufPos);

    /* encode entryToStartAfter|entrySpecification */
    bufPos = BerEncoder_encodeOctetString(0x81, entrySpecification->value.octetString.buf,
            entrySpecification->value.octetString.size, buffer, bufPos);

    request->size = bufPos;
}

