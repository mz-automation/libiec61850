/*
 *  server_example_files.c
 *
 *  - How to use some special MMS file service features
 *  - How to control how files file services can be used
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

static MmsError
fileAccessHandler (void* parameter, MmsServerConnection connection, MmsFileServiceType service,
                                          const char* localFilename, const char* otherFilename)
{
    printf("fileAccessHandler: service = %i, local-file: %s other-file: %s\n", service, localFilename, otherFilename);

    /* Don't allow client to rename files */
    if (service == MMS_FILE_ACCESS_TYPE_RENAME)
        return MMS_ERROR_FILE_FILE_ACCESS_DENIED;

    /* Don't allow client to delete file "IEDSERVER.BIN" */
    if (service == MMS_FILE_ACCESS_TYPE_DELETE) {
        if (strcmp(localFilename, "IEDSERVER.BIN") == 0)
            return MMS_ERROR_FILE_FILE_ACCESS_DENIED;
    }

    /* allow all other accesses */
    return MMS_ERROR_NONE;
}

int
main(int argc, char** argv)
{
    int tcpPort = 102;

    if (argc > 1) {
        tcpPort = atoi(argv[1]);
    }

    printf("Using libIEC61850 version %s\n", LibIEC61850_getVersionString());

    iedServer = IedServer_create(&iedModel);

    /* Set the base path for the MMS file services */
    MmsServer mmsServer = IedServer_getMmsServer(iedServer);

    IedServer_setFilestoreBasepath(iedServer, "./vmd-filestore/");

    /* Set a callback handler to control file accesses */
    MmsServer_installFileAccessHandler(mmsServer, fileAccessHandler, NULL);

    IedServer_setConnectionIndicationHandler(iedServer, (IedConnectionIndicationHandler) connectionHandler, NULL);

    /* MMS server will be instructed to start listening to client connections. */
    IedServer_start(iedServer, tcpPort);

    if (!IedServer_isRunning(iedServer)) {
        printf("Starting server failed! Exit.\n");
        IedServer_destroy(iedServer);
        exit(-1);
    }

    running = 1;

    signal(SIGINT, sigint_handler);


    while (running)
        Thread_sleep(100);


    /* stop MMS server - close TCP server socket and all client sockets */
    IedServer_stop(iedServer);

    /* Cleanup - free all resources */
    IedServer_destroy(iedServer);
    return 0;

} /* main() */
