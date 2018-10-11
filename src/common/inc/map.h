/*
 *  map.h
 *
 *  Copyright 2013-2018 Michael Zillgith
 *
 *	This file is part of libIEC61850.
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

#ifndef MAP_H_
#define MAP_H_

#include "libiec61850_common_api.h"
#include "linked_list.h"

typedef struct sMap* Map;

struct sMap {
	LinkedList entries;

	/* client provided function to compare two keys */
	int (*compareKeys)(void* key1, void* key2);
};

LIB61850_INTERNAL Map
Map_create(void);

LIB61850_INTERNAL int
Map_size(Map map);

LIB61850_INTERNAL void*
Map_addEntry(Map map, void* key, void* value);

LIB61850_INTERNAL void*
Map_removeEntry(Map map, void* key, bool deleteKey);

LIB61850_INTERNAL void*
Map_getEntry(Map map, void* key);

LIB61850_INTERNAL void
Map_delete(Map map, bool deleteKey);

LIB61850_INTERNAL void
Map_deleteStatic(Map map, bool deleteKey);

LIB61850_INTERNAL void
Map_deleteDeep(Map map, bool deleteKey, void (*valueDeleteFunction) (void*));


#endif /* MAP_H_ */
