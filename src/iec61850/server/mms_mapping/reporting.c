/*
 *  reporting.c
 *
 *  Copyright 2013-2022 Michael Zillgith
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
#include "stack_config.h"
#include "hal_thread.h"

#include "simple_allocator.h"
#include "mem_alloc_linked_list.h"
#include "ber_encoder.h"

#include "mms_mapping_internal.h"
#include "mms_value_internal.h"
#include "mms_server_internal.h"
#include "conversions.h"
#include "reporting.h"
#include "ied_server_private.h"
#include <string.h>

/* if not explicitly set by client "ResvTms" will be set to this value */
#define RESV_TMS_IMPLICIT_VALUE 10

#ifndef DEBUG_IED_SERVER
#define DEBUG_IED_SERVER 0
#endif

#if (CONFIG_IEC61850_REPORT_SERVICE == 1)

#ifndef CONFIG_IEC61850_BRCB_WITH_RESVTMS
#define CONFIG_IEC61850_BRCB_WITH_RESVTMS 0
#endif

static ReportBuffer*
ReportBuffer_create(int bufferSize)
{
    ReportBuffer* self = (ReportBuffer*) GLOBAL_MALLOC(sizeof(ReportBuffer));

    if (self) {
        self->lastEnqueuedReport = NULL;
        self->oldestReport = NULL;
        self->nextToTransmit = NULL;
        self->reportsCount = 0;
        self->isOverflow = true;

        self->memoryBlockSize = bufferSize;
        self->memoryBlock = (uint8_t*) GLOBAL_MALLOC(self->memoryBlockSize);

        if (self->memoryBlock == NULL) {
            GLOBAL_FREEMEM(self);
            self = NULL;
        }
        else {
#if (CONFIG_MMS_THREADLESS_STACK != 1)
            self->lock = Semaphore_create(1);
#endif
        }
    }

    return self;
}

static void
ReportBuffer_destroy(ReportBuffer* self)
{
    if (self) {
        GLOBAL_FREEMEM(self->memoryBlock);

#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_destroy(self->lock);
#endif

        GLOBAL_FREEMEM(self);
    }
}

