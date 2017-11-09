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

struct sSV_ASDU {
    char* svID;
    char* datset;
    int dataSize;

    bool hasRefrTm;
    bool hasSmpRate;
    bool hasSmpMod;

    uint8_t* _dataBuffer;

    uint8_t smpSynch;
    uint16_t smpCnt;
    uint32_t confRev;

    uint64_t refrTm;
    uint8_t smpMod;

    uint8_t* smpCntBuf;

    SV_ASDU _next;
};



struct sSampledValuesPublisher {
    uint8_t* buffer;
    uint16_t appId;
    EthernetSocket ethernetSocket;

    int lengthField; /* can probably be removed since packets have fixed size! */
    int payloadStart;

    int payloadLength; /* length of payload buffer */

    int asduCount; /* number of ASDUs in the APDU */
    SV_ASDU asduLIst;


};


static void
preparePacketBuffer(SampledValuesPublisher self, CommParameters* parameters, const char* interfaceID)
{
    uint8_t srcAddr[6];

    if (interfaceID != NULL)
        Ethernet_getInterfaceMACAddress(interfaceID, srcAddr);
    else
        Ethernet_getInterfaceMACAddress(CONFIG_ETHERNET_INTERFACE_ID, srcAddr);

    uint8_t defaultDstAddr[] = CONFIG_SV_DEFAULT_DST_ADDRESS;

    uint8_t* dstAddr;
    uint8_t priority;
    uint16_t vlanId;
    uint16_t appId;

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

    if (interfaceID != NULL)
        self->ethernetSocket = Ethernet_createSocket(interfaceID, dstAddr);
    else
        self->ethernetSocket = Ethernet_createSocket(CONFIG_ETHERNET_INTERFACE_ID, dstAddr);

    self->buffer = (uint8_t*) GLOBAL_MALLOC(SV_MAX_MESSAGE_SIZE);

    memcpy(self->buffer, dstAddr, 6);
    memcpy(self->buffer + 6, srcAddr, 6);

    int bufPos = 12;

    /* Priority tag - IEEE 802.1Q */
    self->buffer[bufPos++] = 0x81;
    self->buffer[bufPos++] = 0x00;

    uint8_t tci1 = priority << 5;
    tci1 += vlanId / 256;

    uint8_t tci2 = vlanId % 256;

    self->buffer[bufPos++] = tci1; /* Priority + VLAN-ID */
    self->buffer[bufPos++] = tci2; /* VLAN-ID */

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
encodeUtcTime(uint64_t timeval, uint8_t* buffer, int bufPos)
{
    uint32_t timeval32 = (timeval / 1000LL);

    uint8_t* timeArray = (uint8_t*) &timeval32;
    uint8_t* valueArray = buffer + bufPos;

#if (ORDER_LITTLE_ENDIAN == 1)
    valueArray[0] = timeArray[3];
    valueArray[1] = timeArray[2];
    valueArray[2] = timeArray[1];
    valueArray[3] = timeArray[0];
#else
    valueArray[0] = timeArray[0];
    valueArray[1] = timeArray[1];
    valueArray[2] = timeArray[2];
    valueArray[3] = timeArray[3];
#endif

    uint32_t remainder = (timeval % 1000LL);
    uint32_t fractionOfSecond = (remainder) * 16777 + ((remainder * 216) / 1000);

    /* encode fraction of second */
    valueArray[4] = ((fractionOfSecond >> 16) & 0xff);
    valueArray[5] = ((fractionOfSecond >> 8) & 0xff);
    valueArray[6] = (fractionOfSecond & 0xff);

    /* encode time quality */
    valueArray[7] = 0x0a; /* 10 bit sub-second time accuracy */

    return bufPos + 8;
}

SampledValuesPublisher
SampledValuesPublisher_create(CommParameters* parameters, const char* interfaceId)
{
    SampledValuesPublisher self = (SampledValuesPublisher) GLOBAL_CALLOC(1, sizeof(struct sSampledValuesPublisher));

    self->asduLIst = NULL;

    preparePacketBuffer(self, parameters, interfaceId);

    return self;
}

SV_ASDU
SampledValuesPublisher_addASDU(SampledValuesPublisher self, char* svID, char* datset, uint32_t confRev)
{
    SV_ASDU newAsdu = (SV_ASDU) GLOBAL_CALLOC(1, sizeof(struct sSV_ASDU));

    newAsdu->svID = svID;
    newAsdu->datset = datset;
    newAsdu->confRev = confRev;

    newAsdu->_next = NULL;

    /* append new ASDU to list */
    if (self->asduLIst == NULL)
        self->asduLIst = newAsdu;
    else {
        SV_ASDU lastAsdu = self->asduLIst;

        while (lastAsdu->_next != NULL)
            lastAsdu = lastAsdu->_next;

        lastAsdu->_next = newAsdu;
    }

    return newAsdu;
}

static int
SV_ASDU_getEncodedSize(SV_ASDU self)
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
    encodedSize += 2;
    encodedSize += self->dataSize;

    /* smpMod */
    if (self->hasSmpMod)
        encodedSize += 4;

    return encodedSize;
}

