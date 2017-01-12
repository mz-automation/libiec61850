/*
 *  control.c
 *
 *  Copyright 2013 Michael Zillgith
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

#include "control.h"
#include "mms_mapping.h"
#include "mms_mapping_internal.h"

#include "mms_client_connection.h"

#include "ied_server_private.h"

#include "mms_value_internal.h"

#include "libiec61850_platform_includes.h"

#if (CONFIG_IEC61850_CONTROL_SERVICE == 1)

#ifndef DEBUG_IED_SERVER
#define DEBUG_IED_SERVER 0
#endif

#define CONTROL_ERROR_NO_ERROR 0
#define CONTROL_ERROR_UNKOWN 1
#define CONTROL_ERROR_TIMEOUT_TEST 2
#define CONTROL_ERROR_OPERATOR_TEST 3

#define STATE_UNSELECTED 0
#define STATE_READY 1
#define STATE_WAIT_FOR_ACTIVATION_TIME 2
#define STATE_PERFORM_TEST 3
#define STATE_WAIT_FOR_EXECUTION 4
#define STATE_OPERATE 5

struct sControlObject
{
    MmsDomain* mmsDomain;
    IedServer iedServer;
    char* lnName;
    char* name;

    int state;

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore stateLock;
#endif

    MmsValue* mmsValue;
    MmsVariableSpecification* typeSpec;

    MmsValue* oper;
    MmsValue* sbo;
    MmsValue* sbow;
    MmsValue* cancel;

    MmsValue* ctlVal;
    MmsValue* ctlNum;
    MmsValue* origin;
    MmsValue* timestamp;

    char* ctlObjectName;

    /* for LastAppIError */
    MmsValue* error;
    MmsValue* addCause;

    bool selected;
    uint64_t selectTime;
    uint32_t selectTimeout;
    MmsValue* sboClass;
    MmsValue* sboTimeout;

    bool timeActivatedOperate;
    uint64_t operateTime;

    bool operateOnce;
    MmsServerConnection mmsConnection;

    MmsValue* emptyString;

    bool initialized;
    uint32_t ctlModel;

    bool testMode;
    bool interlockCheck;
    bool synchroCheck;

    uint32_t operateInvokeId;

    ControlHandler operateHandler;
    void* operateHandlerParameter;

    ControlPerformCheckHandler checkHandler;
    void* checkHandlerParameter;

    ControlWaitForExecutionHandler waitForExecutionHandler;
    void* waitForExecutionHandlerParameter;
};

void
ControlObject_sendLastApplError(ControlObject* self, MmsServerConnection connection, char* ctlVariable, int error,
        ControlAddCause addCause, MmsValue* ctlNum, MmsValue* origin, bool handlerMode);

void
ControlObject_sendCommandTerminationPositive(ControlObject* self);

void
ControlObject_sendCommandTerminationNegative(ControlObject* self);

MmsValue*
Control_readAccessControlObject(MmsMapping* self, MmsDomain* domain, char* variableIdOrig,
        MmsServerConnection connection);

static void
unselectObject(ControlObject* self);

static void
setState(ControlObject* self, int newState)
{
#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_wait(self->stateLock);
#endif

    self->state = newState;

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_post(self->stateLock);
#endif
}

static int
getState(ControlObject* self)
{
    int state;

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_wait(self->stateLock);
#endif

    state = self->state;

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_post(self->stateLock);
#endif

    return state;
}

static void
updateSboTimeoutValue(ControlObject* self)
{
    if (self->sboTimeout != NULL) {
        uint32_t sboTimeoutVal = MmsValue_toInt32(self->sboTimeout);

        if (DEBUG_IED_SERVER)
            printf("set timeout for %s to %u\n", self->ctlObjectName, sboTimeoutVal);

        self->selectTimeout = sboTimeoutVal;
    }
    else
        self->selectTimeout = CONFIG_CONTROL_DEFAULT_SBO_TIMEOUT;
}

static void
initialize(ControlObject* self)
{
    if (!(self->initialized)) {

        MmsServer mmsServer = IedServer_getMmsServer(self->iedServer);

        self->emptyString = MmsValue_newVisibleString(NULL);

        char* ctlModelName = StringUtils_createString(4, self->lnName, "$CF$", self->name, "$ctlModel");

        if (DEBUG_IED_SERVER)
            printf("initialize control for %s\n", ctlModelName);

        MmsValue* ctlModel = MmsServer_getValueFromCache(mmsServer,
                self->mmsDomain, ctlModelName);

        if (ctlModel == NULL) {
            if (DEBUG_IED_SERVER)
                printf("No control model found for variable %s\n", ctlModelName);
        }

        GLOBAL_FREEMEM(ctlModelName);

        char* sboClassName = StringUtils_createString(4, self->lnName, "$CF$", self->name, "$sboClass");

        self->sboClass = MmsServer_getValueFromCache(mmsServer, self->mmsDomain, sboClassName);

        GLOBAL_FREEMEM(sboClassName);

        self->ctlObjectName = (char*) GLOBAL_MALLOC(130);

        StringUtils_createStringInBuffer(self->ctlObjectName, 5, MmsDomain_getName(self->mmsDomain), "/",
                self->lnName, "$CO$", self->name);

        self->error = MmsValue_newIntegerFromInt32(0);
        self->addCause = MmsValue_newIntegerFromInt32(0);

        if (ctlModel != NULL) {
            uint32_t ctlModelVal = MmsValue_toInt32(ctlModel);

            self->ctlModel = ctlModelVal;

            if (DEBUG_IED_SERVER)
                printf("  ctlModel: %i\n", ctlModelVal);

            if ((ctlModelVal == 2) || (ctlModelVal == 4)) { /* SBO */
                char* sboTimeoutName = StringUtils_createString(4, self->lnName, "$CF$", self->name, "$sboTimeout");

                char* controlObjectReference = StringUtils_createString(6, self->mmsDomain->domainName, "/", self->lnName, "$",
                        self->name, "$SBO");

                self->sbo = MmsValue_newVisibleString(controlObjectReference);

                self->sboTimeout = MmsServer_getValueFromCache(mmsServer,
                        self->mmsDomain, sboTimeoutName);

                updateSboTimeoutValue(self);

                setState(self, STATE_UNSELECTED);

                if (DEBUG_IED_SERVER)
                    printf("timeout for %s is %i\n", sboTimeoutName, self->selectTimeout);

                GLOBAL_FREEMEM(controlObjectReference);
                GLOBAL_FREEMEM(sboTimeoutName);
            }
            else {
                self->sbo = MmsValue_newVisibleString(NULL);

                setState(self, STATE_READY);
            }
        }

        self->initialized = true;
    }
}

