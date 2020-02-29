/*
 *  sv_publisher.c
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

#include "stack_config.h"
#include "libiec61850_platform_includes.h"

#include <stdbool.h>
#include "sv_publisher.h"

#include "hal_ethernet.h"
#include "ber_encoder.h"

#ifndef DEBUG_SV_PUBLISHER
#define DEBUG_SV_PUBLISHER 1
#endif

#define CONFIG_SV_DEFAULT_DST_ADDRESS CONFIG_GOOSE_DEFAULT_DST_ADDRESS

#define CONFIG_SV_DEFAULT_PRIORITY 4
#define CONFIG_SV_DEFAULT_VLAN_ID 0
#define CONFIG_SV_DEFAULT_APPID 0x4000

#define SV_MAX_MESSAGE_SIZE 1518

struct sSVPublisher_ASDU {
    const char* svID;
    const char* datset;
    int dataSize;

    bool hasRefrTm;
    bool hasSmpRate;
    bool hasSmpMod;

    uint8_t* _dataBuffer;

    uint8_t smpSynch;
    uint16_t smpCnt;
    uint16_t smpCntLimit;
    uint32_t confRev;

    Timestamp* refrTm;
    uint8_t smpMod;
    uint16_t smpRate;

    uint8_t* smpCntBuf;
    uint8_t* smpSynchBuf;

    SVPublisher_ASDU _next;
};

struct sSVPublisher {
    uint8_t* buffer;
    uint16_t appId;
    EthernetSocket ethernetSocket;

    int lengthField; /* can probably be removed since packets have fixed size! */
    int payloadStart;

    int payloadLength; /* length of payload buffer */

    int asduCount; /* number of ASDUs in the APDU */
    SVPublisher_ASDU asduList;
};


static bool
preparePacketBuffer(SVPublisher self, CommParameters* parameters, const char* interfaceId, bool useVlanTags)
{
    uint8_t defaultDstAddr[] = CONFIG_SV_DEFAULT_DST_ADDRESS;

    uint8_t* dstAddr;
    uint8_t priority;
    uint16_t vlanId;
    uint16_t appId;

    uint8_t srcAddr[6];

    if (interfaceId != NULL)
        Ethernet_getInterfaceMACAddress(interfaceId, srcAddr);
    else
        Ethernet_getInterfaceMACAddress(CONFIG_ETHERNET_INTERFACE_ID, srcAddr);

    if (parameters == NULL) {
        dstAddr = defaultDstAddr;
        priority = CONFIG_SV_DEFAULT_PRIORITY;
        vlanId = CONFIG_SV_DEFAULT_VLAN_ID;
        appId = CONFIG_SV_DEFAULT_APPID;
    }
    else {
        dstAddr = parameters->dstAddress;
        priority = parameters->vlanPriority;
        vlanId = parameters->vlanId;
        appId = parameters->appId;
    }

    if (interfaceId != NULL)
        self->ethernetSocket = Ethernet_createSocket(interfaceId, dstAddr);
    else
        self->ethernetSocket = Ethernet_createSocket(CONFIG_ETHERNET_INTERFACE_ID, dstAddr);

    if (self->ethernetSocket == NULL) {

        if (DEBUG_SV_PUBLISHER)
            printf("SV_PUBLISHER: Failed to allocate Ethernet interface\n");

        return false;
    }

    self->buffer = (uint8_t*) GLOBAL_MALLOC(SV_MAX_MESSAGE_SIZE);

    if (self->buffer) {
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

        /* EtherType Sampled Values */
        self->buffer[bufPos++] = 0x88;
        self->buffer[bufPos++] = 0xBa;

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
    }
    else {
        return false;
    }

    return true;
}


static int
encodeUInt16FixedSize(uint16_t value, uint8_t* buffer, int bufPos)
{
    uint8_t* valueArray = (uint8_t*) &value;

#if (ORDER_LITTLE_ENDIAN == 1)
    buffer[bufPos++] = valueArray[1];
    buffer[bufPos++] = valueArray[0];
#else
    buffer[bufPos++] = valueArray[0];
    buffer[bufPos++] = valueArray[1];
#endif

    return bufPos;
}

