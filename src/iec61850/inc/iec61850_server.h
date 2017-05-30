/*
 *  iec61850_server.h
 *
 *  IEC 61850 server API for libiec61850.
 *
 *  Copyright 2013, 2014 Michael Zillgith
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
 *
 */

#ifndef IED_SERVER_API_H_
#define IED_SERVER_API_H_

#ifdef __cplusplus
extern "C" {
#endif

/** \defgroup server_api_group IEC 61850 server API
 *  @{
 */

#include "mms_server.h"
#include "iec61850_dynamic_model.h"
#include "iec61850_model.h"
#include "hal_filesystem.h"
#include "iec61850_config_file_parser.h"

/**
 * An opaque handle for an IED server instance
 */
typedef struct sIedServer* IedServer;

/**
 * An opaque handle for a client connection
 */
typedef struct sClientConnection* ClientConnection;

/**
 * @defgroup IEC61850_SERVER_GENERAL General server setup and management functions
 *
 * @{
 */


/**
 * \brief Create a new IedServer instance
 *
 * \param iedModel reference to the IedModel data structure to be used as IEC 61850 model of the device
 *
 * \return the newly generated IedServer instance
 */
IedServer
IedServer_create(IedModel* iedModel);

/**
 * \brief Destroy an IedServer instance and release all resources (memory, TCP sockets)
 *
 * \param self the instance of IedServer to operate on.
 */
void
IedServer_destroy(IedServer self);

/**
 * \brief Set the local IP address to listen on
 *
 *  \param self the IedServer instance
 *  \param localIpAddress the local IP address as C string (an internal copy will be created)
 */
void
IedServer_setLocalIpAddress(IedServer self, const char* localIpAddress);

/**
 * \brief Set the virtual filestore basepath for the MMS file services
 *
 * All external file service accesses will be mapped to paths relative to the base directory.
 * NOTE: This function is only available when the CONFIG_SET_FILESTORE_BASEPATH_AT_RUNTIME
 * option in stack_config.h is set.
 *
 * \param self the IedServer instance
 * \param basepath the new virtual filestore basepath
 */
void
IedServer_setFilestoreBasepath(IedServer self, const char* basepath);

/**
 * \brief Start handling client connections
 *
 * \param self the instance of IedServer to operate on.
 * \param tcpPort the TCP port the server is listening
 */
void
IedServer_start(IedServer self, int tcpPort);

/**
 * \brief Stop handling client connections
 *
 * \param self the instance of IedServer to operate on.
 */
void
IedServer_stop(IedServer self);

/**
 * \brief Start handling client connection for non-threaded mode
 *
 * This function will instruct the TCP(IP stack to listen for incoming connections.
 * In order to accept new connection the function IedServer_processIncomingData has to
 * be called periodically.
 *
 * \param self the instance of IedServer to operate on.
 * \param tcpPort the TCP port the server is listening
 */
void
IedServer_startThreadless(IedServer self, int tcpPort);

/**
 * \brief Wait until a server connection is ready (with timeout)
 *
 * The function will wait until a server connection is ready (data available) or the
 * timeout elapsed. This function is intended for the non-threaded mode. Its use is optional.
 * It is equivalent of using select on sockets on Linux. If the return value is != 0 the
 * IedServer_processIncomingData function should be called.
 *
 * \param self the IedServer instance
 * \param timeoutMs the timeout to wait when no connection is ready
 *
 * \return 0 if no connection is ready; otherwise at least one connection is ready
 */
int
IedServer_waitReady(IedServer self, unsigned int timeoutMs);

/**
 * \brief handle incoming TCP data in non-threaded mode
 *
 * The function should be called periodically. If the function is called more often
 * the response time for incoming messages will be faster. As an alternative the
 * function may only be called if new TCP data is available.
 *
 * \param self the instance of IedServer to operate on.
 */
void
IedServer_processIncomingData(IedServer self);

/**
 * \brief perform periodic background tasks in non-threaded mode
 *
 * The function should be called periodically. If the function is called more often
 * the more accurate are internal timeouts (e.g. for reports).
 *
 * \param self the instance of IedServer to operate on.
 */
void
IedServer_performPeriodicTasks(IedServer self);

/**
 * \brief Stop handling client connections for non-threaded mode
 *
 * \param self the instance of IedServer to operate on.
 */
void
IedServer_stopThreadless(IedServer self);

/**
 * \brief Return the data model of the server
 *
 * \param self the instance of IedServer to operate on.
 *
 * \return the IedModel* instance of the server
 */
IedModel*
IedServer_getDataModel(IedServer self);

/**
 * \brief Check if IedServer instance is listening for client connections
 *
 * \param self the instance of IedServer to operate on.
 *
 * \return true if IedServer instance is listening for client connections
 */
bool
IedServer_isRunning(IedServer self);

/**
 * \brief Get access to the underlying MmsServer instance.
 *
 * This function should be handled with care. Since direct interaction with the
 * MmsServer can interfere with the IedServer.
 *
 * \param self the instance of IedServer to operate on.
 *
 * \return MmsServer instance that is used by the IedServer
 */
MmsServer
IedServer_getMmsServer(IedServer self);

/**
 * \brief get the IsoServer instance for this IedServer object
 *
 * \param self the instance of IedServer to operate on.
 *
 * \return the IsoServer instance of this IedServer object
 */
IsoServer
IedServer_getIsoServer(IedServer self);

/**
 * \brief Enable all GOOSE control blocks.
 *
 * This will set the GoEna attribute of all configured GOOSE control blocks
 * to true. If this method is not called at the startup or reset of the server
 * then configured GOOSE control blocks keep inactive until a MMS client enables
 * them by writing to the GOOSE control block.
 *
 * \param self the instance of IedServer to operate on.
 */
void
IedServer_enableGoosePublishing(IedServer self);

/**
 * \brief Disable all GOOSE control blocks.
 *
 * This will set the GoEna attribute of all configured GOOSE control blocks
 * to false. This will stop GOOSE transmission.
 *
 * \param self the instance of IedServer to operate on.
 */
void
IedServer_disableGoosePublishing(IedServer self);

/**
 * \brief Set the ethernet interface to be used by GOOSE publishing
 *
 * This function can be used to set the GOOSE interface ID. If not used or set to NULL the
 * default interface ID from stack_config.h is used. Note the interface ID is operating system
 * specific!
 *
 * \param self the instance of IedServer to operate on.
 * \param interfaceId the ID of the ethernet interface to be used for GOOSE publishing
 */
void
IedServer_setGooseInterfaceId(IedServer self, const char* interfaceId);

/**@}*/

/**
 * @defgroup IEC61850_SERVER_CONNECTION_HANDLING Connection handling and client authentication
 *
 * @{
 */

/**
 * \brief set the authenticator for this server
 *
 * This function sets a user specified authenticator that is used to identify and authenticate clients that
 * wants to connect. The authenticator is called on each connection attempt. Depending on the return value
 * of the authenticator the client connection is accepted or refused. If no authenticator is set all client
 * connections are accepted.
 *
 * \param self the instance of IedServer to operate on.
 * \param authenticator the user provided authenticator callback
 * \param authenticatorParameter user provided paremeter that is passed to the authenticator
 */
void
IedServer_setAuthenticator(IedServer self, AcseAuthenticator authenticator, void* authenticatorParameter);



/**
 * \brief get the peer address of this connection as string
 *
 * Note: the returned string is only valid as long as the client connection exists. It is save to use
 * the string inside of the connection indication callback function.
 *
 * \param self the ClientConnection instance
 * \return peer address as C string.
 */
const char*
ClientConnection_getPeerAddress(ClientConnection self);

/**
 * \brief Get the security token associated with this connection
 *
 * The security token is an opaque handle that is associated with the connection. It is provided by the
 * authenticator (if one is present). If no security token is used then this function returns NULL
 *
 * \param self the ClientConnection instance
 *
 * \return the security token or NULL
 */
void*
ClientConnection_getSecurityToken(ClientConnection self);

/**
 * \brief User provided callback function that is invoked whenever a new client connects or an existing connection is closed
 *        or detected as lost.
 *
 * \param self the instance of IedServer where the connection event occured.
 * \param connection the new or closed client connect object
 * \param connected true if a new connection is indicated, false if the connection has been closed or detected as lost.
 * \param parameter a user provided parameter
 */
typedef void (*IedConnectionIndicationHandler) (IedServer self, ClientConnection connection, bool connected, void* parameter);

/**
 * \brief set a callback function that will be called on connection events (open or close).
 *
 * \param self the instance of IedServer to operate on.
 * \param handler the user provided callback function
 * \param parameter a user provided parameter that is passed to the callback function.
 */
void
IedServer_setConnectionIndicationHandler(IedServer self, IedConnectionIndicationHandler handler, void* parameter);


/**@}*/

/**
 * @defgroup IEC61850_SERVER_DATA_MODEL_ACCESS Data model access and data update
 *
 * @{
 */


/**
 * \brief Lock the MMS server data model.
 *
 * Client requests will be postponed until the lock is removed.
 *
 * NOTE: This method should never be called inside of a library callback function. In the context of
 * a library callback the data model is always already locked! Calling this function inside of a
 * library callback may lead to a deadlock condition.
 *
 * \param self the instance of IedServer to operate on.
 */
void
IedServer_lockDataModel(IedServer self);

/**
 * \brief Unlock the MMS server data model and process pending client requests.
 *
 * NOTE: This method should never be called inside of a library callback function. In the context of
 * a library callback the data model is always already locked!
 *
 * \param self the instance of IedServer to operate on.
 */
void
IedServer_unlockDataModel(IedServer self);

/**
 * \brief Get data attribute value
 *
 * Get the MmsValue object of an MMS Named Variable that is part of the device model.
 * You should not manipulate the received object directly. Instead you should use
 * the IedServer_updateValue method.
 *
 * \param self the instance of IedServer to operate on.
 * \param dataAttribute the data attribute handle
 *
 * \return MmsValue object of the MMS Named Variable or NULL if the value does not exist.
 */
MmsValue*
IedServer_getAttributeValue(IedServer self, DataAttribute* dataAttribute);

/**
 * \brief Get data attribute value of a boolean data attribute
 *
 * Get the value of a data attribute of type MMS_BOOLEAN. If the data attribute
 * is of a different type the returned value is undefined.
 *
 * \param self the instance of IedServer to operate on.
 * \param dataAttribute the data attribute handle
 *
 * \return true or false
 */
bool
IedServer_getBooleanAttributeValue(IedServer self, const DataAttribute* dataAttribute);

/**
 * \brief Get data attribute value of an integer data attribute
 *
 * Get the value of a data attribute of type MMS_INTEGER. If the data attribute
 * is of a different type the returned value is undefined.
 *
 * \param self the instance of IedServer to operate on.
 * \param dataAttribute the data attribute handle
 *
 * \return the value as 32 bit integer
 */
int32_t
IedServer_getInt32AttributeValue(IedServer self, const DataAttribute* dataAttribute);

/**
 * \brief Get data attribute value of an integer data attribute
 *
 * Get the value of a data attribute of type MMS_INTEGER. If the data attribute
 * is of a different type the returned value is undefined.
 *
 * \param self the instance of IedServer to operate on.
 * \param dataAttribute the data attribute handle
 *
 * \return the value as 64 bit integer
 */
int64_t
IedServer_getInt64AttributeValue(IedServer self, const DataAttribute* dataAttribute);

/**
 * \brief Get data attribute value of an unsigned integer data attribute
 *
 * Get the value of a data attribute of type MMS_UNSIGNED. If the data attribute
 * is of a different type the returned value is undefined.
 *
 * \param self the instance of IedServer to operate on.
 * \param dataAttribute the data attribute handle
 *
 * \return the value as 32 bit unsigned integer
 */
uint32_t
IedServer_getUInt32AttributeValue(IedServer self, const DataAttribute* dataAttribute);

/**
 * \brief Get data attribute value of a floating point data attribute
 *
 * Get the value of a data attribute of type MMS_FLOAT. If the data attribute
 * is of a different type the returned value is undefined.
 *
 * \param self the instance of IedServer to operate on.
 * \param dataAttribute the data attribute handle
 *
 * \return the value as 32 bit float
 */
float
IedServer_getFloatAttributeValue(IedServer self, const DataAttribute* dataAttribute);

/**
 * \brief Get data attribute value of a UTC time data attribute
 *
 * Get the value of a data attribute of type MMS_UTC_TIME. If the data attribute
 * is of a different type the returned value is undefined.
 *
 * \param self the instance of IedServer to operate on.
 * \param dataAttribute the data attribute handle
 *
 * \return the value as 32 bit float
 */
uint64_t
IedServer_getUTCTimeAttributeValue(IedServer self, const DataAttribute* dataAttribute);

/**
 * \brief Get data attribute value of a bit string data attribute as integer value
 *
 * Get the value of a data attribute of type MMS_BIT_STRING. If the data attribute
 * is of a different type the returned value is undefined.
 * NOTE: The returned integer is determined by calculating the according the follwing
 * formula: bit0^0 + bit1^1 + ... + bitn^n
 * It is not specified in the IEC 61850 specifications how a bit string can be interpreted
 * as an integer. Therefore this function has to be used with care.
 *
 * \param self the instance of IedServer to operate on.
 * \param dataAttribute the data attribute handle
 *
 * \return the value a 32 bit integer.
 */
uint32_t
IedServer_getBitStringAttributeValue(IedServer self, const DataAttribute* dataAttribute);

/**
 * \brief Get data attribute value of a string type data attribute
 *
 * Get the value of a data attribute of type MMS_STRING or MMS_VISIBLE_STRING. If the data attribute
 * is of a different type the returned value is undefined.
 *
 * \param self the instance of IedServer to operate on.
 * \param dataAttribute the data attribute handle
 *
 * \return the value as a C string (null terminated string)
 */
const char*
IedServer_getStringAttributeValue(IedServer self, const DataAttribute* dataAttribute);


/**
 * \brief Get the MmsValue object related to a FunctionalConstrainedData object
 *
 * Get the MmsValue from the server cache that is associated with the Functional Constrained Data (FCD)
 * object that is specified by the DataObject and the given Function Constraint (FC).
 * Accessing the value will directly influence the values presented by the server. This kind of direct
 * access will also bypass the report notification mechanism (i.e. changes will not cause a report!).
 * Therefore this function should be used with care. It could be useful to access arrays of DataObjects.
 *
 * \param self the instance of IedServer to operate on.
 * \param dataObject the data object to specify the FCD
 * \param fc the FC to specify the FCD
 *
 * \return MmsValue object cached by the server.
 */
MmsValue*
IedServer_getFunctionalConstrainedData(IedServer self, DataObject* dataObject, FunctionalConstraint fc);

/**
 * \brief Update the MmsValue object of an IEC 61850 data attribute.
 *
 * The data attribute handle of type DataAttribute* are imported with static_model.h in the case when
 * the static data model is used.
 * You should use this function instead of directly operating on the MmsValue instance
 * that is hold by the MMS server. Otherwise the IEC 61850 server is not aware of the
 * changed value and will e.g. not generate a report.
 *
 * This function will also check if a trigger condition is satisfied in the case when a report or GOOSE
 * control block is enabled.
 *
 * \param self the instance of IedServer to operate on.
 * \param dataAttribute the data attribute handle
 * \param value MmsValue object used to update the value cached by the server.
 */
void
IedServer_updateAttributeValue(IedServer self, DataAttribute* dataAttribute, MmsValue* value);

/**
 * \brief Update the value of an IEC 61850 float data attribute.
 *
 * Update the value of a float data attribute without handling with MmsValue instances.
 *
 * This function will also check if a trigger condition is satisfied in the case when a report or GOOSE
 * control block is enabled.
 *
 * \param self the instance of IedServer to operate on.
 * \param dataAttribute the data attribute handle
 * \param value the new float value of the data attribute.
 */
void
IedServer_updateFloatAttributeValue(IedServer self, DataAttribute* dataAttribute, float value);

/**
 * \brief Update the value of an IEC 61850 integer32 data attribute.
 *
 * Update the value of a integer data attribute without handling with MmsValue instances.
 *
 * This function will also check if a trigger condition is satisfied in the case when a report or GOOSE
 * control block is enabled.
 *
 * \param self the instance of IedServer to operate on.
 * \param dataAttribute the data attribute handle
 * \param value the new integer value of the data attribute.
 */
void
IedServer_updateInt32AttributeValue(IedServer self, DataAttribute* dataAttribute, int32_t value);

/**
 * \brief Update the value of an IEC 61850 integer64 data attribute (like BCR actVal)
 *
 * Update the value of a integer data attribute without handling with MmsValue instances.
 *
 * This function will also check if a trigger condition is satisfied in the case when a report or GOOSE
 * control block is enabled.
 *
 * \param self the instance of IedServer to operate on.
 * \param dataAttribute the data attribute handle
 * \param value the new 64 bit integer value of the data attribute.
 */
void
IedServer_updateInt64AttributeValue(IedServer self, DataAttribute* dataAttribute, int64_t value);

/**
 * \brief Update the value of an IEC 61850 unsigned integer data attribute.
 *
 * Update the value of a unsigned data attribute without handling with MmsValue instances.
 *
 * This function will also check if a trigger condition is satisfied in the case when a report or GOOSE
 * control block is enabled.
 *
 * \param self the instance of IedServer to operate on.
 * \param dataAttribute the data attribute handle
 * \param value the new unsigned integer value of the data attribute.
 */
void
IedServer_updateUnsignedAttributeValue(IedServer self, DataAttribute* dataAttribute, uint32_t value);

/**
 * \brief Update the value of an IEC 61850 bit string data attribute.
 *
 * Update the value of a bit string data attribute without handling with MmsValue instances.
 *
 * This function will also check if a trigger condition is satisfied in the case when a report or GOOSE
 * control block is enabled.
 *
 * \param self the instance of IedServer to operate on.
 * \param dataAttribute the data attribute handle
 * \param value the new bit string integer value of the data attribute.
 */
void
IedServer_updateBitStringAttributeValue(IedServer self, DataAttribute* dataAttribute, uint32_t value);

/**
 * \brief Update the value of an IEC 61850 boolean data attribute.
 *
 * Update the value of a boolean data attribute without handling with MmsValue instances.
 *
 * This function will also check if a trigger condition is satisfied in the case when a report or GOOSE
 * control block is enabled.
 *
 * \param self the instance of IedServer to operate on.
 * \param dataAttribute the data attribute handle
 * \param value the new boolean value of the data attribute.
 */
void
IedServer_updateBooleanAttributeValue(IedServer self, DataAttribute* dataAttribute, bool value);

/**
 * \brief Update the value of an IEC 61850 visible string data attribute.
 *
 * Update the value of a visible string data attribute without handling MmsValue instances.
 *
 * This function will also check if a trigger condition is satisfied in the case when a report or GOOSE
 * control block is enabled.
 *
 * \param self the instance of IedServer to operate on.
 * \param dataAttribute the data attribute handle
 * \param value the new visible string value of the data attribute.
 */
void
IedServer_updateVisibleStringAttributeValue(IedServer self, DataAttribute* dataAttribute, char *value);

/**
 * \brief Update the value of an IEC 61850 UTC time (timestamp) data attribute.
 *
 * Update the value of a UTC time data attribute without handling MmsValue instances.
 *
 * This function will also check if a trigger condition is satisfied in the case when a report or GOOSE
 * control block is enabled.
 *
 * \param self the instance of IedServer to operate on.
 * \param dataAttribute the data attribute handle
 * \param value the new UTC time value of the data attribute as a ms timestamp
 */
void
IedServer_updateUTCTimeAttributeValue(IedServer self, DataAttribute* dataAttribute, uint64_t value);

/**
 * \brief Update the value of an IEC 61850 UTC time (timestamp) data attribute.
 *
 * Update the value of a UTC time data attribute without handling MmsValue instances.
 *
 * This function will also check if a trigger condition is satisfied in the case when a report or GOOSE
 * control block is enabled.
 *
 * \param self the instance of IedServer to operate on.
 * \param dataAttribute the data attribute handle
 * \param value the new UTC time value of the data attribute as a Timestamp
 */
void
IedServer_updateTimestampAttributeValue(IedServer self, DataAttribute* dataAttribute, Timestamp* timestamp);

/**
 * \brief Update a quality ("q") IEC 61850 data attribute.
 *
 * This is a specialized function to handle Quality data attributes. It can be used instead of the more
 * generic IedServer_updateAttributeValue function.
 *
 * This function will also check if the quality change  (qchg) trigger condition is satisfied and inform a
 * report control block accordingly.
 *
 * \param self the instance of IedServer to operate on.
 * \param dataAttribute the data attribute handle
 * \param quality the new quality value
 *
 */
void
IedServer_updateQuality(IedServer self, DataAttribute* dataAttribute, Quality quality);

/**@}*/


void
IedServer_setLogStorage(IedServer self, const char* logRef, LogStorage logStorage);

/**
 * @defgroup IEC61850_SERVER_SETTING_GROUPS Server side setting group handling
 *
 * @{
 */

/**
 * \brief Change active setting group
 *
 * Inform the IedServer that the active setting group has changed due to an internal event.
 * Before calling this function the user should update the relevant data attributes with FC=SG!
 *
 * \param self the instance of IedServer to operate on.
 * \param sgcb the handle of the setting group control block of the setting group
 * \param newActiveSg the number of the new active setting group
 */
void
IedServer_changeActiveSettingGroup(IedServer self, SettingGroupControlBlock* sgcb, uint8_t newActiveSg);

/**
 * \brief Get the active setting group number
 *
 * \param self the instance of IedServer to operate on.
 * \param sgcb the handle of the setting group control block of the setting group
 *
 * \return the number of the active setting group
 */
uint8_t
IedServer_getActiveSettingGroup(IedServer self, SettingGroupControlBlock* sgcb);

/**
 * \brief Callback handler that is invoked when the active setting group is about to be changed by an
 *        external client.
 *
 * This function is called BEFORE the active setting group is changed. The user can reject to change the
 * active setting group by returning false.
 *
 * \param user provided parameter
 * \param sgcb the setting group control block of the setting group that is about to be changed
 * \param newActSg the new active setting group
 * \param connection the client connection that requests the change
 *
 * \return true if the change is accepted, false otherwise.
 *
 */
typedef bool (*ActiveSettingGroupChangedHandler) (void* parameter, SettingGroupControlBlock* sgcb,
        uint8_t newActSg, ClientConnection connection);

/**
 * \brief Set the callback handler for the SetActSG event
 *
 * \param self the instance of IedServer to operate on.
 * \param sgcb the handle of the setting group control block of the setting group.
 * \param handler the user provided callback handler.
 * \param parameter a user provided parameter that is passed to the control handler.
 */
void
IedServer_setActiveSettingGroupChangedHandler(IedServer self, SettingGroupControlBlock* sgcb,
        ActiveSettingGroupChangedHandler handler, void* parameter);

/**
 * \brief Callback handler that is invoked when the edit setting group is about to be changed by an
 *        external client.
 *
 * In this function the user should update all SE data attributes
 * associated with the given SettingGroupControlBlock.
 * This function is called BEFORE the active setting group is changed. The user can reject to change the
 * edit setting group by returning false. This can be used to implement RBAC.
 *
 * \param user provided parameter
 * \param sgcb the setting group control block of the setting group that is about to be changed
 * \param newEditSg the new edit setting group
 * \param connection the client connection that requests the change
 *
 * \return true if the change is accepted, false otherwise.
 *
 */
typedef bool (*EditSettingGroupChangedHandler) (void* parameter, SettingGroupControlBlock* sgcb,
        uint8_t newEditSg, ClientConnection connection);

/**
 * \brief Set the callback handler for the SetEditSG event
 *
 * \param self the instance of IedServer to operate on.
 * \param sgcb the handle of the setting group control block of the setting group.
 * \param handler the user provided callback handler.
 * \param parameter a user provided parameter that is passed to the control handler.
 */
void
IedServer_setEditSettingGroupChangedHandler(IedServer self, SettingGroupControlBlock* sgcb,
        EditSettingGroupChangedHandler handler, void* parameter);

/**
 * \brief Callback handler that is invoked when the edit setting group has been confirmed by an
 *        external client.
 *
 * \param user provided parameter
 * \param sgcb the setting group control block of the setting group that is about to be changed
 * \param editSg the edit setting group that has been confirmed
 *
 */
typedef void (*EditSettingGroupConfirmationHandler) (void* parameter, SettingGroupControlBlock* sgcb,
        uint8_t editSg);

/**
 * \brief Set the callback handler for the COnfEditSG event
 *
 * \param self the instance of IedServer to operate on.
 * \param sgcb the handle of the setting group control block of the setting group.
 * \param handler the user provided callback handler.
 * \param parameter a user provided parameter that is passed to the control handler.
 */
void
IedServer_setEditSettingGroupConfirmationHandler(IedServer self, SettingGroupControlBlock* sgcb,
        EditSettingGroupConfirmationHandler handler, void* parameter);

/**@}*/

/**
 * @defgroup IEC61850_SERVER_CONTROL Server side control model handling
 *
 * @{
 */

/**
 * \brief result code for ControlPerformCheckHandler
 */
typedef enum {
    CONTROL_ACCEPTED = -1, /** check passed */
    CONTROL_HARDWARE_FAULT = 1, /** check failed due to hardware fault */
    CONTROL_TEMPORARILY_UNAVAILABLE = 2, /** control is already selected or operated */
    CONTROL_OBJECT_ACCESS_DENIED = 3, /** check failed due to access control reason - access denied for this client or state */
    CONTROL_OBJECT_UNDEFINED = 4 /** object not visible in this security context ??? */
} CheckHandlerResult;

/**
 * \brief result codes for control handler (ControlWaitForExecutionHandler and ControlHandler)
 */
typedef enum {
    CONTROL_RESULT_FAILED = 0, /** check or operation failed */
    CONTROL_RESULT_OK = 1,     /** check or operation was successful */
    CONTROL_RESULT_WAITING = 2 /** check or operation is in progress */
} ControlHandlerResult;

/**
 * \brief Control model callback to perform the static tests (optional).
 *
 * User provided callback function for the control model. It will be invoked after
 * a control operation has been invoked by the client. This callback function is
 * intended to perform the static tests. It should check if the interlock conditions
 * are met if the interlockCheck parameter is true.
 *
 * \param parameter the parameter that was specified when setting the control handler
 * \param ctlVal the control value of the control operation.
 * \param test indicates if the operate request is a test operation
 * \param interlockCheck the interlockCheck parameter provided by the client
 * \param connection the connection object of the client connection that invoked the control operation
 *
 * \return CONTROL_ACCEPTED if the static tests had been successful, one of the error codes otherwise
 */
typedef CheckHandlerResult (*ControlPerformCheckHandler) (void* parameter, MmsValue* ctlVal, bool test, bool interlockCheck,
        ClientConnection connection);

/**
 * \brief Control model callback to perform the dynamic tests (optional).
 *
 * User provided callback function for the control model. It will be invoked after
 * a control operation has been invoked by the client. This callback function is
 * intended to perform the dynamic tests. It should check if the synchronization conditions
 * are met if the synchroCheck parameter is set to true.
 * NOTE: Since version 0.7.9 this function is intended to return immediately. If the operation
 * cannot be performed immediately the function SHOULD return CONTROL_RESULT_WAITING and the
 * handler will be invoked again later.
 *
 * \param parameter the parameter that was specified when setting the control handler
 * \param ctlVal the control value of the control operation.
 * \param test indicates if the operate request is a test operation
 * \param synchroCheck the synchroCheck parameter provided by the client
 *
 * \return CONTROL_RESULT_OK if the dynamic tests had been successful, CONTROL_RESULT_FAILED otherwise,
 *         CONTROL_RESULT_WAITING if the test is not yet finished
 */
typedef ControlHandlerResult (*ControlWaitForExecutionHandler) (void* parameter, MmsValue* ctlVal, bool test, bool synchroCheck);

/**
 * \brief Control model callback to actually perform the control operation.
 *
 * User provided callback function for the control model. It will be invoked when
 * a control operation happens (Oper). Here the user should perform the control operation
 * (e.g. by setting an digital output or switching a relay).
 * NOTE: Since version 0.7.9 this function is intended to return immediately. If the operation
 * cannot be performed immediately the function SHOULD return CONTROL_RESULT_WAITING and the
 * handler will be invoked again later.
 *
 * \param parameter the parameter that was specified when setting the control handler
 * \param ctlVal the control value of the control operation.
 * \param test indicates if the operate request is a test operation
 *
 * \return CONTROL_RESULT_OK if the control action bas been successful, CONTROL_RESULT_FAILED otherwise,
 *         CONTROL_RESULT_WAITING if the test is not yet finished
 */
typedef ControlHandlerResult (*ControlHandler) (void* parameter, MmsValue* ctlVal, bool test);

/**
 * \brief Set control handler for controllable data object
 *
 * This functions sets a user provided control handler for a data object. The data object
 * has to be an instance of a controllable CDC (Common Data Class) like e.g. SPC, DPC or APC.
 * The control handler is a callback function that will be called by the IEC server when a
 * client invokes a control operation on the data object.
 *
 * \param self the instance of IedServer to operate on.
 * \param node the controllable data object handle
 * \param handler a callback function of type ControlHandler
 * \param parameter a user provided parameter that is passed to the control handler.
 */
void
IedServer_setControlHandler(IedServer self, DataObject* node, ControlHandler handler, void* parameter);

/**
 * \brief Set a handler for a controllable data object to perform operative tests
 *
 * This functions sets a user provided handler that should perform the operative tests for a control operation.
 * Setting this handler is not required. If not set the server assumes that the checks will always be successful.
 * The handler has to return true upon a successful test of false if the test fails. In the later case the control
 * operation will be aborted.
 *
 * \param self the instance of IedServer to operate on.
 * \param node the controllable data object handle
 * \param handler a callback function of type ControlHandler
 * \param parameter a user provided parameter that is passed to the control handler.
 *
 */
void
IedServer_setPerformCheckHandler(IedServer self, DataObject* node, ControlPerformCheckHandler handler, void* parameter);

/**
 * \brief Set a handler for a controllable data object to perform dynamic tests
 *
 * This functions sets a user provided handler that should perform the dynamic tests for a control operation.
 * Setting this handler is not required. If not set the server assumes that the checks will always be successful.
 * The handler has to return true upon a successful test of false if the test fails. In the later case the control
 * operation will be aborted. If this handler is set than the server will start a new thread before calling the
 * handler. This thread will also be used to execute the ControlHandler.
 *
 * \param self the instance of IedServer to operate on.
 * \param node the controllable data object handle
 * \param handler a callback function of type ControlHandler
 * \param parameter a user provided parameter that is passed to the control handler.
 *
 */
void
IedServer_setWaitForExecutionHandler(IedServer self, DataObject* node, ControlWaitForExecutionHandler handler, void* parameter);

/**@}*/

/**
 * @defgroup IEC61850_SERVER_SVCB Server side sampled values control block (SVCB) handling
 *
 * @{
 */

/** Control block has been enabled by client */
#define IEC61850_SVCB_EVENT_ENABLE 1

/** Control block has been disabled by client */
#define IEC61850_SVCB_EVENT_DISABLE 0

/**
 * \brief callback handler for SVCB events.
 *
 * \param svcb the related SVCB instance
 * \param the event type
 * \param user defined parameter
 */
typedef void (*SVCBEventHandler) (SVControlBlock* svcb, int event, void* parameter);

/**
 * \brief Set a handler for SVCB control block events (enable/disable)
 *
 * \param self the instance of IedServer to operate on.
 * \param svcb the SVCB control block instance
 * \param handler the event handler to be used
 * \param parameter a user provided parameter that is passed to the handler.
 */
void
IedServer_setSVCBHandler(IedServer self, SVControlBlock* svcb, SVCBEventHandler handler, void* parameter);

/**@}*/

/**
 * @defgroup IEC61850_SERVER_EXTERNAL_ACCESS Handle external access to data model and access control
 *
 * @{
 */

/**
 * \brief callback handler to monitor client access to data attributes
 *
 * User provided callback function to observe (monitor) MMS client access to
 * IEC 61850 data attributes. The application can install the same handler
 * multiple times and distinguish data attributes by the dataAttribute parameter.
 *
 * \param the data attribute that has been written by an MMS client.
 * \param connection the connection object of the client connection that invoked the write operation
 */
typedef void (*AttributeChangedHandler) (DataAttribute* dataAttribute, ClientConnection connection);

/**
 * \deprecated Please use IedServer_handleWriteAccess instead!
 * \brief Install an observer for a data attribute.
 *
 * This instructs the server to monitor write attempts by MMS clients to specific
 * data attributes. If a successful write attempt happens the server will call
 * the provided callback function to inform the application. This can be used to
 * monitor important configuration values.
 *
 * \param self the instance of IedServer to operate on.
 * \param dataAttribute the data attribute to monitor
 * \param handler the callback function that is invoked if a client has written to
 *        the monitored data attribute.
 */
void
IedServer_observeDataAttribute(IedServer self, DataAttribute* dataAttribute,
        AttributeChangedHandler handler);

/***************************************************************************
 * Access control
 **************************************************************************/

/**
 * \brief callback handler to intercept/control client access to data attributes
 *
 * User provided callback function to intercept/control MMS client access to
 * IEC 61850 data attributes. The application can install the same handler
 * multiple times and distinguish data attributes by the dataAttribute parameter.
 * This handler can be used to perform write access control do data attributes.
 * One application can be to allow write access only from a specific client. Another
 * application could be to check if the value is in the allowed range before the write
 * is accepted.
 *
 * \param the data attribute that has been written by an MMS client.
 * \param the value the client want to write to the data attribute
 * \param connection the connection object of the client connection that invoked the write operation
 * \param parameter the user provided parameter
 *
 * \return true if access is accepted, false if access is denied.
 */
typedef MmsDataAccessError
(*WriteAccessHandler) (DataAttribute* dataAttribute, MmsValue* value, ClientConnection connection, void* parameter);

/**
 * \brief Install a WriteAccessHandler for a data attribute.
 *
 * This instructs the server to monitor write attempts by MMS clients to specific
 * data attributes. If a client tries to write to the monitored data attribute the
 * handler is invoked. The handler can decide if the write access will be allowed
 * or denied. If a WriteAccessHandler is set for a specific data attribute - the
 * default write access policy will not be performed for that data attribute.
 *
 * \param self the instance of IedServer to operate on.
 * \param dataAttribute the data attribute to monitor
 * \param handler the callback function that is invoked if a client tries to write to
 *        the monitored data attribute.
 * \param parameter a user provided parameter that is passed to the WriteAccessHandler when called.
 */
void
IedServer_handleWriteAccess(IedServer self, DataAttribute* dataAttribute,
        WriteAccessHandler handler, void* parameter);

typedef enum {
    ACCESS_POLICY_ALLOW,
    ACCESS_POLICY_DENY
} AccessPolicy;

/**
 * \brief Change the default write access policy for functional constraint data with a specific FC.
 *
 * \param self the instance of IedServer to operate on.
 * \param fc the FC for which to change the default write access policy.
 * \param policy the new policy to apply.
 *
 */
void
IedServer_setWriteAccessPolicy(IedServer self, FunctionalConstraint fc, AccessPolicy policy);

/**@}*/

/**@}*/

#ifdef __cplusplus
}
#endif

#endif /* IED_SERVER_API_H_ */
