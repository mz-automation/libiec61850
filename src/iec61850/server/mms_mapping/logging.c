/*
 *  logging.c
 *
 *  Copyright 2016 Michael Zillgith
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
#include "stack_config.h"
#include "mms_mapping.h"
#include "logging.h"
#include "linked_list.h"
#include "hal_thread.h"

#include "simple_allocator.h"
#include "mem_alloc_linked_list.h"

#include "ied_server_private.h"
#include "mms_mapping_internal.h"
#include "mms_value_internal.h"

#include "logging_api.h"

#if defined(_MSC_VER) && _MSC_VER < 1900
#define snprintf(buf,len, format,...) _snprintf_s(buf, len,len, format, __VA_ARGS__)
#endif

#if (CONFIG_IEC61850_LOG_SERVICE == 1)

LogInstance*
LogInstance_create(LogicalNode* parentLN, const char* name)
{
    LogInstance* self = (LogInstance*) GLOBAL_MALLOC(sizeof(LogInstance));

    self->name = StringUtils_copyString(name);
    self->parentLN = parentLN;
    self->logStorage = NULL;
    self->locked = false;

    self->oldEntryId = 0;
    self->oldEntryTime = 0;
    self->newEntryId = 0;
    self->newEntryTime = 0;

    return self;
}

void
LogInstance_destroy(LogInstance* self)
{
    GLOBAL_FREEMEM(self->name);
    GLOBAL_FREEMEM(self);
}

void
LogInstance_logSingleData(LogInstance* self, const char* dataRef, MmsValue* value, uint8_t flag)
{
    LogStorage logStorage = self->logStorage;

    if (logStorage != NULL) {

        while (self->locked)
            Thread_sleep(1);

        self->locked = true;

        if (DEBUG_IED_SERVER)
            printf("IED_SERVER: Log value - dataRef: %s flag: %i\n", dataRef, flag);

        uint64_t timestamp = Hal_getTimeInMs();

        uint64_t entryID = LogStorage_addEntry(logStorage, timestamp);

        int dataSize = MmsValue_encodeMmsData(value, NULL, 0, false);

        uint8_t* data = (uint8_t*) GLOBAL_MALLOC(dataSize);

        MmsValue_encodeMmsData(value, data, 0, true);

        LogStorage_addEntryData(logStorage, entryID, dataRef, data, dataSize, flag);

        self->locked = false;

        GLOBAL_FREEMEM(data);

        self->newEntryId = entryID;
        self->newEntryTime = timestamp;

    }
    else
        if (DEBUG_IED_SERVER)
            printf("IED_SERVER: no log storage available for logging!\n");
}

uint64_t
LogInstance_logEntryStart(LogInstance* self)
{
    LogStorage logStorage = self->logStorage;

    if (logStorage != NULL) {

        while (self->locked)
            Thread_sleep(1);

        self->locked = true;

        uint64_t timestamp = Hal_getTimeInMs();

        uint64_t entryID = LogStorage_addEntry(logStorage, timestamp);

        return entryID;
    }
    else {
        if (DEBUG_IED_SERVER)
            printf("IED_SERVER: no log storage available for logging!\n");

        return 0;
    }
}

void
LogInstance_logEntryData(LogInstance* self, uint64_t entryID, const char* dataRef, MmsValue* value, uint8_t flag)
{
    LogStorage logStorage = self->logStorage;

    if (logStorage != NULL) {

        int dataSize = MmsValue_encodeMmsData(value, NULL, 0, false);

        uint8_t* data = (uint8_t*) GLOBAL_MALLOC(dataSize);

        MmsValue_encodeMmsData(value, data, 0, true);

        LogStorage_addEntryData(logStorage, entryID, dataRef, data, dataSize, flag);

        self->locked = false;

        GLOBAL_FREEMEM(data);
    }
}

void
LogInstance_logEntryFinished(LogInstance* self, uint64_t entryID)
{
    (void)entryID;

    self->locked = false;
}

void
LogInstance_updateStatus(LogInstance* self)
{
    if (self->logStorage) {
        LogStorage_getOldestAndNewestEntries(self->logStorage, &(self->newEntryId), &(self->newEntryTime),
                &(self->oldEntryId), &(self->oldEntryTime));
    }
}

void
LogInstance_setLogStorage(LogInstance* self, LogStorage logStorage)
{
    self->logStorage = logStorage;

    LogInstance_updateStatus(self);
}

LogControl*
LogControl_create(LogicalNode* parentLN, MmsMapping* mmsMapping)
{
    LogControl* self = (LogControl*) GLOBAL_MALLOC(sizeof(LogControl));

    self->enabled = false;
    self->dataSet = NULL;
    self->isDynamicDataSet = false;
    self->triggerOps = 0;
    self->logicalNode = parentLN;
    self->mmsMapping = mmsMapping;
    self->dataSetRef = NULL;
    self->logInstance = NULL;
    self->intgPd = 0;
    self->nextIntegrityScan = 0;
    self->logRef = NULL;

    return self;
}

void
LogControl_destroy(LogControl* self)
{
    if (self != NULL) {

        MmsValue_delete(self->mmsValue);
        GLOBAL_FREEMEM(self->name);

        if (self->dataSetRef != NULL)
            GLOBAL_FREEMEM(self->dataSetRef);

        if (self->logRef)
            GLOBAL_FREEMEM(self->logRef);

        GLOBAL_FREEMEM(self);
    }
}

void
LogControl_setLog(LogControl* self, LogInstance* logInstance)
{
    self->logInstance = logInstance;
}

static void
prepareLogControl(LogControl* logControl)
{
    if (logControl->dataSetRef == NULL) {
        logControl->enabled = false;
        return;
    }

    DataSet* dataSet = IedModel_lookupDataSet(logControl->mmsMapping->model, logControl->dataSetRef);

    if (dataSet == NULL)
        return;
    else
        logControl->dataSet = dataSet;
}

static bool
enableLogging(LogControl* self)
{
    if ((self->dataSet != NULL) && (self->logInstance != NULL)) {
        self->enabled = true;

        if ((self->triggerOps & TRG_OPT_INTEGRITY) && (self->intgPd != 0))
            self->nextIntegrityScan = Hal_getTimeInMs();
        else
            self->nextIntegrityScan = 0;

        MmsValue* enabled = MmsValue_getSubElement(self->mmsValue, self->mmsType, "LogEna");

        MmsValue_setBoolean(enabled, true);

        return true;
    }
    else
        return false;
}

static LogControlBlock*
getLCBForLogicalNodeWithIndex(MmsMapping* self, LogicalNode* logicalNode, int index)
{
    int lcbCount = 0;

    LogControlBlock* nextLcb = self->model->lcbs;

    while (nextLcb != NULL ) {
        if (nextLcb->parent == logicalNode) {

            if (lcbCount == index)
                return nextLcb;

            lcbCount++;

        }

        nextLcb = nextLcb->sibling;
    }

    return NULL ;
}

static LogControl*
lookupLogControl(MmsMapping* self, MmsDomain* domain, char* lnName, char* objectName)
{
    LinkedList element = LinkedList_getNext(self->logControls);

    while (element != NULL) {
        LogControl* logControl = (LogControl*) element->data;

        if (logControl->domain == domain) {
            if (strcmp(logControl->logicalNode->name, lnName) == 0) {
                if (strcmp(logControl->logControlBlock->name, objectName) == 0) {
                    return logControl;
                }
            }
        }

        element = LinkedList_getNext(element);
    }

    return NULL;
}

static LogInstance*
getLogInstanceByLogRef(MmsMapping* self, const char* logRef)
{
    char refStr[130];
    char* domainName;
    char* lnName;
    char* logName;

    strncpy(refStr, logRef, 129);

    domainName = refStr;

    lnName = strchr(refStr, '/');

    if (lnName == NULL)
        return NULL;

    if ((lnName - domainName) > 64)
        return NULL;

    lnName[0] = 0;
    lnName++;

    logName = strchr(lnName, '$');

    if (logName == NULL)
        return NULL;

    logName[0] = 0;
    logName++;

    LinkedList instance = LinkedList_getNext(self->logInstances);

    while (instance != NULL) {

        LogInstance* logInstance = (LogInstance*) LinkedList_getData(instance);

        if (strcmp(logInstance->name, logName) == 0) {

            if (strcmp(lnName, logInstance->parentLN->name) == 0) {
                LogicalDevice* ld = (LogicalDevice*) logInstance->parentLN->parent;

                if (strcmp(ld->name, domainName) == 0)
                    return logInstance;
            }
        }

        instance = LinkedList_getNext(instance);
    }

    return NULL;
}

static void
updateLogStatusInLCB(LogControl* self)
{
    LogInstance* logInstance = self->logInstance;

    if (logInstance != NULL) {

        LogInstance_updateStatus(logInstance);

        MmsValue_setBinaryTime(self->oldEntrTm, logInstance->oldEntryTime);
        MmsValue_setBinaryTime(self->newEntrTm, logInstance->newEntryTime);

        MmsValue_setOctetString(self->oldEntr, (uint8_t*) &(logInstance->oldEntryId), 8);
        MmsValue_setOctetString(self->newEntr, (uint8_t*) &(logInstance->newEntryId), 8);
    }
}


static void
freeDynamicDataSet(LogControl* self)
{
    if (self->isDynamicDataSet) {
        if (self->dataSet != NULL) {
            MmsMapping_freeDynamicallyCreatedDataSet(self->dataSet);
            self->isDynamicDataSet = false;
            self->dataSet = NULL;
        }
    }
}

#if (CONFIG_IEC61850_SERVICE_TRACKING == 1)

static void
updateGenericTrackingObjectValues(MmsMapping* self, LogControl* logControl, IEC61850_ServiceType serviceType, MmsDataAccessError errVal)
{
    ServiceTrkInstance trkInst = (ServiceTrkInstance) self->locbTrk;

    if (trkInst) {
        if (trkInst->serviceType)
            MmsValue_setInt32(trkInst->serviceType->mmsValue, (int) serviceType);

        if (trkInst->t)
            MmsValue_setUtcTimeMs(trkInst->t->mmsValue, Hal_getTimeInMs());

        if (trkInst->errorCode)
            MmsValue_setInt32(trkInst->errorCode->mmsValue,
                    private_IedServer_convertMmsDataAccessErrorToServiceError(errVal));

        char objRef[130];

        /* create object reference */
        LogicalNode* ln =  logControl->logControlBlock->parent;
        LogicalDevice* ld = (LogicalDevice*) ln->parent;

        char* iedName = self->iedServer->model->name;

        snprintf(objRef, 129, "%s%s/%s.%s", iedName, ld->name, ln->name, logControl->logControlBlock->name);

        if (trkInst->objRef) {
            IedServer_updateVisibleStringAttributeValue(self->iedServer, trkInst->objRef, objRef);
        }
    }
}

