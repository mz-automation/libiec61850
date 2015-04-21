/*
 *  client_report.c
 *
 *  Client implementation for IEC 61850 reporting.
 *
 *  Copyright 2013, 2014 Michael Zillgith
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

#include "iec61850_client.h"

#include "stack_config.h"

#include "ied_connection_private.h"

#include "libiec61850_platform_includes.h"

struct sClientReport
{
    ReportCallbackFunction callback;
    void* callbackParameter;
    char* rcbReference;
    char* rptId;
    MmsValue* entryId;
    MmsValue* dataReferences;
    MmsValue* dataSetValues;
    ReasonForInclusion* reasonForInclusion;

    /* Presence flags for optional elements */
    bool hasDataSetName;
    bool hasReasonForInclusion;
    bool hasSequenceNumber;
    bool hasDataReference;
    bool hasConfRev;
    bool hasTimestamp;
    bool hasBufOverflow;

    uint64_t timestamp;
    uint16_t seqNum;
    uint32_t confRev;
    bool bufOverflow;
};

char*
ReasonForInclusion_getValueAsString(ReasonForInclusion reasonCode)
{
    switch (reasonCode) {
    case REASON_NOT_INCLUDED:
        return "not-included";
    case REASON_DATA_CHANGE:
        return "data-change";
    case REASON_DATA_UPDATE:
        return "data-update";
    case REASON_QUALITY_CHANGE:
        return "quality-change";
    case REASON_GI:
        return "GI";
    case REASON_INTEGRITY:
        return "integrity";
    default:
        return "unknown";
    }
}

ClientReport
ClientReport_create()
{
    ClientReport self = (ClientReport) GLOBAL_CALLOC(1, sizeof(struct sClientReport));

    return self;
}

void
ClientReport_destroy(ClientReport self)
{
    if (self->entryId)
        MmsValue_delete(self->entryId);

    GLOBAL_FREEMEM(self->rcbReference);

    if (self->rptId != NULL)
        GLOBAL_FREEMEM(self->rptId);

    if (self->dataSetValues != NULL)
        MmsValue_delete(self->dataSetValues);

    if (self->dataReferences != NULL)
        MmsValue_delete(self->dataReferences);

    if (self->reasonForInclusion != NULL)
        GLOBAL_FREEMEM(self->reasonForInclusion);

    GLOBAL_FREEMEM(self);
}

char*
ClientReport_getRcbReference(ClientReport self)
{
    return self->rcbReference;
}

char*
ClientReport_getRptId(ClientReport self)
{
    return self->rptId;
}

ReasonForInclusion
ClientReport_getReasonForInclusion(ClientReport self, int elementIndex)
{
    if (self->reasonForInclusion != NULL)
        return self->reasonForInclusion[elementIndex];
    else
        return REASON_NOT_INCLUDED;
}

MmsValue*
ClientReport_getEntryId(ClientReport self)
{
    return self->entryId;
}

bool
ClientReport_hasTimestamp(ClientReport self)
{
    return self->hasTimestamp;
}

uint64_t
ClientReport_getTimestamp(ClientReport self)
{
    return self->timestamp;
}


bool
ClientReport_hasSeqNum(ClientReport self)
{
    return self->hasSequenceNumber;
}

uint16_t
ClientReport_getSeqNum(ClientReport self)
{
    return self->seqNum;
}

bool
ClientReport_hasDataSetName(ClientReport self)
{
    return self->hasDataSetName;
}

bool
ClientReport_hasReasonForInclusion(ClientReport self)
{
    return self->hasReasonForInclusion;
}

bool
ClientReport_hasConfRev(ClientReport self)
{
    return self->hasConfRev;
}

uint32_t
ClientReport_getConfRev(ClientReport self)
{
    return self->confRev;
}

bool
ClientReport_hasBufOvfl(ClientReport self)
{
    return self->hasBufOverflow;
}

