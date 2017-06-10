/*
 *  iec61850_client.h
 *
 *  Copyright 2013, 2014, 2015 Michael Zillgith
 *
 *  This file is part of libIEC61850.
 *
 *  libIEC61850 is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  libIEC61850 is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with libIEC61850.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  See COPYING file for the complete license text.
 */

#ifndef IEC61850_CLIENT_H_
#define IEC61850_CLIENT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "libiec61850_common_api.h"
#include "iec61850_common.h"
#include "mms_value.h"
#include "mms_client_connection.h"
#include "linked_list.h"

/**
 *  * \defgroup iec61850_client_api_group IEC 61850/MMS client API
 */
/**@{*/

/** an opaque handle to the instance data of a ClientDataSet object */
typedef struct sClientDataSet* ClientDataSet;

/** an opaque handle to the instance data of a ClientReport object */
typedef struct sClientReport* ClientReport;

/** an opaque handle to the instance data of a ClientReportControlBlock object */
typedef struct sClientReportControlBlock* ClientReportControlBlock;

/** an opaque handle to the instance data of a ClientGooseControlBlock object */
typedef struct sClientGooseControlBlock* ClientGooseControlBlock;

/**
 * @defgroup IEC61850_CLIENT_GENERAL General client side connection handling functions and data types
 *
 * @{
 */

/** An opaque handle to the instance data of the IedConnection object */
typedef struct sIedConnection* IedConnection;

/** Detailed description of the last application error of the client connection instance */
typedef struct
{
    int ctlNum;
    int error;
    ControlAddCause addCause;
} LastApplError;

/** Connection state of the IedConnection instance (either idle, connected or closed) */
typedef enum
{
    IED_STATE_IDLE,
    IED_STATE_CONNECTED,
    IED_STATE_CLOSED
} IedConnectionState;

/** used to describe the error reason for most client side service functions */
typedef enum {
    /* general errors */

    /** No error occurred - service request has been successful */
    IED_ERROR_OK = 0,

    /** The service request can not be executed because the client is not yet connected */
    IED_ERROR_NOT_CONNECTED = 1,

    /** Connect service not execute because the client is already connected */
    IED_ERROR_ALREADY_CONNECTED = 2,

    /** The service request can not be executed caused by a loss of connection */
    IED_ERROR_CONNECTION_LOST = 3,

    /** The service or some given parameters are not supported by the client stack or by the server */
    IED_ERROR_SERVICE_NOT_SUPPORTED = 4,

    /** Connection rejected by server */
    IED_ERROR_CONNECTION_REJECTED = 5,

    /* client side errors */

    /** API function has been called with an invalid argument */
    IED_ERROR_USER_PROVIDED_INVALID_ARGUMENT = 10,

    IED_ERROR_ENABLE_REPORT_FAILED_DATASET_MISMATCH = 11,

    /** The object provided object reference is invalid (there is a syntactical error). */
    IED_ERROR_OBJECT_REFERENCE_INVALID = 12,

    /** Received object is of unexpected type */
    IED_ERROR_UNEXPECTED_VALUE_RECEIVED = 13,

    /* service error - error reported by server */

    /** The communication to the server failed with a timeout */
    IED_ERROR_TIMEOUT = 20,

    /** The server rejected the access to the requested object/service due to access control */
    IED_ERROR_ACCESS_DENIED = 21,

    /** The server reported that the requested object does not exist (returned by server) */
    IED_ERROR_OBJECT_DOES_NOT_EXIST = 22,

    /** The server reported that the requested object already exists */
    IED_ERROR_OBJECT_EXISTS = 23,

    /** The server does not support the requested access method (returned by server) */
    IED_ERROR_OBJECT_ACCESS_UNSUPPORTED = 24,

    /** The server expected an object of another type (returned by server) */
    IED_ERROR_TYPE_INCONSISTENT = 25,

    /** The object or service is temporarily unavailable (returned by server) */
    IED_ERROR_TEMPORARILY_UNAVAILABLE = 26,

    /** The specified object is not defined in the server (returned by server) */
    IED_ERROR_OBJECT_UNDEFINED = 27,

    /** The specified address is invalid (returned by server) */
    IED_ERROR_INVALID_ADDRESS = 28,

    /** Service failed due to a hardware fault (returned by server) */
    IED_ERROR_HARDWARE_FAULT = 29,

    /** The requested data type is not supported by the server (returned by server) */
    IED_ERROR_TYPE_UNSUPPORTED = 30,

    /** The provided attributes are inconsistent (returned by server) */
    IED_ERROR_OBJECT_ATTRIBUTE_INCONSISTENT = 31,

    /** The provided object value is invalid (returned by server) */
    IED_ERROR_OBJECT_VALUE_INVALID = 32,

    /** The object is invalidated (returned by server) */
    IED_ERROR_OBJECT_INVALIDATED = 33,

    /** Service not implemented */
    IED_ERROR_SERVICE_NOT_IMPLEMENTED = 98,

    /** unknown error */
    IED_ERROR_UNKNOWN = 99
} IedClientError;

/**************************************************
 * Connection creation and destruction
 **************************************************/

/**
 * \brief create a new IedConnection instance
 *
 * This function creates a new IedConnection instance that is used to handle a connection to an IED.
 * It allocated all required resources. The new connection is in the "idle" state. Before it can be used
 * the connect method has to be called.
 *
 * \return the new IedConnection instance
 */
IedConnection
IedConnection_create(void);

/**
 * \brief destroy an IedConnection instance.
 *
 * The connection will be closed if it is in "connected" state. All allocated resources of the connection
 * will be freed.
 *
 * \param self the connection object
 */
void
IedConnection_destroy(IedConnection self);


/**
 * \brief set the connect timeout in ms
 *
 * Set the connect timeout for this connection. This function has to be called before IedConnection_connect
 * is called.
 *
 * \param self the connection object
 * \param timoutInMs the connection timeout in ms
 */
void
IedConnection_setConnectTimeout(IedConnection self, uint32_t timeoutInMs);

/**************************************************
 * Association service
 **************************************************/

/**
 * \brief Connect to a server
 *
 * \param self the connection object
 * \param error the error code if an error occurs
 * \param hostname the host name or IP address of the server to connect to
 * \param tcpPort the TCP port number of the server to connect to
 */
void
IedConnection_connect(IedConnection self, IedClientError* error, const char* hostname, int tcpPort);

/**
 * \brief Abort the connection
 *
 * This will close the MMS association by sending an ACSE abort message to the server.
 * After sending the abort message the connection is closed immediately.
 * The client can assume the connection to be closed when the function returns and the
 * destroy method can be called. If the connection is not in "connected" state an
 * IED_ERROR_NOT_CONNECTED error will be reported.
 *
 * \param self the connection object
 * \param error the error code if an error occurs
 */
void
IedConnection_abort(IedConnection self, IedClientError* error);

/**
 * \brief Release the connection
 *
 * This will release the MMS association by sending an MMS conclude message to the server.
 * The client can NOT assume the connection to be closed when the function returns, It can
 * also fail if the server returns with a negative response. To be sure that the connection
 * will be close the close or abort methods should be used. If the connection is not in "connected" state an
 * IED_ERROR_NOT_CONNECTED error will be reported.
 *
 * \param self the connection object
 * \param error the error code if an error occurs
 */
void
IedConnection_release(IedConnection self, IedClientError* error);

/**
 * \brief Close the connection
 *
 * This will close the MMS association and the underlying TCP connection.
 *
 * \param self the connection object
 */
void
IedConnection_close(IedConnection self);

/**
 * \brief return the state of the connection.
 *
 * This function can be used to determine if the connection is established or closed.
 *
 * \param self the connection object
 *
 * \return the connection state
 */
IedConnectionState
IedConnection_getState(IedConnection self);

/**
 * \brief Access to last application error received by the client connection
 *
 * \param self the connection object
 *
 * \return the LastApplError value
 */
LastApplError
IedConnection_getLastApplError(IedConnection self);


typedef void (*IedConnectionClosedHandler) (void* parameter, IedConnection connection);

