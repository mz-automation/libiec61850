/*
 * client_example_async.c
 *
 * Shows how to use the asynchronous client API
 *
 * This example is intended to be used with server_example_basic_io.
 */

#include "iec61850_client.h"

#include <stdlib.h>
#include <stdio.h>

#include "hal_thread.h"

static bool fileTransferComplete = false;

static bool
getFileAsyncHandler (uint32_t invokeId, void* parameter, IedClientError err, uint32_t originalInvokeId,
        uint8_t* buffer, uint32_t bytesRead, bool moreFollows)
{
    if ((err != IED_ERROR_OK) || (moreFollows == false)) {

        if (err == IED_ERROR_OK) {
            printf("Received %d bytes\n", bytesRead);
        }

        printf("File transfer complete (err:%d)\n", err);
        fileTransferComplete = true;
    }
    else {
        printf("Received %d bytes\n", bytesRead);
    }

    return true;
}

int main(int argc, char** argv) {

    char* hostname;
    char* filename = NULL;

    if (argc > 1)
        hostname = argv[1];
    else
        hostname = "localhost";

    if (argc > 2)
        filename = argv[2];

    IedClientError error;

    IedConnection con = IedConnection_create();

    IedConnection_connectAsync(con, &error, hostname, 102);

    if (error == IED_ERROR_OK) {

        bool success = true;

        while (IedConnection_getState(con) != IED_STATE_CONNECTED) {

            if (IedConnection_getState(con) == IED_STATE_CLOSED) {
                success = false;
                break;
            }

            Thread_sleep(10);
        }

        if (success) {

            if (filename) {
                IedConnection_getFileAsync(con, &error, filename, getFileAsyncHandler, NULL);

                printf("Started file download\n");

                while (fileTransferComplete == false)
                    Thread_sleep(10);
            }
        }

        Thread_sleep(1000);

        IedConnection_releaseAsync(con, &error);

        if (error != IED_ERROR_OK) {
            printf("Release returned error: %d\n", error);
        }
        else {

            while (IedConnection_getState(con) != IED_STATE_CLOSED) {
                Thread_sleep(10);
            }
        }
    }
    else {
        printf("Failed to connect to %s\n", hostname);
    }


    IedConnection_destroy(con);
    return 0;
}


