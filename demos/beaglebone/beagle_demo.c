/*
 *  beagle_demo.c
 *
 *  This demo shows how to connect the libiec61850 server stack to a real device.
 */

#include "iec61850_server.h"
#include "iso_server.h"
#include "acse.h"
#include "hal_thread.h"
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "static_model.h"

#include "beaglebone_leds.h"

/* import IEC 61850 device model created from SCL-File */
extern IedModel iedModel;

static int running = 0;
static IedServer iedServer = NULL;

static bool automaticOperationMode = true;
static ClientConnection controllingClient = NULL;

static uint32_t dpcState = 0;

void sigint_handler(int signalId)
{
    running = 0;
}

static void
connectionIndicationHandler(IedServer server, ClientConnection connection, bool connected, void *parameter)
{
    const char *clientAddress = ClientConnection_getPeerAddress(connection);

    if (connected)
    {
        printf("BeagleDemoServer: new client connection from %s\n", clientAddress);
    }
    else
    {
        printf("BeagleDemoServer: client connection from %s closed\n", clientAddress);

        if (controllingClient == connection)
        {
            printf("Controlling client has closed connection -> switch to automatic operation mode\n");
            controllingClient = NULL;
            automaticOperationMode = true;
        }
    }
}

static CheckHandlerResult
performCheckHandler(ControlAction action, void *parameter, MmsValue *ctlVal, bool test, bool interlockCheck, ClientConnection connection)
{
    if (controllingClient == NULL)
    {
        printf("Client takes control -> switch to remote control operation mode\n");
        controllingClient = connection;
        automaticOperationMode = false;
    }

    /* test command not accepted if mode is "on" */
    if (test)
        return CONTROL_TEMPORARILY_UNAVAILABLE;

    /* If there is already another client that controls the device reject the control attempt */
    if (controllingClient == connection)
        return CONTROL_ACCEPTED;
    else
        return CONTROL_TEMPORARILY_UNAVAILABLE;
}

static void
updateLED1stVal(bool newLedState, uint64_t timeStamp)
{
    switchLED(LED1, newLedState);

    IedServer_updateUTCTimeAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO1_t, timeStamp);
    IedServer_updateBooleanAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO1_stVal, newLedState);
    IedServer_updateQuality(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO1_q, QUALITY_VALIDITY_GOOD | QUALITY_SOURCE_SUBSTITUTED);
}

static void
updateLED2stVal(bool newLedState, uint64_t timeStamp)
{
    switchLED(LED2, newLedState);

    IedServer_updateUTCTimeAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO2_t, timeStamp);
    IedServer_updateBooleanAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO2_stVal, newLedState);
    IedServer_updateQuality(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO2_q, QUALITY_VALIDITY_QUESTIONABLE | QUALITY_DETAIL_OSCILLATORY);
}

static void
updateLED3stVal(bool newLedState, uint64_t timeStamp)
{
    switchLED(LED3, newLedState);

    IedServer_updateUTCTimeAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO3_t, timeStamp);
    IedServer_updateBooleanAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO3_stVal, newLedState);
    IedServer_updateQuality(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO3_q, QUALITY_VALIDITY_GOOD);
}

static ControlHandlerResult
controlHandlerForBinaryOutput(ControlAction action, void *parameter, MmsValue *value, bool test)
{
    if (test)
        return CONTROL_RESULT_OK;

    uint64_t timeStamp = Hal_getTimeInMs();

    bool newState = MmsValue_getBoolean(value);

    if (parameter == IEDMODEL_GenericIO_GGIO1_SPCSO1)
        updateLED1stVal(newState, timeStamp);

    if (parameter == IEDMODEL_GenericIO_GGIO1_SPCSO2)
        updateLED2stVal(newState, timeStamp);

    if (parameter == IEDMODEL_GenericIO_GGIO1_SPCSO3)
        updateLED3stVal(newState, timeStamp);

    if (parameter == IEDMODEL_GenericIO_GGIO1_DPCSO1) /* example for Double Point Control - DPC */
    {
        Dbpos dpcState = DBPOS_INTERMEDIATE_STATE;

        IedServer_updateDbposValue(iedServer, IEDMODEL_GenericIO_GGIO1_DPCSO1_stVal, dpcState);
        IedServer_updateUTCTimeAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_DPCSO1_t, timeStamp);

        if (newState)
        {
            flashLED(LED4);
            Thread_sleep(3000);
            switchLED(LED4, 1);
            dpcState = DBPOS_ON;
        }
        else
        {
            flashLED(LED4);
            Thread_sleep(3000);
            switchLED(LED4, 0);
            dpcState = DBPOS_OFF;
        }

        IedServer_updateDbposValue(iedServer, IEDMODEL_GenericIO_GGIO1_DPCSO1_stVal, dpcState);
        IedServer_updateUTCTimeAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_DPCSO1_t, Hal_getTimeInMs());
    }

    return CONTROL_RESULT_OK;
}