static bool
isSboClassOperateOnce(ControlObject* self)
{
    if (self->sboClass != NULL) {
        if (MmsValue_toInt32(self->sboClass) == 1)
            return false;
        else
            return true;
    }
    else
        return true; /* default is operate-once ! */
}



static MmsValue*
getOperParameterOperTime(MmsValue* operParameters)
{
    if (MmsValue_getType(operParameters) == MMS_STRUCTURE) {

        if (MmsValue_getArraySize(operParameters) == 7)
            return MmsValue_getElement(operParameters, 1);
    }

    return NULL;
}

static bool
initiateControlTask(ControlObject* self)
{
    ClientConnection iedClientConnection =
            private_IedServer_getClientConnectionByHandle(self->iedServer, self->mmsConnection);

    if (iedClientConnection == NULL)
        return false;

    private_ClientConnection_increaseTasksCount(iedClientConnection);

    return true;
}

static void
exitControlTask(ControlObject* self)
{
    ClientConnection iedClientConnection =
            private_IedServer_getClientConnectionByHandle(self->iedServer, self->mmsConnection);

    if (iedClientConnection == NULL)
        return;

    /* synchronize with connection management */
    private_ClientConnection_decreaseTasksCount(iedClientConnection);
}

static void
abortControlOperation(ControlObject* self)
{
    if ((self->ctlModel == 2) || (self->ctlModel == 4)) {

        if (isSboClassOperateOnce(self))
            unselectObject(self);
        else
            setState(self, STATE_READY);
    }
    else
        setState(self, STATE_READY);

    exitControlTask(self);
}

static ControlHandlerResult
operateControl(ControlObject* self, MmsValue* value, uint64_t currentTime, bool testCondition)
{
    self->selectTime = currentTime;

    if (self->operateHandler != NULL)
        return self->operateHandler(self->operateHandlerParameter, value, testCondition);

    return CONTROL_RESULT_OK;
}

static void
executeControlTask(ControlObject* self)
{
    int state;

executeStateMachine:

    state = getState(self);

    switch (state) {

    case STATE_WAIT_FOR_ACTIVATION_TIME:
    case STATE_WAIT_FOR_EXECUTION:
    {
        ControlHandlerResult dynamicCheckResult = CONTROL_RESULT_OK;
        bool isTimeActivatedControl = false;

        if (state == STATE_WAIT_FOR_ACTIVATION_TIME)
           isTimeActivatedControl = true;

        if (self->waitForExecutionHandler != NULL) {
            dynamicCheckResult = self->waitForExecutionHandler(self->waitForExecutionHandlerParameter, self->ctlVal,
                    self->testMode, self->synchroCheck);
        }

        if (dynamicCheckResult == CONTROL_RESULT_FAILED) {
            if (isTimeActivatedControl) {
                ControlObject_sendLastApplError(self, self->mmsConnection, "Oper",
                        CONTROL_ERROR_NO_ERROR, ADD_CAUSE_BLOCKED_BY_SYNCHROCHECK,
                        self->ctlNum, self->origin, false);
            }
            else
                MmsServerConnection_sendWriteResponse(self->mmsConnection, self->operateInvokeId,
                        DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED, true);

            abortControlOperation(self);
            exitControlTask(self);
        }
        else if (dynamicCheckResult == CONTROL_RESULT_OK) {
            if (isTimeActivatedControl) {
                ControlObject_sendCommandTerminationPositive(self);

                MmsValue* operTm = getOperParameterOperTime(self->oper);

                MmsValue_setUtcTime(operTm, 0);

            }
            else
                MmsServerConnection_sendWriteResponse(self->mmsConnection, self->operateInvokeId,
                        DATA_ACCESS_ERROR_SUCCESS, true);

            setState(self, STATE_OPERATE);

            goto executeStateMachine;
        }
    }
    break;

    case STATE_OPERATE:
    {
        uint64_t currentTime = Hal_getTimeInMs();

        ControlHandlerResult result = operateControl(self, self->ctlVal, currentTime, self->testMode);

        if (result != CONTROL_RESULT_WAITING) {

            if (result == CONTROL_RESULT_OK) {

                if ((self->ctlModel == 4) || (self->ctlModel == 3)) {
                    ControlObject_sendCommandTerminationPositive(self);
                }
            }
            else {

                if ((self->ctlModel == 4) || (self->ctlModel == 3)) {
                    if (DEBUG_IED_SERVER)
                        printf("IED_SERVER: operate failed!\n");

                    ControlObject_sendCommandTerminationNegative(self);
                }
            }

            abortControlOperation(self);
            exitControlTask(self);
        }
    }
    break;

    }
}

