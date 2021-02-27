/*
 *  mms_value.c
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
#include "mms_value.h"
#include "mms_type_spec.h"

#include "mms_common_internal.h"

#include "mms_value_internal.h"

#include "conversions.h"

#include "simple_allocator.h"

#include <time.h> /* for ctime_r */

static int
bitStringByteSize(const MmsValue* value)
{
    int bitSize = value->value.bitString.size;
    return (bitSize / 8) + ((bitSize % 8) > 0);
}

int
MmsValue_getBitStringByteSize(const MmsValue* self)
{
    return bitStringByteSize(self);
}

static bool
updateStructuredComponent(MmsValue* self, const MmsValue* update)
{
    MmsValue** selfValues;
    MmsValue** updateValues;

    if (self->value.structure.size != update->value.structure.size)
        return false;

    selfValues = self->value.structure.components;
    updateValues = update->value.structure.components;

    int i;
    for (i = 0; i < self->value.structure.size; i++) {
        if (MmsValue_update(selfValues[i], updateValues[i]) == false)
            return false;
    }

    return true;
}

MmsValue*
MmsValue_newIntegerFromBerInteger(Asn1PrimitiveValue* berInteger)
{
    MmsValue* self = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));

    if (self == NULL)
        return NULL;

    self->type = MMS_INTEGER;
    self->value.integer = berInteger;

    return self;
}

MmsValue*
MmsValue_newUnsignedFromBerInteger(Asn1PrimitiveValue* berInteger)
{
    MmsValue* self = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));

    if (self == NULL)
        return NULL;

    self->type = MMS_UNSIGNED;
    self->value.integer = berInteger;

    return self;
}

bool
MmsValue_equals(const MmsValue* self, const MmsValue* otherValue)
{
    if (self->type == otherValue->type) {
        switch (self->type)
        {
        case MMS_ARRAY:
        case MMS_STRUCTURE:
            if (self->value.structure.size == otherValue->value.structure.size) {
                int componentCount = self->value.structure.size;

                int i;
                for (i = 0; i < componentCount; i++) {
                    if (!MmsValue_equals(self->value.structure.components[i],
                            otherValue->value.structure.components[i]))
                        return false;
                }

                return true;
            }
            break;

        case MMS_BOOLEAN:
            if (self->value.boolean == otherValue->value.boolean)
                return true;
            break;
        case MMS_FLOAT:
            if (memcmp(self->value.floatingPoint.buf, otherValue->value.floatingPoint.buf,
                    self->value.floatingPoint.formatWidth / 8) == 0)
                return true;
            break;
        case MMS_INTEGER:
        case MMS_UNSIGNED:
            return Asn1PrimitivaValue_compare(self->value.integer, otherValue->value.integer);
            break;
        case MMS_UTC_TIME:
            if (memcmp(self->value.utcTime, otherValue->value.utcTime, 8) == 0)
                return true;
            break;
        case MMS_BIT_STRING:
            if (self->value.bitString.size == otherValue->value.bitString.size) {
                if (memcmp(self->value.bitString.buf, otherValue->value.bitString.buf,
                        bitStringByteSize(self)) == 0)
                    return true;

            }
            break;
        case MMS_BINARY_TIME:
            if (self->value.binaryTime.size == otherValue->value.binaryTime.size) {
                if (memcmp(self->value.binaryTime.buf, otherValue->value.binaryTime.buf,
                        self->value.binaryTime.size) == 0)
                    return true;
            }
            break;

        case MMS_OCTET_STRING:
            if (self->value.octetString.size == otherValue->value.octetString.size) {
                if (memcmp(self->value.octetString.buf, otherValue->value.octetString.buf,
                        self->value.octetString.size) == 0)
                    return true;
            }
            break;

        case MMS_VISIBLE_STRING:
        case MMS_STRING:
            if (self->value.visibleString.buf != NULL) {
                if (otherValue->value.visibleString.buf != NULL) {
                    if (strcmp(self->value.visibleString.buf, otherValue->value.visibleString.buf) == 0)
                        return true;
                }
            }
            else {
                if (otherValue->value.visibleString.buf == NULL)
                    return true;
            }
            break;

        case MMS_DATA_ACCESS_ERROR:
            if (self->value.dataAccessError == otherValue->value.dataAccessError)
                return true;
            break;

        default:
            break;
        }
        return false;

    }
    else
        return false;
}

bool
MmsValue_equalTypes(const MmsValue* self, const MmsValue* otherValue)
{
    if ((self == NULL) || (otherValue == NULL))
        return false;

    if (self->type == otherValue->type) {
        switch (self->type)
        {
        case MMS_ARRAY:
        case MMS_STRUCTURE:
            if (self->value.structure.size == otherValue->value.structure.size) {
                int componentCount = self->value.structure.size;

                int i;
                for (i = 0; i < componentCount; i++) {
                    if (!MmsValue_equalTypes(self->value.structure.components[i],
                            otherValue->value.structure.components[i]))
                        return false;
                }

                return true;
            }
            else
                return false;
            break;

        default:
            return true;
        }
    }
    else
        return false;
}

bool
MmsValue_update(MmsValue* self, const MmsValue* update)
{
    if (self && update) {
        if (self->type == update->type) {
            switch (self->type)
            {
            case MMS_STRUCTURE:
                case MMS_ARRAY:
                if (updateStructuredComponent(self, update) == false)
                    return false;
                break;
            case MMS_BOOLEAN:
                self->value.boolean = update->value.boolean;
                break;
            case MMS_FLOAT:
                if (self->value.floatingPoint.formatWidth == update->value.floatingPoint.formatWidth) {
                    self->value.floatingPoint.exponentWidth = update->value.floatingPoint.exponentWidth;
                    memcpy(self->value.floatingPoint.buf, update->value.floatingPoint.buf,
                            self->value.floatingPoint.formatWidth / 8);
                }
                else
                    return false;
                break;
            case MMS_INTEGER:
            case MMS_UNSIGNED:
                if (BerInteger_setFromBerInteger(self->value.integer, update->value.integer))
                    return true;
                else
                    return false;
                break;
            case MMS_UTC_TIME:
                memcpy(self->value.utcTime, update->value.utcTime, 8);
                break;
            case MMS_BIT_STRING:
                if (self->value.bitString.size == update->value.bitString.size)
                    memcpy(self->value.bitString.buf, update->value.bitString.buf, bitStringByteSize(self));
                else if (update->value.bitString.size != self->value.bitString.size) {
                    int i;

                    for (i = 0; i < update->value.bitString.size; i++) {
                        if (i < self->value.bitString.size)
                            MmsValue_setBitStringBit(self, i, MmsValue_getBitStringBit(update, i));
                        else
                            break;
                    }
                }
                else
                    return false;
                break;
            case MMS_OCTET_STRING:
                {
                    int size = update->value.octetString.size;

                    if (size > self->value.octetString.maxSize) {
                        GLOBAL_FREEMEM(self->value.octetString.buf);
                        self->value.octetString.buf = (uint8_t*) GLOBAL_MALLOC(size);

                        if (self->value.octetString.buf == NULL)
                            return false;

                        self->value.octetString.maxSize = size;
                    }

                    memcpy(self->value.octetString.buf, update->value.octetString.buf, size);

                    self->value.octetString.size = size;
                }
                break;
            case MMS_VISIBLE_STRING:
                MmsValue_setVisibleString(self, update->value.visibleString.buf);
                break;
            case MMS_STRING:
                MmsValue_setMmsString(self, update->value.visibleString.buf);
                break;
            case MMS_BINARY_TIME:
                self->value.binaryTime.size = update->value.binaryTime.size;
                memcpy(self->value.binaryTime.buf, update->value.binaryTime.buf,
                        update->value.binaryTime.size);
                break;
            default:
                return false;
                break;
            }
            return true;
        }
        else
            return false;
    }
    else
        return false;
}