static int ledOnTimeMs = 1000;
static int ledOffTimeMs = 1000;
static int32_t opCnt = 0;

static ControlHandlerResult
controlHandlerForInt32Controls(ControlAction action, void *parameter, MmsValue *value, bool test)
{
    if (test)
        return CONTROL_RESULT_OK;

    if (parameter == IEDMODEL_GenericIO_TIM_GAPC1_OpCntRs)
    {
        int32_t newValue = MmsValue_toInt32(value);

        opCnt = newValue;

        uint64_t currentTime = Hal_getTimeInMs();

        IedServer_updateUTCTimeAttributeValue(iedServer, IEDMODEL_GenericIO_TIM_GAPC1_OpCntRs_t, currentTime);
        IedServer_updateInt32AttributeValue(iedServer, IEDMODEL_GenericIO_TIM_GAPC1_OpCntRs_stVal, opCnt);
    }

    return CONTROL_RESULT_OK;
}

static MmsDataAccessError
int32WriteAccessHandler(DataAttribute *dataAttribute, MmsValue *value, ClientConnection connection, void *parameter)
{
    int newValue = MmsValue_toInt32(value);

    /* Check if value is inside of valid range */
    if (newValue < 0)
        return DATA_ACCESS_ERROR_OBJECT_VALUE_INVALID;

    if (dataAttribute == IEDMODEL_GenericIO_TIM_GAPC1_OpDlTmms_setVal)
    {
        printf("New value for TIM_GAPC1.OpDlTmms.setVal = %i\n", newValue);

        ledOffTimeMs = newValue;

        return DATA_ACCESS_ERROR_SUCCESS;
    }

    if (dataAttribute == IEDMODEL_GenericIO_TIM_GAPC1_RsDlTmms_setVal)
    {
        printf("New value for TIM_GAPC1.RsDlTmms.setVal = %i\n", newValue);

        ledOnTimeMs = newValue;

        return DATA_ACCESS_ERROR_SUCCESS;
    }

    return DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED;
}

