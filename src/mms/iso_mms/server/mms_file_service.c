/*
 *  mms_file_service.c
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
 */

#include "libiec61850_platform_includes.h"
#include "mms_server_internal.h"
#include "mms_client_internal.h"

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

static bool
getFileInfo(const char* basepath, char* filename, uint32_t* fileSize, uint64_t* lastModificationTimestamp)
{
#if (CONFIG_SET_FILESTORE_BASEPATH_AT_RUNTIME == 1)
    char extendedFileName[512];
#else
    char extendedFileName[sizeof(CONFIG_VIRTUAL_FILESTORE_BASEPATH) + 256];
#endif

    mmsMsg_createExtendedFilename(basepath, extendedFileName, filename);

    return FileSystem_getFileInfo(extendedFileName, fileSize, lastModificationTimestamp);
}

static FileHandle
openFile(const char* basepath, char* fileName, bool readWrite)
{
#if (CONFIG_SET_FILESTORE_BASEPATH_AT_RUNTIME == 1)
    char extendedFileName[512];
#else
    char extendedFileName[sizeof(CONFIG_VIRTUAL_FILESTORE_BASEPATH) + 256];
#endif

    mmsMsg_createExtendedFilename(basepath, extendedFileName, fileName);

    return FileSystem_openFile(extendedFileName, readWrite);
}

static DirectoryHandle
openDirectory(const char* basepath, char* directoryName)
{
#if (CONFIG_SET_FILESTORE_BASEPATH_AT_RUNTIME == 1)
    char extendedFileName[512];
#else
    char extendedFileName[sizeof(CONFIG_VIRTUAL_FILESTORE_BASEPATH) + 256];
#endif

    mmsMsg_createExtendedFilename(basepath, extendedFileName, directoryName);

    return FileSystem_openDirectory(extendedFileName);
}

static bool
renameFile(const char* basepath, char* oldFilename, char* newFilename) {

#if (CONFIG_SET_FILESTORE_BASEPATH_AT_RUNTIME == 1)
    char extendedOldFileName[512];
    char extendedNewFileName[512];
#else
    char extendedOldFileName[sizeof(CONFIG_VIRTUAL_FILESTORE_BASEPATH) + 256];
    char extendedNewFileName[sizeof(CONFIG_VIRTUAL_FILESTORE_BASEPATH) + 256];
#endif

    mmsMsg_createExtendedFilename(basepath, extendedOldFileName, oldFilename);
    mmsMsg_createExtendedFilename(basepath, extendedNewFileName, newFilename);

    return FileSystem_renameFile(extendedOldFileName, extendedNewFileName);
}

static bool
deleteFile(const char* basepath, char* fileName) {
#if (CONFIG_SET_FILESTORE_BASEPATH_AT_RUNTIME == 1)
    char extendedFileName[512];
#else
    char extendedFileName[sizeof(CONFIG_VIRTUAL_FILESTORE_BASEPATH) + 256];
#endif

    mmsMsg_createExtendedFilename(basepath, extendedFileName, fileName);

    return FileSystem_deleteFile(extendedFileName);
}

void
mmsMsg_createFileOpenResponse(const char* basepath, uint32_t invokeId, ByteBuffer* response,
        char* filename, MmsFileReadStateMachine* frsm)
{
    uint64_t msTime;

    getFileInfo(basepath, filename, &(frsm->fileSize), &msTime);

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
    bufPos = BerEncoder_encodeInt32(frsm->frsmId, buffer, bufPos);
    bufPos = encodeFileAttributes(0xa1, frsm->fileSize, gtString, buffer, bufPos);

    response->size = bufPos;
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
    if (bufPos == -1)
        goto exit_reject_invalid_pdu;

    if (length > 255) {
        mmsMsg_createMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_REQUEST_INVALID_ARGUMENT, response);
        return;
    }

    char filename[256];

    memcpy(filename, buffer + bufPos, length);
    filename[length] = 0;

    if (DEBUG_MMS_SERVER)
        printf("MMS_SERVER: mms_file_service.c: Delete file (%s)\n", filename);

    if (connection->server->fileAccessHandler != NULL) {
        MmsError access = connection->server->fileAccessHandler(connection->server->fileAccessHandlerParameter,
                            connection, MMS_FILE_ACCESS_TYPE_DELETE, filename, NULL);

        if (access != MMS_ERROR_NONE) {
            mmsMsg_createServiceErrorPdu(invokeId, response, access);
            return;
        }
    }

    if (!getFileInfo(MmsServerConnection_getFilesystemBasepath(connection), filename, NULL, NULL)) {
        if (DEBUG_MMS_SERVER)
            printf("MMS_SERVER: mms_file_service.c:  File (%s) not found\n", filename);

        mmsMsg_createServiceErrorPdu(invokeId, response, MMS_ERROR_FILE_FILE_NON_EXISTENT);
        return;
    }

    if (!deleteFile(MmsServerConnection_getFilesystemBasepath(connection), filename)) {
        if (DEBUG_MMS_SERVER)
            printf("MMS_SERVER: mms_file_service.c:  Delete file (%s) failed\n", filename);

        mmsMsg_createServiceErrorPdu(invokeId, response, MMS_ERROR_FILE_FILE_ACCESS_DENIED);
        return;
    }

    createNullResponseExtendedTag(invokeId, response, 0x4c);
    return;

