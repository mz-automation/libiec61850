/*
 *  mms_server_connection.c
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
 *
 *
 *  MMS client connection handling code for libiec61850.
 *
 *  Handles a MMS client connection.
 */

#include "libiec61850_platform_includes.h"
#include "mms_server_internal.h"
#include "iso_server.h"
#include "ber_encoder.h"
#include "ber_decode.h"

/**********************************************************************************************
 * MMS Common support functions
 *********************************************************************************************/

#define MMS_REJECT_CONFIRMED_REQUEST 1
#define MMS_REJECT_CONFIRMED_RESPONSE 2
#define MMS_REJECT_CONFIRMED_ERROR 3
#define MMS_REJECT_UNCONFIRMED 4
#define MMS_REJECT_PDU_ERROR 5
#define MMS_REJECT_CANCEL_REQUEST 6
#define MMS_REJECT_CANCEL_RESPONSE 7
#define MMS_REJECT_CANCEL_ERROR 8
#define MMS_REJECT_CONCLUDE_REQUEST 9
#define MMS_REJECT_CONCLUDE_RESPONSE 10
#define MMS_REJECT_CONCLUDE_ERROR 11

#define MMS_REJECT_CONFIRMED_REQUEST_OTHER 0
#define MMS_REJECT_CONFIRMED_REQUEST_UNRECOGNIZED_SERVICE 1
#define MMS_REJECT_CONFIRMED_REQUEST_UNRECOGNIZED_MODIFIER 2
#define MMS_REJECT_CONFIRMED_REQUEST_INVALID_INVOKE_ID 3
#define MMS_REJECT_CONFIRMED_REQUEST_INVALID_ARGUMENT 4
#define MMS_REJECT_CONFIRMED_REQUEST_INVALID_MODIFIER 5
#define MMS_REJECT_CONFIRMED_REQUEST_MAX_SERV_OUTSTANDING_EXCEEDED 6
#define MMS_REJECT_CONFIRMED_REQUEST_MAX_RECURSION_EXCEEDED 8
#define MMS_REJECT_CONFIRMED_REQUEST_VALUE_OUT_OF_RANGE 9

#define MMS_REJECT_PDU_ERROR_UNKNOWN_PDU_TYPE 0
#define MMS_REJECT_PDU_ERROR_INVALID_PDU 1
#define MMS_REJECT_PDU_ERROR_ILLEGAL_ACSI_MAPPING 2


static void
mmsMsg_encodeMmsRejectPdu(uint32_t* invokeId, int rejectType, int rejectReason, ByteBuffer* response)
{
    int bufPos = 0;
    uint8_t* buffer = response->buffer;

    uint32_t invokeIdLength = 0;

    uint32_t rejectPduLength = 3;

    if (invokeId != NULL) {
        invokeIdLength = BerEncoder_UInt32determineEncodedSize(*invokeId);
        rejectPduLength += 2 + invokeIdLength;
    }

    /* Encode reject PDU */
    bufPos = BerEncoder_encodeTL(0xa4, rejectPduLength, buffer, bufPos);

    if (invokeId != NULL) {
        /* original invokeId */
        bufPos = BerEncoder_encodeTL(0x80, invokeIdLength, buffer, bufPos);
        bufPos = BerEncoder_encodeUInt32(*invokeId, buffer, bufPos);
    }

    buffer[bufPos++] = (uint8_t) (0x80 + rejectType);
    buffer[bufPos++] = 0x01;
    buffer[bufPos++] = (uint8_t) rejectReason;

    response->size = bufPos;
}

