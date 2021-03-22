/*
 *  mms_goose.c
 *
 *  Copyright 2013-2021 Michael Zillgith
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

#include "stack_config.h"

#if (CONFIG_INCLUDE_GOOSE_SUPPORT == 1)

#define GOOSE_MAX_MESSAGE_SIZE 1518

#include "libiec61850_platform_includes.h"
#include "mms_mapping.h"
#include "linked_list.h"

#include "hal_thread.h"

#include "reporting.h"
#include "mms_mapping_internal.h"

#include "mms_goose.h"
#include "goose_publisher.h"
#include "ied_server_private.h"

#if defined(_MSC_VER) && _MSC_VER < 1900
#define snprintf(buf,len, format,...) _snprintf_s(buf, len,len, format, __VA_ARGS__)
#endif

struct sMmsGooseControlBlock {
    char* name;
    bool goEna;
    unsigned int isDynamicDataSet:1;
    unsigned int useVlanTag:1;

    char* dstAddress;

    MmsDomain* domain;
    LogicalNode* logicalNode;
    MmsVariableSpecification* mmsType;
    MmsValue* mmsValue;
    GoosePublisher publisher;

    DataSet* dataSet;

    LinkedList dataSetValues;
    uint64_t nextPublishTime;
    int retransmissionsLeft; /* number of retransmissions left for the last event */

    int minTime;
    int maxTime;

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore publisherMutex;
#endif

    MmsMapping* mmsMapping;

    char* goCBRef;
    char* goId;
    char* dataSetRef;

    char* gooseInterfaceId;

    bool stateChangePending;
};

static void
setNdsCom(MmsGooseControlBlock mmsGCB, bool value)
{
    MmsValue* ndsComValue = MmsValue_getElement(mmsGCB->mmsValue, 4);

    if (ndsComValue) {
        MmsValue_setBoolean(ndsComValue, value);
    }
}

static bool
getNdsCom(MmsGooseControlBlock mmsGCB)
{
    bool ndsCom = true;

    MmsValue* ndsComValue = MmsValue_getElement(mmsGCB->mmsValue, 4);

    if (ndsComValue)
        ndsCom = MmsValue_getBoolean(ndsComValue);

    return ndsCom;
}

bool
MmsGooseControlBlock_getNdsCom(MmsGooseControlBlock self)
{
    return getNdsCom(self);
}

bool
MmsGooseControlBlock_getGoEna(MmsGooseControlBlock self)
{
    bool retVal = false;

    if (self->mmsValue) {
        MmsValue* goEnaValue = MmsValue_getElement(self->mmsValue, 0);

        if (goEnaValue) {
            retVal = MmsValue_getBoolean(goEnaValue);
        }
    }

    return retVal;
}


int
MmsGooseControlBlock_getMinTime(MmsGooseControlBlock self)
{
    int retVal = -1;

    if (self->mmsValue) {
        MmsValue* minTimeValue = MmsValue_getElement(self->mmsValue, 6);

        if (minTimeValue) {
            retVal = MmsValue_toInt32(minTimeValue);
        }
    }

    return retVal;
}

int
MmsGooseControlBlock_getMaxTime(MmsGooseControlBlock self)
{
    int retVal = -1;

    if (self->mmsValue) {
        MmsValue* maxTimeValue = MmsValue_getElement(self->mmsValue, 7);

        if (maxTimeValue) {
            retVal = MmsValue_toInt32(maxTimeValue);
        }
    }

    return retVal;
}

bool
MmsGooseControlBlock_getFixedOffs(MmsGooseControlBlock self)
{
    bool retVal = false;

    if (self->mmsValue) {
        MmsValue* fixedOffsValue = MmsValue_getElement(self->mmsValue, 8);

        if (fixedOffsValue) {
            retVal = MmsValue_getBoolean(fixedOffsValue);
        }
    }

    return retVal;
}

#if (CONFIG_IEC61850_SERVICE_TRACKING == 1)

