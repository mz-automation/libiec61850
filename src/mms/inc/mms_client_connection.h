/*
 *  mms_client_connection.h
 *
 *  Copyright 2013-2018 Michael Zillgith
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

#ifndef MMS_CLIENT_CONNECTION_H_
#define MMS_CLIENT_CONNECTION_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup mms_client_api_group MMS client API (for IEC 61850 use IEC 61850 client API instead!)
 */
/**@{*/

#include "libiec61850_common_api.h"

#include "mms_common.h"
#include "mms_type_spec.h"
#include "mms_value.h"
#include "iso_connection_parameters.h"
#include "linked_list.h"
#include "tls_config.h"

/**
 * Contains MMS layer specific parameters
 */
typedef struct sMmsConnectionParameters {
	int maxServOutstandingCalling;
	int maxServOutstandingCalled;
	int dataStructureNestingLevel;
	int maxPduSize; /* local detail */
	uint8_t servicesSupported[11];
} MmsConnectionParameters;

typedef struct {
    char* vendorName;
    char* modelName;
    char* revision;
} MmsServerIdentity;

typedef enum {
    MMS_CONNECTION_STATE_CLOSED,
    MMS_CONNECTION_STATE_CONNECTING,
    MMS_CONNECTION_STATE_CONNECTED,
    MMS_CONNECTION_STATE_CLOSING
} MmsConnectionState;

typedef void (*MmsInformationReportHandler) (void* parameter, char* domainName,
        char* variableListName, MmsValue* value, bool isVariableListName);

/**
 * Opaque handle for MMS client connection instance.
 */
typedef struct sMmsConnection* MmsConnection;


/*******************************************************************************
 * Connection management functions
 *******************************************************************************/

/**
 * \brief Create a new MmsConnection instance
 *
 * \return the newly created instance.
 */
LIB61850_API MmsConnection
MmsConnection_create(void);

/**
 * \brief Create a new secure (TLS enabled) MmsConnection instance
 *
 * \param tlsConfig TLS configuration parameters and certificates
 *
 * \return the newly created instance.
 */
LIB61850_API MmsConnection
MmsConnection_createSecure(TLSConfiguration tlsConfig);

/**
 * \brief Create a new MmsConnection instance configured for non-threaded mode
 *
 * NOTE: This constructor doesn't create a background thread for connection handling.
 * The user has to call the MmsConnection_tick function periodically to ensure that
 * the MMS connection can be handled properly.
 *
 * \param tlsConfig TLS configuration parameters and certificates or NULL for non-TLS mode.
 *
 * \return  the newly created instance.
 */
LIB61850_API MmsConnection
MmsConnection_createNonThreaded(TLSConfiguration tlsConfig);

/**
 * \brief Callback function to intercept raw MMS messages
 *
 * IMPORTANT: the message buffer is only valid in the context of the the callback function. If the
 * message data is required elsewhere it has to be copied here!
 *
 * \param parameter user provided parameter that is passed to the callback function
 * \param message buffer of the message.
 * \param messageLength length of the message in bytes
 * \param received if true message has been received, false when message has been sent.
 */
typedef void (*MmsRawMessageHandler) (void* parameter, uint8_t* message, int messageLength, bool received);

/**
 * \brief Set the callback handler to intercept the raw MMS messages of the connection
 *
 * This function can be used to log raw MMS messages. It may be useful for debugging purposes
 * or advanced test tools. This function will only work when the flag CONFIG_MMS_RAW_MESSAGE_LOGGING
 * it set in stack_config.h
 *
 * \param self MmsConnection instance to operate on
 * \param handler the connection specific callback function
 * \param a user provided parameter passed to the callback function (use NULL if not required).
 */
LIB61850_API void
MmsConnection_setRawMessageHandler(MmsConnection self, MmsRawMessageHandler handler, void* parameter);

/**
 * \brief Set the virtual filestore basepath for the MMS obtain file services
 *
 * All external file service accesses will be mapped to paths relative to the base directory.
 * NOTE: This function is only available when the CONFIG_SET_FILESTORE_BASEPATH_AT_RUNTIME
 * option in stack_config.h is set.
 *
 * \param self the MmsServer instance
 * \param basepath the new virtual filestore basepath
 */
LIB61850_API void
MmsConnection_setFilestoreBasepath(MmsConnection self, const char* basepath);

/**
 * \brief Set the request timeout in ms for this connection
 *
 * \param self MmsConnection instance to operate on
 * \param timeoutInMs request timeout in milliseconds
 */
LIB61850_API void
MmsConnection_setRequestTimeout(MmsConnection self, uint32_t timeoutInMs);

/**
 * \brief Set the maximum number outstanding calls allowed for this connection
 *
 * \param self MmsConnection instance to operate on
 * \param calling the maximum outstanding calls allowed by the caller (client)
 * \param called the maximum outstanding calls allowed by the called endpoint (server)
 */
LIB61850_API void
MmsConnnection_setMaxOutstandingCalls(MmsConnection self, int calling, int called);

/**
 * \brief Get the request timeout in ms for this connection
 *
 * \param self MmsConnection instance to operate on
 *
 * \return request timeout in milliseconds
 */
LIB61850_API uint32_t
MmsConnection_getRequestTimeout(MmsConnection self);

/**
 * \brief Set the connect timeout in ms for this connection instance
 *
 * \param self MmsConnection instance to operate on
 * \param timeoutInMs connect timeout in milliseconds
 */
LIB61850_API void
MmsConnection_setConnectTimeout(MmsConnection self, uint32_t timeoutInMs);

/**
 * \brief Install a handler function for MMS information reports (unsolicited messages from the server).
 *
 * The handler function will be called whenever the client receives an MMS information report message.
 * Note that the API user is responsible to properly free the passed MmsValue object.
 *
 * \param self MmsConnection instance to operate on
 * \param handler the handler function to install for this client connection
 * \param parameter a user specified parameter that will be passed to the handler function on each
 *        invocation.
 */
LIB61850_API void
MmsConnection_setInformationReportHandler(MmsConnection self, MmsInformationReportHandler handler,
        void* parameter);

/**
 * \brief Get the ISO connection parameters for an MmsConnection instance
 *
 * \param self MmsConnection instance to operate on
 * \return params the to be used by this connection
 */
LIB61850_API IsoConnectionParameters
MmsConnection_getIsoConnectionParameters(MmsConnection self);

