/*
 * client_example5.c
 *
 * - How to change connection parameters of the lower layers of MMS
 * - How to use password authentication
 *
 */

#include "iec61850_client.h"

#include <stdlib.h>
#include <stdio.h>

#include "hal_thread.h"


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

    /* To change MMS parameters you need to get access to the underlying MmsConnection */
    MmsConnection mmsConnection = IedConnection_getMmsConnection(con);

    /* Get the container for the parameters */
    IsoConnectionParameters parameters = MmsConnection_getIsoConnectionParameters(mmsConnection);

    /* set remote AP-Title according to SCL file example from IEC 61850-8-1 */
    IsoConnectionParameters_setRemoteApTitle(parameters, "1.3.9999.13", 12);

    /* just some arbitrary numbers */
    IsoConnectionParameters_setLocalApTitle(parameters, "1.2.1200.15.3", 1);

    /* use this to skip AP-Title completely - this may be required by some "obscure" servers */
//    IsoConnectionParameters_setRemoteApTitle(parameters, NULL, 0);
//    IsoConnectionParameters_setLocalApTitle(parameters, NULL, 0);
    TSelector localTSelector = { 3, { 0x00, 0x01, 0x02 } };
    TSelector remoteTSelector = { 2, { 0x00, 0x01 } };

    SSelector remoteSSelector = { 2, { 0, 1 } };
    SSelector localSSelector = { 5, { 0, 1, 2, 3, 4 } };

    PSelector localPSelector = {4, { 0x12, 0x34, 0x56, 0x78 } };
    PSelector remotePSelector = {4, { 0x87, 0x65, 0x43, 0x21 } };

    /* change parameters for presentation, session and transport layers */
    IsoConnectionParameters_setRemoteAddresses(parameters, remotePSelector, remoteSSelector, localTSelector);
    IsoConnectionParameters_setLocalAddresses(parameters, localPSelector, localSSelector, remoteTSelector);

    char* password = "top secret";

    /* use authentication */
    AcseAuthenticationParameter auth = AcseAuthenticationParameter_create();
    AcseAuthenticationParameter_setAuthMechanism(auth, ACSE_AUTH_PASSWORD);
    AcseAuthenticationParameter_setPassword(auth, password);

    IsoConnectionParameters_setAcseAuthenticationParameter(parameters, auth);

    IedConnection_setConnectTimeout(con, 10000);

    /* call connect when all parameters are set */
    IedConnection_connect(con, &error, hostname, tcpPort);

    if (error == IED_ERROR_OK) {

        Thread_sleep(1000);

        IedConnection_abort(con, &error);
    }
    else {
        printf("Failed to connect to %s:%i\n", hostname, tcpPort);
    }

    IedConnection_destroy(con);

    AcseAuthenticationParameter_destroy(auth);
    return 0;
}