static int
encodeUInt32FixedSize(uint32_t value, uint8_t* buffer, int bufPos)
{
    uint8_t* valueArray = (uint8_t*) &value;

#if (ORDER_LITTLE_ENDIAN == 1)
    buffer[bufPos++] = valueArray[3];
    buffer[bufPos++] = valueArray[2];
    buffer[bufPos++] = valueArray[1];
    buffer[bufPos++] = valueArray[0];
#else
    buffer[bufPos++] = valueArray[0];
    buffer[bufPos++] = valueArray[1];
    buffer[bufPos++] = valueArray[2];
    buffer[bufPos++] = valueArray[3];
#endif

    return bufPos;
}

static int
encodeInt32FixedSize(int32_t value, uint8_t* buffer, int bufPos)
{
    uint8_t* valueArray = (uint8_t*) &value;

#if (ORDER_LITTLE_ENDIAN == 1)
    buffer[bufPos++] = valueArray[3];
    buffer[bufPos++] = valueArray[2];
    buffer[bufPos++] = valueArray[1];
    buffer[bufPos++] = valueArray[0];
#else
    buffer[bufPos++] = valueArray[0];
    buffer[bufPos++] = valueArray[1];
    buffer[bufPos++] = valueArray[2];
    buffer[bufPos++] = valueArray[3];
#endif

    return bufPos;
}

static int
encodeInt64FixedSize(int64_t value, uint8_t* buffer, int bufPos)
{
    uint8_t* valueArray = (uint8_t*) &value;

#if (ORDER_LITTLE_ENDIAN == 1)
    buffer[bufPos++] = valueArray[7];
    buffer[bufPos++] = valueArray[6];
    buffer[bufPos++] = valueArray[5];
    buffer[bufPos++] = valueArray[4];
    buffer[bufPos++] = valueArray[3];
    buffer[bufPos++] = valueArray[2];
    buffer[bufPos++] = valueArray[1];
    buffer[bufPos++] = valueArray[0];
#else
    buffer[bufPos++] = valueArray[0];
    buffer[bufPos++] = valueArray[1];
    buffer[bufPos++] = valueArray[2];
    buffer[bufPos++] = valueArray[3];
    buffer[bufPos++] = valueArray[4];
    buffer[bufPos++] = valueArray[5];
    buffer[bufPos++] = valueArray[6];
    buffer[bufPos++] = valueArray[7];
#endif

    return bufPos;
}

SVPublisher
SVPublisher_createEx(CommParameters* parameters, const char* interfaceId, bool useVlanTag)
{
    SVPublisher self = (SVPublisher) GLOBAL_CALLOC(1, sizeof(struct sSVPublisher));

    if (self) {
        self->asduList = NULL;

        if (preparePacketBuffer(self, parameters, interfaceId, useVlanTag) == false) {
            SVPublisher_destroy(self);
            self = NULL;
        }

    }

    return self;
}

SVPublisher
SVPublisher_create(CommParameters* parameters, const char* interfaceId)
{
    return SVPublisher_createEx(parameters, interfaceId, true);
}

SVPublisher_ASDU
SVPublisher_addASDU(SVPublisher self, const char* svID, const char* datset, uint32_t confRev)
{
    SVPublisher_ASDU newAsdu = (SVPublisher_ASDU) GLOBAL_CALLOC(1, sizeof(struct sSVPublisher_ASDU));

    newAsdu->svID = svID;
    newAsdu->datset = datset;
    newAsdu->confRev = confRev;
    newAsdu->smpCntLimit = UINT16_MAX;
    newAsdu->_next = NULL;

    /* append new ASDU to list */
    if (self->asduList == NULL)
        self->asduList = newAsdu;
    else {
        SVPublisher_ASDU lastAsdu = self->asduList;

        while (lastAsdu->_next != NULL)
            lastAsdu = lastAsdu->_next;

        lastAsdu->_next = newAsdu;
    }

    return newAsdu;
}