static void
copyGCBValuesToTrackingObject(MmsGooseControlBlock gc)
{
    if (gc->mmsMapping->gocbTrk) {
        GocbTrkInstance trkInst = gc->mmsMapping->gocbTrk;

        if (trkInst->goEna)
            MmsValue_setBoolean(trkInst->goEna->mmsValue, MmsGooseControlBlock_isEnabled(gc));

        if (trkInst->goID)
            MmsValue_setVisibleString(trkInst->goID->mmsValue, gc->goId);

        if (trkInst->datSet)
        {
            char datSet[130];
            LogicalNode* ln = (LogicalNode*) gc->logicalNode;
            LogicalDevice* ld = (LogicalDevice*) ln->parent;
            char* iedName = gc->mmsMapping->mmsDevice->deviceName;
            snprintf(datSet, 129, "%s%s/%s", iedName, ld->name, gc->dataSet->name);
            datSet[129] = 0;
            StringUtils_replace(datSet, '$', '.');
            MmsValue_setVisibleString(trkInst->datSet->mmsValue, datSet);
        }

        if (trkInst->confRev) {
            uint32_t confRev = MmsValue_toUint32(MmsValue_getElement(gc->mmsValue, 3));
            MmsValue_setUint32(trkInst->confRev->mmsValue, confRev);
        }
        if (trkInst->ndsCom) {
            bool ndsCom = MmsValue_getBoolean(MmsValue_getElement(gc->mmsValue, 4));
            MmsValue_setBoolean(trkInst->ndsCom->mmsValue, ndsCom);
        }

        if (trkInst->dstAddress) {
          MmsValue_update(trkInst->dstAddress->mmsValue, MmsValue_getElement(gc->mmsValue, 5));
        }
    }
}

static void
updateGenericTrackingObjectValues(MmsGooseControlBlock gc, IEC61850_ServiceType serviceType, MmsDataAccessError errVal)
{
    ServiceTrkInstance trkInst = NULL;

    if (gc->mmsMapping->gocbTrk) {
        trkInst = (ServiceTrkInstance) gc->mmsMapping->gocbTrk;
    }

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
        LogicalNode* ln = (LogicalNode*) gc->logicalNode;
        LogicalDevice* ld = (LogicalDevice*) ln->parent;

        char* iedName = gc->mmsMapping->iedServer->mmsDevice->deviceName;

        snprintf(objRef, 129, "%s%s/%s.%s", iedName, ld->name, gc->logicalNode->name, gc->name);
        objRef[129] = 0;

        if (trkInst->objRef) {
            IedServer_updateVisibleStringAttributeValue(gc->mmsMapping->iedServer, trkInst->objRef, objRef);
        }
    }
}

#endif /* (CONFIG_IEC61850_SERVICE_TRACKING == 1) */

MmsGooseControlBlock
MmsGooseControlBlock_create()
{
	MmsGooseControlBlock self = (MmsGooseControlBlock) GLOBAL_CALLOC(1, sizeof(struct sMmsGooseControlBlock));

	if (self) {
	    self->useVlanTag = true;
#if (CONFIG_MMS_THREADLESS_STACK != 1)
	    self->publisherMutex = Semaphore_create(1);
#endif
	}

    return self;
}

void
MmsGooseControlBlock_destroy(MmsGooseControlBlock self)
{
#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_destroy(self->publisherMutex);
#endif

    if (self->publisher != NULL)
        GoosePublisher_destroy(self->publisher);

    if (self->dataSetValues != NULL)
        LinkedList_destroyStatic(self->dataSetValues);

    if (self->goCBRef != NULL)
        GLOBAL_FREEMEM(self->goCBRef);

    if (self->goId != NULL)
        GLOBAL_FREEMEM(self->goId);

    if (self->dataSetRef != NULL)
        GLOBAL_FREEMEM(self->dataSetRef);

    if (self->dataSet != NULL) {
        if (self->isDynamicDataSet) {
            MmsMapping_freeDynamicallyCreatedDataSet(self->dataSet);
            self->isDynamicDataSet = false;
            self->dataSet = NULL;
        }
    }

    if (self->gooseInterfaceId != NULL)
        GLOBAL_FREEMEM(self->gooseInterfaceId);

    MmsValue_delete(self->mmsValue);

    GLOBAL_FREEMEM(self);
}

