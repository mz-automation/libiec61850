/*
 *  client_control.c
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

#include "libiec61850_platform_includes.h"

#include "stack_config.h"

#include "iec61850_client.h"
#include "mms_client_connection.h"
#include "ied_connection_private.h"

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

    MmsValue* analogValue; /* for APC-CDCs */

    int edition; /* 1 = Ed. 1 - 2 = Ed. 2 - to distinguish time stamp format */
    bool hasCtlNum; /* Check if ctlNum attribute is present - ctlNum is(/can be?) absent in edition 1 APC CDC */

    bool useConstantT; /* some servers require a constant T parameter for select and operate */
    uint64_t constantT; /* timestamp of select/operate to be used when constant T option is selected */

    LastApplError lastApplError;
    MmsError lastMmsError;
    MmsDataAccessError lastAccessError; /* last error of read or write command */

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
convertToMmsAndInsertFC(char* newItemId, const char* originalObjectName, const char* fc)
{
    int originalLength = (int) strlen(originalObjectName);

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

static void
resetLastApplError(ControlObjectClient self)
{
    self->lastApplError.error = CONTROL_ERROR_NO_ERROR;
    self->lastApplError.addCause = ADD_CAUSE_UNKNOWN;
    self->lastApplError.ctlNum = 0;
}

ControlObjectClient
ControlObjectClient_createEx(const char* objectReference, IedConnection connection, ControlModel ctlModel, MmsVariableSpecification* controlObjectSpec)
{
    ControlObjectClient self = NULL;

    /* check what control elements are available */
    bool hasOper = false;
    bool hasTimeActivatedControl = false;
    bool hasCtlNum = false;
    bool isAPC = false;
    MmsVariableSpecification* ctlVal = NULL;
    MmsVariableSpecification* t = NULL;

    if (MmsVariableSpecification_getType(controlObjectSpec) == MMS_STRUCTURE) {
        MmsVariableSpecification* oper = MmsVariableSpecification_getNamedVariableRecursive(controlObjectSpec, "Oper");

        if (oper)
        {
            hasOper = true;

            ctlVal = MmsVariableSpecification_getNamedVariableRecursive(oper, "ctlVal");

            if (ctlVal == NULL)
                ctlVal = MmsVariableSpecification_getNamedVariableRecursive(oper, "setMag");

            if (ctlVal) {
                if (MmsVariableSpecification_getType(ctlVal) == MMS_STRUCTURE)
                    isAPC = true;
            }

            MmsVariableSpecification* operTm = MmsVariableSpecification_getNamedVariableRecursive(oper, "operTm");

            if (operTm)
                hasTimeActivatedControl = true;

            MmsVariableSpecification* ctlNum = MmsVariableSpecification_getNamedVariableRecursive(oper, "ctlNum");

            if (ctlNum)
                hasCtlNum = true;

            t = MmsVariableSpecification_getNamedVariableRecursive(oper, "T");
        }

        /* TODO Add additional checks dependent on control model */
    }

    if (hasOper == false) {
        if (DEBUG_IED_CLIENT)
            printf("IED_CLIENT: control is missing required element \"Oper\"\n");

        goto exit_function;
    }

    if ((ctlVal == NULL) || (t == NULL)) {
        if (DEBUG_IED_CLIENT)
            printf("IED_CLIENT:   \"Oper\" is missing required element\n");
        goto exit_function;
    }

    self = (ControlObjectClient) GLOBAL_CALLOC(1, sizeof(struct sControlObjectClient));

    if (self == NULL)
        goto exit_function;

    self->objectReference = StringUtils_copyString(objectReference);
    self->connection = connection;
    self->ctlModel = ctlModel;
    self->hasTimeActivatedMode = hasTimeActivatedControl;
    self->hasCtlNum = hasCtlNum;
    self->ctlVal = MmsValue_newDefaultValue(ctlVal);

    if (isAPC)
        self->analogValue = MmsValue_createEmptyStructure(1);
    else
        self->analogValue = NULL;

    /* Check for T element type (Binary time -> Ed.1,UTC time -> Ed.2) */
    if (t) {
        if (MmsVariableSpecification_getType(t) == MMS_BINARY_TIME)
            self->edition = 1;
        else
            self->edition = 2;
    }
    else
        self->edition = 1;

    if (DEBUG_IED_CLIENT)
        printf("IED_CLIENT: Detected edition %i control\n", self->edition);

    iedConnection_addControlClient(connection, self);

exit_function:
    return self;
}

ControlObjectClient
ControlObjectClient_create(const char* objectReference, IedConnection connection)
{
    ControlObjectClient self = NULL;

    /* request control model from server */
    char reference[129];

    if (strlen(objectReference) < 120) {
        strcpy(reference, objectReference);
        strcat(reference, ".ctlModel");
    }
    else
        goto exit_function;

    IedClientError error;

    uint32_t ctlModel = IedConnection_readUnsigned32Value(connection, &error, reference, IEC61850_FC_CF);

    if (error != IED_ERROR_OK) {
        if (DEBUG_IED_CLIENT)
            printf("IED_CLIENT: ControlObjectClient_create: failed to get %s from server\n", reference);

        goto exit_function;
    }

    MmsVariableSpecification* ctlVarSpec =
            IedConnection_getVariableSpecification(connection, &error, objectReference, IEC61850_FC_CO);

    if (error != IED_ERROR_OK) {
        if (DEBUG_IED_CLIENT)
            printf("IED_CLIENT: ControlObjectClient_create: failed to get data directory of control object\n");

        goto exit_function;
    }

    self = ControlObjectClient_createEx(objectReference, connection, (ControlModel) ctlModel, ctlVarSpec);

    MmsVariableSpecification_destroy(ctlVarSpec);

exit_function:
    return self;
}

void
ControlObjectClient_destroy(ControlObjectClient self)
{
    if (self != NULL)
    {
        GLOBAL_FREEMEM(self->objectReference);

        iedConnection_removeControlClient(self->connection, self);

        if (self->ctlVal != NULL)
            MmsValue_delete(self->ctlVal);

        if (self->analogValue != NULL)
            MmsValue_delete(self->analogValue);

        if (self->orIdent != NULL)
            GLOBAL_FREEMEM(self->orIdent);

        GLOBAL_FREEMEM(self);
    }
}

void
ControlObjectClient_setCommandTerminationHandler(ControlObjectClient self, CommandTerminationHandler handler,
        void* handlerParameter)
{
    self->commandTerminaionHandlerParameter = handlerParameter;
    self->commandTerminationHandler = handler;
}

const char*
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
ControlObjectClient_setControlModel(ControlObjectClient self, ControlModel ctlModel)
{
    self->ctlModel = ctlModel;
}

void
ControlObjectClient_changeServerControlModel(ControlObjectClient self, ControlModel ctlModel)
{
    /* TODO write new ctlModel to server */
    self->ctlModel = ctlModel;
}

MmsType
ControlObjectClient_getCtlValType(ControlObjectClient self)
{
    if (self->analogValue != NULL)
        return MmsValue_getType(self->analogValue);
    else
        return MmsValue_getType(self->ctlVal);
}

IedClientError
ControlObjectClient_getLastError(ControlObjectClient self)
{
    if (self->lastAccessError != DATA_ACCESS_ERROR_SUCCESS)
        return iedConnection_mapDataAccessErrorToIedError(self->lastAccessError);
    else
        return iedConnection_mapMmsErrorToIedError(self->lastMmsError);
}

void
ControlObjectClient_setOrigin(ControlObjectClient self, const char* orIdent, int orCat)
{
    if (self->orIdent != NULL)
        GLOBAL_FREEMEM(self->orIdent);

    if (orIdent != NULL)
        self->orIdent = StringUtils_copyString(orIdent);
    else
        self->orIdent = NULL;

    self->orCat = orCat;
}

static MmsValue*
createOriginValue(ControlObjectClient self)
{
    MmsValue* origin = MmsValue_createEmptyStructure(2);

    if (origin == NULL)
        goto exit_function;

    MmsValue* orCat = MmsValue_newIntegerFromInt16(self->orCat);

    if (orCat == NULL)
        goto cleanup_on_error;

    MmsValue_setElement(origin, 0, orCat);

    MmsValue* orIdent;

    if (self->orIdent != NULL) {
        int octetStringLen = strlen(self->orIdent);
        orIdent = MmsValue_newOctetString(0, octetStringLen);

        if (orIdent == NULL)
            goto cleanup_on_error;

        MmsValue_setOctetString(orIdent, (uint8_t*) self->orIdent, octetStringLen);
    }
    else
        orIdent = MmsValue_newOctetString(0, 0);

    if (orIdent == NULL)
        goto cleanup_on_error;

    MmsValue_setElement(origin, 1, orIdent);

    goto exit_function;

    cleanup_on_error:
    MmsValue_delete(origin);
    origin = NULL;

    exit_function:
    return origin;
}

static MmsValue*
prepareOperParameters(ControlObjectClient self, MmsValue* ctlVal, uint64_t operTime)
{
    resetLastApplError(self);

    MmsValue* operParameters;

    int operElementCount = 5;

    if (self->hasTimeActivatedMode)
        operElementCount++;

    if (self->hasCtlNum)
        operElementCount++;

    operParameters = MmsValue_createEmptyStructure(operElementCount);

    /* support simplified usage of APC controls - user doesn't need to create the structure */
    if (self->analogValue != NULL) {
        if (MmsValue_getType(ctlVal) != MMS_STRUCTURE) {
            MmsValue_setElement(self->analogValue, 0, ctlVal);
            ctlVal = self->analogValue;
        }
    }

    MmsValue_setElement(operParameters, 0, ctlVal);

    int index = 1;

    if (self->hasTimeActivatedMode) {
        MmsValue* operTm = MmsValue_newUtcTimeByMsTime(operTime);
        MmsValue_setElement(operParameters, index++, operTm);
    }

    MmsValue* origin = createOriginValue(self);
    MmsValue_setElement(operParameters, index++, origin);

    if (!((self->ctlModel == CONTROL_MODEL_SBO_NORMAL) ||
            (self->ctlModel == CONTROL_MODEL_SBO_ENHANCED)))
    {
        self->ctlNum++;
    }

    if (self->hasCtlNum) {
        MmsValue* ctlNum = MmsValue_newUnsignedFromUint32(self->ctlNum);
        MmsValue_setElement(operParameters, index++, ctlNum);
    }

    uint64_t timestamp;

    if ((self->ctlModel == CONTROL_MODEL_SBO_ENHANCED) && (self->useConstantT))
        timestamp = self->constantT;
    else
        timestamp = Hal_getTimeInMs();

    if (self->useConstantT)
        self->constantT = timestamp;

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
    char itemId[65];

    MmsMapping_getMmsDomainFromObjectReference(self->objectReference, domainId);

    convertToMmsAndInsertFC(itemId, self->objectReference + strlen(domainId) + 1, "CO");

    int controlObjectItemIdLen = strlen(itemId);

    strncat(itemId, "$Oper", 64 - controlObjectItemIdLen);

    if (DEBUG_IED_CLIENT)
        printf("IED_CLIENT: operate: %s/%s\n", domainId, itemId);

    return operParameters;
}

bool
ControlObjectClient_operate(ControlObjectClient self, MmsValue* ctlVal, uint64_t operTime)
{
    bool success = false;

    if (ctlVal == NULL) {
        if (DEBUG_IED_CLIENT)
            printf("IED_CLIENT: operate - (ctlVal == NULL)!\n");

        goto exit_function;
    }

    MmsValue* operParameters = prepareOperParameters(self, ctlVal, operTime);

    char domainId[65];
    char itemId[65];

    MmsMapping_getMmsDomainFromObjectReference(self->objectReference, domainId);

    convertToMmsAndInsertFC(itemId, self->objectReference + strlen(domainId) + 1, "CO");

    int controlObjectItemIdLen = strlen(itemId);

    strncat(itemId, "$Oper", 64 - controlObjectItemIdLen);

    if (DEBUG_IED_CLIENT)
        printf("IED_CLIENT: operate: %s/%s\n", domainId, itemId);

    MmsError mmsError;

    MmsDataAccessError writeResult = MmsConnection_writeVariable(IedConnection_getMmsConnection(self->connection),
            &mmsError, domainId, itemId, operParameters);

    MmsValue_setElement(operParameters, 0, NULL);
    MmsValue_delete(operParameters);

    self->lastMmsError = mmsError;
    self->lastAccessError = writeResult;

    if (mmsError != MMS_ERROR_NONE) {
        if (DEBUG_IED_CLIENT)
            printf("IED_CLIENT: operate failed!\n");

        goto exit_function;
    }

    MmsValue_update(self->ctlVal, ctlVal);

    self->opertime = operTime;

    success = true;

exit_function:

    if (self->analogValue)
        MmsValue_setElement(self->analogValue, 0, NULL);

    return success;
}

static void
internalOperateHandler(uint32_t invokeId, void* parameter, MmsError err, MmsDataAccessError accessError)
{
    ControlObjectClient self = (ControlObjectClient) parameter;

    IedConnectionOutstandingCall call = iedConnection_lookupOutstandingCall(self->connection, invokeId);

    if (call) {

        ControlObjectClient_ControlActionHandler handler = (ControlObjectClient_ControlActionHandler) call->callback;

        IedClientError iedError = iedConnection_mapMmsErrorToIedError(err);

        bool success = false;

        self->lastMmsError = err;
        self->lastAccessError = accessError;

        if (iedError == IED_ERROR_OK) {
            iedError = iedConnection_mapDataAccessErrorToIedError(accessError);

            if (iedError == IED_ERROR_OK)
                success = true;
        }

        handler(invokeId, call->callbackParameter, iedError, CONTROL_ACTION_TYPE_OPERATE, success);

        iedConnection_releaseOutstandingCall(self->connection, call);
    }
    else {
        if (DEBUG_IED_CLIENT)
            printf("IED_CLIENT: internal error - no matching outstanding call (ID: %d)!\n", invokeId);
    }
}

uint32_t
ControlObjectClient_operateAsync(ControlObjectClient self, IedClientError* err, MmsValue* ctlVal, uint64_t operTime,
        ControlObjectClient_ControlActionHandler handler, void* parameter)
{
    *err = IED_ERROR_OK;
    uint32_t invokeId = 0;

    if (ctlVal == NULL) {
        *err = IED_ERROR_USER_PROVIDED_INVALID_ARGUMENT;
        goto exit_function;
    }

    IedConnectionOutstandingCall call = iedConnection_allocateOutstandingCall(self->connection);

    if (call == NULL) {
        *err = IED_ERROR_OUTSTANDING_CALL_LIMIT_REACHED;
        goto exit_function;
    }

    call->callback = handler;
    call->callbackParameter = parameter;

    MmsValue* operParameters = prepareOperParameters(self, ctlVal, operTime);

    char domainId[65];
    char itemId[65];

    MmsMapping_getMmsDomainFromObjectReference(self->objectReference, domainId);

    convertToMmsAndInsertFC(itemId, self->objectReference + strlen(domainId) + 1, "CO");

    int controlObjectItemIdLen = strlen(itemId);

    strncat(itemId, "$Oper", 64 - controlObjectItemIdLen);

    if (DEBUG_IED_CLIENT)
        printf("IED_CLIENT: operate: %s/%s\n", domainId, itemId);

    MmsError mmsError;

    call->invokeId = MmsConnection_writeVariableAsync(self->connection->connection, &mmsError, domainId, itemId, operParameters, internalOperateHandler, self);

    invokeId = call->invokeId;

    MmsValue_setElement(operParameters, 0, NULL);
    MmsValue_delete(operParameters);

    *err = iedConnection_mapMmsErrorToIedError(mmsError);

    if (mmsError != MMS_ERROR_NONE) {
        iedConnection_releaseOutstandingCall(self->connection, call);
    }
    else {
        MmsValue_update(self->ctlVal, ctlVal);

        self->opertime = operTime;
    }

exit_function:

    if (self->analogValue)
        MmsValue_setElement(self->analogValue, 0, NULL);

    return invokeId;
}

static MmsValue*
prepareSBOwParameters(ControlObjectClient self, MmsValue* ctlVal)
{
    int selValElementCount = 5;

    if (self->hasTimeActivatedMode)
        selValElementCount++;

    if (self->hasCtlNum)
        selValElementCount++;

    MmsValue* selValParameters = MmsValue_createEmptyStructure(selValElementCount);

    /* support simplified usage of APC controls - user doesn't need to create the structure */
    if (self->analogValue != NULL) {
        if (MmsValue_getType(ctlVal) != MMS_STRUCTURE) {
            MmsValue_setElement(self->analogValue, 0, ctlVal);
            ctlVal = self->analogValue;
        }
    }

    MmsValue_setElement(selValParameters, 0, ctlVal);

    int index = 1;

    if (self->hasTimeActivatedMode) {
        MmsValue* operTm = MmsValue_newUtcTimeByMsTime(0);
        MmsValue_setElement(selValParameters, index++, operTm);
    }

    MmsValue* origin = createOriginValue(self);
    MmsValue_setElement(selValParameters, index++, origin);

    self->ctlNum++;

    if (self->hasCtlNum) {
        MmsValue* ctlNum = MmsValue_newUnsignedFromUint32(self->ctlNum);
        MmsValue_setElement(selValParameters, index++, ctlNum);
    }

    uint64_t timestamp = Hal_getTimeInMs();
    MmsValue* ctlTime;

    if (self->useConstantT)
        self->constantT = timestamp;

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

    return selValParameters;
}

bool
ControlObjectClient_selectWithValue(ControlObjectClient self, MmsValue* ctlVal)
{
    bool retVal = true;

    resetLastApplError(self);

    char domainId[65];
    char itemId[65];

    MmsMapping_getMmsDomainFromObjectReference(self->objectReference, domainId);

    convertToMmsAndInsertFC(itemId, self->objectReference + strlen(domainId) + 1, "CO");

    strncat(itemId, "$SBOw", 64);

    if (DEBUG_IED_CLIENT)
        printf("IED_CLIENT: select with value: %s/%s\n", domainId, itemId);

    MmsError mmsError;

    int selValElementCount = 5;

    if (self->hasTimeActivatedMode)
        selValElementCount++;

    if (self->hasCtlNum)
        selValElementCount++;

    MmsValue* selValParameters = prepareSBOwParameters(self, ctlVal);

    MmsDataAccessError writeResult = MmsConnection_writeVariable(IedConnection_getMmsConnection(self->connection),
            &mmsError, domainId, itemId, selValParameters);

    MmsValue_setElement(selValParameters, 0, NULL);
    MmsValue_delete(selValParameters);

    self->lastMmsError = mmsError;
    self->lastAccessError = writeResult;

    if (mmsError != MMS_ERROR_NONE) {
        if (DEBUG_IED_CLIENT)
            printf("IED_CLIENT: select-with-value failed!\n");

        retVal = false;

        goto exit_function;
    }
    else {
        if (writeResult != DATA_ACCESS_ERROR_SUCCESS) {
            if (DEBUG_IED_CLIENT)
                printf("IED_CLIENT: select-with-value failed!\n");

            retVal = false;

            goto exit_function;
        }
    }

    MmsValue_update(self->ctlVal, ctlVal);

exit_function:

    if (self->analogValue)
        MmsValue_setElement(self->analogValue, 0, NULL);

    return retVal;
}

static void
internalSelWithValHandler(uint32_t invokeId, void* parameter, MmsError err, MmsDataAccessError accessError)
{
    ControlObjectClient self = (ControlObjectClient) parameter;

    IedConnectionOutstandingCall call = iedConnection_lookupOutstandingCall(self->connection, invokeId);

    if (call) {

        ControlObjectClient_ControlActionHandler handler = (ControlObjectClient_ControlActionHandler) call->callback;

        IedClientError iedError = iedConnection_mapMmsErrorToIedError(err);

        bool success = false;

        self->lastMmsError = err;
        self->lastAccessError = accessError;

        if (iedError == IED_ERROR_OK) {
            iedError = iedConnection_mapDataAccessErrorToIedError(accessError);

            if (iedError == IED_ERROR_OK)
                success = true;
        }

        if (success) {
            if (DEBUG_IED_CLIENT)
                printf("IED_CLIENT: select-with-value+\n");
        }
        else {
            if (DEBUG_IED_CLIENT)
                printf("IED_CLIENT: select-with-value failed!\n");
        }

        handler(invokeId, call->callbackParameter, iedError, CONTROL_ACTION_TYPE_SELECT, success);

        iedConnection_releaseOutstandingCall(self->connection, call);
    }
    else {
        if (DEBUG_IED_CLIENT)
            printf("IED_CLIENT: internal error - no matching outstanding call!\n");
    }
}

uint32_t
ControlObjectClient_selectWithValueAsync(ControlObjectClient self, IedClientError* err, MmsValue* ctlVal,
        ControlObjectClient_ControlActionHandler handler, void* parameter)
{
    *err = IED_ERROR_OK;
    uint32_t invokeId = 0;

    if (ctlVal == NULL) {
        *err = IED_ERROR_USER_PROVIDED_INVALID_ARGUMENT;
        goto exit_function;
    }

    IedConnectionOutstandingCall call = iedConnection_allocateOutstandingCall(self->connection);

    if (call == NULL) {
        *err = IED_ERROR_OUTSTANDING_CALL_LIMIT_REACHED;
        goto exit_function;
    }

    MmsValue* selValParameters = prepareSBOwParameters(self, ctlVal);

    resetLastApplError(self);

    char domainId[65];
    char itemId[65];

    MmsMapping_getMmsDomainFromObjectReference(self->objectReference, domainId);

    convertToMmsAndInsertFC(itemId, self->objectReference + strlen(domainId) + 1, "CO");

    strncat(itemId, "$SBOw", 64);

    call->callback = handler;
    call->callbackParameter = parameter;

    MmsError mmsError;

    if (DEBUG_IED_CLIENT)
        printf("IED_CLIENT: select with value: %s/%s\n", domainId, itemId);

    call->invokeId = MmsConnection_writeVariableAsync(self->connection->connection, &mmsError, domainId, itemId, selValParameters, internalSelWithValHandler, self);

    invokeId = call->invokeId;

    MmsValue_setElement(selValParameters, 0, NULL);
    MmsValue_delete(selValParameters);

    *err = iedConnection_mapMmsErrorToIedError(mmsError);

    if (mmsError != MMS_ERROR_NONE) {
        iedConnection_releaseOutstandingCall(self->connection, call);
    }
    else {
        MmsValue_update(self->ctlVal, ctlVal);
    }

exit_function:

    if (self->analogValue)
        MmsValue_setElement(self->analogValue, 0, NULL);

    return invokeId;
}

bool
ControlObjectClient_select(ControlObjectClient self)
{
    resetLastApplError(self);

    char domainId[65];
    char itemId[65];

    MmsMapping_getMmsDomainFromObjectReference(self->objectReference, domainId);

    convertToMmsAndInsertFC(itemId, self->objectReference + strlen(domainId) + 1, "CO");

    strncat(itemId, "$SBO", 64);

    if (DEBUG_IED_CLIENT)
        printf("IED_CLIENT: select: %s/%s\n", domainId, itemId);

    MmsError mmsError;

    MmsValue* value = MmsConnection_readVariable(IedConnection_getMmsConnection(self->connection),
            &mmsError, domainId, itemId);

    bool selected = false;

    self->ctlNum++;

    self->lastMmsError = mmsError;
    self->lastAccessError = DATA_ACCESS_ERROR_SUCCESS;

    if (value == NULL) {
        if (DEBUG_IED_CLIENT)
            printf("IED_CLIENT: select: read SBO failed!\n");
        goto exit_function;
    }

    if (MmsValue_getType(value) == MMS_VISIBLE_STRING) {
        if (strcmp(MmsValue_toString(value), "") == 0) {
            if (DEBUG_IED_CLIENT)
                printf("select-response-\n");
        }
        else {
            if (DEBUG_IED_CLIENT)
                printf("select-response+: (%s)\n", MmsValue_toString(value));
            selected = true;
        }
    }
    else if (MmsValue_getType(value) == MMS_DATA_ACCESS_ERROR) {
        self->lastAccessError = MmsValue_getDataAccessError(value);

        if (DEBUG_IED_CLIENT)
            printf("IED_CLIENT: select returned data-access-error: %i\n", self->lastAccessError);
    }
    else {
        if (DEBUG_IED_CLIENT)
            printf("IED_CLIENT: select: unexpected response from server!\n");
    }

    MmsValue_delete(value);

exit_function:
    return selected;
}

static void
internalSelectHandler(uint32_t invokeId, void* parameter, MmsError err, MmsValue* value)
{
    ControlObjectClient self = (ControlObjectClient) parameter;

    IedConnectionOutstandingCall call = iedConnection_lookupOutstandingCall(self->connection, invokeId);

    if (call) {

        ControlObjectClient_ControlActionHandler handler =  (ControlObjectClient_ControlActionHandler) call->callback;

        IedClientError iedError = iedConnection_mapMmsErrorToIedError(err);

        bool success = false;

        self->lastMmsError = err;
        self->lastAccessError = DATA_ACCESS_ERROR_SUCCESS;

        self->ctlNum++;

        if (iedError == IED_ERROR_OK) {

            if (MmsValue_getType(value) == MMS_DATA_ACCESS_ERROR) {
                MmsDataAccessError dataAccessError = MmsValue_getDataAccessError(value);
                self->lastAccessError = dataAccessError;
                iedError = iedConnection_mapDataAccessErrorToIedError(dataAccessError);
            }
            else if (MmsValue_getType(value) == MMS_VISIBLE_STRING) {

                char domainId[65];
                char itemId[65];

                MmsMapping_getMmsDomainFromObjectReference(self->objectReference, domainId);

                convertToMmsAndInsertFC(itemId, self->objectReference + strlen(domainId) + 1, "CO");

                strncat(itemId, "$SBO", 64);

                if (strcmp(MmsValue_toString(value), "") == 0) {
                    if (DEBUG_IED_CLIENT)
                        printf("select-response-\n");
                }
                else {
                    if (DEBUG_IED_CLIENT)
                        printf("select-response+: (%s)\n", MmsValue_toString(value));
                    success = true;
                }
            }
            else {
                if (DEBUG_IED_CLIENT)
                    printf("IED_CLIENT: select: unexpected response from server!\n");
            }
        }

        handler(invokeId, call->callbackParameter, iedError, CONTROL_ACTION_TYPE_SELECT, success);

        iedConnection_releaseOutstandingCall(self->connection, call);
    }
    else {
        if (DEBUG_IED_CLIENT)
            printf("IED_CLIENT: internal error - no matching outstanding call!\n");
    }

    MmsValue_delete(value);
}

uint32_t
ControlObjectClient_selectAsync(ControlObjectClient self, IedClientError* err, ControlObjectClient_ControlActionHandler handler, void* parameter)
{
    resetLastApplError(self);

    char domainId[65];
    char itemId[65];

    MmsMapping_getMmsDomainFromObjectReference(self->objectReference, domainId);

    convertToMmsAndInsertFC(itemId, self->objectReference + strlen(domainId) + 1, "CO");

    strncat(itemId, "$SBO", 64);

    IedConnectionOutstandingCall call = iedConnection_allocateOutstandingCall(self->connection);

    if (call == NULL) {
        *err = IED_ERROR_OUTSTANDING_CALL_LIMIT_REACHED;
        return 0;
    }

    call->callback = handler;
    call->callbackParameter = parameter;

    MmsError mmsError;

    if (DEBUG_IED_CLIENT)
        printf("IED_CLIENT: select: %s/%s\n", domainId, itemId);

    call->invokeId = MmsConnection_readVariableAsync(IedConnection_getMmsConnection(self->connection),
            &mmsError, domainId, itemId, internalSelectHandler, self);

    if (mmsError != MMS_ERROR_NONE) {
        iedConnection_releaseOutstandingCall(self->connection, call);
    }

    return call->invokeId;
}

static MmsValue*
createCancelParameters(ControlObjectClient self)
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

    uint64_t timestamp;

    if (self->useConstantT)
        timestamp = self->constantT;
    else
        timestamp = Hal_getTimeInMs();

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

    return cancelParameters;
}

