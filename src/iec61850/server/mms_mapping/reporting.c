/*
 *  reporting.c
 *
 *  Copyright 2013 - 2016 Michael Zillgith
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
#include "mms_mapping.h"
#include "linked_list.h"
#include "array_list.h"
#include "stack_config.h"
#include "hal_thread.h"

#include "simple_allocator.h"
#include "mem_alloc_linked_list.h"

#include "mms_mapping_internal.h"
#include "mms_value_internal.h"
#include "conversions.h"
#include "reporting.h"
#include <string.h>

#ifndef DEBUG_IED_SERVER
#define DEBUG_IED_SERVER 0
#endif

#if (CONFIG_IEC61850_REPORT_SERVICE == 1)

#ifndef CONFIG_IEC61850_BRCB_WITH_RESVTMS
#define CONFIG_IEC61850_BRCB_WITH_RESVTMS 0
#endif


#ifndef CONFIG_IEC61850_EDITION_1
#define CONFIG_IEC61850_EDITION_1 0
#endif

#if (CONFIG_IEC61850_EDITION_1 == 1)
#define CONFIG_REPORTING_SUPPORTS_OWNER 0
#endif

#ifndef CONFIG_REPORTING_SUPPORTS_OWNER
#define CONFIG_REPORTING_SUPPORTS_OWNER 1
#endif


static ReportBuffer*
ReportBuffer_create(void)
{
    ReportBuffer* self = (ReportBuffer*) GLOBAL_MALLOC(sizeof(ReportBuffer));
    self->lastEnqueuedReport = NULL;
    self->oldestReport = NULL;
    self->nextToTransmit = NULL;
    self->memoryBlockSize = CONFIG_REPORTING_DEFAULT_REPORT_BUFFER_SIZE;
    self->memoryBlock = (uint8_t*) GLOBAL_MALLOC(self->memoryBlockSize);
    self->reportsCount = 0;
    self->isOverflow = true;

    return self;
}

static void
ReportBuffer_destroy(ReportBuffer* self)
{
    GLOBAL_FREEMEM(self->memoryBlock);
    GLOBAL_FREEMEM(self);
}

ReportControl*
ReportControl_create(bool buffered, LogicalNode* parentLN)
{
    ReportControl* self = (ReportControl*) GLOBAL_MALLOC(sizeof(ReportControl));
    self->name = NULL;
    self->domain = NULL;
    self->parentLN = parentLN;
    self->rcbValues = NULL;
    self->confRev = NULL;
    self->enabled = false;
    self->reserved = false;
    self->buffered = buffered;
    self->isBuffering = false;
    self->isResync = false;
    self->gi = false;
    self->inclusionField = NULL;
    self->dataSet = NULL;
    self->isDynamicDataSet = false;
    self->clientConnection = NULL;
    self->intgPd = 0;
    self->sqNum = 0;
    self->nextIntgReportTime = 0;
    self->inclusionFlags = NULL;
    self->triggered = false;
    self->timeOfEntry = NULL;
    self->reservationTimeout = 0;
    self->triggerOps = 0;

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    self->createNotificationsMutex = Semaphore_create(1);
#endif

    self->bufferedDataSetValues = NULL;
    self->valueReferences = NULL;
    self->lastEntryId = 0;

    if (buffered) {
        self->reportBuffer = ReportBuffer_create();
    }

    return self;
}

static void
ReportControl_lockNotify(ReportControl* self)
{
#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_wait(self->createNotificationsMutex);
#endif
}

static void
ReportControl_unlockNotify(ReportControl* self)
{
#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_post(self->createNotificationsMutex);
#endif
}


static void
purgeBuf(ReportControl* rc)
{
    if (DEBUG_IED_SERVER) printf("IED_SERVER: reporting.c: run purgeBuf\n");

    ReportBuffer* reportBuffer = rc->reportBuffer;

    reportBuffer->lastEnqueuedReport = NULL;
    reportBuffer->oldestReport = NULL;
    reportBuffer->nextToTransmit = NULL;
    reportBuffer->reportsCount = 0;
}


static void
deleteDataSetValuesShadowBuffer(ReportControl* self)
{
    if (self->bufferedDataSetValues != NULL) {
        assert(self->dataSet != NULL);

        int dataSetSize = DataSet_getSize(self->dataSet);

        int i;

        for (i = 0; i < dataSetSize; i++) {
            if (self->bufferedDataSetValues[i] != NULL)
                MmsValue_delete(self->bufferedDataSetValues[i]);
        }

        GLOBAL_FREEMEM(self->bufferedDataSetValues);

        if (self->valueReferences != NULL)
            GLOBAL_FREEMEM(self->valueReferences);

        self->bufferedDataSetValues = NULL;
    }
}

void
ReportControl_destroy(ReportControl* self)
{
    if (self->rcbValues != NULL )
        MmsValue_delete(self->rcbValues);

    if (self->inclusionFlags != NULL)
        GLOBAL_FREEMEM(self->inclusionFlags);

    if (self->inclusionField != NULL)
        MmsValue_delete(self->inclusionField);

    if (self->buffered == false)
        MmsValue_delete(self->timeOfEntry);

    deleteDataSetValuesShadowBuffer(self);

    if (self->isDynamicDataSet) {
        if (self->dataSet != NULL) {
            MmsMapping_freeDynamicallyCreatedDataSet(self->dataSet);
            self->isDynamicDataSet = false;
            self->dataSet = NULL;
        }
    }

    if (self->buffered)
        ReportBuffer_destroy(self->reportBuffer);

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_destroy(self->createNotificationsMutex);
#endif

    GLOBAL_FREEMEM(self->name);

    GLOBAL_FREEMEM(self);
}

MmsValue*
ReportControl_getRCBValue(ReportControl* rc, char* elementName)
{
    if (rc->buffered) {
        if (strcmp(elementName, "RptID") == 0)
            return MmsValue_getElement(rc->rcbValues, 0);
        else if (strcmp(elementName, "RptEna") == 0)
            return MmsValue_getElement(rc->rcbValues, 1);
        else if (strcmp(elementName, "DatSet") == 0)
            return MmsValue_getElement(rc->rcbValues, 2);
        else if (strcmp(elementName, "ConfRev") == 0)
            return MmsValue_getElement(rc->rcbValues, 3);
        else if (strcmp(elementName, "OptFlds") == 0)
            return MmsValue_getElement(rc->rcbValues, 4);
        else if (strcmp(elementName, "BufTm") == 0)
            return MmsValue_getElement(rc->rcbValues, 5);
        else if (strcmp(elementName, "SqNum") == 0)
            return MmsValue_getElement(rc->rcbValues, 6);
        else if (strcmp(elementName, "TrgOps") == 0)
            return MmsValue_getElement(rc->rcbValues, 7);
        else if (strcmp(elementName, "IntgPd") == 0)
            return MmsValue_getElement(rc->rcbValues, 8);
        else if (strcmp(elementName, "GI") == 0)
            return MmsValue_getElement(rc->rcbValues, 9);
        else if (strcmp(elementName, "PurgeBuf") == 0)
            return MmsValue_getElement(rc->rcbValues, 10);
        else if (strcmp(elementName, "EntryID") == 0)
            return MmsValue_getElement(rc->rcbValues, 11);
        else if (strcmp(elementName, "TimeofEntry") == 0)
            return MmsValue_getElement(rc->rcbValues, 12);
#if (CONFIG_IEC61850_BRCB_WITH_RESVTMS == 1)
        else if (strcmp(elementName, "ResvTms") == 0)
            return MmsValue_getElement(rc->rcbValues, 13);
        else if (strcmp(elementName, "Owner") == 0)
            return MmsValue_getElement(rc->rcbValues, 14);
#else
        else if (strcmp(elementName, "Owner") == 0)
            return MmsValue_getElement(rc->rcbValues, 13);
#endif
    } else {
        if (strcmp(elementName, "RptID") == 0)
            return MmsValue_getElement(rc->rcbValues, 0);
        else if (strcmp(elementName, "RptEna") == 0)
            return MmsValue_getElement(rc->rcbValues, 1);
        else if (strcmp(elementName, "Resv") == 0)
            return MmsValue_getElement(rc->rcbValues, 2);
        else if (strcmp(elementName, "DatSet") == 0)
            return MmsValue_getElement(rc->rcbValues, 3);
        else if (strcmp(elementName, "ConfRev") == 0)
            return MmsValue_getElement(rc->rcbValues, 4);
        else if (strcmp(elementName, "OptFlds") == 0)
            return MmsValue_getElement(rc->rcbValues, 5);
        else if (strcmp(elementName, "BufTm") == 0)
            return MmsValue_getElement(rc->rcbValues, 6);
        else if (strcmp(elementName, "SqNum") == 0)
            return MmsValue_getElement(rc->rcbValues, 7);
        else if (strcmp(elementName, "TrgOps") == 0)
            return MmsValue_getElement(rc->rcbValues, 8);
        else if (strcmp(elementName, "IntgPd") == 0)
            return MmsValue_getElement(rc->rcbValues, 9);
        else if (strcmp(elementName, "GI") == 0)
            return MmsValue_getElement(rc->rcbValues, 10);
        else if (strcmp(elementName, "Owner") == 0)
            return MmsValue_getElement(rc->rcbValues, 11);
    }

    return NULL ;
}

static void
updateTimeOfEntry(ReportControl* self, uint64_t currentTime)
{
    MmsValue* timeOfEntry = self->timeOfEntry;
    MmsValue_setBinaryTime(timeOfEntry, currentTime);
}

static void
sendReport(ReportControl* self, bool isIntegrity, bool isGI)
{
    updateTimeOfEntry(self, Hal_getTimeInMs());

    LinkedList reportElements = LinkedList_create();

    LinkedList deletableElements = LinkedList_create();

    MmsValue* rptId = ReportControl_getRCBValue(self, "RptID");
    MmsValue* optFlds = ReportControl_getRCBValue(self, "OptFlds");
    MmsValue* datSet = ReportControl_getRCBValue(self, "DatSet");

    LinkedList_add(reportElements, rptId);
    LinkedList_add(reportElements, optFlds);

    /* delete option fields for unsupported options */
    MmsValue_setBitStringBit(optFlds, 7, false); /* entryID */
    MmsValue_setBitStringBit(optFlds, 9, false); /* segmentation */

    MmsValue* sqNum = ReportControl_getRCBValue(self, "SqNum");

    if (MmsValue_getBitStringBit(optFlds, 1)) /* sequence number */
        LinkedList_add(reportElements, sqNum);

    if (MmsValue_getBitStringBit(optFlds, 2)) /* report time stamp */
        LinkedList_add(reportElements, self->timeOfEntry);

    if (MmsValue_getBitStringBit(optFlds, 4)) /* data set reference */
        LinkedList_add(reportElements, datSet);

    if (MmsValue_getBitStringBit(optFlds, 6)) { /* bufOvfl */
        MmsValue* bufOvfl = MmsValue_newBoolean(false);

        LinkedList_add(reportElements, bufOvfl);
        LinkedList_add(deletableElements, bufOvfl);
    }

    if (MmsValue_getBitStringBit(optFlds, 8))
        LinkedList_add(reportElements, self->confRev);

    if (isGI || isIntegrity)
        MmsValue_setAllBitStringBits(self->inclusionField);
    else
        MmsValue_deleteAllBitStringBits(self->inclusionField);

    LinkedList_add(reportElements, self->inclusionField);

    /* add data references if selected */
    if (MmsValue_getBitStringBit(optFlds, 5)) { /* data-reference */
        DataSetEntry* dataSetEntry = self->dataSet->fcdas;

        LogicalDevice* ld = (LogicalDevice*) self->parentLN->parent;

        IedModel* iedModel = (IedModel*) ld->parent;

        char* iedName = iedModel->name;

        int iedNameLength = strlen(iedName);

        int i = 0;

        for (i = 0; i < self->dataSet->elementCount; i++) {
            assert(dataSetEntry->value != NULL);

            bool addReferenceForEntry = false;

            if (isGI || isIntegrity)
               addReferenceForEntry = true;
            else
                if (self->inclusionFlags[i] != REPORT_CONTROL_NONE)
                    addReferenceForEntry = true;

            if (addReferenceForEntry) {

                char dataReference[130];
                int currentPos = 0;

                int j;

                for (j = 0; j < iedNameLength; j++) {
                    dataReference[currentPos++] = iedName[j];
                }

                int ldNameLength =  strlen(dataSetEntry->logicalDeviceName);
                for (j = 0; j <  ldNameLength; j++) {
                    dataReference[currentPos] = dataSetEntry->logicalDeviceName[j];
                    currentPos++;
                }

                dataReference[currentPos++] = '/';

                for (j = 0; j < (int) strlen(dataSetEntry->variableName); j++) {
                    dataReference[currentPos++] = dataSetEntry->variableName[j];
                }

                dataReference[currentPos] = 0;

                MmsValue* dataRef = MmsValue_newVisibleString(dataReference);

                LinkedList_add(reportElements, dataRef);
                LinkedList_add(deletableElements, dataRef);
            }

            dataSetEntry = dataSetEntry->sibling;

        }
    }

    /* add data set value elements */
    DataSetEntry* dataSetEntry = self->dataSet->fcdas;

    int i = 0;
    for (i = 0; i < self->dataSet->elementCount; i++) {
        assert(dataSetEntry->value != NULL);

        if (isGI || isIntegrity) {
            LinkedList_add(reportElements, dataSetEntry->value);
        }
        else {
            if (self->inclusionFlags[i] != REPORT_CONTROL_NONE) {
                assert(self->bufferedDataSetValues[i] != NULL);

                LinkedList_add(reportElements, self->bufferedDataSetValues[i]);
                MmsValue_setBitStringBit(self->inclusionField, i, true);
            }
        }

        dataSetEntry = dataSetEntry->sibling;
    }

    /* add reason code to report if requested */
    if (MmsValue_getBitStringBit(optFlds, 3)) {
        for (i = 0; i < self->dataSet->elementCount; i++) {

            if (isGI || isIntegrity) {
                MmsValue* reason = MmsValue_newBitString(6);

                if (isGI)
                    MmsValue_setBitStringBit(reason, 5, true);

                if (isIntegrity)
                    MmsValue_setBitStringBit(reason, 4, true);

                LinkedList_add(reportElements, reason);
                LinkedList_add(deletableElements, reason);
            }
            else if (self->inclusionFlags[i] != REPORT_CONTROL_NONE) {
                MmsValue* reason = MmsValue_newBitString(6);

                if (self->inclusionFlags[i] == REPORT_CONTROL_QUALITY_CHANGED)
                    MmsValue_setBitStringBit(reason, 2, true);
                else if (self->inclusionFlags[i] == REPORT_CONTROL_VALUE_CHANGED)
                    MmsValue_setBitStringBit(reason, 1, true);
                else if (self->inclusionFlags[i] == REPORT_CONTROL_VALUE_UPDATE)
                    MmsValue_setBitStringBit(reason, 3, true);

                LinkedList_add(reportElements, reason);
                LinkedList_add(deletableElements, reason);
            }
        }
    }

    /* clear inclusion flags */
    for (i = 0; i < self->dataSet->elementCount; i++)
        self->inclusionFlags[i] = REPORT_CONTROL_NONE;

    MmsServerConnection_sendInformationReportVMDSpecific(self->clientConnection, "RPT", reportElements, false);

    /* Increase sequence number */
    self->sqNum++;

    /* Unbuffered reporting --> sqNum is 8 bit only!!! */
    if (self->sqNum == 256)
        self->sqNum = 0;

    MmsValue_setUint16(sqNum, self->sqNum);

    LinkedList_destroyDeep(deletableElements, (LinkedListValueDeleteFunction) MmsValue_delete);
    LinkedList_destroyStatic(reportElements);
}