ReportControl*
ReportControl_create(bool buffered, LogicalNode* parentLN, int reportBufferSize, IedServer iedServer)
{
    ReportControl* self = (ReportControl*) GLOBAL_MALLOC(sizeof(ReportControl));

    if (self) {
        self->name = NULL;
        self->domain = NULL;
        self->parentLN = parentLN;
        self->rcbValues = NULL;

#if (CONFIG_MMS_THREADLESS_STACK != 1)
        self->rcbValuesLock = Semaphore_create(1);
#endif

        self->subSeqVal = MmsValue_newUnsigned(16);
        self->segmented = false;
        self->startIndexForNextSegment = 0;
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
        self->hasOwner = false;

#if (CONFIG_MMS_THREADLESS_STACK != 1)
        self->createNotificationsMutex = Semaphore_create(1);
#endif

        self->bufferedDataSetValues = NULL;
        self->valueReferences = NULL;
        self->lastEntryId = 0;
        self->resvTms = 0;

        self->server = iedServer;

        self->reportBuffer = ReportBuffer_create(reportBufferSize);
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
    if (DEBUG_IED_SERVER) printf("IED_SERVER: RCB %s purgeBuf\n", rc->name);

    /* reset trigger */
    rc->triggered = false;

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
    if (self) {
        if (self->rcbValues != NULL )
            MmsValue_delete(self->rcbValues);

        if (self->inclusionFlags != NULL)
            GLOBAL_FREEMEM(self->inclusionFlags);

        if (self->inclusionField != NULL)
            MmsValue_delete(self->inclusionField);

        if (self->buffered == false)
            MmsValue_delete(self->timeOfEntry);

        MmsValue_delete(self->subSeqVal);

        deleteDataSetValuesShadowBuffer(self);

        if (self->isDynamicDataSet) {
            if (self->dataSet != NULL) {
                MmsMapping_freeDynamicallyCreatedDataSet(self->dataSet);
                self->isDynamicDataSet = false;
                self->dataSet = NULL;
            }
        }

        /* restore original sibling of ReportControlBlock */
        self->rcb->sibling = self->sibling;
        self->rcb->trgOps &= ~(64); /* clear runtime mode flag */

        ReportBuffer_destroy(self->reportBuffer);

#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_destroy(self->createNotificationsMutex);
        Semaphore_destroy(self->rcbValuesLock);
#endif

        GLOBAL_FREEMEM(self->name);

        GLOBAL_FREEMEM(self);
    }
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

        if (rc->server->edition >= IEC_61850_EDITION_2) {
#if (CONFIG_IEC61850_BRCB_WITH_RESVTMS == 1)
            if (rc->server->enableBRCBResvTms) {
                if (strcmp(elementName, "ResvTms") == 0)
                    return MmsValue_getElement(rc->rcbValues, 13);
                if (strcmp(elementName, "Owner") == 0)
                    return MmsValue_getElement(rc->rcbValues, 14);
            }
            else {
                if (strcmp(elementName, "Owner") == 0)
                    return MmsValue_getElement(rc->rcbValues, 13);
            }
#else
            if (strcmp(elementName, "Owner") == 0)
                return MmsValue_getElement(rc->rcbValues, 13);
#endif
        }

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

#if (CONFIG_IEC61850_SERVICE_TRACKING == 1)

static void
copyRCBValuesToTrackingObject(MmsMapping* self, ReportControl* rc)
{
#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_wait(rc->rcbValuesLock);
#endif

    if (rc->buffered) {
        if (self->brcbTrk) {
            BrcbTrkInstance trkInst = self->brcbTrk;

            if (trkInst->rptID)
                MmsValue_update(trkInst->rptID->mmsValue, ReportControl_getRCBValue(rc, "RptID"));

            if (trkInst->rptEna)
                MmsValue_update(trkInst->rptEna->mmsValue, ReportControl_getRCBValue(rc, "RptEna"));

            if (trkInst->datSet) {
                char datSet[130];
                const char* datSetStr = MmsValue_toString(ReportControl_getRCBValue(rc, "DatSet"));

                if (datSetStr) {
                    StringUtils_copyStringMax(datSet, 130, datSetStr);

                    StringUtils_replace(datSet, '$', '.');

                    MmsValue_setVisibleString(trkInst->datSet->mmsValue, datSet);
                }
            }

            if (trkInst->confRev)
                MmsValue_update(trkInst->confRev->mmsValue, ReportControl_getRCBValue(rc, "ConfRev"));

            if (trkInst->optFlds)
                MmsValue_update(trkInst->optFlds->mmsValue, ReportControl_getRCBValue(rc, "OptFlds"));

            if (trkInst->bufTm)
                MmsValue_update(trkInst->bufTm->mmsValue, ReportControl_getRCBValue(rc, "BufTm"));

            if (trkInst->sqNum)
                MmsValue_update(trkInst->sqNum->mmsValue, ReportControl_getRCBValue(rc, "SqNum"));

            if (trkInst->trgOps)
                MmsValue_update(trkInst->trgOps->mmsValue, ReportControl_getRCBValue(rc, "TrgOps"));

            if (trkInst->intgPd)
                MmsValue_update(trkInst->intgPd->mmsValue, ReportControl_getRCBValue(rc, "IntgPd"));

            if (trkInst->gi)
                MmsValue_update(trkInst->gi->mmsValue, ReportControl_getRCBValue(rc, "GI"));

            if (trkInst->purgeBuf)
                MmsValue_update(trkInst->purgeBuf->mmsValue, ReportControl_getRCBValue(rc, "PurgeBuf"));

            if (trkInst->entryID)
                MmsValue_update(trkInst->entryID->mmsValue, ReportControl_getRCBValue(rc, "EntryID"));

            if (trkInst->timeOfEntry) {
                MmsValue* timeofEntryValue = ReportControl_getRCBValue(rc, "TimeofEntry");

                if (timeofEntryValue)
                    MmsValue_update(trkInst->timeOfEntry->mmsValue, timeofEntryValue);
            }


            if (trkInst->resvTms) {
                MmsValue* resvTmsValue = ReportControl_getRCBValue(rc, "ResvTms");

                if (resvTmsValue)
                    MmsValue_update(trkInst->resvTms->mmsValue, resvTmsValue);
            }
        }
    }
    else {
        if (self->urcbTrk) {
            UrcbTrkInstance trkInst = self->urcbTrk;

            if (trkInst->rptID)
                MmsValue_update(trkInst->rptID->mmsValue, ReportControl_getRCBValue(rc, "RptID"));

            if (trkInst->rptEna)
                MmsValue_update(trkInst->rptEna->mmsValue, ReportControl_getRCBValue(rc, "RptEna"));

            if (trkInst->resv)
                MmsValue_update(trkInst->rptEna->mmsValue, ReportControl_getRCBValue(rc, "Resv"));

            if (trkInst->datSet) {
                char datSet[130];
                const char* datSetStr = MmsValue_toString(ReportControl_getRCBValue(rc, "DatSet"));

                if (datSetStr) {
                    StringUtils_copyStringMax(datSet, 130, datSetStr);

                    StringUtils_replace(datSet, '$', '.');

                    MmsValue_setVisibleString(trkInst->datSet->mmsValue, datSet);
                }
            }

            if (trkInst->confRev)
                MmsValue_update(trkInst->confRev->mmsValue, ReportControl_getRCBValue(rc, "ConfRev"));

            if (trkInst->optFlds)
                MmsValue_update(trkInst->optFlds->mmsValue, ReportControl_getRCBValue(rc, "OptFlds"));

            if (trkInst->bufTm)
                MmsValue_update(trkInst->bufTm->mmsValue, ReportControl_getRCBValue(rc, "BufTm"));

            if (trkInst->sqNum)
                MmsValue_update(trkInst->sqNum->mmsValue, ReportControl_getRCBValue(rc, "SqNum"));

            if (trkInst->trgOps)
                MmsValue_update(trkInst->trgOps->mmsValue, ReportControl_getRCBValue(rc, "TrgOps"));

            if (trkInst->intgPd)
                MmsValue_update(trkInst->intgPd->mmsValue, ReportControl_getRCBValue(rc, "IntgPd"));

            if (trkInst->gi)
                MmsValue_update(trkInst->gi->mmsValue, ReportControl_getRCBValue(rc, "GI"));
        }
    }

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_post(rc->rcbValuesLock);
#endif
}

static void
updateSingleTrackingValue(MmsMapping* self, ReportControl* rc, char* name, MmsValue* newValue)
{
    if (rc->buffered) {
        if (self->brcbTrk) {
            BrcbTrkInstance trkInst = self->brcbTrk;

            DataAttribute* attributeToUpdate = NULL;

            if (!strcmp(name, "RptID"))
                attributeToUpdate = trkInst->rptID;
            else if (!strcmp(name, "RptEna"))
                attributeToUpdate = trkInst->rptEna;
            else if (!strcmp(name, "DatSet")) {

                char datSet[130];
                const char* datSetStr = MmsValue_toString(newValue);

                if (datSetStr) {
                    StringUtils_copyStringMax(datSet, 130, datSetStr);

                    StringUtils_replace(datSet, '$', '.');

                    MmsValue_setVisibleString(trkInst->datSet->mmsValue, datSet);
                }
            }
            else if (!strcmp(name, "ConfRev"))
                attributeToUpdate = trkInst->confRev;
            else if (!strcmp(name, "OptFlds"))
                attributeToUpdate = trkInst->optFlds;
            else if (!strcmp(name, "BufTm"))
                attributeToUpdate = trkInst->bufTm;
            else if (!strcmp(name, "SqNum"))
                attributeToUpdate = trkInst->sqNum;
            else if (!strcmp(name, "TrgOps"))
                attributeToUpdate = trkInst->trgOps;
            else if (!strcmp(name, "IntgPd"))
                attributeToUpdate = trkInst->intgPd;
            else if (!strcmp(name, "GI"))
                attributeToUpdate = trkInst->gi;
            else if (!strcmp(name, "PurgeBuf"))
                attributeToUpdate = trkInst->purgeBuf;
            else if (!strcmp(name, "TimeofEntry"))
                attributeToUpdate = trkInst->timeOfEntry;
            else if (!strcmp(name, "EntryID"))
                attributeToUpdate = trkInst->entryID;
            else if (!strcmp(name, "ResvTms"))
                attributeToUpdate = trkInst->resvTms;

            if (attributeToUpdate)
                MmsValue_update(attributeToUpdate->mmsValue, newValue);
        }
    }
    else {
        if (self->urcbTrk) {
            UrcbTrkInstance trkInst = self->urcbTrk;

            DataAttribute* attributeToUpdate = NULL;

            if (!strcmp(name, "RptID"))
                attributeToUpdate = trkInst->rptID;
            else if (!strcmp(name, "RptEna"))
                attributeToUpdate = trkInst->rptEna;
            else if (!strcmp(name, "Resv"))
                attributeToUpdate = trkInst->resv;
            else if (!strcmp(name, "DatSet")) {

                char datSet[130];
                const char* datSetStr = MmsValue_toString(newValue);

                if (datSetStr) {
                    StringUtils_copyStringMax(datSet, 130, datSetStr);

                    StringUtils_replace(datSet, '$', '.');

                    MmsValue_setVisibleString(trkInst->datSet->mmsValue, datSet);
                }
            }
            else if (!strcmp(name, "ConfRev"))
                attributeToUpdate = trkInst->confRev;
            else if (!strcmp(name, "OptFlds"))
                attributeToUpdate = trkInst->optFlds;
            else if (!strcmp(name, "BufTm"))
                attributeToUpdate = trkInst->bufTm;
            else if (!strcmp(name, "SqNum"))
                attributeToUpdate = trkInst->sqNum;
            else if (!strcmp(name, "TrgOps"))
                attributeToUpdate = trkInst->trgOps;
            else if (!strcmp(name, "IntgPd"))
                attributeToUpdate = trkInst->intgPd;
            else if (!strcmp(name, "GI"))
                attributeToUpdate = trkInst->gi;

            if (attributeToUpdate)
                MmsValue_update(attributeToUpdate->mmsValue, newValue);
        }
    }
}

static void
updateGenericTrackingObjectValues(MmsMapping* self, ReportControl* rc, IEC61850_ServiceType serviceType, MmsDataAccessError errVal)
{
    ServiceTrkInstance trkInst = NULL;

    if (rc->buffered) {
        if (self->brcbTrk) {
            trkInst = (ServiceTrkInstance) self->brcbTrk;
        }
    }
    else {
        if (self->urcbTrk) {
            trkInst = (ServiceTrkInstance) self->urcbTrk;
        }
    }

    if (trkInst) {
        if (trkInst->serviceType)
            MmsValue_setInt32(trkInst->serviceType->mmsValue, (int) serviceType);

        if (trkInst->t)
            MmsValue_setUtcTimeMsEx(trkInst->t->mmsValue, Hal_getTimeInMs(), self->iedServer->timeQuality);

        if (trkInst->errorCode)
            MmsValue_setInt32(trkInst->errorCode->mmsValue,
                    private_IedServer_convertMmsDataAccessErrorToServiceError(errVal));

        char objRef[130];

        /* create object reference */
        LogicalNode* ln = (LogicalNode*) rc->parentLN;
        LogicalDevice* ld = (LogicalDevice*) ln->parent;

        char* iedName = self->iedServer->mmsDevice->deviceName;

        snprintf(objRef, 129, "%s%s/%s", iedName, ld->name, rc->name);

        /* convert MMS name to ACSI object reference */
        int pos = 0;
        bool replace = false;
        do {
            if (replace) {
                objRef[pos] = objRef[pos + 3];
            }
            else {
                if (objRef[pos] == '$') {
                    objRef[pos] = '.';
                    replace = true;
                }
            }
        } while (objRef[pos++]);

        if (trkInst->objRef) {
            IedServer_updateVisibleStringAttributeValue(self->iedServer, trkInst->objRef, objRef);
        }
    }
}

#endif /* (CONFIG_IEC61850_SERVICE_TRACKING == 1) */

static inline void
clearInclusionFlags(ReportControl* reportControl)
{
    int i;
    for (i = 0; i < reportControl->dataSet->elementCount; i++)
        reportControl->inclusionFlags[i] = REPORT_CONTROL_NONE;
}

static void
updateTimeOfEntry(ReportControl* self, uint64_t currentTime)
{
    MmsValue* timeOfEntry = self->timeOfEntry;
    MmsValue_setBinaryTime(timeOfEntry, currentTime);
}

static DataSetEntry*
getDataSetEntryWithIndex(DataSetEntry* dataSet, int index)
{
    int i = 0;

    while (dataSet) {
        if (i == index)
            return dataSet;

        i++;

        dataSet = dataSet->sibling;
    }

    return NULL;
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

            if (rc->dataSet) {
                if (rc->buffered) {
                    rc->isBuffering = false;
                    purgeBuf(rc);
                }

                /* delete pending events */
                deleteDataSetValuesShadowBuffer(rc);

                if (rc->isDynamicDataSet) {
                    if (rc->dataSet) {
                        MmsMapping_freeDynamicallyCreatedDataSet(rc->dataSet);
                    }
                }

                /* release used data set */
                rc->dataSet = NULL;
            }
        }
        else
            dataSetValue = newDatSet;
    }
    else
        dataSetValue = ReportControl_getRCBValue(rc, "DatSet");

    bool dataSetChanged = true;

    /* check if old and new data sets are the same */
    if (rc->dataSet && dataSetValue) {
        const char* dataSetLdName = rc->dataSet->logicalDeviceName;
        const char* dataSetName = rc->dataSet->name;
        const char* newDataSetName = MmsValue_toString(dataSetValue);

        if (newDataSetName[0] == '@') {
            if ((dataSetLdName == NULL) && (!strcmp(dataSetName, newDataSetName + 1))) {
                dataSetChanged = false;
            }
        }
        else if (newDataSetName[0] == '/') {
            if ((dataSetLdName == NULL) && (!strcmp(dataSetName, newDataSetName + 1))) {
                dataSetChanged = false;
            }
        }
        else {
            if (dataSetLdName && dataSetName) {

                char externalVisibleName[256];

                /* Construct external visible name */
                StringUtils_concatString(externalVisibleName, 256, mapping->model->name, dataSetLdName);
                StringUtils_appendString(externalVisibleName, 256, "/");
                StringUtils_appendString(externalVisibleName, 256, dataSetName);

                if (!(strcmp(externalVisibleName, newDataSetName))) {
                    dataSetChanged = false;
                }
            }
        }

        if (rc->buffered) {
            if (dataSetChanged)
                purgeBuf(rc);
        }
    }

    if (dataSetValue) {
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

        if (dataSetChanged == true) {

            /* delete pending event and create buffer for new data set */
            deleteDataSetValuesShadowBuffer(rc);

            rc->dataSet = dataSet;

            createDataSetValuesShadowBuffer(rc);

            if (rc->inclusionField != NULL)
                MmsValue_delete(rc->inclusionField);

            rc->inclusionField = MmsValue_newBitString(dataSet->elementCount);

            rc->triggered = false;

            if (rc->inclusionFlags != NULL)
                GLOBAL_FREEMEM(rc->inclusionFlags);

            rc->inclusionFlags = (uint8_t*) GLOBAL_CALLOC(dataSet->elementCount, sizeof(uint8_t));

        }

        success = true;

        if (rc->buffered)
            rc->isBuffering = true;

        goto exit_function;
    }
    else {
        success = true;
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
#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_wait(rc->rcbValuesLock);
#endif

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

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_post(rc->rcbValuesLock);
#endif
}

static uint64_t
getNextRoundedStartTime(uint64_t currentTime, uint64_t intgPd)
{
    uint64_t modTime = currentTime % intgPd;
    uint64_t delta = intgPd;

    if (modTime != 0) {
        delta = intgPd - modTime;
    }

    uint64_t nextTime = currentTime + delta;

    return nextTime;
}

static void
refreshIntegrityPeriod(ReportControl* rc)
{
#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_wait(rc->rcbValuesLock);
#endif

    MmsValue* intgPd = ReportControl_getRCBValue(rc, "IntgPd");
    rc->intgPd = MmsValue_toUint32(intgPd);

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_post(rc->rcbValuesLock);
#endif

    if (rc->buffered == false) {

        if (rc->triggerOps & TRG_OPT_INTEGRITY) {

            if (rc->intgPd > 0) {

                if (rc->server->syncIntegrityReportTimes) {
                    rc->nextIntgReportTime = getNextRoundedStartTime(Hal_getTimeInMs(), rc->intgPd);
                }
                else {
                    rc->nextIntgReportTime = Hal_getTimeInMs() + rc->intgPd;
                }
            }
        }
    }
}

static void
refreshBufferTime(ReportControl* rc)
{
#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_wait(rc->rcbValuesLock);
#endif

    MmsValue* bufTm = ReportControl_getRCBValue(rc, "BufTm");
    rc->bufTm = MmsValue_toUint32(bufTm);

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_post(rc->rcbValuesLock);
#endif
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

    int structSize = 11;

    if (reportControl->server->edition >= IEC_61850_EDITION_2) {
        if (reportControl->hasOwner)
            structSize = 12;
    }

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

    if (reportControl->server->edition >= IEC_61850_EDITION_2) {

        if (reportControl->hasOwner) {
            namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
            namedVariable->name = StringUtils_copyString("Owner");
            namedVariable->type = MMS_OCTET_STRING;
            namedVariable->typeSpec.octetString = -64;
            rcb->typeSpec.structure.elements[11] = namedVariable;
            mmsValue->value.structure.components[11] = MmsValue_newOctetString(0, 16); /* size 16 is enough to store client IPv6 address */

            /* initialize pre configured owner */
            if (reportControlBlock->clientReservation[0] == 4) {
                reportControl->resvTms = -1;
                MmsValue_setOctetString(mmsValue->value.structure.components[11], reportControlBlock->clientReservation + 1, 4);
            }
            else if (reportControlBlock->clientReservation[0] == 6) {
                reportControl->resvTms = -1;
                MmsValue_setOctetString(mmsValue->value.structure.components[11], reportControlBlock->clientReservation + 1, 16);
            }
        }
    }

    /* check if there is a pre configured owner */
    if (reportControlBlock->clientReservation[0] > 0) {
        MmsValue_setBoolean(mmsValue->value.structure.components[2], true);
    }

    reportControl->rcbValues = mmsValue;

    reportControl->timeOfEntry = MmsValue_newBinaryTime(false);

    refreshBufferTime(reportControl);
    refreshIntegrityPeriod(reportControl);
    refreshTriggerOptions(reportControl);

    return rcb;
}

static MmsVariableSpecification*
createBufferedReportControlBlock(ReportControlBlock* reportControlBlock,
        ReportControl* reportControl)
{
    MmsVariableSpecification* rcb = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    rcb->name = StringUtils_copyString(reportControlBlock->name);
    rcb->type = MMS_STRUCTURE;

    int brcbElementCount = 13;

    if (reportControl->server->edition >= IEC_61850_EDITION_2) {

#if (CONFIG_IEC61850_BRCB_WITH_RESVTMS == 1)
        if (reportControl->server->enableBRCBResvTms)
            brcbElementCount++;
#endif

        if (reportControl->hasOwner)
            brcbElementCount++;
    }

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

    if (reportControl->server->edition >= IEC_61850_EDITION_2) {
        int currentIndex = 13;

#if (CONFIG_IEC61850_BRCB_WITH_RESVTMS == 1)
        int resvTmsIndex = currentIndex;

        if (reportControl->server->enableBRCBResvTms) {
            namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
            namedVariable->name = StringUtils_copyString("ResvTms");
            namedVariable->type = MMS_INTEGER;
            namedVariable->typeSpec.integer = 16;
            rcb->typeSpec.structure.elements[currentIndex] = namedVariable;
            mmsValue->value.structure.components[currentIndex] = MmsValue_newInteger(16);
            currentIndex++;
        }
#endif /* (CONFIG_IEC61850_BRCB_WITH_RESVTMS == 1) */

        if (reportControl->hasOwner) {
            namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
            namedVariable->name = StringUtils_copyString("Owner");
            namedVariable->type = MMS_OCTET_STRING;
            namedVariable->typeSpec.octetString = -64;
            rcb->typeSpec.structure.elements[currentIndex] = namedVariable;
            mmsValue->value.structure.components[currentIndex] = MmsValue_newOctetString(0, 16); /* size 16 is enough to store client IPv6 address */

            /* initialize pre configured owner */
            if (reportControlBlock->clientReservation[0] == 4) {
                reportControl->resvTms = -1;
                MmsValue_setOctetString(mmsValue->value.structure.components[currentIndex], reportControlBlock->clientReservation + 1, 4);
            }
            else if (reportControlBlock->clientReservation[0] == 6) {
                reportControl->resvTms = -1;
                MmsValue_setOctetString(mmsValue->value.structure.components[currentIndex], reportControlBlock->clientReservation + 1, 16);
            }
        }

#if (CONFIG_IEC61850_BRCB_WITH_RESVTMS == 1)
        if (reportControl->server->enableBRCBResvTms) {
            MmsValue_setInt16(mmsValue->value.structure.components[resvTmsIndex], reportControl->resvTms);
        }
#endif
    }

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
        ReportControl* rc = ReportControl_create(true, logicalNode, self->iedServer->reportBufferSizeBRCBs, self->iedServer);

        rc->domain = domain;

        ReportControlBlock* reportControlBlock = getRCBForLogicalNodeWithIndex(
                self, logicalNode, currentReport, true);

        rc->hasOwner = self->iedServer->enableOwnerForRCB;

        rc->name = StringUtils_createString(3, logicalNode->name, "$BR$",
                reportControlBlock->name);

        rc->rcb = reportControlBlock;

        namedVariable->typeSpec.structure.elements[currentReport] =
                createBufferedReportControlBlock(reportControlBlock, rc);

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
        ReportControl* rc = ReportControl_create(false, logicalNode, self->iedServer->reportBufferSizeURCBs, self->iedServer);

        rc->domain = domain;

        ReportControlBlock* reportControlBlock = getRCBForLogicalNodeWithIndex(
                self, logicalNode, currentReport, false);

        rc->hasOwner = self->iedServer->enableOwnerForRCB;

        rc->name = StringUtils_createString(3, logicalNode->name, "$RP$",
                reportControlBlock->name);

        rc->rcb = reportControlBlock;

        namedVariable->typeSpec.structure.elements[currentReport] =
                createUnbufferedReportControlBlock(reportControlBlock, rc);

        LinkedList_add(self->reportControls, rc);

        currentReport++;
    }

    return namedVariable;
}