MmsValue*
MmsValue_newDataAccessError(MmsDataAccessError accessError)
{
    MmsValue* self = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));

    if (self == NULL)
        return NULL;

    self->type = MMS_DATA_ACCESS_ERROR;
    self->value.dataAccessError = accessError;

    return self;
}

MmsValue*
MmsValue_newBitString(int bitSize)
{
    MmsValue* self = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));

    if (self == NULL)
        return NULL;

    self->type = MMS_BIT_STRING;
    self->value.bitString.size = abs(bitSize);
    self->value.bitString.buf = (uint8_t*) GLOBAL_CALLOC(bitStringByteSize(self), 1);

    if (self->value.bitString.buf == NULL) {
        GLOBAL_FREEMEM(self);
        self = NULL;
    }

    return self;
}

static int
getBitStringByteSize(const MmsValue* self)
{
    int byteSize;

    if (self->value.bitString.size % 8)
        byteSize = (self->value.bitString.size / 8) + 1;
    else
        byteSize = self->value.bitString.size / 8;

    return byteSize;
}

void
MmsValue_deleteAllBitStringBits(MmsValue* self)
{
    int byteSize = getBitStringByteSize(self);

    int i;
    for (i = 0; i < byteSize; i++) {
        self->value.bitString.buf[i] = 0;
    }
}

void
MmsValue_setAllBitStringBits(MmsValue* self)
{
    int byteSize = getBitStringByteSize(self);

    int i;
    for (i = 0; i < byteSize; i++) {
        self->value.bitString.buf[i] = 0xff;
    }

    int padding = (byteSize * 8) - self->value.bitString.size;

    uint8_t paddingMask = 0;

    for (i = 0; i < padding; i++) {
        paddingMask += (1 << i);
    }

    paddingMask = ~paddingMask;

    self->value.bitString.buf[byteSize - 1] = self->value.bitString.buf[byteSize - 1] & paddingMask;
}

int
MmsValue_getBitStringSize(const MmsValue* self)
{
    return self->value.bitString.size;
}

int
MmsValue_getNumberOfSetBits(const MmsValue* self)
{
    int setBitsCount = 0;

    int byteSize = getBitStringByteSize(self);

    int i;
    for (i = 0; i < byteSize; i++) {

        /* deal with wrong padding */
        if (i == (byteSize - 1)) {
            int paddingSize = (byteSize * 8) - self->value.bitString.size;

            uint8_t mask = 0;

            int k;

            for (k = 0; k < paddingSize; k++)
                mask += (1 << k);

            mask = ~mask;

            self->value.bitString.buf[i] = (self->value.bitString.buf[i] & mask);
        }

        uint8_t currentByte = self->value.bitString.buf[i];

        while (currentByte != 0) {
            if ((currentByte & 1) == 1)
                setBitsCount++;
            currentByte >>= 1;
        }
    }

    return setBitsCount;
}

void
MmsValue_setBitStringBit(MmsValue* self, int bitPos, bool value)
{
    if (bitPos < self->value.bitString.size) {
        int bytePos = bitPos / 8;
        int bitPosInByte = 7 - (bitPos % 8);

        int bitMask = (1 << bitPosInByte);

        if (value)
            self->value.bitString.buf[bytePos] |= bitMask;
        else
            self->value.bitString.buf[bytePos] &= (~bitMask);
    }
}

bool
MmsValue_getBitStringBit(const MmsValue* self, int bitPos)
{
    if (bitPos < self->value.bitString.size) {
        int bytePos = bitPos / 8;

        int bitPosInByte = 7 - (bitPos % 8);

        int bitMask = (1 << bitPosInByte);

        if ((self->value.bitString.buf[bytePos] & bitMask) > 0)
            return true;
        else
            return false;

    }
    else
        return false; /* out of range bits are always zero */
}

uint32_t
MmsValue_getBitStringAsInteger(const MmsValue* self)
{
    uint32_t value = 0;

    int bitPos;

    for (bitPos = 0; bitPos < self->value.bitString.size; bitPos++) {
        if (MmsValue_getBitStringBit(self, bitPos)) {
            value += (1 << bitPos);
        }
    }

    return value;
}

void
MmsValue_setBitStringFromInteger(MmsValue* self, uint32_t intValue)
{
    int bitPos;

    for (bitPos = 0; bitPos < self->value.bitString.size; bitPos++) {
        if ((intValue & 1) == 1)
            MmsValue_setBitStringBit(self, bitPos, true);
        else
            MmsValue_setBitStringBit(self, bitPos, false);

        intValue = intValue >> 1;
    }
}

uint32_t
MmsValue_getBitStringAsIntegerBigEndian(const MmsValue* self)
{
    uint32_t value = 0;

    int bitPos;
    int i = 0;

    for (bitPos = (self->value.bitString.size - 1); bitPos >= 0; bitPos--) {

        if (MmsValue_getBitStringBit(self, bitPos)) {
            value += (1 << i);
        }

        i++;
    }

    return value;
}

void
MmsValue_setBitStringFromIntegerBigEndian(MmsValue* self, uint32_t intValue)
{
    int bitPos;

    for (bitPos = (self->value.bitString.size - 1); bitPos >= 0; bitPos--) {
        if ((intValue & 1) == 1)
            MmsValue_setBitStringBit(self, bitPos, true);
        else
            MmsValue_setBitStringBit(self, bitPos, false);

        intValue = intValue >> 1;
    }
}

MmsValue*
MmsValue_newFloat(float value)
{
    MmsValue* self = (MmsValue*) GLOBAL_MALLOC(sizeof(MmsValue));

    if (self) {
        self->type = MMS_FLOAT;
        self->value.floatingPoint.formatWidth = 32;
        self->value.floatingPoint.exponentWidth = 8;

        memcpy(self->value.floatingPoint.buf, &value, sizeof(float));
    }

    return self;
}

void
MmsValue_setFloat(MmsValue* self, float newFloatValue)
{
    if (self->type == MMS_FLOAT) {
        if (self->value.floatingPoint.formatWidth == 32) {
            memcpy(self->value.floatingPoint.buf, &newFloatValue, sizeof(float));
        }
        else if (self->value.floatingPoint.formatWidth == 64) {
            double newDoubleValue = (double) newFloatValue;

            memcpy(self->value.floatingPoint.buf, &newDoubleValue, sizeof(double));
        }
    }
}

void
MmsValue_setDouble(MmsValue* self, double newDoubleValue)
{
    if (self->type == MMS_FLOAT) {
        if (self->value.floatingPoint.formatWidth == 32) {
            float newFloatValue = (float) newDoubleValue;

            memcpy(self->value.floatingPoint.buf, &newFloatValue, sizeof(float));
        }
        else if (self->value.floatingPoint.formatWidth == 64) {
            memcpy(self->value.floatingPoint.buf, &newDoubleValue, sizeof(double));
        }
    }
}

MmsValue*
MmsValue_newDouble(double value)
{
    MmsValue* self = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));

    if (self) {
        self->type = MMS_FLOAT;
        self->value.floatingPoint.formatWidth = 64;
        self->value.floatingPoint.exponentWidth = 11;

        memcpy(self->value.floatingPoint.buf, &value, sizeof(double));
    }

    return self;
}