static void
createDataSetValuesShadowBuffer(ReportControl* rc)
{
    int dataSetSize = DataSet_getSize(rc->dataSet);

    MmsValue** dataSetValues = (MmsValue**) GLOBAL_CALLOC(dataSetSize, sizeof(MmsValue*));

    rc->bufferedDataSetValues = dataSetValues;

    rc->valueReferences = (MmsValue**) GLOBAL_MALLOC(dataSetSize * sizeof(MmsValue*));

    DataSetEntry* dataSetEntry = rc->dataSet->fcdas;

    int i;
    for (i = 0; i < dataSetSize; i++) {
        assert(dataSetEntry != NULL);

        rc->valueReferences[i] = dataSetEntry->value;

        dataSetEntry = dataSetEntry->sibling;
    }
}

static bool
updateReportDataset(MmsMapping* mapping, ReportControl* rc, MmsValue* newDatSet, MmsServerConnection connection)
{
    bool success = false;

    MmsValue* dataSetValue;

    if (newDatSet != NULL) {
        if (strcmp(MmsValue_toString(newDatSet), "") == 0) {
            success = true;
            dataSetValue = NULL;

            if (rc->buffered) {
                rc->isBuffering = false;
                purgeBuf(rc);
            }
        }
        else
            dataSetValue = newDatSet;
    }
    else
        dataSetValue = ReportControl_getRCBValue(rc, "DatSet");

    if (rc->isDynamicDataSet) {
        if (rc->dataSet != NULL) {
            deleteDataSetValuesShadowBuffer(rc);
            MmsMapping_freeDynamicallyCreatedDataSet(rc->dataSet);
            rc->isDynamicDataSet = false;
            rc->dataSet = NULL;
        }
    }

    if (dataSetValue != NULL) {
        const char* dataSetName = MmsValue_toString(dataSetValue);

        DataSet* dataSet = IedModel_lookupDataSet(mapping->model, dataSetName);

#if (MMS_DYNAMIC_DATA_SETS == 1)
        if (dataSet == NULL) {
            dataSet = MmsMapping_getDomainSpecificDataSet(mapping, dataSetName);

            if (dataSet == NULL) {

                /* check if association specific data set is requested */
                if (dataSetName[0] == '@') {

                    if (rc->buffered == false) { /* for buffered report non-permanent datasets are not allowed */
                        if (connection != NULL) {
                            MmsNamedVariableList mmsVariableList
                                = MmsServerConnection_getNamedVariableList(connection, dataSetName + 1);

                            if (mmsVariableList != NULL)
                                dataSet = MmsMapping_createDataSetByNamedVariableList(mapping, mmsVariableList);
                        }
                    }

                }

                /* check for VMD specific data set */
                else if (dataSetName[0] == '/') {
                    MmsNamedVariableList mmsVariableList = MmsDevice_getNamedVariableListWithName(mapping->mmsDevice, dataSetName + 1);

                    if (mmsVariableList != NULL)
                        dataSet = MmsMapping_createDataSetByNamedVariableList(mapping, mmsVariableList);
                }
            }

            if (dataSet == NULL)
                goto exit_function;

            rc->isDynamicDataSet = true;

        }
        else
            rc->isDynamicDataSet = false;
#else

        if (dataSet == NULL)
            goto exit_function;

#endif /* (MMS_DYNAMIC_DATA_SETS == 1) */

        deleteDataSetValuesShadowBuffer(rc);

        rc->dataSet = dataSet;

        createDataSetValuesShadowBuffer(rc);

        if (rc->inclusionField != NULL)
            MmsValue_delete(rc->inclusionField);

        rc->inclusionField = MmsValue_newBitString(dataSet->elementCount);

        if (rc->inclusionFlags != NULL)
            GLOBAL_FREEMEM(rc->inclusionFlags);

        rc->inclusionFlags = (ReportInclusionFlag*) GLOBAL_CALLOC(dataSet->elementCount, sizeof(ReportInclusionFlag));

        success = true;

        if (rc->buffered)
            rc->isBuffering = true;

        goto exit_function;
    }

exit_function:
    return success;
}

static char*
createDataSetReferenceForDefaultDataSet(ReportControlBlock* rcb, ReportControl* reportControl)
{
    char* dataSetReference;

    char* domainName = MmsDomain_getName(reportControl->domain);
    char* lnName = rcb->parent->name;

    dataSetReference = StringUtils_createString(5, domainName, "/", lnName, "$", rcb->dataSetName);

    return dataSetReference;
}


static MmsValue*
createOptFlds(ReportControlBlock* reportControlBlock)
{
    MmsValue* optFlds = MmsValue_newBitString(-10);
    uint8_t options = reportControlBlock->options;

    if (options & RPT_OPT_SEQ_NUM)
        MmsValue_setBitStringBit(optFlds, 1, true);
    if (options & RPT_OPT_TIME_STAMP)
        MmsValue_setBitStringBit(optFlds, 2, true);
    if (options & RPT_OPT_REASON_FOR_INCLUSION)
        MmsValue_setBitStringBit(optFlds, 3, true);
    if (options & RPT_OPT_DATA_SET)
        MmsValue_setBitStringBit(optFlds, 4, true);
    if (options & RPT_OPT_DATA_REFERENCE)
        MmsValue_setBitStringBit(optFlds, 5, true);
    if (options & RPT_OPT_BUFFER_OVERFLOW)
        MmsValue_setBitStringBit(optFlds, 6, true);
    if (options & RPT_OPT_ENTRY_ID)
        MmsValue_setBitStringBit(optFlds, 7, true);
    if (options & RPT_OPT_CONF_REV)
        MmsValue_setBitStringBit(optFlds, 8, true);

    return optFlds;
}