void
MmsGooseControlBlock_useGooseVlanTag(MmsGooseControlBlock self, bool useVlanTag)
{
    self->useVlanTag = useVlanTag;
}

void
MmsGooseControlBlock_setGooseInterfaceId(MmsGooseControlBlock self, const char* interfaceId)
{
    if (self->gooseInterfaceId != NULL)
        GLOBAL_FREEMEM(self->gooseInterfaceId);

    self->gooseInterfaceId = StringUtils_copyString(interfaceId);
}

MmsDomain*
MmsGooseControlBlock_getDomain(MmsGooseControlBlock self)
{
    return self->domain;
}

DataSet*
MmsGooseControlBlock_getDataSet(MmsGooseControlBlock self)
{
    return self->dataSet;
}

LogicalNode*
MmsGooseControlBlock_getLogicalNode(MmsGooseControlBlock self)
{
    return self->logicalNode;
}

char*
MmsGooseControlBlock_getLogicalNodeName(MmsGooseControlBlock self)
{
    return self->logicalNode->name;
}

char*
MmsGooseControlBlock_getName(MmsGooseControlBlock self)
{
    return self->name;
}

MmsValue*
MmsGooseControlBlock_getMmsValues(MmsGooseControlBlock self)
{
    return self->mmsValue;
}

MmsVariableSpecification*
MmsGooseControlBlock_getVariableSpecification(MmsGooseControlBlock self)
{
    return self->mmsType;
}

bool
MmsGooseControlBlock_isEnabled(MmsGooseControlBlock self)
{
    return self->goEna;
}

static int
calculateMaxDataSetSize(DataSet* dataSet)
{
    int dataSetSize = 0;

    DataSetEntry* dataSetEntry = dataSet->fcdas;

    while (dataSetEntry) {
        dataSetSize +=  MmsValue_getMaxEncodedSize(dataSetEntry->value);

        dataSetEntry = dataSetEntry->sibling;
    }

    return dataSetSize;
}

