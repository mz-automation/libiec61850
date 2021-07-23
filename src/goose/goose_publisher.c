/*
 *  goose_publisher.c
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
#include "stack_config.h"
#include "goose_publisher.h"
#include "hal_ethernet.h"
#include "ber_encoder.h"
#include "mms_server_internal.h"
#include "mms_value_internal.h"

#ifndef DEBUG_GOOSE_PUBLISHER
#define DEBUG_GOOSE_PUBLISHER 0
#endif

#define GOOSE_MAX_MESSAGE_SIZE 1518

static bool
prepareGooseBuffer(GoosePublisher self, CommParameters* parameters, const char* interfaceID, bool useVlanTags);

struct sGoosePublisher {
    uint8_t* buffer;

    EthernetSocket ethernetSocket;
    int lengthField;
    int payloadStart;
    int payloadLength;

    char* goID;
    char* goCBRef;
    char* dataSetRef;

    uint32_t confRev;
    uint32_t stNum;
    uint32_t sqNum;
    uint32_t timeAllowedToLive;
    bool needsCommission;
    bool simulation;

    MmsValue* timestamp; /* time when stNum is increased */
};

GoosePublisher
GoosePublisher_createEx(CommParameters* parameters, const char* interfaceID, bool useVlanTag)
{
    GoosePublisher self = (GoosePublisher) GLOBAL_CALLOC(1, sizeof(struct sGoosePublisher));

    if (self) {

        if (prepareGooseBuffer(self, parameters, interfaceID, useVlanTag)) {
            self->timestamp = MmsValue_newUtcTimeByMsTime(Hal_getTimeInMs());

            GoosePublisher_reset(self);
        }
        else {
            GoosePublisher_destroy(self);
            self = NULL;
        }

    }

    return self;
}

GoosePublisher
GoosePublisher_create(CommParameters* parameters, const char* interfaceID)
{
    return GoosePublisher_createEx(parameters, interfaceID, true);
}

void
GoosePublisher_destroy(GoosePublisher self)
{
    if (self->ethernetSocket) {
        Ethernet_destroySocket(self->ethernetSocket);
    }

    MmsValue_delete(self->timestamp);

    if (self->goID != NULL)
        GLOBAL_FREEMEM(self->goID);

    if (self->goCBRef != NULL)
        GLOBAL_FREEMEM(self->goCBRef);

    if (self->dataSetRef != NULL)
        GLOBAL_FREEMEM(self->dataSetRef);

    if (self->buffer)
        GLOBAL_FREEMEM(self->buffer);

    GLOBAL_FREEMEM(self);
}

void
GoosePublisher_setGoID(GoosePublisher self, char* goID)
{
    self->goID = StringUtils_copyString(goID);
}

void
GoosePublisher_setGoCbRef(GoosePublisher self, char* goCbRef)
{
    self->goCBRef = StringUtils_copyString(goCbRef);
}

void
GoosePublisher_setDataSetRef(GoosePublisher self, char* dataSetRef)
{
    self->dataSetRef = StringUtils_copyString(dataSetRef);
}

void
GoosePublisher_setConfRev(GoosePublisher self, uint32_t confRev)
{
    self->confRev = confRev;
}

void
GoosePublisher_setSimulation(GoosePublisher self, bool simulation)
{
    self->simulation = simulation;
}

void
GoosePublisher_setStNum(GoosePublisher self, uint32_t stNum)
{
  self->stNum = stNum;
}

void
GoosePublisher_setSqNum(GoosePublisher self, uint32_t sqNum)
{
  self->sqNum = sqNum;
}

void
GoosePublisher_setNeedsCommission(GoosePublisher self, bool ndsCom)
{
    self->needsCommission = ndsCom;
}

uint64_t
GoosePublisher_increaseStNum(GoosePublisher self)
{
    uint64_t currentTime = Hal_getTimeInMs();

    MmsValue_setUtcTimeMs(self->timestamp, currentTime);

    self->stNum++;

    /* check for overflow */
    if (self->stNum == 0)
        self->stNum = 1;

    self->sqNum = 0;

    return currentTime;
}

void
GoosePublisher_reset(GoosePublisher self) {
    self->sqNum = 0;
    self->stNum = 1;
}

void
GoosePublisher_setTimeAllowedToLive(GoosePublisher self, uint32_t timeAllowedToLive)
{
    self->timeAllowedToLive = timeAllowedToLive;
}