bool
ControlObjectClient_cancel(ControlObjectClient self)
{
    resetLastApplError(self);

    MmsValue* cancelParameters = createCancelParameters(self);

    char domainId[65];
    char itemId[65];

    MmsMapping_getMmsDomainFromObjectReference(self->objectReference, domainId);

    convertToMmsAndInsertFC(itemId, self->objectReference + strlen(domainId) + 1, "CO");

    strncat(itemId, "$Cancel", 64);

    if (DEBUG_IED_CLIENT)
        printf("IED_CLIENT: cancel: %s/%s\n", domainId, itemId);

    MmsError mmsError;

    MmsDataAccessError writeResult = MmsConnection_writeVariable(IedConnection_getMmsConnection(self->connection),
            &mmsError, domainId, itemId, cancelParameters);

    self->lastMmsError = mmsError;
    self->lastAccessError = writeResult;

    MmsValue_setElement(cancelParameters, 0, NULL);
    MmsValue_delete(cancelParameters);

    if (mmsError != MMS_ERROR_NONE) {
        if (DEBUG_IED_CLIENT)
            printf("IED_CLIENT: cancel failed!\n");
        return false;
    }
    else {
        if (writeResult != DATA_ACCESS_ERROR_SUCCESS) {
            if (DEBUG_IED_CLIENT)
                printf("IED_CLIENT: cancel failed!\n");
            return false;
        }
    }

    return true;
}

