/*
 *  ied_server.c
 *
 *  Copyright 2013-2024 Michael Zillgith
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

#include "iec61850_server.h"
#include "mms_mapping.h"
#include "mms_mapping_internal.h"
#include "mms_value_internal.h"
#include "mms_server_libinternal.h"
#include "control.h"
#include "stack_config.h"
#include "ied_server_private.h"
#include "hal_thread.h"
#include "reporting.h"

#include "libiec61850_platform_includes.h"
#include "mms_sv.h"
#include "mms_goose.h"

#ifndef DEBUG_IED_SERVER
#define DEBUG_IED_SERVER 0
#endif

#if (CONFIG_IEC61850_CONTROL_SERVICE == 1)
static bool
createControlObjects(IedServer self, MmsDomain* domain, char* lnName, MmsVariableSpecification* typeSpec, char* namePrefix)
{
    MmsMapping* mapping = self->mmsMapping;

    bool success = false;

    if (typeSpec->type == MMS_STRUCTURE)
    {
        int coCount = typeSpec->typeSpec.structure.elementCount;
        int i;
        for (i = 0; i < coCount; i++)
        {
            char objectName[65];
            objectName[0] = 0;

            if (namePrefix != NULL)
            {
                StringUtils_concatString(objectName, 65, namePrefix, "$");
            }

            bool hasCancel = false;
            int cancelIndex = 0;
            bool hasSBOw = false;
            bool hasSBO = false;
            int sBOwIndex = 0;
            int operIndex = 0;
            int sBOIndex = 0;

            MmsVariableSpecification* coSpec = typeSpec->typeSpec.structure.elements[i];

            if (coSpec->type == MMS_STRUCTURE)
            {
                int coElementCount = coSpec->typeSpec.structure.elementCount;

                MmsVariableSpecification* operSpec = NULL;

                int j;
                for (j = 0; j < coElementCount; j++)
                {
                    MmsVariableSpecification* coElementSpec = coSpec->typeSpec.structure.elements[j];

                    if (strcmp(coElementSpec->name, "Oper") == 0)
                    {
                        operSpec = coElementSpec;
                        operIndex = j;
                    }
                    else if (strcmp(coElementSpec->name, "Cancel") == 0)
                    {
                        hasCancel = true;
                        cancelIndex = j;
                    }
                    else if (strcmp(coElementSpec->name, "SBOw") == 0)
                    {
                        hasSBOw = true;
                        sBOwIndex = j;
                    }
                    else if ((strcmp(coElementSpec->name, "SBO") == 0))
                    {
                        hasSBO = true;
                        sBOIndex = j;
                    }
                    else
                    {
                        if (DEBUG_IED_SERVER)
                            printf("IED_SERVER: createControlObjects: Unknown element in CO: %s\n", coElementSpec->name);

                        break;
                    }
                }

                StringUtils_appendString(objectName, 65, coSpec->name);

                if (operSpec)
                {
                    if (DEBUG_IED_SERVER)
                        printf("IED_SERVER: create control object LN:%s DO:%s\n", lnName, objectName);

                    ControlObject* controlObject = ControlObject_create(self, domain, lnName, objectName, operSpec);

                    if (controlObject == NULL)
                        goto exit_function;

                    MmsValue* structure = MmsValue_newDefaultValue(coSpec);

                    if (structure == NULL)
                    {
                        ControlObject_destroy(controlObject);
                        goto exit_function;
                    }

                    ControlObject_setMmsValue(controlObject, structure);

                    ControlObject_setTypeSpec(controlObject, coSpec);

                    controlObject->oper = MmsValue_getElement(structure, operIndex);

                    if (hasCancel)
                        controlObject->cancel = MmsValue_getElement(structure, cancelIndex);

                    if (hasSBOw)
                        controlObject->sbow = MmsValue_getElement(structure, sBOwIndex);

                    if (hasSBO)
                        controlObject->sbo = MmsValue_getElement(structure, sBOIndex);

                    MmsMapping_addControlObject(mapping, controlObject);
                }
                else
                {
                    if (createControlObjects(self, domain, lnName, coSpec, objectName) == false)
                        goto exit_function;
                }
            }
        }
    }

    success = true;

exit_function:
    return success;
}
#endif /* (CONFIG_IEC61850_CONTROL_SERVICE == 1) */

static bool
createMmsServerCache(IedServer self)
{
    assert(self != NULL);

    bool success = false;

    int domain = 0;

    for (domain = 0; domain < self->mmsDevice->domainCount; domain++)
    {
        /* Install all top level MMS named variables (=Logical nodes) in the MMS server cache */
        MmsDomain* logicalDevice = self->mmsDevice->domains[domain];

        int i;

        for (i = 0; i < logicalDevice->namedVariablesCount; i++)
        {
            char* lnName = logicalDevice->namedVariables[i]->name;

            if (DEBUG_IED_SERVER)
                printf("IED_SERVER: Insert into cache %s - %s\n", logicalDevice->domainName, lnName);

            int fcCount = logicalDevice->namedVariables[i]->typeSpec.structure.elementCount;
            int j;

            for (j = 0; j < fcCount; j++)
            {
                MmsVariableSpecification* fcSpec = logicalDevice->namedVariables[i]->typeSpec.structure.elements[j];

                char* fcName = fcSpec->name;

#if (CONFIG_IEC61850_CONTROL_SERVICE == 1)
                if (strcmp(fcName, "CO") == 0)
                {
                    createControlObjects(self, logicalDevice, lnName, fcSpec, NULL);
                }
                else
#endif /* (CONFIG_IEC61850_CONTROL_SERVICE == 1) */

                if ((strcmp(fcName, "BR") != 0) && (strcmp(fcName, "RP") != 0)

#if (CONFIG_INCLUDE_GOOSE_SUPPORT == 1)
                        && (strcmp(fcName, "GO") != 0)
#endif

#if (CONFIG_IEC61850_SAMPLED_VALUES_SUPPORT == 1)
                        && (strcmp(fcName, "MS") != 0) && (strcmp(fcName, "US") != 0)
#endif

#if (CONFIG_IEC61850_LOG_SERVICE == 1)
                        && (strcmp(fcName, "LG") != 0)
#endif

                   )
                {
                    char variableName[65];

                    StringUtils_createStringInBuffer(variableName, 65, 3, lnName, "$", fcName);

                    MmsValue* defaultValue = MmsValue_newDefaultValue(fcSpec);

                    if (defaultValue == NULL)
                        goto exit_function;

                    if (DEBUG_IED_SERVER)
                        printf("IED_SERVER: Insert into cache %s - %s\n", logicalDevice->domainName, variableName);

                    MmsServer_insertIntoCache(self->mmsServer, logicalDevice, variableName, defaultValue);
                }
            }
        }
    }

    success = true;

exit_function:
    return success;
}

