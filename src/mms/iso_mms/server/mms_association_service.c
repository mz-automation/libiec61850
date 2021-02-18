/*
 *  mms_association_service.c
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

/**********************************************************************************************
 * MMS Server Capabilities
 *********************************************************************************************/

#define MMS_SERVICE_STATUS 0x80
#define MMS_SERVICE_GET_NAME_LIST 0x40
#define MMS_SERVICE_IDENTIFY 0x20
#define MMS_SERVICE_RENAME 0x10
#define MMS_SERVICE_READ 0x08
#define MMS_SERVICE_WRITE 0x04
#define MMS_SERVICE_GET_VARIABLE_ACCESS_ATTRIBUTES 0x02
#define MMS_SERVICE_DEFINE_NAMED_VARIABLE 0x01

#define MMS_SERVICE_DEFINE_SCATTERED_ACCESS 0x80
#define MMS_SERVICE_GET_SCATTERED_ACCESS_ATTRIBUTES 0x40
#define MMS_SERVICE_DELETE_VARIABLE_ACCESS 0x20
#define MMS_SERVICE_DEFINE_NAMED_VARIABLE_LIST 0x10
#define MMS_SERVICE_GET_NAMED_VARIABLE_LIST_ATTRIBUTES 0x08
#define MMS_SERVICE_DELETE_NAMED_VARIABLE_LIST 0x04
#define MMS_SERVICE_DEFINE_NAMED_TYPE 0x02
#define MMS_SERVICE_GET_NAMED_TYPE_ATTRIBUTES 0x01

#define MMS_SERVICE_OBTAIN_FILE 0x02

#define MMS_SERVICE_READ_JOURNAL 0x40

#define MMS_SERVICE_FILE_OPEN 0x80
#define MMS_SERVICE_FILE_READ 0x40
#define MMS_SERVICE_FILE_CLOSE 0x20
#define MMS_SERVICE_FILE_RENAME 0x10
#define MMS_SERVICE_FILE_DELETE 0x08
#define MMS_SERVICE_FILE_DIRECTORY 0x04
#define MMS_SERVICE_UNSOLICITED_STATUS 0x02
#define MMS_SERVICE_INFORMATION_REPORT 0x01

#define MMS_SERVICE_CONCLUDE 0x10
#define MMS_SERVICE_CANCEL 0x08

/* our supported parameter CBB: str1, str2, vnam, vlis, valt */
static uint8_t parameterCBB[] =
{
        0xf1,
        0x00
};

/**********************************************************************************************
 * MMS Initiate Service
 *********************************************************************************************/