ControlObject*
ControlObject_create(IedServer iedServer, MmsDomain* domain, char* lnName, char* name)
{
    ControlObject* self = (ControlObject*) GLOBAL_CALLOC(1, sizeof(ControlObject));

    if (self == NULL)
        goto exit_function;

    if (DEBUG_IED_SERVER)
        printf("create control object for LD: %s, LN: %s, name: %s\n", domain->domainName, lnName, name);

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    self->stateLock = Semaphore_create(1);

    if (self->stateLock == NULL) {
        ControlObject_destroy(self);
        self = NULL;
        goto exit_function;
    }
#endif

    self->name = StringUtils_copyString(name);

    if (self->name == NULL) {
        ControlObject_destroy(self);
        self = NULL;
        goto exit_function;
    }

    self->lnName = lnName;
    self->mmsDomain = domain;
    self->iedServer = iedServer;

exit_function:
    return self;
}

void
ControlObject_destroy(ControlObject* self)
{
    if (self->mmsValue != NULL)
        MmsValue_delete(self->mmsValue);

    if (self->sbo != NULL)
        MmsValue_delete(self->sbo);

    if (self->emptyString != NULL)
        MmsValue_delete(self->emptyString);

    if (self->ctlObjectName != NULL)
        GLOBAL_FREEMEM(self->ctlObjectName);

    if (self->error != NULL)
        MmsValue_delete(self->error);

    if (self->addCause != NULL)
        MmsValue_delete(self->addCause);

    if (self->ctlVal != NULL)
        MmsValue_delete(self->ctlVal);

    if (self->ctlNum != NULL)
        MmsValue_delete(self->ctlNum);

    if (self->origin != NULL)
        MmsValue_delete(self->origin);

    if (self->name != NULL)
        GLOBAL_FREEMEM(self->name);

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    if (self->stateLock != NULL)
        Semaphore_destroy(self->stateLock);
#endif

    GLOBAL_FREEMEM(self);
}

void
ControlObject_setOper(ControlObject* self, MmsValue* oper)
{
    self->oper = oper;
}

void
ControlObject_setCancel(ControlObject* self, MmsValue* cancel)
{
    self->cancel = cancel;
}

void
ControlObject_setSBOw(ControlObject* self, MmsValue* sbow)
{
    self->sbow = sbow;
}

void
ControlObject_setSBO(ControlObject* self, MmsValue* sbo)
{
    self->sbo = sbo;
}

void
ControlObject_setCtlVal(ControlObject* self, MmsValue* ctlVal)
{
    self->ctlVal = ctlVal;
}

char*
ControlObject_getName(ControlObject* self)
{
    return self->name;
}

char*
ControlObject_getLNName(ControlObject* self)
{
    return self->lnName;
}

MmsDomain*
ControlObject_getDomain(ControlObject* self)
{
    return self->mmsDomain;
}

MmsValue*
ControlObject_getOper(ControlObject* self)
{
    return self->oper;
}

MmsValue*
ControlObject_getSBOw(ControlObject* self)
{
    return self->sbow;
}

MmsValue*
ControlObject_getSBO(ControlObject* self)
{
    return self->sbo;
}

MmsValue*
ControlObject_getCancel(ControlObject* self)
{
    return self->cancel;
}

void
ControlObject_setMmsValue(ControlObject* self, MmsValue* value)
{
    self->mmsValue = value;
}

void
ControlObject_setTypeSpec(ControlObject* self, MmsVariableSpecification* typeSpec)
{
    self->typeSpec = typeSpec;
}

MmsVariableSpecification*
ControlObject_getTypeSpec(ControlObject* self)
{
    return self->typeSpec;
}

MmsValue*
ControlObject_getMmsValue(ControlObject* self)
{
    return self->mmsValue;
}

static void
selectObject(ControlObject* self, uint64_t selectTime, MmsServerConnection connection)
{
    if (DEBUG_IED_SERVER)
        printf("IED_SERVER: control %s selected\n", self->ctlObjectName);

    updateSboTimeoutValue(self);
    self->selected = true;
    self->selectTime = selectTime;
    self->mmsConnection = connection;
    setState(self, STATE_READY);
}

static void
unselectObject(ControlObject* self)
{
    setState(self, STATE_UNSELECTED);

    if (DEBUG_IED_SERVER)
        printf("IED_SERVER: control %s unselected\n", self->ctlObjectName);
}

static void
checkSelectTimeout(ControlObject* self, uint64_t currentTime)
{
    if ((self->ctlModel == 2) || (self->ctlModel == 4)) {

        if (getState(self) == STATE_READY) {
            if (self->selectTimeout > 0) {
                if (currentTime > (self->selectTime + self->selectTimeout)) {
                    if (DEBUG_IED_SERVER)
                        printf("IED_SERVER: select-timeout (timeout-val = %i) for control %s\n",
                                self->selectTimeout, self->ctlObjectName);

                    unselectObject(self);
                }
            }
        }
    }
}

bool
ControlObject_unselect(ControlObject* self, MmsServerConnection connection)
{
    if (self->mmsConnection == connection) {
        abortControlOperation(self);
        return true;
    }
    else
        return false;
}

void
ControlObject_installListener(ControlObject* self, ControlHandler listener, void* parameter)
{
    self->operateHandler = listener;
    self->operateHandlerParameter = parameter;
}

void
ControlObject_installCheckHandler(ControlObject* self, ControlPerformCheckHandler handler, void* parameter)
{
    self->checkHandler = handler;
    self->checkHandlerParameter = parameter;
}

void
ControlObject_installWaitForExecutionHandler(ControlObject* self, ControlWaitForExecutionHandler handler,
        void* parameter)
{
    self->waitForExecutionHandler = handler;
    self->waitForExecutionHandlerParameter = parameter;
}