/**
 * \brief Install a handler function that will be called when the connection is lost.
 *
 * \param self the connection object
 * \param handler that callback function
 * \param parameter the user provided parameter that is handed over to the callback function
 */
void
IedConnection_installConnectionClosedHandler(IedConnection self, IedConnectionClosedHandler handler,
        void* parameter);

/**
 * \brief get a handle to the underlying MmsConnection
 *
 * Get access to the underlying MmsConnection instance used by this IedConnection.
 * This can be used to set/change specific MmsConnection parameters or to call low-level MMS services/functions.
 *
 * \param self the connection object
 *
 * \return the MmsConnection instance used by this IedConnection.
 */
MmsConnection
IedConnection_getMmsConnection(IedConnection self);

/** @} */

/**
 * @defgroup IEC61850_CLIENT_SV Client side SV control block handling functions
 *
 * @{
 */

/** SV ASDU contains attribute RefrTm */
#define IEC61850_SV_OPT_REFRESH_TIME 1

/** SV ASDU contains attribute SmpSynch */
#define IEC61850_SV_OPT_SAMPLE_SYNC 2

/** SV ASDU contains attribute SmpRate */
#define IEC61850_SV_OPT_SAMPLE_RATE 4

/** SV ASDU contains attribute DatSet */
#define IEC61850_SV_OPT_DATA_SET 8

/** SV ASDU contains attribute Security */
#define IEC61850_SV_OPT_SECURITY 16


/** an opaque handle to the instance data of a ClientSVControlBlock object */
typedef struct sClientSVControlBlock* ClientSVControlBlock;

/**
 * \brief Create a new ClientSVControlBlock instance
 *
 *  This function simplifies client side access to server MSV/USV control blocks
 *  NOTE: Do not use the functions after the IedConnection object is invalidated!
 *
 * \param connection the IedConnection object with a valid connection to the server.
 * \param reference the object reference of the control block
 *
 * \return the new instance
 */
ClientSVControlBlock
ClientSVControlBlock_create(IedConnection connection, const char* reference);

/**
 * \brief Free all resources related to the ClientSVControlBlock instance.
 *
 * \param self the ClientSVControlBlock instance to operate on
 */
void
ClientSVControlBlock_destroy(ClientSVControlBlock self);

bool
ClientSVControlBlock_isMulticast(ClientSVControlBlock self);

/**
 * \brief Return the error code of the last write or write acccess to the SVCB
 *
 * \param self the ClientSVControlBlock instance to operate on
 *
 * \return the error code of the last read or write access
 */
IedClientError
ClientSVControlBlock_getLastComError(ClientSVControlBlock self);


bool
ClientSVControlBlock_setSvEna(ClientSVControlBlock self, bool svEna);

bool
ClientSVControlBlock_getSvEna(ClientSVControlBlock self);

bool
ClientSVControlBlock_setResv(ClientSVControlBlock self, bool svEna);

bool
ClientSVControlBlock_getResv(ClientSVControlBlock self);

char*
ClientSVControlBlock_getMsvID(ClientSVControlBlock self);

/**
 * \brief Get the (MMS) reference to the data set
 *
 * NOTE: the returned string is dynamically allocated with the
 * GLOBAL_MALLOC macro. The application is responsible to release
 * the memory when the string is no longer needed.
 *
 * \param self the ClientSVControlBlock instance to operate on
 *
 * \return the data set reference as a NULL terminated string
 */
char*
ClientSVControlBlock_getDatSet(ClientSVControlBlock self);

uint32_t
ClientSVControlBlock_getConfRev(ClientSVControlBlock self);

uint16_t
ClientSVControlBlock_getSmpRate(ClientSVControlBlock self);


/**
 * \brief returns the destination address of the SV publisher
 *
 * \param self the ClientSVControlBlock instance to operate on
 */
PhyComAddress
ClientSVControlBlock_getDstAddress(ClientSVControlBlock self);

/**
 * \brief returns the OptFlds bit string as integer
 *
 * \param self the ClientSVControlBlock instance to operate on
 */
int
ClientSVControlBlock_getOptFlds(ClientSVControlBlock self);

/**
 * \brief returns number of sample mode of the SV publisher
 *
 * \param self the ClientSVControlBlock instance to operate on
 */
uint8_t
ClientSVControlBlock_getSmpMod(ClientSVControlBlock self);

/**
 * \brief returns number of ASDUs included in the SV message
 *
 * \param self the ClientSVControlBlock instance to operate on
 */
int
ClientSVControlBlock_getNoASDU(ClientSVControlBlock self);


/** @} */

/**
 * @defgroup IEC61850_CLIENT_GOOSE Client side GOOSE control block handling functions
 *
 * @{
 */

/*********************************************************
 * GOOSE services handling (MMS part)
 ********************************************************/

/** Enable GOOSE publisher GoCB block element */
#define GOCB_ELEMENT_GO_ENA       1

/** GOOSE ID GoCB block element */
#define GOCB_ELEMENT_GO_ID        2

/** Data set GoCB block element */
#define GOCB_ELEMENT_DATSET       4

/** Configuration revision GoCB block element (this is usually read-only) */
#define GOCB_ELEMENT_CONF_REV     8

/** Need commission GoCB block element (read-only according to 61850-7-2) */
#define GOCB_ELEMENT_NDS_COMM    16

/** Destination address GoCB block element (read-only according to 61850-7-2) */
#define GOCB_ELEMENT_DST_ADDRESS 32

/** Minimum time GoCB block element (read-only according to 61850-7-2) */
#define GOCB_ELEMENT_MIN_TIME    64

/** Maximum time GoCB block element (read-only according to 61850-7-2) */
#define GOCB_ELEMENT_MAX_TIME   128

/** Fixed offsets GoCB block element (read-only according to 61850-7-2) */
#define GOCB_ELEMENT_FIXED_OFFS 256

/** select all elements of the GoCB */
#define GOCB_ELEMENT_ALL        511


/**************************************************
 * ClientGooseControlBlock class
 **************************************************/

ClientGooseControlBlock
ClientGooseControlBlock_create(const char* dataAttributeReference);

void
ClientGooseControlBlock_destroy(ClientGooseControlBlock self);

bool
ClientGooseControlBlock_getGoEna(ClientGooseControlBlock self);

void
ClientGooseControlBlock_setGoEna(ClientGooseControlBlock self, bool goEna);

const char*
ClientGooseControlBlock_getGoID(ClientGooseControlBlock self);

void
ClientGooseControlBlock_setGoID(ClientGooseControlBlock self, const char* goID);

const char*
ClientGooseControlBlock_getDatSet(ClientGooseControlBlock self);

void
ClientGooseControlBlock_setDatSet(ClientGooseControlBlock self, const char* datSet);

uint32_t
ClientGooseControlBlock_getConfRev(ClientGooseControlBlock self);

bool
ClientGooseControlBlock_getNdsComm(ClientGooseControlBlock self);

uint32_t
ClientGooseControlBlock_getMinTime(ClientGooseControlBlock self);

uint32_t
ClientGooseControlBlock_getMaxTime(ClientGooseControlBlock self);

bool
ClientGooseControlBlock_getFixedOffs(ClientGooseControlBlock self);

MmsValue* /* MMS_OCTET_STRING */
ClientGooseControlBlock_getDstAddress_addr(ClientGooseControlBlock self);

void
ClientGooseControlBlock_setDstAddress_addr(ClientGooseControlBlock self, MmsValue* macAddr);

uint8_t
ClientGooseControlBlock_getDstAddress_priority(ClientGooseControlBlock self);

void
ClientGooseControlBlock_setDstAddress_priority(ClientGooseControlBlock self, uint8_t priorityValue);

uint16_t
ClientGooseControlBlock_getDstAddress_vid(ClientGooseControlBlock self);

void
ClientGooseControlBlock_setDstAddress_vid(ClientGooseControlBlock self, uint16_t vidValue);

uint16_t
ClientGooseControlBlock_getDstAddress_appid(ClientGooseControlBlock self);

void
ClientGooseControlBlock_setDstAddress_appid(ClientGooseControlBlock self, uint16_t appidValue);


/*********************************************************
 * GOOSE services (access to GOOSE Control Blocks (GoCB))
 ********************************************************/

