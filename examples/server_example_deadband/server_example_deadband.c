/*
 *  server_example_deadband.c
 *
 *  - How to handle dead bands for measured values (MV)
 *  - Use variable and fixed dead band with db and dbRef (edition 2.1)
 *  - Use fixed dead band with db and rangeC (edition 2)
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

    /* disable MMS file service */
    IedServerConfig_enableFileService(config, false);

    /* enable dynamic data set service */
    IedServerConfig_enableDynamicDataSetService(config, true);

    /* disable log service */
    IedServerConfig_enableLogService(config, false);

    /* set maximum number of clients */
    IedServerConfig_setMaxMmsConnections(config, 5);

    /* Create a new IEC 61850 server instance */
    iedServer = IedServer_createWithConfig(&iedModel, NULL, config);

    /* configuration object is no longer required */
    IedServerConfig_destroy(config);

    /* set the identity values for MMS identify service */
    IedServer_setServerIdentity(iedServer, "libiec61850.com", "deadband example", "1.5.0");

    IedServer_setConnectionIndicationHandler(iedServer, (IedConnectionIndicationHandler) connectionHandler, NULL);

    /* Allow write access to CF parameters (here "db" and "rangeC") */
    IedServer_setWriteAccessPolicy(iedServer, IEC61850_FC_CF, ACCESS_POLICY_ALLOW);

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

    float an1InstMag = 5 * sinf(t);
    float an2InstMag = 5 * sinf(t + 1.f);
    float an3InstMag = 5 * sinf(t + 2.f);
    float an4InstMag = 5 * sinf(t + 3.f);

    float an1Mag = an1InstMag;
    float an2Mag = an2InstMag;
    float an3Mag = an3InstMag;
    float an4Mag = an4InstMag;

    float anIn1DbRef = MmsValue_toFloat(IedServer_getAttributeValue(iedServer, IEDMODEL_LD1_AnInGGIO1_AnIn1_dbRef));
    float anIn1ZeroDbRef = MmsValue_toFloat(IedServer_getAttributeValue(iedServer, IEDMODEL_LD1_AnInGGIO1_AnIn1_zeroDbRef));
    float anIn2DbRef = MmsValue_toFloat(IedServer_getAttributeValue(iedServer, IEDMODEL_LD1_AnInGGIO1_AnIn2_dbRef));
    float anIn2ZeroDbRef = MmsValue_toFloat(IedServer_getAttributeValue(iedServer, IEDMODEL_LD1_AnInGGIO1_AnIn2_zeroDbRef));

    while (running) {
        uint64_t timestamp = Hal_getTimeInMs();

        t += 0.001f;

        an1InstMag = 5 * sinf(t);
        an2InstMag = 5 * sinf(t + 1.f);
        an3InstMag = 5 * sinf(t + 2.f);
        an4InstMag = 5 * sinf(t + 3.f);

        Timestamp iecTimestamp;

        Timestamp_clearFlags(&iecTimestamp);
        Timestamp_setTimeInMilliseconds(&iecTimestamp, timestamp);
        Timestamp_setLeapSecondKnown(&iecTimestamp, true);

        float dbValF;
        float minValF;
        float maxValF;

        IedServer_lockDataModel(iedServer);

        /* handle AnIn1 (using db and dbRef as in edition 2.1)  */

        dbValF = (float)MmsValue_toUint32(IedServer_getAttributeValue(iedServer, IEDMODEL_LD1_AnInGGIO1_AnIn1_db)) / 1000.f;

        IedServer_updateFloatAttributeValue(iedServer, IEDMODEL_LD1_AnInGGIO1_AnIn1_instMag_f, an1InstMag);

        if (anIn1DbRef == 0) { /* dbRef = 0 */
            if (fabsf(an1InstMag - an1Mag) > (fabsf(an1Mag) * dbValF * 0.01f)) {
                /* dead band condition -> updated "mag" attribute */
                an1Mag = an1InstMag;
                IedServer_updateFloatAttributeValue(iedServer, IEDMODEL_LD1_AnInGGIO1_AnIn1_mag_f, an1Mag);
            }
        }
        else {
            if (fabsf(an1InstMag - an1Mag) > (fabsf(anIn1DbRef) * dbValF * 0.01f)) {
                /* dead band condition -> updated "mag" attribute */
                an1Mag = an1InstMag;
                IedServer_updateFloatAttributeValue(iedServer, IEDMODEL_LD1_AnInGGIO1_AnIn1_mag_f, an1Mag);
            }
        }

        IedServer_updateTimestampAttributeValue(iedServer, IEDMODEL_LD1_AnInGGIO1_AnIn1_t, &iecTimestamp);

        /* handle AnIn2 (using db and dbRef as in edition 2.1) */

        dbValF = (float)MmsValue_toUint32(IedServer_getAttributeValue(iedServer, IEDMODEL_LD1_AnInGGIO1_AnIn2_db)) / 1000.f;

        IedServer_updateFloatAttributeValue(iedServer, IEDMODEL_LD1_AnInGGIO1_AnIn2_instMag_f, an2InstMag);

        if (anIn1DbRef == 0) { /* dbRef = 0 */
            if (fabsf(an2InstMag - an2Mag) > (fabsf(an2Mag) * dbValF * 0.01f)) {
                /* dead band condition -> updated "mag" attribute */
                an2Mag = an2InstMag;
                IedServer_updateFloatAttributeValue(iedServer, IEDMODEL_LD1_AnInGGIO1_AnIn2_mag_f, an2Mag);
            }
        }
        else {
            if (fabsf(an2InstMag - an2Mag) > (fabsf(anIn2DbRef) * dbValF * 0.01f)) {
                /* dead band condition -> updated "mag" attribute */
                an2Mag = an2InstMag;
                IedServer_updateFloatAttributeValue(iedServer, IEDMODEL_LD1_AnInGGIO1_AnIn2_mag_f, an2Mag);
            }
        }

        IedServer_updateTimestampAttributeValue(iedServer, IEDMODEL_LD1_AnInGGIO1_AnIn2_t, &iecTimestamp);

        /* handle AnIn3 (using db and rangeC as in edition 2) */

        dbValF = (float)MmsValue_toUint32(IedServer_getAttributeValue(iedServer, IEDMODEL_LD1_AnInGGIO1_AnIn3_db)) / 1000.f;
        minValF = MmsValue_toFloat(IedServer_getAttributeValue(iedServer, IEDMODEL_LD1_AnInGGIO1_AnIn3_rangeC_min_f));
        maxValF = MmsValue_toFloat(IedServer_getAttributeValue(iedServer, IEDMODEL_LD1_AnInGGIO1_AnIn3_rangeC_max_f));

        IedServer_updateFloatAttributeValue(iedServer, IEDMODEL_LD1_AnInGGIO1_AnIn3_instMag_f, an3InstMag);

        if (fabsf(an3InstMag - an3Mag) > (fabsf(maxValF - minValF) * dbValF * 0.01f)) {
            /* dead band condition -> updated "mag" attribute */
            an3Mag = an3InstMag;
            IedServer_updateFloatAttributeValue(iedServer, IEDMODEL_LD1_AnInGGIO1_AnIn3_mag_f, an3Mag);
        }

        IedServer_updateTimestampAttributeValue(iedServer, IEDMODEL_LD1_AnInGGIO1_AnIn3_t, &iecTimestamp);

        /* handle AnIn4 (using db and rangeC as in edition 2) */

        IedServer_updateFloatAttributeValue(iedServer, IEDMODEL_LD1_AnInGGIO1_AnIn4_instMag_f, an4InstMag);

        dbValF = (float)MmsValue_toUint32(IedServer_getAttributeValue(iedServer, IEDMODEL_LD1_AnInGGIO1_AnIn4_db)) / 1000.f;
        minValF = MmsValue_toFloat(IedServer_getAttributeValue(iedServer, IEDMODEL_LD1_AnInGGIO1_AnIn4_rangeC_min_f));
        maxValF = MmsValue_toFloat(IedServer_getAttributeValue(iedServer, IEDMODEL_LD1_AnInGGIO1_AnIn4_rangeC_max_f));

        if (fabsf(an4InstMag - an4Mag) > (fabsf(maxValF - minValF) * dbValF * 0.01f)) {
            /* dead band condition -> updated "mag" attribute */
            an4Mag = an4InstMag;
            IedServer_updateFloatAttributeValue(iedServer, IEDMODEL_LD1_AnInGGIO1_AnIn4_mag_f, an4Mag);
        }

        IedServer_updateTimestampAttributeValue(iedServer, IEDMODEL_LD1_AnInGGIO1_AnIn4_t, &iecTimestamp);

        IedServer_unlockDataModel(iedServer);

        Thread_sleep(10);
    }

    /* stop MMS server - close TCP server socket and all client sockets */
    IedServer_stop(iedServer);

    /* Cleanup - free all resources */
    IedServer_destroy(iedServer);

    return 0;
} /* main() */
