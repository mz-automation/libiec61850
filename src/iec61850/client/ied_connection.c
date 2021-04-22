/*
 *  ied_connection.c
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

#include "stack_config.h"
#include "libiec61850_platform_includes.h"

#include "iec61850_client.h"

#include "mms_client_connection.h"

#include "ied_connection_private.h"
#include "mms_value_internal.h"

#define DEFAULT_CONNECTION_TIMEOUT 10000
#define DATA_SET_MAX_NAME_LENGTH 64 /* is 32 according to standard! */
#define OUTSTANDING_CALLS 12

typedef struct sICLogicalDevice
{
    char* name;
    LinkedList variables;
} ICLogicalDevice;

struct sClientDataSet
{
    char* dataSetReference; /* data set reference in MMS format */
    MmsValue* dataSetValues; /* MmsValue instance of type MMS_ARRAY */
};

struct sFileDirectoryEntry {
    char* fileName;
    uint32_t fileSize;
    uint64_t lastModified;
};

IedClientError
iedConnection_mapMmsErrorToIedError(MmsError mmsError)
{
    switch (mmsError) {
    case MMS_ERROR_NONE:
        return IED_ERROR_OK;

    case MMS_ERROR_CONNECTION_LOST:
        return IED_ERROR_CONNECTION_LOST;

    case MMS_ERROR_ACCESS_OBJECT_ACCESS_DENIED:
        return IED_ERROR_ACCESS_DENIED;

    case MMS_ERROR_ACCESS_OBJECT_NON_EXISTENT:
        return IED_ERROR_OBJECT_DOES_NOT_EXIST;

    case MMS_ERROR_ACCESS_OBJECT_ACCESS_UNSUPPORTED:
        return IED_ERROR_OBJECT_ACCESS_UNSUPPORTED;

    case MMS_ERROR_DEFINITION_OBJECT_EXISTS:
        return IED_ERROR_OBJECT_EXISTS;

    case MMS_ERROR_DEFINITION_TYPE_INCONSISTENT:
        return IED_ERROR_TYPE_INCONSISTENT;

    case MMS_ERROR_SERVICE_TIMEOUT:
        return IED_ERROR_TIMEOUT;

    case MMS_ERROR_FILE_FILE_NON_EXISTENT:
        return IED_ERROR_OBJECT_DOES_NOT_EXIST;

    case MMS_ERROR_FILE_DUPLICATE_FILENAME:
        return IED_ERROR_OBJECT_EXISTS;

    case MMS_ERROR_FILE_FILE_ACCESS_DENIED:
        return IED_ERROR_ACCESS_DENIED;

    case MMS_ERROR_CONNECTION_REJECTED:
        return IED_ERROR_CONNECTION_REJECTED;

    case MMS_ERROR_ACCESS_OBJECT_VALUE_INVALID:
    	return IED_ERROR_OBJECT_VALUE_INVALID;

    case MMS_ERROR_PARSING_RESPONSE:
        return IED_ERROR_MALFORMED_MESSAGE;

    case MMS_ERROR_OUTSTANDING_CALL_LIMIT:
        return IED_ERROR_OUTSTANDING_CALL_LIMIT_REACHED;

    case MMS_ERROR_DEFINITION_OBJECT_UNDEFINED:
        return IED_ERROR_OBJECT_UNDEFINED;

    default:
        return IED_ERROR_UNKNOWN;
    }
}

IedClientError
iedConnection_mapDataAccessErrorToIedError(MmsDataAccessError mmsError)
{
    switch (mmsError) {

    case DATA_ACCESS_ERROR_NO_RESPONSE:
        return IED_ERROR_TIMEOUT;

    case DATA_ACCESS_ERROR_SUCCESS:
        return IED_ERROR_OK;

    case DATA_ACCESS_ERROR_OBJECT_INVALIDATED:
        return IED_ERROR_OBJECT_INVALIDATED;

    case DATA_ACCESS_ERROR_HARDWARE_FAULT:
        return IED_ERROR_HARDWARE_FAULT;

    case DATA_ACCESS_ERROR_TEMPORARILY_UNAVAILABLE:
        return IED_ERROR_TEMPORARILY_UNAVAILABLE;

    case DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED:
        return IED_ERROR_ACCESS_DENIED;

    case DATA_ACCESS_ERROR_OBJECT_UNDEFINED:
        return IED_ERROR_OBJECT_UNDEFINED;

    case DATA_ACCESS_ERROR_INVALID_ADDRESS:
        return IED_ERROR_INVALID_ADDRESS;

    case DATA_ACCESS_ERROR_TYPE_UNSUPPORTED:
        return IED_ERROR_TYPE_UNSUPPORTED;

    case DATA_ACCESS_ERROR_TYPE_INCONSISTENT:
        return IED_ERROR_TYPE_INCONSISTENT;

    case DATA_ACCESS_ERROR_OBJECT_ATTRIBUTE_INCONSISTENT:
        return IED_ERROR_OBJECT_ATTRIBUTE_INCONSISTENT;

    case DATA_ACCESS_ERROR_OBJECT_ACCESS_UNSUPPORTED:
        return IED_ERROR_OBJECT_ACCESS_UNSUPPORTED;

    case DATA_ACCESS_ERROR_OBJECT_NONE_EXISTENT:
        return IED_ERROR_OBJECT_DOES_NOT_EXIST;

    case DATA_ACCESS_ERROR_OBJECT_VALUE_INVALID:
        return IED_ERROR_OBJECT_VALUE_INVALID;

    default:
        return IED_ERROR_UNKNOWN;
    }
}

IedConnectionOutstandingCall
iedConnection_allocateOutstandingCall(IedConnection self)
{
    Semaphore_wait(self->outstandingCallsLock);

    IedConnectionOutstandingCall call = NULL;

    int i = 0;

    for (i = 0; i < OUTSTANDING_CALLS; i++) {
        if (self->outstandingCalls[i].used == false) {
            self->outstandingCalls[i].used = true;
            call = &(self->outstandingCalls[i]);
            break;
        }
    }

    Semaphore_post(self->outstandingCallsLock);

    return call;
}

void
iedConnection_releaseOutstandingCall(IedConnection self, IedConnectionOutstandingCall call)
{
    Semaphore_wait(self->outstandingCallsLock);

    call->used = false;

    Semaphore_post(self->outstandingCallsLock);
}

IedConnectionOutstandingCall
iedConnection_lookupOutstandingCall(IedConnection self, uint32_t invokeId)
{
    Semaphore_wait(self->outstandingCallsLock);

    IedConnectionOutstandingCall call = NULL;

    int i = 0;

    for (i = 0; i < OUTSTANDING_CALLS; i++) {
        if ((self->outstandingCalls[i].used) && (self->outstandingCalls[i].invokeId == invokeId)) {
            call = &(self->outstandingCalls[i]);
            break;
        }
    }

    Semaphore_post(self->outstandingCallsLock);

    return call;
}

static ICLogicalDevice*
ICLogicalDevice_create(char* name)
{
    ICLogicalDevice* self = (ICLogicalDevice*) GLOBAL_CALLOC(1, sizeof(struct sICLogicalDevice));

    self->name = StringUtils_copyString(name);

    return self;
}

static void
ICLogicalDevice_setVariableList(ICLogicalDevice* self, LinkedList variables)
{
    self->variables = variables;
}

static void
ICLogicalDevice_destroy(ICLogicalDevice* self)
{
    GLOBAL_FREEMEM(self->name);

    if (self->variables != NULL)
        LinkedList_destroy(self->variables);

    GLOBAL_FREEMEM(self);
}

static ClientDataSet
ClientDataSet_create(const char* dataSetReference)
{
    ClientDataSet self = (ClientDataSet) GLOBAL_CALLOC(1, sizeof(struct sClientDataSet));

    self->dataSetReference = StringUtils_copyString(dataSetReference);
    StringUtils_replace(self->dataSetReference, '.', '$');

    self->dataSetValues = NULL;

    return self;
}

void
ClientDataSet_destroy(ClientDataSet self)
{
    if (self->dataSetValues != NULL)
        MmsValue_delete(self->dataSetValues);

    GLOBAL_FREEMEM(self->dataSetReference);

    GLOBAL_FREEMEM(self);
}

static void
ClientDataSet_setDataSetValues(ClientDataSet self, MmsValue* dataSetValues)
{
    self->dataSetValues = dataSetValues;
}

MmsValue*
ClientDataSet_getValues(ClientDataSet self)
{
    return self->dataSetValues;
}

char*
ClientDataSet_getReference(ClientDataSet self)
{
    return self->dataSetReference;
}

int
ClientDataSet_getDataSetSize(ClientDataSet self)
{
    if (self->dataSetValues != NULL) {
        return MmsValue_getArraySize(self->dataSetValues);
    }
    else
        return 0;
}

bool
iedConnection_doesControlObjectMatch(const char* objRef, const char* cntrlObj)
{
    int i = 0;

    while (objRef[i] != '/') {
        if (objRef[i] != cntrlObj[i])
            return false;

        i++;
    }

    if (cntrlObj[i] != '/')
        return false;

    /* --> LD is equal */

    i++;

    while (objRef[i] != '.') {
        if (objRef[i] != cntrlObj[i])
            return false;
        i++;
    }

    int j = i;

    if (cntrlObj[j++] != '$')
        return false;

    /* --> LN is equal */

    if (cntrlObj[j++] != 'C')
        return false;
    if (cntrlObj[j++] != 'O')
        return false;
    if (cntrlObj[j++] != '$')
        return false;

    /* --> FC is ok */

    i++;

    while (objRef[i] != 0) {
        if (cntrlObj[j] == 0)
            return false;

        if (objRef[i] == '.') {
            if (cntrlObj[j] != '$')
                return false;
        }
        else {
            if (objRef[i] != cntrlObj[j])
                return false;
        }

        i++;
        j++;
    }

    if ((cntrlObj[j] == 0) || (cntrlObj[j] == '$'))
        return true;
    else
        return false;
}

static bool
doesReportMatchControlObject(char* domainName, char* itemName, const char* objectRef)
{
    int i = 0;

    while (domainName[i] != 0) {
        if (domainName[i] != objectRef[i])
            return false;

        i++;
    }

    if (objectRef[i] != '/')
        return false;

    /* --> LD is equal */

    i++;
    int j = 0;

    while (objectRef[i] != '.') {
        if (objectRef[i] != itemName[j])
            return false;
        j++;
        i++;
    }

    if (itemName[j++] != '$')
        return false;

    /* --> LN is equal */

    if (itemName[j++] != 'C')
        return false;
    if (itemName[j++] != 'O')
        return false;
    if (itemName[j++] != '$')
        return false;

    /* --> FC is ok */

    i++;

    while (objectRef[i] != 0) {
        if (itemName[j] == 0)
            return false;

        if (objectRef[i] == '.') {
            if (itemName[j] != '$')
                return false;
        }
        else {
            if (objectRef[i] != itemName[j])
                return false;
        }

        i++;
        j++;
    }

    if (itemName[j++] != '$')
        return false;

    /* --> object name is equal */

    if (itemName[j++] != 'O')
        return false;
    if (itemName[j++] != 'p')
        return false;
    if (itemName[j++] != 'e')
        return false;
    if (itemName[j++] != 'r')
        return false;
    if (itemName[j++] != 0)
        return false;

    return true;
}

static void
handleLastApplErrorMessage(IedConnection self, MmsValue* lastApplError)
{
    if (DEBUG_IED_CLIENT)
        printf("IED_CLIENT: received LastApplError\n");

    if ((MmsValue_getType(lastApplError) != MMS_STRUCTURE) || (MmsValue_getArraySize(lastApplError) != 5))
    {
        if (DEBUG_IED_CLIENT)
            printf("IED_CLIENT: LastApplError has wrong type!\n");

        return;
    }

    MmsValue* cntrlObj = MmsValue_getElement(lastApplError, 0);
    MmsValue* error = MmsValue_getElement(lastApplError, 1);
    /* MmsValue* origin = MmsValue_getElement(lastApplError, 2); */
    MmsValue* ctlNum = MmsValue_getElement(lastApplError, 3);
    MmsValue* addCause = MmsValue_getElement(lastApplError, 4);
    if (DEBUG_IED_CLIENT)
        printf("IED_CLIENT:  CntrlObj: %s\n", MmsValue_toString(cntrlObj));

    if (DEBUG_IED_CLIENT)
        printf("IED_CLIENT:  ctlNum: %u\n", MmsValue_toUint32(ctlNum));

    if (DEBUG_IED_CLIENT)
        printf("IED_CLIENT:  addCause: %i\n", MmsValue_toInt32(addCause));

    if (DEBUG_IED_CLIENT)
        printf("IED_CLIENT:  error: %i\n", MmsValue_toInt32(error));

    self->lastApplError.ctlNum = MmsValue_toUint32(ctlNum);
    self->lastApplError.addCause = (ControlAddCause) MmsValue_toInt32(addCause);
    self->lastApplError.error = (ControlLastApplError) MmsValue_toInt32(error);

    Semaphore_wait(self->clientControlsLock);

    LinkedList control = LinkedList_getNext(self->clientControls);

    while (control) {
        ControlObjectClient object = (ControlObjectClient) control->data;

        const char* objectRef = ControlObjectClient_getObjectReference(object);

        if (iedConnection_doesControlObjectMatch(objectRef, MmsValue_toString(cntrlObj))) {
            ControlObjectClient_setLastApplError(object, self->lastApplError);
        }

        control = LinkedList_getNext(control);
    }

    Semaphore_post(self->clientControlsLock);
}

static void
informationReportHandler(void* parameter, char* domainName,
        char* variableListName, MmsValue* value, bool isVariableListName)
{
    IedConnection self = (IedConnection) parameter;

    if (value) {
        if (DEBUG_IED_CLIENT)
            printf("IED_CLIENT: received information report for %s\n", variableListName);

        if (domainName == NULL) {

            if (isVariableListName) {
                iedConnection_handleReport(self, value);
            }
            else {
                if (strcmp(variableListName, "LastApplError") == 0)
                    handleLastApplErrorMessage(self, value);
                else {
                    if (DEBUG_IED_CLIENT)
                        printf("IED_CLIENT: Received unknown variable list report for list: %s\n", variableListName);
                }
            }
        }
        else {
            if (DEBUG_IED_CLIENT)
                printf("IED_CLIENT: RCVD CommandTermination for %s/%s\n", domainName, variableListName);

            Semaphore_wait(self->clientControlsLock);

            LinkedList control = LinkedList_getNext(self->clientControls);

            while (control) {
               ControlObjectClient object = (ControlObjectClient) control->data;

               const char* objectRef = ControlObjectClient_getObjectReference(object);

               if (doesReportMatchControlObject(domainName, variableListName, objectRef))
                   controlObjectClient_invokeCommandTerminationHandler(object);

               control = LinkedList_getNext(control);
            }

            Semaphore_post(self->clientControlsLock);
        }

        MmsValue_delete(value);
    }
    else {
        if (DEBUG_IED_CLIENT)
             printf("IED_CLIENT: report for %s/%s: value invalid\n", domainName, variableListName);
    }
}

