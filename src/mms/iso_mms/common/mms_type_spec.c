/*
 *  mms_type_spec.c
 *
 *  Copyright 2013-2024 Michael Zillgith
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
#include "mms_common.h"
#include "mms_type_spec.h"
#include "mms_value_internal.h"

void
MmsVariableSpecification_destroy(MmsVariableSpecification* typeSpec)
{
    if (typeSpec == NULL)
        return;

    if (typeSpec->name != NULL)
        GLOBAL_FREEMEM(typeSpec->name);

    if (typeSpec->type == MMS_STRUCTURE)
    {
        int elementCount = typeSpec->typeSpec.structure.elementCount;
        int i;
        for (i = 0; i < elementCount; i++)
        {
            MmsVariableSpecification_destroy(typeSpec->typeSpec.structure.elements[i]);
        }

        GLOBAL_FREEMEM(typeSpec->typeSpec.structure.elements);
    }
    else if (typeSpec->type == MMS_ARRAY)
    {
        MmsVariableSpecification_destroy(typeSpec->typeSpec.array.elementTypeSpec);
    }

    GLOBAL_FREEMEM(typeSpec);
}

static size_t
directChildStrLen(const char* childId)
{
    size_t i = 0;
    size_t childIdLen = strlen(childId);

    while (i < childIdLen)
    {
        if (*(childId + i) == '$')
            break;
        if (*(childId + i) == '.')
            break;

        i++;
    }

    return i;
}

MmsValue*
MmsVariableSpecification_getChildValue(MmsVariableSpecification* typeSpec, MmsValue* value, const char* childId)
{
    if ((typeSpec->type == MMS_STRUCTURE) && (value->type == MMS_STRUCTURE))
    {
        size_t childLen = directChildStrLen(childId);
        int i;

        if (typeSpec->typeSpec.structure.elementCount != value->value.structure.size)
            return NULL;

        for (i = 0; i < typeSpec->typeSpec.structure.elementCount; i++)
        {
            if (strlen(typeSpec->typeSpec.structure.elements[i]->name) == childLen)
            {
                if (strncmp(typeSpec->typeSpec.structure.elements[i]->name, childId, childLen) == 0)
                {
                    if (childLen == strlen(childId))
                    {
                        return value->value.structure.components[i];
                    }
                    else
                    {
                        return MmsVariableSpecification_getChildValue(typeSpec->typeSpec.structure.elements[i],
                                value->value.structure.components[i], childId + childLen + 1);
                    }
                }
            }
        }
        return NULL;
    }
    else
        return NULL;
}

MmsType
MmsVariableSpecification_getType(MmsVariableSpecification* self)
{
    return self->type;
}

bool
MmsVariableSpecification_isValueOfType(MmsVariableSpecification* self, const MmsValue* value)
{
    if ((self->type) == (value->type))
    {
        if ((self->type == MMS_STRUCTURE) || (self->type == MMS_ARRAY))
        {
            int componentCount = self->typeSpec.structure.elementCount;

            if (componentCount != (int) MmsValue_getArraySize(value))
                    return false;

            if (self->type == MMS_STRUCTURE)
            {
                int i;
                for (i = 0; i < componentCount; i++)
                {
                    if (MmsVariableSpecification_isValueOfType(self->typeSpec.structure.elements[i], MmsValue_getElement(value, i)) == false)
                        return false;
                }

                return true;
            }
            else
            {
                int i;
                for (i = 0; i < componentCount; i++)
                {
                    if (MmsVariableSpecification_isValueOfType(self->typeSpec.array.elementTypeSpec, MmsValue_getElement(value, i)) == false)
                        return false;
                }

                return true;
            }
        }
        else if (self->type == MMS_BIT_STRING)
        {
            if (self->typeSpec.bitString == value->value.bitString.size)
                return true;

            if (self->typeSpec.bitString < 0)
                return true;
        }
        else if (self->type == MMS_FLOAT)
        {
            if ((self->typeSpec.floatingpoint.exponentWidth == value->value.floatingPoint.exponentWidth) &&
                    (self->typeSpec.floatingpoint.formatWidth == value->value.floatingPoint.formatWidth))
                return true;
        }
        else if (self->type == MMS_BINARY_TIME)
        {
            if (self->typeSpec.binaryTime == value->value.binaryTime.size)
                return true;
        }
        else
            return true;
    }

    return false;
}

const char*
MmsVariableSpecification_getName(MmsVariableSpecification* self)
{
    return self->name;
}

LinkedList /* <char*> */
MmsVariableSpecification_getStructureElements(MmsVariableSpecification* self)
{
    if (self->type != MMS_STRUCTURE)
        return NULL;

    LinkedList elementNames = LinkedList_create();

    int i;
    for (i = 0; i < self->typeSpec.structure.elementCount; i++)
    {
        MmsVariableSpecification* typeSpec = self->typeSpec.structure.elements[i];

        LinkedList_add(elementNames, StringUtils_copyString(typeSpec->name));
    }

    return elementNames;
}