/**
 * \brief Read access to attributes of a GOOSE control block (GoCB) at the connected server. A GoCB contains
 * the configuration values for a single GOOSE publisher.
 *
 * The requested GoCB has to be specified by its object IEC 61850 ACSI object reference. E.g.
 *
 * "simpleIOGernericIO/LLN0.gcbEvents"
 *
 * This function is used to perform the actual read service for the GoCB values.
 * To access the received values the functions of ClientGooseControlBlock have to be used.
 *
 * If called with a NULL argument for the updateGoCB parameter a new ClientGooseControlBlock instance is created
 * and populated with the values received by the server. It is up to the user to release this object by
 * calling the ClientGooseControlBlock_destroy function when the object is no longer needed. If called with a reference
 * to an existing ClientGooseControlBlock instance the values of the attributes will be updated and no new instance
 * will be created.
 *
 * Note: This function maps to a single MMS read request to retrieve the complete GoCB at once.
 *
 * \param connection the connection object
 * \param error the error code if an error occurs
 * \param goCBReference IEC 61850-7-2 ACSI object reference of the GOOSE control block
 * \param updateRcb a reference to an existing ClientGooseControlBlock instance or NULL
 *
 * \return new ClientGooseControlBlock instance or the instance provided by the user with
 *         the updateRcb parameter.
 */
ClientGooseControlBlock
IedConnection_getGoCBValues(IedConnection self, IedClientError* error, const char* goCBReference, ClientGooseControlBlock updateGoCB);

/**
 * \brief Write access to attributes of a GOOSE control block (GoCB) at the connected server
 *
 * The GoCB and the values to be written are specified with the goCB parameter.
 *
 * The parametersMask parameter specifies which attributes of the remote GoCB have to be set by this request.
 * You can specify multiple attributes by ORing the defined bit values. If all attributes have to be written
 * GOCB_ELEMENT_ALL can be used.
 *
 * The singleRequest parameter specifies the mapping to the corresponding MMS write request. Standard compliant
 * servers should accept both variants. But some server accept only one variant. Then the value of this parameter
 * will be of relevance.
 *
 * \param connection the connection object
 * \param error the error code if an error occurs
 * \param goCB ClientGooseControlBlock instance that actually holds the parameter
 *            values to be written.
 * \param parametersMask specifies the parameters contained in the setGoCBValues request.
 * \param singleRequest specifies if the seGoCBValues services is mapped to a single MMS write request containing
 *        multiple variables or to multiple MMS write requests.
 */
void
IedConnection_setGoCBValues(IedConnection self, IedClientError* error, ClientGooseControlBlock goCB,
        uint32_t parametersMask, bool singleRequest);

/** @} */

/********************************************
 * Reporting services
 ********************************************/

/**
 * @defgroup IEC61850_CLIENT_REPORTS Client side report handling services, functions, and data types
 *
 * @{
 */

/**
 * \brief Read access to attributes of a report control block (RCB) at the connected server
 *
 * The requested RCB has to be specified by its object reference. E.g.
 *
 * "simpleIOGernericIO/LLN0.RP.EventsRCB01"
 *
 * or
 *
 * "simpleIOGenericIO/LLN0.BR.EventsBRCB01"
 *
 * Report control blocks have either "RP" or "BR" as part of their name following the logical node part.
 * "RP" is part of the name of unbuffered RCBs whilst "BR" is part of the name of buffered RCBs.
 *
 * This function is used to perform the actual read service. To access the received values the functions
 * of ClientReportControlBlock have to be used.
 *
 * If called with a NULL argument for the updateRcb parameter a new ClientReportControlBlock instance is created
 * and populated with the values received by the server. It is up to the user to release this object by
 * calling the ClientReportControlBlock_destroy function when the object is no longer needed. If called with a reference
 * to an existing ClientReportControlBlock instance the values of the attributes will be updated and no new instance
 * will be created.
 *
 * Note: This function maps to a single MMS read request to retrieve the complete RCB at once.
 *
 * \param connection the connection object
 * \param error the error code if an error occurs
 * \param rcbReference object reference of the report control block
 * \param updateRcb a reference to an existing ClientReportControlBlock instance or NULL
 *
 * \return new ClientReportControlBlock instance or the instance provided by the user with
 *         the updateRcb parameter.
 */
ClientReportControlBlock
IedConnection_getRCBValues(IedConnection self, IedClientError* error, const char* rcbReference,
        ClientReportControlBlock updateRcb);

/** Describes the reason for the inclusion of the element in the report */
typedef enum {
    /** the element is not included in the received report */
    IEC61850_REASON_NOT_INCLUDED = 0,

    /** the element is included due to a change of the data value */
    IEC61850_REASON_DATA_CHANGE = 1,

    /** the element is included due to a change in the quality of data */
    IEC61850_REASON_QUALITY_CHANGE = 2,

    /** the element is included due to an update of the data value */
    IEC61850_REASON_DATA_UPDATE = 3,

    /** the element is included due to a periodic integrity report task */
    IEC61850_REASON_INTEGRITY = 4,

    /** the element is included due to a general interrogation by the client */
    IEC61850_REASON_GI = 5,

    /** the reason for inclusion is unknown */
    IEC61850_REASON_UNKNOWN = 6
} ReasonForInclusion;

#define REASON_NOT_INCLUDED IEC61850_REASON_NOT_INCLUDED
#define REASON_DATA_CHANGE IEC61850_REASON_DATA_CHANGE
#define REASON_QUALITY_CHANGE IEC61850_REASON_QUALITY_CHANGE
#define REASON_DATA_UPDATE IEC61850_REASON_DATA_UPDATE
#define REASON_INTEGRITY IEC61850_REASON_INTEGRITY
#define REASON_GI IEC61850_REASON_GI
#define REASON_UNKNOWN IEC61850_REASON_UNKNOWN


/* Element encoding mask values for ClientReportControlBlock */

/** include the report ID into the setRCB request */
#define RCB_ELEMENT_RPT_ID            1

/** include the report enable element into the setRCB request */
#define RCB_ELEMENT_RPT_ENA           2

/** include the reservation element into the setRCB request (only available in unbuffered RCBs!) */
#define RCB_ELEMENT_RESV              4

/** include the data set element into the setRCB request */
#define RCB_ELEMENT_DATSET            8

/** include the configuration revision element into the setRCB request */
#define RCB_ELEMENT_CONF_REV         16

/** include the option fields element into the setRCB request */
#define RCB_ELEMENT_OPT_FLDS         32

/** include the bufTm (event buffering time) element into the setRCB request */
#define RCB_ELEMENT_BUF_TM           64

/** include the sequence number element into the setRCB request (should be used!) */
#define RCB_ELEMENT_SQ_NUM          128

/** include the trigger options element into the setRCB request */
#define RCB_ELEMENT_TRG_OPS         256

/** include the integrity period element into the setRCB request */
#define RCB_ELEMENT_INTG_PD         512

/** include the GI (general interrogation) element into the setRCB request */
#define RCB_ELEMENT_GI             1024

/** include the purge buffer element into the setRCB request (only available in buffered RCBs) */
#define RCB_ELEMENT_PURGE_BUF      2048

/** include the entry ID element into the setRCB request (only available in buffered RCBs) */
#define RCB_ELEMENT_ENTRY_ID       4096

/** include the time of entry element into the setRCB request (only available in buffered RCBs) */
#define RCB_ELEMENT_TIME_OF_ENTRY  8192

/** include the reservation time element into the setRCB request (only available in buffered RCBs) */
#define RCB_ELEMENT_RESV_TMS      16384

/** include the owner element into the setRCB request */
#define RCB_ELEMENT_OWNER         32768