void
Control_processControlActions(MmsMapping* self, uint64_t currentTimeInMs)
{
    LinkedList element = LinkedList_getNext(self->controlObjects);

    while (element != NULL) {
        ControlObject* controlObject = (ControlObject*) element->data;

        if (controlObject->state == STATE_WAIT_FOR_ACTIVATION_TIME) {

            if (controlObject->operateTime <= currentTimeInMs) {

                if (DEBUG_IED_SERVER)
                    printf("time activated operate: start operation\n");

                controlObject->timeActivatedOperate = false;

                CheckHandlerResult checkResult = CONTROL_ACCEPTED;

                if (controlObject->checkHandler != NULL) { /* perform operative tests */

                    ClientConnection clientConnection = private_IedServer_getClientConnectionByHandle(self->iedServer,
                            controlObject->mmsConnection);

                    checkResult = controlObject->checkHandler(
                            controlObject->checkHandlerParameter, controlObject->ctlVal, controlObject->testMode,
                            controlObject->interlockCheck, clientConnection);
                }

                if (checkResult == CONTROL_ACCEPTED) {
                    executeControlTask(controlObject);
                }
                else {
                    ControlObject_sendLastApplError(controlObject, controlObject->mmsConnection, "Oper",
                    CONTROL_ERROR_NO_ERROR, ADD_CAUSE_BLOCKED_BY_INTERLOCKING,
                            controlObject->ctlNum, controlObject->origin, false);

                    abortControlOperation(controlObject);
                }
            }

        } /* if (controlObject->state == STATE_WAIT_FOR_ACTICATION_TIME) */
        else if (!((controlObject->state == STATE_UNSELECTED) || (controlObject->state == STATE_READY))) {
            executeControlTask(controlObject);
        }

        element = LinkedList_getNext(element);
    }
}

ControlObject*
Control_lookupControlObject(MmsMapping* self, MmsDomain* domain, char* lnName, char* objectName)
{
    LinkedList element = LinkedList_getNext(self->controlObjects);

    while (element != NULL) {
        ControlObject* controlObject = (ControlObject*) element->data;

        if (ControlObject_getDomain(controlObject) == domain) {
            if (strcmp(ControlObject_getLNName(controlObject), lnName) == 0) {
                if (strcmp(ControlObject_getName(controlObject), objectName) == 0) {
                    return controlObject;
                }
            }
        }

        element = LinkedList_getNext(element);
    }

    return NULL;
}

static MmsValue*
getCtlVal(MmsValue* operParameters)
{
    if (MmsValue_getType(operParameters) == MMS_STRUCTURE) {
        if (MmsValue_getArraySize(operParameters) > 5) {
            return MmsValue_getElement(operParameters, 0);
        }
    }

    return NULL;
}

static MmsValue*
getOperParameterCtlNum(MmsValue* operParameters)
{
    if (MmsValue_getType(operParameters) == MMS_STRUCTURE) {
        if (MmsValue_getArraySize(operParameters) == 7)
            return MmsValue_getElement(operParameters, 3);
        else if (MmsValue_getArraySize(operParameters) == 6)
            return MmsValue_getElement(operParameters, 2);
    }

    return NULL;
}

static MmsValue*
getCancelParameterCtlNum(MmsValue* operParameters)
{
    if (MmsValue_getType(operParameters) == MMS_STRUCTURE) {
        if (MmsValue_getArraySize(operParameters) == 6)
            return MmsValue_getElement(operParameters, 3);
        else if (MmsValue_getArraySize(operParameters) == 5)
            return MmsValue_getElement(operParameters, 2);
    }

    return NULL;
}

static MmsValue*
getCancelParameterOrigin(MmsValue* operParameters)
{
    if (MmsValue_getType(operParameters) == MMS_STRUCTURE) {
        if (MmsValue_getArraySize(operParameters) == 6)
            return MmsValue_getElement(operParameters, 2);
        else if (MmsValue_getArraySize(operParameters) == 5)
            return MmsValue_getElement(operParameters, 1);
    }

    return NULL;
}

static MmsValue*
getOperParameterTest(MmsValue* operParameters)
{
    if (MmsValue_getType(operParameters) == MMS_STRUCTURE)
    {
        if (MmsValue_getArraySize(operParameters) == 7)
            return MmsValue_getElement(operParameters, 5);
        else if (MmsValue_getArraySize(operParameters) == 6)
            return MmsValue_getElement(operParameters, 4);
    }

    return NULL;
}

static MmsValue*
getOperParameterCheck(MmsValue* operParameters)
{
    if (MmsValue_getType(operParameters) == MMS_STRUCTURE)
    {
        if (MmsValue_getArraySize(operParameters) == 7)
            return MmsValue_getElement(operParameters, 6);
        else if (MmsValue_getArraySize(operParameters) == 6)
            return MmsValue_getElement(operParameters, 5);
    }

    return NULL;
}

static MmsValue*
getOperParameterOrigin(MmsValue* operParameters)
{
    if (MmsValue_getType(operParameters) == MMS_STRUCTURE)
    {
        if (MmsValue_getArraySize(operParameters) == 7)
            return MmsValue_getElement(operParameters, 2);
        else if (MmsValue_getArraySize(operParameters) == 6)
            return MmsValue_getElement(operParameters, 1);
    }

    return NULL;
}

static MmsValue*
getOperParameterTime(MmsValue* operParameters)
{
    MmsValue* timeParameter = NULL;

    if (MmsValue_getType(operParameters) == MMS_STRUCTURE)
    {
        if (MmsValue_getArraySize(operParameters) == 7)
            timeParameter = MmsValue_getElement(operParameters, 4);
        else if (MmsValue_getArraySize(operParameters) == 6)
            timeParameter = MmsValue_getElement(operParameters, 3);
    }

    if (timeParameter != NULL)
        if ((MmsValue_getType(timeParameter) == MMS_UTC_TIME) || (MmsValue_getType(timeParameter) == MMS_BINARY_TIME))
            return timeParameter;

    return NULL;
}

