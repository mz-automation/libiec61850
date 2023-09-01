/*
 *  r_session.c
 *
 *  Copyright 2013-2022 Michael Zillgith
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

#include "r_session.h"
#include "hal_socket.h"
#include "hal_thread.h"
#include "lib_memory.h"
#include "string_utilities.h"
#include "libiec61850_platform_includes.h"
#include "r_session_crypto.h"
#include "r_session_internal.h"

#ifndef DEBUG_RSESSION
#define DEBUG_RSESSION 0
#endif

#if (DEBUG_RSESSION == 1)
#include <stdio.h>
#define DEBUG_PRINTF(...) printf("RSESSION:"__VA_ARGS__);printf("\n");
#else
#define DEBUG_PRINTF(...)
#endif


typedef struct sRSessionKeyMaterial* RSessionKeyMaterial;

struct sRSessionKeyMaterial {
    uint32_t keyId;
    RSecurityAlgorithm secAlgo;
    RSignatureAlgorithm sigAlgo;
    uint8_t* key;
    int keyLength;
};

struct sRSession {
    uint32_t spduNumber;

    int protocolVersion; /* default is 2 */

    char* remoteAddress;
    int remotePort;

    char* localAddress;
    int localPort;

    UdpSocket socket;
    Semaphore socketLock;

    uint16_t bufferSize; /* maximum buffer size (range: 128 - 65535) */
    uint8_t* sendBuffer;
    uint8_t* payloadBuffer; /* only required when secAlgo != NONE */

    LinkedList keyList; /* list of RSessionKeyMaterial */
    Semaphore keyListLock;

    RSecurityAlgorithm secAlgo;
    RSignatureAlgorithm sigAlgo;
    uint32_t timeOfCurrentKey;
    uint32_t currentKeyId;
    uint8_t* currentKey;
    int currentKeySize;

    bool updateBufferedKeyMaterial;

    RSecurityAlgorithm currentSecAlgo;
    RSignatureAlgorithm currentSigAlgo;

    int timeToNextKey;
};

#ifdef DEBUG_RSESSION
static void
printBuffer(uint8_t* buffer, int bufSize)
{
    int i;

    for (i = 0; i < bufSize; i++) {
        printf("%02x ", buffer[i]);
        if ((((i + 1) % 16) == 0) || (i + 1 == bufSize))
            printf(" (%i)\n", i + 1);
    }
}
#endif /* DEBUG_RSESSION */

RSessionKeyMaterial
RSessionKeyMaterial_create(uint32_t keyId, uint8_t* key, int keyLength, RSecurityAlgorithm secAlgo, RSignatureAlgorithm sigAlgo)
{
    RSessionKeyMaterial self = (RSessionKeyMaterial) GLOBAL_CALLOC(1, sizeof(struct sRSessionKeyMaterial));

    if (self) {
        self->keyId = keyId;
        self->keyLength = keyLength;
        self->secAlgo = secAlgo;
        self->sigAlgo = sigAlgo;

        self->key = (uint8_t*) GLOBAL_MALLOC(keyLength);

        if (self->key) {
            memcpy(self->key, key, keyLength);
        }
        else {
            GLOBAL_FREEMEM(self);
            self = NULL;
        }
    }

    return self;
}

void
RSessionKeyMaterial_destroy(RSessionKeyMaterial self)
{
    if (self) {
        GLOBAL_FREEMEM(self->key);
        GLOBAL_FREEMEM(self);
    }
 }

RSession
RSession_create()
{
    UdpSocket udpSocket = UdpSocket_create();

    if (udpSocket) {

        RSession self = (RSession) GLOBAL_CALLOC(1, sizeof(struct sRSession));

        if (self) {
            self->socket = udpSocket;
            self->socketLock = Semaphore_create(1);

            self->secAlgo = R_SESSION_SEC_ALGO_NONE;
            self->sigAlgo = R_SESSION_SIG_ALGO_NONE;
            self->protocolVersion = 2;
            self->bufferSize = (uint16_t)65000;
            self->keyListLock = Semaphore_create(1);
            self->keyList = LinkedList_create();
            self->localAddress = NULL;
            self->localPort = 102;
        }

        return self;
    }

    return NULL;
}

