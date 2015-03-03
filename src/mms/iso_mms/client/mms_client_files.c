/*
 *  mms_client_files.c
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
 */

#include "libiec61850_platform_includes.h"
#include "stack_config.h"
#include "mms_common.h"
#include "mms_client_connection.h"
#include "byte_buffer.h"

#include "mms_client_internal.h"
#include "ber_encoder.h"
#include "ber_decode.h"
#include "conversions.h"

void
mmsClient_createFileOpenRequest(uint32_t invokeId, ByteBuffer* request, const char* fileName, uint32_t initialPosition)
{
    uint32_t invokeIdSize = BerEncoder_UInt32determineEncodedSize(invokeId);

    uint32_t fileNameStringSize = strlen(fileName);
    uint32_t fileNameSize = 1+ BerEncoder_determineLengthSize(fileNameStringSize) + fileNameStringSize;

    uint32_t fileNameSeqSize = fileNameSize;

    uint32_t fileOpenRequestSize = fileNameSeqSize + 2 + BerEncoder_UInt32determineEncodedSize(initialPosition) + 2;

    uint32_t confirmedRequestPduSize = 1 + 2 + 2  + invokeIdSize + fileOpenRequestSize;

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

    uint32_t fileNameSeqSize = fileNameSize;

    uint32_t fileDeleteRequestSize = fileNameSeqSize; //  + 2;

    uint32_t confirmedRequestPduSize = 1 + 2 + 2 + invokeIdSize + fileDeleteRequestSize;

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

    uint32_t frsmIdSize = BerEncoder_UInt32determineEncodedSize(frsmId);

    uint32_t confirmedRequestPduSize = 1 + 2 + 2  + invokeIdSize + frsmIdSize;

    int bufPos = 0;
    uint8_t* buffer = request->buffer;

    bufPos = BerEncoder_encodeTL(0xa0, confirmedRequestPduSize, buffer, bufPos);
    bufPos = BerEncoder_encodeTL(0x02, invokeIdSize, buffer, bufPos);
    bufPos = BerEncoder_encodeUInt32(invokeId, buffer, bufPos);

    /* Encode FileRead tag (context | primitive ) [73 = 49h] */
    buffer[bufPos++] = 0x9f;
    buffer[bufPos++] = 0x49;
    bufPos = BerEncoder_encodeLength(frsmIdSize, buffer, bufPos);
    bufPos = BerEncoder_encodeUInt32(frsmId, buffer, bufPos);

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

    uint32_t confirmedRequestPduSize = 1 + 2 + 2  + invokeIdSize + 0;

    uint32_t parameterSize = 0;

    if (fileSpecification)
        parameterSize += encodeFileSpecification(0xa0, fileSpecification, NULL, 0);

    if (continueAfter)
        parameterSize += encodeFileSpecification(0xa1, continueAfter, NULL, 0);

    confirmedRequestPduSize += parameterSize;

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

    uint32_t confirmedRequestPduSize = 1 + 2 + 2  + invokeIdSize + 0;

    uint32_t parameterSize = 0;

    parameterSize += encodeFileSpecification(0xa0, currentFileName, NULL, 0);

    parameterSize += encodeFileSpecification(0xa1, newFileName, NULL, 0);

    confirmedRequestPduSize += parameterSize;

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
        default:
            return false;
        }

        bufPos += length;
    }

    return true;
}

static bool
parseDirectoryEntry(uint8_t* buffer, int bufPos, int maxBufPos, MmsFileDirectoryHandler handler, void* handlerParameter)
{
    char fileNameMemory[400];
    char* filename = NULL;
    uint32_t fileSize = 0;
    uint64_t lastModified = 0;

    while (bufPos < maxBufPos) {
        uint8_t tag = buffer[bufPos++];
        int length;
        bufPos = BerDecoder_decodeLength(buffer, &length, bufPos, maxBufPos);

        switch (tag) {
        case 0xa0: /* file-name */

            filename = fileNameMemory;

            tag = buffer[bufPos++];
            bufPos = BerDecoder_decodeLength(buffer, &length, bufPos, maxBufPos);
            memcpy(filename, buffer + bufPos, length);
            filename[length] = 0;

            bufPos += length;
            break;
        case 0xa1: /* file-attributes */
            if (!parseFileAttributes(buffer, bufPos, bufPos + length, &fileSize, &lastModified))
                return false;
            bufPos += length;
            break;
        default:
            bufPos += length;
            if (DEBUG_MMS_CLIENT)
                printf("mmsClient_parseFileDirectoryResponse: message contains unknown tag!\n");

            return false;
        }
    }

    if (filename != NULL)
        handler(handlerParameter, filename, fileSize, lastModified);
    else
        return false;

    return true;
}

