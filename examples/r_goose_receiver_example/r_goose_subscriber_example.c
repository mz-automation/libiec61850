/*
 * goose_subscriber_example.c
 *
 * This is an example for a standalone GOOSE subscriber
 *
 * Has to be started as root in Linux.
 */

#include "goose_receiver.h"
#include "goose_subscriber.h"
#include "hal_thread.h"
#include "linked_list.h"

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

static bool running = true;

static void
sigint_handler(int signalId)
{
    running = false;
}

static void
gooseListener(GooseSubscriber subscriber, void* parameter)
{
    printf("GOOSE event:\n");
    printf("  stNum: %u sqNum: %u\n", GooseSubscriber_getStNum(subscriber),
            GooseSubscriber_getSqNum(subscriber));
    printf("  timeToLive: %u\n", GooseSubscriber_getTimeAllowedToLive(subscriber));

    uint64_t timestamp = GooseSubscriber_getTimestamp(subscriber);

    printf("  timestamp: %u.%u\n", (uint32_t) (timestamp / 1000), (uint32_t) (timestamp % 1000));
    printf("  message is %s\n", GooseSubscriber_isValid(subscriber) ? "valid" : "INVALID");

    MmsValue* values = GooseSubscriber_getDataSetValues(subscriber);

    char buffer[1024];

    MmsValue_printToBuffer(values, buffer, 1024);

    printf("  allData: %s\n", buffer);
}

int
main(int argc, char** argv)
{
    RSession sessionP = RSession_create();

    if (sessionP) {

        RSession_setRemoteAddress(sessionP, "127.0.0.1", 102);
        RSession_setLocalAddress(sessionP, "0.0.0.0", 102);
        char* key = "0123456789ABCDEF";

        RSession_addKey(sessionP, 1, (uint8_t*)key, 16, R_SESSION_SEC_ALGO_AES_128_GCM, R_SESSION_SIG_ALGO_NONE);

        RSession_addMulticastGroup(sessionP, "230.0.10.11");

        GooseReceiver receiver = GooseReceiver_createRemote(sessionP);

        GooseSubscriber subscriber = GooseSubscriber_create("simpleIOGenericIO/LLN0$GO$gcbAnalogValues", NULL);

        uint8_t dstMac[6] = {0x01,0x0c,0xcd,0x01,0x00,0x01};
        GooseSubscriber_setDstMac(subscriber, dstMac);
        GooseSubscriber_setAppId(subscriber, 1000);

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
    }

    return 0;
}
