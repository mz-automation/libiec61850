/*
 *  mms_value_internal.h
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

#ifndef MMS_VALUE_INTERNAL_H_
#define MMS_VALUE_INTERNAL_H_

#include "mms_value.h"
#include "ber_integer.h"

struct ATTRIBUTE_PACKED sMmsValue {
    MmsType type;
    uint8_t deleteValue;
    union uMmsValue {
        MmsDataAccessError dataAccessError;
        struct {
            int size;
            MmsValue** components;
        } structure;
        bool boolean;
        Asn1PrimitiveValue* integer;
        struct {
            uint8_t exponentWidth;
            uint8_t formatWidth; /* number of bits - either 32 or 64)  */
            uint8_t buf[8];
        } floatingPoint;
        struct {
            uint16_t size;
            uint16_t maxSize;
            uint8_t* buf;
        } octetString;
        struct {
            int size;     /* Number of bits */
            uint8_t* buf;
        } bitString;
        struct {
            char* buf;
            int16_t size; /* size of the string, equals the amount of allocated memory - 1 */
        } visibleString;
        uint8_t utcTime[8];
        struct {
            uint8_t size;
            uint8_t buf[6];
        } binaryTime;
    } value;
};


LIB61850_INTERNAL MmsValue*
MmsValue_newIntegerFromBerInteger(Asn1PrimitiveValue* berInteger);

LIB61850_INTERNAL MmsValue*
MmsValue_newUnsignedFromBerInteger(Asn1PrimitiveValue* berInteger);

#endif /* MMS_VALUE_INTERNAL_H_ */