static int
SVPublisher_ASDU_getEncodedSize(SVPublisher_ASDU self)
{
    int encodedSize = 0;

    /* svID */
    encodedSize += ( 2 + strlen(self->svID) );

    /* datset */
    if (self->datset != NULL)
        encodedSize += ( 2 + strlen(self->datset) );

    /* smpCnt */
    encodedSize += 4;

    /* confRef */
    encodedSize += 6;

    /* refrTm */
    if (self->hasRefrTm)
        encodedSize += 10; /* ??? */

    /* smpSynch */
    encodedSize += 3;

    /* smpRate */
    if (self->hasSmpRate)
        encodedSize += 4;

    /* sample */
    encodedSize += (1 + BerEncoder_determineLengthSize(self->dataSize));
    encodedSize += self->dataSize;

    /* smpMod */
    if (self->hasSmpMod)
        encodedSize += 4;

    return encodedSize;
}

static int
SVPublisher_ASDU_encodeToBuffer(SVPublisher_ASDU self, uint8_t* buffer, int bufPos)
{
    int encodedSize = SVPublisher_ASDU_getEncodedSize(self);

    /* tag and length field */
    bufPos = BerEncoder_encodeTL(0x30, encodedSize, buffer, bufPos);

    /* svID */
    bufPos = BerEncoder_encodeStringWithTag(0x80, self->svID, buffer, bufPos);

    /* DatSet */
    if (self->datset != NULL)
        bufPos = BerEncoder_encodeStringWithTag(0x81, self->datset, buffer, bufPos);

    /* SmpCnt */
    bufPos = BerEncoder_encodeTL(0x82, 2, buffer, bufPos);
    self->smpCntBuf = buffer + bufPos;
    bufPos = encodeUInt16FixedSize(self->smpCnt, buffer, bufPos);

    /* ConfRev */
    bufPos = BerEncoder_encodeTL(0x83, 4, buffer, bufPos);
    bufPos = encodeUInt32FixedSize(self->confRev, buffer, bufPos);

    /* RefrTm */
    if (self->hasRefrTm) {
        bufPos = BerEncoder_encodeTL(0x84, 8, buffer, bufPos);
        self->refrTm = (Timestamp*) (buffer + bufPos);
        bufPos += 8;
    }

    /* SmpSynch */
    bufPos = BerEncoder_encodeTL(0x85, 1, buffer, bufPos);
    self->smpSynchBuf = buffer + bufPos;
    buffer[bufPos++] = self->smpSynch;

    /* SmpRate */
    if (self->hasSmpRate) {
        bufPos = BerEncoder_encodeTL(0x86, 2, buffer, bufPos);
        bufPos = encodeUInt16FixedSize(self->smpRate, buffer, bufPos);
    }

    /* Sample */
    bufPos = BerEncoder_encodeTL(0x87, self->dataSize, buffer, bufPos);

    self->_dataBuffer = buffer + bufPos;

    bufPos += self->dataSize; /* data has to be inserted by user before sending message */
    
    /* SmpMod */
    if (self->hasSmpMod) {
        bufPos = BerEncoder_encodeTL(0x88, 2, buffer, bufPos);
        bufPos = encodeUInt16FixedSize(self->smpMod, buffer, bufPos);
    }

    return bufPos;
}

