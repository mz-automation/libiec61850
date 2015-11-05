/*
 *  goose_receiver.c
 *
 *  Copyright 2014, 2015 Michael Zillgith
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

struct sGooseReceiver {
    bool running;
    bool stopped;
    char* interfaceId;
    uint8_t* buffer;
    EthernetSocket ethSocket;
    LinkedList subscriberList;
};


GooseReceiver
GooseReceiver_create()
{
    GooseReceiver self = (GooseReceiver) GLOBAL_MALLOC(sizeof(struct sGooseReceiver));

    if (self != NULL) {
        self->running = false;
        self->interfaceId = NULL;
        self->buffer = (uint8_t*) GLOBAL_MALLOC(ETH_BUFFER_LENGTH);
        self->ethSocket = NULL;
        self->subscriberList = LinkedList_create();
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

    self->interfaceId = copyString(interfaceId);
}

static void
createNewStringFromBufferElement(MmsValue* value, uint8_t* bufferSrc, int elementLength)
{
    value->value.visibleString.buf = (char*) GLOBAL_MALLOC(elementLength + 1);
    memcpy(value->value.visibleString.buf, bufferSrc, elementLength);
    value->value.visibleString.buf[elementLength] = 0;
    value->value.visibleString.size = elementLength;
}

static int
parseAllData(uint8_t* buffer, int allDataLength, MmsValue* dataSetValues)
{
    int bufPos = 0;
    int elementLength = 0;

    int elementIndex = 0;

    int maxIndex = MmsValue_getArraySize(dataSetValues) - 1;

    while (bufPos < allDataLength) {
        uint8_t tag = buffer[bufPos++];

        if (elementIndex > maxIndex) {
            if (DEBUG_GOOSE_SUBSCRIBER) printf("GOOSE_SUBSCRIBER: Malformed message: too much elements!\n");
            return 0;
        }

        MmsValue* value = MmsValue_getElement(dataSetValues, elementIndex);

        bufPos = BerDecoder_decodeLength(buffer, &elementLength, bufPos, allDataLength);

        if (bufPos + elementLength > allDataLength) {
            if (DEBUG_GOOSE_SUBSCRIBER) printf("GOOSE_SUBSCRIBER: Malformed message: sub element is too large!\n");
            return 0;
        }

        switch (tag) {
        case 0x80: /* reserved for access result */
            printf("GOOSE_SUBSCRIBER:    found reserved value (tag 0x80)!\n");
            break;
        case 0xa1: /* array */
            if (DEBUG_GOOSE_SUBSCRIBER) printf("GOOSE_SUBSCRIBER:    found array\n");
            if (MmsValue_getType(value) == MMS_ARRAY) {
                if (!parseAllData(buffer + bufPos, elementLength, value))
                    return -1;
            }
            break;
        case 0xa2: /* structure */
            if (DEBUG_GOOSE_SUBSCRIBER) printf("GOOSE_SUBSCRIBER:    found structure\n");
            if (MmsValue_getType(value) == MMS_STRUCTURE) {
                if (!parseAllData(buffer + bufPos, elementLength, value))
                    return -1;
            }
            break;
        case 0x83: /* boolean */
            if (DEBUG_GOOSE_SUBSCRIBER) printf("GOOSE_SUBSCRIBER:    found boolean\n");

            if (MmsValue_getType(value) == MMS_BOOLEAN) {
                MmsValue_setBoolean(value, BerDecoder_decodeBoolean(buffer, bufPos));
            }
            else
                if (DEBUG_GOOSE_SUBSCRIBER) printf("GOOSE_SUBSCRIBER:      message contains value of wrong type!\n");

            break;

        case 0x84: /* BIT STRING */
            if (MmsValue_getType(value) == MMS_BIT_STRING) {
                int padding = buffer[bufPos];
                int bitStringLength = (8 * (elementLength - 1)) - padding;
                if (bitStringLength == value->value.bitString.size) {
                    memcpy(value->value.bitString.buf, buffer + bufPos + 1,
                            elementLength - 1);
                }
                else
                    if (DEBUG_GOOSE_SUBSCRIBER)
                        printf("bit-string is of wrong size");
            }
            break;
        case 0x85: /* integer */
            if (MmsValue_getType(value) == MMS_INTEGER) {
                if (elementLength <= value->value.integer->maxSize) {
                    value->value.integer->size = elementLength;
                    memcpy(value->value.integer->octets, buffer + bufPos, elementLength);
                }
            }
            break;
        case 0x86: /* unsigned integer */
            if (MmsValue_getType(value) == MMS_UNSIGNED) {
                if (elementLength <= value->value.integer->maxSize) {
                    value->value.integer->size = elementLength;
                    memcpy(value->value.integer->octets, buffer + bufPos, elementLength);
                }
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
            }
            break;

        case 0x89: /* octet string */
            if (MmsValue_getType(value) == MMS_OCTET_STRING) {
                if (elementLength <= value->value.octetString.maxSize) {
                    value->value.octetString.size = elementLength;
                    memcpy(value->value.octetString.buf, buffer + bufPos, elementLength);
                }
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
            break;
        case 0x8c: /* binary time */
            if (MmsValue_getType(value) == MMS_BINARY_TIME) {
                if ((elementLength == 4) || (elementLength == 6)) {
                    memcpy(value->value.binaryTime.buf, buffer + bufPos, elementLength);
                }
            }
            break;
        case 0x91: /* Utctime */
            if (elementLength == 8) {
                if (MmsValue_getType(value) == MMS_UTC_TIME) {
                    MmsValue_setUtcTimeByBuffer(value, buffer + bufPos);
                }
                else
                    if (DEBUG_GOOSE_SUBSCRIBER) printf("GOOSE_SUBSCRIBER:      message contains value of wrong type!\n");
            }
            else
                if (DEBUG_GOOSE_SUBSCRIBER) printf("GOOSE_SUBSCRIBER:      UTCTime element is of wrong size!\n");
            break;
        default:
            if (DEBUG_GOOSE_SUBSCRIBER)
                printf("GOOSE_SUBSCRIBER:    found unkown tag %02x\n", tag);
            break;
        }

        bufPos += elementLength;

        elementIndex++;
    }

    return 1;
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

        if (bufPos + elementLength > allDataLength) {
            if (DEBUG_GOOSE_SUBSCRIBER) printf("GOOSE_SUBSCRIBER: Malformed message: sub element is too large!\n");
            goto exit_with_error;
        }

        switch (tag) {
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
                printf("GOOSE_SUBSCRIBER:    found unkown tag %02x\n", tag);
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

        if (bufPos + elementLength > allDataLength) {
            if (DEBUG_GOOSE_SUBSCRIBER) printf("GOOSE_SUBSCRIBER: Malformed message: sub element is too large!\n");
            goto exit_with_error;
        }

        MmsValue* value = NULL;

        switch (tag) {
        case 0xa1: /* array */
            if (DEBUG_GOOSE_SUBSCRIBER) printf("GOOSE_SUBSCRIBER:    found array\n");

            value = parseAllDataUnknownValue(self, buffer + bufPos, elementLength, false);

            if (value == NULL)
                goto exit_with_error;

            break;
        case 0xa2: /* structure */
            if (DEBUG_GOOSE_SUBSCRIBER) printf("GOOSE_SUBSCRIBER:    found structure\n");

            value = parseAllDataUnknownValue(self, buffer + bufPos, elementLength, true);

            if (value == NULL)
                goto exit_with_error;

            break;
        case 0x83: /* boolean */
            if (DEBUG_GOOSE_SUBSCRIBER) printf("GOOSE_SUBSCRIBER:    found boolean\n");
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
                if (DEBUG_GOOSE_SUBSCRIBER) printf("GOOSE_SUBSCRIBER:      UTCTime element is of wrong size!\n");
            break;
        default:
            if (DEBUG_GOOSE_SUBSCRIBER) printf("GOOSE_SUBSCRIBER:    found unkown tag %02x\n", tag);
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
    uint32_t confRev;
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

        int gooseEnd = bufPos + gooseLength;

        while (bufPos < gooseEnd) {
            int elementLength;

            uint8_t tag = buffer[bufPos++];
            bufPos = BerDecoder_decodeLength(buffer, &elementLength, bufPos, apduLength);

            if (bufPos + elementLength > apduLength) {
                if (DEBUG_GOOSE_SUBSCRIBER)
                    printf("GOOSE_SUBSCRIBER: Malformed message: sub element is too large!\n");

                goto exit_with_fault;
            }

            if (bufPos == -1)
                goto exit_with_fault;

            switch(tag) {
            case 0x80: /* gocbRef */
                if (DEBUG_GOOSE_SUBSCRIBER) printf("GOOSE_SUBSCRIBER:   Found gocbRef\n");

                {
                    LinkedList element = LinkedList_getNext(self->subscriberList);

                    while (element != NULL) {
                        GooseSubscriber subscriber = (GooseSubscriber) LinkedList_getData(element);

                        if (subscriber->goCBRefLen == elementLength) {
                            if (memcmp(subscriber->goCBRef, buffer + bufPos, elementLength) == 0) {
                                if (DEBUG_GOOSE_SUBSCRIBER) printf("GOOSE_SUBSCRIBER:   gocbRef is matching!\n");
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

                if (DEBUG_GOOSE_SUBSCRIBER) printf("GOOSE_SUBSCRIBER:   Found timeAllowedToLive %u\n", timeAllowedToLive);

                break;

            case 0x82:
                if (DEBUG_GOOSE_SUBSCRIBER) printf("GOOSE_SUBSCRIBER:   Found dataSet\n");
                break;

            case 0x83:
                if (DEBUG_GOOSE_SUBSCRIBER) printf("GOOSE_SUBSCRIBER:   Found goId\n");
                break;

            case 0x84:
                timestampBufPos = buffer + bufPos;
                if (DEBUG_GOOSE_SUBSCRIBER) printf("GOOSE_SUBSCRIBER:   Found timestamp\n");
                break;

            case 0x85:
                stNum = BerDecoder_decodeUint32(buffer, elementLength, bufPos);
                if (DEBUG_GOOSE_SUBSCRIBER) printf("GOOSE_SUBSCRIBER:   Found stNum: %u\n", stNum);
                break;

            case 0x86:
                sqNum = BerDecoder_decodeUint32(buffer, elementLength, bufPos);
                if (DEBUG_GOOSE_SUBSCRIBER) printf("GOOSE_SUBSCRIBER:   Found sqNum: %u\n", sqNum);
                break;

            case 0x87:
                simulation = BerDecoder_decodeBoolean(buffer, bufPos);
                if (DEBUG_GOOSE_SUBSCRIBER) printf("GOOSE_SUBSCRIBER:   Found simulation: %i\n", simulation);
                break;

            case 0x88:
                confRev = BerDecoder_decodeUint32(buffer, elementLength, bufPos);
                if (DEBUG_GOOSE_SUBSCRIBER) printf("GOOSE_SUBSCRIBER:   Found confRev: %u\n", confRev);
                break;

            case 0x89:
                ndsCom = BerDecoder_decodeBoolean(buffer, bufPos);
                if (DEBUG_GOOSE_SUBSCRIBER) printf("GOOSE_SUBSCRIBER:   Found ndsCom: %i\n", ndsCom);
                break;

            case 0x8a:
                numberOfDatSetEntries = BerDecoder_decodeUint32(buffer, elementLength, bufPos);
                if (DEBUG_GOOSE_SUBSCRIBER) printf("GOOSE_SUBSCRIBER:   Found number of entries: %u\n", numberOfDatSetEntries);
                break;

            case 0xab:
                if (DEBUG_GOOSE_SUBSCRIBER) printf("GOOSE_SUBSCRIBER:   Found all data with length: %i\n", elementLength);
                dataSetBufferAddress = buffer + bufPos;
                dataSetBufferLength = elementLength;
                break;

            default:
                if (DEBUG_GOOSE_SUBSCRIBER)  printf("GOOSE_SUBSCRIBER:   Unknown tag %02x\n", tag);
                break;
            }

            bufPos += elementLength;
        }

        if (matchingSubscriber != NULL) {

            matchingSubscriber->timeAllowedToLive = timeAllowedToLive;
            matchingSubscriber->confRev = confRev;
            matchingSubscriber->ndsCom = ndsCom;
            matchingSubscriber->simulation = simulation;
            MmsValue_setUtcTimeByBuffer(matchingSubscriber->timestamp, timestampBufPos);

            if (matchingSubscriber->dataSetValues == NULL)
                matchingSubscriber->dataSetValues = parseAllDataUnknownValue(matchingSubscriber, dataSetBufferAddress, dataSetBufferLength, false);
            else
                parseAllData(dataSetBufferAddress, dataSetBufferLength, matchingSubscriber->dataSetValues);

            bool isValid = true;

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
    if (DEBUG_GOOSE_SUBSCRIBER) printf("GOOSE_SUBSCRIBER: Invalid goose payload\n");
    return -1;
}


static void
parseGooseMessage(GooseReceiver self, int numbytes)
{
    int bufPos;
    bool subscriberFound = false;
    uint8_t* buffer = self->buffer;

    if (numbytes < 22) return;

    /* skip ethernet addresses */
    bufPos = 12;
    int headerLength = 14;

    /* check for VLAN tag */
    if ((buffer[bufPos] == 0x81) && (buffer[bufPos + 1] == 0x00)) {
        bufPos += 4; /* skip VLAN tag */
        headerLength += 4;
    }

    /* check for GOOSE Ethertype */
    if (buffer[bufPos++] != 0x88)
        return;
    if (buffer[bufPos++] != 0xb8)
        return;

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
        printf("GOOSE_SUBSCRIBER:   APPID: %u\n", appId);
        printf("GOOSE_SUBSCRIBER:   LENGTH: %u\n", length);
        printf("GOOSE_SUBSCRIBER:   APDU length: %i\n", apduLength);
    }


    // check if there is an interested subscriber
    LinkedList element = LinkedList_getNext(self->subscriberList);

    while (element != NULL) {
        GooseSubscriber subscriber = (GooseSubscriber) LinkedList_getData(element);

        if (subscriber->appId == appId) {
            subscriberFound = true;
            break;
        }

        element = LinkedList_getNext(element);
    }

    if (subscriberFound)
        parseGoosePayload(self, buffer + bufPos, apduLength);
    else {
        if (DEBUG_GOOSE_SUBSCRIBER)
            printf("GOOSE_SUBSCRIBER: GOOSE message ignored due to unknown APPID value\n");
    }
}


static void
gooseReceiverLoop(void* threadParameter)
{
    GooseReceiver self = (GooseReceiver) threadParameter;

    self->running = true;
    self->stopped = false;

    GooseReceiver_startThreadless(self);

    while (self->running) {

        if (GooseReceiver_tick(self) == false)
            Thread_sleep(1);
    }

   GooseReceiver_stopThreadless(self);

   self->stopped = true;
}


// start GOOSE receiver in a separate thread
void
GooseReceiver_start(GooseReceiver self)
{
    Thread thread = Thread_create((ThreadExecutionFunction) gooseReceiverLoop, (void*) self, true);

    if (thread != NULL) {
        if (DEBUG_GOOSE_SUBSCRIBER)
            printf("GOOSE_SUBSCRIBER: GOOSE receiver started for interface %s\n", self->interfaceId);

        Thread_start(thread);
    }
    else {
        if (DEBUG_GOOSE_SUBSCRIBER)
            printf("GOOSE_SUBSCRIBER: Starting GOOSE receiver failed for interface %s\n", self->interfaceId);
    }

}

void
GooseReceiver_stop(GooseReceiver self)
{
    self->running = false;

    while (self->stopped == false)
        Thread_sleep(1);
}

void
GooseReceiver_destroy(GooseReceiver self)
{
    LinkedList_destroyDeep(self->subscriberList,
            (LinkedListValueDeleteFunction) GooseSubscriber_destroy);

    GLOBAL_FREEMEM(self->buffer);
    GLOBAL_FREEMEM(self);
}

/***************************************
 * Functions for non-threaded operation
 ***************************************/
void
GooseReceiver_startThreadless(GooseReceiver self)
{
    if (self->interfaceId == NULL)
        self->ethSocket = Ethernet_createSocket(CONFIG_ETHERNET_INTERFACE_ID, NULL);
    else
        self->ethSocket = Ethernet_createSocket(self->interfaceId, NULL);

    Ethernet_setProtocolFilter(self->ethSocket, ETH_P_GOOSE);

    self->running = true;
}

void
GooseReceiver_stopThreadless(GooseReceiver self)
{
    Ethernet_destroySocket(self->ethSocket);

    self->running = false;
}

// call after reception of ethernet frame and periodically to to house keeping tasks
bool
GooseReceiver_tick(GooseReceiver self)
{
    int packetSize = Ethernet_receivePacket(self->ethSocket, self->buffer, ETH_BUFFER_LENGTH);

    if (packetSize > 0) {
        parseGooseMessage(self, packetSize);
        return true;
    }
    else
        return false;
}
