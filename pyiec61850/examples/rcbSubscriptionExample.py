#!/usr/bin/python

'''
Example of RCB subscription, with the Python wrapper

This example is intended to be used with server_example_basic_io.

Usage:
    'sudo python3 ./pyiec61850/examples/rcbSubscriptionExample.py'
  or
    'sudo python3 ./pyiec61850/examples/rcbSubscriptionExample.py localhost 102'
  or
    'python3 ./pyiec61850/examples/rcbSubscriptionExample.py localhost 8102'


Swig generates 2 wrapped objects:
    - a generic RCB handler
    - a generic RCB subscriber

The user needs to:
    - create his specific RCB handler in Python,
      for processing the received RCB as he wants
    - create his specific RCB subscriber in Python,
      with his registering parameters
    - connect his handler to his subscriber with a composition relationship
'''

import time
import sys
import pyiec61850 as iec61850


def open_connection(ip_address, mms_port):
    '''Open the connection with the IED'''
    l_connection = iec61850.IedConnection_create()
    l_error = iec61850.IedConnection_connect(l_connection, ip_address, mms_port)
    return l_error, l_connection


def close_connection(i_connection):
    '''Close the connection with the IED'''
    iec61850.IedConnection_close(i_connection)


def destroy_connection(i_connection):
    '''Destroy (free) the Connection object'''
    iec61850.IedConnection_destroy(i_connection)


class PyRCBHandler(iec61850.RCBHandler):
    '''Class processing the received RCB'''

    def __init__(self):
        iec61850.RCBHandler.__init__(self)

    def trigger(self):
        '''Method for triggering the handler and processing the last received RCB.
           In these example, we only print some attributs and data of the RCB'''

        # the following section is the application part of the Swig C subthread:
        # we must catch the Python exceptions, otherwise it will crash.
        try:
            l_rcb_ref = iec61850.ClientReport_getRcbReference(self._client_report)
            print(f"\nNew received RCB: {l_rcb_ref}")

            if iec61850.ClientReport_hasDataSetName(self._client_report):
                l_dataset_name = iec61850.ClientReport_getDataSetName(self._client_report)
                print(f"\tDataSet name: {l_dataset_name}")

            print(f"\tReport id: {iec61850.ClientReport_getRptId(self._client_report)}")

            if iec61850.ClientReport_hasSeqNum(self._client_report):
                l_seq_num = iec61850.ClientReport_getSeqNum(self._client_report)
                print(f"\tSequence num: {l_seq_num}")

            if iec61850.ClientReport_hasSubSeqNum(self._client_report):
                l_sub_seq_num = iec61850.ClientReport_getSubSeqNum(self._client_report)
                print(f"\tSub-sequence num: {l_sub_seq_num}")

            if iec61850.ClientReport_hasTimestamp(self._client_report):
                l_timestamp_millisec =iec61850.ClientReport_getTimestamp(self._client_report)
                print(f"\tTimestamp in millsec: {l_timestamp_millisec}")

            mms_value_array = iec61850.ClientReport_getDataSetValues(self._client_report)
            mms_value_array_size = iec61850.MmsValue_getArraySize(mms_value_array)
            print(f"\tDataSet size: {mms_value_array_size}")

            for mms_value_index in range(mms_value_array_size):
                mms_value = iec61850.MmsValue_getElement(mms_value_array, mms_value_index)
                mms_value_type = iec61850.MmsValue_getTypeString(mms_value)

                if mms_value_type == "boolean":
                    print(f"\tMMS value: {iec61850.MmsValue_getBoolean(mms_value)}")
                elif mms_value_type == "float":
                    print(f"\tMMS value: {iec61850.MmsValue_toFloat(mms_value)}")
                else:
                    print("\tMMS value: other type")

                l_reason = iec61850.ClientReport_getReasonForInclusion(self._client_report,
                                                                       mms_value_index)
                l_reason_str = iec61850.ReasonForInclusion_getValueAsString(l_reason)
                print(f"\tReason for inclusion: {l_reason_str}")

        except RuntimeError as l_error:
            print(f"Runtime Error (in subscriber thread): {l_error}")
        except AssertionError as l_error:
            print(f"Assertion Error (in subscriber thread): {l_error}")
        except Exception as l_exception:
            print(f"Exception (in subscriber thread): {l_exception}")


