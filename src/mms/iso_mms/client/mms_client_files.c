/*
 *  mms_client_files.c
 *
 *  Copyright 2013 - 2016 Michael Zillgith
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
#include "stack_config.h"
#include "mms_common.h"
#include "mms_client_connection.h"
#include "byte_buffer.h"

#include "mms_client_internal.h"
#include "ber_encoder.h"
#include "ber_decode.h"
#include "ber_integer.h"
#include "conversions.h"

#if (MMS_OBTAIN_FILE_SERVICE == 1)

static MmsFileReadStateMachine*
getFreeFrsm(MmsConnection connection)
{
    int i;

    MmsFileReadStateMachine* freeFrsm = NULL;

    for (i = 0; i < CONFIG_MMS_MAX_NUMBER_OF_OPEN_FILES_PER_CONNECTION; i++) {
        if (connection->frsms[i].fileHandle == NULL) {
            freeFrsm = &(connection->frsms[i]);
            break;
        }
    }

    return freeFrsm;
}

static MmsFileReadStateMachine*
getFrsm(MmsConnection connection, int32_t frsmId)
{
    int i;

    MmsFileReadStateMachine* frsm = NULL;

    for (i = 0; i < CONFIG_MMS_MAX_NUMBER_OF_OPEN_FILES_PER_CONNECTION; i++) {
        if (connection->frsms[i].fileHandle != NULL) {
            if (connection->frsms[i].frsmId == frsmId) {
                frsm = &(connection->frsms[i]);
                break;
            }
        }
    }

    return frsm;
}


static int32_t
getNextFrsmId(MmsConnection connection)
{
    uint32_t nextFrsmId = connection->nextFrsmId;
    connection->nextFrsmId++;

    return nextFrsmId;
}

void
mmsClient_handleFileOpenRequest(
    MmsConnection connection,
    uint8_t* buffer, int bufPos, int maxBufPos,
    uint32_t invokeId, ByteBuffer* response)
{
    char filename[256];
    bool hasFileName = false;
    uint32_t filePosition = 0;

    while (bufPos < maxBufPos) {
        uint8_t tag = buffer[bufPos++];
        int length;

        bufPos = BerDecoder_decodeLength(buffer, &length, bufPos, maxBufPos);

        if (bufPos < 0) goto exit_reject_invalid_pdu;

        switch(tag) {
        case 0xa0: /* filename */

            if (!mmsMsg_parseFileName(filename, buffer, &bufPos, bufPos + length, invokeId, response))
                return;

            hasFileName = true;

            break;

        case 0x81: /* initial position */
            filePosition = BerDecoder_decodeUint32(buffer, length, bufPos);
            bufPos += length;
            break;

        case 0x00: /* indefinite length end tag -> ignore */
            break;

        default: /* unrecognized parameter */
            bufPos += length;
            goto exit_reject_invalid_pdu;
        }
    }

    if (hasFileName) {

        MmsFileReadStateMachine* frsm = getFreeFrsm(connection);

        if (frsm != NULL) {

            MmsOutstandingCall obtainFileCall = mmsClient_getMatchingObtainFileRequest(connection, filename);

            if (obtainFileCall) {

                if (DEBUG_MMS_CLIENT)
                    printf("MMS_CLIENT: file open is matching obtain file request for file %s\n", filename);

                obtainFileCall->timeout = Hal_getTimeInMs() + connection->requestTimeout;
            }

            FileHandle fileHandle = mmsMsg_openFile(MmsConnection_getFilestoreBasepath(connection), filename, false);

            if (fileHandle != NULL) {

                frsm->fileHandle = fileHandle;
                frsm->readPosition = filePosition;
                frsm->frsmId = getNextFrsmId(connection);
                frsm->obtainRequest = obtainFileCall;

                mmsMsg_createFileOpenResponse(MmsConnection_getFilestoreBasepath(connection),
                        invokeId, response, filename, frsm);
            }
            else
                mmsMsg_createServiceErrorPdu(invokeId, response, MMS_ERROR_FILE_FILE_NON_EXISTENT);

        }
        else
            mmsMsg_createServiceErrorPdu(invokeId, response, MMS_ERROR_RESOURCE_OTHER);
    }
    else
        goto exit_invalid_parameter;

    return;