static int
encodeInitResponseDetail(MmsServerConnection self, uint8_t* buffer, int bufPos, bool encode)
{
    int initResponseDetailSize = 14 + 5 + 3;

    if (encode == false)
        return initResponseDetailSize + 2;

    bufPos = BerEncoder_encodeTL(0xa4, initResponseDetailSize, buffer, bufPos);

    bufPos = BerEncoder_encodeUInt32WithTL(0x80, 1, buffer, bufPos); /* negotiated protocol version */

    self->negotiatedParameterCBC[0] = self->negotiatedParameterCBC[0] & parameterCBB[0];
    self->negotiatedParameterCBC[1] = self->negotiatedParameterCBC[1] & parameterCBB[1];

    bufPos = BerEncoder_encodeBitString(0x81, 11, self->negotiatedParameterCBC, buffer, bufPos);

#if (CONFIG_MMS_SERVER_CONFIG_SERVICES_AT_RUNTIME == 1)

    uint8_t servicesSupported[] =
    {
            0x00
    #if (MMS_STATUS_SERVICE == 1)
            | MMS_SERVICE_STATUS
    #endif
            | MMS_SERVICE_GET_NAME_LIST
    #if (MMS_IDENTIFY_SERVICE == 1)
            | MMS_SERVICE_IDENTIFY
    #endif
            | MMS_SERVICE_READ
            | MMS_SERVICE_WRITE
            | MMS_SERVICE_GET_VARIABLE_ACCESS_ATTRIBUTES
            ,
            0x00
    #if ((MMS_DATA_SET_SERVICE == 1) && (MMS_GET_DATA_SET_ATTRIBUTES == 1))
            | MMS_SERVICE_GET_NAMED_VARIABLE_LIST_ATTRIBUTES
    #endif
            ,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00
            | MMS_SERVICE_INFORMATION_REPORT
            ,
            0x00
            | MMS_SERVICE_CONCLUDE
            | MMS_SERVICE_CANCEL
    };


    if (self->server->fileServiceEnabled) {

#if (MMS_OBTAIN_FILE_SERVICE == 1)
        servicesSupported[5] |= MMS_SERVICE_OBTAIN_FILE;
#endif

#if (MMS_FILE_SERVICE == 1)
        servicesSupported[9] |= MMS_SERVICE_FILE_OPEN;
        servicesSupported[9] |= MMS_SERVICE_FILE_READ;
        servicesSupported[9] |= MMS_SERVICE_FILE_CLOSE;
#if (MMS_RENAME_FILE_SERVICE == 1)
        servicesSupported[9] |= MMS_SERVICE_FILE_RENAME;
#endif
#if (MMS_DELETE_FILE_SERVICE == 1)
        servicesSupported[9] |= MMS_SERVICE_FILE_DELETE;
#endif
        servicesSupported[9] |= MMS_SERVICE_FILE_DIRECTORY;
#endif /* (MMS_FILE_SERVICE == 1) */

    }

    if (self->server->dynamicVariableListServiceEnabled) {

#if ((MMS_DATA_SET_SERVICE == 1) && (MMS_DYNAMIC_DATA_SETS == 1))
        servicesSupported[1] |= MMS_SERVICE_DEFINE_NAMED_VARIABLE_LIST;
        servicesSupported[1] |= MMS_SERVICE_DELETE_NAMED_VARIABLE_LIST;
#endif

    }

    if (self->server->journalServiceEnabled) {
#if (MMS_JOURNAL_SERVICE == 1)
        servicesSupported[8] |= MMS_SERVICE_READ_JOURNAL;
#endif
    }

#else
    uint8_t servicesSupported[] =
    {
            0x00
    #if (MMS_STATUS_SERVICE == 1)
            | MMS_SERVICE_STATUS
    #endif
            | MMS_SERVICE_GET_NAME_LIST
    #if (MMS_IDENTIFY_SERVICE == 1)
            | MMS_SERVICE_IDENTIFY
    #endif
            | MMS_SERVICE_READ
            | MMS_SERVICE_WRITE
            | MMS_SERVICE_GET_VARIABLE_ACCESS_ATTRIBUTES
            ,
            0x00
    #if ((MMS_DATA_SET_SERVICE == 1) && (MMS_DYNAMIC_DATA_SETS == 1))
            | MMS_SERVICE_DEFINE_NAMED_VARIABLE_LIST
            | MMS_SERVICE_DELETE_NAMED_VARIABLE_LIST
    #endif
    #if ((MMS_DATA_SET_SERVICE == 1) && (MMS_GET_DATA_SET_ATTRIBUTES == 1))
            | MMS_SERVICE_GET_NAMED_VARIABLE_LIST_ATTRIBUTES
    #endif
            ,
            0x00,
            0x00,
            0x00,
            0x00
    #if (MMS_OBTAIN_FILE_SERVICE == 1)
            | MMS_SERVICE_OBTAIN_FILE
    #endif
            ,
            0x00,
            0x00,
            0x00
    #if (MMS_JOURNAL_SERVICE == 1)
            | MMS_SERVICE_READ_JOURNAL
    #endif
            ,
            0x00
    #if (MMS_FILE_SERVICE == 1)
            | MMS_SERVICE_FILE_OPEN
            | MMS_SERVICE_FILE_READ
            | MMS_SERVICE_FILE_CLOSE
            | MMS_SERVICE_FILE_RENAME
            | MMS_SERVICE_FILE_DELETE
            | MMS_SERVICE_FILE_DIRECTORY
    #endif
            | MMS_SERVICE_INFORMATION_REPORT
            ,
            0x00
            | MMS_SERVICE_CONCLUDE
            | MMS_SERVICE_CANCEL
    };
#endif /* (CONFIG_MMS_SERVER_CONFIG_SERVICES_AT_RUNTIME == 1) */

    bufPos = BerEncoder_encodeBitString(0x82, 85, servicesSupported, buffer, bufPos);

    return bufPos;
}


static int
createInitiateResponse(MmsServerConnection self, ByteBuffer* writeBuffer)
{
    uint8_t* buffer = writeBuffer->buffer;
    int bufPos = 0;

    int initiateResponseLength = 0;

    initiateResponseLength += 2 + BerEncoder_UInt32determineEncodedSize(self->maxPduSize);
    initiateResponseLength += 2 + BerEncoder_UInt32determineEncodedSize(self->maxServOutstandingCalling);
    initiateResponseLength += 2 + BerEncoder_UInt32determineEncodedSize(self->maxServOutstandingCalled);
    initiateResponseLength += 2 + BerEncoder_UInt32determineEncodedSize(self->dataStructureNestingLevel);

    initiateResponseLength += encodeInitResponseDetail(self, NULL, 0, false);

    /* Initiate response pdu */
    bufPos = BerEncoder_encodeTL(0xa9, initiateResponseLength, buffer, bufPos);

    bufPos = BerEncoder_encodeUInt32WithTL(0x80, self->maxPduSize, buffer, bufPos);

    bufPos = BerEncoder_encodeUInt32WithTL(0x81, self->maxServOutstandingCalling, buffer, bufPos);

    bufPos = BerEncoder_encodeUInt32WithTL(0x82, self->maxServOutstandingCalled, buffer, bufPos);

    bufPos = BerEncoder_encodeUInt32WithTL(0x83, self->dataStructureNestingLevel, buffer, bufPos);

    bufPos = encodeInitResponseDetail(self, buffer, bufPos, true);

    writeBuffer->size = bufPos;

    return bufPos;
}

