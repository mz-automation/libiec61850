/*
 * goose_subscriber_example.c
 *
 * This is an example for a standalone GOOSE subscriber
 *
 * Has to be started as root in Linux.
 */

#include "goose_receiver.h"
#include "hal_thread.h"

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#define __STDC_FORMAT_MACROS         /* otherwise PRIu64 is not defined for MinGW */
#include <inttypes.h>

static int running = 1;

void sigint_handler(int signalId)
{
    running = 0;
}

void
gooseListener(GooseSubscriber subscriber, void* parameter)
{
    printf("GOOSE event:\n");
    printf("  stNum: %u sqNum: %u\n", GooseSubscriber_getStNum(subscriber),
            GooseSubscriber_getSqNum(subscriber));
    printf("  timeToLive: %u\n", GooseSubscriber_getTimeAllowedToLive(subscriber));
    printf("  timestamp: %"PRIu64"\n", GooseSubscriber_getTimestamp(subscriber));

    MmsValue* values = GooseSubscriber_getDataSetValues(subscriber);

    char buffer[1024];

    MmsValue_printToBuffer(values, buffer, 1024);

    printf("%s\n", buffer);
}

int
main(int argc, char** argv)
{
    MmsValue* dataSetValues = MmsValue_createEmptyArray(4);

    int i;
    for (i = 0; i < 4; i++) {
        MmsValue* dataSetEntry = MmsValue_newBoolean(false);
        MmsValue_setElement(dataSetValues, i, dataSetEntry);
    }

    GooseReceiver receiver = GooseReceiver_create();

     if (argc > 1) {
         printf("Set interface id: %s\n", argv[1]);
         GooseReceiver_setInterfaceId(receiver, argv[1]);
     }
     else {
         printf("Using interface eth0\n");
         GooseReceiver_setInterfaceId(receiver, "eth0");
     }

    GooseSubscriber subscriber = GooseSubscriber_create("simpleIOGenericIO/LLN0$GO$gcbAnalogValues", NULL);

    GooseSubscriber_setAppId(subscriber, 1000);

    GooseSubscriber_setListener(subscriber, gooseListener, NULL);

    GooseReceiver_addSubscriber(receiver, subscriber);

    GooseReceiver_start(receiver);

    signal(SIGINT, sigint_handler);

    while (running) {
        Thread_sleep(100);
    }

    GooseSubscriber_destroy(subscriber);
}
