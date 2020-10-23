/*
 *  server_example_write_handler.c
 */

#include "iec61850_server.h"
#include "hal_thread.h"
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#include "static_model.h"

/* import IEC 61850 device model created from SCL-File */
extern IedModel iedModel;

static int running = 0;
static IedServer iedServer = NULL;

void sigint_handler(int signalId)
{
    running = 0;
}

static MmsDataAccessError
writeAccessHandler (DataAttribute* dataAttribute, MmsValue* value, ClientConnection connection, void* parameter)
{
    if (dataAttribute == IEDMODEL_Inverter_ZINV1_OutVarSet_setMag_f) {

        float newValue = MmsValue_toFloat(value);

        printf("New value for OutVarSet_setMag_f = %f\n", newValue);

        /* Check if value is inside of valid range */
        if ((newValue >= 0.f) && (newValue <= 1000.1f))
            return DATA_ACCESS_ERROR_SUCCESS;
        else
            return DATA_ACCESS_ERROR_OBJECT_VALUE_INVALID;

    }

    return DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED;
}

int
main(int argc, char** argv)
{
    iedServer = IedServer_create(&iedModel);

    /* MMS server will be instructed to start listening to client connections. */
    IedServer_start(iedServer, 102);

    /* Don't allow access to SP variables by default */
    IedServer_setWriteAccessPolicy(iedServer, IEC61850_FC_SP, ACCESS_POLICY_DENY);

    /* Instruct the server that we will be informed if a clients writes to a
     * certain variables we are interested in.
     */
    IedServer_handleWriteAccess(iedServer, IEDMODEL_Inverter_ZINV1_OutVarSet_setMag_f, writeAccessHandler, NULL);

    if (!IedServer_isRunning(iedServer)) {
        printf("Starting server failed! Exit.\n");
        IedServer_destroy(iedServer);
        exit(-1);
    }

    running = 1;

    signal(SIGINT, sigint_handler);

    while (running) {
        Thread_sleep(1);
    }

    /* stop MMS server - close TCP server socket and all client sockets */
    IedServer_stop(iedServer);

    /* Cleanup - free all resources */
    IedServer_destroy(iedServer);

    return 0;
} /* main() */