/**
 * \brief Write access to attributes of a report control block (RCB) at the connected server
 *
 * The requested RCB has to be specified by its object reference (see also IedConnection_getRCBValues).
 * The object reference for the referenced RCB is contained in the provided ClientReportControlBlock instance.
 *
 * The parametersMask parameter specifies which attributes of the remote RCB have to be set by this request.
 * You can specify multiple attributes by ORing the defined bit values.
 *
 * The singleRequest parameter specifies the mapping to the corresponding MMS write request. Standard compliant
 * servers should accept both variants. But some server accept only one variant. Then the value of this parameter
 * will be of relevance.
 *
 * \param connection the connection object
 * \param error the error code if an error occurs
 * \param rcb object reference of the ClientReportControlBlock instance that actually holds the parameter
 *            values to be written.
 * \param parametersMask specifies the parameters contained in the setRCBValues request.
 * \param singleRequest specifies if the setRCBValues services is mapped to a single MMS write request containing
 *        multiple variables or to multiple MMS write requests.
 */
void
IedConnection_setRCBValues(IedConnection self, IedClientError* error, ClientReportControlBlock rcb,
        uint32_t parametersMask, bool singleRequest);

/**
 * \brief Callback function for receiving reports
 *
 * \param parameter a user provided parameter that is handed to the callback function
 * \param report a ClientReport instance that holds the informations contained in the received report
 */
typedef void (*ReportCallbackFunction) (void* parameter, ClientReport report);

/**
 * \brief Install a report handler function for the specified report control block (RCB)
 *
 * This function will replace a report handler set earlier for the specified RCB. The report handler
 * will be called whenever a report for the specified RCB is received.
 * Please note that this function should be called whenever the RCB data set is changed or updated.
 * Otherwise the internal data structures storing the received data set values will not be updated
 * correctly.
 *
 * When replacing a report handler you only have to call this function. There is no separate call to
 * IedConnection_uninstallReportHandler() required.
 *
 * \param self the connection object
 * \param rcbReference object reference of the report control block
 * \param rptId a string that identifies the report. If the rptId is not available then the
 *        rcbReference is used to identify the report.
 * \param handler user provided callback function to be invoked when a report is received.
 * \param handlerParameter user provided parameter that will be passed to the callback function
 */
void
IedConnection_installReportHandler(IedConnection self, const char* rcbReference, const char* rptId, ReportCallbackFunction handler,
        void* handlerParameter);

/**
 * \brief uninstall a report handler function for the specified report control block (RCB)
 *
 * \param self the connection object
 * \param rcbReference object reference of the report control block
 */
void
IedConnection_uninstallReportHandler(IedConnection self, const char* rcbReference);

/**
 * \brief Trigger a general interrogation (GI) report for the specified report control block (RCB)
 *
 * The RCB must have been enabled and GI set as trigger option before this command can be performed.
 *
 * \deprecated Use ClientReportControlBlock_setGI instead
 *
 * \param connection the connection object
 * \param error the error code if an error occurs
 * \param rcbReference object reference of the report control block
 */
void
IedConnection_triggerGIReport(IedConnection self, IedClientError* error, const char* rcbReference);

/****************************************
 * Access to received reports
 ****************************************/

/**
 * \brief Get the name of the report data set
 *
 * NOTE: the returned string is only valid as long as the ClientReport instance exists!
 *
 * \param self the ClientReport instance
 * \return report data set name as 0 terminated string
 */
const char*
ClientReport_getDataSetName(ClientReport self);

/**
 * \brief return the received data set values of the report
 *
 * \param self the ClientReport instance
 * \return an MmsValue array instance containing the data set values
 */
MmsValue*
ClientReport_getDataSetValues(ClientReport self);

/**
 * \brief return reference (name) of the server RCB associated with this ClientReport object
 *
 * \param self the ClientReport instance
 * \return report control block reference as string
 */
char*
ClientReport_getRcbReference(ClientReport self);

/**
 * \brief return RptId of the server RCB associated with this ClientReport object
 *
 * \param self the ClientReport instance
 * \return report control block reference as string
 */
char*
ClientReport_getRptId(ClientReport self);

/**
 * \brief get the reason code (reason for inclusion) for a specific report data set element
 *
 * \param self the ClientReport instance
 * \param elementIndex index of the data set element (starting with 0)
 *
 * \return reason code for the inclusion of the specified element
 */
ReasonForInclusion
ClientReport_getReasonForInclusion(ClientReport self, int elementIndex);

/**
 * \brief get the entry ID of the report
 *
 * Returns the entryId of the report if included in the report. Otherwise returns NULL.
 *
 * \param self the ClientReport instance
 *
 * \return entryId or NULL
 */
MmsValue*
ClientReport_getEntryId(ClientReport self);

/**
 * \brief determine if the last received report contains a timestamp
 *
 * \param self the ClientReport instance
 *
 * \return true if the report contains a timestamp, false otherwise
 */
bool
ClientReport_hasTimestamp(ClientReport self);

bool
ClientReport_hasSeqNum(ClientReport self);

uint16_t
ClientReport_getSeqNum(ClientReport self);

bool
ClientReport_hasDataSetName(ClientReport self);

bool
ClientReport_hasReasonForInclusion(ClientReport self);

bool
ClientReport_hasConfRev(ClientReport self);

uint32_t
ClientReport_getConfRev(ClientReport self);

/**
 * \brief Indicates if the report contains the bufOvfl (buffer overflow) flag
 *
 * \param self the ClientReport instance
 *
 * \returns true if the report contains the bufOvfl flag, false otherwise
 */
bool
ClientReport_hasBufOvfl(ClientReport self);

/**
 * \brief Get the value of the bufOvfl flag
 *
 * \param self the ClientReport instance
 *
 * \returns true if bufOvfl is set, false otherwise
 */
bool
ClientReport_getBufOvfl(ClientReport self);

/**
 * \brief Indicates if the report contains data references for the reported data set members
 *
 * \param self the ClientReport instance
 *
 * \returns true if the report contains data-references, false otherwise
 */
bool
ClientReport_hasDataReference(ClientReport self);

/**
 * \brief get the data-reference of the element of the report data set
 *
 * This function will only return a non-NULL value if the received report contains data-references.
 * This can be determined by the ClientReport_hasDataReference function.
 * NOTE: The returned string is allocated and hold by the ClientReport instance and is only valid until
 * the ClientReport instance exists!
 *
 * \param self the ClientReport instance
 * \param elementIndex  index of the data set element (starting with 0)
 *
 * \param the data reference as string as provided by the report or NULL if the data reference is not available
 */
const char*
ClientReport_getDataReference(ClientReport self, int elementIndex);


/**
 * \brief get the timestamp of the report
 *
 * Returns the timestamp of the report if included in the report. Otherwise the value is undefined.
 * Use the ClientReport_hasTimestamp function first to figure out if the timestamp is valid
 *
 * \param self the ClientReport instance
 *
 * \return the timestamp as milliseconds since 1.1.1970 UTC
 */
uint64_t
ClientReport_getTimestamp(ClientReport self);

/**
 * \brief get the reason for inclusion of as a human readable string
 *
 * \param reasonCode
 *
 * \return the reason for inclusion as static human readable string
 */
char*
ReasonForInclusion_getValueAsString(ReasonForInclusion reasonCode);

/**************************************************
 * ClientReportControlBlock access class
 **************************************************/

ClientReportControlBlock
ClientReportControlBlock_create(const char* rcbReference);

void
ClientReportControlBlock_destroy(ClientReportControlBlock self);

char*
ClientReportControlBlock_getObjectReference(ClientReportControlBlock self);

bool
ClientReportControlBlock_isBuffered(ClientReportControlBlock self);

const char*
ClientReportControlBlock_getRptId(ClientReportControlBlock self);

void
ClientReportControlBlock_setRptId(ClientReportControlBlock self, const char* rptId);

bool
ClientReportControlBlock_getRptEna(ClientReportControlBlock self);

void
ClientReportControlBlock_setRptEna(ClientReportControlBlock self, bool rptEna);

bool
ClientReportControlBlock_getResv(ClientReportControlBlock self);

void
ClientReportControlBlock_setResv(ClientReportControlBlock self, bool resv);

const char*
ClientReportControlBlock_getDataSetReference(ClientReportControlBlock self);