MmsValue*
MmsValue_newIntegerFromInt8(int8_t integer)
{
    MmsValue* self = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));

    if (self) {
        self->type = MMS_INTEGER;
        self->value.integer = BerInteger_createFromInt32((int32_t) integer);

        if (self->value.integer == NULL) {
            GLOBAL_FREEMEM(self);
            self = NULL;
        }
    }

    return self;
}

MmsValue*
MmsValue_newIntegerFromInt16(int16_t integer)
{
    MmsValue* self = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));

    if (self) {
        self->type = MMS_INTEGER;
        self->value.integer = BerInteger_createFromInt32((int32_t) integer);

        if (self->value.integer == NULL) {
            GLOBAL_FREEMEM(self);
            self = NULL;
        }
    }

    return self;
}

void
MmsValue_setInt8(MmsValue* self, int8_t integer)
{
    if (self->type == MMS_INTEGER) {
        if (Asn1PrimitiveValue_getMaxSize(self->value.integer) >= 1) {
            BerInteger_setInt32(self->value.integer, (int32_t) integer);
        }
    }
}

void
MmsValue_setInt16(MmsValue* self, int16_t integer)
{
    if (self->type == MMS_INTEGER) {
        if (Asn1PrimitiveValue_getMaxSize(self->value.integer) >= 2) {
            BerInteger_setInt32(self->value.integer, (int32_t) integer);
        }
    }
}

void
MmsValue_setInt32(MmsValue* self, int32_t integer)
{
    if (self->type == MMS_INTEGER) {
        if (Asn1PrimitiveValue_getMaxSize(self->value.integer) >= 4) {
            BerInteger_setInt32(self->value.integer, integer);
        }
    }
}

void
MmsValue_setInt64(MmsValue* self, int64_t integer)
{
    if (self->type == MMS_INTEGER) {
        if (Asn1PrimitiveValue_getMaxSize(self->value.integer) >= 8) {
            BerInteger_setInt64(self->value.integer, integer);
        }
    }
}

void
MmsValue_setUint32(MmsValue* self, uint32_t integer)
{
    if (self->type == MMS_UNSIGNED) {
        if (Asn1PrimitiveValue_getMaxSize(self->value.integer) >= 4) {
            BerInteger_setUint32(self->value.integer, integer);
        }
    }
}

void
MmsValue_setUint16(MmsValue* self, uint16_t integer)
{
    if (self->type == MMS_UNSIGNED) {
        if (Asn1PrimitiveValue_getMaxSize(self->value.integer) >= 2) {
            BerInteger_setUint16(self->value.integer, integer);
        }
    }
}

void
MmsValue_setUint8(MmsValue* self, uint8_t integer)
{
    if (self->type == MMS_UNSIGNED) {
        if (Asn1PrimitiveValue_getMaxSize(self->value.integer) >= 1) {
            BerInteger_setUint8(self->value.integer, integer);
        }
    }

}

void
MmsValue_setBoolean(MmsValue* self, bool boolValue)
{
    self->value.boolean = boolValue;
}

bool
MmsValue_getBoolean(const MmsValue* self)
{
    return self->value.boolean;
}

MmsValue*
MmsValue_setUtcTime(MmsValue* self, uint32_t timeval)
{
    uint8_t* timeArray = (uint8_t*) &timeval;
    uint8_t* valueArray = self->value.utcTime;

#if (ORDER_LITTLE_ENDIAN == 1)
    memcpyReverseByteOrder(valueArray, timeArray, 4);
#else
    memcpy(valueArray, timeArray, 4);
#endif

    return self;
}

MmsValue*
MmsValue_setUtcTimeMs(MmsValue* self, uint64_t timeval)
{
    uint32_t timeval32 = (timeval / 1000LL);

    uint8_t* timeArray = (uint8_t*) &timeval32;
    uint8_t* valueArray = self->value.utcTime;

#if (ORDER_LITTLE_ENDIAN == 1)
    memcpyReverseByteOrder(valueArray, timeArray, 4);
#else
    memcpy(valueArray, timeArray, 4);
#endif

    uint32_t remainder = (timeval % 1000LL);
    uint32_t fractionOfSecond = (remainder) * 16777 + ((remainder * 216) / 1000);

    /* encode fraction of second */
    valueArray[4] = ((fractionOfSecond >> 16) & 0xff);
    valueArray[5] = ((fractionOfSecond >> 8) & 0xff);
    valueArray[6] = (fractionOfSecond & 0xff);

    /* encode time quality */
    valueArray[7] = 0x0a; /* 10 bit sub-second time accuracy */

    return self;
}

void
MmsValue_setUtcTimeQuality(MmsValue* self, uint8_t timeQuality)
{
    self->value.utcTime[7] = timeQuality;
}

uint8_t
MmsValue_getUtcTimeQuality(const MmsValue* self)
{
    return self->value.utcTime[7];
}

void
MmsValue_setUtcTimeByBuffer(MmsValue* self, const uint8_t* buffer)
{
    if (buffer) {
        uint8_t* valueArray = self->value.utcTime;

        int i;
        for (i = 0; i < 8; i++) {
            valueArray[i] = buffer[i];
        }
    }
}

uint8_t*
MmsValue_getUtcTimeBuffer(MmsValue* self)
{
    return self->value.utcTime;
}

uint64_t
MmsValue_getUtcTimeInMs(const MmsValue* self)
{
    uint32_t timeval32;
    const uint8_t* valueArray = self->value.utcTime;

#if (ORDER_LITTLE_ENDIAN == 1)
    memcpyReverseByteOrder((uint8_t*) &timeval32, valueArray, 4);
#else
    memcpy((uint8_t*) &timeval32, valueArray, 4);
#endif

    uint32_t fractionOfSecond = 0;

    fractionOfSecond = (valueArray[4] << 16);
    fractionOfSecond += (valueArray[5] << 8);
    fractionOfSecond += (valueArray[6]);

    uint32_t remainder = fractionOfSecond / 16777;

    uint64_t msVal = (timeval32 * 1000LL) + remainder;

    return msVal;
}

uint64_t
MmsValue_getUtcTimeInMsWithUs(const MmsValue* self, uint32_t* usec)
{
    uint32_t timeval32;
    const uint8_t* valueArray = self->value.utcTime;

#if (ORDER_LITTLE_ENDIAN == 1)
    memcpyReverseByteOrder((uint8_t*) &timeval32, valueArray, 4);
#else
    memcpy((uint8_t*)&timeval32, valueArray, 4);
#endif

    uint64_t fractionOfSecond = 0;

    fractionOfSecond = (valueArray[4] << 16);
    fractionOfSecond += (valueArray[5] << 8);
    fractionOfSecond += (valueArray[6]);

    uint64_t remainder = fractionOfSecond * 1000000ULL / 0x1000000ULL; /* in usec */

    uint64_t msVal = (timeval32 * 1000LL) + (remainder / 1000LL);

    if (usec != NULL)
        *usec = remainder % 1000LL;

    return msVal;
}

MmsValue*
MmsValue_newIntegerFromInt32(int32_t integer)
{
    MmsValue* self = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));

    if (self) {
        self->type = MMS_INTEGER;
        self->value.integer = BerInteger_createFromInt32(integer);

        if (self->value.integer == NULL) {
            GLOBAL_FREEMEM(self);
            self = NULL;
        }
    }

    return self;
}

