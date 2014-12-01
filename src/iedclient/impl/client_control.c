/*
 *  client_control.c
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

#include "libiec61850_platform_includes.h"

#include "iec61850_client.h"

#include "stack_config.h"

#include "mms_client_connection.h"
#include "mms_mapping.h"

#include "ied_connection_private.h"

#include <stdio.h>

#if _MSC_VER
#define snprintf _snprintf
#endif

#ifndef DEBUG_IED_CLIENT
#define DEBUG_IED_CLIENT 0
#endif

struct sControlObjectClient
{
    ControlModel ctlModel;
    char* objectReference;
    IedConnection connection;
    bool test;
    bool interlockCheck;
    bool synchroCheck;
    bool hasTimeActivatedMode;
    int edition; /* 1 = Ed. 1 - 2 = Ed. 2 */

    LastApplError lastApplError;

    CommandTerminationHandler commandTerminationHandler;
    void* commandTerminaionHandlerParameter;

    /* control operation parameters */
    MmsValue* ctlVal;
    uint64_t opertime;
    uint8_t ctlNum;
    char* orIdent;
    int orCat;
};

static void
convertToMmsAndInsertFC(char* newItemId, char* originalObjectName, char* fc)
{
    int originalLength = strlen(originalObjectName);

    int srcIndex = 0;
    int dstIndex = 0;

    while (originalObjectName[srcIndex] != '.') {
        newItemId[dstIndex] = originalObjectName[srcIndex];
        srcIndex++;
        dstIndex++;
    }

    newItemId[dstIndex++] = '$';
    newItemId[dstIndex++] = fc[0];
    newItemId[dstIndex++] = fc[1];
    newItemId[dstIndex++] = '$';
    srcIndex++;

    while (srcIndex < originalLength) {
        if (originalObjectName[srcIndex] == '.')
            newItemId[dstIndex] = '$';
        else
            newItemId[dstIndex] = originalObjectName[srcIndex];

        dstIndex++;
        srcIndex++;
    }

    newItemId[dstIndex] = 0;
}

ControlObjectClient
ControlObjectClient_create(char* objectReference, IedConnection connection)
{
    /* request control model from server */
    char domainId[65];
    char itemId[129];

    MmsMapping_getMmsDomainFromObjectReference(objectReference, domainId);

    convertToMmsAndInsertFC(itemId, objectReference + strlen(domainId) + 1, "CF");

    strncat(itemId, "$ctlModel", 128);

    MmsError mmsError;

    MmsValue* ctlModel = MmsConnection_readVariable(IedConnection_getMmsConnection(connection),
            &mmsError, domainId, itemId);

    if (ctlModel == NULL) {
        if (DEBUG_IED_CLIENT)
            printf("IED_CLIENT: ControlObjectClient_create: failed to get ctlModel from server\n");
        return NULL;
    }

    int ctlModelVal = MmsValue_toUint32(ctlModel);

    MmsValue_delete(ctlModel);

    IedClientError error;

    LinkedList dataDirectory =
            IedConnection_getDataDirectory(connection, &error, objectReference);

    if (dataDirectory == NULL) {
        if (DEBUG_IED_CLIENT)
            printf("IED_CLIENT: ControlObjectClient_create: failed to get data directory of control object\n");
        return NULL;
    }

    /* check what control elements are available */
    bool hasOper = false;

    LinkedList element = LinkedList_getNext(dataDirectory);

    while (element != NULL) {
        char* objectName = (char*) element->data;

        if (strcmp(objectName, "Oper") == 0)
            hasOper = true;

        element = LinkedList_getNext(element);
    }

    LinkedList_destroy(dataDirectory);

    if (hasOper == false) {
        if (DEBUG_IED_CLIENT)
            printf("IED_CLIENT: control is missing required element \"Oper\"\n");
        return NULL;
    }

    /* check for time activated control */
    bool hasTimeActivatedControl = false;

    strcpy(itemId, objectReference);
    strcat(itemId, ".Oper");
    dataDirectory = IedConnection_getDataDirectory(connection, &error, itemId);

    element = LinkedList_getNext(dataDirectory);

    while (element != NULL) {
        char* objectName = (char*) element->data;

        if (strcmp(objectName, "operTm") == 0) {
            hasTimeActivatedControl = true;
            break;
        }

        element = LinkedList_getNext(element);
    }

    LinkedList_destroy(dataDirectory);

    /* get default parameters for Oper control variable */

    MmsValue* oper = IedConnection_readObject(connection, &error, itemId, CO);

    if (oper == NULL) {
        if (DEBUG_IED_CLIENT)
            printf("reading Oper failed!\n");
        return NULL;
    }

    ControlObjectClient self = (ControlObjectClient) calloc(1, sizeof(struct sControlObjectClient));

    self->objectReference = copyString(objectReference);
    self->connection = connection;
    self->ctlModel = (ControlModel) ctlModelVal;
    self->hasTimeActivatedMode = hasTimeActivatedControl;
    self->ctlVal = MmsValue_getElement(oper, 0);

    /* Check for T element type (EntryTime -> Ed.1, Timestamp -> Ed.2) */
    MmsValue* t;

    if (hasTimeActivatedControl)
        t = MmsValue_getElement(oper, 4);
    else
        t = MmsValue_getElement(oper, 3);

    if (MmsValue_getType(t) == MMS_BINARY_TIME)
        self->edition = 1;
    else
        self->edition = 2;

    if (DEBUG_IED_CLIENT)
        printf("IED_CLIENT: Detected edition %i control\n", self->edition);

    MmsValue_setElement(oper, 0, NULL);
    MmsValue_delete(oper);

    private_IedConnection_addControlClient(connection, self);

    return self;
}