static bool
parseListOfDirectoryEntries(uint8_t* buffer, int bufPos, int maxBufPos,
        MmsFileDirectoryHandler handler, void* handlerParameter)
{
    uint8_t tag = buffer[bufPos++];

    if (tag != 0x30)
        return false;

    int length;

    bufPos = BerDecoder_decodeLength(buffer, &length, bufPos, maxBufPos);

    if (bufPos < 0) return false;

    int endPos = bufPos + length;

    if (endPos > maxBufPos) {
         if (DEBUG_MMS_CLIENT)
             printf("parseListOfDirectoryEntries: message to short!\n");
         return false;
    }

    while (bufPos < endPos) {
        tag = buffer[bufPos++];
        bufPos = BerDecoder_decodeLength(buffer, &length, bufPos, maxBufPos);

        switch (tag) {
        case 0x30: /* Sequence */
            parseDirectoryEntry(buffer, bufPos, bufPos + length, handler, handlerParameter);
            bufPos += length;
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
mmsClient_parseFileDirectoryResponse(MmsConnection self, MmsFileDirectoryHandler handler, void* handlerParameter,
        bool* moreFollows)
{
    uint8_t* buffer = self->lastResponse->buffer;
    int maxBufPos = self->lastResponse->size;
    int bufPos = self->lastResponseBufPos;
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

    if (endPos > maxBufPos) {
        if (DEBUG_MMS_CLIENT)
            printf("mmsClient_parseFileDirectoryResponse: message to short (length:%i maxBufPos:%i)!\n", length, maxBufPos);
        return false;
    }

    *moreFollows = false;

    while (bufPos < endPos) {
        tag = buffer[bufPos++];
        bufPos = BerDecoder_decodeLength(buffer, &length, bufPos, maxBufPos);

        switch (tag) {
        case 0xa0: /* listOfDirectoryEntries */
            parseListOfDirectoryEntries(buffer, bufPos, bufPos + length, handler, handlerParameter);

            bufPos += length;
            break;
        case 0x81: /* moreFollows */
            *moreFollows = BerDecoder_decodeBoolean(buffer, bufPos);
            bufPos += length;
            break;
        default:
            bufPos += length;
            if (DEBUG_MMS_CLIENT)
                printf("mmsClient_parseFileDirectoryResponse: message contains unknown tag!\n");
            break;
        }
    }

    return true;
}

bool
mmsClient_parseFileOpenResponse(MmsConnection self, int32_t* frsmId, uint32_t* fileSize, uint64_t* lastModified)
{
    uint8_t* buffer = self->lastResponse->buffer;
    int maxBufPos = self->lastResponse->size;
    int bufPos = self->lastResponseBufPos;
    int length;

    uint8_t tag = buffer[bufPos++];

    if (tag != 0xbf) {
        if (DEBUG_MMS_CLIENT)
            printf("mmsClient_parseFileOpenResponse: unknown tag %02x\n", tag);
        return false;
    }

    tag = buffer[bufPos++];

    if (tag != 0x48) {
        if (DEBUG_MMS_CLIENT)
            printf("mmsClient_parseFileOpenResponse: unknown tag %02x\n", tag);
        return false;
    }

    bufPos = BerDecoder_decodeLength(buffer, &length, bufPos, maxBufPos);
    if (bufPos < 0)
        return false;

    int endPos = bufPos + length;

    if (endPos > maxBufPos) {
        if (DEBUG_MMS_CLIENT)
            printf("mmsClient_parseFileOpenResponse: message to short (length:%i maxBufPos:%i)!\n", length, maxBufPos);
        return false;
    }

    while (bufPos < endPos) {
        tag = buffer[bufPos++];
        bufPos = BerDecoder_decodeLength(buffer, &length, bufPos, maxBufPos);

        switch (tag) {
        case 0x80: /* frsmId */
            *frsmId = (int32_t) BerDecoder_decodeUint32(buffer, length, bufPos);

            bufPos += length;
            break;
        case 0xa1: /* fileAttributes */
            if (!parseFileAttributes(buffer, bufPos, bufPos + length, fileSize, lastModified))
                return false;
            bufPos += length;
            break;
        default:
            bufPos += length;
            if (DEBUG_MMS_CLIENT)
                printf("mmsClient_parseFileOpenResponse: message contains unknown tag %02x!\n", tag);
            break;
        }
    }

    return true;
}


bool
mmsClient_parseFileReadResponse(MmsConnection self, int32_t frsmId,  bool* moreFollows, MmsFileReadHandler handler, void* handlerParameter)
{
    uint8_t* buffer = self->lastResponse->buffer;
    int maxBufPos = self->lastResponse->size;
    int bufPos = self->lastResponseBufPos;
    int length;

    uint8_t tag = buffer[bufPos++];

    if (tag != 0xbf) {
        if (DEBUG_MMS_CLIENT)
            printf("mmsClient_parseFileReadResponse: unknown tag %02x\n", tag);
        return false;
    }

    tag = buffer[bufPos++];

    *moreFollows = true;

    if (tag != 0x49) {
        if (DEBUG_MMS_CLIENT)
            printf("mmsClient_parseFileReadResponse: unknown tag %02x\n", tag);
        return false;
    }

    bufPos = BerDecoder_decodeLength(buffer, &length, bufPos, maxBufPos);
    if (bufPos < 0)
        return false;

    int endPos = bufPos + length;

    if (endPos > maxBufPos) {
        if (DEBUG_MMS_CLIENT)
            printf("mmsClient_parseFileReadResponse: message to short (length:%i maxBufPos:%i)!\n", length, maxBufPos);
        return false;
    }

    while (bufPos < endPos) {
        tag = buffer[bufPos++];
        bufPos = BerDecoder_decodeLength(buffer, &length, bufPos, maxBufPos);

        switch (tag) {
        case 0x80: /* fileData */
            handler(handlerParameter, frsmId, buffer + bufPos, length);

            bufPos += length;
            break;
        case 0x81: /* moreFollows */
            *moreFollows = BerDecoder_decodeBoolean(buffer, bufPos);
            bufPos += length;
            break;
        default:
            bufPos += length;
            if (DEBUG_MMS_CLIENT)
                printf("mmsClient_parseFileReadResponse: message contains unknown tag %02x!\n", tag);

            return false;
        }
    }

    return true;
}

void
mmsClient_createFileCloseRequest(uint32_t invokeId, ByteBuffer* request, int32_t frsmId)
{
    uint32_t invokeIdSize = BerEncoder_UInt32determineEncodedSize(invokeId);

    uint32_t frsmIdSize = BerEncoder_UInt32determineEncodedSize(frsmId);

    uint32_t confirmedRequestPduSize = 1 + 2 + 2  + invokeIdSize + frsmIdSize;

    int bufPos = 0;
    uint8_t* buffer = request->buffer;

    bufPos = BerEncoder_encodeTL(0xa0, confirmedRequestPduSize, buffer, bufPos);
    bufPos = BerEncoder_encodeTL(0x02, invokeIdSize, buffer, bufPos);
    bufPos = BerEncoder_encodeUInt32(invokeId, buffer, bufPos);

    /* Encode FileClose tag (context | primitive) [74 = 4ah] */
    buffer[bufPos++] = 0x9f;
    buffer[bufPos++] = 0x4a;
    bufPos = BerEncoder_encodeLength(frsmIdSize, buffer, bufPos);
    bufPos = BerEncoder_encodeUInt32(frsmId, buffer, bufPos);

    request->size = bufPos;
}


