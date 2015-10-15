/*
 *  mms_file_service.c
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
#include "mms_server_internal.h"

#if (MMS_FILE_SERVICE == 1)

#include "hal_filesystem.h"
#include "conversions.h"

#define CONFIG_MMS_FILE_SERVICE_MAX_FILENAME_LENGTH 256

static void
createNullResponseExtendedTag(uint32_t invokeId, ByteBuffer* response, uint8_t tag)
{
    uint32_t invokeIdSize = BerEncoder_UInt32determineEncodedSize(invokeId) + 2;

    uint32_t confirmedResponsePDUSize = invokeIdSize + 3 ;

    uint8_t* buffer = response->buffer;

    int bufPos = 0;

    bufPos = BerEncoder_encodeTL(0xa1, confirmedResponsePDUSize, buffer, bufPos);

    bufPos = BerEncoder_encodeTL(0x02, invokeIdSize - 2, buffer, bufPos);
    bufPos = BerEncoder_encodeUInt32(invokeId, buffer, bufPos);

    buffer[bufPos++] = 0x9f; /* Context specific / primitive / extended */
    bufPos = BerEncoder_encodeTL(tag, 0, buffer, bufPos);

    response->size = bufPos;
}

static MmsFileReadStateMachine*
getFreeFrsm(MmsServerConnection connection)
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
getFrsm(MmsServerConnection connection, int32_t frsmId)
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
getNextFrsmId(MmsServerConnection connection)
{
    uint32_t nextFrsmId = connection->nextFrsmId;
    connection->nextFrsmId++;

    return nextFrsmId;
}

static int
encodeFileAttributes(uint8_t tag, uint32_t fileSize, char* gtString, uint8_t* buffer, int bufPos)
{
    uint32_t sizeOfFileSize = BerEncoder_UInt32determineEncodedSize(fileSize);

    uint32_t gtStringSize = strlen(gtString);

    uint32_t fileAttributesSize = 1 + BerEncoder_determineLengthSize(sizeOfFileSize) + sizeOfFileSize
            + 2 + gtStringSize;

    if (buffer == NULL) {


        return fileAttributesSize;
    }
    else {
        bufPos = BerEncoder_encodeTL(tag, fileAttributesSize, buffer, bufPos); /* file attributes */
        bufPos = BerEncoder_encodeTL(0x80, sizeOfFileSize, buffer, bufPos); /* file size */
        bufPos = BerEncoder_encodeUInt32(fileSize, buffer, bufPos);
        bufPos = BerEncoder_encodeOctetString(0x81, (uint8_t*) gtString, gtStringSize, buffer, bufPos);
        return bufPos;
    }
}


static void
createFileOpenResponse(uint32_t invokeId, ByteBuffer* response, char* fullPath, MmsFileReadStateMachine* frsm)
{
    uint64_t msTime;

    FileSystem_getFileInfo(fullPath, &(frsm->fileSize), &msTime);

    char gtString[30];

    Conversions_msTimeToGeneralizedTime(msTime, (uint8_t*) gtString);

    uint32_t fileAttributesSize = encodeFileAttributes(0xa1, frsm->fileSize, gtString, NULL, 0) + 2;

    uint32_t invokeIdSize = BerEncoder_UInt32determineEncodedSize(invokeId) + 2;

    uint32_t frsmIdSize = BerEncoder_UInt32determineEncodedSize(frsm->frsmId) + 2;

    uint32_t openFileResponseSize = fileAttributesSize + frsmIdSize;

    uint32_t confirmedResponsePDUSize = invokeIdSize + 2 + BerEncoder_determineLengthSize(openFileResponseSize)
               + openFileResponseSize;

    uint8_t* buffer = response->buffer;

    int bufPos = 0;

    bufPos = BerEncoder_encodeTL(0xa1, confirmedResponsePDUSize, buffer, bufPos);

    bufPos = BerEncoder_encodeTL(0x02, invokeIdSize - 2, buffer, bufPos);
    bufPos = BerEncoder_encodeUInt32(invokeId, buffer, bufPos);

    buffer[bufPos++] = 0xbf;
    bufPos = BerEncoder_encodeTL(0x48, openFileResponseSize, buffer, bufPos);
    bufPos = BerEncoder_encodeTL(0x80, frsmIdSize - 2, buffer, bufPos);
    bufPos = BerEncoder_encodeUInt32(frsm->frsmId, buffer, bufPos);
    bufPos = encodeFileAttributes(0xa1, frsm->fileSize, gtString, buffer, bufPos);

    response->size = bufPos;
}

