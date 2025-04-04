/*
* client_example_ClientGooseControlAsync.c
*
* This example is intended to be used with server_example_goose.
*/

#include "iec61850_client.h"

#include <stdlib.h>
#include <stdio.h>

#include "hal_thread.h"

static ClientGooseControlBlock GOCB = NULL;

static void
getGoCBValuesHandler(uint32_t invokeId, void* parameter, IedClientError err, ClientGooseControlBlock goCB)
{
    if (err == IED_ERROR_OK)
    {
        if (goCB)
        {
            printf("Access to GoCB\n");
			
            bool GoEna = ClientGooseControlBlock_getGoEna(goCB);
            printf("GoEna Value : % d\n", GoEna);

            const char* id = ClientGooseControlBlock_getGoID(goCB);
            printf("GoID Value: %s\n", id);

            const char* datset = ClientGooseControlBlock_getDatSet(goCB);
            printf("GoDatset Value: %s\n", datset);

            GOCB = goCB;
        }
    }
    else {
        printf("Failed to get GoCV values (err=%i)\n", err);
    }
}

static void
genericServiceHandler(uint32_t invokeId, void* parameter, IedClientError err)
{
    if (err == IED_ERROR_OK) {
        printf("Set GoCB Values successful");
    }
    else {
        printf("Error triggering a report (code: %i)\n", err);
    }
}

int
main(int argc, char** argv)
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

    IedConnection_connectAsync(con, &error, hostname, tcpPort);

    if (error == IED_ERROR_OK)
    {
        bool success = true;

        while (IedConnection_getState(con) != IED_STATE_CONNECTED)
        {
            if (IedConnection_getState(con) == IED_STATE_CLOSED)
            {
                success = false;
                break;
            }

            Thread_sleep(10);
        }

        if (success)
        {	
            /*Read GoCB Values*/
            IedConnection_getGoCBValuesAsync(con, &error, "simpleIOGenericIO/LLN0.gcbEvents", NULL, getGoCBValuesHandler, NULL);

            if (error != IED_ERROR_OK) {
                printf("getGoCBValues service error! %i\n", error);
            }

            while (GOCB == NULL) {}

            if (GOCB != NULL)
            {
                /*Update Go CB Values locally*/
                ClientGooseControlBlock_setGoID(GOCB, "analog");
                ClientGooseControlBlock_setDatSet(GOCB, "simpleIOGenericIO/LLN0$AnalogValues");
                ClientGooseControlBlock_setGoEna(GOCB, false);

                /*Update Go CB Values to server (Throws error because only GoEna is writeable)*/
                IedConnection_setGoCBValuesAsync(con, &error, GOCB, GOCB_ELEMENT_GO_ID | GOCB_ELEMENT_DATSET | GOCB_ELEMENT_GO_ENA, true, genericServiceHandler, NULL);
				
                if (error != IED_ERROR_OK) {
                    printf("setGoCBValues service error: %i\n", error);
                }

                /*Test to see if the values were updated correctly on the server*/
                IedConnection_getGoCBValuesAsync(con, &error, "simpleIOGenericIO/LLN0.gcbEvents", NULL, getGoCBValuesHandler, NULL);

                ClientGooseControlBlock_destroy(GOCB);
            }
        }

        Thread_sleep(10000);

        IedConnection_releaseAsync(con, &error);

        if (error != IED_ERROR_OK)
        {
            printf("Release returned error: %d\n", error);
        }
        else
        {
            while (IedConnection_getState(con) != IED_STATE_CLOSED)
            {
                Thread_sleep(10);
            }
        }
    }
    else {
        printf("Failed to connect to %s:%i\n", hostname, tcpPort);
    }

    IedConnection_destroy(con);

    return 0;
}
