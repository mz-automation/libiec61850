/*
 *  mms_value_cache.c
 *
 *  Copyright 2013-2021 Michael Zillgith
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
#include "mms_value_cache.h"
#include "string_map.h"
#include "stack_config.h"

struct sMmsValueCache {
    MmsDomain* domain;
    Map map;
};

typedef struct sMmsValueCacheEntry {
    MmsValue* value;
    MmsVariableSpecification* typeSpec;
} MmsValueCacheEntry;

MmsValueCache
MmsValueCache_create(MmsDomain* domain)
{
    MmsValueCache self = (MmsValueCache) GLOBAL_CALLOC(1, sizeof(struct sMmsValueCache));

    if (self)
    {
        self->domain = domain;

        self->map = StringMap_create();
    }

    return self;
}

void
MmsValueCache_insertValue(MmsValueCache self, char* itemId, MmsValue* value)
{
    MmsVariableSpecification* typeSpec = MmsDomain_getNamedVariable(self->domain, itemId);

    if (typeSpec)
    {
        MmsValueCacheEntry* cacheEntry = (MmsValueCacheEntry*) GLOBAL_MALLOC(sizeof(MmsValueCacheEntry));

        cacheEntry->value = value;
        cacheEntry->typeSpec = typeSpec;

        Map_addEntry(self->map, StringUtils_copyString(itemId), cacheEntry);
    }
    else
        if (DEBUG)
            printf("Cannot insert value into cache %s : no typeSpec found!\n", itemId);
}

static char*
getParentSubString(char* itemId)
{
    int len = strlen(itemId);

    char* strPos = itemId + len;

    while (--strPos > itemId)
    {
        if (*strPos == '$')
        {
            *strPos = 0;
            return itemId;
        }
    }

    return NULL;
}

static const char*
getChildSubString (const char* itemId, char* parentId)
{
    return itemId + strlen(parentId) + 1;
}

static MmsValue*
searchCacheForValue(MmsValueCache self, const char* itemId, char* parentId, MmsVariableSpecification** outSpec)
{
    MmsValueCacheEntry* cacheEntry;
    MmsValue* value = NULL;

    cacheEntry = (MmsValueCacheEntry*) Map_getEntry(self->map, (void*) parentId);

    if (cacheEntry == NULL)
    {
        char* parentItemId = getParentSubString(parentId);

        if (parentItemId)
            value = searchCacheForValue(self, itemId, parentItemId, outSpec);
    }
    else
    {
        const char* childId = getChildSubString(itemId, parentId);

        MmsVariableSpecification* typeSpec = MmsDomain_getNamedVariable(self->domain, parentId);
        value = MmsVariableSpecification_getChildValue(typeSpec, cacheEntry->value, childId);

        if (outSpec)
        {
            *outSpec = MmsVariableSpecification_getNamedVariableRecursive(typeSpec, childId);
        }
    }

    return value;
}

MmsValue*
MmsValueCache_lookupValue(MmsValueCache self, const char* itemId, MmsVariableSpecification** outSpec)
{
    /*
     * get value for first matching key substring!
     * Then iterate the value for the exact value.
     */
    MmsValue* value = NULL;

    MmsValueCacheEntry* cacheEntry = (MmsValueCacheEntry*) Map_getEntry(self->map, (void*) itemId);

    if (cacheEntry)
    {
        if (outSpec)
            *outSpec = cacheEntry->typeSpec;

        return cacheEntry->value;
    }
    else
    {
        char itemIdCopy[65];
        StringUtils_copyStringToBuffer(itemId, itemIdCopy);

        char* parentItemId = getParentSubString(itemIdCopy);

        if (parentItemId)
            value = searchCacheForValue(self, itemId, parentItemId, outSpec);
    }

    return value;
}

static MmsValue*
searchCacheForValueEx(MmsValueCache self, const char* itemId, char* parentId, int idx, char* componentId, MmsVariableSpecification** outSpec)
{
    MmsValueCacheEntry* cacheEntry;
    MmsValue* value = NULL;

    cacheEntry = (MmsValueCacheEntry*) Map_getEntry(self->map, (void*) parentId);

    if (cacheEntry == NULL)
    {
        char* parentItemId = getParentSubString(parentId);

        if (parentItemId)
        {
            value = searchCacheForValueEx(self, itemId, parentItemId, idx, componentId, outSpec);
        }
    }
    else
    {
        const char* childId = getChildSubString(itemId, parentId);

        if (childId)
        {
            MmsVariableSpecification* typeSpec = MmsDomain_getNamedVariable(self->domain, parentId);

            value = MmsVariableSpecification_getChildValue(typeSpec, cacheEntry->value, childId);

            if (value)
            {
                if (idx != -1)
                {
                    if (MmsValue_getType(value) == MMS_ARRAY)
                    {
                        MmsValue* elementValue = MmsValue_getElement(value, idx);

                        if (elementValue)
                        {
                            if ((componentId != NULL) && (componentId[0] != 0))
                            {
                                MmsVariableSpecification* childSpec = MmsVariableSpecification_getNamedVariableRecursive(typeSpec, childId);

                                if (childSpec)
                                {
                                    MmsVariableSpecification* elementSpec = childSpec->typeSpec.array.elementTypeSpec;

                                    if (elementSpec)
                                    {
                                        MmsValue* componentValue = MmsVariableSpecification_getChildValue(elementSpec, elementValue, componentId);

                                        if (componentValue)
                                            value = componentValue;
                                    }
                                }
                            }
                            else
                            {
                                value = elementValue;
                            }
                        }
                    }
                }
            }

            if (outSpec)
                *outSpec = MmsVariableSpecification_getNamedVariableRecursive(typeSpec, childId);
        }
    }

    return value;
}

MmsValue*
MmsValueCache_lookupValueEx(MmsValueCache self, const char* itemId, int idx, const char* componentId, MmsVariableSpecification** outSpec)
{
    MmsValue* value = NULL;

    MmsValueCacheEntry* cacheEntry = (MmsValueCacheEntry*) Map_getEntry(self->map, (void*) itemId);

    if (cacheEntry)
    {
        if (outSpec)
            *outSpec = cacheEntry->typeSpec;

        return cacheEntry->value;
    }
    else
    {
        char itemIdCopy[65];
        char componentIdCopyBuf[65];

        StringUtils_copyStringMax(itemIdCopy, 65, itemId);

        char* componentIdCopy = NULL;

        if (componentId)
            componentIdCopy = StringUtils_copyStringMax(componentIdCopyBuf, 65, componentId);

        char* parentItemId = getParentSubString(itemIdCopy);

        if (parentItemId)
            value = searchCacheForValueEx(self, itemId, parentItemId, idx, componentIdCopy, outSpec);
    }

    return value;
}

static void
cacheEntryDelete(MmsValueCacheEntry* entry)
{
    if (entry)
    {
        MmsValue_delete(entry->value);
        GLOBAL_FREEMEM(entry);
    }
}

void
MmsValueCache_destroy(MmsValueCache self)
{
    if (self)
    {
        Map_deleteDeep(self->map, true, (void (*) (void*)) cacheEntryDelete);
        GLOBAL_FREEMEM(self);
    }
}
