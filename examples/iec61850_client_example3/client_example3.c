/*
 * client_example3.c
 *
 * How to control a device ... intended to be used with server_example_control
 */

#include "iec61850_client.h"

#include <stdlib.h>
#include <stdio.h>

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


        /************************
         * Direct control
         ***********************/

        ControlObjectClient control
            = ControlObjectClient_create("simpleIOGenericIO/GGIO1.SPCSO1", con);

        MmsValue* ctlVal = MmsValue_newBoolean(true);

        if (ControlObjectClient_operate(control, ctlVal, 0 /* operate now */)) {
            printf("simpleIOGenericIO/GGIO1.SPCSO1 operated successfully\n");
        }
        else {
            printf("failed to operate simpleIOGenericIO/GGIO1.SPCSO1\n");
        }

        MmsValue_delete(ctlVal);

        ControlObjectClient_destroy(control);

        /* Check if status value has changed */

        MmsValue* stVal = IedConnection_readObject(con, &error, "simpleIOGenericIO/GGIO1.SPCSO1.stVal", ST);

        if (error == IED_ERROR_OK) {
            bool state = MmsValue_getBoolean(stVal);

            printf("New status of simpleIOGenericIO/GGIO1.SPCSO1.stVal: %i\n", state);
        }
        else {
            printf("Reading status for simpleIOGenericIO/GGIO1.SPCSO1 failed!\n");
        }


        /************************
         * Select before operate
         ***********************/

        control = ControlObjectClient_create("simpleIOGenericIO/GGIO1.SPCSO2", con);

        if (ControlObjectClient_select(control)) {

            ctlVal = MmsValue_newBoolean(true);

            if (ControlObjectClient_operate(control, ctlVal, 0 /* operate now */)) {
                printf("simpleIOGenericIO/GGIO1.SPCSO2 operated successfully\n");
            }
            else {
                printf("failed to operate simpleIOGenericIO/GGIO1.SPCSO2!\n");
            }

            MmsValue_delete(ctlVal);
        }
        else {
            printf("failed to select simpleIOGenericIO/GGIO1.SPCSO2!\n");
        }

        ControlObjectClient_destroy(control);



        IedConnection_close(con);
    }
    else {
    	printf("Connection failed!\n");
    }

    IedConnection_destroy(con);
}