/**
 * \brief Get the MMS specific connection parameters for an MmsConnection instance
 *
 * \param self MmsConnection instance to operate on
 * \return params the to be used by this connection
 */
LIB61850_API MmsConnectionParameters
MmsConnection_getMmsConnectionParameters(MmsConnection self);

typedef void (*MmsConnectionStateChangedHandler) (MmsConnection connection, void* parameter, MmsConnectionState newState);

LIB61850_API void
MmsConnection_setConnectionStateChangedHandler(MmsConnection self, MmsConnectionStateChangedHandler handler, void* parameter);

/**
 * \brief User provided handler function that will be called if the connection to the server is lost
 *
 * \param connection MmsConnection object of the lost connection
 * \param parameter user provided parameter.
 */
typedef void (*MmsConnectionLostHandler) (MmsConnection connection, void* parameter);

/**
 * \brief Install a callback function that will be called by the client stack if the MMS connection to the server is lost
 *
 * \param handler the user provided callback function
 * \param handlerParameter a parameter that will be passed to the callback function. Can be set to NULL if not required.
 */
LIB61850_API void
MmsConnection_setConnectionLostHandler(MmsConnection self, MmsConnectionLostHandler handler, void* handlerParameter);

/**
 * \brief Set the ISO connection parameters for a MmsConnection instance
 *
 * \param self MmsConnection instance to operate on
 * \param params the ISO client parameters to use
 */
LIB61850_API void
MmsConnection_setIsoConnectionParameters(MmsConnection self, IsoConnectionParameters* params);

/**
 * \brief Destroy an MmsConnection instance and release all resources
 *
 * \param self MmsConnection instance to operate on
 */
LIB61850_API void
MmsConnection_destroy(MmsConnection self);

/*******************************************************************************
 * Blocking functions for connection establishment and data access
 *******************************************************************************/


/**
 * \brief Connect to an MMS server.
 *
 * This will open a new TCP connection and send a MMS initiate request.
 *
 * \param self MmsConnection instance to operate on
 * \param mmsError user provided variable to store error code
 * \param serverName hostname or IP address of the server to connect
 * \param serverPort TCP port number of the server to connect or -1 to use default port (102 for MMS or 3872 for MMS over TLS)
 *
 * \return true on success. false if the connection attempt failed.
 */
LIB61850_API bool
MmsConnection_connect(MmsConnection self, MmsError* mmsError, const char* serverName, int serverPort);



LIB61850_API void
MmsConnection_connectAsync(MmsConnection self, MmsError* mmsError, const char* serverName, int serverPort);

/**
 * \brief Call MmsConnection state machine and connection handling code (for non-threaded mode only)
 *
 * This function has to be called periodically by the user application in non-threaded mode.
 *
 * \return true when connection is currently waiting and calling thread can be suspended, false means
 *         connection is busy and the tick function should be called again as soon as possible.
 */
LIB61850_API bool
MmsConnection_tick(MmsConnection self);

/* NOTE: This function is for test purposes! */
LIB61850_API void
MmsConnection_sendRawData(MmsConnection self, MmsError* mmsError, uint8_t* buffer, int bufSize);

/**
 * \brief Close the connection - not recommended
 *
 * This service simply closes the TCP socket without any hand-shaking with the server.
 * This behavior is not specified. Use with care!
 *
 * \param self MmsConnection instance to operate on
 */
LIB61850_API void
MmsConnection_close(MmsConnection self);

typedef void
(*MmsConnection_ConcludeAbortHandler) (void* parameter, MmsError mmsError, bool success);

/**
 * \brief Uses the MMS/ACSE abort service to close the connection to the server
 *
 * This service should be used to abruptly interrupt the connection to the server. It is not quite clear what the
 * benefit of this service is (simply closing the TCP connection should do the same). Though it is required by
 * conformance tests. In case the server doesn't close the connection after the internal timeout interval the
 * client will close the TCP connection and set mmsError to MMS_ERROR_SERVICE_TIMEOUT.
 *
 * \param self MmsConnection instance to operate on
 * \param mmsError user provided variable to store error code
 */
LIB61850_API void
MmsConnection_abort(MmsConnection self, MmsError* mmsError);

LIB61850_API void
MmsConnection_abortAsync(MmsConnection self, MmsError* mmsError);

/**
 * \brief Uses the MMS conclude service to close the connection to the server
 *
 * This should be used to orderly release the connection to the server. If the server denies the conclude
 * request (by sending a concludeError PDU) this service fails with an error (mmsError set accordingly) and
 * the connection remains open. In this case the close or abort methods have to be used to close the connection.
 * It is not quite clear if this service is really useful but it is required by conformance tests.
 *
 * \param self MmsConnection instance to operate on
 * \param mmsError user provided variable to store error code
 */
LIB61850_API void
MmsConnection_conclude(MmsConnection self, MmsError* mmsError);

LIB61850_API void
MmsConnection_concludeAsync(MmsConnection self, MmsError* mmsError, MmsConnection_ConcludeAbortHandler handler, void* parameter);

typedef void
(*MmsConnection_GenericServiceHandler) (uint32_t invokeId, void* parameter, MmsError mmsError, bool success);

typedef void
(*MmsConnection_GetNameListHandler) (uint32_t invokeId, void* parameter, MmsError mmsError, LinkedList nameList, bool moreFollows);

/**
 * \brief Get the names of all VMD scope variables of the server.
 *
 * This will result in a VMD specific GetNameList request.
 *
 * \param self MmsConnection instance to operate on
 * \param mmsError user provided variable to store error code
 *
 * \return the of VMD specific variable names or NULL if the request failed.
 */
LIB61850_API LinkedList /* <char*> */
MmsConnection_getVMDVariableNames(MmsConnection self, MmsError* mmsError);

LIB61850_API void
MmsConnection_getVMDVariableNamesAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError, const char* continueAfter,
        MmsConnection_GetNameListHandler handler, void* parameter);

/**
 * \brief Get the domains names for all domains of the server.
 *
 * This will result in a VMD specific GetNameList request.
 *
 * \param self MmsConnection instance to operate on
 * \param mmsError user provided variaextern "C" {ble to store error code
 *
 * \return the list of domain names or NULL if the request failed.
 *
 */
LIB61850_API LinkedList /* <char*> */
MmsConnection_getDomainNames(MmsConnection self, MmsError* mmsError);