static bool
parseFileName(char* filename, uint8_t* buffer, int* bufPos, int maxBufPos , uint32_t invokeId, ByteBuffer* response)
{
    uint8_t tag = buffer[(*bufPos)++];
    int length;

    if (tag != 0x19) {
      mmsServer_writeMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_INVALID_PDU, response);
      return false;
    }

    *bufPos = BerDecoder_decodeLength(buffer, &length, *bufPos, maxBufPos);

    if (*bufPos < 0)  {
      mmsServer_writeMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_INVALID_PDU, response);
      return false;
    }

    if (length > 255) {
      mmsServer_writeMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_REQUEST_INVALID_ARGUMENT, response);
      return false;
    }

    memcpy(filename, buffer + *bufPos, length);
    filename[length] = 0;
    *bufPos += length;

    return true;
}

void
mmsServer_handleFileDeleteRequest(
    MmsServerConnection connection,
    uint8_t* buffer, int bufPos, int maxBufPos,
    uint32_t invokeId,
    ByteBuffer* response)
{
    if (buffer[bufPos++] != 0x19)
        goto exit_reject_invalid_pdu;

    int length;

    bufPos = BerDecoder_decodeLength(buffer, &length, bufPos, maxBufPos);

    char filename[256];

    memcpy(filename, buffer + bufPos, length);
    filename[length] = 0;

    if (DEBUG_MMS_SERVER)
        printf("MMS_SERVER: mms_file_service.c: Delete file (%s)\n", filename);

    if (!FileSystem_getFileInfo(filename, NULL, NULL)) {
        if (DEBUG_MMS_SERVER)
            printf("MMS_SERVER: mms_file_service.c:  File (%s) not found\n", filename);

        mmsServer_createConfirmedErrorPdu(invokeId, response, MMS_ERROR_FILE_FILE_NON_EXISTENT);
        return;
    }

    if (!FileSystem_deleteFile(filename)) {
        if (DEBUG_MMS_SERVER)
            printf("MMS_SERVER: mms_file_service.c:  Delete file (%s) failed\n", filename);

        mmsServer_createConfirmedErrorPdu(invokeId, response, MMS_ERROR_FILE_FILE_ACCESS_DENIED);
        return;
    }

    createNullResponseExtendedTag(invokeId, response, 0x4c);
    return;

exit_reject_invalid_pdu:
    mmsServer_writeMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_INVALID_PDU, response);
}

void
mmsServer_handleFileOpenRequest(
    MmsServerConnection connection,
    uint8_t* buffer, int bufPos, int maxBufPos,
    uint32_t invokeId,
    ByteBuffer* response)
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

            if (!parseFileName(filename, buffer, &bufPos, bufPos + length, invokeId, response))
                return;

            hasFileName = true;

            break;

        case 0x81: /* initial position */
            filePosition = BerDecoder_decodeUint32(buffer, length, bufPos);
            bufPos += length;
            break;

        default: /* unrecognized parameter */
            bufPos += length;
            goto exit_reject_invalid_pdu;
        }
    }

    if (hasFileName) {

        MmsFileReadStateMachine* frsm = getFreeFrsm(connection);

        if (frsm != NULL) {
            FileHandle fileHandle = FileSystem_openFile(filename, false);

            if (fileHandle != NULL) {
                frsm->fileHandle = fileHandle;
                frsm->readPosition = filePosition;
                frsm->frsmId = getNextFrsmId(connection);

                createFileOpenResponse(invokeId, response, filename, frsm);
            }
            else
                mmsServer_createConfirmedErrorPdu(invokeId, response, MMS_ERROR_FILE_FILE_NON_EXISTENT);


        }
        else
            mmsServer_createConfirmedErrorPdu(invokeId, response, MMS_ERROR_RESOURCE_OTHER);
    }
    else
        goto exit_invalid_parameter;

    return;

exit_invalid_parameter:
    mmsServer_writeMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_REQUEST_INVALID_ARGUMENT, response);
    return;

exit_reject_invalid_pdu:
    mmsServer_writeMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_INVALID_PDU, response);
}


