/*
 * sv_client_example.c
 *
 * This example is intended to show how SV control blocks are accessed
 */

#include "iec61850_client.h"

#include <stdlib.h>
#include <stdio.h>

#include "hal_thread.h"


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

        char* svcbRef = "simpleIOGenericIO/LLN0.Volt";

        ClientSVControlBlock svcb = ClientSVControlBlock_create(con, svcbRef);

        if (svcb != NULL) {
            if (ClientSVControlBlock_isMulticast(svcb))
                printf("SVCB is multicast\n");
            else
                printf("SVCB is unicast\n");

            if (ClientSVControlBlock_setSvEna(svcb, true))
                printf("SVCB enabled\n");
            else
                printf("Failed to enable SVCB\n");

            printf("SvEna state: %i\n", ClientSVControlBlock_getSvEna(svcb));

            char* msvID = ClientSVControlBlock_getMsvID(svcb);

            if (msvID != NULL) {
                printf("MsvID: %s\n", msvID);
                free(msvID);
            }

        }
        else {
            printf("SVCB %s does not exist on server!\n", svcbRef);
        }

        IedConnection_close(con);
    }
    else {
        printf("Failed to connect to %s:%i\n", hostname, tcpPort);
    }

    IedConnection_destroy(con);
}


