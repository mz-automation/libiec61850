/*
 *  server_example_ca.c
 *
 *  This example shows how to handle complex arrays (arrays of data objects).
 */

#include "iec61850_server.h"
#include "hal_thread.h"
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#include "static_model.h"

#include <assert.h>

/* import IEC 61850 device model created from ICD-File */
extern IedModel iedModel;

static int running = 0;

static void
sigint_handler(int signalId)
{
    running = 0;
}

static void
updateCMVArrayElement(MmsValue *cval, int index, float magnitude, float angle, Quality quality, Timestamp timestamp)
{
    MmsValue* cmv = MmsValue_getElement(cval, index);

    MmsValue* cValElement = MmsValue_getElement(cmv, 0);

    assert(MmsValue_getArraySize(cValElement) == 2);

    MmsValue* cValElement_mag_f = MmsValue_getElement(MmsValue_getElement(cValElement, 0), 0);

    MmsValue* cValElement_ang_f = MmsValue_getElement(MmsValue_getElement(cValElement, 1), 0);

    MmsValue_setFloat(cValElement_mag_f, magnitude);
    MmsValue_setFloat(cValElement_ang_f, angle);

    MmsValue* q = MmsValue_getElement(cmv, 1);
    MmsValue_setBitStringFromInteger(q, (int) quality);

    MmsValue* t = MmsValue_getElement(cmv, 2);
    MmsValue_setUtcTimeByBuffer(t, timestamp.val);
}

int
main(int argc, char **argv)
{

    int tcpPort = 102;

    if (argc > 1) {
        tcpPort = atoi(argv[1]);
    }

    IedServer iedServer = IedServer_create(&iedModel);

    /* Get access to the MHAI1.HA data object handle - for static and dynamic model*/
    DataObject* mhai1_ha_phsAHar = (DataObject*)
            IedModel_getModelNodeByObjectReference(&iedModel, "testComplexArray/MHAI1.HA.phsAHar");

    /* Get access to the corresponding MMS value data structure - the MX(FC) part of the data object */
    MmsValue* mhai1_ha_phsAHar_mx = IedServer_getFunctionalConstrainedData(iedServer, mhai1_ha_phsAHar, IEC61850_FC_MX);

    /* assuming the array has 16 elements */
    float mag = 200.f;
    float angle = 0.01f;

    Quality quality = QUALITY_VALIDITY_GOOD;
    Timestamp timestamp;

    Timestamp_setTimeInMilliseconds(&timestamp, Hal_getTimeInMs());

    int i;
    for (i = 0; i < 16; i++) {
        updateCMVArrayElement(mhai1_ha_phsAHar_mx, i, mag, angle, quality, timestamp);
        mag += 1.f;
        angle += 0.01f;
    }

    /* MMS server will be instructed to start listening to client connections. */
    IedServer_start(iedServer, tcpPort);

    if (!IedServer_isRunning(iedServer)) {
        printf("Starting server failed! Exit.\n");
        IedServer_destroy(iedServer);
        exit(-1);
    }

    running = 1;

    signal(SIGINT, sigint_handler);

    int counter = 0;

    while (running) {
        Thread_sleep(1000);

        Timestamp_setTimeInMilliseconds(&timestamp, Hal_getTimeInMs());

        IedServer_lockDataModel(iedServer);
        for (i = 0; i < 16; i++) {
            updateCMVArrayElement(mhai1_ha_phsAHar_mx, i, mag, angle, quality, timestamp);
            mag += 0.1f;
            angle += 0.05f;
        }
        IedServer_unlockDataModel(iedServer);

        if (counter == 10) {
            /* Now a problem occurs - measurements are invalid */
            quality = QUALITY_VALIDITY_INVALID | QUALITY_DETAIL_FAILURE;
        }

        counter++;
    }

    /* stop MMS server - close TCP server socket and all client sockets */
    IedServer_stop(iedServer);

    /* Cleanup - free all resources */
    IedServer_destroy(iedServer);

    return 0;
} /* main() */