static void
internalCancelHandler(uint32_t invokeId, void* parameter, MmsError err, MmsDataAccessError accessError)
{
    ControlObjectClient self = (ControlObjectClient) parameter;

    IedConnectionOutstandingCall call = iedConnection_lookupOutstandingCall(self->connection, invokeId);

    if (call) {

        ControlObjectClient_ControlActionHandler handler = (ControlObjectClient_ControlActionHandler) call->callback;

        IedClientError iedError = iedConnection_mapMmsErrorToIedError(err);

        bool success = false;

        self->lastMmsError = err;
        self->lastAccessError = accessError;

        if (iedError == IED_ERROR_OK) {
            iedError = iedConnection_mapDataAccessErrorToIedError(accessError);

            if (iedError == IED_ERROR_OK)
                success = true;
        }

        if (success) {
            if (DEBUG_IED_CLIENT)
                printf("IED_CLIENT: cancel+\n");
        }
        else {
            if (DEBUG_IED_CLIENT)
                printf("IED_CLIENT: cancel failed!\n");
        }

        handler(invokeId, call->callbackParameter, iedError, CONTROL_ACTION_TYPE_CANCEL, success);

        iedConnection_releaseOutstandingCall(self->connection, call);
    }
    else {
        if (DEBUG_IED_CLIENT)
            printf("IED_CLIENT: internal error - no matching outstanding call!\n");
    }
}

