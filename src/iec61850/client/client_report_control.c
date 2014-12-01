/*
 *  client_report_control.c
 *
 *  Implementation of the ClientReportControlBlock class
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

#include "mms_mapping.h"

static bool
isBufferedRcb(char* objectReference)
{
    char* separator = strchr(objectReference, '.');

    if (separator == NULL)
        return false; //TODO report an error

    if (*(separator + 1) == 'B')
        return true;
    else
        return false;
}

ClientReportControlBlock
ClientReportControlBlock_create(char* objectReference)
{
    ClientReportControlBlock self = (ClientReportControlBlock) GLOBAL_CALLOC(1, sizeof(struct sClientReportControlBlock));

    self->objectReference = copyString(objectReference);
    self->isBuffered = isBufferedRcb(objectReference);

    return self;
}

void
ClientReportControlBlock_destroy(ClientReportControlBlock self)
{
    GLOBAL_FREEMEM(self->objectReference);

    MmsValue_deleteIfNotNull(self->rptId);
    MmsValue_deleteIfNotNull(self->rptEna);
    MmsValue_deleteIfNotNull(self->resv);
    MmsValue_deleteIfNotNull(self->datSet);
    MmsValue_deleteIfNotNull(self->confRev);
    MmsValue_deleteIfNotNull(self->optFlds);
    MmsValue_deleteIfNotNull(self->bufTm);
    MmsValue_deleteIfNotNull(self->sqNum);
    MmsValue_deleteIfNotNull(self->trgOps);
    MmsValue_deleteIfNotNull(self->intgPd);
    MmsValue_deleteIfNotNull(self->gi);
    MmsValue_deleteIfNotNull(self->purgeBuf);
    MmsValue_deleteIfNotNull(self->entryId);
    MmsValue_deleteIfNotNull(self->timeOfEntry);
    MmsValue_deleteIfNotNull(self->resvTms);
    MmsValue_deleteIfNotNull(self->owner);

    GLOBAL_FREEMEM(self);
}

char*
ClientReportControlBlock_getObjectReference(ClientReportControlBlock self)
{
    return self->objectReference;
}

bool
ClientReportControlBlock_isBuffered(ClientReportControlBlock self)
{
    return self->isBuffered;
}

char*
ClientReportControlBlock_getRptId(ClientReportControlBlock self)
{
    if (self->rptId != NULL)
        return MmsValue_toString(self->rptId);
    else
        return NULL;
}

void
ClientReportControlBlock_setRptId(ClientReportControlBlock self, char* rptId)
{
    if (self->rptId == NULL)
        self->rptId = MmsValue_newVisibleString(rptId);
    else
        MmsValue_setVisibleString(self->rptId, rptId);
}


bool
ClientReportControlBlock_getRptEna(ClientReportControlBlock self)
{
    if (self->rptEna != NULL) {
        return MmsValue_getBoolean(self->rptEna);
    }
    else
        return false;
}

void
ClientReportControlBlock_setRptEna(ClientReportControlBlock self, bool rptEna)
{
    if (self->rptEna == NULL)
        self->rptEna = MmsValue_newBoolean(rptEna);
    else
        MmsValue_setBoolean(self->rptEna, rptEna);
}

bool
ClientReportControlBlock_getResv(ClientReportControlBlock self)
{
    if (self->resv != NULL) {
        return MmsValue_getBoolean(self->resv);
    }
    else
        return false;
}

void
ClientReportControlBlock_setResv(ClientReportControlBlock self, bool resv)
{
    if (self->resv == NULL)
        self->resv = MmsValue_newBoolean(resv);
    else
        MmsValue_setBoolean(self->resv, resv);
}

char*
ClientReportControlBlock_getDataSetReference(ClientReportControlBlock self)
{
    if (self->datSet != NULL)
        return MmsValue_toString(self->datSet);
    else
        return NULL;
}

void
ClientReportControlBlock_setDataSetReference(ClientReportControlBlock self, char* dataSetReference)
{
    if (self->datSet == NULL)
        self->datSet = MmsValue_newVisibleString(dataSetReference);
    else
        MmsValue_setVisibleString(self->datSet, dataSetReference);
}

uint32_t
ClientReportControlBlock_getConfRev(ClientReportControlBlock self)
{
    if (self->confRev != NULL)
        return MmsValue_toUint32(self->confRev);
    else
        return 0;
}

int
ClientReportControlBlock_getOptFlds(ClientReportControlBlock self)
{
    return (MmsValue_getBitStringAsInteger(self->optFlds) / 2);
}

void
ClientReportControlBlock_setOptFlds(ClientReportControlBlock self, int optFlds)
{
    if (self->optFlds == NULL)
        self->optFlds = MmsValue_newBitString(10);

    MmsValue_setBitStringFromInteger(self->optFlds, optFlds * 2); /* bit 0 is reserved in MMS mapping */
}