static void
IedConnection_setState(IedConnection self, IedConnectionState newState)
{
    Semaphore_wait(self->stateMutex);

    if (self->state != newState) {
        if (self->connectionStateChangedHandler)
            self->connectionStateChangedHandler(self->connectionStateChangedHandlerParameter, self, newState);
    }

    self->state = newState;

    Semaphore_post(self->stateMutex);
}

static void
mmsConnectionStateChangedHandler(MmsConnection connection, void* parameter, MmsConnectionState newState)
{
    (void)connection;

    IedConnection self = (IedConnection) parameter;

    if (newState == MMS_CONNECTION_STATE_CONNECTED) {
        IedConnection_setState(self, IED_STATE_CONNECTED);
    }
    else if (newState == MMS_CONNECTION_STATE_CLOSED) {
        IedConnection_setState(self, IED_STATE_CLOSED);

        if (self->connectionCloseHandler != NULL)
            self->connectionCloseHandler(self->connectionClosedParameter, self);

        if (DEBUG_IED_CLIENT)
            printf("IED_CLIENT: Connection closed!\n");
    }
    else if (newState == MMS_CONNECTION_STATE_CLOSING) {
        IedConnection_setState(self, IED_STATE_CLOSING);
    }
    else if (newState == MMS_CONNECTION_STATE_CONNECTING) {
        IedConnection_setState(self, IED_STATE_CONNECTING);
    }
}

static IedConnection
createNewConnectionObject(TLSConfiguration tlsConfig, bool useThreads)
{
    IedConnection self = (IedConnection) GLOBAL_CALLOC(1, sizeof(struct sIedConnection));

    if (self) {
        self->enabledReports = LinkedList_create();
        self->logicalDevices = NULL;
        self->clientControlsLock = Semaphore_create(1);
        self->clientControls = LinkedList_create();


        if (useThreads)
            self->connection = MmsConnection_createSecure(tlsConfig);
        else
            self->connection = MmsConnection_createNonThreaded(tlsConfig);

        self->state = IED_STATE_CLOSED;

        self->stateMutex = Semaphore_create(1);
        self->reportHandlerMutex = Semaphore_create(1);

        self->outstandingCallsLock = Semaphore_create(1);
        self->outstandingCalls = (IedConnectionOutstandingCall) GLOBAL_CALLOC(OUTSTANDING_CALLS, sizeof(struct sIedConnectionOutstandingCall));

        self->connectionTimeout = DEFAULT_CONNECTION_TIMEOUT;

        MmsConnection_setInformationReportHandler(self->connection, informationReportHandler, self);

        MmsConnection_setConnectionStateChangedHandler(self->connection, mmsConnectionStateChangedHandler, self);
    }

    return self;
}

IedConnection
IedConnection_create()
{
    return createNewConnectionObject(NULL, true);
}

IedConnection
IedConnection_createWithTlsSupport(TLSConfiguration tlsConfig)
{
    return createNewConnectionObject(tlsConfig, true);
}

IedConnection
IedConnection_createEx(TLSConfiguration tlsConfig, bool useThreads)
{
    return createNewConnectionObject(tlsConfig, useThreads);
}

bool
IedConnection_tick(IedConnection self)
{
    return MmsConnection_tick(self->connection);
}

void
IedConnection_setConnectTimeout(IedConnection self, uint32_t timeoutInMs)
{
    self->connectionTimeout = timeoutInMs;
}

void
IedConnection_setRequestTimeout(IedConnection self, uint32_t timeoutInMs)
{
    if (self->connection) {
        MmsConnection_setRequestTimeout(self->connection, timeoutInMs);
    }
}

uint32_t
IedConnection_getRequestTimeout(IedConnection self)
{
    if (self->connection) {
        return MmsConnection_getRequestTimeout(self->connection);
    }
    else
        return 0;
}

IedConnectionState
IedConnection_getState(IedConnection self)
{
    IedConnectionState state;

    Semaphore_wait(self->stateMutex);
    state = self->state;
    Semaphore_post(self->stateMutex);

    return state;
}

void
IedConnection_installConnectionClosedHandler(IedConnection self, IedConnectionClosedHandler handler,
        void* parameter)
{
    self->connectionCloseHandler = handler;
    self->connectionClosedParameter = parameter;
}

void
IedConnection_connect(IedConnection self, IedClientError* error, const char* hostname, int tcpPort)
{
    if (IedConnection_getState(self) != IED_STATE_CONNECTED) {

        MmsError mmsError;

        MmsConnection_setConnectTimeout(self->connection, self->connectionTimeout);

        if (MmsConnection_connect(self->connection, &mmsError, hostname, tcpPort)) {
            *error = IED_ERROR_OK;
            IedConnection_setState(self, IED_STATE_CONNECTED);
        }
        else {
            IedConnection_setState(self, IED_STATE_CLOSED);
            *error = iedConnection_mapMmsErrorToIedError(mmsError);
        }
    }
    else
        *error = IED_ERROR_ALREADY_CONNECTED;
}

void
IedConnection_installStateChangedHandler(IedConnection self, IedConnection_StateChangedHandler handler, void* parameter)
{
    self->connectionStateChangedHandler = handler;
    self->connectionStateChangedHandlerParameter = parameter;
}

void
IedConnection_connectAsync(IedConnection self, IedClientError* error, const char* hostname, int tcpPort)
{
    if (IedConnection_getState(self) != IED_STATE_CONNECTED) {

        MmsError mmsError = MMS_ERROR_NONE;

        MmsConnection_setConnectTimeout(self->connection, self->connectionTimeout);

        MmsConnection_connectAsync(self->connection, &mmsError, hostname, tcpPort);

        *error = iedConnection_mapMmsErrorToIedError(mmsError);
    }
    else
        *error = IED_ERROR_ALREADY_CONNECTED;
}

void
IedConnection_abort(IedConnection self, IedClientError* error)
{
    if (IedConnection_getState(self) == IED_STATE_CONNECTED) {
        IedConnection_setState(self, IED_STATE_CLOSING);

        MmsError mmsError;

        MmsConnection_abort(self->connection, &mmsError);

        *error = iedConnection_mapMmsErrorToIedError(mmsError);
    }
    else
        *error = IED_ERROR_NOT_CONNECTED;
}

void
IedConnection_abortAsync(IedConnection self, IedClientError* error)
{
    if (IedConnection_getState(self) == IED_STATE_CONNECTED) {

        MmsError mmsError;

        MmsConnection_abortAsync(self->connection, &mmsError);

        *error = iedConnection_mapMmsErrorToIedError(mmsError);
    }
    else
        *error = IED_ERROR_NOT_CONNECTED;
}

void
IedConnection_release(IedConnection self, IedClientError* error)
{
    if (IedConnection_getState(self) == IED_STATE_CONNECTED) {
        MmsError mmsError;

        MmsConnection_conclude(self->connection, &mmsError);

        *error = iedConnection_mapMmsErrorToIedError(mmsError);
    }
    else
        *error = IED_ERROR_NOT_CONNECTED;
}

void
IedConnection_releaseAsync(IedConnection self, IedClientError* error)
{
    if (IedConnection_getState(self) == IED_STATE_CONNECTED) {
        MmsError mmsError;

        MmsConnection_concludeAsync(self->connection, &mmsError, NULL, NULL);

        *error = iedConnection_mapMmsErrorToIedError(mmsError);
    }
    else
        *error = IED_ERROR_NOT_CONNECTED;
}

void
IedConnection_close(IedConnection self)
{
    if (IedConnection_getState(self) == IED_STATE_CONNECTED) {
        IedConnection_setState(self, IED_STATE_CLOSING);
        MmsConnection_close(self->connection);
    }
}

void
IedConnection_destroy(IedConnection self)
{
    IedConnection_close(self);

    MmsConnection_destroy(self->connection);

    if (self->logicalDevices != NULL)
        LinkedList_destroyDeep(self->logicalDevices, (LinkedListValueDeleteFunction) ICLogicalDevice_destroy);

    if (self->enabledReports != NULL)
        LinkedList_destroyDeep(self->enabledReports, (LinkedListValueDeleteFunction) ClientReport_destroy);

    GLOBAL_FREEMEM(self->outstandingCalls);

    LinkedList_destroyStatic(self->clientControls);

    Semaphore_destroy(self->clientControlsLock);
    Semaphore_destroy(self->outstandingCallsLock);
    Semaphore_destroy(self->stateMutex);
    Semaphore_destroy(self->reportHandlerMutex);

    GLOBAL_FREEMEM(self);
}

LinkedList
IedConnection_getLogicalDeviceVariables(IedConnection self, IedClientError* error, const char* ldName)
{
    MmsError err = MMS_ERROR_NONE;

    LinkedList result = MmsConnection_getDomainVariableNames(self->connection, &err, ldName);

    *error = iedConnection_mapMmsErrorToIedError(err);

    return result;
}

LinkedList
IedConnection_getLogicalDeviceDataSets(IedConnection self, IedClientError* error, const char* ldName)
{
    MmsError err = MMS_ERROR_NONE;

    LinkedList result = MmsConnection_getDomainVariableListNames(self->connection, &err, ldName);

    *error = iedConnection_mapMmsErrorToIedError(err);

    return result;
}

MmsVariableSpecification*
IedConnection_getVariableSpecification(IedConnection self, IedClientError* error, const char* objectReference,
        FunctionalConstraint fc)
{
    char domainIdBuffer[65];
    char itemIdBuffer[129];

    char* domainId;
    char* itemId;

    MmsError mmsError;
    MmsVariableSpecification* varSpec = NULL;

    domainId = MmsMapping_getMmsDomainFromObjectReference(objectReference, domainIdBuffer);
    itemId = MmsMapping_createMmsVariableNameFromObjectReference(objectReference, fc, itemIdBuffer);

    if ((domainId == NULL) || (itemId == NULL)) {
        *error = IED_ERROR_OBJECT_REFERENCE_INVALID;
        goto cleanup_and_exit;
    }

    varSpec =
            MmsConnection_getVariableAccessAttributes(self->connection, &mmsError, domainId, itemId);

    if (varSpec != NULL)
        *error = IED_ERROR_OK;
    else
        *error = iedConnection_mapMmsErrorToIedError(mmsError);

cleanup_and_exit:

    return varSpec;
}

static void
getAccessAttrHandler(uint32_t invokeId, void* parameter, MmsError err, MmsVariableSpecification* typeSpec)
{
    IedConnection self = (IedConnection) parameter;

    IedConnectionOutstandingCall call = iedConnection_lookupOutstandingCall(self, invokeId);

    if (call) {

        IedConnection_GetVariableSpecificationHandler handler =  (IedConnection_GetVariableSpecificationHandler) call->callback;

        handler(invokeId, call->callbackParameter, iedConnection_mapMmsErrorToIedError(err), typeSpec);

        iedConnection_releaseOutstandingCall(self, call);
    }
    else {
        if (DEBUG_IED_CLIENT)
            printf("IED_CLIENT: internal error - no matching outstanding call!\n");
    }
}

uint32_t
IedConnection_getVariableSpecificationAsync(IedConnection self, IedClientError* error, const char* dataAttributeReference,
        FunctionalConstraint fc, IedConnection_GetVariableSpecificationHandler handler, void* parameter)
{
    uint32_t invokeId = 0;

    char domainIdBuffer[65];
    char itemIdBuffer[129];

    char* domainId;
    char* itemId;

    MmsError err;

    domainId = MmsMapping_getMmsDomainFromObjectReference(dataAttributeReference, domainIdBuffer);
    itemId = MmsMapping_createMmsVariableNameFromObjectReference(dataAttributeReference, fc, itemIdBuffer);

    if ((domainId == NULL) || (itemId == NULL)) {
        *error = IED_ERROR_OBJECT_REFERENCE_INVALID;
        return 0;
    }

    IedConnectionOutstandingCall call = iedConnection_allocateOutstandingCall(self);

    if (call == NULL) {
        *error = IED_ERROR_OUTSTANDING_CALL_LIMIT_REACHED;
        return 0;
    }

    call->callback = handler;
    call->callbackParameter = parameter;

    call->invokeId = MmsConnection_getVariableAccessAttributesAsync(self->connection, &err, domainId, itemId, getAccessAttrHandler, self);

    invokeId = call->invokeId;

    *error = iedConnection_mapMmsErrorToIedError(err);

    if (err != MMS_ERROR_NONE)
        iedConnection_releaseOutstandingCall(self, call);

    return invokeId;
}

static void
getNameListHandler(uint32_t invokeId, void* parameter, MmsError mmsError, LinkedList nameList, bool moreFollows)
{
    IedConnection self = (IedConnection) parameter;

    IedConnectionOutstandingCall call = iedConnection_lookupOutstandingCall(self, invokeId);

    if (call) {

        IedConnection_GetNameListHandler handler =  (IedConnection_GetNameListHandler) call->callback;

        handler(invokeId, call->callbackParameter, iedConnection_mapMmsErrorToIedError(mmsError), nameList, moreFollows);

        iedConnection_releaseOutstandingCall(self, call);
    }
    else {
        if (DEBUG_IED_CLIENT)
            printf("IED_CLIENT: internal error - no matching outstanding call!\n");
    }
}

uint32_t
IedConnection_getServerDirectoryAsync(IedConnection self, IedClientError* error, const char* continueAfter, LinkedList result,
        IedConnection_GetNameListHandler handler, void* parameter)
{
    IedConnectionOutstandingCall call = iedConnection_allocateOutstandingCall(self);

    if (call == NULL) {
        *error = IED_ERROR_OUTSTANDING_CALL_LIMIT_REACHED;
        return 0;
    }

    call->callback = handler;
    call->callbackParameter = parameter;

    MmsError err = MMS_ERROR_NONE;

    call->invokeId = MmsConnection_getDomainNamesAsync(self->connection, &err, continueAfter, result, getNameListHandler, self);

    if (err != MMS_ERROR_NONE) {
        *error = iedConnection_mapMmsErrorToIedError(err);

        iedConnection_releaseOutstandingCall(self, call);

        return 0;
    }
    else {
        *error = IED_ERROR_OK;
    }

    return call->invokeId;
}




