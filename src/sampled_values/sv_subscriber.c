/*
 *  sv_receiver.c
 *
 *  Copyright 2015 Michael Zillgith
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

#include "stack_config.h"

#include "libiec61850_platform_includes.h"

#include "hal_ethernet.h"
#include "hal_thread.h"
#include "ber_decode.h"
#include "ber_encoder.h"

#include "sv_subscriber.h"

#ifndef DEBUG_SV_SUBSCRIBER
#define DEBUG_SV_SUBSCRIBER 1
#endif

#define ETH_BUFFER_LENGTH 1518

#define ETH_P_SV 0x88ba

struct sSVReceiver {
    bool running;
    bool stopped;

    bool checkDestAddr; /* option: check destination address (additionally to AppID) to identify application */

    char* interfaceId;

    uint8_t* buffer;
    EthernetSocket ethSocket;
    LinkedList subscriberList;
};

struct sSVSubscriber {
    uint8_t ethAddr[6];
    uint16_t appId;

    SVUpdateListener listener;
    void* listenerParameter;
};

struct sSVClientASDU {

    char* svId;

    uint8_t* smpCnt;
    uint8_t* confRev;
    uint8_t* refrTm;
    uint8_t* smpSynch;


    int dataBufferLength;
    uint8_t* dataBuffer;
};




SVReceiver
SVReceiver_create(void)
{
    SVReceiver self = (SVReceiver) GLOBAL_CALLOC(1, sizeof(struct sSVReceiver));

    if (self != NULL) {
        self->subscriberList = LinkedList_create();
        self->buffer = (uint8_t*) GLOBAL_MALLOC(ETH_BUFFER_LENGTH);

        self->checkDestAddr = false;
    }

    return self;
}

void
SVReceiver_setInterfaceId(SVReceiver self, const char* interfaceId)
{
    if (self->interfaceId != NULL)
        GLOBAL_FREEMEM(self->interfaceId);

    self->interfaceId = StringUtils_copyString(interfaceId);
}

void
SVReceiver_disableDestAddrCheck(SVReceiver self)
{
    self->checkDestAddr = false;
}

void
SVReceiver_addSubscriber(SVReceiver self, SVSubscriber subscriber)
{
    LinkedList_add(self->subscriberList, (void*) subscriber);
}

void
SVReceiver_removeSubscriber(SVReceiver self, SVSubscriber subscriber)
{
    LinkedList_remove(self->subscriberList, (void*) subscriber);
}

static void
svReceiverLoop(void* threadParameter)
{
    SVReceiver self = (SVReceiver) threadParameter;

    self->running = true;
    self->stopped = false;

    SVReceiver_startThreadless(self);

    while (self->running) {

        if (SVReceiver_tick(self) == false)
            Thread_sleep(1);
    }

    SVReceiver_stopThreadless(self);

    self->stopped = true;
}


void
SVReceiver_start(SVReceiver self)
{
    Thread thread = Thread_create((ThreadExecutionFunction) svReceiverLoop, (void*) self, true);

    if (thread != NULL) {
        if (DEBUG_SV_SUBSCRIBER)
            printf("SV_SUBSCRIBER: SV receiver started for interface %s\n", self->interfaceId);

        Thread_start(thread);
    }
    else {
        if (DEBUG_SV_SUBSCRIBER)
            printf("SV_SUBSCRIBER: Starting SV receiver failed for interface %s\n", self->interfaceId);
    }
}

void
SVReceiver_stop(SVReceiver self)
{
    self->running = false;

    while (self->stopped == false)
        Thread_sleep(1);
}

void
SVReceiver_destroy(SVReceiver self)
{
    LinkedList_destroyDeep(self->subscriberList,
            (LinkedListValueDeleteFunction) SVSubscriber_destroy);

    GLOBAL_FREEMEM(self->buffer);
    GLOBAL_FREEMEM(self);
}