uint32_t
ClientReportControlBlock_getBufTm(ClientReportControlBlock self)
{
    if (self->bufTm != NULL)
        return MmsValue_toUint32(self->bufTm);
    else
        return 0;
}

void
ClientReportControlBlock_setBufTm(ClientReportControlBlock self, uint32_t bufTm)
{
    if (self->bufTm == NULL)
        self->bufTm = MmsValue_newUnsignedFromUint32(bufTm);
    else
        MmsValue_setUint32(self->bufTm, bufTm);
}

uint16_t
ClientReportControlBlock_getSqNum(ClientReportControlBlock self)
{
    if (self->sqNum != NULL)
        return (uint16_t) MmsValue_toUint32(self->sqNum);
    else
        return 0;
}

int
ClientReportControlBlock_getTrgOps(ClientReportControlBlock self)
{
    int triggerOps = 0;

    if (self->trgOps != NULL) {
        if (MmsValue_getBitStringBit(self->trgOps, 1))
            triggerOps += TRG_OPT_DATA_CHANGED;
        if (MmsValue_getBitStringBit(self->trgOps, 2))
            triggerOps += TRG_OPT_QUALITY_CHANGED;
        if (MmsValue_getBitStringBit(self->trgOps, 3))
            triggerOps += TRG_OPT_DATA_UPDATE;
        if (MmsValue_getBitStringBit(self->trgOps, 4))
            triggerOps += TRG_OPT_INTEGRITY;
        if (MmsValue_getBitStringBit(self->trgOps, 5))
            triggerOps += TRG_OPT_GI;
    }

    return triggerOps;
}

void
ClientReportControlBlock_setTrgOps(ClientReportControlBlock self, int trgOps)
{
    if (self->trgOps == NULL)
        self->trgOps = MmsValue_newBitString(6);

    MmsValue_setBitStringFromInteger(self->trgOps, trgOps << 1);
}

uint32_t
ClientReportControlBlock_getIntgPd(ClientReportControlBlock self)
{
    if (self->intgPd != NULL)
        return MmsValue_toUint32(self->intgPd);
    else
        return 0;
}

void
ClientReportControlBlock_setIntgPd(ClientReportControlBlock self, uint32_t intgPd)
{
    if (self->intgPd == NULL)
        self->intgPd = MmsValue_newUnsignedFromUint32(intgPd);
    else
        MmsValue_setUint32(self->intgPd, intgPd);
}

bool
ClientReportControlBlock_getGI(ClientReportControlBlock self)
{
    if (self->gi != NULL)
        return MmsValue_getBoolean(self->gi);
    else
        return false;
}

void
ClientReportControlBlock_setGI(ClientReportControlBlock self, bool gi)
{
    if (self->gi == NULL)
        self->gi = MmsValue_newBoolean(gi);
    else
        MmsValue_setBoolean(self->gi, gi);
}

bool
ClientReportControlBlock_getPurgeBuf(ClientReportControlBlock self)
{
    if (self->purgeBuf != NULL)
        return MmsValue_getBoolean(self->purgeBuf);
    else
        return false;
}

void
ClientReportControlBlock_setPurgeBuf(ClientReportControlBlock self, bool purgeBuf)
{
    if (self->purgeBuf == NULL)
        self->purgeBuf = MmsValue_newBoolean(purgeBuf);
    else
        MmsValue_setBoolean(self->purgeBuf, purgeBuf);
}