static void
createFileReadResponse(MmsServerConnection connection, uint32_t invokeId,
        ByteBuffer* response,  MmsFileReadStateMachine* frsm)
{
     /* determine remaining bytes in file */
     uint32_t bytesLeft = frsm->fileSize - frsm->readPosition;

     uint32_t fileChunkSize = 0;

     uint32_t maxFileChunkSize = connection->maxPduSize - 20;

     uint32_t fileReadResponseSize = 1; /* for tag */

     bool moreFollows = true;

     if (bytesLeft > maxFileChunkSize) {
         fileChunkSize = maxFileChunkSize;
     }
     else {
         fileChunkSize = bytesLeft;
         moreFollows = false;
         fileReadResponseSize += 3; /* for moreFollows */
     }

     fileReadResponseSize += fileChunkSize;
     fileReadResponseSize += BerEncoder_determineLengthSize(fileChunkSize);

     frsm->readPosition += fileChunkSize;

     uint32_t invokeIdSize = BerEncoder_UInt32determineEncodedSize(invokeId) + 2;

     uint32_t confirmedResponsePDUSize = invokeIdSize + 2 + BerEncoder_determineLengthSize(fileReadResponseSize)
                + fileReadResponseSize;

     uint8_t* buffer = response->buffer;

     int bufPos = 0;

     bufPos = BerEncoder_encodeTL(0xa1, confirmedResponsePDUSize, buffer, bufPos);

     bufPos = BerEncoder_encodeTL(0x02, invokeIdSize - 2, buffer, bufPos);
     bufPos = BerEncoder_encodeUInt32(invokeId, buffer, bufPos);

     buffer[bufPos++] = 0xbf;
     bufPos = BerEncoder_encodeTL(0x49, fileReadResponseSize, buffer, bufPos);

     bufPos = BerEncoder_encodeTL(0x80, fileChunkSize, buffer, bufPos);
     FileSystem_readFile(frsm->fileHandle, buffer + bufPos, fileChunkSize);
     bufPos += fileChunkSize;

     if (!moreFollows)
         bufPos = BerEncoder_encodeBoolean(0x81, false, buffer, bufPos);

     response->size = bufPos;
}

void
mmsServer_handleFileReadRequest(
    MmsServerConnection connection,
    uint8_t* buffer, int bufPos, int maxBufPos,
    uint32_t invokeId,
    ByteBuffer* response)
{
    int32_t frsmId = (int32_t) BerDecoder_decodeUint32(buffer, maxBufPos - bufPos, bufPos);

    if (DEBUG_MMS_SERVER)
        printf("MMS_SERVER: mmsServer_handleFileReadRequest read request for frsmId: %i\n", frsmId);

    MmsFileReadStateMachine* frsm = getFrsm(connection, frsmId);

    if (frsm != NULL)
        createFileReadResponse(connection, invokeId, response, frsm);
    else
        mmsServer_createConfirmedErrorPdu(invokeId, response, MMS_ERROR_FILE_OTHER);
}

static void
createFileCloseResponse(uint32_t invokeId, ByteBuffer* response)
{
    createNullResponseExtendedTag(invokeId, response, 0x4a);
}


void
mmsServer_handleFileCloseRequest(
    MmsServerConnection connection,
    uint8_t* buffer, int bufPos, int maxBufPos,
    uint32_t invokeId,
    ByteBuffer* response)
{
    int32_t frsmId = (int32_t) BerDecoder_decodeUint32(buffer, maxBufPos - bufPos, bufPos);

    MmsFileReadStateMachine* frsm = getFrsm(connection, frsmId);

    FileSystem_closeFile(frsm->fileHandle);
    frsm->fileHandle = NULL;
    frsm->frsmId = 0;

    createFileCloseResponse(invokeId, response);
}



static int //TODO remove redundancy - same as in client code!
encodeFileSpecification(uint8_t tag, char* fileSpecification, uint8_t* buffer, int bufPos)
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