bool
ClientReport_getBufOvfl(ClientReport self)
{
    return self->bufOverflow;
}

bool
ClientReport_hasDataReference(ClientReport self)
{
    return self->hasDataReference;
}

const char*
ClientReport_getDataReference(ClientReport self, int elementIndex)
{
    char* dataReference = NULL;

    if (self->dataReferences != NULL) {
        MmsValue* dataRefValue = MmsValue_getElement(self->dataReferences, elementIndex);

        if (dataRefValue != NULL) {
            if (MmsValue_getType(dataRefValue) == MMS_VISIBLE_STRING) {
                return MmsValue_toString(dataRefValue);
            }
        }
    }

    return dataReference;
}

MmsValue*
ClientReport_getDataSetValues(ClientReport self)
{
    return self->dataSetValues;
}

static ClientReport
lookupReportHandler(IedConnection self, const char* rcbReference)
{
    LinkedList element = LinkedList_getNext(self->enabledReports);

    while (element != NULL) {
        ClientReport report = (ClientReport) element->data;

        if (strcmp(report->rcbReference, rcbReference) == 0)
            return report;

        element = LinkedList_getNext(element);
    }

    return NULL;
}

void
IedConnection_installReportHandler(IedConnection self, const char* rcbReference, const char* rptId, ReportCallbackFunction handler,
        void* handlerParameter)
{
    ClientReport report = lookupReportHandler(self, rcbReference);

    if (report != NULL) {
        IedConnection_uninstallReportHandler(self, rcbReference);

        if (DEBUG_IED_CLIENT)
            printf("DEBUG_IED_CLIENT: Removed existing report callback handler for %s\n", rcbReference);
    }

    report = ClientReport_create();
    report->callback = handler;
    report->callbackParameter = handlerParameter;
    report->rcbReference = copyString(rcbReference);

    if (rptId != NULL)
        report->rptId = copyString(rptId);
    else
        report->rptId = NULL;

    LinkedList_add(self->enabledReports, report);

    if (DEBUG_IED_CLIENT)
        printf("DEBUG_IED_CLIENT: Installed new report callback handler for %s\n", rcbReference);
}

void
IedConnection_uninstallReportHandler(IedConnection self, const char* rcbReference)
{
    ClientReport report = lookupReportHandler(self, rcbReference);

    if (report != NULL) {
        LinkedList_remove(self->enabledReports, report);
        ClientReport_destroy(report);
    }
}

void
IedConnection_triggerGIReport(IedConnection self, IedClientError* error, const char* rcbReference)
{
    char domainId[65];
    char itemId[129];

    MmsMapping_getMmsDomainFromObjectReference(rcbReference, domainId);

    strcpy(itemId, rcbReference + strlen(domainId) + 1);

    StringUtils_replace(itemId, '.', '$');

    int itemIdLen = strlen(itemId);

    strcpy(itemId + itemIdLen, "$GI");

    MmsConnection mmsCon = IedConnection_getMmsConnection(self);

    MmsError mmsError;

    MmsValue* gi = MmsValue_newBoolean(true);

    MmsConnection_writeVariable(mmsCon, &mmsError, domainId, itemId, gi);

    MmsValue_delete(gi);

    if (mmsError != MMS_ERROR_NONE) {
        if (DEBUG_IED_CLIENT)
            printf("DEBUG_IED_CLIENT: failed to trigger GI for %s!\n", rcbReference);

        *error = iedConnection_mapMmsErrorToIedError(mmsError);
    }
    else {
        *error = IED_ERROR_OK;
    }
}