static MmsValue*
createTrgOps(ReportControlBlock* reportControlBlock) {
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
    if (triggerOps & TRG_OPT_GI)
        MmsValue_setBitStringBit(trgOps, 5, true);

    return trgOps;
}

static void
refreshTriggerOptions(ReportControl* rc)
{
    rc->triggerOps = 0;
    MmsValue* trgOps = ReportControl_getRCBValue(rc, "TrgOps");
    if (MmsValue_getBitStringBit(trgOps, 1))
        rc->triggerOps += TRG_OPT_DATA_CHANGED;

    if (MmsValue_getBitStringBit(trgOps, 2))
        rc->triggerOps += TRG_OPT_QUALITY_CHANGED;

    if (MmsValue_getBitStringBit(trgOps, 3))
        rc->triggerOps += TRG_OPT_DATA_UPDATE;

    if (MmsValue_getBitStringBit(trgOps, 4))
        rc->triggerOps += TRG_OPT_INTEGRITY;

    if (MmsValue_getBitStringBit(trgOps, 5))
        rc->triggerOps += TRG_OPT_GI;
}

static void
refreshIntegrityPeriod(ReportControl* rc)
{
    MmsValue* intgPd = ReportControl_getRCBValue(rc, "IntgPd");
    rc->intgPd = MmsValue_toUint32(intgPd);

    if (rc->buffered == false)
        rc->nextIntgReportTime = 0;
}

static void
refreshBufferTime(ReportControl* rc)
{
    MmsValue* bufTm = ReportControl_getRCBValue(rc, "BufTm");
    rc->bufTm = MmsValue_toUint32(bufTm);
}

static void
composeDefaultRptIdString(char* rptIdString, ReportControl* reportControl)
{
    int bufPos = 0;
    while (reportControl->domain->domainName[bufPos] != 0) {
        rptIdString[bufPos] = reportControl->domain->domainName[bufPos];
        bufPos++;
    }
    rptIdString[bufPos++] = '/';
    int i = 0;
    while (reportControl->name[i] != 0) {
        rptIdString[bufPos] = reportControl->name[i];
        bufPos++;
        i++;
    }
    rptIdString[bufPos] = 0;
}

static MmsValue*
createDefaultRptId(ReportControl* reportControl)
{
    char rptIdString[130]; /* maximum length 129 chars */

    composeDefaultRptIdString(rptIdString, reportControl);

    return MmsValue_newVisibleString(rptIdString);
}

static void
updateWithDefaultRptId(ReportControl* reportControl, MmsValue* rptId)
{
    char rptIdString[130]; /* maximum length 129 chars */

    composeDefaultRptIdString(rptIdString, reportControl);

    MmsValue_setVisibleString(rptId, rptIdString);
}

static MmsVariableSpecification*
createUnbufferedReportControlBlock(ReportControlBlock* reportControlBlock,
        ReportControl* reportControl)
{
    MmsVariableSpecification* rcb = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    rcb->name = StringUtils_copyString(reportControlBlock->name);
    rcb->type = MMS_STRUCTURE;

    MmsValue* mmsValue = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));
    mmsValue->deleteValue = false;
    mmsValue->type = MMS_STRUCTURE;

#if (CONFIG_REPORTING_SUPPORTS_OWNER == 1)
    int structSize = 12;
#else
    int structSize = 11;
#endif

    mmsValue->value.structure.size = structSize;
    mmsValue->value.structure.components = (MmsValue**) GLOBAL_CALLOC(structSize, sizeof(MmsValue*));

    rcb->typeSpec.structure.elementCount = structSize;

    rcb->typeSpec.structure.elements = (MmsVariableSpecification**) GLOBAL_CALLOC(structSize,
            sizeof(MmsVariableSpecification*));

    MmsVariableSpecification* namedVariable = 
			(MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("RptID");
    namedVariable->typeSpec.visibleString = -129;
    namedVariable->type = MMS_VISIBLE_STRING;
    rcb->typeSpec.structure.elements[0] = namedVariable;
    if ((reportControlBlock->rptId != NULL) && (strlen(reportControlBlock->rptId) > 0))
        mmsValue->value.structure.components[0] = MmsValue_newVisibleString(
                reportControlBlock->rptId);
    else
        mmsValue->value.structure.components[0] = createDefaultRptId(reportControl);

    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("RptEna");
    namedVariable->type = MMS_BOOLEAN;
    rcb->typeSpec.structure.elements[1] = namedVariable;
    mmsValue->value.structure.components[1] = MmsValue_newBoolean(false);

    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("Resv");
    namedVariable->type = MMS_BOOLEAN;
    rcb->typeSpec.structure.elements[2] = namedVariable;
    mmsValue->value.structure.components[2] = MmsValue_newBoolean(false);

    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("DatSet");
    namedVariable->typeSpec.visibleString = -129;
    namedVariable->type = MMS_VISIBLE_STRING;
    rcb->typeSpec.structure.elements[3] = namedVariable;

    if (reportControlBlock->dataSetName != NULL) {
    	char* dataSetReference = createDataSetReferenceForDefaultDataSet(reportControlBlock,
    	            reportControl);
    	mmsValue->value.structure.components[3] = MmsValue_newVisibleString(dataSetReference);
    	GLOBAL_FREEMEM(dataSetReference);
    }
    else
    	mmsValue->value.structure.components[3] = MmsValue_newVisibleString("");

    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("ConfRev");
    namedVariable->type = MMS_UNSIGNED;
    namedVariable->typeSpec.unsignedInteger = 32;
    rcb->typeSpec.structure.elements[4] = namedVariable;
    mmsValue->value.structure.components[4] =
            MmsValue_newUnsignedFromUint32(reportControlBlock->confRef);

    reportControl->confRev = mmsValue->value.structure.components[4];

    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("OptFlds");
    namedVariable->type = MMS_BIT_STRING;
    namedVariable->typeSpec.bitString = -10;
    rcb->typeSpec.structure.elements[5] = namedVariable;
    mmsValue->value.structure.components[5] = createOptFlds(reportControlBlock);

    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("BufTm");
    namedVariable->type = MMS_UNSIGNED;
    namedVariable->typeSpec.unsignedInteger = 32;
    rcb->typeSpec.structure.elements[6] = namedVariable;
    mmsValue->value.structure.components[6] =
            MmsValue_newUnsignedFromUint32(reportControlBlock->bufferTime);

    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("SqNum");
    namedVariable->type = MMS_UNSIGNED;
    namedVariable->typeSpec.unsignedInteger = 8;
    rcb->typeSpec.structure.elements[7] = namedVariable;
    mmsValue->value.structure.components[7] = MmsValue_newUnsigned(8);

    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("TrgOps");
    namedVariable->type = MMS_BIT_STRING;
    namedVariable->typeSpec.bitString = -6;
    rcb->typeSpec.structure.elements[8] = namedVariable;
    mmsValue->value.structure.components[8] = createTrgOps(reportControlBlock);

    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("IntgPd");
    namedVariable->type = MMS_UNSIGNED;
    namedVariable->typeSpec.unsignedInteger = 32;
    rcb->typeSpec.structure.elements[9] = namedVariable;
    mmsValue->value.structure.components[9] =
            MmsValue_newUnsignedFromUint32(reportControlBlock->intPeriod);

    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("GI");
    namedVariable->type = MMS_BOOLEAN;
    rcb->typeSpec.structure.elements[10] = namedVariable;
    mmsValue->value.structure.components[10] = MmsValue_newBoolean(false);

#if (CONFIG_REPORTING_SUPPORTS_OWNER == 1)
    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("Owner");
    namedVariable->type = MMS_OCTET_STRING;
    namedVariable->typeSpec.octetString = -64;
    rcb->typeSpec.structure.elements[11] = namedVariable;
    mmsValue->value.structure.components[11] = MmsValue_newOctetString(0, 128);
#endif /* (CONFIG_REPORTING_SUPPORTS_OWNER == 1) */

    reportControl->rcbValues = mmsValue;

    reportControl->timeOfEntry = MmsValue_newBinaryTime(false);

    refreshBufferTime(reportControl);
    refreshIntegrityPeriod(reportControl);
    refreshTriggerOptions(reportControl);

    return rcb;
}

