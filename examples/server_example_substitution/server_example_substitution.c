/*
 *  server_example_substitution.c
 *
 *  - How to use the IEC 61850 substitution service
 *  - Two data objects can be substituted:
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

/* import IEC 61850 device model created from SCL-File */
extern IedModel iedModel;

static int running = 0;
static IedServer iedServer = NULL;

static bool subsAnIn1 = false;
static bool subsInd1 = false;

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
            subsAnIn1 = false;
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
            subsInd1 = false;
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

    return DATA_ACCESS_ERROR_SUCCESS;
}

int
main(int argc, char** argv)
{
    printf("Using libIEC61850 version %s\n", LibIEC61850_getVersionString());




    /* Create a new IEC 61850 server instance */
    iedServer = IedServer_create(&iedModel);

    IedServer_setConnectionIndicationHandler(iedServer, (IedConnectionIndicationHandler) connectionHandler, NULL);

    /* Install write callback handler for substitution variables */

    IedServer_handleWriteAccess(iedServer, IEDMODEL_LD1_GGIO1_AnIn1_subEna, writeAccessHandler, NULL);
    IedServer_handleWriteAccess(iedServer, IEDMODEL_LD1_GGIO1_AnIn1_subMag_f, writeAccessHandler, NULL);
    IedServer_handleWriteAccess(iedServer, IEDMODEL_LD1_GGIO1_AnIn1_subQ, writeAccessHandler, NULL);

    IedServer_handleWriteAccess(iedServer, IEDMODEL_LD1_GGIO1_Ind1_subEna, writeAccessHandler, NULL);
    IedServer_handleWriteAccess(iedServer, IEDMODEL_LD1_GGIO1_Ind1_subVal, writeAccessHandler, NULL);
    IedServer_handleWriteAccess(iedServer, IEDMODEL_LD1_GGIO1_Ind1_subQ, writeAccessHandler, NULL);

    /* MMS server will be instructed to start listening for client connections. */
    IedServer_start(iedServer, 102);

    if (!IedServer_isRunning(iedServer)) {
        printf("Starting server failed! Exit.\n");
        IedServer_destroy(iedServer);
        exit(-1);
    }

    running = 1;

    signal(SIGINT, sigint_handler);

    float t = 0.f;
    bool ind1 = true;

    while (running) {
        uint64_t timestamp = Hal_getTimeInMs();

        t += 0.1f;

        float an1 = sinf(t);

        if (ind1)
            ind1 = false;
        else
            ind1 = true;

        Timestamp iecTimestamp;

        Timestamp_clearFlags(&iecTimestamp);
        Timestamp_setTimeInMilliseconds(&iecTimestamp, timestamp);
        Timestamp_setLeapSecondKnown(&iecTimestamp, true);


        /* toggle clock-not-synchronized flag in timestamp */
        if (((int) t % 2) == 0)
            Timestamp_setClockNotSynchronized(&iecTimestamp, true);

        IedServer_lockDataModel(iedServer);

        if (subsAnIn1 == false) {
            IedServer_updateTimestampAttributeValue(iedServer, IEDMODEL_LD1_GGIO1_AnIn1_t, &iecTimestamp);
            IedServer_updateQuality(iedServer, IEDMODEL_LD1_GGIO1_AnIn1_q, QUALITY_VALIDITY_GOOD);
            IedServer_updateFloatAttributeValue(iedServer, IEDMODEL_LD1_GGIO1_AnIn1_mag_f, an1);
        }

        if (subsInd1 == false) {
            IedServer_updateTimestampAttributeValue(iedServer, IEDMODEL_LD1_GGIO1_Ind1_t, &iecTimestamp);
            IedServer_updateQuality(iedServer, IEDMODEL_LD1_GGIO1_Ind1_q, QUALITY_VALIDITY_GOOD);
            IedServer_updateBooleanAttributeValue(iedServer, IEDMODEL_LD1_GGIO1_Ind1_stVal, ind1);
        }

        IedServer_unlockDataModel(iedServer);

        Thread_sleep(100);
    }

    /* stop MMS server - close TCP server socket and all client sockets */
    IedServer_stop(iedServer);

    /* Cleanup - free all resources */
    IedServer_destroy(iedServer);

} /* main() */
