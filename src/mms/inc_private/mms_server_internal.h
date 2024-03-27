/*
 *  mms_server_internal.h
 *
 *  Copyright 2013-2022 Michael Zillgith
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

#ifndef MMS_SERVER_INTERNAL_H_
#define MMS_SERVER_INTERNAL_H_

#include "libiec61850_platform_includes.h"

#include "MmsPdu.h"
#include "mms_common.h"
#include "mms_server_connection.h"
#include "mms_device_model.h"
#include "mms_common_internal.h"
#include "stack_config.h"
#include "mms_server_libinternal.h"


#include "byte_buffer.h"
#include "string_utilities.h"
#include "map.h"
#include "hal_thread.h"

#include "ber_encoder.h"
#include "ber_decode.h"

#if (MMS_OBTAIN_FILE_SERVICE == 1)
#include "hal_filesystem.h"
#endif


#ifndef DEBUG_MMS_SERVER
#define DEBUG_MMS_SERVER 0
#endif

#ifndef MMS_STATUS_SERVICE
#define MMS_STATUS_SERVICE 1
#endif

#ifndef MMS_IDENTIFY_SERVICE
#define MMS_IDENTIFY_SERVICE 1
#endif

#ifndef MMS_FILE_SERVICE
#define MMS_FILE_SERVICE 1
#endif

#ifndef CONFIG_MMS_SERVER_MAX_GET_FILE_TASKS
#define CONFIG_MMS_SERVER_MAX_GET_FILE_TASKS 5
#endif


#if (MMS_OBTAIN_FILE_SERVICE == 1)

#define MMS_FILE_UPLOAD_STATE_NOT_USED 0
#define MMS_FILE_UPLOAD_STATE_READY 1
#define MMS_FILE_UPLOAD_STATE_FILE_OPEN_SENT 2

#define MMS_FILE_UPLOAD_STATE_SEND_FILE_READ 3
#define MMS_FILE_UPLOAD_STATE_FILE_READ_SENT 4

#define MMS_FILE_UPLOAD_STATE_SEND_FILE_CLOSE 5
#define MMS_FILE_UPLOAD_STATE_FILE_CLOSE_SENT 6

#define MMS_FILE_UPLOAD_STATE_SEND_OBTAIN_FILE_ERROR_SOURCE 8
#define MMS_FILE_UPLOAD_STATE_SEND_OBTAIN_FILE_ERROR_DESTINATION 9
#define MMS_FILE_UPLOAD_STATE_SEND_OBTAIN_FILE_RESPONSE 10

#define MMS_FILE_UPLOAD_STATE_INTERRUPTED 11

typedef struct sMmsObtainFileTask* MmsObtainFileTask;

struct sMmsObtainFileTask {
    MmsServerConnection connection;
    uint32_t lastRequestInvokeId;
    uint32_t obtainFileRequestInvokeId;
    FileHandle fileHandle;
    char destinationFilename[256];
    uint64_t nextTimeout;
    int32_t frmsId;
    int state;
#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore taskLock;
#endif
};

#endif /* (MMS_OBTAIN_FILE_SERVICE == 1) */

struct sMmsServer {

    LinkedList /*<IsoServer>*/ isoServerList;

    MmsDevice* device;

    MmsReadVariableHandler readHandler;
    void* readHandlerParameter;

    MmsReadAccessHandler readAccessHandler;
    void* readAccessHandlerParameter;

    MmsWriteVariableHandler writeHandler;
    void* writeHandlerParameter;

    MmsListAccessHandler listAccessHandler;
    void* listAccessHandlerParameter;

    MmsConnectionHandler connectionHandler;
    void* connectionHandlerParameter;

    MmsNamedVariableListAccessHandler variableListAccessHandler;
    void* variableListAccessHandlerParameter;

    MmsReadJournalHandler readJournalHandler;
    void* readJournalHandlerParameter;

    MmsGetNameListHandler getNameListHandler;
    void* getNameListHandlerParameter;

    AcseAuthenticator authenticator;
    void* authenticatorParameter;

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore openConnectionsLock;
#endif

    Map openConnections;
    Map valueCaches;

    bool blockRequests;

    ByteBuffer* transmitBuffer; /* global buffer for encoding reports, delayed responses... */
#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore transmitBufferMutex;
#endif

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore modelMutex;
#endif

#if (MMS_STATUS_SERVICE == 1)
    int vmdLogicalStatus;
    int vmdPhysicalStatus;
    MmsStatusRequestListener statusRequestListener;
    void* statusRequestListenerParameter;
#endif /* MMS_STATUS_SERVICE == 1 */

#if (MMS_IDENTIFY_SERVICE == 1)
    char* vendorName;
    char* modelName;
    char* revision;
#endif /* MMS_IDENTIFY_SERVICE == 1 */

#if (MMS_OBTAIN_FILE_SERVICE == 1)
    MmsObtainFileHandler obtainFileHandler;
    void* obtainFileHandlerParameter;

    MmsGetFileCompleteHandler getFileCompleteHandler;
    void* getFileCompleteHandlerParameter;

