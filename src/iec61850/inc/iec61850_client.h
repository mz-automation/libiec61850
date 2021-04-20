/*
 *  iec61850_client.h
 *
 *  Copyright 2013-2019 Michael Zillgith
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
    ControlLastApplError error;
    ControlAddCause addCause;
} LastApplError;

/** Connection state of the IedConnection instance - either closed(idle), connecting, connected, or closing) */
typedef enum
{
    IED_STATE_CLOSED = 0,
    IED_STATE_CONNECTING,
    IED_STATE_CONNECTED,
    IED_STATE_CLOSING
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

    /** Cannot send request because outstanding call limit is reached */
    IED_ERROR_OUTSTANDING_CALL_LIMIT_REACHED = 6,

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

    /** Received an invalid response message from the server */
    IED_ERROR_MALFORMED_MESSAGE = 34,

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
 * It allocated all required resources. The new connection is in the "CLOSED" state. Before it can be used
 * the connect method has to be called. The connection will be in non-TLS and thread mode.
 *
 * \return the new IedConnection instance
 */
LIB61850_API IedConnection
IedConnection_create(void);

/**
 * \brief create a new IedConnection instance (extended version)
 *
 * This function creates a new IedConnection instance that is used to handle a connection to an IED.
 * It allocated all required resources. The new connection is in the "CLOSED" state. Before it can be used
 * the \ref IedConnection_connect or \ref IedConnection_connectAsync method has to be called.
 * The connection will use TLS when a TLSConfiguration object is provided. The useThread is false the
 * IedConnection is in non-thread mode and the IedConnection_tick function has to be called periodically to
 * receive messages and perform the house-keeping tasks.
 *
 * \param tlsConfig the TLS configuration to be used, or NULL for non-TLS connection
 * \param useThreads when true, the IedConnection is in thread mode
 *
 * \return the new IedConnection instance
 */
LIB61850_API IedConnection
IedConnection_createEx(TLSConfiguration tlsConfig, bool useThreads);

/**
 * \brief create a new IedConnection instance that has support for TLS
 *
 * This function creates a new IedConnection instance that is used to handle a connection to an IED.
 * It allocated all required resources. The new connection is in the "CLOSED" state. Before it can be used
 * the \ref IedConnection_connect or \ref IedConnection_connectAsync method has to be called.
 * The connection will use TLS when a TLSConfiguration object is provided. The connection will be in thread mode.
 *
 * \deprecated Use \ref IedConnection_createEx instead
 *
 * \param tlsConfig the TLS configuration to be used
 *
 * \return the new IedConnection instance
 */
LIB61850_API IedConnection
IedConnection_createWithTlsSupport(TLSConfiguration tlsConfig);

/**
 * \brief destroy an IedConnection instance.
 *
 * The connection will be closed if it is in "connected" state. All allocated resources of the connection
 * will be freed.
 *
 * \param self the connection object
 */
LIB61850_API void
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
LIB61850_API void
IedConnection_setConnectTimeout(IedConnection self, uint32_t timeoutInMs);

/**
 * \brief set the request timeout in ms
 *
 * Set the request timeout for this connection. You can call this function any time to adjust
 * timeout behavior.
 *
 * \param self the connection object
 * \param timoutInMs the connection timeout in ms
 */
LIB61850_API void
IedConnection_setRequestTimeout(IedConnection self, uint32_t timeoutInMs);

/**
 * \brief get the request timeout in ms for this connection
 *
 * \param self the connection object
 *
 * \return request timeout in milliseconds
 */
LIB61850_API uint32_t
IedConnection_getRequestTimeout(IedConnection self);

/**
 * \brief Perform MMS message handling and house-keeping tasks (for non-thread mode only)
 *
 * This function has to be called periodically by the user application in non-thread mode. The return
 * value helps to decide when the stack has nothing to do and other tasks can be executed.
 *
 * NOTE: When using non-thread mode you should NOT use the synchronous (blocking) API functions. The
 * synchronous functions will block forever when IedConnection_tick is not called in a separate thread.
 *
 * \return true when connection is currently waiting and calling thread can be suspended, false means
 *         connection is busy and the tick function should be called again as soon as possible.
 */
LIB61850_API bool
IedConnection_tick(IedConnection self);

/**
 * \brief Generic serivce callback handler
 *
 * NOTE: This callback handler is used by several asynchronous service functions that require
 * only a simple feedback in form of a success (IED_ERROR_OK) or failure (other \ref err value).
 *
 * \param invokeId the invoke ID used by the related service request
 * \param parameter user provided parameter
 * \param err the result code. IED_ERROR_OK indicates success.
 */
typedef void
(*IedConnection_GenericServiceHandler) (uint32_t invokeId, void* parameter, IedClientError err);

/**************************************************
 * Association service
 **************************************************/

/**
 * \brief Connect to a server
 *
 * NOTE: Function will block until connection is up or timeout happened.
 *
 * \param self the connection object
 * \param error the error code if an error occurs
 * \param hostname the host name or IP address of the server to connect to
 * \param tcpPort the TCP port number of the server to connect to
 */
LIB61850_API void
IedConnection_connect(IedConnection self, IedClientError* error, const char* hostname, int tcpPort);

/**
 * \brief Asynchronously connect to a server
 *
 * The function will return immediately. No error doesn't indicate that the
 * connection is established. The current connection state has to be tracked
 * by polling the \ref IedConnection_getState function or by using
 * \ref IedConnection_StateChangedHandler
 *
 * \param self the connection object
 * \param error the error code if an error occurs
 * \param hostname the host name or IP address of the server to connect to
 * \param tcpPort the TCP port number of the server to connect to
 */
LIB61850_API void
IedConnection_connectAsync(IedConnection self, IedClientError* error, const char* hostname, int tcpPort);

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
LIB61850_API void
IedConnection_abort(IedConnection self, IedClientError* error);

/**
 * \brief Asynchronously abort the connection
 *
 * This will close the MMS association by sending an ACSE abort message to the server.
 * After sending the abort message the connection is closed immediately.
 * If the connection is not in "connected" state an IED_ERROR_NOT_CONNECTED error will be reported.
 *
 * NOTE: This function works asynchronously. The IedConnection object should not be destroyed before the
 * connection state changes to IED_STATE_CLOSED.
 *
 * \param self the connection object
 * \param error the error code if an error occurs
 */
LIB61850_API void
IedConnection_abortAsync(IedConnection self, IedClientError* error);

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
LIB61850_API void
IedConnection_release(IedConnection self, IedClientError* error);

/**
 * \brief Asynchronously release the connection
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
LIB61850_API void
IedConnection_releaseAsync(IedConnection self, IedClientError* error);

/**
 * \brief Close the connection
 *
 * This will close the MMS association and the underlying TCP connection.
 *
 * \param self the connection object
 */
LIB61850_API void
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
LIB61850_API IedConnectionState
IedConnection_getState(IedConnection self);

/**
 * \brief Access to last application error received by the client connection
 *
 * \param self the connection object
 *
 * \return the LastApplError value
 */
LIB61850_API LastApplError
IedConnection_getLastApplError(IedConnection self);

/**
 * \brief Callback handler that is invoked when the connection is closed
 *
 * \deprecated Use \ref IedConnection_StateChangedHandler instead
 *
 * \param user provided parameter
 * \param connection the connection object of the closed connection
 */
typedef void
(*IedConnectionClosedHandler) (void* parameter, IedConnection connection);

/**
 * \brief Install a handler function that is called when the connection is lost/closed.
 *
 * \deprecated Use \ref IedConnection_StateChangedHandler instead
 *
 * \param self the connection object
 * \param handler that callback function
 * \param parameter the user provided parameter that is handed over to the callback function
 */
LIB61850_API void
IedConnection_installConnectionClosedHandler(IedConnection self, IedConnectionClosedHandler handler,
        void* parameter);

/**
 * \brief Callback handler that is invoked whenever the connection state (\ref IedConnectionState) changes
 *
 * \param user provided parameter
 * \param connection the related connection
 * \param newState the new state of the connection
 */
typedef void
(*IedConnection_StateChangedHandler) (void* parameter, IedConnection connection, IedConnectionState newState);

/**
 * \brief Install a handler function that is called when the connection state changes
 *
 * \param self the connection object
 * \param handler that callback function
 * \param parameter the user provided parameter that is handed over to the callback function
 */
LIB61850_API void
IedConnection_installStateChangedHandler(IedConnection self, IedConnection_StateChangedHandler handler, void* parameter);

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
LIB61850_API MmsConnection
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

#define IEC61850_SV_SMPMOD_SAMPLES_PER_PERIOD 0

#define IEC61850_SV_SMPMOD_SAMPLES_PER_SECOND 1

#define IEC61850_SV_SMPMOD_SECONDS_PER_SAMPLE 2


/** an opaque handle to the instance data of a ClientSVControlBlock object */
typedef struct sClientSVControlBlock* ClientSVControlBlock;

/**
 * \brief Create a new ClientSVControlBlock instance
 *
 * This function simplifies client side access to server MSV/USV control blocks
 * NOTE: Do not use the functions after the IedConnection object is invalidated!
 *
 * The access functions cause synchronous read/write calls to the server. For asynchronous
 * access use the \ref IedConnection_readObjectAsync and \ref IedConnection_writeObjectAsync
 * functions.
 *
 * \param connection the IedConnection object with a valid connection to the server.
 * \param reference the object reference of the control block
 *
 * \return the new instance
 */
LIB61850_API ClientSVControlBlock
ClientSVControlBlock_create(IedConnection connection, const char* reference);

/**
 * \brief Free all resources related to the ClientSVControlBlock instance.
 *
 * \param self the ClientSVControlBlock instance to operate on
 */
LIB61850_API void
ClientSVControlBlock_destroy(ClientSVControlBlock self);

/**
 * \brief Test if this SVCB is multicast
 *
 * \param self the ClientSVControlBlock instance to operate on
 *
 * \return true if multicast SCVB, false otherwise (unicast)
 */
LIB61850_API bool
ClientSVControlBlock_isMulticast(ClientSVControlBlock self);

/**
 * \brief Return the error code of the last write or write acccess to the SVCB
 *
 * \param self the ClientSVControlBlock instance to operate on
 *
 * \return the error code of the last read or write access
 */
LIB61850_API IedClientError
ClientSVControlBlock_getLastComError(ClientSVControlBlock self);


LIB61850_API bool
ClientSVControlBlock_setSvEna(ClientSVControlBlock self, bool value);

LIB61850_API bool
ClientSVControlBlock_getSvEna(ClientSVControlBlock self);

LIB61850_API bool
ClientSVControlBlock_setResv(ClientSVControlBlock self, bool value);

LIB61850_API bool
ClientSVControlBlock_getResv(ClientSVControlBlock self);

LIB61850_API char*
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
LIB61850_API char*
ClientSVControlBlock_getDatSet(ClientSVControlBlock self);

LIB61850_API uint32_t
ClientSVControlBlock_getConfRev(ClientSVControlBlock self);

LIB61850_API uint16_t
ClientSVControlBlock_getSmpRate(ClientSVControlBlock self);


/**
 * \brief returns the destination address of the SV publisher
 *
 * \param self the ClientSVControlBlock instance to operate on
 */
LIB61850_API PhyComAddress
ClientSVControlBlock_getDstAddress(ClientSVControlBlock self);

/**
 * \brief Gets the OptFlds parameter of the RCB (decides what information to include in a report)
 *
 * \param self the RCB instance
 * \return bit field representing the optional fields of a report (uses flags from \ref REPORT_OPTIONS)
 */
LIB61850_API int
ClientSVControlBlock_getOptFlds(ClientSVControlBlock self);

/**
 * \brief returns number of sample mode of the SV publisher
 *
 * \param self the ClientSVControlBlock instance to operate on
 */
LIB61850_API uint8_t
ClientSVControlBlock_getSmpMod(ClientSVControlBlock self);

/**
 * \brief returns number of ASDUs included in the SV message
 *
 * \param self the ClientSVControlBlock instance to operate on
 *
 * \return the number of ASDU included in a single SV message
 */
LIB61850_API int
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

LIB61850_API ClientGooseControlBlock
ClientGooseControlBlock_create(const char* dataAttributeReference);

LIB61850_API void
ClientGooseControlBlock_destroy(ClientGooseControlBlock self);

LIB61850_API bool
ClientGooseControlBlock_getGoEna(ClientGooseControlBlock self);

LIB61850_API void
ClientGooseControlBlock_setGoEna(ClientGooseControlBlock self, bool goEna);

LIB61850_API const char*
ClientGooseControlBlock_getGoID(ClientGooseControlBlock self);

LIB61850_API void
ClientGooseControlBlock_setGoID(ClientGooseControlBlock self, const char* goID);

LIB61850_API const char*
ClientGooseControlBlock_getDatSet(ClientGooseControlBlock self);

LIB61850_API void
ClientGooseControlBlock_setDatSet(ClientGooseControlBlock self, const char* datSet);

LIB61850_API uint32_t
ClientGooseControlBlock_getConfRev(ClientGooseControlBlock self);

LIB61850_API bool
ClientGooseControlBlock_getNdsComm(ClientGooseControlBlock self);

LIB61850_API uint32_t
ClientGooseControlBlock_getMinTime(ClientGooseControlBlock self);

LIB61850_API uint32_t
ClientGooseControlBlock_getMaxTime(ClientGooseControlBlock self);

LIB61850_API bool
ClientGooseControlBlock_getFixedOffs(ClientGooseControlBlock self);

LIB61850_API PhyComAddress
ClientGooseControlBlock_getDstAddress(ClientGooseControlBlock self);

LIB61850_API void
ClientGooseControlBlock_setDstAddress(ClientGooseControlBlock self, PhyComAddress value);

LIB61850_API DEPRECATED MmsValue* /* MMS_OCTET_STRING */
ClientGooseControlBlock_getDstAddress_addr(ClientGooseControlBlock self);

LIB61850_API DEPRECATED void
ClientGooseControlBlock_setDstAddress_addr(ClientGooseControlBlock self, MmsValue* macAddr);

LIB61850_API DEPRECATED uint8_t
ClientGooseControlBlock_getDstAddress_priority(ClientGooseControlBlock self);

LIB61850_API DEPRECATED void
ClientGooseControlBlock_setDstAddress_priority(ClientGooseControlBlock self, uint8_t priorityValue);

LIB61850_API DEPRECATED uint16_t
ClientGooseControlBlock_getDstAddress_vid(ClientGooseControlBlock self);

LIB61850_API DEPRECATED void
ClientGooseControlBlock_setDstAddress_vid(ClientGooseControlBlock self, uint16_t vidValue);

LIB61850_API DEPRECATED uint16_t
ClientGooseControlBlock_getDstAddress_appid(ClientGooseControlBlock self);

LIB61850_API DEPRECATED void
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
LIB61850_API ClientGooseControlBlock
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
LIB61850_API void
IedConnection_setGoCBValues(IedConnection self, IedClientError* error, ClientGooseControlBlock goCB,
        uint32_t parametersMask, bool singleRequest);

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
LIB61850_API MmsValue*
IedConnection_readObject(IedConnection self, IedClientError* error, const char* dataAttributeReference, FunctionalConstraint fc);

typedef void
(*IedConnection_ReadObjectHandler) (uint32_t invokeId, void* parameter, IedClientError err, MmsValue* value);

/**
 * \brief read a functional constrained data attribute (FCDA) or functional constrained data (FCD) - async version
 *
 * \param self  the connection object to operate on
 * \param error the error code if an error occurs
 * \param object reference of the object/attribute to read
 * \param fc the functional constraint of the data attribute or data object to read
 * \param handler the user provided callback handler
 * \param parameter user provided parameter that is passed to the callback handler
 *
 * \return the invoke ID of the request
 */
LIB61850_API uint32_t
IedConnection_readObjectAsync(IedConnection self, IedClientError* error, const char* objRef, FunctionalConstraint fc,
        IedConnection_ReadObjectHandler handler, void* parameter);

/**
 * \brief write a functional constrained data attribute (FCDA) or functional constrained data (FCD).
 *
 * \param self  the connection object to operate on
 * \param error the error code if an error occurs
 * \param object reference of the object/attribute to write
 * \param fc the functional constraint of the data attribute or data object to write
 * \param value the MmsValue to write (has to be of the correct type - MMS_STRUCTURE for FCD)
 */
LIB61850_API void
IedConnection_writeObject(IedConnection self, IedClientError* error, const char* dataAttributeReference, FunctionalConstraint fc,
        MmsValue* value);

/**
 * \brief write a functional constrained data attribute (FCDA) or functional constrained data (FCD) - async version
 *
 * \param self  the connection object to operate on
 * \param error the error code if an error occurs
 * \param object reference of the object/attribute to write
 * \param fc the functional constraint of the data attribute or data object to write
 * \param value the MmsValue to write (has to be of the correct type - MMS_STRUCTURE for FCD)
 * \param handler the user provided callback handler
 * \param parameter user provided parameter that is passed to the callback handler
 *
 * \return the invoke ID of the request
 */
LIB61850_API uint32_t
IedConnection_writeObjectAsync(IedConnection self, IedClientError* error, const char* objectReference,
        FunctionalConstraint fc, MmsValue* value, IedConnection_GenericServiceHandler handler, void* parameter);

/**
 * \brief read a functional constrained data attribute (FCDA) of type boolean
 *
 * \param self  the connection object to operate on
 * \param error the error code if an error occurs
 * \param object reference of the data attribute to read
 * \param fc the functional constraint of the data attribute to read
 */
LIB61850_API bool
IedConnection_readBooleanValue(IedConnection self, IedClientError* error, const char* objectReference, FunctionalConstraint fc);

/**
 * \brief read a functional constrained data attribute (FCDA) of type float
 *
 * \param self  the connection object to operate on
 * \param error the error code if an error occurs
 * \param object reference of the data attribute to read
 * \param fc the functional constraint of the data attribute to read
 */
LIB61850_API float
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
LIB61850_API char*
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
LIB61850_API int32_t
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
LIB61850_API int64_t
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
LIB61850_API uint32_t
IedConnection_readUnsigned32Value(IedConnection self, IedClientError* error, const char* objectReference, FunctionalConstraint fc);

/**
 * \brief read a functional constrained data attribute (FCDA) of type Timestamp (UTC Time)
 *
 *  NOTE: If the timestamp parameter is set to NULL the function allocates a new timestamp instance. Otherwise the
 *  return value is a pointer to the user provided timestamp instance. The new timestamp instance has to be freed by
 *  the caller of the function.
 *
 * \param self  the connection object to operate on
 * \param error the error code if an error occurs
 * \param object reference of the data attribute to read
 * \param fc the functional constraint of the data attribute to read
 * \param timestamp a pointer to a user provided timestamp instance or NULL
 *
 * \return the timestamp value
 */
LIB61850_API Timestamp*
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
LIB61850_API Quality
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
LIB61850_API void
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
LIB61850_API void
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
LIB61850_API void
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
LIB61850_API void
IedConnection_writeFloatValue(IedConnection self, IedClientError* error, const char* objectReference,
        FunctionalConstraint fc, float value);

LIB61850_API void
IedConnection_writeVisibleStringValue(IedConnection self, IedClientError* error, const char* objectReference,
        FunctionalConstraint fc, char* value);

LIB61850_API void
IedConnection_writeOctetString(IedConnection self, IedClientError* error, const char* objectReference,
        FunctionalConstraint fc, uint8_t* value, int valueLength);

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
 * "simpleIOGenericIO/LLN0.RP.EventsRCB01"
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
LIB61850_API ClientReportControlBlock
IedConnection_getRCBValues(IedConnection self, IedClientError* error, const char* rcbReference,
        ClientReportControlBlock updateRcb);

typedef void
(*IedConnection_GetRCBValuesHandler) (uint32_t invokeId, void* parameter, IedClientError err, ClientReportControlBlock rcb);

LIB61850_API uint32_t
IedConnection_getRCBValuesAsync(IedConnection self, IedClientError* error, const char* rcbReference, ClientReportControlBlock updateRcb,
        IedConnection_GetRCBValuesHandler handler, void* parameter);

/** Describes the reason for the inclusion of the element in the report */
typedef int ReasonForInclusion;

/** the element is not included in the received report */
#define IEC61850_REASON_NOT_INCLUDED 0

/** the element is included due to a change of the data value */
#define IEC61850_REASON_DATA_CHANGE 1

/** the element is included due to a change in the quality of data */
#define IEC61850_REASON_QUALITY_CHANGE 2

/** the element is included due to an update of the data value */
#define IEC61850_REASON_DATA_UPDATE 4

/** the element is included due to a periodic integrity report task */
#define IEC61850_REASON_INTEGRITY 8

/** the element is included due to a general interrogation by the client */
#define IEC61850_REASON_GI 16

/** the reason for inclusion is unknown (e.g. report is not configured to include reason-for-inclusion) */
#define IEC61850_REASON_UNKNOWN 32

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
LIB61850_API void
IedConnection_setRCBValues(IedConnection self, IedClientError* error, ClientReportControlBlock rcb,
        uint32_t parametersMask, bool singleRequest);

LIB61850_API uint32_t
IedConnection_setRCBValuesAsync(IedConnection self, IedClientError* error, ClientReportControlBlock rcb,
        uint32_t parametersMask, bool singleRequest, IedConnection_GenericServiceHandler handler, void* parameter);

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
LIB61850_API void
IedConnection_installReportHandler(IedConnection self, const char* rcbReference, const char* rptId, ReportCallbackFunction handler,
        void* handlerParameter);

/**
 * \brief uninstall a report handler function for the specified report control block (RCB)
 *
 * \param self the connection object
 * \param rcbReference object reference of the report control block
 */
LIB61850_API void
IedConnection_uninstallReportHandler(IedConnection self, const char* rcbReference);

/**
 * \brief trigger a general interrogation (GI) report for the specified report control block (RCB)
 *
 * The RCB must have been enabled and GI set as trigger option before this command can be performed.
 *
 * \deprecated Use ClientReportControlBlock_setGI instead
 *
 * \param connection the connection object
 * \param error the error code if an error occurs
 * \param rcbReference object reference of the report control block
 */
LIB61850_API void
IedConnection_triggerGIReport(IedConnection self, IedClientError* error, const char* rcbReference);

/****************************************
 * Access to received reports
 ****************************************/

/**
 * \brief get the name of the report data set
 *
 * NOTE: the returned string is only valid as long as the ClientReport instance exists!
 *
 * \param self the ClientReport instance
 * \return report data set name as 0 terminated string
 */
LIB61850_API const char*
ClientReport_getDataSetName(ClientReport self);

/**
 * \brief return the received data set values of the report
 *
 * NOTE: The returned MmsValue instance is handled by the library and only valid as long as the
 * ClientReport instance exists! It should not be used outside the report callback handler to
 * avoid concurrency issues.
 *
 * \param self the ClientReport instance
 * \return an MmsValue array instance containing the data set values
 */
LIB61850_API MmsValue*
ClientReport_getDataSetValues(ClientReport self);

/**
 * \brief return reference (name) of the server RCB associated with this ClientReport object
 *
 * \param self the ClientReport instance
 * \return report control block reference as string
 */
LIB61850_API char*
ClientReport_getRcbReference(ClientReport self);

/**
 * \brief return RptId of the server RCB associated with this ClientReport object
 *
 * \param self the ClientReport instance
 * \return report control block reference as string
 */
LIB61850_API char*
ClientReport_getRptId(ClientReport self);

/**
 * \brief get the reason code (reason for inclusion) for a specific report data set element
 *
 * \param self the ClientReport instance
 * \param elementIndex index of the data set element (starting with 0)
 *
 * \return reason code for the inclusion of the specified element
 */
LIB61850_API ReasonForInclusion
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
LIB61850_API MmsValue*
ClientReport_getEntryId(ClientReport self);

/**
 * \brief determine if the last received report contains a timestamp
 *
 * \param self the ClientReport instance
 *
 * \return true if the report contains a timestamp, false otherwise
 */
LIB61850_API bool
ClientReport_hasTimestamp(ClientReport self);

/**
 * \brief determine if the last received report contains a sequence number
 *
 * \param self the ClientReport instance
 *
 * \return true if the report contains a sequence number, false otherwise
 */
LIB61850_API bool
ClientReport_hasSeqNum(ClientReport self);

/**
 * \brief get the value of the sequence number
 *
 * NOTE: The returned value is undefined if the sequence number is not present in report
 *
 * \param self the ClientReport instance
 *
 * \returns the number of the sequence number when present
 */
LIB61850_API uint16_t
ClientReport_getSeqNum(ClientReport self);

/**
 * \brief determine if the last received report contains the data set name
 *
 * \param self the ClientReport instance
 *
 * \return true if the report contains the data set name, false otherwise
 */
LIB61850_API bool
ClientReport_hasDataSetName(ClientReport self);

/**
 * \brief determine if the last received report contains reason-for-inclusion information
 *
 * \param self the ClientReport instance
 *
 * \return true if the report contains reason-for-inclusion information, false otherwise
 */
LIB61850_API bool
ClientReport_hasReasonForInclusion(ClientReport self);

/**
 * \brief determine if the last received report contains the configuration revision
 *
 * \param self the ClientReport instance
 *
 * \return true if the report contains the configuration revision, false otherwise
 */
LIB61850_API bool
ClientReport_hasConfRev(ClientReport self);

/**
 * \brief get the value of the configuration revision
 *
 * NOTE: The returned value is undefined if configuration revision is not present in report
 *
 * \param self the ClientReport instance
 *
 * \returns the number of the configuration revision
 */
LIB61850_API uint32_t
ClientReport_getConfRev(ClientReport self);

/**
 * \brief indicates if the report contains the bufOvfl (buffer overflow) flag
 *
 * \param self the ClientReport instance
 *
 * \returns true if the report contains the bufOvfl flag, false otherwise
 */
LIB61850_API bool
ClientReport_hasBufOvfl(ClientReport self);

/**
 * \brief get the value of the bufOvfl flag
 *
 * \param self the ClientReport instance
 *
 * \returns true if bufOvfl is set, false otherwise
 */
LIB61850_API bool
ClientReport_getBufOvfl(ClientReport self);

/**
 * \brief indicates if the report contains data references for the reported data set members
 *
 * \param self the ClientReport instance
 *
 * \returns true if the report contains data-references, false otherwise
 */
LIB61850_API bool
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
LIB61850_API const char*
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
LIB61850_API uint64_t
ClientReport_getTimestamp(ClientReport self);

/**
 * \brief indicates if the report contains a sub sequence number and a more segments follow flags (for segmented reporting)
 *
 * \param self the ClientReport instance
 *
 * \returns true if the report contains sub-sequence-number and more-follows-flag, false otherwise
 */
LIB61850_API bool
ClientReport_hasSubSeqNum(ClientReport self);

/**
 * \brief get the sub sequence number of the report (for segmented reporting)
 *
 * Returns the sub sequence number of the report. This is 0 for the first report of a segmented report and
 * will be increased by one for each report segment.
 *
 * \param self the ClientReport instance
 *
 * \return the sub sequence number of the last received report message.
 */
LIB61850_API uint16_t
ClientReport_getSubSeqNum(ClientReport self);

/**
 * \brief get the more segments follow flag of the received report segment (for segmented reporting)
 *
 * Will return true in case this is part of a segmented report and more report segments will follow or false, if
 * the current report is not a segmented report or is the last segment of a segmented report.
 *
 * \param self the ClientReport instance
 *
 * \return true when more segments of the current report will follow, false otherwise
 */
LIB61850_API bool
ClientReport_getMoreSeqmentsFollow(ClientReport self);

/**
 * \brief get the reason for inclusion of as a human readable string
 *
 * \param reasonCode
 *
 * \return the reason for inclusion as static human readable string
 */
LIB61850_API char*
ReasonForInclusion_getValueAsString(ReasonForInclusion reasonCode);

/**************************************************
 * ClientReportControlBlock access class
 **************************************************/

LIB61850_API ClientReportControlBlock
ClientReportControlBlock_create(const char* rcbReference);

LIB61850_API void
ClientReportControlBlock_destroy(ClientReportControlBlock self);

LIB61850_API char*
ClientReportControlBlock_getObjectReference(ClientReportControlBlock self);

LIB61850_API bool
ClientReportControlBlock_isBuffered(ClientReportControlBlock self);

LIB61850_API const char*
ClientReportControlBlock_getRptId(ClientReportControlBlock self);

LIB61850_API void
ClientReportControlBlock_setRptId(ClientReportControlBlock self, const char* rptId);

LIB61850_API bool
ClientReportControlBlock_getRptEna(ClientReportControlBlock self);

LIB61850_API void
ClientReportControlBlock_setRptEna(ClientReportControlBlock self, bool rptEna);

LIB61850_API bool
ClientReportControlBlock_getResv(ClientReportControlBlock self);

LIB61850_API void
ClientReportControlBlock_setResv(ClientReportControlBlock self, bool resv);

LIB61850_API const char*
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
LIB61850_API void
ClientReportControlBlock_setDataSetReference(ClientReportControlBlock self, const char* dataSetReference);

LIB61850_API uint32_t
ClientReportControlBlock_getConfRev(ClientReportControlBlock self);

/**
 * \brief Gets the OptFlds parameter of the RCB (decides what information to include in a report)
 *
 * \param self the RCB instance
 *
 * \return bit field representing the optional fields of a report (uses flags from \ref REPORT_OPTIONS)
 */
LIB61850_API int
ClientReportControlBlock_getOptFlds(ClientReportControlBlock self);

/**
 * \brief Set the OptFlds parameter of the RCB (decides what information to include in a report)
 *
 * \param self the RCB instance
 * \param optFlds bit field representing the optional fields of a report (use flags from \ref REPORT_OPTIONS)
 */
LIB61850_API void
ClientReportControlBlock_setOptFlds(ClientReportControlBlock self, int optFlds);

/**
 * \brief Get the BufTm (buffer time) parameter of the RCB
 *
 * The buffer time is the time to wait after a triggering event before the report is actually sent.
 * It is used to be able to collect events that happen in a short time period and send them in a single report.
 *
 * \param self the RCB instance
 */
LIB61850_API uint32_t
ClientReportControlBlock_getBufTm(ClientReportControlBlock self);

/**
 * \brief Set the BufTm (buffer time) parameter of the RCB
 *
 * The buffer time is the time to wait after a triggering event before the report is actually sent.
 * It is used to be able to collect events that happen in a short time period and send them in a single report.
 *
 * \param self the RCB instance
 * \param bufTm the buffer time in ms
 */
LIB61850_API void
ClientReportControlBlock_setBufTm(ClientReportControlBlock self, uint32_t bufTm);

LIB61850_API uint16_t
ClientReportControlBlock_getSqNum(ClientReportControlBlock self);

LIB61850_API int
ClientReportControlBlock_getTrgOps(ClientReportControlBlock self);

LIB61850_API void
ClientReportControlBlock_setTrgOps(ClientReportControlBlock self, int trgOps);

LIB61850_API uint32_t
ClientReportControlBlock_getIntgPd(ClientReportControlBlock self);

LIB61850_API void
ClientReportControlBlock_setIntgPd(ClientReportControlBlock self, uint32_t intgPd);

LIB61850_API bool
ClientReportControlBlock_getGI(ClientReportControlBlock self);

LIB61850_API void
ClientReportControlBlock_setGI(ClientReportControlBlock self, bool gi);

LIB61850_API bool
ClientReportControlBlock_getPurgeBuf(ClientReportControlBlock self);

/**
 * \brief Set the "PurgeBuf" attribute value (only BRCB)
 *
 * When set to true the report buffer will be cleared.
 *
 * \param purgeBuf attribute value
 */
LIB61850_API void
ClientReportControlBlock_setPurgeBuf(ClientReportControlBlock self, bool purgeBuf);

/**
 *  \brief Check if optional attribute "ResvTms" is present in BRCB
 *
 *  \return true when present, false otherwise
 */
LIB61850_API  bool
ClientReportControlBlock_hasResvTms(ClientReportControlBlock self);

LIB61850_API int16_t
ClientReportControlBlock_getResvTms(ClientReportControlBlock self);

LIB61850_API void
ClientReportControlBlock_setResvTms(ClientReportControlBlock self, int16_t resvTms);

LIB61850_API MmsValue* /* <MMS_OCTET_STRING> */
ClientReportControlBlock_getEntryId(ClientReportControlBlock self);

LIB61850_API void
ClientReportControlBlock_setEntryId(ClientReportControlBlock self, MmsValue* entryId);

LIB61850_API uint64_t
ClientReportControlBlock_getEntryTime(ClientReportControlBlock self);

LIB61850_API MmsValue* /* <MMS_OCTET_STRING> */
ClientReportControlBlock_getOwner(ClientReportControlBlock self);


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
 * \brief get data set values from the server
 *
 * The parameter dataSetReference is the name of the data set to read. It is either in the form LDName/LNodeName.dataSetName
 * for permanent domain or VMD scope data sets or @dataSetName for an association specific data set.
 * If the LDName part of the reference is missing the resulting data set will be of VMD scope.
 * The received data set values are stored in a container object of type ClientDataSet that can be reused in a further
 * read request.
 *
 * \param connection the connection object
 * \param error the error code if an error occurs
 * \param dataSetReference object reference of the data set
 * \param dataSet a data set instance where to store the retrieved values or NULL if a new instance
 *        shall be created.
 *
 * \return data set instance with retrieved values of NULL if an error occurred.
 */
LIB61850_API ClientDataSet
IedConnection_readDataSetValues(IedConnection self, IedClientError* error, const char* dataSetReference, ClientDataSet dataSet);

typedef void
(*IedConnection_ReadDataSetHandler) (uint32_t invokeId, void* parameter, IedClientError err, ClientDataSet dataSet);

/**
 * \brief get data set values from the server - async version
 *
 * The parameter dataSetReference is the name of the data set to read. It is either in the form LDName/LNodeName.dataSetName
 * for permanent domain or VMD scope data sets or @dataSetName for an association specific data set.
 * If the LDName part of the reference is missing the resulting data set will be of VMD scope.
 * The received data set values are stored in a container object of type ClientDataSet that can be reused in a further
 * service request.
 *
 * \param connection the connection object
 * \param error the error code if an error occurs
 * \param dataSetReference object reference of the data set
 * \param dataSet a data set instance where to store the retrieved values or NULL if a new instance
 *        shall be created.
 * \param handler the user provided callback handler
 * \param parameter user provided parameter that is passed to the callback handler
 *
 * \return the invoke ID of the request
 */
LIB61850_API uint32_t
IedConnection_readDataSetValuesAsync(IedConnection self, IedClientError* error, const char* dataSetReference, ClientDataSet dataSet,
        IedConnection_ReadDataSetHandler handler, void* parameter);

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
LIB61850_API void
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
LIB61850_API bool
IedConnection_deleteDataSet(IedConnection self, IedClientError* error, const char* dataSetReference);


/**
 * \brief returns the object references of the elements of a data set
 *
 * The return value contains a linked list containing the object references of FCDs or FCDAs. The format of
 * this object references is LDName/LNodeName.item(arrayIndex)component[FC].
 *
 * \param connection the connection object
 * \param[out] error the error code if an error occurs
 * \param dataSetReference object reference of the data set
 * \param[out] isDeletable this is an output parameter indicating that the requested data set is deletable by clients.
 *                    If this information is not required a NULL pointer can be used.
 *
 * \return LinkedList containing the data set elements as char* strings.
 */
LIB61850_API LinkedList /* <char*> */
IedConnection_getDataSetDirectory(IedConnection self, IedClientError* error, const char* dataSetReference, bool* isDeletable);

/**
 * \brief Write the data set values to the server
 *
 * The parameter dataSetReference is the name of the data set to write. It is either in the form LDName/LNodeName.dataSetName
 * for permanent domain or VMD scope data sets or @dataSetName for an association specific data set.
 * If the LDName part of the reference is missing the resulting data set will be of VMD scope.
 * The values parameter has to be the same number of elements as are members in the data set. The accessResult return parameter
 * contains a value for each data set member.
 *
 * \param connection the connection object
 * \param[out] error the error code if an error occurs
 * \param dataSetReference object reference of the data set
 * \param values the new data set values
 * \param[out] accessResults the access results for each data set member
 */
LIB61850_API void
IedConnection_writeDataSetValues(IedConnection self, IedClientError* error, const char* dataSetReference,
        LinkedList/*<MmsValue*>*/ values, /* OUTPUT */LinkedList* /* <MmsValue*> */accessResults);

/**
 * \brief Callback handler for asynchronous write data set values services (set data set)
 *
 * \param invokeId the invoke ID of the service request
 * \param parameter used provided parameter
 * \param err the error code if an error occurs
 * \param accessResults the list of access results for the data set entries.
 */
typedef void
(*IedConnection_WriteDataSetHandler) (uint32_t invokeId, void* parameter, IedClientError err, LinkedList /* <MmsValue*> */accessResults);

/**
 * \brief Write the data set values to the server - async version
 *
 * The parameter dataSetReference is the name of the data set to write. It is either in the form LDName/LNodeName.dataSetName
 * for permanent domain or VMD scope data sets or @dataSetName for an association specific data set.
 * If the LDName part of the reference is missing the resulting data set will be of VMD scope.
 * The values parameter has to be the same number of elements as are members in the data set.
 *
 * When the service call had been successful the \ref IedConnection_WriteDataSetHandler is called with an error value of
 * IED_ERROR_OK and a list of MmsValue instances of type data access error. These describe the access results of the individual
 * data set entries.
 *
 * \param connection the connection object
 * \param[out] error the error code if an error occurs
 * \param dataSetReference object reference of the data set
 * \param values the new data set values
 * \param handler the user provided callback handler
 * \param parameter user provided parameter that is passed to the callback handler
 *
 * \return the invoke ID of the request
 */
LIB61850_API uint32_t
IedConnection_writeDataSetValuesAsync(IedConnection self, IedClientError* error, const char* dataSetReference,
        LinkedList/*<MmsValue*>*/ values, IedConnection_WriteDataSetHandler handler, void* parameter);


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
LIB61850_API void
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
LIB61850_API MmsValue*
ClientDataSet_getValues(ClientDataSet self);

/**
 * \brief Get the object reference of the data set.
 *
 * \param self the ClientDataSet instance
 *
 * \return the object reference of the data set.
 */
LIB61850_API char*
ClientDataSet_getReference(ClientDataSet self);

/**
 * \brief get the size of the data set (number of members)
 *
 * \param self the ClientDataSet instance
 *
 * \return the number of member contained in the data set.
 */
LIB61850_API int
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

/**
 * \brief Create a new client control object
 *
 * A client control object is used to handle all client side functions of a controllable
 * data object. A controllable data object is an instance of a controllable CDC like e.g.
 * SPC, DPC, APC, ...
 *
 * NOTE: This function will synchronously request information about the control object
 * (like ctlModel) from the server. The function will block until these requests return
 * or time-out.
 *
 * \param objectReference the reference of the controllable data object
 * \param connection the connection instance where the control object has to be reached
 *
 * \return the newly created instance or NULL if the creation failed
 */
LIB61850_API ControlObjectClient
ControlObjectClient_create(const char* objectReference, IedConnection connection);

/**
 * \brief Create a new client control object - doesn't send requests to the server (doesn't block)
 *
 * A client control object is used to handle all client side functions of a controllable
 * data object. A controllable data object is an instance of a controllable CDC like e.g.
 * SPC, DPC, APC, ...
 *
 * \param objectReference the reference of the controllable data object
 * \param connection the connection instance where the control object has to be reached
 * \param ctlModel the control model used by the controllable data object
 * \param controlObjectSpec specification of the controllable data objects - used to derive required information to handle the control object
 */
LIB61850_API ControlObjectClient
ControlObjectClient_createEx(const char* objectReference, IedConnection connection, ControlModel ctlModel, MmsVariableSpecification* controlObjectSpec);

/**
 * \brief Destroy the client control object instance and release all related resources
 *
 * \param self the control object instance to use
 */
LIB61850_API void
ControlObjectClient_destroy(ControlObjectClient self);

/**
 * Cause of the \ref ControlObjectClient_ControlActionHandler invocation
 */
typedef enum
{
    CONTROL_ACTION_TYPE_SELECT = 0, /** < callback was invoked because of a select command */
    CONTROL_ACTION_TYPE_OPERATE = 1,  /** < callback was invoked because of an operate command */
    CONTROL_ACTION_TYPE_CANCEL = 2 /** < callback was invoked because of a cancel command */
} ControlActionType;

/**
 * \brief A callback handler that is invoked when a command termination message is received.
 *
 * This callback is invoked whenever a CommandTermination+ or CommandTermination- message is received.
 * To distinguish between a CommandTermination+ and CommandTermination- please use the
 * ControlObjectClient_getLastApplError function.
 *
 * NOTE: Do not call \ref ControlObjectClient_destroy inside of this callback! Doing so will cause a dead-lock.
 *
 * \param invokeId invoke ID of the command sent by the client
 * \param parameter the user parameter that is passed to the callback function
 * \param err the error code when an error occurred, or IED_ERROR_OK
 * \param type control action type that caused the callback
 * \param success true, when the command was successful, false otherwise
 *
 */
typedef void
(*ControlObjectClient_ControlActionHandler) (uint32_t invokeId, void* parameter, IedClientError err, ControlActionType type, bool success);

/**
 * \brief Get the object reference of the control data object
 *
 * \param self the control object instance to use
 *
 * \return the object reference (string is valid only as long as the \ref ControlObjectClient instance exists).
 */
LIB61850_API const char*
ControlObjectClient_getObjectReference(ControlObjectClient self);

/**
 * \brief Get the current control model (local representation) applied to the control object
 *
 * \param self the control object instance to use
 *
 * \return the current applied control model (\ref ControlModel)
 */
LIB61850_API ControlModel
ControlObjectClient_getControlModel(ControlObjectClient self);

/**
 * \brief Set the applied control model
 *
 * NOTE: This function call will not change the server control model.
 *
 * \param self the control object instance to use
 * \param ctlModel the new control model to apply
 */
LIB61850_API void
ControlObjectClient_setControlModel(ControlObjectClient self, ControlModel ctlModel);

/**
 * \brief Change the control model of the server.
 *
 * NOTE: Not supported by all servers. Information can be found in the PIXIT of the server.
 * Also sets the applied control model for this client control instance.
 *
 * \param self the control object instance to use
 * \param ctlModel the new control model
 */
LIB61850_API void
ControlObjectClient_changeServerControlModel(ControlObjectClient self, ControlModel ctlModel);

/**
 * \brief Get the type of ctlVal.
 *
 * This type is required for the ctlVal parameter of the \ref ControlObjectClient_operate
 * and \ref  ControlObjectClient_selectWithValue functions.
 *
 * \param self the control object instance to use
 *
 * \return MmsType required for the ctlVal value.
 */
LIB61850_API MmsType
ControlObjectClient_getCtlValType(ControlObjectClient self);

/**
 * \brief Get the error code of the last synchronous control action (operate, select, select-with-value, cancel)
 *
 * \param self the control object instance to use
 *
 * \return the client error code
 */
LIB61850_API IedClientError
ControlObjectClient_getLastError(ControlObjectClient self);

/**
 * \brief Send an operate command to the server
 *
 * \param self the control object instance to use
 * \param ctlVal the control value (for APC the value may be either AnalogueValue (MMS_STRUCT) or MMS_FLOAT/MMS_INTEGER
 * \param operTime the time when the command has to be executed (for time activated control). The value represents the local time of the
 *                 server in milliseconds since epoch. If this value is 0 the command will be executed instantly.
 *
 * \return true if operation has been successful, false otherwise.
 */
LIB61850_API bool
ControlObjectClient_operate(ControlObjectClient self, MmsValue* ctlVal, uint64_t operTime);

/**
 * \brief Send a select command to the server
 *
 * The select command is only used for the control model "select-before-operate with normal security"
 * (CONTROL_MODEL_SBO_NORMAL). The select command has to be sent before the operate command can be used.
 *
 * \param self the control object instance to use
 *
 * \return true if operation has been successful, false otherwise.
 */
LIB61850_API bool
ControlObjectClient_select(ControlObjectClient self);

/**
 * \brief Send an select with value command to the server
 *
 * The select-with-value command is only used for the control model "select-before-operate with enhanced security"
 * (CONTROL_MODEL_SBO_ENHANCED). The select-with-value command has to be sent before the operate command can be used.
 *
 * \param self the control object instance to use
 * \param ctlVal the control value (for APC the value may be either AnalogueValue (MMS_STRUCT) or MMS_FLOAT/MMS_INTEGER
 *
 * \return true if select has been successful, false otherwise.
 */
LIB61850_API bool
ControlObjectClient_selectWithValue(ControlObjectClient self, MmsValue* ctlVal);

/**
 * \brief Send a cancel command to the server
 *
 * The cancel command can be used to stop an ongoing operation (when the server and application
 * support this) and to cancel a former select command.
 *
 * \param self the control object instance to use
 *
 * \return true if operation has been successful, false otherwise.
 */
LIB61850_API bool
ControlObjectClient_cancel(ControlObjectClient self);


/**
 * \brief Send an operate command to the server - async version
 *
 * \param self the control object instance to use
 * \param[out] err error code
 * \param ctlVal the control value (for APC the value may be either AnalogueValue (MMS_STRUCT) or MMS_FLOAT/MMS_INTEGER
 * \param operTime the time when the command has to be executed (for time activated control). The value represents the local time of the
 *                 server in milliseconds since epoch. If this value is 0 the command will be executed instantly.
 * \param handler the user provided callback handler
 * \param parameter user provided parameter that is passed to the callback handler
 *
 * \return the invoke ID of the request
 */
LIB61850_API uint32_t
ControlObjectClient_operateAsync(ControlObjectClient self, IedClientError* err, MmsValue* ctlVal, uint64_t operTime,
        ControlObjectClient_ControlActionHandler handler, void* parameter);

/**
 * \brief Send a select command to the server - async version
 *
 * The select command is only used for the control model "select-before-operate with normal security"
 * (CONTROL_MODEL_SBO_NORMAL). The select command has to be sent before the operate command can be used.
 *
 * \param self the control object instance to use
 * \param[out] err error code
 * \param handler the user provided callback handler
 * \param parameter user provided parameter that is passed to the callback handler
 *
 * \return the invoke ID of the request
 */
LIB61850_API uint32_t
ControlObjectClient_selectAsync(ControlObjectClient self, IedClientError* err, ControlObjectClient_ControlActionHandler handler, void* parameter);

/**
 * \brief Send a select-with-value command to the server - async version
 *
 * The select-with-value command is only used for the control model "select-before-operate with enhanced security"
 * (CONTROL_MODEL_SBO_ENHANCED). The select-with-value command has to be sent before the operate command can be used.
 *
 * \param self the control object instance to use
 * \param[out] err error code
 * \param ctlVal the control value (for APC the value may be either AnalogueValue (MMS_STRUCT) or MMS_FLOAT/MMS_INTEGER
 * \param handler the user provided callback handler
 * \param parameter user provided parameter that is passed to the callback handler
 *
 * \return the invoke ID of the request
 */
LIB61850_API uint32_t
ControlObjectClient_selectWithValueAsync(ControlObjectClient self, IedClientError* err, MmsValue* ctlVal,
        ControlObjectClient_ControlActionHandler handler, void* parameter);

/**
 * \brief Send a cancel command to the server - async version
 *
 * The cancel command can be used to stop an ongoing operation (when the server and application
 * support this) and to cancel a former select command.
 *
 * \param self the control object instance to use
 * \param[out] err error code
 * \param handler the user provided callback handler
 * \param parameter user provided parameter that is passed to the callback handler
 *
 * \return the invoke ID of the request
 */
LIB61850_API uint32_t
ControlObjectClient_cancelAsync(ControlObjectClient self, IedClientError* err, ControlObjectClient_ControlActionHandler handler, void* parameter);

/**
 * \brief Get the last received control application error
 *
 * NOTE: this is the content of the "LastApplError" message received from the server.
 *
 * \return the value of the last received application error
 */
LIB61850_API LastApplError
ControlObjectClient_getLastApplError(ControlObjectClient self);

/**
 * \brief Send commands in test mode.
 *
 * When the server supports test mode the commands that are sent with the test flag set
 * are not executed (will have no effect on the attached physical process).
 *
 * \param self the control object instance to use
 * \param value value if the test flag (true = test mode).
 */
LIB61850_API void
ControlObjectClient_setTestMode(ControlObjectClient self, bool value);

/**
 * \brief Set the origin parameter for control commands
 *
 * The origin parameter is used to identify the client/application that sent a control
 * command. It is intended for later analysis.
 *
 * \param self the ControlObjectClient instance
 * \param orIdent originator identification can be an arbitrary string
 * \param orCat originator category (see \ref ORIGINATOR_CATEGORIES)
 */
LIB61850_API void
ControlObjectClient_setOrigin(ControlObjectClient self, const char* orIdent, int orCat);

/**
 * \brief Use a constant T parameter for all command (select, operate, cancel) of a single control sequence
 *
 * NOTE: Some non-standard compliant servers may require this to accept oper/cancel requests
 *
 * \param self the ControlObjectClient instance
 * \param useContantT enable this behavior with true, disable with false
 */
LIB61850_API void
ControlObjectClient_useConstantT(ControlObjectClient self, bool useConstantT);

/**
 * \deprecated use ControlObjectClient_setInterlockCheck instead
 */
LIB61850_API DEPRECATED void
ControlObjectClient_enableInterlockCheck(ControlObjectClient self);

/**
 * \deprecated use ControlObjectClient_setSynchroCheck instead
 */
LIB61850_API DEPRECATED void
ControlObjectClient_enableSynchroCheck(ControlObjectClient self);

/**
 * \deprecated Do not use (ctlNum is handled automatically by the library)! Intended for test purposes only.
 */
LIB61850_API DEPRECATED void
ControlObjectClient_setCtlNum(ControlObjectClient self, uint8_t ctlNum);

/**
 * \brief Set the value of the interlock check flag when a control command is sent
 *
 * \param self the ControlObjectClient instance
 * \param value if true the server will perform a interlock check if supported
 */
LIB61850_API void
ControlObjectClient_setInterlockCheck(ControlObjectClient self, bool value);

/**
 * \brief Set the value of the synchro check flag when a control command is sent
 *
 * \param self the ControlObjectClient instance
 * \param value if true the server will perform a synchro check if supported
 */
LIB61850_API void
ControlObjectClient_setSynchroCheck(ControlObjectClient self, bool value);


/**
 * \brief A callback handler that is invoked when a command termination message is received.
 *
 * This callback is invoked whenever a CommandTermination+ or CommandTermination- message is received.
 * To distinguish between a CommandTermination+ and CommandTermination- please use the
 * \ref ControlObjectClient_getLastApplError function.
 *
 * In case of CommandTermination+ the return value
 * of \ref ControlObjectClient_getLastApplError has error=CONTROL_ERROR_NO_ERROR and
 * addCause=ADD_CAUSE_UNKNOWN set. When addCause is different from ADD_CAUSE_UNKNOWN then the client
 * received a CommandTermination- message.
 *
 * NOTE: Do not call \ref ControlObjectClient_destroy inside of this callback! Doing so will cause a dead-lock.
 *
 * \param parameter the user parameter that is passed to the callback function
 * \param controlClient the ControlObjectClient instance
 */
typedef void (*CommandTerminationHandler) (void* parameter, ControlObjectClient controlClient);

/**
 * \brief Set the command termination callback handler for this control object
 *
 * This callback is invoked whenever a CommandTermination+ or CommandTermination- message is received.
 * To distinguish between a CommandTermination+ and CommandTermination- please use the
 * \ref ControlObjectClient_getLastApplError function. In case of CommandTermination+ the return value
 * of \ref ControlObjectClient_getLastApplError has error=CONTROL_ERROR_NO_ERROR and
 * addCause=ADD_CAUSE_UNKNOWN set. When addCause is different from ADD_CAUSE_UNKNOWN then the client
 * received a CommandTermination- message.
 *
 * \param self the ControlObjectClient instance
 * \param handler the callback function to be used
 * \param handlerParameter a user parameter that is passed to the handler
 */
LIB61850_API void
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
LIB61850_API void
IedConnection_getDeviceModelFromServer(IedConnection self, IedClientError* error);

/**
 * \brief Get the list of logical devices available at the server (DEPRECATED)
 *
 * This function is mapped to the GetServerDirectory(LD) ACSI service.
 *
 * NOTE: This function will call \ref IedConnection_getDeviceModelFromServer if no buffered data model
 * information is available. Otherwise it will use the buffered information.
 *
 * \param self the connection object
 * \param error the error code if an error occurs
 *
 * \return LinkedList with string elements representing the logical device names
 */
LIB61850_API LinkedList /*<char*>*/
IedConnection_getLogicalDeviceList(IedConnection self, IedClientError* error);

/**
 * \brief Get the list of logical devices or files available at the server
 *
 * GetServerDirectory ACSI service implementation. This function will either return the list of
 * logical devices (LD) present at the server or the list of available files.
 *
 * NOTE: When getFIleNames is false zhis function will call
 * \ref IedConnection_getDeviceModelFromServer if no buffered data model
 * information is available. Otherwise it will use the buffered information.
 *
 * \param self the connection object
 * \param error the error code if an error occurs
 * \param getFileNames get list of files instead of logical device names (TO BE IMPLEMENTED)
 *
 * \return LinkedList with string elements representing the logical device names or file names
 */
LIB61850_API LinkedList /*<char*>*/
IedConnection_getServerDirectory(IedConnection self, IedClientError* error, bool getFileNames);

/**
 * \brief Get the list of logical nodes (LN) of a logical device
 *
 * GetLogicalDeviceDirectory ACSI service implementation. Returns the list of logical nodes names present
 * in a logical device. The list is returned as a linked list of type LinkedList with c style string elements.
 *
 * NOTE: This function will call \ref IedConnection_getDeviceModelFromServer if no buffered data model
 * information is available. Otherwise it will use the buffered information.
 *
 * \param self the connection object
 * \param error the error code if an error occurs
 * \param logicalDeviceName the name of the logical device (LD) of interest
 *
 * \return  LinkedList with string elements representing the logical node names
 */
LIB61850_API LinkedList /*<char*>*/
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
 * NOTE: This function will call \ref IedConnection_getDeviceModelFromServer if no buffered data model
 * information is available. Otherwise it will use the buffered information.
 *
 * \param self the connection object
 * \param error the error code if an error occurs
 * \param logicalNodeReference string that represents the LN reference
 *
 * \return the list of all MMS named variables as C strings in a LinkedList type
 */
LIB61850_API LinkedList /*<char*>*/
IedConnection_getLogicalNodeVariables(IedConnection self, IedClientError* error,
		const char* logicalNodeReference);

/**
 * \brief returns the directory of the given logical node (LN) containing elements of the specified ACSI class
 *
 * Implementation of the GetLogicalNodeDirectory ACSI service. In contrast to the ACSI description this
 * function does not always creates a request to the server. For most ACSI classes it simply accesses the
 * data model that was retrieved before or calls \ref IedConnection_getDeviceModelFromServer if no buffered data model
 * information is available. An exception to this rule are the ACSI classes ACSI_CLASS_DATASET and
 * ACSI_CLASS_LOG. Both always perform a request to the server.
 *
 * \param self the connection object
 * \param error the error code if an error occurs
 * \param logicalNodeReference string that represents the LN reference
 * \param acsiClass specifies the ACSI class
 *
 * \return list of all logical node elements of the specified ACSI class type as C strings in a LinkedList
 */
LIB61850_API LinkedList /*<char*>*/
IedConnection_getLogicalNodeDirectory(IedConnection self, IedClientError* error,
		const char* logicalNodeReference, ACSIClass acsiClass);

/**
 * \brief returns the directory of the given data object (DO)
 *
 * Implementation of the GetDataDirectory ACSI service. This will return the list of
 * all data attributes or sub data objects.
 *
 * NOTE: This function will call \ref IedConnection_getDeviceModelFromServer if no buffered data model
 * information is available. Otherwise it will use the buffered information.
 *
 * \param self the connection object
 * \param error the error code if an error occurs
 * \param dataReference string that represents the DO reference
 *
 * \return list of all data attributes or sub data objects as C strings in a LinkedList
 */
LIB61850_API LinkedList /*<char*>*/
IedConnection_getDataDirectory(IedConnection self, IedClientError* error, const char* dataReference);

/**
 * \brief returns the directory of the given data object (DO)
 *
 * Implementation of the GetDataDirectory ACSI service. This will return the list of
 * C strings with all data attributes or sub data objects as elements. The returned
 * strings will contain the functional constraint appended in square brackets when appropriate.
 *
 * NOTE: This function will call \ref IedConnection_getDeviceModelFromServer if no buffered data model
 * information is available. Otherwise it will use the buffered information.
 *
 * \param self the connection object
 * \param error the error code if an error occurs
 * \param dataReference string that represents the DO reference
 *
 * \return list of all data attributes or sub data objects as C strings in a LinkedList
 */
LIB61850_API LinkedList /*<char*>*/
IedConnection_getDataDirectoryFC(IedConnection self, IedClientError* error, const char* dataReference);

/**
 * \brief returns the directory of the given data object/data attribute with the given FC
 *
 * Implementation of the GetDataDirectory ACSI service. This will return the list of
 * C strings with all data attributes or sub data objects as elements.
 *
 * NOTE: This function will call \ref IedConnection_getDeviceModelFromServer if no buffered data model
 * information is available. Otherwise it will use the buffered information.
 *
 * WARNING: Starting with version 1.0.3 the functional constraint will no longer be appended to
 * the name string.
 *
 * \param self the connection object
 * \param error the error code if an error occurs
 * \param dataReference string that represents the DO reference
 * \param fc the functional constraint
 *
 * \return list of all data attributes or sub data objects as C strings in a LinkedList
 */
LIB61850_API LinkedList
IedConnection_getDataDirectoryByFC(IedConnection self, IedClientError* error, const char* dataReference, FunctionalConstraint fc);

/**
 * \brief return the MMS variable type specification of the data attribute referenced by dataAttributeReference and function constraint fc.
 *
 * This function can be used to get the MMS variable type specification for an IEC 61850 data attribute. It is an extension
 * of the ACSI that may be required by generic client applications.
 *
 * NOTE: API user is responsible to free the resources (see \ref MmsVariableSpecification_destroy)
 *
 * \param self the connection object
 * \param error the error code if an error occurs
 * \param dataAttributeReference string that represents the DA reference
 * \param fc functional constraint of the DA
 *
 * \return MmsVariableSpecification of the data attribute.
 */
LIB61850_API MmsVariableSpecification*
IedConnection_getVariableSpecification(IedConnection self, IedClientError* error, const char* dataAttributeReference,
        FunctionalConstraint fc);

/**
 * \brief Get all variables of the logical device
 *
 * NOTE: This function will return all MMS variables of the logical device (MMS domain). The result will be in the
 * MMS notation (like "GGIO1$ST$Ind1$stVal") and also contain the variables of control blocks.
 *
 * \param[in] self the connection object
 * \param[out] error the error code if an error occurs
 * \param[in] ldName the logical device name
 *
 * \return a \ref LinkedList with the MMS variable names as string. Has to be released by the caller
 */
LIB61850_API LinkedList
IedConnection_getLogicalDeviceVariables(IedConnection self, IedClientError* error, const char* ldName);

/**
 * \brief Get the data set names of the logical device
 *
 * NOTE: This function will return all data set names (MMS named variable lists) of the logical device (MMS domain). The result will be in the
 * MMS notation (like "LLN0$dataset1").
 *
 * \param[in] self the connection object
 * \param[out] error the error code if an error occurs
 * \param[in] ldName the logical device name
 *
 * \return a \ref LinkedList with data set names as string. Has to be released by the caller.
 */
LIB61850_API LinkedList
IedConnection_getLogicalDeviceDataSets(IedConnection self, IedClientError* error, const char* ldName);

/*****************************************
 * Asynchronous model discovery functions
 *****************************************/

typedef void
(*IedConnection_GetNameListHandler) (uint32_t invokeId, void* parameter, IedClientError err, LinkedList nameList, bool moreFollows);

/**
 * \brief Get the server directory (logical devices name) - asynchronous version
 *
 * \param[in] self the connection object
 * \param[out] error the error code if an error occurs
 * \param[in] continueAfter the name of the last received element when the call is a continuation, or NULL for the first call
 * \param[in] result list to store (append) the response names, or NULL to create a new list for the response names
 * \param[in] handler will be called when response is received or timed out.
 * \param[in] parameter
 *
 * \return the invoke ID of the request
 */
LIB61850_API uint32_t
IedConnection_getServerDirectoryAsync(IedConnection self, IedClientError* error, const char* continueAfter, LinkedList result,
        IedConnection_GetNameListHandler handler, void* parameter);

/**
 * \brief Get the variables in the logical device - asynchronous version
 *
 * NOTE: This function will return all MMS variables of the logical device (MMS domain). The result will be in the
 * MMS notation (like "GGIO1$ST$Ind1$stVal") and also contain the variables of control blocks.
 *
 * \param[in] self the connection object
 * \param[out] error the error code if an error occurs
 * \param[in] ldName the logical device name
 * \param[in] continueAfter the name of the last received element when the call is a continuation, or NULL for the first call
 * \param[in] result list to store (append) the response names, or NULL to create a new list for the response names
 * \param[in] handler will be called when response is received or timed out.
 * \param[in] parameter
 *
 * \return the invoke ID of the request
 */
LIB61850_API uint32_t
IedConnection_getLogicalDeviceVariablesAsync(IedConnection self, IedClientError* error, const char* ldName, const char* continueAfter, LinkedList result,
        IedConnection_GetNameListHandler handler, void* parameter);

/**
 * \brief Get the data set names in the logical device - asynchronous version
 *
 * NOTE: This function will return all data set names (MMS named variable lists) of the logical device (MMS domain). The result will be in the
 * MMS notation (like "LLN0$dataset1").
 *
 * \param[in] self the connection object
 * \param[out] error the error code if an error occurs
 * \param[in] ldName the logical device name
 * \param[in] continueAfter the name of the last received element when the call is a continuation, or NULL for the first call
 * \param[in] result list to store (append) the response names, or NULL to create a new list for the response names
 * \param[in] handler will be called when response is received or timed out.
 * \param[in] parameter
 *
 * \return the invoke ID of the request
 */
LIB61850_API uint32_t
IedConnection_getLogicalDeviceDataSetsAsync(IedConnection self, IedClientError* error, const char* ldName, const char* continueAfter, LinkedList result,
        IedConnection_GetNameListHandler handler, void* parameter);


typedef void
(*IedConnection_GetVariableSpecificationHandler) (uint32_t invokeId, void* parameter, IedClientError err, MmsVariableSpecification* spec);

/**
 * \brief Get the specification of a variable (data attribute or functional constraint data object) - asynchronous version
 *
 * \param[in] self the connection object
 * \param[out] error the error code if an error occurs
 * \param[in] dataAttributeReference the data attribute reference (FCDA or FCDO)
 * \param[in] handler will be called when response is received or timed out.
 * \param[in] parameter
 *
 * \return the invoke ID of the request
 */
LIB61850_API uint32_t
IedConnection_getVariableSpecificationAsync(IedConnection self, IedClientError* error, const char* dataAttributeReference,
        FunctionalConstraint fc, IedConnection_GetVariableSpecificationHandler handler, void* parameter);

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
LIB61850_API LinkedList /* <MmsJournalEntry> */
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
LIB61850_API LinkedList /* <MmsJournalEntry> */
IedConnection_queryLogAfter(IedConnection self, IedClientError* error, const char* logReference,
        MmsValue* entryID, uint64_t timeStamp, bool* moreFollows);


typedef void
(*IedConnection_QueryLogHandler) (uint32_t invokeId, void* parameter, IedClientError mmsError, LinkedList /* <MmsJournalEntry> */ journalEntries, bool moreFollows);

LIB61850_API uint32_t
IedConnection_queryLogByTimeAsync(IedConnection self, IedClientError* error, const char* logReference,
        uint64_t startTime, uint64_t endTime, IedConnection_QueryLogHandler handler, void* parameter);

LIB61850_API uint32_t
IedConnection_queryLogAfterAsync(IedConnection self, IedClientError* error, const char* logReference,
        MmsValue* entryID, uint64_t timeStamp, IedConnection_QueryLogHandler handler, void* parameter);

/** @} */

/**
 * @defgroup IEC61850_CLIENT_FILE_SERVICE File service related functions, data types, and definitions
 *
 * @{
 */

typedef struct sFileDirectoryEntry* FileDirectoryEntry;

/**
 * @deprecated Will be removed from API
 */
LIB61850_API FileDirectoryEntry
FileDirectoryEntry_create(const char* fileName, uint32_t fileSize, uint64_t lastModified);

/**
 * \brief Destroy a FileDirectoryEntry object (free all resources)
 *
 * NOTE: Usually is called as a parameter of the \ref LinkedList_destroyDeep function.
 *
 * \param self the FileDirectoryEntry object
 */
LIB61850_API void
FileDirectoryEntry_destroy(FileDirectoryEntry self);

/**
 * \brief Get the name of the file
 *
 * \param self the FileDirectoryEntry object
 *
 * \return name of the file as null terminated string
 */
LIB61850_API const char*
FileDirectoryEntry_getFileName(FileDirectoryEntry self);

/**
 * \brief Get the file size in bytes
 *
 * \param self the FileDirectoryEntry object
 *
 * \return size of the file in bytes, or 0 if file size is unknown
 */
LIB61850_API uint32_t
FileDirectoryEntry_getFileSize(FileDirectoryEntry self);

/**
 * \brief Get the timestamp of last modification of the file
 *
 * \param self the FileDirectoryEntry object
 *
 * \return UTC timestamp in milliseconds
 */
LIB61850_API uint64_t
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
LIB61850_API LinkedList /*<FileDirectoryEntry>*/
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
LIB61850_API LinkedList /*<FileDirectoryEntry>*/
IedConnection_getFileDirectoryEx(IedConnection self, IedClientError* error, const char* directoryName, const char* continueAfter,
        bool* moreFollows);

/**
 * \brief Callback handler for the get file directory service
 *
 * Will be called once for each file directory entry and after the last entry with \ref moreFollows = false to indicate
 * to indicate that no more data will follow. In case of an error the callback will be called with
 * \ref err != IED_ERROR_OK and moreFollows = false.
 */

/**
 * \brief Callback handler for the get file directory service
 *
 * Will be called once for each file directory entry and after the last entry with \ref filename = NULL to indicate
 * with \ref moreFollows set to true if more data is available at the server (can only happen when using the \ref IedConnection_getFileDirectoryAsyncEx
 * function). In case of an error the callback will be called with \ref err != IED_ERROR_OK and moreFollows = false.
 *
 * \param invokeId invoke ID of the request
 * \param parameter user provided parameter
 * \param err error code in case of a problem, otherwise IED_ERROR_OK
 * \param filename the filename of the current file directory entry or NULL if no more entries are available
 * \param size the file size in byte of the current file directory entry
 * \param lastModified the last modified timestamp of the current file directory entry
 *
 * \return return false when the request has to be stopped (no further callback invokations), true otherwise
 */
typedef bool
(*IedConnection_FileDirectoryEntryHandler) (uint32_t invokeId, void* parameter, IedClientError err, char* filename, uint32_t size, uint64_t lastModfified,
        bool moreFollows);

/**
 * \brief Get file directory (single request) - asynchronous version
 *
 * The provided handler will be called for each received file directory entry.
 *
 * NOTE: This will only cause a single MMS request. When the resulting file directory doesn't fit into
 * a single MMS PDU another request has to be sent indicating a continuation point with the continueAfter
 * parameter.
 *
 * \param self the connection object
 * \param error the error code if an error occurs
 * \param directoryName the name of the directory or NULL to get the entries of the root directory
 * \param continueAfter last received filename to continue after, or NULL for the first request
 * \param handler the callback handler
 * \param parameter user provided callback parameter
 *
 * \return the invokeId of the first file directory request
 */
LIB61850_API uint32_t
IedConnection_getFileDirectoryAsyncEx(IedConnection self, IedClientError* error, const char* directoryName, const char* continueAfter,
        IedConnection_FileDirectoryEntryHandler handler, void* parameter);

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
LIB61850_API uint32_t
IedConnection_getFile(IedConnection self, IedClientError* error, const char* fileName, IedClientGetFileHandler handler,
        void* handlerParameter);


/**
 * \brief User provided handler to receive the data of the asynchronous GetFile request
 *
 * This handler will be invoked whenever the clients receives a data block from
 * the server. The API user has to copy the data to another location before returning.
 * The other location could for example be a file in the clients file system. When the
 * last data block is received the moreFollows parameter will be set to false.
 *
 * \param invokeId invoke ID of the message containing the received data
 * \param parameter user provided parameter passed to the callback
 * \param err error code in case of an error or IED_ERROR_OK
 * \param originalInvokeId the invoke ID of the original (first) request. This is usually the request to open the file.
 * \param buffer the buffer that contains the received file data
 * \param bytesRead the number of bytes read into the buffer
 * \param moreFollows indicates that more file data is following
 */
typedef bool
(*IedConnection_GetFileAsyncHandler) (uint32_t invokeId, void* parameter, IedClientError err, uint32_t originalInvokeId,
        uint8_t* buffer, uint32_t bytesRead, bool moreFollows);


/**
 * \brief Implementation of the GetFile ACSI service - asynchronous version
 *
 * Download a file from the server.
 *
 * NOTE: This function can cause several request messages to be sent until the complete file is received
 * or the file transfer is canceled. It allocates a background task and an outstanding call slot.
 *
 * \param self the connection object
 * \param error the error code if an error occurs
 * \param fileName the name of the file to be read from the server
 * \param hander callback handler that is called for each received data or error message
 * \param parameter user provided callback parameter
 *
 * \return invokeId of the first sent request
 */
LIB61850_API uint32_t
IedConnection_getFileAsync(IedConnection self, IedClientError* error, const char* fileName, IedConnection_GetFileAsyncHandler handler,
        void* parameter);

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
LIB61850_API void
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
LIB61850_API void
IedConnection_setFile(IedConnection self, IedClientError* error, const char* sourceFilename, const char* destinationFilename);

/**
 * \brief Implementation of the SetFile ACSI service - asynchronous version
 *
 * Upload a file to the server. The file has to be available in the local VMD filestore.
 *
 * \param self the connection object
 * \param error the error code if an error occurs
 * \param sourceFilename the filename of the local (client side) file
 * \param destinationFilename the filename of the remote (service side) file
 * \param handler callback handler that is called when the obtain file response has been received
 * \param parameter user provided callback parameter
 */
LIB61850_API uint32_t
IedConnection_setFileAsync(IedConnection self, IedClientError* error, const char* sourceFilename, const char* destinationFilename,
        IedConnection_GenericServiceHandler handler, void* parameter);

/**
 * \brief Implementation of the DeleteFile ACSI service
 *
 * Delete a file at the server.
 *
 * \param self the connection object
 * \param error the error code if an error occurs
 * \param fileName the name of the file to delete
 */
LIB61850_API void
IedConnection_deleteFile(IedConnection self, IedClientError* error, const char* fileName);

/**
 * \brief Implementation of the DeleteFile ACSI service - asynchronous version
 *
 * Delete a file at the server.
 *
 * \param self the connection object
 * \param error the error code if an error occurs
 * \param fileName the name of the file to delete
 * \param handler callback handler that is called when the obtain file response has been received
 * \param parameter user provided callback parameter
 */
LIB61850_API uint32_t
IedConnection_deleteFileAsync(IedConnection self, IedClientError* error, const char* fileName,
        IedConnection_GenericServiceHandler handler, void* parameter);


/** @} */

/**@}*/

#ifdef __cplusplus
}
#endif


#endif /* IEC61850_CLIENT_H_ */