static bool
convertIPv4AddressStringToByteArray(const char* clientAddressString, uint8_t ipV4Addr[])
{
    int addrElementCount = 0;

    const char* separator = clientAddressString;

    while (separator != NULL && addrElementCount < 4) {
        int intVal = atoi(separator);

        ipV4Addr[addrElementCount] = intVal;

        separator = strchr(separator, '.');

        if (separator != NULL)
            separator++; /* skip '.' character */

        addrElementCount ++;
    }

    if (addrElementCount == 4)
        return true;
    else
        return false;
}

static void
updateOwner(ReportControl* rc, MmsServerConnection connection)
{
    rc->clientConnection = connection;

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_wait(rc->rcbValuesLock);
#endif

    if (rc->server->edition >= IEC_61850_EDITION_2 && rc->hasOwner) {

        MmsValue* owner = ReportControl_getRCBValue(rc, "Owner");

        if (owner != NULL) {

            if (connection != NULL) {
                char* clientAddressString = MmsServerConnection_getClientAddress(connection);

                if (DEBUG_IED_SERVER) printf("IED_SERVER: reporting.c: set owner to %s\n", clientAddressString);

                if (strchr(clientAddressString, '.') != NULL) {
                    if (DEBUG_IED_SERVER)
                        printf("IED_SERVER: reporting.c: client address is IPv4 address\n");

                    uint8_t ipV4Addr[4];

                    bool valid = convertIPv4AddressStringToByteArray(clientAddressString, ipV4Addr);

                    if (valid)
                        MmsValue_setOctetString(owner, ipV4Addr, 4);
                    else
                        MmsValue_setOctetString(owner, ipV4Addr, 0);

                }
                else {
                    uint8_t ipV6Addr[16];

                    bool valid = StringUtils_convertIPv6AdddressStringToByteArray(clientAddressString, ipV6Addr);

                    if (valid) {
                        if (DEBUG_IED_SERVER)
                            printf("IED_SERVER: reporting.c: client address is IPv6 address\n");

                        MmsValue_setOctetString(owner, ipV6Addr, 16);
                    }
                    else {
                        if (DEBUG_IED_SERVER)
                            printf("IED_SERVER: reporting.c: not a valid IPv6 address\n");

                        MmsValue_setOctetString(owner, ipV6Addr, 0);
                    }
                }
            }
            else {
                if (rc->resvTms != -1) {
                    uint8_t emptyAddr[1];
                    MmsValue_setOctetString(owner, emptyAddr, 0);
                }
                else {
                    /* Set to pre-configured owner */
                    if (rc->rcb->clientReservation[0] == 4) {
                        MmsValue_setOctetString(owner, rc->rcb->clientReservation + 1, 4);
                    }
                    else if (rc->rcb->clientReservation[0] == 6) {
                        MmsValue_setOctetString(owner, rc->rcb->clientReservation + 1, 16);
                    }
                }
            }
        }
    }

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_post(rc->rcbValuesLock);
#endif
}

static bool
checkForZeroEntryID(MmsValue* value)
{
    const uint8_t* buffer = MmsValue_getOctetStringBuffer(value);

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
    MmsValue* confRevValue = ReportControl_getRCBValue(self, "ConfRev");

    uint32_t confRev = MmsValue_toUint32(confRevValue);

    confRev++;

    if (confRev == 0)
        confRev = 1;

    MmsValue_setUint32(confRevValue, confRev);
}

static void
checkReservationTimeout(MmsMapping* self, ReportControl* rc)
{
    if (rc->enabled == false && (rc->clientConnection == NULL)) {
        if (rc->reservationTimeout > 0) {
            if (Hal_getTimeInMs() > rc->reservationTimeout) {

                if (rc->resvTms != -1)
                    rc->resvTms = 0;

                if (DEBUG_IED_SERVER)
                    printf("IED_SERVER: reservation timeout expired for %s.%s\n", rc->parentLN->name, rc->name);

#if (CONFIG_IEC61850_BRCB_WITH_RESVTMS == 1)
                if (self->iedServer->enableBRCBResvTms) {
#if (CONFIG_MMS_THREADLESS_STACK != 1)
                    Semaphore_wait(rc->rcbValuesLock);
#endif
                    MmsValue* resvTmsVal = ReportControl_getRCBValue(rc, "ResvTms");
                    if (resvTmsVal)
                        MmsValue_setInt16(resvTmsVal, rc->resvTms);

#if (CONFIG_MMS_THREADLESS_STACK != 1)
                    Semaphore_post(rc->rcbValuesLock);
#endif
                }
#endif

                rc->reservationTimeout = 0;
                updateOwner(rc, NULL);
                rc->reserved = false;

#if (CONFIG_IEC61850_SERVICE_TRACKING == 1)
                copyRCBValuesToTrackingObject(self, rc);
                updateGenericTrackingObjectValues(self, rc, IEC61850_SERVICE_TYPE_INTERNAL_CHANGE, DATA_ACCESS_ERROR_SUCCESS);
#endif /* (CONFIG_IEC61850_SERVICE_TRACKING == 1) */

                if (self->rcbEventHandler) {
                    ClientConnection clientConnection = private_IedServer_getClientConnectionByHandle(self->iedServer, rc->clientConnection);

                    self->rcbEventHandler(self->rcbEventHandlerParameter, rc->rcb, clientConnection, RCB_EVENT_UNRESERVED, NULL, DATA_ACCESS_ERROR_SUCCESS);
                }
            }
        }
    }
}

void
ReportControl_readAccess(ReportControl* rc, MmsMapping* mmsMapping, MmsServerConnection connection, char* elementName)
{
    (void)elementName;

    /* check reservation timeout */
    if (rc->buffered) {
        checkReservationTimeout(mmsMapping, rc);
    }

    if (mmsMapping->rcbEventHandler) {
        ClientConnection clientConnection = private_IedServer_getClientConnectionByHandle(mmsMapping->iedServer, connection);

        mmsMapping->rcbEventHandler(mmsMapping->rcbEventHandlerParameter, rc->rcb, clientConnection, RCB_EVENT_GET_PARAMETER, elementName, DATA_ACCESS_ERROR_SUCCESS);
    }
}

static bool
isIpAddressMatchingWithOwner(ReportControl* rc, const char* ipAddress)
{
    bool retVal = false;

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_wait(rc->rcbValuesLock);
#endif

    MmsValue* owner = ReportControl_getRCBValue(rc, "Owner");

    if (owner != NULL) {

        if (MmsValue_getOctetStringSize(owner) == 0) {
            retVal = true;
            goto exit_function;
        }

        if (strchr(ipAddress, '.') != NULL) {
            uint8_t ipV4Addr[4];

            if (convertIPv4AddressStringToByteArray(ipAddress, ipV4Addr)) {
                if (memcmp(ipV4Addr, MmsValue_getOctetStringBuffer(owner), 4) == 0) {
                    retVal = true;
                    goto exit_function;
                }
            }
        }
        else {
            uint8_t ipV6Addr[16];

            if (StringUtils_convertIPv6AdddressStringToByteArray(ipAddress, ipV6Addr)) {
                if (memcmp(ipV6Addr, MmsValue_getOctetStringBuffer(owner), 16) == 0) {
                    retVal = true;
                    goto exit_function;
                }
            }
            else {
                goto exit_function;
            }
        }
    }

exit_function:

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_post(rc->rcbValuesLock);
#endif

    return retVal;
}

static void
reserveRcb(ReportControl* rc,  MmsServerConnection connection)
{
    rc->reserved = true;
    rc->clientConnection = connection;

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_wait(rc->rcbValuesLock);
#endif

    if (rc->buffered) {
#if (CONFIG_IEC61850_BRCB_WITH_RESVTMS == 1)
        if (rc->server->enableBRCBResvTms) {
            MmsValue* resvTmsVal = ReportControl_getRCBValue(rc, "ResvTms");
            if (resvTmsVal)
                MmsValue_setInt16(resvTmsVal, rc->resvTms);
        }
#endif
    }
    else {
        MmsValue* resvVal = ReportControl_getRCBValue(rc, "Resv");
        if (resvVal)
            MmsValue_setBoolean(resvVal, true);
    }

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_post(rc->rcbValuesLock);
#endif

    updateOwner(rc, connection);
}

