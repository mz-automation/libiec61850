/*
 * iec61850_common.c
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

#include "iec61850_common.h"
#include "iec61850_common_internal.h"

#include "libiec61850_platform_includes.h"

#include "conversions.h"
#include "mms_value_internal.h"

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
Quality_fromMmsValue(const MmsValue* mmsValue)
{
    return (Quality) MmsValue_getBitStringAsInteger(mmsValue);
}

Dbpos
Dbpos_fromMmsValue(const MmsValue* mmsValue)
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
    case IEC61850_FC_ST:
        return "ST";
    case IEC61850_FC_MX:
        return "MX";
    case IEC61850_FC_SP:
        return "SP";
    case IEC61850_FC_SV:
        return "SV";
    case IEC61850_FC_CF:
        return "CF";
    case IEC61850_FC_DC:
        return "DC";
    case IEC61850_FC_SG:
        return "SG";
    case IEC61850_FC_SE:
        return "SE";
    case IEC61850_FC_SR:
        return "SR";
    case IEC61850_FC_OR:
        return "OR";
    case IEC61850_FC_BL:
        return "BL";
    case IEC61850_FC_EX:
        return "EX";
    case IEC61850_FC_CO:
        return "CO";
    case IEC61850_FC_US:
        return "US";
    case IEC61850_FC_MS:
        return "MS";
    case IEC61850_FC_RP:
        return "RP";
    case IEC61850_FC_BR:
        return "BR";
    case IEC61850_FC_LG:
        return "LG";
    case IEC61850_FC_GO:
        return "GO";
    default:
        return NULL;
    }
}

FunctionalConstraint
FunctionalConstraint_fromString(const char* fcString)
{
    if (fcString[0] == 'S') {
        if (fcString[1] == 'T')
            return IEC61850_FC_ST;
        if (fcString[1] == 'P')
            return IEC61850_FC_SP;
        if (fcString[1] == 'V')
            return IEC61850_FC_SV;
        if (fcString[1] == 'G')
            return IEC61850_FC_SG;
        if (fcString[1] == 'E')
            return IEC61850_FC_SE;
        if (fcString[1] == 'R')
            return IEC61850_FC_SR;

        return IEC61850_FC_NONE;
    }

    if (fcString[0] == 'M') {
        if (fcString[1] == 'X')
            return IEC61850_FC_MX;
        if (fcString[1] == 'S')
            return IEC61850_FC_MS;
        return IEC61850_FC_NONE;
    }

    if (fcString[0] == 'C') {
        if (fcString[1] == 'F')
            return IEC61850_FC_CF;
        if (fcString[1] == 'O')
            return IEC61850_FC_CO;
        return IEC61850_FC_NONE;
    }

    if (fcString[0] == 'D') {
        if (fcString[1] == 'C')
            return IEC61850_FC_DC;
        return IEC61850_FC_NONE;
    }

    if (fcString[0] == 'O') {
        if (fcString[1] == 'R')
            return IEC61850_FC_OR;
        return IEC61850_FC_NONE;
    }

    if (fcString[0] == 'B') {
        if (fcString[1] == 'L')
            return IEC61850_FC_BL;
        if (fcString[1] == 'R')
            return IEC61850_FC_BR;
        return IEC61850_FC_NONE;
    }

    if (fcString[0] == 'E') {
        if (fcString[1] == 'X')
            return IEC61850_FC_EX;
        return IEC61850_FC_NONE;
    }

    if (fcString[0] == 'U') {
        if (fcString[1] == 'S')
            return IEC61850_FC_US;
        return IEC61850_FC_NONE;
    }

    if (fcString[0] == 'R') {
        if (fcString[1] == 'P')
            return IEC61850_FC_RP;
        return IEC61850_FC_NONE;
    }

    if (fcString[0] == 'L') {
        if (fcString[1] == 'G')
            return IEC61850_FC_LG;
        return IEC61850_FC_NONE;
    }

    if (fcString[0] == 'G') {
        if (fcString[1] == 'O')
            return IEC61850_FC_GO;
        return IEC61850_FC_NONE;
    }

    return IEC61850_FC_NONE;
}

Timestamp*
Timestamp_create()
{
    Timestamp* self = (Timestamp*) GLOBAL_CALLOC(1, sizeof(Timestamp));

    return self;
}

Timestamp*
Timestamp_createFromByteArray(uint8_t* byteArray)
{
    Timestamp* self = Timestamp_create();

    if (self) {
        int i;

        for (i = 0; i < 8; i++)
            self->val[i] = byteArray[i];
    }

    return self;
}

void
Timestamp_destroy(Timestamp* self)
{
    if (self != NULL)
        GLOBAL_FREEMEM(self);
}

void
Timestamp_clearFlags(Timestamp* self)
{
    self->val[7] = 0;
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
    uint8_t* valueArray =  self->val;

    valueArray[0] = (secondsSinceEpoch / 0x1000000 & 0xff);
    valueArray[1] = (secondsSinceEpoch / 0x10000 & 0xff);
    valueArray[2] = (secondsSinceEpoch / 0x100 & 0xff);
    valueArray[3] = (secondsSinceEpoch & 0xff);

    self->val[4] = 0;
    self->val[5] = 0;
    self->val[6] = 0;

    /* don't touch time quality */
}

