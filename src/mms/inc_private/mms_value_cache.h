/*
 *  mms_value_cache.h
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

#ifndef MMS_VARIABLE_CACHE_H_
#define MMS_VARIABLE_CACHE_H_

#include "mms_device_model.h"
#include "mms_value.h"

typedef struct sMmsValueCache* MmsValueCache;

LIB61850_INTERNAL MmsValueCache
MmsValueCache_create(MmsDomain* domain);

LIB61850_INTERNAL void
MmsValueCache_insertValue(MmsValueCache self, char* itemId, MmsValue* value);

LIB61850_INTERNAL MmsValue*
MmsValueCache_lookupValue(MmsValueCache self, const char* itemId, MmsVariableSpecification** outSpec);

LIB61850_INTERNAL void
MmsValueCache_destroy(MmsValueCache self);

#endif /* MMS_VARIABLE_CACHE_H_ */