uint32_t
IedConnection_getLogicalDeviceVariablesAsync(IedConnection self, IedClientError* error, const char* ldName, const char* continueAfter, LinkedList result,
        IedConnection_GetNameListHandler handler, void* parameter)
{
    IedConnectionOutstandingCall call = iedConnection_allocateOutstandingCall(self);

    if (call == NULL) {
        *error = IED_ERROR_OUTSTANDING_CALL_LIMIT_REACHED;
        return 0;
    }

    call->callback = handler;
    call->callbackParameter = parameter;

    MmsError err = MMS_ERROR_NONE;

    call->invokeId = MmsConnection_getDomainVariableNamesAsync(self->connection, &err, ldName, continueAfter, result, getNameListHandler, self);

    if (err != MMS_ERROR_NONE) {
        *error = iedConnection_mapMmsErrorToIedError(err);

        iedConnection_releaseOutstandingCall(self, call);

        return 0;
    }
    else {
        *error = IED_ERROR_OK;
    }

    return call->invokeId;
}

uint32_t
IedConnection_getLogicalDeviceDataSetsAsync(IedConnection self, IedClientError* error, const char* ldName, const char* continueAfter, LinkedList result,
        IedConnection_GetNameListHandler handler, void* parameter)
{
    IedConnectionOutstandingCall call = iedConnection_allocateOutstandingCall(self);

    if (call == NULL) {
        *error = IED_ERROR_OUTSTANDING_CALL_LIMIT_REACHED;
        return 0;
    }

    call->callback = handler;
    call->callbackParameter = parameter;

    MmsError err = MMS_ERROR_NONE;

    call->invokeId = MmsConnection_getDomainVariableListNamesAsync(self->connection, &err, ldName, continueAfter, result, getNameListHandler, self);

    if (err != MMS_ERROR_NONE) {
        *error = iedConnection_mapMmsErrorToIedError(err);

        iedConnection_releaseOutstandingCall(self, call);

        return 0;
    }
    else {
        *error = IED_ERROR_OK;
    }

    return call->invokeId;
}

static void
readObjectHandlerInternal(uint32_t invokeId, void* parameter, MmsError err, MmsValue* value)
{
    IedConnection self = (IedConnection) parameter;

    IedConnectionOutstandingCall call = iedConnection_lookupOutstandingCall(self, invokeId);

    if (call) {

        IedConnection_ReadObjectHandler handler =  (IedConnection_ReadObjectHandler) call->callback;

        handler(invokeId, call->callbackParameter, iedConnection_mapMmsErrorToIedError(err), value);

        iedConnection_releaseOutstandingCall(self, call);
    }
    else {
        if (DEBUG_IED_CLIENT)
            printf("IED_CLIENT: internal error - no matching outstanding call!\n");
    }
}

uint32_t
IedConnection_readObjectAsync(IedConnection self, IedClientError* error, const char* objRef, FunctionalConstraint fc,
        IedConnection_ReadObjectHandler handler, void* parameter)
{
    *error = IED_ERROR_OK;

    char domainIdBuffer[65];
    char itemIdBuffer[65];

    char* domainId;
    char* itemId;

    domainId = MmsMapping_getMmsDomainFromObjectReference(objRef, domainIdBuffer);
    itemId = MmsMapping_createMmsVariableNameFromObjectReference(objRef, fc, itemIdBuffer);

    if ((domainId == NULL) || (itemId == NULL)) {
        *error = IED_ERROR_OBJECT_REFERENCE_INVALID;
        return 0;
    }

    IedConnectionOutstandingCall call = iedConnection_allocateOutstandingCall(self);

    if (call == NULL) {
        *error = IED_ERROR_OUTSTANDING_CALL_LIMIT_REACHED;
        return 0;
    }

    call->callback = handler;
    call->callbackParameter = parameter;

    MmsError err = MMS_ERROR_NONE;

    /* check if item ID contains an array "(..)" */
    char* brace = strchr(itemId, '(');

    if (brace) {
        char* secondBrace = strchr(brace, ')');

        if (secondBrace) {
            char* endPtr;

            int index = (int) strtol(brace + 1, &endPtr, 10);

            if (endPtr == secondBrace) {
                char* component = NULL;

                if (strlen(secondBrace + 1) > 1)
                    component = secondBrace + 2; /* skip "." after array element specifier */

                *brace = 0;

                call->invokeId = MmsConnection_readSingleArrayElementWithComponentAsync(self->connection, &err, domainId, itemId, index, component, readObjectHandlerInternal, self);
            }
            else
                *error = IED_ERROR_USER_PROVIDED_INVALID_ARGUMENT;
        }
        else
            *error = IED_ERROR_USER_PROVIDED_INVALID_ARGUMENT;
    }
    else
        call->invokeId = MmsConnection_readVariableAsync(self->connection, &err, domainId, itemId, readObjectHandlerInternal, self);

    if ((err != MMS_ERROR_NONE) || (*error != IED_ERROR_OK)) {

        if (err != MMS_ERROR_NONE) {
            *error = iedConnection_mapMmsErrorToIedError(err);
        }

        iedConnection_releaseOutstandingCall(self, call);

        return 0;
    }

    return call->invokeId;
}


MmsValue*
IedConnection_readObject(IedConnection self, IedClientError* error, const char* objectReference,
        FunctionalConstraint fc)
{
    char domainIdBuffer[65];
    char itemIdBuffer[65];
    MmsValue* value = NULL;

    char* domainId;
    char* itemId;

    domainId = MmsMapping_getMmsDomainFromObjectReference(objectReference, domainIdBuffer);
    itemId = MmsMapping_createMmsVariableNameFromObjectReference(objectReference, fc, itemIdBuffer);

    if ((domainId == NULL) || (itemId == NULL)) {
        *error = IED_ERROR_OBJECT_REFERENCE_INVALID;
        return NULL;
    }

    MmsError mmsError = MMS_ERROR_NONE;

    /* check if item ID contains an array "(..)" */
    char* brace = strchr(itemId, '(');

    if (brace) {
        char* secondBrace = strchr(brace, ')');

        if (secondBrace) {
            char* endPtr;

            int index = (int) strtol(brace + 1, &endPtr, 10);

            if (endPtr == secondBrace) {
                char* component = NULL;

                if (strlen(secondBrace + 1) > 1)
                    component = secondBrace + 2; /* skip "." after array element specifier */

                *brace = 0;

                value = MmsConnection_readSingleArrayElementWithComponent(self->connection, &mmsError, domainId, itemId, index, component);
            }
            else
                *error = IED_ERROR_USER_PROVIDED_INVALID_ARGUMENT;
        }
        else
            *error = IED_ERROR_USER_PROVIDED_INVALID_ARGUMENT;
    }
    else
        value = MmsConnection_readVariable(self->connection, &mmsError, domainId, itemId);

    if (value != NULL)
        *error = IED_ERROR_OK;
    else
        *error = iedConnection_mapMmsErrorToIedError(mmsError);

    return value;
}

bool
IedConnection_readBooleanValue(IedConnection self, IedClientError* error, const char* objectReference, FunctionalConstraint fc)
{
    MmsValue* value = IedConnection_readObject(self, error, objectReference, fc);

    bool retVal = false;

    if (value != NULL) {
        if (MmsValue_getType(value) == MMS_BOOLEAN)
            retVal = MmsValue_getBoolean(value);
        else {
            if (MmsValue_getType(value) == MMS_DATA_ACCESS_ERROR)
                *error = iedConnection_mapDataAccessErrorToIedError(MmsValue_getDataAccessError(value));
            else
                *error = IED_ERROR_UNEXPECTED_VALUE_RECEIVED;
        }

        MmsValue_delete(value);
    }

    return retVal;
}

float
IedConnection_readFloatValue(IedConnection self, IedClientError* error, const char* objectReference, FunctionalConstraint fc)
{
    MmsValue* value = IedConnection_readObject(self, error, objectReference, fc);

    float retVal = 0.f;

    if (value != NULL) {
        if (MmsValue_getType(value) == MMS_FLOAT)
            retVal = MmsValue_toFloat(value);
        else {
            if (MmsValue_getType(value) == MMS_DATA_ACCESS_ERROR)
                *error = iedConnection_mapDataAccessErrorToIedError(MmsValue_getDataAccessError(value));
            else
                *error = IED_ERROR_UNEXPECTED_VALUE_RECEIVED;
        }

        MmsValue_delete(value);
    }

    return retVal;
}

char*
IedConnection_readStringValue(IedConnection self, IedClientError* error, const char* objectReference, FunctionalConstraint fc)
{
    MmsValue* value = IedConnection_readObject(self, error, objectReference, fc);

    char* retVal = NULL;

    if (value != NULL) {
        if ((MmsValue_getType(value) == MMS_VISIBLE_STRING) || (MmsValue_getType(value) == MMS_STRING))
            retVal = StringUtils_copyString(MmsValue_toString(value));
        else {
            if (MmsValue_getType(value) == MMS_DATA_ACCESS_ERROR)
                *error = iedConnection_mapDataAccessErrorToIedError(MmsValue_getDataAccessError(value));
            else
                *error = IED_ERROR_UNEXPECTED_VALUE_RECEIVED;
        }

        MmsValue_delete(value);
    }

    return retVal;
}

int32_t
IedConnection_readInt32Value(IedConnection self, IedClientError* error, const char* objectReference, FunctionalConstraint fc)
{
    MmsValue* value = IedConnection_readObject(self, error, objectReference, fc);

    int32_t retVal = 0;

    if (value != NULL) {
        if ((MmsValue_getType(value) == MMS_INTEGER) || (MmsValue_getType(value) == MMS_UNSIGNED))
            retVal = MmsValue_toInt32(value);
        else {
            if (MmsValue_getType(value) == MMS_DATA_ACCESS_ERROR)
                *error = iedConnection_mapDataAccessErrorToIedError(MmsValue_getDataAccessError(value));
            else
                *error = IED_ERROR_UNEXPECTED_VALUE_RECEIVED;
        }

        MmsValue_delete(value);
    }

    return retVal;
}

uint32_t
IedConnection_readUnsigned32Value(IedConnection self, IedClientError* error, const char* objectReference, FunctionalConstraint fc)
{
    MmsValue* value = IedConnection_readObject(self, error, objectReference, fc);

    uint32_t retVal = 0.f;

    if (value != NULL) {
        if ((MmsValue_getType(value) == MMS_INTEGER) || (MmsValue_getType(value) == MMS_UNSIGNED))
            retVal = MmsValue_toUint32(value);
        else {
            if (MmsValue_getType(value) == MMS_DATA_ACCESS_ERROR)
                *error = iedConnection_mapDataAccessErrorToIedError(MmsValue_getDataAccessError(value));
            else
                *error = IED_ERROR_UNEXPECTED_VALUE_RECEIVED;
        }

        MmsValue_delete(value);
    }

    return retVal;
}

int64_t
IedConnection_readInt64Value(IedConnection self, IedClientError* error, const char* objectReference, FunctionalConstraint fc)
{
    MmsValue* value = IedConnection_readObject(self, error, objectReference, fc);

    int64_t retVal = 0;

    if (value != NULL) {
        if ((MmsValue_getType(value) == MMS_INTEGER) || (MmsValue_getType(value) == MMS_UNSIGNED))
            retVal = MmsValue_toInt64(value);
        else {
            if (MmsValue_getType(value) == MMS_DATA_ACCESS_ERROR)
                *error = iedConnection_mapDataAccessErrorToIedError(MmsValue_getDataAccessError(value));
            else
                *error = IED_ERROR_UNEXPECTED_VALUE_RECEIVED;
        }

        MmsValue_delete(value);
    }

    return retVal;
}

Timestamp*
IedConnection_readTimestampValue(IedConnection self, IedClientError* error, const char* objectReference, FunctionalConstraint fc,
        Timestamp* timeStamp)
{
    MmsValue* value = IedConnection_readObject(self, error, objectReference, fc);

    Timestamp* retVal = timeStamp;

    if (value != NULL) {
        if (MmsValue_getType(value) == MMS_UTC_TIME) {

            if (retVal == NULL)
                retVal = (Timestamp*) GLOBAL_MALLOC(sizeof(Timestamp));

            memcpy(retVal->val, value->value.utcTime, 8);
        }
        else {
            if (MmsValue_getType(value) == MMS_DATA_ACCESS_ERROR)
                *error = iedConnection_mapDataAccessErrorToIedError(MmsValue_getDataAccessError(value));
            else
                *error = IED_ERROR_UNEXPECTED_VALUE_RECEIVED;
        }

        MmsValue_delete(value);

    }

    return retVal;
}

Quality
IedConnection_readQualityValue(IedConnection self, IedClientError* error, const char* objectReference, FunctionalConstraint fc)
{
    MmsValue* value = IedConnection_readObject(self, error, objectReference, fc);

    Quality quality = QUALITY_VALIDITY_GOOD;

    if (value != NULL) {

        if ((MmsValue_getType(value) == MMS_BIT_STRING) && (MmsValue_getBitStringSize(value) == 13)) {
            quality = Quality_fromMmsValue(value);
        }
        else {
            if (MmsValue_getType(value) == MMS_DATA_ACCESS_ERROR)
                *error = iedConnection_mapDataAccessErrorToIedError(MmsValue_getDataAccessError(value));
            else
                *error = IED_ERROR_UNEXPECTED_VALUE_RECEIVED;
        }

        MmsValue_delete(value);

    }

    return quality;
}

void
IedConnection_writeObject(IedConnection self, IedClientError* error, const char* objectReference,
        FunctionalConstraint fc, MmsValue* value)
{
    char domainIdBuffer[65];
    char itemIdBuffer[65];

    char* domainId;
    char* itemId;

    domainId = MmsMapping_getMmsDomainFromObjectReference(objectReference, domainIdBuffer);
    itemId = MmsMapping_createMmsVariableNameFromObjectReference(objectReference, fc, itemIdBuffer);

    if ((domainId == NULL) || (itemId == NULL)) {
        *error = IED_ERROR_OBJECT_REFERENCE_INVALID;
        return;
    }

    MmsError mmsError;

    /* check if item ID contains an array "(..)" */
    char* brace = strchr(itemId, '(');

    if (brace) {
        char* secondBrace = strchr(brace, ')');

        if (secondBrace) {
            char* endPtr;

            int index = (int) strtol(brace + 1, &endPtr, 10);

            if (endPtr == secondBrace) {
                char* component = NULL;

                if (strlen(secondBrace + 1) > 1)
                    component = secondBrace + 2; /* skip "." after array element specifier */

                *brace = 0;

                MmsConnection_writeSingleArrayElementWithComponent(self->connection, &mmsError, domainId, itemId, index, component, value);

                *error = iedConnection_mapMmsErrorToIedError(mmsError);
            }
            else
                *error = IED_ERROR_USER_PROVIDED_INVALID_ARGUMENT;
        }
        else
            *error = IED_ERROR_USER_PROVIDED_INVALID_ARGUMENT;
    }
    else {
        MmsConnection_writeVariable(self->connection, &mmsError, domainId, itemId, value);

        *error = iedConnection_mapMmsErrorToIedError(mmsError);
    }
}

