/*
 *  mms_server.c
 *
 *  Copyright 2013-2020 Michael Zillgith
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
#include "mms_server.h"
#include "mms_server_connection.h"
#include "mms_value_cache.h"
#include "mms_server_internal.h"
#include "iso_server_private.h"

static Map
createValueCaches(MmsDevice* device)
{
    Map valueCaches = Map_create();

    int i;
    for (i = 0; i < device->domainCount; i++) {
        MmsValueCache valueCache = MmsValueCache_create(device->domains[i]);
        Map_addEntry(valueCaches, device->domains[i], valueCache);
    }

#if (CONFIG_MMS_SUPPORT_VMD_SCOPE_NAMED_VARIABLES == 1)
    MmsValueCache valueCache = MmsValueCache_create((MmsDomain*) device);
    Map_addEntry(valueCaches, (MmsDomain*) device, valueCache);
#endif

    return valueCaches;
}

MmsServer
MmsServer_create(MmsDevice* device, TLSConfiguration tlsConfiguration)
{
    MmsServer self = (MmsServer) GLOBAL_CALLOC(1, sizeof(struct sMmsServer));

    if (self) {
#if (CONFIG_MMS_THREADLESS_STACK != 1)
        self->openConnectionsLock = Semaphore_create(1);

        if (self->openConnectionsLock == NULL)
            goto exit_error;

        self->modelMutex = Semaphore_create(1);

        if (self->modelMutex == NULL)
            goto exit_error;

        self->transmitBufferMutex = Semaphore_create(1);

        if (self->transmitBufferMutex == NULL)
            goto exit_error;
#endif

        self->isoServerList = LinkedList_create();

        if (self->isoServerList == NULL)
            goto exit_error;

        if (tlsConfiguration) {
            IsoServer isoServer = IsoServer_create(tlsConfiguration);

            if (isoServer == NULL)
                goto exit_error;

            LinkedList_add(self->isoServerList, isoServer);
        }

        self->device = device;
        self->openConnections = Map_create();

        if (self->openConnections == NULL)
            goto exit_error;

        self->valueCaches = createValueCaches(device);

        if (self->valueCaches == NULL)
            goto exit_error;

        self->isLocked = false;

        self->transmitBuffer = ByteBuffer_create(NULL, CONFIG_MMS_MAXIMUM_PDU_SIZE);

        if (self->transmitBuffer == NULL)
            goto exit_error;

#if (CONFIG_MMS_SERVER_CONFIG_SERVICES_AT_RUNTIME == 1)
        self->fileServiceEnabled = true;
        self->dynamicVariableListServiceEnabled = true;
        self->journalServiceEnabled = true;
        self->maxDataSetEntries = CONFIG_MMS_MAX_NUMBER_OF_DATA_SET_MEMBERS;
        self->maxAssociationSpecificDataSets = CONFIG_MMS_MAX_NUMBER_OF_ASSOCIATION_SPECIFIC_DATA_SETS;
        self->maxDomainSpecificDataSets = CONFIG_MMS_MAX_NUMBER_OF_DOMAIN_SPECIFIC_DATA_SETS;
#endif /* (CONFIG_MMS_SERVER_CONFIG_SERVICES_AT_RUNTIME == 1) */
    }

    return self;

exit_error:

    MmsServer_destroy(self);

    return NULL;
}

bool
MmsServer_addAP(MmsServer self, const char* ipAddr, int tcpPort, TLSConfiguration tlsConfiguration)
{
    IsoServer isoServer = IsoServer_create(tlsConfiguration);

    if (isoServer) {

        IsoServer_setLocalIpAddress(isoServer, ipAddr);

        if (tcpPort != -1)
            IsoServer_setTcpPort(isoServer, tcpPort);

        if (self->authenticator)
            IsoServer_setAuthenticator(isoServer, self->authenticator, self->authenticatorParameter);

        LinkedList_add(self->isoServerList, isoServer);

        return true;
    }

    return false;
}

void
MmsServer_setLocalIpAddress(MmsServer self, const char* localIpAddress)
{
    if (LinkedList_size(self->isoServerList) == 0) {
        MmsServer_addAP(self, NULL, -1, NULL);
    }

    LinkedList elem = LinkedList_get(self->isoServerList, 0);

    if (elem) {
        IsoServer isoServer = (IsoServer) LinkedList_getData(elem);

        IsoServer_setLocalIpAddress(isoServer, localIpAddress);
    }
}

