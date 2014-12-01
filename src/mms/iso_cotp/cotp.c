/*
 *  cotp.c
 *
 *  ISO 8073 Connection Oriented Transport Protocol over TCP (RFC1006)
 *
 *  Partial implementation of the ISO 8073 COTP protocol for MMS.
 *
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
#include "cotp.h"
#include "byte_stream.h"
#include "byte_buffer.h"
#include "buffer_chain.h"

#define COTP_RFC1006_HEADER_SIZE 4

#define COTP_DATA_HEADER_SIZE 3

#ifdef CONFIG_COTP_MAX_TPDU_SIZE
#define COTP_MAX_TPDU_SIZE CONFIG_COTP_MAX_TPDU_SIZE
#else
#define COTP_MAX_TPDU_SIZE 8192
#endif

#ifndef DEBUG_COTP
#define DEBUG_COTP 0
#endif

static int
addPayloadToBuffer(CotpConnection* self, int rfc1006Length);

static void
writeOptions(CotpConnection* self)
{
    // max size = 11 byte
    uint8_t* buffer = self->writeBuffer->buffer;
    int bufPos = self->writeBuffer->size;

    if (self->options.tpdu_size != 0) {

        if (DEBUG_COTP)
            printf("COTP: send TPDU size: %i\n", CotpConnection_getTpduSize(self));

        buffer[bufPos++] = 0xc0;
        buffer[bufPos++] = 0x01;
        buffer[bufPos++] = self->options.tpdu_size;
    }

    if (self->options.tsap_id_dst != -1) {
        buffer[bufPos++] = 0xc2;
        buffer[bufPos++] = 0x02;
        buffer[bufPos++] = (uint8_t) (self->options.tsap_id_dst / 0x100);
        buffer[bufPos++] = (uint8_t) (self->options.tsap_id_dst & 0xff);
    }

    if (self->options.tsap_id_src != -1) {
        buffer[bufPos++] = 0xc1;
        buffer[bufPos++] = 0x02;
        buffer[bufPos++] = (uint8_t) (self->options.tsap_id_src / 0x100);
        buffer[bufPos++] = (uint8_t) (self->options.tsap_id_src & 0xff);
    }

    self->writeBuffer->size = bufPos;
}

static int
getOptionsLength(CotpConnection* self)
{
    int optionsLength = 0;
    if (self->options.tpdu_size != 0)
        optionsLength += 3;
    if (self->options.tsap_id_dst != -1)
        optionsLength += 4;
    if (self->options.tsap_id_src != -1)
        optionsLength += 4;
    return optionsLength;
}

static inline void
writeStaticConnectResponseHeader(CotpConnection* self, int optionsLength)
{
    /* always same size (7) and same position in buffer */
    uint8_t* buffer = self->writeBuffer->buffer;

    buffer[4] = 6 + optionsLength;
    buffer[5] = 0xd0;
    buffer[6] = (uint8_t) (self->srcRef / 0x100);
    buffer[7] = (uint8_t) (self->srcRef & 0xff);
    buffer[8] = (uint8_t) (self->dstRef / 0x100);
    buffer[9] = (uint8_t) (self->dstRef & 0xff);
    buffer[10] = (uint8_t) (self->protocolClass);

    self->writeBuffer->size = 11;
}

static void
writeRfc1006Header(CotpConnection* self, int len)
{
    uint8_t* buffer = self->writeBuffer->buffer;

    buffer[0] = 0x03;
    buffer[1] = 0x00;
    buffer[2] = (uint8_t) (len / 0x100);
    buffer[3] = (uint8_t) (len & 0xff);

    self->writeBuffer->size = 4;
}

static void
writeDataTpduHeader(CotpConnection* self, int isLastUnit)
{
    /* always 3 byte starting from byte 5 in buffer */
    uint8_t* buffer = self->writeBuffer->buffer;

    buffer[4] = 0x02;
    buffer[5] = 0xf0;
    if (isLastUnit)
        buffer[6] = 0x80;
    else
        buffer[6] = 0x00;

    self->writeBuffer->size = 7;
}

static bool
sendBuffer(CotpConnection* self)
{
    int writeBufferPosition = ByteBuffer_getSize(self->writeBuffer);

    bool retVal = false;

    if (Socket_write(self->socket, ByteBuffer_getBuffer(self->writeBuffer), writeBufferPosition) == writeBufferPosition)
        retVal = true;

    ByteBuffer_setSize(self->writeBuffer, 0);

    return retVal;
}