void
mmsMsg_createMmsRejectPdu(uint32_t* invokeId, int reason, ByteBuffer* response)
{
    int rejectType = 0;
    int rejectReason = 0;

    switch (reason)
    {

    case MMS_ERROR_REJECT_UNRECOGNIZED_SERVICE:
        rejectType = MMS_REJECT_CONFIRMED_REQUEST;
        rejectReason = MMS_REJECT_CONFIRMED_REQUEST_UNRECOGNIZED_SERVICE;
        break;

    case MMS_ERROR_REJECT_UNKNOWN_PDU_TYPE:
        rejectType = MMS_REJECT_PDU_ERROR;
        rejectReason = MMS_REJECT_PDU_ERROR_UNKNOWN_PDU_TYPE;
        break;

    case MMS_ERROR_REJECT_REQUEST_INVALID_ARGUMENT:
        rejectType = MMS_REJECT_CONFIRMED_REQUEST;
        rejectReason = MMS_REJECT_CONFIRMED_REQUEST_INVALID_ARGUMENT;
        break;

    case MMS_ERROR_REJECT_INVALID_PDU:
        rejectType = MMS_REJECT_PDU_ERROR;
        rejectReason = MMS_REJECT_PDU_ERROR_INVALID_PDU;
        break;

    default:
        rejectType = MMS_REJECT_CONFIRMED_REQUEST;
        rejectReason = MMS_REJECT_CONFIRMED_REQUEST_OTHER;
    }

    mmsMsg_encodeMmsRejectPdu(invokeId, rejectType, rejectReason, response);
}

/**********************************************************************************************
 * MMS General service handling functions
 *********************************************************************************************/

static void
handleConfirmedRequestPdu(
        MmsServerConnection self,
        uint8_t* buffer, int bufPos, int maxBufPos,
        ByteBuffer* response)
{
    uint32_t invokeId = 0;

    while (bufPos < maxBufPos) {
        uint8_t tag = buffer[bufPos++];
        int length;

        bool extendedTag = false;

        if ((tag & 0x1f) == 0x1f) {
            extendedTag = true;
            tag = buffer[bufPos++];
        }

        bufPos = BerDecoder_decodeLength(buffer, &length, bufPos, maxBufPos);

        if (bufPos < 0) {
            mmsMsg_createMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_INVALID_PDU, response);
            return;
        }

        if (extendedTag) {
            switch (tag)
            {

#if (MMS_OBTAIN_FILE_SERVICE == 1)


#if (CONFIG_MMS_SERVER_CONFIG_SERVICES_AT_RUNTIME == 1)
            case 0x2e: /* obtain-file */
                if (self->server->fileServiceEnabled)
                    mmsServer_handleObtainFileRequest(self, buffer, bufPos, bufPos + length, invokeId, response);
                else
                    mmsMsg_createMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_UNRECOGNIZED_SERVICE, response);
                break;
#else
            case 0x2e: /* obtain-file */
                mmsServer_handleObtainFileRequest(self, buffer, bufPos, bufPos + length, invokeId, response);
                break;
#endif /* (CONFIG_MMS_SERVER_CONFIG_SERVICES_AT_RUNTIME == 1) */


#endif /* MMS_OBTAIN_FILE_SERVICE == 1 */

#if (MMS_JOURNAL_SERVICE == 1)

#if (CONFIG_MMS_SERVER_CONFIG_SERVICES_AT_RUNTIME == 1)
            case 0x41: /* read-journal */
                if (self->server->journalServiceEnabled)
                    mmsServer_handleReadJournalRequest(self, buffer, bufPos, bufPos + length, invokeId, response);
                else
                    mmsMsg_createMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_UNRECOGNIZED_SERVICE, response);
                break;

#else
            case 0x41: /* read-journal */
                mmsServer_handleReadJournalRequest(self, buffer, bufPos, bufPos + length, invokeId, response);
                break;
#endif /* (CONFIG_MMS_SERVER_CONFIG_SERVICES_AT_RUNTIME == 1) */

#endif /* (MMS_JOURNAL_SERVICE == 1) */

#if (MMS_FILE_SERVICE == 1)

#if (CONFIG_MMS_SERVER_CONFIG_SERVICES_AT_RUNTIME == 1)
            case 0x48: /* file-open-request */
                if (self->server->fileServiceEnabled)
                    mmsServer_handleFileOpenRequest(self, buffer, bufPos, bufPos + length, invokeId, response);
                else
                    mmsMsg_createMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_UNRECOGNIZED_SERVICE, response);
                break;

