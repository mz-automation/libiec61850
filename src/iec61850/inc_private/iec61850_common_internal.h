/*
 *  iec61850_common_internal.h
 *
 *  Copyright 2019 Michael Zillgith
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

#ifndef SRC_IEC61850_INC_PRIVATE_IEC61850_COMMON_INTERNAL_H_
#define SRC_IEC61850_INC_PRIVATE_IEC61850_COMMON_INTERNAL_H_

#include "iec61850_model.h"

LIB61850_INTERNAL char*
MmsMapping_getMmsDomainFromObjectReference(const char* objectReference, char* buffer);

LIB61850_INTERNAL char*
MmsMapping_createMmsVariableNameFromObjectReference(const char* objectReference, FunctionalConstraint fc, char* buffer);

LIB61850_INTERNAL MmsVariableAccessSpecification*
MmsMapping_ObjectReferenceToVariableAccessSpec(char* objectReference);

LIB61850_INTERNAL char*
MmsMapping_varAccessSpecToObjectReference(MmsVariableAccessSpecification* varAccessSpec);

#endif /* SRC_IEC61850_INC_PRIVATE_IEC61850_COMMON_INTERNAL_H_ */