CotpIndication
CotpConnection_sendDataMessage(CotpConnection* self, BufferChain payload)
{
    int fragments = 1;

    int fragmentPayloadSize = CotpConnection_getTpduSize(self) - COTP_DATA_HEADER_SIZE;

    if (payload->length > fragmentPayloadSize) { /* Check if segmentation is required? */
        fragments = payload->length / fragmentPayloadSize;

        if ((payload->length % fragmentPayloadSize) != 0)
            fragments += 1;
    }

    int currentBufPos = 0;
    int currentLimit;
    int lastUnit;

    BufferChain currentChain = payload;
    int currentChainIndex = 0;

    if (DEBUG_COTP)
        printf("\nCOTP: nextBufferPart: len:%i partLen:%i\n", currentChain->length, currentChain->partLength);

    uint8_t* buffer = self->writeBuffer->buffer;

    while (fragments > 0) {
        if (fragments > 1) {
            currentLimit = currentBufPos + fragmentPayloadSize;
            lastUnit = 0;
        }
        else {
            currentLimit = payload->length;
            lastUnit = 1;
        }

        writeRfc1006Header(self, 7 + (currentLimit - currentBufPos));

        writeDataTpduHeader(self, lastUnit);

        int bufPos = 7;

        int i;
        for (i = currentBufPos; i < currentLimit; i++) {

            if (currentChainIndex >= currentChain->partLength) {
                currentChain = currentChain->nextPart;
                if (DEBUG_COTP)
                    printf("\nCOTP: nextBufferPart: len:%i partLen:%i\n", currentChain->length, currentChain->partLength);
                currentChainIndex = 0;
            }

            if (DEBUG_COTP)
                printf("%02x ", currentChain->buffer[currentChainIndex]);

            buffer[bufPos++] = currentChain->buffer[currentChainIndex];

            currentChainIndex++;

            currentBufPos++;
        }

        self->writeBuffer->size = bufPos;

        if (DEBUG_COTP)
            printf("COTP: Send COTP fragment %i bufpos: %i\n", fragments, currentBufPos);

        if (!sendBuffer(self))
            return ERROR;

        fragments--;
    }

    return OK;
}

static void
allocateWriteBuffer(CotpConnection* self)
{
    if (self->writeBuffer == NULL )
        self->writeBuffer = ByteBuffer_create(NULL,
                CotpConnection_getTpduSize(self) + COTP_RFC1006_HEADER_SIZE);
}

/* client side */
CotpIndication
CotpConnection_sendConnectionRequestMessage(CotpConnection* self, IsoConnectionParameters isoParameters)
{
    allocateWriteBuffer(self);

    const int CON_REQUEST_SIZE = 22;

    if(self->writeBuffer->maxSize < CON_REQUEST_SIZE)
        return ERROR;

    uint8_t* buffer = self->writeBuffer->buffer;

    writeRfc1006Header(self, CON_REQUEST_SIZE);

    /* LI */
    buffer[4] = 17;

    /* TPDU CODE */
    buffer[5] = 0xe0;

    /* DST REF */
    buffer[6] = 0x00;
    buffer[7] = 0x00;

    /* SRC REF */
    buffer[8] = 0x00;
    buffer[9] = 0x02; /* or 0x01 ? */

    /* Class */
    buffer[10] = 0x00;

    self->writeBuffer->size = 11;

    self->options.tsap_id_dst = isoParameters->remoteTSelector;
    self->options.tsap_id_src = 0;

    writeOptions(self);

    if (sendBuffer(self))
        return OK;
    else
        return ERROR;
}

CotpIndication
CotpConnection_sendConnectionResponseMessage(CotpConnection* self)
{
    allocateWriteBuffer(self);

    int optionsLength = getOptionsLength(self);
    int messageLength = 11 + optionsLength;

    writeRfc1006Header(self, messageLength);

    writeStaticConnectResponseHeader(self, optionsLength);

    writeOptions(self);

    if (sendBuffer(self))
        return OK;
    else
        return ERROR;
}