MmsValue*
MmsValue_newUnsignedFromUint32(uint32_t integer)
{
    MmsValue* self = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));

    if (self) {
        self->type = MMS_UNSIGNED;
        self->value.integer = BerInteger_createFromUint32(integer);

        if (self->value.integer == NULL) {
            GLOBAL_FREEMEM(self);
            self = NULL;
        }
    }

    return self;
}

MmsValue*
MmsValue_newIntegerFromInt64(int64_t integer)
{
    MmsValue* self = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));

    if (self) {
        self->type = MMS_INTEGER;
        self->value.integer = BerInteger_createFromInt64(integer);

        if (self->value.integer == NULL) {
            GLOBAL_FREEMEM(self);
            self = NULL;
        }
    }

    return self;
}

/**
 * Convert signed integer to int32_t
 */
int32_t
MmsValue_toInt32(const MmsValue* self)
{
    int32_t integerValue = 0;

    if ((self->type == MMS_INTEGER) || (self->type == MMS_UNSIGNED))
        BerInteger_toInt32(self->value.integer, &integerValue);

    return integerValue;
}

uint32_t
MmsValue_toUint32(const MmsValue* self)
{
    uint32_t integerValue = 0;

    if ((self->type == MMS_INTEGER) || (self->type == MMS_UNSIGNED))
        BerInteger_toUint32(self->value.integer, &integerValue);

    return integerValue;
}

/**
 * Convert signed integer to int64_t and do sign extension if required
 */
int64_t
MmsValue_toInt64(const MmsValue* self)
{
    int64_t integerValue = 0;

    if ((self->type == MMS_INTEGER) || (self->type == MMS_UNSIGNED))
        BerInteger_toInt64(self->value.integer, &integerValue);

    return integerValue;
}

float
MmsValue_toFloat(const MmsValue* self)
{
    if (self->type == MMS_FLOAT) {
        if (self->value.floatingPoint.formatWidth == 32) {
            float val;

            memcpy(&val, self->value.floatingPoint.buf, sizeof(float));

            return val;
        }
        else if (self->value.floatingPoint.formatWidth == 64) {
            double val;

            memcpy(&val, self->value.floatingPoint.buf, sizeof(double));

            return (float) val;
        }
    }

    return 0.f;
}

double
MmsValue_toDouble(const MmsValue* self)
{
    if (self->type == MMS_FLOAT) {

        if (self->value.floatingPoint.formatWidth == 32) {
            float val;

            memcpy(&val, self->value.floatingPoint.buf, sizeof(float));

            return (double) val;
        }
        if (self->value.floatingPoint.formatWidth == 64) {
            double val;

            memcpy(&val, self->value.floatingPoint.buf, sizeof(double));

            return val;
        }
    }

    return (double) 0.f;
}

uint32_t
MmsValue_toUnixTimestamp(const MmsValue* self)
{
    uint32_t timestamp;
    uint8_t* timeArray = (uint8_t*) &timestamp;

#if (ORDER_LITTLE_ENDIAN == 1)
    timeArray[0] = self->value.utcTime[3];
    timeArray[1] = self->value.utcTime[2];
    timeArray[2] = self->value.utcTime[1];
    timeArray[3] = self->value.utcTime[0];
#else
    timeArray[0] = self->value.utcTime[0];
    timeArray[1] = self->value.utcTime[1];
    timeArray[2] = self->value.utcTime[2];
    timeArray[3] = self->value.utcTime[3];
#endif

    return timestamp;
}

int
MmsValue_getSizeInMemory(const MmsValue* self)
{
    int memorySize = MemoryAllocator_getAlignedSize(sizeof(MmsValue));

    switch (self->type)
    {
    case MMS_ARRAY:
    case MMS_STRUCTURE:
        {
            memorySize += (MemoryAllocator_getAlignedSize(sizeof(MmsValue*)) * self->value.structure.size);

            int i;
            for (i = 0; i < self->value.structure.size; i++)
                memorySize += MmsValue_getSizeInMemory(self->value.structure.components[i]);
        }
        break;

    case MMS_BIT_STRING:
        memorySize += MemoryAllocator_getAlignedSize(bitStringByteSize(self));
        break;

    case MMS_INTEGER:
    case MMS_UNSIGNED:
        memorySize += MemoryAllocator_getAlignedSize(sizeof(Asn1PrimitiveValue));
        memorySize += MemoryAllocator_getAlignedSize(self->value.integer->maxSize);
        break;

    case MMS_OCTET_STRING:
        memorySize += MemoryAllocator_getAlignedSize(self->value.octetString.maxSize);
        break;

    case MMS_STRING:
        case MMS_VISIBLE_STRING:
        memorySize += MemoryAllocator_getAlignedSize(strlen(self->value.visibleString.buf) + 1); /* add space for 0 character */
        break;

    default:
        break;
    }

    return memorySize;
}

uint8_t*
MmsValue_cloneToBuffer(const MmsValue* self, uint8_t* destinationAddress)
{
    MmsValue* newValue = (MmsValue*) destinationAddress;

    memcpy(destinationAddress, self, sizeof(MmsValue));
    destinationAddress += MemoryAllocator_getAlignedSize(sizeof(MmsValue));

    switch (self->type)
    {
    case MMS_ARRAY:
    case MMS_STRUCTURE:
        {
            newValue->value.structure.components = (MmsValue**) destinationAddress;
            destinationAddress += (sizeof(MmsValue*) * self->value.structure.size);

            int i;
            for (i = 0; i < self->value.structure.size; i++) {
                memcpy(&(newValue->value.structure.components[i]), &(destinationAddress), sizeof(MmsValue*));
                destinationAddress = MmsValue_cloneToBuffer(self->value.structure.components[i], destinationAddress);
            }
        }
        break;

    case MMS_BIT_STRING:
        memcpy(destinationAddress, self->value.bitString.buf, bitStringByteSize(self));
        newValue->value.bitString.buf = destinationAddress;
        destinationAddress += MemoryAllocator_getAlignedSize(bitStringByteSize(self));
        break;

    case MMS_INTEGER:
    case MMS_UNSIGNED:
        {
            newValue->value.integer = (Asn1PrimitiveValue*) destinationAddress;
            Asn1PrimitiveValue* newAsn1Value = (Asn1PrimitiveValue*) destinationAddress;
            memcpy(destinationAddress, self->value.integer, sizeof(Asn1PrimitiveValue));
            destinationAddress += MemoryAllocator_getAlignedSize(sizeof(Asn1PrimitiveValue));
            newAsn1Value->octets = destinationAddress;
            memcpy(destinationAddress, self->value.integer->octets, self->value.integer->maxSize);
            destinationAddress += MemoryAllocator_getAlignedSize(self->value.integer->maxSize);
        }
        break;

    case MMS_OCTET_STRING:
        newValue->value.octetString.buf = destinationAddress;
        memcpy(destinationAddress, self->value.octetString.buf, self->value.octetString.maxSize);
        destinationAddress += MemoryAllocator_getAlignedSize(self->value.octetString.maxSize);
        break;

    case MMS_STRING:
    case MMS_VISIBLE_STRING:
        newValue->value.visibleString.buf = (char*) destinationAddress;
        newValue->value.visibleString.size = self->value.visibleString.size;
        strcpy((char*) destinationAddress, self->value.visibleString.buf);
        destinationAddress += MemoryAllocator_getAlignedSize(strlen(self->value.visibleString.buf) + 1);
        break;

    default:
        break;
    }

    return destinationAddress;
}

