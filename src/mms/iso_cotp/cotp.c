/*
 *  cotp.c
 *
 *  ISO 8073 Connection Oriented Transport Protocol over TCP (RFC1006)
 *
 *  Partial implementation of the ISO 8073 COTP (ISO TP0) protocol for MMS.
 *
 *  Copyright 2013-2024 Michael Zillgith
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
#include "cotp.h"
#include "byte_buffer.h"
#include "buffer_chain.h"

#define TPKT_RFC1006_HEADER_SIZE 4

#define COTP_DATA_HEADER_SIZE 3

#ifdef CONFIG_COTP_MAX_TPDU_SIZE
#define COTP_MAX_TPDU_SIZE CONFIG_COTP_MAX_TPDU_SIZE
#else
#define COTP_MAX_TPDU_SIZE 8192
#endif

#ifndef DEBUG_COTP
#define DEBUG_COTP 0
#endif

static bool
addPayloadToBuffer(CotpConnection* self, uint8_t* buffer, int payloadLength);

static uint16_t
getUint16(uint8_t* buffer)
{
    return (buffer[0] * 0x100) + buffer[1];
}

static uint8_t
getUint8(uint8_t* buffer)
{
    return buffer[0];
}

static void
writeOptions(CotpConnection* self)
{
    /* max size = 11 byte */
    uint8_t* buffer = self->writeBuffer->buffer;
    int bufPos = self->writeBuffer->size;

    if (self->options.tpduSize != 0)
    {
        if (DEBUG_COTP)
            printf("COTP: send TPDU size: %i\n", CotpConnection_getTpduSize(self));

        buffer[bufPos++] = 0xc0;
        buffer[bufPos++] = 0x01;
        buffer[bufPos++] = self->options.tpduSize;
    }

    if (self->options.tSelDst.size != 0)
    {
        buffer[bufPos++] = 0xc2;
        buffer[bufPos++] = (uint8_t) self->options.tSelDst.size;

        int i;
        for (i = 0; i < self->options.tSelDst.size; i++)
            buffer[bufPos++] = (uint8_t) self->options.tSelDst.value[i];
    }

    if (self->options.tSelSrc.size != 0)
    {
        buffer[bufPos++] = 0xc1;
        buffer[bufPos++] = (uint8_t) self->options.tSelSrc.size;

        int i;
        for (i = 0; i < self->options.tSelSrc.size; i++)
            buffer[bufPos++] = (uint8_t) self->options.tSelSrc.value[i];
    }

    self->writeBuffer->size = bufPos;
}

static int
getOptionsLength(CotpConnection* self)
{
    int optionsLength = 0;

    if (self->options.tpduSize != 0)
        optionsLength += 3;

    if (self->options.tSelDst.size != 0)
        optionsLength += (2 + self->options.tSelDst.size);

    if (self->options.tSelSrc.size != 0)
        optionsLength += (2 + self->options.tSelSrc.size);

    return optionsLength;
}

