/*
 *  server_example_substitution.c
 *
 *  - How to use the IEC 61850 substitution service
 *  - How to use the blocking service
 *  - Two data objects can be substituted and/or blocked:
 *  -- GGIO1.AnIn1
 *  -- GGIO1.Ind1
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

static bool subsAnIn1 = false;
static bool subsInd1 = false;
static bool blkEnaAnIn1 = false;
static bool blkEnaInd1 = false;

static float an1 = 0.f;
static uint64_t timestamp = 0;
static bool ind1 = true;

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

static void
updateProcessValues()
{
    Timestamp iecTimestamp;

    Timestamp_clearFlags(&iecTimestamp);
    Timestamp_setTimeInMilliseconds(&iecTimestamp, timestamp);
    Timestamp_setLeapSecondKnown(&iecTimestamp, true);

    if ((subsAnIn1 == false) && (blkEnaAnIn1 == false)) {
        IedServer_updateTimestampAttributeValue(iedServer, IEDMODEL_LD1_GGIO1_AnIn1_t, &iecTimestamp);
        IedServer_updateQuality(iedServer, IEDMODEL_LD1_GGIO1_AnIn1_q, QUALITY_VALIDITY_GOOD);
        IedServer_updateFloatAttributeValue(iedServer, IEDMODEL_LD1_GGIO1_AnIn1_mag_f, an1);
    }

    if ((subsInd1 == false) && (blkEnaInd1 == false)) {
        IedServer_updateTimestampAttributeValue(iedServer, IEDMODEL_LD1_GGIO1_Ind1_t, &iecTimestamp);
        IedServer_updateQuality(iedServer, IEDMODEL_LD1_GGIO1_Ind1_q, QUALITY_VALIDITY_GOOD);
        IedServer_updateBooleanAttributeValue(iedServer, IEDMODEL_LD1_GGIO1_Ind1_stVal, ind1);
    }
}

static MmsDataAccessError
writeAccessHandler (DataAttribute* dataAttribute, MmsValue* value, ClientConnection connection, void* parameter)
{
    if (dataAttribute == IEDMODEL_LD1_GGIO1_AnIn1_subEna) {

        printf("Received GGIO1.AnIn1.subEna: %i\n", MmsValue_getBoolean(value));

        if (MmsValue_getBoolean(value)) {
            subsAnIn1 = true;

            /* Update values with substituted values */

            Quality quality =
                    Quality_fromMmsValue(IedServer_getAttributeValue(iedServer, IEDMODEL_LD1_GGIO1_AnIn1_subQ));

            Quality_setFlag(&quality, QUALITY_SOURCE_SUBSTITUTED);

            IedServer_updateQuality(iedServer, IEDMODEL_LD1_GGIO1_AnIn1_q, quality);

            IedServer_updateAttributeValue(iedServer, IEDMODEL_LD1_GGIO1_AnIn1_mag_f,
                    IedServer_getAttributeValue(iedServer, IEDMODEL_LD1_GGIO1_AnIn1_subMag_f));
        }
        else {
            IedServer_updateVisibleStringAttributeValue(iedServer, IEDMODEL_LD1_GGIO1_AnIn1_subID, "");

            subsAnIn1 = false;

            updateProcessValues();
        }

    }
    else if (dataAttribute == IEDMODEL_LD1_GGIO1_AnIn1_subMag_f) {

        if (subsAnIn1) {
            IedServer_updateAttributeValue(iedServer, IEDMODEL_LD1_GGIO1_AnIn1_mag_f, value);
        }

    }
    else if (dataAttribute == IEDMODEL_LD1_GGIO1_AnIn1_subQ) {

        if (subsAnIn1) {
            Quality quality = Quality_fromMmsValue(value);

            Quality_setFlag(&quality, QUALITY_SOURCE_SUBSTITUTED);

            IedServer_updateQuality(iedServer, IEDMODEL_LD1_GGIO1_AnIn1_q, quality);
        }

    }
    else if (dataAttribute == IEDMODEL_LD1_GGIO1_AnIn1_blkEna) {
        printf("Received GGIO1.AnIn1.blkEna: %i\n", MmsValue_getBoolean(value));

        blkEnaAnIn1 = MmsValue_getBoolean(value);

        /* Update quality flags */

        Quality quality =
                Quality_fromMmsValue(IedServer_getAttributeValue(iedServer, IEDMODEL_LD1_GGIO1_AnIn1_q));

        if (blkEnaAnIn1)
            Quality_setFlag(&quality, QUALITY_OPERATOR_BLOCKED);
        else
            Quality_unsetFlag(&quality, QUALITY_OPERATOR_BLOCKED);

        IedServer_updateQuality(iedServer, IEDMODEL_LD1_GGIO1_AnIn1_q, quality);
    }
    else if (dataAttribute == IEDMODEL_LD1_GGIO1_Ind1_subEna) {
        printf("Received GGIO1.Ind1.subEna: %i\n", MmsValue_getBoolean(value));

        if (MmsValue_getBoolean(value)) {
            subsInd1 = true;

            /* Update values with substituted values */

            Quality quality =
                    Quality_fromMmsValue(IedServer_getAttributeValue(iedServer, IEDMODEL_LD1_GGIO1_Ind1_subQ));

            Quality_setFlag(&quality, QUALITY_SOURCE_SUBSTITUTED);

            IedServer_updateQuality(iedServer, IEDMODEL_LD1_GGIO1_Ind1_q, quality);

            IedServer_updateAttributeValue(iedServer, IEDMODEL_LD1_GGIO1_Ind1_stVal,
                    IedServer_getAttributeValue(iedServer, IEDMODEL_LD1_GGIO1_Ind1_subVal));
        }
        else {
            IedServer_updateVisibleStringAttributeValue(iedServer, IEDMODEL_LD1_GGIO1_Ind1_subID, "");

            subsInd1 = false;

            updateProcessValues();
        }
    }
    else if (dataAttribute == IEDMODEL_LD1_GGIO1_Ind1_subVal) {

        if (subsInd1) {
            IedServer_updateAttributeValue(iedServer, IEDMODEL_LD1_GGIO1_Ind1_stVal, value);
        }

    }
    else if (dataAttribute == IEDMODEL_LD1_GGIO1_Ind1_subQ) {

        if (subsInd1) {
            Quality quality = Quality_fromMmsValue(value);

            Quality_setFlag(&quality, QUALITY_SOURCE_SUBSTITUTED);

            IedServer_updateQuality(iedServer, IEDMODEL_LD1_GGIO1_Ind1_q, quality);
        }

    }
    else if (dataAttribute == IEDMODEL_LD1_GGIO1_Ind1_blkEna) {
        printf("Received GGIO1.Ind1.blkEna: %i\n", MmsValue_getBoolean(value));

        blkEnaInd1 = MmsValue_getBoolean(value);

        /* Update quality flags */

        Quality quality =
                Quality_fromMmsValue(IedServer_getAttributeValue(iedServer, IEDMODEL_LD1_GGIO1_Ind1_q));

        if (blkEnaAnIn1)
            Quality_setFlag(&quality, QUALITY_OPERATOR_BLOCKED);
        else
            Quality_unsetFlag(&quality, QUALITY_OPERATOR_BLOCKED);

        IedServer_updateQuality(iedServer, IEDMODEL_LD1_GGIO1_Ind1_q, quality);
    }

    return DATA_ACCESS_ERROR_SUCCESS;
}