static int
addFileEntriesToResponse(uint8_t* buffer, int bufPos, int maxBufSize, char* directoryName, char* continueAfterFileName, bool* moreFollows)
{
	int directoryNameLength = strlen(directoryName);

    DirectoryHandle directory = FileSystem_openDirectory(directoryName);

    if (directory != NULL) {
        bool isDirectory;
        char* fileName = FileSystem_readDirectory(directory, &isDirectory);

        while (fileName != NULL) {
        	directoryName[directoryNameLength] = 0;

        	if (directoryNameLength > 0)
        		strcat(directoryName, "/");

        	strcat(directoryName, fileName);

            if (isDirectory) {
            	bufPos = addFileEntriesToResponse(buffer, bufPos, maxBufSize, directoryName, continueAfterFileName, moreFollows);

            	if (*moreFollows == true)
            		break;
            }
            else {
            	if (continueAfterFileName != NULL) {
					if (strcmp(continueAfterFileName, directoryName) == 0)
						continueAfterFileName = NULL;
				}
				else {
					uint64_t msTime;

					uint32_t fileSize;

					FileSystem_getFileInfo(directoryName, &fileSize, &msTime);

					char gtString[30];

					Conversions_msTimeToGeneralizedTime(msTime, (uint8_t*) gtString);

					int fileAttributesSize = encodeFileAttributes(0xa1, fileSize, gtString, NULL, 0);

					int filenameSize = encodeFileSpecification(0xa0, directoryName, NULL, 0);

					int dirEntrySize = 2 + fileAttributesSize + filenameSize;

					int overallEntrySize = 1 + BerEncoder_determineLengthSize(dirEntrySize) + dirEntrySize;

					int bufferSpaceLeft = maxBufSize - bufPos;

					if (overallEntrySize > bufferSpaceLeft) {
						*moreFollows = true;
						break;
					}

					bufPos = BerEncoder_encodeTL(0x30, dirEntrySize, buffer, bufPos); /* SEQUENCE (DirectoryEntry) */
					bufPos = encodeFileSpecification(0xa0, directoryName, buffer, bufPos); /* fileName */
					bufPos = encodeFileAttributes(0xa1, fileSize, gtString, buffer, bufPos); /* file attributes */

				}

            }


            fileName = FileSystem_readDirectory(directory, &isDirectory);
        }

        FileSystem_closeDirectory(directory);
    }

    directoryName[directoryNameLength] = 0;

    return bufPos;
}

static void
createFileDirectoryResponse(uint32_t invokeId, ByteBuffer* response, char* directoryName, char* continueAfterFileName)
{
    int maxSize = response->maxSize - 3; /* reserve space for moreFollows */
    uint8_t* buffer = response->buffer;

    bool moreFollows = false;

    int tempStartPos = 30; /* estimated header part with safety margin */
    int tempCurPos = tempStartPos;
    int tempEncoded = 0;

    if (continueAfterFileName != NULL) {
        if (strlen(continueAfterFileName) == 0)
            continueAfterFileName = NULL;
    }

    tempCurPos = addFileEntriesToResponse(buffer, tempCurPos, maxSize, directoryName, continueAfterFileName, &moreFollows);

	if (tempCurPos < 0) {

       if (DEBUG_MMS_SERVER)
            printf("MMS_SERVER: Error opening directory!\n");

       mmsServer_createConfirmedErrorPdu(invokeId, response, MMS_ERROR_FILE_FILE_NON_EXISTENT);

       return;
    }

    tempEncoded = tempCurPos - tempStartPos;

    uint32_t invokeIdSize = BerEncoder_UInt32determineEncodedSize((uint32_t) invokeId) + 2;

    uint32_t listOfDirectoryEntrySeqSize = tempEncoded;

    uint32_t listOfDirectoryEntrySize = 1 + BerEncoder_determineLengthSize(tempEncoded) + tempEncoded;

    uint32_t fileDirectoryResponseSize = 1 + BerEncoder_determineLengthSize(listOfDirectoryEntrySize)
            + listOfDirectoryEntrySize;

    if (moreFollows)
        fileDirectoryResponseSize += 3; /* for moreFollows */

    uint32_t confirmedResponsePDUSize = invokeIdSize + 2 + BerEncoder_determineLengthSize(fileDirectoryResponseSize)
            + fileDirectoryResponseSize;

    int bufPos = 0;

    bufPos = BerEncoder_encodeTL(0xa1, confirmedResponsePDUSize, buffer, bufPos);

    bufPos = BerEncoder_encodeTL(0x02, invokeIdSize - 2, buffer, bufPos);
    bufPos = BerEncoder_encodeUInt32((uint32_t) invokeId, buffer, bufPos);

    buffer[bufPos++] = 0xbf;
    bufPos = BerEncoder_encodeTL(0x4d, fileDirectoryResponseSize, buffer, bufPos);
    bufPos = BerEncoder_encodeTL(0xa0, listOfDirectoryEntrySize, buffer, bufPos);

    bufPos = BerEncoder_encodeTL(0x30, listOfDirectoryEntrySeqSize, buffer, bufPos); /* SEQUENCE OF (DirectoryEntry) */

    /* memmove is required instead of memcpy since memory areas overlap */
    memmove(buffer + bufPos, buffer + tempStartPos, tempEncoded);

    bufPos += tempEncoded;

    if (moreFollows)
        bufPos = BerEncoder_encodeBoolean(0x81, moreFollows, buffer, bufPos);

    response->size = bufPos;
}

