/*
 * r-sv-example.c
 */


#include "sv_subscriber.h"
#include <signal.h>
#include <stdio.h>
#include "hal_thread.h"

static bool running = true;

void sigint_handler(int signalId)
{
    running = 0;
}

/* Callback handler for received SV messages */
static void
svUpdateListener (SVSubscriber subscriber, void* parameter, SVSubscriber_ASDU asdu)
{
    printf("Received SV message:\n");

    const char* svID = SVSubscriber_ASDU_getSvId(asdu);

    if (svID != NULL)
        printf("  svID=(%s)\n", svID);

    printf("  smpCnt: %i\n", SVSubscriber_ASDU_getSmpCnt(asdu));
    printf("  confRev: %u\n", SVSubscriber_ASDU_getConfRev(asdu));

    /*
     * Access to the data requires a priori knowledge of the data set.
     * For this example we assume a data set consisting of FLOAT32 values.
     * A FLOAT32 value is encoded as 4 bytes. You can find the first FLOAT32
     * value at byte position 0, the second value at byte position 4, the third
     * value at byte position 8, and so on.
     *
     * To prevent damages due configuration, please check the length of the
     * data block of the SV message before accessing the data.
     */
    if (SVSubscriber_ASDU_getDataSize(asdu) >= 8) {
        printf("   DATA[0]: %f\n", SVSubscriber_ASDU_getFLOAT32(asdu, 0));
        printf("   DATA[1]: %f\n", SVSubscriber_ASDU_getFLOAT32(asdu, 4));
    }
}

int
main(int argc, char** argv)
{
    RSession rSession = RSession_create();

    if (rSession)
    {
        RSession_setLocalAddress(rSession, "0.0.0.0", 102);

        RSession_addMulticastGroup(rSession, "230.0.10.10");

        char* key1 = "0123456789ABCDEF";
        char* key2 = "FEDCBA9876543210";

        RSession_addKey(rSession, 1, (uint8_t*)key1, 16, R_SESSION_SEC_ALGO_AES_128_GCM, R_SESSION_SIG_ALGO_NONE);
        RSession_addKey(rSession, 2, (uint8_t*)key2, 16, R_SESSION_SEC_ALGO_AES_128_GCM, R_SESSION_SIG_ALGO_NONE);

        SVReceiver receiver = SVReceiver_createRemote(rSession);

        if (receiver)
        {
            /* Create a subscriber listening to SV messages with APPID 4000h */
            SVSubscriber subscriber = SVSubscriber_create(NULL, 0x4000);

            /* Install a callback handler for the subscriber */
            SVSubscriber_setListener(subscriber, svUpdateListener, NULL);

            /* Connect the subscriber to the receiver */
            SVReceiver_addSubscriber(receiver, subscriber);

            /* Start listening to SV messages - starts a new receiver background thread */
            SVReceiver_start(receiver);

            if (SVReceiver_isRunning(receiver))
            {
                signal(SIGINT, sigint_handler);

                while (running)
                    Thread_sleep(1);

                /* Stop listening to SV messages */
                SVReceiver_stop(receiver);
            }
            else
            {
                printf("Failed to start SV subscriber. Reason can be that the Ethernet interface doesn't exist or root permission are required.\n");
            }

            /* Cleanup and free resources */
            SVReceiver_destroy(receiver);
        }
        else
        {
            printf("Failed to create SV receiver\n");
        }

        RSession_destroy(rSession);
    }
    else
    {
        printf("Failed to create remote session protocol\n");
    }
}
