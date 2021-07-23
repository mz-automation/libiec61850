/*
 *  goose_receiver.c
 *
 *  Copyright 2014-2017 Michael Zillgith
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
#include "goose_subscriber.h"
#include "hal_ethernet.h"
#include "hal_thread.h"

#include "ber_decode.h"

#include "mms_value.h"
#include "mms_value_internal.h"
#include "linked_list.h"

#include "goose_receiver.h"
#include "goose_receiver_internal.h"

#ifndef DEBUG_GOOSE_SUBSCRIBER
#define DEBUG_GOOSE_SUBSCRIBER 0
#endif

#define ETH_BUFFER_LENGTH 1518

#define ETH_P_GOOSE 0x88b8

struct sGooseReceiver
{
    bool running;
    bool stop;
    char* interfaceId;
    uint8_t* buffer;
    EthernetSocket ethSocket;
    LinkedList subscriberList;
#if (CONFIG_MMS_THREADLESS_STACK == 0)
    Thread thread;
#endif
};

GooseReceiver
GooseReceiver_createEx(uint8_t* buffer)
{
    GooseReceiver self = (GooseReceiver) GLOBAL_MALLOC(sizeof(struct sGooseReceiver));

    if (self != NULL) {
        self->running = false;
        self->stop = false;
        self->interfaceId = NULL;
        self->buffer = buffer;
        self->ethSocket = NULL;
        self->subscriberList = LinkedList_create();
#if (CONFIG_MMS_THREADLESS_STACK == 0)
        self->thread = NULL;
#endif
    }

    return self;
}

GooseReceiver
GooseReceiver_create()
{
    GooseReceiver self = GooseReceiver_createEx(NULL);

    if (self) {
        self->buffer = (uint8_t*) GLOBAL_MALLOC(ETH_BUFFER_LENGTH);
    }

    return self;
}

void
GooseReceiver_addSubscriber(GooseReceiver self, GooseSubscriber subscriber)
{
    LinkedList_add(self->subscriberList, (void*) subscriber);
}

void
GooseReceiver_removeSubscriber(GooseReceiver self, GooseSubscriber subscriber)
{
    LinkedList_remove(self->subscriberList, (void*) subscriber);
}

void
GooseReceiver_setInterfaceId(GooseReceiver self, const char* interfaceId)
{
    if (self->interfaceId != NULL)
        GLOBAL_FREEMEM(self->interfaceId);

    self->interfaceId = StringUtils_copyString(interfaceId);
}

const char*
GooseReceiver_getInterfaceId(GooseReceiver self)
{
    if (self->interfaceId)
        return self->interfaceId;
    else
        return CONFIG_ETHERNET_INTERFACE_ID;
}

static void
createNewStringFromBufferElement(MmsValue* value, uint8_t* bufferSrc, int elementLength)
{
    value->value.visibleString.buf = (char*) GLOBAL_MALLOC(elementLength + 1);
    memcpy(value->value.visibleString.buf, bufferSrc, elementLength);
    value->value.visibleString.buf[elementLength] = 0;
    value->value.visibleString.size = elementLength;
}

static GooseParseError
parseAllData(uint8_t* buffer, int allDataLength, MmsValue* dataSetValues)
{
    int bufPos = 0;
    int elementLength = 0;

    int elementIndex = 0;

    int maxIndex = MmsValue_getArraySize(dataSetValues) - 1;
    GooseParseError pe = GOOSE_PARSE_ERROR_NO_ERROR;
    uint8_t tag;

    while (bufPos < allDataLength) {
        tag = buffer[bufPos++];

        if (elementIndex > maxIndex) {
            pe = GOOSE_PARSE_ERROR_OVERFLOW;
            break; /* from while */
        }

        MmsValue* value = MmsValue_getElement(dataSetValues, elementIndex);

        bufPos = BerDecoder_decodeLength(buffer, &elementLength, bufPos, allDataLength);
        if (bufPos < 0) {
            pe = GOOSE_PARSE_ERROR_TAGDECODE;
            break; /* from while */
        }

        switch (tag)
        {
        case 0x80: /* reserved for access result */
            if (DEBUG_GOOSE_SUBSCRIBER)
              printf("GOOSE_SUBSCRIBER:    found reserved value (tag 0x80)!\n");
            break;

        case 0xa1: /* array */
            if (DEBUG_GOOSE_SUBSCRIBER)
                printf("GOOSE_SUBSCRIBER:    found array\n");
            if (MmsValue_getType(value) == MMS_ARRAY) {
                if (parseAllData(buffer + bufPos, elementLength, value) != GOOSE_PARSE_ERROR_NO_ERROR)
                    pe = GOOSE_PARSE_ERROR_SUBLEVEL;
            }
            else {
                pe = GOOSE_PARSE_ERROR_TYPE_MISMATCH;
            }
            break;

        case 0xa2: /* structure */
            if (DEBUG_GOOSE_SUBSCRIBER)
                printf("GOOSE_SUBSCRIBER:    found structure\n");
            if (MmsValue_getType(value) == MMS_STRUCTURE) {
                if (parseAllData(buffer + bufPos, elementLength, value) != GOOSE_PARSE_ERROR_NO_ERROR)
                    pe = GOOSE_PARSE_ERROR_SUBLEVEL;
            }
            else {
                pe = GOOSE_PARSE_ERROR_TYPE_MISMATCH;
            }
            break;

        case 0x83: /* boolean */
            if (DEBUG_GOOSE_SUBSCRIBER)
                printf("GOOSE_SUBSCRIBER:    found boolean\n");

            if (MmsValue_getType(value) == MMS_BOOLEAN) {
                MmsValue_setBoolean(value, BerDecoder_decodeBoolean(buffer, bufPos));
            }
            else {
                pe = GOOSE_PARSE_ERROR_TYPE_MISMATCH;
            }

            break;

        case 0x84: /* BIT STRING */
            if (MmsValue_getType(value) == MMS_BIT_STRING) {
                int padding = buffer[bufPos];
                int bitStringLength = (8 * (elementLength - 1)) - padding;
                if (bitStringLength == value->value.bitString.size) {
                    memcpy(value->value.bitString.buf, buffer + bufPos + 1,
                            elementLength - 1);
                }
                else {
                    pe = GOOSE_PARSE_ERROR_LENGTH_MISMATCH;
                }
            }
            else {
                pe = GOOSE_PARSE_ERROR_TYPE_MISMATCH;
            }
            break;

        case 0x85: /* integer */
            if (MmsValue_getType(value) == MMS_INTEGER) {
                if (elementLength <= value->value.integer->maxSize) {
                    value->value.integer->size = elementLength;
                    memcpy(value->value.integer->octets, buffer + bufPos, elementLength);
                }
                else {
                    pe = GOOSE_PARSE_ERROR_LENGTH_MISMATCH;
                }
            }
            else {
                pe = GOOSE_PARSE_ERROR_TYPE_MISMATCH;
            }
            break;

        case 0x86: /* unsigned integer */
            if (MmsValue_getType(value) == MMS_UNSIGNED) {
                if (elementLength <= value->value.integer->maxSize) {
                    value->value.integer->size = elementLength;
                    memcpy(value->value.integer->octets, buffer + bufPos, elementLength);
                }
                else {
                    pe = GOOSE_PARSE_ERROR_LENGTH_MISMATCH;
                }
            }
            else {
                pe = GOOSE_PARSE_ERROR_TYPE_MISMATCH;
            }
            break;

        case 0x87: /* Float */
            if (MmsValue_getType(value) == MMS_FLOAT) {
                if (elementLength == 9) {
                    MmsValue_setDouble(value, BerDecoder_decodeDouble(buffer, bufPos));
                }
                else if (elementLength == 5) {
                    MmsValue_setFloat(value, BerDecoder_decodeFloat(buffer, bufPos));
                }
                else {
                    pe = GOOSE_PARSE_ERROR_LENGTH_MISMATCH;
                }
            }
            else {
                pe = GOOSE_PARSE_ERROR_TYPE_MISMATCH;
            }
            break;

        case 0x89: /* octet string */
            if (MmsValue_getType(value) == MMS_OCTET_STRING) {
                if (elementLength <= value->value.octetString.maxSize) {
                    value->value.octetString.size = elementLength;
                    memcpy(value->value.octetString.buf, buffer + bufPos, elementLength);
                }
            }
            else {
                pe = GOOSE_PARSE_ERROR_TYPE_MISMATCH;
            }
            break;

        case 0x8a: /* visible string */
            if (MmsValue_getType(value) == MMS_VISIBLE_STRING) {

                if (value->value.visibleString.buf != NULL) {
                    if ((int32_t) value->value.visibleString.size >= elementLength) {
                        memcpy(value->value.visibleString.buf, buffer + bufPos, elementLength);
                        value->value.visibleString.buf[elementLength] = 0;
                    }
                    else {
                        GLOBAL_FREEMEM(value->value.visibleString.buf);

                        createNewStringFromBufferElement(value, buffer + bufPos, elementLength);
                    }
                }
                else
                    createNewStringFromBufferElement(value, buffer + bufPos, elementLength);

            }
            else {
                pe = GOOSE_PARSE_ERROR_TYPE_MISMATCH;
            }
            break;

        case 0x8c: /* binary time */
            if (MmsValue_getType(value) == MMS_BINARY_TIME) {
                if ((elementLength == 4) || (elementLength == 6)) {
                    memcpy(value->value.binaryTime.buf, buffer + bufPos, elementLength);
                }
            }
            else {
                pe = GOOSE_PARSE_ERROR_TYPE_MISMATCH;
            }
            break;

        case 0x91: /* Utctime */
            if (elementLength == 8) {
                if (MmsValue_getType(value) == MMS_UTC_TIME) {
                    MmsValue_setUtcTimeByBuffer(value, buffer + bufPos);
                }
                else {
                  pe = GOOSE_PARSE_ERROR_TYPE_MISMATCH;
                }
            }
            else {
              pe = GOOSE_PARSE_ERROR_LENGTH_MISMATCH;
            }
            break;

        default:
            pe = GOOSE_PARSE_ERROR_UNKNOWN_TAG;
            break;
        }

        if ( pe != GOOSE_PARSE_ERROR_NO_ERROR ) {
            break; /* from while */
        }

        bufPos += elementLength;

        elementIndex++;
    }

    if (elementIndex <= maxIndex) {
        pe = GOOSE_PARSE_ERROR_UNDERFLOW;
    }

    if (DEBUG_GOOSE_SUBSCRIBER) {
        switch ( pe ) {
            case GOOSE_PARSE_ERROR_UNKNOWN_TAG:
                printf("GOOSE_SUBSCRIBER: Found unkown tag %02x!\n", tag);
            break;
            case GOOSE_PARSE_ERROR_TAGDECODE:
                printf("GOOSE_SUBSCRIBER: Malformed message: failed to decode BER length tag!\n");
                break;
            case GOOSE_PARSE_ERROR_SUBLEVEL:
                /* already printed at sublevel */
                break;
            case GOOSE_PARSE_ERROR_OVERFLOW:
                printf("GOOSE_SUBSCRIBER: Malformed message: too many elements!\n");
                break;
            case GOOSE_PARSE_ERROR_UNDERFLOW:
                printf("GOOSE_SUBSCRIBER: Malformed message: too few elements!\n");
                break;
            case GOOSE_PARSE_ERROR_TYPE_MISMATCH:
                printf("GOOSE_SUBSCRIBER: Message contains value of wrong type!\n");
                break;
            case GOOSE_PARSE_ERROR_LENGTH_MISMATCH:
                printf("GOOSE_SUBSCRIBER: Message contains value of wrong length!\n");
                break;
            default:
                break;
        }
    }

    return pe;
}