bool
MmsServer_isRunning(MmsServer self)
{
    LinkedList elem = LinkedList_get(self->isoServerList, 0);

    if (elem) {
        IsoServer isoServer = (IsoServer) LinkedList_getData(elem);

        if (IsoServer_getState(isoServer) == ISO_SVR_STATE_RUNNING)
            return true;
    }

    return false;
}

void
MmsServer_setFilestoreBasepath(MmsServer self, const char* basepath)
{
#if (CONFIG_SET_FILESTORE_BASEPATH_AT_RUNTIME == 1)
    if (self->filestoreBasepath != NULL) {
        GLOBAL_FREEMEM(self->filestoreBasepath);
        self->filestoreBasepath = NULL;
    }

    if (basepath != NULL)
        self->filestoreBasepath = StringUtils_copyString(basepath);
#endif /* (CONFIG_SET_FILESTORE_BASEPATH_AT_RUNTIME == 1) */
}

#if (CONFIG_MMS_SERVER_CONFIG_SERVICES_AT_RUNTIME == 1)

void
MmsServer_setMaxConnections(MmsServer self, int maxConnections)
{
    if (self->isoServerList) {

        if (LinkedList_size(self->isoServerList) == 0) {
            MmsServer_addAP(self, NULL, -1, NULL);
        }

        LinkedList elem = LinkedList_getNext(self->isoServerList);

        while (elem) {
            IsoServer isoServer = (IsoServer) LinkedList_getData(elem);

            IsoServer_setMaxConnections(isoServer, maxConnections);

            elem = LinkedList_getNext(elem);
        }
    }
}

void
MmsServer_enableFileService(MmsServer self, bool enable)
{
    self->fileServiceEnabled = enable;
}

void
MmsServer_enableDynamicNamedVariableListService(MmsServer self, bool enable)
{
    self->dynamicVariableListServiceEnabled = enable;
}

void
MmsServer_setMaxDataSetEntries(MmsServer self, int maxDataSetEntries)
{
    self->maxDataSetEntries = maxDataSetEntries;
}

void
MmsServer_enableJournalService(MmsServer self, bool enable)
{
    self->journalServiceEnabled = enable;
}

void
MmsServer_setMaxAssociationSpecificDataSets(MmsServer self, int maxDataSets)
{
    self->maxAssociationSpecificDataSets = maxDataSets;
}

void
MmsServer_setMaxDomainSpecificDataSets(MmsServer self, int maxDataSets)
{
    self->maxDomainSpecificDataSets = maxDataSets;
}

#endif /* (CONFIG_MMS_SERVER_CONFIG_SERVICES_AT_RUNTIME == 1) */

void
MmsServer_lockModel(MmsServer self)
{
#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_wait(self->modelMutex);
#endif
}

void
MmsServer_unlockModel(MmsServer self)
{
#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_post(self->modelMutex);
#endif
}

ByteBuffer*
MmsServer_reserveTransmitBuffer(MmsServer self)
{
#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_wait(self->transmitBufferMutex);
#endif

    return self->transmitBuffer;
}

void
MmsServer_releaseTransmitBuffer(MmsServer self)
{
    self->transmitBuffer->size = 0;

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_post(self->transmitBufferMutex);
#endif
}

#if (MMS_OBTAIN_FILE_SERVICE == 1)

MmsObtainFileTask
MmsServer_getObtainFileTask(MmsServer self)
{
    int i;

    for (i = 0; i < CONFIG_MMS_SERVER_MAX_GET_FILE_TASKS; i++) {

        if (self->fileUploadTasks[i].state == 0) {
            self->fileUploadTasks[i].state = 1;
            if (self->fileUploadTasks[i].taskLock == NULL)
                self->fileUploadTasks[i].taskLock = Semaphore_create(1);
            return &(self->fileUploadTasks[i]);
        }

    }

    return NULL;
}

#endif /* (MMS_OBTAIN_FILE_SERVICE == 1) */

void
MmsServer_installReadHandler(MmsServer self, MmsReadVariableHandler readHandler, void* parameter)
{
    self->readHandler = readHandler;
    self->readHandlerParameter = parameter;
}

void
MmsServer_installReadAccessHandler(MmsServer self, MmsReadAccessHandler readAccessHandler, void* parameter)
{
    self->readAccessHandler = readAccessHandler;
    self->readAccessHandlerParameter = parameter;
}

void
MmsServer_installWriteHandler(MmsServer self, MmsWriteVariableHandler writeHandler, void* parameter)
{
    self->writeHandler = writeHandler;
    self->writeHandlerParameter = parameter;
}