MmsDataAccessError
Reporting_RCBWriteAccessHandler(MmsMapping* self, ReportControl* rc, char* elementName, MmsValue* value,
        MmsServerConnection connection)
{
    MmsDataAccessError retVal = DATA_ACCESS_ERROR_SUCCESS;

    bool resvTmsAccess = false; /* access is to RecvTms or Resv */
    bool dontUpdate = false;

    ClientConnection clientConnection = private_IedServer_getClientConnectionByHandle(self->iedServer, connection);

    /* check reservation timeout for buffered RCBs */
    if (rc->buffered) {

        checkReservationTimeout(self, rc);

        if (rc->resvTms == 0) {
            /* nothing to to */
        }
        else if (rc->resvTms == -1) {

            if (rc->reserved == false) {

                if (self->iedServer->edition < IEC_61850_EDITION_2_1) {

#if (CONFIG_IEC61850_RCB_ALLOW_ONLY_PRECONFIGURED_CLIENT == 1)
                    if (isIpAddressMatchingWithOwner(rc, MmsServerConnection_getClientAddress(connection))) {
                        rc->reserved = true;
                        rc->clientConnection = connection;

                        if (self->rcbEventHandler) {
                             self->rcbEventHandler(self->rcbEventHandlerParameter, rc->rcb, clientConnection, RCB_EVENT_RESERVED, NULL, DATA_ACCESS_ERROR_SUCCESS);
                        }
                    }
                    else {
                        if (DEBUG_IED_SERVER)
                            printf("IED_SERVER: client IP not matching with pre-assigned owner\n");
                    }
#else
                    rc->reserved = true;
                    rc->clientConnection = connection;

                    if (self->rcbEventHandler) {
                         self->rcbEventHandler(self->rcbEventHandlerParameter, rc->rcb, clientConnection, RCB_EVENT_RESERVED, NULL, DATA_ACCESS_ERROR_SUCCESS);
                    }
#endif

                }
            }
        }
        else if (rc->resvTms > 0) {
            if (rc->reserved == false) {

                if (isIpAddressMatchingWithOwner(rc, MmsServerConnection_getClientAddress(connection))) {
                    rc->reserved = true;
                    rc->clientConnection = connection;
                    rc->reservationTimeout = Hal_getTimeInMs() + (rc->resvTms * 1000);

                    if (self->rcbEventHandler) {
                         self->rcbEventHandler(self->rcbEventHandlerParameter, rc->rcb, clientConnection, RCB_EVENT_RESERVED, NULL, DATA_ACCESS_ERROR_SUCCESS);
                    }
                }
                else {
                    if (DEBUG_IED_SERVER)
                        printf("IED_SERVER: client IP not matching with owner\n");
                }

            }
            else {
                if (rc->clientConnection == connection) {
                    rc->reservationTimeout = Hal_getTimeInMs() + (rc->resvTms * 1000);
                }
            }
        }
    }

    ReportControl_lockNotify(rc);

    /* for edition 2.1 don't allow implicit RCB reservation */
    if (self->iedServer->edition == IEC_61850_EDITION_2_1) {
        if (rc->reserved == false) {
            if ((strcmp(elementName, "Resv")) && (strcmp(elementName, "ResvTms"))) {
                retVal = DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED;

                goto exit_function;
            }
        }
    }

    if ((rc->reserved) && (rc->clientConnection != connection)) {
        retVal = DATA_ACCESS_ERROR_TEMPORARILY_UNAVAILABLE;

        goto exit_function;
    }

#if (CONFIG_IEC61850_RCB_ALLOW_ONLY_PRECONFIGURED_CLIENT == 1)
    if (rc->resvTms == -1) {
        if (isIpAddressMatchingWithOwner(rc, MmsServerConnection_getClientAddress(connection)) == false) {
            retVal = DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED;

            if (DEBUG_IED_SERVER)
                printf("IED_SERVER: client IP not matching with pre-assigned owner --> write access denied!\n");

            goto exit_function;
        }
    }
#endif

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

                if (rc->reserved == false) {

                    rc->resvTms = RESV_TMS_IMPLICIT_VALUE;

                    reserveRcb(rc, connection);

                    if (self->rcbEventHandler) {
                        self->rcbEventHandler(self->rcbEventHandlerParameter, rc->rcb, clientConnection, RCB_EVENT_RESERVED, NULL, DATA_ACCESS_ERROR_SUCCESS);
                    }
                }

                updateOwner(rc, connection);

#if (CONFIG_MMS_THREADLESS_STACK != 1)
                Semaphore_wait(rc->rcbValuesLock);
#endif

                MmsValue* rptEna = ReportControl_getRCBValue(rc, "RptEna");
                MmsValue_update(rptEna, value);

#if (CONFIG_MMS_THREADLESS_STACK != 1)
                Semaphore_post(rc->rcbValuesLock);
#endif

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

#if (CONFIG_MMS_THREADLESS_STACK != 1)
                Semaphore_wait(rc->rcbValuesLock);
#endif

                MmsValue* sqNum = ReportControl_getRCBValue(rc, "SqNum");

                MmsValue_setUint32(sqNum, 0U);

#if (CONFIG_MMS_THREADLESS_STACK != 1)
                Semaphore_post(rc->rcbValuesLock);
#endif

                retVal = DATA_ACCESS_ERROR_SUCCESS;

                if (self->rcbEventHandler) {
                     self->rcbEventHandler(self->rcbEventHandlerParameter, rc->rcb, clientConnection, RCB_EVENT_ENABLE, NULL, DATA_ACCESS_ERROR_SUCCESS);
                }

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
                if (rc->dataSet)
                    clearInclusionFlags(rc);

                /* clear report buffer */
                purgeBuf(rc);

                if (self->rcbEventHandler) {
                     self->rcbEventHandler(self->rcbEventHandlerParameter, rc->rcb, clientConnection, RCB_EVENT_PURGEBUF, NULL, DATA_ACCESS_ERROR_SUCCESS);
                }
            }

            if (self->rcbEventHandler) {
                 self->rcbEventHandler(self->rcbEventHandlerParameter, rc->rcb, clientConnection, RCB_EVENT_DISABLE, NULL, DATA_ACCESS_ERROR_SUCCESS);
            }

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

            if (self->rcbEventHandler) {
                 self->rcbEventHandler(self->rcbEventHandlerParameter, rc->rcb, clientConnection, RCB_EVENT_GI, NULL, DATA_ACCESS_ERROR_SUCCESS);
            }

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
            resvTmsAccess = true;

            rc->reserved = value->value.boolean;

            if (rc->reserved == true) {

                if (self->rcbEventHandler) {
                    self->rcbEventHandler(self->rcbEventHandlerParameter, rc->rcb, clientConnection, RCB_EVENT_RESERVED, NULL, DATA_ACCESS_ERROR_SUCCESS);
                }

                updateOwner(rc, connection);
                rc->clientConnection = connection;
            }
            else {
                updateOwner(rc, NULL);
                rc->clientConnection = NULL;

                if (rc->resvTms == -1)
                    dontUpdate = true;

                if (self->rcbEventHandler) {
                    self->rcbEventHandler(self->rcbEventHandlerParameter, rc->rcb, clientConnection, RCB_EVENT_UNRESERVED, NULL, DATA_ACCESS_ERROR_SUCCESS);
                }
            }

        }
        else if (strcmp(elementName, "PurgeBuf") == 0) {
            if (MmsValue_getType(value) == MMS_BOOLEAN) {

                if (MmsValue_getBoolean(value) == true) {
                    purgeBuf(rc);
                    retVal = DATA_ACCESS_ERROR_SUCCESS;

                    if (self->rcbEventHandler) {
                        self->rcbEventHandler(self->rcbEventHandlerParameter, rc->rcb, clientConnection, RCB_EVENT_PURGEBUF, NULL, DATA_ACCESS_ERROR_SUCCESS);
                    }

                    goto exit_function;
                }
            }

        }
        else if (strcmp(elementName, "DatSet") == 0) {

            if (!(self->iedServer->rcbSettingsWritable & IEC61850_REPORTSETTINGS_DATSET))
            {
                retVal = DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED;
                goto exit_function;
            }

#if (CONFIG_MMS_THREADLESS_STACK != 1)
            Semaphore_wait(rc->rcbValuesLock);
#endif

            MmsValue* datSet = ReportControl_getRCBValue(rc, "DatSet");

            if (!MmsValue_equals(datSet, value)) {

                if (updateReportDataset(self, rc, value, connection)) {

                    MmsValue_update(datSet, value);

                    increaseConfRev(rc);

#if (CONFIG_MMS_THREADLESS_STACK != 1)
                    Semaphore_post(rc->rcbValuesLock);
#endif

                    if (rc->buffered) {
                        purgeBuf(rc);

                        if (self->rcbEventHandler) {
                            self->rcbEventHandler(self->rcbEventHandlerParameter, rc->rcb, clientConnection, RCB_EVENT_PURGEBUF, NULL, DATA_ACCESS_ERROR_SUCCESS);
                        }
                    }
                }
                else {
#if (CONFIG_MMS_THREADLESS_STACK != 1)
                    Semaphore_post(rc->rcbValuesLock);
#endif

                    retVal = DATA_ACCESS_ERROR_OBJECT_VALUE_INVALID;

                    goto exit_function;
                }
            }
            else {
#if (CONFIG_MMS_THREADLESS_STACK != 1)
                Semaphore_post(rc->rcbValuesLock);
#endif
            }

            retVal = DATA_ACCESS_ERROR_SUCCESS;
            goto exit_function;
        }
        else if (strcmp(elementName, "IntgPd") == 0) {

            if (!(self->iedServer->rcbSettingsWritable & IEC61850_REPORTSETTINGS_INTG_PD))
            {
                retVal = DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED;
                goto exit_function;
            }

#if (CONFIG_MMS_THREADLESS_STACK != 1)
            Semaphore_wait(rc->rcbValuesLock);
#endif

            MmsValue* intgPd = ReportControl_getRCBValue(rc, elementName);

            if (!MmsValue_equals(intgPd, value)) {
                MmsValue_update(intgPd, value);

#if (CONFIG_MMS_THREADLESS_STACK != 1)
                Semaphore_post(rc->rcbValuesLock);
#endif

                refreshIntegrityPeriod(rc);

                if (rc->buffered) {

                    if (rc->triggerOps & TRG_OPT_INTEGRITY) {

                        if (rc->intgPd > 0) {

                            if (rc->server->syncIntegrityReportTimes) {
                                rc->nextIntgReportTime = getNextRoundedStartTime(Hal_getTimeInMs(), rc->intgPd);
                            }
                            else {
                                rc->nextIntgReportTime = 0;
                            }
                        }
                    }

                    purgeBuf(rc);

                    if (self->rcbEventHandler) {
                        self->rcbEventHandler(self->rcbEventHandlerParameter, rc->rcb, clientConnection, RCB_EVENT_PURGEBUF, NULL, DATA_ACCESS_ERROR_SUCCESS);
                    }
                }
            }
            else {
#if (CONFIG_MMS_THREADLESS_STACK != 1)
                Semaphore_post(rc->rcbValuesLock);
#endif
            }

            goto exit_function;
        }
        else if (strcmp(elementName, "TrgOps") == 0) {

            if (!(self->iedServer->rcbSettingsWritable & IEC61850_REPORTSETTINGS_TRG_OPS))
            {
                retVal = DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED;
                goto exit_function;
            }

#if (CONFIG_MMS_THREADLESS_STACK != 1)
            Semaphore_wait(rc->rcbValuesLock);
#endif

            MmsValue* trgOps = ReportControl_getRCBValue(rc, elementName);

            if (!MmsValue_equals(trgOps, value)) {
                MmsValue_update(trgOps, value);

#if (CONFIG_MMS_THREADLESS_STACK != 1)
                Semaphore_post(rc->rcbValuesLock);
#endif

                if (rc->buffered) {
                    purgeBuf(rc);

                    if (self->rcbEventHandler) {
                        self->rcbEventHandler(self->rcbEventHandlerParameter, rc->rcb, clientConnection, RCB_EVENT_PURGEBUF, NULL, DATA_ACCESS_ERROR_SUCCESS);
                    }
                }

                refreshTriggerOptions(rc);
            }
            else {
#if (CONFIG_MMS_THREADLESS_STACK != 1)
                Semaphore_post(rc->rcbValuesLock);
#endif
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

#if (CONFIG_MMS_THREADLESS_STACK != 1)
            Semaphore_wait(rc->rcbValuesLock);
#endif
            MmsValue* entryID = ReportControl_getRCBValue(rc, elementName);
            MmsValue_update(entryID, value);

#if (CONFIG_MMS_THREADLESS_STACK != 1)
            Semaphore_post(rc->rcbValuesLock);
#endif

            goto exit_function;
        }

        else if (strcmp(elementName, "BufTm") == 0) {

            if (!(self->iedServer->rcbSettingsWritable & IEC61850_REPORTSETTINGS_BUF_TIME))
            {
                retVal = DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED;
                goto exit_function;
            }

#if (CONFIG_MMS_THREADLESS_STACK != 1)
            Semaphore_wait(rc->rcbValuesLock);
#endif

            MmsValue* bufTm = ReportControl_getRCBValue(rc, elementName);

            if (!MmsValue_equals(bufTm, value)) {
                MmsValue_update(bufTm, value);

#if (CONFIG_MMS_THREADLESS_STACK != 1)
                Semaphore_post(rc->rcbValuesLock);
#endif

                if (rc->buffered) {
                    purgeBuf(rc);

                    if (self->rcbEventHandler) {
                        self->rcbEventHandler(self->rcbEventHandlerParameter, rc->rcb, clientConnection, RCB_EVENT_PURGEBUF, NULL, DATA_ACCESS_ERROR_SUCCESS);
                    }
                }

                refreshBufferTime(rc);
            }
            else {
#if (CONFIG_MMS_THREADLESS_STACK != 1)
                Semaphore_post(rc->rcbValuesLock);
#endif
            }

            goto exit_function;
        }
        else if (strcmp(elementName, "RptID") == 0) {

            if (!(self->iedServer->rcbSettingsWritable & IEC61850_REPORTSETTINGS_RPT_ID))
            {
                retVal = DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED;
                goto exit_function;
            }

#if (CONFIG_MMS_THREADLESS_STACK != 1)
            Semaphore_wait(rc->rcbValuesLock);
#endif

            MmsValue* rptId = ReportControl_getRCBValue(rc, elementName);

            if (!MmsValue_equals(rptId, value)) {
                MmsValue_update(rptId, value);

#if (CONFIG_MMS_THREADLESS_STACK != 1)
                Semaphore_post(rc->rcbValuesLock);
#endif

                if (rc->buffered) {
                    purgeBuf(rc);

                    if (self->rcbEventHandler) {
                        self->rcbEventHandler(self->rcbEventHandlerParameter, rc->rcb, clientConnection, RCB_EVENT_PURGEBUF, NULL, DATA_ACCESS_ERROR_SUCCESS);
                    }
                }
            }
            else {
#if (CONFIG_MMS_THREADLESS_STACK != 1)
                Semaphore_post(rc->rcbValuesLock);
#endif
            }

            goto exit_function;
        }
        else if (strcmp(elementName, "ResvTms") == 0) {
            if (rc->buffered) {

                resvTmsAccess = true;

                if (rc->resvTms != -1) {

                    int resvTms = MmsValue_toInt32(value);

                    if (resvTms >= 0) {
                        rc->resvTms = resvTms;

                        if (rc->resvTms == 0) {
                            rc->reservationTimeout = 0;
                            rc->reserved = false;
                            updateOwner(rc, NULL);

                            if (self->rcbEventHandler) {
                                self->rcbEventHandler(self->rcbEventHandlerParameter, rc->rcb, clientConnection, RCB_EVENT_UNRESERVED, NULL, DATA_ACCESS_ERROR_SUCCESS);
                            }
                        }
                        else {
                            rc->reservationTimeout = Hal_getTimeInMs() + (rc->resvTms * 1000);

                            reserveRcb(rc, connection);

                            if (self->rcbEventHandler) {
                                self->rcbEventHandler(self->rcbEventHandlerParameter, rc->rcb, clientConnection, RCB_EVENT_RESERVED, NULL, DATA_ACCESS_ERROR_SUCCESS);
                            }
                        }

                        MmsValue* resvTmsVal = ReportControl_getRCBValue(rc, "ResvTms");

                        if (resvTmsVal != NULL)
                            MmsValue_update(resvTmsVal, value);
                    }
                    else {
                        retVal = DATA_ACCESS_ERROR_OBJECT_VALUE_INVALID;
                    }
                }
                else {
                    if (self->iedServer->edition < IEC_61850_EDITION_2_1) {
                        retVal = DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED;
                    }
                    else {
                        rc->reservationTimeout = Hal_getTimeInMs() + (RESV_TMS_IMPLICIT_VALUE * 1000);

                        reserveRcb(rc, connection);

                        if (self->rcbEventHandler) {
                            self->rcbEventHandler(self->rcbEventHandlerParameter, rc->rcb, clientConnection, RCB_EVENT_RESERVED, NULL, DATA_ACCESS_ERROR_SUCCESS);
                        }
                    }
                }

                goto exit_function;
            }
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
        else if (strcmp(elementName, "OptFlds") == 0) {

            if (!(self->iedServer->rcbSettingsWritable & IEC61850_REPORTSETTINGS_OPT_FIELDS))
            {
                retVal = DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED;
                goto exit_function;
            }
        }

#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_wait(rc->rcbValuesLock);
#endif

        MmsValue* rcbValue = ReportControl_getRCBValue(rc, elementName);

        if (rcbValue) {
            if (dontUpdate == false) {
                MmsValue_update(rcbValue, value);
            }

#if (CONFIG_MMS_THREADLESS_STACK != 1)
            Semaphore_post(rc->rcbValuesLock);
#endif
        }
        else {
#if (CONFIG_MMS_THREADLESS_STACK != 1)
            Semaphore_post(rc->rcbValuesLock);
#endif

            retVal = DATA_ACCESS_ERROR_OBJECT_VALUE_INVALID;

            goto exit_function;
        }

    }
    else {
        retVal = DATA_ACCESS_ERROR_TEMPORARILY_UNAVAILABLE;
    }

exit_function:

    /* every successful write access reserves the RCB (or for edition 2.1 it means that there is already a reservation) */
    if ((retVal == DATA_ACCESS_ERROR_SUCCESS) && (resvTmsAccess == false)) {
        if (rc->buffered) {
            rc->reservationTimeout = Hal_getTimeInMs() + (RESV_TMS_IMPLICIT_VALUE * 1000);

            if (rc->resvTms == 0) {

                if (rc->reserved == false) {
                    rc->resvTms = RESV_TMS_IMPLICIT_VALUE;

                    reserveRcb(rc, connection);

                    if (self->rcbEventHandler) {
                        self->rcbEventHandler(self->rcbEventHandlerParameter, rc->rcb, clientConnection, RCB_EVENT_RESERVED, NULL, DATA_ACCESS_ERROR_SUCCESS);
                    }
                }

            }
            else if (rc->resvTms == -1) {
                if (rc->reserved == false) {
                    reserveRcb(rc, connection);

                    if (self->rcbEventHandler) {
                        self->rcbEventHandler(self->rcbEventHandlerParameter, rc->rcb, clientConnection, RCB_EVENT_RESERVED, NULL, DATA_ACCESS_ERROR_SUCCESS);
                    }
                }
            }
        }
        else {
            if (rc->reserved == false) {
                reserveRcb(rc, connection);

                if (self->rcbEventHandler) {
                    self->rcbEventHandler(self->rcbEventHandlerParameter, rc->rcb, clientConnection, RCB_EVENT_RESERVED, NULL, DATA_ACCESS_ERROR_SUCCESS);
                }
            }
        }
    }

#if (CONFIG_IEC61850_SERVICE_TRACKING == 1)

    copyRCBValuesToTrackingObject(self, rc);
    updateSingleTrackingValue(self, rc, elementName, value);

#endif /* (CONFIG_IEC61850_SERVICE_TRACKING == 1) */

    ReportControl_unlockNotify(rc);

#if (CONFIG_IEC61850_SERVICE_TRACKING == 1)
    if (rc->buffered)
        updateGenericTrackingObjectValues(self, rc, IEC61850_SERVICE_TYPE_SET_BRCB_VALUES, retVal);
    else
        updateGenericTrackingObjectValues(self, rc, IEC61850_SERVICE_TYPE_SET_URCB_VALUES, retVal);
#endif /* (CONFIG_IEC61850_SERVICE_TRACKING == 1) */

    if (self->rcbEventHandler) {
        self->rcbEventHandler(self->rcbEventHandlerParameter, rc->rcb, clientConnection, RCB_EVENT_SET_PARAMETER, elementName, retVal);
    }

    return retVal;
}

