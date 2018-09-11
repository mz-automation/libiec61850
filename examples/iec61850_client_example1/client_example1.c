/*
 * client_example1.c
 *
 * This example is intended to be used with server_example_basic_io or server_example_goose.
 */

#include "iec61850_client.h"

#include <stdlib.h>
#include <stdio.h>

#include "hal_thread.h"

void
reportCallbackFunction(void* parameter, ClientReport report)
{
    MmsValue* dataSetValues = ClientReport_getDataSetValues(report);

    printf("received report for %s\n", ClientReport_getRcbReference(report));

    int i;
    for (i = 0; i < 4; i++) {
        ReasonForInclusion reason = ClientReport_getReasonForInclusion(report, i);

        if (reason != IEC61850_REASON_NOT_INCLUDED) {
            printf("  GGIO1.SPCSO%i.stVal: %i (included for reason %i)\n", i,
                    MmsValue_getBoolean(MmsValue_getElement(dataSetValues, i)), reason);
        }
    }
}

static void
printValue(char* name, MmsValue* value)
{
    char buf[1000];

    MmsValue_printToBuffer(value, buf, 1000);

    printf("Received value for %s: %s\n", name, buf);
}

static void
readVariableHandler (int invokeId, void* parameter, MmsError mmsError, MmsValue* value)
{
    if (mmsError == MMS_ERROR_NONE) {

        printValue((char*) parameter, value);

        MmsValue_delete(value);
    }
    else {
        printf("Failed to read %s (err=%i)\n", parameter, mmsError);
    }
}

