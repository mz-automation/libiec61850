/*
* client_example_ClientGooseControl.c
*
* This example is intended to be used with server_example_basic_io or server_example_goose.
*/

#include "iec61850_client.h"

#include <stdlib.h>
#include <stdio.h>

#include "hal_thread.h"

int main(int argc, char** argv)
{
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

    if (error == IED_ERROR_OK)
    {
        /*Read GoCB Values*/
        ClientGooseControlBlock goCB = IedConnection_getGoCBValues(con, &error, "simpleIOGenericIO/LLN0.gcbEvents", NULL);

        bool GoEna = ClientGooseControlBlock_getGoEna(goCB);
        printf("GoEna Value: %d\n", GoEna);

        const char* id = ClientGooseControlBlock_getGoID(goCB);
        printf("GoID Value: %s\n", id);

        const char* datset = ClientGooseControlBlock_getDatSet(goCB);
        printf("GoDatset Value: %s\n", datset);

        /*Update Go CB Values locally*/
        ClientGooseControlBlock_setGoID(goCB, "analog");
        ClientGooseControlBlock_setDatSet(goCB, "simpleIOGenericIO/LLN0$AnalogValues"); 
        ClientGooseControlBlock_setGoEna(goCB, false); 

        /*Update Go CB Values to server (Throws error because only GoEna is writeable)*/
        IedConnection_setGoCBValues(con, &error, goCB, GOCB_ELEMENT_GO_ID | GOCB_ELEMENT_DATSET | GOCB_ELEMENT_GO_ENA, true);

        if (error != IED_ERROR_OK)
            printf("Fail to Set Values to Server (code: %i)\n", error);

        /*Test to see if the values were updated correctly on the server*/
        goCB = IedConnection_getGoCBValues(con, &error, "simpleIOGenericIO/LLN0.gcbEvents", NULL);

        bool GoEnaUpdate = ClientGooseControlBlock_getGoEna(goCB);
        printf("GoEna Value: %d\n", GoEnaUpdate);

        const char* idUpdate = ClientGooseControlBlock_getGoID(goCB);
        printf("GoID Value: %s\n", idUpdate);

        const char* datsetUpdate = ClientGooseControlBlock_getDatSet(goCB);
        printf("GoDatset Value: %s\n", datsetUpdate);

        printf("\n");

        Thread_sleep(50000);

close_connection:
        IedConnection_close(con);
	}
    else {
        printf("Failed to connect to %s:%i\n", hostname, tcpPort);
    }

    IedConnection_destroy(con);

    return 0;
}
