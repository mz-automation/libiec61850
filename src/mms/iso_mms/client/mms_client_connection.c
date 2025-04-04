/*
 *  mms_client_connection.c
 *
 *  Copyright 2013-2024 Michael Zillgith
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

#include "libiec61850_platform_includes.h"

#include "mms_client_connection.h"
#include "iso_client_connection.h"
#include "mms_client_internal.h"
#include "stack_config.h"

#include <MmsPdu.h>

#include "byte_buffer.h"
#include "ber_decode.h"

#include "tls_config.h"

#define CONFIG_MMS_CONNECTION_DEFAULT_TIMEOUT 5000
#define CONFIG_MMS_CONNECTION_DEFAULT_CONNECT_TIMEOUT 10000

static void
setConnectionState(MmsConnection self, MmsConnectionState newState)
{
    Semaphore_wait(self->associationStateLock);
    self->connectionState = newState;
    Semaphore_post(self->associationStateLock);

    if (self->stateChangedHandler)
        self->stateChangedHandler(self, self->stateChangedHandlerParameter, newState);
}

static MmsConnectionState
getConnectionState(MmsConnection self)
{
    MmsConnectionState state;

    Semaphore_wait(self->associationStateLock);
    state = self->connectionState;
    Semaphore_post(self->associationStateLock);

    return state;
}

static void
handleUnconfirmedMmsPdu(MmsConnection self, ByteBuffer* message)
{
    if (self->reportHandler)
    {
        MmsPdu_t* mmsPdu = NULL; /* allow asn1c to allocate structure */

        if (DEBUG_MMS_CLIENT)
            printf("MMS_CLIENT: report handler rcvd size:%i\n", ByteBuffer_getSize(message));

        asn_dec_rval_t rval = ber_decode(NULL, &asn_DEF_MmsPdu,
                (void**) &mmsPdu, ByteBuffer_getBuffer(message), ByteBuffer_getSize(message));

        if (rval.code == RC_OK)
        {
            if (DEBUG_MMS_CLIENT)
                printf("MMS_CLIENT: received report (size:%i)\n", (int) rval.consumed);

            if (mmsPdu->present == MmsPdu_PR_unconfirmedPDU)
            {
                if (mmsPdu->choice.unconfirmedPDU.unconfirmedService.present ==
                        UnconfirmedService_PR_informationReport)
                {
                    char* domainId = NULL;

                    InformationReport_t* report =
                            &(mmsPdu->choice.unconfirmedPDU.unconfirmedService.choice.informationReport);

                    if (report->variableAccessSpecification.present ==
                            VariableAccessSpecification_PR_variableListName)
                    {
                        if (report->variableAccessSpecification.choice.variableListName.present
                                == ObjectName_PR_vmdspecific)
                        {
                            int nameSize =
                                    report->variableAccessSpecification.choice.variableListName.choice.vmdspecific.size;

                            uint8_t* buffer =
                                    report->variableAccessSpecification.choice.variableListName.choice.vmdspecific.buf;

                            char* variableListName = StringUtils_createStringFromBuffer(buffer, nameSize);

                            int listSize = report->listOfAccessResult.list.count;

                            MmsValue* values = mmsClient_parseListOfAccessResults(
                                    report->listOfAccessResult.list.array, listSize, true);

                            self->reportHandler(self->reportHandlerParameter, domainId, variableListName, values, true);

                            GLOBAL_FREEMEM(variableListName);
                        }
                        else {
                            /* Ignore domain and association specific information reports (not used by IEC 61850) */
                        }

                    }
                    else if (report->variableAccessSpecification.present == VariableAccessSpecification_PR_listOfVariable)
                    {
                        int listSize = report->listOfAccessResult.list.count;
                        int variableSpecSize = report->variableAccessSpecification.choice.listOfVariable.list.count;

                        if (listSize != variableSpecSize)
                        {
                            if (DEBUG_MMS_CLIENT)
                                printf("report contains wrong number of access results\n");
                            return;
                        }

                        MmsValue* values = mmsClient_parseListOfAccessResults(
                                report->listOfAccessResult.list.array, listSize, false);

                        int i;
                        for (i = 0; i < variableSpecSize; i++)
                        {
                            if (report->variableAccessSpecification.choice.listOfVariable.list.array[i]->variableSpecification.present
                                    == VariableSpecification_PR_name)
                            {
                                if (report->variableAccessSpecification.choice.listOfVariable.list.array[i]
                                        ->variableSpecification.choice.name.present == ObjectName_PR_vmdspecific)
                                {
                                    int nameSize =
                                            report->variableAccessSpecification.choice.listOfVariable.list.array[i]
                                                    ->variableSpecification.choice.name.choice.vmdspecific.size;

                                    uint8_t* buffer =
                                            report->variableAccessSpecification.choice.listOfVariable.list.array[i]
                                                    ->variableSpecification.choice.name.choice.vmdspecific.buf;

                                    if (nameSize < 129)
                                    {
                                        char variableListName[129];
                                        memcpy(variableListName, buffer, nameSize);
                                        variableListName[nameSize] = 0;

                                        MmsValue* value = values;

                                        if (variableSpecSize != 1)
                                            value = MmsValue_getElement(values, i);

                                        self->reportHandler(self->reportHandlerParameter, domainId, variableListName,
                                                value, false);

                                        /* report handler is responsible to delete the MmsValue object */
                                        if (variableSpecSize != 1)
                                            MmsValue_setElement(values, i, NULL);
                                        else
                                            values = NULL;
                                    }
                                }
                                else if (report->variableAccessSpecification.choice.listOfVariable.list.array[i]
                                        ->variableSpecification.choice.name.present == ObjectName_PR_domainspecific)
                                {
                                    int domainNameSize =
                                        report->variableAccessSpecification.choice.listOfVariable.list.array[i]
                                            ->variableSpecification.choice.name.choice.domainspecific.domainId.size;

                                    int itemNameSize =
                                         report->variableAccessSpecification.choice.listOfVariable.list.array[i]
                                             ->variableSpecification.choice.name.choice.domainspecific.itemId.size;

                                    if ((domainNameSize < 65) && (itemNameSize < 65))
                                    {
                                        char domainNameStr[65];
                                        char itemNameStr[65];

                                        uint8_t* domainNameBuffer =
                                            report->variableAccessSpecification.choice.listOfVariable.list.array[i]
                                                   ->variableSpecification.choice.name.choice.domainspecific.domainId.buf;

                                        uint8_t* itemNamebuffer =
                                                report->variableAccessSpecification.choice.listOfVariable.list.array[i]
                                                       ->variableSpecification.choice.name.choice.domainspecific.itemId.buf;

                                        memcpy(domainNameStr, domainNameBuffer, domainNameSize);
                                        domainNameStr[domainNameSize] = 0;
                                        memcpy(itemNameStr, itemNamebuffer, itemNameSize);
                                        itemNameStr[itemNameSize] = 0;

                                        MmsValue* value = values;

                                        if (variableSpecSize != 1)
                                            value = MmsValue_getElement(values, i);

                                        self->reportHandler(self->reportHandlerParameter, domainNameStr, itemNameStr,
                                                value, false);

                                        /* report handler is responsible to delete the MmsValue object */
                                        if (variableSpecSize != 1)
                                            MmsValue_setElement(values, i, NULL);
                                        else
                                            values = NULL;

                                    }
                                }
                            }
                        }

                        if (values != NULL)
                            MmsValue_delete(values);
                    }
                    else
                    {
                        /* Ignore */
                        if (DEBUG_MMS_CLIENT)
                            printf("unrecognized information report\n");
                    }

                }

            }
        }
        else
        {
            if (DEBUG_MMS_CLIENT)
                printf("handleUnconfirmedMmsPdu: error parsing PDU at %u\n", (uint32_t) rval.consumed);
        }

        asn_DEF_MmsPdu.free_struct(&asn_DEF_MmsPdu, mmsPdu, 0);
    }
}

static uint32_t
getNextInvokeId(MmsConnection self)
{
    uint32_t nextInvokeId;

    Semaphore_wait(self->nextInvokeIdLock);
    self->nextInvokeId++;

    if (self->nextInvokeId == 0)
        self->nextInvokeId = 1;

    nextInvokeId = self->nextInvokeId;
    Semaphore_post(self->nextInvokeIdLock);

    return nextInvokeId;
}

static MmsOutstandingCall
checkForOutstandingCall(MmsConnection self, uint32_t invokeId)
{
    int i = 0;

    Semaphore_wait(self->outstandingCallsLock);

    for (i = 0; i < self->maxOutstandingCalled; i++)
    {
        if (self->outstandingCalls[i].isUsed)
        {
            if (self->outstandingCalls[i].invokeId == invokeId)
            {
                Semaphore_post(self->outstandingCallsLock);
                return &(self->outstandingCalls[i]);
            }
        }
    }

    Semaphore_post(self->outstandingCallsLock);

    return NULL;
}

static bool
addToOutstandingCalls(MmsConnection self, uint32_t invokeId, eMmsOutstandingCallType type, void* userCallback, void* userParameter, MmsClientInternalParameter internalParameter)
{
    int i = 0;

    Semaphore_wait(self->outstandingCallsLock);

    for (i = 0; i < self->maxOutstandingCalled; i++)
    {
        if (self->outstandingCalls[i].isUsed == false)
        {
            self->outstandingCalls[i].isUsed = true;
            self->outstandingCalls[i].invokeId = invokeId;
            self->outstandingCalls[i].timeout = Hal_getTimeInMs() + self->requestTimeout;
            self->outstandingCalls[i].type = type;
            self->outstandingCalls[i].userCallback = userCallback;
            self->outstandingCalls[i].userParameter = userParameter;
            self->outstandingCalls[i].internalParameter = internalParameter;
            Semaphore_post(self->outstandingCallsLock);
            return true;
        }
    }

    Semaphore_post(self->outstandingCallsLock);

    return false;
}

static void
removeFromOutstandingCalls(MmsConnection self, uint32_t invokeId)
{
    int i = 0;

    Semaphore_wait(self->outstandingCallsLock);

    for (i = 0; i < self->maxOutstandingCalled; i++)
    {
        if (self->outstandingCalls[i].isUsed)
        {
            if (self->outstandingCalls[i].invokeId == invokeId)
            {
                self->outstandingCalls[i].isUsed = false;
                break;
            }
        }
    }

    Semaphore_post(self->outstandingCallsLock);
}

MmsOutstandingCall
mmsClient_getMatchingObtainFileRequest(MmsConnection self, const char* filename)
{
    int i = 0;

    Semaphore_wait(self->outstandingCallsLock);

    for (i = 0; i < self->maxOutstandingCalled; i++)
    {
        if (self->outstandingCalls[i].isUsed)
        {
            if (self->outstandingCalls[i].type == MMS_CALL_TYPE_OBTAIN_FILE)
            {
                char* storedFilename = (char*) self->outstandingCalls[i].internalParameter.ptr;

                if (storedFilename)
                {
                    if (!strcmp(filename, storedFilename))
                    {
                        Semaphore_post(self->outstandingCallsLock);
                        return &(self->outstandingCalls[i]);
                    }
                }
            }
        }
    }

    Semaphore_post(self->outstandingCallsLock);

    return NULL;
}

static void
sendMessage(MmsConnection self, ByteBuffer* message)
{
#if (CONFIG_MMS_RAW_MESSAGE_LOGGING == 1)
    if (self->rawMmsMessageHandler != NULL)
    {
        MmsRawMessageHandler handler = (MmsRawMessageHandler) self->rawMmsMessageHandler;
        handler(self->rawMmsMessageHandlerParameter, message->buffer, message->size, false);
    }
#endif /* (CONFIG_MMS_RAW_MESSAGE_LOGGING == 1) */

    IsoClientConnection_sendMessage(self->isoClient, message);
}

static MmsError
sendAsyncRequest(MmsConnection self, uint32_t invokeId, ByteBuffer* message, eMmsOutstandingCallType type,
        void* userCallback, void* userParameter, MmsClientInternalParameter internalParameter)
{
    if (addToOutstandingCalls(self, invokeId, type, userCallback, userParameter, internalParameter) == false)
    {
        /* message cannot be sent - release resources */
        IsoClientConnection_releaseTransmitBuffer(self->isoClient);

        return MMS_ERROR_OUTSTANDING_CALL_LIMIT;
    }

    sendMessage(self, message);

    return MMS_ERROR_NONE;
}

static MmsError
convertRejectCodesToMmsError(int rejectType, int rejectReason)
{
    if ((rejectType == 1) && (rejectReason == 1))
        return MMS_ERROR_REJECT_UNRECOGNIZED_SERVICE;
    else if ((rejectType == 5) && (rejectReason == 0))
        return MMS_ERROR_REJECT_UNKNOWN_PDU_TYPE;
    else if ((rejectType == 1) && (rejectReason == 4))
        return MMS_ERROR_REJECT_REQUEST_INVALID_ARGUMENT;
    else if ((rejectType == 5) && (rejectReason == 1))
        return MMS_ERROR_REJECT_INVALID_PDU;
    else
        return MMS_ERROR_REJECT_OTHER;
}

static MmsError
convertServiceErrorToMmsError(MmsServiceError serviceError)
{
    MmsError mmsError;

    switch (serviceError.errorClass)
    {
    case 0: /* class: vmd-state */
        mmsError = MMS_ERROR_VMDSTATE_OTHER;
        break;

    case 1: /* class: application-reference */
        mmsError = MMS_ERROR_APPLICATION_REFERENCE_OTHER;
        break;

    case 2: /* class: definition */
        switch (serviceError.errorCode)
        {
        case 1:
            mmsError = MMS_ERROR_DEFINITION_OBJECT_UNDEFINED;
            break;
        case 2:
            mmsError = MMS_ERROR_DEFINITION_INVALID_ADDRESS;
            break;
        case 3:
            mmsError = MMS_ERROR_DEFINITION_TYPE_UNSUPPORTED;
            break;
        case 4:
            mmsError = MMS_ERROR_DEFINITION_TYPE_INCONSISTENT;
            break;
        case 5:
            mmsError = MMS_ERROR_DEFINITION_OBJECT_EXISTS;
            break;
        case 6:
            mmsError = MMS_ERROR_DEFINITION_OBJECT_ATTRIBUTE_INCONSISTENT;
            break;
        default:
            mmsError = MMS_ERROR_DEFINITION_OTHER;
            break;
        }
        break;

    case 3: /* class: resource */
        mmsError = MMS_ERROR_RESOURCE_OTHER;
        break;

    case 4: /* class: service */

        switch (serviceError.errorCode)
        {
        case 5:
            mmsError = MMS_ERROR_SERVICE_OBJECT_CONSTRAINT_CONFLICT;
            break;

        default:
            mmsError = MMS_ERROR_SERVICE_OTHER;
            break;
        }
        break;

    case 5: /* class: service-preempt */
        mmsError = MMS_ERROR_SERVICE_PREEMPT_OTHER;
        break;

    case 6: /* class: time-resolution */
        mmsError = MMS_ERROR_TIME_RESOLUTION_OTHER;
        break;

    case 7: /* class: access */
        switch (serviceError.errorCode)
        {
        case 1:
            mmsError = MMS_ERROR_ACCESS_OBJECT_ACCESS_UNSUPPORTED;
            break;
        case 2:
            mmsError = MMS_ERROR_ACCESS_OBJECT_NON_EXISTENT;
            break;
        case 3:
            mmsError = MMS_ERROR_ACCESS_OBJECT_ACCESS_DENIED;
            break;
        case 4:
            mmsError = MMS_ERROR_ACCESS_OBJECT_INVALIDATED;
            break;
        default:
            mmsError = MMS_ERROR_ACCESS_OTHER;
            break;
        }
        break;

    case 11: /* class: file */
        switch (serviceError.errorCode)
        {
        case 1:
            mmsError = MMS_ERROR_FILE_FILENAME_AMBIGUOUS;
            break;
        case 2:
            mmsError = MMS_ERROR_FILE_FILE_BUSY;
            break;
        case 3:
            mmsError = MMS_ERROR_FILE_FILENAME_SYNTAX_ERROR;
            break;
        case 4:
            mmsError = MMS_ERROR_FILE_CONTENT_TYPE_INVALID;
            break;
        case 5:
            mmsError = MMS_ERROR_FILE_POSITION_INVALID;
            break;
        case 6:
            mmsError = MMS_ERROR_FILE_FILE_ACCESS_DENIED;
            break;
        case 7:
            mmsError = MMS_ERROR_FILE_FILE_NON_EXISTENT;
            break;
        case 8:
            mmsError = MMS_ERROR_FILE_DUPLICATE_FILENAME;
            break;
        case 9:
            mmsError = MMS_ERROR_FILE_INSUFFICIENT_SPACE_IN_FILESTORE;
            break;
        default:
            mmsError = MMS_ERROR_FILE_OTHER;
            break;
        }
        break;

    default:
        mmsError = MMS_ERROR_OTHER;
    }

    return mmsError;
}

static int
parseServiceError(uint8_t* buffer, int bufPos, int maxLength, MmsServiceError* error)
{
    int endPos = bufPos + maxLength;
    int length;

    while (bufPos < endPos)
    {
        uint8_t tag = buffer[bufPos++];
        bufPos = BerDecoder_decodeLength(buffer, &length, bufPos, endPos);

        if (bufPos < 0)
            return -1;

        switch (tag)
        {
        case 0xa0: /* errorClass */
            {
                uint8_t errorClassTag = buffer[bufPos++];
                bufPos = BerDecoder_decodeLength(buffer, &length, bufPos, endPos);

                if (bufPos < 0)
                    return -1;

                error->errorClass = errorClassTag - 0x80;
                error->errorCode = BerDecoder_decodeUint32(buffer, length, bufPos);

                bufPos += length;
            }
            break;
        case 0x81: /* additionalCode */
            bufPos += length; /* ignore */
            break;
        case 0x82: /* additionalDescription */
            bufPos += length; /* ignore */
            break;
        case 0xa3: /* serviceSpecificInfo */
            bufPos += length; /* ignore */
            break;
        case 0x00: /* indefinite length end tag -> ignore */
            break;
        default:
            bufPos += length; /* ignore */
            break;
        }
    }

    return bufPos;
}