int main(int argc, char **argv)
{
    initLEDs();

    iedServer = IedServer_create(&iedModel);

    /* Set control callback handlers */
    IedServer_setConnectionIndicationHandler(iedServer, (IedConnectionIndicationHandler)connectionIndicationHandler, NULL);

    IedServer_setPerformCheckHandler(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO1,
                                     (ControlPerformCheckHandler)performCheckHandler, IEDMODEL_GenericIO_GGIO1_SPCSO1);

    IedServer_setControlHandler(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO1, (ControlHandler)controlHandlerForBinaryOutput,
                                IEDMODEL_GenericIO_GGIO1_SPCSO1);

    IedServer_setPerformCheckHandler(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO2,
                                     (ControlPerformCheckHandler)performCheckHandler, IEDMODEL_GenericIO_GGIO1_SPCSO2);

    IedServer_setControlHandler(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO2, (ControlHandler)controlHandlerForBinaryOutput,
                                IEDMODEL_GenericIO_GGIO1_SPCSO2);

    IedServer_setPerformCheckHandler(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO3,
                                     (ControlPerformCheckHandler)performCheckHandler, IEDMODEL_GenericIO_GGIO1_SPCSO3);

    IedServer_setControlHandler(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO3, (ControlHandler)controlHandlerForBinaryOutput,
                                IEDMODEL_GenericIO_GGIO1_SPCSO3);

    IedServer_setPerformCheckHandler(iedServer, IEDMODEL_GenericIO_GGIO1_DPCSO1,
                                     (ControlPerformCheckHandler)performCheckHandler, IEDMODEL_GenericIO_GGIO1_DPCSO1);

    IedServer_setControlHandler(iedServer, IEDMODEL_GenericIO_GGIO1_DPCSO1, (ControlHandler)controlHandlerForBinaryOutput,
                                IEDMODEL_GenericIO_GGIO1_DPCSO1);

    IedServer_setControlHandler(iedServer, IEDMODEL_GenericIO_TIM_GAPC1_OpCntRs, (ControlHandler)controlHandlerForInt32Controls,
                                IEDMODEL_GenericIO_TIM_GAPC1_OpCntRs);

    /* Initialize process values */
    MmsValue *DPCSO1_stVal = IedServer_getAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_DPCSO1_stVal);
    MmsValue_setBitStringFromInteger(DPCSO1_stVal, 1); /* set DPC to OFF */

    /* Intitalize setting values */
    IedServer_updateInt32AttributeValue(iedServer, IEDMODEL_GenericIO_TIM_GAPC1_OpDlTmms_setVal, ledOffTimeMs);
    IedServer_updateInt32AttributeValue(iedServer, IEDMODEL_GenericIO_TIM_GAPC1_RsDlTmms_setVal, ledOnTimeMs);

    /* Set callback handler for settings */
    IedServer_handleWriteAccess(iedServer, IEDMODEL_GenericIO_TIM_GAPC1_OpDlTmms_setVal, int32WriteAccessHandler, NULL);
    IedServer_handleWriteAccess(iedServer, IEDMODEL_GenericIO_TIM_GAPC1_RsDlTmms_setVal, int32WriteAccessHandler, NULL);

    /* MMS server will be instructed to start listening to client connections. */
    IedServer_start(iedServer, 102);

    if (!IedServer_isRunning(iedServer))
    {
        printf("Starting server failed! Exit.\n");
        IedServer_destroy(iedServer);
        exit(-1);
    }

    running = 1;

    signal(SIGINT, sigint_handler);

    float t = 0.f;

    bool ledStateValue = false;

    uint64_t nextLedToggleTime = Hal_getTimeInMs() + 1000;

    while (running)
    {
        uint64_t currentTime = Hal_getTimeInMs();

        if (automaticOperationMode)
        {
            if (nextLedToggleTime <= currentTime)
            {
                if (ledStateValue)
                    nextLedToggleTime = currentTime + ledOffTimeMs;
                else
                    nextLedToggleTime = currentTime + ledOnTimeMs;

                ledStateValue = !ledStateValue;

                if (ledStateValue)
                {
                    opCnt++;
                    IedServer_updateUTCTimeAttributeValue(iedServer, IEDMODEL_GenericIO_TIM_GAPC1_OpCntRs_t, currentTime);
                    IedServer_updateInt32AttributeValue(iedServer, IEDMODEL_GenericIO_TIM_GAPC1_OpCntRs_stVal, opCnt);
                }

                updateLED1stVal(ledStateValue, currentTime);
                updateLED2stVal(ledStateValue, currentTime);
                updateLED3stVal(ledStateValue, currentTime);
            }
        }

        t += 0.1f;

        IedServer_lockDataModel(iedServer);

        IedServer_updateFloatAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_AnIn1_mag_f, sinf(t));
        IedServer_updateQuality(iedServer, IEDMODEL_GenericIO_GGIO1_AnIn1_q, QUALITY_VALIDITY_GOOD);
        IedServer_updateUTCTimeAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_AnIn1_t, currentTime);

        IedServer_updateFloatAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_AnIn2_mag_f, sinf(t + 1.f));
        IedServer_updateQuality(iedServer, IEDMODEL_GenericIO_GGIO1_AnIn2_q, QUALITY_VALIDITY_GOOD);
        IedServer_updateUTCTimeAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_AnIn2_t, currentTime);

        IedServer_updateFloatAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_AnIn3_mag_f, sinf(t + 2.f));
        IedServer_updateQuality(iedServer, IEDMODEL_GenericIO_GGIO1_AnIn3_q, QUALITY_VALIDITY_GOOD);
        IedServer_updateUTCTimeAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_AnIn3_t, currentTime);

        IedServer_updateFloatAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_AnIn4_mag_f, sinf(t + 3.f));
        IedServer_updateQuality(iedServer, IEDMODEL_GenericIO_GGIO1_AnIn4_q, QUALITY_VALIDITY_GOOD);
        IedServer_updateUTCTimeAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_AnIn4_t, currentTime);

        IedServer_unlockDataModel(iedServer);

        Thread_sleep(100);
    }

    /* stop MMS server - close TCP server socket and all client sockets */
    IedServer_stop(iedServer);

    /* Cleanup - free all resources */
    IedServer_destroy(iedServer);
} /* main() */