exit_invalid_parameter:
    mmsMsg_createMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_REQUEST_INVALID_ARGUMENT, response);
    return;

exit_reject_invalid_pdu:
    mmsMsg_createMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_INVALID_PDU, response);
}


void
mmsClient_handleFileReadRequest(
    MmsConnection connection,
    uint8_t* buffer, int bufPos, int maxBufPos,
    uint32_t invokeId,
    ByteBuffer* response)
{
    int32_t frsmId = BerDecoder_decodeInt32(buffer, maxBufPos - bufPos, bufPos);

    if (DEBUG_MMS_CLIENT)
        printf("MMS_CLIENT: mmsClient_handleFileReadRequest read request for frsmId: %i\n", frsmId);

    MmsFileReadStateMachine* frsm = getFrsm(connection, frsmId);

    if (frsm) {
        if (frsm->obtainRequest)
            frsm->obtainRequest->timeout = Hal_getTimeInMs() + connection->requestTimeout;

        mmsMsg_createFileReadResponse(connection->parameters.maxPduSize, invokeId, response, frsm);
    }
    else
        mmsMsg_createServiceErrorPdu(invokeId, response, MMS_ERROR_FILE_OTHER);
}

void
mmsClient_handleFileCloseRequest(
    MmsConnection connection,
    uint8_t* buffer, int bufPos, int maxBufPos,
    uint32_t invokeId,
    ByteBuffer* response)
{
    int32_t frsmId = BerDecoder_decodeInt32(buffer, maxBufPos - bufPos, bufPos);

    MmsFileReadStateMachine* frsm = getFrsm(connection, frsmId);

    if (frsm) {
        if (frsm->obtainRequest)
            frsm->obtainRequest->timeout = Hal_getTimeInMs() + connection->requestTimeout;

        if(frsm->fileHandle){
            FileSystem_closeFile(frsm->fileHandle);
            frsm->fileHandle = NULL;
        }
        frsm->frsmId = 0;
        frsm->obtainRequest = NULL;

        mmsMsg_createFileCloseResponse(invokeId, response);
    }
    else
        mmsMsg_createServiceErrorPdu(invokeId, response, MMS_ERROR_FILE_OTHER);
}

void
mmsClient_closeOutstandingOpenFiles(MmsConnection connection)
{
    int i;

    for (i = 0; i < CONFIG_MMS_MAX_NUMBER_OF_OPEN_FILES_PER_CONNECTION; i++) {
        if (connection->frsms[i].fileHandle != NULL) {
            FileSystem_closeFile(connection->frsms[i].fileHandle);
            connection->frsms[i].fileHandle = NULL;
        }
    }
}


#endif /* (MMS_OBTAIN_FILE_SERVICE == 1) */

#if (MMS_FILE_SERVICE == 1)

void
mmsClient_createFileOpenRequest(uint32_t invokeId, ByteBuffer* request, const char* fileName, uint32_t initialPosition)
{
    uint32_t invokeIdSize = BerEncoder_UInt32determineEncodedSize(invokeId);

    uint32_t fileNameStringSize = strlen(fileName);
    uint32_t fileNameSize = 1 + BerEncoder_determineLengthSize(fileNameStringSize) + fileNameStringSize;

    uint32_t fileNameSeqSize = fileNameSize;

    uint32_t fileOpenRequestSize = 1 + BerEncoder_determineLengthSize(fileNameSeqSize) + fileNameSeqSize + 2 + BerEncoder_UInt32determineEncodedSize(initialPosition);

    uint32_t confirmedRequestPduSize = 2  + invokeIdSize + 2 + BerEncoder_determineLengthSize(fileOpenRequestSize) + fileOpenRequestSize;

    int bufPos = 0;
    uint8_t* buffer = request->buffer;

    bufPos = BerEncoder_encodeTL(0xa0, confirmedRequestPduSize, buffer, bufPos);
    bufPos = BerEncoder_encodeTL(0x02, invokeIdSize, buffer, bufPos);
    bufPos = BerEncoder_encodeUInt32(invokeId, buffer, bufPos);

    /* Encode FileOpen tag (context | structured ) [72 = 48h] */
    buffer[bufPos++] = 0xbf;
    buffer[bufPos++] = 0x48;
    bufPos = BerEncoder_encodeLength(fileOpenRequestSize, buffer, bufPos);

    bufPos = BerEncoder_encodeTL(0xa0, fileNameSeqSize, buffer, bufPos);
    bufPos = BerEncoder_encodeOctetString(0x19, (uint8_t*) fileName, fileNameStringSize, buffer, bufPos);

    bufPos = BerEncoder_encodeUInt32WithTL(0x81, initialPosition, buffer, bufPos);

    request->size = bufPos;
}