static bool
parseInitRequestDetail(MmsServerConnection self, uint8_t* buffer, int bufPos, int maxBufPos)
{
    while (bufPos < maxBufPos) {
        uint8_t tag = buffer[bufPos++];
        int length;

        bufPos = BerDecoder_decodeLength(buffer, &length, bufPos, maxBufPos);

        if (bufPos < 0)  {
            /* TODO write initiate error PDU! */
            return false;
        }

        if (bufPos + length > maxBufPos) {
            if (DEBUG_MMS_SERVER)
                printf("MMS_SERVER: length field too long\n");

            return false;
        }

        switch(tag) {
        case 0x80: /* proposed-version-number */
            {
                uint32_t protocolVersion = BerDecoder_decodeUint32(buffer, length, bufPos);

                if (protocolVersion != 1) {
                    if (DEBUG_MMS_SERVER)
                        printf("MMS_SERVER: invalid protocol version %u\n", protocolVersion);

                    return false;
                }
            }

            break;

        case 0x81: /* proposed-parameter-CBC */

            if (length == 3) {
                self->negotiatedParameterCBC[0] = buffer[bufPos + 1];
                self->negotiatedParameterCBC[1] = buffer[bufPos + 2];

                if (DEBUG_MMS_SERVER)
                    printf("MMS_SERVER: requested parameter CBC: %02x %02x\n",
                            self->negotiatedParameterCBC[0],
                            self->negotiatedParameterCBC[1]);
            }
            else {
                if (DEBUG_MMS_SERVER)
                    printf("MMS_SERVER: unexpected parameter CBC length\n");
            }

            break;
        }

        bufPos += length;
    }

    return true;
}

static bool
parseInitiateRequestPdu(MmsServerConnection self, uint8_t* buffer, int bufPos, int maxBufPos)
{
    self->maxPduSize = CONFIG_MMS_MAXIMUM_PDU_SIZE;

    self->dataStructureNestingLevel =
                    DEFAULT_DATA_STRUCTURE_NESTING_LEVEL;

    self->maxServOutstandingCalled = DEFAULT_MAX_SERV_OUTSTANDING_CALLED;

    self->maxServOutstandingCalling = DEFAULT_MAX_SERV_OUTSTANDING_CALLING;

    self->negotiatedParameterCBC[0] = 0;
    self->negotiatedParameterCBC[1] = 0;

    while (bufPos < maxBufPos) {
        uint8_t tag = buffer[bufPos++];
        int length;

        bufPos = BerDecoder_decodeLength(buffer, &length, bufPos, maxBufPos);

        if (bufPos < 0)  {
            /* TODO write initiate error PDU! */
            return false;
        }

        if (bufPos + length > maxBufPos) {
            if (DEBUG_MMS_SERVER)
                printf("MMS_SERVER: length field too long\n");

            return false;
        }

        switch (tag) {
        case 0x80: /* local-detail-calling */
            self->maxPduSize = BerDecoder_decodeUint32(buffer, length, bufPos);

            if (self->maxPduSize > CONFIG_MMS_MAXIMUM_PDU_SIZE)
                self->maxPduSize = CONFIG_MMS_MAXIMUM_PDU_SIZE;

            break;

        case 0x81:  /* proposed-max-serv-outstanding-calling */
            self->maxServOutstandingCalling = BerDecoder_decodeUint32(buffer, length, bufPos);

            if (self->maxServOutstandingCalling > DEFAULT_MAX_SERV_OUTSTANDING_CALLING)
                self->maxServOutstandingCalling = DEFAULT_MAX_SERV_OUTSTANDING_CALLING;

            break;

        case 0x82:  /* proposed-max-serv-outstanding-called */
            self->maxServOutstandingCalled = BerDecoder_decodeUint32(buffer, length, bufPos);

            if (self->maxServOutstandingCalled > DEFAULT_MAX_SERV_OUTSTANDING_CALLED)
                self->maxServOutstandingCalled = DEFAULT_MAX_SERV_OUTSTANDING_CALLED;

            break;
        case 0x83: /* proposed-data-structure-nesting-level */
            self->dataStructureNestingLevel = BerDecoder_decodeUint32(buffer, length, bufPos);
            break;

        case 0xa4: /* mms-init-request-detail */

            if (parseInitRequestDetail(self, buffer, bufPos, bufPos + length) == false)
                return false;

            break;

        case 0x00: /* indefinite length end tag -> ignore */
            break;

        default:
        	break; /* Ignore unknown tags */
        }

        bufPos += length;
    }

    return true;
}

void
mmsServer_handleInitiateRequest (
        MmsServerConnection self,
        uint8_t* buffer, int bufPos, int maxBufPos,
        ByteBuffer* response)
{

    if (parseInitiateRequestPdu(self, buffer, bufPos, maxBufPos)) {
        createInitiateResponse(self, response);
    }
    else {
        /* send initiate error PDU */
        mmsMsg_createInitiateErrorPdu(response, 0);
    }

}

/**********************************************************************************************
 * MMS Conclude Service
 *********************************************************************************************/

void
mmsServer_writeConcludeResponsePdu(ByteBuffer* response)
{
    ByteBuffer_appendByte(response, 0x8c);
    ByteBuffer_appendByte(response, 0x00);
}