int
mmsMsg_parseConfirmedErrorPDU(uint8_t* buffer, int bufPos, int maxBufPos, uint32_t* invokeId, bool* hasInvokeId, MmsServiceError* serviceError)
{
    int length;

    if (hasInvokeId)
        *hasInvokeId = false;

    uint8_t tag = buffer[bufPos++];
    if (tag != 0xa2)
        goto exit_error;

    bufPos = BerDecoder_decodeLength(buffer, &length, bufPos, maxBufPos);
    if (bufPos < 0)
        goto exit_error;

    int endPos = bufPos + length;

    while (bufPos < endPos)
    {
        tag = buffer[bufPos++];

        bufPos = BerDecoder_decodeLength(buffer, &length, bufPos, maxBufPos);

        if (bufPos < 0)
            goto exit_error;

        switch (tag)
        {
        case 0x80: /* invoke Id */
            if (hasInvokeId)
                *hasInvokeId = true;
            if (invokeId != NULL)
                *invokeId = BerDecoder_decodeUint32(buffer, length, bufPos);
            bufPos += length;
            break;
        case 0x81: /* modifierPosition */
            bufPos += length; /* ignore */
            break;
        case 0xa2: /* serviceError */
            bufPos = parseServiceError(buffer, bufPos, length, serviceError);
            if (bufPos < 0)
                goto exit_error;
            break;
        case 0x00: /* indefinite length end tag -> ignore */
            break;
        default:
            bufPos += length; /* ignore */
            break;
        }
    }

    return bufPos;

exit_error:
    if (DEBUG_MMS_CLIENT)
        printf("MMS_CLIENT: error parsing confirmed error PDU\n");

    return -1;
}

int
mmsMsg_parseRejectPDU(uint8_t* buffer, int bufPos, int maxBufPos, uint32_t* invokeId, bool* hasInvokeId, int* rejectType, int* rejectReason)
{
    int length;

    if (hasInvokeId)
        *hasInvokeId = false;

    uint8_t tag = buffer[bufPos++];

    if (tag != 0xa4)
        goto exit_error;

    bufPos = BerDecoder_decodeLength(buffer, &length, bufPos, maxBufPos);
    if (bufPos < 0)
        goto exit_error;

    int endPos = bufPos + length;

    while (bufPos < endPos)
    {
        tag = buffer[bufPos++];

        bufPos = BerDecoder_decodeLength(buffer, &length, bufPos, maxBufPos);

        if (bufPos < 0)
            goto exit_error;

        if (tag == 0x80)
        {
            /* invoke id */
            if (hasInvokeId)
                *hasInvokeId = true;
            if (invokeId != NULL)
                *invokeId = BerDecoder_decodeUint32(buffer, length, bufPos);
        }
        else if (tag > 0x80 && tag < 0x8c)
        {
            *rejectType = tag - 0x80;
            *rejectReason = BerDecoder_decodeInt32(buffer, length, bufPos);
        }
        else {
            /* unknown - ignore */
        }

        bufPos += length;
    }

    return bufPos;

exit_error:
    if (DEBUG_MMS_CLIENT)
        printf("MMS_CLIENT: error parsing reject PDU\n");

    return -1;
}

static void
handleAsyncResponse(MmsConnection self, ByteBuffer* response, uint32_t bufPos, MmsOutstandingCall outstandingCall, MmsError err)
{
    if (outstandingCall->type == MMS_CALL_TYPE_READ_VARIABLE)
    {
        MmsConnection_ReadVariableHandler handler =
                (MmsConnection_ReadVariableHandler) outstandingCall->userCallback;

        if (err != MMS_ERROR_NONE)
        {
            handler(outstandingCall->invokeId, outstandingCall->userParameter, err, NULL);
        }
        else
        {
            if (response)
            {
                MmsValue* value = mmsClient_parseReadResponse(response, NULL, false);

                if (value == NULL)
                    err = MMS_ERROR_PARSING_RESPONSE;

                handler(outstandingCall->invokeId, outstandingCall->userParameter, err, value);
            }

        }
    }
    else if (outstandingCall->type == MMS_CALL_TYPE_READ_MULTIPLE_VARIABLES)
    {
        MmsConnection_ReadVariableHandler handler =
                (MmsConnection_ReadVariableHandler) outstandingCall->userCallback;

        if (err != MMS_ERROR_NONE)
        {
            handler(outstandingCall->invokeId, outstandingCall->userParameter, err, NULL);
        }
        else
        {
            if (response)
            {
                MmsValue* value = mmsClient_parseReadResponse(response, NULL, true);

                if (value == NULL)
                    err = MMS_ERROR_PARSING_RESPONSE;

                handler(outstandingCall->invokeId, outstandingCall->userParameter, err, value);
            }
        }
    }
    else if (outstandingCall->type == MMS_CALL_TYPE_WRITE_VARIABLE)
    {
        MmsConnection_WriteVariableHandler handler =
                (MmsConnection_WriteVariableHandler) outstandingCall->userCallback;

        if (err != MMS_ERROR_NONE)
        {
            handler(outstandingCall->invokeId, outstandingCall->userParameter, err, DATA_ACCESS_ERROR_NO_RESPONSE);
        }
        else
        {
            if (response)
            {
                MmsDataAccessError daError = mmsClient_parseWriteResponse(response, bufPos, &err);

                handler(outstandingCall->invokeId, outstandingCall->userParameter, err, daError);
            }
        }

    }
    else if (outstandingCall->type == MMS_CALL_TYPE_WRITE_MULTIPLE_VARIABLES)
    {
        MmsConnection_WriteMultipleVariablesHandler handler =
                (MmsConnection_WriteMultipleVariablesHandler) outstandingCall->userCallback;

        if (err != MMS_ERROR_NONE)
        {
            handler(outstandingCall->invokeId, outstandingCall->userParameter, err, NULL);
        }
        else
        {
            if (response)
            {
                LinkedList accessResults = NULL;

                mmsClient_parseWriteMultipleItemsResponse(response, bufPos, &err, -1, &accessResults);

                handler(outstandingCall->invokeId, outstandingCall->userParameter, err, accessResults);
            }
        }
    }
    else if (outstandingCall->type == MMS_CALL_TYPE_READ_NVL_DIRECTORY)
    {
        MmsConnection_ReadNVLDirectoryHandler handler =
                (MmsConnection_ReadNVLDirectoryHandler) outstandingCall->userCallback;

        if (err != MMS_ERROR_NONE)
        {
            handler(outstandingCall->invokeId, outstandingCall->userParameter, err, NULL, false);
        }
        else
        {
            if (response)
            {
                bool deletable = false;

                LinkedList accessSpec = mmsClient_parseGetNamedVariableListAttributesResponse(response, &deletable);

                if (accessSpec == NULL)
                    err = MMS_ERROR_PARSING_RESPONSE;

                handler(outstandingCall->invokeId, outstandingCall->userParameter, err, accessSpec, deletable);
            }
        }
    }
    else if (outstandingCall->type == MMS_CALL_TYPE_DEFINE_NVL)
    {
        MmsConnection_GenericServiceHandler handler =
                (MmsConnection_GenericServiceHandler) outstandingCall->userCallback;

        if (err != MMS_ERROR_NONE)
        {
            handler(outstandingCall->invokeId, outstandingCall->userParameter, err, false);
        }
        else
        {
            bool success = false;

            if (!mmsClient_parseDefineNamedVariableResponse(response, NULL))
                err = MMS_ERROR_PARSING_RESPONSE;
            else
                success = true;

            handler(outstandingCall->invokeId, outstandingCall->userParameter, err, success);
        }
    }
    else if (outstandingCall->type == MMS_CALL_TYPE_DELETE_NVL)
    {
        MmsConnection_GenericServiceHandler handler =
                (MmsConnection_GenericServiceHandler) outstandingCall->userCallback;

        if (err != MMS_ERROR_NONE)
        {
            handler(outstandingCall->invokeId, outstandingCall->userParameter, err, false);
        }
        else
        {
            bool success = false;

            long numberMatched = 0;
            long numberDeleted = 0;

            if (mmsClient_parseDeleteNamedVariableListResponse(response, NULL, &numberDeleted, &numberMatched))
                success = true;

            if (numberMatched == 0)
            {
                err = MMS_ERROR_ACCESS_OBJECT_NON_EXISTENT;
            }
            else
            {
                if (numberDeleted == 0)
                    err = MMS_ERROR_ACCESS_OBJECT_ACCESS_DENIED;
            }

            handler(outstandingCall->invokeId, outstandingCall->userParameter, err, success);
        }
    }
    else if (outstandingCall->type == MMS_CALL_TYPE_GET_VAR_ACCESS_ATTR)
    {
        MmsConnection_GetVariableAccessAttributesHandler handler =
                (MmsConnection_GetVariableAccessAttributesHandler) outstandingCall->userCallback;

        if (err != MMS_ERROR_NONE)
        {
            handler(outstandingCall->invokeId, outstandingCall->userParameter, err, NULL);
        }
        else
        {
            MmsVariableSpecification* typeSpec = mmsClient_parseGetVariableAccessAttributesResponse(response, NULL);

            if (typeSpec == NULL)
                err = MMS_ERROR_PARSING_RESPONSE;

            handler(outstandingCall->invokeId, outstandingCall->userParameter, err, typeSpec);
        }
    }
    else if (outstandingCall->type == MMS_CALL_TYPE_GET_SERVER_STATUS)
    {
        MmsConnection_GetServerStatusHandler handler =
                (MmsConnection_GetServerStatusHandler) outstandingCall->userCallback;

        if (err != MMS_ERROR_NONE)
        {
            handler(outstandingCall->invokeId, outstandingCall->userParameter, err, 0, 0);
        }
        else
        {
            int vmdLogicalStatus;
            int vmdPhysicalStatus;

            if (mmsClient_parseStatusResponse(self, response, bufPos, &vmdLogicalStatus, &vmdPhysicalStatus) == false)
                err = MMS_ERROR_PARSING_RESPONSE;

            handler(outstandingCall->invokeId, outstandingCall->userParameter, err, vmdLogicalStatus, vmdPhysicalStatus);
        }
    }
    else if (outstandingCall->type == MMS_CALL_TYPE_IDENTIFY)
    {
        MmsConnection_IdentifyHandler handler =
                (MmsConnection_IdentifyHandler) outstandingCall->userCallback;

        if (err != MMS_ERROR_NONE)
        {
            handler(outstandingCall->invokeId, outstandingCall->userParameter, err, NULL, NULL, NULL);
        }
        else
        {
            if (mmsClient_parseIdentifyResponse(self, response, bufPos, outstandingCall->invokeId, handler,
                        outstandingCall->userParameter) == false)
            {
                handler(outstandingCall->invokeId, outstandingCall->userParameter, MMS_ERROR_PARSING_RESPONSE,
                        NULL, NULL, NULL);
            }

        }
    }
    else if (outstandingCall->type == MMS_CALL_TYPE_READ_JOURNAL)
    {
        MmsConnection_ReadJournalHandler handler =
                (MmsConnection_ReadJournalHandler) outstandingCall->userCallback;

        if (err != MMS_ERROR_NONE)
        {
            handler(outstandingCall->invokeId, outstandingCall->userParameter, err, NULL, false);
        }
        else
        {
            bool moreFollows = false;
            LinkedList entries = NULL;

            if (mmsClient_parseReadJournalResponse(response, bufPos, &moreFollows, &entries) == false) {
                handler(outstandingCall->invokeId, outstandingCall->userParameter, MMS_ERROR_PARSING_RESPONSE, NULL, false);
            }
            else {
                handler(outstandingCall->invokeId, outstandingCall->userParameter, err, entries, moreFollows);
            }
        }
    }
    else if (outstandingCall->type == MMS_CALL_TYPE_GET_NAME_LIST)
    {
        MmsConnection_GetNameListHandler handler =
                (MmsConnection_GetNameListHandler) outstandingCall->userCallback;

        if (err != MMS_ERROR_NONE)
        {
            handler(outstandingCall->invokeId, outstandingCall->userParameter, err, NULL, false);
        }
        else
        {
            LinkedList nameList = (LinkedList) outstandingCall->internalParameter.ptr;

            bool moreFollows = mmsClient_parseGetNameListResponse(&nameList, response);

            if (nameList == NULL) {
                handler(outstandingCall->invokeId, outstandingCall->userParameter, MMS_ERROR_PARSING_RESPONSE, NULL, false);
            }
            else {
                handler(outstandingCall->invokeId, outstandingCall->userParameter, err, nameList, moreFollows);
            }
        }
    }
    else if (outstandingCall->type == MMS_CALL_TYPE_FILE_OPEN)
    {
        MmsConnection_FileOpenHandler handler =
                (MmsConnection_FileOpenHandler) outstandingCall->userCallback;

        if (err != MMS_ERROR_NONE)
        {
            handler(outstandingCall->invokeId, outstandingCall->userParameter, err, 0, 0, 0);
        }
        else
        {
            int32_t frsmId;
            uint32_t fileSize;
            uint64_t lastModified;

            if (mmsMsg_parseFileOpenResponse(ByteBuffer_getBuffer(response), bufPos, ByteBuffer_getSize(response),
                    &frsmId, &fileSize, &lastModified) == false)
            {
                handler(outstandingCall->invokeId, outstandingCall->userParameter, MMS_ERROR_PARSING_RESPONSE, 0, 0, 0);
            }
            else
            {
                handler(outstandingCall->invokeId, outstandingCall->userParameter, err, frsmId, fileSize, lastModified);
            }
        }
    }
    else if (outstandingCall->type == MMS_CALL_TYPE_FILE_READ)
    {
        MmsConnection_FileReadHandler handler =
                (MmsConnection_FileReadHandler) outstandingCall->userCallback;

        int32_t frsmId = outstandingCall->internalParameter.i32;

        if (err != MMS_ERROR_NONE)
        {
            handler(outstandingCall->invokeId, outstandingCall->userParameter, err, frsmId, NULL, 0, false);
        }
        else
        {
            bool moreFollows;

            if (mmsMsg_parseFileReadResponse(ByteBuffer_getBuffer(response), bufPos, ByteBuffer_getSize(response), outstandingCall->invokeId, frsmId, &moreFollows,
                    handler, outstandingCall->userParameter) == false)
            {
                handler(outstandingCall->invokeId, outstandingCall->userParameter, MMS_ERROR_PARSING_RESPONSE, frsmId, NULL, 0, false);
            }
        }
    }
    else if ((outstandingCall->type == MMS_CALL_TYPE_FILE_CLOSE) ||
             (outstandingCall->type == MMS_CALL_TYPE_FILE_DELETE) ||
             (outstandingCall->type == MMS_CALL_TYPE_FILE_RENAME) ||
             (outstandingCall->type == MMS_CALL_TYPE_OBTAIN_FILE))
    {
        MmsConnection_GenericServiceHandler handler =
                (MmsConnection_GenericServiceHandler) outstandingCall->userCallback;

        if (outstandingCall->type == MMS_CALL_TYPE_OBTAIN_FILE)
        {
            if (outstandingCall->internalParameter.ptr)
                GLOBAL_FREEMEM(outstandingCall->internalParameter.ptr);
        }

        if (err != MMS_ERROR_NONE) {
            handler(outstandingCall->invokeId, outstandingCall->userParameter, err, false);
        }
        else {
            handler(outstandingCall->invokeId, outstandingCall->userParameter, err, true);
        }
    }
    else if (outstandingCall->type == MMS_CALL_TYPE_GET_FILE_DIR)
    {
        MmsConnection_FileDirectoryHandler handler =
                (MmsConnection_FileDirectoryHandler) outstandingCall->userCallback;

        if (err != MMS_ERROR_NONE)
        {
            handler(outstandingCall->invokeId, outstandingCall->userParameter, err, NULL, 0, 0, false);
        }
        else
        {
            if (mmsClient_parseFileDirectoryResponse(response, bufPos, outstandingCall->invokeId, handler, outstandingCall->userParameter) == false)
                handler(outstandingCall->invokeId, outstandingCall->userParameter, MMS_ERROR_PARSING_RESPONSE, NULL, 0, 0, false);
        }
    }

    if (err != MMS_ERROR_SERVICE_TIMEOUT)
        removeFromOutstandingCalls(self, outstandingCall->invokeId);
}