static void
copyLCBValuesToTrackingObject(MmsMapping* self, LogControl* logControl)
{
    if (self->locbTrk) {
        LocbTrkInstance trkInst = self->locbTrk;

        if (trkInst->logEna)
            MmsValue_setBoolean(trkInst->logEna->mmsValue, logControl->enabled);

        if (trkInst->logRef)
            MmsValue_setVisibleString(trkInst->logRef->mmsValue, logControl->logRef);

        if (trkInst->datSet) {
            char datSet[130];

            if (logControl->dataSetRef) {
                strncpy(datSet, logControl->dataSetRef, 129);
                datSet[129] = 0;

                StringUtils_replace(datSet, '$', '.');
            }
            else {
                datSet[0] = 0;
            }

            MmsValue_setVisibleString(trkInst->datSet->mmsValue, datSet);
        }

        if (trkInst->intgPd)
            MmsValue_setUint32(trkInst->intgPd->mmsValue, logControl->intgPd);

        if (trkInst->trgOps) {
            MmsValue_setBitStringFromInteger(trkInst->trgOps->mmsValue, logControl->triggerOps * 2);
        }

        /* TODO update other attributes? */
    }
}

#endif /* (CONFIG_IEC61850_SERVICE_TRACKING == 1) */

MmsDataAccessError
LIBIEC61850_LOG_SVC_writeAccessLogControlBlock(MmsMapping* self, MmsDomain* domain, char* variableIdOrig,
        MmsValue* value, MmsServerConnection connection)
{
    (void)connection;

    MmsDataAccessError retVal = DATA_ACCESS_ERROR_SUCCESS;

    bool updateValue = false;

    char variableId[130];

    strncpy(variableId, variableIdOrig, 129);

    char* separator = strchr(variableId, '$');

    *separator = 0;

    char* lnName = variableId;

    if (lnName == NULL)
        return DATA_ACCESS_ERROR_INVALID_ADDRESS;

    char* objectName = MmsMapping_getNextNameElement(separator + 1);

    if (objectName == NULL)
        return DATA_ACCESS_ERROR_INVALID_ADDRESS;

    char* varName = MmsMapping_getNextNameElement(objectName);

    if (varName != NULL)
        *(varName - 1) = 0;

    LogControl* logControl = lookupLogControl(self, domain, lnName, objectName);

    if (logControl == NULL) {
        return DATA_ACCESS_ERROR_OBJECT_NONE_EXISTENT;
    }

    if (strcmp(varName, "LogEna") == 0) {
        bool logEna = MmsValue_getBoolean(value);

        if (logEna == false) {
            logControl->enabled = false;
        }
        else {

            if (enableLogging(logControl)) {
                logControl->enabled = true;

                if (DEBUG_IED_SERVER)
                    printf("IED_SERVER: enabled log control %s\n", logControl->name);
            }
            else {
                retVal = DATA_ACCESS_ERROR_OBJECT_ATTRIBUTE_INCONSISTENT;
                goto exit_function;
            }
        }

       updateValue = true;
    }
    else if (strcmp(varName, "LogRef") == 0) {

        if (logControl->enabled == false) {
            /* check if logRef is valid or NULL */
            const char* logRef = MmsValue_toString(value);

            if (logRef == NULL) {

                logControl->logInstance = NULL;

                updateValue = true;
            }
            else {
               if (strcmp(logRef, "") == 0) {
                   logControl->logInstance = NULL;
                   updateValue = true;
               }
               else {

                   /* remove IED name from logRef */
                   char* iedName = self->mmsDevice->deviceName;

                   uint32_t iedNameLen = strlen(iedName);

                   if (iedNameLen < strlen(logRef)) {
                       if (memcmp(iedName, logRef, iedNameLen) == 0) {
                           logRef = logRef + iedNameLen;
                       }
                   }

                   LogInstance* logInstance = getLogInstanceByLogRef(self, logRef);

                   if (logInstance != NULL) {

                       logControl->logInstance = logInstance;
                       updateValue = true;
                   }
                   else {
                       retVal = DATA_ACCESS_ERROR_OBJECT_VALUE_INVALID;
                       goto exit_function;
                   }
               }
            }
        }
        else {
            retVal = DATA_ACCESS_ERROR_TEMPORARILY_UNAVAILABLE;
            goto exit_function;
        }
    }
    else if (strcmp(varName, "DatSet") == 0) {

        if (logControl->enabled == false) {
             /* check if datSet is valid or NULL/empty */
            const char* dataSetRef = MmsValue_toString(value);

            if (strlen(dataSetRef) == 0) {
                logControl->dataSet = NULL;
                updateValue = true;
            }
            else {
                DataSet* dataSet = IedModel_lookupDataSet(logControl->mmsMapping->model, dataSetRef);

                if (dataSet != NULL) {
                    freeDynamicDataSet(logControl);

                    logControl->dataSet = dataSet;
                    updateValue = true;

                }

#if (MMS_DYNAMIC_DATA_SETS == 1)

                if (dataSet == NULL) {

                    dataSet = MmsMapping_getDomainSpecificDataSet(self, dataSetRef);

                    if (dataSet == NULL) {

                        if (dataSetRef[0] == '/') { /* check for VMD specific data set */
                            MmsNamedVariableList mmsVariableList =
                                    MmsDevice_getNamedVariableListWithName(self->mmsDevice, dataSetRef + 1);

                            if (mmsVariableList != NULL)
                                dataSet = MmsMapping_createDataSetByNamedVariableList(self, mmsVariableList);
                        }
                    }

                    if (dataSet != NULL) {
                        freeDynamicDataSet(logControl);
                        logControl->dataSet = dataSet;
                        logControl->isDynamicDataSet = true;

                        updateValue = true;
                    }

                }

#endif /*(MMS_DYNAMIC_DATA_SETS == 1) */

                if (dataSet == NULL) {
                    if (DEBUG_IED_SERVER)
                        printf("IED_SERVER:   data set (%s) not found!\n", logControl->dataSetRef);

                    retVal = DATA_ACCESS_ERROR_OBJECT_VALUE_INVALID;
                    goto exit_function;
                }
            }
        }
        else {
            retVal = DATA_ACCESS_ERROR_TEMPORARILY_UNAVAILABLE;
            goto exit_function;
        }
    }
    else if (strcmp(varName, "IntgPd") == 0) {
        if (logControl->enabled == false) {
            logControl->intgPd = MmsValue_toUint32(value);
            updateValue = true;
        }
        else {
            retVal = DATA_ACCESS_ERROR_TEMPORARILY_UNAVAILABLE;
            goto exit_function;
        }
    }
    else if (strcmp(varName, "TrgOps") == 0) {
        if (logControl->enabled == false) {
            logControl->triggerOps = (MmsValue_getBitStringAsInteger(value) / 2);
            updateValue = true;
        }
        else {
            retVal = DATA_ACCESS_ERROR_TEMPORARILY_UNAVAILABLE;
            goto exit_function;
        }
    }

    if (updateValue) {
        MmsValue* element = MmsValue_getSubElement(logControl->mmsValue, logControl->mmsType, varName);

        MmsValue_update(element, value);

        retVal = DATA_ACCESS_ERROR_SUCCESS;
        goto exit_function;
    }

    retVal = DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED;

exit_function:

#if (CONFIG_IEC61850_SERVICE_TRACKING == 1)
    copyLCBValuesToTrackingObject(self, logControl);
    updateGenericTrackingObjectValues(self, logControl, IEC61850_SERVICE_TYPE_SET_LCB_VALUES, retVal);
#endif

    return retVal;
}

