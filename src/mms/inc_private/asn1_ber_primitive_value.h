/*
 *  asn1_ber_primitive_value.h
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

#ifndef ASN1_BER_PRIMITIVE_VALUE_H_
#define ASN1_BER_PRIMITIVE_VALUE_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "libiec61850_common_api.h"

typedef struct ATTRIBUTE_PACKED {
	uint8_t size;
	uint8_t maxSize;
	uint8_t* octets;
} Asn1PrimitiveValue;

LIB61850_INTERNAL Asn1PrimitiveValue*
Asn1PrimitiveValue_create(int size);

LIB61850_INTERNAL int
Asn1PrimitiveValue_getSize(Asn1PrimitiveValue* self);

LIB61850_INTERNAL int
Asn1PrimitiveValue_getMaxSize(Asn1PrimitiveValue* self);

LIB61850_INTERNAL Asn1PrimitiveValue*
Asn1PrimitiveValue_clone(Asn1PrimitiveValue* self);

LIB61850_INTERNAL bool
Asn1PrimitivaValue_compare(Asn1PrimitiveValue* self, Asn1PrimitiveValue* otherValue);

LIB61850_INTERNAL void
Asn1PrimitiveValue_destroy(Asn1PrimitiveValue* self);

#ifdef __cplusplus
}
#endif

#endif /* ASN1_BER_PRIMITIVE_VALUE_H_ */
