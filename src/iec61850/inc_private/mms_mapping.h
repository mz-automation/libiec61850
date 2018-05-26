/*
 *  mms_mapping.h
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

#ifndef MMS_MAPPING_H_
#define MMS_MAPPING_H_

#include "iec61850_model.h"
#include "mms_device_model.h"
#include "control.h"

#define REPORT_CONTROL_NONE 0U
#define REPORT_CONTROL_VALUE_UPDATE 1U
#define REPORT_CONTROL_VALUE_CHANGED 2U
#define REPORT_CONTROL_QUALITY_CHANGED 4U
#define REPORT_CONTROL_NOT_UPDATED 8U

typedef enum {
    LOG_CONTROL_NONE,
    LOG_CONTROL_VALUE_UPDATE,
    LOG_CONTROL_VALUE_CHANGED,
    LOG_CONTROL_QUALITY_CHANGED
} LogInclusionFlag;

typedef struct sMmsMapping MmsMapping;

MmsMapping*
MmsMapping_create(IedModel* model, IedServer iedServer);

MmsDevice*
MmsMapping_getMmsDeviceModel(MmsMapping* mapping);

void
MmsMapping_configureSettingGroups(MmsMapping* self);

void
MmsMapping_checkForSettingGroupReservationTimeouts(MmsMapping* self, uint64_t currentTime);

void
MmsMapping_setSgChangedHandler(MmsMapping* self, SettingGroupControlBlock* sgcb,
        ActiveSettingGroupChangedHandler handler, void* parameter);

void
MmsMapping_setEditSgChangedHandler(MmsMapping* self, SettingGroupControlBlock* sgcb,
        EditSettingGroupChangedHandler handler, void* parameter);

void
MmsMapping_setConfirmEditSgHandler(MmsMapping* self, SettingGroupControlBlock* sgcb,
        EditSettingGroupConfirmationHandler handler, void* parameter);

void
MmsMapping_changeActiveSettingGroup(MmsMapping* self, SettingGroupControlBlock* sgcb, uint8_t newActiveSg);

void
MmsMapping_setMmsServer(MmsMapping* self, MmsServer server);

void
MmsMapping_installHandlers(MmsMapping* self);

void
MmsMapping_destroy(MmsMapping* mapping);

void
MmsMapping_startEventWorkerThread(MmsMapping* self);

void
MmsMapping_stopEventWorkerThread(MmsMapping* self);

DataSet*
MmsMapping_createDataSetByNamedVariableList(MmsMapping* self, MmsNamedVariableList variableList);

void
MmsMapping_triggerReportObservers(MmsMapping* self, MmsValue* value, int flag);

void
MmsMapping_triggerLogging(MmsMapping* self, MmsValue* value, LogInclusionFlag flag);

void
MmsMapping_triggerGooseObservers(MmsMapping* self, MmsValue* value);

void
MmsMapping_enableGoosePublishing(MmsMapping* self);

void
MmsMapping_disableGoosePublishing(MmsMapping* self);

char*
MmsMapping_getMmsDomainFromObjectReference(const char* objectReference, char* buffer);

void
MmsMapping_addControlObject(MmsMapping* self, ControlObject* controlObject);

char*
MmsMapping_createMmsVariableNameFromObjectReference(const char* objectReference, FunctionalConstraint fc, char* buffer);

char*
MmsMapping_getNextNameElement(char* name);

void /* Create PHYCOMADDR ACSI type instance */
MmsMapping_createPhyComAddrStructure(MmsVariableSpecification* namedVariable);

ControlObject*
MmsMapping_getControlObject(MmsMapping* self, MmsDomain* domain, char* lnName, char* coName);

MmsNamedVariableList
MmsMapping_getDomainSpecificVariableList(MmsMapping* self, const char* variableListReference);

DataSet*
MmsMapping_getDomainSpecificDataSet(MmsMapping* self, const char* dataSetName);

void
MmsMapping_freeDynamicallyCreatedDataSet(DataSet* dataSet);

MmsVariableAccessSpecification*
MmsMapping_ObjectReferenceToVariableAccessSpec(char* objectReference);

char*
MmsMapping_varAccessSpecToObjectReference(MmsVariableAccessSpecification* varAccessSpec);

void
MmsMapping_setConnectionIndicationHandler(MmsMapping* self, IedConnectionIndicationHandler handler, void* parameter);

void
MmsMapping_setLogStorage(MmsMapping* self, const char* logRef, LogStorage logStorage);

void
MmsMapping_installWriteAccessHandler(MmsMapping* self, DataAttribute* dataAttribute, WriteAccessHandler handler, void* parameter);

MmsDataAccessError
Control_writeAccessControlObject(MmsMapping* self, MmsDomain* domain, char* variableIdOrig,
                         MmsValue* value, MmsServerConnection connection);

ControlObject*
Control_lookupControlObject(MmsMapping* self, MmsDomain* domain, char* lnName, char* objectName);

void
Control_processControlActions(MmsMapping* self, uint64_t currentTimeInMs);

#endif /* MMS_MAPPING_H_ */