void
ControlObjectClient_destroy(ControlObjectClient self)
{
    free(self->objectReference);

    private_IedConnection_removeControlClient(self->connection, self);

    if (self->ctlVal != NULL)
        MmsValue_delete(self->ctlVal);

    if (self->orIdent != NULL)
        free(self->orIdent);

    free(self);
}

void
ControlObjectClient_setCommandTerminationHandler(ControlObjectClient self, CommandTerminationHandler handler,
        void* handlerParameter)
{
    self->commandTerminaionHandlerParameter = handlerParameter;
    self->commandTerminationHandler = handler;
}

char*
ControlObjectClient_getObjectReference(ControlObjectClient self)
{
    return self->objectReference;
}

ControlModel
ControlObjectClient_getControlModel(ControlObjectClient self)
{
    return self->ctlModel;
}

void
ControlObjectClient_setOrigin(ControlObjectClient self, char* orIdent, int orCat)
{
    if (self->orIdent != NULL)
        free(self->orIdent);

    self->orIdent = copyString(orIdent);
    self->orCat = orCat;
}

static MmsValue*
createOriginValue(ControlObjectClient self)
{
    MmsValue* origin = MmsValue_createEmptyStructure(2);
    MmsValue* orCat = MmsValue_newIntegerFromInt16(self->orCat);
    MmsValue_setElement(origin, 0, orCat);

    MmsValue* orIdent;

    if (self->orIdent != NULL) {
        int octetStringLen = strlen(self->orIdent);
        orIdent = MmsValue_newOctetString(0, octetStringLen);
        MmsValue_setOctetString(orIdent, (uint8_t*) self->orIdent, octetStringLen);
    }
    else
        orIdent = MmsValue_newOctetString(0, 0);

    MmsValue_setElement(origin, 1, orIdent);

    return origin;
}