static bool
mmsIsoCallback(IsoIndication indication, void* parameter, ByteBuffer* payload)
{
    MmsConnection self = (MmsConnection) parameter;

    if (DEBUG_MMS_CLIENT)
        if (indication != ISO_IND_TICK)
            printf("MMS_CLIENT: mmsIsoCallback called with indication %i\n", indication);

    if (indication == ISO_IND_TICK)
    {
        /* check timeouts */

        uint64_t currentTime = Hal_getTimeInMs();

        int i = 0;

        for (i = 0; i < self->maxOutstandingCalled; i++)
        {
            Semaphore_wait(self->outstandingCallsLock);

            if (self->outstandingCalls[i].isUsed)
            {
                Semaphore_post(self->outstandingCallsLock);

                if (currentTime > self->outstandingCalls[i].timeout)
                {
                    if (self->outstandingCalls[i].type != MMS_CALL_TYPE_NONE)
                        handleAsyncResponse(self, NULL, 0, &(self->outstandingCalls[i]), MMS_ERROR_SERVICE_TIMEOUT);

                    Semaphore_wait(self->outstandingCallsLock);

                    self->outstandingCalls[i].isUsed = false;

                    Semaphore_post(self->outstandingCallsLock);
                }
            }
            else {
                Semaphore_post(self->outstandingCallsLock);
            }
        }

        if (self->concludeHandler)
        {
            if (currentTime > self->concludeTimeout)
            {
                self->concludeHandler(self->concludeHandlerParameter, MMS_ERROR_SERVICE_TIMEOUT, false);
                self->concludeHandler = NULL;
            }
        }

        return true;
    }

    if (indication == ISO_IND_CLOSED)
    {
        if (DEBUG_MMS_CLIENT)
            printf("MMS_CLIENT: mmsIsoCallback: Connection lost or closed by client!\n");

        setConnectionState(self, MMS_CONNECTION_STATE_CLOSED);

        /* Call user provided callback function */
        if (self->connectionLostHandler != NULL)
            self->connectionLostHandler(self, self->connectionLostHandlerParameter);

        /* Cleanup outstanding calls */
        {
            int i;

            for (i = 0; i < self->maxOutstandingCalled; i++)
            {
                Semaphore_wait(self->outstandingCallsLock);

                if (self->outstandingCalls[i].isUsed)
                {
                    Semaphore_post(self->outstandingCallsLock);

                    if (self->outstandingCalls[i].type != MMS_CALL_TYPE_NONE)
                        handleAsyncResponse(self, NULL, 0, &(self->outstandingCalls[i]), MMS_ERROR_SERVICE_TIMEOUT);

                    Semaphore_wait(self->outstandingCallsLock);

                    self->outstandingCalls[i].isUsed = false;
                }

                Semaphore_post(self->outstandingCallsLock);
            }
        }

        return true;
    }

    if (indication == ISO_IND_ASSOCIATION_FAILED)
    {
        if (DEBUG_MMS_CLIENT)
            printf("MMS_CLIENT: mmsIsoCallback: association failed!\n");

        setConnectionState(self, MMS_CONNECTION_STATE_CLOSING);
        return false;
    }

    if (payload != NULL) 
    {
        if (ByteBuffer_getSize(payload) < 1) {
            return false;
        }
    }

    uint8_t* buf = ByteBuffer_getBuffer(payload);

#if (CONFIG_MMS_RAW_MESSAGE_LOGGING == 1)
    if (self->rawMmsMessageHandler != NULL)
    {
        MmsRawMessageHandler handler = (MmsRawMessageHandler) self->rawMmsMessageHandler;
        handler(self->rawMmsMessageHandlerParameter, buf, payload->size, true);
    }
#endif /* (CONFIG_MMS_RAW_MESSAGE_LOGGING == 1) */

    uint8_t tag = buf[0];

    if (DEBUG_MMS_CLIENT)
        printf("MMS_CLIENT: MMS-PDU: %02x\n", tag);

    if (tag == 0xa9)
    {
        /* initiate response PDU */

        if (indication == ISO_IND_ASSOCIATION_SUCCESS)
        {
            if (mmsClient_parseInitiateResponse(self, payload))
            {
                setConnectionState(self, MMS_CONNECTION_STATE_CONNECTED);
            }
            else
            {
                setConnectionState(self, MMS_CONNECTION_STATE_CLOSING);

                goto exit_with_error;
            }
        }
        else
        {
            setConnectionState(self, MMS_CONNECTION_STATE_CLOSING);

            if (DEBUG_MMS_CLIENT)
                printf("MMS_CLIENT: Failed to parse initiate response!\n");

            return false;
        }
    }
    else if (tag == 0xaa)
    {
        /* initiate error PDU */

        if (DEBUG_MMS_CLIENT)
              printf("MMS_CLIENT: received initiate error PDU\n");

        setConnectionState(self, MMS_CONNECTION_STATE_CLOSING);

        return false;
    }
    else if (tag == 0xa3)
    {
        /* unconfirmed PDU */
        handleUnconfirmedMmsPdu(self, payload);
    }
    else if (tag == 0x8b)
    {
        /* conclude request PDU */
        if (DEBUG_MMS_CLIENT)
            printf("MMS_CLIENT: received conclude.request\n");

        /* TODO block all new user requests? */
    }
    else if (tag == 0x8c)
    {
        /* conclude response PDU */
        if (DEBUG_MMS_CLIENT)
            printf("MMS_CLIENT: received conclude.response+\n");

        if (self->concludeHandler)
        {
            self->concludeHandler(self->concludeHandlerParameter, MMS_ERROR_NONE, true);
            self->concludeHandler = NULL;
        }

        IsoClientConnection_release(self->isoClient);
    }
    else if (tag == 0x8d)
    {
        /* conclude error PDU */
        if (DEBUG_MMS_CLIENT)
            printf("MMS_CLIENT: received conclude.reponse-\n");

        if (self->concludeHandler)
        {
            self->concludeHandler(self->concludeHandlerParameter, MMS_ERROR_CONCLUDE_REJECTED, false);
            self->concludeHandler = NULL;
        }
    }
    else if (tag == 0xa2)
    {
        /* confirmed error PDU */

        if (DEBUG_MMS_CLIENT)
            printf("MMS_CLIENT: Confirmed error PDU!\n");

        uint32_t invokeId;
        bool hasInvokeId = false;

        MmsServiceError serviceError = { 0, 0 };

        if (mmsMsg_parseConfirmedErrorPDU(payload->buffer, 0, payload->size, &invokeId, &hasInvokeId, &serviceError) < 0)
        {
            if (DEBUG_MMS_CLIENT)
                printf("MMS_CLIENT: Error parsing confirmedErrorPDU!\n");

            goto exit_with_error;
        }
        else
        {
            if (hasInvokeId)
            {
                MmsOutstandingCall call = checkForOutstandingCall(self, invokeId);

                if (call)
                {
                    MmsError err = convertServiceErrorToMmsError(serviceError);

                    if (call->type != MMS_CALL_TYPE_NONE)
                    {
                        handleAsyncResponse(self, NULL, 0, call, err);
                    }
                    else
                    {
                        if (DEBUG_MMS_CLIENT)
                            printf("MMS_CLIENT: internal problem (unexpected call type - error PDU)\n");
                    }
                }
                else
                {
                    if (DEBUG_MMS_CLIENT)
                        printf("MMS_CLIENT: server sent unexpected confirmed error PDU!\n");

                    return false;
                }
            }
            else
            {
                if (DEBUG_MMS_CLIENT)
                    printf("MMS_CLIENT: server sent confirmed error PDU without invoke ID!\n");

                return false;
            }

        }
    }
    else if (tag == 0xa4)
    {
        /* reject PDU */

        if (DEBUG_MMS_CLIENT)
            printf("MMS_CLIENT: reject PDU!\n");

        bool hasInvokeId = false;
        uint32_t invokeId = 0;
        int rejectType;
        int rejectReason;

        if (mmsMsg_parseRejectPDU(payload->buffer, 0, payload->size, &invokeId, &hasInvokeId, &rejectType, &rejectReason) >= 0)
        {
            if (DEBUG_MMS_CLIENT)
                printf("MMS_CLIENT: reject PDU invokeID: %u type: %i reason: %i\n", invokeId, rejectType, rejectReason);

            if (hasInvokeId)
            {
                MmsOutstandingCall call = checkForOutstandingCall(self, invokeId);

                if (call)
                {
                    MmsError err = convertRejectCodesToMmsError(rejectType, rejectReason);

                    if (call->type != MMS_CALL_TYPE_NONE)
                    {
                        handleAsyncResponse(self, NULL, 0, call, err);
                    }
                    else
                    {
                        if (DEBUG_MMS_CLIENT)
                            printf("MMS_CLIENT: internal problem (unexpected call type - reject PDU)\n");
                    }
                }
                else {
                    return false;
                }
            }
            else {
                return false;
            }
        }
        else
            goto exit_with_error;
    }
    else if (tag == 0xa1)
    {
        /* confirmed response PDU */

        int length;
        int bufPos = 1;

        bufPos = BerDecoder_decodeLength(buf, &length, bufPos, payload->size);
        if (bufPos < 0)
            goto exit_with_error;

        if (buf[bufPos++] == 0x02)
        {
            int invokeIdLength;

            bufPos = BerDecoder_decodeLength(buf, &invokeIdLength, bufPos, payload->size);
            if (bufPos < 0)
                goto exit_with_error;

            uint32_t invokeId =
                    BerDecoder_decodeUint32(buf, invokeIdLength, bufPos);

            if (DEBUG_MMS_CLIENT)
                printf("MMS_CLIENT: mms_client_connection: rcvd confirmed resp - invokeId: %u length: %i bufLen: %i\n",
                        invokeId, length, payload->size);

            bufPos += invokeIdLength;

            MmsOutstandingCall call = checkForOutstandingCall(self, invokeId);

            if (call)
            {
                if (call->type != MMS_CALL_TYPE_NONE)
                {
                    handleAsyncResponse(self, payload, bufPos, call, MMS_ERROR_NONE);
                }
                else
                {
                    if (DEBUG_MMS_CLIENT)
                        printf("MMS_CLIENT: internal problem (unexpected call type - confirmed response PDU)\n");
                }
            }
            else
            {
                if (DEBUG_MMS_CLIENT)
                    printf("MMS_CLIENT: unexpected message from server!\n");

                return false;
            }
        }
        else
            goto exit_with_error;
    }
#if (MMS_OBTAIN_FILE_SERVICE == 1)
    else if (tag == 0xa0)
    {
        if (DEBUG_MMS_CLIENT)
            printf("MMS_CLIENT: received confirmed request PDU (size=%i)\n", payload->size);

        /* TODO extract function */

        int bufPos = 1;
        int length;

        bufPos = BerDecoder_decodeLength(buf, &length, bufPos, payload->size);
        if (bufPos < 0)
            goto exit_with_error;

        bool hasInvokeId = false;
        uint32_t invokeId = 0;

        while (bufPos < payload->size)
        {
            uint8_t nestedTag = buf[bufPos++];

            bool extendedTag = false;

            if ((nestedTag & 0x1f) == 0x1f)
            {
                extendedTag = true;
                nestedTag = buf[bufPos++];
            }

            bufPos = BerDecoder_decodeLength(buf, &length, bufPos, payload->size);
            if (bufPos < 0)
                goto exit_with_error;

            if (extendedTag)
            {
                if (hasInvokeId == false)
                {
                    if (DEBUG_MMS_CLIENT)
                        printf("MMS_CLIENT: invalid message received - missing invoke ID!\n");

                    goto exit_with_error;
                }

                switch (nestedTag)
                {

#if (MMS_FILE_SERVICE == 1)
                case 0x48: /* file-open-request */
                    {
                        if (DEBUG_MMS_CLIENT)
                            printf("MMS_CLIENT: received file-open-request\n");

                        ByteBuffer* response = IsoClientConnection_allocateTransmitBuffer(self->isoClient);

                        mmsClient_handleFileOpenRequest(self, buf, bufPos, bufPos + length, invokeId, response);

                        IsoClientConnection_sendMessage(self->isoClient, response);
                    }
                    break;

                case 0x49: /* file-read-request */
                    {
                        if (DEBUG_MMS_CLIENT)
                            printf("MMS_CLIENT: received file-read-request\n");

                        ByteBuffer* response = IsoClientConnection_allocateTransmitBuffer(self->isoClient);

                        mmsClient_handleFileReadRequest(self, buf, bufPos, bufPos + length, invokeId, response);

                        IsoClientConnection_sendMessage(self->isoClient, response);
                    }
                    break;

                case 0x4a: /* file-close-request */
                    {
                        if (DEBUG_MMS_CLIENT)
                            printf("MMS_CLIENT: received file-close-request\n");

                        ByteBuffer* response = IsoClientConnection_allocateTransmitBuffer(self->isoClient);

                        mmsClient_handleFileCloseRequest(self, buf, bufPos, bufPos + length, invokeId, response);

                        IsoClientConnection_sendMessage(self->isoClient, response);
                    }
                    break;
#endif /* MMS_FILE_SERVICE == 1 */

                default:
                    /* mmsServer_writeMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_UNRECOGNIZED_SERVICE, response); */
                    if (DEBUG_MMS_CLIENT)
                        printf("MMS_CLIENT: unexpected message from server!\n");

                    break;
                }
            }
            else {
                switch (nestedTag)
                {
                case 0x02: /* invoke Id */
                    invokeId = BerDecoder_decodeUint32(buf, length, bufPos);
                    if (DEBUG_MMS_CLIENT)
                        printf("MMS_CLIENT: received request with invokeId: %u\n", invokeId);

                    hasInvokeId = true;

                    break;

                default:
                    /*  mmsServer_writeMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_UNRECOGNIZED_SERVICE, response); */
                    if (DEBUG_MMS_CLIENT)
                        printf("MMS_CLIENT: unexpected message from server!\n");

                    goto exit_with_error;

                    break;
                }
            }

            bufPos += length;
        }

    }
#endif /* (MMS_OBTAIN_FILE_SERVICE == 1) */
    else
    {
        if (DEBUG_MMS_CLIENT)
            printf("MMS_CLIENT: unknown message type\n");

        goto exit_with_error;
    }

    if (DEBUG_MMS_CLIENT)
        printf("MMS_CLIENT: LEAVE mmsIsoCallback - OK\n");

    return true;

exit_with_error:

    if (DEBUG_MMS_CLIENT)
        printf("MMS_CLIENT: received malformed message from server!\n");

    return false;
}

#if (CONFIG_MMS_THREADLESS_STACK == 0)
static void*
connectionHandlingThread(void* parameter)
{
    MmsConnection self = (MmsConnection) parameter;

    while (self->connectionThreadRunning)
    {
        if (MmsConnection_tick(self))
            Thread_sleep(10);
    }

    return NULL;
}
#endif /* (CONFIG_MMS_THREADLESS_STACK == 0) */

static MmsConnection
MmsConnection_createInternal(TLSConfiguration tlsConfig, bool createThread)
{
#if (CONFIG_MMS_THREADLESS_STACK == 1)
    if (createThread)
        return NULL;
#endif

    MmsConnection self = (MmsConnection) GLOBAL_CALLOC(1, sizeof(struct sMmsConnection));

    if (self)
    {
        self->parameters.dataStructureNestingLevel = -1;
        self->parameters.maxServOutstandingCalled = -1;
        self->parameters.maxServOutstandingCalling = -1;
        self->parameters.maxPduSize = CONFIG_MMS_MAXIMUM_PDU_SIZE;

        self->requestTimeout = CONFIG_MMS_CONNECTION_DEFAULT_TIMEOUT;

        self->nextInvokeIdLock = Semaphore_create(1);
        self->outstandingCallsLock = Semaphore_create(1);

        self->associationStateLock = Semaphore_create(1);
        self->connectionState = MMS_CONNECTION_STATE_CLOSED;

        self->concludeHandler = NULL;
        self->concludeHandlerParameter = NULL;
        self->concludeTimeout = 0;

        self->maxOutstandingCalling = CONFIG_DEFAULT_MAX_SERV_OUTSTANDING_CALLING;
        self->maxOutstandingCalled = CONFIG_DEFAULT_MAX_SERV_OUTSTANDING_CALLED;
        self->outstandingCalls = (MmsOutstandingCall) GLOBAL_CALLOC(CONFIG_DEFAULT_MAX_SERV_OUTSTANDING_CALLED, sizeof(struct sMmsOutstandingCall));

        self->isoParameters = IsoConnectionParameters_create();

        /* Load default values for connection parameters */
        TSelector tSelector = { 2, { 0, 1 } };
        SSelector sSelector = { 2, { 0, 1 } };
        PSelector pSelector = { 4, { 0, 0, 0, 1 } };

        IsoConnectionParameters_setLocalAddresses(self->isoParameters, pSelector, sSelector, tSelector);
        IsoConnectionParameters_setLocalApTitle(self->isoParameters, "1.1.1.999", 12);
        IsoConnectionParameters_setRemoteAddresses(self->isoParameters, pSelector, sSelector, tSelector);
        IsoConnectionParameters_setRemoteApTitle(self->isoParameters, "1.1.1.999.1", 12);

        self->connectTimeout = CONFIG_MMS_CONNECTION_DEFAULT_CONNECT_TIMEOUT;

        self->isoClient = IsoClientConnection_create(self->isoParameters, (IsoIndicationCallback) mmsIsoCallback, (void*) self);

#if (CONFIG_MMS_SUPPORT_TLS == 1)
        if (tlsConfig) {
            IsoConnectionParameters_setTlsConfiguration(self->isoParameters, tlsConfig);
        }
#else
        (void)tlsConfig;
#endif /* (CONFIG_MMS_SUPPORT_TLS == 1) */

#if (CONFIG_MMS_THREADLESS_STACK == 0)
        self->createThread = createThread;
        self->connectionHandlingThread = NULL;
        self->connectionThreadRunning = false;
#endif
    }

    return self;
}

MmsConnection
MmsConnection_create()
{
    return MmsConnection_createInternal(NULL, true);
}

MmsConnection
MmsConnection_createSecure(TLSConfiguration tlsConfig)
{
    return MmsConnection_createInternal(tlsConfig, true);
}

MmsConnection
MmsConnection_createNonThreaded(TLSConfiguration tlsConfig)
{
    return MmsConnection_createInternal(tlsConfig, false);
}

void
MmsConnection_destroy(MmsConnection self)
{
    if (self)
    {
#if (CONFIG_MMS_THREADLESS_STACK == 0)
        if (self->createThread)
        {
            if (self->connectionHandlingThread)
            {
                if (self->connectionThreadRunning)
                {
                    self->connectionThreadRunning = false;
                    Thread_destroy(self->connectionHandlingThread);
                    self->connectionHandlingThread = NULL;
                }
            }
        }
#endif

        if (self->isoClient != NULL)
            IsoClientConnection_destroy(self->isoClient);

        if (self->isoParameters != NULL)
            IsoConnectionParameters_destroy(self->isoParameters);

        Semaphore_destroy(self->nextInvokeIdLock);

        Semaphore_destroy(self->outstandingCallsLock);

        Semaphore_destroy(self->associationStateLock);

        GLOBAL_FREEMEM(self->outstandingCalls);

#if (MMS_OBTAIN_FILE_SERVICE == 1)
#if (CONFIG_SET_FILESTORE_BASEPATH_AT_RUNTIME == 1)
        if (self->filestoreBasepath != NULL)
            GLOBAL_FREEMEM(self->filestoreBasepath);
#endif

        /* Close outstanding open files */
        mmsClient_closeOutstandingOpenFiles(self);
#endif

        GLOBAL_FREEMEM(self);
    }
}

void
MmsConnection_setFilestoreBasepath(MmsConnection self, const char* basepath)
{
#if (MMS_OBTAIN_FILE_SERVICE == 1)
#if (CONFIG_SET_FILESTORE_BASEPATH_AT_RUNTIME == 1)
    if (self->filestoreBasepath != NULL)
    {
        GLOBAL_FREEMEM(self->filestoreBasepath);
        self->filestoreBasepath = NULL;
    }

    if (basepath != NULL)
        self->filestoreBasepath = StringUtils_copyString(basepath);
#endif
#endif
}

char*
MmsConnection_getFilestoreBasepath(MmsConnection self)
{
#if (MMS_OBTAIN_FILE_SERVICE == 1)
#if (CONFIG_SET_FILESTORE_BASEPATH_AT_RUNTIME == 1)
    if (self->filestoreBasepath != NULL)
        return self->filestoreBasepath;
    else
        return CONFIG_VIRTUAL_FILESTORE_BASEPATH;
#else
    return CONFIG_VIRTUAL_FILESTORE_BASEPATH;
#endif

#else
    return CONFIG_VIRTUAL_FILESTORE_BASEPATH;
#endif
}

void
MmsConnection_setRawMessageHandler(MmsConnection self, MmsRawMessageHandler handler, void* parameter)
{
#if (CONFIG_MMS_RAW_MESSAGE_LOGGING == 1)
    self->rawMmsMessageHandler = (void*) handler;
    self->rawMmsMessageHandlerParameter = parameter;
#endif
}

void
MmsConnection_setConnectionLostHandler(MmsConnection self, MmsConnectionLostHandler handler, void* handlerParameter)
{
    self->connectionLostHandler = handler;
    self->connectionLostHandlerParameter = handlerParameter;
}

void
MmsConnection_setConnectionStateChangedHandler(MmsConnection self, MmsConnectionStateChangedHandler handler, void* parameter)
{
    self->stateChangedHandler = handler;
    self->stateChangedHandlerParameter = parameter;
}

