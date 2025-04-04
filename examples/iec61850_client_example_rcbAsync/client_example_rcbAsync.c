/*
 * client_example_rcbAsync.c
 *
 * This example is intended to be used with server_example_basic_io or server_example_goose.
 */

#include "iec61850_client.h"

#include <stdlib.h>
#include <stdio.h>

#include "hal_thread.h"

static ClientDataSet clientDataSet = NULL;
static ClientReportControlBlock RCB = NULL;

static void
printValue(char* name, MmsValue* value)
{
    char buf[1000];

    MmsValue_printToBuffer(value, buf, 1000);
    printf("%s: %s\n", name, buf);
}

static void
readObjectHandler(uint32_t invokeId, void* parameter, IedClientError err, MmsValue* value)
{
    if (err == IED_ERROR_OK) {
        printValue((char*)parameter, value);

        MmsValue_delete(value);
    }
    else {
        printf("Failed to read object %s (err=%i)\n", (char*)parameter, err);
    }
}

static void
readDataSetHandler(uint32_t invokeId, void* parameter, IedClientError err, ClientDataSet dataSet)
{
    if (err == IED_ERROR_OK) {
        clientDataSet = dataSet;
        printf("Data set has %d entries\n", ClientDataSet_getDataSetSize(dataSet));

        MmsValue* values = ClientDataSet_getValues(dataSet);

        if (MmsValue_getType(values) == MMS_ARRAY) {
            int i;

            for (i = 0; i < MmsValue_getArraySize(values); i++) {
                printf(" [%i]", i);
                printValue("", MmsValue_getElement(values, i));
            }
        }
    }
    else {
        printf("Failed to read data set (err=%i\n", err);
    }
}

void
reportCallbackFunction(void* parameter, ClientReport report)
{
    MmsValue* dataSetValues = ClientReport_getDataSetValues(report);
    printf("received report for %s\n", ClientReport_getRcbReference(report));

    int i;

    for (i = 0; i < 4; i++) {
        ReasonForInclusion reason = ClientReport_getReasonForInclusion(report, i);

        if (reason != IEC61850_REASON_NOT_INCLUDED) {
            printf(" GGIO1.SPCSO%i.stVal: %i (included for reason %i)\n", i,
                MmsValue_getBoolean(MmsValue_getElement(dataSetValues, i)), reason);
        }
    }
}

static void
getRCBValuesHandler(uint32_t invokeId, void* parameter, IedClientError err, ClientReportControlBlock rcb) 
{
    if (err == IED_ERROR_OK) {
		
        if (rcb) {
            bool rptEna = ClientReportControlBlock_getRptEna(rcb);
            printf("RptEna = %i\n", rptEna);

            const char* RptId = ClientReportControlBlock_getRptId(rcb);
            printf("RptID = %s\n", RptId);
			
            RCB = rcb;
        }
    }
    else{
        printf("Failed to get RCB Values (err=%i)\n", err);
    }
};

static void
genericServiceHandler(uint32_t invokeId, void* parameter, IedClientError err)
{
    if (err == IED_ERROR_OK) {
        printf("Success triggering a GI report\n");
    }
    else {
        printf("Error triggering a GI report (code: %i)\n", err);
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

    IedConnection_connectAsync(con, &error, hostname, tcpPort);
	
    if (error == IED_ERROR_OK) {
        bool success = true;

        while (IedConnection_getState(con) != IED_STATE_CONNECTED) {
			
            if (IedConnection_getState(con) == IED_STATE_CLOSED) {
                success = false;
                break;
            }

            Thread_sleep(10);
        }

        if (success) {

            /*read measurement value from server*/
            IedConnection_readObjectAsync(con, &error, "simpleIOGenericIO/GGIO1.AnIn1.mag.f", IEC61850_FC_MX, readObjectHandler, "simpleIOGenericIO/GGIO1.AnIn2.mag.f");

            if (error != IED_ERROR_OK) {
                printf("read object error %i\n", error);
            }

            /*read data set*/
            IedConnection_readDataSetValuesAsync(con, &error, "simpleIOGenericIO/LLN0.Events", NULL, readDataSetHandler, NULL);

            if (error != IED_ERROR_OK) {
                printf("read data set error %i\n", error);
            }
			
            /* Read RCB values from server*/
            IedConnection_getRCBValuesAsync(con, &error, "simpleIOGenericIO/LLN0.RP.EventsRCB01", NULL, getRCBValuesHandler, NULL);
			
            if (error != IED_ERROR_OK) {
                printf("getRCBValues service error! %i\n", error);
            }

            while (RCB == NULL) {}

            if (RCB != NULL) {

                /*Set RCB Values locally*/
                ClientReportControlBlock_setResv(RCB, true);
                ClientReportControlBlock_setTrgOps(RCB, TRG_OPT_DATA_CHANGED | TRG_OPT_QUALITY_CHANGED | TRG_OPT_GI);
                ClientReportControlBlock_setDataSetReference(RCB, "simpleIOGenericIO/LLN0$Events"); /* NOTE the "$" instead of "." ! */
                ClientReportControlBlock_setRptEna(RCB, true);
                ClientReportControlBlock_setGI(RCB, true);

                /*Set RCB Values to server*/
                IedConnection_setRCBValuesAsync(con, &error, RCB, RCB_ELEMENT_RESV | RCB_ELEMENT_DATSET | RCB_ELEMENT_TRG_OPS | RCB_ELEMENT_RPT_ENA | RCB_ELEMENT_GI, true, genericServiceHandler, NULL);

                if (error != IED_ERROR_OK) {
                    printf("setRCBValues service error!\n");
                }

                Thread_sleep(1000);

                /*Trigger GI Report*/
                ClientReportControlBlock_setGI(RCB, true);
                IedConnection_setRCBValuesAsync(con, &error, RCB, RCB_ELEMENT_GI, true, genericServiceHandler, NULL);

                if (error != IED_ERROR_OK)
                    printf("Error triggering a GI report (code: %i)\n", error);

                Thread_sleep(60000);
				
                /*Disable Reporting*/
                ClientReportControlBlock_setRptEna(RCB, false);
                IedConnection_setRCBValuesAsync(con, &error, RCB, RCB_ELEMENT_RPT_ENA, true, genericServiceHandler, NULL);

                if (error != IED_ERROR_OK)
                    printf("disable reporting failed (code: %i)\n", error);

                ClientDataSet_destroy(clientDataSet);
                ClientReportControlBlock_destroy(RCB);
				
            }
        }
		
        Thread_sleep(50000);
		
        IedConnection_releaseAsync(con, &error);

        if (error != IED_ERROR_OK) {
            printf("Release returned error: %d\n", error);
        }
        else {
            while (IedConnection_getState(con) != IED_STATE_CLOSED) {
                Thread_sleep(10);
            }
        }
    }
    else {
        printf("Failed to connect to %s:%i\n", hostname, tcpPort);
    }

    IedConnection_destroy(con);
    return 0;
}