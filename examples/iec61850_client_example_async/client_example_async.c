/*
 * client_example_async.c
 *
 * Shows how to use the asynchronous client API
 *
 * This example is intended to be used with server_example_basic_io.
 */

#include "iec61850_client.h"

#include <stdlib.h>
#include <stdio.h>

#include "hal_thread.h"

static ClientDataSet clientDataSet = NULL;

static void
printValue(char* name, MmsValue* value)
{
    char buf[1000];

    MmsValue_printToBuffer(value, buf, 1000);

    printf("%s: %s\n", name, buf);
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
readDataSetHandler(uint32_t invokeId, void* parameter, IedClientError err, ClientDataSet dataSet)
{
    if (err == IED_ERROR_OK) {
        clientDataSet = dataSet;

        printf("Data set has %d entries\n", ClientDataSet_getDataSetSize(dataSet));

        MmsValue* values = ClientDataSet_getValues(dataSet);

        if (MmsValue_getType(values) == MMS_ARRAY) {
            int i;
            for (i = 0; i < MmsValue_getArraySize(values); i++) {
                printf("  [%i]", i);
                printValue("", MmsValue_getElement(values, i));
            }
        }
    }
    else {
        printf("Failed to read data set (err=%i)\n", err);
    }
}

static void
writeDataSetHandler(uint32_t invokeId, void* parameter, IedClientError err, LinkedList /* <MmsValue*> */accessResults)
{
    if (err == IED_ERROR_OK) {

        if (accessResults) {

            int i = 0;

            LinkedList element = LinkedList_getNext(accessResults);

            while (element) {
                MmsValue* accessResultValue = (MmsValue*) LinkedList_getData(element);

                printf("  access-result[%i]", i);
                printValue("", accessResultValue);


                element = LinkedList_getNext(element);
                i++;
            }

            LinkedList_destroyDeep(accessResults, (LinkedListValueDeleteFunction) MmsValue_delete);

        }
    }
    else {
        printf("Failed to write data set (err=%i)\n", err);
    }
}

static void
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

        /* Call logical device variables for each logical node */
        while (element) {

            char* ldName = (char*) LinkedList_getData(element);

            IedClientError cerr;

            printf("LD: %s variables:\n", ldName);

            IedConnection_getLogicalDeviceVariablesAsync(con, &cerr, ldName, NULL, NULL, getNameListHandler, strdup(ldName));

            printf("LD: %s data sets:\n", ldName);

            IedConnection_getLogicalDeviceDataSetsAsync(con, &err, ldName, NULL, NULL, getNameListHandler, strdup(ldName));

            element = LinkedList_getNext(element);
        }

        LinkedList_destroy(nameList);
    }
}

static void
controlActionHandler(uint32_t invokeId, void* parameter, IedClientError err, ControlActionType type, bool success)
{
    printf("control: ID: %d type: %i err: %d success: %i\n", invokeId, type, err, success);
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

            IedConnection_readDataSetValuesAsync(con, &error, "simpleIOGenericIO/LLN0.Events", NULL, readDataSetHandler, NULL);

            if (error != IED_ERROR_OK) {
                printf("read data set error %i\n", error);
            }

            LinkedList values = LinkedList_create();
            LinkedList_add(values, MmsValue_newBoolean(true));
            LinkedList_add(values, MmsValue_newBoolean(false));
            LinkedList_add(values, MmsValue_newBoolean(true));
            LinkedList_add(values, MmsValue_newBoolean(false));

            IedConnection_writeDataSetValuesAsync(con, &error, "simpleIOGenericIO/LLN0.Events", values, writeDataSetHandler, NULL);

            if (error != IED_ERROR_OK) {
                printf("write data set error %i\n", error);
            }

            LinkedList_destroyDeep(values, (LinkedListValueDeleteFunction) MmsValue_delete);

            Thread_sleep(1000);

            ControlObjectClient controlClient = ControlObjectClient_create("simpleIOGenericIO/GGIO1.SPCSO1", con);

            if (controlClient != NULL) {

                ControlObjectClient_setOrigin(controlClient, "test1", CONTROL_ORCAT_AUTOMATIC_REMOTE);

                MmsValue* ctlVal = MmsValue_newBoolean(true);

                ControlObjectClient_operateAsync(controlClient, &error, ctlVal, 0, controlActionHandler, NULL);

                if (error != IED_ERROR_OK) {
                    printf("Failed to send operate %i\n", error);
                }

            }
            else {
                printf("Failed to connect to control object\n");
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

    if (clientDataSet)
        ClientDataSet_destroy(clientDataSet);

    IedConnection_destroy(con);
    return 0;
}


