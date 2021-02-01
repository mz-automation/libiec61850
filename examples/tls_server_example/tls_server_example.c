/*
 *  tls_server_example.c
 *
 *  How to configure a TLS server
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

static void
printAppTitle(ItuObjectIdentifier* oid)
{
    int i;

    for (i = 0; i < oid->arcCount; i++) {
        printf("%i", oid->arc[i]);

        if (i != (oid->arcCount - 1))
            printf(".");
    }
}

static bool
clientAuthenticator(void* parameter, AcseAuthenticationParameter authParameter, void** securityToken, IsoApplicationReference* appRef)
{
    printf("ACSE Authenticator:\n");
    printf("  client ap-title: "); printAppTitle(&(appRef->apTitle)); printf("\n");
    printf("  client ae-qualifier: %i\n", appRef->aeQualifier);
    printf("  auth-mechanism: %i\n", authParameter->mechanism);

    return true;
}

int
main(int argc, char** argv)
{
    int port_number = 8102;
    if (argc > 1)
        port_number = atoi(argv[1]);

    printf("Using libIEC61850 version %s\n", LibIEC61850_getVersionString());
    printf("libIEC61850 IedServer server will listen on %d\n", port_number);

    TLSConfiguration tlsConfig = TLSConfiguration_create();

    TLSConfiguration_setChainValidation(tlsConfig, false);
    TLSConfiguration_setAllowOnlyKnownCertificates(tlsConfig, true);

    if (!TLSConfiguration_setOwnKeyFromFile(tlsConfig, "server-key.pem", NULL)) {
        printf("Failed to load private key!\n");
        return 0;
    }

    if (!TLSConfiguration_setOwnCertificateFromFile(tlsConfig, "server.cer")) {
        printf("ERROR: Failed to load own certificate!\n");
        return 0;
    }

    if (!TLSConfiguration_addCACertificateFromFile(tlsConfig, "root.cer")) {
        printf("ERROR: Failed to load root certificate\n");
        return 0;
    }

    /**
     * Configure two allowed clients
     */

    if (!TLSConfiguration_addAllowedCertificateFromFile(tlsConfig, "client1.cer")) {
        printf("ERROR: Failed to load allowed client certificate\n");
        return 0;
    }

    if (!TLSConfiguration_addAllowedCertificateFromFile(tlsConfig, "client2.cer")) {
        printf("ERROR: Failed to load allowed client certificate\n");
        return 0;
    }

    // Create Model IED
    IedModel* myModel = IedModel_create("testmodel");

    // Create Logical Device
    LogicalDevice* lDevice1 = LogicalDevice_create("SENSORS", myModel);

    // Create Logical Node
    LogicalNode* ttmp1 = LogicalNode_create("TTMP1", lDevice1);

    // Create Model Node
    CDC_ASG_create("TmpSp", (ModelNode*) ttmp1, 0, false);
    CDC_VSG_create("TmpSt", (ModelNode*) ttmp1, 0);

    // Create Data Object
    DataObject* do1 = DataObject_create("Temp1", (ModelNode*) ttmp1, 0);

    // Create Data Attribute
    DataAttribute* fl = DataAttribute_create("float", (ModelNode*) do1, IEC61850_FLOAT64, IEC61850_FC_MX, 0, 0, 0);
    DataAttribute* st = DataAttribute_create("string", (ModelNode*) do1, IEC61850_VISIBLE_STRING_255, IEC61850_FC_DC,0, 0, 0);

    // Create Server Connection
    iedServer = IedServer_createWithTlsSupport(&iedModel, tlsConfig);

    IedServer_setAuthenticator(iedServer, clientAuthenticator, NULL);

    IedServer_setWriteAccessPolicy(iedServer, IEC61850_FC_DC, ACCESS_POLICY_ALLOW);

    /* MMS server will be instructed to start listening to client connections. */
    IedServer_start(iedServer, port_number);

    if (!IedServer_isRunning(iedServer)) {
        printf("Starting server failed! Exit.\n");
        IedServer_destroy(iedServer);
        exit(-1);
    }

    running = 1;

    signal(SIGINT, sigint_handler);

    float t = 0.f;

    while (running) {
        uint64_t timestamp = Hal_getTimeInMs();

        t += 0.1f;

        float an1 = 0.0001;
        float an2 = sinf(t + 1.f);
        float an3 = sinf(t + 2.f);
        float an4 = sinf(t + 3.f);

        IedServer_lockDataModel(iedServer);

        Timestamp iecTimestamp;

        Timestamp_clearFlags(&iecTimestamp);
        Timestamp_setTimeInMilliseconds(&iecTimestamp, timestamp);
        Timestamp_setLeapSecondKnown(&iecTimestamp, true);

        /* toggle clock-not-synchronized flag in timestamp */
        if (((int) t % 2) == 0)
            Timestamp_setClockNotSynchronized(&iecTimestamp, true);

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

    TLSConfiguration_destroy(tlsConfig);

} /* main() */