MmsValue*
MmsValue_clone(const MmsValue* self)
{
    if (self == NULL)
        return NULL;

    MmsValue* newValue = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));

    if (newValue == NULL)
        goto exit_function;

    newValue->deleteValue = self->deleteValue;
    newValue->type = self->type;
    int size;

    switch (self->type)
    {

    case MMS_ARRAY:
    case MMS_STRUCTURE:
        {
            int componentCount = self->value.structure.size;
            newValue->value.structure.size = componentCount;
            newValue->value.structure.components = (MmsValue**) GLOBAL_CALLOC(componentCount, sizeof(MmsValue*));

            if (newValue->value.structure.components) {
                int i;
                for (i = 0; i < componentCount; i++) {

                    if (self->value.structure.components[i]) {
                        newValue->value.structure.components[i] =
                                MmsValue_clone(self->value.structure.components[i]);

                        if (newValue->value.structure.components[i] == NULL) {
                            MmsValue_delete(newValue);
                            newValue = NULL;
                            break;
                        }
                    }
                    else {
                        newValue->value.structure.components[i] = NULL;
                    }

                }
            }
            else {
                GLOBAL_FREEMEM(newValue);
                newValue = NULL;
            }

        }
        break;

    case MMS_INTEGER:
    case MMS_UNSIGNED:
        {
            newValue->value.integer = Asn1PrimitiveValue_clone(self->value.integer);

            if (newValue->value.integer == NULL) {
                GLOBAL_FREEMEM(newValue);
                newValue = NULL;
            }
        }
        break;

    case MMS_FLOAT:
        newValue->value.floatingPoint.formatWidth = self->value.floatingPoint.formatWidth;
        newValue->value.floatingPoint.exponentWidth = self->value.floatingPoint.exponentWidth;
        size = self->value.floatingPoint.formatWidth / 8;
        memcpy(newValue->value.floatingPoint.buf, self->value.floatingPoint.buf, size);
        break;

    case MMS_BIT_STRING:
        newValue->value.bitString.size = self->value.bitString.size;
        size = bitStringByteSize(self);
        newValue->value.bitString.buf = (uint8_t*) GLOBAL_MALLOC(size);
        memcpy(newValue->value.bitString.buf, self->value.bitString.buf, size);
        break;

    case MMS_BOOLEAN:
        newValue->value.boolean = self->value.boolean;
        break;

    case MMS_OCTET_STRING:
        size = self->value.octetString.size;
        newValue->value.octetString.size = size;
        newValue->value.octetString.maxSize = self->value.octetString.maxSize;
        newValue->value.octetString.buf = (uint8_t*) GLOBAL_MALLOC(self->value.octetString.maxSize);
        memcpy(newValue->value.octetString.buf, self->value.octetString.buf, size);
        break;

    case MMS_UTC_TIME:
        memcpy(newValue->value.utcTime, self->value.utcTime, 8);
        break;

    case MMS_BINARY_TIME:
        newValue->value.binaryTime.size = self->value.binaryTime.size;
        memcpy(newValue->value.binaryTime.buf, self->value.binaryTime.buf, 6);
        break;

    case MMS_VISIBLE_STRING:
    case MMS_STRING:
        size = self->value.visibleString.size;
        newValue->value.visibleString.buf = (char*) GLOBAL_MALLOC(size + 1);
        newValue->value.visibleString.size = size;
        strcpy(newValue->value.visibleString.buf, self->value.visibleString.buf);
        break;

    case MMS_DATA_ACCESS_ERROR:
        newValue->value.dataAccessError = self->value.dataAccessError;
        break;

    default:
        break;
    }

exit_function:
    return newValue;
}

uint32_t
MmsValue_getArraySize(const MmsValue* self)
{
    return self->value.structure.size;
}

void
MmsValue_delete(MmsValue* self)
{
    if (self == NULL)
        return;

    switch (self->type)
    {
    case MMS_INTEGER:
    case MMS_UNSIGNED:
        Asn1PrimitiveValue_destroy(self->value.integer);
        break;
    case MMS_BIT_STRING:
        if (self->value.bitString.buf != NULL)
            GLOBAL_FREEMEM(self->value.bitString.buf);
        break;
    case MMS_OCTET_STRING:
        GLOBAL_FREEMEM(self->value.octetString.buf);
        break;
    case MMS_VISIBLE_STRING:
    case MMS_STRING:
        if (self->value.visibleString.buf != NULL)
            GLOBAL_FREEMEM(self->value.visibleString.buf);
        break;
    case MMS_ARRAY:
    case MMS_STRUCTURE:
        {
            int componentCount = self->value.structure.size;
            int i;

            for (i = 0; i < componentCount; i++) {
                if (self->value.structure.components[i] != NULL)
                    MmsValue_delete(self->value.structure.components[i]);
            }
        }
        GLOBAL_FREEMEM(self->value.structure.components);
        break;
    default:
        break;
    }

    GLOBAL_FREEMEM(self);
}

/* delete only when deleteValue field set */
void
MmsValue_deleteConditional(MmsValue* self)
{
    if (self->deleteValue == 1) {

        switch (self->type)
        {
        case MMS_INTEGER:
        case MMS_UNSIGNED:
            Asn1PrimitiveValue_destroy(self->value.integer);
            break;
        case MMS_BIT_STRING:
            GLOBAL_FREEMEM(self->value.bitString.buf);
            break;
        case MMS_OCTET_STRING:
            GLOBAL_FREEMEM(self->value.octetString.buf);
            break;
        case MMS_VISIBLE_STRING:
        case MMS_STRING:
            if (self->value.visibleString.buf != NULL)
                GLOBAL_FREEMEM(self->value.visibleString.buf);
            break;
        case MMS_ARRAY:
        case MMS_STRUCTURE:
            {
                int componentCount = self->value.structure.size;
                int i;

                for (i = 0; i < componentCount; i++) {
                    if (self->value.structure.components[i] != NULL)
                        MmsValue_deleteConditional(self->value.structure.components[i]);
                }
            }
            GLOBAL_FREEMEM(self->value.structure.components);
            break;
        default:
            break;
        }

        GLOBAL_FREEMEM(self);
    }
}

MmsValue*
MmsValue_newInteger(int size /*integer size in bits*/)
{
    MmsValue* self = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));

    if (self) {
        self->type = MMS_INTEGER;

        if (size <= 32)
            self->value.integer = BerInteger_createInt32();
        else
            self->value.integer = BerInteger_createInt64();

        if (self->value.integer == NULL) {
            GLOBAL_FREEMEM(self);
            self = NULL;
        }
    }

    return self;
}

MmsValue*
MmsValue_newUnsigned(int size /*integer size in bits*/)
{
    MmsValue* self = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));

    if (self) {
        self->type = MMS_UNSIGNED;

        if (size <= 32)
            self->value.integer = BerInteger_createInt32();
        else
            self->value.integer = BerInteger_createInt64();

        if (self->value.integer == NULL) {
            GLOBAL_FREEMEM(self);
            self = NULL;
        }
    }

    return self;
}

MmsValue*
MmsValue_newBoolean(bool boolean)
{
    MmsValue* self = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));

    if (self) {
        self->type = MMS_BOOLEAN;

        if (boolean)
            self->value.boolean = 1;
        else
            self->value.boolean = 0;
    }

    return self;
}