static MmsVariableSpecification*
createBufferedReportControlBlock(ReportControlBlock* reportControlBlock,
        ReportControl* reportControl, MmsMapping* mmsMapping)
{
    MmsVariableSpecification* rcb = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    rcb->name = StringUtils_copyString(reportControlBlock->name);
    rcb->type = MMS_STRUCTURE;

    int brcbElementCount = 13;

#if (CONFIG_IEC61850_BRCB_WITH_RESVTMS == 1)
    brcbElementCount++;
#endif

#if (CONFIG_REPORTING_SUPPORTS_OWNER == 1)
    brcbElementCount++;
#endif

    MmsValue* mmsValue = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));
    mmsValue->deleteValue = false;
    mmsValue->type = MMS_STRUCTURE;
    mmsValue->value.structure.size = brcbElementCount;
    mmsValue->value.structure.components = (MmsValue**) GLOBAL_CALLOC(brcbElementCount, sizeof(MmsValue*));

    rcb->typeSpec.structure.elementCount = brcbElementCount;
    rcb->typeSpec.structure.elements = (MmsVariableSpecification**) GLOBAL_CALLOC(brcbElementCount,
            sizeof(MmsVariableSpecification*));

    MmsVariableSpecification* namedVariable = 
			(MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("RptID");
    namedVariable->typeSpec.visibleString = -129;
    namedVariable->type = MMS_VISIBLE_STRING;
    rcb->typeSpec.structure.elements[0] = namedVariable;

    if ((reportControlBlock->rptId != NULL) && (strlen(reportControlBlock->rptId) > 0))
        mmsValue->value.structure.components[0] = MmsValue_newVisibleString(
                reportControlBlock->rptId);
    else
        mmsValue->value.structure.components[0] = createDefaultRptId(reportControl);

    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("RptEna");
    namedVariable->type = MMS_BOOLEAN;
    rcb->typeSpec.structure.elements[1] = namedVariable;
    mmsValue->value.structure.components[1] = MmsValue_newBoolean(false);

    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("DatSet");
    namedVariable->typeSpec.visibleString = -129;
    namedVariable->type = MMS_VISIBLE_STRING;
    rcb->typeSpec.structure.elements[2] = namedVariable;

    if (reportControlBlock->dataSetName != NULL) {
    	char* dataSetReference = createDataSetReferenceForDefaultDataSet(reportControlBlock,
    	            reportControl);

    	mmsValue->value.structure.components[2] = MmsValue_newVisibleString(dataSetReference);
    	GLOBAL_FREEMEM(dataSetReference);
    }
    else
    	mmsValue->value.structure.components[2] = MmsValue_newVisibleString("");

    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("ConfRev");
    namedVariable->type = MMS_UNSIGNED;
    namedVariable->typeSpec.unsignedInteger = 32;
    rcb->typeSpec.structure.elements[3] = namedVariable;
    mmsValue->value.structure.components[3] =
            MmsValue_newUnsignedFromUint32(reportControlBlock->confRef);

    reportControl->confRev = mmsValue->value.structure.components[3];

    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("OptFlds");
    namedVariable->type = MMS_BIT_STRING;
    namedVariable->typeSpec.bitString = -10;
    rcb->typeSpec.structure.elements[4] = namedVariable;
    mmsValue->value.structure.components[4] = createOptFlds(reportControlBlock);

    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("BufTm");
    namedVariable->type = MMS_UNSIGNED;
    namedVariable->typeSpec.unsignedInteger = 32;
    rcb->typeSpec.structure.elements[5] = namedVariable;
    mmsValue->value.structure.components[5] =
            MmsValue_newUnsignedFromUint32(reportControlBlock->bufferTime);

    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("SqNum");
    namedVariable->type = MMS_UNSIGNED;
    namedVariable->typeSpec.unsignedInteger = 16;
    rcb->typeSpec.structure.elements[6] = namedVariable;
    mmsValue->value.structure.components[6] = MmsValue_newUnsigned(16);

    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("TrgOps");
    namedVariable->type = MMS_BIT_STRING;
    namedVariable->typeSpec.bitString = -6;
    rcb->typeSpec.structure.elements[7] = namedVariable;
    mmsValue->value.structure.components[7] = createTrgOps(reportControlBlock);

    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("IntgPd");
    namedVariable->type = MMS_UNSIGNED;
    namedVariable->typeSpec.unsignedInteger = 32;
    rcb->typeSpec.structure.elements[8] = namedVariable;
    mmsValue->value.structure.components[8] =
            MmsValue_newUnsignedFromUint32(reportControlBlock->intPeriod);

    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("GI");
    namedVariable->type = MMS_BOOLEAN;
    rcb->typeSpec.structure.elements[9] = namedVariable;
    mmsValue->value.structure.components[9] = MmsValue_newBoolean(false);

    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("PurgeBuf");
    namedVariable->type = MMS_BOOLEAN;
    rcb->typeSpec.structure.elements[10] = namedVariable;
    mmsValue->value.structure.components[10] = MmsValue_newBoolean(false);

    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("EntryID");
    namedVariable->type = MMS_OCTET_STRING;
    namedVariable->typeSpec.octetString = 8;
    rcb->typeSpec.structure.elements[11] = namedVariable;
    mmsValue->value.structure.components[11] = MmsValue_newOctetString(8, 8);

    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("TimeofEntry");
    namedVariable->type = MMS_BINARY_TIME;
    namedVariable->typeSpec.binaryTime = 6;
    rcb->typeSpec.structure.elements[12] = namedVariable;
    mmsValue->value.structure.components[12] = MmsValue_newBinaryTime(false);

    reportControl->timeOfEntry = mmsValue->value.structure.components[12];

#if ((CONFIG_IEC61850_BRCB_WITH_RESVTMS == 1) || (CONFIG_REPORTING_SUPPORTS_OWNER == 1))
    int currentIndex = 13;
#endif

#if (CONFIG_IEC61850_BRCB_WITH_RESVTMS == 1)
    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("ResvTms");
    namedVariable->type = MMS_INTEGER;
    namedVariable->typeSpec.integer = 16;
    rcb->typeSpec.structure.elements[currentIndex] = namedVariable;
    mmsValue->value.structure.components[currentIndex] = MmsValue_newInteger(16);
    currentIndex++;
#endif /* (CONFIG_IEC61850_BRCB_WITH_RESVTMS == 1) */

#if (CONFIG_REPORTING_SUPPORTS_OWNER == 1)
    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("Owner");
    namedVariable->type = MMS_OCTET_STRING;
    namedVariable->typeSpec.octetString = -64;
    rcb->typeSpec.structure.elements[currentIndex] = namedVariable;
    mmsValue->value.structure.components[currentIndex] = MmsValue_newOctetString(0, 128); /* size 4 is enough to store client IPv4 address */
#endif /* (CONFIG_REPORTING_SUPPORTS_OWNER == 1) */

    reportControl->rcbValues = mmsValue;

    /* get initial values from BRCB */
    refreshBufferTime(reportControl);
    refreshIntegrityPeriod(reportControl);
    refreshTriggerOptions(reportControl);

    return rcb;
} /* createBufferedReportControlBlock() */

static ReportControlBlock*
getRCBForLogicalNodeWithIndex(MmsMapping* self, LogicalNode* logicalNode,
        int index, bool buffered)
{
    int rcbCount = 0;

    ReportControlBlock* nextRcb = self->model->rcbs;

    while (nextRcb != NULL ) {
        if (nextRcb->parent == logicalNode) {

            if (nextRcb->buffered == buffered) {
                if (rcbCount == index)
                    return nextRcb;

                rcbCount++;
            }

        }

        nextRcb = nextRcb->sibling;
    }

    return NULL ;
}

MmsVariableSpecification*
Reporting_createMmsBufferedRCBs(MmsMapping* self, MmsDomain* domain,
        LogicalNode* logicalNode, int reportsCount)
{
    MmsVariableSpecification* namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1,
            sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("BR");
    namedVariable->type = MMS_STRUCTURE;

    namedVariable->typeSpec.structure.elementCount = reportsCount;
    namedVariable->typeSpec.structure.elements = (MmsVariableSpecification**) GLOBAL_CALLOC(reportsCount,
            sizeof(MmsVariableSpecification*));

    int currentReport = 0;

    while (currentReport < reportsCount) {
        ReportControl* rc = ReportControl_create(true, logicalNode);

        rc->domain = domain;

        ReportControlBlock* reportControlBlock = getRCBForLogicalNodeWithIndex(
                self, logicalNode, currentReport, true);

        rc->name = StringUtils_createString(3, logicalNode->name, "$BR$",
                reportControlBlock->name);

        namedVariable->typeSpec.structure.elements[currentReport] =
                createBufferedReportControlBlock(reportControlBlock, rc, self);

        LinkedList_add(self->reportControls, rc);

        currentReport++;
    }

    return namedVariable;
}

MmsVariableSpecification*
Reporting_createMmsUnbufferedRCBs(MmsMapping* self, MmsDomain* domain,
        LogicalNode* logicalNode, int reportsCount)
{
    MmsVariableSpecification* namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1,
            sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("RP");
    namedVariable->type = MMS_STRUCTURE;

    namedVariable->typeSpec.structure.elementCount = reportsCount;
    namedVariable->typeSpec.structure.elements = (MmsVariableSpecification**) GLOBAL_CALLOC(reportsCount,
            sizeof(MmsVariableSpecification*));

    int currentReport = 0;

    while (currentReport < reportsCount) {
        ReportControl* rc = ReportControl_create(false, logicalNode);

        rc->domain = domain;

        ReportControlBlock* reportControlBlock = getRCBForLogicalNodeWithIndex(
                self, logicalNode, currentReport, false);

        rc->name = StringUtils_createString(3, logicalNode->name, "$RP$",
                reportControlBlock->name);

        namedVariable->typeSpec.structure.elements[currentReport] =
                createUnbufferedReportControlBlock(reportControlBlock, rc);

        LinkedList_add(self->reportControls, rc);

        currentReport++;
    }

    return namedVariable;
}

static void
updateOwner(ReportControl* rc, MmsServerConnection connection)
{
    rc->clientConnection = connection;

#if (CONFIG_REPORTING_SUPPORTS_OWNER == 1)
    MmsValue* owner = ReportControl_getRCBValue(rc, "Owner");

    if (owner != NULL) {

        if (connection != NULL) {
            char* clientAddressString = MmsServerConnection_getClientAddress(connection);

            if (DEBUG_IED_SERVER) printf("IED_SERVER: reporting.c: set owner to %s\n", clientAddressString);

            if (strchr(clientAddressString, '.') != NULL) {
                if (DEBUG_IED_SERVER)
                    printf("IED_SERVER: reporting.c:   client address is IPv4 address\n");

                uint8_t ipV4Addr[4];

                int addrElementCount = 0;

                char* separator = clientAddressString;

                while (separator != NULL && addrElementCount < 4) {
                    int intVal = atoi(separator);

                    ipV4Addr[addrElementCount] = intVal;

                    separator = strchr(separator, '.');

                    if (separator != NULL)
                        separator++; // skip '.' character

                    addrElementCount ++;
                }

                if (addrElementCount == 4)
                    MmsValue_setOctetString(owner, ipV4Addr, 4);
                else
                    MmsValue_setOctetString(owner, ipV4Addr, 0);

            }
            else {
                uint8_t ipV6Addr[16];
                MmsValue_setOctetString(owner, ipV6Addr, 0);
                if (DEBUG_IED_SERVER) printf("IED_SERVER: reporting.c:   client address is IPv6 address or unknown\n");
            }
        }
        else {
            uint8_t emptyAddr[1];
            MmsValue_setOctetString(owner, emptyAddr, 0);
        }
    }

#endif /* CONFIG_REPORTING_SUPPORTS_OWNER == 1*/
}


static bool
checkForZeroEntryID(MmsValue* value)
{
    uint8_t* buffer = MmsValue_getOctetStringBuffer(value);

    int i = 0;

    while (i < 8) {
        if (buffer[i] != 0)
            return false;

        i++;
    }

    return true;
}