static MmsValue*
parseAllDataUnknownValue(GooseSubscriber self, uint8_t* buffer, int allDataLength, bool isStructure)
{
    int bufPos = 0;
    int elementLength = 0;

    int elementIndex = 0;

    MmsValue* dataSetValues = NULL;

    while (bufPos < allDataLength) {
        uint8_t tag = buffer[bufPos++];

        bufPos = BerDecoder_decodeLength(buffer, &elementLength, bufPos, allDataLength);
        if (bufPos < 0) {
            if (DEBUG_GOOSE_SUBSCRIBER)
                printf("GOOSE_SUBSCRIBER: Malformed message: failed to decode BER length tag!\n");
            return 0;
        }

        switch (tag)
        {
        case 0x80: /* reserved for access result */
            break;
        case 0xa1: /* array */
            break;
        case 0xa2: /* structure */
            break;
        case 0x83: /* boolean */
            break;
        case 0x84: /* BIT STRING */
            break;
        case 0x85: /* integer */
            break;
        case 0x86: /* unsigned integer */
            break;
        case 0x87: /* Float */
            break;
        case 0x89: /* octet string */
            break;
        case 0x8a: /* visible string */
            break;
        case 0x8c: /* binary time */
            break;
        case 0x91: /* Utctime */
            break;
        default:
            if (DEBUG_GOOSE_SUBSCRIBER)
                printf("GOOSE_SUBSCRIBER:    found unknown tag %02x\n", tag);
            goto exit_with_error;
        }

        bufPos += elementLength;

        elementIndex++;
    }

    if (isStructure)
        dataSetValues = MmsValue_createEmptyStructure(elementIndex);
    else
        dataSetValues = MmsValue_createEmptyArray(elementIndex);

    elementIndex = 0;
    bufPos = 0;

    while (bufPos < allDataLength) {
        uint8_t tag = buffer[bufPos++];

        bufPos = BerDecoder_decodeLength(buffer, &elementLength, bufPos, allDataLength);
        if (bufPos < 0) {
            if (DEBUG_GOOSE_SUBSCRIBER)
                printf("GOOSE_SUBSCRIBER: Malformed message: failed to decode BER length tag!\n");
            return 0;
        }

        MmsValue* value = NULL;

        switch (tag)
        {
        case 0xa1: /* array */
            if (DEBUG_GOOSE_SUBSCRIBER)
                printf("GOOSE_SUBSCRIBER:    found array\n");

            value = parseAllDataUnknownValue(self, buffer + bufPos, elementLength, false);

            if (value == NULL)
                goto exit_with_error;

            break;

        case 0xa2: /* structure */
            if (DEBUG_GOOSE_SUBSCRIBER)
                printf("GOOSE_SUBSCRIBER:    found structure\n");

            value = parseAllDataUnknownValue(self, buffer + bufPos, elementLength, true);

            if (value == NULL)
                goto exit_with_error;

            break;

        case 0x83: /* boolean */
            if (DEBUG_GOOSE_SUBSCRIBER)
                printf("GOOSE_SUBSCRIBER:    found boolean\n");
            value = MmsValue_newBoolean(BerDecoder_decodeBoolean(buffer, bufPos));

            break;

        case 0x84: /* BIT STRING */
            {
                int padding = buffer[bufPos];
                int bitStringLength = (8 * (elementLength - 1)) - padding;
                value = MmsValue_newBitString(bitStringLength);
                memcpy(value->value.bitString.buf, buffer + bufPos + 1, elementLength - 1);

            }
            break;

        case 0x85: /* integer */
            value = MmsValue_newInteger(elementLength * 8);
            memcpy(value->value.integer->octets, buffer + bufPos, elementLength);
            value->value.integer->size = elementLength;
            break;

        case 0x86: /* unsigned integer */
            value = MmsValue_newUnsigned(elementLength * 8);
            memcpy(value->value.integer->octets, buffer + bufPos, elementLength);
            value->value.integer->size = elementLength;
            break;

        case 0x87: /* Float */
            if (elementLength == 9)
                value = MmsValue_newDouble(BerDecoder_decodeDouble(buffer, bufPos));
            else if (elementLength == 5)
                value = MmsValue_newFloat(BerDecoder_decodeFloat(buffer, bufPos));
            break;

        case 0x89: /* octet string */
            value = MmsValue_newOctetString(elementLength, elementLength);
            memcpy(value->value.octetString.buf, buffer + bufPos, elementLength);
            break;

        case 0x8a: /* visible string */
            value = MmsValue_newVisibleStringFromByteArray(buffer + bufPos, elementLength);
            break;

        case 0x8c: /* binary time */
            if (elementLength == 4)
                value = MmsValue_newBinaryTime(true);
            else if (elementLength == 6)
                value = MmsValue_newBinaryTime(false);

            if ((elementLength == 4) || (elementLength == 6))
                memcpy(value->value.binaryTime.buf, buffer + bufPos, elementLength);

            break;

        case 0x91: /* Utctime */
            if (elementLength == 8) {
                value = MmsValue_newUtcTime(0);
                MmsValue_setUtcTimeByBuffer(value, buffer + bufPos);
            }
            else
            if (DEBUG_GOOSE_SUBSCRIBER)
                printf("GOOSE_SUBSCRIBER:      UTCTime element is of wrong size!\n");
            break;

        default:
            if (DEBUG_GOOSE_SUBSCRIBER)
                printf("GOOSE_SUBSCRIBER:    found unkown tag %02x\n", tag);
            goto exit_with_error;
        }

        bufPos += elementLength;

        if (value != NULL) {
            MmsValue_setElement(dataSetValues, elementIndex, value);
            elementIndex++;
        }
    }

    self->dataSetValuesSelfAllocated = true;

    return dataSetValues;

    exit_with_error:

    if (dataSetValues != NULL)
        MmsValue_delete(dataSetValues);

    return NULL;
}