/**
 * \brief Get the domain names of the server (asynchronous version).
 *
 * \param[in] self MmsConnection instance to operate on
 * \param[out] usedInvokeId the invoke ID of the request
 * \param[out] mmsError user provided variable to store error code
 * \param[in] continueAfter the name of the last received element when the call is a continuation, or NULL for the first call
 * \param[in] result list to store (append) the response names, or NULL to create a new list for the response names
 * \param[in] handler will be called when response is received or timed out.
 * \param[in] parameter
 */
LIB61850_API void
MmsConnection_getDomainNamesAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError, const char* continueAfter, LinkedList result,
        MmsConnection_GetNameListHandler handler, void* parameter);

/**
 * \brief Get the names of all variables present in a MMS domain of the server.
 *
 * This will result in a domain specific GetNameList request.
 *
 * \param[in] self MmsConnection instance to operate on
 * \param[out] mmsError user provided variable to store error code
 * \param[in] domainId the domain name for the domain specific request
 *
 * \return the of domain specific variable names or NULL if the request failed.
 */
LIB61850_API LinkedList /* <char*> */
MmsConnection_getDomainVariableNames(MmsConnection self, MmsError* mmsError, const char* domainId);

/**
 * \brief Get the names of all variables present in a MMS domain of the server (asynchronous version).
 *
 * This will result in a domain specific GetNameList request.
 *
 * \param[in] self MmsConnection instance to operate on
 * \param[out] usedInvokeId the invoke ID of the request
 * \param[out] mmsError user provided variable to store error code
 * \param[in] domainId the domain name for the domain specific request
 * \param[in] continueAfter the name of the last received element when the call is a continuation, or NULL for the first call
 * \param[in] result list to store (append) the response names, or NULL to create a new list for the response names
 * \param[in] handler will be called when response is received or timed out.
 * \param[in] parameter
 */
LIB61850_API void
MmsConnection_getDomainVariableNamesAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError, const char* domainId,
        const char* continueAfter, LinkedList result, MmsConnection_GetNameListHandler handler, void* parameter);

/**
 * \brief Get the names of all named variable lists present in a MMS domain or VMD scope of the server.
 *
 * This will result in a domain specific GetNameList request.
 *
 * \param self MmsConnection instance to operate on
 * \param mmsError user provided variable to store error code
 * \param domainId the domain name for the domain specific request or NULL for a VMD scope request
 *
 * \return the domain specific named variable list names or NULL if the request failed.
 */
LIB61850_API LinkedList /* <char*> */
MmsConnection_getDomainVariableListNames(MmsConnection self, MmsError* mmsError, const char* domainId);

LIB61850_API void
MmsConnection_getDomainVariableListNamesAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError, const char* domainId,
        const char* continueAfter, LinkedList result, MmsConnection_GetNameListHandler handler, void* parameter);

/**
 * \brief Get the names of all journals present in a MMS domain of the server
 *
 * This will result in a domain specific GetNameList request.
 *
 * \param self MmsConnection instance to operate on
 * \param mmsError user provided variable to store error code
 * \param domainId the domain name for the domain specific request
 *
 * \return the domain specific journal names or NULL if the request failed.
 */
LIB61850_API LinkedList /* <char*> */
MmsConnection_getDomainJournals(MmsConnection self, MmsError* mmsError, const char* domainId);

LIB61850_API void
MmsConnection_getDomainJournalsAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError, const char* domainId,
        const char* continueAfter, MmsConnection_GetNameListHandler handler, void* parameter);

/**
 * \brief Get the names of all named variable lists associated with this client connection.
 *
 * This will result in an association specific GetNameList request.
 *
 * \param self MmsConnection instance to operate on
 * \param mmsError user provided variable to store error code
 *
 * \return the association specific named variable list names or NULL if the request failed.
 */
LIB61850_API LinkedList /* <char*> */
MmsConnection_getVariableListNamesAssociationSpecific(MmsConnection self, MmsError* mmsError);

LIB61850_API void
MmsConnection_getVariableListNamesAssociationSpecificAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError,
        const char* continueAfter, MmsConnection_GetNameListHandler handler, void* parameter);


/**
 * \brief Read a single variable from the server.
 *
 * \param self MmsConnection instance to operate on
 * \param mmsError user provided variable to store error code
 * \param domainId the domain name of the variable to be read or NULL to read a VMD specific named variable
 * \param itemId name of the variable to be read
 *
 * \return Returns a MmsValue object or NULL if the request failed. The MmsValue object can
 * either be a simple value or a complex value or array. It is also possible that the return value is NULL
 * even if mmsError = MMS_ERROR_NON. This is the case when the servers returns an empty result list.
 */
LIB61850_API MmsValue*
MmsConnection_readVariable(MmsConnection self, MmsError* mmsError, const char* domainId, const char* itemId);


typedef void
(*MmsConnection_ReadVariableHandler) (uint32_t invokeId, void* parameter, MmsError mmsError, MmsValue* value);

/**
 * \brief Read a single variable from the server (asynchronous version)
 *
 * \param{in] self MmsConnection instance to operate on
 * \param[out] usedInvokeId the invoke ID of the request
 * \param[out] mmsError user provided variable to store error code
 * \param[in] domainId the domain name of the variable to be read or NULL to read a VMD specific named variable
 * \param[in] itemId name of the variable to be read
 */
LIB61850_API void
MmsConnection_readVariableAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError, const char* domainId, const char* itemId,
        MmsConnection_ReadVariableHandler handler, void* parameter);

/**
 * \brief Read a component of a single variable from the server.
 *
 * \param self MmsConnection instance to operate on
 * \param mmsError user provided variable to store error code
 * \param domainId the domain name of the variable to be read or NULL to read a VMD specific named variable
 * \param itemId name of the variable to be read
 * \param componentId the component name
 *
 * \return Returns a MmsValue object or NULL if the request failed. The MmsValue object can
 * either be a simple value or a complex value or array. It is also possible that the return value is NULL
 * even if mmsError = MMS_ERROR_NON. This is the case when the servers returns an empty result list.
 */
LIB61850_API MmsValue*
MmsConnection_readVariableComponent(MmsConnection self, MmsError* mmsError,
        const char* domainId, const char* itemId, const char* componentId);

/**
 * \brief Read a component of a single variable from the server (asynchronous version)
 *
 * \param[in] self MmsConnection instance to operate on
 * \param[out] usedInvokeId the invoke ID of the request
 * \param[out] mmsError user provided variable to store error code
 * \param[in] domainId the domain name of the variable to be read or NULL to read a VMD specific named variable
 * \param[in] itemId name of the variable to be read
 * \param[in] componentId the component name
 * \param[in] handler
 * \param[in] parameter
 */