static bool
prepareGooseBuffer(GoosePublisher self, CommParameters* parameters, const char* interfaceID, bool useVlanTags)
{
    uint8_t srcAddr[6];

    if (interfaceID != NULL)
        Ethernet_getInterfaceMACAddress(interfaceID, srcAddr);
    else
        Ethernet_getInterfaceMACAddress(CONFIG_ETHERNET_INTERFACE_ID, srcAddr);

    uint8_t defaultDstAddr[] = CONFIG_GOOSE_DEFAULT_DST_ADDRESS;

    uint8_t* dstAddr;
    uint8_t priority;
    uint16_t vlanId;
    uint16_t appId;

    if (parameters == NULL) {
        dstAddr = defaultDstAddr;
        priority = CONFIG_GOOSE_DEFAULT_PRIORITY;
        vlanId = CONFIG_GOOSE_DEFAULT_VLAN_ID;
        appId = CONFIG_GOOSE_DEFAULT_APPID;
    }
    else {
        dstAddr = parameters->dstAddress;
        priority = parameters->vlanPriority;
        vlanId = parameters->vlanId;
        appId = parameters->appId;
    }

    if (interfaceID != NULL)
        self->ethernetSocket = Ethernet_createSocket(interfaceID, dstAddr);
    else
        self->ethernetSocket = Ethernet_createSocket(CONFIG_ETHERNET_INTERFACE_ID, dstAddr);

    if (self->ethernetSocket) {
        self->buffer = (uint8_t*) GLOBAL_MALLOC(GOOSE_MAX_MESSAGE_SIZE);

        memcpy(self->buffer, dstAddr, 6);
        memcpy(self->buffer + 6, srcAddr, 6);

        int bufPos = 12;

        if (useVlanTags) {
            /* Priority tag - IEEE 802.1Q */
            self->buffer[bufPos++] = 0x81;
            self->buffer[bufPos++] = 0x00;

            uint8_t tci1 = priority << 5;
            tci1 += vlanId / 256;

            uint8_t tci2 = vlanId % 256;

            self->buffer[bufPos++] = tci1; /* Priority + VLAN-ID */
            self->buffer[bufPos++] = tci2; /* VLAN-ID */
        }

        /* EtherType GOOSE */
        self->buffer[bufPos++] = 0x88;
        self->buffer[bufPos++] = 0xB8;

        /* APPID */
        self->buffer[bufPos++] = appId / 256;
        self->buffer[bufPos++] = appId % 256;

        self->lengthField = bufPos;

        /* Length */
        self->buffer[bufPos++] = 0x00;
        self->buffer[bufPos++] = 0x08;

        /* Reserved1 */
        self->buffer[bufPos++] = 0x00;
        self->buffer[bufPos++] = 0x00;

        /* Reserved2 */
        self->buffer[bufPos++] = 0x00;
        self->buffer[bufPos++] = 0x00;

        self->payloadStart = bufPos;

        return true;
    }
    else {
        return false;
    }
}