static void
Reporting_disableReportControlInstance(MmsMapping* self, ReportControl* rc)
{
    if (rc->enabled) {
        if (self->rcbEventHandler) {
            ClientConnection clientConnection = private_IedServer_getClientConnectionByHandle(self->iedServer, rc->clientConnection);

            self->rcbEventHandler(self->rcbEventHandlerParameter, rc->rcb, clientConnection, RCB_EVENT_DISABLE, NULL, DATA_ACCESS_ERROR_SUCCESS);
        }
    }

    rc->enabled = false;
    rc->clientConnection = NULL;

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_wait(rc->rcbValuesLock);
#endif

    MmsValue* rptEna = ReportControl_getRCBValue(rc, "RptEna");
    MmsValue_setBoolean(rptEna, false);

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_post(rc->rcbValuesLock);
#endif

    if (rc->reserved) {
        rc->reserved = false;

        if (self->rcbEventHandler) {
            ClientConnection clientConnection = private_IedServer_getClientConnectionByHandle(self->iedServer, rc->clientConnection);

            self->rcbEventHandler(self->rcbEventHandlerParameter, rc->rcb, clientConnection, RCB_EVENT_UNRESERVED, NULL, DATA_ACCESS_ERROR_SUCCESS);
        }
    }

    if (rc->buffered == false) {

        if (rc->resvTms != -1) {
#if (CONFIG_MMS_THREADLESS_STACK != 1)
            Semaphore_wait(rc->rcbValuesLock);
#endif

            MmsValue* resv = ReportControl_getRCBValue(rc, "Resv");
            MmsValue_setBoolean(resv, false);

#if (CONFIG_MMS_THREADLESS_STACK != 1)
            Semaphore_post(rc->rcbValuesLock);
#endif
        }

        if (rc->resvTms != -1)
            updateOwner(rc, NULL);

        /* delete buffer content */
        purgeBuf(rc);
    }
    else {
        if (rc->resvTms == 0)
            updateOwner(rc, NULL);
        else if (rc->resvTms > 0) {
            rc->reservationTimeout = Hal_getTimeInMs() + (rc->resvTms * 1000);
        }
    }

#if (CONFIG_IEC61850_SERVICE_TRACKING == 1)
    copyRCBValuesToTrackingObject(self, rc);
    updateGenericTrackingObjectValues(self, rc, IEC61850_SERVICE_TYPE_INTERNAL_CHANGE, DATA_ACCESS_ERROR_SUCCESS);
#endif /* (CONFIG_IEC61850_SERVICE_TRACKING == 1) */
}

void
Reporting_deactivateAllReports(MmsMapping* self)
{
    LinkedList rcElem = LinkedList_getNext(self->reportControls);

    while (rcElem) {
        ReportControl* rc = (ReportControl*)LinkedList_getData(rcElem);

        Reporting_disableReportControlInstance(self, rc);

        rcElem = LinkedList_getNext(rcElem);
    }
}

void
Reporting_deactivateReportsForConnection(MmsMapping* self, MmsServerConnection connection)
{
    LinkedList rcElem = LinkedList_getNext(self->reportControls);

    while (rcElem) {
        ReportControl* rc = (ReportControl*)LinkedList_getData(rcElem);

        if (rc->clientConnection == connection) {
            Reporting_disableReportControlInstance(self, rc);
        }

        rcElem = LinkedList_getNext(rcElem);
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
                if (lastReport != NULL)
                    lastReport->next = currentReport->next;
                else
                    lastReport = currentReport;
            }

#if (DEBUG_IED_SERVER == 1)
                printf("IED_SERVER:   REMOVE old GI report with ID ");
                printReportId(currentReport);
                printf("\n");
#endif

            reportBuffer->reportsCount--;

            if (reportBuffer->nextToTransmit == currentReport)
                reportBuffer->nextToTransmit = currentReport->next;

            if (reportBuffer->lastEnqueuedReport == currentReport) {
                if (lastReport != NULL)
                    reportBuffer->lastEnqueuedReport = lastReport;
                else
                    reportBuffer->lastEnqueuedReport = reportBuffer->oldestReport;
            }
        }
        else {
            lastReport = currentReport;
        }

        currentReport = currentReport->next;
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

    ReportBuffer* buffer = reportControl->reportBuffer;

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_wait(buffer->lock);
#endif

    bool isBuffered = reportControl->buffered;
    bool overflow = false;

    updateTimeOfEntry(reportControl, Hal_getTimeInMs());

    int inclusionBitStringSize = MmsValue_getBitStringSize(reportControl->inclusionField);

    /* calculate size of complete buffer entry */
    int bufferEntrySize = MemoryAllocator_getAlignedSize(sizeof(ReportBufferEntry));

    int inclusionFieldSize = MemoryAllocator_getAlignedSize(MmsValue_getBitStringByteSize(reportControl->inclusionField));

    MmsValue inclusionFieldStatic;

    inclusionFieldStatic.type = MMS_BIT_STRING;
    inclusionFieldStatic.value.bitString.size = inclusionBitStringSize;

    MmsValue* inclusionField = &inclusionFieldStatic;

    int dataBlockSize = 0;

    if (isIntegrity || isGI) {

        DataSetEntry* dataSetEntry = reportControl->dataSet->fcdas;

        int i;

        for (i = 0; i < inclusionBitStringSize; i++) {
            /* don't need reason for inclusion in GI or integrity report */

            int encodedSize;

            if (dataSetEntry->value) {
                encodedSize = MmsValue_encodeMmsData(dataSetEntry->value, NULL, 0, false);
            }
            else {
                MmsValue _errVal;
                _errVal.type = MMS_DATA_ACCESS_ERROR;
                _errVal.value.dataAccessError = DATA_ACCESS_ERROR_OBJECT_VALUE_INVALID;

                encodedSize = MmsValue_encodeMmsData(&_errVal, NULL, 0, false);
            }

            dataBlockSize += encodedSize;

            dataSetEntry = dataSetEntry->sibling;
        }

        bufferEntrySize += MemoryAllocator_getAlignedSize(sizeof(int) + dataBlockSize); /* add aligned_size(LEN + DATA) */
    }
    else { /* other trigger reason */
        bufferEntrySize += inclusionFieldSize;

        int reasonForInclusionSize = 0;

        int i;

        for (i = 0; i < inclusionBitStringSize; i++) {

            if (reportControl->inclusionFlags[i] != REPORT_CONTROL_NONE) {

                reasonForInclusionSize++;

                assert(reportControl->bufferedDataSetValues[i] != NULL);

                int encodedSize = MmsValue_encodeMmsData(reportControl->bufferedDataSetValues[i], NULL, 0, false);

                dataBlockSize += encodedSize;
            }
        }

        bufferEntrySize += MemoryAllocator_getAlignedSize(sizeof(int) + dataBlockSize + reasonForInclusionSize); /* add aligned_size (LEN + DATA + REASON) */
    }

    if (DEBUG_IED_SERVER)
        printf("IED_SERVER: enqueueReport: bufferEntrySize: %i\n", bufferEntrySize);

    if (bufferEntrySize > buffer->memoryBlockSize) {
        if (DEBUG_IED_SERVER)
            printf("IED_SERVER: enqueueReport: report buffer too small for report entry! Skip event!\n");

        goto exit_function;
    }

    if (isBuffered) {
        /* remove old buffered GI reports */
        if (isGI) removeAllGIReportsFromReportBuffer(buffer);
    }

    uint8_t* entryBufPos = NULL;
    uint8_t* entryStartPos;

    if (DEBUG_IED_SERVER)
        printf("IED_SERVER: number of reports in report buffer: %i\n", buffer->reportsCount);

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
                        overflow = true;
                    }

#if (DEBUG_IED_SERVER == 1)
                    printf("IED_SERVER: REMOVE report with ID ");
                    printReportId(buffer->oldestReport);
                    printf(" (index: %i, size: %i)\n", (int)((uint8_t*)(buffer->oldestReport) - (uint8_t*)(buffer->memoryBlock)), buffer->oldestReport->entryLength);
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
                        overflow = true;
                    }

#if (DEBUG_IED_SERVER == 1)
                    printf("IED_SERVER: REMOVE[1] report with ID ");
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
                        overflow = true;
                    }

#if (DEBUG_IED_SERVER == 1)
                    printf("IED_SERVER: REMOVE[2] report with ID ");
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
                        overflow = true;
                    }