LIB61850_API void
MmsConnection_readVariableComponentAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError,
        const char* domainId, const char* itemId, const char* componentId,
        MmsConnection_ReadVariableHandler handler, void* parameter);

/**
 * \brief Read one or more elements of a single array variable from the server.
 *
 * \param self MmsConnection instance to operate on
 * \param mmsError user provided variable to store error code
 * \param domainId the domain name of the variable to be read
 * \param itemId name of the variable to be read
 * \param startIndex index of element to read or start index if a element range is to be read
 * \param numberOfElements Number of elements to read or 0 if a single element is to be read
 *
 * \return Returns a MmsValue object or NULL if the request failed. The MmsValue object is either
 * a simple or complex type if numberOfElements is 0, or an array containing the selected
 * array elements of numberOfElements > 0.
 */
LIB61850_API MmsValue*
MmsConnection_readArrayElements(MmsConnection self, MmsError* mmsError, const char* domainId, const char* itemId,
		uint32_t startIndex, uint32_t numberOfElements);

/**
 * \brief Read one or more elements of a single array variable from the server (asynchronous version)
 *
 * NOTE: The MmsValue object received by the callback function is either a simple or complex type if numberOfElements is 0, or an array
 * containing the selected array elements of numberOfElements > 0.
 *
 * \param[in] self MmsConnection instance to operate on
 * \param[out] usedInvokeId the invoke ID of the request
 * \param[out] mmsError user provided variable to store error code
 * \param[in] domainId the domain name of the variable to be read
 * \param[in] itemId name of the variable to be read
 * \param[in] startIndex index of element to read or start index if a element range is to be read
 * \param[in] numberOfElements Number of elements to read or 0 if a single element is to be read
 */
LIB61850_API void
MmsConnection_readArrayElementsAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError, const char* domainId, const char* itemId,
        uint32_t startIndex, uint32_t numberOfElements,
        MmsConnection_ReadVariableHandler handler, void* parameter);


/**
 * \brief Read a single element (with optional component specification) from the server
 *
 * \param self MmsConnection instance to operate on
 * \param mmsError user provided variable to store error code
 * \param domainId the domain name of the variable to be read
 * \param itemId name of the variable to be read
 * \param index array element index
 * \param componentId array element component name
 *
 * \return Returns a MmsValue object or NULL if the request failed.
 */
LIB61850_API MmsValue*
MmsConnection_readSingleArrayElementWithComponent(MmsConnection self, MmsError* mmsError,
        const char* domainId, const char* itemId, uint32_t index, const char* componentId);

LIB61850_API void
MmsConnection_readSingleArrayElementWithComponentAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError,
        const char* domainId, const char* itemId,
        uint32_t index, const char* componentId,
        MmsConnection_ReadVariableHandler handler, void* parameter);

/**
 * \brief Read multiple variables of a domain from the server with one request message.
 *
 * \param self MmsConnection instance to operate on
 * \param mmsError user provided variable to store error code
 * \param domainId the domain name of the requested variables.
 * \param items: LinkedList<char*> is the list of item IDs of the requested variables.
 *
 * \return  Returns a MmsValue object or NULL if the request failed. The MmsValue object is
 * is of type MMS_ARRAY and contains the variable values of simple or complex type
 * in the order as they appeared in the item ID list.
 */
LIB61850_API MmsValue*
MmsConnection_readMultipleVariables(MmsConnection self, MmsError* mmsError, const char* domainId,
		LinkedList /*<char*>*/ items);

LIB61850_API void
MmsConnection_readMultipleVariablesAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError,
        const char* domainId, LinkedList /*<char*>*/items,
        MmsConnection_ReadVariableHandler handler, void* parameter);

/**
 * \brief Write a single variable to the server.
 *
 * NOTE: added return value in version 1.1
 *
 * \param self MmsConnection instance to operate on
 * \param mmsError user provided variable to store error code
 * \param domainId the domain name of the variable to be written
 * \param itemId name of the variable to be written
 * \param value value of the variable to be written
 *
 * \return when successful, the data access error value returned by the server
 */
LIB61850_API MmsDataAccessError
MmsConnection_writeVariable(MmsConnection self, MmsError* mmsError,
        const char* domainId, const char* itemId, MmsValue* value);

typedef void
(*MmsConnection_WriteVariableHandler) (uint32_t invokeId, void* parameter, MmsError mmsError, MmsDataAccessError accessError);

LIB61850_API void
MmsConnection_writeVariableAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError,
        const char* domainId, const char* itemId, MmsValue* value,
        MmsConnection_WriteVariableHandler handler, void* parameter);


/**
 * \brief Write a single variable to the server (using component alternate access)
 *
 * \param self MmsConnection instance to operate on
 * \param mmsError user provided variable to store error code
 * \param domainId the domain name of the variable to be written
 * \param itemId name of the variable to be written
 * \param componentId the name of the variable component
 * \param value value of the variable to be written
 *
 * \return when successful, the data access error value returned by the server
 */
LIB61850_API MmsDataAccessError
MmsConnection_writeVariableComponent(MmsConnection self, MmsError* mmsError,
        const char* domainId, const char* itemId,
        const char* componentId, MmsValue* value);

/**
 * \brief Write a single array element with a component to an array type variable
 *
 * \param self MmsConnection instance to operate on
 * \param mmsError user provided variable to store error code
 * \param domainId the domain name of the variable to be written
 * \param itemId name of the variable to be written
 * \param arrayIndex the index of the array element.
 * \param componentId the name of the component of the array element
 * \param value value of the array element component to be written.
 *
 * \return when successful, the data access error value returned by the server
 */
LIB61850_API MmsDataAccessError
MmsConnection_writeSingleArrayElementWithComponent(MmsConnection self, MmsError* mmsError,
        const char* domainId, const char* itemId,
        uint32_t arrayIndex, const char* componentId, MmsValue* value);

LIB61850_API void
MmsConnection_writeSingleArrayElementWithComponentAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError,
        const char* domainId, const char* itemId,
        uint32_t arrayIndex, const char* componentId, MmsValue* value,
        MmsConnection_WriteVariableHandler handler, void* parameter);