/* only for version 1 of the protocol! */
RSessionError
RSession_setSecurity(RSession self, RSecurityAlgorithm secAlgo, RSignatureAlgorithm sigAlgo)
{
    self->secAlgo = secAlgo;
    self->sigAlgo = sigAlgo;

    return R_SESSION_ERROR_OK;
}

RSessionError
RSession_setLocalAddress(RSession self, const char* localAddress, int localPort)
{
    if (self->localAddress) {
        GLOBAL_FREEMEM(self->localAddress);
        self->localAddress = NULL;
    }

    if (localAddress)
        self->localAddress = StringUtils_copyString(localAddress);

    self->localPort = localPort;

    return R_SESSION_ERROR_OK;
}

RSessionError
RSession_addMulticastGroup(RSession self, const char* multicastAddress)
{
    if (UdpSocket_addGroupMembership(self->socket, multicastAddress))
        return R_SESSION_ERROR_OK;
    else
        return R_SESSION_ERROR_SET_FAILED;
}

RSessionError
RSession_setMulticastTtl(RSession self, int ttl)
{
    RSessionError err = R_SESSION_ERROR_OK;

    if (self->socket) {
        if (!UdpSocket_setMulticastTtl(self->socket, ttl)) {
            err = R_SESSION_ERROR_SET_FAILED;
        }
    }
    else {
        err = R_SESSION_ERROR_NO_SOCKET;
    }

    return err;
}

RSessionError
RSession_setRemoteAddress(RSession self, const char* remoteAddress, int remotePort)
{
    if (self->remoteAddress) {
        GLOBAL_FREEMEM(self->remoteAddress);
        self->remoteAddress = NULL;
    }

    self->remoteAddress = StringUtils_copyString(remoteAddress);
    self->remotePort = remotePort;

    return R_SESSION_ERROR_OK;
}

RSessionError
RSession_start(RSession self)
{
    if (self->socket) {

        bool success = false;

        if (self->localAddress)
            success = UdpSocket_bind(self->socket, self->localAddress, self->localPort);
        else
            success = UdpSocket_bind(self->socket, "0.0.0.0", self->localPort);

        if (success)
            return R_SESSION_ERROR_OK;
        else
            return R_SESSION_ERROR_SET_FAILED;
    }
    else {
        return R_SESSION_ERROR_NO_SOCKET;
    }
}

RSessionError
RSession_stop(RSession self)
{
    Semaphore_wait(self->socketLock);

    if (self->socket) {
        Socket_destroy((Socket)self->socket);
        self->socket = NULL;

        Semaphore_post(self->socketLock);

        return R_SESSION_ERROR_OK;
    }
    else {
        Semaphore_post(self->socketLock);

        return R_SESSION_ERROR_NO_SOCKET;
    }
}

#define PAYLOAD_TYPE_TUNNELED_GOOSE_OR_SV 0xa0
#define PAYLOAD_TYPE_GOOSE_APDU 0xa1
#define PAYLOAD_TYPE_SV_APDU 0xa2
#define PAYLOAD_TYPE_MNGT_APDU 0xa3

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
encodeInt16FixedSize(int16_t value, uint8_t* buffer, int bufPos)
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
decodeUInt32FixedSize(uint32_t* outValue, uint8_t* buffer, int bufPos)
{
    uint32_t value = 0;

    value += buffer[bufPos++] * 0x1000000;
    value += buffer[bufPos++] * 0x10000;
    value += buffer[bufPos++] * 0x100;
    value += buffer[bufPos++];

    *outValue = value;

    return bufPos;
}

static int
decodeUInt16FixedSize(uint16_t* outValue, uint8_t* buffer, int bufPos)
{
    uint16_t value = 0;

    value += buffer[bufPos++] * 0x100;
    value += buffer[bufPos++];

    *outValue = value;

    return bufPos;
}

static int
decodeInt16FixedSize(int16_t* outValue, uint8_t* buffer, int bufPos)
{
    uint16_t value = 0;

    value += buffer[bufPos++] * 0x100;
    value += buffer[bufPos++];

    *outValue = (int16_t) value;

    return bufPos;
}