#else
            case 0x48: /* file-open-request */
                mmsServer_handleFileOpenRequest(self, buffer, bufPos, bufPos + length, invokeId, response);
                break;
#endif /* (CONFIG_MMS_SERVER_CONFIG_SERVICES_AT_RUNTIME == 1) */

#if (CONFIG_MMS_SERVER_CONFIG_SERVICES_AT_RUNTIME == 1)
            case 0x49: /* file-read-request */
                if (self->server->fileServiceEnabled)
                    mmsServer_handleFileReadRequest(self, buffer, bufPos, bufPos + length, invokeId, response);
                else
                    mmsMsg_createMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_UNRECOGNIZED_SERVICE, response);
                break;
#else
            case 0x49: /* file-read-request */
                mmsServer_handleFileReadRequest(self, buffer, bufPos, bufPos + length, invokeId, response);
                break;
#endif /* (CONFIG_MMS_SERVER_CONFIG_SERVICES_AT_RUNTIME == 1) */

#if (CONFIG_MMS_SERVER_CONFIG_SERVICES_AT_RUNTIME == 1)
            case 0x4a: /* file-close-request */
                if (self->server->fileServiceEnabled)
                    mmsServer_handleFileCloseRequest(self, buffer, bufPos, bufPos + length, invokeId, response);
                else
                    mmsMsg_createMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_UNRECOGNIZED_SERVICE, response);
                break;
#else
            case 0x4a: /* file-close-request */
                mmsServer_handleFileCloseRequest(self, buffer, bufPos, bufPos + length, invokeId, response);
                break;
#endif /* (CONFIG_MMS_SERVER_CONFIG_SERVICES_AT_RUNTIME == 1) */

#if (MMS_RENAME_FILE_SERVICE == 1)

#if (CONFIG_MMS_SERVER_CONFIG_SERVICES_AT_RUNTIME == 1)
            case 0x4b: /* file-rename-request */
                if (self->server->fileServiceEnabled)
                    mmsServer_handleFileRenameRequest(self, buffer, bufPos, bufPos + length, invokeId, response);
                else
                    mmsMsg_createMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_UNRECOGNIZED_SERVICE, response);
                break;
#else
            case 0x4b: /* file-rename-request */
                mmsServer_handleFileRenameRequest(self, buffer, bufPos, bufPos + length, invokeId, response);
                break;
#endif /* (CONFIG_MMS_SERVER_CONFIG_SERVICES_AT_RUNTIME == 1) */

#endif /* (MMS_RENAME_FILE_SERVICE == 1) */


#if (MMS_DELETE_FILE_SERVICE == 1)

#if (CONFIG_MMS_SERVER_CONFIG_SERVICES_AT_RUNTIME == 1)
            case 0x4c: /* file-delete-request */
                if (self->server->fileServiceEnabled)
                    mmsServer_handleFileDeleteRequest(self, buffer, bufPos, bufPos + length, invokeId, response);
                else
                    mmsMsg_createMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_UNRECOGNIZED_SERVICE, response);
                break;
#else
            case 0x4c: /* file-delete-request */
                mmsServer_handleFileDeleteRequest(self, buffer, bufPos, bufPos + length, invokeId, response);
                break;
#endif /* (CONFIG_MMS_SERVER_CONFIG_SERVICES_AT_RUNTIME == 1) */

#endif /* (MMS_DELETE_FILE_SERVICE == 1) */

#if (CONFIG_MMS_SERVER_CONFIG_SERVICES_AT_RUNTIME == 1)
            case 0x4d: /* file-directory-request */
                if (self->server->fileServiceEnabled)
                    mmsServer_handleFileDirectoryRequest(self, buffer, bufPos, bufPos + length, invokeId, response);
                else
                    mmsMsg_createMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_UNRECOGNIZED_SERVICE, response);
                break;
