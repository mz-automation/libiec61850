/*
 *  mms_server_internal.h
 *
 *  Copyright 2013, 2014 Michael Zillgith
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

#ifndef MMS_SERVER_INTERNAL_H_
#define MMS_SERVER_INTERNAL_H_

#include "libiec61850_platform_includes.h"

#include "MmsPdu.h"
#include "mms_common.h"
#include "mms_indication.h"
#include "mms_server_connection.h"
#include "mms_device_model.h"
#include "mms_common_internal.h"
#include "stack_config.h"
#include "mms_server.h"


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
};

#endif /* (MMS_OBTAIN_FILE_SERVICE == 1) */

struct sMmsServer {
    IsoServer isoServer;
    MmsDevice* device;

    MmsReadVariableHandler readHandler;
    void* readHandlerParameter;

    MmsReadAccessHandler readAccessHandler;
    void* readAccessHandlerParameter;

    MmsWriteVariableHandler writeHandler;
    void* writeHandlerParameter;

    MmsConnectionHandler connectionHandler;
    void* connectionHandlerParameter;

    MmsNamedVariableListChangedHandler variableListChangedHandler; //TODO this is only required if dynamic data sets are supported!
    void* variableListChangedHandlerParameter;

    Map openConnections;
    Map valueCaches;
    bool isLocked;

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

};

struct sMmsServerConnection {
    int maxServOutstandingCalling;
    int maxServOutstandingCalled;
    int dataStructureNestingLevel;
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
MmsObtainFileTask
MmsServer_getObtainFileTask(MmsServer self);

void
mmsServer_fileUploadTask(MmsServer self, MmsObtainFileTask task);
#endif

ByteBuffer*
MmsServer_reserveTransmitBuffer(MmsServer self);

void
MmsServer_releaseTransmitBuffer(MmsServer self);

/* write_out function required for ASN.1 encoding */
int
mmsServer_write_out(const void *buffer, size_t size, void *app_key);



void
mmsServer_handleDeleteNamedVariableListRequest(MmsServerConnection connection,
        uint8_t* buffer, int bufPos, int maxBufPos,
        uint32_t invokeId,
        ByteBuffer* response);

void
mmsServer_handleGetNamedVariableListAttributesRequest(
		MmsServerConnection connection,
		uint8_t* buffer, int bufPos, int maxBufPos,
		uint32_t invokeId,
		ByteBuffer* response);

void
mmsServer_handleReadRequest(
		MmsServerConnection connection,
		uint8_t* buffer, int bufPos, int maxBufPos,
		uint32_t invokeId,
		ByteBuffer* response);

MmsPdu_t*
mmsServer_createConfirmedResponse(uint32_t invokeId);

void
mmsMsg_createServiceErrorPdu(uint32_t invokeId, ByteBuffer* response, MmsError errorType);

void
mmsServer_createServiceErrorPduWithServiceSpecificInfo(uint32_t invokeId, ByteBuffer* response,
        MmsError errorType, uint8_t* serviceSpecificInfo, int serviceSpecficInfoLength);

void
mmsServer_writeConcludeResponsePdu(ByteBuffer* response);

void
mmsServer_handleInitiateRequest (
        MmsServerConnection self,
        uint8_t* buffer, int bufPos, int maxBufPos,
        ByteBuffer* response);

int
mmsServer_handleGetVariableAccessAttributesRequest(
		MmsServerConnection connection,
		uint8_t* buffer, int bufPos, int maxBufPos,
		uint32_t invokeId,
		ByteBuffer* response);

void
mmsServer_handleDefineNamedVariableListRequest(
        MmsServerConnection connection,
        uint8_t* buffer, int bufPos, int maxBufPos,
        uint32_t invokeId,
        ByteBuffer* response);

void
mmsServer_handleGetNameListRequest(
		MmsServerConnection connection,
		uint8_t* buffer, int bufPos, int maxBufPos,
		uint32_t invokeId,
		ByteBuffer* response);

void
mmsServer_handleWriteRequest(
		MmsServerConnection connection,
		uint8_t* buffer, int bufPos, int maxBufPos,
		uint32_t invokeId,
		ByteBuffer* response);

void
mmsServer_handleIdentifyRequest(
        MmsServerConnection connection,
        uint32_t invokeId,
        ByteBuffer* response);

void
mmsServer_handleStatusRequest(
        MmsServerConnection connection,
        uint8_t* requestBuffer,
        int bufPos,
        uint32_t invokeId,
        ByteBuffer* response);

void
mmsServer_handleReadJournalRequest(
        MmsServerConnection connection,
        uint8_t* requestBuffer,
        int bufPos, int maxBufPos,
        uint32_t invokeId,
        ByteBuffer* response);

void
mmsServer_handleFileDirectoryRequest(
        MmsServerConnection connection,
        uint8_t* buffer, int bufPos, int maxBufPos,
        uint32_t invokeId,
        ByteBuffer* response);

void
mmsServer_handleFileOpenRequest(
        MmsServerConnection connection,
        uint8_t* buffer, int bufPos, int maxBufPos,
        uint32_t invokeId,
        ByteBuffer* response);

void
mmsServer_handleFileDeleteRequest(
        MmsServerConnection connection,
        uint8_t* buffer, int bufPos, int maxBufPos,
        uint32_t invokeId,
        ByteBuffer* response);

void
mmsServer_handleFileRenameRequest(
        MmsServerConnection connection,
        uint8_t* buffer, int bufPos, int maxBufPos,
        uint32_t invokeId,
        ByteBuffer* response);

void
mmsServer_handleFileReadRequest(
        MmsServerConnection connection,
        uint8_t* buffer, int bufPos, int maxBufPos,
        uint32_t invokeId,
        ByteBuffer* response);

void
mmsServer_handleFileCloseRequest(
        MmsServerConnection connection,
        uint8_t* buffer, int bufPos, int maxBufPos,
        uint32_t invokeId,
        ByteBuffer* response);

void
mmsServer_handleObtainFileRequest(
        MmsServerConnection connection,
        uint8_t* buffer, int bufPos, int maxBufPos,
        uint32_t invokeId,
        ByteBuffer* response);

int
mmsServer_isIndexAccess(AlternateAccess_t* alternateAccess);

int
mmsServer_getLowIndex(AlternateAccess_t* alternateAccess);

int
mmsServer_getNumberOfElements(AlternateAccess_t* alternateAccess);

MmsNamedVariableList
mmsServer_getNamedVariableListWithName(LinkedList namedVariableLists, const char* variableListName);

void
mmsServer_deleteVariableList(LinkedList namedVariableLists, char* variableListName);

MmsDataAccessError
mmsServer_setValue(MmsServer self, MmsDomain* domain, char* itemId, MmsValue* value,
        MmsServerConnection connection);

MmsValue*
mmsServer_getValue(MmsServer self, MmsDomain* domain, char* itemId, MmsServerConnection connection);

int
mmsServer_createMmsWriteResponse(MmsServerConnection connection,
        int invokeId, ByteBuffer* response, int numberOfItems, MmsDataAccessError* accessResults);

void
mmsMsg_createMmsRejectPdu(uint32_t* invokeId, int reason, ByteBuffer* response);

MmsError
mmsServer_callVariableListChangedHandler(bool create, MmsVariableListType listType, MmsDomain* domain,
        char* listName, MmsServerConnection connection);

#endif /* MMS_SERVER_INTERNAL_H_ */
