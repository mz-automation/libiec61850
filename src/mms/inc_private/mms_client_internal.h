/*
 *  mms_msg_internal.h
 *
 *  Copyright 2013-2018 Michael Zillgith
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

#ifndef MMS_MSG_INTERNAL_H_
#define MMS_MSG_INTERNAL_H_

#include "MmsPdu.h"
#include "linked_list.h"
#include "mms_client_connection.h"
#include "iso_client_connection.h"
#include "ber_decode.h"

#include "hal_thread.h"

#include "mms_common_internal.h"

#ifndef CONFIG_MMS_RAW_MESSAGE_LOGGING
#define CONFIG_MMS_RAW_MESSAGE_LOGGING 0
#endif

#ifndef DEBUG_MMS_CLIENT
#define DEBUG_MMS_CLIENT 0
#endif

#if 0
typedef enum {
	MMS_CON_IDLE,
	MMS_CON_WAITING,
	MMS_CON_ASSOCIATION_FAILED,
	MMS_CON_ASSOCIATED,
	MMS_CON_RESPONSE_PENDING
} ConnectionState;
#endif

#define CONCLUDE_STATE_CONNECTION_ACTIVE 0
#define CONCLUDE_STATE_REQUESTED 1
#define CONCLUDE_STATE_REJECTED 2
#define CONCLUDE_STATE_ACCEPTED 3

typedef enum {
    MMS_CALL_TYPE_NONE,
    MMS_CALL_TYPE_READ_VARIABLE,
    MMS_CALL_TYPE_READ_MULTIPLE_VARIABLES,
    MMS_CALL_TYPE_WRITE_VARIABLE,
    MMS_CALL_TYPE_WRITE_MULTIPLE_VARIABLES,
    MMS_CALL_TYPE_READ_NVL_DIRECTORY,
    MMS_CALL_TYPE_DEFINE_NVL,
    MMS_CALL_TYPE_DELETE_NVL,
    MMS_CALL_TYPE_GET_VAR_ACCESS_ATTR,
    MMS_CALL_TYPE_GET_SERVER_STATUS,
    MMS_CALL_TYPE_IDENTIFY,
    MMS_CALL_TYPE_READ_JOURNAL,
    MMS_CALL_TYPE_GET_NAME_LIST,
    MMS_CALL_TYPE_FILE_OPEN,
    MMS_CALL_TYPE_FILE_READ,
    MMS_CALL_TYPE_FILE_CLOSE,
    MMS_CALL_TYPE_FILE_DELETE,
    MMS_CALL_TYPE_FILE_RENAME,
    MMS_CALL_TYPE_OBTAIN_FILE,
    MMS_CALL_TYPE_GET_FILE_DIR
} eMmsOutstandingCallType;

struct sMmsOutstandingCall
{
    bool isUsed;
    uint32_t invokeId;
    eMmsOutstandingCallType type;
    void* userCallback;
    void* userParameter;
    void* internalParameter;
    uint64_t timeout;
};

typedef struct sMmsOutstandingCall* MmsOutstandingCall;

/* private instance variables */
struct sMmsConnection {
    Semaphore lastInvokeIdLock;
    uint32_t lastInvokeId;

	Semaphore outstandingCallsLock;
	MmsOutstandingCall outstandingCalls;

	uint32_t requestTimeout;
	uint32_t connectTimeout;

	IsoClientConnection isoClient;

#if (CONFIG_MMS_THREADLESS_STACK == 0)
	Thread connectionHandlingThread;
	bool createThread;
	bool connectionThreadRunning;
#endif

	volatile MmsConnectionState connectionState;
	Semaphore associationStateLock;

	MmsConnectionParameters parameters;
	IsoConnectionParameters isoParameters;

	MmsConnectionStateChangedHandler stateChangedHandler;
	void* stateChangedHandlerParameter;

	MmsInformationReportHandler reportHandler;
	void* reportHandlerParameter;

	MmsConnectionLostHandler connectionLostHandler;
	void* connectionLostHandlerParameter;

	MmsConnection_ConcludeAbortHandler concludeHandler;
	void* concludeHandlerParameter;
	uint64_t concludeTimeout;

#if (CONFIG_MMS_RAW_MESSAGE_LOGGING == 1)
	void* rawMmsMessageHandler;
	void* rawMmsMessageHandlerParameter;
#endif

#if (MMS_OBTAIN_FILE_SERVICE == 1)
    int32_t nextFrsmId;
    MmsFileReadStateMachine frsms[CONFIG_MMS_MAX_NUMBER_OF_OPEN_FILES_PER_CONNECTION];

#if (CONFIG_SET_FILESTORE_BASEPATH_AT_RUNTIME == 1)
    char* filestoreBasepath;
#endif

#endif /* (MMS_OBTAIN_FILE_SERVICE == 1) */
};