void
MmsConnection_setRequestTimeout(MmsConnection self, uint32_t timeoutInMs)
{
    self->requestTimeout = timeoutInMs;
}

void
MmsConnnection_setMaxOutstandingCalls(MmsConnection self, int calling, int called)
{
    if (calling < 1)
        calling = 1;

    if (called < 1)
        called = 1;

    if (self->outstandingCalls)
    {
        GLOBAL_FREEMEM(self->outstandingCalls);
    }

    self->maxOutstandingCalling = calling;
    self->maxOutstandingCalled = called;
    self->outstandingCalls = (MmsOutstandingCall)GLOBAL_CALLOC(called, sizeof(struct sMmsOutstandingCall));
}

uint32_t
MmsConnection_getRequestTimeout(MmsConnection self)
{
    return self->requestTimeout;
}

void
MmsConnection_setConnectTimeout(MmsConnection self, uint32_t timeoutInMs)
{
    self->connectTimeout = timeoutInMs;
}

void
MmsConnection_setLocalDetail(MmsConnection self, int32_t localDetail)
{
    self->parameters.maxPduSize = localDetail;
}

int32_t
MmsConnection_getLocalDetail(MmsConnection self)
{
    return self->parameters.maxPduSize;
}

IsoConnectionParameters
MmsConnection_getIsoConnectionParameters(MmsConnection self)
{
    return self->isoParameters;
}

MmsConnectionParameters
MmsConnection_getMmsConnectionParameters(MmsConnection self)
{
    return self->parameters;
}

struct connectParameters
{
    Semaphore sem;
    MmsConnectionState state;
    MmsConnectionStateChangedHandler originalHandler;
    void* originalParameter;
};

static void
internalConnectionStateChangedHandler (MmsConnection connection, void* parameter, MmsConnectionState newState)
{
    struct connectParameters* conParams = (struct connectParameters*) parameter;

    if ((newState == MMS_CONNECTION_STATE_CLOSED) || (newState == MMS_CONNECTION_STATE_CONNECTED))
    {
        conParams->state = newState;

        /* unblock user thread */
        Semaphore_post(conParams->sem);
    }
    else {
        if (conParams->originalHandler)
            conParams->originalHandler(connection, conParams->originalParameter, newState);
    }
}

bool
MmsConnection_connect(MmsConnection self, MmsError* mmsError, const char* serverName, int serverPort)
{
    bool success = false;

    struct connectParameters conParams;

    conParams.sem = Semaphore_create(1);
    conParams.state = MMS_CONNECTION_STATE_CONNECTING;
    conParams.originalHandler = self->stateChangedHandler;
    conParams.originalParameter = self->stateChangedHandlerParameter;

    Semaphore_wait(conParams.sem);

    self->stateChangedHandler = internalConnectionStateChangedHandler;
    self->stateChangedHandlerParameter = &conParams;

    MmsError err;

    MmsConnection_connectAsync(self, &err, serverName, serverPort);

    if (err == MMS_ERROR_NONE)
    {
        Semaphore_wait(conParams.sem);

        if (conParams.state == MMS_CONNECTION_STATE_CONNECTED)
        {
            *mmsError = MMS_ERROR_NONE;
            success = true;
        }
        else
        {
            *mmsError = MMS_ERROR_CONNECTION_REJECTED;
        }

        if (conParams.originalHandler)
            conParams.originalHandler(self, conParams.originalParameter, conParams.state);
    }
    else
    {
        *mmsError = err;
    }

    Semaphore_destroy(conParams.sem);

    self->stateChangedHandler = conParams.originalHandler;
    self->stateChangedHandlerParameter = conParams.originalParameter;

    return success;
}

void
MmsConnection_connectAsync(MmsConnection self, MmsError* mmsError, const char* serverName, int serverPort)
{
    if (serverPort == -1)
    {
#if (CONFIG_MMS_SUPPORT_TLS == 1)
        if (self->isoParameters->tlsConfiguration)
            serverPort = 3782;
        else
            serverPort = 102;
#else
        serverPort = 102;
#endif
    }

#if (CONFIG_MMS_THREADLESS_STACK == 0)
    if (self->createThread)
    {
        if (self->connectionHandlingThread == NULL)
        {
            self->connectionHandlingThread = Thread_create(connectionHandlingThread, self, false);
            self->connectionThreadRunning = true;
            Thread_start(self->connectionHandlingThread);
        }
    }
#endif

    IsoConnectionParameters_setTcpParameters(self->isoParameters, serverName, serverPort);

    if (self->parameters.maxPduSize == -1)
        self->parameters.maxPduSize = CONFIG_MMS_MAXIMUM_PDU_SIZE;

    ByteBuffer* payload = IsoClientConnection_allocateTransmitBuffer(self->isoClient);

    mmsClient_createInitiateRequest(self, payload);

#if (CONFIG_MMS_RAW_MESSAGE_LOGGING == 1)
    if (self->rawMmsMessageHandler != NULL)
    {
        MmsRawMessageHandler handler = (MmsRawMessageHandler) self->rawMmsMessageHandler;
        handler(self->rawMmsMessageHandlerParameter, payload->buffer, payload->size, false);
    }
#endif /* (CONFIG_MMS_RAW_MESSAGE_LOGGING == 1) */

    if (IsoClientConnection_associateAsync(self->isoClient, self->connectTimeout, self->requestTimeout))
    {
        setConnectionState(self, MMS_CONNECTION_STATE_CONNECTING);
        *mmsError = MMS_ERROR_NONE;
    }
    else {
        *mmsError = MMS_ERROR_OTHER;
    }
}

bool
MmsConnection_tick(MmsConnection self)
{
    return IsoClientConnection_handleConnection(self->isoClient);
}

void
MmsConnection_close(MmsConnection self)
{
    self->connectionLostHandler = NULL;

    if (getConnectionState(self) == MMS_CONNECTION_STATE_CONNECTED)
        IsoClientConnection_close(self->isoClient);
}

void
MmsConnection_abortAsync(MmsConnection self, MmsError* mmsError)
{
    self->connectionLostHandler = NULL;

    if (getConnectionState(self) == MMS_CONNECTION_STATE_CONNECTED)
    {
        IsoClientConnection_abortAsync(self->isoClient);
        *mmsError = MMS_ERROR_NONE;
    }
    else {
        *mmsError = MMS_ERROR_CONNECTION_LOST;
    }
}

void
MmsConnection_abort(MmsConnection self, MmsError* mmsError)
{
    *mmsError = MMS_ERROR_NONE;

    self->connectionLostHandler = NULL;

    bool success = false;

    if (getConnectionState(self) == MMS_CONNECTION_STATE_CONNECTED)
    {
        IsoClientConnection_abortAsync(self->isoClient);

        uint64_t timeout = Hal_getTimeInMs() + self->requestTimeout;

        while (Hal_getTimeInMs() < timeout)
        {
            if (getConnectionState(self) == MMS_CONNECTION_STATE_CLOSED)
            {
                success = true;
                break;
            }
            else {
                Thread_sleep(10);
            }
        }
    }
    
    if (success == false)
    {
        IsoClientConnection_close(self->isoClient);
        *mmsError = MMS_ERROR_SERVICE_TIMEOUT;
    }

    IsoClientConnection_close(self->isoClient);
}

struct concludeParameters
{
    Semaphore sem;
    MmsError err;
    bool success;
};

static void
concludeHandler(void* parameter, MmsError mmsError, bool success)
{
    struct concludeParameters* parameters = (struct concludeParameters*) parameter;

    parameters->err = mmsError;
    parameters->success = success;

    /* unblock user thread */
    Semaphore_post(parameters->sem);
}

void
MmsConnection_conclude(MmsConnection self, MmsError* mmsError)
{
    MmsError err = MMS_ERROR_NONE;

    struct concludeParameters parameter;

    parameter.sem = Semaphore_create(1);;
    parameter.success = false;
    parameter.err = MMS_ERROR_NONE;

    Semaphore_wait(parameter.sem);

    MmsConnection_concludeAsync(self, &err, concludeHandler, &parameter);

    if (err == MMS_ERROR_NONE)
    {
        Semaphore_wait(parameter.sem);
        err = parameter.err;
    }

    Semaphore_destroy(parameter.sem);

    if (mmsError)
        *mmsError = err;
}

void
MmsConnection_concludeAsync(MmsConnection self, MmsError* mmsError, MmsConnection_ConcludeAbortHandler handler, void* parameter)
{
    if (getConnectionState(self) != MMS_CONNECTION_STATE_CONNECTED)
    {
        *mmsError = MMS_ERROR_CONNECTION_LOST;
        goto exit_function;
    }

    *mmsError = MMS_ERROR_NONE;

    ByteBuffer* concludeMessage = IsoClientConnection_allocateTransmitBuffer(self->isoClient);

    mmsClient_createConcludeRequest(self, concludeMessage);

    self->concludeHandler = handler;
    self->concludeHandlerParameter = parameter;
    self->concludeTimeout = Hal_getTimeInMs() + self->requestTimeout;

    IsoClientConnection_sendMessage(self->isoClient, concludeMessage);

exit_function:
    return;
}

void
MmsConnection_setInformationReportHandler(MmsConnection self, MmsInformationReportHandler handler,
        void* parameter)
{
    self->reportHandler = handler;
    self->reportHandlerParameter = parameter;
}

static void
mmsClient_getNameListSingleRequestAsync(
        MmsConnection self,
        uint32_t* usedInvokeId,
        MmsError* mmsError,
        const char* domainId,
        MmsObjectClass objectClass,
        bool associationSpecific,
        const char* continueAfter,
        MmsConnection_GetNameListHandler handler,
        void* parameter,
        LinkedList nameList)
{
    if (getConnectionState(self) != MMS_CONNECTION_STATE_CONNECTED)
    {
        if (mmsError)
            *mmsError = MMS_ERROR_CONNECTION_LOST;
        goto exit_function;
    }

    ByteBuffer* payload = IsoClientConnection_allocateTransmitBuffer(self->isoClient);

    uint32_t invokeId = getNextInvokeId(self);

    if (usedInvokeId)
        *usedInvokeId = invokeId;

    if (associationSpecific)
        mmsClient_createMmsGetNameListRequestAssociationSpecific(invokeId,
                payload, continueAfter);
    else
    {
        if (objectClass == MMS_OBJECT_CLASS_DOMAIN)
            mmsClient_createMmsGetNameListRequestVMDspecific(invokeId,
                    payload, continueAfter);
        else
            mmsClient_createGetNameListRequestDomainOrVMDSpecific(invokeId, domainId,
                    payload, objectClass, continueAfter);
    }

    MmsClientInternalParameter intParam;
    intParam.ptr = nameList;

    MmsError err = sendAsyncRequest(self, invokeId, payload, MMS_CALL_TYPE_GET_NAME_LIST, handler, parameter, intParam);

    if (mmsError)
        *mmsError = err;

exit_function:
    return;
}

struct getNameListParameters
{
    Semaphore sem;
    MmsError err;
    LinkedList nameList;
    bool moreFollows;
};

static void
getNameListHandler(uint32_t invokeId, void* parameter, MmsError mmsError, LinkedList nameList, bool moreFollows)
{
    (void)invokeId;

    struct getNameListParameters* parameters = (struct getNameListParameters*) parameter;

    parameters->err = mmsError;
    parameters->nameList = nameList;
    parameters->moreFollows = moreFollows;

    /* unblock user thread */
    Semaphore_post(parameters->sem);
}

static LinkedList /* <char*> */
mmsClient_getNameList(MmsConnection self, MmsError *mmsError,
        const char* domainId,
        MmsObjectClass objectClass,
        bool associationSpecific)
{
    LinkedList list = NULL;
    bool moreFollows = false;

    struct getNameListParameters parameter;

    MmsError err;

    parameter.sem = Semaphore_create(1);;
    parameter.moreFollows = false;
    parameter.nameList = NULL;

    Semaphore_wait(parameter.sem);

    mmsClient_getNameListSingleRequestAsync(self, NULL, &err, domainId, objectClass, associationSpecific, NULL,
            getNameListHandler, &parameter, NULL);

    if (err == MMS_ERROR_NONE)
    {
        Semaphore_wait(parameter.sem);
        err = parameter.err;
        list = parameter.nameList;
        moreFollows = parameter.moreFollows;
    }

    Semaphore_destroy(parameter.sem);

    while (moreFollows)
    {
        parameter.sem = Semaphore_create(1);

        char* continueAfter = NULL;

        if (list)
            continueAfter = (char*) LinkedList_getLastElement(list)->data;

        Semaphore_wait(parameter.sem);

        mmsClient_getNameListSingleRequestAsync(self, NULL, &err, domainId, objectClass, associationSpecific, continueAfter,
               getNameListHandler, &parameter, list);

        if (err == MMS_ERROR_NONE)
        {
            Semaphore_wait(parameter.sem);
            err = parameter.err;
            list = parameter.nameList;
            moreFollows = parameter.moreFollows;
        }
        else
        {
            /* exit loop when message cannot be sent */
            moreFollows = false;
        }

        Semaphore_destroy(parameter.sem);
    }

    if (mmsError)
        *mmsError = err;

    if (err != MMS_ERROR_NONE)
    {
        if (list)
        {
            LinkedList_destroy(list);
            list = NULL;
        }
    }

    return list;
}

LinkedList /* <char*> */
MmsConnection_getVMDVariableNames(MmsConnection self, MmsError* mmsError)
{
    return mmsClient_getNameList(self, mmsError, NULL, MMS_OBJECT_CLASS_NAMED_VARIABLE, false);
}

void
MmsConnection_getVMDVariableNamesAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError, const char* continueAfter,
        MmsConnection_GetNameListHandler handler, void* parameter)
{
    mmsClient_getNameListSingleRequestAsync(self, usedInvokeId, mmsError, NULL, MMS_OBJECT_CLASS_NAMED_VARIABLE,
            false, continueAfter, handler, parameter, NULL);
}

LinkedList /* <char*> */
MmsConnection_getDomainNames(MmsConnection self, MmsError* mmsError)
{
    return mmsClient_getNameList(self, mmsError, NULL, MMS_OBJECT_CLASS_DOMAIN, false);
}

void
MmsConnection_getDomainNamesAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError, const char* continueAfter, LinkedList result,
        MmsConnection_GetNameListHandler handler, void* parameter)
{
    mmsClient_getNameListSingleRequestAsync(self, usedInvokeId, mmsError, NULL, MMS_OBJECT_CLASS_DOMAIN, false,
            continueAfter, handler, parameter, result);
}

LinkedList /* <char*> */
MmsConnection_getDomainVariableNames(MmsConnection self, MmsError* mmsError, const char* domainId)
{
    return mmsClient_getNameList(self, mmsError, domainId, MMS_OBJECT_CLASS_NAMED_VARIABLE, false);
}

void
MmsConnection_getDomainVariableNamesAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError, const char* domainId,
        const char* continueAfter, LinkedList result, MmsConnection_GetNameListHandler handler, void* parameter)
{
    mmsClient_getNameListSingleRequestAsync(self, usedInvokeId, mmsError, domainId, MMS_OBJECT_CLASS_NAMED_VARIABLE, false,
            continueAfter, handler, parameter, result);
}

LinkedList /* <char*> */
MmsConnection_getDomainVariableListNames(MmsConnection self, MmsError* mmsError, const char* domainId)
{
    return mmsClient_getNameList(self, mmsError, domainId, MMS_OBJECT_CLASS_NAMED_VARIABLE_LIST, false);
}

void
MmsConnection_getDomainVariableListNamesAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError, const char* domainId,
        const char* continueAfter, LinkedList result, MmsConnection_GetNameListHandler handler, void* parameter)
{
    mmsClient_getNameListSingleRequestAsync(self, usedInvokeId, mmsError, domainId, MMS_OBJECT_CLASS_NAMED_VARIABLE_LIST, false,
            continueAfter, handler, parameter, result);
}

LinkedList /* <char*> */
MmsConnection_getDomainJournals(MmsConnection self, MmsError* mmsError, const char* domainId)
{
    return mmsClient_getNameList(self, mmsError, domainId, MMS_OBJECT_CLASS_JOURNAL, false);
}

void
MmsConnection_getDomainJournalsAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError, const char* domainId,
        const char* continueAfter, MmsConnection_GetNameListHandler handler, void* parameter)
{
    mmsClient_getNameListSingleRequestAsync(self, usedInvokeId, mmsError, domainId, MMS_OBJECT_CLASS_JOURNAL, false,
            continueAfter, handler, parameter, NULL);
}

LinkedList /* <char*> */
MmsConnection_getVariableListNamesAssociationSpecific(MmsConnection self, MmsError* mmsError)
{
    return mmsClient_getNameList(self, mmsError, NULL, MMS_OBJECT_CLASS_NAMED_VARIABLE_LIST, true);
}

void
MmsConnection_getVariableListNamesAssociationSpecificAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError,
        const char* continueAfter, MmsConnection_GetNameListHandler handler, void* parameter)
{
    mmsClient_getNameListSingleRequestAsync(self, usedInvokeId, mmsError, NULL, MMS_OBJECT_CLASS_NAMED_VARIABLE_LIST, true,
            continueAfter, handler, parameter, NULL);
}

struct readNVParameters
{
    Semaphore sem;
    MmsError err;
    MmsValue* value;
};

static void
readVariableHandler(uint32_t invokeId, void* parameter, MmsError mmsError, MmsValue* value)
{
    (void)invokeId;

    struct readNVParameters* parameters = (struct readNVParameters*) parameter;

    parameters->err = mmsError;
    parameters->value = value;

    /* unblock user thread */
    Semaphore_post(parameters->sem);
}

void
MmsConnection_readVariableAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError, const char* domainId, const char* itemId,
        MmsConnection_ReadVariableHandler handler, void* parameter)
{
    if (getConnectionState(self) != MMS_CONNECTION_STATE_CONNECTED)
    {
        if (mmsError)
            *mmsError = MMS_ERROR_CONNECTION_LOST;

        goto exit_function;
    }

    ByteBuffer* payload = IsoClientConnection_allocateTransmitBuffer(self->isoClient);

    uint32_t invokeId = getNextInvokeId(self);

    if (usedInvokeId)
        *usedInvokeId = invokeId;

    mmsClient_createReadRequest(invokeId, domainId, itemId, payload);

    MmsClientInternalParameter intParam;
    intParam.ptr = NULL;

    MmsError err = sendAsyncRequest(self, invokeId, payload, MMS_CALL_TYPE_READ_VARIABLE, handler, parameter, intParam);

    if (mmsError)
        *mmsError = err;

exit_function:
    return;
}