void
mmsServer_handleFileRenameRequest(
        MmsServerConnection connection,
        uint8_t* buffer, int bufPos, int maxBufPos,
        uint32_t invokeId,
        ByteBuffer* response)
{
    char currentFileName[256] = "";
    char newFileName[256] = "";

    while (bufPos < maxBufPos) {
        uint8_t tag = buffer[bufPos++];
        int length;

        bufPos = BerDecoder_decodeLength(buffer, &length, bufPos, maxBufPos);

        if (bufPos < 0)  {
            mmsServer_writeMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_INVALID_PDU, response);
            return;
        }

        switch(tag) {
        case 0xa0: /* currentFilename */
            if (!parseFileName(currentFileName, buffer, &bufPos, bufPos + length, invokeId, response))
                return;

            if (DEBUG_MMS_SERVER)
                printf("MMS_SERVER: currentFileName: (%s)\n", currentFileName);

            break;

        case 0xa1: /* newFilename */
            if (!parseFileName(newFileName, buffer, &bufPos, bufPos + length, invokeId, response))
                return;

            if (DEBUG_MMS_SERVER)
                printf("MMS_SERVER: newFileName: (%s)\n", newFileName);

            break;
        default: /* ignore unknown tag */
            if (DEBUG_MMS_SERVER)
                printf("MMS_SERVER: unknown tag: (%02x)\n", tag);

            bufPos += length;
            break;
        }
    }

    if ((strlen(currentFileName) != 0) && (strlen(newFileName) != 0)) {
        if (FileSystem_renameFile(currentFileName, newFileName)){
            /* send positive response */
            createNullResponseExtendedTag(invokeId, response, 0x4b);
        }
        else
        {
            if (DEBUG_MMS_SERVER)
                printf("MMS_SERVER: rename file failed!\n");

            mmsServer_createConfirmedErrorPdu(invokeId, response, MMS_ERROR_FILE_OTHER);
        }
    }
    else
        mmsServer_writeMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_INVALID_PDU, response);

}

void
mmsServer_handleFileDirectoryRequest(
    MmsServerConnection connection,
    uint8_t* buffer, int bufPos, int maxBufPos,
    uint32_t invokeId,
    ByteBuffer* response)
{
    if (DEBUG_MMS_SERVER)
        printf("MMS_SERVER: handleFileDirectoryRequest bufPos:%i, maxBufPus:%i\n", bufPos, maxBufPos);

    char filename[256] = "";

    char continueAfterFileName[256];

    char* continueAfter = NULL;

    while (bufPos < maxBufPos) {
        uint8_t tag = buffer[bufPos++];
        int length;

        bufPos = BerDecoder_decodeLength(buffer, &length, bufPos, maxBufPos);

        if (bufPos < 0)  {
            mmsServer_writeMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_INVALID_PDU, response);
            return;
        }

        switch(tag) {
        case 0xa0: /* filename */
            if (!parseFileName(filename, buffer, &bufPos, bufPos + length, invokeId, response))
                return;

            /* check for wildcard character(*) */
            if ((strcmp(filename, "*") == 0) || (strcmp(filename, "/") == 0) || (strcmp(filename, "\\") == 0))
            	filename[0] = 0;

            break;

        case 0xa1: /* continue-after */
            if (!parseFileName(continueAfterFileName, buffer, &bufPos, bufPos + length, invokeId, response))
                return;

            continueAfter = continueAfterFileName;

            break;

        default: /* unrecognized parameter */
            if (DEBUG_MMS_SERVER)
                printf("MMS_SERVER: handleFileDirectoryRequest: unrecognized parameter\n");
            mmsServer_writeMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_INVALID_PDU, response);
            return;
        }


    }

    createFileDirectoryResponse(invokeId, response, filename, continueAfter);
}

#endif /* MMS_FILE_SERVICE == 1 */