bool
ControlObjectClient_operate(ControlObjectClient self, MmsValue* ctlVal, uint64_t operTime)
{
    MmsValue* operParameters;

    if (self->hasTimeActivatedMode)
        operParameters = MmsValue_createEmptyStructure(7);
    else
        operParameters = MmsValue_createEmptyStructure(6);

    MmsValue_setElement(operParameters, 0, ctlVal);

    int index = 1;

    if (self->hasTimeActivatedMode) {
        MmsValue* operTm = MmsValue_newUtcTimeByMsTime(operTime);
        MmsValue_setElement(operParameters, index++, operTm);
    }

    MmsValue* origin = createOriginValue(self);
    MmsValue_setElement(operParameters, index++, origin);

    self->ctlNum++;
    MmsValue* ctlNum = MmsValue_newUnsignedFromUint32(self->ctlNum);
    MmsValue_setElement(operParameters, index++, ctlNum);

    uint64_t timestamp = Hal_getTimeInMs();
    MmsValue* ctlTime;

    if (self->edition == 2)
        ctlTime = MmsValue_newUtcTimeByMsTime(timestamp);
    else {
        ctlTime = MmsValue_newBinaryTime(false);
        MmsValue_setBinaryTime(ctlTime, timestamp);
    }
    MmsValue_setElement(operParameters, index++, ctlTime);

    MmsValue* ctlTest = MmsValue_newBoolean(self->test);
    MmsValue_setElement(operParameters, index++, ctlTest);

    MmsValue* check = MmsValue_newBitString(2);
    MmsValue_setBitStringBit(check, 1, self->interlockCheck);
    MmsValue_setBitStringBit(check, 0, self->synchroCheck);
    MmsValue_setElement(operParameters, index++, check);

    char domainId[65];
    char itemId[130];

    MmsMapping_getMmsDomainFromObjectReference(self->objectReference, domainId);

    convertToMmsAndInsertFC(itemId, self->objectReference + strlen(domainId) + 1, "CO");

    strncat(itemId, "$Oper", 129);

    if (DEBUG_IED_CLIENT)
        printf("IED_CLIENT: operate: %s/%s\n", domainId, itemId);

    MmsError mmsError;

    MmsConnection_writeVariable(IedConnection_getMmsConnection(self->connection),
            &mmsError, domainId, itemId, operParameters);

    MmsValue_setElement(operParameters, 0, NULL);
    MmsValue_delete(operParameters);

    if (mmsError != MMS_ERROR_NONE) {
        if (DEBUG_IED_CLIENT)
            printf("IED_CLIENT: operate failed!\n");
        return false;
    }

    return true;
}

bool
ControlObjectClient_selectWithValue(ControlObjectClient self, MmsValue* ctlVal)
{
    char domainId[65];
    char itemId[130];

    MmsMapping_getMmsDomainFromObjectReference(self->objectReference, domainId);

    convertToMmsAndInsertFC(itemId, self->objectReference + strlen(domainId) + 1, "CO");

    strncat(itemId, "$SBOw", 129);

    if (DEBUG_IED_CLIENT)
        printf("IED_CLIENT: select with value: %s/%s\n", domainId, itemId);

    MmsError mmsError;

    MmsValue* selValParameters;

    if (self->hasTimeActivatedMode)
        selValParameters = MmsValue_createEmptyStructure(7);
    else
        selValParameters = MmsValue_createEmptyStructure(6);

    MmsValue_setElement(selValParameters, 0, ctlVal);

    int index = 1;

    if (self->hasTimeActivatedMode) {
        MmsValue* operTm = MmsValue_newUtcTimeByMsTime(0);
        MmsValue_setElement(selValParameters, index++, operTm);
    }

    MmsValue* origin = createOriginValue(self);
    MmsValue_setElement(selValParameters, index++, origin);

    MmsValue* ctlNum = MmsValue_newUnsignedFromUint32(self->ctlNum + 1);
    MmsValue_setElement(selValParameters, index++, ctlNum);


    uint64_t timestamp = Hal_getTimeInMs();
    MmsValue* ctlTime;

    if (self->edition == 2)
        ctlTime = MmsValue_newUtcTimeByMsTime(timestamp);
    else {
        ctlTime = MmsValue_newBinaryTime(false);
        MmsValue_setBinaryTime(ctlTime, timestamp);
    }

    MmsValue_setElement(selValParameters, index++, ctlTime);

    MmsValue* ctlTest = MmsValue_newBoolean(self->test);
    MmsValue_setElement(selValParameters, index++, ctlTest);

    MmsValue* check = MmsValue_newBitString(2);
    MmsValue_setBitStringBit(check, 1, self->interlockCheck);
    MmsValue_setBitStringBit(check, 0, self->synchroCheck);
    MmsValue_setElement(selValParameters, index++, check);

    MmsConnection_writeVariable(IedConnection_getMmsConnection(self->connection),
            &mmsError, domainId, itemId, selValParameters);

    MmsValue_setElement(selValParameters, 0, NULL);
    MmsValue_delete(selValParameters);

    if (mmsError != MMS_ERROR_NONE) {
        if (DEBUG_IED_CLIENT)
            printf("IED_CLIENT: select-with-value failed!\n");
        return false;
    }

    return true;
}