void
MmsServer_installConnectionHandler(MmsServer self, MmsConnectionHandler connectionHandler, void* parameter)
{
    self->connectionHandler = connectionHandler;
    self->connectionHandlerParameter = parameter;
}

void
MmsServer_installVariableListChangedHandler(MmsServer self, MmsNamedVariableListChangedHandler handler, void* parameter)
{
    self->variableListChangedHandler = handler;
    self->variableListChangedHandlerParameter = parameter;
}

void
MmsServer_setClientAuthenticator(MmsServer self, AcseAuthenticator authenticator, void* authenticatorParameter)
{
    self->authenticator = authenticator;
    self->authenticatorParameter = authenticatorParameter;

    if (self->isoServerList) {

        LinkedList elem = LinkedList_getNext(self->isoServerList);

        while (elem) {
            IsoServer isoServer = (IsoServer) LinkedList_getData(elem);

            IsoServer_setAuthenticator(isoServer, authenticator, authenticatorParameter);

            elem = LinkedList_getNext(elem);
        }
    }
}

#if (MMS_FILE_SERVICE == 1)
void
MmsServer_installFileAccessHandler(MmsServer self, MmsFileAccessHandler handler, void* parameter)
{
    self->fileAccessHandler = handler;
    self->fileAccessHandlerParameter = parameter;
}
#endif /* (MMS_FILE_SERVICE == 1) */

#if (MMS_OBTAIN_FILE_SERVICE == 1)
void
MmsServer_installObtainFileHandler(MmsServer self, MmsObtainFileHandler handler, void* parameter)
{
    self->obtainFileHandler = handler;
    self->obtainFileHandlerParameter = parameter;
}

void
MmsServer_installGetFileCompleteHandler(MmsServer self, MmsGetFileCompleteHandler handler, void* parameter)
{
    self->getFileCompleteHandler = handler;
    self->getFileCompleteHandlerParameter = parameter;
}
#endif /* (MMS_OBTAIN_FILE_SERVICE == 1) */

static void
closeConnection(void* con)
{
    MmsServerConnection connection = (MmsServerConnection) con;

    MmsServerConnection_destroy(connection);
}

static void
deleteSingleCache(MmsValueCache cache)
{
    MmsValueCache_destroy(cache);
}

void
MmsServer_destroy(MmsServer self)
{
    if (self) {
        LinkedList_destroyDeep(self->isoServerList, (LinkedListValueDeleteFunction) IsoServer_destroy);

        Map_deleteDeep(self->openConnections, false, closeConnection);
        Map_deleteDeep(self->valueCaches, false, (void (*) (void*)) deleteSingleCache);

#if (CONFIG_MMS_THREADLESS_STACK != 1)
        if (self->openConnectionsLock)
            Semaphore_destroy(self->openConnectionsLock);

        if (self->modelMutex)
            Semaphore_destroy(self->modelMutex);

        if (self->transmitBufferMutex)
            Semaphore_destroy(self->transmitBufferMutex);
#endif

        if (self->transmitBuffer)
            ByteBuffer_destroy(self->transmitBuffer);

#if (CONFIG_SET_FILESTORE_BASEPATH_AT_RUNTIME == 1)
        if (self->filestoreBasepath != NULL)
            GLOBAL_FREEMEM(self->filestoreBasepath);
#endif

#if (MMS_OBTAIN_FILE_SERVICE == 1)
        int i;
        for (i = 0; i < CONFIG_MMS_SERVER_MAX_GET_FILE_TASKS; i++) {
            if (self->fileUploadTasks[i].taskLock)
                Semaphore_destroy(self->fileUploadTasks[i].taskLock);
        }
#endif

        GLOBAL_FREEMEM(self);
    }
}

MmsValue*
MmsServer_getValueFromCache(MmsServer self, MmsDomain* domain, const char* itemId)
{
    MmsValueCache cache = (MmsValueCache) Map_getEntry(self->valueCaches, domain);

    if (cache != NULL)
        return MmsValueCache_lookupValue(cache, itemId, NULL);

    return NULL ;
}

MmsValue*
MmsServer_getValueFromCacheEx(MmsServer self, MmsDomain* domain, const char* itemId, MmsVariableSpecification** typeSpec)
{
    MmsValueCache cache = (MmsValueCache) Map_getEntry(self->valueCaches, domain);

    if (cache != NULL)
        return MmsValueCache_lookupValue(cache, itemId, typeSpec);

    return NULL ;
}