static int
SV_ASDU_encodeToBuffer(SV_ASDU self, uint8_t* buffer, int bufPos)
{
    int encodedSize = SV_ASDU_getEncodedSize(self);

    /* tag and length field */
    bufPos = BerEncoder_encodeTL(0x30, encodedSize, buffer, bufPos);

    /* svID */
    bufPos = BerEncoder_encodeStringWithTag(0x80, self->svID, buffer, bufPos);

    /* DatSet */
    if (self->datset != NULL)
        bufPos = BerEncoder_encodeStringWithTag(0x81, self->datset, buffer, bufPos);

    //uint8_t octetString[4];

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
        bufPos = encodeUtcTime(self->refrTm, buffer, bufPos);
    }

    /* SmpSynch */
    bufPos = BerEncoder_encodeTL(0x85, 1, buffer, bufPos);
    buffer[bufPos++] = self->smpSynch;

    /* SmpRate */
    //TODO implement me

    /* Sample */
    bufPos = BerEncoder_encodeTL(0x87, self->dataSize, buffer, bufPos);

    self->_dataBuffer = buffer + bufPos;

    bufPos += self->dataSize; /* data has to inserted by user before sending message */

    /* SmpMod */
    if (self->hasSmpMod) {
        bufPos = BerEncoder_encodeTL(0x84, 4, buffer, bufPos);
        buffer[bufPos++] = 0;
        buffer[bufPos++] = 0;
        buffer[bufPos++] = 0;
        buffer[bufPos++] = self->smpMod;

    }

    return bufPos;
}

void
SampledValuesPublisher_setupComplete(SampledValuesPublisher self)
{
    int numberOfAsdu = 0;

    /* determine number of ASDUs and length of all ASDUs */
    SV_ASDU nextAsdu = self->asduLIst;
    int totalASDULength = 0;

    while (nextAsdu != NULL) {
        numberOfAsdu++;
        int asduLength = SV_ASDU_getEncodedSize(nextAsdu);

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

    nextAsdu = self->asduLIst;

    while (nextAsdu != NULL) {
        bufPos = SV_ASDU_encodeToBuffer(nextAsdu, buffer, bufPos);

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
SampledValuesPublisher_publish(SampledValuesPublisher self)
{
    if (DEBUG_SV_PUBLISHER)
        printf("SV_PUBLISHER: send SV message\n");

    Ethernet_sendPacket(self->ethernetSocket, self->buffer, self->payloadStart + self->payloadLength);

}


void
SampledValuesPublisher_destroy(SampledValuesPublisher self)
{
    GLOBAL_FREEMEM(self->buffer);
}


void
SV_ASDU_resetBuffer(SV_ASDU self)
{
    self->dataSize = 0;
}


int
SV_ASDU_addINT8(SV_ASDU self)
{
    int index = self->dataSize;

    self->dataSize += 1;

    return index;
}

void
SV_ASDU_setINT8(SV_ASDU self, int index, int8_t value)
{
    self->_dataBuffer[index] = value;
}

int
SV_ASDU_addINT32(SV_ASDU self)
{
    int index = self->dataSize;

    self->dataSize += 4;

    return index;
}

void
SV_ASDU_setINT32(SV_ASDU self, int index, int32_t value)
{
    encodeInt32FixedSize(value, self->_dataBuffer, index);
}

int
SV_ASDU_addFLOAT(SV_ASDU self)
{
    int index = self->dataSize;

    self->dataSize += 4;

    return index;
}

void
SV_ASDU_setFLOAT(SV_ASDU self, int index, float value)
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
SV_ASDU_addFLOAT64(SV_ASDU self)
{
    int index = self->dataSize;
    self->dataSize += 8;
    return index;
}

void
SV_ASDU_setFLOAT64(SV_ASDU self, int index, double value)
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

uint16_t
SV_ASDU_getSmpCnt(SV_ASDU self)
{
    return self->smpCnt;
}

void
SV_ASDU_setSmpCnt(SV_ASDU self, uint16_t value)
{
    self->smpCnt = value;

    encodeUInt16FixedSize(self->smpCnt, self->smpCntBuf, 0);
}

void
SV_ASDU_increaseSmpCnt(SV_ASDU self)
{
    self->smpCnt++;

    encodeUInt16FixedSize(self->smpCnt, self->smpCntBuf, 0);
}

void
SV_ASDU_setRefrTm(SV_ASDU self, uint64_t refrTm)
{
    self->hasRefrTm = true;
    self->refrTm = refrTm;
}

void
SV_ASDU_setSmpMod(SV_ASDU self, uint8_t smpMod)
{
    self->hasSmpMod = true;
    self->smpMod = smpMod;
}