exit_reject_invalid_pdu:
    mmsMsg_createMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_INVALID_PDU, response);
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

        if (connection->server->fileAccessHandler != NULL) {
            MmsError access = connection->server->fileAccessHandler(connection->server->fileAccessHandlerParameter,
                                connection, MMS_FILE_ACCESS_TYPE_OPEN, filename, NULL);

            if (access != MMS_ERROR_NONE) {
                mmsMsg_createServiceErrorPdu(invokeId, response, access);
                return;
            }
        }

        MmsFileReadStateMachine* frsm = getFreeFrsm(connection);

        if (frsm != NULL) {
            FileHandle fileHandle = openFile(MmsServerConnection_getFilesystemBasepath(connection), filename, false);

            if (fileHandle != NULL) {
                frsm->fileHandle = fileHandle;
                frsm->readPosition = filePosition;
                frsm->frsmId = getNextFrsmId(connection);

                mmsMsg_createFileOpenResponse(MmsServerConnection_getFilesystemBasepath(connection),
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

#if (MMS_OBTAIN_FILE_SERVICE == 1)


static void /* Confirmed service error (ServiceError) */
createServiceErrorObtainFileError(uint32_t invokeId, ByteBuffer* response,
        MmsError errorType, uint32_t value)
{
    uint8_t buffer[8];

    int size = BerEncoder_encodeUInt32WithTL(0x80, value, buffer, 0);

    mmsServer_createServiceErrorPduWithServiceSpecificInfo(invokeId, response, errorType,
            buffer, size);
}


static void
createObtainFileResponse(uint32_t invokeId, ByteBuffer* response)
{
    createNullResponseExtendedTag(invokeId, response, 0x2e);
}

void
mmsServer_fileUploadTask(MmsServer self, MmsObtainFileTask task)
{
    switch (task->state) {

        case MMS_FILE_UPLOAD_STATE_NOT_USED:
            break;

        case MMS_FILE_UPLOAD_STATE_FILE_OPEN_SENT:
            {
                if (Hal_getTimeInMs() > task->nextTimeout) {

                    if (DEBUG_MMS_SERVER)
                        printf("MMS_SERVER: file open timeout!\n");

                    task->state = MMS_FILE_UPLOAD_STATE_SEND_OBTAIN_FILE_ERROR_SOURCE;

                    if(task->fileHandle){
                        FileSystem_closeFile(task->fileHandle);
                        task->fileHandle = NULL;
                    }
                    deleteFile(MmsServer_getFilesystemBasepath(self), task->destinationFilename);
                }
            }
            break;

        case MMS_FILE_UPLOAD_STATE_SEND_FILE_READ:
            {
                IsoConnection_lock(task->connection->isoConnection);

                ByteBuffer* request = MmsServer_reserveTransmitBuffer(self);

                task->lastRequestInvokeId = MmsServerConnection_getNextRequestInvokeId(task->connection);

                mmsClient_createFileReadRequest(task->lastRequestInvokeId, request, task->frmsId);

                task->state = MMS_FILE_UPLOAD_STATE_FILE_READ_SENT;
                IsoConnection_sendMessage(task->connection->isoConnection, request);

                MmsServer_releaseTransmitBuffer(self);

                IsoConnection_unlock(task->connection->isoConnection);

                task->nextTimeout = Hal_getTimeInMs() + 2000; /* timeout 2000 ms */
            }

            break;

        case MMS_FILE_UPLOAD_STATE_FILE_READ_SENT:

            if (Hal_getTimeInMs() > task->nextTimeout) {

                if (DEBUG_MMS_SERVER)
                    printf("MMS_SERVER: file read timeout!\n");

                task->state = MMS_FILE_UPLOAD_STATE_SEND_OBTAIN_FILE_ERROR_SOURCE;

                if(task->fileHandle){
                    FileSystem_closeFile(task->fileHandle);
                    task->fileHandle = NULL;
                }
                deleteFile(MmsServer_getFilesystemBasepath(self), task->destinationFilename);
            }

            break;

        case MMS_FILE_UPLOAD_STATE_SEND_FILE_CLOSE:
            {
                IsoConnection_lock(task->connection->isoConnection);

                ByteBuffer* request = MmsServer_reserveTransmitBuffer(self);

                task->lastRequestInvokeId = MmsServerConnection_getNextRequestInvokeId(task->connection);

                mmsClient_createFileCloseRequest(task->lastRequestInvokeId, request, task->frmsId);

                task->state = MMS_FILE_UPLOAD_STATE_FILE_CLOSE_SENT;

                IsoConnection_sendMessage(task->connection->isoConnection, request);

                MmsServer_releaseTransmitBuffer(self);

                IsoConnection_unlock(task->connection->isoConnection);

                task->nextTimeout = Hal_getTimeInMs() + 2000; /* timeout 2000 ms */

            }
            break;

        case MMS_FILE_UPLOAD_STATE_FILE_CLOSE_SENT:

            if (Hal_getTimeInMs() > task->nextTimeout) {

                if (DEBUG_MMS_SERVER)
                    printf("MMS_SERVER: file close timeout!\n");

                task->state = MMS_FILE_UPLOAD_STATE_SEND_OBTAIN_FILE_ERROR_SOURCE;

                FileSystem_closeFile(task->fileHandle);
                task->fileHandle = NULL;
                deleteFile(MmsServer_getFilesystemBasepath(self), task->destinationFilename);
            }

            break;

        case MMS_FILE_UPLOAD_STATE_SEND_OBTAIN_FILE_ERROR_SOURCE:

            {
                /* send ObtainFileError */
                IsoConnection_lock(task->connection->isoConnection);

                ByteBuffer* response = MmsServer_reserveTransmitBuffer(self);

                createServiceErrorObtainFileError(task->obtainFileRequestInvokeId, response, MMS_ERROR_FILE_FILE_NON_EXISTENT, 0);

                IsoConnection_sendMessage(task->connection->isoConnection, response);

                MmsServer_releaseTransmitBuffer(self);

                IsoConnection_unlock(task->connection->isoConnection);

                if(task->fileHandle){
                    FileSystem_closeFile(task->fileHandle);
                    task->fileHandle = NULL;
                }
                
                deleteFile(MmsServer_getFilesystemBasepath(self), task->destinationFilename);

                if (DEBUG_MMS_SERVER)
                    printf("MMS_SERVER: ObtainFile service: failed to open file from client\n");

                task->state = MMS_FILE_UPLOAD_STATE_NOT_USED;
            }
            break;

        case MMS_FILE_UPLOAD_STATE_SEND_OBTAIN_FILE_ERROR_DESTINATION:
            {
                /* send ObtainFileError */
                IsoConnection_lock(task->connection->isoConnection);

                ByteBuffer* response = MmsServer_reserveTransmitBuffer(self);

                createServiceErrorObtainFileError(task->obtainFileRequestInvokeId, response, MMS_ERROR_FILE_OTHER, 1);

                IsoConnection_sendMessage(task->connection->isoConnection, response);

                MmsServer_releaseTransmitBuffer(self);

                IsoConnection_unlock(task->connection->isoConnection);

                if (task->fileHandle) {
                    FileSystem_closeFile(task->fileHandle);
                    task->fileHandle = NULL;

                    if (task->destinationFilename[0])
                        deleteFile(MmsServer_getFilesystemBasepath(self), task->destinationFilename);
                }

                if (DEBUG_MMS_SERVER)
                    printf("MMS_SERVER: ObtainFile service: failed to create local file\n");

                task->state = MMS_FILE_UPLOAD_STATE_NOT_USED;
            }

            break;

        case MMS_FILE_UPLOAD_STATE_SEND_OBTAIN_FILE_RESPONSE:
            {
                IsoConnection_lock(task->connection->isoConnection);

                ByteBuffer* response = MmsServer_reserveTransmitBuffer(self);

                createObtainFileResponse(task->obtainFileRequestInvokeId, response);

                task->state = MMS_FILE_UPLOAD_STATE_NOT_USED;

                IsoConnection_sendMessage(task->connection->isoConnection, response);

                MmsServer_releaseTransmitBuffer(self);

                IsoConnection_unlock(task->connection->isoConnection);

                if (self->getFileCompleteHandler)
                    self->getFileCompleteHandler(self->getFileCompleteHandlerParameter, task->connection, task->destinationFilename);
            }
            break;
        case MMS_FILE_UPLOAD_STATE_INTERRUPTED:
            {
                if (DEBUG_MMS_SERVER)
                    printf("MMS_SERVER: file service interrupted, due to client disconnection\n");

                if (task->fileHandle){
                    FileSystem_closeFile(task->fileHandle);
                    task->fileHandle = NULL;

                    if (task->destinationFilename[0])
                        deleteFile(MmsServer_getFilesystemBasepath(self), task->destinationFilename);
                }
                task->state = MMS_FILE_UPLOAD_STATE_NOT_USED;
            }
            break;
    }
}

#if (MMS_OBTAIN_FILE_SERVICE == 1)

void
mmsServerConnection_stopFileUploadTasks(MmsServerConnection self)
{
    MmsServer server = self->server;

    int i;

    for (i = 0; i < CONFIG_MMS_SERVER_MAX_GET_FILE_TASKS; i++) {

        if (server->fileUploadTasks[i].state != 0) {

            if (server->fileUploadTasks[i].connection == self) {

                Semaphore_wait(server->fileUploadTasks[i].taskLock);

                /* stop file upload task */
                server->fileUploadTasks[i].state = MMS_FILE_UPLOAD_STATE_INTERRUPTED;

                Semaphore_post(server->fileUploadTasks[i].taskLock);
            }

        }
    }
}

#endif /*(MMS_OBTAIN_FILE_SERVICE == 1) */
void
mmsServer_handleObtainFileRequest(
        MmsServerConnection connection,
        uint8_t* buffer, int bufPos, int maxBufPos,
        uint32_t invokeId,
        ByteBuffer* response)
{
    char sourceFilename[256];
    bool hasSourceFileName = false;

    char destinationFilename[256];
    bool hasDestinationFilename = false;

    while (bufPos < maxBufPos) {
        uint8_t tag = buffer[bufPos++];
        int length;

        bufPos = BerDecoder_decodeLength(buffer, &length, bufPos, maxBufPos);

        if (bufPos < 0) goto exit_reject_invalid_pdu;

        switch(tag) {

        case 0xa1: /* source filename */

            if (!mmsMsg_parseFileName(sourceFilename, buffer, &bufPos, bufPos + length, invokeId, response))
                return;

            hasSourceFileName = true;

            break;

        case 0xa2: /* destination filename */

            if (!mmsMsg_parseFileName(destinationFilename, buffer, &bufPos, bufPos + length, invokeId, response))
                return;

            hasDestinationFilename = true;

            break;

        case 0x00: /* indefinite length end tag -> ignore */
            break;

        default: /* unrecognized parameter */
            bufPos += length;
            goto exit_reject_invalid_pdu;
        }
    }

    if (hasSourceFileName && hasDestinationFilename) {

        /* Call user to check if access is allowed */
        if (connection->server->fileAccessHandler != NULL) {
            MmsError access = connection->server->fileAccessHandler(connection->server->fileAccessHandlerParameter,
                                connection, MMS_FILE_ACCESS_TYPE_OBTAIN, destinationFilename, sourceFilename);

            if (access != MMS_ERROR_NONE) {
                mmsMsg_createServiceErrorPdu(invokeId, response, access);
                return;
            }
        }

        /* call callback to check if access is allowed -- DEPRECATED */
        if (connection->server->obtainFileHandler)
            if (connection->server->obtainFileHandler(connection->server->obtainFileHandlerParameter, connection, sourceFilename, destinationFilename) == false)
                goto exit_access_denied;

        /*  check if destination file already exists. If exists return error message */

#if (CONFIG_SET_FILESTORE_BASEPATH_AT_RUNTIME == 1)
        char extendedFileName[512];
#else
        char extendedFileName[sizeof(CONFIG_VIRTUAL_FILESTORE_BASEPATH) + 256];
#endif
        mmsMsg_createExtendedFilename(MmsServerConnection_getFilesystemBasepath(connection),
                extendedFileName, destinationFilename);

        if (FileSystem_getFileInfo(extendedFileName, NULL, NULL)) {
            if (DEBUG_MMS_SERVER)
                printf("MMS_SERVER: obtainFile - file already exists on server\n");
            goto exit_file_already_exists;
        }

        if (DEBUG_MMS_SERVER)
            printf("MMS_SERVER: Start download file %s from client to local file %s...\n", sourceFilename, destinationFilename);

        MmsObtainFileTask task = MmsServer_getObtainFileTask(connection->server);

        if (task != NULL) {

            FileHandle fileHandle = openFile(MmsServerConnection_getFilesystemBasepath(connection),
                    destinationFilename, true);

            task->connection = connection;
            task->obtainFileRequestInvokeId = invokeId;

            if (fileHandle == NULL) {
                task->state = MMS_FILE_UPLOAD_STATE_SEND_OBTAIN_FILE_ERROR_DESTINATION;
            }
            else {
                /* send file open request */
                task->lastRequestInvokeId = MmsServerConnection_getNextRequestInvokeId(connection);
                task->fileHandle = fileHandle;

                strcpy(task->destinationFilename, destinationFilename);

                ByteBuffer* request = MmsServer_reserveTransmitBuffer(connection->server);

                mmsClient_createFileOpenRequest(task->lastRequestInvokeId, request, sourceFilename, 0);

                IsoConnection_sendMessage(task->connection->isoConnection, request);

                MmsServer_releaseTransmitBuffer(connection->server);

                task->nextTimeout = Hal_getTimeInMs() + 2000; /* timeout 2000 ms */

                task->state = MMS_FILE_UPLOAD_STATE_FILE_OPEN_SENT;
            }
        }
        else
            goto exit_unavailable;
    }
    else
        goto exit_invalid_parameter;

    return;

exit_invalid_parameter:
    mmsMsg_createMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_REQUEST_INVALID_ARGUMENT, response);
    return;

exit_access_denied:
    createServiceErrorObtainFileError(invokeId, response, MMS_ERROR_FILE_FILE_ACCESS_DENIED, 1);
    return;

exit_file_already_exists:
    createServiceErrorObtainFileError(invokeId, response, MMS_ERROR_FILE_DUPLICATE_FILENAME, 1);
    return;

exit_unavailable:
    mmsMsg_createServiceErrorPdu(invokeId, response, MMS_ERROR_ACCESS_TEMPORARILY_UNAVAILABLE);
    return;

exit_reject_invalid_pdu:
    mmsMsg_createMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_INVALID_PDU, response);
}

#endif /* (MMS_OBTAIN_FILE_SERVICE == 1) */

void
mmsMsg_createFileReadResponse(int maxPduSize, uint32_t invokeId,
        ByteBuffer* response,  MmsFileReadStateMachine* frsm)
{
     /* determine remaining bytes in file */
     uint32_t bytesLeft = frsm->fileSize - frsm->readPosition;

     uint32_t fileChunkSize = 0;

     uint32_t maxFileChunkSize = maxPduSize - 20;

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
    int32_t frsmId = BerDecoder_decodeInt32(buffer, maxBufPos - bufPos, bufPos);

    if (DEBUG_MMS_SERVER)
        printf("MMS_SERVER: mmsServer_handleFileReadRequest read request for frsmId: %i\n", frsmId);

    MmsFileReadStateMachine* frsm = getFrsm(connection, frsmId);

    if (frsm != NULL)
        mmsMsg_createFileReadResponse(connection->maxPduSize, invokeId, response, frsm);
    else
        mmsMsg_createServiceErrorPdu(invokeId, response, MMS_ERROR_FILE_OTHER);
}

void
mmsMsg_createFileCloseResponse(uint32_t invokeId, ByteBuffer* response)
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
    int32_t frsmId = BerDecoder_decodeInt32(buffer, maxBufPos - bufPos, bufPos);

    MmsFileReadStateMachine* frsm = getFrsm(connection, frsmId);

    if (frsm) {
        FileSystem_closeFile(frsm->fileHandle);
        frsm->fileHandle = NULL;
        frsm->frsmId = 0;

        mmsMsg_createFileCloseResponse(invokeId, response);
    }
    else {
        if (DEBUG_MMS_SERVER)
            printf("MMS_SERVER: Unused file ID %i\n", frsmId);

        mmsMsg_createServiceErrorPdu(invokeId, response, MMS_ERROR_FILE_OTHER);
    }
}



