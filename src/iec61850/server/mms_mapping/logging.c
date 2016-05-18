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
#include "array_list.h"
#include "hal_thread.h"

#include "simple_allocator.h"
#include "mem_alloc_linked_list.h"

#include "mms_mapping_internal.h"
#include "mms_value_internal.h"


LogInstance*
LogInstance_create(LogicalNode* parentLN, const char* name)
{
    LogInstance* self = (LogInstance*) GLOBAL_MALLOC(sizeof(LogInstance));

    self->name = copyString(name);
    self->parentLN = parentLN;
    self->logStorage = NULL;

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

        printf("Log value - dataRef: %s flag: %i\n", dataRef, flag);

        uint64_t timestamp = Hal_getTimeInMs();

        uint64_t entryID = LogStorage_addEntry(logStorage, timestamp);

        int dataSize = MmsValue_encodeMmsData(value, NULL, 0, false);

        uint8_t* data = GLOBAL_MALLOC(dataSize);

        MmsValue_encodeMmsData(value, data, 0, true);

        LogStorage_addEntryData(logStorage, entryID, dataRef, data, dataSize, flag);

        self->newEntryId = entryID;
        self->newEntryTime = timestamp;

    }
    else
        printf("no log storage available!\n");
}

void
LogInstance_setLogStorage(LogInstance* self, LogStorage logStorage)
{
    self->logStorage = logStorage;

    LogStorage_getOldestAndNewestEntries(logStorage, &(self->newEntryId), &(self->newEntryTime),
            &(self->oldEntryId), &(self->oldEntryTime));

    printf("Attached storage to log: %s\n", self->name);
    printf("  oldEntryID: %llu oldEntryTm: %llu\n", self->oldEntryId, self->oldEntryTime);
    printf("  newEntryID: %llu newEntryTm: %llu\n", self->newEntryId, self->newEntryTime);

}

LogControl*
LogControl_create(LogicalNode* parentLN, MmsMapping* mmsMapping)
{
    LogControl* self = (LogControl*) GLOBAL_MALLOC(sizeof(LogControl));

    self->enabled = false;
    self->dataSet = NULL;
    self->triggerOps = 0;
    self->logicalNode = parentLN;
    self->mmsMapping = mmsMapping;
    self->dataSetRef = NULL;
    self->logInstance = NULL;

    return self;
}

void
LogControl_destroy(LogControl* self)
{
    if (self != NULL) {
        GLOBAL_FREEMEM(self);
    }
}