#if (DEBUG_IED_SERVER == 1)
                    printf("IED_SERVER: REMOVE[3] report with ID ");
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

    entry->timeOfEntry = timeOfEntry;

    if (isBuffered) {
        /* ENTRY_ID is set to system time in ms! */
        uint64_t entryId = timeOfEntry;

        if (entryId <= reportControl->lastEntryId)
            entryId = reportControl->lastEntryId + 1;

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
#if (CONFIG_MMS_THREADLESS_STACK != 1)
            Semaphore_wait(reportControl->rcbValuesLock);
#endif

            MmsValue* entryIdValue = MmsValue_getElement(reportControl->rcbValues, 11);
            MmsValue_setOctetString(entryIdValue, (uint8_t*) entry->entryId, 8);

#if (CONFIG_MMS_THREADLESS_STACK != 1)
            Semaphore_post(reportControl->rcbValuesLock);
#endif
        }

        reportControl->lastEntryId = entryId;
    }

    if (isIntegrity)
        entry->flags = 1;
    else if (isGI)
        entry->flags = 2;
    else
        entry->flags = 0;

    entry->entryLength = bufferEntrySize;

    entryBufPos += MemoryAllocator_getAlignedSize(sizeof(ReportBufferEntry));

    if (isIntegrity || isGI) {
        DataSetEntry* dataSetEntry = reportControl->dataSet->fcdas;

        /* encode LEN */
        memcpy(entryBufPos, (uint8_t*)(&dataBlockSize), sizeof(int));
        entryBufPos += sizeof(int);

        /* encode DATA */
        int i;

        for (i = 0; i < inclusionBitStringSize; i++) {

            if (dataSetEntry->value) {
                entryBufPos += MmsValue_encodeMmsData(dataSetEntry->value, entryBufPos, 0, true);
            }
            else {
                MmsValue _errVal;
                _errVal.type = MMS_DATA_ACCESS_ERROR;
                _errVal.value.dataAccessError = DATA_ACCESS_ERROR_OBJECT_VALUE_INVALID;

                entryBufPos += MmsValue_encodeMmsData(&_errVal, entryBufPos, 0, true);
            }

            dataSetEntry = dataSetEntry->sibling;
        }

    }
    else {
        /* encode inclusion bit string */
        inclusionFieldStatic.value.bitString.buf = entryBufPos;
        memset(entryBufPos, 0, inclusionFieldSize);
        entryBufPos += inclusionFieldSize;

        /* encode LEN */
        memcpy(entryBufPos, (uint8_t*)(&dataBlockSize), sizeof(int));
        entryBufPos += sizeof(int);

        /* encode DATA */
        int i;

        for (i = 0; i < inclusionBitStringSize; i++) {

            if (reportControl->inclusionFlags[i] != REPORT_CONTROL_NONE) {

                /* update inclusion bit string for report entry */
                MmsValue_setBitStringBit(inclusionField, i, true);

                assert(reportControl->bufferedDataSetValues[i] != NULL);

                entryBufPos += MmsValue_encodeMmsData(reportControl->bufferedDataSetValues[i], entryBufPos, 0, true);
            }

        }

        /* encode REASON */
        for (i = 0; i < inclusionBitStringSize; i++) {

            if (reportControl->inclusionFlags[i] != REPORT_CONTROL_NONE) {
                *entryBufPos = (uint8_t) reportControl->inclusionFlags[i];
                entryBufPos ++;
            }
        }
    }

    clearInclusionFlags(reportControl);

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

exit_function:

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_post(buffer->lock);
#endif

    if (reportControl->server) {
        MmsMapping* mmsMapping = reportControl->server->mmsMapping;

        if (mmsMapping->rcbEventHandler) {
            if (overflow) {
                mmsMapping->rcbEventHandler(mmsMapping->rcbEventHandlerParameter, reportControl->rcb, NULL, RCB_EVENT_OVERFLOW, NULL, DATA_ACCESS_ERROR_SUCCESS);
            }

            mmsMapping->rcbEventHandler(mmsMapping->rcbEventHandlerParameter, reportControl->rcb, NULL, RCB_EVENT_REPORT_CREATED, NULL, DATA_ACCESS_ERROR_SUCCESS);
        }
    }

    return;
} /* enqueuReport() */

#define SENT_REPORT_ENTRY_FAILED 0
#define SENT_REPORT_ENTRY_FINISHED 1
#define SENT_REPORT_ENTRY_MORE_FOLLOWS 2