static bool
lookupKey(RSession self, uint32_t keyId, uint8_t** key, int* keySize,  RSecurityAlgorithm* secAlgo, RSignatureAlgorithm* sigAlgo)
{
    if (keyId == 0) {
        DEBUG_PRINTF("Invalid key ID");
        return false;
    }

    if (self->currentKeyId != keyId) {

        if (RSession_setActiveKey(self, keyId) != R_SESSION_ERROR_OK) {
            DEBUG_PRINTF("unknown key-ID %u", keyId);
            /* TODO audit-log? */
            return false;
        }

    }

    *key = self->currentKey;
    *keySize = self->currentKeySize;
    *secAlgo = self->currentSecAlgo;
    *sigAlgo = self->currentSigAlgo;

    return true;
}

static RSessionError
parseSessionMessage(RSession self, uint8_t* buffer, int msgSize, RSessionPayloadElementHandler handler, void* handlerParam)
{
    int bufPos = 0;

    if (msgSize < 4)
        goto exit_parse_error;

    if (buffer[bufPos++] != 0x01)
        goto exit_parse_error;

    if (buffer[bufPos++] != 0x40)
        goto exit_parse_error;

    /* SI */
    uint8_t payloadType = buffer[bufPos++];

    if ((payloadType == 0xa2) || (payloadType == 0xa1) || (payloadType == 0xa0)) {

    }
    else {
        DEBUG_PRINTF("unknown payload type %i", payloadType);
        goto exit_error;
    }

    int sessionHeaderLength = buffer[bufPos++];

    if ((msgSize < (sessionHeaderLength + 4))  || (sessionHeaderLength < 10)) {
        DEBUG_PRINTF("message too small");
        goto exit_error;
    }

    if (buffer[bufPos++] != 0x80) {
        DEBUG_PRINTF("protocol error");
        goto exit_error;
    }

    int commonSessionHeaderLength = buffer[bufPos++];

    if (commonSessionHeaderLength < 10) {
        DEBUG_PRINTF("common session header too small");
        goto exit_error;
    }

    /* SPDU length */
    uint32_t spduLength = 0;
    bufPos = decodeUInt32FixedSize(&spduLength, buffer, bufPos);

    /* SPDU number */
    uint32_t spduNumber = 0;
    bufPos = decodeUInt32FixedSize(&spduNumber, buffer, bufPos);

    /* protocol version */
    int16_t protocolVersion = 0;
    bufPos = decodeInt16FixedSize(&protocolVersion, buffer, bufPos);

    if (protocolVersion == 1) {
        /* parse version 1 common header parts */

        /* TimeOfCurrentKey */
        uint32_t timeOfCurrentKey;
        bufPos = decodeUInt32FixedSize(&timeOfCurrentKey, buffer, bufPos);

        /* TimeToNextKey */
        int16_t timeToNextKey;
        bufPos = decodeInt16FixedSize(&timeToNextKey, buffer, bufPos);

        RSecurityAlgorithm secAlgo = (RSecurityAlgorithm) buffer[bufPos++];

        RSignatureAlgorithm sigAlgo = (RSignatureAlgorithm) buffer[bufPos++];

        /* Check if algorithms match the configured algorithms */
        if (secAlgo != self->secAlgo) {
            DEBUG_PRINTF("encryption algorithm doesn't match with configuration");
            goto exit_error;
        }

        if (sigAlgo != self->sigAlgo) {
            DEBUG_PRINTF("signature algorithm(%i) doesn't match with configuration(%i)", sigAlgo, self->sigAlgo);
            goto exit_error;
        }

        /* Key ID */

        uint32_t keyId;

        bufPos = decodeUInt32FixedSize(&keyId, buffer, bufPos);

        uint8_t* key = NULL;
        int keySize = 0;

        if (sigAlgo != R_SESSION_SIG_ALGO_NONE) {
            if (lookupKey(self, keyId, &key, &keySize, &secAlgo, &sigAlgo) == false) {
                DEBUG_PRINTF("ERROR - key not found");
                goto exit_error;
            }
        }
        else if (secAlgo != R_SESSION_SEC_ALGO_NONE) {
            if (lookupKey(self, keyId, &key, &keySize, &secAlgo, &sigAlgo) == false) {
                DEBUG_PRINTF("ERROR - key not found");
                goto exit_error;
            }
        }

        uint32_t payloadLength;

        bufPos = decodeUInt32FixedSize(&payloadLength, buffer, bufPos);

        /* parse payload elements */
        uint32_t payloadEnd = bufPos + payloadLength;

        if (payloadEnd > (uint32_t)msgSize) {
            DEBUG_PRINTF("ERROR - payload size field invalid");
            goto exit_error;
        }

        uint8_t signatureBuffer[128];

        /* Check signature */
        if (sigAlgo != R_SESSION_SIG_ALGO_NONE) {
            if (key) {
                if (RSessionCrypto_createHMAC(buffer, payloadEnd, key, keySize, signatureBuffer, 32)) {
                    if (buffer[payloadEnd] != 0x85) {
                        DEBUG_PRINTF("ERROR - no signature found");
                        goto exit_error;
                    }
                    else {
                        if (sigAlgo == R_SESSION_SIG_ALGO_HMAC_SHA256_128) {
                            /* TODO is payloadEnd +2 correct? */
                            if (memcmp(signatureBuffer, buffer + payloadEnd + 1, 16)) {
                                DEBUG_PRINTF("ERROR - signature not matching!");
                                goto exit_error;
                            }
                        }
                        else if (sigAlgo == R_SESSION_SIG_ALGO_HMAC_SHA256_256) {
                            /* TODO is payloadEnd +2 correct? */
                            if (memcmp(signatureBuffer, buffer + payloadEnd + 1, 32)) {
                                DEBUG_PRINTF("ERROR - signature not matching!");
                                goto exit_error;
                            }
                        }
                    }
                }
                else {
                    DEBUG_PRINTF("ERROR - failed to calculate HMAC!");
                    goto exit_error;
                }
            }
            else {
                DEBUG_PRINTF("ERROR - key not found!");
                goto exit_error;
            }
        }

        while ((uint32_t)bufPos < payloadEnd) {
            int payloadElementType = buffer[bufPos++];

            bool simulation;

            if (buffer[bufPos++])
                simulation = true;
            else
                simulation = false;

            uint16_t appId;
            bufPos = decodeUInt16FixedSize(&appId, buffer, bufPos);

            uint16_t asduLength;
            bufPos = decodeUInt16FixedSize(&asduLength, buffer, bufPos);

            DEBUG_PRINTF("ASDU %02x sim: %i APPID: %04x length: %i", payloadElementType, simulation, appId, asduLength);

            if (payloadElementType == 0x81 ||
                payloadElementType == 0x82) {
                /* user payload */

                //TODO copy ASDU payload to ???
                handler(handlerParam, appId, buffer + bufPos, asduLength);
            }
            else {
                DEBUG_PRINTF("unexpected payload type! (expect 81h (GOOSE) or 82h (SV))");
            }

            bufPos += asduLength;
        }

    }
    else if (protocolVersion == 2) {
        /* parse version 2 common header parts */

        /* TimeOfCurrentKey */
        uint32_t timeOfCurrentKey;
        bufPos = decodeUInt32FixedSize(&timeOfCurrentKey, buffer, bufPos);

        /* TimeToNextKey */
        int16_t timeToNextKey;
        bufPos = decodeInt16FixedSize(&timeToNextKey, buffer, bufPos);

        RSecurityAlgorithm secAlgo = R_SESSION_SEC_ALGO_NONE;
        RSignatureAlgorithm sigAlgo = R_SESSION_SIG_ALGO_NONE;

        uint8_t* key = NULL;
        int keySize = 0;

        /* Key ID */
        uint32_t keyId;
        bufPos = decodeUInt32FixedSize(&keyId, buffer, bufPos);

        if (keyId != 0) {
            /* get key material associated with the key ID */

            if (lookupKey(self, keyId, &key, &keySize, &secAlgo, &sigAlgo) == false) {
                DEBUG_PRINTF("ERROR - key not found");
                goto exit_error;
            }
        }
        else {
            DEBUG_PRINTF("ERROR - invalid key ID");
            goto exit_error;
        }

        DEBUG_PRINTF("PV: 2 sec-algo: %i sig-algo: %i", secAlgo, sigAlgo);

        uint8_t* iv = NULL;

        /* IV */
        int ivLen = buffer[bufPos++];

        DEBUG_PRINTF("IV: size = %i\n", ivLen);

        if (ivLen > 0) {
            iv = buffer + bufPos;
            bufPos += ivLen;
        }

        uint32_t payloadLength;

        bufPos = decodeUInt32FixedSize(&payloadLength, buffer, bufPos);

        uint8_t* payloadStart = buffer + bufPos;
        int payloadStartPos = bufPos;

        /* parse payload elements */
        uint32_t payloadEnd = bufPos + payloadLength;

        if (payloadEnd > (uint32_t)msgSize) {
            DEBUG_PRINTF("ERROR - payload size field invalid");
            goto exit_error;
        }

        uint8_t signatureBuffer[128];

        /* Check signature */
        if (sigAlgo != R_SESSION_SIG_ALGO_NONE) {
            if (key) {
                if (RSessionCrypto_createHMAC(buffer, payloadEnd, key, keySize, signatureBuffer, 32)) {
                    if (buffer[payloadEnd] != 0x85) {
                        DEBUG_PRINTF("ERROR - no signature found");
                        goto exit_error;
                    }
                    else {
                        if (sigAlgo == R_SESSION_SIG_ALGO_HMAC_SHA256_128) {
                            /* TODO is payloadEnd +2 correct? */
                            if (memcmp(signatureBuffer, buffer + payloadEnd + 1, 16)) {
                                DEBUG_PRINTF("ERROR - signature not matching!");
                                goto exit_error;
                            }
                        }
                        else if (sigAlgo == R_SESSION_SIG_ALGO_HMAC_SHA256_256) {
                            /* TODO is payloadEnd +2 correct? */
                            if (memcmp(signatureBuffer, buffer + payloadEnd + 1, 32)) {
                                DEBUG_PRINTF("ERROR - signature not matching!");
                                goto exit_error;
                            }
                        }
                    }
                }
                else {
                    DEBUG_PRINTF("ERROR - failed to calculate HMAC!");
                    goto exit_error;
                }
            }
            else {
                DEBUG_PRINTF("ERROR - key not found!");
                goto exit_error;
            }
        }

        /* Check signature and decrypt application layer */
        if (secAlgo != R_SESSION_SEC_ALGO_NONE) {
            /* Check for HMAC */
            if (payloadEnd + 18 <= (uint32_t)msgSize) {
                if (self->payloadBuffer == NULL)
                    self->payloadBuffer = (uint8_t*)GLOBAL_MALLOC(65000);

                if (self->payloadBuffer) {
                    //TODO check MMAC tag
                    uint8_t* mac = buffer + payloadEnd + 2;
                    int macSize = buffer[payloadEnd + 1];

                    int payloadSize = payloadEnd - payloadStartPos;

                    if (RSessionCrypto_gcmAuthAndDecrypt(key, keySize, iv, ivLen, buffer, payloadStartPos, payloadStart, payloadSize, self->payloadBuffer, mac, macSize)) {
                        memcpy(buffer + bufPos, self->payloadBuffer, payloadSize);
                    }
                    else {
                        DEBUG_PRINTF("ERROR - auth and decrypt failed!");
                        goto exit_error;
                    }
                }
            }
            else {
                DEBUG_PRINTF("ERROR - sec algo - message too small!");
                goto exit_error;
            }
        }

        while ((uint32_t)bufPos < payloadEnd) {
            int payloadElementType = buffer[bufPos++];

            bool simulation;

            if (buffer[bufPos++])
                simulation = true;
            else
                simulation = false;

            uint16_t appId;
            bufPos = decodeUInt16FixedSize(&appId, buffer, bufPos);

            uint16_t asduLength;
            bufPos = decodeUInt16FixedSize(&asduLength, buffer, bufPos);

            DEBUG_PRINTF("ASDU %02x sim: %i APPID: %04x length: %i", payloadElementType, simulation, appId, asduLength);

            if (payloadElementType == 0x81 ||
                payloadElementType == 0x82) {
                /* user payload */

                //TODO copy ASDU payload to ???
                handler(handlerParam, appId, buffer + bufPos, asduLength);
            }
            else {
                DEBUG_PRINTF("unexpected payload type! (expect 81h (GOOSE) or 82h (SV))");
            }

            bufPos += asduLength;
        }
    }
    else {
        DEBUG_PRINTF("only protocol version 1 and 2 supported (received version %i)", protocolVersion);
        goto exit_error;
    }

    return R_SESSION_ERROR_OK;

exit_parse_error:

    DEBUG_PRINTF("ERROR - Failed to parse message");

exit_error:

    return R_SESSION_ERROR_INVALID_MESSAGE;
}