    struct sMmsObtainFileTask fileUploadTasks[CONFIG_MMS_SERVER_MAX_GET_FILE_TASKS];
#endif

#if (MMS_FILE_SERVICE == 1)
    MmsFileAccessHandler fileAccessHandler;
    void* fileAccessHandlerParameter;
#endif

#if (CONFIG_SET_FILESTORE_BASEPATH_AT_RUNTIME == 1)
    char* filestoreBasepath;
#endif

#if (CONFIG_MMS_SERVER_CONFIG_SERVICES_AT_RUNTIME == 1)
    int maxConnections;
    bool fileServiceEnabled;
    bool dynamicVariableListServiceEnabled;
    int maxDataSetEntries;
    bool journalServiceEnabled;
    int maxAssociationSpecificDataSets;
    int maxDomainSpecificDataSets;
#endif /* (CONFIG_SET_FILESTORE_BASEPATH_AT_RUNTIME == 1) */

};

struct sMmsServerConnection {
    int maxServOutstandingCalling;
    int maxServOutstandingCalled;
    int dataStructureNestingLevel;
    uint8_t negotiatedParameterCBC[2];
    uint32_t maxPduSize; /* local detail */
    IsoConnection isoConnection;
    MmsServer server;
    uint32_t lastInvokeId;

#if (MMS_OBTAIN_FILE_SERVICE == 1)
    uint32_t lastRequestInvokeId; /* only used by obtainFile service */
#endif

#if (MMS_DYNAMIC_DATA_SETS == 1)
    LinkedList /*<MmsNamedVariableList>*/namedVariableLists; /* aa-specific named variable lists */
#endif

#if (MMS_FILE_SERVICE == 1)
    int32_t nextFrsmId;
    MmsFileReadStateMachine frsms[CONFIG_MMS_MAX_NUMBER_OF_OPEN_FILES_PER_CONNECTION];
#endif
};

#if (MMS_OBTAIN_FILE_SERVICE == 1)
LIB61850_INTERNAL MmsObtainFileTask
MmsServer_getObtainFileTask(MmsServer self);

LIB61850_INTERNAL void
mmsServer_fileUploadTask(MmsServer self, MmsObtainFileTask task, int taskState);
#endif

LIB61850_INTERNAL ByteBuffer*
MmsServer_reserveTransmitBuffer(MmsServer self);

LIB61850_INTERNAL void
MmsServer_releaseTransmitBuffer(MmsServer self);

LIB61850_INTERNAL void
MmsServer_callConnectionHandler(MmsServer self, MmsServerConnection connection);

/* write_out function required for ASN.1 encoding */
LIB61850_INTERNAL int
mmsServer_write_out(const void *buffer, size_t size, void *app_key);



LIB61850_INTERNAL void
mmsServer_handleDeleteNamedVariableListRequest(MmsServerConnection connection,
        uint8_t* buffer, int bufPos, int maxBufPos,
        uint32_t invokeId,
        ByteBuffer* response);

LIB61850_INTERNAL void
mmsServer_handleGetNamedVariableListAttributesRequest(
		MmsServerConnection connection,
		uint8_t* buffer, int bufPos, int maxBufPos,
		uint32_t invokeId,
		ByteBuffer* response);

LIB61850_INTERNAL void
mmsServer_handleReadRequest(
		MmsServerConnection connection,
		uint8_t* buffer, int bufPos, int maxBufPos,
		uint32_t invokeId,
		ByteBuffer* response);

LIB61850_INTERNAL MmsPdu_t*
mmsServer_createConfirmedResponse(uint32_t invokeId);

LIB61850_INTERNAL void
mmsMsg_createInitiateErrorPdu(ByteBuffer* response, uint8_t initiateErrorCode);

LIB61850_INTERNAL void
mmsServer_createServiceErrorPduWithServiceSpecificInfo(uint32_t invokeId, ByteBuffer* response,
        MmsError errorType, uint8_t* serviceSpecificInfo, int serviceSpecficInfoLength);

LIB61850_INTERNAL void
mmsServer_writeConcludeResponsePdu(ByteBuffer* response);

LIB61850_INTERNAL void
mmsServer_handleInitiateRequest (
        MmsServerConnection self,
        uint8_t* buffer, int bufPos, int maxBufPos,
        ByteBuffer* response);

LIB61850_INTERNAL int
mmsServer_handleGetVariableAccessAttributesRequest(
		MmsServerConnection connection,
		uint8_t* buffer, int bufPos, int maxBufPos,
		uint32_t invokeId,
		ByteBuffer* response);

LIB61850_INTERNAL void
mmsServer_handleDefineNamedVariableListRequest(
        MmsServerConnection connection,
        uint8_t* buffer, int bufPos, int maxBufPos,
        uint32_t invokeId,
        ByteBuffer* response);

LIB61850_INTERNAL void
mmsServer_handleGetNameListRequest(
		MmsServerConnection connection,
		uint8_t* buffer, int bufPos, int maxBufPos,
		uint32_t invokeId,
		ByteBuffer* response);

LIB61850_INTERNAL void
mmsServer_handleWriteRequest(
		MmsServerConnection connection,
		uint8_t* buffer, int bufPos, int maxBufPos,
		uint32_t invokeId,
		ByteBuffer* response);

