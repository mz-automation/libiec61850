/*
 * tls_client_exmaple.c
 *
 * This example shows how to configure TLS
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
securityEventHandler(void* parameter, TLSEventLevel eventLevel, int eventCode, const char* msg, TLSConnection con)
{
    (void)parameter;

    char* peerAddr = TLSConnection_getPeerAddress(con, NULL);

    printf("[SECURITY EVENT] %s (%s)(t: %i, c: %i)\n", msg, peerAddr, eventLevel, eventCode);

    free(peerAddr);
}

int main(int argc, char** argv) {

    char* hostname;

    if (argc > 1)
        hostname = argv[1];
    else
        hostname = "localhost";

    TLSConfiguration tlsConfig = TLSConfiguration_create();

    TLSConfiguration_setChainValidation(tlsConfig, true);
    TLSConfiguration_setAllowOnlyKnownCertificates(tlsConfig, false);

    TLSConfiguration_setEventHandler(tlsConfig, securityEventHandler, NULL);

    if (!TLSConfiguration_setOwnKeyFromFile(tlsConfig, "client_CA1_1.key", NULL)) {
        printf("ERROR: Failed to load private key!\n");
        return 0;
    }

    if (!TLSConfiguration_setOwnCertificateFromFile(tlsConfig, "client_CA1_1.pem")) {
        printf("ERROR: Failed to load own certificate!\n");
        return 0;
    }

    if (!TLSConfiguration_addCACertificateFromFile(tlsConfig, "root_CA1.pem")) {
        printf("ERROR: Failed to load root certificate\n");
        return 0;
    }

    IedClientError error;

    IedConnection con = IedConnection_createWithTlsSupport(tlsConfig);

    IedConnection_connect(con, &error, hostname, -1);

    if (error == IED_ERROR_OK) {

        LinkedList serverDirectory = IedConnection_getServerDirectory(con, &error, false);

        if (error != IED_ERROR_OK)
            printf("failed to read server directory (error=%i)\n", error);

        if (serverDirectory)
            LinkedList_destroy(serverDirectory);

        /* read an analog measurement value from server */
        MmsValue* value = IedConnection_readObject(con, &error, "simpleIOGenericIO/GGIO1.AnIn1.mag.f", IEC61850_FC_MX);

        if (value != NULL) {
            float fval = MmsValue_toFloat(value);
            printf("read float value: %f\n", fval);
            MmsValue_delete(value);
        }

        /* write a variable to the server */
        value = MmsValue_newVisibleString("libiec61850.com");
        IedConnection_writeObject(con, &error, "simpleIOGenericIO/GGIO1.NamPlt.vendor", IEC61850_FC_DC, value);

        if (error != IED_ERROR_OK)
            printf("failed to write simpleIOGenericIO/GGIO1.NamPlt.vendor!\n");

        MmsValue_delete(value);


        /* read data set */
        ClientDataSet clientDataSet = IedConnection_readDataSetValues(con, &error, "simpleIOGenericIO/LLN0.Events", NULL);

        if (clientDataSet == NULL)
            printf("failed to read dataset\n");

        /* Read RCB values */
        ClientReportControlBlock rcb =
                IedConnection_getRCBValues(con, &error, "simpleIOGenericIO/LLN0.RP.EventsRCB01", NULL);


        bool rptEna = ClientReportControlBlock_getRptEna(rcb);

        printf("RptEna = %i\n", rptEna);

        /* Install handler for reports */
        IedConnection_installReportHandler(con, "simpleIOGenericIO/LLN0.RP.EventsRCB01",
                ClientReportControlBlock_getRptId(rcb), reportCallbackFunction, NULL);

        /* Set trigger options and enable report */
        ClientReportControlBlock_setTrgOps(rcb, TRG_OPT_DATA_UPDATE | TRG_OPT_INTEGRITY | TRG_OPT_GI);
        ClientReportControlBlock_setRptEna(rcb, true);
        ClientReportControlBlock_setIntgPd(rcb, 5000);
        IedConnection_setRCBValues(con, &error, rcb, RCB_ELEMENT_RPT_ENA | RCB_ELEMENT_TRG_OPS | RCB_ELEMENT_INTG_PD, true);

        if (error != IED_ERROR_OK)
            printf("report activation failed (code: %i)\n", error);

        Thread_sleep(1000);

        /* trigger GI report */
        ClientReportControlBlock_setGI(rcb, true);
        IedConnection_setRCBValues(con, &error, rcb, RCB_ELEMENT_GI, true);

        if (error != IED_ERROR_OK)
            printf("Error triggering a GI report (code: %i)\n", error);

        Thread_sleep(60000);

        /* disable reporting */
        ClientReportControlBlock_setRptEna(rcb, false);
        IedConnection_setRCBValues(con, &error, rcb, RCB_ELEMENT_RPT_ENA, true);

        if (error != IED_ERROR_OK)
            printf("disable reporting failed (code: %i)\n", error);

        ClientDataSet_destroy(clientDataSet);

        ClientReportControlBlock_destroy(rcb);

        close_connection:

        IedConnection_close(con);
    }
    else {
        printf("Failed to connect to %s\n", hostname);
    }

    IedConnection_destroy(con);

    TLSConfiguration_destroy(tlsConfig);
    return 0;
}