void
mmsClient_createFileDeleteRequest(uint32_t invokeId, ByteBuffer* request, const char* fileName)
{
    uint32_t invokeIdSize = BerEncoder_UInt32determineEncodedSize(invokeId);

    uint32_t fileNameStringSize = strlen(fileName);
    uint32_t fileNameSize = 1 + BerEncoder_determineLengthSize(fileNameStringSize) + fileNameStringSize;

    uint32_t fileDeleteRequestSize = fileNameSize;

    uint32_t confirmedRequestPduSize = 1 + 2 + invokeIdSize + 1 + BerEncoder_determineLengthSize(fileDeleteRequestSize) + fileDeleteRequestSize;

    int bufPos = 0;
    uint8_t* buffer = request->buffer;

    bufPos = BerEncoder_encodeTL(0xa0, confirmedRequestPduSize, buffer, bufPos);
    bufPos = BerEncoder_encodeTL(0x02, invokeIdSize, buffer, bufPos);
    bufPos = BerEncoder_encodeUInt32(invokeId, buffer, bufPos);

    /* Encode FileOpen tag (context | structured ) [76 = 4ch] */
    buffer[bufPos++] = 0xbf;
    buffer[bufPos++] = 0x4c;

    bufPos = BerEncoder_encodeLength(fileDeleteRequestSize, buffer, bufPos);
    bufPos = BerEncoder_encodeOctetString(0x19, (uint8_t*) fileName, fileNameStringSize, buffer, bufPos);

    request->size = bufPos;
}

void
mmsClient_createFileReadRequest(uint32_t invokeId, ByteBuffer* request, int32_t frsmId)
{
    uint32_t invokeIdSize = BerEncoder_UInt32determineEncodedSize(invokeId);

    uint8_t frsmIdBuf[5];
    Asn1PrimitiveValue frsmIdBer;
    frsmIdBer.octets = frsmIdBuf;
    frsmIdBer.maxSize = 5;
    frsmIdBer.size = 0;

    BerInteger_setInt32(&frsmIdBer, frsmId);

    uint32_t frsmIdSize = frsmIdBer.size;

    uint32_t confirmedRequestPduSize = 1 + 2 + 2  + invokeIdSize + frsmIdSize;

    int bufPos = 0;
    uint8_t* buffer = request->buffer;

    bufPos = BerEncoder_encodeTL(0xa0, confirmedRequestPduSize, buffer, bufPos);
    bufPos = BerEncoder_encodeTL(0x02, invokeIdSize, buffer, bufPos);
    bufPos = BerEncoder_encodeUInt32(invokeId, buffer, bufPos);

    /* Encode FileRead tag (context | primitive ) [73 = 49h] */
    buffer[bufPos++] = 0x9f;
    bufPos = BerEncoder_encodeOctetString(0x49, frsmIdBer.octets, frsmIdBer.size, buffer, bufPos);

    request->size = bufPos;
}

