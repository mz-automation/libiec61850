/*
 *  ber_encoder.c
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
#include "ber_encoder.h"

int
BerEncoder_encodeLength(uint32_t length, uint8_t* buffer, int bufPos)
{
    if (length < 128) {
        buffer[bufPos++] = (uint8_t) length;
    }
    else if (length < 256) {
        buffer[bufPos++] = 0x81;
        buffer[bufPos++] = (uint8_t) length;
    }
    else if (length < 65535) {
        buffer[bufPos++] = 0x82;

        buffer[bufPos++] = (uint8_t) (length / 256);
        buffer[bufPos++] = (uint8_t) (length % 256);
    }
    else {
        buffer[bufPos++] = 0x83;

        buffer[bufPos++] = (uint8_t) (length / 0x10000);
        buffer[bufPos++] = (uint8_t) ((length & 0xffff) / 0x100);
        buffer[bufPos++] = (uint8_t) (length % 256);
    }

    return bufPos;
}

int
BerEncoder_encodeTL(uint8_t tag, uint32_t length, uint8_t* buffer, int bufPos)
{
    buffer[bufPos++] = tag;
    bufPos = BerEncoder_encodeLength(length, buffer, bufPos);

    return bufPos;
}

int
BerEncoder_encodeBoolean(uint8_t tag, bool value, uint8_t* buffer, int bufPos)
{
    buffer[bufPos++] = tag;
    buffer[bufPos++] = 1;

    if (value)
        buffer[bufPos++] = 0xff;
    else
        buffer[bufPos++] = 0x00;

    return bufPos;
}

int
BerEncoder_encodeStringWithTag(uint8_t tag, const char* string, uint8_t* buffer, int bufPos)
{
    buffer[bufPos++] = tag;

    if (string != NULL) {
        int length = (int) strlen(string);

        bufPos = BerEncoder_encodeLength((uint32_t) length, buffer, bufPos);

        int i;
        for (i = 0; i < length; i++) {
            buffer[bufPos++] = (uint8_t) string[i];
        }
    }
    else
        buffer[bufPos++] = 0;

    return bufPos;
}

int
BerEncoder_encodeAsn1PrimitiveValue(uint8_t tag, Asn1PrimitiveValue* value, uint8_t* buffer, int bufPos)
{
    buffer[bufPos++] = tag;

    bufPos = BerEncoder_encodeLength(value->size, buffer, bufPos);

    int i;
    for (i = 0; i < value->size; i++) {
        buffer[bufPos++] = value->octets[i];
    }

    return bufPos;
}

int
BerEncoder_encodeOctetString(uint8_t tag, uint8_t* octetString, uint32_t octetStringSize, uint8_t* buffer, int bufPos)
{
    buffer[bufPos++] = tag;

    bufPos = BerEncoder_encodeLength(octetStringSize, buffer, bufPos);

    uint32_t i;
    for (i = 0; i < octetStringSize; i++) {
        buffer[bufPos++] = octetString[i];
    }

    return bufPos;
}

int
BerEncoder_encodeBitString(uint8_t tag, int bitStringSize, uint8_t* bitString, uint8_t* buffer, int bufPos)
{
    buffer[bufPos++] = tag;

    int byteSize = bitStringSize / 8;

    if (bitStringSize % 8)
        byteSize++;

    int padding = (byteSize * 8) - bitStringSize;

    bufPos = BerEncoder_encodeLength(byteSize + 1, buffer, bufPos);

    buffer[bufPos++] = padding;

    int i;

    for (i = 0; i < byteSize; i++) {
        buffer[bufPos++] = bitString[i];
    }

    uint8_t paddingMask = 0;

    for (i = 0; i < padding; i++) {
        paddingMask += (1 << i);
    }

    paddingMask = ~paddingMask;

    buffer[bufPos -1] = buffer[bufPos -1] & paddingMask;

    return bufPos;
}

int
BerEncoder_determineEncodedBitStringSize(int bitStringSize)
{
    int size = 2; /* for tag and padding */

    int byteSize = bitStringSize / 8;

    if (bitStringSize % 8)
        byteSize++;

    size += BerEncoder_determineLengthSize(byteSize);

    size += byteSize;

    return size;
}

void
BerEncoder_revertByteOrder(uint8_t* octets, const int size)
{
    int i;
    uint8_t temp;

    for (i = 0; i < size / 2; i++) {
        temp = octets[i];
        octets[i] = octets[(size - 1) - i];
        octets[(size - 1) - i] = temp;
    }
}


int
BerEncoder_compressInteger(uint8_t* integer, int originalSize)
{
    uint8_t* integerEnd = integer + originalSize - 1;
    uint8_t* bytePosition;

    for (bytePosition = integer; bytePosition < integerEnd; bytePosition++) {

        if (bytePosition[0] == 0x00) {
            if ((bytePosition[1] & 0x80) == 0)
                continue;
        }
        else if (bytePosition[0] == 0xff) {
            if ((bytePosition[1] & 0x80) == 0x80)
                continue;
        }

        break;
    }

    int bytesToDelete = bytePosition - integer;
    int newSize = originalSize;

    if (bytesToDelete) {
        newSize -= bytesToDelete;
        uint8_t* newEnd = integer + newSize;

        uint8_t *newBytePosition;

        for(newBytePosition = integer; newBytePosition < newEnd; newBytePosition++) {
            *newBytePosition = *bytePosition;
             bytePosition++;
        }

    }

    return newSize;
}