int
encodePacket(RSession self, uint8_t payloadType, uint8_t* buffer, int bufPos, RSessionPayloadElement elements)
{
    /* calculate total payload length */
    int payloadLength = 0;

    int startPos = bufPos;

    uint8_t* iv = NULL;
    int ivSize = 0;

    RSessionPayloadElement element = elements;

    while (element) {
        payloadLength += 6; /* payload type, simulation, APPID, length */
        payloadLength += element->payloadSize;

        element = element->nextElement;
    }

    /* Connection less transport protocol */
    buffer[bufPos++] = 0x01;
    buffer[bufPos++] = 0x40;

    /* SI */
    buffer[bufPos++] = payloadType;

    /* Session header length */
    buffer[bufPos++] = 24;

    /* Common session header tag */
    buffer[bufPos++] = 0x80;

    /* Common session header length */
    buffer[bufPos++] = 18; // 12

    /* SPDU length */
    int spduLength = 4 + 2 + 4 + 4 + 1 + 1 + 4 + payloadLength;
    DEBUG_PRINTF("payloadlength: %i spdu_len: %i", payloadLength, spduLength);
    bufPos = encodeUInt32FixedSize(spduLength, buffer, bufPos);

    /* SPDU number */
    bufPos = encodeUInt32FixedSize(self->spduNumber, buffer, bufPos);

    /* version */
    bufPos = encodeUInt16FixedSize(self->protocolVersion, buffer, bufPos);

    /* TimeOfCurrentKey */
    bufPos = encodeUInt32FixedSize(self->timeOfCurrentKey, buffer, bufPos);

    /* TimeToNextKey */
    bufPos = encodeInt16FixedSize((int16_t)self->timeToNextKey, buffer, bufPos);

    if (self->protocolVersion == 1) {
        /* encryption algorithm */
        buffer[bufPos++] = (uint8_t) self->secAlgo; /* 0 = none */

        /* signature algorithm */
        buffer[bufPos++] = (uint8_t) self->sigAlgo; /* 0 = none */

        /* Key ID */
        bufPos = encodeUInt32FixedSize(self->currentKeyId, buffer, bufPos);
    }
    else { /* protocol version 2 */
        /* Key ID */
        bufPos = encodeUInt32FixedSize(self->currentKeyId, buffer, bufPos);
        self->secAlgo = self->currentSecAlgo;
        self->sigAlgo = self->currentSigAlgo;

        DEBUG_PRINTF("PV: 2 sec-algo: %i sig-algo: %i\n", self->secAlgo, self->sigAlgo);

        /* IV */
        if (self->secAlgo != R_SESSION_SEC_ALGO_NONE) {
            /* TODO get and encode IV (initialization vector) */

            buffer[bufPos++] = 12;

            iv = buffer + bufPos;
            ivSize = 12;

            if (RSessionCrypto_createRandomData(iv, ivSize) == false) {
                DEBUG_PRINTF("ERROR - Failed to create random IV");
            }

            bufPos += ivSize;
        }
        else {
            buffer[bufPos++] = 0; /* empty initialization vector */
        }
    }

    /* user payload length */
    bufPos = encodeUInt32FixedSize(payloadLength, buffer, bufPos);

    int encryptedPartStartPos = bufPos;

    /* encode user payload elements */
    element = elements;

    while (element) {

        /* payload type ? (according to example in annex G) */
        buffer[bufPos++] = element->payloadType;

        /* simulation */
        buffer[bufPos++] = element->simulation;

        /* APPID */
        bufPos = encodeUInt16FixedSize(element->appId, buffer, bufPos);

        /* APDU length */
        bufPos = encodeUInt16FixedSize(element->payloadSize + 2, buffer, bufPos);

        /* copy user payload */
        memcpy(buffer + bufPos, element->payload, element->payloadSize);
        bufPos += element->payloadSize;

        element = element->nextElement;
    }

    if (self->sigAlgo != R_SESSION_SIG_ALGO_NONE) {

        int signatureCoveredLength = bufPos - startPos;

        DEBUG_PRINTF("Signature: %i", signatureCoveredLength);

        /* add signature */
        if (self->sigAlgo == R_SESSION_SIG_ALGO_HMAC_SHA256_256) {

            buffer[bufPos++] = 0x85;
            buffer[bufPos++] = 16;

            RSessionCrypto_createHMAC(buffer + startPos, signatureCoveredLength, self->currentKey, self->currentKeySize, buffer + bufPos, 32);

            bufPos += 32;
        }
        else if (self->sigAlgo == R_SESSION_SIG_ALGO_HMAC_SHA256_128) {
            buffer[bufPos++] = 0x85;
            buffer[bufPos++] = 16;
            //buffer[bufPos++] = 0x20; /* 32 octets */

            RSessionCrypto_createHMAC(buffer + startPos, signatureCoveredLength, self->currentKey, self->currentKeySize, buffer + bufPos, 16);

            bufPos += 16;
        }
        else {
            DEBUG_PRINTF("ERROR - unsupported signature type");
        }
    }

    int payloadEndPos = bufPos;

    if (self->secAlgo != R_SESSION_SEC_ALGO_NONE) {
        /* create signature and encrypt payload */

        buffer[bufPos++] = 0x85;
        buffer[bufPos++] = 16;

        int addPartSize = encryptedPartStartPos - startPos;
        int encryptedPartSize = payloadEndPos - encryptedPartStartPos;

#ifdef DEBUG_RSESSION
        printBuffer(buffer + startPos, bufPos - startPos);
#endif

        DEBUG_PRINTF("===> encrypt ====");

        if (RSessionCrypto_gcmEncryptAndTag(self->currentKey, self->currentKeySize, iv, ivSize, buffer + startPos, addPartSize, buffer + encryptedPartStartPos, encryptedPartSize, buffer + bufPos, 16) == false) {
            DEBUG_PRINTF("ERROR - encryption failed");
        }
        else {
            bufPos += 16;
        }
    }

    self->spduNumber++;

    return bufPos;
}

