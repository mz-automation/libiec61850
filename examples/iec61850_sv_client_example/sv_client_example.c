/*
 * sv_client_example.c
 *
 * This example is intended to show how SV control blocks are accessed
 */

#include "iec61850_client.h"

#include <stdlib.h>
#include <stdio.h>

#include "hal_thread.h"

static void
printDstAddr(PhyComAddress dstAddress)
{
    printf("  addr: ");

    int i;
    for (i = 0; i < 6; i++)
        printf("%02x", dstAddress.dstAddress[i]);

    printf("\n  prio:  %u\n", dstAddress.vlanPriority);
    printf("  vid:   %u\n", dstAddress.vlanId);
    printf("  appID:  %u\n", dstAddress.appId);
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

            char* datSetName = ClientSVControlBlock_getDatSet(svcb);

            if (datSetName != NULL) {
                printf("DatSet: %s\n", datSetName);
                free(datSetName);
            }

            printf("ConfRev: %i\n", ClientSVControlBlock_getConfRev(svcb));
            printf("SmpRate: %i\n", ClientSVControlBlock_getSmpRate(svcb));
            printf("SmpMod: %i\n", ClientSVControlBlock_getSmpMod(svcb));

            int optFlds = ClientSVControlBlock_getOptFlds(svcb);

            printf("OptFlds: ");
            if (optFlds & IEC61850_SV_OPT_REFRESH_TIME)
                printf("refresh-time ");
            if (optFlds & IEC61850_SV_OPT_SAMPLE_SYNC)
                printf("sample-synch ");
            if (optFlds & IEC61850_SV_OPT_SAMPLE_RATE)
                printf("sample-rate ");
            if (optFlds & IEC61850_SV_OPT_DATA_SET)
                printf("date-set ");
            if (optFlds & IEC61850_SV_OPT_SECURITY)
                printf("security ");
            printf("\n");

            printf("noASDU: %i\n", ClientSVControlBlock_getNoASDU(svcb));

            PhyComAddress dstAddress = ClientSVControlBlock_getDstAddress(svcb);

            printDstAddr(dstAddress);

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
    return 0;
}