int
BerEncoder_encodeUInt32(uint32_t value, uint8_t* buffer, int bufPos)
{
    uint8_t* valueArray = (uint8_t*) &value;
    uint8_t valueBuffer[5];

    valueBuffer[0] = 0;

    int i;
    for (i = 0; i < 4; i++) {
        valueBuffer[i + 1] = valueArray[i];
    }

#if (ORDER_LITTLE_ENDIAN == 1)
    BerEncoder_revertByteOrder(valueBuffer + 1, 4);
#endif

    int size = BerEncoder_compressInteger(valueBuffer, 5);

    for (i = 0; i < size; i++) {
        buffer[bufPos++] = valueBuffer[i];
    }

    return bufPos;
}

int
BerEncoder_encodeInt32(int32_t value, uint8_t* buffer, int bufPos)
{
    uint8_t* valueArray = (uint8_t*) &value;
    uint8_t valueBuffer[4];

    int i;

#if (ORDER_LITTLE_ENDIAN == 1)
    for (i = 0; i < 4; i++) {
        valueBuffer[3 - i] = valueArray[i];
    }
#else
    for (i = 0; i < 4; i++) {
        valueBuffer[i] = valueArray[i];
    }
#endif

    int size = BerEncoder_compressInteger(valueBuffer, 4);

    for (i = 0; i < size; i++) {
        buffer[bufPos++] = valueBuffer[i];
    }

    return bufPos;
}

int
BerEncoder_encodeUInt32WithTL(uint8_t tag, uint32_t value, uint8_t* buffer, int bufPos)
{
    uint8_t* valueArray = (uint8_t*) &value;
    uint8_t valueBuffer[5];

    valueBuffer[0] = 0;

    int i;
    for (i = 0; i < 4; i++) {
        valueBuffer[i + 1] = valueArray[i];
    }

#if (ORDER_LITTLE_ENDIAN == 1)
    BerEncoder_revertByteOrder(valueBuffer + 1, 4);
#endif

    int size = BerEncoder_compressInteger(valueBuffer, 5);

    buffer[bufPos++] = tag;
    buffer[bufPos++] = (uint8_t) size;

    for (i = 0; i < size; i++) {
        buffer[bufPos++] = valueBuffer[i];
    }

    return bufPos;
}

int
BerEncoder_encodeFloat(uint8_t* floatValue, uint8_t formatWidth, uint8_t exponentWidth,
        uint8_t* buffer, int bufPos)
{
    uint8_t* valueBuffer = buffer + bufPos;

    int byteSize = formatWidth / 8;

    valueBuffer[0] = exponentWidth;

    int i;
    for (i = 0; i < byteSize; i++) {
        valueBuffer[i + 1] = floatValue[i];
    }

#if (ORDER_LITTLE_ENDIAN == 1)
    BerEncoder_revertByteOrder(valueBuffer + 1, byteSize);
#endif

    bufPos = bufPos + 1 + byteSize;

    return bufPos;
}

int
BerEncoder_UInt32determineEncodedSize(uint32_t value)
{
    uint8_t* valueArray = (uint8_t*) &value;
    uint8_t valueBuffer[5];

    valueBuffer[0] = 0;

    int i;
    for (i = 0; i < 4; i++) {
       valueBuffer[i + 1] = valueArray[i];
    }

#if (ORDER_LITTLE_ENDIAN == 1)
    BerEncoder_revertByteOrder(valueBuffer + 1, 4);
#endif

    int size = BerEncoder_compressInteger(valueBuffer, 5);

    return size;
}

int
BerEncoder_determineLengthSize(uint32_t length)
{
    if (length < 128)
        return 1;
    if (length < 256)
        return 2;
    if (length < 65536)
        return 3;
    else
        return 4;
}

int
BerEncoder_determineEncodedStringSize(const char* string)
{
    if (string != NULL) {
        int size = 1;

        int length = strlen(string);

        size += BerEncoder_determineLengthSize(length);

        size += length;

        return size;
    }
    else
        return 2;
}

int
BerEncoder_encodeOIDToBuffer(const char* oidString, uint8_t* buffer, int maxBufLen)
{
    int encodedBytes = 0;

    int x = atoi(oidString);

    char sepChar = '.';

    const char* separator = strchr(oidString, '.');

    if (separator == NULL) {
        sepChar = ',';
        separator = strchr(oidString, ',');
    }

    if (separator == NULL) {
        sepChar = ' ';
        separator = strchr(oidString, ' ');
    }

    if (separator == NULL) return 0;

    int y = atoi(separator + 1);

    int val = x * 40 + y;

    if (encodedBytes < maxBufLen)
        buffer[encodedBytes] = (uint8_t) val;
    else
        return 0;

    encodedBytes++;

    while (1) {
        separator = strchr(separator + 1, sepChar);

        if (separator == NULL)
            break;

        val = atoi(separator + 1);

        int requiredBytes = 0;

        if (val == 0) {
            buffer[encodedBytes++] = 0;
        }
        else {
            int val2 = val;
            while (val2 > 0) {
                requiredBytes++;
                val2 = val2 >> 7;
            }

            while (requiredBytes > 0) {
                val2 = val >> (7 * (requiredBytes - 1));

                val2 = val2 & 0x7f;

                if (requiredBytes > 1)
                    val2 += 128;

                if (encodedBytes == maxBufLen)
                    return 0;

                buffer[encodedBytes++] = (uint8_t) val2;

                requiredBytes--;
            }
        }

    }

    return encodedBytes;
}
