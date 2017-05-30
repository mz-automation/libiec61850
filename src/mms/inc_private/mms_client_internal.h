/*
 *  mms_msg_internal.h
 *
 *  Copyright 2013 Michael Zillgith
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

typedef enum {
	MMS_STATE_CLOSED,
	MMS_STATE_CONNECTING,
	MMS_STATE_CONNECTED
} AssociationState;

typedef enum {
	MMS_CON_IDLE,
	MMS_CON_WAITING,
	MMS_CON_ASSOCIATION_FAILED,
	MMS_CON_ASSOCIATED,
	MMS_CON_RESPONSE_PENDING
} ConnectionState;

#define CONCLUDE_STATE_CONNECTION_ACTIVE 0
#define CONCLUDE_STATE_REQUESTED 1
#define CONCLUDE_STATE_REJECTED 2
#define CONCLUDE_STATE_ACCEPTED 3

/* private instance variables */
struct sMmsConnection {
    Semaphore lastInvokeIdLock;
    uint32_t lastInvokeId;

	Semaphore lastResponseLock;
    volatile uint32_t responseInvokeId;
	ByteBuffer* lastResponse;
	volatile uint32_t lastResponseBufPos;
	volatile MmsError lastResponseError;

	Semaphore outstandingCallsLock;
	uint32_t* outstandingCalls;

	uint32_t requestTimeout;
	uint32_t connectTimeout;

	IsoClientConnection isoClient;
	AssociationState associationState;
	ConnectionState connectionState;

	MmsConnectionParameters parameters;
	IsoConnectionParameters isoParameters;

	MmsInformationReportHandler reportHandler;
	void* reportHandlerParameter;

	MmsConnectionLostHandler connectionLostHandler;
	void* connectionLostHandlerParameter;

#if (CONFIG_MMS_RAW_MESSAGE_LOGGING == 1)
	void* rawMmsMessageHandler;
	void* rawMmsMessageHandlerParameter;
#endif

	/* state of an active connection conclude/release process */
	int concludeState;

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

int
mmsClient_createInitiateRequest(MmsConnection self, ByteBuffer* writeBuffer);

MmsPdu_t*
mmsClient_createConfirmedRequestPdu(uint32_t invokeId);

int
mmsClient_createMmsGetNameListRequestVMDspecific(long invokeId, ByteBuffer* writeBuffer, const char* continueAfter);

bool
mmsClient_parseGetNameListResponse(LinkedList* nameList, ByteBuffer* message, uint32_t* invokeId);

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

void
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

MmsServerIdentity*
mmsClient_parseIdentifyResponse(MmsConnection self);

void
mmsClient_createStatusRequest(uint32_t invokeId, ByteBuffer* request, bool extendedDerivation);

bool
mmsClient_parseStatusResponse(MmsConnection self, int* vmdLogicalStatus, int* vmdPhysicalStatus);

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
mmsClient_parseFileDirectoryResponse(MmsConnection self, MmsFileDirectoryHandler handler, void* handlerParameter,
        bool* moreFollows);

bool
mmsClient_parseInitiateResponse(MmsConnection self);

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
mmsClient_parseReadJournalResponse(MmsConnection self, bool* moreFollows, LinkedList* result);

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