bool
MmsGooseControlBlock_enable(MmsGooseControlBlock self, MmsMapping* mmsMapping)
{
    bool retVal = false;

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_wait(self->publisherMutex);
#endif

    if (!MmsGooseControlBlock_isEnabled(self)) {

        if (self->dataSetRef != NULL) {
            GLOBAL_FREEMEM(self->dataSetRef);

            if (self->dataSet != NULL)
                if (self->isDynamicDataSet) {
                    MmsMapping_freeDynamicallyCreatedDataSet(self->dataSet);
                    self->isDynamicDataSet = false;
                    self->dataSet = NULL;
                }

            if (self->dataSetValues != NULL) {
                LinkedList_destroyStatic(self->dataSetValues);
                self->dataSetValues = NULL;
            }
        }

        self->dataSet = NULL;

        const char* dataSetRef = MmsValue_toString(MmsValue_getElement(self->mmsValue, 2));

        if (dataSetRef != NULL) {

            self->dataSetRef = StringUtils_copyString(dataSetRef);

            self->dataSet = IedModel_lookupDataSet(self->mmsMapping->model, self->dataSetRef);

            self->isDynamicDataSet = false;

            if (self->dataSet == NULL) {
                self->dataSet = MmsMapping_getDomainSpecificDataSet(self->mmsMapping, self->dataSetRef);

                self->isDynamicDataSet = true;
            }

        }

        if (self->dataSet != NULL) {

            int dataSetSize = calculateMaxDataSetSize(self->dataSet);

            /*  Calculate maximum GOOSE message size */
            int maxGooseMessageSize =  26 + 51 + 6;
            maxGooseMessageSize += strlen(self->goCBRef);
            if (self->goId)
                maxGooseMessageSize += strlen(self->goId);
            else
                maxGooseMessageSize += strlen(self->goCBRef);
            maxGooseMessageSize += strlen(self->dataSetRef);

            maxGooseMessageSize += dataSetSize;

            if (maxGooseMessageSize > GOOSE_MAX_MESSAGE_SIZE) {
                setNdsCom(self, true);

#if (CONFIG_IEC61850_SERVICE_TRACKING == 1)
                copyGCBValuesToTrackingObject(self);
                updateGenericTrackingObjectValues(self, IEC61850_SERVICE_TYPE_SET_GOCB_VALUES, DATA_ACCESS_ERROR_OBJECT_VALUE_INVALID);
#endif /* (CONFIG_IEC61850_SERVICE_TRACKING == 1) */
            }
            else {
                MmsValue* goEna = MmsValue_getElement(self->mmsValue, 0);

                MmsValue_setBoolean(goEna, true);


                MmsValue* dstAddress = MmsValue_getElement(self->mmsValue, 5);

                CommParameters commParameters;
                commParameters.appId = MmsValue_toInt32(MmsValue_getElement(dstAddress, 3));
                commParameters.vlanId = MmsValue_toInt32(MmsValue_getElement(dstAddress, 2));
                commParameters.vlanPriority = MmsValue_toInt32(MmsValue_getElement(dstAddress, 1));

                MmsValue* macAddress = MmsValue_getElement(dstAddress, 0);

                memcpy(commParameters.dstAddress, MmsValue_getOctetStringBuffer(macAddress), 6);

                if (mmsMapping->useIntegratedPublisher) {

                    if (self->gooseInterfaceId)
                        self->publisher = GoosePublisher_createEx(&commParameters, self->gooseInterfaceId, self->useVlanTag);
                    else
                        self->publisher = GoosePublisher_createEx(&commParameters, self->mmsMapping->gooseInterfaceId, self->useVlanTag);

                    if (self->publisher) {
                        self->minTime = MmsValue_toUint32(MmsValue_getElement(self->mmsValue, 6));
                        self->maxTime = MmsValue_toUint32(MmsValue_getElement(self->mmsValue, 7));

                        GoosePublisher_setTimeAllowedToLive(self->publisher, self->maxTime * 3);

                        GoosePublisher_setDataSetRef(self->publisher, self->dataSetRef);

                        GoosePublisher_setGoCbRef(self->publisher, self->goCBRef);

                        uint32_t confRev = MmsValue_toUint32(MmsValue_getElement(self->mmsValue, 3));

                        GoosePublisher_setConfRev(self->publisher, confRev);

                        bool needsCom = MmsValue_getBoolean(MmsValue_getElement(self->mmsValue, 4));

                        GoosePublisher_setNeedsCommission(self->publisher, needsCom);

                        if (self->goId != NULL)
                            GoosePublisher_setGoID(self->publisher, self->goId);

                        /* prepare data set values */
                        self->dataSetValues = LinkedList_create();

                        DataSetEntry* dataSetEntry = self->dataSet->fcdas;

                        while (dataSetEntry != NULL) {
                            LinkedList_add(self->dataSetValues, dataSetEntry->value);
                            dataSetEntry = dataSetEntry->sibling;
                        }

                    }
                    else {
                        if (DEBUG_IED_SERVER)
                            printf("IED_SERVER: Failed to create GOOSE publisher!\n");
                    }
                }

                self->goEna = true;

                retVal = true;

#if (CONFIG_IEC61850_SERVICE_TRACKING == 1)
                copyGCBValuesToTrackingObject(self);
                updateGenericTrackingObjectValues(self, IEC61850_SERVICE_TYPE_SET_GOCB_VALUES, DATA_ACCESS_ERROR_SUCCESS);
#endif /* (CONFIG_IEC61850_SERVICE_TRACKING == 1) */
            }

        }

    }
    else {
        if (DEBUG_IED_SERVER)
            printf("GoCB already enabled!\n");
    }

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_post(self->publisherMutex);
#endif

    return retVal;
}

