/*
 *  mms_client_connection.h
 *
 *  Copyright 2013-2016 Michael Zillgith
 *
 *	This file is part of libIEC61850.
 *
 *	libIEC61850 is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	libIEC61850 is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with libIEC61850.  If not, see <http://www.gnu.org/licenses/>.
 *
 *	See COPYING file for the complete license text.
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
MmsConnection
MmsConnection_create(void);

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
void
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
void
MmsConnection_setFilestoreBasepath(MmsConnection self, const char* basepath);

/**
 * \brief Set the request timeout in ms for this connection
 *
 * \param self MmsConnection instance to operate on
 * \param timeoutInMs request timeout in milliseconds
 */
void
MmsConnection_setRequestTimeout(MmsConnection self, uint32_t timeoutInMs);

/**
 * \brief Set the connect timeout in ms for this connection instance
 *
 * \param self MmsConnection instance to operate on
 * \param timeoutInMs connect timeout in milliseconds
 */
void
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
void
MmsConnection_setInformationReportHandler(MmsConnection self, MmsInformationReportHandler handler,
        void* parameter);

/**
 * \brief Get the ISO connection parameters for an MmsConnection instance
 *
 * \param self MmsConnection instance to operate on
 * \return params the to be used by this connection
 */
IsoConnectionParameters
MmsConnection_getIsoConnectionParameters(MmsConnection self);

/**
 * \brief Get the MMS specific connection parameters for an MmsConnection instance
 *
 * \param self MmsConnection instance to operate on
 * \return params the to be used by this connection
 */
MmsConnectionParameters
MmsConnection_getMmsConnectionParameters(MmsConnection self);

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
void
MmsConnection_setConnectionLostHandler(MmsConnection self, MmsConnectionLostHandler handler, void* handlerParameter);

/**
 * \brief Set the ISO connection parameters for a MmsConnection instance
 *
 * \param self MmsConnection instance to operate on
 * \param params the ISO client parameters to use
 */
void
MmsConnection_setIsoConnectionParameters(MmsConnection self, IsoConnectionParameters* params);

/**
 * \brief Destroy an MmsConnection instance and release all resources
 *
 * \param self MmsConnection instance to operate on
 */
void
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
 * \param serverPort TCP port number of the server to connect (usually 102)
 *
 * \return true on success. false if the connection attempt failed.
 */
bool
MmsConnection_connect(MmsConnection self, MmsError* mmsError, const char* serverName, int serverPort);

/**
 * \brief Close the connection - not recommended
 *
 * This service simply closes the TCP socket without any hand-shaking with the server.
 * This behavior is not specified. Use with care!
 *
 * \param self MmsConnection instance to operate on
 */
void
MmsConnection_close(MmsConnection self);

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
void
MmsConnection_abort(MmsConnection self, MmsError* mmsError);

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
void
MmsConnection_conclude(MmsConnection self, MmsError* mmsError);


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
LinkedList /* <char*> */
MmsConnection_getVMDVariableNames(MmsConnection self, MmsError* mmsError);

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
LinkedList /* <char*> */
MmsConnection_getDomainNames(MmsConnection self, MmsError* mmsError);

/**
 * \brief Get the names of all variables present in a MMS domain of the server.
 *
 * This will result in a domain specific GetNameList request.
 *
 * \param self MmsConnection instance to operate on
 * \param mmsError user provided variable to store error code
 * \param domainId the domain name for the domain specific request
 *
 * \return the of domain specific variable names or NULL if the request failed.
 */
LinkedList /* <char*> */
MmsConnection_getDomainVariableNames(MmsConnection self, MmsError* mmsError, const char* domainId);

/**
 * \brief Get the names of all named variable lists present in a MMS domain of the server.
 *
 * This will result in a domain specific GetNameList request.
 *
 * \param self MmsConnection instance to operate on
 * \param mmsError user provided variable to store error code
 * \param domainId the domain name for the domain specific request
 *
 * \return the domain specific named variable list names or NULL if the request failed.
 */
LinkedList /* <char*> */
MmsConnection_getDomainVariableListNames(MmsConnection self, MmsError* mmsError, const char* domainId);

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
LinkedList /* <char*> */
MmsConnection_getDomainJournals(MmsConnection self, MmsError* mmsError, const char* domainId);

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
LinkedList /* <char*> */
MmsConnection_getVariableListNamesAssociationSpecific(MmsConnection self, MmsError* mmsError);


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
MmsValue*
MmsConnection_readVariable(MmsConnection self, MmsError* mmsError, const char* domainId, const char* itemId);

/**
 * \brief Read an element of a single array variable from the server.
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
MmsValue*
MmsConnection_readArrayElements(MmsConnection self, MmsError* mmsError, const char* domainId, const char* itemId,
		uint32_t startIndex, uint32_t numberOfElements);

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
MmsValue*
MmsConnection_readMultipleVariables(MmsConnection self, MmsError* mmsError, const char* domainId,
		LinkedList /*<char*>*/ items);