static bool
checkReportBufferForEntryID(ReportControl* rc, MmsValue* value)
{
    bool retVal = false;

    ReportBufferEntry* entry = rc->reportBuffer->oldestReport;

    while (entry != NULL) {
        if (memcmp(entry->entryId, value->value.octetString.buf, 8) == 0) {
            ReportBufferEntry* nextEntryForResync = entry->next;

            rc->reportBuffer->nextToTransmit = nextEntryForResync;
            rc->isResync = true;

            retVal = true;
            break;
        }

        entry = entry->next;
    }

    return retVal;
}

static void
increaseConfRev(ReportControl* self)
{
    uint32_t confRev = MmsValue_toUint32(self->confRev);

    confRev++;

    if (confRev == 0)
        confRev = 1;

    MmsValue_setUint32(self->confRev, confRev);
}

MmsDataAccessError
Reporting_RCBWriteAccessHandler(MmsMapping* self, ReportControl* rc, char* elementName, MmsValue* value,
        MmsServerConnection connection)
{
    MmsDataAccessError retVal = DATA_ACCESS_ERROR_SUCCESS;

    ReportControl_lockNotify(rc);

    if (strcmp(elementName, "RptEna") == 0) {

        if (value->value.boolean == true) {

            if (rc->enabled == true) {
                retVal = DATA_ACCESS_ERROR_TEMPORARILY_UNAVAILABLE;
                goto exit_function;
            }

            if (DEBUG_IED_SERVER)
                printf("IED_SERVER: Activate report for client %s\n",
                        MmsServerConnection_getClientAddress(connection));

            if (updateReportDataset(self, rc, NULL, connection)) {

                updateOwner(rc, connection);

                MmsValue* rptEna = ReportControl_getRCBValue(rc, "RptEna");

                MmsValue_update(rptEna, value);

                if (rc->buffered) {

                    if (rc->isResync == false) {
                        rc->reportBuffer->nextToTransmit = rc->reportBuffer->oldestReport;
                        rc->reportBuffer->isOverflow = true;
                    }

                    rc->isResync = false;
                }

                rc->enabled = true;
                rc->gi = false;

                refreshBufferTime(rc);
                refreshTriggerOptions(rc);
                refreshIntegrityPeriod(rc);

                rc->sqNum = 0;

                MmsValue* sqNum = ReportControl_getRCBValue(rc, "SqNum");

                MmsValue_setUint32(sqNum, 0U);

                retVal = DATA_ACCESS_ERROR_SUCCESS;
                goto exit_function;
            }
            else {
                retVal = DATA_ACCESS_ERROR_TEMPORARILY_UNAVAILABLE;
                goto exit_function;
            }
        }
        else {
            if (rc->enabled == false)
                goto exit_function;

            if (((rc->enabled) || (rc->reserved)) && (rc->clientConnection != connection)) {
                retVal = DATA_ACCESS_ERROR_TEMPORARILY_UNAVAILABLE;
                goto exit_function;
            }

            if (DEBUG_IED_SERVER)
                printf("IED_SERVER: Deactivate report for client %s\n",
                        MmsServerConnection_getClientAddress(connection));

            if (rc->buffered) {
                rc->reportBuffer->isOverflow = true;
                rc->isResync = false;
            }
            else {
                GLOBAL_FREEMEM(rc->inclusionFlags);
                rc->inclusionFlags = NULL;

                MmsValue* resv = ReportControl_getRCBValue(rc, "Resv");
                MmsValue_setBoolean(resv, false);

                rc->triggered = false;

                rc->reserved = false;
            }

            updateOwner(rc, NULL);

            rc->enabled = false;
        }

    }

    if (strcmp(elementName, "GI") == 0) {
        if ((rc->enabled) && (rc->clientConnection == connection)) {

            if (MmsValue_getBoolean(value)) {
                if (rc->triggerOps & TRG_OPT_GI)
                    rc->gi = true;
            }

            retVal = DATA_ACCESS_ERROR_SUCCESS;
            goto exit_function;
        }
        else {
            retVal = DATA_ACCESS_ERROR_TEMPORARILY_UNAVAILABLE;
            goto exit_function;
        }
    }

    if (rc->enabled == false) {

        if ((rc->reserved) && (rc->clientConnection != connection)) {
            retVal = DATA_ACCESS_ERROR_TEMPORARILY_UNAVAILABLE;
            goto exit_function;
        }

        if (strcmp(elementName, "Resv") == 0) {
            rc->reserved = value->value.boolean;

            if (rc->reserved == true)
                rc->clientConnection = connection;
        }
        else if (strcmp(elementName, "PurgeBuf") == 0) {
            if (MmsValue_getType(value) == MMS_BOOLEAN) {

                if (MmsValue_getBoolean(value) == true) {
                    purgeBuf(rc);
                    retVal = DATA_ACCESS_ERROR_SUCCESS;
                    goto exit_function;
                }
            }

        }
        else if (strcmp(elementName, "DatSet") == 0) {
            MmsValue* datSet = ReportControl_getRCBValue(rc, "DatSet");

            if (!MmsValue_equals(datSet, value)) {

                if (updateReportDataset(self, rc, value, connection)) {

                    if (rc->buffered)
                        purgeBuf(rc);

                    MmsValue_update(datSet, value);

                    increaseConfRev(rc);
                }
                else {
                    retVal = DATA_ACCESS_ERROR_OBJECT_VALUE_INVALID;
                    goto exit_function;
                }
            }

            retVal = DATA_ACCESS_ERROR_SUCCESS;
            goto exit_function;
        }
        else if (strcmp(elementName, "IntgPd") == 0) {
            MmsValue* intgPd = ReportControl_getRCBValue(rc, elementName);

            if (!MmsValue_equals(intgPd, value)) {
                MmsValue_update(intgPd, value);

                refreshIntegrityPeriod(rc);

                if (rc->buffered) {
                    rc->nextIntgReportTime = 0;
                    purgeBuf(rc);
                }
            }

            goto exit_function;
        }
        else if (strcmp(elementName, "TrgOps") == 0) {
            MmsValue* trgOps = ReportControl_getRCBValue(rc, elementName);

            if (!MmsValue_equals(trgOps, value)) {
                MmsValue_update(trgOps, value);

                if (rc->buffered)
                    purgeBuf(rc);

                refreshTriggerOptions(rc);
            }

            goto exit_function;
        }
        else if (strcmp(elementName, "EntryID") == 0) {

            if (MmsValue_getOctetStringSize(value) != 8) {
                retVal = DATA_ACCESS_ERROR_OBJECT_VALUE_INVALID;
                goto exit_function;
            }

            if (checkForZeroEntryID(value) == false) {

                if (!checkReportBufferForEntryID(rc, value)) {
                    rc->reportBuffer->isOverflow = true;
                    retVal = DATA_ACCESS_ERROR_OBJECT_VALUE_INVALID;
                    goto exit_function;
                }

                rc->reportBuffer->isOverflow = false;
            }
            else {
                rc->reportBuffer->nextToTransmit = rc->reportBuffer->oldestReport;
                rc->reportBuffer->isOverflow = true;
                rc->isResync = false;
            }

            MmsValue* entryID = ReportControl_getRCBValue(rc, elementName);
            MmsValue_update(entryID, value);

            goto exit_function;
        }

        else if (strcmp(elementName, "BufTm") == 0) {
            MmsValue* bufTm = ReportControl_getRCBValue(rc, elementName);

            if (!MmsValue_equals(bufTm, value)) {
                MmsValue_update(bufTm, value);

                if (rc->buffered)
                    purgeBuf(rc);

                refreshBufferTime(rc);
            }

            goto exit_function;
        }
        else if (strcmp(elementName, "RptID") == 0) {
            MmsValue* rptId = ReportControl_getRCBValue(rc, elementName);

            if (rc->buffered)
                purgeBuf(rc);

            if (strlen(MmsValue_toString(value)) == 0)
                updateWithDefaultRptId(rc, rptId);
            else
                MmsValue_update(rptId, value);

            goto exit_function;
        }
        else if (strcmp(elementName, "ConfRev") == 0) {
            retVal = DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED;
            goto exit_function;
        }
        else if (strcmp(elementName, "SqNum") == 0) {
            retVal = DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED;
            goto exit_function;
        }
        else if (strcmp(elementName, "Owner") == 0) {
            retVal = DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED;
            goto exit_function;
        }
        else if (strcmp(elementName, "TimeofEntry") == 0) {
            retVal = DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED;
            goto exit_function;
        }

        MmsValue* rcbValue = ReportControl_getRCBValue(rc, elementName);

        if (rcbValue != NULL)
            MmsValue_update(rcbValue, value);
        else {
            retVal = DATA_ACCESS_ERROR_OBJECT_VALUE_INVALID;
            goto exit_function;
        }

    }
    else
        retVal = DATA_ACCESS_ERROR_TEMPORARILY_UNAVAILABLE;

exit_function:
    ReportControl_unlockNotify(rc);

    return retVal;
}

void
Reporting_deactivateReportsForConnection(MmsMapping* self, MmsServerConnection connection)
{
    LinkedList reportControl = self->reportControls;

    while ((reportControl = LinkedList_getNext(reportControl)) != NULL) {
        ReportControl* rc = (ReportControl*) reportControl->data;

        if (rc->clientConnection == connection) {

            rc->enabled = false;
            rc->clientConnection = NULL;

            MmsValue* rptEna = ReportControl_getRCBValue(rc, "RptEna");
            MmsValue_setBoolean(rptEna, false);

            if (rc->buffered == false) {

                if (rc->inclusionField != NULL) {
                    MmsValue_delete(rc->inclusionField);
                    rc->inclusionField = NULL;
                }

                MmsValue* resv = ReportControl_getRCBValue(rc, "Resv");
                MmsValue_setBoolean(resv, false);

                rc->reserved = false;
            }

            updateOwner(rc, NULL);
        }
    }
}

#if (DEBUG_IED_SERVER == 1)
static void
printEnqueuedReports(ReportControl* reportControl)
{
    ReportBuffer* rb = reportControl->reportBuffer;

#if 0
    printf("IED_SERVER: --- Enqueued reports ---\n");

    if (rb->oldestReport == NULL) {
        printf("IED_SERVER:   -- no reports --\n");
    }
    else {
        ReportBufferEntry* entry = rb->oldestReport;

        while (entry != NULL) {
            printf("IED_SERVER: ");

            int i = 0;
            printf("  ");
            for (i = 0; i < 8; i++) {
                printf("%02x ", entry->entryId[i]);
            }
            printf(" at [%p] next [%p] (len=%i pos=%i)", entry, entry->next,
                    entry->entryLength, (int) ((uint8_t*) entry - rb->memoryBlock));

            if (entry == rb->lastEnqueuedReport)
                printf("   <-- lastEnqueued");

            if (entry == rb->nextToTransmit)
                printf("   <-- nexttoTransmit");

            printf("\n");

            entry = entry->next;
        }
    }
#endif
    printf("IED_SERVER:   BRCB %s reports: %i\n", reportControl->name, rb->reportsCount);
    printf("IED_SERVER: -------------------------\n");
}

