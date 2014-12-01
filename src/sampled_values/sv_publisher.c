/*
 *  sv_publisher.c
 *
 *  Copyright 2013 Michael Zillgith
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

#define CONFIG_SV_DEFAULT_DST_ADDRESS CONFIG_GOOSE_DEFAULT_DST_ADDRESS

#define CONFIG_SV_DEFAULT_PRIORITY 4
#define CONFIG_SV_DEFAULT_VLAN_ID 0
#define CONFIG_SV_DEFAULT_APPID 0x4000

#define SV_MAX_MESSAGE_SIZE 1518

typedef struct sSampledValuesPublisher* SampledValuesPublisher;

struct sSampledValuesPublisher {
    uint8_t* buffer;
    uint16_t appId;
    EthernetSocket ethernetSocket;
    int lengthField; /* can probably be removed since packets have fixed size! */
    int payloadStart;

    char* id;
    char* dataSetRef; /* date set reference */

    uint16_t smpCount; /* sample counter - reset by sync */
    uint32_t confRev; /* Configuration revision according to CB */
    uint8_t smpSynch; /* Synchronization status */
    uint16_t smpRate;

    uint64_t refreshTime; /* local buffer refresh time */
    MmsValue* refTime;

    bool hasDataSetName; /* optional fields in sv asdu */
    bool hasRefreshTime;
    bool hasSampleRate;
};


