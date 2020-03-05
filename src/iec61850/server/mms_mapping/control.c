/*
 *  control.c
 *
 *  Copyright 2013-2019 Michael Zillgith
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
#include "iec61850_server.h"
#include "ied_server_private.h"
#include "mms_value_internal.h"

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

#define PENDING_EVENT_SELECTED 1
#define PENDING_EVENT_UNSELECTED 2
#define PENDING_EVENT_OP_RCVD_TRUE 4
#define PENDING_EVENT_OP_RCVD_FALSE 8
#define PENDING_EVENT_OP_OK_TRUE 16
#define PENDING_EVENT_OP_OK_FALSE 32

static MmsValue emptyString = {MMS_STRUCTURE};

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
setStSeld(ControlObject* self, bool value)
{
    if (self->stSeld) {
#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_wait(self->pendingEventsLock);
#endif

        if (value)
            self->pendingEvents |= PENDING_EVENT_SELECTED;
        else
            self->pendingEvents |= PENDING_EVENT_UNSELECTED;

#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_post(self->pendingEventsLock);
#endif
    }
}

static void
setOpRcvd(ControlObject* self, bool value)
{
    if (self->opRcvd) {
#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_wait(self->pendingEventsLock);
#endif

        if (value)
            self->pendingEvents |= PENDING_EVENT_OP_RCVD_TRUE;
        else
            self->pendingEvents |= PENDING_EVENT_OP_RCVD_FALSE;

#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_post(self->pendingEventsLock);
#endif
    }
}

static void
setOpOk(ControlObject* self, bool value, uint64_t currentTimeInMs)
{
    if (self->opOk) {
#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_wait(self->pendingEventsLock);
#endif

        if (value) {
            if (self->tOpOk) {
                MmsValue* timestamp = self->tOpOk->mmsValue;

                MmsValue_setUtcTimeMs(timestamp, currentTimeInMs);

                /* TODO update time quality */
            }


            self->pendingEvents |= PENDING_EVENT_OP_OK_TRUE;
        }
        else
            self->pendingEvents |= PENDING_EVENT_OP_OK_FALSE;

#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_post(self->pendingEventsLock);
#endif
    }
}

static void
updateSboTimeoutValue(ControlObject* self)
{
    if (self->sboTimeout != NULL) {
        uint32_t sboTimeoutVal = MmsValue_toInt32(self->sboTimeout);

        if (DEBUG_IED_SERVER)
            printf("IED_SERVER: set timeout for %s/%s.%s to %u\n", MmsDomain_getName(self->mmsDomain), self->lnName, self->name, sboTimeoutVal);

        self->selectTimeout = sboTimeoutVal;
    }
    else
        self->selectTimeout = CONFIG_CONTROL_DEFAULT_SBO_TIMEOUT;
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

    self->addCauseValue = ADD_CAUSE_UNKNOWN;

    if (self->operateHandler != NULL)
        return self->operateHandler((ControlAction) self, self->operateHandlerParameter, value, testCondition);

    return CONTROL_RESULT_OK;
}

static void
executeControlTask(ControlObject* self, uint64_t currentTimeInMs)
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

        self->addCauseValue = ADD_CAUSE_BLOCKED_BY_SYNCHROCHECK;

        if (self->waitForExecutionHandler != NULL) {
            dynamicCheckResult = self->waitForExecutionHandler((ControlAction) self, self->waitForExecutionHandlerParameter, self->ctlVal,
                    self->testMode, self->synchroCheck);
        }

        if (dynamicCheckResult == CONTROL_RESULT_FAILED) {
            if (isTimeActivatedControl) {
                ControlObject_sendLastApplError(self, self->mmsConnection, "Oper",
                        CONTROL_ERROR_NO_ERROR, self->addCauseValue,
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

            setOpOk(self, true, currentTimeInMs);

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

            setOpOk(self, false, currentTimeInMs);
        }
    }
    break;

    }
}

