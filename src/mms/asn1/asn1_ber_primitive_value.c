/*
 *  asn1_ber_primitive_value.c
 *
 *  Copyright 2013-2020 Michael Zillgith
 *
 *	This file is part of libIEC61850.
 *
 *	libIEC61850 is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	libIEC61850 is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with libIEC61850.  If not, see <http://www.gnu.org/licenses/>.
 *
 *	See COPYING file for the complete license text.
 */

#include "libiec61850_platform_includes.h"

#include "asn1_ber_primitive_value.h"

Asn1PrimitiveValue*
Asn1PrimitiveValue_create(int size)
{
	Asn1PrimitiveValue* self = (Asn1PrimitiveValue*) GLOBAL_MALLOC(sizeof(Asn1PrimitiveValue));

	if (self) {
	    self->size = 1;
	    self->maxSize = size;

	    self->octets = (uint8_t*) GLOBAL_CALLOC(1, size);

	    if (self->octets == NULL) {
	        GLOBAL_FREEMEM(self);
	        self = NULL;
	    }
	}

	return self;
}

Asn1PrimitiveValue*
Asn1PrimitiveValue_clone(Asn1PrimitiveValue* self)
{
	Asn1PrimitiveValue* clone = (Asn1PrimitiveValue*) GLOBAL_MALLOC(sizeof(Asn1PrimitiveValue));

	if (clone) {
	    clone->size = self->size;
	    clone->maxSize = self->maxSize;

	    clone->octets = (uint8_t*) GLOBAL_MALLOC(self->maxSize);

	    if (clone->octets) {
	        memcpy(clone->octets, self->octets, clone->maxSize);
	    }
	    else {
	        GLOBAL_FREEMEM(clone);
	        clone = NULL;
	    }

	}

	return clone;
}

bool
Asn1PrimitivaValue_compare(Asn1PrimitiveValue* self, Asn1PrimitiveValue* otherValue)
{
    if (self->size == otherValue->size) {
        if (memcmp(self->octets, otherValue->octets, self->size) == 0)
            return true;
        else
            return false;
    }
    else
        return false;
}

int
Asn1PrimitiveValue_getSize(Asn1PrimitiveValue* self)
{
	return self->size;
}

int
Asn1PrimitiveValue_getMaxSize(Asn1PrimitiveValue* self)
{
	return self->maxSize;
}

void
Asn1PrimitiveValue_destroy(Asn1PrimitiveValue* self)
{
    if (self) {
        GLOBAL_FREEMEM(self->octets);
        GLOBAL_FREEMEM(self);
    }
}
