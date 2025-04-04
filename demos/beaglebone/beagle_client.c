/*
 * beagle_client.c
 *
 * Switch LEDs on the BeagleBone
 */

#include "iec61850_client.h"
#include "hal_thread.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    char *hostname;
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
        /************************
         * Direct control
         ***********************/

        bool led4State = false;

        ControlObjectClient controlLED1 = ControlObjectClient_create("beagleGenericIO/GGIO1.SPCSO1", con);

        ControlObjectClient controlLED2 = ControlObjectClient_create("beagleGenericIO/GGIO1.SPCSO2", con);

        ControlObjectClient controlLED3 = ControlObjectClient_create("beagleGenericIO/GGIO1.SPCSO3", con);

        ControlObjectClient controlLED4 = ControlObjectClient_create("beagleGenericIO/GGIO1.DPCSO1", con);

        MmsValue *ctlValOn = MmsValue_newBoolean(true);

        MmsValue *ctlValOff = MmsValue_newBoolean(false);

        if (!ControlObjectClient_operate(controlLED1, ctlValOff, 0))
            goto control_error;

        ControlObjectClient_select(controlLED2);
        if (!ControlObjectClient_operate(controlLED2, ctlValOff, 0))
            goto control_error;

        if (!ControlObjectClient_operate(controlLED4, ctlValOff, 0))
            goto control_error;

        while (1)
        {
            if (!ControlObjectClient_operate(controlLED3, ctlValOff, 0))
                goto control_error;
            if (!ControlObjectClient_operate(controlLED1, ctlValOn, 0))
                goto control_error;

            Thread_sleep(1000);

            if (!ControlObjectClient_operate(controlLED1, ctlValOff, 0))
                goto control_error;

            ControlObjectClient_select(controlLED2);
            if (!ControlObjectClient_operate(controlLED2, ctlValOn, 0))
                goto control_error;

            Thread_sleep(1000);

            ControlObjectClient_select(controlLED2);
            if (!ControlObjectClient_operate(controlLED2, ctlValOff, 0))
                goto control_error;

            if (!ControlObjectClient_operate(controlLED3, ctlValOn, 0))
                goto control_error;

            Thread_sleep(1000);

            if (led4State == false)
            {
                if (!ControlObjectClient_operate(controlLED4, ctlValOn, 0))
                    goto control_error;
                led4State = true;
            }
            else
            {
                if (!ControlObjectClient_operate(controlLED4, ctlValOff, 0))
                    goto control_error;
                led4State = false;
            }
        }

        goto exit_control_loop;

    control_error:
        printf("Error controlling device!\n");

    exit_control_loop:

        MmsValue_delete(ctlValOn);
        MmsValue_delete(ctlValOff);

        ControlObjectClient_destroy(controlLED1);
        ControlObjectClient_destroy(controlLED2);
        ControlObjectClient_destroy(controlLED3);
        ControlObjectClient_destroy(controlLED4);

        IedConnection_close(con);
    }
    else
    {
        printf("Connection failed!\n");
    }

    IedConnection_destroy(con);
}
