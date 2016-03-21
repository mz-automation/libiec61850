/*
 * client_example_files.c
 *
 * This example demonstrates the usage of the file services
 *
 * - How to browse the file system of the server
 * - How to download a file from the server
 *
 * Note: intended to be used with server_example3
 *
 */

#include "iec61850_client.h"

#include <stdlib.h>
#include <stdio.h>

#include "hal_thread.h"

#define MAX_BUFFER_SIZE 2000000

static uint8_t downloadBuffer[MAX_BUFFER_SIZE];
static int bufferPosition = 0;

static bool
downloadHandler(void* parameter, uint8_t* buffer, uint32_t bytesRead)
{
    printf("received %i bytes\n", bytesRead);

    if (bufferPosition + bytesRead < MAX_BUFFER_SIZE) {
        memcpy(downloadBuffer + bufferPosition, buffer, bytesRead);
        return true;
    }
    else
        return false;
}

int main(int argc, char** argv) {

    char* hostname;
    int tcpPort = 102;

    if (argc > 1)
        hostname = argv[1];
    else
        hostname = "localhost";

    if (argc > 2)
        tcpPort = atoi(argv[2]);

    IedClientError error;

    IedConnection con = IedConnection_create();

    MmsConnection mmsCon = IedConnection_getMmsConnection(con);

    MmsConnection_setLocalDetail(mmsCon, 800);

    IedConnection_connect(con, &error, hostname, tcpPort);

    if (error == IED_ERROR_OK) {

        /* Get the root directory */
        LinkedList rootDirectory =
                IedConnection_getFileDirectory(con, &error, NULL);

        if (error != IED_ERROR_OK) {
            printf("Error retrieving file directory\n");
            goto abort_connection;
        }


        LinkedList directoryEntry = LinkedList_getNext(rootDirectory);

        while (directoryEntry != NULL) {

            FileDirectoryEntry entry = (FileDirectoryEntry) directoryEntry->data;

            printf("%s %i\n", FileDirectoryEntry_getFileName(entry), FileDirectoryEntry_getFileSize(entry));

            directoryEntry = LinkedList_getNext(directoryEntry);
        }


        /* Download a file from the server */
        IedConnection_getFile(con, &error, "IEDSERVER.BIN", downloadHandler, NULL);

        if (error != IED_ERROR_OK)
            printf("Failed to get file!\n");

        /* Delete file at server */
        IedConnection_deleteFile(con, &error, "IEDSERVER.BIN");

        if (error != IED_ERROR_OK)
            printf("Failed to delete file! (code=%i)\n", error);

        abort_connection:

        IedConnection_abort(con, &error);
    }
    else {
        printf("Failed to connect to %s:%i\n", hostname, tcpPort);
    }

    IedConnection_destroy(con);
}