LIB61850_API void
MmsConnection_writeVariableComponentAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError,
        const char* domainId, const char* itemId, const char* componentId, MmsValue* value,
        MmsConnection_WriteVariableHandler handler, void* parameter);

/**
 * \brief Write a single array element or a sub array to an array type variable
 *
 *  When a single array element is addressed the MmsValue object value has to be of the type
 *  of the array elements. When multiple array elements have to be written (index range) the
 *  MmsValue object value has to be of type MMS_ARRAY containing "numberOfElements" elements.
 *
 * \param self MmsConnection instance to operate on
 * \param mmsError user provided variable to store error code
 * \param domainId the domain name of the variable to be written
 * \param index the index of the array element or the start index of a index range
 * \param numberOfElements the number of array elements to write starting with index. If 0 only one array element is written.
 * \param itemId name of the variable to be written
 * \param value value of the array element(s) to be written. Has to be of the type of
 *        the array elements or of type MMS_ARRAY when it is a sub array (index range)
 *
 * \return when successful, the data access error value returned by the server
 */
LIB61850_API MmsDataAccessError
MmsConnection_writeArrayElements(MmsConnection self, MmsError* mmsError,
        const char* domainId, const char* itemId, int index, int numberOfElements,
        MmsValue* value);

LIB61850_API void
MmsConnection_writeArrayElementsAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError,
        const char* domainId, const char* itemId, int index, int numberOfElements,
        MmsValue* value,
        MmsConnection_WriteVariableHandler handler, void* parameter);


typedef void
(*MmsConnection_WriteMultipleVariablesHandler) (uint32_t invokeId, void* parameter, MmsError mmsError, LinkedList /* <MmsValue*> */ accessResults);


/**
 * \brief Write multiple variables to the server.
 *
 * This function will write multiple variables to the server.
 *
 * The parameter accessResults is a pointer to a LinkedList reference. The methods will create a new LinkedList
 * object that contains the AccessResults of the single variable write attempts. It is up to the user to free this
 * objects properly (e.g. with LinkedList_destroyDeep(accessResults, MmsValue_delete)).
 *
 * \param[in] self MmsConnection instance to operate on
 * \param[out] mmsError user provided variable to store error code
 * \param[in] domainId the common domain name of all variables to be written
 * \param[in] items a linked list containing the names of the variables to be written. The names are C strings.
 * \param[out] values values of the variables to be written
 * \param[out] the MmsValue objects of type MMS_DATA_ACCESS_ERROR representing the write success of a single variable
 *        write.
 */
LIB61850_API void
MmsConnection_writeMultipleVariables(MmsConnection self, MmsError* mmsError, const char* domainId,
        LinkedList /*<char*>*/ items, LinkedList /* <MmsValue*> */ values,
        LinkedList* /* <MmsValue*> */ accessResults);

LIB61850_API void
MmsConnection_writeMultipleVariablesAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError, const char* domainId,
        LinkedList /*<char*>*/ items, LinkedList /* <MmsValue*> */ values,
        MmsConnection_WriteMultipleVariablesHandler handler, void* parameter);

/**
 * \brief Write named variable list values to the server.
 *
 * The parameter accessResults is a pointer to a LinkedList reference. The methods will create a new LinkedList
 * object that contains the AccessResults of the single variable write attempts. It is in the responsibility of
 * the user to free this objects properly (e.g. with LinkedList_destroyDeep(accessResults, MmsValue_delete)).
 * If accessResult is the to NULL the result will not be stored.
 *
 * \param[in] self MmsConnection instance to operate on
 * \param[out] mmsError user provided variable to store error code
 * \param[in] isAssociationSpecifc true if the named variable list is an association specific named variable list
 * \param[in] domainId the common domain name of all variables to be written
 * \param[out] values values of the variables to be written
 * \param[out] the MmsValue objects of type MMS_DATA_ACCESS_ERROR representing the write success of a single variable
 *        write.
 */
LIB61850_API void
MmsConnection_writeNamedVariableList(MmsConnection self, MmsError* mmsError, bool isAssociationSpecific,
        const char* domainId, const char* itemId, LinkedList /* <MmsValue*> */values,
        LinkedList* /* <MmsValue*> */accessResults);


LIB61850_API void
MmsConnection_writeNamedVariableListAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError, bool isAssociationSpecific,
        const char* domainId, const char* itemId, LinkedList /* <MmsValue*> */values,
        MmsConnection_WriteMultipleVariablesHandler handler, void* parameter);

/**
 * \brief Get the variable access attributes of a MMS named variable of the server
 *
 * \param self MmsConnection instance to operate on
 * \param mmsError user provided variable to store error code
 * \param domainId the domain name of the variable or NULL for a VMD specific request
 * \param itemId name of the variable
 *
 * \return Returns a MmsTypeSpecification object or NULL if the request failed.
 */
LIB61850_API MmsVariableSpecification*
MmsConnection_getVariableAccessAttributes(MmsConnection self, MmsError* mmsError,
        const char* domainId, const char* itemId);

typedef void
(*MmsConnection_GetVariableAccessAttributesHandler) (uint32_t invokeId, void* parameter, MmsError mmsError, MmsVariableSpecification* spec);


LIB61850_API void
MmsConnection_getVariableAccessAttributesAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError,
        const char* domainId, const char* itemId,
        MmsConnection_GetVariableAccessAttributesHandler, void* parameter);

/**
 * \brief Read the values of a domain specific named variable list
 *
 * The resulting named variable list will either be of domain scope (when the domainId argument
 * is present) or VMD scope when the domainId argument is NULL.
 *
 * \param self MmsConnection instance to operate on
 * \param mmsError user provided variable to store error code
 * \param domainId the domain name of the requested variables.
 * \param listName the name of the named variable list
 * \param specWithResult if specWithResult is set to true, a IEC 61850 compliant request will be sent.
 *
 * \return Returns a MmsValue object or NULL if the request failed. The MmsValue object is
 * is of type MMS_ARRAY and contains the variable values of simple or complex type
 * in the order as they appeared in named variable list definition.
 */
LIB61850_API MmsValue*
MmsConnection_readNamedVariableListValues(MmsConnection self, MmsError* mmsError, const char* domainId,
        const char* listName, bool specWithResult);

LIB61850_API void
MmsConnection_readNamedVariableListValuesAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError,
        const char* domainId, const char* listName, bool specWithResult,
        MmsConnection_ReadVariableHandler handler, void* parameter);


