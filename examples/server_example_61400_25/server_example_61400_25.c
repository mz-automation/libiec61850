/*
 *  server_example1.c
 *
 *  Copyright 2013 Michael Zillgith
 *
 *	This file is part of libIEC61850.
 *
 *	libIEC61850 is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	libIEC61850 is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with libIEC61850.  If not, see <http://www.gnu.org/licenses/>.
 *
 *	See COPYING file for the complete license text.
 */

#include "iec61850_server.h"
#include "hal_thread.h"
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#include "static_model.h"

static IedServer iedServer;

static int running = 0;

void sigint_handler(int signalId)
{
    running = 0;
}

bool
controlHandlerForIntegerOutput(ControlAction action, void* parameter, MmsValue* value, bool test)
{
    if (MmsValue_getType(value) != MMS_INTEGER)
        return false;

    printf("Received integer control value: %i\n", MmsValue_toInt32(value));

    uint64_t timeStamp = Hal_getTimeInMs();

    if (parameter == IEDMODEL_WTG_WTUR1_SetTurOp_actSt) {
        IedServer_updateUTCTimeAttributeValue(iedServer, IEDMODEL_WTG_WTUR1_SetTurOp_actSt_t, timeStamp);
        IedServer_updateAttributeValue(iedServer, IEDMODEL_WTG_WTUR1_SetTurOp_actSt_stVal, value);
    }

    return true;
}

int
main(int argc, char** argv)
{
    int tcpPort = 102;

    if (argc > 1) {
        tcpPort = atoi(argv[1]);
    }

    iedServer = IedServer_create(&iedModel);

    /* set control model for WTUR1.SetTurOp.actSt to direct-control-with-normal-security */
    IedServer_updateInt32AttributeValue(iedServer, IEDMODEL_WTG_WTUR1_SetTurOp_actSt_ctlModel, 1);

    IedServer_setControlHandler(iedServer, IEDMODEL_WTG_WTUR1_SetTurOp_actSt,
            (ControlHandler) controlHandlerForIntegerOutput,
            IEDMODEL_WTG_WTUR1_SetTurOp_actSt);

    /* MMS server will be instructed to start listening to client connections. */
    IedServer_start(iedServer, tcpPort);

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