static void
writeVariableHandler(uint32_t invokeId, void* parameter, MmsError err, MmsDataAccessError accessError)
{
    IedConnection self = (IedConnection) parameter;

    IedConnectionOutstandingCall call = iedConnection_lookupOutstandingCall(self, invokeId);

    if (call) {

        IedConnection_GenericServiceHandler handler =  (IedConnection_GenericServiceHandler) call->callback;

        IedClientError iedError = iedConnection_mapMmsErrorToIedError(err);

        if (iedError == IED_ERROR_OK)
            iedError = iedConnection_mapDataAccessErrorToIedError(accessError);

        handler(invokeId, call->callbackParameter, iedError);

        iedConnection_releaseOutstandingCall(self, call);
    }
    else {
        if (DEBUG_IED_CLIENT)
            printf("IED_CLIENT: internal error - no matching outstanding call!\n");
    }
}

uint32_t
IedConnection_writeObjectAsync(IedConnection self, IedClientError* error, const char* objectReference,
        FunctionalConstraint fc, MmsValue* value, IedConnection_GenericServiceHandler handler, void* parameter)
{
    *error = IED_ERROR_OK;

    char domainIdBuffer[65];
    char itemIdBuffer[65];

    char* domainId;
    char* itemId;

    domainId = MmsMapping_getMmsDomainFromObjectReference(objectReference, domainIdBuffer);
    itemId = MmsMapping_createMmsVariableNameFromObjectReference(objectReference, fc, itemIdBuffer);

    if ((domainId == NULL) || (itemId == NULL)) {
        *error = IED_ERROR_OBJECT_REFERENCE_INVALID;
        return 0;
    }

    IedConnectionOutstandingCall call = iedConnection_allocateOutstandingCall(self);

    if (call == NULL) {
        *error = IED_ERROR_OUTSTANDING_CALL_LIMIT_REACHED;
        return 0;
    }

    call->callback = handler;
    call->callbackParameter = parameter;
    call->invokeId = 0;

    MmsError err;

    /* check if item ID contains an array "(..)" */
    char* brace = strchr(itemId, '(');

    if (brace) {
        char* secondBrace = strchr(brace, ')');

        if (secondBrace) {
            char* endPtr;

            int index = (int) strtol(brace + 1, &endPtr, 10);

            if (endPtr == secondBrace) {
                char* component = NULL;

                if (strlen(secondBrace + 1) > 1)
                    component = secondBrace + 2; /* skip "." after array element specifier */

                *brace = 0;

                call->invokeId = MmsConnection_writeSingleArrayElementWithComponentAsync(self->connection, &err, domainId, itemId, index, component, value,
                        writeVariableHandler, self);

                *error = iedConnection_mapMmsErrorToIedError(err);
            }
            else
                *error = IED_ERROR_USER_PROVIDED_INVALID_ARGUMENT;
        }
        else
            *error = IED_ERROR_USER_PROVIDED_INVALID_ARGUMENT;
    }
    else {
        call->invokeId = MmsConnection_writeVariableAsync(self->connection, &err, domainId, itemId, value, writeVariableHandler, self);

        *error = iedConnection_mapMmsErrorToIedError(err);
    }

    if (*error != IED_ERROR_OK) {
        iedConnection_releaseOutstandingCall(self, call);
        return 0;
    }

    return call->invokeId;
}

void
IedConnection_writeBooleanValue(IedConnection self, IedClientError* error, const char* objectReference,
        FunctionalConstraint fc, bool value)
{
    MmsValue mmsValue;
    mmsValue.deleteValue = 0;
    mmsValue.type = MMS_BOOLEAN;
    mmsValue.value.boolean = value;

    IedConnection_writeObject(self, error, objectReference, fc, &mmsValue);
}

void
IedConnection_writeInt32Value(IedConnection self, IedClientError* error, const char* objectReference,
        FunctionalConstraint fc, int32_t value)
{
    uint8_t valueBuffer[4];

    Asn1PrimitiveValue pVal;
    pVal.maxSize = 4;
    pVal.size = 4;
    pVal.octets = valueBuffer;

    MmsValue mmsValue;
    mmsValue.type = MMS_INTEGER;
    mmsValue.deleteValue = 0;
    mmsValue.value.integer = &pVal;

    MmsValue_setInt32(&mmsValue, value);

    IedConnection_writeObject(self, error, objectReference, fc, &mmsValue);
}


void
IedConnection_writeUnsigned32Value(IedConnection self, IedClientError* error, const char* objectReference,
        FunctionalConstraint fc, uint32_t value)
{
    uint8_t valueBuffer[4];

    Asn1PrimitiveValue pVal;
    pVal.maxSize = 4;
    pVal.size = 4;
    pVal.octets = valueBuffer;

    MmsValue mmsValue;
    mmsValue.type = MMS_UNSIGNED;
    mmsValue.deleteValue = 0;
    mmsValue.value.integer = &pVal;

    MmsValue_setUint32(&mmsValue, value);

    IedConnection_writeObject(self, error, objectReference, fc, &mmsValue);
}

void
IedConnection_writeFloatValue(IedConnection self, IedClientError* error, const char* objectReference,
        FunctionalConstraint fc, float value)
{
    MmsValue mmsValue;
    mmsValue.type = MMS_FLOAT;
    mmsValue.value.floatingPoint.exponentWidth = 8;
    mmsValue.value.floatingPoint.formatWidth = 32;
    memcpy(mmsValue.value.floatingPoint.buf, (uint8_t*) &value, sizeof(value));

    IedConnection_writeObject(self, error, objectReference, fc, &mmsValue);
}

void
IedConnection_writeOctetString(IedConnection self, IedClientError* error, const char* objectReference,
        FunctionalConstraint fc, uint8_t* value, int valueLength)
{
    MmsValue mmsValue;
    mmsValue.type = MMS_OCTET_STRING;
    mmsValue.value.octetString.buf = value;
    mmsValue.value.octetString.size = valueLength;
    mmsValue.value.octetString.size = valueLength;

    IedConnection_writeObject(self, error, objectReference, fc, &mmsValue);
}

void
IedConnection_writeVisibleStringValue(IedConnection self, IedClientError* error, const char* objectReference,
        FunctionalConstraint fc, char* value)
{
    MmsValue mmsValue;
    mmsValue.deleteValue = 0;
    mmsValue.type = MMS_VISIBLE_STRING;
    mmsValue.value.visibleString.buf = value;

    IedConnection_writeObject(self, error, objectReference, fc, &mmsValue);
}

void
IedConnection_getDeviceModelFromServer(IedConnection self, IedClientError* error)
{
    MmsError mmsError = MMS_ERROR_NONE;

    if (error)
        *error = IED_ERROR_OK;

    LinkedList logicalDeviceNames = MmsConnection_getDomainNames(self->connection, &mmsError);

    if (logicalDeviceNames != NULL) {

        if (self->logicalDevices != NULL) {
            LinkedList_destroyDeep(self->logicalDevices, (LinkedListValueDeleteFunction) ICLogicalDevice_destroy);
            self->logicalDevices = NULL;
        }

        LinkedList logicalDevice = LinkedList_getNext(logicalDeviceNames);

        LinkedList logicalDevices = LinkedList_create();

        while (logicalDevice != NULL) {
            char* name = (char*) logicalDevice->data;

            LinkedList variables = MmsConnection_getDomainVariableNames(self->connection,
                    &mmsError, name);

            if (variables != NULL) {
                ICLogicalDevice* icLogicalDevice = ICLogicalDevice_create(name);

                ICLogicalDevice_setVariableList(icLogicalDevice, variables);

                LinkedList_add(logicalDevices, icLogicalDevice);
            }
            else {
                if (error)
                    *error = iedConnection_mapMmsErrorToIedError(mmsError);
                break;
            }

            logicalDevice = LinkedList_getNext(logicalDevice);
        }

        if (mmsError != MMS_ERROR_NONE) {
            LinkedList_destroyDeep(logicalDevices, (LinkedListValueDeleteFunction) ICLogicalDevice_destroy);
        }
        else {
            self->logicalDevices = logicalDevices;
        }

        LinkedList_destroy(logicalDeviceNames);
    }
    else
        *error = iedConnection_mapMmsErrorToIedError(mmsError);
}

LinkedList /*<char*>*/
IedConnection_getLogicalDeviceList(IedConnection self, IedClientError* error)
{
    *error = IED_ERROR_OK;

    if (self->logicalDevices == NULL) {
        IedConnection_getDeviceModelFromServer(self, error);

        if (*error != IED_ERROR_OK)
            return NULL;
    }

    if (self->logicalDevices != NULL) {
        LinkedList logicalDevice = LinkedList_getNext(self->logicalDevices);

        LinkedList logicalDeviceList = LinkedList_create();

        while (logicalDevice != NULL) {
            ICLogicalDevice* icLogicalDevice = (ICLogicalDevice*) logicalDevice->data;

            char* logicalDeviceName = StringUtils_copyString(icLogicalDevice->name);

            LinkedList_add(logicalDeviceList, logicalDeviceName);

            logicalDevice = LinkedList_getNext(logicalDevice);
        }

        *error = IED_ERROR_OK;
        return logicalDeviceList;
    }
    else {
        *error = IED_ERROR_UNKNOWN;
        return NULL;
    }
}

static void
mmsFileDirectoryHandler(void* parameter, char* filename, uint32_t size, uint64_t lastModified)
{
    LinkedList fileNames = (LinkedList) parameter;

    FileDirectoryEntry newDirectoryEntry = FileDirectoryEntry_create(filename, size, lastModified);

    LinkedList_add(fileNames, (void*) newDirectoryEntry);
}

LinkedList /*<FileDirectoryEntry>*/
IedConnection_getFileDirectory(IedConnection self, IedClientError* error, const char* directoryName)
{
    *error = IED_ERROR_OK;

    MmsError mmsError = MMS_ERROR_NONE;

    LinkedList fileNames = LinkedList_create();

    char* continueAfter = NULL;

    bool moreFollows = false;

    do {
        moreFollows =
                MmsConnection_getFileDirectory(self->connection, &mmsError, directoryName, continueAfter,
                        mmsFileDirectoryHandler, fileNames);

        if (mmsError != MMS_ERROR_NONE) {
            *error = iedConnection_mapMmsErrorToIedError(mmsError);
            LinkedList_destroyDeep(fileNames, (LinkedListValueDeleteFunction) FileDirectoryEntry_destroy);

            return NULL;
        }

        if (moreFollows) {
            FileDirectoryEntry lastDirectoryEntry = (FileDirectoryEntry)
                    LinkedList_getData(LinkedList_getLastElement(fileNames));

            continueAfter = lastDirectoryEntry->fileName;
        }

    } while (moreFollows == true);

    return fileNames;
}

LinkedList /*<FileDirectoryEntry>*/
IedConnection_getFileDirectoryEx(IedConnection self, IedClientError* error, const char* directoryName, const char* continueAfter,
        bool* moreFollows)
{
    *error = IED_ERROR_OK;

    MmsError mmsError = MMS_ERROR_NONE;

    LinkedList fileNames = LinkedList_create();

    bool moreFollowsInternal =  MmsConnection_getFileDirectory(self->connection, &mmsError, directoryName, continueAfter,
                                    mmsFileDirectoryHandler, fileNames);

    if (mmsError != MMS_ERROR_NONE) {
        *error = iedConnection_mapMmsErrorToIedError(mmsError);
        LinkedList_destroyDeep(fileNames, (LinkedListValueDeleteFunction) FileDirectoryEntry_destroy);

        return NULL;
    }

    if (moreFollows != NULL)
        *moreFollows = moreFollowsInternal;

    return fileNames;
}

static void
fileDirectoryHandlerEx(uint32_t invokeId, void* parameter, MmsError err, char* filename, uint32_t size, uint64_t lastModfified,
        bool moreFollows)
{
    IedConnection self = (IedConnection) parameter;

    IedConnectionOutstandingCall call = iedConnection_lookupOutstandingCall(self, invokeId);

    if (call) {

        if (call->specificParameter2.getFileDirectory.cont) {
            IedConnection_FileDirectoryEntryHandler handler = (IedConnection_FileDirectoryEntryHandler) call->callback;

            call->specificParameter2.getFileDirectory.cont =
                    handler(invokeId, call->callbackParameter, iedConnection_mapMmsErrorToIedError(err), filename, size, lastModfified, moreFollows);
        }

        if (filename == NULL)
            iedConnection_releaseOutstandingCall(self, call);
    }
    else {
        if (DEBUG_IED_CLIENT)
            printf("IED_CLIENT: internal error - no matching outstanding call!\n");
    }
}

uint32_t
IedConnection_getFileDirectoryAsyncEx(IedConnection self, IedClientError* error, const char* directoryName, const char* continueAfter,
        IedConnection_FileDirectoryEntryHandler handler, void* parameter)
{
    MmsError err = MMS_ERROR_NONE;

    IedConnectionOutstandingCall call = iedConnection_allocateOutstandingCall(self);

    if (call == NULL) {
       *error = IED_ERROR_OUTSTANDING_CALL_LIMIT_REACHED;
       return 0;
    }

    call->callback = handler;
    call->callbackParameter = parameter;
    call->specificParameter2.getFileDirectory.cont = true;

    call->invokeId = MmsConnection_getFileDirectoryAsync(self->connection, &err, directoryName, continueAfter,
            fileDirectoryHandlerEx, self);

    *error = iedConnection_mapMmsErrorToIedError(err);

    if (err != MMS_ERROR_NONE) {
        iedConnection_releaseOutstandingCall(self, call);
        return 0;
    }

    return call->invokeId;
}

struct sClientProvidedFileReadHandler {
    IedClientGetFileHandler handler;
    void* handlerParameter;
    bool retVal;
    uint32_t byteReceived;
};

static void
mmsFileReadHandler(void* parameter, int32_t frsmId, uint8_t* buffer, uint32_t bytesReceived)
{
    (void)frsmId;

    struct sClientProvidedFileReadHandler* handler = (struct sClientProvidedFileReadHandler*) parameter;

    handler->retVal = handler->handler(handler->handlerParameter, buffer, bytesReceived);

    handler->byteReceived += bytesReceived;
}