#else
            case 0x4d: /* file-directory-request */
                mmsServer_handleFileDirectoryRequest(self, buffer, bufPos, bufPos + length, invokeId, response);
                break;
#endif /* (CONFIG_MMS_SERVER_CONFIG_SERVICES_AT_RUNTIME == 1) */

#endif /* MMS_FILE_SERVICE == 1 */

            default:
                mmsMsg_createMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_UNRECOGNIZED_SERVICE, response);
                return;
                break;
            }
        }
        else {
            switch (tag)
            {
            case 0x02: /* invoke Id */
                invokeId = BerDecoder_decodeUint32(buffer, length, bufPos);
                if (DEBUG_MMS_SERVER)
                    printf("MMS_SERVER: received request with invokeId: %i\n", invokeId);
                self->lastInvokeId = invokeId;
                break;

#if (MMS_STATUS_SERVICE == 1)
            case 0x80: /* status-request */
                mmsServer_handleStatusRequest(self, buffer, bufPos, invokeId, response);
                break;
#endif /* MMS_STATUS_SERVICE == 1 */

#if (MMS_GET_NAME_LIST == 1)
            case 0xa1: /* get-name-list-request */
                mmsServer_handleGetNameListRequest(self, buffer, bufPos, bufPos + length,
                        invokeId, response);
                break;
#endif /* (MMS_GET_NAME_LIST == 1) */

#if (MMS_IDENTIFY_SERVICE == 1)
            case 0x82: /* identify */
                mmsServer_handleIdentifyRequest(self, invokeId, response);
                break;
#endif /* MMS_IDENTIFY_SERVICE == 1 */

            case 0xa4: /* read-request */
                mmsServer_handleReadRequest(self, buffer, bufPos, bufPos + length,
                        invokeId, response);
                break;

#if (MMS_WRITE_SERVICE == 1)
            case 0xa5: /* write-request */
                mmsServer_handleWriteRequest(self, buffer, bufPos, bufPos + length,
                        invokeId, response);
                break;
#endif /* (MMS_WRITE_SERVICE == 1) */

#if (MMS_GET_VARIABLE_ACCESS_ATTRIBUTES == 1)
            case 0xa6: /* get-variable-access-attributes-request */
                mmsServer_handleGetVariableAccessAttributesRequest(self,
                        buffer, bufPos, bufPos + length,
                        invokeId, response);
                break;
#endif /* MMS_GET_VARIABLE_ACCESS_ATTRIBUTES == 1 */

#if (MMS_DYNAMIC_DATA_SETS == 1)

#if (CONFIG_MMS_SERVER_CONFIG_SERVICES_AT_RUNTIME == 1)

            case 0xab: /* define-named-variable-list */
                if (self->server->dynamicVariableListServiceEnabled)
                    mmsServer_handleDefineNamedVariableListRequest(self,
                            buffer, bufPos, bufPos + length,
                            invokeId, response);
                else
                    mmsMsg_createMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_UNRECOGNIZED_SERVICE, response);
                break;

            case 0xad: /* delete-named-variable-list-request */
                if (self->server->dynamicVariableListServiceEnabled)
                    mmsServer_handleDeleteNamedVariableListRequest(self,
                            buffer, bufPos, bufPos + length,
                            invokeId, response);
                else
                    mmsMsg_createMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_UNRECOGNIZED_SERVICE, response);
                break;

#else
            case 0xab: /* define-named-variable-list */
                mmsServer_handleDefineNamedVariableListRequest(self,
                        buffer, bufPos, bufPos + length,
                        invokeId, response);
                break;

            case 0xad: /* delete-named-variable-list-request */
                mmsServer_handleDeleteNamedVariableListRequest(self,
                        buffer, bufPos, bufPos + length,
                        invokeId, response);
                break;

#endif /* (CONFIG_MMS_SERVER_CONFIG_SERVICES_AT_RUNTIME == 1) */

#endif /* (MMS_DYNAMIC_DATA_SETS == 1) */

