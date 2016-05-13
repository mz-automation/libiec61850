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

    bool hasNames = false;

    while (bufPos < maxBufPos) {
        uint8_t tag = requestBuffer[bufPos++];
        int length;

        bufPos = BerDecoder_decodeLength(requestBuffer, &length, bufPos, maxBufPos);

        if (bufPos < 0)  {
            mmsServer_writeMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_INVALID_PDU, response);
            return;
        }

        switch (tag) {

        case 0xa0:
            {

                uint8_t objectIdTag = requestBuffer[bufPos++];
                bufPos = BerDecoder_decodeLength(requestBuffer, &length, bufPos, maxBufPos);

                switch (objectIdTag) {
                case 0xa1: /* domain-specific */
                    printf("  domain-specific-log\n");

                    if (!parseStringWithMaxLength(domainId, 64, requestBuffer, &bufPos, bufPos + length, invokeId, response)) {
                        return;
                    }

                    if (!parseStringWithMaxLength(logName, 64, requestBuffer, &bufPos, bufPos + length, invokeId, response)) {
                        return;
                    }

                    printf("  domain: %s log: %s\n", domainId, logName);

                    break;


                default:
                    mmsServer_writeMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_UNRECOGNIZED_MODIFIER, response);
                    return;

                }
            }

            break;

        default:
            mmsServer_writeMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_INVALID_PDU, response);
            return;
        }

        bufPos += length;
    }
}

#endif /* (MMS_JOURNAL_SERVICE == 1) */