void
Timestamp_setTimeInMilliseconds(Timestamp* self, msSinceEpoch millisSinceEpoch)
{
    uint32_t timeval32 = (uint32_t) (millisSinceEpoch / 1000LL);

    uint8_t* valueArray = self->val;

    valueArray[0] = (timeval32 / 0x1000000 & 0xff);
    valueArray[1] = (timeval32 / 0x10000 & 0xff);
    valueArray[2] = (timeval32 / 0x100 & 0xff);
    valueArray[3] = (timeval32 & 0xff);

    uint32_t remainder = (millisSinceEpoch % 1000LL);
    uint32_t fractionOfSecond = (remainder) * 16777 + ((remainder * 216) / 1000);

    /* encode fraction of second */
    valueArray[4] = ((fractionOfSecond >> 16) & 0xff);
    valueArray[5] = ((fractionOfSecond >> 8) & 0xff);
    valueArray[6] = (fractionOfSecond & 0xff);

    /* don't touch time quality */
}

void
Timestamp_setTimeInNanoseconds(Timestamp* self, nsSinceEpoch nsTime)
{
    uint32_t timeval32 = (uint32_t)(nsTime / 1000000000ULL);

    uint8_t* valueArray = self->val;

    valueArray[0] = (timeval32 / 0x1000000 & 0xff);
    valueArray[1] = (timeval32 / 0x10000 & 0xff);
    valueArray[2] = (timeval32 / 0x100 & 0xff);
    valueArray[3] = (timeval32 & 0xff);

    uint64_t remainder = (nsTime % 1000000000ULL);

    remainder = remainder << 24;
    remainder = remainder / 1000000000UL;

    uint32_t fractionOfSecond = (uint32_t) remainder;

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

    timeval32 = valueArray[3];
    timeval32 += valueArray[2] * 0x100;
    timeval32 += valueArray[1] * 0x10000;
    timeval32 += valueArray[0] * 0x1000000;

    return timeval32;
}

msSinceEpoch
Timestamp_getTimeInMs(Timestamp* self)
{
    uint32_t timeval32;
    uint8_t* valueArray = self->val;

    timeval32 = valueArray[3];
    timeval32 += valueArray[2] * 0x100;
    timeval32 += valueArray[1] * 0x10000;
    timeval32 += valueArray[0] * 0x1000000;

    uint32_t fractionOfSecond = 0;

    fractionOfSecond = (valueArray[4] << 16);
    fractionOfSecond += (valueArray[5] << 8);
    fractionOfSecond += (valueArray[6]);

    uint32_t remainder = fractionOfSecond / 16777;

    uint64_t msVal = (timeval32 * 1000LL) + remainder;

    return (uint64_t) msVal;
}

nsSinceEpoch
Timestamp_getTimeInNs(Timestamp* self)
{
    uint32_t timeval32;
    uint8_t* valueArray = self->val;

    timeval32 = valueArray[3];
    timeval32 += valueArray[2] * 0x100;
    timeval32 += valueArray[1] * 0x10000;
    timeval32 += valueArray[0] * 0x1000000;

    uint32_t fractionOfSecond;

    fractionOfSecond = valueArray[6];
    fractionOfSecond += valueArray[5] * 0x100;
    fractionOfSecond += valueArray[4] * 0x10000;

    uint64_t nsVal = fractionOfSecond;

    nsVal = nsVal * 1000000000UL;
    nsVal = nsVal >> 24;

    uint64_t timeval64 = (uint64_t) timeval32 * 1000000000ULL + nsVal;

    return timeval64;
}

void
Timestamp_setByMmsUtcTime(Timestamp* self, MmsValue* mmsValue)
{
    if (MmsValue_getType(mmsValue) == MMS_UTC_TIME)
        memcpy(self->val, mmsValue->value.utcTime, 8);
}