ControlObject*
ControlObject_create(IedServer iedServer, MmsDomain* domain, char* lnName, char* name, MmsVariableSpecification* operSpec)
{
    ControlObject* self = (ControlObject*) GLOBAL_CALLOC(1, sizeof(ControlObject));

    if (self == NULL)
        goto exit_function;

    if (DEBUG_IED_SERVER)
        printf("IED_SERVER: create control object for LD: %s, LN: %s, name: %s\n", domain->domainName, lnName, name);

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    self->stateLock = Semaphore_create(1);
    self->pendingEventsLock = Semaphore_create(1);

    if ((self->stateLock == NULL) || (self->pendingEventsLock == NULL)) {
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

    MmsVariableSpecification* ctlValSpec = MmsVariableSpecification_getChildSpecificationByName(operSpec, "ctlVal", NULL);

    if (ctlValSpec) {
        self->ctlVal = MmsValue_newDefaultValue(ctlValSpec);
    }
    else {
        if (DEBUG_IED_SERVER)
            printf("IED_SERVER: control object %s/%s.%s has no ctlVal element!\n", domain->domainName, lnName, name);
    }


    MmsVariableSpecification* originSpec = MmsVariableSpecification_getChildSpecificationByName(operSpec, "origin", NULL);

    if (originSpec) {
        self->origin = MmsValue_newDefaultValue(originSpec);
    }
    else {
        if (DEBUG_IED_SERVER)
            printf("IED_SERVER: control object %s/%s.%s has no origin element!\n", domain->domainName, lnName, name);
    }

    self->ctlNum = MmsValue_newUnsigned(8);

exit_function:
    return self;
}

void
ControlObject_initialize(ControlObject* self)
{
    MmsServer mmsServer = IedServer_getMmsServer(self->iedServer);

    if (emptyString.type != MMS_VISIBLE_STRING) {
        emptyString.type = MMS_VISIBLE_STRING;
        emptyString.value.visibleString.size = 0;
        emptyString.value.visibleString.buf = NULL;
    }

    char strBuf[129];

    char* ctlModelName = StringUtils_createStringInBuffer(strBuf, 4, self->lnName, "$CF$", self->name, "$ctlModel");

    if (DEBUG_IED_SERVER)
        printf("IED_SERVER: initialize control for %s\n", ctlModelName);

    MmsValue* ctlModel = MmsServer_getValueFromCache(mmsServer,
            self->mmsDomain, ctlModelName);

    if (ctlModel == NULL) {
        if (DEBUG_IED_SERVER)
            printf("IED_SERVER: No control model found for variable %s\n", ctlModelName);
    }

    char* sboClassName = StringUtils_createStringInBuffer(strBuf, 4, self->lnName, "$CF$", self->name, "$sboClass");

    self->sboClass = MmsServer_getValueFromCache(mmsServer, self->mmsDomain, sboClassName);

    char* ctlNumName = StringUtils_createStringInBuffer(strBuf, 4, self->lnName, "$ST$", self->name, "$ctlNum");

    self->ctlNumSt = MmsServer_getValueFromCache(mmsServer, self->mmsDomain, ctlNumName);

    char* originName = StringUtils_createStringInBuffer(strBuf, 4, self->lnName, "$ST$", self->name, "$origin");

    self->originSt = MmsServer_getValueFromCache(mmsServer, self->mmsDomain, originName);

    char* sboTimeoutName = StringUtils_createStringInBuffer(strBuf, 4, self->lnName, "$CF$", self->name, "$sboTimeout");

    self->sboTimeout = MmsServer_getValueFromCache(mmsServer,
         self->mmsDomain, sboTimeoutName);

    updateSboTimeoutValue(self);

    if (self->sbo) {
        char* controlObjectReference = StringUtils_createStringInBuffer(strBuf, 6, self->mmsDomain->domainName,
                "/", self->lnName, "$CO$", self->name, "$SBO");

        MmsValue_setVisibleString(self->sbo, controlObjectReference);
    }

    char* stSeldName = StringUtils_createStringInBuffer(strBuf, 6, self->mmsDomain->domainName, "/", self->lnName, ".", self->name, ".stSeld");

    self->stSeld = (DataAttribute*) IedModel_getModelNodeByObjectReference(self->iedServer->model, stSeldName);

    if ((self->stSeld) && (self->stSeld->type != IEC61850_BOOLEAN)) {
        self->stSeld = NULL;

        if (DEBUG_IED_SERVER)
            printf("IED_SERVER:  ERROR - stSeld of wrong type!\n");
    }

    char* opRcvdName = StringUtils_createStringInBuffer(strBuf, 6, self->mmsDomain->domainName, "/", self->lnName, ".", self->name, ".opRcvd");

    self->opRcvd = (DataAttribute*) IedModel_getModelNodeByObjectReference(self->iedServer->model, opRcvdName);

    if ((self->opRcvd) && (self->opRcvd->type != IEC61850_BOOLEAN)) {
        self->opRcvd = NULL;

        if (DEBUG_IED_SERVER)
            printf("IED_SERVER:  ERROR - opRcvd of wrong type!\n");
    }

    char* opOkName = StringUtils_createStringInBuffer(strBuf, 6, self->mmsDomain->domainName, "/", self->lnName, ".", self->name, ".opOk");

    self->opOk = (DataAttribute*) IedModel_getModelNodeByObjectReference(self->iedServer->model, opOkName);

    if ((self->opOk) && (self->opOk->type != IEC61850_BOOLEAN)) {
        self->opOk = NULL;

        if (DEBUG_IED_SERVER)
            printf("IED_SERVER:  ERROR - opOk of wrong type!\n");
    }

    char* tOpOkName = StringUtils_createStringInBuffer(strBuf, 6, self->mmsDomain->domainName, "/", self->lnName, ".", self->name, ".tOpOk");

    self->tOpOk = (DataAttribute*) IedModel_getModelNodeByObjectReference(self->iedServer->model, tOpOkName);

    if ((self->tOpOk) && (self->tOpOk->type != IEC61850_TIMESTAMP)) {
        self->tOpOk = NULL;

        if (DEBUG_IED_SERVER)
            printf("IED_SERVER:  ERROR - tOpOk of wrong type!\n");
    }

    self->error = MmsValue_newIntegerFromInt32(0);
    self->addCause = MmsValue_newIntegerFromInt32(0);

    if (ctlModel != NULL) {
        int ctlModelVal = MmsValue_toInt32(ctlModel);

        if (DEBUG_IED_SERVER)
            printf("IED_SERVER:  ctlModel: %i\n", ctlModelVal);

        if ((ctlModelVal < 0) || (ctlModelVal > 4)) {
            ctlModelVal = 1;
            if (DEBUG_IED_SERVER)
                printf("IED_SERVER:  invalid control model!\n");
        }

        self->ctlModel = ctlModelVal;

        if ((ctlModelVal == 2) || (ctlModelVal == 4)) /* SBO */
            setState(self, STATE_UNSELECTED);
        else
            setState(self, STATE_READY);
    }
}

static void
ControlObject_handlePendingEvents(ControlObject* self)
{
#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_wait(self->pendingEventsLock);
#endif

    if (self->pendingEvents > 0) {

        if (self->pendingEvents & PENDING_EVENT_SELECTED) {
            if (self->stSeld)
                IedServer_updateBooleanAttributeValue(self->iedServer, self->stSeld, true);

            self->pendingEvents &= ~(PENDING_EVENT_SELECTED);
        }

        if (self->pendingEvents & PENDING_EVENT_UNSELECTED) {
            if (self->stSeld)
                IedServer_updateBooleanAttributeValue(self->iedServer, self->stSeld, false);

            self->pendingEvents &= ~(PENDING_EVENT_UNSELECTED);
        }

        if (self->pendingEvents & PENDING_EVENT_OP_RCVD_TRUE) {
            if (self->opRcvd)
                IedServer_updateBooleanAttributeValue(self->iedServer, self->opRcvd, true);

            self->pendingEvents &= ~(PENDING_EVENT_OP_RCVD_TRUE);
        }

        if (self->pendingEvents & PENDING_EVENT_OP_RCVD_FALSE) {
            if (self->opRcvd)
                IedServer_updateBooleanAttributeValue(self->iedServer, self->opRcvd, false);

            self->pendingEvents &= ~(PENDING_EVENT_OP_RCVD_FALSE);
        }

        if (self->pendingEvents & PENDING_EVENT_OP_OK_TRUE) {
            if (self->opOk)
                IedServer_updateBooleanAttributeValue(self->iedServer, self->opOk, true);

            self->pendingEvents &= ~(PENDING_EVENT_OP_OK_TRUE);
        }

        if (self->pendingEvents & PENDING_EVENT_OP_OK_FALSE) {
            if (self->opOk)
                IedServer_updateBooleanAttributeValue(self->iedServer, self->opOk, false);

            self->pendingEvents &= ~(PENDING_EVENT_OP_OK_FALSE);
        }
    }

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_post(self->pendingEventsLock);
#endif
}

void
ControlObject_destroy(ControlObject* self)
{
    if (self->mmsValue)
        MmsValue_delete(self->mmsValue);

    if (self->error)
        MmsValue_delete(self->error);

    if (self->addCause)
        MmsValue_delete(self->addCause);

    if (self->ctlVal)
        MmsValue_delete(self->ctlVal);

    if (self->ctlNum)
        MmsValue_delete(self->ctlNum);

    if (self->origin)
        MmsValue_delete(self->origin);

    if (self->name)
        GLOBAL_FREEMEM(self->name);

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    if (self->stateLock)
        Semaphore_destroy(self->stateLock);

    if (self->pendingEventsLock)
        Semaphore_destroy(self->pendingEventsLock);
#endif

    GLOBAL_FREEMEM(self);
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
        printf("IED_SERVER: control %s/%s.%s selected\n", MmsDomain_getName(self->mmsDomain), self->lnName, self->name);

    updateSboTimeoutValue(self);

    self->selectTime = selectTime;
    self->mmsConnection = connection;
    setStSeld(self, true);
    setState(self, STATE_READY);
}

static void
unselectObject(ControlObject* self)
{
    setState(self, STATE_UNSELECTED);

    setStSeld(self, false);

    if (DEBUG_IED_SERVER)
        printf("IED_SERVER: control %s/%s.%s unselected\n", MmsDomain_getName(self->mmsDomain), self->lnName, self->name);
}

static void
checkSelectTimeout(ControlObject* self, uint64_t currentTime)
{
    if ((self->ctlModel == 2) || (self->ctlModel == 4)) {

        if (getState(self) == STATE_READY) {
            if (self->selectTimeout > 0) {
                if (currentTime > (self->selectTime + self->selectTimeout)) {
                    if (DEBUG_IED_SERVER)
                        printf("IED_SERVER: select-timeout (timeout-val = %i) for control %s/%s.%s\n",
                                self->selectTimeout, MmsDomain_getName(self->mmsDomain), self->lnName, self->name);

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
ControlObject_updateControlModel(ControlObject* self, ControlModel value, DataObject* ctlObject)
{
    self->ctlModel = (uint32_t) value;

    if ((self->ctlModel == 2) || (self->ctlModel == 4)) /* SBO */
        setState(self, STATE_UNSELECTED);
    else
        setState(self, STATE_READY);

    DataAttribute* ctlModel = (DataAttribute*) ModelNode_getChild((ModelNode*) ctlObject, "ctlModel");

    if (ctlModel) {
        IedServer_updateInt32AttributeValue(self->iedServer, ctlModel, (int32_t) value);
    }
}

void
Control_processControlActions(MmsMapping* self, uint64_t currentTimeInMs)
{
    LinkedList element = LinkedList_getNext(self->controlObjects);

    while (element != NULL) {
        ControlObject* controlObject = (ControlObject*) element->data;

        if (controlObject->state == STATE_WAIT_FOR_ACTIVATION_TIME) {

            if (controlObject->operateTime <= currentTimeInMs) {

                /* enter state Perform Test */
                setOpRcvd(controlObject, true);

                if (DEBUG_IED_SERVER)
                    printf("IED_SERVER: time activated operate: perform test\n");

                controlObject->timeActivatedOperate = false;

                CheckHandlerResult checkResult = CONTROL_ACCEPTED;

                if (controlObject->checkHandler != NULL) { /* perform operative tests */

                    controlObject->addCauseValue = ADD_CAUSE_BLOCKED_BY_INTERLOCKING;

                    checkResult = controlObject->checkHandler((ControlAction) self,
                            controlObject->checkHandlerParameter, controlObject->ctlVal, controlObject->testMode,
                            controlObject->interlockCheck);
                }

                if (checkResult == CONTROL_ACCEPTED) {

                    if (DEBUG_IED_SERVER)
                        printf("IED_SERVER: time activated operate: command accepted\n");

                    /* leave state Perform Test */
                    setOpRcvd(controlObject, false);

                    executeControlTask(controlObject, currentTimeInMs);
                }
                else {

                    ControlObject_sendLastApplError(controlObject, controlObject->mmsConnection, "Oper",
                            CONTROL_ERROR_NO_ERROR, controlObject->addCauseValue,
                                controlObject->ctlNum, controlObject->origin, false);

                    /* leave state Perform Test */
                    setOpRcvd(controlObject, false);

                    abortControlOperation(controlObject);
                }
            }

        } /* if (controlObject->state == STATE_WAIT_FOR_ACTICATION_TIME) */
        else if (!((controlObject->state == STATE_UNSELECTED) || (controlObject->state == STATE_READY))) {
            executeControlTask(controlObject, currentTimeInMs);
        }
        else if (controlObject->state == STATE_READY) {
            checkSelectTimeout(controlObject, currentTimeInMs);
        }

        ControlObject_handlePendingEvents(controlObject);

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

    MmsVariableAccessSpecification varSpec;

    varSpec.itemId = itemId;
    varSpec.domainId = MmsDomain_getName(self->mmsDomain);

    struct sLinkedList _varSpecList1;
    _varSpecList1.next = NULL;
    struct sLinkedList _varSpecList;
    _varSpecList.data = NULL;
    _varSpecList.next = &_varSpecList1;

    struct sLinkedList _values1;
    _values1.next = NULL;
    struct sLinkedList _values;
    _values.data = NULL;
    _values.next = &_values1;

    _varSpecList1.data = &varSpec;
    _values1.data = self->oper;

    MmsServerConnection_sendInformationReportListOfVariables(self->mmsConnection, &_varSpecList, &_values, false);
}

void
ControlObject_sendCommandTerminationNegative(ControlObject* self)
{
    /* create LastApplError */

    struct sLinkedList _varSpecList2;
    _varSpecList2.next = NULL;
    struct sLinkedList _varSpecList1;
    _varSpecList1.next = &_varSpecList2;
    struct sLinkedList _varSpecList;
    _varSpecList.data = NULL;
    _varSpecList.next = &_varSpecList1;

    struct sLinkedList _values2;
    _values2.next = NULL;
    struct sLinkedList _values1;
    _values1.next = &_values2;
    struct sLinkedList _values;
    _values.data = NULL;
    _values.next = &_values1;

    MmsValue lastApplErrorMemory;

    MmsValue* lastApplError = &lastApplErrorMemory;
    lastApplError->type = MMS_STRUCTURE;
    lastApplError->value.structure.size = 5;

    MmsValue* componentContainer[5];

    lastApplError->value.structure.components = componentContainer;

    char ctlObj[130];

    StringUtils_createStringInBuffer(ctlObj, 6, MmsDomain_getName(self->mmsDomain), "/",
            self->lnName, "$CO$", self->name, "$Oper");

    MmsValue ctlObjValueMemory;

    MmsValue* ctlObjValue = &ctlObjValueMemory;
    ctlObjValue->type = MMS_VISIBLE_STRING;
    ctlObjValue->value.visibleString.buf = ctlObj;
    ctlObjValue->value.visibleString.size = sizeof(ctlObj);

    MmsValue_setElement(lastApplError, 0, ctlObjValue);

    MmsValue_setInt32(self->error, CONTROL_ERROR_UNKOWN);
    MmsValue_setInt32(self->addCause, self->addCauseValue);

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

    _varSpecList1.data = &lastApplErrorVarSpec;
    _varSpecList2.data = &operVarSpec;

    _values1.data = lastApplError;
    _values2.data = self->oper;

    MmsServerConnection_sendInformationReportListOfVariables(self->mmsConnection, &_varSpecList, &_values, false);
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

    lastApplError->value.structure.components = componentContainer;

    char ctlObj[130];

    StringUtils_createStringInBuffer(ctlObj, 7, MmsDomain_getName(self->mmsDomain), "/",
            self->lnName, "$CO$", self->name, "$", ctlVariable);

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
    MmsValue_update(controlObject->ctlVal, ctlVal);
    MmsValue_update(controlObject->ctlNum, ctlNum);
    MmsValue_update(controlObject->origin, origin);

    if (controlObject->ctlNumSt)
        MmsValue_update(controlObject->ctlNumSt, ctlNum);

    if (controlObject->originSt)
        MmsValue_update(controlObject->originSt, origin);
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

        if (varName != NULL) {
            if (strcmp(varName, "Oper") == 0)
                value = controlObject->oper;
            else if (strcmp(varName, "SBOw") == 0)
                value = controlObject->sbow;
            else if (strcmp(varName, "SBO") == 0) {
                if (controlObject->ctlModel == 2) {

                    uint64_t currentTime = Hal_getTimeInMs();

                    value = &emptyString;

                    checkSelectTimeout(controlObject, currentTime);

                    if (getState(controlObject) == STATE_UNSELECTED) {
                        CheckHandlerResult checkResult = CONTROL_ACCEPTED;

                        /* opRcvd must not be set here! */

                        controlObject->addCauseValue = ADD_CAUSE_UNKNOWN;

                        if (controlObject->checkHandler != NULL) { /* perform operative tests */

                            controlObject->isSelect = 1;

                            checkResult = controlObject->checkHandler((ControlAction) controlObject,
                                    controlObject->checkHandlerParameter, NULL, false, false);

                            controlObject->isSelect = 0;
                        }

                        if (checkResult == CONTROL_ACCEPTED) {
                            selectObject(controlObject, currentTime, connection);
                            value = controlObject->sbo;
                        }
                    }

                }
                else {
                    if (DEBUG_IED_SERVER)
                        printf("IED_SERVER: select not applicable for control model %i\n", controlObject->ctlModel);

                    value = controlObject->sbo;
                }
            }

            else if (strcmp(varName, "Cancel") == 0)
                value = controlObject->cancel;
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

    if (controlObject->ctlModel == CONTROL_MODEL_STATUS_ONLY) {
        indication = DATA_ACCESS_ERROR_OBJECT_ACCESS_UNSUPPORTED;
        goto free_and_return;
    }

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
                        ControlObject_sendLastApplError(controlObject, connection, "SBOw", CONTROL_ERROR_NO_ERROR,
                                ADD_CAUSE_LOCKED_BY_OTHER_CLIENT, ctlNum, origin, true);
                    else
                        ControlObject_sendLastApplError(controlObject, connection, "SBOw", CONTROL_ERROR_NO_ERROR,
                                ADD_CAUSE_OBJECT_ALREADY_SELECTED, ctlNum, origin, true);

                    if (DEBUG_IED_SERVER)
                        printf("IED_SERVER: SBOw - select failed!\n");
                }
                else {

                    CheckHandlerResult checkResult = CONTROL_ACCEPTED;

                    /* opRcvd must not be set here! */

                    bool interlockCheck = MmsValue_getBitStringBit(check, 1);

                    bool testCondition = MmsValue_getBoolean(test);

                    controlObject->addCauseValue = ADD_CAUSE_SELECT_FAILED;

                    updateControlParameters(controlObject, ctlVal, ctlNum, origin);

                    if (controlObject->checkHandler != NULL) { /* perform operative tests */

                        controlObject->isSelect = 1;

                        checkResult = controlObject->checkHandler((ControlAction) controlObject,
                                controlObject->checkHandlerParameter, ctlVal, testCondition, interlockCheck);

                        controlObject->isSelect = 0;
                    }

                    if (checkResult == CONTROL_ACCEPTED) {
                        selectObject(controlObject, currentTime, connection);

                        indication = DATA_ACCESS_ERROR_SUCCESS;

                        if (DEBUG_IED_SERVER)
                            printf("IED_SERVER: SBOw - selected successful\n");
                    }
                    else {
                        indication = (MmsDataAccessError) checkResult;

                        ControlObject_sendLastApplError(controlObject, connection, "SBOw", 0,
                                controlObject->addCauseValue, ctlNum, origin, true);

                        if (DEBUG_IED_SERVER)
                            printf("IED_SERVER: SBOw - select rejected by application!\n");
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

                    CheckHandlerResult checkResult = CONTROL_ACCEPTED;
                    if (controlObject->checkHandler != NULL) { /* perform operative tests */

                        checkResult = controlObject->checkHandler((ControlAction) controlObject,
                            controlObject->checkHandlerParameter, ctlVal, testCondition, interlockCheck);

                    }

                    if(checkResult == CONTROL_ACCEPTED){
                        initiateControlTask(controlObject);

                        setState(controlObject, STATE_WAIT_FOR_ACTIVATION_TIME);

                        if (DEBUG_IED_SERVER)
                            printf("IED_SERVER: Oper - activate time activated control\n");

                        indication = DATA_ACCESS_ERROR_SUCCESS;
                    }
                    else{
                        indication = (MmsDataAccessError) checkResult;
                    }
                }
            }
            else{
                controlObject->operateTime = 0;
            }

            MmsValue_update(controlObject->oper, value);

            if (controlObject->timeActivatedOperate == false) {

                CheckHandlerResult checkResult = CONTROL_ACCEPTED;

                /* enter state Perform Test */
                setOpRcvd(controlObject, true);

                controlObject->addCauseValue = ADD_CAUSE_UNKNOWN;

                if (controlObject->checkHandler != NULL) { /* perform operative tests */

                    checkResult = controlObject->checkHandler((ControlAction) controlObject,
                            controlObject->checkHandlerParameter, ctlVal, testCondition, interlockCheck);
                }

                if (checkResult == CONTROL_ACCEPTED) {
                    indication = DATA_ACCESS_ERROR_NO_RESPONSE;

                    controlObject->mmsConnection = connection;

                    controlObject->operateInvokeId = MmsServerConnection_getLastInvokeId(connection);

                    setState(controlObject, STATE_WAIT_FOR_EXECUTION);

                    /* leave state Perform Test */
                    setOpRcvd(controlObject, false);

                    initiateControlTask(controlObject);
                }
                else {
                    indication = (MmsDataAccessError) checkResult;

                    /* leave state Perform Test */
                    setOpRcvd(controlObject, false);

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
            goto free_and_return;
        }
    }

free_and_return:

    return indication;
}

void
ControlAction_setAddCause(ControlAction self, ControlAddCause addCause)
{
    ControlObject* controlObject = (ControlObject*) self;

    controlObject->addCauseValue = addCause;
}

int
ControlAction_getOrCat(ControlAction self)
{
    ControlObject* controlObject = (ControlObject*) self;

    if (controlObject->origin) {
        MmsValue* orCat = MmsValue_getElement(controlObject->origin, 0);

        if (orCat) {
            return MmsValue_toInt32(orCat);
        }
    }

    return 0;
}

uint8_t*
ControlAction_getOrIdent(ControlAction self, int* orIdentSize)
{
    ControlObject* controlObject = (ControlObject*) self;

    if (controlObject->origin) {
        MmsValue* orIdent = MmsValue_getElement(controlObject->origin, 1);

        if (orIdent) {
            if (MmsValue_getType(orIdent) == MMS_OCTET_STRING) {
                *orIdentSize  = MmsValue_getOctetStringSize(orIdent);
                return MmsValue_getOctetStringBuffer(orIdent);
            }
        }
    }

    return NULL;
}

int
ControlAction_getCtlNum(ControlAction self)
{
    ControlObject* controlObject = (ControlObject*) self;

    if (controlObject->ctlNum) {
        return MmsValue_toInt32(controlObject->ctlNum);
    }

    return -1;
}

bool
ControlAction_isSelect(ControlAction self)
{
    ControlObject* controlObject = (ControlObject*) self;

    if (controlObject->isSelect)
        return true;
    else
        return false;
}

ClientConnection
ControlAction_getClientConnection(ControlAction self)
{
    ControlObject* controlObject = (ControlObject*) self;

    return private_IedServer_getClientConnectionByHandle(controlObject->iedServer, controlObject->mmsConnection);
}

DataObject*
ControlAction_getControlObject(ControlAction self)
{
    ControlObject* controlObject = (ControlObject*) self;

    return controlObject->dataObject;
}

uint64_t
ControlAction_getControlTime(ControlAction self)
{
    ControlObject* controlObject = (ControlObject*) self;

    return controlObject->operateTime;
}

#endif /* (CONFIG_IEC61850_CONTROL_SERVICE == 1) */