MmsVariableSpecification*
MmsVariableSpecification_getNamedVariableRecursive(MmsVariableSpecification* variable, const char* nameId)
{
    if (variable->type == MMS_ARRAY) {
        return MmsVariableSpecification_getNamedVariableRecursive(variable->typeSpec.array.elementTypeSpec, nameId);
    }

    const char* separator = strchr(nameId, '$');
    int i;

    if (separator == NULL)
    {
        i = 0;

        if (variable->type == MMS_STRUCTURE)
        {
            for (i = 0; i < variable->typeSpec.structure.elementCount; i++)
            {
                if (strcmp(variable->typeSpec.structure.elements[i]->name, nameId) == 0) {
                    return variable->typeSpec.structure.elements[i];
                }
            }
        }

        return NULL;
    }
    else
    {
        MmsVariableSpecification* namedVariable = NULL;
        i = 0;

        for (i = 0; i < variable->typeSpec.structure.elementCount; i++)
        {
            if (strlen(variable->typeSpec.structure.elements[i]->name) == (unsigned) (separator - nameId))
            {
                if (strncmp(variable->typeSpec.structure.elements[i]->name, nameId, separator - nameId) == 0)
                {
                    namedVariable = variable->typeSpec.structure.elements[i];
                    break;
                }
            }
        }

        if (namedVariable != NULL)
        {
            if (namedVariable->type == MMS_STRUCTURE)
            {
                namedVariable = MmsVariableSpecification_getNamedVariableRecursive(namedVariable, separator + 1);
            }
            else if (namedVariable->type == MMS_ARRAY)
            {
                namedVariable = namedVariable->typeSpec.array.elementTypeSpec;

                namedVariable = MmsVariableSpecification_getNamedVariableRecursive(namedVariable, separator + 1);
            }
            else /* ERROR: request for a child element of a simple (leaf-node) variable */
                return NULL;
        }

        return namedVariable;
    }
}

int
MmsVariableSpecification_getSize(MmsVariableSpecification* self)
{
    switch (self->type)
    {
    case MMS_STRUCTURE:
        return self->typeSpec.structure.elementCount;
    case MMS_ARRAY:
        return self->typeSpec.array.elementCount;
    case MMS_INTEGER:
        return self->typeSpec.integer;
    case MMS_UNSIGNED:
        return self->typeSpec.unsignedInteger;
    case MMS_FLOAT:
        return self->typeSpec.floatingpoint.formatWidth;
    case MMS_BIT_STRING:
        return self->typeSpec.bitString;
    case MMS_BINARY_TIME:
        return self->typeSpec.binaryTime;
    case MMS_OCTET_STRING:
        return self->typeSpec.octetString;
    case MMS_VISIBLE_STRING:
        return self->typeSpec.visibleString;
    case MMS_STRING:
        return self->typeSpec.mmsString;
    default:
        return -1;
    }
}

MmsVariableSpecification*
MmsVariableSpecification_getChildSpecificationByIndex(MmsVariableSpecification* self, int index)
{
    if (self->type != MMS_STRUCTURE)
        return NULL;

    if (index >= self->typeSpec.structure.elementCount)
        return NULL;

    if (index < 0)
        return NULL;

    return self->typeSpec.structure.elements[index];
}

MmsVariableSpecification*
MmsVariableSpecification_getChildSpecificationByName(MmsVariableSpecification* self, const char* name, int* index)
{
    if (self->type != MMS_STRUCTURE)
        return NULL;

    int i;

    for (i = 0; i < self->typeSpec.structure.elementCount; i++)
    {
        if (!strcmp(self->typeSpec.structure.elements[i]->name, name))
        {
            if (index != NULL)
                *index = i;

            return self->typeSpec.structure.elements[i];
        }
    }

    return NULL;
}

MmsVariableSpecification*
MmsVariableSpecification_getArrayElementSpecification(MmsVariableSpecification* self)
{
    if (self->type != MMS_ARRAY)
        return NULL;

    return self->typeSpec.array.elementTypeSpec;
}

int
MmsVariableSpecification_getExponentWidth(MmsVariableSpecification* self)
{
    if (self->type != MMS_FLOAT)
        return -1;

    return (int) self->typeSpec.floatingpoint.exponentWidth;
}