void
MmsGooseControlBlock_disable(MmsGooseControlBlock self, MmsMapping* mmsMapping)
{
    if (MmsGooseControlBlock_isEnabled(self)) {
        MmsValue* goEna = MmsValue_getElement(self->mmsValue, 0);

        MmsValue_setBoolean(goEna, false);

        self->goEna = false;

#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_wait(self->publisherMutex);
#endif

        if (mmsMapping->useIntegratedPublisher) {
            if (self->publisher != NULL) {
                GoosePublisher_destroy(self->publisher);
                self->publisher = NULL;
                LinkedList_destroyStatic(self->dataSetValues);
                self->dataSetValues = NULL;
            }
        }

#if (CONFIG_IEC61850_SERVICE_TRACKING == 1)
        MmsDataAccessError retVal = DATA_ACCESS_ERROR_SUCCESS;
        copyGCBValuesToTrackingObject(self);
        updateGenericTrackingObjectValues(self, IEC61850_SERVICE_TYPE_SET_GOCB_VALUES, retVal);
#endif /* (CONFIG_IEC61850_SERVICE_TRACKING == 1) */

#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_post(self->publisherMutex);
#endif
    }
}


void
MmsGooseControlBlock_checkAndPublish(MmsGooseControlBlock self, uint64_t currentTime, MmsMapping* mapping)
{
    if (self->publisher) {
        if (currentTime >= self->nextPublishTime) {

            IedServer_lockDataModel(mapping->iedServer);

            if (currentTime >= self->nextPublishTime) {

#if (CONFIG_MMS_THREADLESS_STACK != 1)
                Semaphore_wait(self->publisherMutex);
#endif

                GoosePublisher_publish(self->publisher, self->dataSetValues);

                if (self->retransmissionsLeft > 0) {
                    self->nextPublishTime = currentTime + self->minTime;


                    if (self->retransmissionsLeft > 1)
                        GoosePublisher_setTimeAllowedToLive(self->publisher, self->minTime * 3);
                    else
                        GoosePublisher_setTimeAllowedToLive(self->publisher, self->maxTime * 3);

                    self->retransmissionsLeft--;
                }
                else {
                    GoosePublisher_setTimeAllowedToLive(self->publisher, self->maxTime * 3);

                    self->nextPublishTime = currentTime + self->maxTime;
                }

#if (CONFIG_MMS_THREADLESS_STACK != 1)
                Semaphore_post(self->publisherMutex);
#endif
            }

            IedServer_unlockDataModel(mapping->iedServer);

        }
        else if ((self->nextPublishTime - currentTime) > ((uint32_t) self->maxTime * 2)) {
            self->nextPublishTime = currentTime + self->minTime;
        }
    }
}

void
MmsGooseControlBlock_setStateChangePending(MmsGooseControlBlock self)
{
    self->stateChangePending = true;
}

void
MmsGooseControlBlock_publishNewState(MmsGooseControlBlock self)
{
    if (self->publisher == false)
        return;

    if (self->stateChangePending) {
#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_wait(self->publisherMutex);
#endif

    uint64_t currentTime = GoosePublisher_increaseStNum(self->publisher);

    self->retransmissionsLeft = CONFIG_GOOSE_EVENT_RETRANSMISSION_COUNT;

    if (self->retransmissionsLeft > 0) {
        self->nextPublishTime = currentTime + self->minTime;

        GoosePublisher_setTimeAllowedToLive(self->publisher, self->minTime * 3);
    }
    else {
        self->nextPublishTime = currentTime + self->maxTime;

        GoosePublisher_setTimeAllowedToLive(self->publisher, self->maxTime * 3);
    }

    GoosePublisher_publish(self->publisher, self->dataSetValues);

    self->stateChangePending = false;

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_post(self->publisherMutex);
#endif
    }
}