static int
parseOptions(CotpConnection* self, int opt_len)
{
    int read_bytes = 0;
    uint8_t option_type, option_len, uint8_value;
    uint16_t uint16_value;
    int i;
    Socket sock = self->socket;

    while (read_bytes < opt_len) {
        if (ByteStream_readUint8(sock, &option_type) == -1)
            goto cpo_error;
        if (ByteStream_readUint8(sock, &option_len) == -1)
            goto cpo_error;

        read_bytes += 2;

        if (DEBUG_COTP)
            printf("COTP: option: %02x len: %02x\n", option_type, option_len);

        switch (option_type) {
        case 0xc0:
			{
				if (ByteStream_readUint8(sock, &uint8_value) == -1)
					goto cpo_error;
				read_bytes++;

				int requestedTpduSize = (1 << uint8_value);

				if (DEBUG_COTP)
				    printf("COTP: requested TPDU size: %i\n", requestedTpduSize);

				CotpConnection_setTpduSize(self, requestedTpduSize);
			}
            break;
        case 0xc1:
            if (option_len == 2) {
                if (ByteStream_readUint16(sock, &uint16_value) == -1)
                    goto cpo_error;
                read_bytes += 2;
                self->options.tsap_id_src = (int32_t) uint16_value;
            } else
                goto cpo_error;
            break;
        case 0xc2:
            if (option_len == 2) {
                if (ByteStream_readUint16(sock, &uint16_value) == -1)
                    goto cpo_error;
                self->options.tsap_id_dst = (int32_t) uint16_value;
                read_bytes += 2;
            } else
                goto cpo_error;
            break;
        case 0xc6: /* additional option selection */
            if (option_len == 1) {
                ByteStream_readUint8(sock, &uint8_value); /* ignore value */
                read_bytes++;
            }
            else
                goto cpo_error;
            break;
        default:

            if (DEBUG_COTP)
                printf("COTP: Unknown option %02x\n", option_type);

            for (i = 0; i < opt_len; i++) {
                if (ByteStream_readUint8(sock, &uint8_value) == -1)
                    goto cpo_error;
            }

            read_bytes += opt_len;

            break;
        }
    }

    return 1;

    cpo_error:
    if (DEBUG_COTP)
        printf("COTP: cotp_parse_options: error parsing options!\n");
    return -1;
}

void
CotpConnection_init(CotpConnection* self, Socket socket,
        ByteBuffer* payloadBuffer)
{
    self->state = 0;
    self->socket = socket;
    self->srcRef = -1;
    self->dstRef = -1;
    self->protocolClass = -1;
	self->options.tpdu_size = 0;
	self->options.tsap_id_src = -1;
	self->options.tsap_id_dst = -1;
    self->payload = payloadBuffer;

    /* default TPDU size is maximum size */
    CotpConnection_setTpduSize(self, COTP_MAX_TPDU_SIZE);

    self->writeBuffer = NULL;
}

void
CotpConnection_destroy(CotpConnection* self)
{
    if (self->writeBuffer != NULL)
        ByteBuffer_destroy(self->writeBuffer);
}

int /* in byte */
CotpConnection_getTpduSize(CotpConnection* self)
{
    return (1 << self->options.tpdu_size);
}

void
CotpConnection_setTpduSize(CotpConnection* self, int tpduSize /* in byte */)
{
    int newTpduSize = 1;

    if (tpduSize > COTP_MAX_TPDU_SIZE)
        tpduSize = COTP_MAX_TPDU_SIZE;

    while ((1 << newTpduSize) < tpduSize)
        newTpduSize++;

    if ((1 << newTpduSize) > tpduSize)
        newTpduSize--;

    self->options.tpdu_size = newTpduSize;
}

ByteBuffer*
CotpConnection_getPayload(CotpConnection* self)
{
    return self->payload;
}

int
CotpConnection_getSrcRef(CotpConnection* self)
{
    return self->srcRef;
}

int
CotpConnection_getDstRef(CotpConnection* self)
{
    return self->dstRef;
}

/*
 CR TPDU (from RFC 0905):

 1    2        3        4       5   6    7    8    p  p+1...end
 +--+------+---------+---------+---+---+------+-------+---------+
 |LI|CR CDT|     DST - REF     |SRC-REF|CLASS |VARIAB.|USER     |
 |  |1110  |0000 0000|0000 0000|   |   |OPTION|PART   |DATA     |
 +--+------+---------+---------+---+---+------+-------+---------+
 */