static int
parseGoosePayload(GooseReceiver self, uint8_t* buffer, int apduLength)
{
    int bufPos = 0;
    uint32_t timeAllowedToLive = 0;
    uint32_t stNum = 0;
    uint32_t sqNum = 0;
    uint32_t confRev = 0;
    bool simulation = false;
    bool ndsCom = false;
    GooseSubscriber matchingSubscriber = NULL;
    uint8_t* timestampBufPos = NULL;
    uint8_t* dataSetBufferAddress = NULL;
    int dataSetBufferLength = 0;

    uint32_t numberOfDatSetEntries = 0;

    if (buffer[bufPos++] == 0x61) {
        int gooseLength;
        bufPos = BerDecoder_decodeLength(buffer, &gooseLength, bufPos, apduLength);
        if (bufPos < 0) {
            if (DEBUG_GOOSE_SUBSCRIBER)
                printf("GOOSE_SUBSCRIBER: Malformed message: failed to decode BER length tag!\n");
            return 0;
        }

        int gooseEnd = bufPos + gooseLength;

        while (bufPos < gooseEnd) {
            int elementLength;

            uint8_t tag = buffer[bufPos++];
            bufPos = BerDecoder_decodeLength(buffer, &elementLength, bufPos, apduLength);
            if (bufPos < 0) {
                if (DEBUG_GOOSE_SUBSCRIBER)
                    printf("GOOSE_SUBSCRIBER: Malformed message: failed to decode BER length tag!\n");
                return 0;
            }

            if (bufPos == -1)
                goto exit_with_fault;

            switch (tag)
            {
            case 0x80: /* gocbRef */
                if (DEBUG_GOOSE_SUBSCRIBER)
                    printf("GOOSE_SUBSCRIBER:   Found gocbRef\n");

                {
                    LinkedList element = LinkedList_getNext(self->subscriberList);

                    while (element != NULL) {
                        GooseSubscriber subscriber = (GooseSubscriber) LinkedList_getData(element);

                        if (subscriber->isObserver)
                        {
                            if (elementLength > 129) {
                                if (DEBUG_GOOSE_SUBSCRIBER)
                                    printf("GOOSE_SUBSCRIBER:   gocbRef too long!\n");
                            }
                            else {
                                memcpy(subscriber->goCBRef, buffer + bufPos, elementLength);
                                subscriber->goCBRef[elementLength] = 0;
                            }

                            matchingSubscriber = subscriber;
                            break;
                        }
                        else if (subscriber->goCBRefLen == elementLength) {
                            if (memcmp(subscriber->goCBRef, buffer + bufPos, elementLength) == 0) {
                                if (DEBUG_GOOSE_SUBSCRIBER)
                                    printf("GOOSE_SUBSCRIBER:   gocbRef is matching!\n");
                                matchingSubscriber = subscriber;
                                break;
                            }
                        }

                        element = LinkedList_getNext(element);
                    }

                    if (matchingSubscriber == NULL)
                        return 0;
                }

                break;

            case 0x81: /* timeAllowedToLive */

                timeAllowedToLive = BerDecoder_decodeUint32(buffer, elementLength, bufPos);

                if (DEBUG_GOOSE_SUBSCRIBER)
                    printf("GOOSE_SUBSCRIBER:   Found timeAllowedToLive %u\n", timeAllowedToLive);

                break;

            case 0x82: /* DatSet */
                if (DEBUG_GOOSE_SUBSCRIBER)
                    printf("GOOSE_SUBSCRIBER:   Found dataSet\n");
                {
                    if (matchingSubscriber) {
                        if (elementLength > 129) {
                            if (DEBUG_GOOSE_SUBSCRIBER)
                                printf("GOOSE_SUBSCRIBER:   datSet too long!\n");
                        }
                        else {
                            memcpy(matchingSubscriber->datSet, buffer + bufPos, elementLength);
                            matchingSubscriber->datSet[elementLength] = 0;
                        }
                    }
                }
                break;

            case 0x83: /* GoID **/
                if (DEBUG_GOOSE_SUBSCRIBER)
                    printf("GOOSE_SUBSCRIBER:   Found goId\n");
                {
                    if (matchingSubscriber) {
                        if (elementLength > 129) {
                            if (DEBUG_GOOSE_SUBSCRIBER)
                                printf("GOOSE_SUBSCRIBER:   goId too long!\n");
                        }
                        else {
                            memcpy(matchingSubscriber->goId, buffer + bufPos, elementLength);
                            matchingSubscriber->goId[elementLength] = 0;
                        }
                    }
                }
                break;

            case 0x84:
                timestampBufPos = buffer + bufPos;
                if (DEBUG_GOOSE_SUBSCRIBER)
                    printf("GOOSE_SUBSCRIBER:   Found timestamp\n");
                break;

            case 0x85:
                stNum = BerDecoder_decodeUint32(buffer, elementLength, bufPos);
                if (DEBUG_GOOSE_SUBSCRIBER)
                    printf("GOOSE_SUBSCRIBER:   Found stNum: %u\n", stNum);
                break;

            case 0x86:
                sqNum = BerDecoder_decodeUint32(buffer, elementLength, bufPos);
                if (DEBUG_GOOSE_SUBSCRIBER)
                    printf("GOOSE_SUBSCRIBER:   Found sqNum: %u\n", sqNum);
                break;

            case 0x87:
                simulation = BerDecoder_decodeBoolean(buffer, bufPos);
                if (DEBUG_GOOSE_SUBSCRIBER)
                    printf("GOOSE_SUBSCRIBER:   Found simulation: %i\n", simulation);
                break;

            case 0x88:
                confRev = BerDecoder_decodeUint32(buffer, elementLength, bufPos);
                if (DEBUG_GOOSE_SUBSCRIBER)
                    printf("GOOSE_SUBSCRIBER:   Found confRev: %u\n", confRev);
                break;

            case 0x89:
                ndsCom = BerDecoder_decodeBoolean(buffer, bufPos);
                if (DEBUG_GOOSE_SUBSCRIBER)
                    printf("GOOSE_SUBSCRIBER:   Found ndsCom: %i\n", ndsCom);
                break;

            case 0x8a:
                numberOfDatSetEntries = BerDecoder_decodeUint32(buffer, elementLength, bufPos);
                if (DEBUG_GOOSE_SUBSCRIBER)
                    printf("GOOSE_SUBSCRIBER:   Found number of entries: %u\n", numberOfDatSetEntries);
                break;

            case 0xab:
                if (DEBUG_GOOSE_SUBSCRIBER)
                    printf("GOOSE_SUBSCRIBER:   Found all data with length: %i\n", elementLength);
                dataSetBufferAddress = buffer + bufPos;
                dataSetBufferLength = elementLength;
                break;

            default:
                if (DEBUG_GOOSE_SUBSCRIBER)
                    printf("GOOSE_SUBSCRIBER:   Unknown tag %02x\n", tag);
                break;
            }

            bufPos += elementLength;
        }

        if (matchingSubscriber != NULL) {

            matchingSubscriber->timeAllowedToLive = timeAllowedToLive;
            matchingSubscriber->ndsCom = ndsCom;
            matchingSubscriber->simulation = simulation;

            if (matchingSubscriber->dataSetValuesSelfAllocated) {
                /* when confRev changed replaced old data set */
                if ((matchingSubscriber->dataSetValues != NULL) && (matchingSubscriber->confRev != confRev)) {
                    MmsValue_delete(matchingSubscriber->dataSetValues);
                    matchingSubscriber->dataSetValues = NULL;
                }
            }

            matchingSubscriber->confRev = confRev;

            if (timestampBufPos)
                MmsValue_setUtcTimeByBuffer(matchingSubscriber->timestamp, timestampBufPos);
            else {
                if (DEBUG_GOOSE_SUBSCRIBER)
                    printf("GOOSE_SUBSCRIBER: GOOSE message has no time stamp\n");

                MmsValue_setUtcTime(matchingSubscriber->timestamp, 0);
            }
            
            if (matchingSubscriber->isObserver && matchingSubscriber->dataSetValues != NULL) {
                MmsValue_delete(matchingSubscriber->dataSetValues);
                matchingSubscriber->dataSetValues = NULL;
            }

            bool isValid = true;

            if (matchingSubscriber->dataSetValues == NULL)
                matchingSubscriber->dataSetValues = parseAllDataUnknownValue(matchingSubscriber, dataSetBufferAddress, dataSetBufferLength, false);
            else {
                GooseParseError parseError = parseAllData(dataSetBufferAddress, dataSetBufferLength, matchingSubscriber->dataSetValues);

                if (parseError != GOOSE_PARSE_ERROR_NO_ERROR) {
                    isValid = false;
                }

                matchingSubscriber->parseError = parseError;
            }

            if (matchingSubscriber->stNum == stNum) {
                if (matchingSubscriber->sqNum >= sqNum) {
                    isValid = false;
                }
            }

            matchingSubscriber->stateValid = isValid;

            matchingSubscriber->stNum = stNum;
            matchingSubscriber->sqNum = sqNum;

            matchingSubscriber->invalidityTime = Hal_getTimeInMs() + timeAllowedToLive;

            if (matchingSubscriber->listener != NULL)
                matchingSubscriber->listener(matchingSubscriber, matchingSubscriber->listenerParameter);

            return 1;
        }

        return 0;
    }

exit_with_fault:
    if (DEBUG_GOOSE_SUBSCRIBER)
        printf("GOOSE_SUBSCRIBER: Invalid goose payload\n");
    return -1;
}

