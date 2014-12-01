/*
 *  server_example2.c
 *
 *  Copyright 2013 Michael Zillgith
 *
 *  This file is part of libIEC61850.
 *
 *  libIEC61850 is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  libIEC61850 is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with libIEC61850.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  See COPYING file for the complete license text.
 */

#include "iec61850_server.h"
#include "hal_thread.h"
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>


/* Include the generated header with the model access handles */
#include "static_model.h"

/* import IEC 61850 device model created from SCL-File */
extern IedModel iedModel;

static int running = 0;

void sigint_handler(int signalId)
{
    running = 0;
}

int 
main(int argc, char** argv)
{
    IedServer iedServer = IedServer_create(&iedModel);

    // TODO get stored values from persistent storage

    // TODO set initial measurement and status values from process

    /* MMS server will be instructed to start listening to client connections. */
    IedServer_start(iedServer, 102);

    if (!IedServer_isRunning(iedServer)) {
        printf("Starting server failed! Exit.\n");
        IedServer_destroy(iedServer);
        exit(-1);
    }

    running = 1;

    signal(SIGINT, sigint_handler);

    float power = 500.f;

    while (running) {

        uint64_t timeval = Hal_getTimeInMs();

        IedServer_lockDataModel(iedServer);

        IedServer_updateFloatAttributeValue(iedServer, IEDMODEL_Inverter_MMXU1_TotW_mag_f, power);
        IedServer_updateUTCTimeAttributeValue(iedServer, IEDMODEL_Inverter_MMXU1_TotW_t, timeval);

        IedServer_unlockDataModel(iedServer);

        power += 0.1f;

        Thread_sleep(500);
    }

    /* stop MMS server - close TCP server socket and all client sockets */
    IedServer_stop(iedServer);

    /* Cleanup - free all resources */
    IedServer_destroy(iedServer);

    return 0;
} /* main() */