/**
 * \brief set the data set to be observed by the RCB
 *
 * The data set reference is a mixture of MMS and IEC 61850 syntax! In general the reference has
 * the form:
 * LDName/LNName$DataSetName
 *
 * e.g. "simpleIOGenericIO/LLN0$Events"
 *
 * It is standard that data sets are defined in LN0 logical nodes. But this is not mandatory.
 *
 * Note: As a result of changing the data set the server will increase the confRev attribute of the RCB.
 *
 * \param self the RCB instance
 * \param dataSetReference the reference of the data set
 */
void
ClientReportControlBlock_setDataSetReference(ClientReportControlBlock self, const char* dataSetReference);

uint32_t
ClientReportControlBlock_getConfRev(ClientReportControlBlock self);

int
ClientReportControlBlock_getOptFlds(ClientReportControlBlock self);

void
ClientReportControlBlock_setOptFlds(ClientReportControlBlock self, int optFlds);

uint32_t
ClientReportControlBlock_getBufTm(ClientReportControlBlock self);

void
ClientReportControlBlock_setBufTm(ClientReportControlBlock self, uint32_t bufTm);

uint16_t
ClientReportControlBlock_getSqNum(ClientReportControlBlock self);

int
ClientReportControlBlock_getTrgOps(ClientReportControlBlock self);

void
ClientReportControlBlock_setTrgOps(ClientReportControlBlock self, int trgOps);

uint32_t
ClientReportControlBlock_getIntgPd(ClientReportControlBlock self);

void
ClientReportControlBlock_setIntgPd(ClientReportControlBlock self, uint32_t intgPd);

bool
ClientReportControlBlock_getGI(ClientReportControlBlock self);

void
ClientReportControlBlock_setGI(ClientReportControlBlock self, bool gi);

bool
ClientReportControlBlock_getPurgeBuf(ClientReportControlBlock self);

void
ClientReportControlBlock_setPurgeBuf(ClientReportControlBlock self, bool purgeBuf);

int16_t
ClientReportControlBlock_getResvTms(ClientReportControlBlock self);

void
ClientReportControlBlock_setResvTms(ClientReportControlBlock self, int16_t resvTms);

MmsValue* /* <MMS_OCTET_STRING> */
ClientReportControlBlock_getEntryId(ClientReportControlBlock self);

void
ClientReportControlBlock_setEntryId(ClientReportControlBlock self, MmsValue* entryId);

uint64_t
ClientReportControlBlock_getEntryTime(ClientReportControlBlock self);

MmsValue* /* <MMS_OCTET_STRING> */
ClientReportControlBlock_getOwner(ClientReportControlBlock self);


/** @} */

/****************************************
 * Data model access services
 ****************************************/

/**
 * @defgroup IEC61850_CLIENT_DATA_ACCESS Client side data access (read/write) service functions
 *
 * @{
 */

/**
 * \brief read a functional constrained data attribute (FCDA) or functional constrained data (FCD).
 *
 * \param self  the connection object to operate on
 * \param error the error code if an error occurs
 * \param object reference of the object/attribute to read
 * \param fc the functional constraint of the data attribute or data object to read
 *
 * \return the MmsValue instance of the received value or NULL if the request failed
 */
MmsValue*
IedConnection_readObject(IedConnection self, IedClientError* error, const char* dataAttributeReference, FunctionalConstraint fc);

/**
 * \brief write a functional constrained data attribute (FCDA) or functional constrained data (FCD).
 *
 * \param self  the connection object to operate on
 * \param error the error code if an error occurs
 * \param object reference of the object/attribute to write
 * \param fc the functional constraint of the data attribute or data object to write
 * \param value the MmsValue to write (has to be of the correct type - MMS_STRUCTURE for FCD)
 */
void
IedConnection_writeObject(IedConnection self, IedClientError* error, const char* dataAttributeReference, FunctionalConstraint fc,
        MmsValue* value);


/**
 * \brief read a functional constrained data attribute (FCDA) of type boolean
 *
 * \param self  the connection object to operate on
 * \param error the error code if an error occurs
 * \param object reference of the data attribute to read
 * \param fc the functional constraint of the data attribute to read
 */
bool
IedConnection_readBooleanValue(IedConnection self, IedClientError* error, const char* objectReference, FunctionalConstraint fc);

/**
 * \brief read a functional constrained data attribute (FCDA) of type float
 *
 * \param self  the connection object to operate on
 * \param error the error code if an error occurs
 * \param object reference of the data attribute to read
 * \param fc the functional constraint of the data attribute to read
 */
float
IedConnection_readFloatValue(IedConnection self, IedClientError* error, const char* objectReference, FunctionalConstraint fc);

/**
 * \brief read a functional constrained data attribute (FCDA) of type VisibleString or MmsString
 *
 * NOTE: the returned char buffer is dynamically allocated and has to be freed by the caller!
 *
 * \param self  the connection object to operate on
 * \param error the error code if an error occurs
 * \param object reference of the data attribute to read
 * \param fc the functional constraint of the data attribute to read
 *
 * \return a C string representation of the value. Has to be freed by the caller!
 */
char*
IedConnection_readStringValue(IedConnection self, IedClientError* error, const char* objectReference, FunctionalConstraint fc);

/**
 * \brief read a functional constrained data attribute (FCDA) of type Integer or Unsigned and return the result as int32_t
 *
 * \param self  the connection object to operate on
 * \param error the error code if an error occurs
 * \param object reference of the data attribute to read
 * \param fc the functional constraint of the data attribute to read
 *
 * \return an int32_t value of the read data attributes
 */
int32_t
IedConnection_readInt32Value(IedConnection self, IedClientError* error, const char* objectReference, FunctionalConstraint fc);

/**
 * \brief read a functional constrained data attribute (FCDA) of type Integer or Unsigned and return the result as int64_t
 *
 * \param self  the connection object to operate on
 * \param error the error code if an error occurs
 * \param object reference of the data attribute to read
 * \param fc the functional constraint of the data attribute to read
 *
 * \return an int64_t value of the read data attributes
 */
int64_t
IedConnection_readInt64Value(IedConnection self, IedClientError* error, const char* objectReference, FunctionalConstraint fc);

/**
 * \brief read a functional constrained data attribute (FCDA) of type Integer or Unsigned and return the result as uint32_t
 *
 * \param self  the connection object to operate on
 * \param error the error code if an error occurs
 * \param object reference of the data attribute to read
 * \param fc the functional constraint of the data attribute to read
 *
 * \return an uint32_t value of the read data attributes
 */
uint32_t
IedConnection_readUnsigned32Value(IedConnection self, IedClientError* error, const char* objectReference, FunctionalConstraint fc);

/**
 * \brief read a functional constrained data attribute (FCDA) of type Timestamp (UTC Time)
 *
 *  NOTE: If the timestamp parameter is set to NULL the function allocates a new timestamp instance. Otherwise the
 *  return value is a pointer to the user provided timestamp instance.
 *
 * \param self  the connection object to operate on
 * \param error the error code if an error occurs
 * \param object reference of the data attribute to read
 * \param fc the functional constraint of the data attribute to read
 * \param timestamp a pointer to a user provided timestamp instance or NULL
 *
 * \return the timestamp value
 */
Timestamp*
IedConnection_readTimestampValue(IedConnection self, IedClientError* error, const char* objectReference, FunctionalConstraint fc,
        Timestamp* timeStamp);

/**
 * \brief read a functional constrained data attribute (FCDA) of type Quality
 *
 * \param self  the connection object to operate on
 * \param error the error code if an error occurs
 * \param object reference of the data attribute to read
 * \param fc the functional constraint of the data attribute to read
 *
 * \return the timestamp value
 */
Quality
IedConnection_readQualityValue(IedConnection self, IedClientError* error, const char* objectReference, FunctionalConstraint fc);

/**
 * \brief write a functional constrained data attribute (FCDA) of type boolean
 *
 * \param self  the connection object to operate on
 * \param error the error code if an error occurs
 * \param object reference of the data attribute to read
 * \param fc the functional constraint of the data attribute or data object to write
 * \param value the boolean value to write
 */
void
IedConnection_writeBooleanValue(IedConnection self, IedClientError* error, const char* objectReference,
        FunctionalConstraint fc, bool value);