void
SVReceiver_startThreadless(SVReceiver self)
{
    if (self->interfaceId == NULL)
        self->ethSocket = Ethernet_createSocket(CONFIG_ETHERNET_INTERFACE_ID, NULL);
    else
        self->ethSocket = Ethernet_createSocket(self->interfaceId, NULL);

    Ethernet_setProtocolFilter(self->ethSocket, ETH_P_SV);

    self->running = true;
}

void
SVReceiver_stopThreadless(SVReceiver self)
{
    Ethernet_destroySocket(self->ethSocket);

    self->running = false;
}


static void
parseASDU(SVReceiver self, SVSubscriber subscriber, uint8_t* buffer, int length)
{
    int bufPos = 0;

    struct sSVClientASDU asdu;
    memset(&asdu, 0, sizeof(struct sSVClientASDU));

    int svIdLength = 0;


    while (bufPos < length) {
        int elementLength;

        uint8_t tag = buffer[bufPos++];

        bufPos = BerDecoder_decodeLength(buffer, &elementLength, bufPos, length);

        switch (tag) {

        case 0x80:
            asdu.svId = (char*) (buffer + bufPos);
            svIdLength = elementLength;
            break;

        case 0x82:
            asdu.smpCnt = buffer + bufPos;
            break;

        case 0x83:
            asdu.confRev = buffer + bufPos;
            break;

        case 0x84:
            asdu.refrTm = buffer + bufPos;
            break;

        case 0x85:
            asdu.smpSynch = buffer + bufPos;
            break;

        case 0x87:
            asdu.dataBuffer = buffer + bufPos;
            asdu.dataBufferLength = elementLength;
            break;

        default: /* ignore unknown tag */
            break;
        }

        bufPos += elementLength;
    }

    if (asdu.svId != NULL)
        asdu.svId[svIdLength] = 0;

    /* Call callback handler */
    if (subscriber->listener != NULL)
        subscriber->listener(subscriber, subscriber->listenerParameter, &asdu);
}

static void
parseSequenceOfASDU(SVReceiver self, SVSubscriber subscriber, uint8_t* buffer, int length)
{
    int bufPos = 0;

    while (bufPos < length) {
        int elementLength;

        uint8_t tag = buffer[bufPos++];

        bufPos = BerDecoder_decodeLength(buffer, &elementLength, bufPos, length);

        switch (tag) {
        case 0x30:
            parseASDU(self, subscriber, buffer + bufPos, elementLength);
            break;

        default: /* ignore unknown tag */
            break;
        }

        bufPos += elementLength;
    }
}

static void
parseSVPayload(SVReceiver self, SVSubscriber subscriber, uint8_t* buffer, int apduLength)
{
    int bufPos = 0;

    if (buffer[bufPos++] == 0x60) {
        int elementLength;

        bufPos = BerDecoder_decodeLength(buffer, &elementLength, bufPos, apduLength);

        int svEnd = bufPos + elementLength;

        while (bufPos < svEnd) {
            uint8_t tag = buffer[bufPos++];

            bufPos = BerDecoder_decodeLength(buffer, &elementLength, bufPos, svEnd);

            if (bufPos + elementLength > apduLength) {
                if (DEBUG_SV_SUBSCRIBER)
                    printf("SV_SUBSCRIBER: Malformed message: sub element is too large!\n");

                goto exit_error;
            }

            if (bufPos == -1)
                goto exit_error;

            switch(tag) {
            case 0x80: /* noASDU (INTEGER) */
                /* ignore */
                break;

            case 0xa2: /* asdu (SEQUENCE) */
                parseSequenceOfASDU(self, subscriber, buffer + bufPos, elementLength);
                break;

            default: /* ignore unknown tag */
                break;
            }


            bufPos += elementLength;
        }

        return;
    }

exit_error:
    if (DEBUG_SV_SUBSCRIBER)
        printf("SV_SUBSCRIBER: Invalid SV message!\n");

    return;
}

