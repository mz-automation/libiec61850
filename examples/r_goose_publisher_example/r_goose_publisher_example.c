/*
 * r_goose_publisher_example.c
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <signal.h>

#include "mms_value.h"
#include "goose_publisher.h"
#include "hal_thread.h"

static bool running = true;

static void
sigint_handler(int signalId)
{
    running = false;
}

/* has to be executed as root! */
int
main(int argc, char **argv)
{
    char* dstAddress;

    if (argc > 1)
        dstAddress = argv[1];
    else
        dstAddress = "230.0.10.11";

    printf("Using destination address %s\n", dstAddress);

    RSession rSession = RSession_create();

    if (rSession) {

        signal(SIGINT, sigint_handler);

        /* Call RSession_setLocalAddress to use a particular interface to send the R-GOOSE messages */
        //RSession_setLocalAddress(rSession, "169.254.110.126", -1);
        RSession_setRemoteAddress(rSession, dstAddress, 102);

        LinkedList dataSetValues = LinkedList_create();

        int intVal = 1234;

        MmsValue* mmsIntVal = MmsValue_newIntegerFromInt32(intVal);
        MmsValue* mmsTimeValue = MmsValue_newBinaryTime(false);
        MmsValue_setBinaryTime(mmsTimeValue, Hal_getTimeInMs());

        LinkedList_add(dataSetValues, mmsIntVal);
        LinkedList_add(dataSetValues, mmsTimeValue);
        LinkedList_add(dataSetValues, MmsValue_newIntegerFromInt32(5678));

        GoosePublisher publisher = GoosePublisher_createRemote(rSession, 0x4001);

        char* key = "0123456789ABCDEF";

        RSession_addKey(rSession, 1, (uint8_t*)key, 16, R_SESSION_SEC_ALGO_AES_128_GCM, R_SESSION_SIG_ALGO_NONE);
        RSession_setActiveKey(rSession, 1);

        if (publisher) {
            GoosePublisher_setGoCbRef(publisher, "simpleIOGenericIO/LLN0$GO$gcbAnalogValues");
            GoosePublisher_setConfRev(publisher, 1);
            GoosePublisher_setDataSetRef(publisher, "simpleIOGenericIO/LLN0$AnalogValues");
            GoosePublisher_setTimeAllowedToLive(publisher, 500);

            RSession_start(rSession);

            while (running) {
                int i = 0;

                for (i = 0; i < 6; i++) {
                    Thread_sleep(200);

                    if (i == 5) {
                        MmsValue_setInt32(mmsIntVal, ++intVal);
                        MmsValue_setBinaryTime(mmsTimeValue, Hal_getTimeInMs());
                        GoosePublisher_increaseStNum(publisher);
                    }

                    if (GoosePublisher_publish(publisher, dataSetValues) == -1) {
                        printf("Error sending message!\n");
                    }

                    if (running == false)
                        break;
                }
            }

            GoosePublisher_destroy(publisher);
        }
        else {
            printf("Failed to create GOOSE publisher. Reason can be that the Ethernet interface doesn't exist or root permission are required.\n");
        }

        LinkedList_destroyDeep(dataSetValues, (LinkedListValueDeleteFunction) MmsValue_delete);
    }
    else {
        printf("Failed to create RSession instance\n");
    }

    return 0;
}