static void
installDefaultValuesForDataAttribute(IedServer self, LogicalDevice* ld, DataAttribute* dataAttribute,
        char* objectReference, int position, int idx, char* componentId, int compIdPos)
{
    if (dataAttribute->name)
    {
        if (idx == -1) {
            sprintf(objectReference + position, ".%s", dataAttribute->name);
        }
        else
        {
            if (compIdPos == 0)
                sprintf(componentId, "%s", dataAttribute->name);
            else
                sprintf(componentId + compIdPos, "$%s", dataAttribute->name);
        }
    }
    else
    {
        if (compIdPos == 0)
            componentId[0] = 0;
    }

    char mmsVariableName[65]; /* maximum size is 64 according to 61850-8-1 */

    MmsValue* value = dataAttribute->mmsValue;

    MmsMapping_createMmsVariableNameFromObjectReference(objectReference, dataAttribute->fc, mmsVariableName);

    char domainName[65];

    if (ld->ldName == NULL)
    {
        char ldInst[65];

        MmsMapping_getMmsDomainFromObjectReference(objectReference, ldInst);

        StringUtils_concatString(domainName, 65, self->model->name, ldInst);
    }
    else
    {
        StringUtils_copyStringMax(domainName, 65, ld->ldName);
    }

    MmsDomain* domain = MmsDevice_getDomain(self->mmsDevice, domainName);

    if (domain == NULL)
    {
        if (DEBUG_IED_SERVER)
            printf("Error domain (%s) not found for %s!\n", domainName, objectReference);
        return;
    }

    MmsValue* cacheValue = NULL;

    if (idx == -1)
        cacheValue = MmsServer_getValueFromCache(self->mmsServer, domain, mmsVariableName);
    else
        cacheValue = MmsServer_getValueFromCacheEx2(self->mmsServer, domain, mmsVariableName, idx, componentId);

    dataAttribute->mmsValue = cacheValue;

    if (value)
    {
        if (cacheValue)
            MmsValue_update(cacheValue, value);

        #if (DEBUG_IED_SERVER == 1)
            if (cacheValue == NULL) {
                printf("IED_SERVER: exception: invalid initializer for %s\n", mmsVariableName);
            }
        #endif

        MmsValue_delete(value);
    }

    int childPosition = strlen(objectReference);
    int childCompIdPos = strlen(componentId);

    if (dataAttribute->elementCount > 0)
    {
        int subIdx = 0;

        DataAttribute* subDataAttribute = (DataAttribute*) dataAttribute->firstChild;

        int childIdPos = childCompIdPos;

        while (subDataAttribute)
        {
            installDefaultValuesForDataAttribute(self, ld, subDataAttribute, objectReference, childPosition, subIdx, componentId, childIdPos);

            subIdx++;

            subDataAttribute = (DataAttribute*) subDataAttribute->sibling;
        }
    }
    else
    {
        DataAttribute* subDataAttribute = (DataAttribute*) dataAttribute->firstChild;

        int childIdPos = childCompIdPos;

        while (subDataAttribute)
        {
            installDefaultValuesForDataAttribute(self, ld, subDataAttribute, objectReference, childPosition, idx, componentId, childIdPos);

            subDataAttribute = (DataAttribute*) subDataAttribute->sibling;
        }
    }
}

static void
installDefaultValuesForDataObject(IedServer self, LogicalDevice* ld, DataObject* dataObject,
        char* objectReference, int position, int idx, char* componentId, int compIdPos)
{
    if (dataObject->elementCount > 0)
    {
        if (DEBUG_IED_SERVER)
            printf("IED_SERVER: DataObject %s is an array\n", dataObject->name);

        ModelNode* arrayElemNode = dataObject->firstChild;

        sprintf(objectReference + position, ".%s", dataObject->name);
        int childPosition = strlen(objectReference);

        int arrayIdx = 0;

        while (arrayElemNode)
        {
            installDefaultValuesForDataObject(self, ld, (DataObject*)arrayElemNode, objectReference, childPosition, arrayIdx, componentId, compIdPos);

            arrayIdx++;
            arrayElemNode = arrayElemNode->sibling;
        }

        return;
    }

    if (dataObject->arrayIndex == -1)
    {
        if (idx == -1)
        {
            sprintf(objectReference + position, ".%s", dataObject->name);
        }
        else
        {
            if (compIdPos == 0)
                sprintf(componentId, "%s", dataObject->name);
            else
                sprintf(componentId + compIdPos, "$%s", dataObject->name);
        }
    }

    if (compIdPos == 0)
        componentId[0] = 0;

    ModelNode* childNode = dataObject->firstChild;

    int childPosition = strlen(objectReference);
    int childCompIdPos = strlen(componentId);

    while (childNode)
    {
        if (childNode->modelType == DataObjectModelType)
        {
            installDefaultValuesForDataObject(self, ld, (DataObject*) childNode, objectReference, childPosition, idx, componentId, childCompIdPos);
        }
        else if (childNode->modelType == DataAttributeModelType)
        {
            installDefaultValuesForDataAttribute(self, ld, (DataAttribute*) childNode, objectReference, childPosition, idx, componentId, childCompIdPos);
        }

        childNode = childNode->sibling;
    }
}

static void
installDefaultValuesInCache(IedServer self)
{
    IedModel* model = self->model;

    char componentId[130];
    componentId[0] = 0;

    char objectReference[130];

    LogicalDevice* logicalDevice = model->firstChild;

    while (logicalDevice)
    {
        if (logicalDevice->ldName)
            sprintf(objectReference, "%s", logicalDevice->ldName);
        else
            sprintf(objectReference, "%s", logicalDevice->name);

        LogicalNode* logicalNode = (LogicalNode*) logicalDevice->firstChild;

        char* nodeReference = objectReference + strlen(objectReference);

        while (logicalNode)
        {
            sprintf(nodeReference, "/%s", logicalNode->name);

            DataObject* dataObject = (DataObject*) logicalNode->firstChild;

            int refPosition = strlen(objectReference);

            while (dataObject)
            {
                componentId[0] = 0;
                installDefaultValuesForDataObject(self, logicalDevice, dataObject, objectReference, refPosition, -1, componentId, 0);

                dataObject = (DataObject*) dataObject->sibling;
            }

            logicalNode = (LogicalNode*) logicalNode->sibling;
        }

        logicalDevice = (LogicalDevice*) logicalDevice->sibling;
    }
}

static void
updateDataSetsWithCachedValues(IedServer self)
{
    DataSet* dataSet = self->model->dataSets;

    int iedNameLength = strlen(self->model->name);

    if (iedNameLength <= 64)
    {
        while (dataSet)
        {
            DataSetEntry* dataSetEntry = dataSet->fcdas;

            while (dataSetEntry)
            {
                MmsDomain* domain = NULL;

                LogicalDevice* ld = IedModel_getDeviceByInst(self->model, dataSetEntry->logicalDeviceName);

                if (ld)
                {
                    if (ld->ldName)
                    {
                        domain = MmsDevice_getDomain(self->mmsDevice, ld->ldName);
                    }

                    if (domain == NULL)
                    {
                        char domainName[65];

                        StringUtils_concatString(domainName, 65, self->model->name, dataSetEntry->logicalDeviceName);

                        domain = MmsDevice_getDomain(self->mmsDevice, domainName);
                    }

                }
                else
                {
                    if (DEBUG_IED_SERVER)
                        printf("IED_SERVER: ERROR - LD %s not found\n", dataSetEntry->logicalDeviceName);
                }

                char variableName[66];
                variableName[0] = 0;

                StringUtils_appendString(variableName, 66, dataSetEntry->variableName);

                MmsVariableSpecification* typeSpec = NULL;

                MmsValue* value = MmsServer_getValueFromCacheEx(self->mmsServer, domain, variableName, &typeSpec);

                if (value == NULL)
                {
                    if (DEBUG_IED_SERVER)
                    {
                        printf("IED_SERVER: LD: %s dataset: %s : error cannot get value from cache for %s -> %s!\n",
                                dataSet->logicalDeviceName, dataSet->name,
                                dataSetEntry->logicalDeviceName,
                                dataSetEntry->variableName);
                    }
                }
                else
                {
                    /* check if array element */

                    if (dataSetEntry->index != -1)
                    {
                        if (typeSpec->type == MMS_ARRAY)
                        {
                            MmsValue* elementValue = MmsValue_getElement(value, dataSetEntry->index);

                            if (elementValue)
                            {
                                if (dataSetEntry->componentName)
                                {
                                    MmsVariableSpecification* elementType = typeSpec->typeSpec.array.elementTypeSpec;

                                    MmsValue* subElementValue = MmsVariableSpecification_getChildValue(elementType, elementValue, dataSetEntry->componentName);

                                    if (subElementValue)
                                    {
                                        dataSetEntry->value = subElementValue;
                                    }
                                    else
                                    {
                                        if (DEBUG_IED_SERVER)
                                            printf("IED_SERVER: ERROR - component %s of array element not found\n", dataSetEntry->componentName);
                                    }
                                }
                                else
                                {
                                    dataSetEntry->value = elementValue;
                                }
                            }
                            else
                            {
                                if (DEBUG_IED_SERVER)
                                    printf("IED_SERVER: ERROR - array element %i not found\n", dataSetEntry->index);
                            }
                        }
                        else
                        {
                            if (DEBUG_IED_SERVER)
                                printf("IED_SERVER: ERROR - variable %s/%s is not an array\n", dataSetEntry->logicalDeviceName, dataSetEntry->variableName);
                        }
                    }
                    else
                    {
                        dataSetEntry->value = value;
                    }
                }

                dataSetEntry = dataSetEntry->sibling;
            }

            dataSet = dataSet->sibling;
        }
    }
}