static void
parseGooseMessage(GooseReceiver self, uint8_t* buffer, int numbytes)
{
    int bufPos;
    bool subscriberFound = false;

    if (numbytes < 22)
        return;

    /* skip ethernet addresses */
    bufPos = 12;
    int headerLength = 14;

    uint8_t priority = 0;
    uint16_t vlanId = 0;
    bool vlanSet = false;
    /* check for VLAN tag */
    if ((buffer[bufPos] == 0x81) && (buffer[bufPos + 1] == 0x00)) {
        priority = buffer[bufPos + 2] & 0xF8 >> 5;
        vlanId = ((buffer[bufPos + 2] & 0x07) << 8) + buffer[bufPos + 3];
        vlanSet = true;
        bufPos += 4; /* skip VLAN tag */
        headerLength += 4;
    }

    /* check for GOOSE Ethertype */
    if (buffer[bufPos++] != 0x88)
        return;
    if (buffer[bufPos++] != 0xb8)
        return;
    
    uint8_t srcMac[6];
    memcpy(srcMac,&buffer[6],6);

    uint8_t dstMac[6];
    memcpy(dstMac,buffer,6);

    uint16_t appId;

    appId = buffer[bufPos++] * 0x100;
    appId += buffer[bufPos++];

    uint16_t length;

    length = buffer[bufPos++] * 0x100;
    length += buffer[bufPos++];

    /* skip reserved fields */
    bufPos += 4;

    int apduLength = length - 8;

    if (numbytes < length + headerLength) {
        if (DEBUG_GOOSE_SUBSCRIBER)
            printf("GOOSE_SUBSCRIBER: Invalid PDU size\n");
        return;
    }

    if (DEBUG_GOOSE_SUBSCRIBER) {
        printf("GOOSE_SUBSCRIBER: GOOSE message:\nGOOSE_SUBSCRIBER: ----------------\n");
        printf("GOOSE_SUBSCRIBER:   DST-MAC: %02x:%02x:%02x:%02x:%02X:%02X\n",
               dstMac[0], dstMac[1], dstMac[2], dstMac[3], dstMac[4], dstMac[5]);
        printf("GOOSE_SUBSCRIBER:   APPID: %u\n", appId);
        printf("GOOSE_SUBSCRIBER:   LENGTH: %u\n", length);
        printf("GOOSE_SUBSCRIBER:   APDU length: %i\n", apduLength);
    }

    /* check if there is an interested subscriber */
    LinkedList element = LinkedList_getNext(self->subscriberList);

    while (element != NULL) {
        GooseSubscriber subscriber = (GooseSubscriber) LinkedList_getData(element);
        
        if (subscriber->isObserver)
        {
            subscriber->appId = appId;
            memcpy(subscriber->srcMac, srcMac,6);
            memcpy(subscriber->dstMac, dstMac, 6);
            subscriberFound = true;
            subscriber->vlanSet = vlanSet;
            subscriber->vlanId = vlanId;
            subscriber->vlanPrio = priority;
            break;
        }

        if (((subscriber->appId == -1) || (subscriber->appId == appId)) &&
                (!subscriber->dstMacSet || (memcmp(subscriber->dstMac, dstMac,6) == 0))) {
            subscriberFound = true;
            break;
        }

        element = LinkedList_getNext(element);
    }

    if (subscriberFound)
        parseGoosePayload(self, buffer + bufPos, apduLength);
    else {
        if (DEBUG_GOOSE_SUBSCRIBER)
            printf("GOOSE_SUBSCRIBER: GOOSE message ignored due to unknown DST-MAC or APPID value\n");
    }
}