int16_t
ClientReportControlBlock_getResvTms(ClientReportControlBlock self)
{
    if (self->resvTms != NULL)
        return (int16_t) MmsValue_toInt32(self->resvTms);
    else
        return 0;
}

void
ClientReportControlBlock_setResvTms(ClientReportControlBlock self, int16_t resvTms)
{
    if (self->resvTms == NULL)
        self->resvTms = MmsValue_newIntegerFromInt16(resvTms);
    else
        MmsValue_setInt32(self->resvTms, (int32_t) resvTms);
}

MmsValue*
ClientReportControlBlock_getEntryId(ClientReportControlBlock self)
{
    return self->entryId;
}

void
ClientReportControlBlock_setEntryId(ClientReportControlBlock self, MmsValue* entryId)
{
    if (self->entryId != NULL) {
        MmsValue_update(self->entryId, entryId);
    }
    else {
        if (MmsValue_getType(entryId) != MMS_OCTET_STRING) {
            if (DEBUG_IED_CLIENT)
                printf("IED_CLIENT: ClientReportControlBlock_setEntryId invalid argument type\n");
        }
        else
            self->entryId = MmsValue_clone(entryId);
    }
}

uint64_t
ClientReportControlBlock_getEntryTime(ClientReportControlBlock self)
{
    if (self->timeOfEntry != NULL)
        return MmsValue_getBinaryTimeAsUtcMs(self->timeOfEntry);
    else
        return 0;
}

MmsValue*
ClientReportControlBlock_getOwner(ClientReportControlBlock self)
{
    return self->owner;
}

static void
updateOrClone(MmsValue** valuePtr, MmsValue* values, int index)
{
    if (*valuePtr != NULL)
        MmsValue_update(*valuePtr, MmsValue_getElement(values, index));
    else
        *valuePtr = MmsValue_clone(MmsValue_getElement(values, index));
}

bool
private_ClientReportControlBlock_updateValues(ClientReportControlBlock self, MmsValue* values)
{
    int rcbElementCount = MmsValue_getArraySize(values);

    updateOrClone(&(self->rptId), values, 0);
    updateOrClone(&(self->rptEna), values, 1);

    if (self->isBuffered) {
        updateOrClone(&(self->datSet), values, 2);
        updateOrClone(&(self->confRev), values, 3);
        updateOrClone(&(self->optFlds), values, 4);
        updateOrClone(&(self->bufTm), values, 5);
        updateOrClone(&(self->sqNum), values, 6);
        updateOrClone(&(self->trgOps), values,7);
        updateOrClone(&(self->intgPd), values, 8);
        updateOrClone(&(self->gi), values, 9);
        updateOrClone(&(self->purgeBuf), values, 10);
        updateOrClone(&(self->entryId), values, 11);
        updateOrClone(&(self->timeOfEntry), values, 12);

        if (rcbElementCount > 13) {
            MmsValue* element13 = MmsValue_getElement(values, 13);

            if (MmsValue_getType(element13) == MMS_OCTET_STRING)
                updateOrClone(&(self->owner), values, 13);
            else {
                updateOrClone(&(self->resvTms), values, 13);

                if (rcbElementCount > 14)
                    updateOrClone(&(self->owner), values, 14);
            }
        }

    }
    else {
        updateOrClone(&(self->resv), values, 2);
        updateOrClone(&(self->datSet), values, 3);
        updateOrClone(&(self->confRev), values, 4);
        updateOrClone(&(self->optFlds), values, 5);
        updateOrClone(&(self->bufTm), values, 6);
        updateOrClone(&(self->sqNum), values, 7);
        updateOrClone(&(self->trgOps), values, 8);
        updateOrClone(&(self->intgPd), values, 9);
        updateOrClone(&(self->gi), values, 10);

        if (rcbElementCount == 12) /* owner is optional */
            updateOrClone(&(self->owner), values, 11);
    }

	return true;
}