/**
 * \brief Read the values of a association specific named variable list
 *
 * \param self MmsConnection instance to operate on
 * \param mmsError user provided variable to store error code
 * \param listName the name of the named variable list
 * \param specWithResult if specWithResult is set to true, a IEC 61850 compliant request will be sent.
 *
 * \return Returns a MmsValue object or NULL if the request failed. The MmsValue object is
 * is of type MMS_ARRAY and contains the variable values of simple or complex type
 * in the order as they appeared in named variable list definition.
 */
LIB61850_API MmsValue*
MmsConnection_readNamedVariableListValuesAssociationSpecific(MmsConnection self, MmsError* mmsError,
        const char* listName, bool specWithResult);

LIB61850_API void
MmsConnection_readNamedVariableListValuesAssociationSpecificAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError,
        const char* listName, bool specWithResult,
        MmsConnection_ReadVariableHandler handler, void* parameter);

/**
 * \brief Define a new VMD or domain scoped named variable list at the server.
 *
 * The resulting named variable list will either be of domain scope (when the domainId argument
 * is present) or VMD scope when the domainId argument is NULL.
 *
 * \param self MmsConnection instance to operate on
 * \param mmsError user provided variable to store error code
 * \param domainId the domain name of the domain for the new variable list
 * \param listName the name of the named variable list
 * \param variableSpecs a list of variable specifications for the new variable list. The list
 *        elements have to be of type MmsVariableAccessSpecification*.
 */
LIB61850_API void
MmsConnection_defineNamedVariableList(MmsConnection self, MmsError* mmsError, const char* domainId,
        const char* listName,	LinkedList variableSpecs);

LIB61850_API void
MmsConnection_defineNamedVariableListAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError, const char* domainId,
        const char* listName, LinkedList variableSpecs,
        MmsConnection_GenericServiceHandler handler, void* parameter);



/**
 * \brief Define a new association specific named variable list at the server.
 *
 * \param self MmsConnection instance to operate on
 * \param mmsError user provided variable to store error code
 * \param listName the name of the named variable list
 * \param variableSpecs list of variable specifications for the new variable list.The list
 *        elements have to be of type MmsVariableAccessSpecification*.
 */
LIB61850_API void
MmsConnection_defineNamedVariableListAssociationSpecific(MmsConnection self, MmsError* mmsError,
        const char* listName,	LinkedList variableSpecs);

LIB61850_API void
MmsConnection_defineNamedVariableListAssociationSpecificAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError,
        const char* listName, LinkedList variableSpecs,
        MmsConnection_GenericServiceHandler handler, void* parameter);

/**
 * \brief Read the entry list of a named variable list at the server.
 *
 * The resulting named variable list will either be of domain scope (when the domainId argument
 * is present) or VMD scope when the domainId argument is NULL.
 *
 * \param self MmsConnection instance to operate on
 * \param mmsError user provided variable to store error code
 * \param domainId the domain name of the domain of the variable list
 * \param listName the name of the named variable list
 * \param deletable THIS IS A OUTPUT PARAMETER - indicates if the variable list is deletable by the
 * client. The user may provide a NULL pointer if the value doesn't matter.
 *
 * \return List of names of the variable list entries or NULL if the request failed
 */
LIB61850_API LinkedList /* <MmsVariableAccessSpecification*> */
MmsConnection_readNamedVariableListDirectory(MmsConnection self, MmsError* mmsError,
        const char* domainId, const char* listName, bool* deletable);


typedef void
(*MmsConnection_ReadNVLDirectoryHandler) (uint32_t invokeId, void* parameter, MmsError mmsError, LinkedList /* <MmsVariableAccessSpecification*> */ specs, bool deletable);


LIB61850_API void
MmsConnection_readNamedVariableListDirectoryAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError,
        const char* domainId, const char* listName,
        MmsConnection_ReadNVLDirectoryHandler handler, void* parameter);


/**
 * \brief Read the entry list of an association specific named variable list at the server.
 *
 * \param self MmsConnection instance to operate on
 * \param mmsError user provided variable to store error code
 * \param listName the name of the named variable list
 *
 * \return List of names of the variable list entries or NULL if the request failed
 */
LIB61850_API LinkedList /* <MmsVariableAccessSpecification*> */
MmsConnection_readNamedVariableListDirectoryAssociationSpecific(MmsConnection self, MmsError* mmsError,
        const char* listName, bool* deletable);

LIB61850_API void
MmsConnection_readNamedVariableListDirectoryAssociationSpecificAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError,
        const char* listName,
        MmsConnection_ReadNVLDirectoryHandler handler, void* parameter);

/**
 * \brief Delete a named variable list at the server.
 *
 * The resulting named variable list will either be of domain scope (when the domainId argument
 * is present) or VMD scope when the domainId argument is NULL.
 *
 * \param self MmsConnection instance to operate on
 * \param mmsError user provided variable to store error code
 * \param domainId the domain name of the domain of the variable list
 * \param listName the name of the named variable list
 *
 * \return true if named variable list has been deleted, false otherwise
 */
LIB61850_API bool
MmsConnection_deleteNamedVariableList(MmsConnection self, MmsError* mmsError, const char* domainId, const char* listName);


LIB61850_API void
MmsConnection_deleteNamedVariableListAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError, const char* domainId, const char* listName,
        MmsConnection_GenericServiceHandler handler, void* parameter);

/**
 * \brief Delete an association specific named variable list at the server.
 *
 * \param self MmsConnection instance to operate on
 * \param mmsError user provided variable to store error code
 * \param listName the name of the named variable list
 *
 * \return true if named variable list has been deleted, false otherwise
 */
LIB61850_API bool
MmsConnection_deleteAssociationSpecificNamedVariableList(MmsConnection self, MmsError* mmsError,
        const char* listName);


LIB61850_API void
MmsConnection_deleteAssociationSpecificNamedVariableListAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError, const char* listName,
        MmsConnection_GenericServiceHandler handler, void* parameter);

/**
 * \brief Create a new MmsVariableSpecification that can be used to define named variable lists.
 *
 * The created object can be deleted with free(). If the parameter strings were dynamically
 * allocated the deallocation is in the responsibility of the user.
 *
 * \param domainId the MMS domain name of the variable
 * \param itemId the name for the MMS variable
 *
 * \return reference to the new MmsVariableSpecfication object
 */
LIB61850_API MmsVariableAccessSpecification*
MmsVariableAccessSpecification_create(char* domainId, char* itemId);