static int
encodeFileSpecification(uint8_t tag, const char* fileSpecification, uint8_t* buffer, int bufPos)
{
    uint32_t fileNameStringSize = strlen(fileSpecification);
    uint32_t fileNameSeqSize = 1 + BerEncoder_determineLengthSize(fileNameStringSize) + fileNameStringSize;

    if (buffer != NULL) {

        bufPos = BerEncoder_encodeTL(tag, fileNameSeqSize, buffer, bufPos);
        bufPos = BerEncoder_encodeOctetString(0x19, (uint8_t*) fileSpecification, fileNameStringSize, buffer, bufPos);

        return bufPos;
    }
    else {
        return fileNameSeqSize + 1 + BerEncoder_determineLengthSize(fileNameSeqSize);
    }
}

void
mmsClient_createFileDirectoryRequest(uint32_t invokeId, ByteBuffer* request, const char* fileSpecification, const char* continueAfter)
{
    uint32_t invokeIdSize = BerEncoder_UInt32determineEncodedSize(invokeId);

    uint32_t confirmedRequestPduSize = 1 + 2 + 1  + invokeIdSize;

    uint32_t parameterSize = 0;

    if (fileSpecification)
        parameterSize += encodeFileSpecification(0xa0, fileSpecification, NULL, 0);

    if (continueAfter)
        parameterSize += encodeFileSpecification(0xa1, continueAfter, NULL, 0);

    confirmedRequestPduSize += BerEncoder_determineLengthSize(parameterSize) + parameterSize;

    int bufPos = 0;
    uint8_t* buffer = request->buffer;

    bufPos = BerEncoder_encodeTL(0xa0, confirmedRequestPduSize, buffer, bufPos);
    bufPos = BerEncoder_encodeTL(0x02, invokeIdSize, buffer, bufPos);
    bufPos = BerEncoder_encodeUInt32(invokeId, buffer, bufPos);

    /* Encode FileRead tag (context | structured ) [77 = 4dh] */
    buffer[bufPos++] = 0xbf;
    buffer[bufPos++] = 0x4d;
    bufPos = BerEncoder_encodeLength(parameterSize, buffer, bufPos);

    if (fileSpecification)
        bufPos = encodeFileSpecification(0xa0, fileSpecification, buffer, bufPos);

    if (continueAfter)
        bufPos = encodeFileSpecification(0xa1, continueAfter, buffer, bufPos);

    request->size = bufPos;
}


void
mmsClient_createFileRenameRequest(uint32_t invokeId, ByteBuffer* request, const char* currentFileName, const char* newFileName)
{
    uint32_t invokeIdSize = BerEncoder_UInt32determineEncodedSize(invokeId);

    uint32_t parameterSize = 0;

    parameterSize += encodeFileSpecification(0xa0, currentFileName, NULL, 0);

    parameterSize += encodeFileSpecification(0xa1, newFileName, NULL, 0);

    uint32_t confirmedRequestPduSize = 2  + invokeIdSize + 2 + BerEncoder_determineLengthSize(parameterSize) + parameterSize;

    int bufPos = 0;
    uint8_t* buffer = request->buffer;

    bufPos = BerEncoder_encodeTL(0xa0, confirmedRequestPduSize, buffer, bufPos);
    bufPos = BerEncoder_encodeTL(0x02, invokeIdSize, buffer, bufPos);
    bufPos = BerEncoder_encodeUInt32(invokeId, buffer, bufPos);

    /* Encode FileRead tag (context | structured ) [75 = 4bh] */
    buffer[bufPos++] = 0xbf;
    buffer[bufPos++] = 0x4b;
    bufPos = BerEncoder_encodeLength(parameterSize, buffer, bufPos);

    bufPos = encodeFileSpecification(0xa0, currentFileName, buffer, bufPos);

    bufPos = encodeFileSpecification(0xa1, newFileName, buffer, bufPos);

    request->size = bufPos;
}

