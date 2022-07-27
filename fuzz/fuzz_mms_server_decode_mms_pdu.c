#include <stdio.h>
#include <stdlib.h>

#include "iec61850_server.h"
#include "iec61850_client.h"
#include "iso_presentation.h"

static IedModel*
setupModel1()
{
    IedModel* model = IedModel_create("testmodel");

    LogicalDevice* lDevice1 = LogicalDevice_create("SENSORS", model);

    LogicalNode* lln0 = LogicalNode_create("LLN0", lDevice1);

    DataObject* lln0_mod = CDC_ENS_create("Mod", (ModelNode*) lln0, 0);
    DataObject* lln0_health = CDC_ENS_create("Health", (ModelNode*) lln0, 0);

    /* Add a temperature sensor LN */
    LogicalNode* ttmp1 = LogicalNode_create("TTMP1", lDevice1);
    DataObject* ttmp1_tmpsv = CDC_SAV_create("TmpSv", (ModelNode*) ttmp1, 0, false);

    DataAttribute* temperatureValue = (DataAttribute*) ModelNode_getChild((ModelNode*) ttmp1_tmpsv, "instMag.f");
    DataAttribute* temperatureTimestamp = (DataAttribute*) ModelNode_getChild((ModelNode*) ttmp1_tmpsv, "t");

    /* Add a voltage transient LN */
    LogicalNode*  qvtr1 = LogicalNode_create("QVTR1", lDevice1);

    DataObject* qvtr1_varStr = CDC_SPS_create("VarStr", (ModelNode*) qvtr1, 0);
    DataObject* qvtr1_evtCnt = CDC_HST_create("EvtCnt", (ModelNode*) qvtr1, 0, 10);

    DataSet* dataSet = DataSet_create("events", lln0);
    DataSetEntry_create(dataSet, "TTMP1$MX$TmpSv$instMag$f", -1, NULL);

    uint8_t rptOptions = RPT_OPT_SEQ_NUM | RPT_OPT_TIME_STAMP | RPT_OPT_REASON_FOR_INCLUSION;

    ReportControlBlock_create("events01", lln0, "events01", false, NULL, 1, TRG_OPT_DATA_CHANGED, rptOptions, 50, 0);
    ReportControlBlock_create("events02", lln0, "events02", false, NULL, 1, TRG_OPT_DATA_UPDATE | TRG_OPT_INTEGRITY, rptOptions, 50, 0);

    return model;
}

int LLVMFuzzerTestOneInput(const char *data, size_t size)
{
    IedModel* model = setupModel1();

    IedServer iedServer = IedServer_create(model);

    IedServer_start(iedServer, 10002);

	IedClientError error;

	IedConnection con = IedConnection_create();

	IedConnection_connect(con, &error, "localhost", 10002);

    MmsConnection mmsCon = IedConnection_getMmsConnection(con);

    MmsError mmsError;

    MmsConnection_sendRawData(mmsCon, &mmsError, (uint8_t*) data, size);

	IedConnection_close(con);

	IedConnection_destroy(con);

    IedServer_stop(iedServer);
    IedServer_destroy(iedServer);

    IedModel_destroy(model);

    return 0;
}