#if (CONFIG_MMS_THREADLESS_STACK == 0)
static void*
gooseReceiverLoop(void *threadParameter)
{
    GooseReceiver self = (GooseReceiver) threadParameter;
    EthernetHandleSet handleSet = EthernetHandleSet_new();
    EthernetHandleSet_addSocket(handleSet, self->ethSocket);

    if (self->running) {

        while (self->running) {
            switch (EthernetHandleSet_waitReady(handleSet, 100))
            {
            case -1:
                if (DEBUG_GOOSE_SUBSCRIBER)
                    printf("GOOSE_SUBSCRIBER: EhtnernetHandleSet_waitReady() failure\n");
                break;
            case 0:
                break;
            default:
                GooseReceiver_tick(self);
            }
            if (self->stop)
                break;
        }

        GooseReceiver_stopThreadless(self);
    }

    EthernetHandleSet_destroy(handleSet);

    return NULL;
}
#endif

/* start GOOSE receiver in a separate thread */
void
GooseReceiver_start(GooseReceiver self)
{
#if (CONFIG_MMS_THREADLESS_STACK == 0)
    if (GooseReceiver_startThreadless(self)) {
        self->thread = Thread_create((ThreadExecutionFunction) gooseReceiverLoop, (void*) self, false);

        if (self->thread != NULL) {
            if (DEBUG_GOOSE_SUBSCRIBER)
                printf("GOOSE_SUBSCRIBER: GOOSE receiver started for interface %s\n", self->interfaceId);

            Thread_start(self->thread);
        }
        else {
            if (DEBUG_GOOSE_SUBSCRIBER)
                printf("GOOSE_SUBSCRIBER: Starting GOOSE receiver failed for interface %s\n", self->interfaceId);
        }
    }
#endif
}

