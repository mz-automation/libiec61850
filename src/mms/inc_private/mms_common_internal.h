/*
 *  mms_common_internal.h
 *
 *  Copyright 2013, 2014, 2015 Michael Zillgith
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

#ifndef MMS_COMMON_INTERNAL_H_
#define MMS_COMMON_INTERNAL_H_

#include "mms_value.h"
#include "MmsPdu.h"
#include "conversions.h"
#include "byte_buffer.h"
#include "mms_server.h"

#if (MMS_FILE_SERVICE == 1)

#ifndef CONFIG_MMS_MAX_NUMBER_OF_OPEN_FILES_PER_CONNECTION
#define CONFIG_MMS_MAX_NUMBER_OF_OPEN_FILES_PER_CONNECTION 5
#endif

#include "hal_filesystem.h"

typedef struct {
        int32_t frsmId;
        uint32_t readPosition;
        uint32_t fileSize;
        FileHandle fileHandle;
} MmsFileReadStateMachine;

/* include for MmsFileReadHandler definition */
#include "mms_client_connection.h"

bool
mmsMsg_parseFileOpenResponse(uint8_t* buffer, int bufPos, int maxBufPos, int32_t* frsmId, uint32_t* fileSize, uint64_t* lastModified);

bool
mmsMsg_parseFileReadResponse(uint8_t* buffer, int bufPos, int maxBufPos, int32_t frsmId,  bool* moreFollows, MmsFileReadHandler handler, void* handlerParameter);

void
mmsMsg_createFileReadResponse(int maxPduSize, uint32_t invokeId, ByteBuffer* response,  MmsFileReadStateMachine* frsm);

void
mmsMsg_createFileCloseResponse(uint32_t invokeId, ByteBuffer* response);

void
mmsMsg_createFileOpenResponse(const char* basepath, uint32_t invokeId, ByteBuffer* response, char* fullPath, MmsFileReadStateMachine* frsm);

bool
mmsMsg_parseFileName(char* filename, uint8_t* buffer, int* bufPos, int maxBufPos , uint32_t invokeId, ByteBuffer* response);

void
mmsMsg_createExtendedFilename(const char* basepath, char* extendedFileName, char* fileName);

FileHandle
mmsMsg_openFile(const char* basepath, char* fileName, bool readWrite);

#endif /* (MMS_FILE_SERVICE == 1) */

typedef struct sMmsServiceError
{
    int errorClass;
    int errorCode;
} MmsServiceError;


void /* Confirmed service error (ServiceError) */
mmsMsg_createServiceErrorPdu(uint32_t invokeId, ByteBuffer* response, MmsError errorType);

void
mmsMsg_createMmsRejectPdu(uint32_t* invokeId, int reason, ByteBuffer* response);

int
mmsMsg_parseConfirmedErrorPDU(uint8_t* buffer, int bufPos, int maxBufPos, uint32_t* invokeId, MmsServiceError* serviceError);


MmsValue*
mmsMsg_parseDataElement(Data_t* dataElement);

Data_t*
mmsMsg_createBasicDataElement(MmsValue* value);

AccessResult_t**
mmsMsg_createAccessResultsList(MmsPdu_t* mmsPdu, int resultsCount);

char*
mmsMsg_createStringFromAsnIdentifier(Identifier_t identifier);

void
mmsMsg_copyAsn1IdentifierToStringBuffer(Identifier_t identifier, char* buffer, int bufSize);

void
mmsMsg_deleteAccessResultList(AccessResult_t** accessResult, int variableCount);

#endif /* MMS_COMMON_INTERNAL */