IedServer
IedServer_createWithConfig(IedModel* dataModel, TLSConfiguration tlsConfiguration, IedServerConfig serverConfiguration)
{
    IedServer self = (IedServer) GLOBAL_CALLOC(1, sizeof(struct sIedServer));

    if (self)
    {
        self->model = dataModel;

        self->running = false;
        self->localIpAddress = NULL;
        self->ignoreReadAccess = false;

#if (CONFIG_IEC61850_EDITION_1 == 1)
        self->edition = IEC_61850_EDITION_1;
#else
        self->edition = IEC_61850_EDITION_2;
#endif

#if (CONFIG_MMS_SERVER_CONFIG_SERVICES_AT_RUNTIME == 1)
        self->logServiceEnabled = true;

        if (serverConfiguration)
        {
            self->logServiceEnabled = serverConfiguration->enableLogService;
            self->edition = serverConfiguration->edition;
        }

#endif /* (CONFIG_MMS_SERVER_CONFIG_SERVICES_AT_RUNTIME == 1) */

#if (CONFIG_MMS_THREADLESS_STACK != 1)
        self->dataModelLock = Semaphore_create(1);
        self->clientConnectionsLock = Semaphore_create(1);
#endif /* (CONFIG_MMS_SERVER_CONFIG_SERVICES_AT_RUNTIME == 1) */

#if (CONFIG_IEC61850_REPORT_SERVICE == 1)
        if (serverConfiguration)
        {
            self->reportBufferSizeBRCBs = serverConfiguration->reportBufferSize;
            self->reportBufferSizeURCBs = serverConfiguration->reportBufferSizeURCBs;
            self->enableBRCBResvTms = serverConfiguration->enableResvTmsForBRCB;
            self->enableOwnerForRCB = serverConfiguration->enableOwnerForRCB;
            self->syncIntegrityReportTimes = serverConfiguration->syncIntegrityReportTimes;
            self->rcbSettingsWritable = serverConfiguration->reportSettingsWritable;
        }
        else
        {
            self->reportBufferSizeBRCBs = CONFIG_REPORTING_DEFAULT_REPORT_BUFFER_SIZE;
            self->reportBufferSizeURCBs = CONFIG_REPORTING_DEFAULT_REPORT_BUFFER_SIZE;
            self->enableOwnerForRCB = false;
            self->syncIntegrityReportTimes = false;
            self->rcbSettingsWritable = IEC61850_REPORTSETTINGS_RPT_ID +
                                        IEC61850_REPORTSETTINGS_BUF_TIME +
                                        IEC61850_REPORTSETTINGS_DATSET +
                                        IEC61850_REPORTSETTINGS_TRG_OPS +
                                        IEC61850_REPORTSETTINGS_OPT_FIELDS +
                                        IEC61850_REPORTSETTINGS_INTG_PD;
#if (CONFIG_IEC61850_BRCB_WITH_RESVTMS == 1)
            self->enableBRCBResvTms = true;
#else
            self->enableBRCBResvTms = false;
#endif
        }
#endif

#if (CONFIG_IEC61850_SETTING_GROUPS == 1)
        if (serverConfiguration)
        {
            self->enableEditSG = serverConfiguration->enableEditSG;
            self->hasSGCBResvTms = serverConfiguration->enableResvTmsForSGCB;
        }
        else
        {
            self->enableEditSG = true;
            self->hasSGCBResvTms = true;
        }
#endif

        self->mmsMapping = MmsMapping_create(dataModel, self);

        if (self->mmsMapping)
        {
            self->mmsDevice = MmsMapping_getMmsDeviceModel(self->mmsMapping);

            self->mmsServer = MmsServer_create(self->mmsDevice, tlsConfiguration);

#if (CONFIG_MMS_SERVER_CONFIG_SERVICES_AT_RUNTIME == 1)
            if (serverConfiguration)
            {
                MmsServer_enableFileService(self->mmsServer, serverConfiguration->enableFileService);
                MmsServer_enableDynamicNamedVariableListService(self->mmsServer, serverConfiguration->enableDynamicDataSetService);
                MmsServer_setMaxAssociationSpecificDataSets(self->mmsServer, serverConfiguration->maxAssociationSpecificDataSets);
                MmsServer_setMaxDomainSpecificDataSets(self->mmsServer, serverConfiguration->maxDomainSpecificDataSets);
                MmsServer_setMaxDataSetEntries(self->mmsServer, serverConfiguration->maxDataSetEntries);
                MmsServer_enableJournalService(self->mmsServer, serverConfiguration->enableLogService);
                MmsServer_setFilestoreBasepath(self->mmsServer, serverConfiguration->fileServiceBasepath);
                MmsServer_setMaxConnections(self->mmsServer, serverConfiguration->maxMmsConnections);
            }
#endif

            MmsMapping_setMmsServer(self->mmsMapping, self->mmsServer);

            MmsMapping_installHandlers(self->mmsMapping);

            createMmsServerCache(self);

            dataModel->initializer();

            installDefaultValuesInCache(self); /* This will also connect cached MmsValues to DataAttributes */

            updateDataSetsWithCachedValues(self);

            self->clientConnections = LinkedList_create();

            /* default write access policy allows access to SP, SE and SV FCDAs but denies access to DC and CF FCDAs */
            self->writeAccessPolicies = ALLOW_WRITE_ACCESS_SP | ALLOW_WRITE_ACCESS_SV | ALLOW_WRITE_ACCESS_SE;

            MmsMapping_initializeControlObjects(self->mmsMapping);

#if (CONFIG_IEC61850_REPORT_SERVICE == 1)
            Reporting_activateBufferedReports(self->mmsMapping);
#endif

#if (CONFIG_IEC61850_SETTING_GROUPS == 1)
            MmsMapping_configureSettingGroups(self->mmsMapping);
#endif

#if (CONFIG_INCLUDE_GOOSE_SUPPORT)
		    if (serverConfiguration) {
                MmsMapping_useIntegratedGoosePublisher(self->mmsMapping, serverConfiguration->useIntegratedGoosePublisher);
		    }
#endif

            IedServer_setTimeQuality(self, true, false, false, 10);
        }
        else
        {
            IedServer_destroy(self);
            self = NULL;
        }
    }

    return self;
}

IedServer
IedServer_create(IedModel* dataModel)
{
    return IedServer_createWithConfig(dataModel, NULL, NULL);
}

IedServer
IedServer_createWithTlsSupport(IedModel* dataModel, TLSConfiguration tlsConfiguration)
{
    return IedServer_createWithConfig(dataModel, tlsConfiguration, NULL);
}

void
IedServer_setRCBEventHandler(IedServer self, IedServer_RCBEventHandler handler, void* parameter)
{
    self->mmsMapping->rcbEventHandler = handler;
    self->mmsMapping->rcbEventHandlerParameter = parameter;
}

void
IedServer_destroy(IedServer self)
{
    if (self)
    {
    /* Stop server if running */
        if (self->running)
        {
#if (CONFIG_MMS_THREADLESS_STACK == 1)
            IedServer_stopThreadless(self);
#else
            IedServer_stop(self);
#endif
        }

#if ((CONFIG_MMS_SINGLE_THREADED == 1) && (CONFIG_MMS_THREADLESS_STACK == 0))

        if (self->serverThread)
            Thread_destroy(self->serverThread);

#endif

        MmsServer_destroy(self->mmsServer);

        if (self->localIpAddress)
            GLOBAL_FREEMEM(self->localIpAddress);

        if (self->mmsMapping)
            MmsMapping_destroy(self->mmsMapping);

        LinkedList_destroyDeep(self->clientConnections, (LinkedListValueDeleteFunction) private_ClientConnection_destroy);

#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_destroy(self->dataModelLock);
        Semaphore_destroy(self->clientConnectionsLock);
#endif

#if (CONFIG_IEC61850_SUPPORT_SERVER_IDENTITY == 1)

        if (self->vendorName)
            GLOBAL_FREEMEM(self->vendorName);

        if (self->modelName)
            GLOBAL_FREEMEM(self->modelName);

        if (self->revision)
            GLOBAL_FREEMEM(self->revision);
#endif /* (CONFIG_IEC61850_SUPPORT_SERVER_IDENTITY == 1) */

        GLOBAL_FREEMEM(self);
    }
}