#if (MMS_GET_DATA_SET_ATTRIBUTES == 1)
            case 0xac: /* get-named-variable-list-attributes-request */
                mmsServer_handleGetNamedVariableListAttributesRequest(self,
                        buffer, bufPos, bufPos + length,
                        invokeId, response);
                break;
#endif /* (MMS_GET_DATA_SET_ATTRIBUTES == 1) */

            case 0x00: /* indefinite length end tag -> ignore */
                break;

            default:
                mmsMsg_createMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_UNRECOGNIZED_SERVICE, response);
                return;
                break;
            }
        }

        bufPos += length;
    }
}

#if (MMS_OBTAIN_FILE_SERVICE == 1)

static void
handleConfirmedErrorPdu(
        MmsServerConnection self,
        uint8_t* buffer, int bufPos, int maxBufPos,
        ByteBuffer* response)
{
    (void)response;

    uint32_t invokeId = 0;
    bool hasInvokeId = false;
    MmsServiceError serviceError;

    if (mmsMsg_parseConfirmedErrorPDU(buffer, bufPos, maxBufPos, &invokeId, &hasInvokeId, &serviceError)) {

        if (DEBUG_MMS_SERVER)
            printf("MMS_SERVER: Handle confirmed error PDU: invokeID: %u\n", invokeId);

        if (hasInvokeId) {
            /* check if message is related to an existing file upload task */
            int i;
            for (i = 0; i < CONFIG_MMS_SERVER_MAX_GET_FILE_TASKS; i++) {

                if (self->server->fileUploadTasks[i].state != MMS_FILE_UPLOAD_STATE_NOT_USED) {

                    if (self->server->fileUploadTasks[i].lastRequestInvokeId == invokeId) {

                        self->server->fileUploadTasks[i].state = MMS_FILE_UPLOAD_STATE_SEND_OBTAIN_FILE_ERROR_SOURCE;
                        return;
                    }

                }
            }
        }

    }
    else {
        if (DEBUG_MMS_SERVER)
            printf("MMS_SERVER: error parsing confirmed error PDU\n");
    }
}

static MmsObtainFileTask
getUploadTaskByInvokeId(MmsServer mmsServer, uint32_t invokeId)
{
    int i;
    for (i = 0; i < CONFIG_MMS_SERVER_MAX_GET_FILE_TASKS; i++) {
        if (mmsServer->fileUploadTasks[i].lastRequestInvokeId == invokeId)
            return &(mmsServer->fileUploadTasks[i]);
    }

    return NULL;
}

static void
mmsFileReadHandler(uint32_t invokeId, void* parameter, MmsError mmsError, int32_t frsmId, uint8_t* buffer, uint32_t bytesReceived, bool moreFollows)
{
    (void)invokeId;
    (void)frsmId;
    (void)moreFollows;

    MmsObtainFileTask task = (MmsObtainFileTask) parameter;

    if (mmsError == MMS_ERROR_NONE) {
        if (DEBUG_MMS_SERVER)
            printf("MMS_SERVER:  file %i received %i bytes\n", task->frmsId, bytesReceived);

        if(task->fileHandle){
            FileSystem_writeFile(task->fileHandle, buffer, bytesReceived);
        }
        else{
            if (DEBUG_MMS_SERVER)
                printf("MMS_SERVER: problem reading file %i file already closed\n", task->frmsId);
        }
    }
    else {
        if (DEBUG_MMS_SERVER)
            printf("MMS_SERVER:  problem reading file %i (error code: %i)\n", task->frmsId, mmsError);
    }
}