MmsValue*
LIBIEC61850_LOG_SVC_readAccessControlBlock(MmsMapping* self, MmsDomain* domain, char* variableIdOrig)
{
    MmsValue* value = NULL;

    char variableId[130];

    strncpy(variableId, variableIdOrig, 129);

    char* separator = strchr(variableId, '$');

    *separator = 0;

    char* lnName = variableId;

    if (lnName == NULL)
        return NULL;

    char* objectName = MmsMapping_getNextNameElement(separator + 1);

    if (objectName == NULL)
        return NULL;

    char* varName = MmsMapping_getNextNameElement(objectName);

    if (varName != NULL)
        *(varName - 1) = 0;

    LogControl* logControl = lookupLogControl(self, domain, lnName, objectName);

    if (logControl != NULL) {

        updateLogStatusInLCB(logControl);

        if (varName != NULL) {
            value = MmsValue_getSubElement(logControl->mmsValue, logControl->mmsType, varName);
        }
        else {
            value = logControl->mmsValue;
        }
    }

    return value;
}


static char*
createDataSetReferenceForDefaultDataSet(LogControlBlock* lcb, LogControl* logControl)
{
    char* dataSetReference;

    char* domainName = MmsDomain_getName(logControl->domain);
    char* lnName = lcb->parent->name;

    dataSetReference = StringUtils_createString(5, domainName, "/", lnName, "$", lcb->dataSetName);

    return dataSetReference;
}

