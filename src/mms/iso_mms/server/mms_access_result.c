/*
 *  mms_access_result.c
 *
 *  Copyright 2013-2021 Michael Zillgith
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
#include "mms_value_internal.h"

static int
encodeArrayAccessResult(MmsValue* value, uint8_t* buffer, int bufPos, bool encode)
{
    if (value == NULL) /* TODO report internal error */
        return 0;

    int elementsSize = 0;
    int i;
    int size;

    int arraySize = MmsValue_getArraySize(value);

    for (i = 0; i < arraySize; i++) {
        MmsValue* element = MmsValue_getElement(value, i);

        elementsSize += MmsValue_encodeMmsData(element, NULL, 0, false);
    }

    if (encode) {
        buffer[bufPos++] = 0xa1; /* tag for array */
        bufPos = BerEncoder_encodeLength(elementsSize, buffer, bufPos);

        for (i = 0; i < arraySize; i++) {
            MmsValue* element = MmsValue_getElement(value, i);

            bufPos = MmsValue_encodeMmsData(element, buffer, bufPos, true);
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
    int componentsSize = 0;
    int i;
    int size;

    int componentCount = value->value.structure.size;

    MmsValue** components = value->value.structure.components;

    for (i = 0; i < componentCount; i++)
        componentsSize += MmsValue_encodeMmsData(components[i], NULL, 0, false);

    if (encode) {
        buffer[bufPos++] = 0xa2; /* tag for structure */
        bufPos = BerEncoder_encodeLength(componentsSize, buffer, bufPos);

        for (i = 0; i < componentCount; i++)
            bufPos = MmsValue_encodeMmsData(components[i], buffer, bufPos, true);

        size = bufPos;
    }
    else
        size = 1 + componentsSize + BerEncoder_determineLengthSize(componentsSize);

    return size;
}


/*
 * Returns the number of elements in an MMS Data element
 * or -1 in case of an parsing error.
 */
static int
getNumberOfElements(uint8_t* buffer, int bufPos, int elementLength)
{
    int elementIndex = 0;
    int elementEndBufPos = bufPos + elementLength;

    while (bufPos < elementEndBufPos) {
         uint8_t tag = buffer[bufPos++];

         bufPos = BerDecoder_decodeLength(buffer, &elementLength, bufPos, elementEndBufPos);

         if (bufPos < 0)
             goto exit_with_error;

         switch (tag) {
         case 0x80: /* reserved for access result */
             break;
         case 0xa1: /* array */
             break;
         case 0xa2: /* structure */
             break;
         case 0x83: /* boolean */
             break;
         case 0x84: /* BIT STRING */
             break;
         case 0x85: /* integer */
             break;
         case 0x86: /* unsigned integer */
             break;
         case 0x87: /* Float */
             break;
         case 0x89: /* octet string */
             break;
         case 0x8a: /* visible string */
             break;
         case 0x8c: /* binary time */
             break;
         case 0x91: /* Utctime */
             break;
         case 0x00: /* indefinite length end tag -> ignore */
             break;
         default:
             goto exit_with_error;
         }

         bufPos += elementLength;

         elementIndex++;
    }

    return elementIndex;

exit_with_error:
    return -1;
}

static MmsValue*
MmsValue_decodeMmsDataRecursive(uint8_t* buffer, int bufPos, int bufferLength, int* endBufPos, int depth, int maxDepth)
{
    depth++;

    if (depth > maxDepth)
        return NULL;

    MmsValue* value = NULL;

    int dataEndBufPos = bufferLength;

    if (bufferLength < 1)
        goto exit_with_error;

    uint8_t tag = buffer[bufPos++];

    int dataLength;

    bufPos = BerDecoder_decodeLength(buffer, &dataLength, bufPos, dataEndBufPos);

    if (bufPos < 0)
        goto exit_with_error;

    /* if not indefinite length end tag, visible-string, mms-string, or octet-string, data length must be > 0 */
    if (tag != 0)
    {
        if (tag != 0x8a && tag != 0x90 && tag != 0x89 && dataLength == 0)
            goto exit_with_error;
    }

    switch (tag) {

    case 0xa1: /* MMS_ARRAY */
    case 0xa2: /* MMS_STRUCTURE */
    {
        int elementCount = getNumberOfElements(buffer, bufPos, dataLength);

        if (elementCount < 0)
            goto exit_with_error;

        if (tag == 0xa1)
            value = MmsValue_createEmptyArray(elementCount);
        else
            value = MmsValue_createEmptyStructure(elementCount);

        int i;

        for (i = 0; i < elementCount; i++)
        {
            int elementLength;

            int newBufPos = BerDecoder_decodeLength(buffer, &elementLength, bufPos + 1, dataEndBufPos);

            if (newBufPos < 0)
                goto exit_with_error;

            int elementBufLength = newBufPos - bufPos + elementLength;

            MmsValue* elementValue = MmsValue_decodeMmsDataRecursive(buffer, bufPos, bufPos + elementBufLength, NULL, depth, maxDepth);

            if (elementValue == NULL)
                goto exit_with_error;

            MmsValue_setElement(value, i, elementValue);

            bufPos = newBufPos + elementLength;
        }

        if (value == NULL)
            goto exit_with_error;
    }

        break;

    case 0x80: /* MMS_DATA_ACCESS_ERROR */
        value = MmsValue_newDataAccessError((MmsDataAccessError) BerDecoder_decodeUint32(buffer, dataLength, bufPos));
        bufPos += dataLength;
        break;

    case 0x83: /* MMS_BOOLEAN */
        value = MmsValue_newBoolean(BerDecoder_decodeBoolean(buffer, bufPos));
        bufPos += dataLength;
        break;

    case 0x84: /* MMS_BIT_STRING */
    {
        int padding = buffer[bufPos];

        if (padding > 7)
            goto exit_with_error;

        int bitStringLength = (8 * (dataLength - 1)) - padding;
        value = MmsValue_newBitString(bitStringLength);
        memcpy(value->value.bitString.buf, buffer + bufPos + 1, dataLength - 1);
        bufPos += dataLength;
    }
        break;

    case 0x85: /* MMS_INTEGER */
        if (dataLength > 8)
            goto exit_with_error;

        value = MmsValue_newInteger(dataLength * 8);
        memcpy(value->value.integer->octets, buffer + bufPos, dataLength);
        value->value.integer->size = dataLength;
        bufPos += dataLength;
        break;

    case 0x86: /* MMS_UNSIGNED */
        if (dataLength > 8)
            goto exit_with_error;

        value = MmsValue_newUnsigned(dataLength * 8);
        memcpy(value->value.integer->octets, buffer + bufPos, dataLength);
        value->value.integer->size = dataLength;

        bufPos += dataLength;
        break;

    case 0x87: /* MMS_FLOAT */
        if (dataLength == 9)
            value = MmsValue_newDouble(BerDecoder_decodeDouble(buffer, bufPos));
        else if (dataLength == 5)
            value = MmsValue_newFloat(BerDecoder_decodeFloat(buffer, bufPos));
        bufPos += dataLength;
        break;

    case 0x89: /* MMS_OCTET_STRING */
        value = MmsValue_newOctetString(dataLength, dataLength);
        memcpy(value->value.octetString.buf, buffer + bufPos, dataLength);
        bufPos += dataLength;
        break;

    case 0x8a: /* MMS_VISIBLE_STRING */
        value = MmsValue_newVisibleStringFromByteArray(buffer + bufPos, dataLength);
        bufPos += dataLength;
        break;

    case 0x8c: /* MMS_BINARY_TIME */
        if (dataLength == 4)
            value = MmsValue_newBinaryTime(true);
        else if (dataLength == 6)
            value = MmsValue_newBinaryTime(false);

        if ((dataLength == 4) || (dataLength == 6))
            memcpy(value->value.binaryTime.buf, buffer + bufPos, dataLength);

        bufPos += dataLength;

        break;

    case 0x90: /* MMS_STRING */
        value = MmsValue_newVisibleStringFromByteArray(buffer + bufPos, dataLength);
        value->type = MMS_STRING;
        bufPos += dataLength;

        break;

    case 0x91: /* MMS_UTC_TIME */
        if (dataLength == 8)
        {
            value = MmsValue_newUtcTime(0);
            MmsValue_setUtcTimeByBuffer(value, buffer + bufPos);
            bufPos += dataLength;
        }
        else
            goto exit_with_error;

        break;

    case 0x00: /* indefinite length end tag -> ignore */
        break;

    default: /* unknown tag -> decoding error */
        goto exit_with_error;
    }

    if (endBufPos != NULL)
        *endBufPos = bufPos;

    return value;

exit_with_error:

    if (value != NULL)
        MmsValue_delete(value);

    return NULL;
}

MmsValue*
MmsValue_decodeMmsDataMaxRecursion(uint8_t* buffer, int bufPos, int bufferLength, int* endBufPos, int maxDepth)
{
    return MmsValue_decodeMmsDataRecursive(buffer, bufPos, bufferLength, endBufPos, 0, maxDepth);
}

MmsValue*
MmsValue_decodeMmsData(uint8_t* buffer, int bufPos, int bufferLength, int* endBufPos)
{
    return MmsValue_decodeMmsDataMaxRecursion(buffer, bufPos, bufferLength, endBufPos, 25);
}

static int
MmsValue_getMaxStructSize(MmsValue* self)
{
    int componentsSize = 0;
    int i;
    int size;

    int componentCount = self->value.structure.size;

    MmsValue** components = self->value.structure.components;

    for (i = 0; i < componentCount; i++)
        componentsSize += MmsValue_getMaxEncodedSize(components[i]);

    size = 1 + componentsSize + BerEncoder_determineLengthSize(componentsSize);

    return size;
}

int
MmsValue_getMaxEncodedSize(MmsValue* self)
{
    int size = 0;
    int elementSize = 0;

    switch (self->type)
    {
    case MMS_STRUCTURE:
        size = MmsValue_getMaxStructSize(self);
        break;
    case MMS_ARRAY:
        size = MmsValue_getMaxStructSize(self);
        break;
    case MMS_BOOLEAN:
        size = 3;
        break;
    case MMS_DATA_ACCESS_ERROR:
        size = 7; /* TL * size of uint32 max */
        break;
    case MMS_VISIBLE_STRING:
        elementSize = abs(self->value.visibleString.size);
        size = 1 + elementSize + BerEncoder_determineLengthSize(elementSize);
        break;
    case MMS_UNSIGNED:
        size = 2 + self->value.integer->maxSize;
        break;
    case MMS_INTEGER:
        size = 2 + self->value.integer->maxSize;
        break;
    case MMS_UTC_TIME:
        size = 10;
        break;
    case MMS_BIT_STRING:
        elementSize = abs(self->value.bitString.size);
        size = BerEncoder_determineEncodedBitStringSize(elementSize);
        break;
    case MMS_BINARY_TIME:
        size = 2 + self->value.binaryTime.size;
        break;
    case MMS_OCTET_STRING:
        elementSize = abs(self->value.octetString.maxSize);
        size = 1 + BerEncoder_determineLengthSize(elementSize) + elementSize;
        break;
    case MMS_FLOAT:
        elementSize = (self->value.floatingPoint.formatWidth / 8) + 1;
        size = elementSize + 2; /* 2 for tag and length */
        break;
    case MMS_STRING:
        elementSize = abs(self->value.visibleString.size);
        size = 1 + elementSize + BerEncoder_determineLengthSize(elementSize);
        break;
    default:
        if (DEBUG_MMS_SERVER)
            printf("MmsVariableSpecification_getMaxEncodedSize: error unsupported type!\n");
        break;
    }

    return size;
}

static int
getMaxStructSize(MmsVariableSpecification* variable)
{
    int componentsSize = 0;
    int i;
    int size;

    int componentCount = variable->typeSpec.structure.elementCount;

    MmsVariableSpecification** components = variable->typeSpec.structure.elements;

    for (i = 0; i < componentCount; i++)
        componentsSize += MmsVariableSpecification_getMaxEncodedSize(components[i]);

    size = 1 + componentsSize + BerEncoder_determineLengthSize(componentsSize);

    return size;
}

int
MmsVariableSpecification_getMaxEncodedSize(MmsVariableSpecification* self)
{
    int size = 0;
    int elementSize = 0;

    switch (self->type)
    {
    case MMS_STRUCTURE:
        size = getMaxStructSize(self);
        break;
    case MMS_ARRAY:
        elementSize = MmsVariableSpecification_getMaxEncodedSize(self->typeSpec.array.elementTypeSpec)
            * self->typeSpec.array.elementCount;
        size = 1 + elementSize + BerEncoder_determineLengthSize(elementSize);
        break;
    case MMS_BOOLEAN:
        size = 3;
        break;
    case MMS_DATA_ACCESS_ERROR:
        size = 7; /* TL * size of uint32 max */
        break;
    case MMS_VISIBLE_STRING:
        elementSize = abs(self->typeSpec.visibleString);
        size = 1 + elementSize + BerEncoder_determineLengthSize(elementSize);
        break;
    case MMS_UNSIGNED:
        size = 2 + (self->typeSpec.unsignedInteger / 8) + 1;
        break;
    case MMS_INTEGER:
        size = 2 + (self->typeSpec.integer / 8) + 1;
        break;
    case MMS_UTC_TIME:
        size = 10;
        break;
    case MMS_BIT_STRING:
        elementSize = abs(self->typeSpec.bitString);
        size = BerEncoder_determineEncodedBitStringSize(elementSize);
        break;
    case MMS_BINARY_TIME:
        size = 2 + self->typeSpec.binaryTime;
        break;
    case MMS_OCTET_STRING:
        elementSize = abs(self->typeSpec.octetString);
        size = 1 + BerEncoder_determineLengthSize(elementSize) + elementSize;
        break;
    case MMS_FLOAT:
        elementSize = (self->typeSpec.floatingpoint.formatWidth / 8) + 1;
        size = elementSize + 2; /* 2 for tag and length */
        break;
    case MMS_STRING:
        elementSize = abs(self->typeSpec.mmsString);
        size = 1 + elementSize + BerEncoder_determineLengthSize(elementSize);
        break;
    default:
        if (DEBUG_MMS_SERVER)
            printf("MmsVariableSpecification_getMaxEncodedSize: error unsupported type!\n");
        break;
    }

    return size;
}

int
MmsValue_encodeMmsData(MmsValue* self, uint8_t* buffer, int bufPos, bool encode)
{
    int size = 0;

    switch (self->type) {
    case MMS_BOOLEAN:
        if (encode)
            bufPos = BerEncoder_encodeBoolean(0x83, self->value.boolean, buffer, bufPos);
        else
            size = 3;
        break;

    case MMS_STRUCTURE:
        if (encode)
            bufPos = encodeStructuredAccessResult(self, buffer, bufPos, true);
        else
            size = encodeStructuredAccessResult(self, buffer, bufPos, false);

        break;

    case MMS_ARRAY:
        if (encode)
            bufPos = encodeArrayAccessResult(self, buffer, bufPos, true);
        else
            size = encodeArrayAccessResult(self, buffer, bufPos, false);
        break;

    case MMS_DATA_ACCESS_ERROR:
        if (encode) {
            int length = BerEncoder_UInt32determineEncodedSize((uint32_t) self->value.dataAccessError);

            bufPos = BerEncoder_encodeTL(0x80, length, buffer, bufPos);
            bufPos = BerEncoder_encodeUInt32((uint32_t) self->value.dataAccessError, buffer, bufPos);
        }
        else
            size = 2 + BerEncoder_UInt32determineEncodedSize((uint32_t) self->value.dataAccessError);
        break;

    case MMS_VISIBLE_STRING:
        if (encode)
            bufPos = BerEncoder_encodeStringWithTag(0x8a, self->value.visibleString.buf, buffer, bufPos);
        else
            size = BerEncoder_determineEncodedStringSize(self->value.visibleString.buf);
        break;
    case MMS_UNSIGNED:
        if (encode)
            bufPos = BerEncoder_encodeAsn1PrimitiveValue(0x86, self->value.integer, buffer, bufPos);
        else
            size = 2 + self->value.integer->size;
        break;
    case MMS_INTEGER:
        if (encode)
            bufPos = BerEncoder_encodeAsn1PrimitiveValue(0x85, self->value.integer, buffer, bufPos);
        else
            size = 2 + self->value.integer->size;
        break;
    case MMS_UTC_TIME:
        if (encode)
            bufPos = BerEncoder_encodeOctetString(0x91, self->value.utcTime, 8, buffer, bufPos);
        else
            size = 10;
        break;
    case MMS_BIT_STRING:
        if (encode)
            bufPos = BerEncoder_encodeBitString(0x84, self->value.bitString.size,
                    self->value.bitString.buf, buffer, bufPos);
        else
            size = BerEncoder_determineEncodedBitStringSize(self->value.bitString.size);
        break;

    case MMS_BINARY_TIME:
        if (encode)
            bufPos = BerEncoder_encodeOctetString(0x8c, self->value.binaryTime.buf,
                    self->value.binaryTime.size, buffer, bufPos);
        else
            size = 2 + self->value.binaryTime.size;
        break;
    case MMS_OCTET_STRING:
        if (encode)
            bufPos = BerEncoder_encodeOctetString(0x89, self->value.octetString.buf,
                    self->value.octetString.size, buffer, bufPos);
        else
            size = 1 + BerEncoder_determineLengthSize(self->value.octetString.size)
                        + self->value.octetString.size;
        break;

    case MMS_FLOAT:
        {
            int floatSize = (self->value.floatingPoint.formatWidth / 8) + 1;

            if (encode) {
                bufPos = BerEncoder_encodeTL(0x87, floatSize, buffer, bufPos);
                bufPos = BerEncoder_encodeFloat(self->value.floatingPoint.buf,
                        self->value.floatingPoint.formatWidth,
                        self->value.floatingPoint.exponentWidth,
                        buffer, bufPos);
            }
            else
                size = floatSize + 2; /* 2 for tag and length */
        }
        break;
    case MMS_STRING:
        if (encode)
            bufPos = BerEncoder_encodeStringWithTag(0x90, self->value.visibleString.buf, buffer, bufPos);
        else
            size = BerEncoder_determineEncodedStringSize(self->value.visibleString.buf);
        break;
    default:
        if (DEBUG_MMS_SERVER)
            printf("MmsValue_encodeMmsData: error unsupported type!\n");
        size = 0;
        break;
    }

    if (encode)
        return bufPos;
    else
        return size;
}