uint32_t
IedConnection_getFile(IedConnection self, IedClientError* error, const char* fileName, IedClientGetFileHandler handler,
        void* handlerParameter)
{
    *error = IED_ERROR_OK;

    MmsError mmsError;

    uint32_t fileSize;

    int32_t frsmId =
            MmsConnection_fileOpen(self->connection, &mmsError, fileName, 0, &fileSize, NULL);

    if (mmsError != MMS_ERROR_NONE) {
        *error = iedConnection_mapMmsErrorToIedError(mmsError);
        return 0;
    }

    struct sClientProvidedFileReadHandler clientFileReadHandler;
    clientFileReadHandler.handler = handler;
    clientFileReadHandler.handlerParameter = handlerParameter;
    clientFileReadHandler.retVal = true;
    clientFileReadHandler.byteReceived = 0;

    while (true) {
        bool moreFollows =
                MmsConnection_fileRead(self->connection, &mmsError, frsmId, mmsFileReadHandler,
                        &clientFileReadHandler);

        if (mmsError != MMS_ERROR_NONE) {
            *error = iedConnection_mapMmsErrorToIedError(mmsError);
            return 0;
        }

        if (clientFileReadHandler.retVal == false) {
            *error = IED_ERROR_UNKNOWN;
            break;
        }

        if (moreFollows == false)
            break;
    }

    MmsConnection_fileClose(self->connection, &mmsError, frsmId);

    *error = iedConnection_mapMmsErrorToIedError(mmsError);

    return clientFileReadHandler.byteReceived;
}

static void
mmsConnectionFileCloseHandler (uint32_t invokeId, void* parameter, MmsError mmsError, bool success)
{
    (void)success;

    if (mmsError != MMS_ERROR_NONE) {
        if (DEBUG_IED_CLIENT)
            printf("IED_CLIENT: failed to close file error: %i (mms-error: %i)\n", iedConnection_mapMmsErrorToIedError(mmsError), mmsError);
    }

    IedConnection self = (IedConnection) parameter;

    IedConnectionOutstandingCall call = iedConnection_lookupOutstandingCall(self, invokeId);

    if (call) {
        iedConnection_releaseOutstandingCall(self, call);
    }
    else {
        if (DEBUG_IED_CLIENT)
            printf("IED_CLIENT: internal error - no matching outstanding call!\n");
    }
}

static void
mmsConnectionFileReadHandler (uint32_t invokeId, void* parameter, MmsError mmsError, int32_t frsmId, uint8_t* buffer, uint32_t byteReceived,
        bool moreFollows)
{
    IedConnection self = (IedConnection) parameter;

    IedConnectionOutstandingCall call = iedConnection_lookupOutstandingCall(self, invokeId);

    if (call) {

        IedConnection_GetFileAsyncHandler handler =  (IedConnection_GetFileAsyncHandler) call->callback;

        if (mmsError != MMS_ERROR_NONE) {
            IedClientError err = iedConnection_mapMmsErrorToIedError(mmsError);

            handler(call->specificParameter2.getFileInfo.originalInvokeId, call->callbackParameter, err, invokeId, NULL, 0, false);

            if (mmsError != MMS_ERROR_SERVICE_TIMEOUT) {
                /* close file */
                call->invokeId = MmsConnection_fileCloseAsync(self->connection, &mmsError, frsmId, mmsConnectionFileCloseHandler, self);

                if (mmsError != MMS_ERROR_NONE)
                    iedConnection_releaseOutstandingCall(self, call);
            }
            else {
                if (DEBUG_IED_CLIENT)
                    printf("IED_CLIENT: getFile timeout -> stop download\n");

                iedConnection_releaseOutstandingCall(self, call);
            }
        }
        else {
            bool cont = handler(call->specificParameter2.getFileInfo.originalInvokeId, call->callbackParameter, IED_ERROR_OK, invokeId, buffer, byteReceived, moreFollows);

            if ((moreFollows == false) || (cont == false)) {
                /* close file */
                call->invokeId = MmsConnection_fileCloseAsync(self->connection, &mmsError, frsmId, mmsConnectionFileCloseHandler, self);

                if (mmsError != MMS_ERROR_NONE)
                    iedConnection_releaseOutstandingCall(self, call);
            }
            else {
                /* send next read request */

                call->invokeId = MmsConnection_fileReadAsync(self->connection, &mmsError, frsmId,
                        mmsConnectionFileReadHandler, self);

                if (mmsError != MMS_ERROR_NONE) {
                    IedClientError err = iedConnection_mapMmsErrorToIedError(mmsError);

                    handler(invokeId, call->callbackParameter, err, invokeId, NULL, 0, false);

                    /* close file */
                    call->invokeId = MmsConnection_fileCloseAsync(self->connection, &mmsError, frsmId, mmsConnectionFileCloseHandler, self);

                    if (mmsError != MMS_ERROR_NONE) {
                        iedConnection_releaseOutstandingCall(self, call);
                    }

                }
            }
        }

    }
    else {
        if (DEBUG_IED_CLIENT)
            printf("IED_CLIENT: internal error - no matching outstanding call!\n");
    }
}

static void
mmsConnectionFileOpenHandler (uint32_t invokeId, void* parameter, MmsError mmsError, int32_t frsmId, uint32_t fileSize, uint64_t lastModified)
{
    (void)fileSize;
    (void)lastModified;

    IedConnection self = (IedConnection) parameter;

    IedConnectionOutstandingCall call = iedConnection_lookupOutstandingCall(self, invokeId);

    if (call) {

        IedConnection_GetFileAsyncHandler handler =  (IedConnection_GetFileAsyncHandler) call->callback;

        call->specificParameter2.getFileInfo.originalInvokeId = invokeId;

        if (mmsError != MMS_ERROR_NONE) {
            IedClientError err = iedConnection_mapMmsErrorToIedError(mmsError);

            handler(invokeId, call->callbackParameter, err, invokeId, NULL, 0, false);

            iedConnection_releaseOutstandingCall(self, call);
        }
        else {
            call->specificParameter2.getFileInfo.originalInvokeId = invokeId;
            call->invokeId = MmsConnection_fileReadAsync(self->connection, &mmsError, frsmId, mmsConnectionFileReadHandler, self);

            if (mmsError != MMS_ERROR_NONE) {
                IedClientError err = iedConnection_mapMmsErrorToIedError(mmsError);

                handler(invokeId, call->callbackParameter, err, invokeId, NULL, 0, false);

                /* close file */
                call->invokeId = MmsConnection_fileCloseAsync(self->connection, &mmsError, frsmId, mmsConnectionFileCloseHandler, self);

                if (mmsError != MMS_ERROR_NONE)
                    iedConnection_releaseOutstandingCall(self, call);
            }
        }

    }
    else {
        if (DEBUG_IED_CLIENT)
            printf("IED_CLIENT: internal error - no matching outstanding call!\n");
    }
}


uint32_t
IedConnection_getFileAsync(IedConnection self, IedClientError* error, const char* fileName, IedConnection_GetFileAsyncHandler handler,
        void* parameter)
{
    MmsError err = MMS_ERROR_NONE;

    IedConnectionOutstandingCall call = iedConnection_allocateOutstandingCall(self);

    if (call == NULL) {
       *error = IED_ERROR_OUTSTANDING_CALL_LIMIT_REACHED;
       return 0;
    }

    call->callback = handler;
    call->callbackParameter = parameter;

    call->invokeId = MmsConnection_fileOpenAsync(self->connection, &err, fileName, 0, mmsConnectionFileOpenHandler, self);

    *error = iedConnection_mapMmsErrorToIedError(err);

    if (err != MMS_ERROR_NONE) {
        iedConnection_releaseOutstandingCall(self, call);
        return 0;
    }

    return call->invokeId;
}

void
IedConnection_setFilestoreBasepath(IedConnection self, const char* basepath)
{
    /* simply pass the call to MMS client API */
    MmsConnection_setFilestoreBasepath(self->connection, basepath);
}

void
IedConnection_setFile(IedConnection self, IedClientError* error, const char* sourceFilename, const char* destinationFilename)
{
#if (MMS_OBTAIN_FILE_SERVICE == 1)
    *error = IED_ERROR_OK;

    MmsError mmsError;

    MmsConnection_obtainFile(self->connection, &mmsError, sourceFilename, destinationFilename);

    if (mmsError != MMS_ERROR_NONE) {
        *error = iedConnection_mapMmsErrorToIedError(mmsError);
    }
#else
    *error = IED_ERROR_SERVICE_NOT_IMPLEMENTED;
#endif /* (MMS_OBTAIN_FILE_SERVICE == 1) */
}

static void
deleteFileAndSetFileHandler (uint32_t invokeId, void* parameter, MmsError mmsError, bool success)
{
    (void)success;

    IedConnection self = (IedConnection) parameter;

    IedConnectionOutstandingCall call = iedConnection_lookupOutstandingCall(self, invokeId);

    if (call) {
        IedConnection_GenericServiceHandler handler =  (IedConnection_GenericServiceHandler) call->callback;

        handler(invokeId, call->callbackParameter, iedConnection_mapMmsErrorToIedError(mmsError));

        iedConnection_releaseOutstandingCall(self, call);
    }
    else {
        if (DEBUG_IED_CLIENT)
            printf("IED_CLIENT: internal error - no matching outstanding call!\n");
    }
}

uint32_t
IedConnection_setFileAsync(IedConnection self, IedClientError* error, const char* sourceFilename, const char* destinationFilename,
        IedConnection_GenericServiceHandler handler, void* parameter)
{
    MmsError err = MMS_ERROR_NONE;

    IedConnectionOutstandingCall call = iedConnection_allocateOutstandingCall(self);

    if (call == NULL) {
       *error = IED_ERROR_OUTSTANDING_CALL_LIMIT_REACHED;
       return 0;
    }

    call->callback = handler;
    call->callbackParameter = parameter;

    call->invokeId = MmsConnection_obtainFileAsync(self->connection, &err, sourceFilename, destinationFilename, deleteFileAndSetFileHandler, self);

    *error = iedConnection_mapMmsErrorToIedError(err);

    if (err != MMS_ERROR_NONE) {
        iedConnection_releaseOutstandingCall(self, call);
        return 0;
    }

    return call->invokeId;
}

void
IedConnection_deleteFile(IedConnection self, IedClientError* error, const char* fileName)
{
    *error = IED_ERROR_OK;

    MmsError mmsError;

    MmsConnection_fileDelete(self->connection, &mmsError, fileName);

    *error = iedConnection_mapMmsErrorToIedError(mmsError);
}

uint32_t
IedConnection_deleteFileAsync(IedConnection self, IedClientError* error, const char* fileName,
        IedConnection_GenericServiceHandler handler, void* parameter)
{
    MmsError err = MMS_ERROR_NONE;

    IedConnectionOutstandingCall call = iedConnection_allocateOutstandingCall(self);

    if (call == NULL) {
       *error = IED_ERROR_OUTSTANDING_CALL_LIMIT_REACHED;
       return 0;
    }

    call->callback = handler;
    call->callbackParameter = parameter;

    call->invokeId = MmsConnection_fileDeleteAsync(self->connection, &err, fileName, deleteFileAndSetFileHandler, self);

    *error = iedConnection_mapMmsErrorToIedError(err);

    if (err != MMS_ERROR_NONE) {
        iedConnection_releaseOutstandingCall(self, call);
        return 0;
    }

    return call->invokeId;
}


LinkedList /*<char*>*/
IedConnection_getServerDirectory(IedConnection self, IedClientError* error, bool getFileNames)
{
    if (getFileNames)
        return IedConnection_getFileDirectory(self, error, NULL);
    else
        return IedConnection_getLogicalDeviceList(self, error);
}

LinkedList /*<char*>*/
IedConnection_getLogicalDeviceDirectory(IedConnection self, IedClientError* error,
        const char* logicalDeviceName)
{
    *error = IED_ERROR_OK;

    if (self->logicalDevices == NULL)
        IedConnection_getDeviceModelFromServer(self, error);

    if (*error != IED_ERROR_OK)
        return NULL;

    LinkedList logicalDevice = LinkedList_getNext(self->logicalDevices);

    while (logicalDevice != NULL) {
        ICLogicalDevice* device = (ICLogicalDevice*) logicalDevice->data;

        if (strcmp(device->name, logicalDeviceName) == 0) {
            LinkedList logicalNodeNames = LinkedList_create();

            LinkedList variable = LinkedList_getNext(device->variables);

            while (variable != NULL) {
                char* variableName = (char*) variable->data;

                if (strchr(variableName, '$') == NULL)
                    LinkedList_add(logicalNodeNames, StringUtils_copyString((char*) variable->data));

                variable = LinkedList_getNext(variable);
            }

            return logicalNodeNames;
        }

        logicalDevice = LinkedList_getNext(logicalDevice);
    }

    *error = IED_ERROR_OBJECT_REFERENCE_INVALID;

    return NULL;
}

static bool
addToStringSet(LinkedList set, char* string)
{
    LinkedList element = set;

    while (LinkedList_getNext(element) != NULL) {
        if (strcmp((char*) LinkedList_getNext(element)->data, string) == 0)
            return false;

        element = LinkedList_getNext(element);
    }

    LinkedList_insertAfter(element, string);
    return true;
}

static void
addVariablesWithFc(char* fc, char* lnName, LinkedList variables, LinkedList lnDirectory)
{
    LinkedList variable = LinkedList_getNext(variables);

    while (variable != NULL) {
        char* variableName = (char*) variable->data;

        char* fcPos = strchr(variableName, '$');

        if (fcPos != NULL) {
            if (memcmp(fcPos + 1, fc, 2) != 0)
                goto next_element;

            int lnNameLen = fcPos - variableName;

            if (strncmp(variableName, lnName, lnNameLen) == 0) {
                char* fcEndPos = strchr(fcPos + 1, '$');

                if (fcEndPos != NULL) {
                    char* nameEndPos = strchr(fcEndPos + 1, '$');

                    if (nameEndPos == NULL)
                        addToStringSet(lnDirectory, StringUtils_copyString(fcEndPos + 1));
                }
            }
        }

        next_element:

        variable = LinkedList_getNext(variable);
    }
}

static LinkedList
getLogicalNodeDirectoryLogs(IedConnection self, IedClientError* error, const char* logicalDeviceName,
        const char* logicalNodeName)
{
    MmsConnection mmsCon = self->connection;

    MmsError mmsError;

    LinkedList journals = MmsConnection_getDomainJournals(mmsCon, &mmsError, logicalDeviceName);

    if (mmsError != MMS_ERROR_NONE) {
        *error = iedConnection_mapMmsErrorToIedError(mmsError);
        return NULL;
    }

    LinkedList logs = LinkedList_create();

    LinkedList journal = LinkedList_getNext(journals);

    while (journal != NULL) {

        char* journalName = (char*) LinkedList_getData(journal);

        char* logName = strchr(journalName, '$');

        if (logName != NULL) {
            logName[0] = 0;
            logName += 1;

            if (strcmp(journalName, logicalNodeName) == 0) {
                char* log = StringUtils_copyString(logName);
                LinkedList_add(logs, (void*) log);
            }
        }

        journal = LinkedList_getNext(journal);
    }

    LinkedList_destroy(journals);

    return logs;
}