/**
 * \brief Write a single variable to the server.
 *
 * \param self MmsConnection instance to operate on
 * \param mmsError user provided variable to store error code
 * \param domainId the domain name of the variable to be written
 * \param itemId name of the variable to be written
 * \param value value of the variable to be written
 */
void
MmsConnection_writeVariable(MmsConnection self, MmsError* mmsError,
        const char* domainId, const char* itemId, MmsValue* value);

/**
 * \brief Write multiple variables to the server.
 *
 * This function will write multiple variables at the server.
 *
 * The parameter accessResults is a pointer to a LinkedList reference. The methods will create a new LinkedList
 * object that contains the AccessResults of the single variable write attempts. It is up to the user to free this
 * objects properly (e.g. with LinkedList_destroyDeep(accessResults, MmsValue_delete)).
 *
 * \param self MmsConnection instance to operate on
 * \param mmsError user provided variable to store error code
 * \param domainId the common domain name of all variables to be written
 * \param items a linked list containing the names of the variables to be written. The names are C strings.
 * \param values values of the variables to be written
 * \param (OUTPUT) the MmsValue objects of type MMS_DATA_ACCESS_ERROR representing the write success of a single variable
 *        write.
 */
void
MmsConnection_writeMultipleVariables(MmsConnection self, MmsError* mmsError, const char* domainId,
        LinkedList /*<char*>*/ items, LinkedList /* <MmsValue*> */ values,
        LinkedList* /* <MmsValue*> */ accessResults);

/**
 * \brief Get the variable access attributes of a MMS named variable of the server
 *
 * \param self MmsConnection instance to operate on
 * \param mmsError user provided variable to store error code
 * \param domainId the domain name of the variable
 * \param itemId name of the variable
 *
 * \return Returns a MmsTypeSpecification object or NULL if the request failed.
 */
MmsVariableSpecification*
MmsConnection_getVariableAccessAttributes(MmsConnection self, MmsError* mmsError,
        const char* domainId, const char* itemId);

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
MmsValue*
MmsConnection_readNamedVariableListValues(MmsConnection self, MmsError* mmsError, const char* domainId,
        const char* listName,	bool specWithResult);


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
MmsValue*
MmsConnection_readNamedVariableListValuesAssociationSpecific(MmsConnection self, MmsError* mmsError,
        const char* listName,	bool specWithResult);

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
void
MmsConnection_defineNamedVariableList(MmsConnection self, MmsError* mmsError, const char* domainId,
        const char* listName,	LinkedList variableSpecs);


/**
 * \brief Define a new association specific named variable list at the server.
 *
 * \param self MmsConnection instance to operate on
 * \param mmsError user provided variable to store error code
 * \param listName the name of the named variable list
 * \param variableSpecs list of variable specifications for the new variable list.The list
 *        elements have to be of type MmsVariableAccessSpecification*.
 */
void
MmsConnection_defineNamedVariableListAssociationSpecific(MmsConnection self, MmsError* mmsError,
        const char* listName,	LinkedList variableSpecs);

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
LinkedList /* <MmsVariableAccessSpecification*> */
MmsConnection_readNamedVariableListDirectory(MmsConnection self, MmsError* mmsError,
        const char* domainId, const char* listName, bool* deletable);



/**
 * \brief Read the entry list of an association specific named variable list at the server.
 *
 * \param self MmsConnection instance to operate on
 * \param mmsError user provided variable to store error code
 * \param listName the name of the named variable list
 *
 * \return List of names of the variable list entries or NULL if the request failed
 */
LinkedList /* <MmsVariableAccessSpecification*> */
MmsConnection_readNamedVariableListDirectoryAssociationSpecific(MmsConnection self, MmsError* mmsError,
        const char* listName, bool* deletable);

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
bool
MmsConnection_deleteNamedVariableList(MmsConnection self, MmsError* mmsError, const char* domainId, const char* listName);

/**
 * \brief Delete an association specific named variable list at the server.
 *
 * \param self MmsConnection instance to operate on
 * \param mmsError user provided variable to store error code
 * \param listName the name of the named variable list
 *
 * \return true if named variable list has been deleted, false otherwise
 */
bool
MmsConnection_deleteAssociationSpecificNamedVariableList(MmsConnection self, MmsError* mmsError,
        const char* listName);

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
MmsVariableAccessSpecification*
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
MmsVariableAccessSpecification*
MmsVariableAccessSpecification_createAlternateAccess(char* domainId, char* itemId, int32_t index,
		char* componentName);

/**
 * \brief Delete the MmsVariableAccessSpecification data structure
 *
 * \param self the instance to delete
 */