void
ControlObject_sendCommandTerminationPositive(ControlObject* self)
{
    char itemId[68]; /* 64 characters + space for FC + separator + string terminator */

    StringUtils_createStringInBuffer(itemId, 4, self->lnName, "$CO$", self->name, "$Oper");

    if (DEBUG_IED_SERVER)
        printf("IED_SERVER: send CommandTermination+: %s\n", itemId);

    char* domainId = MmsDomain_getName(self->mmsDomain);

    MmsVariableAccessSpecification varSpec;

    varSpec.itemId = itemId;
    varSpec.domainId = domainId;

    LinkedList varSpecList = LinkedList_create();
    LinkedList values = LinkedList_create();

    if ((varSpecList != NULL) && (values != NULL))
    {
        LinkedList_add(varSpecList, &varSpec);
        LinkedList_add(values, self->oper);

        MmsServerConnection_sendInformationReportListOfVariables(self->mmsConnection, varSpecList, values, false);

        LinkedList_destroyStatic(varSpecList);
        LinkedList_destroyStatic(values);
    }
}

void
ControlObject_sendCommandTerminationNegative(ControlObject* self)
{
    /* create LastApplError */

    MmsValue lastApplErrorMemory;

    MmsValue* lastApplError = &lastApplErrorMemory;
    lastApplError->type = MMS_STRUCTURE;
    lastApplError->value.structure.size = 5;

    MmsValue* componentContainer[5];

    lastApplError->value.structure.components = componentContainer;

    char ctlObj[130];

    StringUtils_createStringInBuffer(ctlObj, 2, self->ctlObjectName, "$Oper");

    MmsValue ctlObjValueMemory;

    MmsValue* ctlObjValue = &ctlObjValueMemory;
    ctlObjValue->type = MMS_VISIBLE_STRING;
    ctlObjValue->value.visibleString.buf = ctlObj;
    ctlObjValue->value.visibleString.size = sizeof(ctlObj);

    MmsValue_setElement(lastApplError, 0, ctlObjValue);

    MmsValue_setInt32(self->error, CONTROL_ERROR_UNKOWN);
    MmsValue_setInt32(self->addCause, ADD_CAUSE_UNKNOWN);

    MmsValue_setElement(lastApplError, 1, self->error);
    MmsValue_setElement(lastApplError, 2, self->origin);
    MmsValue_setElement(lastApplError, 3, self->ctlNum);
    MmsValue_setElement(lastApplError, 4, self->addCause);

    MmsVariableAccessSpecification lastApplErrorVarSpec;

    lastApplErrorVarSpec.itemId = "LastApplError";
    lastApplErrorVarSpec.domainId = NULL;

    /* create oper variable */

    char itemId[130];

    StringUtils_createStringInBuffer(itemId, 4, self->lnName, "$CO$", self->name, "$Oper");

    char* domainId = MmsDomain_getName(self->mmsDomain);

    MmsVariableAccessSpecification operVarSpec;

    operVarSpec.itemId = itemId;
    operVarSpec.domainId = domainId;


    /* create response */

    if (DEBUG_IED_SERVER)
        printf("IED_SERVER: send CommandTermination-: %s\n", itemId);

    LinkedList varSpecList = LinkedList_create();
    LinkedList values = LinkedList_create();

    LinkedList_add(varSpecList, &lastApplErrorVarSpec);
    LinkedList_add(varSpecList, &operVarSpec);
    LinkedList_add(values, lastApplError);
    LinkedList_add(values, self->oper);

    MmsServerConnection_sendInformationReportListOfVariables(self->mmsConnection, varSpecList, values, false);

    LinkedList_destroyStatic(varSpecList);
    LinkedList_destroyStatic(values);
} /* ControlObject_sendCommandTerminationNegative() */



void
ControlObject_sendLastApplError(ControlObject* self, MmsServerConnection connection, char* ctlVariable, int error,
        ControlAddCause addCause, MmsValue* ctlNum, MmsValue* origin, bool handlerMode)
{
    MmsValue lastApplErrorMemory;

    MmsValue* lastApplError = &lastApplErrorMemory;
    lastApplError->type = MMS_STRUCTURE;
    lastApplError->value.structure.size = 5;

    MmsValue* componentContainer[5];

    lastApplError->value.structure.components =componentContainer;

    char ctlObj[130];

    StringUtils_createStringInBuffer(ctlObj, 3, self->ctlObjectName, "$", ctlVariable);

    if (DEBUG_IED_SERVER) {
        printf("IED_SERVER: sendLastApplError:\n");
        printf("IED_SERVER:    control object: %s\n", ctlObj);
        printf("IED_SERVER:    ctlNum: %u\n", MmsValue_toUint32(ctlNum));
    }

    MmsValue ctlObjValueMemory;

    MmsValue* ctlObjValue = &ctlObjValueMemory;
    ctlObjValue->type = MMS_VISIBLE_STRING;
    ctlObjValue->value.visibleString.buf = ctlObj;
    ctlObjValue->value.visibleString.size = sizeof(ctlObj);

    MmsValue_setElement(lastApplError, 0, ctlObjValue);

    MmsValue_setInt32(self->error, error);
    MmsValue_setInt32(self->addCause, addCause);

    MmsValue_setElement(lastApplError, 1, self->error);
    MmsValue_setElement(lastApplError, 2, origin);
    MmsValue_setElement(lastApplError, 3, ctlNum);
    MmsValue_setElement(lastApplError, 4, self->addCause);

    MmsServerConnection_sendInformationReportSingleVariableVMDSpecific(connection,
            "LastApplError", lastApplError, handlerMode);
}

static void
updateControlParameters(ControlObject* controlObject, MmsValue* ctlVal, MmsValue* ctlNum, MmsValue* origin)
{
    if (controlObject->ctlVal != NULL)
        MmsValue_delete(controlObject->ctlVal);

    if (controlObject->ctlNum != NULL)
        MmsValue_delete(controlObject->ctlNum);

    if (controlObject->origin != NULL)
        MmsValue_delete(controlObject->origin);

    controlObject->ctlVal = MmsValue_clone(ctlVal);
    controlObject->ctlNum = MmsValue_clone(ctlNum);
    controlObject->origin = MmsValue_clone(origin);
}

