/*
 *  mms_access_result.c
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

#include "libiec61850_platform_includes.h"
#include "mms_access_result.h"
#include "mms_server_internal.h"
#include "mms_value_internal.h"

static int
encodeArrayAccessResult(MmsValue* value, uint8_t* buffer, int bufPos, bool encode)
{
    if (value == NULL) // TODO report internal error
        return 0;

    int elementsSize = 0;
    int i;
    int size;

    int arraySize = MmsValue_getArraySize(value);

    for (i = 0; i < arraySize; i++) {
        MmsValue* element = MmsValue_getElement(value, i);

        elementsSize += mmsServer_encodeAccessResult(element, NULL, 0, false);
    }

    if (encode) {
        buffer[bufPos++] = 0xa1; /* tag for array */
        bufPos = BerEncoder_encodeLength(elementsSize, buffer, bufPos);

        for (i = 0; i < arraySize; i++) {
            MmsValue* element = MmsValue_getElement(value, i);

            bufPos = mmsServer_encodeAccessResult(element, buffer, bufPos, true);
        }

        size = bufPos;
    }
    else {
        size = 1 + elementsSize + BerEncoder_determineLengthSize(elementsSize);
    }

    return size;
}

static int
encodeStructuredAccessResult(MmsValue* value, uint8_t* buffer, int bufPos, bool encode)
{
    if (value == NULL) // TODO report internal error
        return 0;

    int componentsSize = 0;
    int i;
    int size;

    int components = value->value.structure.size;

    for (i = 0; i < components; i++) {
        MmsValue* component = value->value.structure.components[i];

        componentsSize += mmsServer_encodeAccessResult(component, NULL, 0, false);
    }

    if (encode) {
        buffer[bufPos++] = 0xa2; /* tag for structure */
        bufPos = BerEncoder_encodeLength(componentsSize, buffer, bufPos);

        for (i = 0; i < components; i++) {
            MmsValue* component = value->value.structure.components[i];

            bufPos = mmsServer_encodeAccessResult(component, buffer, bufPos, true);
        }

        size = bufPos;
    }
    else {
        size = 1 + componentsSize + BerEncoder_determineLengthSize(componentsSize);
    }

    return size;
}

int
mmsServer_encodeAccessResult(MmsValue* value, uint8_t* buffer, int bufPos, bool encode)
{
//    if (value == NULL) // TODO report internal error
//        return 0;

    int size;

    switch (value->type) {
    case MMS_BOOLEAN:
        if (encode)
            bufPos = BerEncoder_encodeBoolean(0x83, value->value.boolean, buffer, bufPos);
        else
            size = 3;
        break;

    case MMS_STRUCTURE:
        if (encode)
            bufPos = encodeStructuredAccessResult(value, buffer, bufPos, true);
        else
            size = encodeStructuredAccessResult(value, buffer, bufPos, false);

        break;

    case MMS_ARRAY:
        if (encode)
            bufPos = encodeArrayAccessResult(value, buffer, bufPos, true);
        else
            size = encodeArrayAccessResult(value, buffer, bufPos, false);
        break;

    case MMS_DATA_ACCESS_ERROR:
        if (encode) {
            int length = BerEncoder_UInt32determineEncodedSize((uint32_t) value->value.dataAccessError);

            bufPos = BerEncoder_encodeTL(0x80, length, buffer, bufPos);
            bufPos = BerEncoder_encodeUInt32((uint32_t) value->value.dataAccessError, buffer, bufPos);
        }
        else
            size = 2 + BerEncoder_UInt32determineEncodedSize((uint32_t) value->value.dataAccessError);
        break;

    case MMS_VISIBLE_STRING:
        if (encode)
            bufPos = BerEncoder_encodeStringWithTag(0x8a, value->value.visibleString.buf, buffer, bufPos);
        else
            size = BerEncoder_determineEncodedStringSize(value->value.visibleString.buf);
        break;
    case MMS_UNSIGNED:
        if (encode)
            bufPos = BerEncoder_encodeAsn1PrimitiveValue(0x86, value->value.integer, buffer, bufPos);
        else
            size = 2 + value->value.integer->size;
        break;
    case MMS_INTEGER:
        if (encode)
            bufPos = BerEncoder_encodeAsn1PrimitiveValue(0x85, value->value.integer, buffer, bufPos);
        else
            size = 2 + value->value.integer->size;
        break;
    case MMS_UTC_TIME:
        if (encode)
            bufPos = BerEncoder_encodeOctetString(0x91, value->value.utcTime, 8, buffer, bufPos);
        else
            size = 10;
        break;
    case MMS_BIT_STRING:
        if (encode)
            bufPos = BerEncoder_encodeBitString(0x84, value->value.bitString.size,
                    value->value.bitString.buf, buffer, bufPos);
        else
            size = BerEncoder_determineEncodedBitStringSize(value->value.bitString.size);
        break;

    case MMS_BINARY_TIME:
        if (encode)
            bufPos = BerEncoder_encodeOctetString(0x8c, value->value.binaryTime.buf,
                    value->value.binaryTime.size, buffer, bufPos);
        else
            size = 2 + value->value.binaryTime.size;
        break;
    case MMS_OCTET_STRING:
        if (encode)
            bufPos = BerEncoder_encodeOctetString(0x89, value->value.octetString.buf,
                    value->value.octetString.size, buffer, bufPos);
        else
            size = 1 + BerEncoder_determineLengthSize(value->value.octetString.size)
                        + value->value.octetString.size;
        break;

    case MMS_FLOAT:
        {
            int floatSize = (value->value.floatingPoint.formatWidth / 8) + 1;

            if (encode) {
                bufPos = BerEncoder_encodeTL(0x87, floatSize, buffer, bufPos);
                bufPos = BerEncoder_encodeFloat(value->value.floatingPoint.buf,
                        value->value.floatingPoint.formatWidth,
                        value->value.floatingPoint.exponentWidth,
                        buffer, bufPos);
            }
            else
                size = floatSize + 2; /* 2 for tag and length */
        }
        break;
    case MMS_STRING:
        if (encode)
            bufPos = BerEncoder_encodeStringWithTag(0x90, value->value.visibleString.buf, buffer, bufPos);
        else
            size = BerEncoder_determineEncodedStringSize(value->value.visibleString.buf);
        break;
    default:
        if (DEBUG_MMS_SERVER)
            printf("encodeAccessResult: error unsupported type!\n");
        size = 0;
        break;
    }

    if (encode)
        return bufPos;
    else
        return size;
}

