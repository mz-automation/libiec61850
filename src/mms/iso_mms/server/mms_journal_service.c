/*
 *  mms_journal_service.c
 *
 *  Copyright 2016 Michael Zillgith
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
#include "mms_value_internal.h"

#if (MMS_JOURNAL_SERVICE == 1)

static bool
parseStringWithMaxLength(char* filename, int maxLength, uint8_t* buffer, int* bufPos, int maxBufPos , uint32_t invokeId, ByteBuffer* response)
{
    uint8_t tag = buffer[(*bufPos)++];
    int length;

    if (tag != 0x1a) {
      mmsServer_writeMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_INVALID_PDU, response);
      return false;
    }

    *bufPos = BerDecoder_decodeLength(buffer, &length, *bufPos, maxBufPos);

    if (*bufPos < 0)  {
      mmsServer_writeMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_INVALID_PDU, response);
      return false;
    }

    if (length > maxLength) {
      mmsServer_writeMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_REQUEST_INVALID_ARGUMENT, response);
      return false;
    }

    memcpy(filename, buffer + *bufPos, length);
    filename[length] = 0;
    *bufPos += length;

    return true;
}


void
mmsServer_handleReadJournalRequest(
        MmsServerConnection connection,
        uint8_t* requestBuffer,
        int bufPos, int maxBufPos,
        uint32_t invokeId,
        ByteBuffer* response)
{
    printf("READ-JOURNAL\n");

    char domainId[65];
    char logName[65];
    char entryIdBuf[64]; /* maximum size of entry id is 64 bytes! */

    MmsValue entrySpec;
    entrySpec.type = MMS_OCTET_STRING;
    entrySpec.value.octetString.buf = entryIdBuf;
    entrySpec.value.octetString.maxSize = 64;

    MmsValue rangeStart;
    MmsValue rangeStop;

    bool hasNames = false;
    bool hasRangeStartSpec = false;
    bool hasRangeStopSpec = false;
    bool hasTimeSpec = false;
    bool hasEntrySpec = false;

    while (bufPos < maxBufPos) {
        uint8_t tag = requestBuffer[bufPos++];
        int length;

        bufPos = BerDecoder_decodeLength(requestBuffer, &length, bufPos, maxBufPos);

        if (bufPos < 0)  {
            mmsServer_writeMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_INVALID_PDU, response);
            return;
        }

        switch (tag) {

        case 0xa0: /* journalName */
            {

                uint8_t objectIdTag = requestBuffer[bufPos++];
                bufPos = BerDecoder_decodeLength(requestBuffer, &length, bufPos, maxBufPos);

                switch (objectIdTag) {
                case 0xa1: /* domain-specific */
                    printf("domain-specific-log  \n");

                    if (!parseStringWithMaxLength(domainId, 64, requestBuffer, &bufPos, bufPos + length, invokeId, response)) {
                        return;
                    }

                    if (!parseStringWithMaxLength(logName, 64, requestBuffer, &bufPos, bufPos + length, invokeId, response)) {
                        return;
                    }

                    printf("  domain: %s log: %s\n", domainId, logName);

                    hasNames = true;

                    break;

                default:
                    mmsServer_writeMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_UNRECOGNIZED_MODIFIER, response);
                    return;

                }
            }

            break;

        case 0xa1: /* rangeStartSpecification */
            {
                uint8_t subTag = requestBuffer[bufPos++];
                bufPos = BerDecoder_decodeLength(requestBuffer, &length, bufPos, maxBufPos);

                if (subTag != 0x80) {
                    mmsServer_writeMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_UNRECOGNIZED_MODIFIER, response);
                    return;
                }


                if ((length == 4) || (length == 6)) {

                    rangeStart.type = MMS_BINARY_TIME;
                    rangeStart.value.binaryTime.size = length;

                    memcpy(rangeStart.value.binaryTime.buf, requestBuffer + bufPos, length);

                    char stringBuf[100];
                    MmsValue_printToBuffer(&rangeStart, stringBuf, 100);

                    printf("rangeStartSpec: %s\n", stringBuf);

                    hasRangeStartSpec = true;
                }
                else {
                    mmsServer_writeMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_REQUEST_INVALID_ARGUMENT, response);
                    return;
                }

                bufPos += length;
            }

            break;

        case 0xa2: /* rangeStopSpecification */
            {
                uint8_t subTag = requestBuffer[bufPos++];
                bufPos = BerDecoder_decodeLength(requestBuffer, &length, bufPos, maxBufPos);

                if (subTag != 0x80) {
                    mmsServer_writeMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_UNRECOGNIZED_MODIFIER, response);
                    return;
                }


                if ((length == 4) || (length == 6)) {
                    rangeStop.type = MMS_BINARY_TIME;
                    rangeStop.value.binaryTime.size = length;

                    memcpy(rangeStop.value.binaryTime.buf, requestBuffer + bufPos, length);

                    char stringBuf[100];
                    MmsValue_printToBuffer(&rangeStop, stringBuf, 100);

                    printf("rangeStopSpec: %s\n", stringBuf);

                    hasRangeStopSpec = true;
                }
                else {
                    mmsServer_writeMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_REQUEST_INVALID_ARGUMENT, response);
                    return;
                }

                bufPos += length;
            }

            break;

        case 0xa5: /* entryToStartAfter */
            {
                printf("entryToStartAfter\n");
                int maxSubBufPos = bufPos + length;

                while (bufPos < maxSubBufPos) {
                    uint8_t subTag = requestBuffer[bufPos++];
                    bufPos = BerDecoder_decodeLength(requestBuffer, &length, bufPos, maxBufPos);

                    switch (subTag) {
                    case 0x80: /* timeSpecification */

                        if ((length == 4) || (length == 6)) {
                            rangeStart.type = MMS_BINARY_TIME;
                            rangeStart.value.binaryTime.size = length;

                            memcpy(rangeStart.value.binaryTime.buf, requestBuffer + bufPos, length);

                            char stringBuf[100];
                            MmsValue_printToBuffer(&rangeStart, stringBuf, 100);

                            printf("timeSpecification: %s\n", stringBuf);

                            hasRangeStopSpec = true;
                        }
                        else {
                            mmsServer_writeMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_REQUEST_INVALID_ARGUMENT, response);
                            return;
                        }

                        break;

                    case 0x81: /* entrySpecification */

                        if (length <= entrySpec.value.octetString.maxSize) {
                            memcpy(entrySpec.value.octetString.buf, requestBuffer + bufPos, length);
                            entrySpec.value.octetString.size = length;

                            printf("EntrySpecification with size %i\n", length);

                            hasEntrySpec = true;
                        }
                        else {
                            mmsServer_writeMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_REQUEST_INVALID_ARGUMENT, response);
                            return;
                        }

                        break;

                    default:
                        mmsServer_writeMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_UNRECOGNIZED_MODIFIER, response);
                        return;
                    }

                    bufPos += length;
                }

            }
            break;

        default:
            mmsServer_writeMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_INVALID_PDU, response);
            return;
        }
    }

    //TODO check if required fields are present
}

#endif /* (MMS_JOURNAL_SERVICE == 1) */