int main(int argc, char** argv) {

    char* hostname;
    int tcpPort = 102;

    if (argc > 1)
        hostname = argv[1];
    else
        hostname = "localhost";

    if (argc > 2)
        tcpPort = atoi(argv[2]);

    IedClientError error;

    IedConnection con = IedConnection_create();

    IedConnection_connect(con, &error, hostname, tcpPort);

    if (error == IED_ERROR_OK) {

        MmsConnection mmsCon = IedConnection_getMmsConnection(con);

        MmsError mmsErr;

        MmsValue* val1 = MmsConnection_readVariable(mmsCon, &mmsErr, "simpleIOGenericIO", "GGIO1$MX$AnIn1$mag$f");

        if (mmsErr != MMS_ERROR_NONE)
            printf("Request 0 failed\n");
        else
            printValue("sync read", val1);

        MmsConnection_readVariableAsync(mmsCon, &mmsErr, "simpleIOGenericIO", "GGIO1$MX$AnIn1$mag$f", readVariableHandler, "GGIO1$MX$AnIn1$mag$f");

        if (mmsErr != MMS_ERROR_NONE)
            printf("Request 1 failed\n");

        MmsConnection_readVariableAsync(mmsCon, &mmsErr, "simpleIOGenericIO", "GGIO1$MX$AnIn2$mag$f", readVariableHandler, "GGIO1$MX$AnIn2$mag$f");

        if (mmsErr != MMS_ERROR_NONE)
            printf("Request 2 failed\n");

        MmsConnection_readVariableAsync(mmsCon, &mmsErr, "simpleIOGenericIO", "GGIO1$MX$AnIn3$mag$f", readVariableHandler, "GGIO1$MX$AnIn3$mag$f");

        if (mmsErr != MMS_ERROR_NONE)
            printf("Request 3 failed\n");

        MmsConnection_readVariableAsync(mmsCon, &mmsErr, "simpleIOGenericIO", "GGIO1$MX$AnIn4$mag$f", readVariableHandler, "GGIO1$MX$AnIn4$mag$f");

        if (mmsErr != MMS_ERROR_NONE)
            printf("Request 4 failed\n");

        MmsConnection_readVariableAsync(mmsCon, &mmsErr, "simpleIOGenericIO", "GGIO1$MX$AnIn1$mag$f", readVariableHandler, "GGIO1$MX$AnIn1$mag$f");

        if (mmsErr != MMS_ERROR_NONE)
            printf("Request 1 failed\n");

        MmsConnection_readVariableAsync(mmsCon, &mmsErr, "simpleIOGenericIO", "GGIO1$MX$AnIn2$mag$f", readVariableHandler, "GGIO1$MX$AnIn2$mag$f");

        if (mmsErr != MMS_ERROR_NONE)
            printf("Request 2 failed\n");

        MmsConnection_readVariableAsync(mmsCon, &mmsErr, "simpleIOGenericIO", "GGIO1$MX$AnIn3$mag$f", readVariableHandler, "GGIO1$MX$AnIn3$mag$f");

        if (mmsErr != MMS_ERROR_NONE)
            printf("Request 3 failed\n");

        MmsConnection_readVariableAsync(mmsCon, &mmsErr, "simpleIOGenericIO", "GGIO1$MX$AnIn4$mag$f", readVariableHandler, "GGIO1$MX$AnIn4$mag$f");

        if (mmsErr != MMS_ERROR_NONE)
            printf("Request 4 failed\n");

        MmsConnection_readVariableAsync(mmsCon, &mmsErr, "simpleIOGenericIO", "GGIO1$MX$AnIn1$mag$f", readVariableHandler, "GGIO1$MX$AnIn1$mag$f");

        if (mmsErr != MMS_ERROR_NONE)
            printf("Request 1 failed\n");

        MmsConnection_readVariableAsync(mmsCon, &mmsErr, "simpleIOGenericIO", "GGIO1$MX$AnIn2$mag$f", readVariableHandler, "GGIO1$MX$AnIn2$mag$f");

        if (mmsErr != MMS_ERROR_NONE)
            printf("Request 2 failed\n");

        MmsConnection_readVariableAsync(mmsCon, &mmsErr, "simpleIOGenericIO", "GGIO1$MX$AnIn3$mag$f", readVariableHandler, "GGIO1$MX$AnIn3$mag$f");

        if (mmsErr != MMS_ERROR_NONE)
            printf("Request 3 failed\n");

        MmsConnection_readVariableAsync(mmsCon, &mmsErr, "simpleIOGenericIO", "GGIO1$MX$AnIn4$mag$f", readVariableHandler, "GGIO1$MX$AnIn4$mag$f");

        if (mmsErr != MMS_ERROR_NONE)
            printf("Request 4 failed\n");

        MmsConnection_readVariableAsync(mmsCon, &mmsErr, "simpleIOGenericIO", "GGIO1$MX$AnIn1$mag$f", readVariableHandler, "GGIO1$MX$AnIn1$mag$f");

        if (mmsErr != MMS_ERROR_NONE)
            printf("Request 1 failed\n");

        MmsConnection_readVariableAsync(mmsCon, &mmsErr, "simpleIOGenericIO", "GGIO1$MX$AnIn2$mag$f", readVariableHandler, "GGIO1$MX$AnIn2$mag$f");

        if (mmsErr != MMS_ERROR_NONE)
            printf("Request 2 failed\n");

        MmsConnection_readVariableAsync(mmsCon, &mmsErr, "simpleIOGenericIO", "GGIO1$MX$AnIn3$mag$f", readVariableHandler, "GGIO1$MX$AnIn3$mag$f");

        if (mmsErr != MMS_ERROR_NONE)
            printf("Request 3 failed\n");

        MmsConnection_readVariableAsync(mmsCon, &mmsErr, "simpleIOGenericIO", "GGIO1$MX$AnIn4$mag$f", readVariableHandler, "GGIO1$MX$AnIn4$mag$f");

        if (mmsErr != MMS_ERROR_NONE)
            printf("Request 4 failed\n");


        Thread_sleep(10000);


//        /* read an analog measurement value from server */
//        MmsValue* value = IedConnection_readObject(con, &error, "simpleIOGenericIO/GGIO1.AnIn1.mag.f", IEC61850_FC_MX);
//
//        if (value != NULL) {
//            float fval = MmsValue_toFloat(value);
//            printf("read float value: %f\n", fval);
//            MmsValue_delete(value);
//        }
//
//        /* write a variable to the server */
//        value = MmsValue_newVisibleString("libiec61850.com");
//        IedConnection_writeObject(con, &error, "simpleIOGenericIO/GGIO1.NamPlt.vendor", IEC61850_FC_DC, value);
//
//        if (error != IED_ERROR_OK)
//            printf("failed to write simpleIOGenericIO/GGIO1.NamPlt.vendor!\n");
//
//        MmsValue_delete(value);
//
//
//        /* read data set */
//        ClientDataSet clientDataSet = IedConnection_readDataSetValues(con, &error, "simpleIOGenericIO/LLN0.Events", NULL);
//
//        if (clientDataSet == NULL)
//            printf("failed to read dataset\n");
//
//        /* Read RCB values */
//        ClientReportControlBlock rcb =
//                IedConnection_getRCBValues(con, &error, "simpleIOGenericIO/LLN0.RP.EventsRCB01", NULL);
//
//
//        bool rptEna = ClientReportControlBlock_getRptEna(rcb);
//
//        printf("RptEna = %i\n", rptEna);
//
//        /* Install handler for reports */
//        IedConnection_installReportHandler(con, "simpleIOGenericIO/LLN0.RP.EventsRCB01",
//                ClientReportControlBlock_getRptId(rcb), reportCallbackFunction, NULL);
//
//        /* Set trigger options and enable report */
//        ClientReportControlBlock_setTrgOps(rcb, TRG_OPT_DATA_UPDATE | TRG_OPT_INTEGRITY | TRG_OPT_GI);
//        ClientReportControlBlock_setRptEna(rcb, true);
//        ClientReportControlBlock_setIntgPd(rcb, 5000);
//        IedConnection_setRCBValues(con, &error, rcb, RCB_ELEMENT_RPT_ENA | RCB_ELEMENT_TRG_OPS | RCB_ELEMENT_INTG_PD, true);
//
//        if (error != IED_ERROR_OK)
//            printf("report activation failed (code: %i)\n", error);
//
//        Thread_sleep(1000);
//
//        /* trigger GI report */
//        ClientReportControlBlock_setGI(rcb, true);
//        IedConnection_setRCBValues(con, &error, rcb, RCB_ELEMENT_GI, true);
//
//        if (error != IED_ERROR_OK)
//            printf("Error triggering a GI report (code: %i)\n", error);
//
//        Thread_sleep(60000);
//
//        /* disable reporting */
//        ClientReportControlBlock_setRptEna(rcb, false);
//        IedConnection_setRCBValues(con, &error, rcb, RCB_ELEMENT_RPT_ENA, true);
//
//        if (error != IED_ERROR_OK)
//            printf("disable reporting failed (code: %i)\n", error);
//
//        ClientDataSet_destroy(clientDataSet);
//
//        ClientReportControlBlock_destroy(rcb);
//
//        close_connection:

        IedConnection_close(con);
    }
    else {
        printf("Failed to connect to %s:%i\n", hostname, tcpPort);
    }

    IedConnection_destroy(con);
}