static void
printReportId(ReportBufferEntry* report)
{
    int i = 0;
    for (i = 0; i < 8; i++) {
        printf("%02x ", report->entryId[i]);
    }
}
#endif


static void
removeAllGIReportsFromReportBuffer(ReportBuffer* reportBuffer)
{
    ReportBufferEntry* currentReport = reportBuffer->oldestReport;
    ReportBufferEntry* lastReport = NULL;

    while (currentReport != NULL) {
        if (currentReport->flags & 2) {

            if (currentReport == reportBuffer->oldestReport) {
                reportBuffer->oldestReport = currentReport->next;
            }
            else {
                lastReport->next = currentReport->next;

            }

#if (DEBUG_IED_SERVER == 1)
                printf("IED_SERVER:   REMOVE old GI report with ID ");
                printReportId(currentReport);
                printf("\n");
#endif

            if (reportBuffer->nextToTransmit == currentReport)
                reportBuffer->nextToTransmit = currentReport->next;

            currentReport = currentReport->next;
        }
        else {
            lastReport = currentReport;
            currentReport = currentReport->next;
        }
    }

    if (reportBuffer->oldestReport == NULL)
        reportBuffer->lastEnqueuedReport = NULL;
}

static void
enqueueReport(ReportControl* reportControl, bool isIntegrity, bool isGI, uint64_t timeOfEntry)
{
    if (DEBUG_IED_SERVER)
        printf("IED_SERVER: enqueueReport: RCB name: %s (SQN:%u) enabled:%i buffered:%i buffering:%i intg:%i GI:%i\n",
            reportControl->name, (unsigned) reportControl->sqNum, reportControl->enabled,
            reportControl->isBuffering, reportControl->buffered, isIntegrity, isGI);

    updateTimeOfEntry(reportControl, Hal_getTimeInMs());

    ReportBuffer* buffer = reportControl->reportBuffer;

    /* calculate size of complete buffer entry */
    int bufferEntrySize = sizeof(ReportBufferEntry);

    int inclusionFieldSize = MmsValue_getBitStringByteSize(reportControl->inclusionField);

    MmsValue inclusionFieldStatic;

    inclusionFieldStatic.type = MMS_BIT_STRING;
    inclusionFieldStatic.value.bitString.size = MmsValue_getBitStringSize(reportControl->inclusionField);

    MmsValue* inclusionField = &inclusionFieldStatic;

    if (isIntegrity || isGI) {

        DataSetEntry* dataSetEntry = reportControl->dataSet->fcdas;

        int i;

        for (i = 0; i < MmsValue_getBitStringSize(reportControl->inclusionField); i++) {
            assert(dataSetEntry != NULL);

            bufferEntrySize += 1; /* reason-for-inclusion */

            bufferEntrySize += MmsValue_getSizeInMemory(dataSetEntry->value);

            dataSetEntry = dataSetEntry->sibling;
        }
    }
    else { /* other trigger reason */
        bufferEntrySize += inclusionFieldSize;

        int i;

        for (i = 0; i < MmsValue_getBitStringSize(reportControl->inclusionField); i++) {

            if (reportControl->inclusionFlags[i] != REPORT_CONTROL_NONE) {
                bufferEntrySize += 1; /* reason-for-inclusion */

                assert(reportControl->bufferedDataSetValues[i] != NULL);

                bufferEntrySize += MmsValue_getSizeInMemory(reportControl->bufferedDataSetValues[i]);
            }
        }
    }

    if (DEBUG_IED_SERVER)
        printf("IED_SERVER: enqueueReport: bufferEntrySize: %i\n", bufferEntrySize);

    if (bufferEntrySize > buffer->memoryBlockSize) {
        if (DEBUG_IED_SERVER)
            printf("IED_SERVER: enqueueReport: report buffer too small for report entry! Skip event!\n");

        goto exit_function;
    }

    if (isGI) removeAllGIReportsFromReportBuffer(buffer);

    uint8_t* entryBufPos = NULL;
    uint8_t* entryStartPos;

    if (DEBUG_IED_SERVER)
        printf("IED_SERVER: number of buffered reports:%i\n", buffer->reportsCount);

    if (buffer->lastEnqueuedReport == NULL) { /* buffer is empty - we start at the beginning of the memory block */
        entryBufPos = buffer->memoryBlock;
        buffer->oldestReport = (ReportBufferEntry*) entryBufPos;
        buffer->nextToTransmit = (ReportBufferEntry*) entryBufPos;
    }
    else {

        assert(buffer->lastEnqueuedReport != NULL);
        assert(buffer->oldestReport != NULL);

        if (DEBUG_IED_SERVER) {
            printf("IED_SERVER:   buffer->lastEnqueuedReport: %p\n", buffer->lastEnqueuedReport);
            printf("IED_SERVER:   buffer->oldestReport: %p\n", buffer->oldestReport);
            printf("IED_SERVER:   buffer->nextToTransmit: %p\n", buffer->nextToTransmit);
        }

        if (DEBUG_IED_SERVER) printf ("IED_SERVER: Last buffer offset: %i\n", (int) ((uint8_t*) buffer->lastEnqueuedReport - buffer->memoryBlock));

        if (buffer->lastEnqueuedReport == buffer->oldestReport) { /* --> buffer->reportsCount == 1 */
            assert(buffer->reportsCount == 1);

            entryBufPos = (uint8_t*) ((uint8_t*) buffer->lastEnqueuedReport + buffer->lastEnqueuedReport->entryLength);

            if ((entryBufPos + bufferEntrySize) > (buffer->memoryBlock + buffer->memoryBlockSize)) { /* buffer overflow */
                entryBufPos = buffer->memoryBlock;

#if (DEBUG_IED_SERVER == 1)
                printf("IED_SERVER: REMOVE report with ID ");
                printReportId(buffer->oldestReport);
                printf("\n");
#endif

                buffer->reportsCount = 0;
                buffer->oldestReport = (ReportBufferEntry*) entryBufPos;
                buffer->oldestReport->next = NULL;
                buffer->nextToTransmit = NULL;
            }
            else {
                if (buffer->nextToTransmit == buffer->oldestReport)
                    buffer->nextToTransmit = buffer->lastEnqueuedReport;

                buffer->oldestReport = buffer->lastEnqueuedReport;
                buffer->oldestReport->next = (ReportBufferEntry*) entryBufPos;
            }

        }
        else if (buffer->lastEnqueuedReport > buffer->oldestReport) {
            entryBufPos = (uint8_t*) ((uint8_t*) buffer->lastEnqueuedReport + buffer->lastEnqueuedReport->entryLength);

            if ((entryBufPos + bufferEntrySize) > (buffer->memoryBlock + buffer->memoryBlockSize)) { /* buffer overflow */
                entryBufPos = buffer->memoryBlock;

                /* remove old reports until enough space for new entry is available */
                while ((entryBufPos + bufferEntrySize) > (uint8_t*) buffer->oldestReport) {
                    assert(buffer->oldestReport != NULL);

                    if (buffer->nextToTransmit == buffer->oldestReport) {
                        buffer->nextToTransmit = buffer->oldestReport->next;
                        buffer->isOverflow = true;
                    }

#if (DEBUG_IED_SERVER == 1)
                    printf("IED_SERVER: REMOVE report with ID ");
                    printReportId(buffer->oldestReport);
                    printf("\n");
#endif

                    buffer->oldestReport = buffer->oldestReport->next;

                    buffer->reportsCount--;

                    if (buffer->oldestReport == NULL) {
                        buffer->oldestReport = (ReportBufferEntry*) entryBufPos;
                        buffer->oldestReport->next = NULL;
                        break;
                    }
                }
            }

            buffer->lastEnqueuedReport->next = (ReportBufferEntry*) entryBufPos;
        }
        else if (buffer->lastEnqueuedReport < buffer->oldestReport) {
            entryBufPos = (uint8_t*) ((uint8_t*) buffer->lastEnqueuedReport + buffer->lastEnqueuedReport->entryLength);

            if ((entryBufPos + bufferEntrySize) > (buffer->memoryBlock + buffer->memoryBlockSize)) { /* buffer overflow */
                entryBufPos = buffer->memoryBlock;

                /* remove older reports in upper buffer part */
                while ((uint8_t*) buffer->oldestReport > buffer->memoryBlock) {
                    assert(buffer->oldestReport != NULL);

                    if (buffer->nextToTransmit == buffer->oldestReport) {
                        buffer->nextToTransmit = buffer->oldestReport->next;
                        buffer->isOverflow = true;
                    }

#if (DEBUG_IED_SERVER == 1)
                    printf("IED_SERVER: REMOVE report with ID ");
                    printReportId(buffer->oldestReport);
                    printf("\n");
#endif

                    buffer->oldestReport = buffer->oldestReport->next;
                    buffer->reportsCount--;
                }

                /* remove older reports in lower buffer part that will be overwritten by new report */
                while ((entryBufPos + bufferEntrySize) > (uint8_t*) buffer->oldestReport) {
                    if (buffer->oldestReport == NULL)
                        break;

                    assert(buffer->oldestReport != NULL);

                    if (buffer->nextToTransmit == buffer->oldestReport) {
                        buffer->nextToTransmit = buffer->oldestReport->next;
                        buffer->isOverflow = true;
                    }

#if (DEBUG_IED_SERVER == 1)
                    printf("IED_SERVER: REMOVE report with ID ");
                    printReportId(buffer->oldestReport);
                    printf("\n");
#endif

                    buffer->oldestReport = buffer->oldestReport->next;
                    buffer->reportsCount--;
                }
            }
            else {
                while (((entryBufPos + bufferEntrySize) > (uint8_t*) buffer->oldestReport) && ((uint8_t*) buffer->oldestReport != buffer->memoryBlock)) {

                    if (buffer->oldestReport == NULL)
                        break;

                    assert(buffer->oldestReport != NULL);

                    if (buffer->nextToTransmit == buffer->oldestReport) {
                        buffer->nextToTransmit = buffer->oldestReport->next;
                        buffer->isOverflow = true;
                    }

#if (DEBUG_IED_SERVER == 1)
                    printf("IED_SERVER: REMOVE report with ID ");
                    printReportId(buffer->oldestReport);
                    printf("\n");
#endif

                    buffer->oldestReport = buffer->oldestReport->next;
                    buffer->reportsCount--;
                }
            }

            buffer->lastEnqueuedReport->next = (ReportBufferEntry*) entryBufPos;
        }

    }

    entryStartPos = entryBufPos;
    buffer->lastEnqueuedReport = (ReportBufferEntry*) entryBufPos;
    buffer->lastEnqueuedReport->next = NULL;
    buffer->reportsCount++;

    ReportBufferEntry* entry = (ReportBufferEntry*) entryBufPos;

    /* ENTRY_ID is set to system time in ms! */
    uint64_t entryId = timeOfEntry;

    if (entryId <= reportControl->lastEntryId)
    	entryId = reportControl->lastEntryId + 1;

    entry->timeOfEntry = entryId;

#if (ORDER_LITTLE_ENDIAN == 1)
    memcpyReverseByteOrder(entry->entryId, (uint8_t*) &entryId, 8);
#else
    memcpy (entry->entryId, (uint8_t*) &entryId, 8);
#endif

#if (DEBUG_IED_SERVER == 1)
    printf("IED_SERVER: ENCODE REPORT WITH ID: ");
    printReportId(entry);
    printf(" at pos %p\n", entryStartPos);
#endif

    if (reportControl->enabled == false) {
        MmsValue* entryIdValue = MmsValue_getElement(reportControl->rcbValues, 11);
        MmsValue_setOctetString(entryIdValue, (uint8_t*) entry->entryId, 8);
    }

    if (isIntegrity)
        entry->flags = 1;
    else if (isGI)
        entry->flags = 2;
    else
        entry->flags = 0;

    if ((bufferEntrySize % sizeof(void*)) > 0)
       bufferEntrySize  = sizeof(void*) * ((bufferEntrySize  + sizeof(void*) - 1) / sizeof(void*));

    entry->entryLength = bufferEntrySize;

    entryBufPos += sizeof(ReportBufferEntry);

    if (isIntegrity || isGI) {
        DataSetEntry* dataSetEntry = reportControl->dataSet->fcdas;

        int i;

        for (i = 0; i < MmsValue_getBitStringSize(reportControl->inclusionField); i++) {

            assert(dataSetEntry != NULL);

            *entryBufPos = (uint8_t) reportControl->inclusionFlags[i];
            entryBufPos++;

            entryBufPos = MmsValue_cloneToBuffer(dataSetEntry->value, entryBufPos);

            dataSetEntry = dataSetEntry->sibling;
        }

    }
    else {
        inclusionFieldStatic.value.bitString.buf = entryBufPos;
        memset(entryBufPos, 0, inclusionFieldSize);
        entryBufPos += inclusionFieldSize;

        int i;

        for (i = 0; i < MmsValue_getBitStringSize(reportControl->inclusionField); i++) {

            if (reportControl->inclusionFlags[i] != REPORT_CONTROL_NONE) {

                assert(reportControl->bufferedDataSetValues[i] != NULL);

                *entryBufPos = (uint8_t) reportControl->inclusionFlags[i];
                entryBufPos++;

                entryBufPos = MmsValue_cloneToBuffer(reportControl->bufferedDataSetValues[i], entryBufPos);

                MmsValue_setBitStringBit(inclusionField, i, true);
            }

        }
    }

    /* clear inclusion flags */
    int i;

    for (i = 0; i < reportControl->dataSet->elementCount; i++)
        reportControl->inclusionFlags[i] = REPORT_CONTROL_NONE;

    if (DEBUG_IED_SERVER)
        printf("IED_SERVER: enqueueReport: encoded %i bytes for report (estimated %i) at buffer offset %i\n",
            (int) (entryBufPos - entryStartPos), bufferEntrySize, (int) (entryStartPos - buffer->memoryBlock));

#if (DEBUG_IED_SERVER == 1)
    printEnqueuedReports(reportControl);
#endif

    if (buffer->nextToTransmit == NULL)
        buffer->nextToTransmit = buffer->lastEnqueuedReport;

    if (buffer->oldestReport == NULL)
        buffer->oldestReport = buffer->lastEnqueuedReport;

    reportControl->lastEntryId = entryId;

exit_function:
    return;
} /* enqueuReport() */