void
IedServer_setAuthenticator(IedServer self, AcseAuthenticator authenticator, void* authenticatorParameter)
{
    MmsServer_setClientAuthenticator(self->mmsServer, authenticator, authenticatorParameter);
}

MmsServer
IedServer_getMmsServer(IedServer self)
{
    return self->mmsServer;
}

#if (CONFIG_MMS_THREADLESS_STACK != 1)
#if (CONFIG_MMS_SINGLE_THREADED == 1)
static void
singleThreadedServerThread(void* parameter)
{
    IedServer self = (IedServer) parameter;

    MmsMapping* mmsMapping = self->mmsMapping;

    bool running = true;

    if (DEBUG_IED_SERVER)
        printf("IED_SERVER: server thread started!\n");

    while (running)
    {
        MmsServer_handleIncomingMessages(self->mmsServer);

        IedServer_performPeriodicTasks(self);

        running = mmsMapping->reportThreadRunning;
    }

    if (DEBUG_IED_SERVER)
        printf("IED_SERVER: server thread finished!\n");
}
#endif /* (CONFIG_MMS_SINGLE_THREADED == 1) */
#endif /* (CONFIG_MMS_THREADLESS_STACK != 1) */

#if (CONFIG_MMS_THREADLESS_STACK != 1)
void
IedServer_start(IedServer self, int tcpPort)
{
    if (self->running == false)
    {
#if (CONFIG_MMS_SINGLE_THREADED == 1)
        MmsServer_startListeningThreadless(self->mmsServer, tcpPort);

        self->mmsMapping->reportThreadRunning = true;

        self->serverThread = Thread_create((ThreadExecutionFunction) singleThreadedServerThread, (void*) self, false);

        Thread_start(self->serverThread);
#else
        MmsServer_startListening(self->mmsServer, tcpPort);
        MmsMapping_startEventWorkerThread(self->mmsMapping);
#endif

        self->running = true;
    }
}
#endif

bool
IedServer_isRunning(IedServer self)
{
    return MmsServer_isRunning(self->mmsServer);
}

IedModel*
IedServer_getDataModel(IedServer self)
{
    return self->model;
}

#if (CONFIG_MMS_THREADLESS_STACK != 1)
void
IedServer_stop(IedServer self)
{
    if (self->running)
    {
        self->running = false;

        MmsMapping_stopEventWorkerThread(self->mmsMapping);

        Reporting_deactivateAllReports(self->mmsMapping);

#if (CONFIG_MMS_SINGLE_THREADED == 1)
        Thread_destroy(self->serverThread);
        self->serverThread = NULL;

        MmsServer_stopListeningThreadless(self->mmsServer);
#else
        MmsServer_stopListening(self->mmsServer);
#endif
    }
}
#endif /* (CONFIG_MMS_THREADLESS_STACK != 1) */

void
IedServer_setFilestoreBasepath(IedServer self, const char* basepath)
{
    /* simply pass to MMS server API */
    MmsServer_setFilestoreBasepath(self->mmsServer, basepath);
}

void
IedServer_setLocalIpAddress(IedServer self, const char* localIpAddress)
{
    if (self->localIpAddress)
        GLOBAL_FREEMEM(self->localIpAddress);

    self->localIpAddress = StringUtils_copyString(localIpAddress);

    MmsServer_setLocalIpAddress(self->mmsServer, self->localIpAddress);
}


void
IedServer_startThreadless(IedServer self, int tcpPort)
{
    if (self->running == false)
    {
        MmsServer_startListeningThreadless(self->mmsServer, tcpPort);
        self->running = true;
    }
}

int
IedServer_getNumberOfOpenConnections(IedServer self)
{
    return MmsServer_getConnectionCounter(self->mmsServer);
}

int
IedServer_waitReady(IedServer self, unsigned int timeoutMs)
{
   return MmsServer_waitReady(self->mmsServer, timeoutMs);
}

void
IedServer_processIncomingData(IedServer self)
{
    MmsServer_handleIncomingMessages(self->mmsServer);
}

bool
IedServer_addAccessPoint(IedServer self, const char* ipAddr, int tcpPort, TLSConfiguration tlsConfiguration)
{
    if (self->mmsServer)
    {
        return MmsServer_addAP(self->mmsServer, ipAddr, tcpPort, tlsConfiguration);
    }
    else
    {
        return false;
    }
}

void
IedServer_stopThreadless(IedServer self)
{
    if (self->running)
    {
        self->running = false;

        MmsServer_stopListeningThreadless(self->mmsServer);
    }
}

void
IedServer_lockDataModel(IedServer self)
{
    MmsServer_lockModel(self->mmsServer);

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_wait(self->mmsMapping->isModelLockedMutex);
#endif

    self->mmsMapping->isModelLocked = true;

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_post(self->mmsMapping->isModelLockedMutex);
#endif
}

void
IedServer_unlockDataModel(IedServer self)
{
#if (CONFIG_INCLUDE_GOOSE_SUPPORT == 1)
    /* check if GOOSE messages have to be sent */
    GOOSE_sendPendingEvents(self->mmsMapping);
#endif /* (CONFIG_INCLUDE_GOOSE_SUPPORT == 1) */

    /* check if reports have to be sent! */
    Reporting_processReportEventsAfterUnlock(self->mmsMapping);

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_wait(self->mmsMapping->isModelLockedMutex);
#endif

    MmsServer_unlockModel(self->mmsServer);

    self->mmsMapping->isModelLocked = false;

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_post(self->mmsMapping->isModelLockedMutex);
#endif
}

#if (CONFIG_IEC61850_CONTROL_SERVICE == 1)
static ControlObject*
lookupControlObject(IedServer self, DataObject* node)
{
    char objectReference[130];

    ModelNode_getObjectReference((ModelNode*) node, objectReference);

    char* separator = strchr(objectReference, '/');

    *separator = 0;

    MmsDomain* domain = MmsDevice_getDomain(self->mmsDevice, objectReference);

    char* lnName = separator + 1;

    separator = strchr(lnName, '.');

    assert(separator != NULL);

    *separator = 0;

    char* objectName = separator + 1;

    StringUtils_replace(objectName, '.', '$');

    if (DEBUG_IED_SERVER)
        printf("IED_SERVER: looking for control object: %s\n", objectName);

    ControlObject* controlObject = MmsMapping_getControlObject(self->mmsMapping, domain,
            lnName, objectName);

    if (controlObject)
        controlObject->dataObject = node;

    return controlObject;
}

void
IedServer_setControlHandler(
        IedServer self,
        DataObject* node,
        ControlHandler listener,
        void* parameter)
{
    ControlObject* controlObject = lookupControlObject(self, node);

    if (controlObject) {
        ControlObject_installListener(controlObject, listener, parameter);
        if (DEBUG_IED_SERVER)
            printf("IED_SERVER: Installed control handler for %s!\n", node->name);
    }
    else
        if (DEBUG_IED_SERVER)
            printf("IED_SERVER: Failed to install control handler!\n");
}

void
IedServer_setPerformCheckHandler(IedServer self, DataObject* node, ControlPerformCheckHandler handler, void* parameter)
{
    ControlObject* controlObject = lookupControlObject(self, node);

    if (controlObject)
        ControlObject_installCheckHandler(controlObject, handler, parameter);
}

void
IedServer_setWaitForExecutionHandler(IedServer self, DataObject* node, ControlWaitForExecutionHandler handler,
        void* parameter)
{
    ControlObject* controlObject = lookupControlObject(self, node);

    if (controlObject)
        ControlObject_installWaitForExecutionHandler(controlObject, handler, parameter);
}

void
IedServer_setSelectStateChangedHandler(IedServer self, DataObject* node, ControlSelectStateChangedHandler handler,
        void* parameter)
{
    ControlObject* controlObject = lookupControlObject(self, node);

    if (controlObject)
        ControlObject_installSelectStateChangedHandler(controlObject, handler, parameter);
}