static MmsValue*
createTrgOps(LogControlBlock* logControlBlock) {
    MmsValue* trgOps = MmsValue_newBitString(-6);

    uint8_t triggerOps = logControlBlock->trgOps;

    if (triggerOps & TRG_OPT_DATA_CHANGED)
        MmsValue_setBitStringBit(trgOps, 1, true);
    if (triggerOps & TRG_OPT_QUALITY_CHANGED)
        MmsValue_setBitStringBit(trgOps, 2, true);
    if (triggerOps & TRG_OPT_DATA_UPDATE)
        MmsValue_setBitStringBit(trgOps, 3, true);
    if (triggerOps & TRG_OPT_INTEGRITY)
        MmsValue_setBitStringBit(trgOps, 4, true);

    return trgOps;
}

static void
LogControl_updateLogEna(LogControl* self)
{
    MmsValue_setBoolean(MmsValue_getElement(self->mmsValue, 0), self->enabled);
}

static MmsVariableSpecification*
createLogControlBlock(MmsMapping* self, LogControlBlock* logControlBlock,
        LogControl* logControl)
{
    MmsVariableSpecification* lcb = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    lcb->name = StringUtils_copyString(logControlBlock->name);
    lcb->type = MMS_STRUCTURE;

    MmsValue* mmsValue = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));
    mmsValue->deleteValue = false;
    mmsValue->type = MMS_STRUCTURE;

    int structSize = 9;

    mmsValue->value.structure.size = structSize;
    mmsValue->value.structure.components = (MmsValue**) GLOBAL_CALLOC(structSize, sizeof(MmsValue*));

    lcb->typeSpec.structure.elementCount = structSize;

    lcb->typeSpec.structure.elements = (MmsVariableSpecification**) GLOBAL_CALLOC(structSize,
            sizeof(MmsVariableSpecification*));

    /* LogEna */
    MmsVariableSpecification* namedVariable =
            (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));

    namedVariable->name = StringUtils_copyString("LogEna");
    namedVariable->type = MMS_BOOLEAN;

    lcb->typeSpec.structure.elements[0] = namedVariable;
    mmsValue->value.structure.components[0] = MmsValue_newBoolean(logControlBlock->logEna);

    /* LogRef */
    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("LogRef");
    namedVariable->typeSpec.visibleString = -129;
    namedVariable->type = MMS_VISIBLE_STRING;
    lcb->typeSpec.structure.elements[1] = namedVariable;

    if (logControlBlock->logRef != NULL) {
        char logRef[130];

        int maxLogRefLength = 129 - strlen(self->model->name);

        strcpy(logRef, self->model->name);
        strncat(logRef, logControlBlock->logRef, maxLogRefLength);

        mmsValue->value.structure.components[1] = MmsValue_newVisibleString(logRef);

        StringUtils_replace(logRef, '$', '.');

        logControl->logRef = StringUtils_copyString(logRef);
    }
    else {
        char* logRef = StringUtils_createString(4, logControl->domain->domainName, "/", logControlBlock->parent->name,
                "$GeneralLog");

        mmsValue->value.structure.components[1] = MmsValue_newVisibleString(logRef);

        StringUtils_replace(logRef, '$', '.');

        logControl->logRef = logRef;
    }

    /* DatSet */
    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("DatSet");
    namedVariable->typeSpec.visibleString = -129;
    namedVariable->type = MMS_VISIBLE_STRING;
    lcb->typeSpec.structure.elements[2] = namedVariable;

    if (logControlBlock->dataSetName != NULL) {
        char* dataSetReference = createDataSetReferenceForDefaultDataSet(logControlBlock, logControl);

        logControl->dataSetRef = dataSetReference;

        mmsValue->value.structure.components[2] = MmsValue_newVisibleString(dataSetReference);
    }
    else
        mmsValue->value.structure.components[2] = MmsValue_newVisibleString("");

    /* OldEntrTm */
    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("OldEntrTm");
    namedVariable->type = MMS_BINARY_TIME;
    namedVariable->typeSpec.binaryTime = 6;
    lcb->typeSpec.structure.elements[3] = namedVariable;

    mmsValue->value.structure.components[3] = MmsValue_newBinaryTime(false);

    logControl->oldEntrTm = mmsValue->value.structure.components[3];

    /* NewEntrTm */
    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("NewEntrTm");
    namedVariable->type = MMS_BINARY_TIME;
    namedVariable->typeSpec.binaryTime = 6;
    lcb->typeSpec.structure.elements[4] = namedVariable;

    mmsValue->value.structure.components[4] = MmsValue_newBinaryTime(false);

    logControl->newEntrTm = mmsValue->value.structure.components[4];

    /* OldEntr */
    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("OldEntr");
    namedVariable->type = MMS_OCTET_STRING;
    namedVariable->typeSpec.octetString = 8;

    lcb->typeSpec.structure.elements[5] = namedVariable;

    mmsValue->value.structure.components[5] = MmsValue_newOctetString(8, 8);

    logControl->oldEntr = mmsValue->value.structure.components[5];

    /* NewEntr */
    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("NewEntr");
    namedVariable->type = MMS_OCTET_STRING;
    namedVariable->typeSpec.octetString = 8;

    lcb->typeSpec.structure.elements[6] = namedVariable;

    mmsValue->value.structure.components[6] = MmsValue_newOctetString(8, 8);

    logControl->newEntr = mmsValue->value.structure.components[6];

    /* TrgOps */
    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("TrgOps");
    namedVariable->type = MMS_BIT_STRING;
    namedVariable->typeSpec.bitString = -6;
    lcb->typeSpec.structure.elements[7] = namedVariable;
    mmsValue->value.structure.components[7] = createTrgOps(logControlBlock);

    /* IntgPd */
    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("IntgPd");
    namedVariable->type = MMS_UNSIGNED;
    namedVariable->typeSpec.unsignedInteger = 32;
    lcb->typeSpec.structure.elements[8] = namedVariable;
    mmsValue->value.structure.components[8] =
            MmsValue_newUnsignedFromUint32(logControlBlock->intPeriod);

    logControl->intgPd = logControlBlock->intPeriod;

    logControl->mmsType = lcb;
    logControl->mmsValue = mmsValue;
    logControl->logControlBlock = logControlBlock;
    logControl->triggerOps = logControlBlock->trgOps;

    logControl->enabled = logControlBlock->logEna;

    prepareLogControl(logControl);

    return lcb;
}