static void
parseSVMessage(SVReceiver self, int numbytes)
{
    int bufPos;
    bool subscriberFound = false;
    uint8_t* buffer = self->buffer;

    if (numbytes < 22) return;

    /* Ethernet source address */
    uint8_t* srcAddr = buffer;

    /* skip ethernet addresses */
    bufPos = 12;
    int headerLength = 14;

    /* check for VLAN tag */
    if ((buffer[bufPos] == 0x81) && (buffer[bufPos + 1] == 0x00)) {
        bufPos += 4; /* skip VLAN tag */
        headerLength += 4;
    }

    /* check for SV Ethertype */
    if (buffer[bufPos++] != 0x88)
        return;
    if (buffer[bufPos++] != 0xba)
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
        if (DEBUG_SV_SUBSCRIBER)
            printf("SV_SUBSCRIBER: Invalid PDU size\n");
        return;
    }

    if (DEBUG_SV_SUBSCRIBER) {
        printf("SV_SUBSCRIBER: SV message: ----------------\n");
        printf("SV_SUBSCRIBER:   APPID: %u\n", appId);
        printf("SV_SUBSCRIBER:   LENGTH: %u\n", length);
        printf("SV_SUBSCRIBER:   APDU length: %i\n", apduLength);
    }


    /* check if there is a matching subscriber */
    LinkedList element = LinkedList_getNext(self->subscriberList);

    SVSubscriber subscriber;

    while (element != NULL) {
        subscriber = (SVSubscriber) LinkedList_getData(element);

        if (subscriber->appId == appId) {

            if (self->checkDestAddr) {
                if (memcmp(srcAddr, subscriber->ethAddr, 6) == 0) {
                    subscriberFound = true;
                    break;
                }
                else
                    if (DEBUG_SV_SUBSCRIBER)
                        printf("SV_SUBSCRIBER: Checking ethernet src address failed!\n");
            }
            else {
                subscriberFound = true;
                break;
            }


        }

        element = LinkedList_getNext(element);
    }


    if (subscriberFound)
        parseSVPayload(self, subscriber, buffer + bufPos, apduLength);
    else {
        if (DEBUG_SV_SUBSCRIBER)
            printf("SV_SUBSCRIBER: SV message ignored due to unknown APPID value\n");
    }
}

bool
SVReceiver_tick(SVReceiver self)
{
    int packetSize = Ethernet_receivePacket(self->ethSocket, self->buffer, ETH_BUFFER_LENGTH);

    if (packetSize > 0) {
        parseSVMessage(self, packetSize);
        return true;
    }
    else
        return false;
}

SVSubscriber
SVSubscriber_create(const uint8_t* ethAddr, uint16_t appID)
{
    SVSubscriber self = (SVSubscriber) GLOBAL_CALLOC(1, sizeof(struct sSVSubscriber));

    if (self != NULL) {
        self->appId = appID;

        if (ethAddr != NULL)
            memcpy(self->ethAddr, ethAddr, 6);
    }

    return self;
}

void
SVSubscriber_destroy(SVSubscriber self)
{
    if (self != NULL)
        GLOBAL_FREEMEM(self);
}


void
SVSubscriber_setListener(SVSubscriber self,  SVUpdateListener listener, void* parameter)
{
    self->listener = listener;
    self->listenerParameter = parameter;
}

uint16_t
SVClientASDU_getSmpCnt(SVClientASDU self)
{
    uint16_t retVal;
    uint8_t* valBytes = (uint8_t*) &retVal;

#if (ORDER_LITTLE_ENDIAN == 1)
    valBytes[0] = self->smpCnt[1];
    valBytes[1] = self->smpCnt[0];
#else
    valBytes[0] = self->smpCnt[0];
    valBytes[1] = self->smpCnt[1];
#endif

    return retVal;
}

