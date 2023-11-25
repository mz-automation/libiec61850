/*
 *  server_example_access_control.c
 *
 *  - How to use access control mechanisms
 *  - How to implement RBAC features based on access control mechanisms
 */

#include "iec61850_server.h"
#include "hal_thread.h"
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "static_model.h"

static int running = 0;
static IedServer iedServer = NULL;

void
sigint_handler(int signalId)
{
    running = 0;
}

static const char*
ACSIClassToStr(ACSIClass acsiClass)
{
    switch (acsiClass)
    {
        case ACSI_CLASS_BRCB:
            return "BRCB";
        case ACSI_CLASS_URCB:
            return "URCB";
        case ACSI_CLASS_GoCB:
            return "GoCB";
        case ACSI_CLASS_SGCB:
            return "SGCB";
        case ACSI_CLASS_LCB:
            return "LCB";
        case ACSI_CLASS_GsCB:
            return "GsCB";
        case ACSI_CLASS_LOG:
            return "log";
        case ACSI_CLASS_DATA_SET:
            return "dataset";
        case ACSI_CLASS_DATA_OBJECT:
            return "data-object";
        case ACSI_CLASS_MSVCB:
            return "MSVCB";
        case ACSI_CLASS_USVCB:
            return "USVCB";
        default:
            return "unknown";
    }
}

static ControlHandlerResult
controlHandlerForBinaryOutput(ControlAction action, void* parameter, MmsValue* value, bool test)
{
    if (test)
        return CONTROL_RESULT_FAILED;

    if (MmsValue_getType(value) == MMS_BOOLEAN) {
        printf("received binary control command: ");

        if (MmsValue_getBoolean(value))
            printf("on\n");
        else
            printf("off\n");
    }
    else
        return CONTROL_RESULT_FAILED;

    uint64_t timeStamp = Hal_getTimeInMs();

    if (parameter == IEDMODEL_GenericIO_GGIO1_SPCSO1) {
        IedServer_updateUTCTimeAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO1_t, timeStamp);
        IedServer_updateAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO1_stVal, value);
    }

    if (parameter == IEDMODEL_GenericIO_GGIO1_SPCSO2) {
        IedServer_updateUTCTimeAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO2_t, timeStamp);
        IedServer_updateAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO2_stVal, value);
    }

    if (parameter == IEDMODEL_GenericIO_GGIO1_SPCSO3) {
        IedServer_updateUTCTimeAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO3_t, timeStamp);
        IedServer_updateAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO3_stVal, value);
    }

    if (parameter == IEDMODEL_GenericIO_GGIO1_SPCSO4) {
        IedServer_updateUTCTimeAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO4_t, timeStamp);
        IedServer_updateAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO4_stVal, value);
    }

    return CONTROL_RESULT_OK;
}

static void
connectionHandler (IedServer self, ClientConnection connection, bool connected, void* parameter)
{
    if (connected)
        printf("Connection opened\n");
    else
        printf("Connection closed\n");
}

/*
 * This handler is called before the rcbEventHandler and can be use to allow or permit read or write access to the RCB
 */
static bool
controlBlockAccessHandler(void* parameter, ClientConnection connection, ACSIClass acsiClass, LogicalDevice* ld, LogicalNode* ln, const char* objectName, const char* subObjectName, IedServer_ControlBlockAccessType accessType)
{
    printf("%s %s access %s/%s.%s.%s\n", ACSIClassToStr(acsiClass), accessType == IEC61850_CB_ACCESS_TYPE_WRITE ? "write" : "read", ld->name, ln->name, objectName, subObjectName);

    /* allow only read access to LCBs */
    if (acsiClass == ACSI_CLASS_LCB) {
        if (accessType == IEC61850_CB_ACCESS_TYPE_READ)
            return true;
        else
            return false;
    }
    
    /* allow only read access to BRCBs */
    if (acsiClass == ACSI_CLASS_BRCB) {
        if (accessType == IEC61850_CB_ACCESS_TYPE_READ)
            return true;
        else
            return false;
    }

    /* to all other control blocks allow read and write access */
    return true;
}

static void
rcbEventHandler(void* parameter, ReportControlBlock* rcb, ClientConnection connection, IedServer_RCBEventType event, const char* parameterName, MmsDataAccessError serviceError)
{
    if ((event == RCB_EVENT_SET_PARAMETER) || (event == RCB_EVENT_GET_PARAMETER)) {
        printf("RCB: %s event: %i\n", ReportControlBlock_getName(rcb), event);
        printf("  param:  %s\n", parameterName);
        printf("  result: %i\n", serviceError);
    }

    if (event == RCB_EVENT_ENABLE) {
        printf("RCB: %s event: %i\n", ReportControlBlock_getName(rcb), event);
        char* rptId = ReportControlBlock_getRptID(rcb);
        printf("   rptID:  %s\n", rptId);
        char* dataSet = ReportControlBlock_getDataSet(rcb);
        printf("   datSet: %s\n", dataSet);

        free(rptId);
        free(dataSet);
    }
}