MmsValue*
MmsConnection_readVariable(MmsConnection self, MmsError* mmsError,
        const char* domainId, const char* itemId)
{
    MmsValue* value = NULL;
    MmsError err = MMS_ERROR_NONE;

    struct readNVParameters parameter;

    parameter.sem = Semaphore_create(1);;
    parameter.value = NULL;
    parameter.err = MMS_ERROR_NONE;

    Semaphore_wait(parameter.sem);

    MmsConnection_readVariableAsync(self, NULL, &err, domainId, itemId, readVariableHandler, &parameter);

    if (err == MMS_ERROR_NONE)
    {
        Semaphore_wait(parameter.sem);

        value = parameter.value;
        err = parameter.err;
    }

    Semaphore_destroy(parameter.sem);

    if (mmsError)
        *mmsError = err;

    return value;
}

void
MmsConnection_readVariableComponentAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError,
        const char* domainId, const char* itemId, const char* componentId,
        MmsConnection_ReadVariableHandler handler, void* parameter)
{
    if (getConnectionState(self) != MMS_CONNECTION_STATE_CONNECTED)
    {
        if (mmsError)
            *mmsError = MMS_ERROR_CONNECTION_LOST;

        goto exit_function;
    }

    ByteBuffer* payload = IsoClientConnection_allocateTransmitBuffer(self->isoClient);

    uint32_t invokeId = getNextInvokeId(self);

    if (usedInvokeId)
        *usedInvokeId = invokeId;

    mmsClient_createReadRequestComponent(invokeId, domainId, itemId, componentId, payload);

    MmsClientInternalParameter intParam;
    intParam.ptr = NULL;

    MmsError err = sendAsyncRequest(self, invokeId, payload, MMS_CALL_TYPE_READ_VARIABLE, handler, parameter, intParam);

    if (mmsError)
        *mmsError = err;

exit_function:
    return;
}

MmsValue*
MmsConnection_readVariableComponent(MmsConnection self, MmsError* mmsError,
        const char* domainId, const char* itemId, const char* componentId)
{
    MmsValue* value = NULL;
    MmsError err = MMS_ERROR_NONE;

    struct readNVParameters parameter;

    parameter.sem = Semaphore_create(1);;
    parameter.value = NULL;
    parameter.err = MMS_ERROR_NONE;

    Semaphore_wait(parameter.sem);

    MmsConnection_readVariableComponentAsync(self, NULL, &err, domainId, itemId, componentId, readVariableHandler, &parameter);

    if (err == MMS_ERROR_NONE)
    {
        Semaphore_wait(parameter.sem);

        value = parameter.value;
        err = parameter.err;
    }

    Semaphore_destroy(parameter.sem);

    if (mmsError)
        *mmsError = err;

    return value;
}

MmsValue*
MmsConnection_readArrayElements(MmsConnection self, MmsError* mmsError,
        const char* domainId, const char* itemId,
        uint32_t startIndex, uint32_t numberOfElements)
{
    MmsValue* value = NULL;
    MmsError err = MMS_ERROR_NONE;

    struct readNVParameters parameter;

    parameter.sem = Semaphore_create(1);;
    parameter.value = NULL;
    parameter.err = MMS_ERROR_NONE;

    Semaphore_wait(parameter.sem);

    MmsConnection_readArrayElementsAsync(self, NULL, &err, domainId, itemId, startIndex, numberOfElements,
            readVariableHandler, &parameter);

    if (err == MMS_ERROR_NONE)
    {
        Semaphore_wait(parameter.sem);

        value = parameter.value;
        err = parameter.err;
    }

    Semaphore_destroy(parameter.sem);

    if (mmsError)
        *mmsError = err;

    return value;
}

void
MmsConnection_readArrayElementsAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError, const char* domainId, const char* itemId,
        uint32_t startIndex, uint32_t numberOfElements,
        MmsConnection_ReadVariableHandler handler, void* parameter)
{
    if (getConnectionState(self) != MMS_CONNECTION_STATE_CONNECTED)
    {
        if (mmsError)
            *mmsError = MMS_ERROR_CONNECTION_LOST;
        goto exit_function;
    }

    ByteBuffer* payload = IsoClientConnection_allocateTransmitBuffer(self->isoClient);

    uint32_t invokeId = getNextInvokeId(self);

    if (usedInvokeId)
        *usedInvokeId = invokeId;

    mmsClient_createReadRequestAlternateAccessIndex(invokeId, domainId, itemId, startIndex,
            numberOfElements, payload);

    MmsClientInternalParameter intParam;
    intParam.ptr = NULL;

    MmsError err = sendAsyncRequest(self, invokeId, payload, MMS_CALL_TYPE_READ_VARIABLE, handler, parameter, intParam);

    if (mmsError)
        *mmsError = err;

exit_function:
    return;
}

MmsValue*
MmsConnection_readSingleArrayElementWithComponent(MmsConnection self, MmsError* mmsError,
        const char* domainId, const char* itemId, uint32_t index, const char* componentId)
{
    MmsValue* value = NULL;
    MmsError err = MMS_ERROR_NONE;

    struct readNVParameters parameter;

    parameter.sem = Semaphore_create(1);;
    parameter.value = NULL;
    parameter.err = MMS_ERROR_NONE;

    Semaphore_wait(parameter.sem);

    MmsConnection_readSingleArrayElementWithComponentAsync(self, NULL, &err, domainId, itemId, index, componentId,
            readVariableHandler, &parameter);

    if (err == MMS_ERROR_NONE)
    {
        Semaphore_wait(parameter.sem);

        value = parameter.value;
        err = parameter.err;
    }

    Semaphore_destroy(parameter.sem);

    if (mmsError)
        *mmsError = err;

    return value;
}

void
MmsConnection_readSingleArrayElementWithComponentAsync(MmsConnection self, uint32_t* usedInvokeId,
        MmsError* mmsError,
        const char* domainId, const char* itemId,
        uint32_t index, const char* componentId,
        MmsConnection_ReadVariableHandler handler, void* parameter)
{
    if (getConnectionState(self) != MMS_CONNECTION_STATE_CONNECTED)
    {
        if (mmsError)
            *mmsError = MMS_ERROR_CONNECTION_LOST;
        goto exit_function;
    }

    ByteBuffer* payload = IsoClientConnection_allocateTransmitBuffer(self->isoClient);

    uint32_t invokeId = getNextInvokeId(self);

    if (usedInvokeId)
        *usedInvokeId = invokeId;

    mmsClient_createReadRequestAlternateAccessSingleIndexComponent(invokeId, domainId, itemId, index, componentId,
            payload);

    MmsClientInternalParameter intParam;
    intParam.ptr = NULL;

    MmsError err = sendAsyncRequest(self, invokeId, payload, MMS_CALL_TYPE_READ_VARIABLE, handler, parameter, intParam);

    if (mmsError)
        *mmsError = err;

exit_function:
    return;
}

MmsValue*
MmsConnection_readMultipleVariables(MmsConnection self, MmsError* mmsError,
        const char* domainId, LinkedList /*<char*>*/items)
{
    MmsValue* value = NULL;
    MmsError err = MMS_ERROR_NONE;

    struct readNVParameters parameter;

    parameter.sem = Semaphore_create(1);;
    parameter.value = NULL;
    parameter.err = MMS_ERROR_NONE;

    Semaphore_wait(parameter.sem);

    MmsConnection_readMultipleVariablesAsync(self, NULL, &err, domainId, items,
            readVariableHandler, &parameter);

    if (err == MMS_ERROR_NONE)
    {
        Semaphore_wait(parameter.sem);

        value = parameter.value;
        err = parameter.err;
    }

    Semaphore_destroy(parameter.sem);

    if (mmsError)
        *mmsError = err;

    return value;
}

void
MmsConnection_readMultipleVariablesAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError,
        const char* domainId, LinkedList /*<char*>*/items,
        MmsConnection_ReadVariableHandler handler, void* parameter)
{
    if (getConnectionState(self) != MMS_CONNECTION_STATE_CONNECTED)
    {
        if (mmsError)
            *mmsError = MMS_ERROR_CONNECTION_LOST;
        goto exit_function;
    }

    ByteBuffer* payload = IsoClientConnection_allocateTransmitBuffer(self->isoClient);

    uint32_t invokeId = getNextInvokeId(self);

    if (usedInvokeId)
        *usedInvokeId = invokeId;

    if (mmsClient_createReadRequestMultipleValues(invokeId, domainId, items, payload) > 0)
    {
        MmsClientInternalParameter intParam;
        intParam.ptr = NULL;

        MmsError err = sendAsyncRequest(self, invokeId, payload, MMS_CALL_TYPE_READ_MULTIPLE_VARIABLES, handler, parameter, intParam);

        if (mmsError)
            *mmsError = err;
    }
    else
    {
        if (mmsError)
            *mmsError = MMS_ERROR_RESOURCE_CAPABILITY_UNAVAILABLE;
    }

exit_function:
    return;
}

MmsValue*
MmsConnection_readNamedVariableListValues(MmsConnection self, MmsError* mmsError,
        const char* domainId, const char* listName,
        bool specWithResult)
{
    MmsValue* value = NULL;
    MmsError err = MMS_ERROR_NONE;

    struct readNVParameters parameter;

    parameter.sem = Semaphore_create(1);;
    parameter.value = NULL;
    parameter.err = MMS_ERROR_NONE;

    Semaphore_wait(parameter.sem);

    MmsConnection_readNamedVariableListValuesAsync(self, NULL, &err, domainId, listName, specWithResult,
            readVariableHandler, &parameter);

    if (err == MMS_ERROR_NONE)
    {
        Semaphore_wait(parameter.sem);

        value = parameter.value;
        err = parameter.err;
    }

    Semaphore_destroy(parameter.sem);

    if (mmsError)
        *mmsError = err;

    return value;
}

void
MmsConnection_readNamedVariableListValuesAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError,
        const char* domainId, const char* listName, bool specWithResult,
        MmsConnection_ReadVariableHandler handler, void* parameter)
{
    if (getConnectionState(self) != MMS_CONNECTION_STATE_CONNECTED)
    {
        if (mmsError)
            *mmsError = MMS_ERROR_CONNECTION_LOST;
        goto exit_function;
    }

    ByteBuffer* payload = IsoClientConnection_allocateTransmitBuffer(self->isoClient);

    uint32_t invokeId = getNextInvokeId(self);

    if (usedInvokeId)
        *usedInvokeId = invokeId;

    mmsClient_createReadNamedVariableListRequest(invokeId, domainId, listName,
            payload, specWithResult);

    MmsClientInternalParameter intParam;
    intParam.ptr = NULL;

    MmsError err = sendAsyncRequest(self, invokeId, payload, MMS_CALL_TYPE_READ_MULTIPLE_VARIABLES, handler, parameter, intParam);

    if (mmsError)
        *mmsError = err;

exit_function:
    return;
}

MmsValue*
MmsConnection_readNamedVariableListValuesAssociationSpecific(
        MmsConnection self, MmsError* mmsError,
        const char* listName,
        bool specWithResult)
{
    MmsValue* value = NULL;
    MmsError err = MMS_ERROR_NONE;

    struct readNVParameters parameter;

    parameter.sem = Semaphore_create(1);;
    parameter.value = NULL;
    parameter.err = MMS_ERROR_NONE;

    Semaphore_wait(parameter.sem);

    MmsConnection_readNamedVariableListValuesAssociationSpecificAsync(self, NULL, &err, listName, specWithResult,
            readVariableHandler, &parameter);

    if (err == MMS_ERROR_NONE)
    {
        Semaphore_wait(parameter.sem);

        value = parameter.value;
        err = parameter.err;
    }

    Semaphore_destroy(parameter.sem);

    if (mmsError)
        *mmsError = err;

    return value;
}

void
MmsConnection_readNamedVariableListValuesAssociationSpecificAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError,
        const char* listName, bool specWithResult,
        MmsConnection_ReadVariableHandler handler, void* parameter)
{
    if (getConnectionState(self) != MMS_CONNECTION_STATE_CONNECTED)
    {
        if (mmsError)
            *mmsError = MMS_ERROR_CONNECTION_LOST;
        goto exit_function;
    }

    ByteBuffer* payload = IsoClientConnection_allocateTransmitBuffer(self->isoClient);

    uint32_t invokeId = getNextInvokeId(self);

    if (usedInvokeId)
        *usedInvokeId = invokeId;

    mmsClient_createReadAssociationSpecificNamedVariableListRequest(invokeId, listName,
            payload, specWithResult);

    MmsClientInternalParameter intParam;
    intParam.ptr = NULL;

    MmsError err = sendAsyncRequest(self, invokeId, payload, MMS_CALL_TYPE_READ_MULTIPLE_VARIABLES, handler, parameter, intParam);

    if (mmsError)
        *mmsError = err;

exit_function:
    return;
}

struct readNVLDirectoryParameters
{
    Semaphore waitForResponse;
    MmsError err;
    LinkedList specs;
    bool deletable;
};

static void
readNVLDirectoryHandler(uint32_t invokeId, void* parameter, MmsError mmsError, LinkedList /* <MmsVariableAccessSpecification*> */ specs, bool deletable)
{
    (void)invokeId;

    struct readNVLDirectoryParameters* parameters = (struct readNVLDirectoryParameters*) parameter;

    parameters->err = mmsError;
    parameters->deletable = deletable;
    parameters->specs = specs;

    /* unblock user thread */
    Semaphore_post(parameters->waitForResponse);
}

LinkedList /* <MmsVariableAccessSpecification*> */
MmsConnection_readNamedVariableListDirectory(MmsConnection self, MmsError* mmsError,
        const char* domainId, const char* listName, bool* deletable)
{
    LinkedList specs = NULL;

    Semaphore waitForResponse = Semaphore_create(1);

    Semaphore_wait(waitForResponse);

    struct readNVLDirectoryParameters parameter;

    MmsError err;

    parameter.waitForResponse = waitForResponse;

    MmsConnection_readNamedVariableListDirectoryAsync(self, NULL, &err, domainId, listName,
            readNVLDirectoryHandler, &parameter);

    if (err == MMS_ERROR_NONE)
    {
        Semaphore_wait(waitForResponse);
        err = parameter.err;
        specs = parameter.specs;

        if (deletable)
            *deletable = parameter.deletable;
    }

    Semaphore_destroy(waitForResponse);

    if (mmsError)
        *mmsError = err;

    return specs;
}

void
MmsConnection_readNamedVariableListDirectoryAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError,
        const char* domainId, const char* listName,
        MmsConnection_ReadNVLDirectoryHandler handler, void* parameter)
{
    if (getConnectionState(self) != MMS_CONNECTION_STATE_CONNECTED)
    {
        if (mmsError)
            *mmsError = MMS_ERROR_CONNECTION_LOST;
        goto exit_function;
    }

    ByteBuffer* payload = IsoClientConnection_allocateTransmitBuffer(self->isoClient);

    uint32_t invokeId = getNextInvokeId(self);

    if (usedInvokeId)
        *usedInvokeId = invokeId;

    mmsClient_createGetNamedVariableListAttributesRequest(invokeId, payload, domainId,
            listName);

    MmsClientInternalParameter intParam;
    intParam.ptr = NULL;

    MmsError err = sendAsyncRequest(self, invokeId, payload, MMS_CALL_TYPE_READ_NVL_DIRECTORY, handler, parameter, intParam);

    if (mmsError)
        *mmsError = err;

exit_function:
    return;
}

LinkedList /* <MmsVariableAccessSpecification*> */
MmsConnection_readNamedVariableListDirectoryAssociationSpecific(MmsConnection self, MmsError* mmsError,
        const char* listName, bool* deletable)
{
    LinkedList specs = NULL;

    Semaphore waitForResponse = Semaphore_create(1);

    Semaphore_wait(waitForResponse);

    struct readNVLDirectoryParameters parameter;

    MmsError err;

    parameter.waitForResponse = waitForResponse;

    MmsConnection_readNamedVariableListDirectoryAssociationSpecificAsync(self, NULL, &err, listName,
            readNVLDirectoryHandler, &parameter);

    if (err == MMS_ERROR_NONE)
    {
        Semaphore_wait(waitForResponse);
        err = parameter.err;
        specs = parameter.specs;

        if (deletable)
            *deletable = parameter.deletable;
    }

    Semaphore_destroy(waitForResponse);

    if (mmsError)
        *mmsError = err;

    return specs;
}

void
MmsConnection_readNamedVariableListDirectoryAssociationSpecificAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError,
        const char* listName,
        MmsConnection_ReadNVLDirectoryHandler handler, void* parameter)
{
    if (getConnectionState(self) != MMS_CONNECTION_STATE_CONNECTED)
    {
        if (mmsError)
            *mmsError = MMS_ERROR_CONNECTION_LOST;
        goto exit_function;
    }

    ByteBuffer* payload = IsoClientConnection_allocateTransmitBuffer(self->isoClient);

    uint32_t invokeId = getNextInvokeId(self);

    if (usedInvokeId)
        *usedInvokeId = invokeId;

    mmsClient_createGetNamedVariableListAttributesRequestAssociationSpecific(invokeId, payload,
            listName);

    MmsClientInternalParameter intParam;
    intParam.ptr = NULL;

    MmsError err = sendAsyncRequest(self, invokeId, payload, MMS_CALL_TYPE_READ_NVL_DIRECTORY, handler, parameter, intParam);

    if (mmsError)
        *mmsError = err;

exit_function:
    return;
}

struct defineNVLParameters
{
    Semaphore waitForResponse;
    MmsError err;
    bool success;
};

static void
defineNVLHandler(uint32_t invokeId, void* parameter, MmsError mmsError, bool success)
{
    (void)invokeId;

    struct defineNVLParameters* parameters = (struct defineNVLParameters*) parameter;

    parameters->err = mmsError;
    parameters->success = success;

    /* unblock user thread */
    Semaphore_post(parameters->waitForResponse);
}


void
MmsConnection_defineNamedVariableList(MmsConnection self, MmsError* mmsError,
        const char* domainId, const char* listName, LinkedList variableSpecs)
{
    struct defineNVLParameters parameter;

    MmsError err;

    parameter.waitForResponse = Semaphore_create(1);

    Semaphore_wait(parameter.waitForResponse);

    MmsConnection_defineNamedVariableListAsync(self, NULL, &err, domainId, listName, variableSpecs,
            defineNVLHandler, &parameter);

    if (err == MMS_ERROR_NONE)
    {
        Semaphore_wait(parameter.waitForResponse);
        err = parameter.err;
    }

    Semaphore_destroy(parameter.waitForResponse);

    if (mmsError)
        *mmsError = err;
}