static void
sendNextReportEntry(ReportControl* self)
{
#define LOCAL_STORAGE_MEMORY_SIZE 65536

    if (self->reportBuffer->nextToTransmit == NULL)
        return;

    char* localStorage = (char*) GLOBAL_MALLOC(LOCAL_STORAGE_MEMORY_SIZE); /* reserve 64k for dynamic memory allocation -
                                     this can be optimized - maybe there is a good guess for the
                                     required memory size */

    if (localStorage == NULL) /* out-of-memory */
        goto return_out_of_memory;

    MemoryAllocator ma;
    MemoryAllocator_init(&ma, localStorage, LOCAL_STORAGE_MEMORY_SIZE);

    ReportBufferEntry* report = self->reportBuffer->nextToTransmit;

#if (DEBUG_IED_SERVER == 1)
    printf("IED_SERVER: SEND NEXT REPORT: ");
    printReportId(report);
    printf(" size: %i\n", report->entryLength);
#endif

    MmsValue* entryIdValue = MmsValue_getElement(self->rcbValues, 11);
    MmsValue_setOctetString(entryIdValue, (uint8_t*) report->entryId, 8);

    MemAllocLinkedList reportElements = MemAllocLinkedList_create(&ma);

    assert(reportElements != NULL);

    if (reportElements == NULL)
        goto return_out_of_memory;

    MmsValue* rptId = ReportControl_getRCBValue(self, "RptID");
    MmsValue* optFlds = ReportControl_getRCBValue(self, "OptFlds");

    if (MemAllocLinkedList_add(reportElements, rptId) == NULL)
        goto return_out_of_memory;

    if (MemAllocLinkedList_add(reportElements, optFlds) == NULL)
        goto return_out_of_memory;

    MmsValue inclusionFieldStack;

    inclusionFieldStack.type = MMS_BIT_STRING;
    inclusionFieldStack.value.bitString.size = MmsValue_getBitStringSize(self->inclusionField);

    uint8_t* currentReportBufferPos = (uint8_t*) report + sizeof(ReportBufferEntry);

    inclusionFieldStack.value.bitString.buf = currentReportBufferPos;

    MmsValue* inclusionField = &inclusionFieldStack;

    if (report->flags == 0)
        currentReportBufferPos += MmsValue_getBitStringByteSize(inclusionField);
    else {
        inclusionFieldStack.value.bitString.buf =
                (uint8_t*) MemoryAllocator_allocate(&ma, MmsValue_getBitStringByteSize(inclusionField));

        if (inclusionFieldStack.value.bitString.buf == NULL)
            goto return_out_of_memory;
    }

    uint8_t* valuesInReportBuffer = currentReportBufferPos;

    MmsValue_setBitStringBit(optFlds, 9, false); /* segmentation */

    MmsValue* sqNum = ReportControl_getRCBValue(self, "SqNum");

    if (MmsValue_getBitStringBit(optFlds, 1)) /* sequence number */
        if (MemAllocLinkedList_add(reportElements, sqNum) == NULL)
            goto return_out_of_memory;

    if (MmsValue_getBitStringBit(optFlds, 2)) { /* report time stamp */
    	MmsValue* timeOfEntry = (MmsValue*) MemoryAllocator_allocate(&ma, sizeof(MmsValue));

    	if (timeOfEntry == NULL) goto return_out_of_memory;

    	timeOfEntry->deleteValue = 0;
    	timeOfEntry->type = MMS_BINARY_TIME;
    	timeOfEntry->value.binaryTime.size = 6;

    	MmsValue_setBinaryTime(timeOfEntry, report->timeOfEntry);

        if (MemAllocLinkedList_add(reportElements, timeOfEntry) == NULL)
            goto return_out_of_memory;
    }

    if (MmsValue_getBitStringBit(optFlds, 4)) {/* data set reference */
        MmsValue* datSet = ReportControl_getRCBValue(self, "DatSet");
        if (MemAllocLinkedList_add(reportElements, datSet) == NULL)
            goto return_out_of_memory;
    }

    if (MmsValue_getBitStringBit(optFlds, 6)) { /* bufOvfl */

        MmsValue* bufOvfl = (MmsValue*) MemoryAllocator_allocate(&ma, sizeof(MmsValue));

        if (bufOvfl == NULL) goto return_out_of_memory;

        bufOvfl->deleteValue = 0;
        bufOvfl->type = MMS_BOOLEAN;
        bufOvfl->value.boolean = self->reportBuffer->isOverflow;

        if (self->reportBuffer->isOverflow)
            self->reportBuffer->isOverflow = false;

        if (MemAllocLinkedList_add(reportElements, bufOvfl) == NULL)
            goto return_out_of_memory;
    }

    if (MmsValue_getBitStringBit(optFlds, 7)) { /* entryID */
        MmsValue* entryId = (MmsValue*) MemoryAllocator_allocate(&ma, sizeof(MmsValue));

        if (entryId == NULL) goto return_out_of_memory;

        entryId->deleteValue = 0;
        entryId->type = MMS_OCTET_STRING;
        entryId->value.octetString.buf = report->entryId;
        entryId->value.octetString.size = 8;
        entryId->value.octetString.maxSize = 8;

        if (MemAllocLinkedList_add(reportElements, entryId) == NULL)
            goto return_out_of_memory;
    }

    if (MmsValue_getBitStringBit(optFlds, 8))
        if (MemAllocLinkedList_add(reportElements, self->confRev) == NULL)
            goto return_out_of_memory;

    if (report->flags > 0)
        MmsValue_setAllBitStringBits(inclusionField);

    if (MemAllocLinkedList_add(reportElements, inclusionField) == NULL)
        goto return_out_of_memory;

    /* add data references if selected */
    if (MmsValue_getBitStringBit(optFlds, 5)) { /* data-reference */
        DataSetEntry* dataSetEntry = self->dataSet->fcdas;

        LogicalDevice* ld = (LogicalDevice*) self->parentLN->parent;

        IedModel* iedModel = (IedModel*) ld->parent;

        char* iedName = iedModel->name;

        int iedNameLength = strlen(iedName);

        int i = 0;

        for (i = 0; i < self->dataSet->elementCount; i++) {
          assert(dataSetEntry->value != NULL);

          bool addReferenceForEntry = false;

          if (report->flags > 0)
             addReferenceForEntry = true;
          else
              if (MmsValue_getBitStringBit(inclusionField, i))
                  addReferenceForEntry = true;

          if (addReferenceForEntry) {

              int ldNameLength =  strlen(dataSetEntry->logicalDeviceName);
              int variableNameLength = strlen(dataSetEntry->variableName);

              int refLen = iedNameLength
                      + ldNameLength
                      + variableNameLength + 1;

              char* dataReference = (char*) MemoryAllocator_allocate(&ma, refLen + 1);

              if (dataReference == NULL) goto return_out_of_memory;

              int currentPos = 0;

              int j;

              for (j = 0; j < iedNameLength; j++) {
                  dataReference[currentPos++] = iedName[j];
              }

              for (j = 0; j <  ldNameLength; j++) {
                  dataReference[currentPos] = dataSetEntry->logicalDeviceName[j];
                  currentPos++;
              }

              dataReference[currentPos++] = '/';

              for (j = 0; j < variableNameLength; j++) {
                  dataReference[currentPos++] = dataSetEntry->variableName[j];
              }

              dataReference[currentPos] = 0;

              MmsValue* dataRef = (MmsValue*) MemoryAllocator_allocate(&ma, sizeof(MmsValue));

              if (dataRef == NULL) goto return_out_of_memory;

              dataRef->deleteValue = 0;
              dataRef->type = MMS_VISIBLE_STRING;
              dataRef->value.visibleString.buf = dataReference;
              dataRef->value.visibleString.size = refLen;


              if (MemAllocLinkedList_add(reportElements, dataRef) == NULL)
                  goto return_out_of_memory;

          }

          dataSetEntry = dataSetEntry->sibling;

        }
    }

    /* add data set value elements */
    int i = 0;
    for (i = 0; i < self->dataSet->elementCount; i++) {

        if (report->flags > 0) {
            currentReportBufferPos++;
            if (MemAllocLinkedList_add(reportElements, currentReportBufferPos) == NULL)
                goto return_out_of_memory;

            currentReportBufferPos += MmsValue_getSizeInMemory((MmsValue*) currentReportBufferPos);
        }
        else {
            if (MmsValue_getBitStringBit(inclusionField, i)) {
                currentReportBufferPos++;
                if (MemAllocLinkedList_add(reportElements, currentReportBufferPos) == NULL)
                    goto return_out_of_memory;
                currentReportBufferPos += MmsValue_getSizeInMemory((MmsValue*) currentReportBufferPos);
            }
        }
    }

    /* add reason code to report if requested */
    if (MmsValue_getBitStringBit(optFlds, 3)) {
        currentReportBufferPos = valuesInReportBuffer;

        for (i = 0; i < self->dataSet->elementCount; i++) {

            if (report->flags > 0) {
                MmsValue* reason = (MmsValue*) MemoryAllocator_allocate(&ma, sizeof(MmsValue));

                if (reason == NULL) goto return_out_of_memory;

                reason->deleteValue = 0;
                reason->type = MMS_BIT_STRING;
                reason->value.bitString.size = 6;
                reason->value.bitString.buf = (uint8_t*) MemoryAllocator_allocate(&ma, 1);

                if (reason->value.bitString.buf == NULL) goto return_out_of_memory;

                MmsValue_deleteAllBitStringBits(reason);

                if (report->flags & 0x02) /* GI */
                    MmsValue_setBitStringBit(reason, 5, true);

                if (report->flags & 0x01) /* Integrity */
                    MmsValue_setBitStringBit(reason, 4, true);

                if (MemAllocLinkedList_add(reportElements, reason) == NULL)
                    goto return_out_of_memory;

                currentReportBufferPos++;

                MmsValue* dataSetElement = (MmsValue*) currentReportBufferPos;

                currentReportBufferPos += MmsValue_getSizeInMemory(dataSetElement);
            }
            else if (MmsValue_getBitStringBit(inclusionField, i)) {
                MmsValue* reason = (MmsValue*) MemoryAllocator_allocate(&ma, sizeof(MmsValue));

                if (reason == NULL) goto return_out_of_memory;

                reason->deleteValue = 0;
                reason->type = MMS_BIT_STRING;
                reason->value.bitString.size = 6;
                reason->value.bitString.buf = (uint8_t*) MemoryAllocator_allocate(&ma, 1);

                if (reason->value.bitString.buf == NULL)
                    goto return_out_of_memory;

                MmsValue_deleteAllBitStringBits(reason);

                switch((ReportInclusionFlag) *currentReportBufferPos) {
                case REPORT_CONTROL_QUALITY_CHANGED:
                    MmsValue_setBitStringBit(reason, 2, true);
                    break;
                case REPORT_CONTROL_VALUE_CHANGED:
                    MmsValue_setBitStringBit(reason, 1, true);
                    break;
                case REPORT_CONTROL_VALUE_UPDATE:
                    MmsValue_setBitStringBit(reason, 3, true);
                    break;
                default:
                    break;
                }

                currentReportBufferPos++;

                MmsValue* dataSetElement = (MmsValue*) currentReportBufferPos;

                currentReportBufferPos += MmsValue_getSizeInMemory(dataSetElement);

                if (MemAllocLinkedList_add(reportElements, reason) == NULL)
                    goto return_out_of_memory;
            }
        }
    }

    MmsServerConnection_sendInformationReportVMDSpecific(self->clientConnection, "RPT", (LinkedList) reportElements, false);

    /* Increase sequence number */
    self->sqNum++;
    MmsValue_setUint32(sqNum, self->sqNum);

    assert(self->reportBuffer->nextToTransmit != self->reportBuffer->nextToTransmit->next);

    self->reportBuffer->nextToTransmit = self->reportBuffer->nextToTransmit->next;

    if (DEBUG_IED_SERVER)
        printf("IED_SERVER: sendNextReportEntry: memory(used/size): %i/%i\n",
                (int) (ma.currentPtr - ma.memoryBlock), ma.size);

#if (DEBUG_IED_SERVER == 1)
    printf("IED_SERVER: reporting.c nextToTransmit: %p\n", self->reportBuffer->nextToTransmit);
    printEnqueuedReports(self);
#endif

    goto cleanup_and_return;

return_out_of_memory:

    if (DEBUG_IED_SERVER)
        printf("IED_SERVER: sendNextReportEntry failed - memory allocation problem!\n");

    //TODO set some flag to notify application here

cleanup_and_return:

    if (localStorage != NULL)
        GLOBAL_FREEMEM(localStorage);

}

