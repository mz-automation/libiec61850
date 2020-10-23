/*
 * client_example4.c
 *
 * How to create and delete data sets at a remote server.
 *
 * This example is intended to be used with server_example3 or server_example_goose.
 */

#include "iec61850_client.h"

#include <stdlib.h>
#include <stdio.h>

#include "hal_thread.h"

static void
printDataSetValues(MmsValue* dataSet)
{
    int i;
    for (i = 0; i < 4; i++) {
        printf("  GGIO1.AnIn%i.mag.f: %f\n", i,
                MmsValue_toFloat(MmsValue_getElement(MmsValue_getElement(
                                            MmsValue_getElement(dataSet, i), 0), 0)));
    }
}

void
reportCallbackFunction(void* parameter, ClientReport report)
{
    MmsValue* dataSetValues = (MmsValue*) parameter;

    printf("received report\n");

    printDataSetValues(dataSetValues);
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

    IedConnection_connect(con, &error, hostname, tcpPort);

    if (error == IED_ERROR_OK) {

        /* Create a new data set at the remote server */
        LinkedList newDataSetEntries = LinkedList_create();

        LinkedList_add(newDataSetEntries, "simpleIOGenericIO/GGIO1.AnIn1[MX]");
        LinkedList_add(newDataSetEntries, "simpleIOGenericIO/GGIO1.AnIn2[MX]");
        LinkedList_add(newDataSetEntries, "simpleIOGenericIO/GGIO1.AnIn3[MX]");
        LinkedList_add(newDataSetEntries, "simpleIOGenericIO/GGIO1.AnIn4[MX]");

        IedConnection_createDataSet(con, &error, "simpleIOGenericIO/LLN0.AnalogueValues", newDataSetEntries);

        LinkedList_destroyStatic(newDataSetEntries);


        if (error == IED_ERROR_OK ) {

            /* read new data set */
            ClientDataSet clientDataSet;

            clientDataSet = IedConnection_readDataSetValues(con, &error, "simpleIOGenericIO/LLN0.AnalogueValues", NULL);

            if (error == IED_ERROR_OK) {
                printDataSetValues(ClientDataSet_getValues(clientDataSet));

                ClientDataSet_destroy(clientDataSet);
            }
            else {
                printf("Failed to read data set (error code: %d)\n", error);
            }

            IedConnection_deleteDataSet(con, &error, "simpleIOGenericIO/LLN0.AnalogueValues");
        }
        else {
            printf("Failed to create data set (error code: %d)\n", error);
        }

        IedConnection_close(con);
    }
    else {
        printf("Failed to connect to %s:%i\n", hostname, tcpPort);
    }

    IedConnection_destroy(con);
    return 0;
}


