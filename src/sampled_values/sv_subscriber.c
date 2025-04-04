/*
 *  sv_receiver.c
 *
 *  Copyright 2015-2024 Michael Zillgith
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

#define __STDC_FORMAT_MACROS 1
#include "stack_config.h"
#include <inttypes.h>

#include "libiec61850_platform_includes.h"

#include "hal_ethernet.h"
#include "hal_thread.h"
#include "hal_socket.h"
#include "ber_decode.h"
#include "ber_encoder.h"

#include "sv_subscriber.h"

#include "r_session_internal.h"

#ifndef DEBUG_SV_SUBSCRIBER
#define DEBUG_SV_SUBSCRIBER 1
#endif

#define ETH_BUFFER_LENGTH 1518

#define ETH_P_SV 0x88ba

struct sSVReceiver
{
    bool running;
    bool stopped;

    bool checkDestAddr; /* option: check destination address (additionally to AppID) to identify application */

    char* interfaceId;

    uint8_t* buffer;
    EthernetSocket ethSocket;

#if (CONFIG_IEC61850_R_SMV == 1)
    RSession session;
#endif /* (CONFIG_IEC61850_R_SMV == 1) */

    LinkedList subscriberList;

#if (CONFIG_MMS_THREADLESS_STACK == 0)
    Semaphore subscriberListLock;
    Thread thread;
#endif
};

struct sSVSubscriber
{
    uint8_t ethAddr[6];

    uint16_t appId;

    SVUpdateListener listener;
    void* listenerParameter;
};

struct sSVSubscriber_ASDU
{
    char* svId;
    char* datSet;

    uint8_t* smpCnt;
    uint8_t* confRev;
    uint8_t* refrTm;
    uint8_t* smpSynch;
    uint8_t* smpMod;
    uint8_t* smpRate;

    int dataBufferLength;
    uint8_t* dataBuffer;
};

SVReceiver
SVReceiver_create(void)
{
    SVReceiver self = (SVReceiver) GLOBAL_CALLOC(1, sizeof(struct sSVReceiver));

    if (self)
    {
        self->subscriberList = LinkedList_create();
        self->buffer = (uint8_t*) GLOBAL_MALLOC(ETH_BUFFER_LENGTH);

        self->checkDestAddr = false;

#if (CONFIG_MMS_THREADLESS_STACK == 0)
        self->subscriberListLock = Semaphore_create(1);
        self->thread = NULL;
#endif
    }

    return self;
}

#if (CONFIG_IEC61850_R_SMV == 1)
SVReceiver
SVReceiver_createRemote(RSession session)
{
    SVReceiver self = (SVReceiver) GLOBAL_CALLOC(1, sizeof(struct sSVReceiver));

    if (self != NULL)
    {
        self->subscriberList = LinkedList_create();
        self->buffer = NULL;

        self->checkDestAddr = false;

        self->session = session;

#if (CONFIG_MMS_THREADLESS_STACK == 0)
        self->subscriberListLock = Semaphore_create(1);
#endif
    }

    return self;
}
#endif /* (CONFIG_IEC61850_R_SMV == 1) */

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
SVReceiver_enableDestAddrCheck(SVReceiver self)
{
    self->checkDestAddr = true;
}

void
SVReceiver_addSubscriber(SVReceiver self, SVSubscriber subscriber)
{
#if (CONFIG_MMS_THREADLESS_STACK == 0)
    Semaphore_wait(self->subscriberListLock);
#endif

    LinkedList_add(self->subscriberList, (void*) subscriber);

#if (CONFIG_MMS_THREADLESS_STACK == 0)
    Semaphore_post(self->subscriberListLock);
#endif
}

void
SVReceiver_removeSubscriber(SVReceiver self, SVSubscriber subscriber)
{
#if (CONFIG_MMS_THREADLESS_STACK == 0)
    Semaphore_wait(self->subscriberListLock);
#endif

    LinkedList_remove(self->subscriberList, (void*) subscriber);

#if (CONFIG_MMS_THREADLESS_STACK == 0)
    Semaphore_post(self->subscriberListLock);
#endif
}

