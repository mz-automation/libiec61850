/*
 *  server_example_dynamic.c
 *
 *  This example shows how to build a data model at runtime by API calls.
 *
 */

#include "iec61850_server.h"
#include "hal_thread.h"
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>


static int running = 0;

void sigint_handler(int signalId)
{
	running = 0;
}

int main(int argc, char** argv) {

    int tcpPort = 102;

    if (argc > 1) {
        tcpPort = atoi(argv[1]);
    }

    /*********************
     * Setup data model
     ********************/

    IedModel* model = IedModel_create("testmodel");

    LogicalDevice* lDevice1 = LogicalDevice_create("SENSORS", model);

    LogicalNode* lln0 = LogicalNode_create("LLN0", lDevice1);

    DataObject* lln0_mod = CDC_ENS_create("Mod", (ModelNode*) lln0, 0);
    DataObject* lln0_health = CDC_ENS_create("Health", (ModelNode*) lln0, 0);

    SettingGroupControlBlock_create(lln0, 1, 1);

    /* Add a temperature sensor LN */
    LogicalNode* ttmp1 = LogicalNode_create("TTMP1", lDevice1);
    DataObject* ttmp1_tmpsv = CDC_SAV_create("TmpSv", (ModelNode*) ttmp1, 0, false);

    DataAttribute* temperatureValue = (DataAttribute*) ModelNode_getChild((ModelNode*) ttmp1_tmpsv, "instMag.f");
    DataAttribute* temperatureTimestamp = (DataAttribute*) ModelNode_getChild((ModelNode*) ttmp1_tmpsv, "t");

    LogicalNode* ggio1 = LogicalNode_create("GGIO1", lDevice1);
    DataObject* ggio1_anIn1 = CDC_APC_create("AnOut1", (ModelNode*)ggio1, 0, CDC_CTL_MODEL_HAS_CANCEL | CDC_CTL_MODEL_SBO_ENHANCED, false);

    DataSet* dataSet = DataSet_create("events", lln0);
    DataSetEntry_create(dataSet, "TTMP1$MX$TmpSv$instMag$f", -1, NULL);

    uint8_t rptOptions = RPT_OPT_SEQ_NUM | RPT_OPT_TIME_STAMP | RPT_OPT_REASON_FOR_INCLUSION;

    ReportControlBlock_create("events01", lln0, "events01", false, NULL, 1, TRG_OPT_DATA_CHANGED, rptOptions, 50, 0);
    ReportControlBlock_create("events02", lln0, "events02", false, NULL, 1, TRG_OPT_DATA_CHANGED, rptOptions, 50, 0);

    GSEControlBlock_create("gse01", lln0, "events01", "events", 1, false, 200, 3000);

    /*********************
     * run server
     ********************/

	IedServer iedServer = IedServer_create(model);

	/* MMS server will be instructed to start listening to client connections. */
	IedServer_start(iedServer, tcpPort);

	if (!IedServer_isRunning(iedServer)) {
		printf("Starting server failed! Exit.\n");
		IedServer_destroy(iedServer);
		exit(-1);
	}

	running = 1;

	signal(SIGINT, sigint_handler);

	float val = 0.f;

	while (running) {
	    IedServer_lockDataModel(iedServer);

	    IedServer_updateUTCTimeAttributeValue(iedServer, temperatureTimestamp, Hal_getTimeInMs());
	    IedServer_updateFloatAttributeValue(iedServer, temperatureValue, val);

	    IedServer_unlockDataModel(iedServer);

	    val += 0.1f;

		Thread_sleep(100);
	}

	/* stop MMS server - close TCP server socket and all client sockets */
	IedServer_stop(iedServer);

	/* Cleanup - free all resources */
	IedServer_destroy(iedServer);

	/* destroy dynamic data model */
	IedModel_destroy(model);
  return 0;
} /* main() */