void
mmsClient_createObtainFileRequest(uint32_t invokeId, ByteBuffer* request, const char* sourceFile, const char* destinationFile)
{
    uint32_t invokeIdSize = BerEncoder_UInt32determineEncodedSize(invokeId);

    uint32_t parameterSize = 0;

    parameterSize += encodeFileSpecification(0xa0, sourceFile, NULL, 0);

    parameterSize += encodeFileSpecification(0xa1, destinationFile, NULL, 0);

    uint32_t confirmedRequestPduSize = 2  + invokeIdSize + 2 + BerEncoder_determineLengthSize(parameterSize) + parameterSize;

    int bufPos = 0;
    uint8_t* buffer = request->buffer;

    bufPos = BerEncoder_encodeTL(0xa0, confirmedRequestPduSize, buffer, bufPos);
    bufPos = BerEncoder_encodeTL(0x02, invokeIdSize, buffer, bufPos);
    bufPos = BerEncoder_encodeUInt32(invokeId, buffer, bufPos);

    /* Encode ObtainFile tag (context | structured ) [46 = 2eh] */
    buffer[bufPos++] = 0xbf;
    buffer[bufPos++] = 0x2e;
    bufPos = BerEncoder_encodeLength(parameterSize, buffer, bufPos);

    bufPos = encodeFileSpecification(0xa1, sourceFile, buffer, bufPos);

    bufPos = encodeFileSpecification(0xa2, destinationFile, buffer, bufPos);

    request->size = bufPos;
}


static bool
parseFileAttributes(uint8_t* buffer, int bufPos, int maxBufPos, uint32_t* fileSize, uint64_t* lastModified)
{
    int endPos = maxBufPos;

    while (bufPos < endPos) {
        uint8_t tag = buffer[bufPos++];
        int length;

        bufPos = BerDecoder_decodeLength(buffer, &length, bufPos, endPos);
        if (bufPos < 0)
            return false;

        switch (tag) {
        case 0x80: /* sizeOfFile */
            if (fileSize != NULL)
                *fileSize = BerDecoder_decodeUint32(buffer, length, bufPos);
            break;
        case 0x81: /* lastModified */
            {
                if (lastModified != NULL) {
                    char gtString[40];
                    memcpy(gtString, buffer + bufPos, length);
                    gtString[length] = 0;
                    *lastModified = Conversions_generalizedTimeToMsTime(gtString);
                }
            }
            break;
        case 0x00: /* indefinite length end tag -> ignore */
            break;
        default:
            return false;
        }

        bufPos += length;
    }

    return true;
}

static bool
parseDirectoryEntry(uint8_t* buffer, int bufPos, int maxBufPos, uint32_t invokeId, MmsConnection_FileDirectoryHandler handler, void* parameter)
{
    char fileNameMemory[400];
    char* filename = NULL;
    uint32_t fileSize = 0;
    uint64_t lastModified = 0;

    while (bufPos < maxBufPos) {
        uint8_t tag = buffer[bufPos++];
        int length;

        bufPos = BerDecoder_decodeLength(buffer, &length, bufPos, maxBufPos);
        if (bufPos < 0) {
            if (DEBUG_MMS_CLIENT)
                printf("MMS_CLIENT: invalid length field\n");
            return false;
        }

        switch (tag) {
        case 0xa0: /* file-name */

            filename = fileNameMemory;

            tag = buffer[bufPos++];

            bufPos = BerDecoder_decodeLength(buffer, &length, bufPos, maxBufPos);
            if (bufPos < 0) {
                if (DEBUG_MMS_CLIENT)
                    printf("MMS_CLIENT: invalid length field\n");
                return false;
            }

            memcpy(filename, buffer + bufPos, length);
            filename[length] = 0;

            bufPos += length;
            break;
        case 0xa1: /* file-attributes */
            if (!parseFileAttributes(buffer, bufPos, bufPos + length, &fileSize, &lastModified))
                return false;
            bufPos += length;
            break;
        case 0x00: /* indefinite length end tag -> ignore */
            break;
        default:
            bufPos += length;
            if (DEBUG_MMS_CLIENT)
                printf("MMS_CLIENT: message contains unknown tag!\n");

            return false;
        }
    }

    if (filename != NULL)
        handler(invokeId, parameter, MMS_ERROR_NONE, filename, fileSize, lastModified, true);
    else
        return false;

    return true;
}