static void*
svReceiverLoop(void* threadParameter)
{
    SVReceiver self = (SVReceiver) threadParameter;

#if (CONFIG_IEC61850_L2_SMV == 1)
    if (self->ethSocket)
    {
        EthernetHandleSet handleSet = EthernetHandleSet_new();
        EthernetHandleSet_addSocket(handleSet, self->ethSocket);

        self->stopped = false;

        while (self->running)
        {
            switch (EthernetHandleSet_waitReady(handleSet, 100))
            {
            case -1:
                if (DEBUG_SV_SUBSCRIBER)
                    printf("SV_SUBSCRIBER: EhtnernetHandleSet_waitReady() failure\n");
                break;
            case 0:
                break;
            default:
                SVReceiver_tick(self);
            }
        }

        EthernetHandleSet_destroy(handleSet);
    }
#endif /* (CONFIG_IEC61850_L2_SMV == 1) */

#if (CONFIG_IEC61850_R_SMV == 1)
    if (self->session)
    {
        self->stopped = false;

        HandleSet handleSet = Handleset_new();

        Handleset_addSocket(handleSet, RSession_getSocket(self->session));

        while (self->running)
        {
            switch (Handleset_waitReady(handleSet, 100))
            {
            case -1:
                if (DEBUG_SV_SUBSCRIBER)
                    printf("SV_SUBSCRIBER: HandleSet_waitReady() failure\n");
                break;
            case 0:
                break;
            default:
                SVReceiver_tick(self);
            }
        }

        Handleset_destroy(handleSet);
    }
#endif /* (CONFIG_IEC61850_R_SMV == 1) */

    self->stopped = true;

    return NULL;
}

void
SVReceiver_start(SVReceiver self)
{
    if (SVReceiver_startThreadless(self))
    {
        if (self->interfaceId)
        {
            if (DEBUG_SV_SUBSCRIBER)
                printf("SV_SUBSCRIBER: SV receiver started for interface %s\n", self->interfaceId);
        }
        else {
            if (DEBUG_SV_SUBSCRIBER)
                printf("SV_SUBSCRIBER: R-SV receiver started\n");
        }

#if (CONFIG_MMS_THREADLESS_STACK == 0)

        self->thread = Thread_create((ThreadExecutionFunction) svReceiverLoop, (void*) self, false);

        if (self->thread)
        {
            Thread_start(self->thread);
        }
        else
        {
            if (DEBUG_SV_SUBSCRIBER)
                printf("SV_SUBSCRIBER: Failed to start thread\n");
        }

#endif /* (CONFIG_MMS_THREADLESS_STACK == 0) */
    }
    else
    {
        if (DEBUG_SV_SUBSCRIBER)
            printf("SV_SUBSCRIBER: Starting SV receiver failed for interface %s\n", self->interfaceId);
    }
}

bool
SVReceiver_isRunning(SVReceiver self)
{
    return self->running;
}

void
SVReceiver_stop(SVReceiver self)
{
    if (self->running)
    {
        self->running = false;

#if (CONFIG_MMS_THREADLESS_STACK == 0)
        if (self->thread)
        {
            Thread_destroy(self->thread);
            self->thread = NULL;
        }
#endif /* (CONFIG_MMS_THREADLESS_STACK == 0) */

        SVReceiver_stopThreadless(self);
    }
}

void
SVReceiver_destroy(SVReceiver self)
{
    SVReceiver_stop(self);

    LinkedList_destroyDeep(self->subscriberList,
            (LinkedListValueDeleteFunction) SVSubscriber_destroy);

    if (self->interfaceId)
        GLOBAL_FREEMEM(self->interfaceId);

#if (CONFIG_MMS_THREADLESS_STACK == 0)
    if (self->thread) {
        Thread_destroy(self->thread);
        self->thread = NULL;
    }

    Semaphore_destroy(self->subscriberListLock);
#endif

    GLOBAL_FREEMEM(self->buffer);
    GLOBAL_FREEMEM(self);
}