/*
 * NOTE: For library internal use!
 */
RSessionError
RSession_sendMessage(RSession self, RSessionProtocol_SPDU_ID spduId, bool simulation, uint16_t appId, uint8_t* payload, int payloadSize)
{
    if (self->socket == NULL) {
        self->socket = UdpSocket_create();
    }

    if (self->sendBuffer == NULL) {
        self->sendBuffer = (uint8_t*) GLOBAL_MALLOC(self->bufferSize);

        if (self->sendBuffer == NULL)
            return R_SESSION_ERROR_OUT_OF_MEMORY;
    }

    if (self->socket) {

        struct sRSessionPayloadElement element;

        element.simulation = simulation;
        element.appId = appId;
        element.payload = payload;
        element.payloadType = (spduId & 0x0f) + 0x80;
        element.payloadSize = payloadSize;
        element.nextElement = NULL;

        int msgSize = encodePacket(self, (uint8_t) spduId, self->sendBuffer, 0, &element);

#ifdef DEBUG_RSESSION
        printBuffer(self->sendBuffer, msgSize);
#endif

        if (UdpSocket_sendTo(self->socket, self->remoteAddress, self->remotePort, self->sendBuffer, msgSize)) {
            return R_SESSION_ERROR_OK;
        }
        else {
            return R_SESSION_ERROR_FAILED_TO_SEND;
        }

    }
    else {
        return R_SESSION_ERROR_NO_SOCKET;
    }
}