/**
 * MMS Object class enumeration type
 */
typedef enum {
	MMS_OBJECT_CLASS_NAMED_VARIABLE = 0,
	MMS_OBJECT_CLASS_NAMED_VARIABLE_LIST = 2,
	MMS_OBJECT_CLASS_JOURNAL = 8,
	MMS_OBJECT_CLASS_DOMAIN = 9
} MmsObjectClass;

char*
MmsConnection_getFilestoreBasepath(MmsConnection self);

MmsValue*
mmsClient_parseListOfAccessResults(AccessResult_t** accessResultList, int listSize, bool createArray);

uint32_t
mmsClient_getInvokeId(ConfirmedResponsePdu_t* confirmedResponse);

int
mmsClient_write_out(void *buffer, size_t size, void *app_key);

void
mmsClient_createInitiateRequest(MmsConnection self, ByteBuffer* writeBuffer);

MmsPdu_t*
mmsClient_createConfirmedRequestPdu(uint32_t invokeId);

AlternateAccess_t*
mmsClient_createAlternateAccess(uint32_t index, uint32_t elementCount);

void
mmsClient_deleteAlternateAccess(AlternateAccess_t* alternateAccess);

void
mmsClient_deleteAlternateAccessIndexComponent(AlternateAccess_t* alternateAccess);

AlternateAccess_t*
mmsClient_createAlternateAccessIndexComponent(uint32_t index, const char* componentName);

int
mmsClient_createMmsGetNameListRequestVMDspecific(long invokeId, ByteBuffer* writeBuffer, const char* continueAfter);

bool
mmsClient_parseGetNameListResponse(LinkedList* nameList, ByteBuffer* message);

int
mmsClient_createGetNameListRequestDomainOrVMDSpecific(long invokeId, const char* domainName,
		ByteBuffer* writeBuffer, MmsObjectClass objectClass, const char* continueAfter);

MmsValue*
mmsClient_parseReadResponse(ByteBuffer* message, uint32_t* invokeId, bool createArray);

int
mmsClient_createReadRequest(uint32_t invokeId, const char* domainId, const char* itemId, ByteBuffer* writeBuffer);

int
mmsClient_createReadRequestAlternateAccessIndex(uint32_t invokeId, const char* domainId, const char* itemId,
		uint32_t index, uint32_t elementCount, ByteBuffer* writeBuffer);

int
mmsClient_createReadRequestAlternateAccessSingleIndexComponent(uint32_t invokeId, const char* domainId, const char* itemId,
        uint32_t index, const char* component, ByteBuffer* writeBuffer);

int
mmsClient_createReadRequestMultipleValues(uint32_t invokeId, const char* domainId, LinkedList /*<char*>*/ items,
		ByteBuffer* writeBuffer);

int
mmsClient_createReadNamedVariableListRequest(uint32_t invokeId, const char* domainId, const char* itemId,
		ByteBuffer* writeBuffer, bool specWithResult);

int
mmsClient_createReadAssociationSpecificNamedVariableListRequest(
		uint32_t invokeId,
		const char* itemId,
		ByteBuffer* writeBuffer,
		bool specWithResult);

void
mmsClient_createGetNamedVariableListAttributesRequest(uint32_t invokeId, ByteBuffer* writeBuffer,
		const char* domainId, const char* listNameId);

void
mmsClient_createGetNamedVariableListAttributesRequestAssociationSpecific(uint32_t invokeId,
        ByteBuffer* writeBuffer, const char* listNameId);

LinkedList
mmsClient_parseGetNamedVariableListAttributesResponse(ByteBuffer* message, uint32_t* invokeId,
		bool* /*OUT*/ deletable);

int
mmsClient_createGetVariableAccessAttributesRequest(
        uint32_t invokeId,
        const char* domainId, const char* itemId,
		ByteBuffer* writeBuffer);

MmsVariableSpecification*
mmsClient_parseGetVariableAccessAttributesResponse(ByteBuffer* message, uint32_t* invokeId);

MmsDataAccessError
mmsClient_parseWriteResponse(ByteBuffer* message, int32_t bufPos, MmsError* mmsError);

void
mmsClient_parseWriteMultipleItemsResponse(ByteBuffer* message, int32_t bufPos, MmsError* mmsError,
        int itemCount, LinkedList* accessResults);

int
mmsClient_createWriteRequest(uint32_t invokeId, const char* domainId, const char* itemId, MmsValue* value,
		ByteBuffer* writeBuffer);

int
mmsClient_createWriteMultipleItemsRequest(uint32_t invokeId, const char* domainId, LinkedList itemIds, LinkedList values,
        ByteBuffer* writeBuffer);