MmsValue*
MmsValue_newOctetString(int size, int maxSize)
{
    MmsValue* self = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));

    if (self) {
        self->type = MMS_OCTET_STRING;
        self->value.octetString.size = size;
        self->value.octetString.maxSize = maxSize;
        self->value.octetString.buf = (uint8_t*) GLOBAL_CALLOC(1, maxSize);

        if (self->value.octetString.buf == NULL) {
            GLOBAL_FREEMEM(self);
            self = NULL;
        }
    }

    return self;
}

void
MmsValue_setOctetString(MmsValue* self, uint8_t* buf, int size)
{
    if (size <= self->value.octetString.maxSize) {
        memcpy(self->value.octetString.buf, buf, size);
        self->value.octetString.size = size;
    }
}

uint16_t
MmsValue_getOctetStringSize(const MmsValue* self)
{
    return self->value.octetString.size;
}

uint16_t
MmsValue_getOctetStringMaxSize(MmsValue* self)
{
    return self->value.octetString.maxSize;
}

uint8_t*
MmsValue_getOctetStringBuffer(MmsValue* self)
{
    return self->value.octetString.buf;
}

MmsValue*
MmsValue_newStructure(const MmsVariableSpecification* typeSpec)
{
    MmsValue* self = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));

    if (self) {
        self->type = MMS_STRUCTURE;
        int componentCount = typeSpec->typeSpec.structure.elementCount;
        self->value.structure.size = componentCount;
        self->value.structure.components = (MmsValue**) GLOBAL_CALLOC(componentCount, sizeof(MmsValue*));

        if (self->value.structure.components) {
            int i;
            for (i = 0; i < componentCount; i++) {
                self->value.structure.components[i] =
                        MmsValue_newDefaultValue(typeSpec->typeSpec.structure.elements[i]);

                if (self->value.structure.components[i] == NULL) {
                    MmsValue_delete(self);
                    self = NULL;
                    break;
                }
            }
        }
        else {
            GLOBAL_FREEMEM(self);
            self = NULL;
        }
    }

    return self;
}

MmsValue*
MmsValue_newDefaultValue(const MmsVariableSpecification* typeSpec)
{
    MmsValue* self = NULL;

    switch (typeSpec->type)
    {
    case MMS_INTEGER:
        self = MmsValue_newInteger(typeSpec->typeSpec.integer);
        break;

    case MMS_UNSIGNED:
        self = MmsValue_newUnsigned(typeSpec->typeSpec.unsignedInteger);
        break;

    case MMS_FLOAT:
        self = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));

        if (self == NULL)
            goto exit_function;

        self->type = MMS_FLOAT;
        self->value.floatingPoint.exponentWidth = typeSpec->typeSpec.floatingpoint.exponentWidth;
        self->value.floatingPoint.formatWidth = typeSpec->typeSpec.floatingpoint.formatWidth;
        memset(self->value.floatingPoint.buf, 0, 8);
        break;

    case MMS_BIT_STRING:
        self = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));

        if (self == NULL)
            goto exit_function;

        self->type = MMS_BIT_STRING;
        {
            int bitSize = abs(typeSpec->typeSpec.bitString);
            self->value.bitString.size = bitSize;
            int size = (bitSize / 8) + ((bitSize % 8) > 0);
            self->value.bitString.buf = (uint8_t*) GLOBAL_CALLOC(1, size);
        }
        break;

    case MMS_OCTET_STRING:
        self = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));

        if (self == NULL)
            goto exit_function;

        self->type = MMS_OCTET_STRING;

        if (typeSpec->typeSpec.octetString < 0)
            self->value.octetString.size = 0;
        else
            self->value.octetString.size = typeSpec->typeSpec.octetString;

        self->value.octetString.maxSize = abs(typeSpec->typeSpec.octetString);
        self->value.octetString.buf = (uint8_t*) GLOBAL_CALLOC(1, abs(typeSpec->typeSpec.octetString));

        if (self->value.octetString.buf == NULL) {
            GLOBAL_FREEMEM(self);
            self = NULL;
            goto exit_function;
        }

        break;

    case MMS_VISIBLE_STRING:
        self = MmsValue_newVisibleStringWithSize(abs(typeSpec->typeSpec.visibleString));
        break;

    case MMS_BOOLEAN:
        self = MmsValue_newBoolean(false);
        break;

    case MMS_UTC_TIME:
        self = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));

        if (self == NULL)
            goto exit_function;

        self->type = MMS_UTC_TIME;
        break;

    case MMS_ARRAY:
        self = MmsValue_createArray(typeSpec->typeSpec.array.elementTypeSpec,
                typeSpec->typeSpec.array.elementCount);
        break;

    case MMS_STRUCTURE:
        self = MmsValue_newStructure(typeSpec);
        break;

    case MMS_STRING:
        self = MmsValue_newMmsStringWithSize(abs(typeSpec->typeSpec.visibleString));
        break;

    case MMS_BINARY_TIME:
        if (typeSpec->typeSpec.binaryTime == 4)
            self = MmsValue_newBinaryTime(true);
        else
            self = MmsValue_newBinaryTime(false);
        break;

    default:
        break;
    }

    if (self != NULL)
        self->deleteValue = 0;

exit_function:
    return self;
}

static void
setVisibleStringValue(MmsValue* self, const char* string)
{
    if (self->value.visibleString.buf != NULL) {
        if (string != NULL) {

            int newStringSize = strlen(string);

            if (newStringSize > self->value.visibleString.size) {
                GLOBAL_FREEMEM(self->value.visibleString.buf);
                self->value.visibleString.buf = (char*) GLOBAL_MALLOC(newStringSize + 1);

                if (self->value.visibleString.buf == NULL)
                    goto exit_function;

                self->value.visibleString.size = newStringSize;
            }

            strncpy(self->value.visibleString.buf, string, self->value.visibleString.size + 1);
            self->value.visibleString.buf[self->value.visibleString.size] = 0;
        }
        else
            self->value.visibleString.buf[0] = 0;
    }

exit_function:
    return;
}

static MmsValue*
MmsValue_newString(const char* string, MmsType type)
{
    MmsValue* self = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));

    if (self == NULL)
        goto exit_function;

    self->type = type;

    if (string == NULL) {
        self->value.visibleString.size = 0;
        self->value.visibleString.buf = NULL;
    }
    else {
        int stringSize = strlen(string);

        self->value.visibleString.size = stringSize;
        self->value.visibleString.buf = (char*) GLOBAL_MALLOC(stringSize + 1);

        if (self->value.visibleString.buf == NULL) {
            GLOBAL_FREEMEM(self);
            self = NULL;
            goto exit_function;
        }

        setVisibleStringValue(self, string);
    }

exit_function:
    return self;
}

MmsValue*
MmsValue_newVisibleString(const char* string)
{
    return MmsValue_newString(string, MMS_VISIBLE_STRING);
}

static MmsValue*
MmsValue_newStringWithSize(int size, MmsType type)
{
    MmsValue* self = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));

    if (self == NULL)
        goto exit_function;

    self->type = type;

    self->value.visibleString.size = size;
    self->value.visibleString.buf = (char*) GLOBAL_MALLOC(size + 1);

    if (self->value.visibleString.buf == NULL) {
        GLOBAL_FREEMEM(self);
        self = NULL;
        goto exit_function;
    }

    self->value.visibleString.buf[0] = 0;

exit_function:
    return self;
}

MmsValue*
MmsValue_newVisibleStringWithSize(int size)
{
    return MmsValue_newStringWithSize(size, MMS_VISIBLE_STRING);
}

MmsValue*
MmsValue_newMmsString(char* string)
{
    return MmsValue_newString(string, MMS_STRING);
}