static bool
doesElementEquals(char* element, char* name)
{
    int i = 0;

    while (name[i] != 0) {
        if (element[i] == 0)
            return false;

        if (element[i] != name[i])
            return false;

        i++;
    }

    if ((element[i] != 0) && (element[i] != '$'))
        return false;

    return true;
}

MmsValue*
Control_readAccessControlObject(MmsMapping* self, MmsDomain* domain, char* variableIdOrig,
        MmsServerConnection connection)
{
    MmsValue* value = NULL;

    if (DEBUG_IED_SERVER)
        printf("IED_SERVER: readAccessControlObject: %s\n", variableIdOrig);

    char variableId[129];

    strncpy(variableId, variableIdOrig, 128);
    variableId[128] = 0;

    char* separator = strchr(variableId, '$');

    if (separator == NULL)
        return NULL;

    *separator = 0;

    char* lnName = variableId;

    if (lnName == NULL)
        return NULL;

    char* objectName = MmsMapping_getNextNameElement(separator + 1);

    if (objectName == NULL)
        return NULL;

    char* varName =  MmsMapping_getNextNameElement(objectName);

    if (varName != NULL) {

        bool foundVar = false;

        char* nextVarName = varName;

        do {
            if (doesElementEquals(varName, "Oper") ||
                doesElementEquals(varName, "SBO") ||
                doesElementEquals(varName, "SBOw") ||
                doesElementEquals(varName, "Cancel"))
            {
                *(varName - 1) = 0;
                foundVar = true;
                break;
            }

            nextVarName  = MmsMapping_getNextNameElement(varName);

            if (nextVarName != NULL)
                varName = nextVarName;

        } while (nextVarName != NULL);

        if (foundVar == false)
            varName = NULL;
    }

    if (DEBUG_IED_SERVER)
        printf("IED_SERVER: read_access control object: objectName: (%s) varName: (%s)\n", objectName, varName);

    ControlObject* controlObject = Control_lookupControlObject(self, domain, lnName, objectName);

    if (controlObject != NULL) {

        initialize(controlObject);

        if (varName != NULL) {
            if (strcmp(varName, "Oper") == 0)
                value = ControlObject_getOper(controlObject);
            else if (strcmp(varName, "SBOw") == 0)
                value = ControlObject_getSBOw(controlObject);
            else if (strcmp(varName, "SBO") == 0) {
                if (controlObject->ctlModel == 2) {

                    uint64_t currentTime = Hal_getTimeInMs();

                    value = controlObject->emptyString;

                    checkSelectTimeout(controlObject, currentTime);

                    if (getState(controlObject) == STATE_UNSELECTED) {
                        CheckHandlerResult checkResult = CONTROL_ACCEPTED;

                        if (controlObject->checkHandler != NULL) { /* perform operative tests */

                            ClientConnection clientConnection = private_IedServer_getClientConnectionByHandle(self->iedServer,
                                                                                connection);

                            checkResult = controlObject->checkHandler(
                                    controlObject->checkHandlerParameter, NULL, false, false,
                                    clientConnection);
                        }

                        if (checkResult == CONTROL_ACCEPTED) {
                            selectObject(controlObject, currentTime, connection);
                            value = ControlObject_getSBO(controlObject);
                        }
                    }

                }
                else {
                    if (DEBUG_IED_SERVER)
                        printf("IED_SERVER: select not applicable for control model %i\n", controlObject->ctlModel);

                    value = ControlObject_getSBO(controlObject);
                }
            }

            else if (strcmp(varName, "Cancel") == 0)
                value = ControlObject_getCancel(controlObject);
            else {
                value = MmsValue_getSubElement(ControlObject_getMmsValue(controlObject),
                        ControlObject_getTypeSpec(controlObject), varName);
            }
        }
        else
            value = ControlObject_getMmsValue(controlObject);
    }

    return value;
}

//INFO: function can be removed if no certification is required
static bool
checkValidityOfOriginParameter(MmsValue* origin)
{
    if (MmsValue_getType(origin) != MMS_STRUCTURE)
        return false;

    if (MmsValue_getArraySize(origin) != 2)
        return false;

    MmsValue* orIdent = MmsValue_getElement(origin, 1);

    if (MmsValue_getType(orIdent) != MMS_OCTET_STRING)
        return false;

    if (MmsValue_getOctetStringSize(orIdent) > 64)
        return false;

    MmsValue* orCat = MmsValue_getElement(origin, 0);

    if (MmsValue_getType(orCat) != MMS_INTEGER)
        return false;

    int orCatIntValue = MmsValue_toInt32(orCat);

    if ((orCatIntValue < 0) || (orCatIntValue > 8))
        return false;

    return true;
}

static MmsDataAccessError
getDataAccessErrorFromCheckHandlerResult(CheckHandlerResult checkResult)
{
    MmsDataAccessError indication;

    if (checkResult == CONTROL_HARDWARE_FAULT)
        indication = DATA_ACCESS_ERROR_HARDWARE_FAULT;
    else
    if (checkResult == CONTROL_TEMPORARILY_UNAVAILABLE)
        indication = DATA_ACCESS_ERROR_TEMPORARILY_UNAVAILABLE;
    else
    if (checkResult == CONTROL_OBJECT_UNDEFINED)
        indication = DATA_ACCESS_ERROR_OBJECT_UNDEFINED;
    else if (checkResult == CONTROL_OBJECT_ACCESS_DENIED)
        indication = DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED;
    else
        indication = DATA_ACCESS_ERROR_SUCCESS;

    return indication;
}