bool
GooseReceiver_isRunning(GooseReceiver self)
{
    return self->running;
}

void
GooseReceiver_stop(GooseReceiver self)
{
#if (CONFIG_MMS_THREADLESS_STACK == 0)
    self->stop = true;
    self->running = false;

    if (self->thread)
        Thread_destroy(self->thread);

    self->stop = false;
#endif
}

void
GooseReceiver_destroy(GooseReceiver self)
{
#if (CONFIG_MMS_THREADLESS_STACK == 0)
    if ((self->thread != NULL) && (GooseReceiver_isRunning(self)))
        GooseReceiver_stop(self);
#endif

    if (self->interfaceId != NULL)
        GLOBAL_FREEMEM(self->interfaceId);

    LinkedList_destroyDeep(self->subscriberList,
            (LinkedListValueDeleteFunction) GooseSubscriber_destroy);

    GLOBAL_FREEMEM(self->buffer);
    GLOBAL_FREEMEM(self);
}

/***************************************
 * Functions for non-threaded operation
 ***************************************/
EthernetSocket
GooseReceiver_startThreadless(GooseReceiver self)
{
    if (self->interfaceId == NULL)
        self->ethSocket = Ethernet_createSocket(CONFIG_ETHERNET_INTERFACE_ID, NULL);
    else
        self->ethSocket = Ethernet_createSocket(self->interfaceId, NULL);

    if (self->ethSocket != NULL) {
        Ethernet_setProtocolFilter(self->ethSocket, ETH_P_GOOSE);
        self->running = true;
    }
    else
        self->running = false;

    return self->ethSocket;
}

void
GooseReceiver_stopThreadless(GooseReceiver self)
{
    if (self->ethSocket)
        Ethernet_destroySocket(self->ethSocket);

    self->running = false;
}

/* call after reception of ethernet frame */
bool
GooseReceiver_tick(GooseReceiver self)
{
    int packetSize = Ethernet_receivePacket(self->ethSocket, self->buffer, ETH_BUFFER_LENGTH);

    if (packetSize > 0) {
        parseGooseMessage(self, self->buffer, packetSize);
        return true;
    }
    else
        return false;
}

void
GooseReceiver_handleMessage(GooseReceiver self, uint8_t* buffer, int size)
{
    parseGooseMessage(self, buffer, size);
}