int
main(int argc, char** argv)
{
    int tcpPort = 102;

    if (argc > 1) {
        tcpPort = atoi(argv[1]);
    }

    printf("Using libIEC61850 version %s\n", LibIEC61850_getVersionString());

    /* Create a new IEC 61850 server instance */
    iedServer = IedServer_create(&iedModel);

    IedServer_setConnectionIndicationHandler(iedServer, (IedConnectionIndicationHandler) connectionHandler, NULL);

    /* Install write callback handler for substitution variables */

    IedServer_handleWriteAccess(iedServer, IEDMODEL_LD1_GGIO1_AnIn1_subEna, writeAccessHandler, NULL);
    IedServer_handleWriteAccess(iedServer, IEDMODEL_LD1_GGIO1_AnIn1_subMag_f, writeAccessHandler, NULL);
    IedServer_handleWriteAccess(iedServer, IEDMODEL_LD1_GGIO1_AnIn1_subQ, writeAccessHandler, NULL);
    IedServer_handleWriteAccess(iedServer, IEDMODEL_LD1_GGIO1_AnIn1_blkEna, writeAccessHandler, NULL);

    IedServer_handleWriteAccess(iedServer, IEDMODEL_LD1_GGIO1_Ind1_subEna, writeAccessHandler, NULL);
    IedServer_handleWriteAccess(iedServer, IEDMODEL_LD1_GGIO1_Ind1_subVal, writeAccessHandler, NULL);
    IedServer_handleWriteAccess(iedServer, IEDMODEL_LD1_GGIO1_Ind1_subQ, writeAccessHandler, NULL);
    IedServer_handleWriteAccess(iedServer, IEDMODEL_LD1_GGIO1_Ind1_blkEna, writeAccessHandler, NULL);

    /* MMS server will be instructed to start listening for client connections. */
    IedServer_start(iedServer, tcpPort);

    if (!IedServer_isRunning(iedServer)) {
        printf("Starting server failed! Exit.\n");
        IedServer_destroy(iedServer);
        exit(-1);
    }

    running = 1;

    signal(SIGINT, sigint_handler);

    float t = 0.f;

    while (running) {
        timestamp = Hal_getTimeInMs();

        t += 0.1f;

        an1 = sinf(t);

        if (ind1)
            ind1 = false;
        else
            ind1 = true;

        IedServer_lockDataModel(iedServer);

        updateProcessValues();

        IedServer_unlockDataModel(iedServer);

        Thread_sleep(100);
    }

    /* stop MMS server - close TCP server socket and all client sockets */
    IedServer_stop(iedServer);

    /* Cleanup - free all resources */
    IedServer_destroy(iedServer);

    return 0;
} /* main() */