bool
ControlObjectClient_select(ControlObjectClient self)
{
    char domainId[65];
    char itemId[130];

    MmsMapping_getMmsDomainFromObjectReference(self->objectReference, domainId);

    convertToMmsAndInsertFC(itemId, self->objectReference + strlen(domainId) + 1, "CO");

    strncat(itemId, "$SBO", 129);

    if (DEBUG_IED_CLIENT)
        printf("IED_CLIENT: select: %s/%s\n", domainId, itemId);

    MmsError mmsError;

    MmsValue* value = MmsConnection_readVariable(IedConnection_getMmsConnection(self->connection),
            &mmsError, domainId, itemId);

    int selected = false;

    if (value == NULL) {
        if (DEBUG_IED_CLIENT)
            printf("IED_CLIENT: select: read SBO failed!\n");
        return false;
    }

    char sboReference[130];

    snprintf(sboReference, 129, "%s/%s", domainId, itemId);

    if (MmsValue_getType(value) == MMS_VISIBLE_STRING) {
        if (strcmp(MmsValue_toString(value),  "") == 0) {
            if (DEBUG_IED_CLIENT)
                printf("select-response-\n");
        }
        else if (strcmp(MmsValue_toString(value), sboReference)) {
            if (DEBUG_IED_CLIENT)
                printf("select-response+: (%s)\n", MmsValue_toString(value));
            selected = true;
        }
        else {
            if (DEBUG_IED_CLIENT)
                printf("IED_CLIENT: select-response: (%s)\n", MmsValue_toString(value));
        }
    }
    else {
        if (DEBUG_IED_CLIENT)
            printf("IED_CLIENT: select: unexpected response from server!\n");
    }

    MmsValue_delete(value);

    return selected;
}

bool
ControlObjectClient_cancel(ControlObjectClient self)
{
    MmsValue* cancelParameters;

    if (self->hasTimeActivatedMode)
        cancelParameters = MmsValue_createEmptyStructure(6);
    else
        cancelParameters = MmsValue_createEmptyStructure(5);

    MmsValue_setElement(cancelParameters, 0, self->ctlVal);

    int index = 1;

    if (self->hasTimeActivatedMode) {
        MmsValue* operTm = MmsValue_newUtcTimeByMsTime(self->opertime);
        MmsValue_setElement(cancelParameters, index++, operTm);
    }

    MmsValue* origin = createOriginValue(self);

    MmsValue_setElement(cancelParameters, index++, origin);

    MmsValue* ctlNum = MmsValue_newUnsignedFromUint32(self->ctlNum);
    MmsValue_setElement(cancelParameters, index++, ctlNum);

    uint64_t timestamp = Hal_getTimeInMs();
    MmsValue* ctlTime;

    if (self->edition == 2)
        ctlTime = MmsValue_newUtcTimeByMsTime(timestamp);
    else {
        ctlTime = MmsValue_newBinaryTime(false);
        MmsValue_setBinaryTime(ctlTime, timestamp);
    }
    MmsValue_setElement(cancelParameters, index++, ctlTime);

    MmsValue* ctlTest = MmsValue_newBoolean(self->test);
    MmsValue_setElement(cancelParameters, index++, ctlTest);

    char domainId[65];
    char itemId[130];

    MmsMapping_getMmsDomainFromObjectReference(self->objectReference, domainId);

    convertToMmsAndInsertFC(itemId, self->objectReference + strlen(domainId) + 1, "CO");

    strncat(itemId, "$Cancel", 129);

    if (DEBUG_IED_CLIENT)
        printf("IED_CLIENT: cancel: %s/%s\n", domainId, itemId);

    MmsError mmsError;

    MmsConnection_writeVariable(IedConnection_getMmsConnection(self->connection),
            &mmsError, domainId, itemId, cancelParameters);

    MmsValue_setElement(cancelParameters, 0, NULL);
    MmsValue_delete(cancelParameters);

    if (mmsError != MMS_ERROR_NONE) {
        if (DEBUG_IED_CLIENT)
            printf("IED_CLIENT: cancel failed!\n");
        return false;
    }

    return true;
}

void
ControlObjectClient_enableInterlockCheck(ControlObjectClient self)
{
    self->interlockCheck = true;
}

void
ControlObjectClient_enableSynchroCheck(ControlObjectClient self)
{
    self->synchroCheck = true;
}

void
ControlObjectClient_setLastApplError(ControlObjectClient self, LastApplError lastApplError)
{
    self->lastApplError = lastApplError;
}

LastApplError
ControlObjectClient_getLastApplError(ControlObjectClient self)
{
    return self->lastApplError;
}

void
private_ControlObjectClient_invokeCommandTerminationHandler(ControlObjectClient self)
{
    if (self->commandTerminationHandler != NULL)
             self->commandTerminationHandler(self->commandTerminaionHandlerParameter, self);
}
