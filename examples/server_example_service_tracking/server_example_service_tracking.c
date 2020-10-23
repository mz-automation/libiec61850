/*
 *  server_example_service_tracking.c
 *
 *  - How enable and use service tracking
 */

#include "iec61850_server.h"
#include "hal_thread.h"
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "static_model.h"

/* import IEC 61850 device model created from SCL-File */
extern IedModel iedModel;

static int running = 0;
static IedServer iedServer = NULL;

void
sigint_handler(int signalId)
{
    running = 0;
}

typedef struct {
    float strVal;
    int opDlTmms;
    int rsDlTmms;
    int rstTms;
} PTOC1Settings;

static PTOC1Settings ptoc1Settings[] = {
        {1.0f, 500, 500, 500},
        {2.0f, 1500, 2500, 750},
        {3.0f, 500, 1500, 750},
        {3.5f, 1250, 1750, 500},
        {3.75f, 1250, 1750, 750}
};

static void
loadActiveSgValues (int actSG)
{
    IedServer_updateFloatAttributeValue(iedServer, IEDMODEL_GenericIO_PTOC1_StrVal_setMag_f, ptoc1Settings[actSG - 1].strVal);
    IedServer_updateInt32AttributeValue(iedServer, IEDMODEL_GenericIO_PTOC1_OpDlTmms_setVal, ptoc1Settings[actSG - 1].opDlTmms);
    IedServer_updateInt32AttributeValue(iedServer, IEDMODEL_GenericIO_PTOC1_RsDlTmms_setVal, ptoc1Settings[actSG - 1].rsDlTmms);
    IedServer_updateInt32AttributeValue(iedServer, IEDMODEL_GenericIO_PTOC1_RstTms_setVal, ptoc1Settings[actSG - 1].rstTms);
}

static void
loadEditSgValues (int editSG)
{
    IedServer_updateFloatAttributeValue(iedServer, IEDMODEL_SE_GenericIO_PTOC1_StrVal_setMag_f, ptoc1Settings[editSG - 1].strVal);
    IedServer_updateInt32AttributeValue(iedServer, IEDMODEL_SE_GenericIO_PTOC1_OpDlTmms_setVal, ptoc1Settings[editSG - 1].opDlTmms);
    IedServer_updateInt32AttributeValue(iedServer, IEDMODEL_SE_GenericIO_PTOC1_RsDlTmms_setVal, ptoc1Settings[editSG - 1].rsDlTmms);
    IedServer_updateInt32AttributeValue(iedServer, IEDMODEL_SE_GenericIO_PTOC1_RstTms_setVal, ptoc1Settings[editSG - 1].rstTms);
}

static bool
activeSgChangedHandler (void* parameter, SettingGroupControlBlock* sgcb,
        uint8_t newActSg, ClientConnection connection)
{
    printf("Switch to setting group %i\n", (int) newActSg);

    loadActiveSgValues(newActSg);

    return true;
}

static bool
editSgChangedHandler (void* parameter, SettingGroupControlBlock* sgcb,
        uint8_t newEditSg, ClientConnection connection)
{
    printf("Set edit setting group to %i\n", (int) newEditSg);

    loadEditSgValues(newEditSg);

    return true;
}

static void
editSgConfirmedHandler(void* parameter, SettingGroupControlBlock* sgcb,
        uint8_t editSg)
{
    printf("Received edit sg confirm for sg %i\n", editSg);

    ptoc1Settings[editSg - 1].strVal = MmsValue_toFloat(IEDMODEL_SE_GenericIO_PTOC1_StrVal_setMag_f->mmsValue);
    ptoc1Settings[editSg - 1].opDlTmms = MmsValue_toInt32(IEDMODEL_SE_GenericIO_PTOC1_OpDlTmms_setVal->mmsValue);
    ptoc1Settings[editSg - 1].rsDlTmms = MmsValue_toInt32(IEDMODEL_SE_GenericIO_PTOC1_RsDlTmms_setVal->mmsValue);
    ptoc1Settings[editSg - 1].rstTms = MmsValue_toInt32(IEDMODEL_SE_GenericIO_PTOC1_RstTms_setVal->mmsValue);

    if (IedServer_getActiveSettingGroup(iedServer, sgcb) == editSg) {
        loadActiveSgValues(editSg);
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

int
main(int argc, char** argv)
{
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

    /* TODO enable service tracking feature */
    //IedServerConfig_enableServiceTracking(config, true);

    /* enable dynamic data set service */
    IedServerConfig_enableDynamicDataSetService(config, true);

    /* disable log service */
    IedServerConfig_enableLogService(config, true);

    /* set maximum number of clients */
    IedServerConfig_setMaxMmsConnections(config, 5);

    /* Create a new IEC 61850 server instance */
    iedServer = IedServer_createWithConfig(&iedModel, NULL, config);

    /* configuration object is no longer required */
    IedServerConfig_destroy(config);

    SettingGroupControlBlock* sgcb = LogicalDevice_getSettingGroupControlBlock(IEDMODEL_GenericIO);

    loadActiveSgValues(sgcb->actSG);

    IedServer_setActiveSettingGroupChangedHandler(iedServer, sgcb, activeSgChangedHandler, NULL);
    IedServer_setEditSettingGroupChangedHandler(iedServer, sgcb, editSgChangedHandler, NULL);
    IedServer_setEditSettingGroupConfirmationHandler(iedServer, sgcb, editSgConfirmedHandler, NULL);

    if (argc > 1) {
        char* ethernetIfcID = argv[1];

        printf("Using GOOSE interface: %s\n", ethernetIfcID);

        /* set GOOSE interface for all GOOSE publishers (GCBs) */
        IedServer_setGooseInterfaceId(iedServer, ethernetIfcID);
    }

    /* set the identity values for MMS identify service */
    IedServer_setServerIdentity(iedServer, "MZ", "service_tracking", "1.0.0");

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

    /* MMS server will be instructed to start listening for client connections. */
    IedServer_start(iedServer, 102);

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

        Thread_sleep(100);
    }

    /* stop MMS server - close TCP server socket and all client sockets */
    IedServer_stop(iedServer);

    /* Cleanup - free all resources */
    IedServer_destroy(iedServer);

    return 0;
} /* main() */