void
private_IedConnection_handleReport(IedConnection self, MmsValue* value)
{
    MmsValue* rptIdValue = MmsValue_getElement(value, 0);

    LinkedList element = LinkedList_getNext(self->enabledReports);
    ClientReport matchingReport = NULL;

    while (element != NULL) {
        ClientReport report = (ClientReport) element->data;

        char* rptId = report->rptId;

        if (rptId == NULL)
            rptId = report->rcbReference;

        if (strcmp(MmsValue_toString(rptIdValue), rptId) == 0) {
            matchingReport = report;
            break;
        }

        element = LinkedList_getNext(element);
    }

    if (matchingReport == NULL)
        goto exit_function;

    matchingReport->hasSequenceNumber = false;
    matchingReport->hasTimestamp = false;
    matchingReport->hasReasonForInclusion = false;
    matchingReport->hasDataReference = false;
    matchingReport->hasConfRev = false;
    matchingReport->hasDataSetName = false;
    matchingReport->hasBufOverflow = false;

   if (DEBUG_IED_CLIENT)
        printf("DEBUG_IED_CLIENT: received report with ID %s\n", MmsValue_toString(rptIdValue));

    MmsValue* optFlds = MmsValue_getElement(value, 1);

    int inclusionIndex = 2;

    /* has sequence-number */
    if (MmsValue_getBitStringBit(optFlds, 1) == true) {

        MmsValue* seqNum = MmsValue_getElement(value, inclusionIndex);

        if (MmsValue_getType(seqNum) == MMS_UNSIGNED) {
            matchingReport->seqNum = (uint16_t) MmsValue_toUint32(seqNum);
            matchingReport->hasSequenceNumber = true;
        }

        inclusionIndex++;
    }

    /* has report-timestamp */
    if (MmsValue_getBitStringBit(optFlds, 2) == true) {
        MmsValue* timeStampValue = MmsValue_getElement(value, inclusionIndex);

        if (MmsValue_getType(timeStampValue) == MMS_BINARY_TIME) {
            matchingReport->hasTimestamp = true;
            matchingReport->timestamp = MmsValue_getBinaryTimeAsUtcMs(timeStampValue);
        }

        inclusionIndex++;
    }

    /* check if data set name is present */
    if (MmsValue_getBitStringBit(optFlds, 4) == true) {
        matchingReport->hasDataSetName = true;
        inclusionIndex++;
    }

    if (DEBUG_IED_CLIENT)
        printf("DEBUG_IED_CLIENT: Found enabled report!\n");

    /* check bufOvfl */
    if (MmsValue_getBitStringBit(optFlds, 6) == true) {
        MmsValue* bufOverflow = MmsValue_getElement(value, inclusionIndex);

        matchingReport->hasBufOverflow = true;
        matchingReport->bufOverflow = MmsValue_getBoolean(bufOverflow);

        inclusionIndex++;
    }

    /* check for entryId */
    if (MmsValue_getBitStringBit(optFlds, 7) == true) {
        MmsValue* entryId = MmsValue_getElement(value, inclusionIndex);

        if (matchingReport->entryId != NULL) {

            if (!MmsValue_update(matchingReport->entryId, entryId)) {
                MmsValue_delete(matchingReport->entryId);
                matchingReport->entryId = MmsValue_clone(entryId);
            }
        }
        else {
            matchingReport->entryId = MmsValue_clone(entryId);
        }

        inclusionIndex++;
    }

    /* check for confRev */
    if (MmsValue_getBitStringBit(optFlds, 8) == true) {
        MmsValue* confRev = MmsValue_getElement(value, inclusionIndex);

        if (MmsValue_getType(confRev) == MMS_UNSIGNED) {
            matchingReport->confRev = MmsValue_toUint32(confRev);
            matchingReport->hasConfRev = true;
        }

        inclusionIndex++;
    }


    /* skip segmentation fields */
    if (MmsValue_getBitStringBit(optFlds, 9) == true)
        inclusionIndex += 2;

    MmsValue* inclusion = MmsValue_getElement(value, inclusionIndex);

    int dataSetSize = MmsValue_getBitStringSize(inclusion);

    int includedElements = MmsValue_getNumberOfSetBits(inclusion);

    if (DEBUG_IED_CLIENT)
        printf("DEBUG_IED_CLIENT: Report includes %i data set elements of %i\n", includedElements,
                dataSetSize);

    int valueIndex = inclusionIndex + 1;

    /* parse data-references if required */
    if (MmsValue_getBitStringBit(optFlds, 5) == true) {

        if (matchingReport->dataReferences == NULL)
            matchingReport->dataReferences = MmsValue_createEmptyArray(dataSetSize);

        matchingReport->hasDataReference = true;

        int elementIndex;

        for (elementIndex = 0; elementIndex < dataSetSize; elementIndex++) {
            if (MmsValue_getBitStringBit(inclusion, elementIndex) == true) {
                MmsValue* dataSetElement = MmsValue_getElement(matchingReport->dataReferences, elementIndex);

                if (dataSetElement == NULL) {
                    dataSetElement = MmsValue_clone(MmsValue_getElement(value, valueIndex));

                    MmsValue_setElement(matchingReport->dataReferences, elementIndex, dataSetElement);
                }

                valueIndex += 1;
            }
        }

        // valueIndex += includedElements;
    }

    int i;

    if (matchingReport->dataSetValues == NULL) {
        matchingReport->dataSetValues = MmsValue_createEmptyArray(dataSetSize);
        matchingReport->reasonForInclusion = (ReasonForInclusion*)
                GLOBAL_MALLOC(sizeof(ReasonForInclusion) * dataSetSize);

        int elementIndex;

        for (elementIndex = 0; elementIndex < dataSetSize; elementIndex++)
            matchingReport->reasonForInclusion[elementIndex] = REASON_NOT_INCLUDED;

    }

    MmsValue* dataSetValues = matchingReport->dataSetValues;

    bool hasReasonForInclusion = MmsValue_getBitStringBit(optFlds, 3);

    if (hasReasonForInclusion)
        matchingReport->hasReasonForInclusion = true;

    int reasonForInclusionIndex = valueIndex + includedElements;

    for (i = 0; i < dataSetSize; i++) {
        if (MmsValue_getBitStringBit(inclusion, i) == true) {

            MmsValue* dataSetElement = MmsValue_getElement(dataSetValues, i);

            MmsValue* newElementValue = MmsValue_getElement(value, valueIndex);

            if (dataSetElement == NULL)
                MmsValue_setElement(dataSetValues, i, MmsValue_clone(newElementValue));
            else
                MmsValue_update(dataSetElement, newElementValue);

            if (DEBUG_IED_CLIENT)
                printf("DEBUG_IED_CLIENT:  update element value type: %i\n", MmsValue_getType(newElementValue));

            valueIndex++;

            if (hasReasonForInclusion) {
                MmsValue* reasonForInclusion = MmsValue_getElement(value, reasonForInclusionIndex);

                if (MmsValue_getBitStringBit(reasonForInclusion, 1) == true)
                    matchingReport->reasonForInclusion[i] = REASON_DATA_CHANGE;
                else if (MmsValue_getBitStringBit(reasonForInclusion, 2) == true)
                    matchingReport->reasonForInclusion[i] = REASON_QUALITY_CHANGE;
                else if (MmsValue_getBitStringBit(reasonForInclusion, 3) == true)
                    matchingReport->reasonForInclusion[i] = REASON_DATA_UPDATE;
                else if (MmsValue_getBitStringBit(reasonForInclusion, 4) == true)
                    matchingReport->reasonForInclusion[i] = REASON_INTEGRITY;
                else if (MmsValue_getBitStringBit(reasonForInclusion, 5) == true)
                    matchingReport->reasonForInclusion[i] = REASON_GI;
            }
            else {
                matchingReport->reasonForInclusion[i] = REASON_UNKNOWN;
            }
        }
        else {
            matchingReport->reasonForInclusion[i] = REASON_NOT_INCLUDED;
        }
    }

    if (matchingReport->callback != NULL)
        matchingReport->callback(matchingReport->callbackParameter, matchingReport);

exit_function:
    return;
}