/**
 * \brief Create a new MmsVariableSpecification that can be used to define named variable lists.
 *
 * The created object can be deleted with free(). If the parameter strings were dynamically
 * allocated the deallocation is in the responsibility of the user. This function should be
 * used for named variable list entries that are array elements or components of array elements
 * in the case when the array element is of complex (structured) type.
 *
 * \param domainId the MMS domain name of the variable
 * \param itemId the name for the MMS variable
 * \param index the array index to describe an array element
 * \param componentName the name of the component of the array element. Should be set to NULL
 *        if the array element is of simple type or the whole array element is required.
 *
 * \return reference to the new MmsVariableSpecfication object
 */
LIB61850_API MmsVariableAccessSpecification*
MmsVariableAccessSpecification_createAlternateAccess(char* domainId, char* itemId, int32_t index,
		char* componentName);

/**
 * \brief Delete the MmsVariableAccessSpecification data structure
 *
 * \param self the instance to delete
 */
LIB61850_API void
MmsVariableAccessSpecification_destroy(MmsVariableAccessSpecification* self);

/**
 * \brief Get the MMS local detail parameter (local detail means maximum MMS PDU size).
 *
 * This defaults to 65000 (or the value specified in the stack_config.h file.
 * This function should not be called after a successful connection attempt.
 *
 * \param  self MmsConnection instance to operate on
 * \param localDetail the maximum size of the MMS PDU that will be accepted.
 */
LIB61850_API void
MmsConnection_setLocalDetail(MmsConnection self, int32_t localDetail);

LIB61850_API int32_t
MmsConnection_getLocalDetail(MmsConnection self);

/**
 * \brief get the identity of the connected server
 *
 * This function will return the identity of the server if the server supports the MMS identify service.
 * The server identity consists of a vendor name, model name, and a revision.
 *
 * \param  self MmsConnection instance to operate on
 * \param mmsError user provided variable to store error code
 */
LIB61850_API MmsServerIdentity*
MmsConnection_identify(MmsConnection self, MmsError* mmsError);

typedef void
(*MmsConnection_IdentifyHandler) (uint32_t invokeId, void* parameter, MmsError mmsError,
        char* vendorName, char* modelName, char* revision);

LIB61850_API void
MmsConnection_identifyAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError,
        MmsConnection_IdentifyHandler handler, void* parameter);

/**
 * \brief Destroy (free) an MmsServerIdentity object
 *
 * \param self the object to destroy
 */
LIB61850_API void
MmsServerIdentity_destroy(MmsServerIdentity* self);

/**
 * \brief get the VMD status of the connected server (is MMS status service)
 *
 * This function will return the status of the connected server by invoking the MMS status service.
 * The services returns the logical and physical states of the server.
 *
 * \param[in] self MmsConnection instance to operate on
 * \param[out] mmsError user provided variable to store error code
 * \param[out] vmdLogicalStatus user provided variable to store the logical state of the VMD
 * \param[out] vmdPhysicalStatus user provided variable to store the physical state of the VMD
 * \param[in] extendedDerivation instructs the server to invoke self-diagnosis routines to determine server status
 */
LIB61850_API void
MmsConnection_getServerStatus(MmsConnection self, MmsError* mmsError, int* vmdLogicalStatus, int* vmdPhysicalStatus,
        bool extendedDerivation);

typedef void
(*MmsConnection_GetServerStatusHandler) (uint32_t invokeId, void* parameter, MmsError mmsError, int vmdLogicalStatus, int vmdPhysicalStatus);

LIB61850_API void
MmsConnection_getServerStatusAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError, bool extendedDerivation,
        MmsConnection_GetServerStatusHandler handler, void* parameter);

/*******************************************************************************
 * functions for MMS file services
 *******************************************************************************/

typedef void
(*MmsFileDirectoryHandler) (void* parameter, char* filename, uint32_t size, uint64_t lastModified);

/**
 * \brief Callback handler for the get file directory service
 *
 * Will be called once for each file directory entry and after the last entry with \ref filename = NULL to indicate
 * with \ref moreFollows set to true when more data is available server side. In case of an error the callback will be called with
 * \ref mmsError != MMS_ERROR_NONE and moreFollows = false.
 */
typedef void
(*MmsConnection_FileDirectoryHandler) (uint32_t invokeId, void* parameter, MmsError mmsError, char* filename, uint32_t size, uint64_t lastModfified,
        bool moreFollows);

typedef void
(*MmsFileReadHandler) (void* parameter, int32_t frsmId, uint8_t* buffer, uint32_t bytesReceived);

/**
 * \brief Callback handler for the file read service
 *
 * Will be called for every received part of the file and when there is an error during reading the file.
 *
 * \param invokeId invokeID of the response
 * \param parameter user provided context parameter
 * \param mmsError error code
 * \param frsmId ID of the file
 * \param buffer buffer where the received bytes are stored
 * \param bytesReceived number of bytes received with this response
 * \param moreFollows more messages with parts of the file are following
 */
typedef void
(*MmsConnection_FileReadHandler) (uint32_t invokeId, void* parameter, MmsError mmsError, int32_t frsmId, uint8_t* buffer, uint32_t byteReceived,
        bool moreFollows);


/**
 * \brief open a file for read
 *
 * \param self MmsConnection instance to operate on
 * \param mmsError user provided variable to store error code
 *
 * \return the FRSM ID (file read state machine) handle of the opened file
 */
LIB61850_API int32_t
MmsConnection_fileOpen(MmsConnection self, MmsError* mmsError, const char* filename, uint32_t initialPosition,
        uint32_t* fileSize, uint64_t* lastModified);

typedef void
(*MmsConnection_FileOpenHandler) (uint32_t invokeId, void* parameter, MmsError mmsError, int32_t frsmId, uint32_t fileSize, uint64_t lastModified);

LIB61850_API void
MmsConnection_fileOpenAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError, const char* filename, uint32_t initialPosition, MmsConnection_FileOpenHandler handler,
        void* parameter);


/**
 * \brief read the next data block from the file
 *
 * \param self MmsConnection instance to operate on
 * \param mmsError user provided variable to store error code
 * \param frsmId the FRSM ID (file read state machine) handle of the file
 * \param handler callback that is invoked to deliver the received data
 * \param handlerParameter user provided paramter that is passed to the callback function
 *
 * \return true if more data follows, false if last data has been received.
 */