/**
 * \brief write a functional constrained data attribute (FCDA) of type integer
 *
 * \param self  the connection object to operate on
 * \param error the error code if an error occurs
 * \param object reference of the data attribute to read
 * \param fc the functional constraint of the data attribute or data object to write
 * \param value the int32_t value to write
 */
void
IedConnection_writeInt32Value(IedConnection self, IedClientError* error, const char* objectReference,
        FunctionalConstraint fc, int32_t value);

/**
 * \brief write a functional constrained data attribute (FCDA) of type unsigned (integer)
 *
 * \param self  the connection object to operate on
 * \param error the error code if an error occurs
 * \param object reference of the data attribute to read
 * \param fc the functional constraint of the data attribute or data object to write
 * \param value the uint32_t value to write
 */
void
IedConnection_writeUnsigned32Value(IedConnection self, IedClientError* error, const char* objectReference,
        FunctionalConstraint fc, uint32_t value);

/**
 * \brief write a functional constrained data attribute (FCDA) of type float
 *
 * \param self  the connection object to operate on
 * \param error the error code if an error occurs
 * \param object reference of the data attribute to read
 * \param fc the functional constraint of the data attribute or data object to write
 * \param value the float value to write
 */
void
IedConnection_writeFloatValue(IedConnection self, IedClientError* error, const char* objectReference,
        FunctionalConstraint fc, float value);

void
IedConnection_writeVisibleStringValue(IedConnection self, IedClientError* error, const char* objectReference,
        FunctionalConstraint fc, char* value);

void
IedConnection_writeOctetString(IedConnection self, IedClientError* error, const char* objectReference,
        FunctionalConstraint fc, uint8_t* value, int valueLength);

/** @} */

/****************************************
 * Data set handling
 ****************************************/

/**
 * @defgroup IEC61850_CLIENT_DATA_SET Client side data set service functions and data types
 *
 * @{
 */

/**
 * \brief get data set values from a server device
 *
 * \param connection the connection object
 * \param error the error code if an error occurs
 * \param dataSetReference object reference of the data set
 * \param dataSet a data set instance where to store the retrieved values or NULL if a new instance
 *        shall be created.
 *
 * \return data set instance with retrieved values of NULL if an error occurred.
 */
ClientDataSet
IedConnection_readDataSetValues(IedConnection self, IedClientError* error, const char* dataSetReference, ClientDataSet dataSet);

/**
 * \brief create a new data set at the connected server device
 *
 * This function creates a new data set at the server. The parameter dataSetReference is the name of the new data set
 * to create. It is either in the form LDName/LNodeName.dataSetName for permanent domain or VMD scope data sets or
 * @dataSetName for an association specific data set. If the LDName part of the reference is missing the resulting
 * data set will be of VMD scope.
 *
 * The dataSetElements parameter contains a linked list containing the object references of FCDs or FCDAs. The format of
 * this object references is LDName/LNodeName.item(arrayIndex)component[FC].
 *
 * \param connection the connection object
 * \param error the error code if an error occurs
 * \param dataSetReference object reference of the data set
 * \param dataSetElements a list of object references defining the members of the new data set
 *
 */
void
IedConnection_createDataSet(IedConnection self, IedClientError* error, const char* dataSetReference, LinkedList /* char* */ dataSetElements);

/**
 * \brief delete a deletable data set at the connected server device
 *
 * This function deletes a data set at the server. The parameter dataSetReference is the name of the data set
 * to delete. It is either in the form LDName/LNodeName.dataSetName or @dataSetName for an association specific data set.
 *
 *
 * \param connection the connection object
 * \param error the error code if an error occurs
 * \param dataSetReference object reference of the data set
 *
 * \return true if data set has been deleted, false otherwise
 */
bool
IedConnection_deleteDataSet(IedConnection self, IedClientError* error, const char* dataSetReference);


/**
 * \brief returns the object references of the elements of a data set
 *
 * The return value contains a linked list containing the object references of FCDs or FCDAs. The format of
 * this object references is LDName/LNodeName.item(arrayIndex)component[FC].
 *
 * \param connection the connection object
 * \param error the error code if an error occurs
 * \param dataSetReference object reference of the data set
 * \param isDeletable this is an output parameter indicating that the requested data set is deletable by clients.
 *                    If this information is not required a NULL pointer can be used.
 *
 * \return LinkedList containing the data set elements as char* strings.
 */
LinkedList /* <char*> */
IedConnection_getDataSetDirectory(IedConnection self, IedClientError* error, const char* dataSetReference, bool* isDeletable);

/********************************************************
 * Data set object (local representation of a data set)
 *******************************************************/

/**
 * \brief destroy an ClientDataSet instance. Has to be called by the application.
 *
 * Note: A ClientDataSet cannot be created directly by the application but only by the IedConnection_readDataSetValues
 *       function. Therefore there is no public ClientDataSet_create function.
 *
 * \param self the ClientDataSet instance
 */
void
ClientDataSet_destroy(ClientDataSet self);

/**
 * \brief get the data set values locally stored in the ClientDataSet instance.
 *
 * This function returns a pointer to the locally stored MmsValue instance of this
 * ClientDataSet instance. The MmsValue instance is of type MMS_ARRAY and contains one
 * array element for each data set member.
 * Note: This call does not invoke any interaction with the associated server. It will
 * only provide access to already stored value. To update the values with the current values
 * of the server the IecConnection_readDataSetValues function has to be called!
 *
 * \param self the ClientDataSet instance
 *
 * \return the locally stored data set values as MmsValue object of type MMS_ARRAY.
 */
MmsValue*
ClientDataSet_getValues(ClientDataSet self);

/**
 * \brief Get the object reference of the data set.
 *
 * \param self the ClientDataSet instance
 *
 * \return the object reference of the data set.
 */
char*
ClientDataSet_getReference(ClientDataSet self);

/**
 * \brief get the size of the data set (number of members)
 *
 * \param self the ClientDataSet instance
 *
 * \return the number of member contained in the data set.
 */
int
ClientDataSet_getDataSetSize(ClientDataSet self);

/** @} */

/************************************
 *  Control service functions
 ************************************/

/**
 * @defgroup IEC61850_CLIENT_CONTROL Client side control service functions
 *
 * @{
 */

typedef struct sControlObjectClient* ControlObjectClient;

typedef enum {
	CONTROL_MODEL_STATUS_ONLY,
	CONTROL_MODEL_DIRECT_NORMAL,
	CONTROL_MODEL_SBO_NORMAL,
	CONTROL_MODEL_DIRECT_ENHANCED,
	CONTROL_MODEL_SBO_ENHANCED
} ControlModel;


/**
 * \brief Create a new client control object
 *
 * A client control object is used to handle all client side aspects of a controllable
 * data object.
 *
 * \param objectReference the reference of the controllable data object
 * \param connection the connection instance where the control object has to be reached
 *
 * \return the newly created instance or NULL if the creation failed
 */
ControlObjectClient
ControlObjectClient_create(const char* objectReference, IedConnection connection);

void
ControlObjectClient_destroy(ControlObjectClient self);

char*
ControlObjectClient_getObjectReference(ControlObjectClient self);

ControlModel
ControlObjectClient_getControlModel(ControlObjectClient self);

/**
 * \brief Send an operate command to the server
 *
 * \param self the control object instance to use
 * \param ctlVal the control value (for APC the value may be either AnalogueValue (MMS_STRUCT) or MMS_FLOAT/MMS_INTEGER
 * \param operTime the time when the command has to be executed (for time activated control). If this value is 0 the command will be executed instantly.
 *
 * \return true if operation has been successful, false otherwise.
 */
bool
ControlObjectClient_operate(ControlObjectClient self, MmsValue* ctlVal, uint64_t operTime);

bool
ControlObjectClient_select(ControlObjectClient self);

/**
 * \brief Send an select with value command to the server
 *
 * \param self the control object instance to use
 * \param ctlVal the control value (for APC the value may be either AnalogueValue (MMS_STRUCT) or MMS_FLOAT/MMS_INTEGER
 *
 * \return true if select has been successful, false otherwise.
 */
bool
ControlObjectClient_selectWithValue(ControlObjectClient self, MmsValue* ctlVal);

bool
ControlObjectClient_cancel(ControlObjectClient self);

