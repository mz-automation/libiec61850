/*
 * client_example_no_thread.c
 *
 * Shows how to use IedConnection in non-thread mode together with the asynchronous client API
 *
 * NOTE:
 * - in non-thread mode only asynchronous service functions can be used!
 * - the IedConnection_tick function has to be called periodically
 * - the asynchronous API returns service results by callback functions
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

/* callback function for IedConnection_readObjectAsync */
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

/* callback function for IedConnection_readDataSetValuesAsync */
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
                MmsValue* accessResultValue = LinkedList_getData(element);

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
getControlVariableVarSpecHandler(uint32_t invokeId, void* parameter, IedClientError err, MmsVariableSpecification* spec)
{
    if (err == IED_ERROR_OK) {

        MmsVariableSpecification** ctlVarSpec = (MmsVariableSpecification**)parameter;

        *ctlVarSpec = spec;
    }
    else {
        printf("Failed to get variable specification for object %s (err=%i)\n", (char*) parameter, err);
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

/* callback function for service functions IedConnection_getLogicalDeviceVariablesAsync, IedConnection_getLogicalDeviceDataSetsAsync */
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

/* callback function for IedConnection_getServerDirectoryAsync */
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

            printf("LD: %s variables:\n", ldName);

            IedConnection_getLogicalDeviceVariablesAsync(con, &err, ldName, NULL, NULL, getNameListHandler, strdup(ldName));

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

static void
stateChangedHandler(void* parameter, IedConnection connection, IedConnectionState newState)
{
    printf("Connection state changed: ");

    switch (newState) {
    case IED_STATE_CLOSED:
        printf("CLOSED\n");
        break;
    case IED_STATE_CLOSING:
        printf("CLOSING\n");
        break;
    case IED_STATE_CONNECTING:
        printf("CONNECTING\n");
        break;
    case IED_STATE_CONNECTED:
        printf("CONNECTED\n");
        break;
    }
}

static void
getRCBValuesHandler (uint32_t invokeId, void* parameter, IedClientError err, ClientReportControlBlock rcb)
{
    if (parameter) {
        printf("get rcb values handler: %s (error:%i)\n", (char*)parameter, err);
    }
}

static void
genericServiceHandler (uint32_t invokeId, void* parameter, IedClientError err)
{
    if (parameter) {
        printf("generic service handler: %s (error:%i)\n", (char*)parameter, err);
    }
}

static void waitWithTick(IedConnection con, int waitMs)
{
    uint64_t startTime = Hal_getTimeInMs();

    while (Hal_getTimeInMs() < (startTime + (uint64_t) waitMs))
    {
        if (IedConnection_tick(con) == true)
            Thread_sleep(10);
    }
}

int
main(int argc, char** argv) {

    char* hostname;
    int tcpPort = 102;

    if (argc > 1)
        hostname = argv[1];
    else
        hostname = "localhost";

    if (argc > 2)
        tcpPort = atoi(argv[2]);

    IedClientError error;

    ClientReportControlBlock rcb = NULL;

    /* create new IedConnection with non-thread mode */
    IedConnection con = IedConnection_createEx(NULL, false);

    /* track connection state change */
    IedConnection_installStateChangedHandler(con, stateChangedHandler, NULL);

    /* invoke association with server */
    IedConnection_connectAsync(con, &error, hostname, tcpPort);

    if (error == IED_ERROR_OK) {

        bool success = true;

        /* Call IedConnection_tick until state is CONNECTED or CLOSED */
        while (IedConnection_getState(con) != IED_STATE_CONNECTED) {

            if (IedConnection_getState(con) == IED_STATE_CLOSED) {
                success = false;
                break;
            }

            if (IedConnection_tick(con) == true)
                Thread_sleep(10);
        }

        if (success) {

            IedConnection_getServerDirectoryAsync(con, &error, NULL, NULL, getServerDirectoryHandler, con);

            if (error != IED_ERROR_OK) {
                printf("read server directory error %i\n", error);
            }

            waitWithTick(con, 1000);

            rcb = ClientReportControlBlock_create("simpleIOGenericIO/LLN0.RP.EventsRCB01");

            /* Read RCB values */
            IedConnection_getRCBValuesAsync(con, &error, "simpleIOGenericIO/LLN0.RP.EventsRCB01", rcb, getRCBValuesHandler, NULL);

            if (error != IED_ERROR_OK) {
                printf("getRCBValues service error!\n");
            }
            else {
                waitWithTick(con, 1000);

                /* prepare the parameters of the RCP */
                ClientReportControlBlock_setResv(rcb, true);
                ClientReportControlBlock_setDataSetReference(rcb, "simpleIOGenericIO/LLN0$Events"); /* NOTE the "$" instead of "." ! */
                ClientReportControlBlock_setRptEna(rcb, true);
                ClientReportControlBlock_setGI(rcb, true);

                /* Configure the report receiver */
                IedConnection_installReportHandler(con, "simpleIOGenericIO/LLN0.RP.EventsRCB", ClientReportControlBlock_getRptId(rcb), reportCallbackFunction,
                        NULL);

                /* Write RCB parameters and enable report */
                IedConnection_setRCBValuesAsync(con, &error, rcb, RCB_ELEMENT_RESV | RCB_ELEMENT_DATSET | RCB_ELEMENT_RPT_ENA | RCB_ELEMENT_GI, true, genericServiceHandler, NULL);

                waitWithTick(con, 1000);
            }

            IedConnection_readObjectAsync(con, &error, "simpleIOGenericIO/GGIO1.AnIn1.mag.f", IEC61850_FC_MX, readObjectHandler, "simpleIOGenericIO/GGIO1.AnIn1.mag.f");

            if (error != IED_ERROR_OK) {
                printf("read object error %i\n", error);
            }

            waitWithTick(con, 1000);

            IedConnection_readObjectAsync(con, &error, "simpleIOGenericIO/GGIO1.AnIn2.mag.f", IEC61850_FC_MX, readObjectHandler, "simpleIOGenericIO/GGIO1.AnIn2.mag.f");

            if (error != IED_ERROR_OK) {
                printf("read object error %i\n", error);
            }

            waitWithTick(con, 1000);

            IedConnection_getVariableSpecificationAsync(con, &error, "simpleIOGenericIO/GGIO1.AnIn1", IEC61850_FC_MX, getVarSpecHandler, "simpleIOGenericIO/GGIO1.AnIn1");

            if (error != IED_ERROR_OK) {
                printf("get variable specification error %i\n", error);
            }

            waitWithTick(con, 1000);

            IedConnection_readDataSetValuesAsync(con, &error, "simpleIOGenericIO/LLN0.Events", NULL, readDataSetHandler, NULL);

            if (error != IED_ERROR_OK) {
                printf("read data set error %i\n", error);
            }

            waitWithTick(con, 1000);

            LinkedList values = LinkedList_create();
            LinkedList_add(values, MmsValue_newBoolean(true));
            LinkedList_add(values, MmsValue_newBoolean(false));
            LinkedList_add(values, MmsValue_newBoolean(true));
            LinkedList_add(values, MmsValue_newBoolean(false));

            IedConnection_writeDataSetValuesAsync(con, &error, "simpleIOGenericIO/LLN0.Events", values, writeDataSetHandler, NULL);

            if (error != IED_ERROR_OK) {
                printf("write data set error %i\n", error);
            }

            waitWithTick(con, 1000);

            LinkedList_destroyDeep(values, (LinkedListValueDeleteFunction) MmsValue_delete);

            waitWithTick(con, 1000);

            /* Get the variable specification for the controllable data object by online service */
            MmsVariableSpecification* ctlVarSpec = NULL;

            IedConnection_getVariableSpecificationAsync(con, &error, "simpleIOGenericIO/GGIO1.SPCSO1", IEC61850_FC_CO,
                    getControlVariableVarSpecHandler, &ctlVarSpec);

            waitWithTick(con, 1000);

            if (ctlVarSpec != NULL) {

                /* ControlObjectClient_create function cannot be used in non-thread mode - use ControlObjectClient_createEx instead */
                ControlObjectClient controlClient = ControlObjectClient_createEx("simpleIOGenericIO/GGIO1.SPCSO1", con,
                        CONTROL_MODEL_DIRECT_NORMAL, ctlVarSpec);

                if (controlClient != NULL) {

                    ControlObjectClient_setOrigin(controlClient, "test1", CONTROL_ORCAT_AUTOMATIC_REMOTE);

                    MmsValue* ctlVal = MmsValue_newBoolean(true);

                    ControlObjectClient_operateAsync(controlClient, &error, ctlVal, 0, controlActionHandler, NULL);

                    waitWithTick(con, 1000);

                    MmsValue_delete(ctlVal);

                    if (error != IED_ERROR_OK) {
                        printf("Failed to send operate %i\n", error);
                    }

                    ControlObjectClient_destroy(controlClient);
                }
                else {
                    printf("Failed to connect to control object\n");
                }

                MmsVariableSpecification_destroy(ctlVarSpec);
            }

        }

        IedConnection_releaseAsync(con, &error);

        if (error != IED_ERROR_OK) {
            printf("Release returned error: %d\n", error);
        }
        else {

            /* Call IedConnection_tick until state is CLOSED */
            while (IedConnection_getState(con) != IED_STATE_CLOSED) {
                if (IedConnection_tick(con) == true)
                    Thread_sleep(10);
            }
        }

        if (rcb != NULL)
            ClientReportControlBlock_destroy(rcb);
    }
    else {
        printf("Failed to connect to %s:%i\n", hostname, tcpPort);
    }

    if (clientDataSet)
        ClientDataSet_destroy(clientDataSet);

    IedConnection_destroy(con);
    return 0;
}