bool
SVReceiver_startThreadless(SVReceiver self)
{
#if (CONFIG_IEC61850_R_SMV == 1)
    if (self->session)
    {
        if (RSession_start(self->session) == R_SESSION_ERROR_OK)
        {
            self->running = true;

            return true;
        }
    }
    else
    {
#endif /* (CONFIG_IEC61850_R_SMV == 1) */

#if (CONFIG_IEC61850_L2_SMV == 1)
        if (self->interfaceId == NULL)
            self->ethSocket = Ethernet_createSocket(CONFIG_ETHERNET_INTERFACE_ID, NULL);
        else
            self->ethSocket = Ethernet_createSocket(self->interfaceId, NULL);

        if (self->ethSocket)
        {
            Ethernet_setProtocolFilter(self->ethSocket, ETH_P_SV);

            self->running = true;

            return true;
        }
#endif /* (CONFIG_IEC61850_L2_SMV == 1) */

#if (CONFIG_IEC61850_R_SMV == 1)
    }
#endif /* (CONFIG_IEC61850_R_SMV == 1) */

    return false;
}

void
SVReceiver_stopThreadless(SVReceiver self)
{
#if (CONFIG_IEC61850_L2_SMV == 1)
    if (self->ethSocket)
        Ethernet_destroySocket(self->ethSocket);
#endif /* (CONFIG_IEC61850_L2_SMV == 1) */

#if (CONFIG_IEC61850_R_SMV == 1)
    if (self->session) {
        RSession_stop(self->session);
    }
#endif /* (CONFIG_IEC61850_R_SMV == 1) */

    self->running = false;
}

static void
parseASDU(SVReceiver self, SVSubscriber subscriber, uint8_t* buffer, int length)
{
    (void)self;

    int bufPos = 0;
    int svIdLength = 0;
    int datSetLength = 0;

    struct sSVSubscriber_ASDU asdu;
    memset(&asdu, 0, sizeof(struct sSVSubscriber_ASDU));

    while (bufPos < length)
    {
        int elementLength;

        uint8_t tag = buffer[bufPos++];

        bufPos = BerDecoder_decodeLength(buffer, &elementLength, bufPos, length);
        if (bufPos < 0)
        {
            if (DEBUG_SV_SUBSCRIBER) printf("SV_SUBSCRIBER: Malformed message: failed to decode BER length tag!\n");
            return;
        }

        switch (tag)
        {
        case 0x80:
            asdu.svId = (char*) (buffer + bufPos);
            svIdLength = elementLength;
            break;

        case 0x81:
            asdu.datSet = (char*) (buffer + bufPos);
            datSetLength = elementLength;
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

        case 0x86:
            asdu.smpRate = buffer + bufPos;
            break;

        case 0x87:
            asdu.dataBuffer = buffer + bufPos;
            asdu.dataBufferLength = elementLength;
            break;

        case 0x88:
            asdu.smpMod = buffer + bufPos;
            break;

        default: /* ignore unknown tag */
            if (DEBUG_SV_SUBSCRIBER) printf("SV_SUBSCRIBER: found unknown tag %02x\n", tag);
            break;
        }

        bufPos += elementLength;
    }

    if (asdu.svId != NULL)
        asdu.svId[svIdLength] = 0;
    if (asdu.datSet != NULL)
        asdu.datSet[datSetLength] = 0;
    
    if (DEBUG_SV_SUBSCRIBER)
    {
        printf("SV_SUBSCRIBER:   SV ASDU: ----------------\n");
        printf("SV_SUBSCRIBER:     DataLength: %d\n", asdu.dataBufferLength);
        printf("SV_SUBSCRIBER:     SvId: %s\n", asdu.svId);
        printf("SV_SUBSCRIBER:     SmpCnt: %u\n", SVSubscriber_ASDU_getSmpCnt(&asdu));
        printf("SV_SUBSCRIBER:     ConfRev: %u\n", SVSubscriber_ASDU_getConfRev(&asdu));
        
        if (SVSubscriber_ASDU_hasDatSet(&asdu))
            printf("SV_SUBSCRIBER:     DatSet: %s\n", asdu.datSet);

        if (SVSubscriber_ASDU_hasRefrTm(&asdu))
#ifndef _MSC_VER
            printf("SV_SUBSCRIBER:     RefrTm[ns]: %"PRIu64"\n", SVSubscriber_ASDU_getRefrTmAsNs(&asdu));
#else
            printf("SV_SUBSCRIBER:     RefrTm[ns]: %llu\n", SVSubscriber_ASDU_getRefrTmAsNs(&asdu));
#endif
        if (SVSubscriber_ASDU_hasSmpMod(&asdu))
            printf("SV_SUBSCRIBER:     SmpMod: %d\n", SVSubscriber_ASDU_getSmpMod(&asdu));
        if (SVSubscriber_ASDU_hasSmpRate(&asdu))
            printf("SV_SUBSCRIBER:     SmpRate: %d\n", SVSubscriber_ASDU_getSmpRate(&asdu));
    }

    /* Call callback handler */
    if (subscriber)
    {
        if (subscriber->listener != NULL)
            subscriber->listener(subscriber, subscriber->listenerParameter, &asdu);
    }
}