void
ControlObjectClient_setLastApplError(ControlObjectClient self, LastApplError lastAppIError);

LastApplError
ControlObjectClient_getLastApplError(ControlObjectClient self);

void
ControlObjectClient_setTestMode(ControlObjectClient self, bool value);

void
ControlObjectClient_setOrigin(ControlObjectClient self, const char* orIdent, int orCat);

/**
 * \brief Use a constant T parameter for all command (select, operate, cancel) of a single control sequence
 *
 * NOTE: Some non-standard compliant servers may require this to accept oper/cancel requests
 *
 * \param self the ControlObjectClient instance
 * \param useContantT enable this behaviour with true, disable with false
 */
void
ControlObjectClient_useConstantT(ControlObjectClient self, bool useConstantT);

/**
 * \deprecated use ControlObjectClient_setInterlockCheck instead
 */
void
ControlObjectClient_enableInterlockCheck(ControlObjectClient self);

/**
 * \deprecated use ControlObjectClient_setSynchroCheck instead
 */
void
ControlObjectClient_enableSynchroCheck(ControlObjectClient self);

/**
 * \brief Set the value of the interlock check flag when a control command is sent
 *
 * \param self the ControlObjectClient instance
 * \param value if true the server will perform a interlock check if supported
 */
void
ControlObjectClient_setInterlockCheck(ControlObjectClient self, bool value);

/**
 * \brief Set the value of the synchro check flag when a control command is sent
 *
 * \param self the ControlObjectClient instance
 * \param value if true the server will perform a synchro check if supported
 */
void
ControlObjectClient_setSynchroCheck(ControlObjectClient self, bool value);


/**
 * \brief Private a callback handler that is invoked when a command termination message is received.
 *
 * This callback is invoked whenever a CommandTermination+ or CommandTermination- message is received.
 * To distinguish between a CommandTermination+ and CommandTermination- please use the
 * ControlObjectClient_getLastApplError function.
 *
 * \param self the ControlObjectClient instance
 * \param handler the callback function to be used
 * \param handlerParameter an arbitrary parameter that is passed to the handler
 */
typedef void (*CommandTerminationHandler) (void* parameter, ControlObjectClient controlClient);

void
ControlObjectClient_setCommandTerminationHandler(ControlObjectClient self, CommandTerminationHandler handler,
        void* handlerParameter);

/** @} */

/*************************************
 * Model discovery services
 ************************************/

/**
 * @defgroup IEC61850_CLIENT_MODEL_DISCOVERY Model discovery services
 *
 * @{
 */

/**
 * \brief Retrieve the device model from the server
 *
 * This function retrieves the complete device model from the server. The model is buffered an can be browsed
 * by subsequent API calls. This API call is mapped to multiple ACSI services.
 *
 * \param self the connection object
 * \param error the error code if an error occurs
 *
 */
void
IedConnection_getDeviceModelFromServer(IedConnection self, IedClientError* error);

/**
 * \brief Get the list of logical devices available at the server (DEPRECATED)
 *
 * This function is mapped to the GetServerDirectory(LD) ACSI service.
 *
 * \param self the connection object
 * \param error the error code if an error occurs
 *
 * \return LinkedList with string elements representing the logical device names
 */
LinkedList /*<char*>*/
IedConnection_getLogicalDeviceList(IedConnection self, IedClientError* error);

/**
 * \brief Get the list of logical devices or files available at the server
 *
 * GetServerDirectory ACSI service implementation. This function will either return the list of
 * logical devices (LD) present at the server or the list of available files.
 *
 * \param self the connection object
 * \param error the error code if an error occurs
 * \param getFileNames get list of files instead of logical device names (TO BE IMPLEMENTED)
 *
 * \return LinkedList with string elements representing the logical device names or file names
 */
LinkedList /*<char*>*/
IedConnection_getServerDirectory(IedConnection self, IedClientError* error, bool getFileNames);

/**
 * \brief Get the list of logical nodes (LN) of a logical device
 *
 * LGetLogicalDeviceDirectory ACSI service implementation. Returns the list of logical nodes present
 * in a logical device. The list is returned as a linked list of type LinkedList where the elements
 * are C style strings.
 *
 * \param self the connection object
 * \param error the error code if an error occurs
 * \param getFileNames get list of files instead of logical device names (TO BE IMPLEMENTED)
 *
 * \return  LinkedList with string elements representing the logical node names
 */
LinkedList /*<char*>*/
IedConnection_getLogicalDeviceDirectory(IedConnection self, IedClientError* error, const char* logicalDeviceName);

typedef enum {
    ACSI_CLASS_DATA_OBJECT,
    ACSI_CLASS_DATA_SET,
    ACSI_CLASS_BRCB,
    ACSI_CLASS_URCB,
    ACSI_CLASS_LCB,
    ACSI_CLASS_LOG,
    ACSI_CLASS_SGCB,
    ACSI_CLASS_GoCB,
    ACSI_CLASS_GsCB,
    ACSI_CLASS_MSVCB,
    ACSI_CLASS_USVCB
} ACSIClass;

/**
 * \brief returns a list of all MMS variables that are children of the given logical node
 *
 * This function cannot be mapped to any ACSI service. It is a convenience function for generic clients that
 * wants to show a list of all available children of the MMS named variable representing the logical node.
 *
 * \param self the connection object
 * \param error the error code if an error occurs
 * \param logicalNodeReference string that represents the LN reference
 *
 * \return the list of all MMS named variables as C strings in a LinkedList type
 *
 */
LinkedList /*<char*>*/
IedConnection_getLogicalNodeVariables(IedConnection self, IedClientError* error,
		const char* logicalNodeReference);

/**
 * \brief returns the directory of the given logical node (LN) containing elements of the specified ACSI class
 *
 * Implementation of the GetLogicalNodeDirectory ACSI service. In contrast to the ACSI description this
 * function does not always creates a request to the server. For most ACSI classes it simply accesses the
 * data model that was retrieved before. An exception to this rule are the ACSI classes ACSI_CLASS_DATASET and
 * ACSI_CLASS_LOG. Both always perform a request to the server.
 *
 * \param self the connection object
 * \param error the error code if an error occurs
 * \param logicalNodeReference string that represents the LN reference
 * \param acsiClass specifies the ACSI class
 *
 * \return list of all logical node elements of the specified ACSI class type as C strings in a LinkedList
 *
 */
LinkedList /*<char*>*/
IedConnection_getLogicalNodeDirectory(IedConnection self, IedClientError* error,
		const char* logicalNodeReference, ACSIClass acsiClass);

/**
 * \brief returns the directory of the given data object (DO)
 *
 * Implementation of the GetDataDirectory ACSI service. This will return the list of
 * all data attributes or sub data objects.
 *
 * \param self the connection object
 * \param error the error code if an error occurs
 * \param dataReference string that represents the DO reference
 *
 * \return list of all data attributes or sub data objects as C strings in a LinkedList
 *
 */
LinkedList /*<char*>*/
IedConnection_getDataDirectory(IedConnection self, IedClientError* error, const char* dataReference);

/**
 * \brief returns the directory of the given data object (DO)
 *
 * Implementation of the GetDataDirectory ACSI service. This will return the list of
 * C strings with all data attributes or sub data objects as elements. The returned
 * strings will contain the functional constraint appended in square brackets when appropriate.
 *
 * \param self the connection object
 * \param error the error code if an error occurs
 * \param dataReference string that represents the DO reference
 *
 * \return list of all data attributes or sub data objects as C strings in a LinkedList
 *
 */
LinkedList /*<char*>*/
IedConnection_getDataDirectoryFC(IedConnection self, IedClientError* error, const char* dataReference);

/**
 * \brief returns the directory of the given data object/data attribute with the given FC
 *
 * Implementation of the GetDataDirectory ACSI service. This will return the list of
 * C strings with all data attributes or sub data objects as elements. The returned
 * strings will contain the functional constraint appended in square brackets when appropriate.
 *
 * \param self the connection object
 * \param error the error code if an error occurs
 * \param dataReference string that represents the DO reference
 * \param fc the functional constraint
 *
 * \return list of all data attributes or sub data objects as C strings in a LinkedList
 *
 */