static void
writeStaticConnectResponseHeader(CotpConnection* self, int optionsLength)
{
    /* always same size (7) and same position in buffer */
    uint8_t* buffer = self->writeBuffer->buffer;

    buffer[4] = 6 + optionsLength;
    buffer[5] = 0xd0;
    buffer[6] = (uint8_t) (self->remoteRef / 0x100);
    buffer[7] = (uint8_t) (self->remoteRef & 0xff);
    buffer[8] = (uint8_t) (self->localRef / 0x100);
    buffer[9] = (uint8_t) (self->localRef & 0xff);
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

static int
writeToSocket(CotpConnection* self, uint8_t* buf, int size)
{
#if (CONFIG_MMS_SUPPORT_TLS == 1)
    if (self->tlsSocket)
        return TLSSocket_write(self->tlsSocket, buf, size);
    else
        return Socket_write(self->socket, buf, size);
#else
    return Socket_write(self->socket, buf, size);
#endif
}

static bool
flushBuffer(CotpConnection* self)
{
    if (self->socketExtensionBufferFill > 0)
    {
        int sentBytes = writeToSocket(self, self->socketExtensionBuffer, self->socketExtensionBufferFill);

        if (sentBytes > 0)
        {
            if (sentBytes != self->socketExtensionBufferFill)
            {
                int target = 0;
                int i;
                uint8_t* buf = self->socketExtensionBuffer;

                for (i = sentBytes; i < self->socketExtensionBufferFill; i++)
                {
                    buf[target++] = buf[i];
                }

                self->socketExtensionBufferFill = self->socketExtensionBufferFill - sentBytes;
            }
            else
            {
                self->socketExtensionBufferFill = 0;
            }
        }
        else if (sentBytes == -1)
        {
            return false;
        }
    }

    return true;
}

static bool
sendBuffer(CotpConnection* self)
{
    int remainingSize = ByteBuffer_getSize(self->writeBuffer);
    uint8_t* buffer = ByteBuffer_getBuffer(self->writeBuffer);

    bool retVal = false;

    if (flushBuffer(self) == false)
    {
        goto exit_function;
    }

    int sentBytes = 0;

    if (self->socketExtensionBufferFill == 0)
    {
        sentBytes = writeToSocket(self, buffer, remainingSize);
    }

    if (sentBytes == -1)
        goto exit_function;

    if (sentBytes != remainingSize)
    {
        /* write additional data to extension buffer */
        if (self->socketExtensionBuffer)
        {
            uint8_t* extBuf = self->socketExtensionBuffer;
            int extCurrentPos = self->socketExtensionBufferFill;
            int bytesNotSent = remainingSize - sentBytes;

            int i;
            for (i = 0; i < bytesNotSent; i++)
            {
                extBuf[i + extCurrentPos] = buffer[sentBytes + i];
            }

            self->socketExtensionBufferFill = extCurrentPos + bytesNotSent;
        }
        else
        {
            goto exit_function;
        }
    }

    retVal = true;

    ByteBuffer_setSize(self->writeBuffer, 0);

exit_function:
    return retVal;
}

CotpIndication
CotpConnection_sendDataMessage(CotpConnection* self, BufferChain payload)
{
    CotpIndication retValue = COTP_OK;

    int fragments = 1;

    int fragmentPayloadSize = CotpConnection_getTpduSize(self) - COTP_DATA_HEADER_SIZE;

    if (payload->length > fragmentPayloadSize)
    {
        /* Check if segmentation is required? */
        fragments = payload->length / fragmentPayloadSize;

        if ((payload->length % fragmentPayloadSize) != 0)
            fragments += 1;
    }

    /* calculate total size of fragmented message */
    int totalSize = (fragments * (COTP_DATA_HEADER_SIZE + 4)) + payload->length;

    /* try to flush extension buffer */
    if (flushBuffer(self) == false)
    {
        return COTP_ERROR;
    }

    /* check if totalSize will fit in extension buffer */
    if (self->socketExtensionBuffer)
    {
        int freeExtBufSize = self->socketExtensionBufferSize - self->socketExtensionBufferFill;

        if (freeExtBufSize < totalSize)
        {
            return COTP_ERROR;
        }
    }

    int currentBufPos = 0;
    int currentLimit;
    int lastUnit;

    BufferChain currentChain = payload;
    int currentChainIndex = 0;

    if (DEBUG_COTP)
        printf("COTP: nextBufferPart: len:%i partLen:%i\n", currentChain->length, currentChain->partLength);

    uint8_t* buffer = self->writeBuffer->buffer;

    while (fragments > 0)
    {
        if (fragments > 1)
        {
            currentLimit = currentBufPos + fragmentPayloadSize;
            lastUnit = 0;
        }
        else
        {
            currentLimit = payload->length;
            lastUnit = 1;
        }

        writeRfc1006Header(self, 7 + (currentLimit - currentBufPos));

        writeDataTpduHeader(self, lastUnit);

        int bufPos = 7;

        int i;
        for (i = currentBufPos; i < currentLimit; i++)
        {
            if (currentChainIndex >= currentChain->partLength)
            {
                currentChain = currentChain->nextPart;
                if (DEBUG_COTP)
                    printf("COTP: nextBufferPart: len:%i partLen:%i\n", currentChain->length, currentChain->partLength);
                currentChainIndex = 0;
            }

            buffer[bufPos++] = currentChain->buffer[currentChainIndex];

            currentChainIndex++;

            currentBufPos++;
        }

        self->writeBuffer->size = bufPos;

        if (DEBUG_COTP)
            printf("COTP: Send COTP fragment %i bufpos: %i\n", fragments, currentBufPos);

        if (!sendBuffer(self))
        {
            retValue = COTP_ERROR;

            if (DEBUG_COTP)
                printf("COTP: sending message failed!\n");

            goto exit_function;
        }

        fragments--;
    }

exit_function:

    if (DEBUG_COTP)
        printf("COTP: message transmission finished (fragments=%i, return=%i)\n", fragments, retValue);

    return retValue;
}

static void
allocateWriteBuffer(CotpConnection* self)
{
    if (self->writeBuffer == NULL )
        self->writeBuffer = ByteBuffer_create(NULL,
                CotpConnection_getTpduSize(self) + TPKT_RFC1006_HEADER_SIZE);
}

/* client side */
CotpIndication
CotpConnection_sendConnectionRequestMessage(CotpConnection* self, IsoConnectionParameters isoParameters)
{
    allocateWriteBuffer(self);

    self->options.tSelDst = isoParameters->remoteTSelector;
    self->options.tSelSrc = isoParameters->localTSelector;

    int cotpRequestSize = getOptionsLength(self) + 6;

    int conRequestSize = cotpRequestSize + 5;

    if(self->writeBuffer->maxSize < conRequestSize)
        return COTP_ERROR;

    uint8_t* buffer = self->writeBuffer->buffer;

    writeRfc1006Header(self, conRequestSize);

    /* LI */
    buffer[4] = (uint8_t) cotpRequestSize;

    /* TPDU CODE */
    buffer[5] = 0xe0;

    /* DST REF */
    buffer[6] = 0x00;
    buffer[7] = 0x00;

    /* SRC REF */
    buffer[8] = (uint8_t) (self->localRef / 0x100);
    buffer[9] = (uint8_t) (self->localRef & 0xff);

    /* Class */
    buffer[10] = 0x00;

    self->writeBuffer->size = 11;

    writeOptions(self);

    if (sendBuffer(self))
        return COTP_OK;
    else
        return COTP_ERROR;
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
        return COTP_OK;
    else
        return COTP_ERROR;
}

static bool
parseOptions(CotpConnection* self, uint8_t* buffer, int bufLen)
{
    int bufPos = 0;

    while (bufPos < bufLen)
    {
        uint8_t optionType = buffer[bufPos++];
        uint8_t optionLen = buffer[bufPos++];

        if (optionLen > (bufLen - bufPos))
        {
            if (DEBUG_COTP)
                printf("COTP: option to long optionLen:%i bufPos:%i bufLen:%i\n", optionLen, bufPos, bufLen);
            goto cpo_error;
        }

        if (DEBUG_COTP)
            printf("COTP: option: %02x len: %02x\n", optionType, optionLen);

        switch (optionType) {
        case 0xc0:
			if (optionLen == 1)
            {
				int requestedTpduSize = (1 << buffer[bufPos++]);

                CotpConnection_setTpduSize(self, requestedTpduSize);

				if (DEBUG_COTP)
				    printf("COTP: requested TPDU size: %i\n", requestedTpduSize);
			}
			else
			    goto cpo_error;
            break;

        case 0xc1: /* remote T-selector */
            if (optionLen < 5)
            {
                self->options.tSelSrc.size = optionLen;

                int i;
                for (i = 0; i < optionLen; i++)
                    self->options.tSelSrc.value[i] = buffer[bufPos++];
            }
            else
                goto cpo_error;
            break;

        case 0xc2: /* local T-selector */
            if (optionLen < 5)
            {
                self->options.tSelDst.size = optionLen;

                int i;
                for (i = 0; i < optionLen; i++)
                    self->options.tSelDst.value[i] = buffer[bufPos++];
            }
            else
                goto cpo_error;
            break;

        case 0xc6: /* additional option selection */
            if (optionLen == 1)
                bufPos++; /* ignore value */
            else
                goto cpo_error;
            break;

        default:
            if (DEBUG_COTP)
                printf("COTP: Unknown option %02x\n", optionType);

            bufPos += optionLen; /* ignore value */

            break;
        }
    }

    return true;

cpo_error:
    if (DEBUG_COTP)
        printf("COTP: cotp_parse_options: error parsing options!\n");
    return false;
}

void
CotpConnection_init(CotpConnection* self, Socket socket,
        ByteBuffer* payloadBuffer, ByteBuffer* readBuffer, ByteBuffer* writeBuffer,
        uint8_t* socketExtensionBuffer, int socketExtensionBufferSize)
{
    self->state = 0;
    self->socket = socket;
    self->handleSet = Handleset_new();
    Handleset_addSocket( self->handleSet, self->socket );

#if (CONFIG_MMS_SUPPORT_TLS == 1)
    self->tlsSocket = NULL;
#endif
    self->remoteRef = -1;
    self->localRef = 1;
    self->protocolClass = -1;
	self->options.tpduSize = 0;

	TSelector tsel;
	tsel.size = 2;
	tsel.value[0] = 0;
	tsel.value[1] = 1;

	self->options.tSelSrc = tsel;
	self->options.tSelDst = tsel;
    self->payload = payloadBuffer;

    CotpConnection_resetPayload(self);

    /* default TPDU size is maximum size */
    CotpConnection_setTpduSize(self, COTP_MAX_TPDU_SIZE);

    self->writeBuffer = writeBuffer;
    self->readBuffer = readBuffer;
    self->packetSize = 0;

    self->socketExtensionBuffer = socketExtensionBuffer;
    self->socketExtensionBufferSize = socketExtensionBufferSize;
    self->socketExtensionBufferFill = 0;
}

int /* in byte */
CotpConnection_getTpduSize(CotpConnection* self)
{
    return (1 << self->options.tpduSize);
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

    self->options.tpduSize = newTpduSize;
}

ByteBuffer*
CotpConnection_getPayload(CotpConnection* self)
{
    return self->payload;
}

int
CotpConnection_getRemoteRef(CotpConnection* self)
{
    return self->remoteRef;
}

int
CotpConnection_getLocalRef(CotpConnection* self)
{
    return self->localRef;
}

/*
 CR TPDU (from RFC 0905):

 1    2        3        4       5   6    7    8    p  p+1...end
 +--+------+---------+---------+---+---+------+-------+---------+
 |LI|CR CDT|     DST - REF     |SRC-REF|CLASS |VARIAB.|USER     |
 |  |1110  |0000 0000|0000 0000|   |   |OPTION|PART   |DATA     |
 +--+------+---------+---------+---+---+------+-------+---------+
 */


static bool
parseConnectRequestTpdu(CotpConnection* self, uint8_t* buffer, uint8_t len)
{
    if (len < 6)
        return false;

    self->remoteRef = getUint16(buffer + 2);
    self->protocolClass = getUint8(buffer + 4);

    return parseOptions(self, buffer + 5, len - 6);
}

static bool
parseConnectConfirmTpdu(CotpConnection* self, uint8_t* buffer, uint8_t len)
{
    if (len < 6)
        return false;

    self->remoteRef = getUint16(buffer);
    self->protocolClass = getUint8(buffer + 4);

    return parseOptions(self, buffer + 5, len - 6);
}

static bool
parseDataTpdu(CotpConnection* self, uint8_t* buffer, uint8_t len)
{
    if (len != 2)
        return false;

    uint8_t flowControl = getUint8(buffer);

    if (flowControl & 0x80)
        self->isLastDataUnit = true;
    else
        self->isLastDataUnit = false;

    return true;
}

static bool
addPayloadToBuffer(CotpConnection* self, uint8_t* buffer,  int payloadLength)
{
    if (payloadLength < 1)
    {
        if (DEBUG_COTP)
            printf("COTP: missing payload\n");

        return false;
    }

    if (DEBUG_COTP)
        printf("COTP: add to payload buffer (cur size: %i, len: %i)\n", self->payload->size, payloadLength);

    if ((self->payload->size + payloadLength) > self->payload->maxSize)
        return false;

    memcpy(self->payload->buffer + self->payload->size, buffer, payloadLength);

    self->payload->size += payloadLength;

    return true;
}

static CotpIndication
parseCotpMessage(CotpConnection* self)
{
    uint8_t* buffer = self->readBuffer->buffer + 4;
    int tpduLength = self->readBuffer->size - 4;

    uint8_t len;
    uint8_t tpduType;

    len = buffer[0];

    if (len > tpduLength)
    {
      if (DEBUG_COTP)
          printf("COTP: parseCotpMessage: len=%d tpduLength=%d\n", len, tpduLength);

      return COTP_ERROR;
    }

    tpduType = buffer[1];

    switch (tpduType) {
    case 0xe0:
        if (parseConnectRequestTpdu(self, buffer + 2, len))
            return COTP_CONNECT_INDICATION;
        else
            return COTP_ERROR;
    case 0xd0:
        if (parseConnectConfirmTpdu(self, buffer + 2, len))
            return COTP_CONNECT_INDICATION;
        else
            return COTP_ERROR;
    case 0xf0:
        if (parseDataTpdu(self, buffer + 2, len))
        {
            if (addPayloadToBuffer(self, buffer + 3, tpduLength - 3) != 1)
                return COTP_ERROR;

            if (self->isLastDataUnit)
                return COTP_DATA_INDICATION;
            else
                return COTP_MORE_FRAGMENTS_FOLLOW;
        }
        else
            return COTP_ERROR;
    default:
        return COTP_ERROR;
    }

}

CotpIndication
CotpConnection_parseIncomingMessage(CotpConnection* self)
{
    CotpIndication indication = parseCotpMessage(self);

    self->readBuffer->size = 0;
    self->packetSize = 0;

    return indication;
}

void
CotpConnection_resetPayload(CotpConnection* self)
{
    self->payload->size = 0;
}

static int
readFromSocket(CotpConnection* self, uint8_t* buf, int size)
{
#if (CONFIG_MMS_SUPPORT_TLS == 1)
    if (self->tlsSocket)
        return TLSSocket_read(self->tlsSocket, buf, size);
    else
    {
        switch (Handleset_waitReady(self->handleSet, 10))
        {
            case -1:
                return -1;
            case 0:
                return 0;
            default:
                break;
        }

        return Socket_read(self->socket, buf, size);
    }
#else
    switch (Handleset_waitReady(self->handleSet, 10))
    {
        case -1:
            return -1;
        case 0:
            return 0;
        default:
            break;
    }

    return Socket_read(self->socket, buf, size);
#endif
}

void
CotpConnection_flushBuffer(CotpConnection* self)
{
    if (self->socketExtensionBufferFill > 0)
        flushBuffer(self);
}

TpktState
CotpConnection_readToTpktBuffer(CotpConnection* self)
{
    uint8_t* buffer = self->readBuffer->buffer;
    int bufferSize = self->readBuffer->maxSize;
    int bufPos = self->readBuffer->size;

    assert (bufferSize > 4);

    if (self->socketExtensionBufferFill > 0)
    {
        if (flushBuffer(self) == false)
            goto exit_error;

        if (self->socketExtensionBufferFill > 0) 
            goto exit_waiting;
    }

    int readBytes;

    if (bufPos < 4)
    {
        readBytes = readFromSocket(self, buffer + bufPos, 4 - bufPos);

        if (readBytes < 0)
            goto exit_closed;

        if (DEBUG_COTP)
        {
            if (readBytes > 0)
                printf("TPKT: read %i bytes from socket\n", readBytes);
        }

        bufPos += readBytes;

        if (bufPos == 4)
        {
            if ((buffer[0] == 3) && (buffer[1] == 0))
            {
                self->packetSize = (buffer[2] * 0x100) + buffer[3];

                if (DEBUG_COTP)
                    printf("TPKT: header complete (msg size = %i)\n", self->packetSize);

                if (self->packetSize > bufferSize)
                {
                    if (DEBUG_COTP) printf("TPKT: packet too large\n");
                    goto exit_error;
                }
            }
            else
            {
                if (DEBUG_COTP) printf("TPKT: failed to decode TPKT header.\n");
                goto exit_error;
            }
        }
        else
            goto exit_waiting;
    }

    if (self->packetSize <= bufPos)
        goto exit_error;

    readBytes = readFromSocket(self, buffer + bufPos, self->packetSize - bufPos);

    if (readBytes < 0)
        goto exit_closed;

    bufPos += readBytes;

    if (bufPos < self->packetSize)
       goto exit_waiting;

    if (DEBUG_COTP) printf("TPKT: message complete (size = %i)\n", self->packetSize);

    self->readBuffer->size = bufPos;
    return TPKT_PACKET_COMPLETE;

exit_closed:
    if (DEBUG_COTP) printf("TPKT: socket closed or socket error\n");
    self->readBuffer->size = 0;
    return TPKT_ERROR;

exit_error:
    if (DEBUG_COTP) printf("TPKT: Error parsing message\n");
    self->readBuffer->size = 0;
    return TPKT_ERROR;

exit_waiting:

    if (DEBUG_COTP)
        if (bufPos != 0)
            printf("TPKT: waiting (read %i of %i)\n", bufPos, self->packetSize);

    self->readBuffer->size = bufPos;
    return TPKT_WAITING;
}