static bool
dataSetAccessHandler(void* parameter, ClientConnection connection, IedServer_DataSetOperation operation, const char* datasetRef)
{
    printf("Data set access: %s operation: %i\n", datasetRef, operation);

    return true;
}

static MmsDataAccessError
readAccessHandler(LogicalDevice* ld, LogicalNode* ln, DataObject* dataObject, FunctionalConstraint fc, ClientConnection connection, void* parameter)
{
    printf("Read access to %s/%s.%s\n", ld->name, ln->name, dataObject ? dataObject->name : "-");

    if (dataObject == NULL) {
        if (!strcmp(ln->name, "GGIO1")) {
            return DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED;
        }
    }
    else {
        if (!strcmp(ln->name, "GGIO1") && !strcmp(dataObject->name, "AnIn1")) {
            return DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED;
        }
    }

    return DATA_ACCESS_ERROR_SUCCESS;
}

static bool
listObjectsAccessHandler(void* parameter, ClientConnection connection, ACSIClass acsiClass, LogicalDevice* ld, LogicalNode* ln, const char* objectName, const char* subObjectName, FunctionalConstraint fc)
{
    if (subObjectName)
        printf("list objects access[2] to %s/%s.%s.%s [acsi-class: %s(%i)] [FC=%s]\n", ld->name, ln ? ln->name : "-", objectName, subObjectName, ACSIClassToStr(acsiClass), acsiClass, FunctionalConstraint_toString(fc));
    else
        printf("list objects access[2] to %s/%s.%s [acsi-class: %s(%i)] [FC=%s]\n", ld->name, ln ? ln->name : "-", objectName, ACSIClassToStr(acsiClass), acsiClass, FunctionalConstraint_toString(fc));

    // if (acsiClass == ACSI_CLASS_BRCB) {
    //     return true;
    // }

    // return false;

    return true;
}

static bool
directoryAccessHandler(void* parameter, ClientConnection connection, IedServer_DirectoryCategory category, LogicalDevice* logicalDevice)
{
    switch(category) {
        case DIRECTORY_CAT_LD_LIST:
            printf("Get list of logical devices from %s\n", ClientConnection_getPeerAddress(connection));
            break;
        case DIRECTORY_CAT_DATASET_LIST:
            printf("Get list of datasets for LD %s from %s\n", ModelNode_getName((ModelNode*)logicalDevice), ClientConnection_getPeerAddress(connection));
            break;
        case DIRECTORY_CAT_DATA_LIST:
            printf("Get list of data for LD %s from %s\n", ModelNode_getName((ModelNode*)logicalDevice), ClientConnection_getPeerAddress(connection));
            break;
        case DIRECTORY_CAT_LOG_LIST:
            printf("Get list of logs for LD %s from %s -> reject\n", ModelNode_getName((ModelNode*)logicalDevice), ClientConnection_getPeerAddress(connection));
            return false;
            break;
    }

    return true;
}