static int
parseConnectRequestTpdu(CotpConnection* self, uint8_t len)
{
    uint16_t dstRef;
    uint16_t srcRef;
    uint8_t protocolClass;
    Socket sock = self->socket;

    if (ByteStream_readUint16(sock, &dstRef) != 2)
        return -1;
    else
        self->dstRef = dstRef;

    if (ByteStream_readUint16(sock, &srcRef) != 2)
        return -1;
    else
        self->srcRef = srcRef;

    if (ByteStream_readUint8(sock, &protocolClass) != 1)
        return -1;
    else
        self->protocolClass = protocolClass;

    return parseOptions(self, len - 6);
}

static int
parseConnectConfirmTpdu(CotpConnection* self, uint8_t len)
{
    uint16_t dstRef;
    uint16_t srcRef;
    uint8_t protocolClass;
    Socket sock = self->socket;

    if (ByteStream_readUint16(sock, &dstRef) != 2)
        return -1;
    else
        self->srcRef = dstRef;

    if (ByteStream_readUint16(sock, &srcRef) != 2)
        return -1;
    else
        self->dstRef = srcRef;

    if (ByteStream_readUint8(sock, &protocolClass) != 1)
        return -1;
    else
        self->protocolClass = protocolClass;

    return parseOptions(self, len - 6);
}

static int
parseDataTpdu(CotpConnection* self, uint8_t len)
{
    uint8_t flowControl;

    if (len != 2)
        return -1;

    if (ByteStream_readUint8(self->socket, &flowControl) != 1)
        return -1;
    else {
        if (flowControl & 0x80)
            self->isLastDataUnit = true;
        else
            self->isLastDataUnit = false;
    }

    return 1;
}

static CotpIndication
parseRFC1006Header(CotpConnection* self, uint16_t* rfc1006_length)
{
    Socket sock = self->socket;
    uint8_t b;

    if (ByteStream_readUint8(sock, &b) == -1)
        return ERROR;
    if (b != 3)
        return ERROR;

    if (ByteStream_readUint8(sock, &b) == -1)
        return ERROR;
    if (b != 0)
        return ERROR;

    if (ByteStream_readUint16(sock, rfc1006_length) == -1)
        return ERROR;

    return OK;
}


static CotpIndication parseIncomingMessage(CotpConnection* self);

static int
addPayloadToBuffer(CotpConnection* self, int rfc1006Length)
{
    int payloadLength = rfc1006Length - 7;

    if ((self->payload->size + payloadLength) > self->payload->maxSize)
        return 0;

    int readLength = ByteStream_readOctets(self->socket,
            self->payload->buffer + self->payload->size,
            payloadLength);

    if (readLength != payloadLength) {
        if (DEBUG_COTP)
            printf("COTP: read %i bytes should have been %i\n", readLength, payloadLength);
        return 0;
    }
    else {
        self->payload->size += payloadLength;

        if (self->isLastDataUnit == false) {
            if (parseIncomingMessage(self) == DATA_INDICATION)
                return 1;
            else
                return 0;
        }
        else
            return 1;
    }
}

static CotpIndication
parseIncomingMessage(CotpConnection* self)
{
    uint8_t len;
    uint8_t tpduType;
    uint16_t rfc1006Length;
    Socket sock = self->socket;

    if (parseRFC1006Header(self, &rfc1006Length) == ERROR)
        return ERROR;

    if (ByteStream_readUint8(sock, &len) != 1) {
        return ERROR;
    }

    if (ByteStream_readUint8(sock, &tpduType) == 1) {
        switch (tpduType) {
        case 0xe0:
            if (parseConnectRequestTpdu(self, len) == 1)
                return CONNECT_INDICATION;
            else
                return ERROR;
        case 0xd0:
            self->isLastDataUnit = true;
            if (parseConnectConfirmTpdu(self, len) == 1)
                return CONNECT_INDICATION;
            else
                return ERROR;
        case 0xf0:
            if (parseDataTpdu(self, len) == 1) {
                if (addPayloadToBuffer(self, rfc1006Length) == 1)
                    return DATA_INDICATION;
                else
                    return ERROR;
            }
            else
                return ERROR;
        default:
            return ERROR;
        }
    }
    else
        return ERROR;
}

CotpIndication
CotpConnection_parseIncomingMessage(CotpConnection* self)
{
    self->payload->size = 0;

    return parseIncomingMessage(self);
}