ClientReportControlBlock
IedConnection_getRCBValues(IedConnection self, IedClientError* error, char* rcbReference,
        ClientReportControlBlock updateRcb)
{
    MmsError mmsError = MMS_ERROR_NONE;

    ClientReportControlBlock returnRcb = updateRcb;

    char domainId[65];
    char itemId[129];

    MmsMapping_getMmsDomainFromObjectReference(rcbReference, domainId);

    strcpy(itemId, rcbReference + strlen(domainId) + 1);

    StringUtils_replace(itemId, '.', '$');

    if (DEBUG_IED_CLIENT)
        printf("DEBUG_IED_CLIENT: readRCBValues for %s\n", rcbReference);

    MmsValue* rcb = MmsConnection_readVariable(self->connection, &mmsError, domainId, itemId);

    if (mmsError != MMS_ERROR_NONE) {
        *error = iedConnection_mapMmsErrorToIedError(mmsError);

        return NULL;
    }

    if (rcb == NULL) {
        *error = IED_ERROR_OBJECT_DOES_NOT_EXIST;
        return NULL;
    }

    if (MmsValue_getType(rcb) != MMS_STRUCTURE) {
        if (DEBUG_IED_CLIENT)
            printf("DEBUG_IED_CLIENT: getRCBValues returned wrong type!\n");

        MmsValue_delete(rcb);

        *error = IED_ERROR_UNKNOWN;
        return NULL;
    }

    if (returnRcb == NULL)
        returnRcb = ClientReportControlBlock_create(rcbReference);

    private_ClientReportControlBlock_updateValues(returnRcb, rcb);

    MmsValue_delete(rcb);

    *error = IED_ERROR_OK;

    return returnRcb;
}