void
IedServer_updateCtlModel(IedServer self, DataObject* ctlObject, ControlModel value)
{
    ControlObject* controlObject = lookupControlObject(self, ctlObject);

    if (controlObject != NULL)
        ControlObject_updateControlModel(controlObject, value, ctlObject);
}

#endif /* (CONFIG_IEC61850_CONTROL_SERVICE == 1) */

#if (CONFIG_IEC61850_SAMPLED_VALUES_SUPPORT == 1)

void
IedServer_setSVCBHandler(IedServer self, SVControlBlock* svcb, SVCBEventHandler handler, void* parameter)
{
    LIBIEC61850_SV_setSVCBHandler(self->mmsMapping, svcb, handler, parameter);
}

#endif /* (CONFIG_IEC61850_SAMPLED_VALUES_SUPPORT == 1) */

#if (CONFIG_INCLUDE_GOOSE_SUPPORT == 1)

void
IedServer_setGoCBHandler(IedServer self, GoCBEventHandler handler, void* parameter)
{
    self->mmsMapping->goCbHandler = handler;
    self->mmsMapping->goCbHandlerParameter = parameter;
}

#endif /* (CONFIG_INCLUDE_GOOSE_SUPPORT == 1) */

MmsValue*
IedServer_getAttributeValue(IedServer self, DataAttribute* dataAttribute)
{
    (void)self;
    return dataAttribute->mmsValue;
}

bool
IedServer_getBooleanAttributeValue(IedServer self, const DataAttribute* dataAttribute)
{
    (void)self;

    assert(self != NULL);
    assert(dataAttribute != NULL);
    assert(dataAttribute->mmsValue != NULL);
    assert(MmsValue_getType(dataAttribute->mmsValue) == MMS_BOOLEAN);

    return MmsValue_getBoolean(dataAttribute->mmsValue);
}

int32_t
IedServer_getInt32AttributeValue(IedServer self, const DataAttribute* dataAttribute)
{
    (void)self;

    assert(self != NULL);
    assert(dataAttribute != NULL);
    assert(dataAttribute->mmsValue != NULL);
    assert((MmsValue_getType(dataAttribute->mmsValue) == MMS_INTEGER) ||
            (MmsValue_getType(dataAttribute->mmsValue) == MMS_UNSIGNED));

    return MmsValue_toInt32(dataAttribute->mmsValue);
}

int64_t
IedServer_getInt64AttributeValue(IedServer self, const DataAttribute* dataAttribute)
{
    (void)self;

    assert(self != NULL);
    assert(dataAttribute != NULL);
    assert(dataAttribute->mmsValue != NULL);
    assert((MmsValue_getType(dataAttribute->mmsValue) == MMS_INTEGER) ||
            (MmsValue_getType(dataAttribute->mmsValue) == MMS_UNSIGNED));

    return MmsValue_toInt64(dataAttribute->mmsValue);
}

uint32_t
IedServer_getUInt32AttributeValue(IedServer self, const DataAttribute* dataAttribute)
{
    (void)self;

    assert(self != NULL);
    assert(dataAttribute != NULL);
    assert(dataAttribute->mmsValue != NULL);
    assert((MmsValue_getType(dataAttribute->mmsValue) == MMS_INTEGER) ||
            (MmsValue_getType(dataAttribute->mmsValue) == MMS_UNSIGNED));

    return MmsValue_toUint32(dataAttribute->mmsValue);
}

float
IedServer_getFloatAttributeValue(IedServer self, const DataAttribute* dataAttribute)
{
    (void)self;

    assert(self != NULL);
    assert(dataAttribute != NULL);
    assert(dataAttribute->mmsValue != NULL);
    assert(MmsValue_getType(dataAttribute->mmsValue) == MMS_FLOAT);

    return MmsValue_toFloat(dataAttribute->mmsValue);
}

uint64_t
IedServer_getUTCTimeAttributeValue(IedServer self, const DataAttribute* dataAttribute)
{
    (void)self;

    assert(self != NULL);
    assert(dataAttribute != NULL);
    assert(dataAttribute->mmsValue != NULL);
    assert(MmsValue_getType(dataAttribute->mmsValue) == MMS_UTC_TIME);

    return MmsValue_getUtcTimeInMs(dataAttribute->mmsValue);
}

uint32_t
IedServer_getBitStringAttributeValue(IedServer self, const DataAttribute* dataAttribute)
{
    (void)self;

    assert(self != NULL);
    assert(dataAttribute != NULL);
    assert(dataAttribute->mmsValue != NULL);
    assert(MmsValue_getType(dataAttribute->mmsValue) == MMS_BIT_STRING);
    assert(MmsValue_getBitStringSize(dataAttribute->mmsValue) < 33);

    return MmsValue_getBitStringAsInteger(dataAttribute->mmsValue);
}

const char*
IedServer_getStringAttributeValue(IedServer self, const DataAttribute* dataAttribute)
{
    (void)self;

    assert(self != NULL);
    assert(dataAttribute != NULL);
    assert(dataAttribute->mmsValue != NULL);
    assert(MmsValue_getType(dataAttribute->mmsValue) == MMS_VISIBLE_STRING);

    return MmsValue_toString(dataAttribute->mmsValue);
}

static inline void
checkForUpdateTrigger(IedServer self, DataAttribute* dataAttribute)
{
#if ((CONFIG_IEC61850_REPORT_SERVICE == 1) || (CONFIG_IEC61850_LOG_SERVICE == 1))
    if (dataAttribute->triggerOptions & TRG_OPT_DATA_UPDATE)
    {
#if (CONFIG_IEC61850_REPORT_SERVICE == 1)
        MmsMapping_triggerReportObservers(self->mmsMapping, dataAttribute->mmsValue,
                REPORT_CONTROL_VALUE_UPDATE);
#endif

#if (CONFIG_IEC61850_LOG_SERVICE == 1)
        MmsMapping_triggerLogging(self->mmsMapping, dataAttribute->mmsValue,
                LOG_CONTROL_VALUE_UPDATE);
#endif

    }
#endif /* ((CONFIG_IEC61850_REPORT_SERVICE == 1) || (CONFIG_IEC61850_LOG_SERVICE == 1)) */
}

static inline void
checkForChangedTriggers(IedServer self, DataAttribute* dataAttribute)
{
#if (CONFIG_IEC61850_REPORT_SERVICE == 1) || (CONFIG_INCLUDE_GOOSE_SUPPORT == 1)
    if (dataAttribute->triggerOptions & TRG_OPT_DATA_CHANGED) {

#if (CONFIG_IEC61850_L2_GOOSE == 1 || CONFIG_IEC61850_R_GOOSE == 1)
        MmsMapping_triggerGooseObservers(self->mmsMapping, dataAttribute->mmsValue);
#endif /* (CONFIG_IEC61850_L2_GOOSE == 1 || CONFIG_IEC61850_R_GOOSE == 1) */

#if (CONFIG_IEC61850_REPORT_SERVICE == 1)
        MmsMapping_triggerReportObservers(self->mmsMapping, dataAttribute->mmsValue,
                REPORT_CONTROL_VALUE_CHANGED);
#endif

#if (CONFIG_IEC61850_LOG_SERVICE == 1)
        MmsMapping_triggerLogging(self->mmsMapping, dataAttribute->mmsValue,
                LOG_CONTROL_VALUE_CHANGED);
#endif
    }

    else if (dataAttribute->triggerOptions & TRG_OPT_QUALITY_CHANGED) {

#if (CONFIG_IEC61850_L2_GOOSE == 1 || CONFIG_IEC61850_R_GOOSE == 1)
        MmsMapping_triggerGooseObservers(self->mmsMapping, dataAttribute->mmsValue);
#endif /* (CONFIG_IEC61850_L2_GOOSE == 1 || CONFIG_IEC61850_R_GOOSE == 1) */

#if (CONFIG_IEC61850_REPORT_SERVICE == 1)
        MmsMapping_triggerReportObservers(self->mmsMapping, dataAttribute->mmsValue,
                REPORT_CONTROL_QUALITY_CHANGED);
#endif

#if (CONFIG_IEC61850_LOG_SERVICE == 1)
        MmsMapping_triggerLogging(self->mmsMapping, dataAttribute->mmsValue,
                LOG_CONTROL_QUALITY_CHANGED);
#endif

    }
#endif /* (CONFIG_IEC61850_REPORT_SERVICE== 1) || (CONFIG_INCLUDE_GOOSE_SUPPORT == 1) */
}

