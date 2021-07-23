/*
 *  ber_integer.c
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
#include "ber_integer.h"
#include "ber_encoder.h"

static int
setIntegerValue(Asn1PrimitiveValue* self, uint8_t* valueBuffer, int bufferSize)
{
#if (ORDER_LITTLE_ENDIAN == 1)
    BerEncoder_revertByteOrder(valueBuffer, bufferSize);
#endif

    int size = BerEncoder_compressInteger(valueBuffer, bufferSize);

    if (size <= self->maxSize) {
        self->size = size;
        memcpy(self->octets, valueBuffer, size);
        return 1;
    }
    else
        return 0;
}

Asn1PrimitiveValue*
BerInteger_createInt32()
{
    return Asn1PrimitiveValue_create(5);
}

Asn1PrimitiveValue*
BerInteger_createFromBuffer(uint8_t* buf, int size)
{
    int maxSize;

    if (size > 8)
        maxSize = size;
    else
        maxSize = 8;

    Asn1PrimitiveValue* self = Asn1PrimitiveValue_create(maxSize);

    if (self) {
        memcpy(self->octets, buf, size);

        self->size = size;
    }

    return self;
}

int
BerInteger_setFromBerInteger(Asn1PrimitiveValue* self, Asn1PrimitiveValue* value)
{
    if (self->maxSize >= value->size) {
        self->size = value->size;

        memcpy(self->octets, value->octets, value->size);

        return 1;
    }
    else
        return 0;
}

int
BerInteger_setInt32(Asn1PrimitiveValue* self, int32_t value)
{
    int32_t valueCopy = value;
    uint8_t* valueBuffer = (uint8_t*) &valueCopy;

    return setIntegerValue(self, valueBuffer, sizeof(value));
}

Asn1PrimitiveValue*
BerInteger_createFromInt32(int32_t value)
{
    Asn1PrimitiveValue* asn1Value = BerInteger_createInt32();

    if (asn1Value) {
        BerInteger_setInt32(asn1Value, value);
    }

    return asn1Value;
}

int
BerInteger_setUint16(Asn1PrimitiveValue* self, uint16_t value)
{
    uint16_t valueCopy = value;
    uint8_t* valueBuffer = (uint8_t*) &valueCopy;

    uint8_t byteBuffer[3];

    int i;

#if (ORDER_LITTLE_ENDIAN == 1)
    byteBuffer[2] = 0;

    for (i = 0; i < 2; i++)
        byteBuffer[i] = valueBuffer[i];
#else
    byteBuffer[0] = 0;

    for (i = 0; i < 2; i++)
        byteBuffer[i + 1] = valueBuffer[i];
#endif /* (ORDER_LITTLE_ENDIAN == 1) */

    return setIntegerValue(self, byteBuffer, sizeof(byteBuffer));
}

int
BerInteger_setUint8(Asn1PrimitiveValue* self, uint8_t value)
{
    uint8_t valueCopy = value;
    uint8_t* valueBuffer = (uint8_t*) &valueCopy;

    return setIntegerValue(self, valueBuffer, sizeof(value));
}

int
BerInteger_setUint32(Asn1PrimitiveValue* self, uint32_t value)
{
    uint32_t valueCopy = value;
    uint8_t* valueBuffer = (uint8_t*) &valueCopy;

    uint8_t byteBuffer[5];

    int i;

#if (ORDER_LITTLE_ENDIAN == 1)
    byteBuffer[4] = 0;

    for (i = 0; i < 4; i++)
        byteBuffer[i] = valueBuffer[i];
#else
    byteBuffer[0] = 0;

    for (i = 0; i < 4; i++)
        byteBuffer[i + 1] = valueBuffer[i];
#endif /* (ORDER_LITTLE_ENDIAN == 1) */

    return setIntegerValue(self, byteBuffer, 5);
}

Asn1PrimitiveValue*
BerInteger_createFromUint32(uint32_t value)
{
    Asn1PrimitiveValue* asn1Value = BerInteger_createInt32();

    if (asn1Value) {
        BerInteger_setUint32(asn1Value, value);
    }

    return asn1Value;
}

Asn1PrimitiveValue*
BerInteger_createInt64()
{
    return Asn1PrimitiveValue_create(9);
}

int
BerInteger_setInt64(Asn1PrimitiveValue* self, int64_t value)
{
    int64_t valueCopy = value;
    uint8_t* valueBuffer = (uint8_t*) &valueCopy;

    return setIntegerValue(self, valueBuffer, sizeof(value));
}

Asn1PrimitiveValue*
BerInteger_createFromInt64(int64_t value)
{
    Asn1PrimitiveValue* asn1Value = BerInteger_createInt64();

    if (asn1Value) {
        BerInteger_setInt64(asn1Value, value);
    }

    return asn1Value;
}

void
BerInteger_toInt32(Asn1PrimitiveValue* self, int32_t* nativeValue)
{
    uint8_t* buf = self->octets;
    int i;

    if (buf[0] & 0x80) /* sign extension */
        *nativeValue = 0xffffffff;
    else
        *nativeValue = 0;

    for (i = 0; i < self->size; i++)
        *nativeValue = (*nativeValue << 8) | buf[i];
}

void
BerInteger_toUint32(Asn1PrimitiveValue* self, uint32_t* nativeValue)
{
    uint8_t* buf = self->octets;
    int i;

    *nativeValue = 0;

    for (i = 0; i < self->size; i++)
        *nativeValue = (*nativeValue << 8) | buf[i];
}

void
BerInteger_toInt64(Asn1PrimitiveValue* self, int64_t* nativeValue)
{
    uint8_t* buf = self->octets;
    int i;

    if (buf[0] & 0x80) /* sign extension */
        *nativeValue = 0xffffffffffffffff;
    else
        *nativeValue = 0;

    for (i = 0; i < self->size; i++)
        *nativeValue = (*nativeValue << 8) | buf[i];
}