static int /* TODO remove redundancy - same as in client code! */
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
addFileEntriesToResponse(const char* basepath, uint8_t* buffer, int bufPos, int maxBufSize, char* directoryName, char** continueAfterFileName, bool* moreFollows)
{
	int directoryNameLength = strlen(directoryName);

    DirectoryHandle directory = openDirectory(basepath, directoryName);

    if (directory != NULL) {

        bool isDirectory;
        char* fileName = FileSystem_readDirectory(directory, &isDirectory);

        while (fileName != NULL) {
        	directoryName[directoryNameLength] = 0;

        	if (directoryNameLength > 0) {
        		if (directoryName[directoryNameLength - 1] != '/')
        			strcat(directoryName, "/");
        	}

        	strcat(directoryName, fileName);

            bufPos = addFileEntriesToResponse(basepath, buffer, bufPos, maxBufSize, directoryName, continueAfterFileName, moreFollows);

            if (*moreFollows == true)
                break;

            fileName = FileSystem_readDirectory(directory, &isDirectory);
        }

        FileSystem_closeDirectory(directory);
    }
    else {

        if (*continueAfterFileName != NULL) {
            if (strcmp(*continueAfterFileName, directoryName) == 0) {
                *continueAfterFileName = NULL;
            }
        }
        else {
            uint64_t msTime;

            uint32_t fileSize;

            if (getFileInfo(basepath, directoryName, &fileSize, &msTime)) {
                char gtString[30];

                Conversions_msTimeToGeneralizedTime(msTime, (uint8_t*) gtString);

                int fileAttributesSize = encodeFileAttributes(0xa1, fileSize, gtString, NULL, 0);

                int filenameSize = encodeFileSpecification(0xa0, directoryName, NULL, 0);

                int dirEntrySize = 2 + fileAttributesSize + filenameSize;

                int overallEntrySize = 1 + BerEncoder_determineLengthSize(dirEntrySize) + dirEntrySize;

                int bufferSpaceLeft = maxBufSize - bufPos;

                if (overallEntrySize > bufferSpaceLeft) {
                    *moreFollows = true;
                }
                else {

                    bufPos = BerEncoder_encodeTL(0x30, dirEntrySize, buffer, bufPos); /* SEQUENCE (DirectoryEntry) */
                    bufPos = encodeFileSpecification(0xa0, directoryName, buffer, bufPos); /* fileName */
                    bufPos = encodeFileAttributes(0xa1, fileSize, gtString, buffer, bufPos); /* file attributes */
                }
            }
            else
                bufPos = -1;

        }

    }

    directoryName[directoryNameLength] = 0;

    return bufPos;
}