void
MmsServer_insertIntoCache(MmsServer self, MmsDomain* domain, char* itemId, MmsValue* value)
{
    MmsValueCache cache = (MmsValueCache) Map_getEntry(self->valueCaches, domain);

    if (cache != NULL) {
        MmsValueCache_insertValue(cache, itemId, value);
    }
}

MmsDataAccessError
mmsServer_setValue(MmsServer self, MmsDomain* domain, char* itemId, MmsValue* value,
        MmsServerConnection connection)
{
    MmsDataAccessError indication;

    if (self->writeHandler != NULL) {
        indication = self->writeHandler(self->writeHandlerParameter, domain,
                itemId, value, connection);
    }
    else {
        MmsValue* cachedValue;

        if (domain == NULL)
            domain = (MmsDomain*) self->device;

        cachedValue = MmsServer_getValueFromCache(self, domain, itemId);

        if (cachedValue != NULL) {
            MmsValue_update(cachedValue, value);
            indication = DATA_ACCESS_ERROR_SUCCESS;
        } else
            indication = DATA_ACCESS_ERROR_OBJECT_VALUE_INVALID;
    }

    return indication;
}

MmsValue*
mmsServer_getValue(MmsServer self, MmsDomain* domain, char* itemId, MmsServerConnection connection, bool isDirectAccess)
{
    MmsValue* value = NULL;

    if (self->readAccessHandler != NULL) {
        MmsDataAccessError accessError =
                self->readAccessHandler(self->readAccessHandlerParameter, (domain == (MmsDomain*) self->device) ? NULL : domain,
                        itemId, connection, isDirectAccess);

        if (accessError != DATA_ACCESS_ERROR_SUCCESS) {
            value = MmsValue_newDataAccessError(accessError);
            MmsValue_setDeletable(value);
            goto exit_function;
        }
    }

    value = MmsServer_getValueFromCache(self, domain, itemId);

    if (value == NULL)
        if (self->readHandler != NULL)
            value = self->readHandler(self->readHandlerParameter, (domain == (MmsDomain*) self->device) ? NULL : domain,
                    itemId, connection, isDirectAccess);

exit_function:
    return value;
}


MmsDevice*
MmsServer_getDevice(MmsServer self)
{
    return self->device;
}

static void /* will be called by ISO server stack */
isoConnectionIndicationHandler(IsoConnectionIndication indication,
		void* parameter, IsoConnection connection)
{
    MmsServer self = (MmsServer) parameter;

    if (indication == ISO_CONNECTION_OPENED) {
        MmsServerConnection mmsCon = MmsServerConnection_init(0, self, connection);

#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_wait(self->openConnectionsLock);
#endif

        Map_addEntry(self->openConnections, connection, mmsCon);

#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_post(self->openConnectionsLock);
#endif

        if (self->connectionHandler != NULL)
            self->connectionHandler(self->connectionHandlerParameter,
                    mmsCon, MMS_SERVER_NEW_CONNECTION);
    }
    else if (indication == ISO_CONNECTION_CLOSED) {

#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_wait(self->openConnectionsLock);
#endif

        MmsServerConnection mmsCon = (MmsServerConnection)
                Map_removeEntry(self->openConnections, connection, false);

#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_post(self->openConnectionsLock);
#endif

        if (self->connectionHandler != NULL)
            self->connectionHandler(self->connectionHandlerParameter,
                    mmsCon, MMS_SERVER_CONNECTION_CLOSED);

        if (mmsCon != NULL)
            MmsServerConnection_destroy(mmsCon);
    }
}

#if (CONFIG_MMS_THREADLESS_STACK != 1) && (CONFIG_MMS_SINGLE_THREADED != 1)
void
MmsServer_startListening(MmsServer self, int tcpPort)
{
    if (self->isoServerList) {

        if (LinkedList_size(self->isoServerList) == 0) {
            MmsServer_addAP(self, NULL, -1, NULL);
        }

        LinkedList elem = LinkedList_getNext(self->isoServerList);

        while (elem) {
            IsoServer isoServer = (IsoServer) LinkedList_getData(elem);

            IsoServer_setConnectionHandler(isoServer, isoConnectionIndicationHandler, (void*) self);

            if (tcpPort != -1)
                IsoServer_setTcpPort(isoServer, tcpPort);

            IsoServer_startListening(isoServer);

            elem = LinkedList_getNext(elem);
        }
    }
}