MmsVariableSpecification*
Logging_createLCBs(MmsMapping* self, MmsDomain* domain, LogicalNode* logicalNode,
        int lcbCount)
{
    MmsVariableSpecification* namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1,
            sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("LG");
    namedVariable->type = MMS_STRUCTURE;

    namedVariable->typeSpec.structure.elementCount = lcbCount;
    namedVariable->typeSpec.structure.elements = (MmsVariableSpecification**) GLOBAL_CALLOC(lcbCount,
            sizeof(MmsVariableSpecification*));

    int currentLcb = 0;

    while (currentLcb < lcbCount) {

        LogControl* logControl = LogControl_create(logicalNode, self);

        LogControlBlock* logControlBlock = getLCBForLogicalNodeWithIndex(self, logicalNode, currentLcb);

        logControl->name = StringUtils_createString(3, logicalNode->name, "$LG$", logControlBlock->name);
        logControl->domain = domain;

        namedVariable->typeSpec.structure.elements[currentLcb] =
                createLogControlBlock(self, logControlBlock, logControl);

        if (logControlBlock->logRef != NULL)
            logControl->logInstance = getLogInstanceByLogRef(self, logControlBlock->logRef);

        if (logControl->enabled)
            enableLogging(logControl);

        LogControl_updateLogEna(logControl);

        LinkedList_add(self->logControls, logControl);

        currentLcb++;
    }

    return namedVariable;
}