static int
sendNextReportEntrySegment(ReportControl* self)
{
    if (self->clientConnection == NULL)
        return SENT_REPORT_ENTRY_FAILED;

    if (self->reportBuffer->nextToTransmit == NULL)
        return SENT_REPORT_ENTRY_FINISHED;

    bool sentSuccess = true;

    bool isBuffered = self->buffered;

    int maxMmsPduSize = MmsServerConnection_getMaxMmsPduSize(self->clientConnection);

    int estimatedSegmentSize = 19; /* maximum size of header information (header can have 13-19 byte) */
    estimatedSegmentSize += 8; /* reserve space for more-segments-follow (3 byte) and sub-seq-num (3-5 byte) */

    bool segmented = self->segmented;
    bool moreFollows = false;

    bool hasSeqNum = false;
    bool hasReportTimestamp = false;
    bool hasDataSetReference = false;
    bool hasBufOvfl = false;
    bool hasEntryId = false;
    bool hasConfRev = false;

    uint32_t accessResultSize = 0;

    ReportBufferEntry* report = self->reportBuffer->nextToTransmit;

#if (DEBUG_IED_SERVER == 1)
    printf("IED_SERVER: SEND NEXT REPORT: ");
    if (isBuffered)
        printReportId(report);
    printf(" size: %i\n", report->entryLength);
#endif

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_wait(self->rcbValuesLock);
#endif

    MmsValue* confRev = ReportControl_getRCBValue(self, "ConfRev");

    if (isBuffered) {
        MmsValue* entryIdValue = MmsValue_getElement(self->rcbValues, 11);
        MmsValue_setOctetString(entryIdValue, (uint8_t*) report->entryId, 8);
    }

    char rptIdBuf[130];

    MmsValue rptId;
    rptId.type = MMS_VISIBLE_STRING;
    rptId.value.visibleString.size = sizeof(rptIdBuf) - 1;
    rptId.value.visibleString.buf = rptIdBuf;

    MmsValue* rptIdFromRcb = ReportControl_getRCBValue(self, "RptID");

    const char* rptIdStr = MmsValue_toString(rptIdFromRcb);

    if (rptIdStr[0] == 0) {
        /* use default rptId when RptID is empty in RCB */
        updateWithDefaultRptId(self, &rptId);
    }
    else {
        MmsValue_setVisibleString(&rptId, rptIdStr);
    }

    MmsValue* optFlds = ReportControl_getRCBValue(self, "OptFlds");

    if (isBuffered == false) {
        /* delete option fields for unrelated options (not present in unbuffered report) */
        MmsValue_setBitStringBit(optFlds, 6, false); /* bufOvfl */
        MmsValue_setBitStringBit(optFlds, 7, false); /* entryID */
    }

    accessResultSize += MmsValue_encodeMmsData(&rptId, NULL, 0, false);
    accessResultSize += 5; /* add size of OptFlds */

    MmsValue _inclusionField;

    uint8_t* currentReportBufferPos = (uint8_t*) report + sizeof(ReportBufferEntry);

    MmsValue* inclusionField = NULL;

    if (report->flags == 0) {

        inclusionField = &_inclusionField;

        _inclusionField.type = MMS_BIT_STRING;
        _inclusionField.value.bitString.size = MmsValue_getBitStringSize(self->inclusionField);
        _inclusionField.value.bitString.buf = currentReportBufferPos;

        currentReportBufferPos += MemoryAllocator_getAlignedSize(MmsValue_getBitStringByteSize(inclusionField));
    }

    MmsValue_deleteAllBitStringBits(self->inclusionField);

    int dataLen;

    /* get LEN (length of encoded data) from report buffer */
    memcpy((uint8_t*)(&dataLen), currentReportBufferPos, sizeof(int));
    currentReportBufferPos += sizeof(int);

    uint8_t* valuesInReportBuffer = currentReportBufferPos;

    MmsValue_setBitStringBit(optFlds, 9, false); /* segmentation */

    MmsValue* sqNum = ReportControl_getRCBValue(self, "SqNum");

    if (MmsValue_getBitStringBit(optFlds, 1)) { /* sequence number */
        hasSeqNum = true;
        accessResultSize += MmsValue_encodeMmsData(sqNum, NULL, 0, false);
    }

    MmsValue _timeOfEntry;
    MmsValue* timeOfEntry = NULL;

    if (MmsValue_getBitStringBit(optFlds, 2)) { /* report time stamp */
        hasReportTimestamp = true;
    	_timeOfEntry.type = MMS_BINARY_TIME;
    	_timeOfEntry.value.binaryTime.size = 6;
        timeOfEntry = &_timeOfEntry;

    	MmsValue_setBinaryTime(timeOfEntry, report->timeOfEntry);

    	accessResultSize += MmsValue_encodeMmsData(timeOfEntry, NULL, 0, false);
    }

    MmsValue* datSet = ReportControl_getRCBValue(self, "DatSet");

    if (MmsValue_getBitStringBit(optFlds, 4)) {/* data set reference */
        hasDataSetReference = true;
        accessResultSize += MmsValue_encodeMmsData(datSet, NULL, 0, false);
    }

    MmsValue _bufOvfl;
    MmsValue* bufOvfl = NULL;

    if (isBuffered) {
        if (MmsValue_getBitStringBit(optFlds, 6)) { /* bufOvfl */
            hasBufOvfl = true;

            bufOvfl = &_bufOvfl;

            _bufOvfl.type = MMS_BOOLEAN;
            _bufOvfl.value.boolean = self->reportBuffer->isOverflow;

            accessResultSize += MmsValue_encodeMmsData(bufOvfl, NULL, 0, false);
        }
    }

    MmsValue _entryId;
    MmsValue* entryId = NULL;

    if (isBuffered) {
        if (MmsValue_getBitStringBit(optFlds, 7)) { /* entryID */
            hasEntryId = true;
            entryId = &_entryId;

            entryId->type = MMS_OCTET_STRING;
            entryId->value.octetString.buf = report->entryId;
            entryId->value.octetString.size = 8;
            entryId->value.octetString.maxSize = 8;

            accessResultSize += MmsValue_encodeMmsData(entryId, NULL, 0, false);
        }
    }

    if (MmsValue_getBitStringBit(optFlds, 8)) {
        hasConfRev = true;
        accessResultSize += MmsValue_encodeMmsData(confRev, NULL, 0, false);
    }

    accessResultSize += MmsValue_encodeMmsData(self->inclusionField, NULL, 0, false);

    /* here ends the base part that is equal for all sub reports and independent of the
     * number of included data points
     */

    estimatedSegmentSize += accessResultSize;
    int startElementIndex = self->startIndexForNextSegment; /* get value from segmented report control info */

    bool withDataReference = MmsValue_getBitStringBit(optFlds, 5);
    bool withReasonCode = MmsValue_getBitStringBit(optFlds, 3);

    LogicalDevice* ld = (LogicalDevice*) self->parentLN->parent;

    IedModel* iedModel = (IedModel*) ld->parent;

    int maxIndex = 0;

    char* iedName = iedModel->name;
    int iedNameLength = (int) strlen(iedName);

    int i;

    MmsValue _moreFollows;
    _moreFollows.type = MMS_BOOLEAN;
    _moreFollows.value.boolean = false;

    MmsValue* subSeqNum = self->subSeqVal;

    int numberOfAddedElements = 0;

    for (i = 0; i < self->dataSet->elementCount; i++) {

        if ((report->flags > 0) || MmsValue_getBitStringBit(inclusionField, i)) {

            if (i >= startElementIndex) {

                int elementSize = 0;

                if (withDataReference) {

                    DataSetEntry* dataSetEntry = getDataSetEntryWithIndex(self->dataSet->fcdas, i);

                    char dataReference[130];
                    int currentPos = 0;

                    int j;

                    for (j = 0; j < iedNameLength; j++) {
                        dataReference[currentPos++] = iedName[j];
                    }

                    int ldNameLength = (int) strlen(dataSetEntry->logicalDeviceName);
                    for (j = 0; j < ldNameLength; j++) {
                        dataReference[currentPos] = dataSetEntry->logicalDeviceName[j];
                        currentPos++;
                    }

                    dataReference[currentPos++] = '/';

                    for (j = 0; j < (int) strlen(dataSetEntry->variableName); j++) {
                        dataReference[currentPos++] = dataSetEntry->variableName[j];
                    }

                    dataReference[currentPos] = 0;

                    MmsValue _dataRef;
                    _dataRef.type = MMS_VISIBLE_STRING;
                    _dataRef.value.visibleString.buf = dataReference;
                    _dataRef.value.visibleString.size = currentPos;

                    elementSize += MmsValue_encodeMmsData(&_dataRef, NULL, 0, false);
                }

                /* get size of data */
                {
                    int length;

                    int lenSize = BerDecoder_decodeLength(currentReportBufferPos + 1, &length, 0, report->entryLength);

                    if (lenSize < 0) {
                        if (DEBUG_IED_SERVER)
                            printf("IED_SERVER: internal error in report buffer\n");

                        return false;
                    }

                    int dataElementSize =  1 + lenSize + length;

                    elementSize += dataElementSize;
                    currentReportBufferPos += dataElementSize;
                }


                if (withReasonCode) {
                    elementSize += 4; /* reason code size is always 4 byte */
                }

                if ((estimatedSegmentSize + elementSize) > maxMmsPduSize) {

                    segmented = true;
                    moreFollows = true;
                    _moreFollows.value.boolean = true;

                    if (startElementIndex == 0)
                        MmsValue_setUint32(subSeqNum, 0);

                    break;
                }

                MmsValue_setBitStringBit(self->inclusionField, i, true);

                numberOfAddedElements++;

                accessResultSize += elementSize;
                estimatedSegmentSize += elementSize;
            }
            else {
                /* move element pointer in report buffer to skip elements that were already sent in former segments */
                int length;

                int lenSize = BerDecoder_decodeLength(currentReportBufferPos + 1, &length, 0, report->entryLength);

                if (lenSize < 0) {
                    if (DEBUG_IED_SERVER)
                        printf("IED_SERVER: internal error in report buffer\n");

                    return false;
                }

                int dataElementSize =  1 + lenSize + length;

                currentReportBufferPos += dataElementSize;
            }
        }

        maxIndex++;
    }

    MmsValue_setBitStringBit(optFlds, 9, segmented); /* set segmentation flag */

    /* now calculate the exact information report segment size */

    if (segmented) {
        int segmentedSize = MmsValue_encodeMmsData(&_moreFollows, NULL, 0, false) + MmsValue_encodeMmsData(subSeqNum, NULL, 0, false);
        accessResultSize += segmentedSize;
    }

    uint32_t variableAccessSpecSize = 7; /* T L "RPT" */
    uint32_t listOfAccessResultSize = accessResultSize + BerEncoder_determineLengthSize(accessResultSize) + 1;
    uint32_t informationReportContentSize = variableAccessSpecSize + listOfAccessResultSize;
    uint32_t informationReportSize = 1 + informationReportContentSize + BerEncoder_determineLengthSize(informationReportContentSize);
    uint32_t completeMessageSize = 1 + informationReportSize + BerEncoder_determineLengthSize(informationReportSize);

    if (((int) completeMessageSize > maxMmsPduSize) || (numberOfAddedElements == 0)) {
        if (DEBUG_IED_SERVER)
            printf("IED_SERVER: MMS PDU size too small to encode report data (max PDU size = %i) -> skip message!\n", maxMmsPduSize);

        self->startIndexForNextSegment = 0;
        segmented = false;
        moreFollows = false;
        sentSuccess = true;

        goto exit_remove_report;
    }

    /* encode the report message */

    ReportControl_unlockNotify(self);

    IsoConnection_lock(self->clientConnection->isoConnection);

    ByteBuffer* reportBuffer = MmsServer_reserveTransmitBuffer(self->server->mmsServer);

    uint8_t* buffer = reportBuffer->buffer;
    int bufPos = 0;

    /* encode header */
    bufPos = BerEncoder_encodeTL(0xa3, informationReportSize, buffer, bufPos);
    bufPos = BerEncoder_encodeTL(0xa0, informationReportContentSize, buffer, bufPos);

    bufPos = BerEncoder_encodeTL(0xa1, 5, buffer, bufPos);
    bufPos = BerEncoder_encodeStringWithTag(0x80, "RPT", buffer, bufPos);

    bufPos = BerEncoder_encodeTL(0xa0, accessResultSize, buffer, bufPos);

    /* encode access-results */

    bufPos = MmsValue_encodeMmsData(&rptId, buffer, bufPos, true);
    bufPos = MmsValue_encodeMmsData(optFlds, buffer, bufPos, true);

    if (hasSeqNum)
        bufPos = MmsValue_encodeMmsData(sqNum, buffer, bufPos, true);

    if (hasReportTimestamp)
        bufPos = MmsValue_encodeMmsData(timeOfEntry, buffer, bufPos, true);

    if (hasDataSetReference)
        bufPos = MmsValue_encodeMmsData(datSet, buffer, bufPos, true);

    if (hasBufOvfl)
        bufPos = MmsValue_encodeMmsData(bufOvfl, buffer, bufPos, true);

    if (hasEntryId)
        bufPos = MmsValue_encodeMmsData(entryId, buffer, bufPos, true);

    if (hasConfRev)
        bufPos = MmsValue_encodeMmsData(confRev, buffer, bufPos, true);

    if (segmented) {
        bufPos = MmsValue_encodeMmsData(subSeqNum, buffer, bufPos, true);
        bufPos = MmsValue_encodeMmsData(&_moreFollows, buffer, bufPos, true);
    }

    bufPos = MmsValue_encodeMmsData(self->inclusionField, buffer, bufPos, true);

    /* encode data references if selected */
    if (MmsValue_getBitStringBit(optFlds, 5)) { /* data-reference */
        DataSetEntry* dataSetEntry = getDataSetEntryWithIndex(self->dataSet->fcdas, startElementIndex);

        for (i = startElementIndex; i < maxIndex; i++) {
            bool addReferenceForEntry = false;

            if (report->flags > 0)
               addReferenceForEntry = true;
            else if (MmsValue_getBitStringBit(self->inclusionField, i))
                addReferenceForEntry = true;

            if (addReferenceForEntry) {

                char dataReference[130];
                int currentPos = 0;

                int j;

                for (j = 0; j < iedNameLength; j++) {
                    dataReference[currentPos++] = iedName[j];
                }

                int ldNameLength =  (int) strlen(dataSetEntry->logicalDeviceName);
                for (j = 0; j <  ldNameLength; j++) {
                    dataReference[currentPos] = dataSetEntry->logicalDeviceName[j];
                    currentPos++;
                }

                dataReference[currentPos++] = '/';

                for (j = 0; j < (int) strlen(dataSetEntry->variableName); j++) {
                    dataReference[currentPos++] = dataSetEntry->variableName[j];
                }

                dataReference[currentPos] = 0;

                MmsValue _dataRef;
                _dataRef.type = MMS_VISIBLE_STRING;
                _dataRef.value.visibleString.buf = dataReference;
                _dataRef.value.visibleString.size = currentPos;

                bufPos = MmsValue_encodeMmsData(&_dataRef, buffer, bufPos, true);
            }

            dataSetEntry = dataSetEntry->sibling;
        }
    }

    /* move to start position in report buffer */
    currentReportBufferPos = valuesInReportBuffer;

    /* encode data set value elements */
    for (i = 0; i < maxIndex; i++) {

        bool isInBuffer = false;

        if (report->flags > 0)
            isInBuffer = true;
        else {
            if (MmsValue_getBitStringBit(inclusionField, i))
                isInBuffer = true;
        }

        if (isInBuffer)
        {
            int length;

            int lenSize = BerDecoder_decodeLength(currentReportBufferPos + 1, &length, 0, report->entryLength);

            if (lenSize < 0) {
                if (DEBUG_IED_SERVER)
                    printf("IED_SERVER: internal error in report buffer\n");

                return SENT_REPORT_ENTRY_FAILED;
            }

            int dataElementSize =  1 + lenSize + length;

            if (i >= startElementIndex) {
                /* copy value from report entry to message buffer */
                memcpy(buffer + bufPos, currentReportBufferPos, dataElementSize);
                bufPos += dataElementSize;
            }

            currentReportBufferPos += dataElementSize;
        }
    }

    /* add reason code to report if requested */
    if (withReasonCode) {

        /* move to start position in report buffer */
        currentReportBufferPos = valuesInReportBuffer + dataLen;

        uint8_t bsBuf[1];

        MmsValue _reason;
        _reason.type = MMS_BIT_STRING;
        _reason.value.bitString.size = 6;
        _reason.value.bitString.buf = bsBuf;

        for (i = 0; i < maxIndex; i++) {

            bool isIncluded = false;

            if (report->flags > 0) {
                bsBuf[0] = 0; /* clear all bits */

                if (report->flags & 0x02) /* GI */
                    MmsValue_setBitStringBit(&_reason, 5, true);

                if (report->flags & 0x01) /* Integrity */
                    MmsValue_setBitStringBit(&_reason, 4, true);

                isIncluded = true;
            }
            else if (MmsValue_getBitStringBit(self->inclusionField, i)) {
                bsBuf[0] = 0; /* clear all bits */

                uint8_t reasonForInclusion = *currentReportBufferPos;

                if (reasonForInclusion & REPORT_CONTROL_QUALITY_CHANGED)
                    MmsValue_setBitStringBit(&_reason, 2, true);

                if (reasonForInclusion & REPORT_CONTROL_VALUE_CHANGED)
                    MmsValue_setBitStringBit(&_reason, 1, true);

                if (reasonForInclusion & REPORT_CONTROL_VALUE_UPDATE)
                    MmsValue_setBitStringBit(&_reason, 3, true);

                isIncluded = true;
            }

            if (isIncluded) {

                if (i >= startElementIndex)
                    bufPos =  MmsValue_encodeMmsData(&_reason, buffer, bufPos, true);

                currentReportBufferPos++;
            }
        }
    }

    reportBuffer->size = bufPos;

    sentSuccess = MmsServerConnection_sendMessage(self->clientConnection, reportBuffer);

    MmsServer_releaseTransmitBuffer(self->server->mmsServer);

    IsoConnection_unlock(self->clientConnection->isoConnection);

    if (sentSuccess == false) {
        moreFollows = false;
        goto exit_function;
    }

    if (moreFollows == false) {
        /* reset sub sequence number */
        segmented = false;
        self->startIndexForNextSegment = 0;
    }
    else {
        /* increase sub sequence number */
        uint32_t subSeqNumVal = MmsValue_toUint32(subSeqNum);
        subSeqNumVal++;
        MmsValue_setUint32(subSeqNum, subSeqNumVal);

        self->startIndexForNextSegment = maxIndex;
    }

exit_remove_report:

    if (segmented == false) {

        assert(self->reportBuffer->nextToTransmit != self->reportBuffer->nextToTransmit->next);

        self->reportBuffer->nextToTransmit = self->reportBuffer->nextToTransmit->next;

    #if (DEBUG_IED_SERVER == 1)
        printf("IED_SERVER: reporting.c nextToTransmit: %p\n", self->reportBuffer->nextToTransmit);
        printEnqueuedReports(self);
    #endif

        /* Increase sequence number */
        self->sqNum++;

        if (isBuffered == false) {
            /* Unbuffered reporting --> sqNum is 8 bit only!!! */
            if (self->sqNum == 256)
                self->sqNum = 0;
        }

        MmsValue_setUint16(sqNum, self->sqNum);

        if (self->reportBuffer->isOverflow)
            self->reportBuffer->isOverflow = false;
    }

exit_function:

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_post(self->rcbValuesLock);
#endif

    self->segmented = segmented;

    if (sentSuccess == false)
        return SENT_REPORT_ENTRY_FAILED;

    if (moreFollows)
        return SENT_REPORT_ENTRY_MORE_FOLLOWS;
    else
        return SENT_REPORT_ENTRY_FINISHED;
}

static void
sendNextReportEntry(ReportControl* self)
{
#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_wait(self->reportBuffer->lock);
#endif

    int messageCount = 0;

    while (self->reportBuffer->nextToTransmit) {
        messageCount++;

        bool sendNextEntrySegment = true;

        int sendResult = SENT_REPORT_ENTRY_FAILED;

        while (sendNextEntrySegment) {
            sendResult = sendNextReportEntrySegment(self);

            if (sendResult != SENT_REPORT_ENTRY_FAILED) {
                messageCount++;
            }

            if (sendResult != SENT_REPORT_ENTRY_MORE_FOLLOWS)
                sendNextEntrySegment = false;
        }

        if (sendResult == SENT_REPORT_ENTRY_FAILED) {
            break;
        }

        if (messageCount > 100)
            break;
    }

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_post(self->reportBuffer->lock);
#endif
}

void
Reporting_activateBufferedReports(MmsMapping* self)
{
    LinkedList element = self->reportControls;

    while ((element = LinkedList_getNext(element)) != NULL ) {
        ReportControl* rc = (ReportControl*) element->data;

        if (rc->buffered) {
#if (CONFIG_MMS_THREADLESS_STACK != 1)
            Semaphore_wait(rc->rcbValuesLock);
#endif

            if (updateReportDataset(self, rc, NULL, NULL))
                rc->isBuffering = true;
            else
                rc->isBuffering = false;

#if (CONFIG_MMS_THREADLESS_STACK != 1)
            Semaphore_post(rc->rcbValuesLock);
#endif
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
                    rc->triggered = false;
                    enqueueReport(rc, false, false, currentTimeInMs);
                }

                enqueueReport(rc, false, true, currentTimeInMs);

                rc->gi = false;

                rc->triggered = false;
            }
        }

        if (rc->triggerOps & TRG_OPT_INTEGRITY) {

            if (rc->intgPd > 0) {

                if (currentTimeInMs >= rc->nextIntgReportTime) {

                    /* send current events in event buffer before integrity report */
                    if (rc->triggered) {
                        enqueueReport(rc, false, false, currentTimeInMs);
                        rc->triggered = false;
                    }

                    if (rc->server->syncIntegrityReportTimes) {
                        rc->nextIntgReportTime = getNextRoundedStartTime(currentTimeInMs, rc->intgPd);
                    }
                    else {
                        rc->nextIntgReportTime = rc->nextIntgReportTime + rc->intgPd;
                    }

                    /* check for system time change effects */
                    if ((rc->nextIntgReportTime < currentTimeInMs) || (rc->nextIntgReportTime > currentTimeInMs + rc->intgPd)) {

                        if (rc->server->syncIntegrityReportTimes) {
                            rc->nextIntgReportTime = getNextRoundedStartTime(currentTimeInMs, rc->intgPd);
                        }
                        else {
                            rc->nextIntgReportTime = currentTimeInMs + rc->intgPd;
                        }
                    }

                    enqueueReport(rc, true, false, currentTimeInMs);

                    rc->triggered = false;
                }
                else {
                    /* check for system time change effects */
                    if ((rc->nextIntgReportTime < currentTimeInMs) || (rc->nextIntgReportTime > currentTimeInMs + rc->intgPd)) {
                        if (rc->server->syncIntegrityReportTimes) {
                            rc->nextIntgReportTime = getNextRoundedStartTime(currentTimeInMs, rc->intgPd);
                        }
                        else {
                            rc->nextIntgReportTime = currentTimeInMs + rc->intgPd;
                        }
                    }
                }
            }
        }

        if (rc->triggered) {
            if (currentTimeInMs >= rc->reportTime) {

                enqueueReport(rc, false, false, currentTimeInMs);

                rc->triggered = false;
            }
        }
    }
}