void
IedConnection_setRCBValues(IedConnection self, IedClientError* error, ClientReportControlBlock rcb,
        uint32_t parametersMask, bool singleRequest)
{
    *error = IED_ERROR_OK;

    MmsError mmsError = MMS_ERROR_NONE;

    bool isBuffered = ClientReportControlBlock_isBuffered(rcb);

    char domainId[65];
    char itemId[129];

    char* rcbReference = ClientReportControlBlock_getObjectReference(rcb);

    MmsMapping_getMmsDomainFromObjectReference(rcbReference, domainId);

    strcpy(itemId, rcbReference + strlen(domainId) + 1);

    StringUtils_replace(itemId, '.', '$');

    if (DEBUG_IED_CLIENT)
        printf("DEBUG_IED_CLIENT: setRCBValues for %s\n", rcbReference);

    int itemIdLen = strlen(itemId);

    /* create the list of requested itemIds references */
    LinkedList itemIds = LinkedList_create();
    LinkedList values = LinkedList_create();

    /* add resv/resvTms as first element and rptEna as last element */
    if (parametersMask & RCB_ELEMENT_RESV) {
        if (isBuffered)
            goto error_invalid_parameter;

        strcpy(itemId + itemIdLen, "$Resv");

        LinkedList_add(itemIds, copyString(itemId));
        LinkedList_add(values, rcb->resv);
    }

    if (parametersMask & RCB_ELEMENT_RESV_TMS) {
        if (!isBuffered)
            goto error_invalid_parameter;

        strcpy(itemId + itemIdLen, "$ResvTms");

        LinkedList_add(itemIds, copyString(itemId));
        LinkedList_add(values, rcb->resvTms);
    }

    if (parametersMask & RCB_ELEMENT_RPT_ID) {
        strcpy(itemId + itemIdLen, "$RptID");

        LinkedList_add(itemIds, copyString(itemId));
        LinkedList_add(values, rcb->rptId);
    }

    if (parametersMask & RCB_ELEMENT_DATSET) {
        strcpy(itemId + itemIdLen, "$DatSet");

        LinkedList_add(itemIds, copyString(itemId));
        LinkedList_add(values, rcb->datSet);
    }

    if (parametersMask & RCB_ELEMENT_ENTRY_ID) {
        strcpy(itemId + itemIdLen, "$EntryID");

        LinkedList_add(itemIds, copyString(itemId));
        LinkedList_add(values, rcb->entryId);
    }

    if (parametersMask & RCB_ELEMENT_OPT_FLDS) {
        strcpy(itemId + itemIdLen, "$OptFlds");

        LinkedList_add(itemIds, copyString(itemId));
        LinkedList_add(values, rcb->optFlds);
    }

    if (parametersMask & RCB_ELEMENT_BUF_TM) {
        strcpy(itemId + itemIdLen, "$BufTm");

        LinkedList_add(itemIds, copyString(itemId));
        LinkedList_add(values, rcb->bufTm);
    }

    if (parametersMask & RCB_ELEMENT_TRG_OPS) {
        strcpy(itemId + itemIdLen, "$TrgOps");

        LinkedList_add(itemIds, copyString(itemId));
        LinkedList_add(values, rcb->trgOps);
    }

    if (parametersMask & RCB_ELEMENT_INTG_PD) {
        strcpy(itemId + itemIdLen, "$IntgPd");

        LinkedList_add(itemIds, copyString(itemId));
        LinkedList_add(values, rcb->intgPd);
    }

    if (parametersMask & RCB_ELEMENT_GI) {
        strcpy(itemId + itemIdLen, "$GI");

        LinkedList_add(itemIds, copyString(itemId));
        LinkedList_add(values, rcb->gi);
    }

    if (parametersMask & RCB_ELEMENT_PURGE_BUF) {
        if (!isBuffered)
            goto error_invalid_parameter;

        strcpy(itemId + itemIdLen, "$PurgeBuf");

        LinkedList_add(itemIds, copyString(itemId));
        LinkedList_add(values, rcb->purgeBuf);
    }

    if (parametersMask & RCB_ELEMENT_TIME_OF_ENTRY) {
        if (!isBuffered)
            goto error_invalid_parameter;

        strcpy(itemId + itemIdLen, "$TimeOfEntry");

        LinkedList_add(itemIds, copyString(itemId));
        LinkedList_add(values, rcb->timeOfEntry);
    }

    if (parametersMask & RCB_ELEMENT_RPT_ENA) {
        strcpy(itemId + itemIdLen, "$RptEna");

        LinkedList_add(itemIds, copyString(itemId));
        LinkedList_add(values, rcb->rptEna);
    }

    if (singleRequest) {
        LinkedList accessResults = NULL;

        MmsConnection_writeMultipleVariables(self->connection, &mmsError, domainId, itemIds, values, &accessResults);

        if (accessResults != NULL) {

            LinkedList accessResult = LinkedList_getNext(accessResults);

            while (accessResult != NULL) {
                MmsValue* dataAccessError = (MmsValue*) accessResult->data;

                if (MmsValue_getDataAccessError(dataAccessError) != DATA_ACCESS_ERROR_SUCCESS) {
                    *error = IED_ERROR_UNKNOWN;
                    break;
                }

                accessResult = LinkedList_getNext(accessResult);
            }

            LinkedList_destroyDeep(accessResults, (LinkedListValueDeleteFunction) MmsValue_delete);
        }
        else
            *error = iedConnection_mapMmsErrorToIedError(mmsError);

        goto exit_function;
    }
    else {
        LinkedList itemIdElement = LinkedList_getNext(itemIds);
        LinkedList valueElement = LinkedList_getNext(values);

        while (itemIdElement != NULL) {
            char* rcbItemId = (char*) itemIdElement->data;
            MmsValue* value = (MmsValue*) valueElement->data;

            MmsConnection_writeVariable(self->connection, &mmsError, domainId, rcbItemId, value);

            if (mmsError != MMS_ERROR_NONE)
                break;

            itemIdElement = LinkedList_getNext(itemIdElement);
            valueElement = LinkedList_getNext(valueElement);
        }

        *error = iedConnection_mapMmsErrorToIedError(mmsError);
        goto exit_function;
    }

    error_invalid_parameter:
    *error = IED_ERROR_USER_PROVIDED_INVALID_ARGUMENT;

    exit_function:
    LinkedList_destroy(itemIds);
    LinkedList_destroyStatic(values);
}