MmsValue*
Timestamp_toMmsValue(Timestamp* self, MmsValue* mmsValue)
{
    MmsValue* convertedValue = mmsValue;

    if (convertedValue == NULL)
        convertedValue = MmsValue_newUtcTime(0);

    if (convertedValue != NULL)
        memcpy(convertedValue->value.utcTime, self->val, 8);

    return convertedValue;
}

char*
MmsMapping_getMmsDomainFromObjectReference(const char* objectReference, char* buffer)
{
    int objRefLength = strlen(objectReference);
    char* domainName = NULL;

    /* check for object reference size limit VISIBLESTRING129 */
    if (objRefLength > 129)
        goto exit_function;

    /* check if LD name is present */
    int i;
    for (i = 0; i < objRefLength; i++) {
        if (objectReference[i] == '/') {
            break;
        }
    }

    /* check for LD name limit (=64 characters) */
    if (i > 64)
        goto exit_function;

    if (i == objRefLength)
        goto exit_function;

    if (buffer == NULL)
        domainName = (char*) GLOBAL_MALLOC(i + 1);
    else
        domainName = buffer;

    int j;
    for (j = 0; j < i; j++) {
        domainName[j] = objectReference[j];
    }

    domainName[j] = 0;

exit_function:
    return domainName;
}

char*
MmsMapping_createMmsVariableNameFromObjectReference(const char* objectReference,
        FunctionalConstraint fc, char* buffer)
{
    int objRefLength = strlen(objectReference);

    /* check for object reference size limit VISIBLESTRING129 */
    if (objRefLength > 129)
        return NULL;

    /* check if LD name is present */
    int i;
    for (i = 0; i < objRefLength; i++) {
        if (objectReference[i] == '/') {
            break;
        }
    }

    /* check for LD name limit (= 64 characters) */
    if (i > 64)
        return NULL;

    if (i == objRefLength)
        i = 0; /* for the case when no LD name is present */
    else
        i++;


    if (fc == IEC61850_FC_NONE) {

        int len = objRefLength - i;

        char* mmsVariableName;

        if (buffer == NULL)
            mmsVariableName = (char*) GLOBAL_MALLOC(len + 1);
        else
            mmsVariableName = buffer;

        strncpy(mmsVariableName, objectReference + i, len);
		mmsVariableName[len] = 0;

        return mmsVariableName;
    }

    char* fcString = FunctionalConstraint_toString(fc);

    if (fcString == NULL)
        return NULL;

    int namePartLength = objRefLength - i - 1;

    /* ensure that limit due to MMS name part length = 64 is not exceeded */
    if (namePartLength > 61)
        return NULL;

    char* mmsVariableName;

    if (buffer == NULL)
        mmsVariableName = (char*) GLOBAL_MALLOC(namePartLength + 5);
    else
        mmsVariableName = buffer;

    int sourceIndex = i;
    int destIndex = 0;

    bool fcAdded = false;

    while (sourceIndex < objRefLength) {

        if (objectReference[sourceIndex] != '.')
            mmsVariableName[destIndex++] = objectReference[sourceIndex++];
        else {

            if (!fcAdded) {
                mmsVariableName[destIndex++] = '$';
                mmsVariableName[destIndex++] = fcString[0];
                mmsVariableName[destIndex++] = fcString[1];
                mmsVariableName[destIndex++] = '$';

                fcAdded = true;
            }
            else
                mmsVariableName[destIndex++] = '$';

            sourceIndex++;
        }
    }

    if (!fcAdded) {
        mmsVariableName[destIndex++] = '$';
        mmsVariableName[destIndex++] = fcString[0];
        mmsVariableName[destIndex++] = fcString[1];
    }

    mmsVariableName[destIndex] = 0;

    return mmsVariableName;
}