static int32_t
createGoosePayload(GoosePublisher self, LinkedList dataSetValues, uint8_t* buffer, size_t maxPayloadSize) {

    /* Step 1 - calculate length fields */
    uint32_t goosePduLength = 0;

    goosePduLength += BerEncoder_determineEncodedStringSize(self->goCBRef);

    uint32_t timeAllowedToLive = self->timeAllowedToLive;

    goosePduLength += 2 + BerEncoder_UInt32determineEncodedSize(timeAllowedToLive);

    goosePduLength += BerEncoder_determineEncodedStringSize(self->dataSetRef);

    if (self->goID != NULL)
        goosePduLength += BerEncoder_determineEncodedStringSize(self->goID);
    else
        goosePduLength += BerEncoder_determineEncodedStringSize(self->goCBRef);

    goosePduLength += 2 + 8; /* for T (UTCTIME) */

    goosePduLength += 2 + BerEncoder_UInt32determineEncodedSize(self->sqNum);

    goosePduLength += 2 + BerEncoder_UInt32determineEncodedSize(self->stNum);

    goosePduLength += 2 + BerEncoder_UInt32determineEncodedSize(self->confRev);

    goosePduLength += 6; /* for ndsCom and simulation */

    uint32_t numberOfDataSetEntries = LinkedList_size(dataSetValues);

    goosePduLength += 2 + BerEncoder_UInt32determineEncodedSize(numberOfDataSetEntries);

    uint32_t dataSetSize = 0;

    LinkedList element = LinkedList_getNext(dataSetValues);

    while (element != NULL) {
        MmsValue* dataSetEntry = (MmsValue*) element->data;

        if (dataSetEntry) {
            dataSetSize += MmsValue_encodeMmsData(dataSetEntry, NULL, 0, false);
        }
        else {
            /* TODO encode MMS NULL */
            if (DEBUG_GOOSE_PUBLISHER)
                printf("GOOSE_PUBLISHER: NULL value in data set!\n");
        }

        element = LinkedList_getNext(element);
    }

    uint32_t allDataSize = dataSetSize + BerEncoder_determineLengthSize(dataSetSize) + 1;

    goosePduLength += allDataSize;

    uint32_t payloadSize = 1 + BerEncoder_determineLengthSize(goosePduLength) + goosePduLength;

    if (payloadSize > maxPayloadSize)
        return -1;

    /* Step 2 - encode to buffer */

    int32_t bufPos = 0;

    /* Encode GOOSE PDU */
    bufPos = BerEncoder_encodeTL(0x61, goosePduLength, buffer, bufPos);

    /* Encode gocbRef */
    bufPos = BerEncoder_encodeStringWithTag(0x80, self->goCBRef, buffer, bufPos);

    /* Encode timeAllowedToLive */
    bufPos = BerEncoder_encodeUInt32WithTL(0x81, timeAllowedToLive, buffer, bufPos);

    /* Encode datSet reference */
    bufPos = BerEncoder_encodeStringWithTag(0x82, self->dataSetRef, buffer, bufPos);

    /* Encode goID */
    if (self->goID != NULL)
        bufPos = BerEncoder_encodeStringWithTag(0x83, self->goID, buffer, bufPos);
    else
        bufPos = BerEncoder_encodeStringWithTag(0x83, self->goCBRef, buffer, bufPos);

    /* Encode t */
    bufPos = BerEncoder_encodeOctetString(0x84, self->timestamp->value.utcTime, 8, buffer, bufPos);

    /* Encode stNum */
    bufPos = BerEncoder_encodeUInt32WithTL(0x85, self->stNum, buffer, bufPos);

    /* Encode sqNum */
    bufPos = BerEncoder_encodeUInt32WithTL(0x86, self->sqNum, buffer, bufPos);

    /* Encode simulation */
    bufPos = BerEncoder_encodeBoolean(0x87, self->simulation, buffer, bufPos);

    /* Encode confRef */
    bufPos = BerEncoder_encodeUInt32WithTL(0x88, self->confRev, buffer, bufPos);

    /* Encode ndsCom */
    bufPos = BerEncoder_encodeBoolean(0x89, self->needsCommission, buffer, bufPos);

    /* Encode numDatSetEntries */
    bufPos = BerEncoder_encodeUInt32WithTL(0x8a, numberOfDataSetEntries, buffer, bufPos);

    /* Encode all data */
    bufPos = BerEncoder_encodeTL(0xab, dataSetSize, buffer, bufPos);

    /* Encode data set entries */
    element = LinkedList_getNext(dataSetValues);

    while (element != NULL) {
        MmsValue* dataSetEntry = (MmsValue*) element->data;

        if (dataSetEntry) {
            bufPos = MmsValue_encodeMmsData(dataSetEntry, buffer, bufPos, true);
        }
        else {
            /* TODO encode MMS NULL */
        }

        element = LinkedList_getNext(element);
    }

    return bufPos;
}

int
GoosePublisher_publish(GoosePublisher self, LinkedList dataSet)
{
    uint8_t* buffer = self->buffer + self->payloadStart;

    size_t maxPayloadSize = GOOSE_MAX_MESSAGE_SIZE - self->payloadStart;

    self->payloadLength = createGoosePayload(self, dataSet, buffer, maxPayloadSize);

    if (self->payloadLength == -1)
        return -1;

    self->sqNum++;

    if (self->sqNum == 0)
        self->sqNum = 1;

    int lengthIndex = self->lengthField;

    size_t gooseLength = self->payloadLength + 8;

    self->buffer[lengthIndex] = gooseLength / 256;
    self->buffer[lengthIndex + 1] = gooseLength & 0xff;

    if (DEBUG_GOOSE_PUBLISHER)
        printf("GOOSE_PUBLISHER: send GOOSE message\n");

    Ethernet_sendPacket(self->ethernetSocket, self->buffer, self->payloadStart + self->payloadLength);

    return 0;
}

int
GoosePublisher_publishAndDump(GoosePublisher self, LinkedList dataSet, char *msgBuf, int32_t *msgLen, int32_t bufSize)
{
    int rc = GoosePublisher_publish(self, dataSet);

    if (rc == 0)
            {
        int copied = self->payloadStart + self->payloadLength;

        if (bufSize < copied)
            copied = bufSize;

        memcpy(msgBuf, self->buffer, copied);
        *msgLen = copied;
    }

    return rc;
}
