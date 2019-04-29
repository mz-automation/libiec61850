/*
 *  mms_types.h
 *
 *  Copyright 2013, 2014 Michael Zillgith
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

#ifndef MMS_TYPES_H_
#define MMS_TYPES_H_

#include "libiec61850_common_api.h"

typedef enum  {
    MMS_VALUE_NO_RESPONSE,
	MMS_VALUE_OK,
	MMS_VALUE_ACCESS_DENIED,
	MMS_VALUE_VALUE_INVALID,
	MMS_VALUE_TEMPORARILY_UNAVAILABLE,
	MMS_VALUE_OBJECT_ACCESS_UNSUPPORTED
} MmsValueIndication;

/**
 * \addtogroup MMS_VAR_SPEC
 */
/**@{*/

/**
 * Type definition for MMS Named Variables
 */
typedef struct sMmsVariableSpecification MmsVariableSpecification;

/**@}*/

struct ATTRIBUTE_PACKED sMmsVariableSpecification {
    MmsType type;
    char* name;
    union uMmsTypeSpecification
    {
        struct sMmsArray {
            int elementCount; /* number of array elements */
            MmsVariableSpecification* elementTypeSpec;
        } array;
        struct sMmsStructure {
            int elementCount;
            MmsVariableSpecification** elements;
        } structure;
        int boolean; /* dummy - not required */
        int integer; /* size of integer in bits */
        int unsignedInteger; /* size of integer in bits */
        struct sMmsFloat
        {
            uint8_t exponentWidth;
            uint8_t formatWidth;
        } floatingpoint;
        int bitString; /* Number of bits in bitstring */
        int octetString; /* Number of octets in octet string */
        int visibleString; /* Maximum size of string */
        int mmsString;
        int utctime; /* dummy - not required */
        int binaryTime; /* size: either 4 or 6 */
    } typeSpec;
};


#endif /* MMS_TYPES_H_ */