void
MmsConnection_defineNamedVariableListAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError, const char* domainId,
        const char* listName, LinkedList variableSpecs,
        MmsConnection_GenericServiceHandler handler, void* parameter)
{
    if (getConnectionState(self) != MMS_CONNECTION_STATE_CONNECTED)
    {
        if (mmsError)
            *mmsError = MMS_ERROR_CONNECTION_LOST;
        goto exit_function;
    }

    ByteBuffer* payload = IsoClientConnection_allocateTransmitBuffer(self->isoClient);

    uint32_t invokeId = getNextInvokeId(self);

    if (usedInvokeId)
        *usedInvokeId = invokeId;

    mmsClient_createDefineNamedVariableListRequest(invokeId, payload, domainId,
            listName, variableSpecs, false);

    MmsClientInternalParameter intParam;
    intParam.ptr = NULL;

    MmsError err = sendAsyncRequest(self, invokeId, payload, MMS_CALL_TYPE_DEFINE_NVL, handler, parameter, intParam);

    if (mmsError)
        *mmsError = err;

exit_function:
    return;
}

void
MmsConnection_defineNamedVariableListAssociationSpecific(MmsConnection self,
        MmsError* mmsError, const char* listName, LinkedList variableSpecs)
{
    struct defineNVLParameters parameter;

    MmsError err;

    parameter.waitForResponse = Semaphore_create(1);

    Semaphore_wait(parameter.waitForResponse);

    MmsConnection_defineNamedVariableListAssociationSpecificAsync(self, NULL, &err, listName, variableSpecs,
            defineNVLHandler, &parameter);

    if (err == MMS_ERROR_NONE)
    {
        Semaphore_wait(parameter.waitForResponse);
        err = parameter.err;
    }

    Semaphore_destroy(parameter.waitForResponse);

    if (mmsError)
        *mmsError = err;
}

void
MmsConnection_defineNamedVariableListAssociationSpecificAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError,
        const char* listName, LinkedList variableSpecs,
        MmsConnection_GenericServiceHandler handler, void* parameter)
{
    if (getConnectionState(self) != MMS_CONNECTION_STATE_CONNECTED)
    {
        if (mmsError)
            *mmsError = MMS_ERROR_CONNECTION_LOST;
        goto exit_function;
    }

    ByteBuffer* payload = IsoClientConnection_allocateTransmitBuffer(self->isoClient);

    uint32_t invokeId = getNextInvokeId(self);

    if (usedInvokeId)
        *usedInvokeId = invokeId;

    mmsClient_createDefineNamedVariableListRequest(invokeId, payload, NULL,
            listName, variableSpecs, true);

    MmsClientInternalParameter intParam;
    intParam.ptr = NULL;

    MmsError err = sendAsyncRequest(self, invokeId, payload, MMS_CALL_TYPE_DEFINE_NVL, handler, parameter, intParam);

    if (mmsError)
        *mmsError = err;

exit_function:
    return;
}

bool
MmsConnection_deleteNamedVariableList(MmsConnection self, MmsError* mmsError,
        const char* domainId, const char* listName)
{
    bool isDeleted = false;

    struct defineNVLParameters parameter;

    MmsError err;

    parameter.waitForResponse = Semaphore_create(1);

    Semaphore_wait(parameter.waitForResponse);

    MmsConnection_deleteNamedVariableListAsync(self, NULL, &err, domainId, listName, defineNVLHandler, &parameter);

    if (err == MMS_ERROR_NONE)
    {
        Semaphore_wait(parameter.waitForResponse);
        err = parameter.err;
        isDeleted = parameter.success;
    }

    Semaphore_destroy(parameter.waitForResponse);

    if (mmsError)
        *mmsError = err;

    return isDeleted;
}

void
MmsConnection_deleteNamedVariableListAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError, const char* domainId, const char* listName,
        MmsConnection_GenericServiceHandler handler, void* parameter)
{
    if (getConnectionState(self) != MMS_CONNECTION_STATE_CONNECTED)
    {
        if (mmsError)
            *mmsError = MMS_ERROR_CONNECTION_LOST;
        goto exit_function;
    }

    ByteBuffer* payload = IsoClientConnection_allocateTransmitBuffer(self->isoClient);

    uint32_t invokeId = getNextInvokeId(self);

    if (usedInvokeId)
        *usedInvokeId = invokeId;

    mmsClient_createDeleteNamedVariableListRequest(invokeId, payload, domainId, listName);

    MmsClientInternalParameter intParam;
    intParam.ptr = NULL;

    MmsError err = sendAsyncRequest(self, invokeId, payload, MMS_CALL_TYPE_DELETE_NVL, handler, parameter, intParam);

    if (mmsError)
        *mmsError = err;

exit_function:
    return;
}

bool
MmsConnection_deleteAssociationSpecificNamedVariableList(MmsConnection self,
        MmsError* mmsError, const char* listName)
{
    bool isDeleted = false;

    struct defineNVLParameters parameter;

    MmsError err;

    parameter.waitForResponse = Semaphore_create(1);

    Semaphore_wait(parameter.waitForResponse);

    MmsConnection_deleteAssociationSpecificNamedVariableListAsync(self, NULL, &err, listName, defineNVLHandler, &parameter);

    if (err == MMS_ERROR_NONE)
    {
        Semaphore_wait(parameter.waitForResponse);
        err = parameter.err;
        isDeleted = parameter.success;
    }

    Semaphore_destroy(parameter.waitForResponse);

    if (mmsError)
        *mmsError = err;

    return isDeleted;
}

void
MmsConnection_deleteAssociationSpecificNamedVariableListAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError, const char* listName,
        MmsConnection_GenericServiceHandler handler, void* parameter)
{
    if (getConnectionState(self) != MMS_CONNECTION_STATE_CONNECTED)
    {
        if (mmsError)
            *mmsError = MMS_ERROR_CONNECTION_LOST;
        goto exit_function;
    }

    ByteBuffer* payload = IsoClientConnection_allocateTransmitBuffer(self->isoClient);

    uint32_t invokeId = getNextInvokeId(self);

    if (usedInvokeId)
        *usedInvokeId = invokeId;

    mmsClient_createDeleteAssociationSpecificNamedVariableListRequest(
            invokeId, payload, listName);

    MmsClientInternalParameter intParam;
    intParam.ptr = NULL;

    MmsError err = sendAsyncRequest(self, invokeId, payload, MMS_CALL_TYPE_DELETE_NVL, handler, parameter, intParam);

    if (mmsError)
        *mmsError = err;

exit_function:
    return;
}

struct getVarAccessAttrParameters
{
    Semaphore waitForResponse;
    MmsError err;
    MmsVariableSpecification* typeSpec;
};

static void
getAccessAttrHandler(uint32_t invokeId, void* parameter, MmsError mmsError, MmsVariableSpecification* typeSpec)
{
    (void)invokeId;

    struct getVarAccessAttrParameters* parameters = (struct getVarAccessAttrParameters*) parameter;

    parameters->err = mmsError;
    parameters->typeSpec = typeSpec;

    /* unblock user thread */
    Semaphore_post(parameters->waitForResponse);
}

MmsVariableSpecification*
MmsConnection_getVariableAccessAttributes(MmsConnection self, MmsError* mmsError,
        const char* domainId, const char* itemId)
{
    MmsVariableSpecification* typeSpec = NULL;

    struct getVarAccessAttrParameters parameter;

    MmsError err;

    parameter.waitForResponse = Semaphore_create(1);

    Semaphore_wait(parameter.waitForResponse);

    MmsConnection_getVariableAccessAttributesAsync(self, NULL, &err, domainId, itemId, getAccessAttrHandler, &parameter);

    if (err == MMS_ERROR_NONE)
    {
        Semaphore_wait(parameter.waitForResponse);
        err = parameter.err;
        typeSpec = parameter.typeSpec;
    }

    Semaphore_destroy(parameter.waitForResponse);

    if (mmsError)
        *mmsError = err;

    return typeSpec;
}

void
MmsConnection_getVariableAccessAttributesAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError,
        const char* domainId, const char* itemId,
        MmsConnection_GetVariableAccessAttributesHandler handler, void* parameter)
{
    if (getConnectionState(self) != MMS_CONNECTION_STATE_CONNECTED)
    {
        if (mmsError)
            *mmsError = MMS_ERROR_CONNECTION_LOST;
        goto exit_function;
    }

    ByteBuffer* payload = IsoClientConnection_allocateTransmitBuffer(self->isoClient);

    uint32_t invokeId = getNextInvokeId(self);

    if (usedInvokeId)
        *usedInvokeId = invokeId;

    mmsClient_createGetVariableAccessAttributesRequest(invokeId, domainId, itemId, payload);

    MmsClientInternalParameter intParam;
    intParam.ptr = NULL;

    MmsError err = sendAsyncRequest(self, invokeId, payload, MMS_CALL_TYPE_GET_VAR_ACCESS_ATTR, handler, parameter, intParam);

    if (mmsError)
        *mmsError = err;

exit_function:
    return;
}

struct identifyParameters
{
    Semaphore waitForResponse;
    MmsError err;
    MmsServerIdentity* identify;
};

static void
identifyHandler(uint32_t invokeId, void* parameter, MmsError mmsError, char* vendorName, char* modelName, char* revision)
{
    (void)invokeId;

    struct identifyParameters* parameters = (struct identifyParameters*) parameter;

    parameters->err = mmsError;

    parameters->identify = (MmsServerIdentity*) GLOBAL_MALLOC(sizeof(MmsServerIdentity));
    parameters->identify->vendorName = StringUtils_copyString(vendorName);
    parameters->identify->modelName = StringUtils_copyString(modelName);
    parameters->identify->revision = StringUtils_copyString(revision);

    /* unblock user thread */
    Semaphore_post(parameters->waitForResponse);
}


MmsServerIdentity*
MmsConnection_identify(MmsConnection self, MmsError* mmsError)
{
    MmsServerIdentity* identity = NULL;

    struct identifyParameters parameter;

    MmsError err = MMS_ERROR_NONE;

    parameter.waitForResponse = Semaphore_create(1);
    parameter.err = MMS_ERROR_NONE;
    parameter.identify = NULL;

    Semaphore_wait(parameter.waitForResponse);

    MmsConnection_identifyAsync(self, NULL, &err, identifyHandler, &parameter);

    if (err == MMS_ERROR_NONE)
    {
        Semaphore_wait(parameter.waitForResponse);
        err = parameter.err;
        identity = parameter.identify;
    }

    Semaphore_destroy(parameter.waitForResponse);

    if (mmsError)
        *mmsError = err;

    return identity;
}

void
MmsConnection_identifyAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError,
        MmsConnection_IdentifyHandler handler, void* parameter)
{
    if (getConnectionState(self) != MMS_CONNECTION_STATE_CONNECTED)
    {
        if (mmsError)
            *mmsError = MMS_ERROR_CONNECTION_LOST;
        goto exit_function;
    }

    ByteBuffer* payload = IsoClientConnection_allocateTransmitBuffer(self->isoClient);

    uint32_t invokeId = getNextInvokeId(self);

    if (usedInvokeId)
        *usedInvokeId = invokeId;

    mmsClient_createIdentifyRequest(invokeId, payload);

    MmsClientInternalParameter intParam;
    intParam.ptr = NULL;

    MmsError err = sendAsyncRequest(self, invokeId, payload, MMS_CALL_TYPE_IDENTIFY, handler, parameter, intParam);

    if (mmsError)
        *mmsError = err;

exit_function:
    return;
}

struct getServerStatusParameters
{
    Semaphore waitForResponse;
    MmsError err;
    int vmdLogicalStatus;
    int vmdPhysicalStatus;
};

static void
getServerStatusHandler(uint32_t invokeId, void* parameter, MmsError mmsError, int vmdLogicalStatus, int vmdPhysicalStatus)
{
    (void)invokeId;

    struct getServerStatusParameters* parameters = (struct getServerStatusParameters*) parameter;

    parameters->err = mmsError;
    parameters->vmdLogicalStatus = vmdLogicalStatus;
    parameters->vmdPhysicalStatus = vmdPhysicalStatus;

    /* unblock user thread */
    Semaphore_post(parameters->waitForResponse);
}


void
MmsConnection_getServerStatus(MmsConnection self, MmsError* mmsError, int* vmdLogicalStatus, int* vmdPhysicalStatus,
        bool extendedDerivation)
{
    struct getServerStatusParameters parameter;

    MmsError err = MMS_ERROR_NONE;

    parameter.waitForResponse = Semaphore_create(1);
    parameter.err = MMS_ERROR_NONE;
    parameter.vmdLogicalStatus = 0;
    parameter.vmdPhysicalStatus = 0;

    Semaphore_wait(parameter.waitForResponse);

    MmsConnection_getServerStatusAsync(self, NULL, &err, extendedDerivation, getServerStatusHandler, &parameter);

    if (err == MMS_ERROR_NONE)
    {
        Semaphore_wait(parameter.waitForResponse);
        err = parameter.err;

        if (vmdLogicalStatus)
            *vmdLogicalStatus = parameter.vmdLogicalStatus;

        if (vmdPhysicalStatus)
            *vmdPhysicalStatus = parameter.vmdPhysicalStatus;
    }

    Semaphore_destroy(parameter.waitForResponse);

    if (mmsError)
        *mmsError = err;
}

void
MmsConnection_getServerStatusAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError, bool extendedDerivation,
        MmsConnection_GetServerStatusHandler handler, void* parameter)
{
    if (getConnectionState(self) != MMS_CONNECTION_STATE_CONNECTED)
    {
        if (mmsError)
            *mmsError = MMS_ERROR_CONNECTION_LOST;
        goto exit_function;
    }

    ByteBuffer* payload = IsoClientConnection_allocateTransmitBuffer(self->isoClient);

    uint32_t invokeId = getNextInvokeId(self);

    if (usedInvokeId)
        *usedInvokeId = invokeId;

    mmsClient_createStatusRequest(invokeId, payload, extendedDerivation);

    MmsClientInternalParameter intParam;
    intParam.ptr = NULL;

    MmsError err = sendAsyncRequest(self, invokeId, payload, MMS_CALL_TYPE_GET_SERVER_STATUS, handler, parameter, intParam);

    if (mmsError)
        *mmsError = err;

exit_function:
    return;
}

static void
MmsJournalVariable_destroy(MmsJournalVariable self)
{
    if (self)
    {
        GLOBAL_FREEMEM(self->tag);
        MmsValue_delete(self->value);
        GLOBAL_FREEMEM(self);
    }
}

void
MmsJournalEntry_destroy(MmsJournalEntry self)
{
    if (self)
    {
        MmsValue_delete(self->entryID);
        MmsValue_delete(self->occurenceTime);
        LinkedList_destroyDeep(self->journalVariables,
                (LinkedListValueDeleteFunction) MmsJournalVariable_destroy);

        GLOBAL_FREEMEM(self);
    }
}

MmsValue*
MmsJournalEntry_getEntryID(MmsJournalEntry self)
{
    return self->entryID;
}

MmsValue*
MmsJournalEntry_getOccurenceTime(MmsJournalEntry self)
{
    return self->occurenceTime;
}

LinkedList /* <MmsJournalVariable> */
MmsJournalEntry_getJournalVariables(MmsJournalEntry self)
{
    return self->journalVariables;
}

const char*
MmsJournalVariable_getTag(MmsJournalVariable self)
{
    return self->tag;
}

MmsValue*
MmsJournalVariable_getValue(MmsJournalVariable self)
{
    return self->value;
}

struct readJournalParameters
{
    Semaphore waitForResponse;
    MmsError err;
    LinkedList entries;
    bool moreFollows;
};

static void
readJournalHandler(uint32_t invokeId, void* parameter, MmsError mmsError, LinkedList entries, bool moreFollows)
{
    (void)invokeId;

    struct readJournalParameters* parameters = (struct readJournalParameters*) parameter;

    parameters->err = mmsError;
    parameters->entries = entries;
    parameters->moreFollows = moreFollows;

    /* unblock user thread */
    Semaphore_post(parameters->waitForResponse);
}


LinkedList
MmsConnection_readJournalTimeRange(MmsConnection self, MmsError* mmsError, const char* domainId, const char* itemId,
        MmsValue* startTime, MmsValue* endTime, bool* moreFollows)
{
    struct readJournalParameters parameter;

    MmsError err = MMS_ERROR_NONE;

    parameter.waitForResponse = Semaphore_create(1);
    parameter.err = MMS_ERROR_NONE;
    parameter.entries = NULL;
    parameter.moreFollows = false;

    Semaphore_wait(parameter.waitForResponse);

    MmsConnection_readJournalTimeRangeAsync(self, NULL, &err, domainId, itemId, startTime, endTime, readJournalHandler, &parameter);

    if (err == MMS_ERROR_NONE)
    {
        Semaphore_wait(parameter.waitForResponse);

        err = parameter.err;

        if (moreFollows)
            *moreFollows = parameter.moreFollows;
    }

    Semaphore_destroy(parameter.waitForResponse);

    if (mmsError)
        *mmsError = err;

    return parameter.entries;
}

void
MmsConnection_readJournalTimeRangeAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError, const char* domainId, const char* itemId,
        MmsValue* startTime, MmsValue* endTime, MmsConnection_ReadJournalHandler handler, void* parameter)
{
    if (getConnectionState(self) != MMS_CONNECTION_STATE_CONNECTED)
    {
        if (mmsError)
            *mmsError = MMS_ERROR_CONNECTION_LOST;
        goto exit_function;
    }

    if ((MmsValue_getType(startTime) != MMS_BINARY_TIME) ||
            (MmsValue_getType(endTime) != MMS_BINARY_TIME))
    {
        if (mmsError)
            *mmsError = MMS_ERROR_INVALID_ARGUMENTS;
        goto exit_function;
    }

    ByteBuffer* payload = IsoClientConnection_allocateTransmitBuffer(self->isoClient);

    uint32_t invokeId = getNextInvokeId(self);

    if (usedInvokeId)
        *usedInvokeId = invokeId;

    mmsClient_createReadJournalRequestWithTimeRange(invokeId, payload, domainId, itemId, startTime, endTime);

    MmsClientInternalParameter intParam;
    intParam.ptr = NULL;

    MmsError err = sendAsyncRequest(self, invokeId, payload, MMS_CALL_TYPE_READ_JOURNAL, handler, parameter, intParam);

    if (mmsError)
        *mmsError = err;

exit_function:
    return;
}