class PyRCBSubscriber:
    '''Class representing a RCB subscriber in Python,
       and that owns the RCB handler in Python for processing the received RCB'''

    def __init__(self):
        self._libiec61850_rcb_client = None
        self._internal_rcb_handler = PyRCBHandler()
        self._wrapped_rcb_subscriber = iec61850.RCBSubscriber() # generic RCB subscriber
        self._connection = None    # do not destroy it
        self._libiec61850_error_code = iec61850.IED_ERROR_OK

    def __del__(self):
        if self._connection is not None:
            self.destroy()


    def subscribe(self, i_connection, i_report_control_block_ref):
        '''Select the subscription parameters and create the RCB subscription'''

        # preconditions
        assert iec61850.IedConnection_getState(i_connection) == iec61850.IED_STATE_CONNECTED, \
               "error: Not connected"
        assert i_report_control_block_ref, "error: the reference of the ReportControlBlock is empty"
        assert self._libiec61850_rcb_client is None, "error: the RCB client is already created"

        self._connection = i_connection

        # Like the usual RCB subscription with the C API:
        # read data set
        print(f"RCBSubscription: create subscription for: '{i_report_control_block_ref}'")
        l_return_value = iec61850.pyWrap_IedConnection_getRCBValues(self._connection,
                                                                    i_report_control_block_ref,
                                                                    None)
        if isinstance(l_return_value, int):
            self._libiec61850_error_code = l_return_value
        else:
            [self._libiec61850_rcb_client, self._libiec61850_error_code] = l_return_value

        if self._libiec61850_error_code != iec61850.IED_ERROR_OK:
            raise RuntimeError('IEC61850 error')


        # Specific instructions with Python:
        # Initialize the generic and wrapped 'subscriber'
        l_client_report_control_block_id = \
                iec61850.ClientReportControlBlock_getRptId(self._libiec61850_rcb_client)
        self._wrapped_rcb_subscriber.setIedConnection(self._connection)
        self._wrapped_rcb_subscriber.setRcbReference(i_report_control_block_ref)
        self._wrapped_rcb_subscriber.setRcbRptId(l_client_report_control_block_id)

        # Specific instructions with Python:
        # Connect the specific callback/handler
        self._internal_rcb_handler.thisown = 0  # the following subscriber will be the owner of this handler
        self._wrapped_rcb_subscriber.setEventHandler(self._internal_rcb_handler)


        # Like the usual RCB subscription, same feature but with a specific Python object
        # Install handler for reports
        l_registering_status = self._wrapped_rcb_subscriber.subscribe()
        assert l_registering_status is True, "Error: Failed to register the RCBSubscriber"

        # Like the usual RCB subscription with the C API:
        # Set trigger options and enable report
        l_trigger_options = iec61850.TRG_OPT_DATA_UPDATE | \
                            iec61850.TRG_OPT_INTEGRITY | \
                            iec61850.TRG_OPT_GI | \
                            iec61850.TRG_OPT_DATA_CHANGED | \
                            iec61850.TRG_OPT_QUALITY_CHANGED
        l_rcb_attributes = iec61850.RCB_ELEMENT_RPT_ENA | iec61850.RCB_ELEMENT_TRG_OPS

        iec61850.ClientReportControlBlock_setTrgOps(self._libiec61850_rcb_client,
                                                    l_trigger_options)

        iec61850.ClientReportControlBlock_setRptEna(self._libiec61850_rcb_client, True)

        self._libiec61850_error_code = \
            iec61850.pyWrap_IedConnection_setRCBValues(self._connection,
                                                       self._libiec61850_rcb_client,
                                                       l_rcb_attributes,
                                                       True)

        # Check subscription status
        if self._libiec61850_error_code != iec61850.IED_ERROR_OK:
            raise RuntimeError('IEC61850 error')


    def destroy(self):
        '''Stop the RCB subscription and destroy the internal objects'''

        if self._libiec61850_rcb_client and \
           iec61850.IedConnection_getState(self._connection) == iec61850.IED_STATE_CONNECTED:
            # Disable reporting
            iec61850.ClientReportControlBlock_setRptEna(self._libiec61850_rcb_client, False)
            self._libiec61850_error_code = \
                iec61850.pyWrap_IedConnection_setRCBValues(self._connection,
                                                    self._libiec61850_rcb_client,
                                                    iec61850.RCB_ELEMENT_RPT_ENA,
                                                    True)
            # Check the 'disable reporting' command result
            if self._libiec61850_error_code != iec61850.IED_ERROR_OK:
                raise RuntimeError('IEC61850 error')

            # Destroy the libiec61850 objects
            if self._libiec61850_rcb_client:
                iec61850.ClientReportControlBlock_destroy(self._libiec61850_rcb_client)
                self._libiec61850_rcb_client = None

            # Destroy the RCB subscriber
            if self._wrapped_rcb_subscriber:
                del self._wrapped_rcb_subscriber
                self._wrapped_rcb_subscriber = None


# MAIN
L_RCBREF_STATUS = "simpleIOGenericIO/LLN0.RP.EventsRCB01"
L_RCBREF_MEASUREMENTS = "simpleIOGenericIO/LLN0.BR.Measurements01"

HOSTNAME = "localhost"
PORT = 102
if len(sys.argv) > 1:
    HOSTNAME = sys.argv[1]
if len(sys.argv) > 2:
    PORT = int(sys.argv[2])

[error, con] = open_connection(HOSTNAME, PORT)

if error == iec61850.IED_ERROR_OK:
    try:
        rcb_subscriber_1 = PyRCBSubscriber()
        rcb_subscriber_1.subscribe(con, L_RCBREF_STATUS)

        rcb_subscriber_2 = PyRCBSubscriber()
        rcb_subscriber_2.subscribe(con, L_RCBREF_MEASUREMENTS)

        time.sleep(3)

        rcb_subscriber_1.destroy()
        rcb_subscriber_2.destroy()

    except RuntimeError as caught_exception:
        print(f"exception: {caught_exception}")
    except AssertionError as caught_exception:
        print(f"exception: {caught_exception}")

    close_connection(con)
else:
    print("Connection error")

destroy_connection(con)
