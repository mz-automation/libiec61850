/*
 *  server_example_sg.c
 */

#include "iec61850_server.h"
#include "hal_thread.h"
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>


/* Include the generated header with the model access handles */
#include "static_model.h"

static IedServer iedServer;
static int running = 0;

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

void sigint_handler(int signalId)
{
    running = 0;
}

static void
loadActiveSgValues (int actSG)
{
    IedServer_updateFloatAttributeValue(iedServer, IEDMODEL_PROT_PTOC1_StrVal_setMag_f, ptoc1Settings[actSG - 1].strVal);
    IedServer_updateInt32AttributeValue(iedServer, IEDMODEL_PROT_PTOC1_OpDlTmms_setVal, ptoc1Settings[actSG - 1].opDlTmms);
    IedServer_updateInt32AttributeValue(iedServer, IEDMODEL_PROT_PTOC1_RsDlTmms_setVal, ptoc1Settings[actSG - 1].rsDlTmms);
    IedServer_updateInt32AttributeValue(iedServer, IEDMODEL_PROT_PTOC1_RstTms_setVal, ptoc1Settings[actSG - 1].rstTms);
}

static void
loadEditSgValues (int editSG)
{
    IedServer_updateFloatAttributeValue(iedServer, IEDMODEL_SE_PROT_PTOC1_StrVal_setMag_f, ptoc1Settings[editSG - 1].strVal);
    IedServer_updateInt32AttributeValue(iedServer, IEDMODEL_SE_PROT_PTOC1_OpDlTmms_setVal, ptoc1Settings[editSG - 1].opDlTmms);
    IedServer_updateInt32AttributeValue(iedServer, IEDMODEL_SE_PROT_PTOC1_RsDlTmms_setVal, ptoc1Settings[editSG - 1].rsDlTmms);
    IedServer_updateInt32AttributeValue(iedServer, IEDMODEL_SE_PROT_PTOC1_RstTms_setVal, ptoc1Settings[editSG - 1].rstTms);
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

    ptoc1Settings[editSg - 1].strVal = MmsValue_toFloat(IEDMODEL_SE_PROT_PTOC1_StrVal_setMag_f->mmsValue);
    ptoc1Settings[editSg - 1].opDlTmms = MmsValue_toInt32(IEDMODEL_SE_PROT_PTOC1_OpDlTmms_setVal->mmsValue);
    ptoc1Settings[editSg - 1].rsDlTmms = MmsValue_toInt32(IEDMODEL_SE_PROT_PTOC1_RsDlTmms_setVal->mmsValue);
    ptoc1Settings[editSg - 1].rstTms = MmsValue_toInt32(IEDMODEL_SE_PROT_PTOC1_RstTms_setVal->mmsValue);

    if (IedServer_getActiveSettingGroup(iedServer, sgcb) == editSg) {
        loadActiveSgValues(editSg);
    }
}

static MmsDataAccessError
readAccessHandler(LogicalDevice* ld, LogicalNode* ln, DataObject* dataObject, FunctionalConstraint fc, ClientConnection connection, void* parameter)
{
    void* securityToken = ClientConnection_getSecurityToken(connection);

    if (dataObject)
        printf("Read access to %s/%s.%s[%s]\n", ld->name, ln->name, dataObject->name, FunctionalConstraint_toString(fc));
    else
        printf("Read access to %s/%s[%s]\n", ld->name, ln->name, FunctionalConstraint_toString(fc));

    return DATA_ACCESS_ERROR_SUCCESS;
}

int 
main(int argc, char** argv)
{
    int tcpPort = 102;

    if (argc > 1) {
        tcpPort = atoi(argv[1]);
    }

    IedServerConfig config = IedServerConfig_create();

    //IedServerConfig_enableEditSG(config, false);
    IedServerConfig_enableResvTmsForSGCB(config, false);

    iedServer = IedServer_createWithConfig(&iedModel, NULL, config);

    IedServer_setTimeQuality(iedServer, true, false, false, 10);

    IedServerConfig_destroy(config);

    LogicalDevice* ld = IEDMODEL_PROT;

    SettingGroupControlBlock* sgcb = LogicalDevice_getSettingGroupControlBlock(ld);

    loadActiveSgValues(sgcb->actSG);

    IedServer_setActiveSettingGroupChangedHandler(iedServer, sgcb, activeSgChangedHandler, NULL);
    IedServer_setEditSettingGroupChangedHandler(iedServer, sgcb, editSgChangedHandler, NULL);
    IedServer_setEditSettingGroupConfirmationHandler(iedServer, sgcb, editSgConfirmedHandler, NULL);

    IedServer_setReadAccessHandler(iedServer, readAccessHandler, NULL);

    /* MMS server will be instructed to start listening to client connections. */
    IedServer_start(iedServer, tcpPort);

    if (!IedServer_isRunning(iedServer)) {
        printf("Starting server failed! Exit.\n");
        IedServer_destroy(iedServer);
        exit(-1);
    }

    running = 1;

    signal(SIGINT, sigint_handler);

    float power = 500.f;

    while (running) {

        uint64_t timeval = Hal_getTimeInMs();

        IedServer_lockDataModel(iedServer);

        IedServer_unlockDataModel(iedServer);

        power += 0.1f;

        Thread_sleep(500);
    }

    /* stop MMS server - close TCP server socket and all client sockets */
    IedServer_stop(iedServer);

    /* Cleanup - free all resources */
    IedServer_destroy(iedServer);

    return 0;
} /* main() */