void
RSession_setBufferSize(RSession self, uint16_t bufferSize)
{
    if (bufferSize > 127) {
        self->bufferSize = bufferSize;
    }
    else {
        self->bufferSize = 128;
    }
}

RSessionError
RSession_receiveMessage(RSession self, RSessionPayloadElementHandler handler, void* parameter)
{
    Semaphore_wait(self->socketLock);

    if (self->socket) {
        char ipAddrBuf[128];

        if (self->sendBuffer == NULL) {
            self->sendBuffer = (uint8_t*) GLOBAL_MALLOC(self->bufferSize);

            if (self->sendBuffer == NULL) {
                Semaphore_post(self->socketLock);

                return R_SESSION_ERROR_OUT_OF_MEMORY;
            }
        }

        //TODO use handleset

        int msgSize = UdpSocket_receiveFrom(self->socket, ipAddrBuf, 128, self->sendBuffer, self->bufferSize);

        Semaphore_post(self->socketLock);

        if (msgSize < 1) {
            DEBUG_PRINTF("RESSSION: Failed to receive message");


            return R_SESSION_ERROR_FAILED_TO_RECEIVE;
        }
        else {
            return parseSessionMessage(self, self->sendBuffer, msgSize, handler, parameter);
        }
    }
    else {
        Semaphore_post(self->socketLock);

        return R_SESSION_ERROR_NO_SOCKET;
    }
}