static bool
parseListOfDirectoryEntries(uint8_t* buffer, int bufPos, int maxBufPos, uint32_t invokeId,
        MmsConnection_FileDirectoryHandler handler, void* parameter)
{
    uint8_t tag = buffer[bufPos++];

    if (tag != 0x30)
        return false;

    int length;

    bufPos = BerDecoder_decodeLength(buffer, &length, bufPos, maxBufPos);

    if (bufPos < 0) return false;

    int endPos = bufPos + length;

    while (bufPos < endPos) {
        tag = buffer[bufPos++];

        bufPos = BerDecoder_decodeLength(buffer, &length, bufPos, maxBufPos);
        if (bufPos < 0) return false;

        switch (tag) {
        case 0x30: /* Sequence */
            parseDirectoryEntry(buffer, bufPos, bufPos + length, invokeId, handler, parameter);
            bufPos += length;
            break;
        case 0x00: /* indefinite length end tag -> ignore */
            break;
        default:
            bufPos += length;
            if (DEBUG_MMS_CLIENT)
                printf("mmsClient_parseFileDirectoryResponse: message contains unknown tag!\n");
            return false;
        }
    }

    return true;
}


bool
mmsClient_parseFileDirectoryResponse(ByteBuffer* response, int bufPos, uint32_t invokeId, MmsConnection_FileDirectoryHandler handler, void* parameter)
{
    uint8_t* buffer = response->buffer;
    int maxBufPos = response->size;
    int length;

    uint8_t tag = buffer[bufPos++];

    if (tag != 0xbf) {
        if (DEBUG_MMS_CLIENT)
            printf("mmsClient_parseFileDirectoryResponse: unknown tag %02x\n", tag);
        return false;
    }

    tag = buffer[bufPos++];

    if (tag != 0x4d) {
        if (DEBUG_MMS_CLIENT)
            printf("mmsClient_parseFileDirectoryResponse: unknown tag %02x\n", tag);
        return false;
    }

    bufPos = BerDecoder_decodeLength(buffer, &length, bufPos, maxBufPos);
    if (bufPos < 0) return false;

    int endPos = bufPos + length;

    bool moreFollows = false;

    while (bufPos < endPos) {
        tag = buffer[bufPos++];

        bufPos = BerDecoder_decodeLength(buffer, &length, bufPos, maxBufPos);
        if (bufPos < 0) return false;

        switch (tag) {
        case 0xa0: /* listOfDirectoryEntries */
            parseListOfDirectoryEntries(buffer, bufPos, bufPos + length, invokeId, handler, parameter);

            bufPos += length;
            break;
        case 0x81: /* moreFollows */
            moreFollows = BerDecoder_decodeBoolean(buffer, bufPos);
            bufPos += length;
            break;
        case 0x00: /* indefinite length end tag -> ignore */
            break;
        default:
            bufPos += length;
            if (DEBUG_MMS_CLIENT)
                printf("mmsClient_parseFileDirectoryResponse: message contains unknown tag!\n");
            break;
        }
    }

    handler(invokeId, parameter, MMS_ERROR_NONE, NULL, 0, 0, moreFollows);

    return true;
}

bool
mmsMsg_parseFileOpenResponse(uint8_t* buffer, int bufPos, int maxBufPos, int32_t* frsmId, uint32_t* fileSize, uint64_t* lastModified)
{
    int length;

    uint8_t tag = buffer[bufPos++];

    if (tag != 0xbf) {
        if (DEBUG_MMS_CLIENT)
            printf("MMS: mmsClient_parseFileOpenResponse: unknown tag %02x\n", tag);
        return false;
    }

    tag = buffer[bufPos++];

    if (tag != 0x48) {
        if (DEBUG_MMS_CLIENT)
            printf("MMS_CLIENT: mmsClient_parseFileOpenResponse: unknown tag %02x\n", tag);
        return false;
    }

    bufPos = BerDecoder_decodeLength(buffer, &length, bufPos, maxBufPos);
    if (bufPos < 0)
        return false;

    int endPos = bufPos + length;

    while (bufPos < endPos) {
        tag = buffer[bufPos++];

        bufPos = BerDecoder_decodeLength(buffer, &length, bufPos, maxBufPos);
        if (bufPos < 0)
            return false;

        switch (tag) {
        case 0x80: /* frsmId */
            *frsmId = BerDecoder_decodeInt32(buffer, length, bufPos);

            bufPos += length;
            break;
        case 0xa1: /* fileAttributes */
            if (!parseFileAttributes(buffer, bufPos, bufPos + length, fileSize, lastModified))
                return false;
            bufPos += length;
            break;
        case 0x00: /* indefinite length end tag -> ignore */
            break;
        default:
            bufPos += length;
            if (DEBUG_MMS_CLIENT)
                printf("MMS_CLIENT/SERVER: mmsClient_parseFileOpenResponse: message contains unknown tag %02x!\n", tag);
            break;
        }
    }

    return true;
}