MmsValue*
MmsValue_newMmsStringWithSize(int size)
{
    return MmsValue_newStringWithSize(size, MMS_STRING);
}

MmsValue*
MmsValue_newBinaryTime(bool timeOfDay)
{
    MmsValue* self = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));

    if (self) {
        self->type = MMS_BINARY_TIME;

        if (timeOfDay == true)
            self->value.binaryTime.size = 4;
        else
            self->value.binaryTime.size = 6;
    }

    return self;
}

void
MmsValue_setBinaryTime(MmsValue* self, uint64_t timestamp)
{
    uint64_t mmsTime;

    if (timestamp > 441763200000LL)
        mmsTime = timestamp - (441763200000LL);
    else
        mmsTime = 0;

    uint8_t* binaryTimeBuf = self->value.binaryTime.buf;

    if (self->value.binaryTime.size == 6) {
        uint16_t daysDiff = mmsTime / (86400000LL);
        uint8_t* daysDiffBuf = (uint8_t*) &daysDiff;

#if (ORDER_LITTLE_ENDIAN == 1)
        binaryTimeBuf[4] = daysDiffBuf[1];
        binaryTimeBuf[5] = daysDiffBuf[0];
#else
        binaryTimeBuf[4] = daysDiffBuf[0];
        binaryTimeBuf[5] = daysDiffBuf[1];
#endif

    }

    uint32_t msSinceMidnight = mmsTime % (86400000LL);
    uint8_t* msSinceMidnightBuf = (uint8_t*) &msSinceMidnight;

#if (ORDER_LITTLE_ENDIAN == 1)
    binaryTimeBuf[0] = msSinceMidnightBuf[3];
    binaryTimeBuf[1] = msSinceMidnightBuf[2];
    binaryTimeBuf[2] = msSinceMidnightBuf[1];
    binaryTimeBuf[3] = msSinceMidnightBuf[0];
#else
    binaryTimeBuf[0] = msSinceMidnightBuf[0];
    binaryTimeBuf[1] = msSinceMidnightBuf[1];
    binaryTimeBuf[2] = msSinceMidnightBuf[2];
    binaryTimeBuf[3] = msSinceMidnightBuf[3];
#endif
}

uint64_t
MmsValue_getBinaryTimeAsUtcMs(const MmsValue* self)
{
    uint64_t timestamp = 0;

    const uint8_t* binaryTimeBuf = self->value.binaryTime.buf;

    if (self->value.binaryTime.size == 6) {

        uint16_t daysDiff;

        daysDiff = binaryTimeBuf[4] * 256;
        daysDiff += binaryTimeBuf[5];

        uint64_t mmsTime;

        mmsTime = daysDiff * (86400000LL);

        timestamp = mmsTime + (441763200000LL);
    }

    uint32_t msSinceMidnight = 0;

    msSinceMidnight = binaryTimeBuf[0] << 24;
    msSinceMidnight += binaryTimeBuf[1] << 16;
    msSinceMidnight += binaryTimeBuf[2] << 8;
    msSinceMidnight += binaryTimeBuf[3];

    timestamp += msSinceMidnight;

    return timestamp;
}

MmsDataAccessError
MmsValue_getDataAccessError(const MmsValue* self)
{
    return self->value.dataAccessError;
}

void
MmsValue_setMmsString(MmsValue* self, const char* string)
{
    if (self->type == MMS_STRING) {
        setVisibleStringValue(self, string);
    }
}

static MmsValue*
MmsValue_newStringFromByteArray(const uint8_t* byteArray, int size, MmsType type)
{
    MmsValue* self = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));

    if (self == NULL)
        goto exit_function;

    self->type = type;

    self->value.visibleString.size = size;

    self->value.visibleString.buf = StringUtils_createStringFromBuffer(byteArray, size);

    if (self->value.visibleString.buf == NULL) {
        GLOBAL_FREEMEM(self);
        self = NULL;
    }

exit_function:
    return self;
}

MmsValue*
MmsValue_newVisibleStringFromByteArray(uint8_t* byteArray, int size)
{
    return MmsValue_newStringFromByteArray(byteArray, size, MMS_VISIBLE_STRING);
}

MmsValue*
MmsValue_newMmsStringFromByteArray(uint8_t* byteArray, int size)
{
    return MmsValue_newStringFromByteArray(byteArray, size, MMS_STRING);
}

void
MmsValue_setVisibleString(MmsValue* self, const char* string)
{
    if (self->type == MMS_VISIBLE_STRING) {
        setVisibleStringValue(self, string);
    }
}

const char*
MmsValue_toString(MmsValue* self)
{
    if ((self->type == MMS_VISIBLE_STRING) || (self->type == MMS_STRING))
        return self->value.visibleString.buf;

    return NULL;
}

int
MmsValue_getStringSize(MmsValue* self)
{
    if ((self->type == MMS_VISIBLE_STRING) || (self->type == MMS_STRING))
        return self->value.visibleString.size;
    else
        return 0;
}

MmsValue*
MmsValue_newUtcTime(uint32_t timeval)
{
    MmsValue* self = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));

    if (self == NULL)
        goto exit_function;

    self->type = MMS_UTC_TIME;

    uint8_t* timeArray = (uint8_t*) &timeval;
    uint8_t* valueArray = self->value.utcTime;

#if (ORDER_LITTLE_ENDIAN == 1)
    valueArray[0] = timeArray[3];
    valueArray[1] = timeArray[2];
    valueArray[2] = timeArray[1];
    valueArray[3] = timeArray[0];
#else
    valueArray[0] = timeArray[0];
    valueArray[1] = timeArray[1];
    valueArray[2] = timeArray[2];
    valueArray[3] = timeArray[3];
#endif

exit_function:
    return self;
}

MmsValue*
MmsValue_newUtcTimeByMsTime(uint64_t timeval)
{
    MmsValue* self = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));

    if (self == NULL)
        return NULL;

    self->type = MMS_UTC_TIME;
    MmsValue_setUtcTimeMs(self, timeval);

    return self;
}

MmsValue*
MmsValue_createArray(MmsVariableSpecification* elementType, int size)
{
    MmsValue* self = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));

    if (self == NULL)
        goto exit_function;

    self->type = MMS_ARRAY;
    self->value.structure.size = size;
    self->value.structure.components = (MmsValue**) GLOBAL_CALLOC(size, sizeof(MmsValue*));

    if (self->value.structure.components == NULL) {
        GLOBAL_FREEMEM(self);
        self = NULL;
        goto exit_function;
    }

    int i;
    for (i = 0; i < size; i++) {
        self->value.structure.components[i] = MmsValue_newDefaultValue(elementType);

        if (self->value.structure.components[i] == NULL) {
            MmsValue_delete(self);
            self = NULL;
            goto exit_function;
        }
    }

exit_function:
    return self;
}

MmsValue*
MmsValue_createEmptyArray(int size)
{
    MmsValue* self = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));

    if (self == NULL)
        goto exit_function;

    self->type = MMS_ARRAY;
    self->value.structure.size = size;
    self->value.structure.components = (MmsValue**) GLOBAL_CALLOC(size, sizeof(MmsValue*));

    if (self->value.structure.components == NULL) {
        GLOBAL_FREEMEM(self);
        self = NULL;
        goto exit_function;
    }

    int i;
    for (i = 0; i < size; i++) {
        self->value.structure.components[i] = NULL;
    }

exit_function:
    return self;
}