RSessionError
RSession_addKey(RSession self, uint32_t keyId, uint8_t* key, int keyLength, RSecurityAlgorithm secAlgo, RSignatureAlgorithm sigAlgo)
{
    RSessionKeyMaterial keyMaterial = RSessionKeyMaterial_create(keyId, key, keyLength, secAlgo, sigAlgo);

    if (keyMaterial) {
        Semaphore_wait(self->keyListLock);
        LinkedList_add(self->keyList, keyMaterial);
        Semaphore_post(self->keyListLock);

        return R_SESSION_ERROR_OK;
    }
    else {
        return R_SESSION_ERROR_OUT_OF_MEMORY;
    }
}

static RSessionKeyMaterial
getKeyById(RSession self, uint32_t keyId)
{
    LinkedList keyElem = LinkedList_getNext(self->keyList);

    while (keyElem) {
        RSessionKeyMaterial keyMaterial = (RSessionKeyMaterial) LinkedList_getData(keyElem);

        if (keyMaterial->keyId == keyId) {
            return keyMaterial;
        }

        keyElem = LinkedList_getNext(keyElem);
    }

    return NULL;
}

RSessionError
RSession_removeKey(RSession self, uint32_t keyId)
{
    RSessionError retVal = R_SESSION_ERROR_OK;

    Semaphore_wait(self->keyListLock);

    RSessionKeyMaterial keyMaterial = getKeyById(self, keyId);

    if (keyMaterial) {
        LinkedList_remove(self->keyList, keyMaterial);

        RSessionKeyMaterial_destroy(keyMaterial);
    }
    else {
        retVal = R_SESSION_ERROR_INVALID_KEY;
    }

    Semaphore_post(self->keyListLock);

    if (self->currentKeyId == keyId) {
        /* active key removed! */
        self->currentKeyId = 0;
    }

    return retVal;
}

