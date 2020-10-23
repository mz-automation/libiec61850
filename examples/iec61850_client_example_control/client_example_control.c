/*
 * client_example_control.c
 *
 * How to control a device ... intended to be used with server_example_control
 */

#include "iec61850_client.h"
#include "hal_thread.h"

#include <stdlib.h>
#include <stdio.h>

static void commandTerminationHandler(void *parameter, ControlObjectClient connection)
{


    LastApplError lastApplError = ControlObjectClient_getLastApplError(connection);

    /* if lastApplError.error != 0 this indicates a CommandTermination- */
    if (lastApplError.error != 0) {
        printf("Received CommandTermination-.\n");
        printf(" LastApplError: %i\n", lastApplError.error);
        printf("      addCause: %i\n", lastApplError.addCause);
    }
    else
        printf("Received CommandTermination+.\n");
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


        /************************
         * Direct control
         ***********************/

        ControlObjectClient control
            = ControlObjectClient_create("simpleIOGenericIO/GGIO1.SPCSO1", con);

        MmsValue* ctlVal = MmsValue_newBoolean(true);

        ControlObjectClient_setOrigin(control, NULL, 3);

        if (ControlObjectClient_operate(control, ctlVal, 0 /* operate now */)) {
            printf("simpleIOGenericIO/GGIO1.SPCSO1 operated successfully\n");
        }
        else {
            printf("failed to operate simpleIOGenericIO/GGIO1.SPCSO1\n");
        }

        MmsValue_delete(ctlVal);

        ControlObjectClient_destroy(control);

        /* Check if status value has changed */

        MmsValue* stVal = IedConnection_readObject(con, &error, "simpleIOGenericIO/GGIO1.SPCSO1.stVal", IEC61850_FC_ST);

        if (error == IED_ERROR_OK) {
            bool state = MmsValue_getBoolean(stVal);
            MmsValue_delete(stVal);

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


        /****************************************
         * Direct control with enhanced security
         ****************************************/

        control = ControlObjectClient_create("simpleIOGenericIO/GGIO1.SPCSO3", con);

        ControlObjectClient_setCommandTerminationHandler(control, commandTerminationHandler, NULL);

        ctlVal = MmsValue_newBoolean(true);

        if (ControlObjectClient_operate(control, ctlVal, 0 /* operate now */)) {
            printf("simpleIOGenericIO/GGIO1.SPCSO3 operated successfully\n");
        }
        else {
            printf("failed to operate simpleIOGenericIO/GGIO1.SPCSO3\n");
        }

        MmsValue_delete(ctlVal);

        /* Wait for command termination message */
        Thread_sleep(1000);

        ControlObjectClient_destroy(control);

        /* Check if status value has changed */

       stVal = IedConnection_readObject(con, &error, "simpleIOGenericIO/GGIO1.SPCSO3.stVal", IEC61850_FC_ST);

        if (error == IED_ERROR_OK) {
            bool state = MmsValue_getBoolean(stVal);

            printf("New status of simpleIOGenericIO/GGIO1.SPCSO3.stVal: %i\n", state);

            MmsValue_delete(stVal);
        }
        else {
            printf("Reading status for simpleIOGenericIO/GGIO1.SPCSO3 failed!\n");
        }

        /***********************************************
         * Select before operate with enhanced security
         ***********************************************/

        control = ControlObjectClient_create("simpleIOGenericIO/GGIO1.SPCSO4", con);

        ControlObjectClient_setCommandTerminationHandler(control, commandTerminationHandler, NULL);

        ctlVal = MmsValue_newBoolean(true);

        if (ControlObjectClient_selectWithValue(control, ctlVal)) {

            if (ControlObjectClient_operate(control, ctlVal, 0 /* operate now */)) {
                printf("simpleIOGenericIO/GGIO1.SPCSO4 operated successfully\n");
            }
            else {
                printf("failed to operate simpleIOGenericIO/GGIO1.SPCSO4!\n");
            }

        }
        else {
            printf("failed to select simpleIOGenericIO/GGIO1.SPCSO4!\n");
        }

        MmsValue_delete(ctlVal);

        /* Wait for command termination message */
        Thread_sleep(1000);

        ControlObjectClient_destroy(control);


        /*********************************************************************
         * Direct control with enhanced security (expect CommandTermination-)
         *********************************************************************/

        control = ControlObjectClient_create("simpleIOGenericIO/GGIO1.SPCSO9", con);

        ControlObjectClient_setCommandTerminationHandler(control, commandTerminationHandler, NULL);

        ctlVal = MmsValue_newBoolean(true);

        if (ControlObjectClient_operate(control, ctlVal, 0 /* operate now */)) {
            printf("simpleIOGenericIO/GGIO1.SPCSO9 operated successfully\n");
        }
        else {
            printf("failed to operate simpleIOGenericIO/GGIO1.SPCSO9\n");
        }

        MmsValue_delete(ctlVal);

        /* Wait for command termination message */
        Thread_sleep(1000);

        ControlObjectClient_destroy(control);


        IedConnection_close(con);
    }
    else {
    	printf("Connection failed!\n");
    }

    IedConnection_destroy(con);
    return 0;
}


