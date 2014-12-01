/*
 * 	mms_domain.c
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

#include "mms_device_model.h"
#include "mms_server_internal.h"

static void
freeNamedVariables(MmsVariableSpecification** variables, int variablesCount)
{
	int i;
	for (i = 0; i < variablesCount; i++) {
	    MmsVariableSpecification_destroy(variables[i]);
	}
}

MmsDomain*
MmsDomain_create(char* domainName)
{
	MmsDomain* self = (MmsDomain*) calloc(1, sizeof(MmsDomain));

	self->domainName = copyString(domainName);
	self->namedVariableLists = LinkedList_create();

	return self;
}

void
MmsDomain_destroy(MmsDomain* self)
{
	free(self->domainName);

	if (self->namedVariables != NULL) {
		freeNamedVariables(self->namedVariables,
				self->namedVariablesCount);

		free(self->namedVariables);
	}

	LinkedList_destroyDeep(self->namedVariableLists, (LinkedListValueDeleteFunction) MmsNamedVariableList_destroy);

	free(self);
}

char*
MmsDomain_getName(MmsDomain* self)
{
	return self->domainName;
}

bool
MmsDomain_addNamedVariableList(MmsDomain* self, MmsNamedVariableList variableList)
{
	//TODO check if operation is allowed!

	LinkedList_add(self->namedVariableLists, variableList);

	return true;
}

MmsNamedVariableList
MmsDomain_getNamedVariableList(MmsDomain* self, char* variableListName)
{
	MmsNamedVariableList variableList = NULL;

	LinkedList element = LinkedList_getNext(self->namedVariableLists);

	while (element != NULL) {
		MmsNamedVariableList varList = (MmsNamedVariableList) element->data;

		if (strcmp(MmsNamedVariableList_getName(varList), variableListName) == 0) {
			variableList = varList;
			break;
		}

		element = LinkedList_getNext(element);
	}

	return variableList;
}

void
MmsDomain_deleteNamedVariableList(MmsDomain* self, char* variableListName)
{
	mmsServer_deleteVariableList(self->namedVariableLists, variableListName);
}

LinkedList
MmsDomain_getNamedVariableLists(MmsDomain* self)
{
	return self->namedVariableLists;
}

MmsVariableSpecification*
MmsDomain_getNamedVariable(MmsDomain* self, char* nameId)
{
	if (self->namedVariables != NULL) {

		char* separator = strchr(nameId, '$');

		int i;

		if (separator == NULL) {

			for (i = 0; i < self->namedVariablesCount; i++) {
				if (strcmp(self->namedVariables[i]->name, nameId) == 0) {
					return self->namedVariables[i];
				}
			}

			return NULL;
		}
		else {
			MmsVariableSpecification* namedVariable = NULL;

			for (i = 0; i < self->namedVariablesCount; i++) {

				if (strlen(self->namedVariables[i]->name) == (unsigned) (separator - nameId)) {

					if (strncmp(self->namedVariables[i]->name, nameId, separator - nameId) == 0) {
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