LinkedList
MmsConnection_readJournalStartAfter(MmsConnection self, MmsError* mmsError, const char* domainId, const char* itemId,
        MmsValue* timeSpecification, MmsValue* entrySpecification, bool* moreFollows)
{
    struct readJournalParameters parameter;

    MmsError err = MMS_ERROR_NONE;

    parameter.waitForResponse = Semaphore_create(1);
    parameter.err = MMS_ERROR_NONE;
    parameter.entries = NULL;
    parameter.moreFollows = false;

    Semaphore_wait(parameter.waitForResponse);

    MmsConnection_readJournalStartAfterAsync(self, NULL, &err, domainId, itemId, timeSpecification, entrySpecification, readJournalHandler, &parameter);

    if (err == MMS_ERROR_NONE)
    {
        Semaphore_wait(parameter.waitForResponse);

        err = parameter.err;

        if (moreFollows)
            *moreFollows = parameter.moreFollows;
    }

    Semaphore_destroy(parameter.waitForResponse);

    if (mmsError)
        *mmsError = err;

    return parameter.entries;
}

void
MmsConnection_readJournalStartAfterAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError, const char* domainId, const char* itemId,
        MmsValue* timeSpecification, MmsValue* entrySpecification, MmsConnection_ReadJournalHandler handler, void* parameter)
{
    if (getConnectionState(self) != MMS_CONNECTION_STATE_CONNECTED)
    {
        if (mmsError)
            *mmsError = MMS_ERROR_CONNECTION_LOST;
        goto exit_function;
    }

    if ((MmsValue_getType(timeSpecification) != MMS_BINARY_TIME) ||
            (MmsValue_getType(entrySpecification) != MMS_OCTET_STRING))
    {
        if (mmsError)
            *mmsError = MMS_ERROR_INVALID_ARGUMENTS;
        goto exit_function;
    }

    ByteBuffer* payload = IsoClientConnection_allocateTransmitBuffer(self->isoClient);

    uint32_t invokeId = getNextInvokeId(self);

    if (usedInvokeId)
        *usedInvokeId = invokeId;

    mmsClient_createReadJournalRequestStartAfter(invokeId, payload, domainId, itemId, timeSpecification, entrySpecification);

    MmsClientInternalParameter intParam;
    intParam.ptr = NULL;

    MmsError err = sendAsyncRequest(self, invokeId, payload, MMS_CALL_TYPE_READ_JOURNAL, handler, parameter, intParam);

    if (mmsError)
        *mmsError = err;

exit_function:
    return;
}

struct fileOpenParameters
{
    Semaphore waitForResponse;
    MmsError err;
    int32_t frsmId;
    uint32_t fileSize;
    uint64_t lastModified;
};

static void
fileOpenHandler(uint32_t invokeId, void* parameter, MmsError mmsError, int32_t frsmId, uint32_t fileSize, uint64_t lastModified)
{
    (void)invokeId;

    struct fileOpenParameters* parameters = (struct fileOpenParameters*) parameter;

    parameters->err = mmsError;
    parameters->frsmId = frsmId;
    parameters->fileSize = fileSize;
    parameters->lastModified = lastModified;

    /* unblock user thread */
    Semaphore_post(parameters->waitForResponse);
}


int32_t
MmsConnection_fileOpen(MmsConnection self, MmsError* mmsError, const char* filename, uint32_t initialPosition,
        uint32_t* fileSize, uint64_t* lastModified)
{
#if (MMS_FILE_SERVICE == 1)

    struct fileOpenParameters parameter;

    MmsError err = MMS_ERROR_NONE;

    parameter.waitForResponse = Semaphore_create(1);
    parameter.err = MMS_ERROR_NONE;
    parameter.frsmId = 0;
    parameter.fileSize = 0;
    parameter.lastModified = 0;

    Semaphore_wait(parameter.waitForResponse);

    MmsConnection_fileOpenAsync(self, NULL, &err, filename, initialPosition, fileOpenHandler, &parameter);

    if (err == MMS_ERROR_NONE)
    {
        Semaphore_wait(parameter.waitForResponse);

        err = parameter.err;
    }

    Semaphore_destroy(parameter.waitForResponse);

    if (fileSize)
        *fileSize = parameter.fileSize;

    if (lastModified)
        *lastModified = parameter.lastModified;

    if (mmsError)
        *mmsError = err;

    return parameter.frsmId;
#else
    if (DEBUG_MMS_CLIENT)
        printf("MMS_CLIENT: service not supported\n");

    *mmsError = MMS_ERROR_OTHER;
    return 0;
#endif
}

void
MmsConnection_fileOpenAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError, const char* filename, uint32_t initialPosition, MmsConnection_FileOpenHandler handler,
        void* parameter)
{
#if (MMS_FILE_SERVICE == 1)

    if (getConnectionState(self) != MMS_CONNECTION_STATE_CONNECTED)
    {
        if (mmsError)
            *mmsError = MMS_ERROR_CONNECTION_LOST;
        goto exit_function;
    }

    ByteBuffer* payload = IsoClientConnection_allocateTransmitBuffer(self->isoClient);

    uint32_t invokeId = getNextInvokeId(self);

    if (usedInvokeId)
        *usedInvokeId = invokeId;

    mmsClient_createFileOpenRequest(invokeId, payload, filename, initialPosition);

    MmsClientInternalParameter intParam;
    intParam.ptr = NULL;

    MmsError err = sendAsyncRequest(self, invokeId, payload, MMS_CALL_TYPE_FILE_OPEN, handler, parameter, intParam);

    if (mmsError)
        *mmsError = err;

exit_function:
    return;
#else
    if (DEBUG_MMS_CLIENT)
        printf("MMS_CLIENT: service not supported\n");

    *mmsError = MMS_ERROR_OTHER;
    return;
#endif
}

struct fileOperationParameters
{
    Semaphore waitForResponse;
    MmsError err;
    bool success;
};

static void
fileOperationHandler(uint32_t invokeId, void* parameter, MmsError mmsError, bool success)
{
    (void)invokeId;

    struct fileOperationParameters* parameters = (struct fileOperationParameters*) parameter;

    parameters->err = mmsError;
    parameters->success = success;

    /* unblock user thread */
    Semaphore_post(parameters->waitForResponse);
}

void
MmsConnection_fileClose(MmsConnection self, MmsError* mmsError, int32_t frsmId)
{
#if (MMS_FILE_SERVICE == 1)

    struct fileOperationParameters parameter;

    MmsError err = MMS_ERROR_NONE;

    parameter.waitForResponse = Semaphore_create(1);
    parameter.err = MMS_ERROR_NONE;
    parameter.success = false;

    Semaphore_wait(parameter.waitForResponse);

    MmsConnection_fileCloseAsync(self, NULL, &err, frsmId, fileOperationHandler, &parameter);

    if (err == MMS_ERROR_NONE)
    {
        Semaphore_wait(parameter.waitForResponse);

        err = parameter.err;
    }

    Semaphore_destroy(parameter.waitForResponse);

    if (mmsError)
        *mmsError = err;

#else
    if (DEBUG_MMS_CLIENT)
        printf("MMS_CLIENT: service not supported\n");

    *mmsError = MMS_ERROR_OTHER;
#endif
}

void
MmsConnection_fileCloseAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError, uint32_t frsmId, MmsConnection_GenericServiceHandler handler, void* parameter)
{
#if (MMS_FILE_SERVICE == 1)

    if (getConnectionState(self) != MMS_CONNECTION_STATE_CONNECTED)
    {
        if (mmsError)
            *mmsError = MMS_ERROR_CONNECTION_LOST;
        goto exit_function;
    }

    ByteBuffer* payload = IsoClientConnection_allocateTransmitBuffer(self->isoClient);

    uint32_t invokeId = getNextInvokeId(self);

    if (usedInvokeId)
        *usedInvokeId = invokeId;

    mmsClient_createFileCloseRequest(invokeId, payload, frsmId);

    MmsClientInternalParameter intParam;
    intParam.ptr = NULL;

    MmsError err = sendAsyncRequest(self, invokeId, payload, MMS_CALL_TYPE_FILE_CLOSE, handler, parameter, intParam);

    if (mmsError)
        *mmsError = err;

exit_function:
    return;
#else
    if (DEBUG_MMS_CLIENT)
        printf("MMS_CLIENT: service not supported\n");

    *mmsError = MMS_ERROR_OTHER;
    return;
#endif
}

void
MmsConnection_fileDelete(MmsConnection self, MmsError* mmsError, const char* fileName)
{
#if (MMS_FILE_SERVICE == 1)

    struct fileOperationParameters parameter;

    MmsError err = MMS_ERROR_NONE;

    parameter.waitForResponse = Semaphore_create(1);
    parameter.err = MMS_ERROR_NONE;
    parameter.success = false;

    Semaphore_wait(parameter.waitForResponse);

    MmsConnection_fileDeleteAsync(self, NULL, &err, fileName, fileOperationHandler, &parameter);

    if (err == MMS_ERROR_NONE)
    {
        Semaphore_wait(parameter.waitForResponse);

        err = parameter.err;
    }

    Semaphore_destroy(parameter.waitForResponse);

    if (mmsError)
        *mmsError = err;

#else
    if (DEBUG_MMS_CLIENT)
        printf("MMS_CLIENT: service not supported\n");

    *mmsError = MMS_ERROR_OTHER;
#endif
}

void
MmsConnection_fileDeleteAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError, const char* fileName,
        MmsConnection_GenericServiceHandler handler, void* parameter)
{
#if (MMS_FILE_SERVICE == 1)

    if (getConnectionState(self) != MMS_CONNECTION_STATE_CONNECTED)
    {
        if (mmsError)
            *mmsError = MMS_ERROR_CONNECTION_LOST;
        goto exit_function;
    }

    ByteBuffer* payload = IsoClientConnection_allocateTransmitBuffer(self->isoClient);

    uint32_t invokeId = getNextInvokeId(self);

    if (usedInvokeId)
        *usedInvokeId = invokeId;

    mmsClient_createFileDeleteRequest(invokeId, payload, fileName);

    MmsClientInternalParameter intParam;
    intParam.ptr = NULL;

    MmsError err = sendAsyncRequest(self, invokeId, payload, MMS_CALL_TYPE_FILE_DELETE, handler, parameter, intParam);

    if (mmsError)
        *mmsError = err;

exit_function:
    return;
#else
    if (DEBUG_MMS_CLIENT)
        printf("MMS_CLIENT: service not supported\n");

    *mmsError = MMS_ERROR_OTHER;
    return;
#endif
}

struct fileReadParameters
{
    Semaphore waitForResponse;
    MmsError err;
    MmsFileReadHandler handler;
    void* handlerParameter;
    bool moreFollows;
};

static void
fileReadHandler(uint32_t invokeId, void* parameter, MmsError mmsError, int frsmId, uint8_t* buffer, uint32_t byteReceived,
        bool moreFollows)
{
    (void)invokeId;

    struct fileReadParameters* parameters = (struct fileReadParameters*) parameter;

    parameters->err = mmsError;

    if (mmsError == MMS_ERROR_NONE)
        parameters->handler(parameters->handlerParameter, frsmId, buffer, byteReceived);

    parameters->moreFollows = moreFollows;

    /* unblock user thread */
    Semaphore_post(parameters->waitForResponse);
}

bool
MmsConnection_fileRead(MmsConnection self, MmsError* mmsError, int32_t frsmId, MmsFileReadHandler handler,
        void* handlerParameter)
{
#if (MMS_FILE_SERVICE == 1)

    struct fileReadParameters parameter;

    MmsError err = MMS_ERROR_NONE;

    parameter.waitForResponse = Semaphore_create(1);
    parameter.err = MMS_ERROR_NONE;
    parameter.handler = handler;
    parameter.handlerParameter = handlerParameter;
    parameter.moreFollows = false;

    Semaphore_wait(parameter.waitForResponse);

    MmsConnection_fileReadAsync(self, NULL,  &err, frsmId, fileReadHandler, &parameter);

    if (err == MMS_ERROR_NONE)
    {
        Semaphore_wait(parameter.waitForResponse);

        err = parameter.err;
    }

    Semaphore_destroy(parameter.waitForResponse);

    if (mmsError)
        *mmsError = err;

    return parameter.moreFollows;

#else
    if (DEBUG_MMS_CLIENT)
        printf("MMS_CLIENT: service not supported\n");

    *mmsError = MMS_ERROR_OTHER;
    return false;
#endif
}

void
MmsConnection_fileReadAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError, int32_t frsmId, MmsConnection_FileReadHandler handler, void* parameter)
{
#if (MMS_FILE_SERVICE == 1)

    if (getConnectionState(self) != MMS_CONNECTION_STATE_CONNECTED) {
        if (mmsError)
            *mmsError = MMS_ERROR_CONNECTION_LOST;
        goto exit_function;
    }

    ByteBuffer* payload = IsoClientConnection_allocateTransmitBuffer(self->isoClient);

    uint32_t invokeId = getNextInvokeId(self);

    if (usedInvokeId)
        *usedInvokeId = invokeId;

    mmsClient_createFileReadRequest(invokeId, payload, frsmId);

    MmsClientInternalParameter intParam;
    intParam.i32 = frsmId;

    MmsError err = sendAsyncRequest(self, invokeId, payload, MMS_CALL_TYPE_FILE_READ, handler, parameter, intParam);

    if (mmsError)
        *mmsError = err;

exit_function:
    return;
#else
    if (DEBUG_MMS_CLIENT)
        printf("MMS_CLIENT: service not supported\n");

    *mmsError = MMS_ERROR_OTHER;
    return;
#endif
}

struct getFileDirParameters
{
    Semaphore waitForResponse;
    MmsError err;
    bool moreFollows;
    MmsFileDirectoryHandler handler;
    void* handlerParameter;
};

static void
getFileDirHandler(uint32_t invokeId, void* parameter, MmsError mmsError, char* filename, uint32_t size, uint64_t lastModified,
        bool moreFollows)
{
    (void)invokeId;

    struct getFileDirParameters* parameters = (struct getFileDirParameters*) parameter;

    parameters->err = mmsError;

    if ((mmsError != MMS_ERROR_NONE) || (filename == NULL))
    {
        parameters->moreFollows = moreFollows;

        /* last call --> unblock user thread */
        Semaphore_post(parameters->waitForResponse);
    }
    else {
        parameters->handler(parameters->handlerParameter, filename, size, lastModified);
    }
}

bool
MmsConnection_getFileDirectory(MmsConnection self, MmsError* mmsError, const char* fileSpecification, const char* continueAfter,
        MmsFileDirectoryHandler handler, void* handlerParameter)
{
#if (MMS_FILE_SERVICE == 1)

    bool moreFollows = false;

    struct getFileDirParameters parameter;
    parameter.handler = handler;
    parameter.handlerParameter = handlerParameter;

    MmsError err;

    parameter.waitForResponse = Semaphore_create(1);

    Semaphore_wait(parameter.waitForResponse);

    MmsConnection_getFileDirectoryAsync(self, NULL, &err, fileSpecification, continueAfter, getFileDirHandler, &parameter);

    if (err == MMS_ERROR_NONE)
    {
        Semaphore_wait(parameter.waitForResponse);
        err = parameter.err;
        moreFollows = parameter.moreFollows;
    }

    Semaphore_destroy(parameter.waitForResponse);

    if (mmsError)
        *mmsError = err;

    return moreFollows;
#else
    if (DEBUG_MMS_CLIENT)
        printf("MMS_CLIENT: service not supported\n");

    *mmsError = MMS_ERROR_OTHER;
    return false;
#endif
}

void
MmsConnection_getFileDirectoryAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError, const char* fileSpecification, const char* continueAfter,
        MmsConnection_FileDirectoryHandler handler, void* parameter)
{
#if (MMS_FILE_SERVICE == 1)

    if (getConnectionState(self) != MMS_CONNECTION_STATE_CONNECTED)
    {
        if (mmsError)
            *mmsError = MMS_ERROR_CONNECTION_LOST;
        goto exit_function;
    }

    ByteBuffer* payload = IsoClientConnection_allocateTransmitBuffer(self->isoClient);

    uint32_t invokeId = getNextInvokeId(self);

    if (usedInvokeId)
        *usedInvokeId = invokeId;

    mmsClient_createFileDirectoryRequest(invokeId, payload, fileSpecification, continueAfter);

    MmsClientInternalParameter intParam;
    intParam.ptr = NULL;

    MmsError err = sendAsyncRequest(self, invokeId, payload, MMS_CALL_TYPE_GET_FILE_DIR, handler, parameter, intParam);

    if (mmsError)
        *mmsError = err;

exit_function:
    return;
#else
    if (DEBUG_MMS_CLIENT)
        printf("MMS_CLIENT: service not supported\n");

    *mmsError = MMS_ERROR_OTHER;
    return;
#endif
}

void
MmsConnection_fileRename(MmsConnection self, MmsError* mmsError, const char* currentFileName, const char* newFileName)
{
#if (MMS_FILE_SERVICE == 1)

    struct fileOperationParameters parameter;

    MmsError err = MMS_ERROR_NONE;

    parameter.waitForResponse = Semaphore_create(1);
    parameter.err = MMS_ERROR_NONE;
    parameter.success = false;

    Semaphore_wait(parameter.waitForResponse);

    MmsConnection_fileRenameAsync(self, NULL, &err, currentFileName, newFileName, fileOperationHandler, &parameter);

    if (err == MMS_ERROR_NONE)
    {
        Semaphore_wait(parameter.waitForResponse);

        err = parameter.err;
    }

    Semaphore_destroy(parameter.waitForResponse);

    if (mmsError)
        *mmsError = err;
#else
    if (DEBUG_MMS_CLIENT)
        printf("MMS_CLIENT: service not supported\n");

    *mmsError = MMS_ERROR_OTHER;
#endif
}

void
MmsConnection_fileRenameAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError, const char* currentFileName, const char* newFileName,
        MmsConnection_GenericServiceHandler handler, void* parameter)
{
#if (MMS_FILE_SERVICE == 1)

    if (getConnectionState(self) != MMS_CONNECTION_STATE_CONNECTED)
    {
        if (mmsError)
            *mmsError = MMS_ERROR_CONNECTION_LOST;
        goto exit_function;
    }

    ByteBuffer* payload = IsoClientConnection_allocateTransmitBuffer(self->isoClient);

    uint32_t invokeId = getNextInvokeId(self);

    if (usedInvokeId)
        *usedInvokeId = invokeId;

    mmsClient_createFileRenameRequest(invokeId, payload, currentFileName, newFileName);

    MmsClientInternalParameter intParam;
    intParam.ptr = NULL;

    MmsError err = sendAsyncRequest(self, invokeId, payload, MMS_CALL_TYPE_FILE_RENAME, handler, parameter, intParam);

    if (mmsError)
        *mmsError = err;

exit_function:
    return;
#else
    if (DEBUG_MMS_CLIENT)
        printf("MMS_CLIENT: service not supported\n");

    *mmsError = MMS_ERROR_OTHER;
    return;
#endif
}

