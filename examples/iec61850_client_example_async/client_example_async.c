/*
 * client_example_async.c
 *
 * Shows how to use the asynchronous client API
 *
 * This example is intended to be used with server_example_basic_io or server_example_goose.
 */

#include "iec61850_client.h"

#include <stdlib.h>
#include <stdio.h>

#include "hal_thread.h"

void
reportCallbackFunction(void* parameter, ClientReport report)
{
    MmsValue* dataSetValues = ClientReport_getDataSetValues(report);

    printf("received report for %s\n", ClientReport_getRcbReference(report));

    int i;
    for (i = 0; i < 4; i++) {
        ReasonForInclusion reason = ClientReport_getReasonForInclusion(report, i);

        if (reason != IEC61850_REASON_NOT_INCLUDED) {
            printf("  GGIO1.SPCSO%i.stVal: %i (included for reason %i)\n", i,
                    MmsValue_getBoolean(MmsValue_getElement(dataSetValues, i)), reason);
        }
    }
}

static void
printValue(char* name, MmsValue* value)
{
    char buf[1000];

    MmsValue_printToBuffer(value, buf, 1000);

    printf("Received value for %s: %s\n", name, buf);
}

static void
readObjectHandler (uint32_t invokeId, void* parameter, IedClientError err, MmsValue* value)
{
    if (err == IED_ERROR_OK) {
        printValue((char*) parameter, value);

        MmsValue_delete(value);
    }
    else {
        printf("Failed to read object %s (err=%i)\n", (char*) parameter, err);
    }
}

static void
getVarSpecHandler (uint32_t invokeId, void* parameter, IedClientError err, MmsVariableSpecification* spec)
{
    if (err == IED_ERROR_OK) {
        printf("variable: %s has type %d\n", (char*) parameter, MmsVariableSpecification_getType(spec));

        MmsVariableSpecification_destroy(spec);
    }
    else {
        printf("Failed to get variable specification for object %s (err=%i)\n", (char*) parameter, err);
    }
}

static void
getNameListHandler(uint32_t invokeId, void* parameter, IedClientError err, LinkedList nameList, bool moreFollows)
{
    if (err != IED_ERROR_OK) {
        printf("Get name list error: %d\n", err);
    }
    else {

        char* ldName = (char*) parameter;

        LinkedList element = LinkedList_getNext(nameList);

        while (element) {

            char* variableName = (char*) LinkedList_getData(element);

            printf("  %s/%s\n", ldName, variableName);

            element = LinkedList_getNext(element);
        }

        LinkedList_destroy(nameList);

        free(ldName);
    }
}

static void
getServerDirectoryHandler(uint32_t invokeId, void* parameter, IedClientError err, LinkedList nameList, bool moreFollows)
{
    IedConnection con = (IedConnection) parameter;

    if (err != IED_ERROR_OK) {
        printf("Get server directory error: %d\n", err);
    }
    else {
        LinkedList element = LinkedList_getNext(nameList);

        while (element) {

            char* ldName = (char*) LinkedList_getData(element);

            printf("LD: %s\n", ldName);

            IedClientError cerr;

            IedConnection_getLogicalDeviceVariablesAsync(con, &cerr, ldName, NULL, NULL, getNameListHandler, strdup(ldName));

            element = LinkedList_getNext(element);
        }

        LinkedList_destroy(nameList);
    }
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

    IedConnection_connectAsync(con, &error, hostname, tcpPort);

    if (error == IED_ERROR_OK) {

        bool success = true;

        while (IedConnection_getState(con) != IED_STATE_CONNECTED) {

            if (IedConnection_getState(con) == IED_STATE_CLOSED) {
                success = false;
                break;
            }

            Thread_sleep(10);
        }

        if (success) {

            IedConnection_getServerDirectoryAsync(con, &error, NULL, NULL, getServerDirectoryHandler, con);

            if (error != IED_ERROR_OK) {
                printf("read server directory error %i\n", error);
            }

            Thread_sleep(1000);


            IedConnection_readObjectAsync(con, &error, "simpleIOGenericIO/GGIO1.AnIn1.mag.f", IEC61850_FC_MX, readObjectHandler, "simpleIOGenericIO/GGIO1.AnIn1.mag.f");

            if (error != IED_ERROR_OK) {
                printf("read object error %i\n", error);
            }

            IedConnection_readObjectAsync(con, &error, "simpleIOGenericIO/GGIO1.AnIn2.mag.f", IEC61850_FC_MX, readObjectHandler, "simpleIOGenericIO/GGIO1.AnIn2.mag.f");

            if (error != IED_ERROR_OK) {
                printf("read object error %i\n", error);
            }

            IedConnection_getVariableSpecificationAsync(con, &error, "simpleIOGenericIO/GGIO1.AnIn1", IEC61850_FC_MX, getVarSpecHandler, "simpleIOGenericIO/GGIO1.AnIn1");

            if (error != IED_ERROR_OK) {
                printf("get variable specification error %i\n", error);
            }
        }

        Thread_sleep(1000);

        IedConnection_releaseAsync(con, &error);

        if (error != IED_ERROR_OK) {
            printf("Release returned error: %d\n", error);
        }
        else {

            while (IedConnection_getState(con) != IED_STATE_CLOSED) {
                Thread_sleep(10);
            }
        }
    }
    else {
        printf("Failed to connect to %s:%i\n", hostname, tcpPort);
    }

    IedConnection_destroy(con);
}