LinkedList
IedConnection_getDataDirectoryByFC(IedConnection self, IedClientError* error, const char* dataReference, FunctionalConstraint fc);

/**
 * \brief return the MMS variable type specification of the data attribute referenced by dataAttributeReference and function constraint fc.
 *
 * This function can be used to get the MMS variable type specification for an IEC 61850 data attribute. It is an extension
 * of the ACSI that may be required by generic client applications.
 *
 * \param self the connection object
 * \param error the error code if an error occurs
 * \param dataAttributeReference string that represents the DA reference
 * \param fc functional constraint of the DA
 *
 * \return MmsVariableSpecification of the data attribute.
 *
 */
MmsVariableSpecification*
IedConnection_getVariableSpecification(IedConnection self, IedClientError* error, const char* dataAttributeReference,
        FunctionalConstraint fc);

/** @} */

/**
 * @defgroup IEC61850_CLIENT_LOG_SERVICE Log service related functions, data types, and definitions
 *
 * @{
 */

/**
 * \brief Implementation of the QueryLogByTime ACSI service
 *
 * Read log entries from a log at the server. The log entries to read are specified by
 * a starting time and an end time. If the complete range does not fit in a single MMS message
 * the moreFollows flag will be set to true, to indicate that more entries are available for the
 * specified time range.
 *
 * \param self the connection object
 * \param error the error code if an error occurs
 * \param logReference log object reference in the form <LD name>/<LN name>$<log name>
 * \param startTime as millisecond UTC timestamp
 * \param endTime as millisecond UTC timestamp
 * \param moreFollows (output value) indicates that more entries are available that match the specification.
 *
 * \return list of MmsJournalEntry objects matching the specification
 */
LinkedList /* <MmsJournalEntry> */
IedConnection_queryLogByTime(IedConnection self, IedClientError* error, const char* logReference,
        uint64_t startTime, uint64_t endTime, bool* moreFollows);

/**
 * \brief Implementation of the QueryLogAfter ACSI service
 *
 * Read log entries from a log at the server following the entry with the specified entryID and timestamp.
 * If the complete range does not fit in a single MMS message
 * the moreFollows flag will be set to true, to indicate that more entries are available for the
 * specified time range.
 *
 * \param self the connection object
 * \param error the error code if an error occurs
 * \param logReference log object reference in the form <LD name>/<LN name>$<log name>
 * \param entryID usually the entryID of the last received entry
 * \param timeStamp as millisecond UTC timestamp
 * \param moreFollows (output value) indicates that more entries are available that match the specification.
 *
 * \return list of MmsJournalEntry objects matching the specification
 */
LinkedList /* <MmsJournalEntry> */
IedConnection_queryLogAfter(IedConnection self, IedClientError* error, const char* logReference,
        MmsValue* entryID, uint64_t timeStamp, bool* moreFollows);



/** @} */

/**
 * @defgroup IEC61850_CLIENT_FILE_SERVICE File service related functions, data types, and definitions
 *
 * @{
 */

typedef struct sFileDirectoryEntry* FileDirectoryEntry;

FileDirectoryEntry
FileDirectoryEntry_create(const char* fileName, uint32_t fileSize, uint64_t lastModified);

void
FileDirectoryEntry_destroy(FileDirectoryEntry self);

char*
FileDirectoryEntry_getFileName(FileDirectoryEntry self);

uint32_t
FileDirectoryEntry_getFileSize(FileDirectoryEntry self);

uint64_t
FileDirectoryEntry_getLastModified(FileDirectoryEntry self);


/**
 * \brief returns the directory entries of the specified file directory.
 *
 * Requires the server to support file services.
 *
 * NOTE: the returned linked list has to be freed by the user. You can user the following statement
 * to free the list of directory entries:
 *
 * LinkedList_destroyDeep(fileNames, (LinkedListValueDeleteFunction) FileDirectoryEntry_destroy);
 *
 * where fileNames is the return value of this function.
 *
 * \param self the connection object
 * \param error the error code if an error occurs
 * \param directoryName the name of the directory or NULL to get the entries of the root directory
 *
 * \return the list of directory entries. The return type is a LinkedList with FileDirectoryEntry elements
 */
LinkedList /*<FileDirectoryEntry>*/
IedConnection_getFileDirectory(IedConnection self, IedClientError* error, const char* directoryName);


/**
 * \brief returns the directory entries of the specified file directory returned by a single file directory request.
 *
 * This function will only create a single request and the result may only be the directory that fits
 * into a single MMS PDU. If the server contains more directory entries, this will be indicated by setting
 * the moreFollows variable (if provided by the caller). If the directory entry does not fit into a single MMS
 * PDU the next part of the directory list can be requested by setting the continueAfter parameter with the value
 * of the last filename of the received list.
 *
 * Requires the server to support file services.
 *
 * NOTE: the returned linked list has to be freed by the user. You can user the following statement
 * to free the list of directory entries:
 *
 * LinkedList_destroyDeep(fileNames, (LinkedListValueDeleteFunction) FileDirectoryEntry_destroy);
 *
 * where fileNames is the return value of this function.
 *
 * \param self the connection object
 * \param error the error code if an error occurs
 * \param directoryName the name of the directory or NULL to get the entries of the root directory
 * \param continueAfter last received filename to continue after, or NULL for the first request
 * \param moreFollows if provided by the caller (non NULL) the function will indicate if more directory entries
 *                    are available.
 *
 * \return the list of directory entries. The return type is a LinkedList with FileDirectoryEntry elements
 */
LinkedList /*<FileDirectoryEntry>*/
IedConnection_getFileDirectoryEx(IedConnection self, IedClientError* error, const char* directoryName, const char* continueAfter,
        bool* moreFollows);

/**
 * \brief user provided handler to receive the data of the GetFile request
 *
 * This handler will be invoked whenever the clients receives a data block from
 * the server. The API user has to copy the data to another location before returning.
 * The other location could for example be a file in the clients file system.
 *
 * \param parameter user provided parameter
 * \param buffer pointer to the buffer containing the received data
 * \param bytesRead number of bytes available in the buffer
 *
 * \return true if the client implementation shall continue to download data false if the download
 *         should be stopped. E.g. if the file cannot be stored client side due to missing resources.
 */
typedef bool
(*IedClientGetFileHandler) (void* parameter, uint8_t* buffer, uint32_t bytesRead);

/**
 * \brief Implementation of the GetFile ACSI service
 *
 * Download a file from the server.
 *
 * \param self the connection object
 * \param error the error code if an error occurs
 * \param fileName the name of the file to be read from the server
 *
 * \return number of bytes received
 */
uint32_t
IedConnection_getFile(IedConnection self, IedClientError* error, const char* fileName, IedClientGetFileHandler handler,
        void* handlerParameter);

/**
 * \brief Set the virtual filestore basepath for the setFile service
 *
 * All external file service accesses will be mapped to paths relative to the base directory.
 * NOTE: This function is only available when the CONFIG_SET_FILESTORE_BASEPATH_AT_RUNTIME
 * option in stack_config.h is set.
 *
 * \param self the connection object
 * \param basepath the new virtual filestore basepath
 */
void
IedConnection_setFilestoreBasepath(IedConnection, const char* basepath);

/**
 * \brief Implementation of the SetFile ACSI service
 *
 * Upload a file to the server. The file has to be available in the local VMD filestore.
 *
 * \param self the connection object
 * \param error the error code if an error occurs
 * \param sourceFilename the filename of the local (client side) file
 * \param destinationFilename the filename of the remote (service side) file
 */
void
IedConnection_setFile(IedConnection self, IedClientError* error, const char* sourceFilename, const char* destinationFilename);


/**
 * \brief Implementation of the DeleteFile ACSI service
 *
 * Delete a file at the server.
 *
 * \param self the connection object
 * \param error the error code if an error occurs
 * \param fileName the name of the file to delete
 */
void
IedConnection_deleteFile(IedConnection self, IedClientError* error, const char* fileName);


/** @} */

/**@}*/

#ifdef __cplusplus
}
#endif


#endif /* IEC61850_CLIENT_H_ */