void
IedServer_updateAttributeValue(IedServer self, DataAttribute* dataAttribute, MmsValue* value)
{
    assert(self != NULL);
    assert(dataAttribute != NULL);
    assert(MmsValue_getType(dataAttribute->mmsValue) == MmsValue_getType(value));

    if (MmsValue_equals(dataAttribute->mmsValue, value) == false)
    {
        if (dataAttribute->type == IEC61850_BOOLEAN)
        {
            /* Special treatment because of transient option */
            IedServer_updateBooleanAttributeValue(self, dataAttribute, MmsValue_getBoolean(value));
        }
        else
        {
#if (CONFIG_MMS_THREADLESS_STACK != 1)
            Semaphore_wait(self->dataModelLock);
#endif

            MmsValue_update(dataAttribute->mmsValue, value);

#if (CONFIG_MMS_THREADLESS_STACK != 1)
            Semaphore_post(self->dataModelLock);
#endif

            checkForChangedTriggers(self, dataAttribute);
        }
    }

    checkForUpdateTrigger(self, dataAttribute);
}

void
IedServer_updateFloatAttributeValue(IedServer self, DataAttribute* dataAttribute, float value)
{
    assert(dataAttribute != NULL);
    assert(MmsValue_getType(dataAttribute->mmsValue) == MMS_FLOAT);
    assert(self != NULL);

    float currentValue = MmsValue_toFloat(dataAttribute->mmsValue);

    if (currentValue != value)
    {
#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_wait(self->dataModelLock);
#endif
        MmsValue_setFloat(dataAttribute->mmsValue, value);
#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_post(self->dataModelLock);
#endif
        checkForChangedTriggers(self, dataAttribute);
    }

    checkForUpdateTrigger(self, dataAttribute);
}

void
IedServer_updateInt32AttributeValue(IedServer self, DataAttribute* dataAttribute, int32_t value)
{
    assert(dataAttribute != NULL);
    assert(MmsValue_getType(dataAttribute->mmsValue) == MMS_INTEGER);
    assert(self != NULL);

    int32_t currentValue = MmsValue_toInt32(dataAttribute->mmsValue);

    if (currentValue != value)
    {
#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_wait(self->dataModelLock);
#endif
        MmsValue_setInt32(dataAttribute->mmsValue, value);
#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_post(self->dataModelLock);
#endif

        checkForChangedTriggers(self, dataAttribute);
    }

    checkForUpdateTrigger(self, dataAttribute);
}

void
IedServer_updateDbposValue(IedServer self, DataAttribute* dataAttribute, Dbpos value)
{
    Dbpos currentValue = Dbpos_fromMmsValue(dataAttribute->mmsValue);

    if (currentValue != value)
    {
#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_wait(self->dataModelLock);
#endif
        Dbpos_toMmsValue(dataAttribute->mmsValue, value);
#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_post(self->dataModelLock);
#endif

        checkForChangedTriggers(self, dataAttribute);
    }

    checkForUpdateTrigger(self, dataAttribute);
}

void
IedServer_updateInt64AttributeValue(IedServer self, DataAttribute* dataAttribute, int64_t value)
{
    assert(dataAttribute != NULL);
    assert(MmsValue_getType(dataAttribute->mmsValue) == MMS_INTEGER);
    assert(self != NULL);

    int64_t currentValue = MmsValue_toInt64(dataAttribute->mmsValue);

    if (currentValue != value)
    {
#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_wait(self->dataModelLock);
#endif
        MmsValue_setInt64(dataAttribute->mmsValue, value);
#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_post(self->dataModelLock);
#endif

        checkForChangedTriggers(self, dataAttribute);
    }

    checkForUpdateTrigger(self, dataAttribute);
}

void
IedServer_updateUnsignedAttributeValue(IedServer self, DataAttribute* dataAttribute, uint32_t value)
{
    assert(dataAttribute != NULL);
    assert(MmsValue_getType(dataAttribute->mmsValue) == MMS_UNSIGNED);
    assert(self != NULL);

    uint32_t currentValue = MmsValue_toUint32(dataAttribute->mmsValue);

    if (currentValue != value)
    {
#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_wait(self->dataModelLock);
#endif
        MmsValue_setUint32(dataAttribute->mmsValue, value);
#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_post(self->dataModelLock);
#endif

        checkForChangedTriggers(self, dataAttribute);
    }

    checkForUpdateTrigger(self, dataAttribute);
}

void
IedServer_updateBitStringAttributeValue(IedServer self, DataAttribute* dataAttribute, uint32_t value)
{
    assert(dataAttribute != NULL);
    assert(MmsValue_getType(dataAttribute->mmsValue) == MMS_BIT_STRING);
    assert(self != NULL);

    uint32_t currentValue = MmsValue_getBitStringAsInteger(dataAttribute->mmsValue);

    if (currentValue != value)
    {
#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_wait(self->dataModelLock);
#endif
        MmsValue_setBitStringFromInteger(dataAttribute->mmsValue, value);
#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_post(self->dataModelLock);
#endif

        checkForChangedTriggers(self, dataAttribute);
    }

    checkForUpdateTrigger(self, dataAttribute);
}

void
IedServer_updateBooleanAttributeValue(IedServer self, DataAttribute* dataAttribute, bool value)
{
    assert(self != NULL);
    assert(dataAttribute != NULL);
    assert(MmsValue_getType(dataAttribute->mmsValue) == MMS_BOOLEAN);

    bool currentValue = MmsValue_getBoolean(dataAttribute->mmsValue);

    if (currentValue != value)
    {
        bool callCheckTriggers = true;

        if (dataAttribute->triggerOptions & TRG_OPT_TRANSIENT)
        {
            if (currentValue == true)
                callCheckTriggers = false;
        }

#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_wait(self->dataModelLock);
#endif
        MmsValue_setBoolean(dataAttribute->mmsValue, value);
#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_post(self->dataModelLock);
#endif

        if (callCheckTriggers)
            checkForChangedTriggers(self, dataAttribute);
    }

    checkForUpdateTrigger(self, dataAttribute);
}

void
IedServer_updateVisibleStringAttributeValue(IedServer self, DataAttribute* dataAttribute, char *value)
{
    assert(dataAttribute != NULL);
    assert(MmsValue_getType(dataAttribute->mmsValue) == MMS_VISIBLE_STRING);
    assert(self != NULL);

    const char* currentValue = MmsValue_toString(dataAttribute->mmsValue);

    if (strcmp(currentValue, value))
    {
#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_wait(self->dataModelLock);
#endif
        MmsValue_setVisibleString(dataAttribute->mmsValue, value);
#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_post(self->dataModelLock);
#endif

        checkForChangedTriggers(self, dataAttribute);
    }

    checkForUpdateTrigger(self, dataAttribute);
}

void
IedServer_updateUTCTimeAttributeValue(IedServer self, DataAttribute* dataAttribute, uint64_t value)
{
    assert(dataAttribute != NULL);
    assert(MmsValue_getType(dataAttribute->mmsValue) == MMS_UTC_TIME);
    assert(self != NULL);

    uint64_t currentValue = MmsValue_getUtcTimeInMs(dataAttribute->mmsValue);

    if (currentValue != value)
    {
#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_wait(self->dataModelLock);
#endif
        MmsValue_setUtcTimeMsEx(dataAttribute->mmsValue, value, self->timeQuality);
#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_post(self->dataModelLock);
#endif

        checkForChangedTriggers(self, dataAttribute);
    }

    checkForUpdateTrigger(self, dataAttribute);
}

