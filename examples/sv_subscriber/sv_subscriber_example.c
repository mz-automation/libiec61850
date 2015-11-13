/*
 * subscriber_test.c
 *
 * Example program for Sampled Values (SV) subscriber
 *
 */

#include "hal_thread.h"
#include <signal.h>
#include <stdio.h>
#include "sv_subscriber.h"


static bool running = true;

void sigint_handler(int signalId)
{
    running = 0;
}


/* Callback handler for received SV messages */
static void
svUpdateListener (SVSubscriber subscriber, void* parameter, SVClientASDU asdu)
{
    printf("svUpdateListener called\n");

    const char* svID = SVClientASDU_getSvId(asdu);

    if (svID != NULL)
        printf("  svID=(%s)\n", svID);

    printf("  smpCnt: %i\n", SVClientASDU_getSmpCnt(asdu));
    printf("  confRev: %u\n", SVClientASDU_getConfRev(asdu));

    if (SVClientASDU_getDataSize(asdu) >= 8) {
        printf("   DATA[0]: %f\n", SVClientASDU_getFLOAT32(asdu, 0));
        printf("   DATA[1]: %f\n", SVClientASDU_getFLOAT32(asdu, 4));
    }
}

int
main(int argc, char** argv)
{
    SVReceiver receiver = SVReceiver_create();

    if (argc > 1) {
        printf("Set interface id: %s\n", argv[1]);
        SVReceiver_setInterfaceId(receiver, argv[1]);
    }
    else {
        printf("Using interface eth0\n");
        SVReceiver_setInterfaceId(receiver, "eth0");
    }

    SVSubscriber subscriber = SVSubscriber_create(NULL, 0x4000);

    SVSubscriber_setListener(subscriber, svUpdateListener, NULL);

    SVReceiver_addSubscriber(receiver, subscriber);

    SVReceiver_start(receiver);

    signal(SIGINT, sigint_handler);

    while (running) {
        Thread_sleep(1);
    }

    SVReceiver_stop(receiver);

    SVReceiver_destroy(receiver);
}