static void
createFileDirectoryResponse(const char* basepath, uint32_t invokeId, ByteBuffer* response, int maxPduSize, char* directoryName, char* continueAfterFileName)
{
    int maxSize = maxPduSize - 3; /* reserve space for moreFollows */
    uint8_t* buffer = response->buffer;

    bool moreFollows = false;

    int tempStartPos = 30; /* estimated header part with safety margin */
    int tempCurPos = tempStartPos;
    int tempEncoded = 0;

    if (continueAfterFileName != NULL) {
        if (strlen(continueAfterFileName) == 0)
            continueAfterFileName = NULL;
    }

    tempCurPos = addFileEntriesToResponse(basepath, buffer, tempCurPos, maxSize, directoryName, &continueAfterFileName, &moreFollows);

	if (tempCurPos < 0) {

       if (DEBUG_MMS_SERVER)
            printf("MMS_SERVER: Error opening directory!\n");

       mmsMsg_createServiceErrorPdu(invokeId, response, MMS_ERROR_FILE_FILE_NON_EXISTENT);

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
    bufPos = BerEncoder_encodeUInt32(invokeId, buffer, bufPos);

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

        if (bufPos < 0) {
            mmsMsg_createMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_INVALID_PDU, response);
            return;
        }

        switch(tag) {
        case 0xa0: /* currentFilename */
            if (!mmsMsg_parseFileName(currentFileName, buffer, &bufPos, bufPos + length, invokeId, response))
                return;

            if (DEBUG_MMS_SERVER)
                printf("MMS_SERVER: currentFileName: (%s)\n", currentFileName);

            break;

        case 0xa1: /* newFilename */
            if (!mmsMsg_parseFileName(newFileName, buffer, &bufPos, bufPos + length, invokeId, response))
                return;

            if (DEBUG_MMS_SERVER)
                printf("MMS_SERVER: newFileName: (%s)\n", newFileName);

            break;
        case 0x00: /* indefinite length end tag -> ignore */
            break;
        default: /* ignore unknown tag */
            if (DEBUG_MMS_SERVER)
                printf("MMS_SERVER: unknown tag: (%02x)\n", tag);

            bufPos += length;
            break;
        }
    }

    if ((strlen(currentFileName) != 0) && (strlen(newFileName) != 0)) {

        /* Call user to check if access is allowed */
        if (connection->server->fileAccessHandler != NULL) {
            MmsError access = connection->server->fileAccessHandler(connection->server->fileAccessHandlerParameter,
                                connection, MMS_FILE_ACCESS_TYPE_RENAME, currentFileName, newFileName);

            if (access != MMS_ERROR_NONE) {
                mmsMsg_createServiceErrorPdu(invokeId, response, access);
                return;
            }
        }

        if (renameFile(MmsServerConnection_getFilesystemBasepath(connection), currentFileName, newFileName)){
            /* send positive response */
            createNullResponseExtendedTag(invokeId, response, 0x4b);
        }
        else
        {
            if (DEBUG_MMS_SERVER)
                printf("MMS_SERVER: rename file failed!\n");

            mmsMsg_createServiceErrorPdu(invokeId, response, MMS_ERROR_FILE_OTHER);
        }
    }
    else
        mmsMsg_createMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_INVALID_PDU, response);

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

        if (bufPos < 0) {
            mmsMsg_createMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_INVALID_PDU, response);
            return;
        }

        switch(tag) {
        case 0xa0: /* filename */
            if (!mmsMsg_parseFileName(filename, buffer, &bufPos, bufPos + length, invokeId, response))
                return;

            /* check for wildcard character(*) */
            if ((strcmp(filename, "*") == 0) || (strcmp(filename, "/") == 0) || (strcmp(filename, "\\") == 0))
            	filename[0] = 0;

            break;

        case 0xa1: /* continue-after */
            if (!mmsMsg_parseFileName(continueAfterFileName, buffer, &bufPos, bufPos + length, invokeId, response))
                return;

            continueAfter = continueAfterFileName;

            break;

        case 0x00: /* indefinite length end tag -> ignore */
            break;

        default: /* unrecognized parameter */
            if (DEBUG_MMS_SERVER)
                printf("MMS_SERVER: handleFileDirectoryRequest: unrecognized parameter\n");
            mmsMsg_createMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_INVALID_PDU, response);
            return;
        }


    }

    int maxPduSize = connection->maxPduSize;

    /* Call user to check if access is allowed */
    if (connection->server->fileAccessHandler != NULL) {
        MmsError access = connection->server->fileAccessHandler(connection->server->fileAccessHandlerParameter,
                            connection, MMS_FILE_ACCESS_TYPE_READ_DIRECTORY, filename, continueAfter);

        if (access != MMS_ERROR_NONE) {
            mmsMsg_createServiceErrorPdu(invokeId, response, access);
            return;
        }
    }

    createFileDirectoryResponse(MmsServerConnection_getFilesystemBasepath(connection),
            invokeId, response, maxPduSize, filename, continueAfter);
}

#endif /* MMS_FILE_SERVICE == 1 */