void
IedServer_updateTimestampAttributeValue(IedServer self, DataAttribute* dataAttribute, Timestamp* timestamp)
{
    assert(dataAttribute != NULL);
    assert(MmsValue_getType(dataAttribute->mmsValue) == MMS_UTC_TIME);
    assert(self != NULL);

    if (memcmp(dataAttribute->mmsValue->value.utcTime, timestamp->val, 8))
    {
#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_wait(self->dataModelLock);
#endif
        MmsValue_setUtcTimeByBuffer(dataAttribute->mmsValue, timestamp->val);
#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_post(self->dataModelLock);
#endif

        checkForChangedTriggers(self, dataAttribute);
    }

    checkForUpdateTrigger(self, dataAttribute);
}

void
IedServer_updateQuality(IedServer self, DataAttribute* dataAttribute, Quality quality)
{
    assert(strcmp(dataAttribute->name, "q") == 0);
    assert(MmsValue_getType(dataAttribute->mmsValue) == MMS_BIT_STRING);
    assert(MmsValue_getBitStringSize(dataAttribute->mmsValue) >= 12);
    assert(MmsValue_getBitStringSize(dataAttribute->mmsValue) <= 15);

    uint32_t oldQuality = MmsValue_getBitStringAsInteger(dataAttribute->mmsValue);

    if (oldQuality != (uint32_t) quality)
    {
#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_wait(self->dataModelLock);
#endif
        MmsValue_setBitStringFromInteger(dataAttribute->mmsValue, (uint32_t) quality);

#if (CONFIG_MMS_THREADLESS_STACK != 1)
        Semaphore_post(self->dataModelLock);
#endif

#if (CONFIG_IEC61850_L2_GOOSE == 1 || CONFIG_IEC61850_R_GOOSE == 1)
        MmsMapping_triggerGooseObservers(self->mmsMapping, dataAttribute->mmsValue);
#endif /* (CONFIG_IEC61850_L2_GOOSE == 1 || CONFIG_IEC61850_R_GOOSE == 1) */

#if (CONFIG_IEC61850_REPORT_SERVICE == 1)
        if (dataAttribute->triggerOptions & TRG_OPT_QUALITY_CHANGED)
            MmsMapping_triggerReportObservers(self->mmsMapping, dataAttribute->mmsValue,
                                REPORT_CONTROL_QUALITY_CHANGED);
#endif

#if (CONFIG_IEC61850_LOG_SERVICE == 1)
        if (dataAttribute->triggerOptions & TRG_OPT_QUALITY_CHANGED)
            MmsMapping_triggerLogging(self->mmsMapping, dataAttribute->mmsValue,
                    LOG_CONTROL_QUALITY_CHANGED);
#endif

    }
}

void
IedServer_useGooseVlanTag(IedServer self, LogicalNode* ln, const char* gcbName, bool useVlanTag)
{
#if (CONFIG_INCLUDE_GOOSE_SUPPORT == 1)
    MmsMapping_useGooseVlanTag(self->mmsMapping, ln, gcbName, useVlanTag);
#endif /* (CONFIG_INCLUDE_GOOSE_SUPPORT == 1) */
}

void
IedServer_setGooseInterfaceIdEx(IedServer self, LogicalNode* ln, const char* gcbName, const char* interfaceId)
{
#if (CONFIG_INCLUDE_GOOSE_SUPPORT == 1)
    MmsMapping_setGooseInterfaceId(self->mmsMapping, ln, gcbName, interfaceId);
#endif /* (CONFIG_INCLUDE_GOOSE_SUPPORT == 1) */
}

void
IedServer_enableGoosePublishing(IedServer self)
{
#if (CONFIG_INCLUDE_GOOSE_SUPPORT == 1)
    MmsMapping_enableGoosePublishing(self->mmsMapping);
#endif /* (CONFIG_INCLUDE_GOOSE_SUPPORT == 1) */
}

void
IedServer_disableGoosePublishing(IedServer self)
{
#if (CONFIG_INCLUDE_GOOSE_SUPPORT == 1)
    MmsMapping_disableGoosePublishing(self->mmsMapping);
#endif /* (CONFIG_INCLUDE_GOOSE_SUPPORT == 1) */
}

void
IedServer_setWriteAccessPolicy(IedServer self, FunctionalConstraint fc, AccessPolicy policy)
{
    if (policy == ACCESS_POLICY_ALLOW)
    {
        switch (fc)
        {
        case IEC61850_FC_DC:
            self->writeAccessPolicies |= ALLOW_WRITE_ACCESS_DC;
            break;
        case IEC61850_FC_CF:
            self->writeAccessPolicies |= ALLOW_WRITE_ACCESS_CF;
            break;
        case IEC61850_FC_SP:
            self->writeAccessPolicies |= ALLOW_WRITE_ACCESS_SP;
            break;
        case IEC61850_FC_SV:
            self->writeAccessPolicies |= ALLOW_WRITE_ACCESS_SV;
            break;
        case IEC61850_FC_SE:
            self->writeAccessPolicies |= ALLOW_WRITE_ACCESS_SE;
            break;
        default: /* ignore - request is invalid */
            break;
        }
    }
    else
    {
        switch (fc)
        {
        case IEC61850_FC_DC:
            self->writeAccessPolicies &= ~ALLOW_WRITE_ACCESS_DC;
            break;
        case IEC61850_FC_CF:
            self->writeAccessPolicies &= ~ALLOW_WRITE_ACCESS_CF;
            break;
        case IEC61850_FC_SP:
            self->writeAccessPolicies &= ~ALLOW_WRITE_ACCESS_SP;
            break;
        case IEC61850_FC_SV:
            self->writeAccessPolicies &= ~ALLOW_WRITE_ACCESS_SV;
            break;
        case IEC61850_FC_SE:
            self->writeAccessPolicies &= ~ALLOW_WRITE_ACCESS_SE;
            break;
        default: /* ignore - request is invalid */
            break;
        }
    }
}

void
IedServer_handleWriteAccess(IedServer self, DataAttribute* dataAttribute, WriteAccessHandler handler, void* parameter)
{
    if (dataAttribute == NULL)
    {
        if (DEBUG_IED_SERVER)
            printf("IED_SERVER: IedServer_handleWriteAccess - dataAttribute == NULL!\n");
    }
    else
    {
        MmsMapping_installWriteAccessHandler(self->mmsMapping, dataAttribute, handler, parameter);
    }
}

void
IedServer_handleWriteAccessForComplexAttribute(IedServer self, DataAttribute* dataAttribute, WriteAccessHandler handler, void* parameter)
{
    if (dataAttribute == NULL)
    {
        if (DEBUG_IED_SERVER)
            printf("IED_SERVER: IedServer_handleWriteAccessForComplexAttribute - dataAttribute == NULL!\n");
    }
    else
    {
        MmsMapping_installWriteAccessHandler(self->mmsMapping, dataAttribute, handler, parameter);

        DataAttribute* subDa = (DataAttribute*) dataAttribute->firstChild;

        while (subDa)
        {
            IedServer_handleWriteAccessForComplexAttribute(self, subDa, handler, parameter);

            subDa = (DataAttribute*) subDa->sibling;
        }
    }
}

void
IedServer_handleWriteAccessForDataObject(IedServer self, DataObject* dataObject, FunctionalConstraint fc, WriteAccessHandler handler, void* parameter)
{
    if (dataObject == NULL)
    {
        if (DEBUG_IED_SERVER)
            printf("IED_SERVER: IedServer_handlerWriteAccessForDataObject - dataObject == NULL!\n");
    }
    else
    {
        ModelNode* childElement = dataObject->firstChild;

        while (childElement)
        {
            if (childElement->modelType == DataAttributeModelType)
            {
                DataAttribute* dataAttribute = (DataAttribute*) childElement;

                if (dataAttribute->fc == fc)
                {
                    IedServer_handleWriteAccessForComplexAttribute(self, dataAttribute, handler, parameter);
                }
            }
            else if (childElement->modelType == DataObjectModelType)
            {
                IedServer_handleWriteAccessForDataObject(self, (DataObject*) childElement, fc, handler, parameter);
            }

            childElement = childElement->sibling;
        }
    }
}

void
IedServer_setReadAccessHandler(IedServer self, ReadAccessHandler handler, void* parameter)
{
    MmsMapping_installReadAccessHandler(self->mmsMapping, handler, parameter);
}

void
IedServer_setConnectionIndicationHandler(IedServer self, IedConnectionIndicationHandler handler, void* parameter)
{
    MmsMapping_setConnectionIndicationHandler(self->mmsMapping, handler, parameter);
}

