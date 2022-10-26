/*
 *  iec61850_9_2_LE_example.c
 *
 *  Copyright 2016 Michael Zillgith
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
#include "sv_publisher.h"
#include "hal_thread.h"
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#define _USE_MATH_DEFINES
#include <math.h>

/* Include the generated header with the model access handles */
#include "static_model.h"

static int running = 0;
static int svcbEnabled = 1;

void sigint_handler(int signalId)
{
    running = 0;
}

static int amp1;
static int amp2;
static int amp3;
static int amp4;

static int amp1q;
static int amp2q;
static int amp3q;
static int amp4q;

static int vol1;
static int vol2;
static int vol3;
static int vol4;

static int vol1q;
static int vol2q;
static int vol3q;
static int vol4q;

static SVPublisher svPublisher;
static SVPublisher_ASDU asdu;

static void
setupSVPublisher(const char* svInterface)
{
    svPublisher = SVPublisher_create(NULL, svInterface);

    if (svPublisher) {

        asdu = SVPublisher_addASDU(svPublisher, "xxxxMUnn01", NULL, 1);

        amp1 = SVPublisher_ASDU_addINT32(asdu);
        amp1q = SVPublisher_ASDU_addQuality(asdu);
        amp2 = SVPublisher_ASDU_addINT32(asdu);
        amp2q = SVPublisher_ASDU_addQuality(asdu);
        amp3 = SVPublisher_ASDU_addINT32(asdu);
        amp3q = SVPublisher_ASDU_addQuality(asdu);
        amp4 = SVPublisher_ASDU_addINT32(asdu);
        amp4q = SVPublisher_ASDU_addQuality(asdu);

        vol1 = SVPublisher_ASDU_addINT32(asdu);
        vol1q = SVPublisher_ASDU_addQuality(asdu);
        vol2 = SVPublisher_ASDU_addINT32(asdu);
        vol2q = SVPublisher_ASDU_addQuality(asdu);
        vol3 = SVPublisher_ASDU_addINT32(asdu);
        vol3q = SVPublisher_ASDU_addQuality(asdu);
        vol4 = SVPublisher_ASDU_addINT32(asdu);
        vol4q = SVPublisher_ASDU_addQuality(asdu);

        SVPublisher_ASDU_setSmpCntWrap(asdu, 4000);
        SVPublisher_ASDU_setRefrTm(asdu, 0);

        SVPublisher_setupComplete(svPublisher);
    }
}

static void sVCBEventHandler (SVControlBlock* svcb, int event, void* parameter)
{
    if (event == IEC61850_SVCB_EVENT_ENABLE)
        svcbEnabled = 1;
    else if (event == IEC61850_SVCB_EVENT_DISABLE)
        svcbEnabled = 0;
}