static void
LogControl_logAllDatasetEntries(LogControl* self, const char* iedName)
{
    if (self->dataSet == NULL)
        return;

    if (self->logInstance != NULL) {

        char dataRef[130];

        LogInstance* log = self->logInstance;

        uint64_t entryID = LogInstance_logEntryStart(log);

        DataSetEntry* dataSetEntry = self->dataSet->fcdas;

        while (dataSetEntry != NULL) {

            sprintf(dataRef, "%s%s/%s", iedName, dataSetEntry->logicalDeviceName, dataSetEntry->variableName);

            LogInstance_logEntryData(log, entryID, dataRef, dataSetEntry->value, TRG_OPT_INTEGRITY * 2);

            dataSetEntry = dataSetEntry->sibling;
        }

        LogInstance_logEntryFinished(log, entryID);

    }
}

void
Logging_processIntegrityLogs(MmsMapping* self, uint64_t currentTimeInMs)
{
    LinkedList logControlElem = LinkedList_getNext(self->logControls);

    while (logControlElem != NULL) {

        LogControl* logControl = (LogControl*) LinkedList_getData(logControlElem);

        if (logControl->enabled) {

            if (logControl->nextIntegrityScan != 0) {

                if (currentTimeInMs >= logControl->nextIntegrityScan) {

                    if (DEBUG_IED_SERVER)
                        printf("IED_SERVER: INTEGRITY SCAN for log %s\n", logControl->name);

                    LogControl_logAllDatasetEntries(logControl, self->mmsDevice->deviceName);

                    logControl->nextIntegrityScan += logControl->intgPd;
                }
            }
        }

        logControlElem = LinkedList_getNext(logControlElem);
    }
}