static LinkedList
getLogicalNodeDirectoryDataSets(IedConnection self, IedClientError* error, const char* logicalDeviceName,
        const char* logicalNodeName)
{
    MmsConnection mmsCon = self->connection;

    MmsError mmsError;

    LinkedList dataSets = MmsConnection_getDomainVariableListNames(mmsCon, &mmsError, logicalDeviceName);

    if (mmsError != MMS_ERROR_NONE) {
        *error = iedConnection_mapMmsErrorToIedError(mmsError);
        return NULL;
    }

    LinkedList lnDataSets = LinkedList_create();

    LinkedList dataSet = LinkedList_getNext(dataSets);

    while (dataSet != NULL) {
        char* dataSetName = (char*) LinkedList_getData(dataSet);

        char* lnDataSetName = strchr(dataSetName, '$');

        if (lnDataSetName != NULL) {
            lnDataSetName[0] = 0;
            lnDataSetName += 1;

            if (strcmp(dataSetName, logicalNodeName) == 0) {
                char* lnDataSet = StringUtils_copyString(lnDataSetName);
                LinkedList_add(lnDataSets, (void*) lnDataSet);
            }
        }

        dataSet = LinkedList_getNext(dataSet);
    }

    LinkedList_destroy(dataSets);

    return lnDataSets;
}

LinkedList /*<char*>*/
IedConnection_getLogicalNodeDirectory(IedConnection self, IedClientError* error,
        const char* logicalNodeReference, ACSIClass acsiClass)
{
    *error = IED_ERROR_OK;

    if (strlen(logicalNodeReference) > 129) {
        *error = IED_ERROR_OBJECT_REFERENCE_INVALID;
        return NULL;
    }

    char lnRefCopy[130];

    strncpy(lnRefCopy, logicalNodeReference, 129);
	lnRefCopy[129] = 0;

    char* ldSep = strchr(lnRefCopy, '/');

    if (ldSep == NULL) {
        *error = IED_ERROR_USER_PROVIDED_INVALID_ARGUMENT;
        return NULL;
    }

    *ldSep = 0;

    char* logicalDeviceName = lnRefCopy;

    char* logicalNodeName = ldSep + 1;

    if (acsiClass == ACSI_CLASS_LOG)
        return getLogicalNodeDirectoryLogs(self, error, logicalDeviceName, logicalNodeName);

    if (acsiClass == ACSI_CLASS_DATA_SET)
        return getLogicalNodeDirectoryDataSets(self, error, logicalDeviceName, logicalNodeName);

    if (self->logicalDevices == NULL)
        IedConnection_getDeviceModelFromServer(self, error);

    if (*error != IED_ERROR_OK)
        return NULL;

    /* search for logical device */

    LinkedList device = LinkedList_getNext(self->logicalDevices);

    ICLogicalDevice* ld = NULL;

    while (device != NULL) {
        ICLogicalDevice* ldCandidate = (ICLogicalDevice*) device->data;

        if (strcmp(logicalDeviceName, ldCandidate->name) == 0) {
            ld = ldCandidate;
            break;
        }

        device = LinkedList_getNext(device);
    }

    if (ld == NULL) {
        *error = IED_ERROR_OBJECT_REFERENCE_INVALID;
        return NULL;
    }

    LinkedList lnDirectory = LinkedList_create();

    switch (acsiClass) {

    case ACSI_CLASS_DATA_OBJECT:
        {
            LinkedList variable = LinkedList_getNext(ld->variables);

            while (variable != NULL) {
                char* variableName = (char*) variable->data;

                char* fcPos = strchr(variableName, '$');

                if (fcPos != NULL) {
                    if (memcmp(fcPos + 1, "RP", 2) == 0)
                        goto next_element;

                    if (memcmp(fcPos + 1, "BR", 2) == 0)
                        goto next_element;

                    if (memcmp(fcPos + 1, "GO", 2) == 0)
                        goto next_element;

                    int lnNameLen = fcPos - variableName;

                    if (strncmp(variableName, logicalNodeName, lnNameLen) == 0) {
                        char* fcEndPos = strchr(fcPos + 1, '$');

                        if (fcEndPos != NULL) {
                            char* nameEndPos = strchr(fcEndPos + 1, '$');

                            if (nameEndPos == NULL) {
                                char* dataObjectName = StringUtils_copyString(fcEndPos + 1);

                                if (!addToStringSet(lnDirectory, dataObjectName))
                                    GLOBAL_FREEMEM(dataObjectName);
                            }
                        }
                    }
                }

                next_element:

                variable = LinkedList_getNext(variable);
            }
        }
        break;

    case ACSI_CLASS_SGCB:
        {
            LinkedList variable = LinkedList_getNext(ld->variables);

            while (variable != NULL) {
                char* variableName = (char*) variable->data;

                if (strcmp(variableName, "LLN0$SP$SGCB") == 0)
                    LinkedList_add(lnDirectory, (void*) StringUtils_copyString("SGCB"));

                variable = LinkedList_getNext(variable);
            }
        }
        break;

    case ACSI_CLASS_BRCB:
        addVariablesWithFc("BR", logicalNodeName, ld->variables, lnDirectory);
        break;

    case ACSI_CLASS_URCB:
        addVariablesWithFc("RP", logicalNodeName, ld->variables, lnDirectory);
        break;

    case ACSI_CLASS_GoCB:
        addVariablesWithFc("GO", logicalNodeName, ld->variables, lnDirectory);
        break;

    case ACSI_CLASS_LCB:
        addVariablesWithFc("LG", logicalNodeName, ld->variables, lnDirectory);
        break;

    default:
        if (DEBUG_IED_CLIENT)
            printf("IED_CLIENT: ACSI class not yet supported!\n");
        break;
    }

    *error = IED_ERROR_OK;
    return lnDirectory;
}

LinkedList /*<char*>*/
IedConnection_getLogicalNodeVariables(IedConnection self, IedClientError* error,
        const char* logicalNodeReference)
{
    *error = IED_ERROR_OK;

    if (strlen(logicalNodeReference) > 129) {
        *error = IED_ERROR_OBJECT_REFERENCE_INVALID;
        return NULL;
    }

    if (self->logicalDevices == NULL)
        IedConnection_getDeviceModelFromServer(self, error);

    if (*error != IED_ERROR_OK)
        return NULL;

    char lnRefCopy[130];

    strncpy(lnRefCopy, logicalNodeReference, 129);
	lnRefCopy[129] = 0;

    char* ldSep = strchr(lnRefCopy, '/');

    if (ldSep == NULL) {
        *error = IED_ERROR_OBJECT_REFERENCE_INVALID;
        return NULL;
    }

    *ldSep = 0;

    char* logicalDeviceName = lnRefCopy;

    char* logicalNodeName = ldSep + 1;

    /* search for logical device */

    LinkedList device = LinkedList_getNext(self->logicalDevices);

    ICLogicalDevice* ld = NULL;

    while (device != NULL) {
        ICLogicalDevice* ldCandidate = (ICLogicalDevice*) device->data;

        if (strcmp(logicalDeviceName, ldCandidate->name) == 0) {
            ld = ldCandidate;
            break;
        }

        device = LinkedList_getNext(device);
    }

    if (ld == NULL) {
        *error = IED_ERROR_OBJECT_REFERENCE_INVALID;
        return NULL;
    }

    if (DEBUG_IED_CLIENT)
        printf("DEBUG_IED_CLIENT: Found LD %s search for variables of LN %s ...\n", logicalDeviceName, logicalNodeName);

    LinkedList variable = LinkedList_getNext(ld->variables);

    LinkedList lnDirectory = LinkedList_create();

    while (variable != NULL) {
        char* variableName = (char*) variable->data;

        char* fcPos = strchr(variableName, '$');

        if (fcPos != NULL) {
            int lnNameLen = fcPos - variableName;

            if (strncmp(variableName, logicalNodeName, lnNameLen) == 0) {
                LinkedList_add(lnDirectory, StringUtils_copyString(fcPos + 1));
            }
        }

        variable = LinkedList_getNext(variable);
    }

    *error = IED_ERROR_OK;
    return lnDirectory;
}

static LinkedList
getDataDirectory(IedConnection self, IedClientError* error,
        const char* dataReference, bool withFc)
{
    *error = IED_ERROR_OK;

    if (strlen(dataReference) > 129) {
        *error = IED_ERROR_OBJECT_REFERENCE_INVALID;
        return NULL;
    }

    if (self->logicalDevices == NULL)
        IedConnection_getDeviceModelFromServer(self, error);

    if (*error != IED_ERROR_OK)
        return NULL;

    char dataRefCopy[130];

    strncpy(dataRefCopy, dataReference, 129);
	dataRefCopy[129] = 0;

    char* ldSep = strchr(dataRefCopy, '/');

    *ldSep = 0;

    char* logicalDeviceName = dataRefCopy;

    char* logicalNodeName = ldSep + 1;

    char* logicalNodeNameEnd = strchr(logicalNodeName, '.');

    if (logicalNodeNameEnd == NULL) {
        *error = IED_ERROR_OBJECT_REFERENCE_INVALID;
        return NULL;
    }

    int logicalNodeNameLen = logicalNodeNameEnd - logicalNodeName;

    char* dataNamePart = logicalNodeNameEnd + 1;

    int dataNamePartLen = strlen(dataNamePart);

    if (dataNamePartLen < 1) {
        *error = IED_ERROR_OBJECT_REFERENCE_INVALID;
        return NULL;
    }

    StringUtils_replace(dataNamePart, '.', '$');

    /* search for logical device */

    LinkedList device = LinkedList_getNext(self->logicalDevices);

    ICLogicalDevice* ld = NULL;

    while (device != NULL) {
        ICLogicalDevice* ldCandidate = (ICLogicalDevice*) device->data;

        if (strcmp(logicalDeviceName, ldCandidate->name) == 0) {
            ld = ldCandidate;
            break;
        }

        device = LinkedList_getNext(device);
    }

    if (ld == NULL) {
        *error = IED_ERROR_OBJECT_REFERENCE_INVALID;
        return NULL;
    }

    LinkedList variable = LinkedList_getNext(ld->variables);

    LinkedList dataDirectory = LinkedList_create();

    while (variable != NULL) {
        char* variableName = (char*) variable->data;

        char* fcPos = strchr(variableName, '$');

        if (fcPos != NULL) {
            int lnNameLen = fcPos - variableName;

            if (logicalNodeNameLen == lnNameLen) {

                if (memcmp(variableName, logicalNodeName, lnNameLen) == 0) {

                    /* ok we are in the correct logical node */

                    /* skip FC */
                    char* fcEnd = strchr(fcPos + 1, '$');

                    if (fcEnd == NULL)
                        goto next_variable;

                    char* remainingPart = fcEnd + 1;

                    int remainingLen = strlen(remainingPart);

                    if (remainingLen <= dataNamePartLen)
                        goto next_variable;

                    if (remainingPart[dataNamePartLen] == '$') {

                        if (memcmp(dataNamePart, remainingPart, dataNamePartLen) == 0) {

                            char* subElementName = remainingPart + dataNamePartLen + 1;

                            char* subElementNameSep = strchr(subElementName, '$');

                            if (subElementNameSep != NULL)
                                goto next_variable;

                            char* elementName;

                            if (withFc) {
                                int elementNameLen = strlen(subElementName);

                                elementName = (char*) GLOBAL_MALLOC(elementNameLen + 5);
                                memcpy(elementName, subElementName, elementNameLen);
                                elementName[elementNameLen] = '[';
                                elementName[elementNameLen + 1] = *(fcPos + 1);
                                elementName[elementNameLen + 2] = *(fcPos + 2);
                                elementName[elementNameLen + 3] = ']';
                                elementName[elementNameLen + 4] = 0;
                            }
                            else
                                elementName = StringUtils_copyString(subElementName);

                            if (!addToStringSet(dataDirectory, elementName))
                                GLOBAL_FREEMEM(elementName);
                        }
                    }
                }
            }
        }

        next_variable:

        variable = LinkedList_getNext(variable);
    }

    *error = IED_ERROR_OK;
    return dataDirectory;

}

LinkedList
IedConnection_getDataDirectory(IedConnection self, IedClientError* error, const char* dataReference)
{
    return getDataDirectory(self, error, dataReference, false);
}

LinkedList
IedConnection_getDataDirectoryFC(IedConnection self, IedClientError* error, const char* dataReference)
{
    return getDataDirectory(self, error, dataReference, true);
}