void
SVPublisher_setupComplete(SVPublisher self)
{
    int numberOfAsdu = 0;

    /* determine number of ASDUs and length of all ASDUs */
    SVPublisher_ASDU nextAsdu = self->asduList;
    int totalASDULength = 0;

    while (nextAsdu != NULL) {
        numberOfAsdu++;
        int asduLength = SVPublisher_ASDU_getEncodedSize(nextAsdu);

        /* tag and length field */
        asduLength += BerEncoder_determineLengthSize(asduLength);
        asduLength++;

        totalASDULength += asduLength;

        nextAsdu = nextAsdu->_next;
    }

    /* encode frame to buffer */
    int sequenceSize = 1 + BerEncoder_determineLengthSize(totalASDULength) + totalASDULength;

    int innerSize = 2 + BerEncoder_UInt32determineEncodedSize(numberOfAsdu) + sequenceSize;

    uint8_t* buffer = self->buffer + self->payloadStart;

    int bufPos = BerEncoder_encodeTL(0x60, innerSize, buffer, 0);

    /* noASDU */
    bufPos = BerEncoder_encodeUInt32WithTL(0x80, numberOfAsdu, buffer, bufPos);

    /* seqASDU */
    bufPos = BerEncoder_encodeTL(0xa2, totalASDULength, buffer, bufPos);

    nextAsdu = self->asduList;

    while (nextAsdu != NULL) {
        bufPos = SVPublisher_ASDU_encodeToBuffer(nextAsdu, buffer, bufPos);

        nextAsdu = nextAsdu->_next;
    }

    /* Update length field */
    int payloadLength = bufPos;

    size_t msgLength = payloadLength + 8;

    int lengthIndex = self->lengthField;

    self->buffer[lengthIndex] = msgLength / 256;
    self->buffer[lengthIndex + 1] = msgLength & 0xff;

    self->payloadLength = payloadLength;

}

void
SVPublisher_publish(SVPublisher self)
{
    if (DEBUG_SV_PUBLISHER)
        printf("SV_PUBLISHER: send SV message\n");

    Ethernet_sendPacket(self->ethernetSocket, self->buffer, self->payloadStart + self->payloadLength);
}

void
SVPublisher_destroy(SVPublisher self)
{
    if (self->ethernetSocket)
        Ethernet_destroySocket(self->ethernetSocket);

    if (self->buffer)
        GLOBAL_FREEMEM(self->buffer);

    SVPublisher_ASDU asdu = self->asduList;

    while (asdu) {
        SVPublisher_ASDU nextAsdu = asdu->_next;

        GLOBAL_FREEMEM(asdu);

        asdu = nextAsdu;
    }

    GLOBAL_FREEMEM(self);
}


void
SVPublisher_ASDU_resetBuffer(SVPublisher_ASDU self)
{
    self->dataSize = 0;
}


int
SVPublisher_ASDU_addINT8(SVPublisher_ASDU self)
{
    int index = self->dataSize;

    self->dataSize += 1;

    return index;
}

void
SVPublisher_ASDU_setINT8(SVPublisher_ASDU self, int index, int8_t value)
{
    self->_dataBuffer[index] = value;
}

int
SVPublisher_ASDU_addINT32(SVPublisher_ASDU self)
{
    int index = self->dataSize;

    self->dataSize += 4;

    return index;
}

void
SVPublisher_ASDU_setINT32(SVPublisher_ASDU self, int index, int32_t value)
{
    encodeInt32FixedSize(value, self->_dataBuffer, index);
}

int
SVPublisher_ASDU_addINT64(SVPublisher_ASDU self)
{
    int index = self->dataSize;

    self->dataSize += 8;

    return index;
}

void
SVPublisher_ASDU_setINT64(SVPublisher_ASDU self, int index, int64_t value)
{
    encodeInt64FixedSize(value, self->_dataBuffer, index);
}

int
SVPublisher_ASDU_addFLOAT(SVPublisher_ASDU self)
{
    int index = self->dataSize;

    self->dataSize += 4;

    return index;
}

void
SVPublisher_ASDU_setFLOAT(SVPublisher_ASDU self, int index, float value)
{
    uint8_t* buf = (uint8_t*) &value;

#if (ORDER_LITTLE_ENDIAN == 1)
    BerEncoder_revertByteOrder(buf, 4);
#endif

    int i;

    uint8_t* buffer = self->_dataBuffer + index;

    for (i = 0; i < 4; i++) {
        buffer[i] = buf[i];
    }
}


