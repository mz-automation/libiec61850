/*
 * r-sv-example.c
 */

#include <stdio.h>
#include <signal.h>

#include "sv_publisher.h"
#include "hal_thread.h"

static bool running = true;

static void
sigint_handler(int signalId)
{
    running = false;
}

int
main(int argc, char** argv)
{
    RSession rSession = RSession_create();

    if (rSession)
    {
        /* Call RSession_setLocalAddress to use a particular interface to send the R-GOOSE messages */
        //RSession_setLocalAddress(rSession, "169.254.110.126", -1);
        RSession_setRemoteAddress(rSession, "230.0.10.10", 102);

        SVPublisher svPublisher = SVPublisher_createRemote(rSession, 0x4000);

        uint32_t activeKeyId = 1;
        uint64_t nextKeyChangeTime = Hal_getTimeInMs() + 5000;

        if (svPublisher)
        {
            signal(SIGINT, sigint_handler);

            char* key1 = "0123456789ABCDEF";
            char* key2 = "FEDCBA9876543210";

            RSession_addKey(rSession, 1, (uint8_t*)key1, 16, R_SESSION_SEC_ALGO_AES_128_GCM, R_SESSION_SIG_ALGO_NONE);
            RSession_addKey(rSession, 2, (uint8_t*)key2, 16, R_SESSION_SEC_ALGO_AES_128_GCM, R_SESSION_SIG_ALGO_NONE);

            RSession_setActiveKey(rSession, activeKeyId);

            SVPublisher_ASDU asdu1 = SVPublisher_addASDU(svPublisher, "svpub1", NULL, 1);

            int float1 = SVPublisher_ASDU_addFLOAT(asdu1);
            int float2 = SVPublisher_ASDU_addFLOAT(asdu1);
            int ts1 = SVPublisher_ASDU_addTimestamp(asdu1);

            SVPublisher_ASDU asdu2 = SVPublisher_addASDU(svPublisher, "svpub2", NULL, 1);

            int float3 = SVPublisher_ASDU_addFLOAT(asdu2);
            int float4 = SVPublisher_ASDU_addFLOAT(asdu2);
            int ts2 = SVPublisher_ASDU_addTimestamp(asdu2);

            SVPublisher_setupComplete(svPublisher);

            float fVal1 = 1234.5678f;
            float fVal2 = 0.12345f;

            RSession_start(rSession);

            while (running)
            {
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

                if (Hal_getTimeInMs() >= nextKeyChangeTime)
                {
                    /* change key */

                    if (activeKeyId == 1)
                        activeKeyId = 2;
                    //else
                   //     activeKeyId = 1;
                    RSession_removeKey(rSession, 1);

                    nextKeyChangeTime = Hal_getTimeInMs() + 5000;
                }

                Thread_sleep(50);
            }

            SVPublisher_destroy(svPublisher);
        }
        else
        {
            printf("Failed to create SV publisher\n");
        }

        RSession_destroy(rSession);
    }
    else
    {
        printf("Failed to create remote session instance\n");
    }
}