static LinkedList
getDataDirectoryByFc(IedConnection self, IedClientError* error,
        const char* dataReference, FunctionalConstraint fc)
{
    *error = IED_ERROR_OK;

    if (strlen(dataReference) > 129) {
        *error = IED_ERROR_OBJECT_REFERENCE_INVALID;
        return NULL;
    }

    char* fcString = FunctionalConstraint_toString(fc);

    if (fcString == NULL) {
        *error = IED_ERROR_OBJECT_REFERENCE_INVALID;
        return NULL;
    }

    if (self->logicalDevices == NULL)
        IedConnection_getDeviceModelFromServer(self, error);

    if (*error != IED_ERROR_OK)
        return NULL;

    char dataRefCopy[130];

    strncpy(dataRefCopy, dataReference, 129);
    dataRefCopy[129] = 0;

    char* ldSep = strchr(dataRefCopy, '/');

    *ldSep = 0;

    char* logicalDeviceName = dataRefCopy;

    char* logicalNodeName = ldSep + 1;

    char* logicalNodeNameEnd = strchr(logicalNodeName, '.');

    if (logicalNodeNameEnd == NULL) {
        *error = IED_ERROR_OBJECT_REFERENCE_INVALID;
        return NULL;
    }

    int logicalNodeNameLen = logicalNodeNameEnd - logicalNodeName;

    char* dataNamePart = logicalNodeNameEnd + 1;

    int dataNamePartLen = strlen(dataNamePart);

    if (dataNamePartLen < 1) {
        *error = IED_ERROR_OBJECT_REFERENCE_INVALID;
        return NULL;
    }

    StringUtils_replace(dataNamePart, '.', '$');

    /* search for logical device */

    LinkedList device = LinkedList_getNext(self->logicalDevices);

    ICLogicalDevice* ld = NULL;

    while (device != NULL) {
        ICLogicalDevice* ldCandidate = (ICLogicalDevice*) device->data;

        if (strcmp(logicalDeviceName, ldCandidate->name) == 0) {
            ld = ldCandidate;
            break;
        }

        device = LinkedList_getNext(device);
    }

    if (ld == NULL) {
        *error = IED_ERROR_OBJECT_REFERENCE_INVALID;
        return NULL;
    }

    LinkedList variable = LinkedList_getNext(ld->variables);

    LinkedList dataDirectory = LinkedList_create();

    while (variable != NULL) {
        char* variableName = (char*) variable->data;

        char* fcPos = strchr(variableName, '$');

        if (fcPos != NULL) {
            int lnNameLen = fcPos - variableName;

            if (logicalNodeNameLen == lnNameLen) {

                if (memcmp(variableName, logicalNodeName, lnNameLen) == 0) {

                    /* ok we are in the correct logical node */

                    /* skip FC */
                    char* fcEnd = strchr(fcPos + 1, '$');

                    if (fcEnd == NULL)
                        goto next_variable;

                    if ((fcPos[1] != fcString[0]) || (fcPos[2] != fcString[1]))
                        goto next_variable;

                    char* remainingPart = fcEnd + 1;

                    int remainingLen = strlen(remainingPart);

                    if (remainingLen <= dataNamePartLen)
                        goto next_variable;

                    if (remainingPart[dataNamePartLen] == '$') {

                        if (memcmp(dataNamePart, remainingPart, dataNamePartLen) == 0) {

                            char* subElementName = remainingPart + dataNamePartLen + 1;

                            char* subElementNameSep = strchr(subElementName, '$');

                            if (subElementNameSep != NULL)
                                goto next_variable;

                            int elementNameLen = strlen(subElementName);

                            char* elementName = (char*) GLOBAL_MALLOC(elementNameLen + 1);
                            memcpy(elementName, subElementName, elementNameLen);
                            elementName[elementNameLen] = 0;

                            if (!addToStringSet(dataDirectory, elementName))
                                GLOBAL_FREEMEM(elementName);
                        }
                    }
                }
            }
        }

        next_variable:

        variable = LinkedList_getNext(variable);
    }

    *error = IED_ERROR_OK;
    return dataDirectory;

}


LinkedList
IedConnection_getDataDirectoryByFC(IedConnection self, IedClientError* error, const char* dataReference, FunctionalConstraint fc)
{
    return getDataDirectoryByFc(self, error, dataReference, fc);
}

void
IedConnection_createDataSet(IedConnection self, IedClientError* error, const char* dataSetReference,
        LinkedList /* <char*> */dataSetElements)
{

    char domainIdBuffer[65];
    char itemIdBuffer[DATA_SET_MAX_NAME_LENGTH + 1];

    const char* domainId;
    const char* itemId;
    bool isAssociationSpecific = false;

    if (dataSetReference[0] != '@') {

        if ((dataSetReference[0] == '/') || (strchr(dataSetReference, '/') == NULL)) {
            domainId = NULL;

            if (dataSetReference[0] == '/')
                itemId = dataSetReference + 1;
            else
                itemId = dataSetReference;
        }
        else {
            domainId = MmsMapping_getMmsDomainFromObjectReference(dataSetReference, domainIdBuffer);

            if (domainId == NULL) {
                *error = IED_ERROR_OBJECT_REFERENCE_INVALID;
                goto exit_function;
            }

            int domainIdLength = strlen(domainId);

            if ((strlen(dataSetReference) - domainIdLength - 1) > 32) {
                *error = IED_ERROR_OBJECT_REFERENCE_INVALID;
                goto exit_function;
            }

            char* itemIdRef = StringUtils_copyStringToBuffer(dataSetReference + domainIdLength + 1, itemIdBuffer);
            StringUtils_replace(itemIdRef, '.', '$');
            itemId = itemIdRef;
        }
    }
    else {
        itemId = dataSetReference + 1;
        isAssociationSpecific = true;
    }

    MmsError mmsError;

    LinkedList dataSetEntries = LinkedList_create();

    LinkedList dataSetElement = LinkedList_getNext(dataSetElements);

    while (dataSetElement != NULL) {

        MmsVariableAccessSpecification* dataSetEntry =
                MmsMapping_ObjectReferenceToVariableAccessSpec((char*) dataSetElement->data);

        if (dataSetEntry == NULL) {
            *error = IED_ERROR_OBJECT_REFERENCE_INVALID;
            goto cleanup_list;
        }

        LinkedList_add(dataSetEntries, (void*) dataSetEntry);

        dataSetElement = LinkedList_getNext(dataSetElement);
    }

    if (isAssociationSpecific)
        MmsConnection_defineNamedVariableListAssociationSpecific(self->connection, &mmsError,
                itemId, dataSetEntries);
    else
        MmsConnection_defineNamedVariableList(self->connection, &mmsError,
                domainId, itemId, dataSetEntries);

    *error = iedConnection_mapMmsErrorToIedError(mmsError);

cleanup_list:
    /* delete list and all elements */
    LinkedList_destroyDeep(dataSetEntries, (LinkedListValueDeleteFunction) MmsVariableAccessSpecification_destroy);

exit_function:
    return;
}

bool
IedConnection_deleteDataSet(IedConnection self, IedClientError* error, const char* dataSetReference)
{
    char domainIdBuf[65];
    char* domainId = domainIdBuf;
    char itemId[DATA_SET_MAX_NAME_LENGTH + 1];
    bool isAssociationSpecific = false;
    bool isDeleted = false;

    int dataSetReferenceLength = strlen(dataSetReference);

    if (dataSetReference[0] != '@') {
        if ((dataSetReference[0] == '/') || (strchr(dataSetReference, '/') == NULL)) {
            domainId = NULL;

            if (dataSetReference[0] == '/')
                strcpy(itemId, dataSetReference + 1);
            else
                strcpy(itemId, dataSetReference);
        }
        else {

            if (MmsMapping_getMmsDomainFromObjectReference(dataSetReference, domainId) == NULL) {
                *error = IED_ERROR_OBJECT_REFERENCE_INVALID;
                goto exit_function;
            }

            const char* itemIdString = dataSetReference + strlen(domainId) + 1;

            if (strlen(itemIdString) > DATA_SET_MAX_NAME_LENGTH) {
                *error = IED_ERROR_OBJECT_REFERENCE_INVALID;
                goto exit_function;
            }

            StringUtils_copyStringToBuffer(itemIdString, itemId);

            StringUtils_replace(itemId, '.', '$');
        }
    }
    else {
        if (dataSetReferenceLength > 33) {
            *error = IED_ERROR_OBJECT_REFERENCE_INVALID;
            goto exit_function;
        }

        strcpy(itemId, dataSetReference + 1);

        isAssociationSpecific = true;
    }

    MmsError mmsError;

    if (isAssociationSpecific)
        isDeleted = MmsConnection_deleteAssociationSpecificNamedVariableList(self->connection, &mmsError, itemId);
    else
        isDeleted = MmsConnection_deleteNamedVariableList(self->connection, &mmsError, domainId, itemId);

    *error = iedConnection_mapMmsErrorToIedError(mmsError);

exit_function:
    return isDeleted;
}

LinkedList /* <char*> */
IedConnection_getDataSetDirectory(IedConnection self, IedClientError* error, const char* dataSetReference, bool* isDeletable)
{
    bool deletable = false;

    LinkedList dataSetMembers = NULL;

    char domainIdBuffer[65];
    char itemIdBuffer[DATA_SET_MAX_NAME_LENGTH + 1];

    const char* domainId = NULL;
    const char* itemId = NULL;

    bool isAssociationSpecific = false;

    if (dataSetReference[0] != '@') {
        if ((dataSetReference[0] == '/') || (strchr(dataSetReference, '/') == NULL)) {
            domainId = NULL;

            if (dataSetReference[0] == '/')
                itemId = dataSetReference + 1;
            else
                itemId = dataSetReference;
        }
        else {
            domainId = MmsMapping_getMmsDomainFromObjectReference(dataSetReference, domainIdBuffer);

            if (domainId == NULL) {
                *error = IED_ERROR_OBJECT_REFERENCE_INVALID;
                goto exit_function;
            }

            const char* itemIdRef = dataSetReference + strlen(domainId) + 1;

            if (strlen(itemIdRef) > DATA_SET_MAX_NAME_LENGTH) {
                *error = IED_ERROR_OBJECT_REFERENCE_INVALID;
                goto exit_function;
            }

            char* itemIdRefInBuffer = StringUtils_copyStringToBuffer(itemIdRef, itemIdBuffer);
            StringUtils_replace(itemIdRefInBuffer, '.', '$');
            itemId = itemIdRefInBuffer;
        }
    }
    else {
        itemId = dataSetReference + 1;
        isAssociationSpecific = true;
    }

    MmsError mmsError;

    LinkedList entries;

    if (isAssociationSpecific)
        entries = MmsConnection_readNamedVariableListDirectoryAssociationSpecific(self->connection,
                &mmsError, itemId, &deletable);
    else
        entries = MmsConnection_readNamedVariableListDirectory(self->connection,
                &mmsError, domainId, itemId, &deletable);

    if (mmsError == MMS_ERROR_NONE) {

        LinkedList entry = LinkedList_getNext(entries);

        dataSetMembers = LinkedList_create();

        while (entry != NULL) {
            MmsVariableAccessSpecification* varAccessSpec = (MmsVariableAccessSpecification*) entry->data;

            char* objectReference = MmsMapping_varAccessSpecToObjectReference(varAccessSpec);

            LinkedList_add(dataSetMembers, objectReference);

            entry = LinkedList_getNext(entry);
        }

        if (isDeletable != NULL)
            *isDeletable = deletable;

        LinkedList_destroyDeep(entries, (LinkedListValueDeleteFunction) MmsVariableAccessSpecification_destroy);
    }

    *error = iedConnection_mapMmsErrorToIedError(mmsError);

exit_function:
    return dataSetMembers;
}

ClientDataSet
IedConnection_readDataSetValues(IedConnection self, IedClientError* error, const char* dataSetReference,
        ClientDataSet dataSet)
{
    char domainIdBuffer[65];
    char itemIdBuffer[DATA_SET_MAX_NAME_LENGTH + 1];

    const char* domainId = NULL;
    const char* itemId = NULL;

    bool isAssociationSpecific = false;

    if (dataSetReference[0] != '@') {

        if ((dataSetReference[0] == '/') || (strchr(dataSetReference, '/') == NULL)) {
            domainId = NULL;

            if (dataSetReference[0] == '/')
                itemId = dataSetReference + 1;
            else
                itemId = dataSetReference;
        }
        else {
            domainId = MmsMapping_getMmsDomainFromObjectReference(dataSetReference, domainIdBuffer);

            if (domainId == NULL) {
                *error = IED_ERROR_OBJECT_REFERENCE_INVALID;
                goto exit_function;
            }

            const char* itemIdRefOrig = dataSetReference + strlen(domainId) + 1;

            if (strlen(itemIdRefOrig) > DATA_SET_MAX_NAME_LENGTH) {
                *error = IED_ERROR_OBJECT_REFERENCE_INVALID;
                goto exit_function;
            }

            char* itemIdRef = StringUtils_copyStringToBuffer(itemIdRefOrig, itemIdBuffer);

            StringUtils_replace(itemIdRef, '.', '$');
            itemId = itemIdRef;
        }
    }
    else {
        itemId = dataSetReference + 1;
        isAssociationSpecific = true;
    }

    MmsError mmsError;

    MmsValue* dataSetVal;

    if (isAssociationSpecific)
        dataSetVal = MmsConnection_readNamedVariableListValuesAssociationSpecific(self->connection,
                &mmsError, itemId, true);
    else
        dataSetVal = MmsConnection_readNamedVariableListValues(self->connection, &mmsError,
                    domainId, itemId, true);

    if (dataSetVal == NULL) {
        *error = iedConnection_mapMmsErrorToIedError(mmsError);
        goto exit_function;
    }
    else
        *error = IED_ERROR_OK;

    if (dataSet == NULL) {
        dataSet = ClientDataSet_create(dataSetReference);
        ClientDataSet_setDataSetValues(dataSet, dataSetVal);
    }
    else {
        MmsValue* dataSetValues = ClientDataSet_getValues(dataSet);
        MmsValue_update(dataSetValues, dataSetVal);
        MmsValue_delete(dataSetVal);
    }

exit_function:
    return dataSet;
}

static void
getDataSetHandlerInternal(uint32_t invokeId, void* parameter, MmsError err, MmsValue* value)
{
    IedConnection self = (IedConnection) parameter;

    IedConnectionOutstandingCall call = iedConnection_lookupOutstandingCall(self, invokeId);

    if (call) {

        IedConnection_ReadDataSetHandler handler =  (IedConnection_ReadDataSetHandler) call->callback;

        ClientDataSet dataSet = (ClientDataSet) call->specificParameter;
        char* dataSetReference = (char*) call->specificParameter2.pointer;

        if (value != NULL) {

            if (dataSet == NULL) {
                dataSet = ClientDataSet_create(dataSetReference);
                ClientDataSet_setDataSetValues(dataSet, value);
                GLOBAL_FREEMEM(dataSetReference);
            }
            else {
                MmsValue* dataSetValues = ClientDataSet_getValues(dataSet);
                MmsValue_update(dataSetValues, value);
                MmsValue_delete(value);
            }
        }

        handler(invokeId, call->callbackParameter, iedConnection_mapMmsErrorToIedError(err), dataSet);

        iedConnection_releaseOutstandingCall(self, call);
    }
    else {
        if (DEBUG_IED_CLIENT)
            printf("IED_CLIENT: internal error - no matching outstanding call!\n");
    }
}