static void
handleConfirmedResponsePdu(
        MmsServerConnection self,
        uint8_t* buffer, int bufPos, int maxBufPos,
        ByteBuffer* response)
{
    uint32_t invokeId = 0;

    while (bufPos < maxBufPos) {
        int startBufPos = bufPos;

        uint8_t tag = buffer[bufPos++];
        int length;

        bool extendedTag = false;

        if ((tag & 0x1f) == 0x1f) {
            extendedTag = true;
            tag = buffer[bufPos++];
        }

        bufPos = BerDecoder_decodeLength(buffer, &length, bufPos, maxBufPos);

        if (bufPos < 0) {
            mmsMsg_createMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_UNRECOGNIZED_SERVICE, response);
            return;
        }

        if (extendedTag) {
            switch (tag)
            {

#if (MMS_FILE_SERVICE == 1)
            case 0x48: /* file-open-response */

                if (DEBUG_MMS_SERVER)
                    printf("MMS_SERVER: received file-open-response\n");

                {
                    MmsObtainFileTask fileTask = getUploadTaskByInvokeId(self->server, invokeId);

                    if (fileTask != NULL) {

                        int32_t frmsId;

                        if (mmsMsg_parseFileOpenResponse(buffer, startBufPos, maxBufPos, &frmsId, NULL, NULL)) {
                            fileTask->frmsId = frmsId;
                            fileTask->state = MMS_FILE_UPLOAD_STATE_SEND_FILE_READ;
                        }
                        else {
                            if (DEBUG_MMS_SERVER)
                                printf("MMS_SERVER: error parsing file-open-response\n");
                            fileTask->state = MMS_FILE_UPLOAD_STATE_SEND_OBTAIN_FILE_ERROR_SOURCE;
                        }
                    }
                    else {
                        /* ignore */

                        if (DEBUG_MMS_SERVER)
                            printf("MMS_SERVER: unexpected file-open-response\n");
                    }
                }

                break;

            case 0x49: /* file-read-response */
                {
                    if (DEBUG_MMS_SERVER)
                        printf("MMS_SERVER: received file-read-response\n");

                    MmsObtainFileTask fileTask = getUploadTaskByInvokeId(self->server, invokeId);

                    if (fileTask != NULL) {

                        bool moreFollows;

                        if(fileTask->fileHandle == NULL){
                            fileTask->state = MMS_FILE_UPLOAD_STATE_SEND_OBTAIN_FILE_ERROR_DESTINATION;
                        }
                        else{
                            if (mmsMsg_parseFileReadResponse(buffer, startBufPos, maxBufPos, invokeId, fileTask->frmsId, &moreFollows, mmsFileReadHandler, (void*) fileTask)) {

                                if (moreFollows) {
                                    fileTask->state = MMS_FILE_UPLOAD_STATE_SEND_FILE_READ;
                                }
                                else {
                                    fileTask->state = MMS_FILE_UPLOAD_STATE_SEND_FILE_CLOSE;
                                }
                            }
                            else {
                                fileTask->state = MMS_FILE_UPLOAD_STATE_SEND_OBTAIN_FILE_ERROR_SOURCE;

                                if (DEBUG_MMS_SERVER)
                                    printf("MMS_SERVER: error parsing file-read-response\n");
                            }
                        }
                    }
                    else {
                        /* ignore */

                        if (DEBUG_MMS_SERVER)
                            printf("MMS_SERVER: unexpected file-read-response\n");
                    }
                }

                break;

            case 0x4a: /* file-close-response */
                {
                    if (DEBUG_MMS_SERVER)
                        printf("MMS_SERVER: received file-close-response\n");

                    MmsObtainFileTask fileTask = getUploadTaskByInvokeId(self->server, invokeId);

                    if (fileTask != NULL) {
                        if(fileTask->fileHandle){
                            FileSystem_closeFile(fileTask->fileHandle);
                        }
                        fileTask->state = MMS_FILE_UPLOAD_STATE_SEND_OBTAIN_FILE_RESPONSE;
                    }
                    else {
                        /* ignore */

                        if (DEBUG_MMS_SERVER)
                            printf("MMS_SERVER: unexpected file-close-response\n");
                    }
                }
                break;
#endif /* MMS_FILE_SERVICE == 1 */

            default:
                mmsMsg_createMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_UNRECOGNIZED_SERVICE, response);
                return;
                break;
            }
        }
        else {
            switch (tag)
            {
            case 0x02: /* invoke Id */
                invokeId = BerDecoder_decodeUint32(buffer, length, bufPos);
                if (DEBUG_MMS_SERVER)
                    printf("MMS_SERVER: received request with invokeId: %i\n", invokeId);
                self->lastInvokeId = invokeId;
                break;

            default:
                mmsMsg_createMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_UNRECOGNIZED_SERVICE, response);
                return;
                break;
            }
        }

        bufPos += length;
    }
} /* handleConfirmedResponsePdu */
#endif /* (MMS_OBTAIN_FILE_SERVICE == 1) */

