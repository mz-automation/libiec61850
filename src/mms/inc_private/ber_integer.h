/*
 *  ber_integer.h
 *
 *  Copyright 2013-2018 Michael Zillgith
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

#ifndef BER_INTEGER_H_
#define BER_INTEGER_H_

#include "asn1_ber_primitive_value.h"

#ifdef __cplusplus
extern "C" {
#endif

LIB61850_INTERNAL Asn1PrimitiveValue*
BerInteger_createFromBuffer(uint8_t* buf, int size);

LIB61850_INTERNAL Asn1PrimitiveValue*
BerInteger_createInt32(void);

LIB61850_INTERNAL int
BerInteger_setFromBerInteger(Asn1PrimitiveValue* self, Asn1PrimitiveValue* value);

LIB61850_INTERNAL int
BerInteger_setInt32(Asn1PrimitiveValue* self, int32_t value);

LIB61850_INTERNAL Asn1PrimitiveValue*
BerInteger_createFromInt32(int32_t value);

LIB61850_INTERNAL int
BerInteger_setUint8(Asn1PrimitiveValue* self, uint8_t value);

LIB61850_INTERNAL int
BerInteger_setUint16(Asn1PrimitiveValue* self, uint16_t value);

LIB61850_INTERNAL int
BerInteger_setUint32(Asn1PrimitiveValue* self, uint32_t value);

LIB61850_INTERNAL Asn1PrimitiveValue*
BerInteger_createFromUint32(uint32_t value);

LIB61850_INTERNAL Asn1PrimitiveValue*
BerInteger_createFromInt64(int64_t value);

LIB61850_INTERNAL Asn1PrimitiveValue*
BerInteger_createInt64(void);

LIB61850_INTERNAL int
BerInteger_setInt64(Asn1PrimitiveValue* self, int64_t value);

LIB61850_INTERNAL void
BerInteger_toInt32(Asn1PrimitiveValue* self, int32_t* nativeValue);

LIB61850_INTERNAL void
BerInteger_toUint32(Asn1PrimitiveValue* self, uint32_t* nativeValue);

LIB61850_INTERNAL void
BerInteger_toInt64(Asn1PrimitiveValue* self, int64_t* nativeValue);

#ifdef __cplusplus
}
#endif

#endif /* BER_INTEGER_H_ */