static MmsVariableSpecification*
createMmsGooseControlBlock(char* gcbName)
{
    MmsVariableSpecification* gcb = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    gcb->name = StringUtils_copyString(gcbName);
    gcb->type = MMS_STRUCTURE;
    gcb->typeSpec.structure.elementCount = 9;
    gcb->typeSpec.structure.elements = (MmsVariableSpecification**) GLOBAL_CALLOC(9, sizeof(MmsVariableSpecification*));

    MmsVariableSpecification* namedVariable;

    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("GoEna");
    namedVariable->type = MMS_BOOLEAN;

    gcb->typeSpec.structure.elements[0] = namedVariable;

    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("GoID");
    namedVariable->typeSpec.visibleString = -129;
    namedVariable->type = MMS_VISIBLE_STRING;

    gcb->typeSpec.structure.elements[1] = namedVariable;

    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("DatSet");
    namedVariable->typeSpec.visibleString = -129;
    namedVariable->type = MMS_VISIBLE_STRING;

    gcb->typeSpec.structure.elements[2] = namedVariable;

    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("ConfRev");
    namedVariable->type = MMS_UNSIGNED;
    namedVariable->typeSpec.unsignedInteger = 32;

    gcb->typeSpec.structure.elements[3] = namedVariable;

    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("NdsCom");
    namedVariable->type = MMS_BOOLEAN;

    gcb->typeSpec.structure.elements[4] = namedVariable;

    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("DstAddress");
    MmsMapping_createPhyComAddrStructure(namedVariable);

    gcb->typeSpec.structure.elements[5] = namedVariable;

    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("MinTime");
    namedVariable->type = MMS_UNSIGNED;
    namedVariable->typeSpec.unsignedInteger = 32;

    gcb->typeSpec.structure.elements[6] = namedVariable;

    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("MaxTime");
    namedVariable->type = MMS_UNSIGNED;
    namedVariable->typeSpec.unsignedInteger = 32;

    gcb->typeSpec.structure.elements[7] = namedVariable;

    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("FixedOffs");
    namedVariable->type = MMS_BOOLEAN;

    gcb->typeSpec.structure.elements[8] = namedVariable;

    return gcb;
}

static GSEControlBlock*
getGCBForLogicalNodeWithIndex(MmsMapping* self, LogicalNode* logicalNode, int index)
{
    int gseCount = 0;

    GSEControlBlock* gcb = self->model->gseCBs;

    /* Iterate list of GoCBs */
    while (gcb != NULL ) {
        if (gcb->parent == logicalNode) {
            if (gseCount == index)
                return gcb;

            gseCount++;
        }

        gcb = gcb->sibling;
    }

    return NULL ;
}

static char*
createDataSetReference(char* domainName, char* lnName, char* dataSetName)
{
    char* dataSetReference;

    dataSetReference = StringUtils_createString(5, domainName, "/", lnName, "$", dataSetName);

    return dataSetReference;
}

void
GOOSE_sendPendingEvents(MmsMapping* self)
{
    if (self->useIntegratedPublisher) {
        LinkedList element = self->gseControls;

        while ((element = LinkedList_getNext(element)) != NULL) {
            MmsGooseControlBlock gcb = (MmsGooseControlBlock) element->data;

            if (MmsGooseControlBlock_isEnabled(gcb)) {
                MmsGooseControlBlock_publishNewState(gcb);
            }
        }
    }
}