static void
parseSequenceOfASDU(SVReceiver self, SVSubscriber subscriber, uint8_t* buffer, int length)
{
    int bufPos = 0;

    while (bufPos < length)
    {
        int elementLength;

        uint8_t tag = buffer[bufPos++];

        bufPos = BerDecoder_decodeLength(buffer, &elementLength, bufPos, length);
        if (bufPos < 0)
        {
            if (DEBUG_SV_SUBSCRIBER) printf("SV_SUBSCRIBER: Malformed message: failed to decode BER length tag!\n");
            return;
        }

        switch (tag)
        {
        case 0x30:
            parseASDU(self, subscriber, buffer + bufPos, elementLength);
            break;

        default: /* ignore unknown tag */
            if (DEBUG_SV_SUBSCRIBER) printf("SV_SUBSCRIBER: found unknown tag %02x\n", tag);
            break;
        }

        bufPos += elementLength;
    }
}

static void
parseSVPayload(SVReceiver self, SVSubscriber subscriber, uint8_t* buffer, int apduLength)
{
    int bufPos = 0;

    if (buffer[bufPos++] == 0x60)
    {
        int elementLength;

        bufPos = BerDecoder_decodeLength(buffer, &elementLength, bufPos, apduLength);
        if (bufPos < 0)
        {
            if (DEBUG_SV_SUBSCRIBER) printf("SV_SUBSCRIBER: Malformed message: failed to decode BER length tag!\n");
            return;
        }

        int svEnd = bufPos + elementLength;

        while (bufPos < svEnd)
        {
            uint8_t tag = buffer[bufPos++];

            bufPos = BerDecoder_decodeLength(buffer, &elementLength, bufPos, svEnd);
            if (bufPos < 0)
                goto exit_error;

            switch(tag)
            {
            case 0x80: /* noASDU (INTEGER) */
                /* ignore */
                break;

            case 0xa2: /* asdu (SEQUENCE) */
                parseSequenceOfASDU(self, subscriber, buffer + bufPos, elementLength);
                break;

            default: /* ignore unknown tag */
                if (DEBUG_SV_SUBSCRIBER) printf("SV_SUBSCRIBER: found unknown tag %02x\n", tag);
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
handleSVApdu(SVReceiver self, uint16_t appId, uint8_t* apdu, int apduLength, uint8_t* dstAddr)
{
    if (DEBUG_SV_SUBSCRIBER) {
        printf("SV_SUBSCRIBER: SV message: ----------------\n");
        printf("SV_SUBSCRIBER:   APPID: %u\n", appId);
        printf("SV_SUBSCRIBER:   APDU length: %i\n", apduLength);
    }

    /* check if there is a matching subscriber */

#if (CONFIG_MMS_THREADLESS_STACK == 0)
    Semaphore_wait(self->subscriberListLock);
#endif

    LinkedList element = LinkedList_getNext(self->subscriberList);

    SVSubscriber subscriber;

    bool subscriberFound = false;

    while (element)
    {
        subscriber = (SVSubscriber) LinkedList_getData(element);

        if (subscriber->appId == appId)
        {
            if (self->checkDestAddr)
            {
                if (self->ethSocket)
                {
                    if (memcmp(dstAddr, subscriber->ethAddr, 6) == 0)
                    {
                        subscriberFound = true;
                        break;
                    }
                    else
                    {
                        if (DEBUG_SV_SUBSCRIBER)
                            printf("SV_SUBSCRIBER: Checking ethernet dest address failed!\n");
                    }
                }
                else
                {
                    //TODO check destination IP address for R-SV
                }
            }
            else
            {
                subscriberFound = true;
                break;
            }
        }

        element = LinkedList_getNext(element);
    }

#if (CONFIG_MMS_THREADLESS_STACK == 0)
    Semaphore_post(self->subscriberListLock);
#endif

    if (subscriberFound)
    {
        parseSVPayload(self, subscriber, apdu, apduLength);
    }
    else
    {
        if (DEBUG_SV_SUBSCRIBER)
            printf("SV_SUBSCRIBER: SV message ignored due to unknown APPID value or dest address mismatch\n");
    }
}

static void
parseSVMessage(SVReceiver self, int numbytes)
{
    int bufPos;
    uint8_t* buffer = self->buffer;

    if (numbytes < 22) return;

    /* Ethernet source address */
    uint8_t* dstAddr = buffer;

    /* skip ethernet addresses */
    bufPos = 12;
    int headerLength = 14;

    /* check for VLAN tag */
    if ((buffer[bufPos] == 0x81) && (buffer[bufPos + 1] == 0x00))
    {
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

    if (numbytes < length + headerLength)
    {
        if (DEBUG_SV_SUBSCRIBER)
            printf("SV_SUBSCRIBER: Invalid PDU size\n");
        return;
    }

    handleSVApdu(self, appId, buffer + bufPos, apduLength, dstAddr);
}

#if (CONFIG_IEC61850_R_SMV == 1)
static void
handleSessionPayloadElement(void* parameter, uint16_t appId, uint8_t* payloadData, int payloadSize)
{
    (void)appId;
    SVReceiver self = (SVReceiver) parameter;

    handleSVApdu(self, appId, payloadData, payloadSize, NULL);
}
#endif /* (CONFIG_IEC61850_R_SMV == 1) */

bool
SVReceiver_tick(SVReceiver self)
{
#if (CONFIG_IEC61850_L2_SMV == 1)
    if (self->ethSocket)
    {
        int packetSize = Ethernet_receivePacket(self->ethSocket, self->buffer, ETH_BUFFER_LENGTH);

        if (packetSize > 0)
        {
            parseSVMessage(self, packetSize);
            return true;
        }
    }
#endif /* (CONFIG_IEC61850_L2_SMV == 1) */

#if (CONFIG_IEC61850_R_SMV == 1)
    if (self->session)
    {
        if (RSession_receiveMessage(self->session, handleSessionPayloadElement, (void*) self) == R_SESSION_ERROR_OK)
            return true;
    }
#endif /* (CONFIG_IEC61850_R_SMV == 1) */

    return false;
}

SVSubscriber
SVSubscriber_create(const uint8_t* ethAddr, uint16_t appID)
{
    SVSubscriber self = (SVSubscriber) GLOBAL_CALLOC(1, sizeof(struct sSVSubscriber));

    if (self)
    {
        self->appId = appID;

        if (ethAddr)
            memcpy(self->ethAddr, ethAddr, 6);
    }

    return self;
}

void
SVSubscriber_destroy(SVSubscriber self)
{
    if (self)
        GLOBAL_FREEMEM(self);
}

void
SVSubscriber_setListener(SVSubscriber self,  SVUpdateListener listener, void* parameter)
{
    self->listener = listener;
    self->listenerParameter = parameter;
}

uint8_t
SVSubscriber_ASDU_getSmpSynch(SVSubscriber_ASDU self)
{
    return self->smpSynch[0];
}

uint16_t
SVSubscriber_ASDU_getSmpCnt(SVSubscriber_ASDU self)
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

static nsSinceEpoch
decodeUtcTimeToNsTime(uint8_t* buffer, uint8_t* timeQuality)
{
    uint32_t timeval32;

    timeval32 = buffer[3];
    timeval32 += buffer[2] * 0x100;
    timeval32 += buffer[1] * 0x10000;
    timeval32 += buffer[0] * 0x1000000;

    uint32_t fractionOfSecond;

    fractionOfSecond = buffer[6];
    fractionOfSecond += buffer[5] * 0x100;
    fractionOfSecond += buffer[4] * 0x10000;

    uint64_t nsVal = fractionOfSecond;

    nsVal = nsVal * 1000000000UL;
    nsVal = nsVal >> 24;

    if (timeQuality)
        *timeQuality = buffer[7];

    uint64_t timeval64 = (uint64_t) timeval32 * 1000000000ULL + nsVal;

    return timeval64;
}

msSinceEpoch
SVSubscriber_ASDU_getRefrTmAsMs(SVSubscriber_ASDU self)
{
    msSinceEpoch msTime = 0;

    if (self->refrTm)
        msTime = decodeUtcTimeToNsTime(self->refrTm, NULL);

    return (msTime / 1000000ULL);
}

nsSinceEpoch
SVSubscriber_ASDU_getRefrTmAsNs(SVSubscriber_ASDU self)
{
    nsSinceEpoch nsTime = 0;

    if (self->refrTm)
        nsTime = decodeUtcTimeToNsTime(self->refrTm, NULL);

    return nsTime;
}

bool
SVSubscriber_ASDU_hasRefrTm(SVSubscriber_ASDU self)
{
    return (self->refrTm != NULL);
}

bool
SVSubscriber_ASDU_hasDatSet(SVSubscriber_ASDU self)
{
    return (self->datSet != NULL);
}

bool
SVSubscriber_ASDU_hasSmpRate(SVSubscriber_ASDU self)
{
    return (self->smpRate != NULL);
}

bool
SVSubscriber_ASDU_hasSmpMod(SVSubscriber_ASDU self)
{
    return (self->smpMod != NULL);
}

const char*
SVSubscriber_ASDU_getSvId(SVSubscriber_ASDU self)
{
    return self->svId;
}

const char*
SVSubscriber_ASDU_getDatSet(SVSubscriber_ASDU self)
{
    return self->datSet;
}

static inline void
memcpy_reverse(void* to, const void* from, size_t size)
{
    size_t i;

    for (i = 0; i < size; i++)
        ((uint8_t*)to)[size - 1 - i] = ((uint8_t*)from)[i];
}

uint32_t
SVSubscriber_ASDU_getConfRev(SVSubscriber_ASDU self)
{
    uint32_t retVal;

#if (ORDER_LITTLE_ENDIAN == 1)
    memcpy_reverse(&retVal, self->confRev, sizeof(uint32_t));
#else
    memcpy(&retVal, self->confRev, sizeof(uint32_t));
#endif

    return retVal;
}

uint8_t
SVSubscriber_ASDU_getSmpMod(SVSubscriber_ASDU self)
{
    uint8_t retVal = *((uint8_t*) (self->smpMod));

    return retVal;
}

uint16_t
SVSubscriber_ASDU_getSmpRate(SVSubscriber_ASDU self)
{
    uint16_t retVal;

#if (ORDER_LITTLE_ENDIAN == 1)
    memcpy_reverse(&retVal, self->smpRate, sizeof(uint16_t));
#else
    memcpy(&retVal, self->smpRate, sizeof(uint16_t));
#endif

    return retVal;
}

int8_t
SVSubscriber_ASDU_getINT8(SVSubscriber_ASDU self, int index)
{
    int8_t retVal = *((int8_t*) (self->dataBuffer + index));

    return retVal;
}

int16_t
SVSubscriber_ASDU_getINT16(SVSubscriber_ASDU self, int index)
{
    int16_t retVal;

#if (ORDER_LITTLE_ENDIAN == 1)
    memcpy_reverse(&retVal, (self->dataBuffer + index), sizeof(uint16_t));
#else
    memcpy(&retVal, (self->dataBuffer + index), sizeof(uint16_t));
#endif

    return retVal;
}

int32_t
SVSubscriber_ASDU_getINT32(SVSubscriber_ASDU self, int index)
{
    int32_t retVal;

#if (ORDER_LITTLE_ENDIAN == 1)
    memcpy_reverse(&retVal, (self->dataBuffer + index), sizeof(int32_t));
#else
    memcpy(&retVal, (self->dataBuffer + index), sizeof(int32_t));
#endif

    return retVal;
}

int64_t
SVSubscriber_ASDU_getINT64(SVSubscriber_ASDU self, int index)
{
    int64_t retVal;

#if (ORDER_LITTLE_ENDIAN == 1)
    memcpy_reverse(&retVal, (self->dataBuffer + index), sizeof(int64_t));
#else
    memcpy(&retVal, (self->dataBuffer + index), sizeof(int64_t));
#endif

    return retVal;
}

uint8_t
SVSubscriber_ASDU_getINT8U(SVSubscriber_ASDU self, int index)
{
    uint8_t retVal = *((uint8_t*) (self->dataBuffer + index));

    return retVal;
}

uint16_t
SVSubscriber_ASDU_getINT16U(SVSubscriber_ASDU self, int index)
{
    uint16_t retVal;

#if (ORDER_LITTLE_ENDIAN == 1)
    memcpy_reverse(&retVal, (self->dataBuffer + index), sizeof(uint16_t));
#else
    memcpy(&retVal, (self->dataBuffer + index), sizeof(uint16_t));
#endif

    return retVal;
}

uint32_t
SVSubscriber_ASDU_getINT32U(SVSubscriber_ASDU self, int index)
{
    uint32_t retVal;

#if (ORDER_LITTLE_ENDIAN == 1)
    memcpy_reverse(&retVal, (self->dataBuffer + index), sizeof(uint32_t));
#else
    memcpy(&retVal, (self->dataBuffer + index), sizeof(uint32_t));
#endif

    return retVal;
}

uint64_t
SVSubscriber_ASDU_getINT64U(SVSubscriber_ASDU self, int index)
{
    uint64_t retVal;

#if (ORDER_LITTLE_ENDIAN == 1)
    memcpy_reverse(&retVal, (self->dataBuffer + index), sizeof(uint64_t));
#else
    memcpy(&retVal, (self->dataBuffer + index), sizeof(uint64_t));
#endif

    return retVal;
}

float
SVSubscriber_ASDU_getFLOAT32(SVSubscriber_ASDU self, int index)
{
    float retVal;

#if (ORDER_LITTLE_ENDIAN == 1)
    memcpy_reverse(&retVal, (self->dataBuffer + index), sizeof(float));
#else
    memcpy(&retVal, (self->dataBuffer + index), sizeof(float));
#endif

    return retVal;
}

double
SVSubscriber_ASDU_getFLOAT64(SVSubscriber_ASDU self, int index)
{
    double retVal;

#if (ORDER_LITTLE_ENDIAN == 1)
    memcpy_reverse(&retVal, (self->dataBuffer + index), sizeof(double));
#else
    memcpy(&retVal, (self->dataBuffer + index), sizeof(double));
#endif

    return retVal;
}

Timestamp
SVSubscriber_ASDU_getTimestamp(SVSubscriber_ASDU self, int index)
{
    Timestamp retVal;

    memcpy(retVal.val, self->dataBuffer + index, sizeof(retVal.val));

    return retVal;
}

Quality
SVSubscriber_ASDU_getQuality(SVSubscriber_ASDU self, int index)
{
    Quality retVal;

    uint8_t* buffer = self->dataBuffer + index;

    retVal = buffer[3];
    retVal += (buffer[2] * 0x100);

    return retVal;
}

int
SVSubscriber_ASDU_getDataSize(SVSubscriber_ASDU self)
{
    return self->dataBufferLength;
}