uint32_t
ControlObjectClient_cancelAsync(ControlObjectClient self, IedClientError* err, ControlObjectClient_ControlActionHandler handler, void* parameter)
{
    *err = IED_ERROR_OK;
    uint32_t invokeId = 0;

    IedConnectionOutstandingCall call = iedConnection_allocateOutstandingCall(self->connection);

    if (call == NULL) {
        *err = IED_ERROR_OUTSTANDING_CALL_LIMIT_REACHED;
        goto exit_function;
    }

    MmsValue* cancelParameters = createCancelParameters(self);

    resetLastApplError(self);

    char domainId[65];
    char itemId[65];

    MmsMapping_getMmsDomainFromObjectReference(self->objectReference, domainId);

    convertToMmsAndInsertFC(itemId, self->objectReference + strlen(domainId) + 1, "CO");

    strncat(itemId, "$Cancel", 64);

    call->callback = handler;
    call->callbackParameter = parameter;

    MmsError mmsError;

    if (DEBUG_IED_CLIENT)
        printf("IED_CLIENT: select with value: %s/%s\n", domainId, itemId);

    call->invokeId = MmsConnection_writeVariableAsync(self->connection->connection, &mmsError, domainId, itemId, cancelParameters, internalCancelHandler, self);

    invokeId = call->invokeId;

    MmsValue_setElement(cancelParameters, 0, NULL);
    MmsValue_delete(cancelParameters);

    *err = iedConnection_mapMmsErrorToIedError(mmsError);

    if (mmsError != MMS_ERROR_NONE) {
        iedConnection_releaseOutstandingCall(self->connection, call);
    }

exit_function:
    return invokeId;
}

void
ControlObjectClient_useConstantT(ControlObjectClient self, bool useConstantT)
{
    self->useConstantT = useConstantT;
}

void
ControlObjectClient_enableInterlockCheck(ControlObjectClient self)
{
    self->interlockCheck = true;
}

void
ControlObjectClient_setInterlockCheck(ControlObjectClient self, bool value)
{
    self->interlockCheck = value;
}

void
ControlObjectClient_enableSynchroCheck(ControlObjectClient self)
{
    self->synchroCheck = true;
}

void
ControlObjectClient_setSynchroCheck(ControlObjectClient self, bool value)
{
    self->synchroCheck = value;
}

void
ControlObjectClient_setTestMode(ControlObjectClient self, bool value)
{
    self->test = value;
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
ControlObjectClient_setCtlNum(ControlObjectClient self, uint8_t ctlNum)
{
    self->ctlNum = ctlNum;
}

void
controlObjectClient_invokeCommandTerminationHandler(ControlObjectClient self)
{
    if (self->commandTerminationHandler != NULL)
        self->commandTerminationHandler(self->commandTerminaionHandlerParameter, self);
}