void
MmsServer_stopListening(MmsServer self)
{
    if (self->isoServerList) {
        LinkedList elem = LinkedList_getNext(self->isoServerList);

        while (elem) {
            IsoServer isoServer = (IsoServer) LinkedList_getData(elem);

            IsoServer_stopListening(isoServer);

            elem = LinkedList_getNext(elem);
        }
    }
}
#endif /* (CONFIG_MMS_THREADLESS_STACK != 1) && (CONFIG_MMS_SINGLE_THREADED != 1)*/

void
MmsServer_startListeningThreadless(MmsServer self, int tcpPort)
{
    if (self->isoServerList) {

        if (LinkedList_size(self->isoServerList) == 0) {
            MmsServer_addAP(self, NULL, -1, NULL);
        }

        LinkedList elem = LinkedList_getNext(self->isoServerList);

        while (elem) {
            IsoServer isoServer = (IsoServer) LinkedList_getData(elem);

            IsoServer_setConnectionHandler(isoServer, isoConnectionIndicationHandler, (void*) self);

            if (tcpPort != -1)
                IsoServer_setTcpPort(isoServer, tcpPort);

            IsoServer_startListeningThreadless(isoServer);

            elem = LinkedList_getNext(elem);
        }
    }
}

int
MmsServer_waitReady(MmsServer self, unsigned int timeoutMs)
{
    bool isFirst = true;
    int result = 0;

    if (self->isoServerList) {
        LinkedList elem = LinkedList_getNext(self->isoServerList);

        while (elem) {
            IsoServer isoServer = (IsoServer) LinkedList_getData(elem);

            int serverResult;

            if (isFirst) {
                serverResult = IsoServer_waitReady(isoServer, timeoutMs);
                isFirst = false;
            }
            else {
                serverResult = IsoServer_waitReady(isoServer, 0);
            }

            if (serverResult > 0)
                result += serverResult;

            elem = LinkedList_getNext(elem);
        }
    }

    return result;
}

void
MmsServer_handleIncomingMessages(MmsServer self)
{
    if (self->isoServerList) {
        LinkedList elem = LinkedList_getNext(self->isoServerList);

        while (elem) {
            IsoServer isoServer = (IsoServer) LinkedList_getData(elem);

            IsoServer_processIncomingMessages(isoServer);

            elem = LinkedList_getNext(elem);
        }
    }
}

void
MmsServer_handleBackgroundTasks(MmsServer self)
{

#if (MMS_OBTAIN_FILE_SERVICE == 1)

    int i;
    for (i = 0; i < CONFIG_MMS_SERVER_MAX_GET_FILE_TASKS; i++)
    {
        if (self->fileUploadTasks[i].state != 0) {

            Semaphore_wait(self->fileUploadTasks[i].taskLock);

            if (self->fileUploadTasks[i].state != 0)
                mmsServer_fileUploadTask(self, &(self->fileUploadTasks[i]));

            Semaphore_post(self->fileUploadTasks[i].taskLock);
        }
    }

#endif /* (MMS_OBTAIN_FILE_SERVICE == 1) */
}

int
MmsServer_getConnectionCounter(MmsServer self)
{
    int count = 0;

    if (self->isoServerList) {
        LinkedList elem = LinkedList_getNext(self->isoServerList);

        while (elem) {
            IsoServer isoServer = (IsoServer) LinkedList_getData(elem);

            count += IsoServer_getConnectionCounter(isoServer);

            elem = LinkedList_getNext(elem);
        }
    }

    return count;
}

void
MmsServer_callConnectionHandler(MmsServer self, MmsServerConnection connection)
{
    if (self->connectionHandler) {
        self->connectionHandler(self->connectionHandlerParameter, connection, MMS_SERVER_CONNECTION_TICK);
    }
}

void
MmsServer_stopListeningThreadless(MmsServer self)
{
    if (self->isoServerList) {
        LinkedList elem = LinkedList_getNext(self->isoServerList);

        while (elem) {
            IsoServer isoServer = (IsoServer) LinkedList_getData(elem);

            IsoServer_stopListeningThreadless(isoServer);

            elem = LinkedList_getNext(elem);
        }
    }
}

const char*
MmsServer_getFilesystemBasepath(MmsServer self)
{
#if (CONFIG_SET_FILESTORE_BASEPATH_AT_RUNTIME == 1)
    if (self->filestoreBasepath != NULL)
        return self->filestoreBasepath;
    else
        return CONFIG_VIRTUAL_FILESTORE_BASEPATH;
#else
    return CONFIG_VIRTUAL_FILESTORE_BASEPATH;
#endif
}