bool
mmsMsg_parseFileReadResponse(uint8_t* buffer, int bufPos, int maxBufPos, uint32_t invokeId, int frsmId, bool* moreFollows, MmsConnection_FileReadHandler handler, void* handlerParameter)
{
    int length;
    uint8_t* data = NULL;
    int dataLen = 0;


    uint8_t tag = buffer[bufPos++];


    if (tag != 0xbf) {
        if (DEBUG_MMS_CLIENT)
            printf("MMS_CLIENT/SERVER: mmsClient_parseFileReadResponse: unknown tag %02x\n", tag);
        return false;
    }

    tag = buffer[bufPos++];

    *moreFollows = true;

    if (tag != 0x49) {
        if (DEBUG_MMS_CLIENT)
            printf("MMS_CLIENT/SERVER: mmsClient_parseFileReadResponse: unknown tag %02x\n", tag);
        return false;
    }

    bufPos = BerDecoder_decodeLength(buffer, &length, bufPos, maxBufPos);
    if (bufPos < 0)
        return false;

    int endPos = bufPos + length;

    while (bufPos < endPos) {
        tag = buffer[bufPos++];

        bufPos = BerDecoder_decodeLength(buffer, &length, bufPos, maxBufPos);
        if (bufPos < 0)
            return false;

        switch (tag) {
        case 0x80: /* fileData */
            data = buffer + bufPos;
            dataLen = length;

            bufPos += length;
            break;

        case 0x81: /* moreFollows */
            *moreFollows = BerDecoder_decodeBoolean(buffer, bufPos);
            bufPos += length;
            break;

        case 0x00: /* indefinite length end tag -> ignore */
            break;

        default:
            bufPos += length;
            if (DEBUG_MMS_CLIENT)
                printf("MMS_CLIENT/SERVER: mmsClient_parseFileReadResponse: message contains unknown tag %02x!\n", tag);

            return false;
        }
    }

    handler(invokeId, handlerParameter, MMS_ERROR_NONE, frsmId, data, dataLen, *moreFollows);

    return true;
}

void
mmsClient_createFileCloseRequest(uint32_t invokeId, ByteBuffer* request, int32_t frsmId)
{
    uint32_t invokeIdSize = BerEncoder_UInt32determineEncodedSize(invokeId);

    uint8_t frsmIdBuf[5];
    Asn1PrimitiveValue frsmIdBer;
    frsmIdBer.octets = frsmIdBuf;
    frsmIdBer.maxSize = 5;
    frsmIdBer.size = 0;

    BerInteger_setInt32(&frsmIdBer, frsmId);

    uint32_t confirmedRequestPduSize = 1 + 2 + 2  + invokeIdSize + frsmIdBer.size;

    int bufPos = 0;
    uint8_t* buffer = request->buffer;

    bufPos = BerEncoder_encodeTL(0xa0, confirmedRequestPduSize, buffer, bufPos);
    bufPos = BerEncoder_encodeTL(0x02, invokeIdSize, buffer, bufPos);
    bufPos = BerEncoder_encodeUInt32(invokeId, buffer, bufPos);

    /* Encode FileClose tag (context | primitive) [74 = 4ah] */
    buffer[bufPos++] = 0x9f;
    bufPos = BerEncoder_encodeOctetString(0x4a, frsmIdBer.octets, frsmIdBer.size, buffer, bufPos);

    request->size = bufPos;
}

#endif /* (MMS_FILE_SERVICE == 1) */