void
RSession_removeAllKeys(RSession self)
{
    Semaphore_wait(self->keyListLock);

    LinkedList_destroyDeep(self->keyList, (LinkedListValueDeleteFunction)RSessionKeyMaterial_destroy);

    self->keyList = LinkedList_create();

    self->currentKeyId = 0;

    Semaphore_post(self->keyListLock);
}

/**
 * \brief Set the active key for the sender
 */
RSessionError
RSession_setActiveKey(RSession self, uint32_t keyId)
{
    RSessionError retVal = R_SESSION_ERROR_OK;

    Semaphore_wait(self->keyListLock);

    RSessionKeyMaterial keyMaterial = getKeyById(self, keyId);

    if (keyMaterial) {
        self->currentKeySize = keyMaterial->keyLength;
        self->currentKey = keyMaterial->key;
        self->currentKeyId = keyMaterial->keyId;
        self->currentSecAlgo = keyMaterial->secAlgo;
        self->currentSigAlgo = keyMaterial->sigAlgo;
    }
    else {
        retVal = R_SESSION_ERROR_INVALID_KEY;
    }

    Semaphore_post(self->keyListLock);

    return retVal;
}

void
RSession_destroy(RSession self)
{
    if (self) {
        if (self->socket)
            Socket_destroy((Socket)self->socket);

        if (self->sendBuffer)
            GLOBAL_FREEMEM(self->sendBuffer);

        if (self->remoteAddress)
            GLOBAL_FREEMEM(self->remoteAddress);

        if (self->localAddress)
            GLOBAL_FREEMEM(self->localAddress);

        if (self->payloadBuffer)
            GLOBAL_FREEMEM(self->payloadBuffer);

        if (self->keyList)
            LinkedList_destroyDeep(self->keyList, (LinkedListValueDeleteFunction)RSessionKeyMaterial_destroy);

        if (self->keyListLock)
            Semaphore_destroy(self->keyListLock);

        if (self->socketLock)
            Semaphore_destroy(self->socketLock);

        GLOBAL_FREEMEM(self);
    }
}

Socket
RSession_getSocket(RSession self)
{
    return (Socket)self->socket;
}