MmsValue*
MmsValue_createEmptyStructure(int size)
{
    MmsValue* self = MmsValue_createEmptyArray(size);

    if (self == NULL)
        return NULL;

    self->type = MMS_STRUCTURE;

    return self;
}

void
MmsValue_setElement(MmsValue* complexValue, int index, MmsValue* elementValue)
{
    if ((complexValue->type != MMS_ARRAY) && (complexValue->type != MMS_STRUCTURE))
        return;

    if ((index < 0) || (index >= complexValue->value.structure.size))
        return;

    complexValue->value.structure.components[index] = elementValue;
}

MmsValue*
MmsValue_getElement(const MmsValue* complexValue, int index)
{
    if ((complexValue->type != MMS_ARRAY) && (complexValue->type != MMS_STRUCTURE))
        return NULL;

    if ((index < 0) || (index >= complexValue->value.structure.size))
        return NULL;

    return complexValue->value.structure.components[index];
}

void
MmsValue_setDeletable(MmsValue* self)
{
    self->deleteValue = 1;
}

void
MmsValue_setDeletableRecursive(MmsValue* self)
{
    if (self != NULL) {

        if ((MmsValue_getType(self) == MMS_ARRAY) || (MmsValue_getType(self) == MMS_STRUCTURE)) {
            int i;
            int elementCount = MmsValue_getArraySize(self);

            for (i = 0; i < elementCount; i++)
                MmsValue_setDeletableRecursive(MmsValue_getElement(self, i));
        }

        MmsValue_setDeletable(self);
    }
}

int
MmsValue_isDeletable(MmsValue* self)
{
    return self->deleteValue;
}

MmsType
MmsValue_getType(const MmsValue* self)
{
    return self->type;
}

MmsValue*
MmsValue_getSubElement(MmsValue* self, MmsVariableSpecification* varSpec, char* mmsPath)
{
    return MmsVariableSpecification_getChildValue(varSpec, self, mmsPath);
}

char*
MmsValue_getTypeString(MmsValue* self)
{
    switch (MmsValue_getType(self))
    {
    case MMS_ARRAY:
        return "array";
    case MMS_BCD:
        return "bcd";
    case MMS_BINARY_TIME:
        return "binary-time";
    case MMS_BIT_STRING:
        return "bit-string";
    case MMS_BOOLEAN:
        return "boolean";
    case MMS_DATA_ACCESS_ERROR:
        return "access-error";
    case MMS_FLOAT:
        return "float";
    case MMS_GENERALIZED_TIME:
        return "generalized-time";
    case MMS_INTEGER:
        return "integer";
    case MMS_OBJ_ID:
        return "oid";
    case MMS_STRING:
        return "mms-string";
    case MMS_STRUCTURE:
        return "structure";
    case MMS_OCTET_STRING:
        return "octet-string";
    case MMS_UNSIGNED:
        return "unsigned";
    case MMS_UTC_TIME:
        return "utc-time";
    case MMS_VISIBLE_STRING:
        return "visible-string";
    default:
        return "unknown(error)";
    }
}

const char*
MmsValue_printToBuffer(const MmsValue* self, char* buffer, int bufferSize)
{
    if (self == NULL) {
        strncpy(buffer, "(null)", bufferSize);

        /* Ensure buffer is always 0 terminated */
        if (bufferSize > 0)
            buffer[bufferSize - 1] = 0;

        return buffer;
    }

    if (bufferSize)
        buffer[0] = 0;

    switch (MmsValue_getType(self))
    {
    case MMS_STRUCTURE:
    case MMS_ARRAY:
        {
            if (bufferSize == 0)
                break;
            buffer[0] = '{';

            int bufPos = 1;

            int arraySize = MmsValue_getArraySize(self);
            int i;
            for (i = 0; i < arraySize; i++) {

                const char* currentStr = MmsValue_printToBuffer((const MmsValue*) MmsValue_getElement(self, i), buffer + bufPos, bufferSize - bufPos);

                bufPos += strlen(currentStr);

                if (bufPos >= bufferSize)
                    break;

                if (i != (arraySize - 1)) {
                    buffer[bufPos++] = ',';
                }
            }

            if (bufPos < (bufferSize - 1)) {
                buffer[bufPos++] = '}';
                buffer[bufPos] = 0;
            }
            else
                buffer[bufferSize - 1] = 0;

        }
        break;

    case MMS_BINARY_TIME:
        {
            uint8_t tempBuf[20];
            Conversions_msTimeToGeneralizedTime(MmsValue_getBinaryTimeAsUtcMs(self), tempBuf);
            snprintf(buffer, bufferSize, "%s", tempBuf);
        }
        break;

    case MMS_BIT_STRING:
        {
            int bufPos = 0;

            int size = MmsValue_getBitStringSize(self);

            /* Behave like strncpy and fill buffer with zeros */
            if (size > bufferSize) {
                memset(buffer, 0, bufferSize);
                break;
            }

            int i;
            for (i = 0; i < size; i++) {
                if (MmsValue_getBitStringBit(self, i))
                    buffer[bufPos++] = '1';
                else
                    buffer[bufPos++] = '0';
            }
            buffer[bufPos] = 0;
        }
        break;

    case MMS_BOOLEAN:
        if (MmsValue_getBoolean(self))
            strncpy(buffer, "true", bufferSize);
        else
            strncpy(buffer, "false", bufferSize);

        /* Ensure buffer is always 0 terminated */
        if (bufferSize > 0)
            buffer[bufferSize - 1] = 0;

        break;

    case MMS_DATA_ACCESS_ERROR:
        snprintf(buffer, bufferSize, "error %i", self->value.dataAccessError);
        break;

    case MMS_FLOAT:
        snprintf(buffer, bufferSize, "%f", MmsValue_toFloat(self));
        break;

    case MMS_GENERALIZED_TIME: /* type not supported */
        strncpy(buffer, "generalized time", bufferSize);

        /* Ensure buffer is always 0 terminated */
        if (bufferSize > 0)
            buffer[bufferSize - 1] = 0;

        break;

    case MMS_INTEGER:
        snprintf(buffer, bufferSize, "%lld", (long long) MmsValue_toInt64(self));
        break;

    case MMS_OCTET_STRING:
        {
            int size = MmsValue_getOctetStringSize(self);
            int bufPos = 0;
            int i;
            for (i = 0; i < size; i++) {
                snprintf(buffer + bufPos, bufferSize - bufPos, "%02x", self->value.octetString.buf[i]);
                bufPos += 2;

                if (bufPos >= bufferSize)
                    break;
            }
        }
        break;

    case MMS_UNSIGNED:
        snprintf(buffer, bufferSize, "%u", MmsValue_toUint32(self));
        break;

    case MMS_UTC_TIME:
        {
            uint8_t tempBuf[20];
            Conversions_msTimeToGeneralizedTime(MmsValue_getUtcTimeInMs(self), tempBuf);
            snprintf(buffer, bufferSize, "%s", tempBuf);
        }
        break;

    case MMS_STRING:
    case MMS_VISIBLE_STRING:
        strncpy(buffer, MmsValue_toString((MmsValue*) self), bufferSize);

        /* Ensure buffer is always 0 terminated */
        if (bufferSize > 0)
            buffer[bufferSize - 1] = 0;

        break;

    default:
        strncpy(buffer, "unknown type", bufferSize);

        /* Ensure buffer is always 0 terminated */
        if (bufferSize > 0)
            buffer[bufferSize - 1] = 0;

        break;
    }

    return buffer;
}
