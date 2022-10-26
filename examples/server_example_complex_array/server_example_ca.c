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

static int running = 0;

static void
sigint_handler(int signalId)
{
    running = 0;
}

static void
updateCMVArrayElement(IedServer server, DataObject* phsAHar, int idx, float magnitude, float angle, Quality quality, Timestamp timestamp)
{
    DataObject* phsAHarArrayElem = (DataObject*)ModelNode_getChildWithIdx((ModelNode*)phsAHar, idx);

    if (phsAHarArrayElem) {

        DataAttribute* mag = (DataAttribute*)ModelNode_getChild((ModelNode*)phsAHarArrayElem, "cVal.mag.f");
        DataAttribute* ang = (DataAttribute*)ModelNode_getChild((ModelNode*)phsAHarArrayElem, "cVal.ang.f");
        DataAttribute* q = (DataAttribute*)ModelNode_getChild((ModelNode*)phsAHarArrayElem, "q");
        DataAttribute* t = (DataAttribute*)ModelNode_getChild((ModelNode*)phsAHarArrayElem, "t");

        if (mag && ang && q && t) {
            IedServer_updateQuality(server, q, quality);
            IedServer_updateTimestampAttributeValue(server, t, &timestamp);
            IedServer_updateFloatAttributeValue(server, mag, magnitude);
            IedServer_updateFloatAttributeValue(server, ang, angle);
        }
        else {
            printf("one of mag, ang, q, t not found\n");
        }
    }
    else {
        printf("Element with index %i not found\n", idx);
    }
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
            IedModel_getModelNodeByShortObjectReference(&iedModel, "ComplexArray/MHAI1.HA.phsAHar");

    /* assuming the array has 16 elements */
    float mag = 200.f;
    float angle = 0.01f;

    Quality quality = QUALITY_VALIDITY_GOOD;
    Timestamp timestamp;

    Timestamp_setTimeInMilliseconds(&timestamp, Hal_getTimeInMs());

    int i;
    for (i = 0; i < 16; i++) {
        updateCMVArrayElement(iedServer, mhai1_ha_phsAHar, i, mag, angle, quality, timestamp);
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
            updateCMVArrayElement(iedServer, mhai1_ha_phsAHar, i, mag, angle, quality, timestamp);
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
