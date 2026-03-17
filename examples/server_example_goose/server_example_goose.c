/*
 *  server_example_goose.c
 *
 *  This example demonstrates how to use GOOSE publishing, Reporting and the
 *  control model.
 *
 *  MMS can be optionally disabled to demonstrate GOOSE-only mode.
 *  In this case, the server will not listen for client connections and will only publish GOOSE messages.
 */

#include "hal_thread.h" /* for Thread_sleep() */
#include "iec61850_server.h"
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "static_model.h"

static int running = 0;
static IedServer iedServer = NULL;

static void
sigint_handler(int signalId)
{
    running = 0;
}

static void
controlHandlerForBinaryOutput(ControlAction action, void* parameter, MmsValue* value)
{
    uint64_t timestamp = Hal_getTimeInMs();

    if (parameter == IEDMODEL_GenericIO_GGIO1_SPCSO1)
    {
        IedServer_updateUTCTimeAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO1_t, timestamp);
        IedServer_updateAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO1_stVal, value);
    }

    if (parameter == IEDMODEL_GenericIO_GGIO1_SPCSO2)
    {
        IedServer_updateUTCTimeAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO2_t, timestamp);
        IedServer_updateAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO2_stVal, value);
    }

    if (parameter == IEDMODEL_GenericIO_GGIO1_SPCSO3)
    {
        IedServer_updateUTCTimeAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO3_t, timestamp);
        IedServer_updateAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO3_stVal, value);
    }

    if (parameter == IEDMODEL_GenericIO_GGIO1_SPCSO4)
    {
        IedServer_updateUTCTimeAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO4_t, timestamp);
        IedServer_updateAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO4_stVal, value);
    }
}

static void
goCbEventHandler(MmsGooseControlBlock goCb, int event, void* parameter)
{
    printf("Access to GoCB: %s\n", MmsGooseControlBlock_getName(goCb));
    printf("         GoEna: %i\n", MmsGooseControlBlock_getGoEna(goCb));
}

static void
printUsage(const char* progName)
{
    printf("Usage: %s [OPTIONS]\n", progName);
    printf("\n");
    printf("Options:\n");
    printf("  --ifc <interface>    Network interface for GOOSE publishing (all GCBs)\n");
    printf("  --ifc2 <interface>   Network interface for gcbAnalogValues GCB\n");
    printf("  --disable-mms        Disable the MMS server (GOOSE only mode)\n");
    printf("  --help               Show this help message\n");
}

int
main(int argc, char** argv)
{
    char* ifcId = NULL;
    char* ifc2Id = NULL;
    bool disableMms = false;

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--help") == 0)
        {
            printUsage(argv[0]);
            return 0;
        }
        else if (strcmp(argv[i], "--ifc") == 0)
        {
            if (i + 1 < argc)
                ifcId = argv[++i];
            else
            {
                fprintf(stderr, "Error: --ifc requires an interface name\n");
                return 1;
            }
        }
        else if (strcmp(argv[i], "--ifc2") == 0)
        {
            if (i + 1 < argc)
                ifc2Id = argv[++i];
            else
            {
                fprintf(stderr, "Error: --ifc2 requires an interface name\n");
                return 1;
            }
        }
        else if (strcmp(argv[i], "--disable-mms") == 0)
        {
            disableMms = true;
        }
        else
        {
            fprintf(stderr, "Unknown argument: %s\n", argv[i]);
            printUsage(argv[0]);
            return 1;
        }
    }

    IedServerConfig config = IedServerConfig_create();

    iedServer = IedServer_createWithConfig(&iedModel, NULL, config);

    IedServerConfig_destroy(config);

    if (ifcId)
    {
        printf("Using GOOSE interface: %s\n", ifcId);

        /* set GOOSE interface for all GOOSE publishers (GCBs) */
        IedServer_setGooseInterfaceId(iedServer, ifcId);
    }

    if (ifc2Id)
    {
        printf("Using GOOSE interface for GenericIO/LLN0.gcbAnalogValues: %s\n", ifc2Id);

        /* set GOOSE interface for a particular GOOSE publisher (GCB) */
        IedServer_setGooseInterfaceIdEx(iedServer, IEDMODEL_GenericIO_LLN0, "gcbAnalogValues", ifc2Id);
    }

    IedServer_setGoCBHandler(iedServer, goCbEventHandler, NULL);

    if (!disableMms)
    {
        /* MMS server will be instructed to start listening to client connections. */
        IedServer_start(iedServer, 102);
    }

    IedServer_startGoosePublishing(iedServer);

    IedServer_setControlHandler(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO1,
                                (ControlHandler)controlHandlerForBinaryOutput, IEDMODEL_GenericIO_GGIO1_SPCSO1);

    IedServer_setControlHandler(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO2,
                                (ControlHandler)controlHandlerForBinaryOutput, IEDMODEL_GenericIO_GGIO1_SPCSO2);

    IedServer_setControlHandler(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO3,
                                (ControlHandler)controlHandlerForBinaryOutput, IEDMODEL_GenericIO_GGIO1_SPCSO3);

    IedServer_setControlHandler(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO4,
                                (ControlHandler)controlHandlerForBinaryOutput, IEDMODEL_GenericIO_GGIO1_SPCSO4);

    if (!IedServer_isRunning(iedServer))
    {
        printf("Starting server failed! Exit.\n");
        IedServer_destroy(iedServer);
        exit(-1);
    }

    /* Start GOOSE publishing */
    IedServer_enableGoosePublishing(iedServer);

    running = 1;

    signal(SIGINT, sigint_handler);

    float anIn1 = 0.f;

    int eventCount = 10;

    while (running)
    {
        IedServer_lockDataModel(iedServer);

        IedServer_updateUTCTimeAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_AnIn1_t, Hal_getTimeInMs());
        IedServer_updateFloatAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_AnIn1_mag_f, anIn1);

        if (eventCount)
        {
            IedServer_updateUTCTimeAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO4_t, Hal_getTimeInMs());

            if (eventCount % 2)
            {
                IedServer_updateQuality(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO4_q, QUALITY_VALIDITY_GOOD);
                IedServer_updateBooleanAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO4_stVal, true);
            }
            else
            {
                IedServer_updateQuality(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO4_q, QUALITY_VALIDITY_INVALID);
                IedServer_updateBooleanAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO4_stVal, false);
            }

            eventCount--;
        }

        IedServer_unlockDataModel(iedServer);

        anIn1 += 0.1;

        Thread_sleep(1000);
    }

    /* stop MMS server - close TCP server socket and all client sockets */
    if (!disableMms)
        IedServer_stop(iedServer);

    /* stop GOOSE-only publishing */
    IedServer_stopGoosePublishing(iedServer);

    /* Cleanup - free all resources */
    IedServer_destroy(iedServer);

    return 0;
} /* main() */
