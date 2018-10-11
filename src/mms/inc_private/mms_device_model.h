/*
 *  mms_model.h
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

#ifndef MMS_DEVICE_MODEL_H_
#define MMS_DEVICE_MODEL_H_

#include "libiec61850_common_api.h"

#include "mms_type_spec.h"
#include "mms_common.h"
#include "mms_named_variable_list.h"
#include "logging_api.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    char* deviceName;

    /* MMS VMD scope variables support */
    int namedVariablesCount;
    MmsVariableSpecification** namedVariables;

    /* MMS VMD scope named variables list support */
    LinkedList /*<MmsNamedVariableList>*/ namedVariableLists;

    /* MMS domain support */
    int domainCount;
    MmsDomain** domains;
} MmsDevice;


struct sMmsJournal {
    char* name;
    LogStorage logStorage;
};

typedef struct sMmsJournal* MmsJournal;

/*
 * Server side data structure to hold information of an MMS domain (Logical Device)
 */
struct sMmsDomain {
    char* domainName;
    int namedVariablesCount;
    MmsVariableSpecification** namedVariables;
    LinkedList /*<MmsNamedVariableList>*/ namedVariableLists;
    LinkedList /* <MmsJournal> */ journals;
};

/**
 * @defgroup mms_server_api_group MMS server API (for IEC 61850 use IEC 61850 server API instead!)
 *
 * @{
 */

/**
 * \brief Create a new MmsDomain instance
 *
 * This method should not be invoked by client code!
 *
 * \param domainName the name of the MMS domain
 *
 * \return the new MmsDomain instance
 */
LIB61850_INTERNAL MmsDomain*
MmsDomain_create(char* domainName);

LIB61850_INTERNAL char*
MmsDomain_getName(MmsDomain* self);

LIB61850_INTERNAL void
MmsDomain_addJournal(MmsDomain* self, const char* name);

LIB61850_INTERNAL MmsJournal
MmsDomain_getJournal(MmsDomain* self, const char* name);

/**
 * Delete a MmsDomain instance
 *
 * This method should not be invoked by client code!
 */
LIB61850_INTERNAL void
MmsDomain_destroy(MmsDomain* self);

/**
 * \brief Add a new MMS Named Variable List (Data set) to a MmsDomain instance
 *
 * The passed MmsNamedVariableList instance will be handled by the MmsDomain instance
 * and will be destroyed if the MmsDomain_destroy function on this MmsDomain instance
 * is called.
 *
 * \param self instance of MmsDomain to operate on
 * \param variableList new named variable list that will be added to this MmsDomain
 *
 * \return true if operation was successful.
 */
LIB61850_INTERNAL bool
MmsDomain_addNamedVariableList(MmsDomain* self, MmsNamedVariableList variableList);

/**
 * \brief Delete a MMS Named Variable List from this MmsDomain instance
 *
 * A call to this function will also destroy the MmsNamedVariableList instance.
 *
 * \param self instance of MmsDomain to operate on
 * \param variableListName the name of the variable list to delete.
 *
 */
LIB61850_INTERNAL void
MmsDomain_deleteNamedVariableList(MmsDomain* self, char* variableListName);

LIB61850_INTERNAL MmsNamedVariableList
MmsDomain_getNamedVariableList(MmsDomain* self, const char* variableListName);

LIB61850_INTERNAL LinkedList
MmsDomain_getNamedVariableLists(MmsDomain* self);

LIB61850_INTERNAL LinkedList
MmsDomain_getNamedVariableListValues(MmsDomain* self, char* variableListName);

LIB61850_INTERNAL LinkedList
MmsDomain_createNamedVariableListValues(MmsDomain* self, char* variableListName);

/**
 * \brief Get the MmsTypeSpecification instance of a MMS named variable
 *
 * \param self instance of MmsDomain to operate on
 * \param nameId name of the named variable
 *
 * \return MmsTypeSpecification instance of the named variable
 */
LIB61850_INTERNAL MmsVariableSpecification*
MmsDomain_getNamedVariable(MmsDomain* self, char* nameId);

/**
 * \brief Create a new MmsDevice instance.
 *
 * MmsDevice objects are the root objects of the address space of an MMS server.
 *
 * An MmsDevice object can contain one or more MmsDomain instances.
 *
 * \param deviceName the name of the MMS device or NULL if the device has no name.
 *
 * \return the new MmsDevice instance
 */
LIB61850_INTERNAL MmsDevice*
MmsDevice_create(char* deviceName);

/**
 * \brief Delete the MmsDevice instance
 */
LIB61850_INTERNAL void
MmsDevice_destroy(MmsDevice* self);

/**
 * \brief Get the MmsDomain object with the specified MMS domain name.
 *
 * \param deviceName the name of the MMS device or NULL if the device has no name.
 *
 * \return the new MmsDevice instance
 */
LIB61850_INTERNAL MmsDomain*
MmsDevice_getDomain(MmsDevice* self, const char* domainId);

/**
 * \brief Get the MmsTypeSpecification instance of a MMS named variable of VMD scope
 *
 * \param self instance of MmsDevice to operate on
 * \param variableName name of the named variable
 *
 * \return MmsTypeSpecification instance of the named variable
 */
LIB61850_INTERNAL MmsVariableSpecification*
MmsDevice_getNamedVariable(MmsDevice* self, char* variableName);

LIB61850_INTERNAL LinkedList
MmsDevice_getNamedVariableLists(MmsDevice* self);

LIB61850_INTERNAL MmsNamedVariableList
MmsDevice_getNamedVariableListWithName(MmsDevice* self, const char* variableListName);

LIB61850_INTERNAL MmsJournal
MmsJournal_create(const char* name);

LIB61850_INTERNAL void
MmsJournal_destroy(MmsJournal self);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif /* MMS_DEVICE_MODEL_H_ */