int
SVPublisher_ASDU_addFLOAT64(SVPublisher_ASDU self)
{
    int index = self->dataSize;
    self->dataSize += 8;
    return index;
}

void
SVPublisher_ASDU_setFLOAT64(SVPublisher_ASDU self, int index, double value)
{
    uint8_t* buf = (uint8_t*) &value;

#if (ORDER_LITTLE_ENDIAN == 1)
    BerEncoder_revertByteOrder(buf, 8);
#endif

    int i;

    uint8_t* buffer = self->_dataBuffer + index;

    for (i = 0; i < 8; i++) {
        buffer[i] = buf[i];
    }
}

int
SVPublisher_ASDU_addTimestamp(SVPublisher_ASDU self)
{
    int index = self->dataSize;
    self->dataSize += 8;
    return index;
}

void
SVPublisher_ASDU_setTimestamp(SVPublisher_ASDU self, int index, Timestamp value)
{
    int i;

    uint8_t* buffer = self->_dataBuffer + index;

    for (i = 0; i < 8; i++) {
        buffer[i] = value.val[i];
    }
}

int
SVPublisher_ASDU_addQuality(SVPublisher_ASDU self)
{
    int index = self->dataSize;
    self->dataSize += 4;
    return index;
}

void
SVPublisher_ASDU_setQuality(SVPublisher_ASDU self, int index, Quality value)
{
    uint8_t* buffer = self->_dataBuffer + index;
    buffer[0] = 0;
    buffer[1] = 0;
    buffer[2] = value / 0x100;
    buffer[3] = value % 0x100;
}

uint16_t
SVPublisher_ASDU_getSmpCnt(SVPublisher_ASDU self)
{
    return self->smpCnt;
}

void
SVPublisher_ASDU_setSmpCnt(SVPublisher_ASDU self, uint16_t value)
{
    self->smpCnt = value;

    if (self->smpCntBuf != NULL)
        encodeUInt16FixedSize(self->smpCnt, self->smpCntBuf, 0);
}

void
SVPublisher_ASDU_setSmpCntWrap(SVPublisher_ASDU self, uint16_t value)
{
    self->smpCntLimit = value;
}

void
SVPublisher_ASDU_increaseSmpCnt(SVPublisher_ASDU self)
{
    self->smpCnt = ((self->smpCnt + 1) % self->smpCntLimit);

    if (self->smpCntBuf != NULL)
        encodeUInt16FixedSize(self->smpCnt, self->smpCntBuf, 0);
}

void
SVPublisher_ASDU_enableRefrTm(SVPublisher_ASDU self)
{
    self->hasRefrTm = true;
}

void
SVPublisher_ASDU_setRefrTmNs(SVPublisher_ASDU self, nsSinceEpoch refrTmNs)
{
    self->hasRefrTm = true;

    if (self->refrTm) {
        Timestamp_setTimeInNanoseconds(self->refrTm, refrTmNs);
        Timestamp_setSubsecondPrecision(self->refrTm, 20);
    }
}

void
SVPublisher_ASDU_setRefrTm(SVPublisher_ASDU self, msSinceEpoch refrTm)
{
    self->hasRefrTm = true;

    if (self->refrTm) {
        Timestamp_setTimeInMilliseconds(self->refrTm, refrTm);
        Timestamp_setSubsecondPrecision(self->refrTm, 10);
    }
}

void
SVPublisher_ASDU_setRefrTmByTimestamp(SVPublisher_ASDU self, Timestamp* refrTm)
{
    self->hasRefrTm = true;

    if (self->refrTm)
        memcpy(self->refrTm, refrTm, 8);
}

void
SVPublisher_ASDU_setSmpMod(SVPublisher_ASDU self, uint8_t smpMod)
{
    self->hasSmpMod = true;
    self->smpMod = smpMod;
}