void
Reporting_processReportEvents(MmsMapping* self, uint64_t currentTimeInMs)
{
#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_wait(self->isModelLockedMutex);
#endif

    if (self->isModelLocked == false) {

        LinkedList element = self->reportControls;

        while ((element = LinkedList_getNext(element)) != NULL ) {
            ReportControl* rc = (ReportControl*) element->data;

            ReportControl_lockNotify(rc);

            processEventsForReport(rc, currentTimeInMs);

            ReportControl_unlockNotify(rc);
        }
    }

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_post(self->isModelLockedMutex);
#endif
}

/*
 * To be called only by connection thread!
 */
void
Reporting_sendReports(MmsMapping* self, MmsServerConnection connection)
{
    LinkedList element = LinkedList_getNext(self->reportControls);

    while (element) {
        ReportControl* rc = (ReportControl*) LinkedList_getData(element);

        if (rc->clientConnection == connection) {

            ReportControl_lockNotify(rc);

            if (rc->enabled) {
                sendNextReportEntry(rc);
            }

            ReportControl_unlockNotify(rc);
        }

        element = LinkedList_getNext(element);
    }
}

static inline void
copySingleValueToReportBuffer(ReportControl* self, int dataSetEntryIndex)
{
    if (self->bufferedDataSetValues[dataSetEntryIndex] == NULL)
        self->bufferedDataSetValues[dataSetEntryIndex] = MmsValue_clone(self->valueReferences[dataSetEntryIndex]);
    else
        MmsValue_update(self->bufferedDataSetValues[dataSetEntryIndex], self->valueReferences[dataSetEntryIndex]);
}

static void
copyValuesToReportBuffer(ReportControl* self)
{
    int i;
    for (i = 0; i < self->dataSet->elementCount; i++) {
        if (self->inclusionFlags[i] & REPORT_CONTROL_NOT_UPDATED) {
            copySingleValueToReportBuffer(self, i);

            /* clear not-updated flag */
            self->inclusionFlags[i] &= (~REPORT_CONTROL_NOT_UPDATED);
        }
    }
}

/* check if report have to be sent after data model update */
void
Reporting_processReportEventsAfterUnlock(MmsMapping* self)
{
    LinkedList element = self->reportControls;

    uint64_t currentTime = Hal_getTimeInMs();

    while ((element = LinkedList_getNext(element)) != NULL ) {
        ReportControl* rc = (ReportControl*) element->data;

        ReportControl_lockNotify(rc);

        if ((rc->enabled) || (rc->isBuffering)) {

            if (rc->triggered) {
                copyValuesToReportBuffer(rc);

                processEventsForReport(rc, currentTime);
            }

        }

        ReportControl_unlockNotify(rc);
    }
}

void
ReportControl_valueUpdated(ReportControl* self, int dataSetEntryIndex, int flag, bool modelLocked)
{
    ReportControl_lockNotify(self);

    if (self->inclusionFlags[dataSetEntryIndex] & flag) { /* report for this data set entry is already pending (bypass BufTm) */
        self->reportTime = Hal_getTimeInMs();

        if (modelLocked) {
            /* buffer all relevant values */
            copyValuesToReportBuffer(self);
        }

        processEventsForReport(self, self->reportTime);
    }

    if (modelLocked) {
        /* set flag to update values when report is to be sent or data model unlocked */
        self->inclusionFlags[dataSetEntryIndex] = self->inclusionFlags[dataSetEntryIndex] | flag | REPORT_CONTROL_NOT_UPDATED;

    }
    else {
        self->inclusionFlags[dataSetEntryIndex] = flag;

        /* buffer value for report */
        copySingleValueToReportBuffer(self, dataSetEntryIndex);
    }

    if (self->triggered == false) {
        uint64_t currentTime = Hal_getTimeInMs();

        MmsValue_setBinaryTime(self->timeOfEntry, currentTime);

        self->reportTime = currentTime + self->bufTm;
    }

    self->triggered = true;

    ReportControl_unlockNotify(self);
}

bool
ReportControlBlock_getRptEna(ReportControlBlock* self)
{
    if (self->trgOps & 64) {
        ReportControl* rc = (ReportControl*)(self->sibling);

        return rc->enabled;
    }
    else {
        return false;
    }
}

char*
ReportControlBlock_getRptID(ReportControlBlock* self)
{
    if (self->trgOps & 64) {
        ReportControl* rc = (ReportControl*)(self->sibling);

#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_wait(rc->rcbValuesLock);
#endif

        MmsValue* rptIdValue = ReportControl_getRCBValue(rc, "RptID");

        char* rptIdStr = strdup(MmsValue_toString(rptIdValue));

#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_post(rc->rcbValuesLock);
#endif

        return rptIdStr;
    }
    else {
        return strdup(self->rptId);
    }
}

char*
ReportControlBlock_getDataSet(ReportControlBlock* self)
{
    if (self->trgOps & 64) {
        ReportControl* rc = (ReportControl*)(self->sibling);

#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_wait(rc->rcbValuesLock);
#endif

        MmsValue* dataSetValue = ReportControl_getRCBValue(rc, "DatSet");

        char* dataSetStr = strdup(MmsValue_toString(dataSetValue));

#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_post(rc->rcbValuesLock);
#endif

        return dataSetStr;
    }
    else {
        return strdup(self->dataSetName);
    }
}

uint32_t
ReportControlBlock_getConfRev(ReportControlBlock* self)
{
    if (self->trgOps & 64) {
        ReportControl* rc = (ReportControl*)(self->sibling);

#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_wait(rc->rcbValuesLock);
#endif

        MmsValue* confRevValue = ReportControl_getRCBValue(rc, "ConfRev");

        uint32_t confRev = MmsValue_toUint32(confRevValue);

#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_post(rc->rcbValuesLock);
#endif

        return confRev;
    }
    else {
        return self->confRef;
    }
}

uint32_t
ReportControlBlock_getOptFlds(ReportControlBlock* self)
{
    if (self->trgOps & 64) {
        ReportControl* rc = (ReportControl*)(self->sibling);

#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_wait(rc->rcbValuesLock);
#endif

        MmsValue* optFldsValue = ReportControl_getRCBValue(rc, "OptFlds");

        uint32_t optFlds = MmsValue_getBitStringAsInteger(optFldsValue) / 2;

#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_post(rc->rcbValuesLock);
#endif

        return optFlds;
    }
    else {
        return self->options;
    }
}

uint32_t
ReportControlBlock_getBufTm(ReportControlBlock* self)
{
    if (self->trgOps & 64) {
        ReportControl* rc = (ReportControl*)(self->sibling);

#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_wait(rc->rcbValuesLock);
#endif

        MmsValue* bufTmValue = ReportControl_getRCBValue(rc, "BufTm");

        uint32_t bufTm = MmsValue_toUint32(bufTmValue);

#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_post(rc->rcbValuesLock);
#endif

        return bufTm;
    }
    else {
        return self->bufferTime;
    }
}

uint16_t
ReportControlBlock_getSqNum(ReportControlBlock* self)
{
    if (self->trgOps & 64) {
        ReportControl* rc = (ReportControl*)(self->sibling);

#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_wait(rc->rcbValuesLock);
#endif

        MmsValue* sqNumValue = ReportControl_getRCBValue(rc, "SqNum");

        uint16_t sqNum = (uint16_t)MmsValue_toUint32(sqNumValue);

#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_post(rc->rcbValuesLock);
#endif

        return sqNum;
    }
    else {
        return 0;
    }
}

uint32_t
ReportControlBlock_getTrgOps(ReportControlBlock* self)
{
    if (self->trgOps & 64) {
        ReportControl* rc = (ReportControl*)(self->sibling);

        return rc->triggerOps;
    }
    else {
        return (int)(self->trgOps);
    }
}

uint32_t
ReportControlBlock_getIntgPd(ReportControlBlock* self)
{
    if (self->trgOps & 64) {
        ReportControl* rc = (ReportControl*)(self->sibling);

        return rc->intgPd;
    }
    else {
        return self->intPeriod;
    }
}

bool
ReportControlBlock_getGI(ReportControlBlock* self)
{
    if (self->trgOps & 64) {
        ReportControl* rc = (ReportControl*)(self->sibling);

#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_wait(rc->rcbValuesLock);
#endif

        MmsValue* giValue = ReportControl_getRCBValue(rc, "GI");

        bool gi = MmsValue_getBoolean(giValue);

#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_post(rc->rcbValuesLock);
#endif

        return gi;
    }
    else {
        return false;
    }
}

bool
ReportControlBlock_getPurgeBuf(ReportControlBlock* self)
{
    bool purgeBuf = false;

    if (self->trgOps & 64) {
        ReportControl* rc = (ReportControl*)(self->sibling);

#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_wait(rc->rcbValuesLock);
#endif

        MmsValue* purgeBufValue = ReportControl_getRCBValue(rc, "PurgeBuf");

        if (purgeBufValue) {
            purgeBuf = MmsValue_getBoolean(purgeBufValue);
        }

#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_post(rc->rcbValuesLock);
#endif
    }

    return purgeBuf;
}

MmsValue*
ReportControlBlock_getEntryId(ReportControlBlock* self)
{
    MmsValue* entryId = NULL;

    if (self->trgOps & 64) {
        ReportControl* rc = (ReportControl*)(self->sibling);

#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_wait(rc->rcbValuesLock);
#endif

        MmsValue* entryIdValue = ReportControl_getRCBValue(rc, "EntryID");

        entryId = MmsValue_clone(entryIdValue);

#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_post(rc->rcbValuesLock);
#endif
    }

    return entryId;
}

uint64_t
ReportControlBlock_getTimeofEntry(ReportControlBlock* self)
{
    uint64_t timeofEntry = 0;

    if (self->trgOps & 64) {
        ReportControl* rc = (ReportControl*)(self->sibling);

#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_wait(rc->rcbValuesLock);
#endif

        MmsValue* timeofEntryValue = ReportControl_getRCBValue(rc, "TimeofEntry");

        if (timeofEntryValue) {
            timeofEntry = MmsValue_getBinaryTimeAsUtcMs(timeofEntryValue);
        }

#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_post(rc->rcbValuesLock);
#endif
    }

    return timeofEntry;
}

int16_t
ReportControlBlock_getResvTms(ReportControlBlock* self)
{
    int16_t resvTms = 0;

    if (self->trgOps & 64) {
        ReportControl* rc = (ReportControl*)(self->sibling);

#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_wait(rc->rcbValuesLock);
#endif

        MmsValue* resvTmsValue = ReportControl_getRCBValue(rc, "ResvTms");

        if (resvTmsValue) {
            resvTms = (int16_t)MmsValue_toInt32(resvTmsValue);
        }

#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_post(rc->rcbValuesLock);
#endif
    }

    return resvTms;
}

bool
ReportControlBlock_getResv(ReportControlBlock* self)
{
    bool resv = false;

    if (self->trgOps & 64) {
        ReportControl* rc = (ReportControl*)(self->sibling);

#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_wait(rc->rcbValuesLock);
#endif

        MmsValue* resvValue = ReportControl_getRCBValue(rc, "Resv");

        if (resvValue) {
            resv = MmsValue_getBoolean(resvValue);
        }

#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_post(rc->rcbValuesLock);
#endif
    }

    return resv;
}

MmsValue*
ReportControlBlock_getOwner(ReportControlBlock* self)
{
    if (self->trgOps & 64) {
        ReportControl* rc = (ReportControl*)(self->sibling);

        if (rc->hasOwner) {

#if (CONFIG_MMS_THREADLESS_STACK != 1)
            Semaphore_wait(rc->rcbValuesLock);
#endif

            MmsValue* ownerValue = ReportControl_getRCBValue(rc, "Owner");

            MmsValue* ownerValueCopy = MmsValue_clone(ownerValue);

#if (CONFIG_MMS_THREADLESS_STACK != 1)
            Semaphore_post(rc->rcbValuesLock);
#endif

            return ownerValueCopy;
        }
        else
            return NULL;
    }
    else {
        return NULL;
    }
}

#endif /* (CONFIG_IEC61850_REPORT_SERVICE == 1) */
