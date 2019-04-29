/*
 *  mms_device.c
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
#include "mms_server_internal.h"
#include "mms_device_model.h"
#include "stack_config.h"

MmsDevice*
MmsDevice_create(char* deviceName)
{
    MmsDevice* self = (MmsDevice*) GLOBAL_CALLOC(1, sizeof(MmsDevice));
    self->deviceName = deviceName;

    self->namedVariableLists = LinkedList_create();

    return self;
}

void
MmsDevice_destroy(MmsDevice* self)
{

    int i;
    for (i = 0; i < self->domainCount; i++) {
        MmsDomain_destroy(self->domains[i]);
    }

#if (CONFIG_MMS_SUPPORT_VMD_SCOPE_NAMED_VARIABLES == 1)
    if (self->namedVariables != NULL) {
        for (i = 0; i < self->namedVariablesCount; i++) {
            MmsVariableSpecification_destroy(self->namedVariables[i]);
        }

        GLOBAL_FREEMEM(self->namedVariables);
    }
#endif /* (CONFIG_MMS_SUPPORT_VMD_SCOPE_NAMED_VARIABLES == 1) */

    LinkedList_destroyDeep(self->namedVariableLists, (LinkedListValueDeleteFunction) MmsNamedVariableList_destroy);

    GLOBAL_FREEMEM(self->domains);
    GLOBAL_FREEMEM(self);
}

MmsDomain*
MmsDevice_getDomain(MmsDevice* self, const char* domainId)
{
    int i;

    for (i = 0; i < self->domainCount; i++) {
        if (strcmp(self->domains[i]->domainName, domainId) == 0) {
            return self->domains[i];
        }

    }

    return NULL;
}

#if (CONFIG_MMS_SUPPORT_VMD_SCOPE_NAMED_VARIABLES == 1)
MmsVariableSpecification*
MmsDevice_getNamedVariable(MmsDevice* self, char* variableName)
{
    if (self->namedVariables != NULL) {
        char* separator = strchr(variableName, '$');

        int i;

        if (separator == NULL) {

            for (i = 0; i < self->namedVariablesCount; i++) {
                if (strcmp(self->namedVariables[i]->name, variableName) == 0) {
                    return self->namedVariables[i];
                }
            }

            return NULL;
        }
        else {
            MmsVariableSpecification* namedVariable = NULL;

            for (i = 0; i < self->namedVariablesCount; i++) {

                if (strlen(self->namedVariables[i]->name) == (unsigned) (separator - variableName)) {

                    if (strncmp(self->namedVariables[i]->name, variableName, separator - variableName) == 0) {
                        namedVariable = self->namedVariables[i];
                        break;
                    }
                }
            }

            if (namedVariable != NULL) {
                namedVariable = MmsVariableSpecification_getNamedVariableRecursive(namedVariable, separator + 1);
            }

            return namedVariable;
        }
    }

    return NULL;
}
#endif /* (CONFIG_MMS_SUPPORT_VMD_SCOPE_NAMED_VARIABLES == 1) */

LinkedList
MmsDevice_getNamedVariableLists(MmsDevice* self)
{
    return self->namedVariableLists;
}

MmsNamedVariableList
MmsDevice_getNamedVariableListWithName(MmsDevice* self, const char* variableListName)
{
    return mmsServer_getNamedVariableListWithName(self->namedVariableLists, variableListName);
}