void
LogControl_setLog(LogControl* self, LogInstance* logInstance)
{
    self->logInstance = logInstance;
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

static void
updateLogStatusInLCB(LogControl* self)
{
    LogInstance* logInstance = self->logInstance;

    if (logInstance != NULL) {

        MmsValue_setBinaryTime(self->oldEntrTm, logInstance->oldEntryTime);
        MmsValue_setBinaryTime(self->newEntrTm, logInstance->newEntryTime);

        MmsValue_setOctetString(self->oldEntr, &(logInstance->oldEntryId), 8);
        MmsValue_setOctetString(self->newEntr, &(logInstance->newEntryId), 8);
    }
}


MmsValue*
LIBIEC61850_LOG_SVC_readAccessControlBlock(MmsMapping* self, MmsDomain* domain, char* variableIdOrig)
{
    MmsValue* value = NULL;

    printf("READ ACCESS LOG CB\n");

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

    dataSetReference = createString(5, domainName, "/", lnName, "$", lcb->dataSetName);

    return dataSetReference;
}

static MmsValue*
createTrgOps(LogControlBlock* reportControlBlock) {
    MmsValue* trgOps = MmsValue_newBitString(-6);

    uint8_t triggerOps = reportControlBlock->trgOps;

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

static bool
enableLogging(LogControl* logControl)
{
    printf("Enable LCB %s...\n", logControl->name);

    if (logControl->dataSetRef == NULL) {
        printf("  no data set specified!\n");
        return false;
    }

    DataSet* dataSet = IedModel_lookupDataSet(logControl->mmsMapping->model, logControl->dataSetRef);

    if (dataSet == NULL) {
        printf("   data set (%s) not found!\n", logControl->dataSetRef);
        return false;
    }
    else
        logControl->dataSet = dataSet;

    printf("  enabled\n");

    return true;
}

static MmsVariableSpecification*
createLogControlBlock(MmsMapping* self, LogControlBlock* logControlBlock,
        LogControl* logControl)
{
    MmsVariableSpecification* lcb = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    lcb->name = copyString(logControlBlock->name);
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

    namedVariable->name = copyString("LogEna");
    namedVariable->type = MMS_BOOLEAN;

    lcb->typeSpec.structure.elements[0] = namedVariable;
    mmsValue->value.structure.components[0] = MmsValue_newBoolean(logControlBlock->logEna);

    /* LogRef */
    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = copyString("LogRef");
    namedVariable->typeSpec.visibleString = -129;
    namedVariable->type = MMS_VISIBLE_STRING;
    lcb->typeSpec.structure.elements[1] = namedVariable;

    if (logControlBlock->logRef != NULL) {
        char logRef[130];

        int maxLogRefLength = 129 - strlen(self->model->name);

        strcpy(logRef, self->model->name);
        strncat(logRef, logControlBlock->logRef, maxLogRefLength);

        mmsValue->value.structure.components[1] = MmsValue_newVisibleString(logRef);
    }
    else {
        char* logRef = createString(4, logControl->domain->domainName, "/", logControlBlock->parent->name,
                "$GeneralLog");

        mmsValue->value.structure.components[1] = MmsValue_newVisibleString(logRef);

        GLOBAL_FREEMEM(logRef);
    }

    /* DatSet */
    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = copyString("DatSet");
    namedVariable->typeSpec.visibleString = -129;
    namedVariable->type = MMS_VISIBLE_STRING;
    lcb->typeSpec.structure.elements[2] = namedVariable;

    if (logControlBlock->dataSetName != NULL) {
        char* dataSetReference = createDataSetReferenceForDefaultDataSet(logControlBlock, logControl);

        printf("createLogControlBlock dataSetRef: %s\n", dataSetReference);

        logControl->dataSetRef = dataSetReference;

        mmsValue->value.structure.components[2] = MmsValue_newVisibleString(dataSetReference);
        //GLOBAL_FREEMEM(dataSetReference);
    }
    else
        mmsValue->value.structure.components[2] = MmsValue_newVisibleString("");

    /* OldEntrTm */
    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = copyString("OldEntrTm");
    namedVariable->type = MMS_BINARY_TIME;
    namedVariable->typeSpec.binaryTime = 6;
    lcb->typeSpec.structure.elements[3] = namedVariable;

    mmsValue->value.structure.components[3] = MmsValue_newBinaryTime(false);

    logControl->oldEntrTm = mmsValue->value.structure.components[3];

    /* NewEntrTm */
    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = copyString("NewEntrTm");
    namedVariable->type = MMS_BINARY_TIME;
    namedVariable->typeSpec.binaryTime = 6;
    lcb->typeSpec.structure.elements[4] = namedVariable;

    mmsValue->value.structure.components[4] = MmsValue_newBinaryTime(false);

    logControl->newEntrTm = mmsValue->value.structure.components[4];

    /* OldEntr */
    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = copyString("OldEntr");
    namedVariable->type = MMS_OCTET_STRING;
    namedVariable->typeSpec.octetString = 8;

    lcb->typeSpec.structure.elements[5] = namedVariable;

    mmsValue->value.structure.components[5] = MmsValue_newOctetString(8, 8);

    logControl->oldEntr = mmsValue->value.structure.components[5];

    /* NewEntr */
    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = copyString("NewEntr");
    namedVariable->type = MMS_OCTET_STRING;
    namedVariable->typeSpec.octetString = 8;

    lcb->typeSpec.structure.elements[6] = namedVariable;

    mmsValue->value.structure.components[6] = MmsValue_newOctetString(8, 8);

    logControl->newEntr = mmsValue->value.structure.components[6];

    /* TrgOps */
    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = copyString("TrgOps");
    namedVariable->type = MMS_BIT_STRING;
    namedVariable->typeSpec.bitString = -6;
    lcb->typeSpec.structure.elements[7] = namedVariable;
    mmsValue->value.structure.components[7] = createTrgOps(logControlBlock);

    /* IntgPd */
    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = copyString("IntgPd");
    namedVariable->type = MMS_UNSIGNED;
    namedVariable->typeSpec.unsignedInteger = 32;
    lcb->typeSpec.structure.elements[8] = namedVariable;
    mmsValue->value.structure.components[8] =
            MmsValue_newUnsignedFromUint32(logControlBlock->intPeriod);


    logControl->mmsType = lcb;
    logControl->mmsValue = mmsValue;
    logControl->logControlBlock = logControlBlock;
    logControl->triggerOps = logControlBlock->trgOps;

    logControl->enabled = logControlBlock->logEna;

    if (logControl->enabled) {
        enableLogging(logControl);
    }

    return lcb;
}

static LogInstance*
getLogInstanceByLogRef(MmsMapping* self, const char* logRef)
{
    char refStr[130];
    char* domainName;
    char* lnName;
    char* logName;

    strncpy(refStr, logRef, 129);

    printf("getLogInst... %s\n", refStr);

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

    printf("LOG: dn: %s ln: %s name: %s\n", domainName, lnName, logName);

    LinkedList instance = LinkedList_getNext(self->logInstances);

    while (instance != NULL) {

        LogInstance* logInstance = LinkedList_getData(instance);

        printf("logInstance: %s\n", logInstance->name);

        if (strcmp(logInstance->name, logName) == 0) {
            printf ("  lnName: %s\n", logInstance->parentLN->name);

            if (strcmp(lnName, logInstance->parentLN->name) == 0) {
                LogicalDevice* ld = (LogicalDevice*) logInstance->parentLN->parent;

                printf("  ldName: %s\n", ld->name);

                if (strcmp(ld->name, domainName) == 0)
                    return logInstance;
            }
        }

        instance = LinkedList_getNext(instance);
    }

    return NULL;
}

#if 0
static LogInstance*
getLogInstance(MmsMapping* self, LogicalNode* logicalNode, const char* logName)
{
    if (logName == NULL)
        return NULL;

    LinkedList logInstance = LinkedList_getNext(self->logInstances);

    while (logInstance != NULL) {
        LogInstance* instance = (LogInstance*) LinkedList_getData(logInstance);

        printf("LOG: %s (%s)\n", instance->name, logName);

        if ((strcmp(instance->name, logName) == 0) && (logicalNode == instance->parentLN))
            return instance;

        logInstance = LinkedList_getNext(logInstance);
    }

    return NULL;
}
#endif

MmsVariableSpecification*
Logging_createLCBs(MmsMapping* self, MmsDomain* domain, LogicalNode* logicalNode,
        int lcbCount)
{
    MmsVariableSpecification* namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1,
            sizeof(MmsVariableSpecification));
    namedVariable->name = copyString("LG");
    namedVariable->type = MMS_STRUCTURE;

    namedVariable->typeSpec.structure.elementCount = lcbCount;
    namedVariable->typeSpec.structure.elements = (MmsVariableSpecification**) GLOBAL_CALLOC(lcbCount,
            sizeof(MmsVariableSpecification*));

    int currentLcb = 0;

    while (currentLcb < lcbCount) {

        LogControl* logControl = LogControl_create(logicalNode, self);

        LogControlBlock* logControlBlock = getLCBForLogicalNodeWithIndex(self, logicalNode, currentLcb);

        logControl->name = createString(3, logicalNode->name, "$LG$", logControlBlock->name);
        logControl->domain = domain;

        namedVariable->typeSpec.structure.elements[currentLcb] =
                createLogControlBlock(self, logControlBlock, logControl);

        //getLogInstanceByLogRef(self, logControlBlock->logRef);
        //logControl->logInstance = getLogInstance(self, logicalNode, logControlBlock->logRef);

        if (logControlBlock->logRef != NULL)
            logControl->logInstance = getLogInstanceByLogRef(self, logControlBlock->logRef);

        LinkedList_add(self->logControls, logControl);

        currentLcb++;
    }

    return namedVariable;
}

void
MmsMapping_setLogStorage(MmsMapping* self, const char* logRef, LogStorage logStorage)
{
    LogInstance* logInstance = getLogInstanceByLogRef(self, logRef);

    if (logInstance != NULL)
        LogInstance_setLogStorage(logInstance, logStorage);

    //if (DEBUG_IED_SERVER)
        if (logInstance == NULL)
            printf("IED_SERVER: MmsMapping_setLogStorage no matching log for %s found!\n", logRef);
}
