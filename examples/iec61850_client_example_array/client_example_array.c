/*
 * client_example_array.c
 *
 * SHows how to handle array access from client side
 *
 * This example is intended to be used with server_example_complex_array
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

        /* read complete array */
        MmsValue* array = IedConnection_readObject(con, &error, "testComplexArray/MHAI1.HA.phsAHar", IEC61850_FC_MX);

        if (array != NULL) {
            MmsValue_delete(array);
        }
        else
            printf("Failed to read array!\n");

        /* read array element at different component levels */
        char* arrayElementRef = "testComplexArray/MHAI1.HA.phsAHar(2).cVal.mag.f";

        MmsValue* element = IedConnection_readObject(con, &error, arrayElementRef, IEC61850_FC_MX);

        if (element != NULL) {
            MmsValue_delete(element);
        }
        else
            printf("Failed to read array element %s!\n", arrayElementRef);


        arrayElementRef = "testComplexArray/MHAI1.HA.phsAHar(2).cVal.mag";

        element = IedConnection_readObject(con, &error, arrayElementRef, IEC61850_FC_MX);

        if (element != NULL) {
            MmsValue_delete(element);
        }
        else
            printf("Failed to read array element %s!\n", arrayElementRef);

        arrayElementRef = "testComplexArray/MHAI1.HA.phsAHar(2).cVal";

        element = IedConnection_readObject(con, &error, arrayElementRef, IEC61850_FC_MX);

        if (element != NULL) {
            MmsValue_delete(element);
        }
        else
            printf("Failed to read array element %s!\n", arrayElementRef);

        arrayElementRef = "testComplexArray/MHAI1.HA.phsAHar(2)";

        element = IedConnection_readObject(con, &error, arrayElementRef, IEC61850_FC_MX);

        if (element != NULL) {
            MmsValue_delete(element);
        }
        else
            printf("Failed to read array element %s!\n", arrayElementRef);

        close_connection:

        IedConnection_close(con);
    }
    else {
        printf("Failed to connect to %s:%i\n", hostname, tcpPort);
    }

    IedConnection_destroy(con);
    return 0;
}


