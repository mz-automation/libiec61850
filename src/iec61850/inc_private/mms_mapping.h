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

#include "iec61850_common_internal.h"
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

LIB61850_INTERNAL MmsMapping*
MmsMapping_create(IedModel* model, IedServer iedServer);

LIB61850_INTERNAL MmsDevice*
MmsMapping_getMmsDeviceModel(MmsMapping* mapping);

LIB61850_INTERNAL void
MmsMapping_initializeControlObjects(MmsMapping* self);

LIB61850_INTERNAL void
MmsMapping_configureSettingGroups(MmsMapping* self);

LIB61850_INTERNAL void
MmsMapping_checkForSettingGroupReservationTimeouts(MmsMapping* self, uint64_t currentTime);

LIB61850_INTERNAL void
MmsMapping_setSgChangedHandler(MmsMapping* self, SettingGroupControlBlock* sgcb,
        ActiveSettingGroupChangedHandler handler, void* parameter);

LIB61850_INTERNAL void
MmsMapping_setEditSgChangedHandler(MmsMapping* self, SettingGroupControlBlock* sgcb,
        EditSettingGroupChangedHandler handler, void* parameter);

LIB61850_INTERNAL void
MmsMapping_setConfirmEditSgHandler(MmsMapping* self, SettingGroupControlBlock* sgcb,
        EditSettingGroupConfirmationHandler handler, void* parameter);

LIB61850_INTERNAL void
MmsMapping_changeActiveSettingGroup(MmsMapping* self, SettingGroupControlBlock* sgcb, uint8_t newActiveSg);

LIB61850_INTERNAL void
MmsMapping_setMmsServer(MmsMapping* self, MmsServer server);

LIB61850_INTERNAL void
MmsMapping_installHandlers(MmsMapping* self);

LIB61850_INTERNAL void
MmsMapping_destroy(MmsMapping* mapping);

LIB61850_INTERNAL void
MmsMapping_startEventWorkerThread(MmsMapping* self);

LIB61850_INTERNAL void
MmsMapping_stopEventWorkerThread(MmsMapping* self);

LIB61850_INTERNAL DataSet*
MmsMapping_createDataSetByNamedVariableList(MmsMapping* self, MmsNamedVariableList variableList);

LIB61850_INTERNAL void
MmsMapping_triggerReportObservers(MmsMapping* self, MmsValue* value, int flag);

LIB61850_INTERNAL void
MmsMapping_triggerLogging(MmsMapping* self, MmsValue* value, LogInclusionFlag flag);

LIB61850_INTERNAL void
MmsMapping_triggerGooseObservers(MmsMapping* self, MmsValue* value);

LIB61850_INTERNAL void
MmsMapping_enableGoosePublishing(MmsMapping* self);

LIB61850_INTERNAL void
MmsMapping_disableGoosePublishing(MmsMapping* self);

LIB61850_INTERNAL void
MmsMapping_useIntegratedGoosePublisher(MmsMapping* self, bool enable);

LIB61850_INTERNAL void
MmsMapping_useGooseVlanTag(MmsMapping* self, LogicalNode* ln, const char* gcbName, bool useVlanTag);

LIB61850_INTERNAL void
MmsMapping_setGooseInterfaceId(MmsMapping* self,  LogicalNode* ln, const char* gcbName, const char* interfaceId);

LIB61850_INTERNAL void
MmsMapping_addControlObject(MmsMapping* self, ControlObject* controlObject);

LIB61850_INTERNAL char*
MmsMapping_getNextNameElement(char* name);

LIB61850_INTERNAL void /* Create PHYCOMADDR ACSI type instance */
MmsMapping_createPhyComAddrStructure(MmsVariableSpecification* namedVariable);

LIB61850_INTERNAL ControlObject*
MmsMapping_getControlObject(MmsMapping* self, MmsDomain* domain, char* lnName, char* coName);

LIB61850_INTERNAL MmsNamedVariableList
MmsMapping_getDomainSpecificVariableList(MmsMapping* self, const char* variableListReference);

LIB61850_INTERNAL DataSet*
MmsMapping_getDomainSpecificDataSet(MmsMapping* self, const char* dataSetName);

LIB61850_INTERNAL void
MmsMapping_freeDynamicallyCreatedDataSet(DataSet* dataSet);

LIB61850_INTERNAL void
MmsMapping_setConnectionIndicationHandler(MmsMapping* self, IedConnectionIndicationHandler handler, void* parameter);

LIB61850_INTERNAL void
MmsMapping_setLogStorage(MmsMapping* self, const char* logRef, LogStorage logStorage);

LIB61850_INTERNAL void
MmsMapping_installWriteAccessHandler(MmsMapping* self, DataAttribute* dataAttribute, WriteAccessHandler handler, void* parameter);

LIB61850_INTERNAL void
MmsMapping_installReadAccessHandler(MmsMapping* self, ReadAccessHandler handler, void* paramter);

LIB61850_INTERNAL MmsDataAccessError
Control_writeAccessControlObject(MmsMapping* self, MmsDomain* domain, const char* variableIdOrig,
                         MmsValue* value, MmsServerConnection connection);

LIB61850_INTERNAL MmsValue*
Control_readAccessControlObject(MmsMapping* self, MmsDomain* domain, char* variableIdOrig,
        MmsServerConnection connection, bool isDirectAccess);

LIB61850_INTERNAL ControlObject*
Control_lookupControlObject(MmsMapping* self, MmsDomain* domain, char* lnName, char* objectName);

LIB61850_INTERNAL void
Control_processControlActions(MmsMapping* self, uint64_t currentTimeInMs);

#endif /* MMS_MAPPING_H_ */