MmsVariableAccessSpecification*
MmsMapping_ObjectReferenceToVariableAccessSpec(char* objectReference)
{
    char* domainIdEnd = strchr(objectReference, '/');

    if (domainIdEnd == NULL) /* no logical device name present */
        return NULL;

    int domainIdLen = domainIdEnd - objectReference;

    if (domainIdLen > 64)
        return NULL;

    char* fcStart = strchr(objectReference, '[');

    if (fcStart == NULL) /* no FC present */
        return NULL;

    char* fcEnd = strchr(fcStart, ']');

    if (fcEnd == NULL) /* syntax error in FC */
        return NULL;

    if ((fcEnd - fcStart) != 3) /* syntax error in FC */
        return NULL;

    FunctionalConstraint fc = FunctionalConstraint_fromString(fcStart + 1);

    MmsVariableAccessSpecification* accessSpec =
            (MmsVariableAccessSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableAccessSpecification));

    accessSpec->domainId = StringUtils_createStringFromBuffer((uint8_t*) objectReference, domainIdLen);

    char* indexBrace = strchr(domainIdEnd, '(');

    char* itemIdEnd = indexBrace;

    if (itemIdEnd == NULL)
        itemIdEnd = strchr(domainIdEnd, '[');

    int objRefLen = strlen(objectReference);

    accessSpec->arrayIndex = -1; /* -1 --> not present */

    if (itemIdEnd != NULL) {
        int itemIdLen = itemIdEnd - domainIdEnd - 1;

        char itemIdStr[129];

        memcpy(itemIdStr, (domainIdEnd + 1), itemIdLen);
        itemIdStr[itemIdLen] = 0;

        accessSpec->itemId = MmsMapping_createMmsVariableNameFromObjectReference(itemIdStr, fc, NULL);

        if (indexBrace != NULL) {

            char* indexStart = itemIdEnd + 1;

            char* indexEnd = strchr(indexStart, ')');

            int indexLen = indexEnd - indexStart;

            int index = StringUtils_digitsToInt(indexStart, indexLen);

            accessSpec->arrayIndex = (int32_t) index;

            int componentNameLen = objRefLen - ((indexEnd + 2) - objectReference) - 4;

            if (componentNameLen > 0) {
                accessSpec->componentName = StringUtils_createStringFromBuffer((uint8_t*) (indexEnd + 2), componentNameLen);
                StringUtils_replace(accessSpec->componentName, '.', '$');
            }
        }
    }

    return accessSpec;
}


static int
getNumberOfDigits(int value)
{
    int numberOfDigits = 1;

    while (value > 9) {
        numberOfDigits++;
        value /= 10;
    }

    return numberOfDigits;
}

char*
MmsMapping_varAccessSpecToObjectReference(MmsVariableAccessSpecification* varAccessSpec)
{
    char* domainId = varAccessSpec->domainId;

    int domainIdLen = strlen(domainId);

    char* itemId = varAccessSpec->itemId;

    char* separator = strchr(itemId, '$');

    int itemIdLen = strlen(itemId);

    int arrayIndexLen = 0;

    int componentPartLen = 0;

    if (varAccessSpec->componentName != NULL)
        componentPartLen = strlen(varAccessSpec->componentName);

    if (varAccessSpec->arrayIndex > -1)
        arrayIndexLen = 2 + getNumberOfDigits(varAccessSpec->arrayIndex);

    int newStringLen = (domainIdLen + 1) + (itemIdLen - 2) + arrayIndexLen + 4 /* for FC */+ componentPartLen + 1;

    char* newString = (char*) GLOBAL_MALLOC(newStringLen);

    char* targetPos = newString;

    /* Copy domain id part */
    char* currentPos = domainId;

    while (currentPos < (domainId + domainIdLen)) {
        *targetPos = *currentPos;
        targetPos++;
        currentPos++;
    }

    *targetPos = '/';
    targetPos++;

    /* Copy item id parts */
    currentPos = itemId;

    while (currentPos < separator) {
        *targetPos = *currentPos;
        targetPos++;
        currentPos++;
    }

    *targetPos = '.';
    targetPos++;

    currentPos = separator + 4;

    while (currentPos < (itemId + itemIdLen)) {
        if (*currentPos == '$')
            *targetPos = '.';
        else
            *targetPos = *currentPos;

        targetPos++;
        currentPos++;
    }

    /* Add array index part */
    if (varAccessSpec->arrayIndex > -1) {
        sprintf(targetPos, "(%i)", varAccessSpec->arrayIndex);
        targetPos += arrayIndexLen;
    }

    /* Add component part */
    if (varAccessSpec->componentName != NULL) {
        *targetPos = '.';
        targetPos++;

        int i;
        for (i = 0; i < componentPartLen; i++) {
            if (varAccessSpec->componentName[i] == '$')
                *targetPos = '.';
            else
                *targetPos = varAccessSpec->componentName[i];

            targetPos++;
        }
    }

    /* add FC part */
    *targetPos = '[';
    targetPos++;
    *targetPos = *(separator + 1);
    targetPos++;
    *targetPos = *(separator + 2);
    targetPos++;
    *targetPos = ']';
    targetPos++;

    *targetPos = 0; /* add terminator */

    return newString;
}

char*
LibIEC61850_getVersionString()
{
    return LIBIEC61850_VERSION;
}