LIB61850_API bool
MmsConnection_fileRead(MmsConnection self, MmsError* mmsError, int32_t frsmId, MmsFileReadHandler handler, void* handlerParameter);

LIB61850_API void
MmsConnection_fileReadAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError, int32_t frsmId, MmsConnection_FileReadHandler handler, void* parameter);

/**
 * \brief close the file with the specified frsmID
 *
 * \param self MmsConnection instance to operate on
 * \param mmsError user provided variable to store error code
 * \param frsmId id of the file to close
 */
LIB61850_API void
MmsConnection_fileClose(MmsConnection self, MmsError* mmsError, int32_t frsmId);

LIB61850_API void
MmsConnection_fileCloseAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError, uint32_t frsmId, MmsConnection_GenericServiceHandler handler, void* parameter);

/**
 * \brief delete the file with the specified name
 *
 * \param self MmsConnection instance to operate on
 * \param mmsError user provided variable to store error code
 * \param fileName name of the file to delete
 */
LIB61850_API void
MmsConnection_fileDelete(MmsConnection self, MmsError* mmsError, const char* fileName);

LIB61850_API void
MmsConnection_fileDeleteAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError, const char* fileName,
        MmsConnection_GenericServiceHandler handler, void* parameter);

/**
 * \brief rename the file with the specified name
 *
 * \param self MmsConnection instance to operate on
 * \param mmsError user provided variable to store error code
 * \param currentFileName name of the file to rename
 * \param newFileName new name of the file
 */
LIB61850_API void
MmsConnection_fileRename(MmsConnection self, MmsError* mmsError, const char* currentFileName, const char* newFileName);

LIB61850_API void
MmsConnection_fileRenameAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError, const char* currentFileName, const char* newFileName,
        MmsConnection_GenericServiceHandler handler, void* parameter);

/**
 * \brief Send an obtainFile request to the server (used to initiate file download to server)
 *
 * \param self MmsConnection instance to operate on
 * \param mmsError user provided variable to store error code
 * \param sourceFile the name of the source file (client side name)
 * \param destinationFile the name of the destination file (server side name)
 */
LIB61850_API void
MmsConnection_obtainFile(MmsConnection self, MmsError* mmsError, const char* sourceFile, const char* destinationFile);

LIB61850_API void
MmsConnection_obtainFileAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError, const char* sourceFile, const char* destinationFile,
        MmsConnection_GenericServiceHandler handler, void* parameter);

/**
 * \brief get the file directory of the server.
 *
 * This function will return the directory entries of the given server directory. For each directory entry
 * the provided callback handler is called. If the
 *
 * \param self MmsConnection instance to operate on
 * \param mmsError user provided variable to store error code
 * \param fileSpecification the file specification of the directory to browse or NULL to browse the root directory
 * \param continueAfter continuation point or NULL for the first request. The continuation point is the first entry
 *                      after the provided continuation file name.
 * \param handler user provided callback handler
 * \param handlerParameter user provided parameter that is passed to the handler
 *
 * \return (more follows) true if more data is available
 */
LIB61850_API bool
MmsConnection_getFileDirectory(MmsConnection self, MmsError* mmsError, const char* fileSpecification, const char* continueAfter,
        MmsFileDirectoryHandler handler, void* handlerParameter);

LIB61850_API void
MmsConnection_getFileDirectoryAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError, const char* fileSpecification, const char* continueAfter,
        MmsConnection_FileDirectoryHandler handler, void* parameter);

typedef struct sMmsJournalEntry* MmsJournalEntry;

typedef struct sMmsJournalVariable* MmsJournalVariable;

struct sMmsJournalEntry {
    MmsValue* entryID; /* type MMS_OCTET_STRING */
    MmsValue* occurenceTime; /* type MMS_BINARY_TIME */
    LinkedList journalVariables;
};

struct sMmsJournalVariable {
    char* tag;
    MmsValue* value;
};

/**
 * \brief Destroy a single MmsJournalEntry instance.
 *
 * This function will destroy the whole MmsJournalEntry object including the attached list
 * of MmsJournalVariable objects. It is intended to be used in conjunction with the
 * LinkedList_destroyDeep function in order to free the result of MmsConnection_readJournalTimeRange
 * or MmsConnection_readJournalStartAfter
 *
 * LinkedList_destroyDeep(journalEntries, (LinkedListValueDeleteFunction)
 *                           MmsJournalEntry_destroy);
 *
 * \param self the MmsJournalEntry instance to destroy
 */
LIB61850_API void
MmsJournalEntry_destroy(MmsJournalEntry self);

LIB61850_API MmsValue*
MmsJournalEntry_getEntryID(MmsJournalEntry self);

LIB61850_API MmsValue*
MmsJournalEntry_getOccurenceTime(MmsJournalEntry self);

LIB61850_API LinkedList /* <MmsJournalVariable> */
MmsJournalEntry_getJournalVariables(MmsJournalEntry self);

LIB61850_API const char*
MmsJournalVariable_getTag(MmsJournalVariable self);

LIB61850_API MmsValue*
MmsJournalVariable_getValue(MmsJournalVariable self);

typedef void
(*MmsConnection_ReadJournalHandler) (uint32_t invokeId, void* parameter, MmsError mmsError, LinkedList /* <MmsJournalEntry> */ journalEntries, bool moreFollows);


LIB61850_API LinkedList /* <MmsJournalEntry> */
MmsConnection_readJournalTimeRange(MmsConnection self, MmsError* mmsError, const char* domainId, const char* itemId,
        MmsValue* startTime, MmsValue* endTime, bool* moreFollows);

LIB61850_API void
MmsConnection_readJournalTimeRangeAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError, const char* domainId, const char* itemId,
        MmsValue* startTime, MmsValue* endTime, MmsConnection_ReadJournalHandler handler, void* parameter);

LIB61850_API LinkedList /* <MmsJournalEntry> */
MmsConnection_readJournalStartAfter(MmsConnection self, MmsError* mmsError, const char* domainId, const char* itemId,
        MmsValue* timeSpecification, MmsValue* entrySpecification, bool* moreFollows);

LIB61850_API void
MmsConnection_readJournalStartAfterAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError, const char* domainId, const char* itemId,
        MmsValue* timeSpecification, MmsValue* entrySpecification, MmsConnection_ReadJournalHandler handler, void* parameter);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif /* MMS_CLIENT_CONNECTION_H_ */