uint32_t
IedConnection_readDataSetValuesAsync(IedConnection self, IedClientError* error, const char* dataSetReference, ClientDataSet dataSet,
        IedConnection_ReadDataSetHandler handler, void* parameter)
{
    char domainIdBuffer[65];
    char itemIdBuffer[DATA_SET_MAX_NAME_LENGTH + 1];

    const char* domainId = NULL;
    const char* itemId = NULL;

    bool isAssociationSpecific = false;

    if (dataSetReference[0] != '@') {

        if ((dataSetReference[0] == '/') || (strchr(dataSetReference, '/') == NULL)) {
            domainId = NULL;

            if (dataSetReference[0] == '/')
                itemId = dataSetReference + 1;
            else
                itemId = dataSetReference;
        }
        else {
            domainId = MmsMapping_getMmsDomainFromObjectReference(dataSetReference, domainIdBuffer);

            if (domainId == NULL) {
                *error = IED_ERROR_OBJECT_REFERENCE_INVALID;
                return 0;
            }

            const char* itemIdRefOrig = dataSetReference + strlen(domainId) + 1;

            if (strlen(itemIdRefOrig) > DATA_SET_MAX_NAME_LENGTH) {
                *error = IED_ERROR_OBJECT_REFERENCE_INVALID;
                return 0;
            }

            char* itemIdRef = StringUtils_copyStringToBuffer(itemIdRefOrig, itemIdBuffer);

            StringUtils_replace(itemIdRef, '.', '$');
            itemId = itemIdRef;
        }
    }
    else {
        itemId = dataSetReference + 1;
        isAssociationSpecific = true;
    }

    IedConnectionOutstandingCall call = iedConnection_allocateOutstandingCall(self);

    if (call == NULL) {
        *error = IED_ERROR_OUTSTANDING_CALL_LIMIT_REACHED;
        return 0;
    }

    call->callback = handler;
    call->callbackParameter = parameter;
    call->specificParameter = dataSet;

    if (dataSet == NULL)
        call->specificParameter2.pointer = StringUtils_copyString(dataSetReference);
    else
        call->specificParameter2.pointer = NULL;

    MmsError err = MMS_ERROR_NONE;

    if (isAssociationSpecific)
        call->invokeId = MmsConnection_readNamedVariableListValuesAssociationSpecificAsync(self->connection,
                &err, itemId, true, getDataSetHandlerInternal, self);
    else
        call->invokeId = MmsConnection_readNamedVariableListValuesAsync(self->connection, &err,
                    domainId, itemId, true, getDataSetHandlerInternal, self);

    *error = iedConnection_mapMmsErrorToIedError(err);

    if (err != MMS_ERROR_NONE) {

        GLOBAL_FREEMEM(call->specificParameter2.pointer);

        iedConnection_releaseOutstandingCall(self, call);

        return 0;
    }

    return call->invokeId;
}

void
IedConnection_writeDataSetValues(IedConnection self, IedClientError* error, const char* dataSetReference,
        LinkedList/*<MmsValue*>*/ values, /* OUTPUT */LinkedList* /* <MmsValue*> */accessResults)
{
    char domainIdBuffer[65];
    char itemIdBuffer[DATA_SET_MAX_NAME_LENGTH + 1];

    const char* domainId = NULL;
    const char* itemId = NULL;

    bool isAssociationSpecific = false;

    if (dataSetReference[0] != '@') {

        if ((dataSetReference[0] == '/') || (strchr(dataSetReference, '/') == NULL)) {
            domainId = NULL;

            if (dataSetReference[0] == '/')
                itemId = dataSetReference + 1;
            else
                itemId = dataSetReference;
        }
        else {
            domainId = MmsMapping_getMmsDomainFromObjectReference(dataSetReference, domainIdBuffer);

            if (domainId == NULL) {
                *error = IED_ERROR_OBJECT_REFERENCE_INVALID;
                goto exit_function;
            }

            const char* itemIdRefOrig = dataSetReference + strlen(domainId) + 1;

            if (strlen(itemIdRefOrig) > DATA_SET_MAX_NAME_LENGTH) {
                *error = IED_ERROR_OBJECT_REFERENCE_INVALID;
                goto exit_function;
            }

            char* itemIdRef = StringUtils_copyStringToBuffer(itemIdRefOrig, itemIdBuffer);

            StringUtils_replace(itemIdRef, '.', '$');
            itemId = itemIdRef;
        }
    }
    else {
        itemId = dataSetReference + 1;
        isAssociationSpecific = true;
    }

    MmsError mmsError;

    MmsConnection_writeNamedVariableList(self->connection, &mmsError, isAssociationSpecific, domainId, itemId, values, accessResults);

    *error = iedConnection_mapMmsErrorToIedError(mmsError);

exit_function:
    return;
}

static void
writeDataSetHandlerInternal(uint32_t invokeId, void* parameter, MmsError err, LinkedList /* <MmsValue*> */ accessResults)
{
    IedConnection self = (IedConnection) parameter;

    IedConnectionOutstandingCall call = iedConnection_lookupOutstandingCall(self, invokeId);

    if (call) {

        IedConnection_WriteDataSetHandler handler =  (IedConnection_WriteDataSetHandler) call->callback;

        handler(invokeId, call->callbackParameter, iedConnection_mapMmsErrorToIedError(err), accessResults);

        iedConnection_releaseOutstandingCall(self, call);
    }
    else {
        if (DEBUG_IED_CLIENT)
            printf("IED_CLIENT: internal error - no matching outstanding call!\n");
    }
}

LIB61850_API uint32_t
IedConnection_writeDataSetValuesAsync(IedConnection self, IedClientError* error, const char* dataSetReference,
        LinkedList/*<MmsValue*>*/ values, IedConnection_WriteDataSetHandler handler, void* parameter)
{
    char domainIdBuffer[65];
    char itemIdBuffer[DATA_SET_MAX_NAME_LENGTH + 1];

    const char* domainId = NULL;
    const char* itemId = NULL;

    bool isAssociationSpecific = false;

    if (dataSetReference[0] != '@') {

        if ((dataSetReference[0] == '/') || (strchr(dataSetReference, '/') == NULL)) {
            domainId = NULL;

            if (dataSetReference[0] == '/')
                itemId = dataSetReference + 1;
            else
                itemId = dataSetReference;
        }
        else {
            domainId = MmsMapping_getMmsDomainFromObjectReference(dataSetReference, domainIdBuffer);

            if (domainId == NULL) {
                *error = IED_ERROR_OBJECT_REFERENCE_INVALID;
                return 0;
            }

            const char* itemIdRefOrig = dataSetReference + strlen(domainId) + 1;

            if (strlen(itemIdRefOrig) > DATA_SET_MAX_NAME_LENGTH) {
                *error = IED_ERROR_OBJECT_REFERENCE_INVALID;
                return 0;
            }

            char* itemIdRef = StringUtils_copyStringToBuffer(itemIdRefOrig, itemIdBuffer);

            StringUtils_replace(itemIdRef, '.', '$');
            itemId = itemIdRef;
        }
    }
    else {
        itemId = dataSetReference + 1;
        isAssociationSpecific = true;
    }

    IedConnectionOutstandingCall call = iedConnection_allocateOutstandingCall(self);

    if (call == NULL) {
        *error = IED_ERROR_OUTSTANDING_CALL_LIMIT_REACHED;
        return 0;
    }

    call->callback = handler;
    call->callbackParameter = parameter;

    MmsError err = MMS_ERROR_NONE;

    call->invokeId = MmsConnection_writeNamedVariableListAsync(self->connection, &err, isAssociationSpecific, domainId, itemId, values, writeDataSetHandlerInternal, self);

    *error = iedConnection_mapMmsErrorToIedError(err);

    if (err != MMS_ERROR_NONE) {
        iedConnection_releaseOutstandingCall(self, call);

        return 0;
    }

    return call->invokeId;
}

LinkedList /* <MmsJournalEntry> */
IedConnection_queryLogByTime(IedConnection self, IedClientError* error, const char* logReference,
        uint64_t startTime, uint64_t endTime, bool* moreFollows)
{
    MmsError mmsError;

    char logRef[130];

    strncpy(logRef, logReference, 129);

    char* logDomain = logRef;
    char* logName = strchr(logRef, '/');

    if (logName != NULL) {

        logName[0] = 0;
        logName++;

        MmsValue* startTimeMms = MmsValue_newBinaryTime(false);
        MmsValue_setBinaryTime(startTimeMms, startTime);

        MmsValue* endTimeMms = MmsValue_newBinaryTime(false);
        MmsValue_setBinaryTime(endTimeMms, endTime);

        LinkedList journalEntries = MmsConnection_readJournalTimeRange(self->connection, &mmsError, logDomain, logName,
                startTimeMms, endTimeMms, moreFollows);

        MmsValue_delete(startTimeMms);
        MmsValue_delete(endTimeMms);

        if (mmsError != MMS_ERROR_NONE) {
            *error = iedConnection_mapMmsErrorToIedError(mmsError);
            return NULL;
        }
        else
            return journalEntries;
    }
    else {
        *error = IED_ERROR_OBJECT_REFERENCE_INVALID;
        return NULL;
    }

}

static void
readJournalHandler(uint32_t invokeId, void* parameter, MmsError err, LinkedList /* <MmsJournalEntry> */ journalEntries, bool moreFollows)
{
    IedConnection self = (IedConnection) parameter;

    IedConnectionOutstandingCall call = iedConnection_lookupOutstandingCall(self, invokeId);

    if (call) {

        IedConnection_QueryLogHandler handler =  (IedConnection_QueryLogHandler) call->callback;

        handler(invokeId, call->callbackParameter, iedConnection_mapMmsErrorToIedError(err), journalEntries, moreFollows);

        iedConnection_releaseOutstandingCall(self, call);
    }
    else {
        if (DEBUG_IED_CLIENT)
            printf("IED_CLIENT: internal error - no matching outstanding call!\n");
    }
}

uint32_t
IedConnection_queryLogByTimeAsync(IedConnection self, IedClientError* error, const char* logReference,
        uint64_t startTime, uint64_t endTime, IedConnection_QueryLogHandler handler, void* parameter)
{
    char logRef[130];

    strncpy(logRef, logReference, 129);

    char* logDomain = logRef;
    char* logName = strchr(logRef, '/');

    if (logName != NULL) {

        logName[0] = 0;
        logName++;

        IedConnectionOutstandingCall call = iedConnection_allocateOutstandingCall(self);

        if (call == NULL) {
            *error = IED_ERROR_OUTSTANDING_CALL_LIMIT_REACHED;
            return 0;
        }

        call->callback = handler;
        call->callbackParameter = parameter;

        MmsError err = MMS_ERROR_NONE;

        MmsValue* startTimeMms = MmsValue_newBinaryTime(false);
        MmsValue_setBinaryTime(startTimeMms, startTime);

        MmsValue* endTimeMms = MmsValue_newBinaryTime(false);
        MmsValue_setBinaryTime(endTimeMms, endTime);

        call->invokeId = MmsConnection_readJournalTimeRangeAsync(self->connection, &err, logDomain, logName,
                startTimeMms, endTimeMms, readJournalHandler, self);

        MmsValue_delete(startTimeMms);
        MmsValue_delete(endTimeMms);

        *error = iedConnection_mapMmsErrorToIedError(err);

        if (err != MMS_ERROR_NONE) {
            iedConnection_releaseOutstandingCall(self, call);
            return 0;
        }

        return call->invokeId;
    }
    else {
        *error = IED_ERROR_OBJECT_REFERENCE_INVALID;
        return 0;
    }

}

uint32_t
IedConnection_queryLogAfterAsync(IedConnection self, IedClientError* error, const char* logReference,
        MmsValue* entryID, uint64_t timeStamp, IedConnection_QueryLogHandler handler, void* parameter)
{
    char logRef[130];

    strncpy(logRef, logReference, 129);

    char* logDomain = logRef;
    char* logName = strchr(logRef, '/');

    if (logName != NULL) {

        logName[0] = 0;
        logName++;

        IedConnectionOutstandingCall call = iedConnection_allocateOutstandingCall(self);

        if (call == NULL) {
            *error = IED_ERROR_OUTSTANDING_CALL_LIMIT_REACHED;
            return 0;
        }

        call->callback = handler;
        call->callbackParameter = parameter;

        MmsError err = MMS_ERROR_NONE;

        MmsValue* timeStampMms = MmsValue_newBinaryTime(false);
        MmsValue_setBinaryTime(timeStampMms, timeStamp);

        call->invokeId = MmsConnection_readJournalStartAfterAsync(self->connection, &err, logDomain, logName,
               timeStampMms, entryID, readJournalHandler, self);

        MmsValue_delete(timeStampMms);

        *error = iedConnection_mapMmsErrorToIedError(err);

        if (err != MMS_ERROR_NONE) {
            iedConnection_releaseOutstandingCall(self, call);
            return 0;
        }

        return call->invokeId;
    }
    else {
        *error = IED_ERROR_OBJECT_REFERENCE_INVALID;
        return 0;
    }
}

LinkedList /* <MmsJournalEntry> */
IedConnection_queryLogAfter(IedConnection self, IedClientError* error, const char* logReference,
        MmsValue* entryID, uint64_t timeStamp, bool* moreFollows)
{
    MmsError mmsError;

    char logRef[130];

    strncpy(logRef, logReference, 129);

    char* logDomain = logRef;
    char* logName = strchr(logRef, '/');

    if (logName != NULL) {

        logName[0] = 0;
        logName++;

        MmsValue* timeStampMms = MmsValue_newBinaryTime(false);
        MmsValue_setBinaryTime(timeStampMms, timeStamp);

        LinkedList journalEntries = MmsConnection_readJournalStartAfter(self->connection, &mmsError, logDomain, logName,
               timeStampMms, entryID,  moreFollows);

        MmsValue_delete(timeStampMms);

        if (mmsError != MMS_ERROR_NONE) {
            *error = iedConnection_mapMmsErrorToIedError(mmsError);
            return NULL;
        }
        else
            return journalEntries;
    }
    else {
        *error = IED_ERROR_OBJECT_REFERENCE_INVALID;
        return NULL;
    }
}



MmsConnection
IedConnection_getMmsConnection(IedConnection self)
{
    return self->connection;
}

LastApplError
IedConnection_getLastApplError(IedConnection self)
{
    return self->lastApplError;
}

void
iedConnection_addControlClient(IedConnection self, ControlObjectClient control)
{
    Semaphore_wait(self->clientControlsLock);

    LinkedList_add(self->clientControls, control);

    Semaphore_post(self->clientControlsLock);
}

void
iedConnection_removeControlClient(IedConnection self, ControlObjectClient control)
{
    Semaphore_wait(self->clientControlsLock);

    LinkedList_remove(self->clientControls, control);

    Semaphore_post(self->clientControlsLock);
}

FileDirectoryEntry
FileDirectoryEntry_create(const char* fileName, uint32_t fileSize, uint64_t lastModified)
{
    FileDirectoryEntry self = (FileDirectoryEntry) GLOBAL_CALLOC(1, sizeof(struct sFileDirectoryEntry));

    self->fileName = StringUtils_copyString(fileName);
    self->fileSize = fileSize;
    self->lastModified = lastModified;

    return self;
}

void
FileDirectoryEntry_destroy(FileDirectoryEntry self)
{
    GLOBAL_FREEMEM(self->fileName);
    GLOBAL_FREEMEM(self);
}

const char*
FileDirectoryEntry_getFileName(FileDirectoryEntry self)
{
    return self->fileName;
}

uint32_t
FileDirectoryEntry_getFileSize(FileDirectoryEntry self)
{
    return self->fileSize;
}

uint64_t
FileDirectoryEntry_getLastModified(FileDirectoryEntry self)
{
    return self->lastModified;
}

