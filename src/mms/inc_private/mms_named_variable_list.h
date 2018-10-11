/*
 *  mms_named_variable_list.h
 *
 *  Copyright 2013-2018 Michael Zillgith
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

#ifndef MMS_NAMED_VARIABLE_LIST_H_
#define MMS_NAMED_VARIABLE_LIST_H_

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup mms_server_api_group
 *  @{
 */

#include "libiec61850_common_api.h"
#include "linked_list.h"
#include "mms_common.h"

struct sMmsNamedVariableList {
	bool deletable;
	MmsDomain* domain;
	char* name;
	LinkedList listOfVariables;
};

LIB61850_INTERNAL MmsNamedVariableListEntry
MmsNamedVariableListEntry_create(MmsAccessSpecifier accessSpecifier);

LIB61850_INTERNAL void
MmsNamedVariableListEntry_destroy(MmsNamedVariableListEntry self);

LIB61850_INTERNAL MmsDomain*
MmsNamedVariableListEntry_getDomain(MmsNamedVariableListEntry self);

LIB61850_INTERNAL char*
MmsNamedVariableListEntry_getVariableName(MmsNamedVariableListEntry self);

LIB61850_INTERNAL MmsNamedVariableList
MmsNamedVariableList_create(MmsDomain* domain, char* name, bool deletable);

LIB61850_INTERNAL char*
MmsNamedVariableList_getName(MmsNamedVariableList self);

LIB61850_INTERNAL MmsDomain*
MmsNamedVariableList_getDomain(MmsNamedVariableList self);

LIB61850_INTERNAL bool
MmsNamedVariableList_isDeletable(MmsNamedVariableList self);

LIB61850_INTERNAL void
MmsNamedVariableList_addVariable(MmsNamedVariableList self, MmsNamedVariableListEntry variable);

LIB61850_INTERNAL LinkedList
MmsNamedVariableList_getVariableList(MmsNamedVariableList self);

LIB61850_INTERNAL void
MmsNamedVariableList_destroy(MmsNamedVariableList self);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif /* MMS_NAMED_VARIABLE_LIST_H_ */