void
SVPublisher_ASDU_setSmpRate(SVPublisher_ASDU self, uint16_t smpRate)
{
    self->hasSmpRate = true;
    self->smpRate = smpRate;
}

void
SVPublisher_ASDU_setSmpSynch(SVPublisher_ASDU self, uint16_t smpSynch)
{
    self->smpSynch = smpSynch;
    *(self->smpSynchBuf) = self->smpSynch;
}

/*******************************************************************
 * Wrapper functions to support old API (remove in future versions)
 *******************************************************************/

SVPublisher
SampledValuesPublisher_create(CommParameters* parameters, const char* interfaceId)
{
    return SVPublisher_create(parameters, interfaceId);
}

SVPublisher_ASDU
SampledValuesPublisher_addASDU(SVPublisher self, char* svID, char* datset, uint32_t confRev)
{
    return SVPublisher_addASDU(self, svID, datset, confRev);
}

void
SampledValuesPublisher_setupComplete(SVPublisher self)
{
    SVPublisher_setupComplete(self);
}

void
SampledValuesPublisher_publish(SVPublisher self)
{
    SVPublisher_publish(self);
}

void
SampledValuesPublisher_destroy(SVPublisher self)
{
    SVPublisher_destroy(self);
}

void
SV_ASDU_resetBuffer(SVPublisher_ASDU self)
{
    SVPublisher_ASDU_resetBuffer(self);
}

int
SV_ASDU_addINT8(SVPublisher_ASDU self)
{
    return SVPublisher_ASDU_addINT8(self);
}

void
SV_ASDU_setINT8(SVPublisher_ASDU self, int index, int8_t value)
{
    SVPublisher_ASDU_setINT8(self, index, value);
}

int
SV_ASDU_addINT32(SVPublisher_ASDU self)
{
    return SVPublisher_ASDU_addINT32(self);
}

void
SV_ASDU_setINT32(SVPublisher_ASDU self, int index, int32_t value)
{
    SVPublisher_ASDU_setINT32(self, index, value);
}

int
SV_ASDU_addINT64(SVPublisher_ASDU self)
{
    return SVPublisher_ASDU_addINT64(self);
}

void
SV_ASDU_setINT64(SVPublisher_ASDU self, int index, int64_t value)
{
    SVPublisher_ASDU_setINT64(self, index, value);
}

int
SV_ASDU_addFLOAT(SVPublisher_ASDU self)
{
    return SVPublisher_ASDU_addFLOAT(self);
}

void
SV_ASDU_setFLOAT(SVPublisher_ASDU self, int index, float value)
{
    SVPublisher_ASDU_setFLOAT(self, index, value);
}

int
SV_ASDU_addFLOAT64(SVPublisher_ASDU self)
{
    return SVPublisher_ASDU_addFLOAT64(self);
}

void
SV_ASDU_setFLOAT64(SVPublisher_ASDU self, int index, double value)
{
    SVPublisher_ASDU_setFLOAT64(self, index, value);
}

void
SV_ASDU_setSmpCnt(SVPublisher_ASDU self, uint16_t value)
{
    SVPublisher_ASDU_setSmpCnt(self, value);
}

uint16_t
SV_ASDU_getSmpCnt(SVPublisher_ASDU self)
{
    return SVPublisher_ASDU_getSmpCnt(self);
}

void
SV_ASDU_increaseSmpCnt(SVPublisher_ASDU self)
{
    SVPublisher_ASDU_increaseSmpCnt(self);
}

void
SV_ASDU_setRefrTm(SVPublisher_ASDU self, uint64_t refrTm)
{
    SVPublisher_ASDU_setRefrTm(self, refrTm);
}

void
SV_ASDU_setSmpMod(SVPublisher_ASDU self, uint8_t smpMod)
{
    SVPublisher_ASDU_setSmpMod(self, smpMod);
}

void
SV_ASDU_setSmpRate(SVPublisher_ASDU self, uint16_t smpRate)
{
    SVPublisher_ASDU_setSmpRate(self, smpRate);
}