MmsValue*
IedServer_getFunctionalConstrainedData(IedServer self, DataObject* dataObject, FunctionalConstraint fc)
{
    char buffer[128]; /* buffer for variable name string */
    char* currentStart = buffer + 127;
    currentStart[0] = 0;
    MmsValue* value = NULL;

    int nameLen;

    while (dataObject->modelType == DataObjectModelType)
    {
        nameLen = strlen(dataObject->name);
        currentStart -= nameLen;
        memcpy(currentStart, dataObject->name, nameLen);
        currentStart--;
        *currentStart = '$';

        if (dataObject->parent->modelType == DataObjectModelType)
            dataObject = (DataObject*) dataObject->parent;
        else
            break;
    }

    char* fcString = FunctionalConstraint_toString(fc);

    currentStart--;
    *currentStart = fcString[1];
    currentStart--;
    *currentStart = fcString[0];
    currentStart--;
    *currentStart = '$';

    LogicalNode* ln = (LogicalNode*) dataObject->parent;

    nameLen = strlen(ln->name);

    currentStart -= nameLen;
    memcpy(currentStart, ln->name, nameLen);

    LogicalDevice* ld = (LogicalDevice*) ln->parent;

    char domainName[65];

    if ((strlen(self->model->name) + strlen(ld->name)) > 64)
    {
        if (DEBUG_IED_SERVER)
            printf("IED_SERVER: LD name too long!\n");

        goto exit_function;
    }

    StringUtils_concatString(domainName, 65, self->model->name, ld->name);

    MmsDomain* domain = MmsDevice_getDomain(self->mmsDevice, domainName);

    if (domain == NULL)
    {
        if (DEBUG_IED_SERVER)
            printf("IED_SERVER: internal error - domain does not exist!\n");

        goto exit_function;
    }

    value = MmsServer_getValueFromCache(self->mmsServer, domain, currentStart);

exit_function:
    return value;
}

void
IedServer_changeActiveSettingGroup(IedServer self, SettingGroupControlBlock* sgcb, uint8_t newActiveSg)
{
#if (CONFIG_IEC61850_SETTING_GROUPS == 1)
    MmsMapping_changeActiveSettingGroup(self->mmsMapping, sgcb, newActiveSg);
#endif
}

uint8_t
IedServer_getActiveSettingGroup(IedServer self, SettingGroupControlBlock* sgcb)
{
    (void)self;

    return sgcb->actSG;
}

void
IedServer_setActiveSettingGroupChangedHandler(IedServer self, SettingGroupControlBlock* sgcb,
        ActiveSettingGroupChangedHandler handler, void* parameter)
{
#if (CONFIG_IEC61850_SETTING_GROUPS == 1)
    MmsMapping_setSgChangedHandler(self->mmsMapping, sgcb, handler, parameter);
#endif
}

void
IedServer_setEditSettingGroupChangedHandler(IedServer self, SettingGroupControlBlock* sgcb,
        EditSettingGroupChangedHandler handler, void* parameter)
{
#if (CONFIG_IEC61850_SETTING_GROUPS == 1)
    MmsMapping_setEditSgChangedHandler(self->mmsMapping, sgcb, handler, parameter);
#endif
}

void
IedServer_setEditSettingGroupConfirmationHandler(IedServer self, SettingGroupControlBlock* sgcb,
        EditSettingGroupConfirmationHandler handler, void* parameter)
{
#if (CONFIG_IEC61850_SETTING_GROUPS == 1)
    MmsMapping_setConfirmEditSgHandler(self->mmsMapping, sgcb, handler, parameter);
#endif
}

void
IedServer_setLogStorage(IedServer self, const char* logRef, LogStorage logStorage)
{
#if (CONFIG_IEC61850_LOG_SERVICE == 1)
    MmsMapping_setLogStorage(self->mmsMapping, logRef, logStorage);
#else
    (void)self;
    (void)logRef;
    (void)logStorage;
#endif
}

void
IedServer_setServerIdentity(IedServer self, const char* vendor, const char* model, const char* revision)
{
#if (CONFIG_IEC61850_SUPPORT_SERVER_IDENTITY == 1)

    if (self->vendorName)
        GLOBAL_FREEMEM(self->vendorName);

    if (self->modelName)
        GLOBAL_FREEMEM(self->modelName);

    if (self->revision)
        GLOBAL_FREEMEM(self->revision);

    if (vendor)
    	self->vendorName = StringUtils_copyString(vendor);

    if (model)
    	self->modelName = StringUtils_copyString(model);

    if (revision)
    	self->revision = StringUtils_copyString(revision);

    MmsServer_setServerIdentity(self->mmsServer, self->vendorName, self->modelName, self->revision);
#endif
}

ClientConnection
private_IedServer_getClientConnectionByHandle(IedServer self, void* serverConnectionHandle)
{
#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_wait(self->clientConnectionsLock);
#endif

    LinkedList element = LinkedList_getNext(self->clientConnections);
    ClientConnection matchingConnection = NULL;

    while (element != NULL)
    {
        ClientConnection connection = (ClientConnection) element->data;

        if (private_ClientConnection_getServerConnectionHandle(connection) == serverConnectionHandle)
        {
            matchingConnection = connection;
            break;
        }

        element = LinkedList_getNext(element);
    }

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_post(self->clientConnectionsLock);
#endif

    return matchingConnection;
}

void
private_IedServer_addNewClientConnection(IedServer self, ClientConnection newClientConnection)
{
#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_wait(self->clientConnectionsLock);
#endif

    LinkedList_add(self->clientConnections, (void*) newClientConnection);

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_post(self->clientConnectionsLock);
#endif
}

void
private_IedServer_removeClientConnection(IedServer self, ClientConnection clientConnection)
{
#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_wait(self->clientConnectionsLock);
#endif

    LinkedList_remove(self->clientConnections, clientConnection);

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_post(self->clientConnectionsLock);
#endif
}

void
IedServer_setGooseInterfaceId(IedServer self, const char* interfaceId)
{
#if (CONFIG_INCLUDE_GOOSE_SUPPORT == 1)
    self->mmsMapping->gooseInterfaceId = StringUtils_copyString(interfaceId);
#endif
}

void
IedServer_setTimeQuality(IedServer self, bool leapSecondKnown, bool clockFailure, bool clockNotSynchronized, int subsecondPrecision)
{
    uint8_t timeQuality = 0;

    if (clockNotSynchronized)
        timeQuality += 0x20;

    if (clockFailure)
        timeQuality += 0x40;

    if (leapSecondKnown)
        timeQuality += 0x80;

    timeQuality += (subsecondPrecision & 0x1f);

    self->timeQuality = timeQuality;
}

void
IedServer_ignoreClientRequests(IedServer self, bool enable)
{
    if (self->mmsServer) {
        MmsServer_ignoreClientRequests(self->mmsServer, enable);
    }
}

void
IedServer_setDataSetAccessHandler(IedServer self, IedServer_DataSetAccessHandler handler, void* parameter)
{
    self->mmsMapping->dataSetAccessHandler = handler;
    self->mmsMapping->dataSetAccessHandlerParameter = parameter;
}

void
IedServer_setDirectoryAccessHandler(IedServer self, IedServer_DirectoryAccessHandler handler, void* parameter)
{
    self->mmsMapping->directoryAccessHandler = handler;
    self->mmsMapping->directoryAccessHandlerParameter = parameter;
}

void
IedServer_setListObjectsAccessHandler(IedServer self, IedServer_ListObjectsAccessHandler handler, void* parameter)
{
    self->mmsMapping->listObjectsAccessHandler = handler;
    self->mmsMapping->listObjectsAccessHandlerParameter = parameter;
}

void
IedServer_setControlBlockAccessHandler(IedServer self, IedServer_ControlBlockAccessHandler handler, void* parameter)
{
    self->mmsMapping->controlBlockAccessHandler = handler;
    self->mmsMapping->controlBlockAccessHandlerParameter = parameter;
}

void
IedServer_ignoreReadAccess(IedServer self, bool ignore)
{
    self->ignoreReadAccess = ignore;
}