void
MmsMapping_setLogStorage(MmsMapping* self, const char* logRef, LogStorage logStorage)
{
    LogInstance* logInstance = getLogInstanceByLogRef(self, logRef);

    if (logInstance != NULL) {
        LogInstance_setLogStorage(logInstance, logStorage);

        char domainName[65];

        MmsMapping_getMmsDomainFromObjectReference(logRef, domainName);

        char domainNameWithIEDName[65];

        strcpy(domainNameWithIEDName, self->model->name);
        strcat(domainNameWithIEDName, domainName);

        MmsDomain* mmsDomain = MmsDevice_getDomain(self->mmsDevice, domainNameWithIEDName);

        if (mmsDomain == NULL) {
            if (DEBUG_IED_SERVER)
                printf("IED_SERVER: MmsMapping_setLogStorage: domain %s not found!\n", domainNameWithIEDName);

            return;
        }

        MmsJournal mmsJournal = NULL;

        const char* logName = strchr(logRef, '/');

        if (logName != NULL) {
            logName += 1;
            mmsJournal = MmsDomain_getJournal(mmsDomain, logName);
        }

        if (mmsJournal != NULL)
            mmsJournal->logStorage = logStorage;
        else
            if (DEBUG_IED_SERVER)
                printf("IED_SERVER: Failed to retrieve MMS journal for log!\n");
    }

    if (DEBUG_IED_SERVER)
        if (logInstance == NULL)
            printf("IED_SERVER: MmsMapping_setLogStorage no matching log for %s found!\n", logRef);
}

#endif /* (CONFIG_IEC61850_LOG_SERVICE == 1) */