static inline void
MmsServerConnection_parseMessage(MmsServerConnection self, ByteBuffer* message, ByteBuffer* response)
{
    uint8_t* buffer = message->buffer;

    if (message->size < 2)
        goto parsing_error;

    int bufPos = 0;

    uint8_t pduType = buffer[bufPos++];
    int pduLength;

    bufPos = BerDecoder_decodeLength(buffer, &pduLength, bufPos, message->size);

    if (bufPos < 0)
        goto parsing_error;

    if (DEBUG_MMS_SERVER)
        printf("MMS_SERVER: recvd MMS-PDU type: %02x size: %i\n", pduType, pduLength);

    switch (pduType)
    {
    case 0xa8: /* Initiate request PDU */
        mmsServer_handleInitiateRequest(self, buffer, bufPos, bufPos + pduLength, response);
        break;

    case 0xa0: /* Confirmed request PDU */
        handleConfirmedRequestPdu(self, buffer, bufPos, bufPos + pduLength, response);
        break;

#if (MMS_OBTAIN_FILE_SERVICE == 1)
    case 0xa1: /* Confirmed response PDU */
        handleConfirmedResponsePdu(self, buffer, bufPos, bufPos + pduLength, response);
        break;

    case 0xa2: /* Confirmed error PDU */
        handleConfirmedErrorPdu(self, buffer, 0, bufPos + pduLength, response);
        break;

#endif /* (MMS_OBTAIN_FILE_SERVICE == 1) */
    case 0x8b: /* Conclude request PDU */
        mmsServer_writeConcludeResponsePdu(response);
        break;

    case 0xa4: /* Reject PDU - silently ignore */
        if (DEBUG_MMS_SERVER)
            printf("MMS_SERVER: received reject PDU!\n");
        break;

    case 0x00: /* indefinite length end tag -> ignore */
        break;

    default:
        mmsMsg_createMmsRejectPdu(NULL, MMS_ERROR_REJECT_UNKNOWN_PDU_TYPE, response);
        break;
    }

    return;

parsing_error:
    if (DEBUG_MMS_SERVER)
        printf("MMS_SERVER: error parsing message\n");

    return;
}

static void /* is called by IsoConnection */
messageReceived(void* parameter, ByteBuffer* message, ByteBuffer* response)
{
    MmsServerConnection self = (MmsServerConnection) parameter;

    MmsServerConnection_parseMessage(self, message, response);
}

static void /* is called by IsoConnection */
connectionTickHandler(void* parameter)
{
    MmsServerConnection self = (MmsServerConnection) parameter;

    MmsServer_callConnectionHandler(self->server, self);
}

/**********************************************************************************************
 * MMS server connection public API functions
 *********************************************************************************************/

MmsServerConnection
MmsServerConnection_init(MmsServerConnection connection, MmsServer server, IsoConnection isoCon)
{
    MmsServerConnection self;

    if (connection == NULL)
        self = (MmsServerConnection) GLOBAL_CALLOC(1, sizeof(struct sMmsServerConnection));
    else
        self = connection;

    self->maxServOutstandingCalled = 0;
    self->maxServOutstandingCalling = 0;
    self->maxPduSize = CONFIG_MMS_MAXIMUM_PDU_SIZE;
    self->dataStructureNestingLevel = 0;
    self->server = server;
    self->isoConnection = isoCon;

#if (MMS_DYNAMIC_DATA_SETS == 1)
    self->namedVariableLists = LinkedList_create();
#endif

#if (MMS_OBTAIN_FILE_SERVICE == 1)
    self->lastRequestInvokeId = 0;
#endif

    IsoConnection_installListener(isoCon, messageReceived,
            (UserLayerTickHandler) connectionTickHandler,
            (void*) self);

    return self;
}

