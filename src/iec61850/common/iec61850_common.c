/*
 * iec61850_common.c
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

#include "iec61850_common.h"

#include "libiec61850_platform_includes.h"

#include "conversions.h"

Validity
Quality_getValidity(Quality* self)
{
    return (*self & 0x3);
}

void
Quality_setValidity(Quality* self, Validity validity)
{
    *self = *self & (0xfffc);
    *self = *self | validity;
}

bool
Quality_isFlagSet(Quality* self, int flag)
{
    if ((*self & flag) > 0)
        return true;
    else
        return false;
}

void
Quality_setFlag(Quality* self, int flag)
{
    *self = *self | flag;
}

void
Quality_unsetFlag(Quality* self, int flag)
{
    *self = *self & (~flag);
}


Quality
Quality_fromMmsValue(MmsValue* mmsValue)
{
    return (Quality) MmsValue_getBitStringAsInteger(mmsValue);
}

Dbpos
Dbpos_fromMmsValue(MmsValue* mmsValue)
{
    return (Dbpos) MmsValue_getBitStringAsIntegerBigEndian(mmsValue);
}

MmsValue*
Dbpos_toMmsValue(MmsValue* mmsValue, Dbpos dbpos)
{
    if (mmsValue == NULL) {
        mmsValue = MmsValue_newBitString(2);
    }
    else {
        if (MmsValue_getType(mmsValue) != MMS_BIT_STRING)
            return NULL;

        if (MmsValue_getBitStringSize(mmsValue) != 2)
            return NULL;
    }

    assert((int) dbpos >= 0);
    assert((int) dbpos < 4);

    MmsValue_setBitStringFromIntegerBigEndian(mmsValue, dbpos);

    return mmsValue;
}

char*
FunctionalConstraint_toString(FunctionalConstraint fc) {
    switch (fc) {
    case ST:
        return "ST";
    case MX:
        return "MX";
    case SP:
        return "SP";
    case SV:
        return "SV";
    case CF:
        return "CF";
    case DC:
        return "DC";
    case SG:
        return "SG";
    case SE:
        return "SE";
    case SR:
        return "SR";
    case OR:
        return "OR";
    case BL:
        return "BL";
    case EX:
        return "EX";
    case CO:
        return "CO";
    default:
        return NULL;
    }
}

FunctionalConstraint
FunctionalConstraint_fromString(char* fcString)
{
    if (fcString[0] == 'S') {
        if (fcString[1] == 'T')
            return ST;
        if (fcString[1] == 'P')
            return SP;
        if (fcString[1] == 'V')
            return SV;
        if (fcString[1] == 'G')
            return SG;
        if (fcString[1] == 'E')
            return SE;
        if (fcString[1] == 'R')
            return SR;

        return NONE;
    }

    if (fcString[0] == 'M') {
        if (fcString[1] == 'X')
            return MX;
        return NONE;
    }

    if (fcString[0] == 'C') {
        if (fcString[1] == 'F')
            return CF;
        if (fcString[1] == 'O')
            return CO;
        return NONE;
    }

    if (fcString[0] == 'D') {
        if (fcString[1] == 'C')
            return DC;
        return NONE;
    }

    if (fcString[0] == 'O') {
        if (fcString[1] == 'R')
            return OR;
        return NONE;
    }

    if (fcString[0] == 'B') {
        if (fcString[1] == 'L')
            return BL;
        return NONE;
    }

    if (fcString[0] == 'E') {
        if (fcString[1] == 'X')
            return EX;
        return NONE;
    }

    return NONE;
}

bool
Timestamp_isLeapSecondKnown(Timestamp* self)
{
    if (self->val[7] & 0x80)
        return true;
    else
        return false;
}

void
Timestamp_setLeapSecondKnown(Timestamp* self, bool value)
{
    if (value)
        self->val[7] |= 0x80;
    else
        self->val[7] &= 0x7f;
}

bool
Timestamp_hasClockFailure(Timestamp* self)
{
    if (self->val[7] & 0x40)
        return true;
    else
        return false;
}

void
Timestamp_setClockFailure(Timestamp* self, bool value)
{
    if (value)
        self->val[7] |= 0x40;
    else
        self->val[7] &= 0xbf;
}

bool
Timestamp_isClockNotSynchronized(Timestamp* self)
{
    if (self->val[7] & 0x20)
        return true;
    else
        return false;
}

void
Timestamp_setClockNotSynchronized(Timestamp* self, bool value)
{
    if (value)
        self->val[7] |= 0x20;
    else
        self->val[7] &= 0xdf;
}

int
Timestamp_getSubsecondPrecision(Timestamp* self)
{
    return (int) (self->val[7] & 0x1f);
}

void
Timestamp_setSubsecondPrecision(Timestamp* self, int subsecondPrecision)
{
    uint8_t ssp = subsecondPrecision & 0x1f;

    self->val[7] &= 0xe0;
    self->val[7] += ssp;
}

void
Timestamp_setTimeInSeconds(Timestamp* self, uint32_t secondsSinceEpoch)
{
    uint8_t* timeArray = (uint8_t*) &secondsSinceEpoch;
    uint8_t* valueArray =  self->val;

#if (ORDER_LITTLE_ENDIAN == 1)
        memcpyReverseByteOrder(valueArray, timeArray, 4);
#else
        memcpy(valueArray, timeArray, 4);
#endif

    self->val[4] = 0;
    self->val[5] = 0;
    self->val[6] = 0;

    /* don't touch time quality */
}

void
Timestamp_setTimeInMilliseconds(Timestamp* self, uint64_t millisSinceEpoch)
{
    uint32_t timeval32 = (millisSinceEpoch / 1000LL);

    uint8_t* timeArray = (uint8_t*) &timeval32;
    uint8_t* valueArray = self->val;

#if (ORDER_LITTLE_ENDIAN == 1)
        memcpyReverseByteOrder(valueArray, timeArray, 4);
#else
        memcpy(valueArray, timeArray, 4);
#endif

    uint32_t remainder = (millisSinceEpoch % 1000LL);
    uint32_t fractionOfSecond = (remainder) * 16777 + ((remainder * 216) / 1000);

    /* encode fraction of second */
    valueArray[4] = ((fractionOfSecond >> 16) & 0xff);
    valueArray[5] = ((fractionOfSecond >> 8) & 0xff);
    valueArray[6] = (fractionOfSecond & 0xff);

    /* don't touch time quality */
}

uint32_t
Timestamp_getTimeInSeconds(Timestamp* self)
{
    uint32_t timeval32;
    uint8_t* valueArray = self->val;

#if (ORDER_LITTLE_ENDIAN == 1)
    memcpyReverseByteOrder((uint8_t*) &timeval32, valueArray, 4);
#else
    memcpy((uint8_t*) &timeval32, valueArray, 4);
#endif

    return timeval32;
}

uint64_t
Timestamp_getTimeInMs(Timestamp* self)
{
    uint32_t timeval32;
    uint8_t* valueArray = self->val;

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

    return (uint64_t) msVal;
}