static void
preparePacketBuffer(SampledValuesPublisher self, CommParameters* parameters, char* interfaceID)
{
    //TODO can GOOSE code be reused?

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

    self->buffer = (uint8_t*) malloc(SV_MAX_MESSAGE_SIZE);

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
encodeUInt16WithTLFixedSize(uint8_t tag, uint16_t value, uint8_t* buffer, int bufPos)
{
    uint8_t* valueArray = (uint8_t*) &value;

    buffer[bufPos++] = tag;
    buffer[bufPos++] = 2;

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
encodeUInt32WithTLFixedSize(uint8_t tag, uint32_t value, uint8_t* buffer, int bufPos)
{
    uint8_t* valueArray = (uint8_t*) &value;

    buffer[bufPos++] = tag;
    buffer[bufPos++] = 2;

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
determineEncodedMmsValueSize(MmsValue* value)
{
   switch (MmsValue_getType(value)) {
   case MMS_ARRAY:
   case MMS_STRUCTURE: {
	   int compCount = value->value.structure.size;
	   int i;
	   int encodedSize = 0;

	   for (i = 0; i < compCount; i++) {
		   encodedSize += determineEncodedMmsValueSize(value->value.structure.components[i]);
	   }

	   return encodedSize;
   }
   case MMS_INTEGER:
       return value->value.integer->maxSize;
       break;
   case MMS_UNSIGNED:
       return value->value.unsignedInteger->maxSize;
   case MMS_FLOAT:
       return value->value.floatingPoint.formatWidth / 8;
   case MMS_BIT_STRING:
	   return 4;
   case MMS_UTC_TIME:
	   return 8;
   case MMS_BINARY_TIME:
	   return 6;
   case MMS_BOOLEAN:
	   return 1;
   default:
	   printf("Data type not supported for sampled values service!\n");
	   return 0;
   }
}

static int
encodeMmsValue(MmsValue* value, uint8_t* buffer, int bufPos)
{
	switch (MmsValue_getType(value)) {
	   case MMS_ARRAY:
	   case MMS_STRUCTURE: {
		   int compCount = value->value.structure.size;
		   int i;

		   for (i = 0; i < compCount; i++) {
			   bufPos = encodeMmsValue(value->value.structure.components[i], buffer, bufPos);
		   }

		   return bufPos;
	   }
	}
}

static int
createSVPayload(SampledValuesPublisher self, LinkedList dataSetValues, uint8_t* buffer, int maxPayloadSize) {

    /* Step 1 - calculate length fields */
    uint32_t asduLength = 0;

    asduLength += BerEncoder_determineEncodedStringSize(self->id);

    if (self->hasDataSetName)
        asduLength += BerEncoder_determineEncodedStringSize(self->dataSetRef);

    asduLength += 4 + 6 + 3 + 4; /* for smpCnt + confRev + smpSynch + smpMod */

    if (self->hasRefreshTime)
        asduLength += 10; /* for refrTim */

    if (self->hasSampleRate)
        asduLength += 4; /* for smpRate */

    uint32_t numberOfDataSetEntries = LinkedList_size(dataSetValues);

    asduLength += 2 + BerEncoder_UInt32determineEncodedSize(numberOfDataSetEntries);

    uint32_t dataSetSize = 0;

    LinkedList element = LinkedList_getNext(dataSetValues);

    while (element != NULL) {
        MmsValue* dataSetEntry = (MmsValue*) element->data;

        dataSetSize += determineEncodedMmsValueSize(dataSetEntry);

        element = LinkedList_getNext(element);
    }

    uint32_t allDataSize = dataSetSize + BerEncoder_determineLengthSize(dataSetSize) + 1;

    asduLength += allDataSize;

    uint32_t sequenceOfAsduLength = 1 + BerEncoder_DetermineLengthSize(asduLength) + asduLength;

    uint32_t apduLength = 1 + BerEncoder_DetermineLengthSize(sequenceOfAsduLength) + sequenceOfAsduLength;

    //TODO add length of ASDU header

    if (apduLength > maxPayloadSize)
        return -1;

    /* Step 2 - encode to buffer */

    int bufPos = 0;

    /* Encode SV PDU */
    bufPos = BerEncoder_encodeTL(0x60, apduLength, buffer, bufPos);

    /* Encode No. of ASDUs */
    bufPos = encodeUInt16WithTLFixedSize(0x80, 1, buffer, bufPos);
    //bufPos = BerEncoder_encodeUInt32WithTL(0x80, 1, buffer, bufPos);

    /* Encode Sequence of ASDUs */
    bufPos = BerEncoder_encodeTL(0xa2, sequenceOfAsduLength, buffer, bufPos);

    /* Encode ASDU */
    bufPos = BerEncoder_encodeTL(0x30, asduLength, buffer, bufPos);

    /* Encode svID */
    bufPos = BerEncoder_encodeStringWithTag(0x80, self->id, buffer, bufPos);

    /* Encode dataSetRef */
    if (self->hasDataSetName)
        bufPos = BerEncoder_encodeStringWithTag(0x81, self->dataSetRef, buffer, bufPos);

    /* Encode smpCnt */
    bufPos = encodeUInt16WithTLFixedSize(0x82, self->smpCount, buffer, bufPos);

    /* Encode confRev */
    bufPos = encodeUInt32WithTLFixedSize(0x83, self->confRev, buffer, bufPos);

    /* Encode refreshTime */
    if (self->hasRefreshTime) {
    	MmsValue_setUtcTimeMs(self->refTime, self->refreshTime);
    	bufPos = BerEncoder_encodeOctetString(0x84, self->refTime->value.utcTime, 8, buffer, bufPos);
    }

    /* Encode smpSynch */
    bufPos = BerEncoder_encodeTL(0x85, dataSetSize, buffer, bufPos);
    buffer[bufPos++] = self->smpSynch;

    /* Encode smpRate */
    if (self->hasSampleRate) {
    	bufPos = encodeUInt16WithTLFixedSize(0x86, self->smpRate, buffer, bufPos);
    }

    /* Encode sequence of data */
    bufPos = BerEncoder_encodeTL(0x87, dataSetSize, buffer, bufPos);

    /* Encode data set entries */
    element = LinkedList_getNext(dataSetValues);

    while (element != NULL) {
        MmsValue* dataSetEntry = (MmsValue*) element->data;

        bufPos = mmsServer_encodeAccessResult(dataSetEntry, buffer, bufPos, true);

        element = LinkedList_getNext(element);
    }

    return bufPos;
}