static uint64_t
decodeUtcTime(uint8_t* buffer, uint8_t* timeQuality)
{
    uint32_t timeval32;

    timeval32 = buffer[3];
    timeval32 += buffer[2] * 0x100;
    timeval32 += buffer[1] * 0x10000;
    timeval32 += buffer[0] * 0x1000000;

    uint32_t msVal;

    uint32_t fractionOfSecond;

    fractionOfSecond = buffer[6];
    fractionOfSecond += buffer[5] * 0x100;
    fractionOfSecond += buffer[4] * 0x10000;

    msVal = (uint32_t) (((uint64_t) fractionOfSecond * 1000) / 16777215);

    if (timeQuality != NULL)
        *timeQuality = buffer[7];

    uint64_t timeval64 = (uint64_t) timeval32 * 1000 + (uint64_t) msVal;

    return timeval64;
}

uint64_t
SVClientASDU_getRefrTmAsMs(SVClientASDU self)
{
    uint64_t msTime = 0;

    if (self->refrTm != NULL)
        msTime = decodeUtcTime(self->refrTm, NULL);

    return msTime;
}

bool
SVClientASDU_hasRefrTm(SVClientASDU self)
{
    return (self->refrTm != NULL);
}


const char*
SVClientASDU_getSvId(SVClientASDU self)
{
    return self->svId;
}

uint32_t
SVClientASDU_getConfRev(SVClientASDU self)
{
    uint32_t retVal = *((uint32_t*) (self->confRev));

#if (ORDER_LITTLE_ENDIAN == 1)
    uint8_t* buf = (uint8_t*) (&retVal);

    BerEncoder_revertByteOrder(buf, 4);
#endif

    return retVal;
}

int8_t
SVClientASDU_getINT8(SVClientASDU self, int index)
{
    int8_t retVal = *((int8_t*) (self->dataBuffer + index));

    return retVal;
}

int16_t
SVClientASDU_getINT16(SVClientASDU self, int index)
{
    int16_t retVal = *((int16_t*) (self->dataBuffer + index));

#if (ORDER_LITTLE_ENDIAN == 1)
    uint8_t* buf = (uint8_t*) (&retVal);

    BerEncoder_revertByteOrder(buf, 2);
#endif

    return retVal;
}

int32_t
SVClientASDU_getINT32(SVClientASDU self, int index)
{
    int32_t retVal = *((int32_t*) (self->dataBuffer + index));

#if (ORDER_LITTLE_ENDIAN == 1)
    uint8_t* buf = (uint8_t*) (&retVal);

    BerEncoder_revertByteOrder(buf, 4);
#endif

    return retVal;
}

uint8_t
SVClientASDU_getINT8U(SVClientASDU self, int index)
{
    uint8_t retVal = *((uint8_t*) (self->dataBuffer + index));

    return retVal;
}

uint16_t
SVClientASDU_getINT16U(SVClientASDU self, int index)
{
    uint16_t retVal = *((uint16_t*) (self->dataBuffer + index));

#if (ORDER_LITTLE_ENDIAN == 1)
    uint8_t* buf = (uint8_t*) (&retVal);

    BerEncoder_revertByteOrder(buf, 2);
#endif

    return retVal;
}

uint32_t
SVClientASDU_getINT32U(SVClientASDU self, int index)
{
    uint32_t retVal = *((uint32_t*) (self->dataBuffer + index));

#if (ORDER_LITTLE_ENDIAN == 1)
    uint8_t* buf = (uint8_t*) (&retVal);

    BerEncoder_revertByteOrder(buf, 4);
#endif

    return retVal;
}


float
SVClientASDU_getFLOAT32(SVClientASDU self, int index)
{
    float retVal = *((float*) (self->dataBuffer + index));

#if (ORDER_LITTLE_ENDIAN == 1)
    uint8_t* buf = (uint8_t*) (&retVal);

    BerEncoder_revertByteOrder(buf, 4);
#endif

    return retVal;
}

double
SVClientASDU_getFLOAT64(SVClientASDU self, int index)
{
    double retVal = *((double*) (self->dataBuffer + index));

#if (ORDER_LITTLE_ENDIAN == 1)
    uint8_t* buf = (uint8_t*) (&retVal);

    BerEncoder_revertByteOrder(buf, 8);
#endif

    return retVal;
}


int
SVClientASDU_getDataSize(SVClientASDU self)
{
    return self->dataBufferLength;
}

