/*
 * sv_subscriber_example.c
 *
 * Example program for Sampled Values (SV) subscriber
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
    SampledValuesPublisher svPublisher = SampledValuesPublisher_create("eth0");

    SV_ASDU asdu1 = SampledValuesPublisher_addASDU(svPublisher, "svpub1", NULL, 1);

    int float1 = SV_ASDU_addFLOAT(asdu1);
    int float2 = SV_ASDU_addFLOAT(asdu1);

    SV_ASDU asdu2 = SampledValuesPublisher_addASDU(svPublisher, "svpub2", NULL, 1);

    int float3 = SV_ASDU_addFLOAT(asdu2);
    int float4 = SV_ASDU_addFLOAT(asdu2);

    SampledValuesPublisher_setupComplete(svPublisher);

    float fVal1 = 1234.5678f;
    float fVal2 = 0.12345f;

    int i;

    while (running) {
        SV_ASDU_setFLOAT(asdu1, float1, fVal1);
        SV_ASDU_setFLOAT(asdu1, float2, fVal2);

        SV_ASDU_increaseSmpCnt(asdu1);
        SV_ASDU_increaseSmpCnt(asdu2);

        fVal1 += 1.1f;
        fVal2 += 0.1f;

        SampledValuesPublisher_publish(svPublisher);

        //Thread_sleep(50);
    }

    SampledValuesPublisher_destroy(svPublisher);
}