LIB61850_INTERNAL void
mmsServer_handleIdentifyRequest(
        MmsServerConnection connection,
        uint32_t invokeId,
        ByteBuffer* response);

LIB61850_INTERNAL void
mmsServer_handleStatusRequest(
        MmsServerConnection connection,
        uint8_t* requestBuffer,
        int bufPos,
        uint32_t invokeId,
        ByteBuffer* response);

LIB61850_INTERNAL void
mmsServer_handleReadJournalRequest(
        MmsServerConnection connection,
        uint8_t* requestBuffer,
        int bufPos, int maxBufPos,
        uint32_t invokeId,
        ByteBuffer* response);

LIB61850_INTERNAL void
mmsServer_handleFileDirectoryRequest(
        MmsServerConnection connection,
        uint8_t* buffer, int bufPos, int maxBufPos,
        uint32_t invokeId,
        ByteBuffer* response);

LIB61850_INTERNAL void
mmsServer_handleFileOpenRequest(
        MmsServerConnection connection,
        uint8_t* buffer, int bufPos, int maxBufPos,
        uint32_t invokeId,
        ByteBuffer* response);

LIB61850_INTERNAL void
mmsServer_handleFileDeleteRequest(
        MmsServerConnection connection,
        uint8_t* buffer, int bufPos, int maxBufPos,
        uint32_t invokeId,
        ByteBuffer* response);

LIB61850_INTERNAL void
mmsServer_handleFileRenameRequest(
        MmsServerConnection connection,
        uint8_t* buffer, int bufPos, int maxBufPos,
        uint32_t invokeId,
        ByteBuffer* response);

LIB61850_INTERNAL void
mmsServer_handleFileReadRequest(
        MmsServerConnection connection,
        uint8_t* buffer, int bufPos, int maxBufPos,
        uint32_t invokeId,
        ByteBuffer* response);

LIB61850_INTERNAL void
mmsServer_handleFileCloseRequest(
        MmsServerConnection connection,
        uint8_t* buffer, int bufPos, int maxBufPos,
        uint32_t invokeId,
        ByteBuffer* response);

LIB61850_INTERNAL void
mmsServer_handleObtainFileRequest(
        MmsServerConnection connection,
        uint8_t* buffer, int bufPos, int maxBufPos,
        uint32_t invokeId,
        ByteBuffer* response);

LIB61850_INTERNAL void
mmsServerConnection_stopFileUploadTasks(MmsServerConnection self);

LIB61850_INTERNAL bool
mmsServer_isIndexAccess(AlternateAccess_t* alternateAccess);

LIB61850_INTERNAL bool
mmsServer_isComponentAccess(AlternateAccess_t* alternateAccess);

LIB61850_INTERNAL bool
mmsServer_isAccessToArrayComponent(AlternateAccess_t* alternateAccess);

LIB61850_INTERNAL MmsValue*
mmsServer_getComponentOfArrayElement(AlternateAccess_t* alternateAccess, MmsVariableSpecification* namedVariable,
        MmsValue* structuredValue, char* componentId);

LIB61850_INTERNAL int
mmsServer_getLowIndex(AlternateAccess_t* alternateAccess);

LIB61850_INTERNAL int
mmsServer_getNumberOfElements(AlternateAccess_t* alternateAccess);

LIB61850_INTERNAL MmsNamedVariableList
mmsServer_getNamedVariableListWithName(LinkedList namedVariableLists, const char* variableListName);

LIB61850_INTERNAL void
mmsServer_deleteVariableList(LinkedList namedVariableLists, char* variableListName);

LIB61850_INTERNAL MmsDataAccessError
mmsServer_setValue(MmsServer self, MmsDomain* domain, char* itemId, MmsValue* value,
        MmsServerConnection connection);

LIB61850_INTERNAL MmsDataAccessError
mmsServer_setValueEx(MmsServer self, MmsDomain* domain, char* itemId, MmsValue* value,
        MmsServerConnection connection, int arrayIdx, const char* componentId);

/**
 * \brief Get the current value of a variable in the server data model
 *
 * \param isDirectAccess the access is result of a direct single read access to the variable and no part of broader read request
 */
LIB61850_INTERNAL MmsValue*
mmsServer_getValue(MmsServer self, MmsDomain* domain, char* itemId, MmsServerConnection connection, bool isDirectAccess);

LIB61850_INTERNAL bool
mmsServer_checkListAccess(MmsServer self, MmsGetNameListType listType, MmsDomain* domain, char* itemId, MmsServerConnection connection);

LIB61850_INTERNAL void
mmsServer_createMmsWriteResponse(MmsServerConnection connection,
        uint32_t invokeId, ByteBuffer* response, int numberOfItems, MmsDataAccessError* accessResults);

LIB61850_INTERNAL MmsError
mmsServer_callVariableListChangedHandler(MmsVariableListAccessType accessType, MmsVariableListType listType, MmsDomain* domain,
        char* listName, MmsServerConnection connection);

#endif /* MMS_SERVER_INTERNAL_H_ */
