/*
 * goose_subscriber_example.c
 *
 * This is an example for a standalone GOOSE subscriber
 *
 * Has to be started as root in Linux.
 */

#include "goose_subscriber.h"
#include "thread.h"

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>


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
    MmsValue* dataSetValues = MmsValue_createEmtpyArray(4);

    int i;
    for (i = 0; i < 4; i++) {
        MmsValue* dataSetEntry = MmsValue_newBoolean(false);
        MmsValue_setElement(dataSetValues, i, dataSetEntry);
    }

 //   GooseSubscriber subscriber = GooseSubscriber_create("simpleIOGenericIO/LLN0$GO$gcbEvents", dataSetValues);

    GooseSubscriber subscriber = GooseSubscriber_create("simpleIOGenericIO/LLN0$GO$gcbAnalogValues", NULL);

    if (argc > 1) {
    	printf("Set interface id: %s\n", argv[1]);
    	GooseSubscriber_setInterfaceId(subscriber, argv[1]);
    }

    GooseSubscriber_setAppId(subscriber, 1000);

    GooseSubscriber_setListener(subscriber, gooseListener, NULL);

    GooseSubscriber_subscribe(subscriber);

    signal(SIGINT, sigint_handler);

    while (running) {
        Thread_sleep(100);
    }

    GooseSubscriber_destroy(subscriber);
}