void
Reporting_activateBufferedReports(MmsMapping* self)
{
    LinkedList element = self->reportControls;

    while ((element = LinkedList_getNext(element)) != NULL ) {
        ReportControl* rc = (ReportControl*) element->data;

        if (rc->buffered) {
            if (updateReportDataset(self, rc, NULL, NULL))
                rc->isBuffering = true;
            else
                rc->isBuffering = false;
        }
    }
}

static void
processEventsForReport(ReportControl* rc, uint64_t currentTimeInMs)
{
    if ((rc->enabled) || (rc->isBuffering)) {

        if (rc->triggerOps & TRG_OPT_GI) {
            if (rc->gi) {

                /* send current events in event buffer before GI report */
                if (rc->triggered) {
                    if (rc->buffered)
                        enqueueReport(rc, false, false, currentTimeInMs);
                    else
                        sendReport(rc, false, false);

                    rc->triggered = false;
                }

                if (rc->buffered)
                    enqueueReport(rc, false, true, currentTimeInMs);
                else
                    sendReport(rc, false, true);

                rc->gi = false;

                rc->triggered = false;
            }
        }

        if (rc->triggerOps & TRG_OPT_INTEGRITY) {

            if (rc->intgPd > 0) {
                if (currentTimeInMs >= rc->nextIntgReportTime) {

                    /* send current events in event buffer before integrity report */
                    if (rc->triggered) {
                        if (rc->buffered)
                            enqueueReport(rc, false, false, currentTimeInMs);
                        else
                            sendReport(rc, false, false);

                        rc->triggered = false;
                    }

                    rc->nextIntgReportTime = currentTimeInMs + rc->intgPd;

                    if (rc->buffered)
                        enqueueReport(rc, true, false, currentTimeInMs);
                    else
                        sendReport(rc, true, false);

                    rc->triggered = false;
                }
            }
        }

        if (rc->triggered) {
            if (currentTimeInMs >= rc->reportTime) {

                if (rc->buffered)
                    enqueueReport(rc, false, false, currentTimeInMs);
                else
                    sendReport(rc, false, false);

                rc->triggered = false;
            }
        }

        if (rc->buffered && rc->enabled)
            sendNextReportEntry(rc);
    }


}

void
Reporting_processReportEvents(MmsMapping* self, uint64_t currentTimeInMs)
{
    LinkedList element = self->reportControls;

    while ((element = LinkedList_getNext(element)) != NULL ) {
        ReportControl* rc = (ReportControl*) element->data;

        ReportControl_lockNotify(rc);

        processEventsForReport(rc, currentTimeInMs);

        ReportControl_unlockNotify(rc);
    }
}

void
ReportControl_valueUpdated(ReportControl* self, int dataSetEntryIndex, ReportInclusionFlag flag)
{
    ReportControl_lockNotify(self);

    if (self->inclusionFlags[dataSetEntryIndex] != 0) { /* report for this data set entry is already pending (bypass BufTm) */
        self->reportTime = Hal_getTimeInMs();
        processEventsForReport(self, self->reportTime);
    }

    self->inclusionFlags[dataSetEntryIndex] = flag;

    /* buffer value for report */
    if (self->bufferedDataSetValues[dataSetEntryIndex] == NULL)
        self->bufferedDataSetValues[dataSetEntryIndex] = MmsValue_clone(self->valueReferences[dataSetEntryIndex]);
    else
        MmsValue_update(self->bufferedDataSetValues[dataSetEntryIndex], self->valueReferences[dataSetEntryIndex]);

    if (self->triggered == false) {
        uint64_t currentTime = Hal_getTimeInMs();

        MmsValue_setBinaryTime(self->timeOfEntry, currentTime);

        self->reportTime = currentTime + self->bufTm;
    }

    self->triggered = true;

    ReportControl_unlockNotify(self);
}

#endif /* (CONFIG_IEC61850_REPORT_SERVICE == 1) */