MmsDataAccessError
Control_writeAccessControlObject(MmsMapping* self, MmsDomain* domain, char* variableIdOrig,
        MmsValue* value, MmsServerConnection connection)
{
    MmsDataAccessError indication = DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED;

    if (DEBUG_IED_SERVER)
        printf("IED_SERVER: writeAccessControlObject: %s\n", variableIdOrig);

    char variableId[65];

    strncpy(variableId, variableIdOrig, 64);
    variableId[64] = 0;

    char* separator = strchr(variableId, '$');

    if (separator == NULL)
        goto free_and_return;

    *separator = 0;

    char* lnName = variableId;

    if (lnName == NULL)
        goto free_and_return;

    char* objectName = MmsMapping_getNextNameElement(separator + 1);

    if (objectName == NULL)
        goto free_and_return;

    char* varName =  MmsMapping_getNextNameElement(objectName);

    if (varName != NULL) {

        bool foundVar = false;

        char* nextVarName = varName;

        do {
            if (doesElementEquals(varName, "Oper") ||
                doesElementEquals(varName, "SBO") ||
                doesElementEquals(varName, "SBOw") ||
                doesElementEquals(varName, "Cancel"))
            {
                *(varName - 1) = 0;
                foundVar = true;
                break;
            }

            nextVarName  = MmsMapping_getNextNameElement(varName);

            if (nextVarName != NULL)
                varName = nextVarName;

        } while (nextVarName != NULL);

        if (foundVar == false)
            varName = NULL;
    }


    if (DEBUG_IED_SERVER)
        printf("IED_SERVER: write access control: objectName: (%s) varName: (%s)\n", objectName, varName);


    if (varName == NULL) {
        indication = DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED;
        goto free_and_return;
    }

    ControlObject* controlObject = Control_lookupControlObject(self, domain, lnName, objectName);

    if (controlObject == NULL) {
        indication = DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED;
        goto free_and_return;
    }

    initialize(controlObject);

    if (strcmp(varName, "SBOw") == 0) { /* select with value */

        if (controlObject->ctlModel == 4) {

            MmsValue* ctlVal = getCtlVal(value);

            if (ctlVal != NULL) {

                MmsValue* ctlNum = getOperParameterCtlNum(value);
                MmsValue* origin = getOperParameterOrigin(value);
                MmsValue* check = getOperParameterCheck(value);
                MmsValue* test = getOperParameterTest(value);

                if (checkValidityOfOriginParameter(origin) == false) {
                    indication = DATA_ACCESS_ERROR_OBJECT_VALUE_INVALID;
                    goto free_and_return;
                }

                uint64_t currentTime = Hal_getTimeInMs();

                checkSelectTimeout(controlObject, currentTime);

                int state = getState(controlObject);

                if (state != STATE_UNSELECTED) {
                    indication = DATA_ACCESS_ERROR_TEMPORARILY_UNAVAILABLE;

                    if (connection != controlObject->mmsConnection)
                        ControlObject_sendLastApplError(controlObject, connection, "SBOw", 0,
                                ADD_CAUSE_LOCKED_BY_OTHER_CLIENT, ctlNum, origin, true);
                    else
                        ControlObject_sendLastApplError(controlObject, connection, "SBOw", 0,
                                ADD_CAUSE_OBJECT_ALREADY_SELECTED, ctlNum, origin, true);

                    if (DEBUG_IED_SERVER)
                        printf("SBOw: select failed!\n");
                }
                else {

                    CheckHandlerResult checkResult = CONTROL_ACCEPTED;

                    bool interlockCheck = MmsValue_getBitStringBit(check, 1);

                    bool testCondition = MmsValue_getBoolean(test);

                    if (controlObject->checkHandler != NULL) { /* perform operative tests */

                        ClientConnection clientConnection = private_IedServer_getClientConnectionByHandle(self->iedServer,
                                                    connection);

                        checkResult = controlObject->checkHandler(
                                controlObject->checkHandlerParameter, ctlVal, testCondition, interlockCheck,
                                clientConnection);
                    }

                    if (checkResult == CONTROL_ACCEPTED) {
                        selectObject(controlObject, currentTime, connection);

                        updateControlParameters(controlObject, ctlVal, ctlNum, origin);

                        indication = DATA_ACCESS_ERROR_SUCCESS;

                        if (DEBUG_IED_SERVER)
                            printf("SBOw: selected successful\n");
                    }
                    else {
                        indication = getDataAccessErrorFromCheckHandlerResult(checkResult);

                        ControlObject_sendLastApplError(controlObject, connection, "SBOw", 0,
                                ADD_CAUSE_SELECT_FAILED, ctlNum, origin, true);

                        if (DEBUG_IED_SERVER)
                            printf("SBOw: select rejected by application!\n");
                    }
                }
            }
            else {
                indication = DATA_ACCESS_ERROR_OBJECT_VALUE_INVALID;
            }
        }
        else {
            indication = DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED;
            goto free_and_return;
        }
    }
    else if (strcmp(varName, "Oper") == 0) {
        MmsValue* ctlVal = getCtlVal(value);
        MmsValue* test = getOperParameterTest(value);
        MmsValue* ctlNum = getOperParameterCtlNum(value);
        MmsValue* origin = getOperParameterOrigin(value);
        MmsValue* check = getOperParameterCheck(value);
        MmsValue* timeParameter = getOperParameterTime(value);

        if ((ctlVal == NULL) || (test == NULL) || (ctlNum == NULL) || (origin == NULL) || (check == NULL)
                || (timeParameter == NULL)) {
            indication = DATA_ACCESS_ERROR_OBJECT_VALUE_INVALID;
            goto free_and_return;
        }

        if (checkValidityOfOriginParameter(origin) == false) {
            indication = DATA_ACCESS_ERROR_OBJECT_VALUE_INVALID;
            goto free_and_return;
        }

        uint64_t currentTime = Hal_getTimeInMs();

        checkSelectTimeout(controlObject, currentTime);

        int state = getState(controlObject);

        if (state == STATE_WAIT_FOR_ACTIVATION_TIME) {
            indication = DATA_ACCESS_ERROR_TEMPORARILY_UNAVAILABLE;

            ControlObject_sendLastApplError(controlObject, connection, "Oper",
                    CONTROL_ERROR_NO_ERROR, ADD_CAUSE_COMMAND_ALREADY_IN_EXECUTION,
                        ctlNum, origin, true);

            goto free_and_return;
        }
        else if (state == STATE_READY) {

            bool interlockCheck = MmsValue_getBitStringBit(check, 1);
            bool synchroCheck = MmsValue_getBitStringBit(check, 0);

            bool testCondition = MmsValue_getBoolean(test);

            controlObject->testMode = testCondition;

            if ((controlObject->ctlModel == 2) || (controlObject->ctlModel == 4)) {
                if (controlObject->mmsConnection != connection) {
                    indication = DATA_ACCESS_ERROR_TEMPORARILY_UNAVAILABLE;
                    if (DEBUG_IED_SERVER)
                        printf("IED_SERVER: Oper - operate from wrong client connection!\n");
                    goto free_and_return;
                }

                if (controlObject->ctlModel == 4) { /* select-before-operate with enhanced security */
                    if ((MmsValue_equals(ctlVal, controlObject->ctlVal) &&
                         MmsValue_equals(origin, controlObject->origin) &&
                         MmsValue_equals(ctlNum, controlObject->ctlNum)) == false)
                    {

                        indication = DATA_ACCESS_ERROR_TYPE_INCONSISTENT;
                        ControlObject_sendLastApplError(controlObject, connection, "Oper",
                                CONTROL_ERROR_NO_ERROR, ADD_CAUSE_INCONSISTENT_PARAMETERS,
                                    ctlNum, origin, true);

                        goto free_and_return;
                    }
                }
            }

            updateControlParameters(controlObject, ctlVal, ctlNum, origin);

            MmsValue* operTm = getOperParameterOperTime(value);

            if (operTm != NULL) {
                controlObject->operateTime = MmsValue_getUtcTimeInMs(operTm);

                if (controlObject->operateTime != 0) {
                    controlObject->timeActivatedOperate = true;
                    controlObject->synchroCheck = synchroCheck;
                    controlObject->interlockCheck = interlockCheck;
                    controlObject->mmsConnection = connection;

                    initiateControlTask(controlObject);

                    setState(controlObject, STATE_WAIT_FOR_ACTIVATION_TIME);

                    if (DEBUG_IED_SERVER)
                        printf("Oper: activate time activated control\n");

                    indication = DATA_ACCESS_ERROR_SUCCESS;
                }
            }

            MmsValue_update(controlObject->oper, value);

            if (controlObject->timeActivatedOperate == false) {

                CheckHandlerResult checkResult = CONTROL_ACCEPTED;

                if (controlObject->checkHandler != NULL) { /* perform operative tests */

                    ClientConnection clientConnection = private_IedServer_getClientConnectionByHandle(self->iedServer,
                            connection);

                    checkResult = controlObject->checkHandler(
                            controlObject->checkHandlerParameter, ctlVal, testCondition, interlockCheck,
                            clientConnection);
                }


                if (checkResult == CONTROL_ACCEPTED) {
                    indication = DATA_ACCESS_ERROR_NO_RESPONSE;

                    controlObject->mmsConnection = connection;

                    controlObject->operateInvokeId = MmsServerConnection_getLastInvokeId(connection);

                    setState(controlObject, STATE_WAIT_FOR_EXECUTION);

                    initiateControlTask(controlObject);

#if (CONFIG_MMS_THREADLESS_STACK == 1)
                    //TODO call this in single threaded version to increase response time!?
                    //executeControlTask(controlObject);
#endif

                }
                else {
                    indication = getDataAccessErrorFromCheckHandlerResult(checkResult);

                    abortControlOperation(controlObject);
                }
            }

        }
        else if (state == STATE_UNSELECTED) {
            if (DEBUG_IED_SERVER)
                printf("IED_SERVER: Oper failed - control not selected!\n");

            indication = DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED;
            ControlObject_sendLastApplError(controlObject, connection, "Oper",
                    CONTROL_ERROR_NO_ERROR, ADD_CAUSE_OBJECT_NOT_SELECTED,
                        ctlNum, origin, true);

            goto free_and_return;
        }
    }
    else if (strcmp(varName, "Cancel") == 0) {
        if (DEBUG_IED_SERVER)
            printf("IED_SERVER: control received cancel!\n");

        int state = getState(controlObject);

        MmsValue* ctlNum = getCancelParameterCtlNum(value);
        MmsValue* origin = getCancelParameterOrigin(value);

        if ((ctlNum == NULL) || (origin == NULL)) {
            indication = DATA_ACCESS_ERROR_TYPE_INCONSISTENT;
            if (DEBUG_IED_SERVER)
                printf("IED_SERVER: Invalid cancel message!\n");
            goto free_and_return;
        }

        if ((controlObject->ctlModel == 2) || (controlObject->ctlModel == 4)) {
            if (state != STATE_UNSELECTED) {
                if (controlObject->mmsConnection == connection) {
                    indication = DATA_ACCESS_ERROR_SUCCESS;
                    unselectObject(controlObject);
                    goto free_and_return;
                }
                else {
                    indication = DATA_ACCESS_ERROR_TEMPORARILY_UNAVAILABLE;
                    ControlObject_sendLastApplError(controlObject, connection, "Cancel",
                            CONTROL_ERROR_NO_ERROR, ADD_CAUSE_LOCKED_BY_OTHER_CLIENT,
                                ctlNum, origin, true);
                }
            }
        }

        if (controlObject->timeActivatedOperate) {
            controlObject->timeActivatedOperate = false;
            abortControlOperation(controlObject);
            indication = DATA_ACCESS_ERROR_SUCCESS;
            goto free_and_return;
        }
    }

free_and_return:

    return indication;
}

#endif /* (CONFIG_IEC61850_CONTROL_SERVICE == 1) */

