/*
 *  map.c
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

#include "libiec61850_platform_includes.h"
#include "map.h"

typedef struct sMapEntry
{
    void* key;
    void* value;
} MapEntry;

static int
comparePointerKeys(void* key1, void* key2)
{
    if (key2 == key1)
        return 0;
    else
        return -1;
}

Map
Map_create()
{
    Map map = (Map) GLOBAL_CALLOC(1, sizeof(struct sMap));
    map->entries = LinkedList_create();
    map->compareKeys = comparePointerKeys;
    return map;
}

int
Map_size(Map map)
{
    return LinkedList_size(map->entries);
}

void*
Map_addEntry(Map map, void* key, void* value)
{
    MapEntry* entry = (MapEntry*) GLOBAL_MALLOC(sizeof(MapEntry));
    entry->key = key;
    entry->value = value;
    LinkedList_add(map->entries, entry);

    return entry->key;
}

void*
Map_removeEntry(Map map, void* key, bool deleteKey)
{
    LinkedList element = map->entries;
    LinkedList lastElement = element;
    MapEntry* entry;
    void* value = NULL;

    while ((element = LinkedList_getNext(element)) != NULL) {
        entry = (MapEntry*) element->data;

        if (map->compareKeys(key, entry->key) == 0) {
            lastElement->next = element->next;
            value = entry->value;

            if (deleteKey == true)
                GLOBAL_FREEMEM(entry->key);
            GLOBAL_FREEMEM(entry);
            GLOBAL_FREEMEM(element);

            break;
        }

        lastElement = element;
    }

    return value;
}

void*
Map_getEntry(Map map, void* key)
{
    LinkedList element = map->entries;

    while ((element = LinkedList_getNext(element)) != NULL) {
        MapEntry* entry = (MapEntry*) element->data;
        if (map->compareKeys(key, entry->key) == 0) {
            return entry->value;
        };
    }

    return NULL;
}

void
Map_delete(Map map, bool deleteKey)
{
    LinkedList element = map->entries;

    while ((element = LinkedList_getNext(element)) != NULL) {
        MapEntry* entry = (MapEntry*) element->data;
        if (deleteKey == true)
            GLOBAL_FREEMEM(entry->key);
        GLOBAL_FREEMEM(entry->value);
    }

    LinkedList_destroy(map->entries);
    GLOBAL_FREEMEM(map);
}

void
Map_deleteStatic(Map map, bool deleteKey)
{
    LinkedList element = map->entries;

    if (deleteKey == true) {
        while ((element = LinkedList_getNext(element)) != NULL) {
            MapEntry* entry = (MapEntry*) element->data;
            GLOBAL_FREEMEM(entry->key);
        }
    }

    LinkedList_destroy(map->entries);
    GLOBAL_FREEMEM(map);
}

void
Map_deleteDeep(Map map, bool deleteKey, void (*valueDeleteFunction)(void*))
{
    if (map) {
        LinkedList element = map->entries;

        while ((element = LinkedList_getNext(element)) != NULL) {
            MapEntry* entry = (MapEntry*) element->data;
            if (deleteKey == true)
                GLOBAL_FREEMEM(entry->key);
            valueDeleteFunction(entry->value);
        }

        LinkedList_destroy(map->entries);

        GLOBAL_FREEMEM(map);
    }
}