int
mmsClient_createWriteRequestNamedVariableList(uint32_t invokeId, bool isAssociationSpecific, const char* domainId, const char* itemId,
        LinkedList values, ByteBuffer* writeBuffer);

int
mmsClient_createWriteRequestArray(uint32_t invokeId, const char* domainId, const char* itemId,
        int startIndex, int elementCount,
        MmsValue* value, ByteBuffer* writeBuffer);

int
mmsClient_createWriteRequestAlternateAccessSingleIndexComponent(uint32_t invokeId, const char* domainId, const char* itemId,
        uint32_t arrayIndex, const char* component,
        MmsValue* value,
        ByteBuffer* writeBuffer);

void
mmsClient_createDefineNamedVariableListRequest(uint32_t invokeId, ByteBuffer* writeBuffer,
		const char* domainId, const char* listNameId, LinkedList /*<char*>*/ listOfVariables,
		bool associationSpecific);

bool
mmsClient_parseDefineNamedVariableResponse(ByteBuffer* message, uint32_t* invokeId);

void
mmsClient_createDeleteNamedVariableListRequest(long invokeId, ByteBuffer* writeBuffer,
        const char* domainId, const char* listNameId);

bool
mmsClient_parseDeleteNamedVariableListResponse(ByteBuffer* message, uint32_t* invokeId);

void
mmsClient_createDeleteAssociationSpecificNamedVariableListRequest(
		long invokeId,
		ByteBuffer* writeBuffer,
		const char* listNameId);

void
mmsClient_createIdentifyRequest(uint32_t invokeId, ByteBuffer* request);

bool
mmsClient_parseIdentifyResponse(MmsConnection self, ByteBuffer* response, uint32_t bufPos, uint32_t invokeId, MmsConnection_IdentifyHandler handler, void* parameter);

void
mmsClient_createStatusRequest(uint32_t invokeId, ByteBuffer* request, bool extendedDerivation);

bool
mmsClient_parseStatusResponse(MmsConnection self, ByteBuffer* response, int bufPos, int* vmdLogicalStatus, int* vmdPhysicalStatus);

void
mmsClient_createFileOpenRequest(uint32_t invokeId, ByteBuffer* request, const char* fileName, uint32_t initialPosition);

void
mmsClient_createFileReadRequest(uint32_t invokeId, ByteBuffer* request, int32_t frsmId);

void
mmsClient_createFileCloseRequest(uint32_t invokeId, ByteBuffer* request, int32_t frsmId);

void
mmsClient_createFileRenameRequest(uint32_t invokeId, ByteBuffer* request, const char* currentFileName, const char* newFileName);

void
mmsClient_createObtainFileRequest(uint32_t invokeId, ByteBuffer* request, const char* sourceFile, const char* destinationFile);

void
mmsClient_createFileDeleteRequest(uint32_t invokeId, ByteBuffer* request, const char* fileName);

void
mmsClient_createFileDirectoryRequest(uint32_t invokeId, ByteBuffer* request, const char* fileSpecification, const char* continueAfter);

bool
mmsClient_parseFileDirectoryResponse(ByteBuffer* response, int bufPos, uint32_t invokeId, MmsConnection_FileDirectoryHandler handler, void* parameter);

bool
mmsClient_parseInitiateResponse(MmsConnection self, ByteBuffer* response);

int
mmsClient_createConcludeRequest(MmsConnection self, ByteBuffer* message);

int
mmsClient_createMmsGetNameListRequestAssociationSpecific(long invokeId, ByteBuffer* writeBuffer,
		const char* continueAfter);

void
mmsClient_createReadJournalRequestWithTimeRange(uint32_t invokeId, ByteBuffer* request, const char* domainId, const char* itemId,
        MmsValue* startingTime, MmsValue* endingTime);

void
mmsClient_createReadJournalRequestStartAfter(uint32_t invokeId, ByteBuffer* request, const char* domainId, const char* itemId,
        MmsValue* timeSpecification, MmsValue* entrySpecification);

bool
mmsClient_parseReadJournalResponse(MmsConnection self, ByteBuffer* response, int respBufPos, bool* moreFollows, LinkedList* result);


void
mmsClient_handleFileOpenRequest(MmsConnection connection,
    uint8_t* buffer, int bufPos, int maxBufPos,
    uint32_t invokeId, ByteBuffer* response);

void
mmsClient_handleFileReadRequest(
    MmsConnection connection,
    uint8_t* buffer, int bufPos, int maxBufPos,
    uint32_t invokeId,
    ByteBuffer* response);

void
mmsClient_handleFileCloseRequest(
MmsConnection connection,
uint8_t* buffer, int bufPos, int maxBufPos,
uint32_t invokeId,
ByteBuffer* response);


#endif /* MMS_MSG_INTERNAL_H_ */
