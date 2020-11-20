/*
 * goose_observer.c
 *
 * This is an example for generic GOOSE observer
 *
 * Has to be started as root in Linux.
 */

#include "goose_receiver.h"
#include "goose_subscriber.h"
#include "hal_thread.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

static int running = 1;

void sigint_handler(int signalId)
{
    running = 0;
}

void
gooseListener(GooseSubscriber subscriber, void* parameter)
{
    printf("GOOSE event:\n");
    printf("  vlanTag: %s\n", GooseSubscriber_isVlanSet(subscriber) ? "found" : "NOT found");
    if (GooseSubscriber_isVlanSet(subscriber))
    {
        printf("    vlanId: %u\n", GooseSubscriber_getVlanId(subscriber));
        printf("    vlanPrio: %u\n", GooseSubscriber_getVlanPrio(subscriber));
    }
    printf("  appId: %d\n", GooseSubscriber_getAppId(subscriber));
    uint8_t macBuf[6];
    GooseSubscriber_getSrcMac(subscriber,macBuf);
    printf("  srcMac: %02X:%02X:%02X:%02X:%02X:%02X\n", macBuf[0],macBuf[1],macBuf[2],macBuf[3],macBuf[4],macBuf[5]);
    GooseSubscriber_getDstMac(subscriber,macBuf);
    printf("  dstMac: %02X:%02X:%02X:%02X:%02X:%02X\n", macBuf[0],macBuf[1],macBuf[2],macBuf[3],macBuf[4],macBuf[5]);
    printf("  goId: %s\n", GooseSubscriber_getGoId(subscriber));
    printf("  goCbRef: %s\n", GooseSubscriber_getGoCbRef(subscriber));
    printf("  dataSet: %s\n", GooseSubscriber_getDataSet(subscriber));
    printf("  confRev: %u\n", GooseSubscriber_getConfRev(subscriber));
    printf("  ndsCom: %s\n", GooseSubscriber_needsCommission(subscriber) ? "true" : "false");
    printf("  simul: %s\n", GooseSubscriber_isTest(subscriber) ? "true" : "false");
    printf("  stNum: %u sqNum: %u\n", GooseSubscriber_getStNum(subscriber),
            GooseSubscriber_getSqNum(subscriber));
    printf("  timeToLive: %u\n", GooseSubscriber_getTimeAllowedToLive(subscriber));

    uint64_t timestamp = GooseSubscriber_getTimestamp(subscriber);

    printf("  timestamp: %u.%u\n", (uint32_t) (timestamp / 1000), (uint32_t) (timestamp % 1000));
    printf("  message is %s\n", GooseSubscriber_isValid(subscriber) ? "valid" : "INVALID");

    MmsValue* values = GooseSubscriber_getDataSetValues(subscriber);

    char buffer[1024];

    MmsValue_printToBuffer(values, buffer, 1024);

    printf("  AllData: %s\n", buffer);
}

int
main(int argc, char** argv)
{
    GooseReceiver receiver = GooseReceiver_create();

     if (argc > 1) {
         printf("Set interface id: %s\n", argv[1]);
         GooseReceiver_setInterfaceId(receiver, argv[1]);
     }
     else {
         printf("Using interface eth0\n");
         GooseReceiver_setInterfaceId(receiver, "eth0");
     }

    GooseSubscriber subscriber = GooseSubscriber_create("", NULL);
    GooseSubscriber_setObserver(subscriber);
    GooseSubscriber_setListener(subscriber, gooseListener, NULL);

    GooseReceiver_addSubscriber(receiver, subscriber);

    GooseReceiver_start(receiver);

    if (GooseReceiver_isRunning(receiver)) {
        signal(SIGINT, sigint_handler);

        while (running) {
            Thread_sleep(100);
        }
    }
    else {
        printf("Failed to start GOOSE subscriber. Reason can be that the Ethernet interface doesn't exist or root permission are required.\n");
    }

    GooseReceiver_stop(receiver);

    GooseReceiver_destroy(receiver);
    return 0;
}