void
MmsServerConnection_destroy(MmsServerConnection self)
{

#if (MMS_FILE_SERVICE == 1)
    int frsmIndex = 0;

    for (frsmIndex = 0; frsmIndex < CONFIG_MMS_MAX_NUMBER_OF_OPEN_FILES_PER_CONNECTION; frsmIndex++)
        if (self->frsms[frsmIndex].fileHandle != NULL)
            FileSystem_closeFile(self->frsms[frsmIndex].fileHandle);

    mmsServerConnection_stopFileUploadTasks(self);
#endif

#if (MMS_DYNAMIC_DATA_SETS == 1)
    LinkedList_destroyDeep(self->namedVariableLists, (LinkedListValueDeleteFunction) MmsNamedVariableList_destroy);
#endif

    GLOBAL_FREEMEM(self);
}

int
MmsServerConnection_getMaxMmsPduSize(MmsServerConnection self)
{
    return self->maxPduSize;
}

void
MmsServerConnection_sendMessage(MmsServerConnection self, ByteBuffer* message)
{
    IsoConnection_sendMessage(self->isoConnection, message);
}

#if (MMS_DYNAMIC_DATA_SETS == 1)
bool
MmsServerConnection_addNamedVariableList(MmsServerConnection self, MmsNamedVariableList variableList)
{
    /* TODO check if operation is allowed! */

    LinkedList_add(self->namedVariableLists, variableList);

    return true;
}

void
MmsServerConnection_deleteNamedVariableList(MmsServerConnection self, char* listName)
{
    mmsServer_deleteVariableList(self->namedVariableLists, listName);
}

MmsNamedVariableList
MmsServerConnection_getNamedVariableList(MmsServerConnection self, const char* variableListName)
{
    return mmsServer_getNamedVariableListWithName(self->namedVariableLists, variableListName);
}
#endif /* (MMS_DYNAMIC_DATA_SETS == 1) */

char*
MmsServerConnection_getClientAddress(MmsServerConnection self)
{
    return IsoConnection_getPeerAddress(self->isoConnection);
}

char*
MmsServerConnection_getLocalAddress(MmsServerConnection self)
{
    return IsoConnection_getLocalAddress(self->isoConnection);
}

void*
MmsServerConnection_getSecurityToken(MmsServerConnection self)
{
    return IsoConnection_getSecurityToken(self->isoConnection);
}

#if (MMS_DYNAMIC_DATA_SETS == 1)
LinkedList
MmsServerConnection_getNamedVariableLists(MmsServerConnection self)
{
    return self->namedVariableLists;
}
#endif /* (MMS_DYNAMIC_DATA_SETS == 1) */

uint32_t
MmsServerConnection_getLastInvokeId(MmsServerConnection self)
{
    return self->lastInvokeId;
}

#if (MMS_OBTAIN_FILE_SERVICE == 1)
uint32_t
MmsServerConnection_getNextRequestInvokeId(MmsServerConnection self)
{
    self->lastRequestInvokeId++;
    return self->lastRequestInvokeId;
}
#endif /* (MMS_OBTAIN_FILE_SERVICE == 1) */

const char*
MmsServerConnection_getFilesystemBasepath(MmsServerConnection self)
{
#if (CONFIG_SET_FILESTORE_BASEPATH_AT_RUNTIME == 1)
    if (self->server->filestoreBasepath != NULL)
        return self->server->filestoreBasepath;
    else
        return CONFIG_VIRTUAL_FILESTORE_BASEPATH;
#else
    return CONFIG_VIRTUAL_FILESTORE_BASEPATH;
#endif
}