void
MmsConnection_obtainFile(MmsConnection self, MmsError* mmsError, const char* sourceFile, const char* destinationFile)
{
#if ((MMS_FILE_SERVICE == 1) && (MMS_OBTAIN_FILE_SERVICE == 1))

    struct fileOperationParameters parameter;

    MmsError err = MMS_ERROR_NONE;

    parameter.waitForResponse = Semaphore_create(1);
    parameter.err = MMS_ERROR_NONE;
    parameter.success = false;

    Semaphore_wait(parameter.waitForResponse);

    MmsConnection_obtainFileAsync(self, NULL, &err, sourceFile, destinationFile, fileOperationHandler, &parameter);

    if (err == MMS_ERROR_NONE)
    {
        Semaphore_wait(parameter.waitForResponse);

        err = parameter.err;
    }

    Semaphore_destroy(parameter.waitForResponse);

    if (mmsError)
        *mmsError = err;

#else
    if (DEBUG_MMS_CLIENT)
        printf("MMS_CLIENT: service not supported\n");

    *mmsError = MMS_ERROR_OTHER;
#endif
}

struct writeVariableParameters
{
    Semaphore waitForResponse;
    MmsError err;
    MmsDataAccessError accessError;
};

void
MmsConnection_obtainFileAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError, const char* sourceFile, const char* destinationFile,
        MmsConnection_GenericServiceHandler handler, void* parameter)
{
#if (MMS_FILE_SERVICE == 1)

    if (getConnectionState(self) != MMS_CONNECTION_STATE_CONNECTED)
    {
        if (mmsError)
            *mmsError = MMS_ERROR_CONNECTION_LOST;
        goto exit_function;
    }

    ByteBuffer* payload = IsoClientConnection_allocateTransmitBuffer(self->isoClient);

    uint32_t invokeId = getNextInvokeId(self);

    if (usedInvokeId)
        *usedInvokeId = invokeId;

    mmsClient_createObtainFileRequest(invokeId, payload, sourceFile, destinationFile);

    MmsClientInternalParameter intParam;

    if (sourceFile)
        intParam.ptr = StringUtils_copyString(sourceFile);
    else
        intParam.ptr = NULL;

    MmsError err = sendAsyncRequest(self, invokeId, payload, MMS_CALL_TYPE_OBTAIN_FILE, handler, parameter, intParam);

    if (mmsError)
        *mmsError = err;

exit_function:
    return;
#else
    if (DEBUG_MMS_CLIENT)
        printf("MMS_CLIENT: service not supported\n");

    *mmsError = MMS_ERROR_OTHER;
    return;
#endif
}

static void
writeVariableHandler(uint32_t invokeId, void* parameter, MmsError mmsError, MmsDataAccessError accessError)
{
    (void)invokeId;

    struct writeVariableParameters* parameters = (struct writeVariableParameters*) parameter;

    parameters->err = mmsError;
    parameters->accessError = accessError;

    /* unblock user thread */
    Semaphore_post(parameters->waitForResponse);
}

MmsDataAccessError
MmsConnection_writeVariable(MmsConnection self, MmsError* mmsError,
        const char* domainId, const char* itemId,
        MmsValue* value)
{
    struct writeVariableParameters parameter;

    MmsError err = MMS_ERROR_NONE;

    parameter.waitForResponse = Semaphore_create(1);
    parameter.err = MMS_ERROR_NONE;
    parameter.accessError = DATA_ACCESS_ERROR_SUCCESS;

    Semaphore_wait(parameter.waitForResponse);

    MmsConnection_writeVariableAsync(self, NULL, &err, domainId, itemId, value, writeVariableHandler, &parameter);

    if (err == MMS_ERROR_NONE)
    {
        Semaphore_wait(parameter.waitForResponse);

        err = parameter.err;
    }

    Semaphore_destroy(parameter.waitForResponse);

    if (mmsError)
        *mmsError = err;

    return parameter.accessError;
}

void
MmsConnection_writeVariableAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError,
        const char* domainId, const char* itemId, MmsValue* value,
        MmsConnection_WriteVariableHandler handler, void* parameter)
{
    if (getConnectionState(self) != MMS_CONNECTION_STATE_CONNECTED)
    {
        if (mmsError)
            *mmsError = MMS_ERROR_CONNECTION_LOST;
        goto exit_function;
    }

    ByteBuffer* payload = IsoClientConnection_allocateTransmitBuffer(self->isoClient);

    uint32_t invokeId = getNextInvokeId(self);

    if (usedInvokeId)
        *usedInvokeId = invokeId;

    mmsClient_createWriteRequest(invokeId, domainId, itemId, value, payload);

    MmsClientInternalParameter intParam;
    intParam.ptr = NULL;

    MmsError err = sendAsyncRequest(self, invokeId, payload, MMS_CALL_TYPE_WRITE_VARIABLE, handler, parameter, intParam);

    if (mmsError)
        *mmsError = err;

exit_function:
    return;
}

MmsDataAccessError
MmsConnection_writeSingleArrayElementWithComponent(MmsConnection self, MmsError* mmsError,
        const char* domainId, const char* itemId,
        uint32_t arrayIndex, const char* componentId, MmsValue* value)
{
    struct writeVariableParameters parameter;

    MmsError err = MMS_ERROR_NONE;

    parameter.waitForResponse = Semaphore_create(1);
    parameter.err = MMS_ERROR_NONE;
    parameter.accessError = DATA_ACCESS_ERROR_SUCCESS;

    Semaphore_wait(parameter.waitForResponse);

    MmsConnection_writeSingleArrayElementWithComponentAsync(self, NULL, &err, domainId, itemId, arrayIndex, componentId, value, writeVariableHandler, &parameter);

    if (err == MMS_ERROR_NONE)
    {
        Semaphore_wait(parameter.waitForResponse);

        err = parameter.err;
    }

    Semaphore_destroy(parameter.waitForResponse);

    if (mmsError)
        *mmsError = err;

    return parameter.accessError;
}

void
MmsConnection_writeSingleArrayElementWithComponentAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError,
        const char* domainId, const char* itemId,
        uint32_t arrayIndex, const char* componentId, MmsValue* value,
        MmsConnection_WriteVariableHandler handler, void* parameter)
{
    if (getConnectionState(self) != MMS_CONNECTION_STATE_CONNECTED)
    {
        if (mmsError)
            *mmsError = MMS_ERROR_CONNECTION_LOST;
        goto exit_function;
    }

    ByteBuffer* payload = IsoClientConnection_allocateTransmitBuffer(self->isoClient);

    uint32_t invokeId = getNextInvokeId(self);

    if (usedInvokeId)
        *usedInvokeId = invokeId;

    mmsClient_createWriteRequestAlternateAccessSingleIndexComponent(invokeId, domainId, itemId, arrayIndex,
            componentId, value, payload);

    MmsClientInternalParameter intParam;
    intParam.ptr = NULL;

    MmsError err = sendAsyncRequest(self, invokeId, payload, MMS_CALL_TYPE_WRITE_VARIABLE, handler, parameter, intParam);

    if (mmsError)
        *mmsError = err;

exit_function:
    return;
}

MmsDataAccessError
MmsConnection_writeVariableComponent(MmsConnection self, MmsError* mmsError,
        const char* domainId, const char* itemId,
        const char* componentId, MmsValue* value)
{
    struct writeVariableParameters parameter;

    MmsError err = MMS_ERROR_NONE;

    parameter.waitForResponse = Semaphore_create(1);
    parameter.err = MMS_ERROR_NONE;
    parameter.accessError = DATA_ACCESS_ERROR_SUCCESS;

    Semaphore_wait(parameter.waitForResponse);

    MmsConnection_writeVariableComponentAsync(self, NULL, &err, domainId, itemId, componentId, value, writeVariableHandler, &parameter);

    if (err == MMS_ERROR_NONE)
    {
        Semaphore_wait(parameter.waitForResponse);

        err = parameter.err;
    }

    Semaphore_destroy(parameter.waitForResponse);

    if (mmsError)
        *mmsError = err;

    return parameter.accessError;
}

void
MmsConnection_writeVariableComponentAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError,
        const char* domainId, const char* itemId, const char* componentId, MmsValue* value,
        MmsConnection_WriteVariableHandler handler, void* parameter)
{
    if (getConnectionState(self) != MMS_CONNECTION_STATE_CONNECTED)
    {
        if (mmsError)
            *mmsError = MMS_ERROR_CONNECTION_LOST;
        goto exit_function;
    }

    ByteBuffer* payload = IsoClientConnection_allocateTransmitBuffer(self->isoClient);

    uint32_t invokeId = getNextInvokeId(self);

    if (usedInvokeId)
        *usedInvokeId = invokeId;

    mmsClient_createWriteRequestComponent(invokeId, domainId, itemId, componentId, value, payload);

    MmsClientInternalParameter intParam;
    intParam.ptr = NULL;

    MmsError err = sendAsyncRequest(self, invokeId, payload, MMS_CALL_TYPE_WRITE_VARIABLE, handler, parameter, intParam);

    if (mmsError)
        *mmsError = err;

exit_function:
    return;
}

struct writeMultipleVariablesParameter
{
    Semaphore sem;
    MmsError err;
    LinkedList result;
};

static void
writeMultipleVariablesHandler(uint32_t invokeId, void* parameter, MmsError mmsError, LinkedList /* <MmsValue*> */ accessResults)
{
    (void)invokeId;

    struct writeMultipleVariablesParameter* parameters = (struct writeMultipleVariablesParameter*) parameter;

    parameters->err = mmsError;
    parameters->result = accessResults;

    /* unblock user thread */
    Semaphore_post(parameters->sem);
}

void
MmsConnection_writeMultipleVariables(MmsConnection self, MmsError* mmsError, const char* domainId,
        LinkedList /*<char*>*/items,
        LinkedList /* <MmsValue*> */values,
        /* OUTPUT */LinkedList* /* <MmsValue*> */accessResults)
{
    MmsError err = MMS_ERROR_NONE;

    struct writeMultipleVariablesParameter parameter;

    parameter.sem = Semaphore_create(1);;
    parameter.err = MMS_ERROR_NONE;
    parameter.result = NULL;

    Semaphore_wait(parameter.sem);

    MmsConnection_writeMultipleVariablesAsync(self, NULL, &err, domainId, items, values, writeMultipleVariablesHandler, &parameter);

    if (err == MMS_ERROR_NONE)
    {
        Semaphore_wait(parameter.sem);

        err = parameter.err;

        if (accessResults)
            *accessResults = parameter.result;
        else
            LinkedList_destroyDeep(parameter.result, (LinkedListValueDeleteFunction) MmsValue_delete);
    }
    else {
        if (accessResults)
            *accessResults = NULL;
    }

    if (mmsError)
        *mmsError = err;

    Semaphore_destroy(parameter.sem);
}

void
MmsConnection_writeMultipleVariablesAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError, const char* domainId,
        LinkedList /*<char*>*/ items, LinkedList /* <MmsValue*> */ values,
        MmsConnection_WriteMultipleVariablesHandler handler, void* parameter)
{
    if (getConnectionState(self) != MMS_CONNECTION_STATE_CONNECTED)
    {
        if (mmsError)
            *mmsError = MMS_ERROR_CONNECTION_LOST;
        goto exit_function;
    }

    ByteBuffer* payload = IsoClientConnection_allocateTransmitBuffer(self->isoClient);

    uint32_t invokeId = getNextInvokeId(self);

    if (usedInvokeId)
        *usedInvokeId = invokeId;

    if (mmsClient_createWriteMultipleItemsRequest(invokeId, domainId, items, values, payload) != -1)
    {
        MmsClientInternalParameter intParam;
        intParam.ptr = NULL;

        MmsError err = sendAsyncRequest(self, invokeId, payload, MMS_CALL_TYPE_WRITE_MULTIPLE_VARIABLES, handler, parameter, intParam);

        if (mmsError)
            *mmsError = err;
    }
    else
    {
        *mmsError = MMS_ERROR_RESOURCE_OTHER;
    }


exit_function:
    return;
}

MmsDataAccessError
MmsConnection_writeArrayElements(MmsConnection self, MmsError* mmsError,
        const char* domainId, const char* itemId, int index, int numberOfElements,
        MmsValue* value)
{
    struct writeVariableParameters parameter;

    MmsError err = MMS_ERROR_NONE;

    parameter.waitForResponse = Semaphore_create(1);
    parameter.err = MMS_ERROR_NONE;
    parameter.accessError = DATA_ACCESS_ERROR_SUCCESS;

    Semaphore_wait(parameter.waitForResponse);

    MmsConnection_writeArrayElementsAsync(self, NULL, &err, domainId, itemId, index, numberOfElements, value, writeVariableHandler, &parameter);

    if (err == MMS_ERROR_NONE)
    {
        Semaphore_wait(parameter.waitForResponse);

        err = parameter.err;
    }

    Semaphore_destroy(parameter.waitForResponse);

    if (mmsError)
        *mmsError = err;

    return parameter.accessError;
}

void
MmsConnection_writeArrayElementsAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError,
        const char* domainId, const char* itemId, int index, int numberOfElements,
        MmsValue* value,
        MmsConnection_WriteVariableHandler handler, void* parameter)
{
    if (getConnectionState(self) != MMS_CONNECTION_STATE_CONNECTED)
    {
        if (mmsError)
            *mmsError = MMS_ERROR_CONNECTION_LOST;
        goto exit_function;
    }

    ByteBuffer* payload = IsoClientConnection_allocateTransmitBuffer(self->isoClient);

    uint32_t invokeId = getNextInvokeId(self);

    if (usedInvokeId)
        *usedInvokeId = invokeId;

    mmsClient_createWriteRequestArray(invokeId, domainId, itemId, index, numberOfElements, value, payload);

    MmsClientInternalParameter intParam;
    intParam.ptr = NULL;

    MmsError err = sendAsyncRequest(self, invokeId, payload, MMS_CALL_TYPE_WRITE_VARIABLE, handler, parameter, intParam);

    if (mmsError)
        *mmsError = err;

exit_function:
    return;
}

void
MmsConnection_writeNamedVariableList(MmsConnection self, MmsError* mmsError, bool isAssociationSpecific,
        const char* domainId, const char* itemId, LinkedList /* <MmsValue*> */values,
        /* OUTPUT */LinkedList* /* <MmsValue*> */accessResults)
{
    MmsError err = MMS_ERROR_NONE;

    struct writeMultipleVariablesParameter parameter;

    parameter.sem = Semaphore_create(1);;
    parameter.err = MMS_ERROR_NONE;
    parameter.result = NULL;

    Semaphore_wait(parameter.sem);

    MmsConnection_writeNamedVariableListAsync(self, NULL, &err, isAssociationSpecific, domainId, itemId, values, writeMultipleVariablesHandler, &parameter);

    if (err == MMS_ERROR_NONE)
    {
        Semaphore_wait(parameter.sem);

        err = parameter.err;

        if (accessResults)
            *accessResults = parameter.result;
        else
            LinkedList_destroyDeep(parameter.result, (LinkedListValueDeleteFunction) MmsValue_delete);
    }
    else
    {
        if (accessResults)
            *accessResults = NULL;
    }

    if (mmsError)
        *mmsError = err;

    Semaphore_destroy(parameter.sem);
}

void
MmsConnection_writeNamedVariableListAsync(MmsConnection self, uint32_t* usedInvokeId, MmsError* mmsError, bool isAssociationSpecific,
        const char* domainId, const char* itemId, LinkedList /* <MmsValue*> */values,
        MmsConnection_WriteMultipleVariablesHandler handler, void* parameter)
{
    if (getConnectionState(self) != MMS_CONNECTION_STATE_CONNECTED)
    {
        if (mmsError)
            *mmsError = MMS_ERROR_CONNECTION_LOST;
        goto exit_function;
    }

    ByteBuffer* payload = IsoClientConnection_allocateTransmitBuffer(self->isoClient);

    uint32_t invokeId = getNextInvokeId(self);

    if (usedInvokeId)
        *usedInvokeId = invokeId;

    mmsClient_createWriteRequestNamedVariableList(invokeId, isAssociationSpecific, domainId, itemId, values, payload);

    MmsClientInternalParameter intParam;
    intParam.ptr = NULL;

    MmsError err = sendAsyncRequest(self, invokeId, payload, MMS_CALL_TYPE_WRITE_MULTIPLE_VARIABLES, handler, parameter, intParam);

    if (mmsError)
        *mmsError = err;

exit_function:
    return;
}

void
MmsConnection_sendRawData(MmsConnection self, MmsError* mmsError, uint8_t* buffer, int bufSize)
{
    if (getConnectionState(self) != MMS_CONNECTION_STATE_CONNECTED)
    {
        if (mmsError)
            *mmsError = MMS_ERROR_CONNECTION_LOST;
        goto exit_function;
    }

    ByteBuffer* payload = IsoClientConnection_allocateTransmitBuffer(self->isoClient);

    ByteBuffer_append(payload, buffer, bufSize);

    sendMessage(self, payload);

    if (mmsError)
        *mmsError = MMS_ERROR_NONE;

exit_function:
    return;
}

void
MmsServerIdentity_destroy(MmsServerIdentity* self)
{
    if (self)
    {
        if (self->modelName != NULL)
            GLOBAL_FREEMEM(self->modelName);

        if (self->vendorName != NULL)
            GLOBAL_FREEMEM(self->vendorName);

        if (self->revision != NULL)
            GLOBAL_FREEMEM(self->revision);

        GLOBAL_FREEMEM(self);
    }
}

MmsVariableAccessSpecification*
MmsVariableAccessSpecification_create(char* domainId, char* itemId)
{
    MmsVariableAccessSpecification* self = (MmsVariableAccessSpecification*)
            GLOBAL_MALLOC(sizeof(MmsVariableAccessSpecification));

    if (self)
    {
        self->domainId = domainId;
        self->itemId = itemId;
        self->arrayIndex = -1;
        self->componentName = NULL;
    }

    return self;
}

MmsVariableAccessSpecification*
MmsVariableAccessSpecification_createAlternateAccess(char* domainId, char* itemId, int32_t index,
        char* componentName)
{
    MmsVariableAccessSpecification* self = (MmsVariableAccessSpecification*)
            GLOBAL_MALLOC(sizeof(MmsVariableAccessSpecification));

    if (self)
    {
        self->domainId = domainId;
        self->itemId = itemId;
        self->arrayIndex = index;
        self->componentName = componentName;
    }

    return self;
}

void
MmsVariableAccessSpecification_destroy(MmsVariableAccessSpecification* self)
{
    if (self)
    {
        if (self->domainId != NULL)
            GLOBAL_FREEMEM((void*) self->domainId);

        if (self->itemId != NULL)
            GLOBAL_FREEMEM((void*) self->itemId);

        if (self->componentName != NULL)
            GLOBAL_FREEMEM((void*) self->componentName);

        GLOBAL_FREEMEM(self);
    }
}