MmsVariableSpecification*
GOOSE_createGOOSEControlBlocks(MmsMapping* self, MmsDomain* domain,
        LogicalNode* logicalNode, int gseCount)
{
    MmsVariableSpecification* namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1,
            sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("GO");
    namedVariable->type = MMS_STRUCTURE;

    namedVariable->typeSpec.structure.elementCount = gseCount;
    namedVariable->typeSpec.structure.elements = (MmsVariableSpecification**) GLOBAL_CALLOC(gseCount,
            sizeof(MmsVariableSpecification*));

    int currentGCB = 0;

    while (currentGCB < gseCount) {
        GSEControlBlock* gooseControlBlock = getGCBForLogicalNodeWithIndex(
                self, logicalNode, currentGCB);

        MmsVariableSpecification* gseTypeSpec = createMmsGooseControlBlock(gooseControlBlock->name);

        MmsValue* gseValues = MmsValue_newStructure(gseTypeSpec);

        namedVariable->typeSpec.structure.elements[currentGCB] = gseTypeSpec;

        MmsGooseControlBlock mmsGCB = MmsGooseControlBlock_create();


        mmsGCB->goCBRef = StringUtils_createString(5, MmsDomain_getName(domain), "/", logicalNode->name,
                "$GO$", gooseControlBlock->name);

        if (gooseControlBlock->appId != NULL) {
            MmsValue* goID = MmsValue_getElement(gseValues, 1);

            MmsValue_setVisibleString(goID, gooseControlBlock->appId);

            mmsGCB->goId = StringUtils_copyString(gooseControlBlock->appId);
        }

        if ((gooseControlBlock->dataSetName != NULL) && (gooseControlBlock->dataSetName[0] != 0)) {
        	mmsGCB->dataSetRef = createDataSetReference(MmsDomain_getName(domain),
        			logicalNode->name, gooseControlBlock->dataSetName);
        }
        else {
        	mmsGCB->dataSetRef = NULL;
        }

        MmsValue* dataSetRef = MmsValue_getElement(gseValues, 2);

        MmsValue_setVisibleString(dataSetRef, mmsGCB->dataSetRef);

        /* Set communication parameters */
        uint8_t priority = CONFIG_GOOSE_DEFAULT_PRIORITY;
        uint8_t dstAddr[] = CONFIG_GOOSE_DEFAULT_DST_ADDRESS;
        uint16_t vid = CONFIG_GOOSE_DEFAULT_VLAN_ID;
        uint16_t appId = CONFIG_GOOSE_DEFAULT_APPID;

        if (gooseControlBlock->address != NULL) {
            priority = gooseControlBlock->address->vlanPriority;
            vid = gooseControlBlock->address->vlanId;
            appId = gooseControlBlock->address->appId;

            int i;
            for (i = 0; i < 6; i++) {
                dstAddr[i] = gooseControlBlock->address->dstAddress[i];
            }
        }

        MmsValue* dstAddress = MmsValue_getElement(gseValues, 5);
        MmsValue* addr = MmsValue_getElement(dstAddress, 0);
        MmsValue_setOctetString(addr, dstAddr, 6);

        MmsValue* prio = MmsValue_getElement(dstAddress, 1);
        MmsValue_setUint8(prio, priority);

        MmsValue* vlanId = MmsValue_getElement(dstAddress, 2);
        MmsValue_setUint16(vlanId, vid);

        MmsValue* appIdVal = MmsValue_getElement(dstAddress, 3);
        MmsValue_setUint16(appIdVal, appId);

        MmsValue* confRef = MmsValue_getElement(gseValues, 3);

        MmsValue_setUint32(confRef, gooseControlBlock->confRev);

        mmsGCB->dataSet = NULL;

        mmsGCB->mmsValue = gseValues;
        mmsGCB->mmsType = gseTypeSpec;
        mmsGCB->name = gooseControlBlock->name;

        mmsGCB->domain = domain;
        mmsGCB->logicalNode = logicalNode;

        if (gooseControlBlock->minTime == -1)
            mmsGCB->minTime = CONFIG_GOOSE_EVENT_RETRANSMISSION_INTERVAL;
        else
            mmsGCB->minTime = gooseControlBlock->minTime;

        if (gooseControlBlock->maxTime == -1)
            mmsGCB->maxTime = CONFIG_GOOSE_STABLE_STATE_TRANSMISSION_INTERVAL;
        else
            mmsGCB->maxTime = gooseControlBlock->maxTime;

        MmsValue* minTime = MmsValue_getElement(gseValues, 6);
        MmsValue_setUint32(minTime, mmsGCB->minTime);

        MmsValue* maxTime = MmsValue_getElement(gseValues, 7);
        MmsValue_setUint32(maxTime, mmsGCB->maxTime);

        if (mmsGCB->dataSetRef)
            setNdsCom(mmsGCB, false);
        else
            setNdsCom(mmsGCB, true);

        mmsGCB->mmsMapping = self;

        mmsGCB->stateChangePending = false;

        LinkedList_add(self->gseControls, mmsGCB);

        currentGCB++;
    }

    return namedVariable;
}

#endif /* (CONFIG_INCLUDE_GOOSE_SUPPORT == 1) */