void
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
void
MmsConnection_setLocalDetail(MmsConnection self, int32_t localDetail);

int32_t
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
MmsServerIdentity*
MmsConnection_identify(MmsConnection self, MmsError* mmsError);

void
MmsServerIdentity_destroy(MmsServerIdentity* self);

/**
 * \brief get the VMD status of the connected server (is MMS status service)
 *
 * This function will return the status of the connected server by invoking the MMS status service.
 * The services returns the logical and physical states of the server.
 *
 * \param self MmsConnection instance to operate on
 * \param mmsError user provided variable to store error code
 * \param vmdLogicalStatus user provided variable to store the logical state of the VMD
 * \param vmdPhysicalStatus user provided variable to store the physical state of the VMD
 * \param extendedDerivation instructs the server to invoke self-diagnosis routines to determine server status
 */
void
MmsConnection_getServerStatus(MmsConnection self, MmsError* mmsError, int* vmdLogicalStatus, int* vmdPhysicalStatus,
        bool extendedDerivation);

/*******************************************************************************
 * functions for MMS file services
 *******************************************************************************/

typedef void
(*MmsFileDirectoryHandler) (void* parameter, char* filename, uint32_t size, uint64_t lastModified);

typedef void
(*MmsFileReadHandler) (void* parameter, int32_t frsmId, uint8_t* buffer, uint32_t bytesReceived);


/**
 * \brief open a file for read
 *
 * \param self MmsConnection instance to operate on
 * \param mmsError user provided variable to store error code
 *
 * \return the FRSM ID (file read state machine) handle of the opened file
 */
int32_t
MmsConnection_fileOpen(MmsConnection self, MmsError* mmsError, const char* filename, uint32_t initialPosition,
        uint32_t* fileSize, uint64_t* lastModified);

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
bool
MmsConnection_fileRead(MmsConnection self, MmsError* mmsError, int32_t frsmId, MmsFileReadHandler handler, void* handlerParameter);

/**
 * \brief close the file with the specified frsmID
 *
 * \param self MmsConnection instance to operate on
 * \param mmsError user provided variable to store error code
 * \param frsmId id of the file to close
 */
void
MmsConnection_fileClose(MmsConnection self, MmsError* mmsError, int32_t frsmId);

/**
 * \brief delete the file with the specified name
 *
 * \param self MmsConnection instance to operate on
 * \param mmsError user provided variable to store error code
 * \param fileName name of the file to delete
 */
void
MmsConnection_fileDelete(MmsConnection self, MmsError* mmsError, const char* fileName);

/**
 * \brief rename the file with the specified name
 *
 * \param self MmsConnection instance to operate on
 * \param mmsError user provided variable to store error code
 * \param currentFileName name of the file to rename
 * \param newFileName new name of the file
 */
void
MmsConnection_fileRename(MmsConnection self, MmsError* mmsError, const char* currentFileName, const char* newFileName);


/**
 * \brief Send an obtainFile request to the server (used to initiate file download to server)
 *
 * \param self MmsConnection instance to operate on
 * \param mmsError user provided variable to store error code
 * \param sourceFile the name of the source file (client side name)
 * \param destinationFile the name of the destination file (server side name)
 */
void
MmsConnection_obtainFile(MmsConnection self, MmsError* mmsError, const char* sourceFile, const char* destinationFile);

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
bool
MmsConnection_getFileDirectory(MmsConnection self, MmsError* mmsError, const char* fileSpecification, const char* continueAfter,
        MmsFileDirectoryHandler handler, void* handlerParameter);

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
void
MmsJournalEntry_destroy(MmsJournalEntry self);

const MmsValue*
MmsJournalEntry_getEntryID(MmsJournalEntry self);

const MmsValue*
MmsJournalEntry_getOccurenceTime(MmsJournalEntry self);

const LinkedList /* <MmsJournalVariable> */
MmsJournalEntry_getJournalVariables(MmsJournalEntry self);

const char*
MmsJournalVariable_getTag(MmsJournalVariable self);

const MmsValue*
MmsJournalVariable_getValue(MmsJournalVariable self);


LinkedList
MmsConnection_readJournalTimeRange(MmsConnection self, MmsError* mmsError, const char* domainId, const char* itemId,
        MmsValue* startingTime, MmsValue* endingTime, bool* moreFollows);

LinkedList
MmsConnection_readJournalStartAfter(MmsConnection self, MmsError* mmsError, const char* domainId, const char* itemId,
        MmsValue* timeSpecification, MmsValue* entrySpecification, bool* moreFollows);

/**
 * \brief Destroy (free) an MmsServerIdentity object
 *
 * \param self the object to destroy
 */
void
MmsServerIdentity_destroy(MmsServerIdentity* self);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif /* MMS_CLIENT_CONNECTION_H_ */