int
main(int argc, char** argv)
{
    int tcpPort = 102;

    if (argc > 1) {
        tcpPort = atoi(argv[1]);
    }

    printf("Using libIEC61850 version %s\n", LibIEC61850_getVersionString());

    /* Create new server configuration object */
    IedServerConfig config = IedServerConfig_create();

    /* Set buffer size for buffered report control blocks to 200000 bytes */
    IedServerConfig_setReportBufferSize(config, 200000);

    /* Set stack compliance to a specific edition of the standard (WARNING: data model has also to be checked for compliance) */
    IedServerConfig_setEdition(config, IEC_61850_EDITION_2);

    /* Set the base path for the MMS file services */
    IedServerConfig_setFileServiceBasePath(config, "./vmd-filestore/");

    /* disable MMS file service */
    IedServerConfig_enableFileService(config, false);

    /* enable dynamic data set service */
    IedServerConfig_enableDynamicDataSetService(config, true);

    /* disable log service */
    IedServerConfig_enableLogService(config, false);

    /* set maximum number of clients */
    IedServerConfig_setMaxMmsConnections(config, 2);

    /* Create a new IEC 61850 server instance */
    iedServer = IedServer_createWithConfig(&iedModel, NULL, config);

    /* configuration object is no longer required */
    IedServerConfig_destroy(config);

    /* set the identity values for MMS identify service */
    IedServer_setServerIdentity(iedServer, "libiec61850.com", "access control example", "1.0.0");

    /* Install handler for operate command */
    IedServer_setControlHandler(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO1,
            (ControlHandler) controlHandlerForBinaryOutput,
            IEDMODEL_GenericIO_GGIO1_SPCSO1);

    IedServer_setControlHandler(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO2,
            (ControlHandler) controlHandlerForBinaryOutput,
            IEDMODEL_GenericIO_GGIO1_SPCSO2);

    IedServer_setControlHandler(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO3,
            (ControlHandler) controlHandlerForBinaryOutput,
            IEDMODEL_GenericIO_GGIO1_SPCSO3);

    IedServer_setControlHandler(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO4,
            (ControlHandler) controlHandlerForBinaryOutput,
            IEDMODEL_GenericIO_GGIO1_SPCSO4);

    IedServer_setConnectionIndicationHandler(iedServer, (IedConnectionIndicationHandler) connectionHandler, NULL);

    /* Install handler to log RCB events */
    IedServer_setRCBEventHandler(iedServer, rcbEventHandler, NULL);

    /* Install handler to control access to control blocks (RCBs, LCBs, GoCBs, SVCBs, SGCBs)*/
    IedServer_setControlBlockAccessHandler(iedServer, controlBlockAccessHandler, NULL);

    /* By default access to variables with FC=DC and FC=CF is not allowed.
     * This allow to write to simpleIOGenericIO/GGIO1.NamPlt.vendor variable used
     * by iec61850_client_example1.
     */
    IedServer_setWriteAccessPolicy(iedServer, IEC61850_FC_DC, ACCESS_POLICY_ALLOW);

    /* Install handler to perform access control on datasets */
    IedServer_setDataSetAccessHandler(iedServer, dataSetAccessHandler, NULL);

    /* Install handler to perform read access control on data model elements
     * NOTE: when read access to a data model element is blocked this will also prevent the client
     * to read the data model element  in a data set or enable a RCB instance that uses a dataset
     * containing the restricted data model element.
     */
    IedServer_setReadAccessHandler(iedServer, readAccessHandler, NULL);

    IedServer_setDirectoryAccessHandler(iedServer, directoryAccessHandler, NULL);

    /* control visibility of data objects in directory (get-name-list) and variable description (get-variable-access-attributes) services */
    IedServer_setListObjectsAccessHandler(iedServer, listObjectsAccessHandler, NULL);

    /* MMS server will be instructed to start listening for client connections. */
    IedServer_start(iedServer, tcpPort);

    if (!IedServer_isRunning(iedServer)) {
        printf("Starting server failed (maybe need root permissions or another server is already using the port)! Exit.\n");
        IedServer_destroy(iedServer);
        exit(-1);
    }

    running = 1;

    signal(SIGINT, sigint_handler);

    float t = 0.f;

    while (running) {
        uint64_t timestamp = Hal_getTimeInMs();

        t += 0.1f;

        float an1 = sinf(t);
        float an2 = sinf(t + 1.f);
        float an3 = sinf(t + 2.f);
        float an4 = sinf(t + 3.f);

        Timestamp iecTimestamp;

        Timestamp_clearFlags(&iecTimestamp);
        Timestamp_setTimeInMilliseconds(&iecTimestamp, timestamp);
        Timestamp_setLeapSecondKnown(&iecTimestamp, true);

        /* toggle clock-not-synchronized flag in timestamp */
        if (((int) t % 2) == 0)
            Timestamp_setClockNotSynchronized(&iecTimestamp, true);

#if 1
        IedServer_lockDataModel(iedServer);

        IedServer_updateTimestampAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_AnIn1_t, &iecTimestamp);
        IedServer_updateFloatAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_AnIn1_mag_f, an1);

        IedServer_updateTimestampAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_AnIn2_t, &iecTimestamp);
        IedServer_updateFloatAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_AnIn2_mag_f, an2);

        IedServer_updateTimestampAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_AnIn3_t, &iecTimestamp);
        IedServer_updateFloatAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_AnIn3_mag_f, an3);

        IedServer_updateTimestampAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_AnIn4_t, &iecTimestamp);
        IedServer_updateFloatAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_AnIn4_mag_f, an4);

        IedServer_unlockDataModel(iedServer);
#endif

        Thread_sleep(100);
    }

    /* stop MMS server - close TCP server socket and all client sockets */
    IedServer_stop(iedServer);

    /* Cleanup - free all resources */
    IedServer_destroy(iedServer);

    return 0;
} /* main() */