int 
main(int argc, char** argv)
{
    char* svInterface;

    if (argc > 1)
        svInterface = argv[1];
    else
        svInterface = "eth0";

    IedServer iedServer = IedServer_create(&iedModel);

    /* MMS server will be instructed to start listening to client connections. */
    IedServer_start(iedServer, 102);

    if (!IedServer_isRunning(iedServer)) {
        printf("Starting server failed! Exit.\n");
        IedServer_destroy(iedServer);
        exit(-1);
    }

    running = 1;

    signal(SIGINT, sigint_handler);

    setupSVPublisher(svInterface);

    if (svPublisher) {

        SVControlBlock* svcb = IedModel_getSVControlBlock(&iedModel, IEDMODEL_MUnn_LLN0, "MSVCB01");

        if (svcb == NULL) {
            printf("Lookup svcb failed!\n");
            exit(1);
        }

        IedServer_setSVCBHandler(iedServer, svcb, sVCBEventHandler, NULL);

        Quality q = QUALITY_VALIDITY_GOOD;

        int vol = (int) (6350.f * sqrt(2));
        int amp = 0;
        float phaseAngle = 0.f;

        int voltageA;
        int voltageB;
        int voltageC;
        int voltageN;
        int currentA;
        int currentB;
        int currentC;
        int currentN;

        int sampleCount = 0;

        uint64_t nextCycleStart = Hal_getTimeInMs() + 1000;

        while (running) {

            /* update measurement values */
            int samplePoint = sampleCount % 80;

            double angleA = (2 * M_PI / 80) * samplePoint;
            double angleB = (2 * M_PI / 80) * samplePoint - ( 2 * M_PI / 3);
            double angleC = (2 * M_PI / 80) * samplePoint - ( 4 * M_PI / 3);

            voltageA = (vol * sin(angleA)) * 100;
            voltageB = (vol * sin(angleB)) * 100;
            voltageC = (vol * sin(angleC)) * 100;
            voltageN = voltageA + voltageB + voltageC;

            currentA = (amp * sin(angleA - phaseAngle)) * 1000;
            currentB = (amp * sin(angleB - phaseAngle)) * 1000;
            currentC = (amp * sin(angleC - phaseAngle)) * 1000;
            currentN = currentA + currentB + currentC;

            IedServer_lockDataModel(iedServer);

            IedServer_updateInt32AttributeValue(iedServer, IEDMODEL_MUnn_TCTR1_Amp_instMag_i, currentA);
            IedServer_updateQuality(iedServer, IEDMODEL_MUnn_TCTR1_Amp_q, q);
            IedServer_updateInt32AttributeValue(iedServer, IEDMODEL_MUnn_TCTR2_Amp_instMag_i, currentB);
            IedServer_updateQuality(iedServer, IEDMODEL_MUnn_TCTR2_Amp_q, q);
            IedServer_updateInt32AttributeValue(iedServer, IEDMODEL_MUnn_TCTR3_Amp_instMag_i, currentC);
            IedServer_updateQuality(iedServer, IEDMODEL_MUnn_TCTR3_Amp_q, q);
            IedServer_updateInt32AttributeValue(iedServer, IEDMODEL_MUnn_TCTR4_Amp_instMag_i, currentN);
            IedServer_updateQuality(iedServer, IEDMODEL_MUnn_TCTR4_Amp_q, q);

            IedServer_updateInt32AttributeValue(iedServer, IEDMODEL_MUnn_TVTR1_Vol_instMag_i, voltageA);
            IedServer_updateQuality(iedServer, IEDMODEL_MUnn_TVTR1_Vol_q, q);
            IedServer_updateInt32AttributeValue(iedServer, IEDMODEL_MUnn_TVTR2_Vol_instMag_i, voltageB);
            IedServer_updateQuality(iedServer, IEDMODEL_MUnn_TVTR2_Vol_q, q);
            IedServer_updateInt32AttributeValue(iedServer, IEDMODEL_MUnn_TVTR3_Vol_instMag_i, voltageC);
            IedServer_updateQuality(iedServer, IEDMODEL_MUnn_TVTR3_Vol_q, q);
            IedServer_updateInt32AttributeValue(iedServer, IEDMODEL_MUnn_TVTR4_Vol_instMag_i, voltageN);
            IedServer_updateQuality(iedServer, IEDMODEL_MUnn_TVTR4_Vol_q, q);

            IedServer_unlockDataModel(iedServer);

            if (svcbEnabled) {

                SVPublisher_ASDU_setINT32(asdu, amp1, currentA);
                SVPublisher_ASDU_setQuality(asdu, amp1q, q);
                SVPublisher_ASDU_setINT32(asdu, amp2, currentB);
                SVPublisher_ASDU_setQuality(asdu, amp2q, q);
                SVPublisher_ASDU_setINT32(asdu, amp3, currentC);
                SVPublisher_ASDU_setQuality(asdu, amp3q, q);
                SVPublisher_ASDU_setINT32(asdu, amp4, currentN);
                SVPublisher_ASDU_setQuality(asdu, amp4q, q);

                SVPublisher_ASDU_setINT32(asdu, vol1, voltageA);
                SVPublisher_ASDU_setQuality(asdu, vol1q, q);
                SVPublisher_ASDU_setINT32(asdu, vol2, voltageB);
                SVPublisher_ASDU_setQuality(asdu, vol2q, q);
                SVPublisher_ASDU_setINT32(asdu, vol3, voltageC);
                SVPublisher_ASDU_setQuality(asdu, vol3q, q);
                SVPublisher_ASDU_setINT32(asdu, vol4, voltageN);
                SVPublisher_ASDU_setQuality(asdu, vol4q, q);

                SVPublisher_ASDU_setRefrTmNs(asdu, Hal_getTimeInNs());

                SVPublisher_ASDU_setSmpCnt(asdu, (uint16_t) sampleCount);

                SVPublisher_publish(svPublisher);
            }

            sampleCount = ((sampleCount + 1) % 4000);

            if ((sampleCount % 400) == 0) {
                uint64_t timeval = Hal_getTimeInMs();

                while (timeval < nextCycleStart + 100) {
                    Thread_sleep(1);

                    timeval = Hal_getTimeInMs();
                }

                nextCycleStart = nextCycleStart + 100;
            }
        }
    }
    else {
        printf("Cannot start SV publisher!\n");
    }

    /* stop MMS server - close TCP server socket and all client sockets */
    IedServer_stop(iedServer);

    /* Cleanup - free all resources */
    SVPublisher_destroy(svPublisher);
    IedServer_destroy(iedServer);

    return 0;
} /* main() */
