/*
 * sv_publisher_example.c
 *
 * Example program for Sampled Values (SV) publisher
 *
 */

#include <signal.h>
#include <stdio.h>
#include "hal_thread.h"
#include "sv_publisher.h"

static bool running = true;

void sigint_handler(int signalId)
{
    running = 0;
}

int
main(int argc, char** argv)
{
    char* interface;
  
    if (argc > 1)
        interface = argv[1];
    else
        interface = "eth0";
  
    printf("Using interface %s\n", interface);

    signal(SIGINT, sigint_handler);

    SVPublisher svPublisher = SVPublisher_create(NULL, interface);

    if (svPublisher) {

        SVPublisher_ASDU asdu1 = SVPublisher_addASDU(svPublisher, "svpub1", NULL, 1);

        int float1 = SVPublisher_ASDU_addFLOAT(asdu1);
        int float2 = SVPublisher_ASDU_addFLOAT(asdu1);
        int ts1 = SVPublisher_ASDU_addTimestamp(asdu1);

        SVPublisher_ASDU asdu2 = SVPublisher_addASDU(svPublisher, "svpub2", NULL, 1);

        int float3 = SVPublisher_ASDU_addFLOAT(asdu2);
        int float4 = SVPublisher_ASDU_addFLOAT(asdu2);
        int ts2 = SVPublisher_ASDU_addTimestamp(asdu2);

        SVPublisher_setupComplete(svPublisher);
        SVPublisher_ASDU_setSmpSynch(asdu1, 2);
        SVPublisher_ASDU_setSmpSynch(asdu2, 1);

        float fVal1 = 1234.5678f;
        float fVal2 = 0.12345f;

        while (running) {
            Timestamp ts;
            Timestamp_clearFlags(&ts);
            Timestamp_setTimeInMilliseconds(&ts, Hal_getTimeInMs());

            SVPublisher_ASDU_setFLOAT(asdu1, float1, fVal1);
            SVPublisher_ASDU_setFLOAT(asdu1, float2, fVal2);
            SVPublisher_ASDU_setTimestamp(asdu1, ts1, ts);

            SVPublisher_ASDU_setFLOAT(asdu2, float3, fVal1 * 2);
            SVPublisher_ASDU_setFLOAT(asdu2, float4, fVal2 * 2);
            SVPublisher_ASDU_setTimestamp(asdu2, ts2, ts);

            SVPublisher_ASDU_increaseSmpCnt(asdu1);
            SVPublisher_ASDU_increaseSmpCnt(asdu2);

            fVal1 += 1.1f;
            fVal2 += 0.1f;

            SVPublisher_publish(svPublisher);

            Thread_sleep(50);
        }

        SVPublisher_destroy(svPublisher);
    }
    else {
        printf("Failed to create SV publisher\n");
    }
}
