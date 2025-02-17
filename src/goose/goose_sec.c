/*
 *  goose_sec.c
 *
 *  Copyright 2013-2025 Michael Zillgith
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

#include <stdint.h>
#include <stdio.h>

#include "l2_security.h"
#include "ber_encoder.h"
#include "ber_decode.h"
#include "r_session_crypto.h"

struct sL2Security
{
    RSignatureAlgorithm currentSigAlgo;

    uint32_t timeOfCurrentKey;
    uint32_t currentKeyId;
    uint8_t* currentKey;
    int currentKeySize;

    int timeToNextKey;
};

static uint16_t poly[] = { 
    0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf, 
    0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7, 
    0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e, 
    0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876, 
    0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd, 
    0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5, 
    0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c, 
    0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974, 
    0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb, 
    0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3, 
    0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a, 
    0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72, 
    0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9, 
    0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1, 
    0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738, 
    0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70, 
    0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7, 
    0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff, 
    0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036, 
    0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e, 
    0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5, 
    0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd, 
    0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134, 
    0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c, 
    0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3, 
    0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb, 
    0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232, 
    0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a, 
    0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1, 
    0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9, 
    0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330, 
    0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78 
};

/* see https://www.rfc-editor.org/rfc/rfc1662#page-19 */
static uint16_t
calculateCRC(uint8_t* data, int size)
{
    uint16_t crc = 0xffff;

    int i;

    for (i = 0; i < size; i++) {
        crc = (uint16_t)(((uint16_t)(crc >> 8)) ^ poly[(crc ^ data[i]) & 0xff]);
    }

    return (uint16_t)(~crc);
}

uint16_t
L2Security_calculateCRC16(uint8_t* data, int size)
{
    return calculateCRC(data, size);
}

static void
printBuffer(uint8_t* buffer, int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%02x", buffer[i]);
    }
    printf("\n");
}

/**
 * \brief Create the security extension
 * 
 * \param buffer buffer with the encoded GOOSE message (security extension will be added)
 * \param start start of the message payload used to calculate the MAC
 * \param length length of the message payload 
 * 
 * \return length of the security extension
 */
uint16_t
L2Security_addSecurityExtension(L2Security self, uint8_t* buffer, int start, int length, int maxBufSize, bool encode)
{
    printf("L2Security_addSecurityExtension: start=%i, length=%i, maxBufSize=%i\n", start, length, maxBufSize);
    if (self->currentSigAlgo != MC_SEC_SIG_ALGO_NONE)
    {
        int ivSize = 0;
        int mACSize = 0;
        uint8_t* ivBuf = NULL;

        /* determine length of the mAC */
        if (self->currentSigAlgo == MC_SEC_SIG_ALGO_HMAC_SHA256_128) {
            mACSize = 2 + 16;
        }
        else if (self->currentSigAlgo == MC_SEC_SIG_ALGO_HMAC_SHA256_256) {
            mACSize = 2 + 32;
        }
        else if (self->currentSigAlgo == MC_SEC_SIG_ALGO_AES_GMAC_128) {
            mACSize = 2 + 16;
            ivSize = 12;
        }
        else if (self->currentSigAlgo == MC_SEC_SIG_ALGO_AES_GMAC_256) {
            mACSize = 2 + 32;
            ivSize = 12;
        }
        else {
            /* signature algorithm not supported */
            printf("Signature algorithm not supported\n");
            return 0;
        }

        uint32_t securityExtensionSize = 0;

        /* TODO check for applicable/valid key */
    
        /* determine length of the AuthenticationValue */

        uint32_t authValueSize = 3; /* TLV(Version) */

        /* TimeofCurrentKey */
        authValueSize += (2 + BerEncoder_UInt32determineEncodedSize(self->timeOfCurrentKey));

        /* TimeToNextKey */
        authValueSize += (2 + BerEncoder_Int32determineEncodedSize(self->timeToNextKey));

        /* IV */
        if (ivSize > 0)
        {
            authValueSize += (2 + ivSize);
        }

        /* Key ID */
        authValueSize += (2 + BerEncoder_UInt32determineEncodedSize(self->currentKeyId));

        securityExtensionSize += (1 + BerEncoder_determineLengthSize(authValueSize) + authValueSize);
        securityExtensionSize += mACSize;

        /* check that total size fits into the buffer! */

        int bufPos = start + length;

        if (bufPos + securityExtensionSize > maxBufSize) {
            printf("L2_SECURITY: security extension doesn't fit into message\n");
            return 0;
        }

        /* start encoding ... */
        if (encode)
        {
            bufPos = BerEncoder_encodeTL(0xa0, securityExtensionSize, buffer, bufPos);

            bufPos = BerEncoder_encodeTL(0xa4, authValueSize, buffer, bufPos);

            /* encode AuthenticationValue content */

            /* Version */
            bufPos =  BerEncoder_encodeInt32WithTL(0x80, 1, buffer, bufPos);

            /* TimeofCurrentKey */
            bufPos = BerEncoder_encodeInt32WithTL(0x81, self->timeOfCurrentKey, buffer, bufPos);

            /* TimeofNextKey */
            bufPos = BerEncoder_encodeInt32WithTL(0x82, self->timeToNextKey, buffer, bufPos);

            /* IV */
            if (ivSize > 0)
            {
                bufPos = BerEncoder_encodeTL(0x83, ivSize, buffer, bufPos);
                ivBuf = buffer + bufPos;
                bufPos += ivSize;
            }

            /* KeyID */
            bufPos = BerEncoder_encodeInt32WithTL(0x84, self->currentKeyId, buffer, bufPos);

            int macEnd = bufPos;

            /* encode mAC */
            bufPos = BerEncoder_encodeTL(0x85, mACSize - 2, buffer, bufPos);

            if (self->currentSigAlgo == MC_SEC_SIG_ALGO_HMAC_SHA256_128) {
                RSessionCrypto_createHMAC(buffer + start, macEnd, self->currentKey, self->currentKeySize, buffer + bufPos, 16);
                bufPos += 16;
            }
            else if (self->currentSigAlgo == MC_SEC_SIG_ALGO_HMAC_SHA256_256) {
                RSessionCrypto_createHMAC(buffer + start, macEnd - start, self->currentKey, self->currentKeySize, buffer + bufPos, 32);
                bufPos += 32;
            }
            else if (self->currentSigAlgo == MC_SEC_SIG_ALGO_AES_GMAC_128)
            {
                /* create IV */
                if (RSessionCrypto_createRandomData(ivBuf, ivSize) == false) {
                    printf("ERROR - Failed to create random IV\n");
                }

                if (RSessionCrypto_createAES_GMAC(self->currentKey, self->currentKeySize, ivBuf, ivSize, buffer + start, macEnd - start, buffer + bufPos, 16) == false)
                {
                    printf("ERROR - Failed to create GMAC\n");
                }

                bufPos += 16;
            }
            else if (self->currentSigAlgo == MC_SEC_SIG_ALGO_AES_GMAC_256)
            {
                /* create IV */
                if (RSessionCrypto_createRandomData(ivBuf, ivSize) == false) {
                    printf("ERROR - Failed to create random IV\n");
                }

                if (RSessionCrypto_createAES_GMAC(self->currentKey, self->currentKeySize, ivBuf, ivSize, buffer + start, macEnd - start, buffer + bufPos, 32) == false)
                {
                    printf("ERROR - Failed to create GMAC\n");
                }

                bufPos += 32;
            }
            else {
                /* signature algorithm not supported */
                printf("Signature algorithm not supported\n");
                return 0;
            }
        }

        return securityExtensionSize + 2;
    }
    else
    {
        printf("L2_SECURITY: no signature algorithm set\n");
        return 0;
    }
}

static bool
checkSecurityExtension(L2Security self, uint8_t* buffer, int secExtLen, uint8_t* macStart)
{
    bool hasIV = false;
    int ivSize = 0;
    uint8_t* ivBuffer = NULL;
    int mACSize = 0;
    uint8_t* mACBuffer = NULL;
    int bufPos = 0;

    /* determine length of the mAC */
    if (self->currentSigAlgo == MC_SEC_SIG_ALGO_HMAC_SHA256_128) {
        mACSize = 2 + 16;
    }
    else if (self->currentSigAlgo == MC_SEC_SIG_ALGO_HMAC_SHA256_256) {
        mACSize = 2 + 32;
    }
    else if (self->currentSigAlgo == MC_SEC_SIG_ALGO_AES_GMAC_128) {
        mACSize = 2 + 16;
    }
    else if (self->currentSigAlgo == MC_SEC_SIG_ALGO_AES_GMAC_256) {
        mACSize = 2 + 32;
    }
    else {
        /* signature algorithm not supported */
        printf("L2_SECURITY: signature algorithm not supported\n");
        return false;
    }

    while (bufPos < secExtLen)
    {
        int macEnd = (buffer - macStart) + bufPos;

        uint8_t tag = buffer[bufPos++];

        int len = 0;

        bufPos = BerDecoder_decodeLength(buffer, &len, bufPos, secExtLen);

        if (bufPos == -1)
        {
            printf("L2_SECURITY: invalid len for tag %02x\n", tag);
            return false;
        }

        if (tag == 0xa4)
        {
            /* AuthenticationValue */
            uint8_t* authValueBuf = buffer + bufPos;
            int authValuePos = 0;
            int authValueLen = len;

            while (authValuePos < authValueLen)
            {
                uint8_t authTag = authValueBuf[authValuePos++];

                int authLen = 0;

                authValuePos = BerDecoder_decodeLength(authValueBuf, &authLen, authValuePos, authValueLen);

                if (authValuePos == -1)
                {
                    printf("L2_SECURITY: invalid len for tag %02x in AuthenticationValue\n", authTag);
                    return false;
                }

                if (authTag == 0x80)
                {
                    /* Version */
                    int32_t version = BerDecoder_decodeInt32(authValueBuf, authLen, authValuePos);

                    if (version != 1)
                    {
                        printf("L2_SECURITY: invalid version (%i) in AuthenticationValue\n", version);
                        return false;
                    }
                }
                else if (authTag == 0x81)
                {
                    /* TimeofCurrentKey */
                    self->timeOfCurrentKey = BerDecoder_decodeUint32(authValueBuf, authLen, authValuePos);
                }
                else if (authTag == 0x82)
                {
                    /* TimeToNextKey */
                    self->timeToNextKey = BerDecoder_decodeInt32(authValueBuf, authLen, authValuePos);
                }
                else if (authTag == 0x83)
                {
                    /* IV */
                    hasIV = true;
                    ivBuffer = authValueBuf + authValuePos;
                    ivSize = authLen;
                }
                else if (authTag == 0x84)
                {
                    /* KeyID */
                    uint32_t keyId = BerDecoder_decodeUint32(authValueBuf, authLen, authValuePos);

                    if (keyId != self->currentKeyId)
                    {
                        printf("L2_SECURITY: invalid key ID in AuthenticationValue\n");
                        return false;
                    }
                }
                else
                {
                    printf("L2_SECURITY: invalid tag in AuthenticationValue\n");
                    return false;
                }

                authValuePos += authLen;
            }
        }
        else if (tag == 0x85)
        {
            /* mAC */

            mACBuffer = buffer + bufPos;
            mACSize = len;

            printf("L2_SECURITY: found MAC with size: %i\n", mACSize);

            if (self->currentSigAlgo == MC_SEC_SIG_ALGO_HMAC_SHA256_128)
            {
                printf("Algo: HMAC_SHA256_128\n");

                uint8_t calculatedMac[16];

                RSessionCrypto_createHMAC(macStart, macEnd, self->currentKey, self->currentKeySize, calculatedMac, sizeof(calculatedMac));

                if (memcmp(calculatedMac, mACBuffer, 16) != 0)
                {
                    printf("L2_SECURITY: MAC mismatch\n");
                    return false;
                }
            }
            else if (self->currentSigAlgo == MC_SEC_SIG_ALGO_HMAC_SHA256_256)
            {
                printf("Algo: HMAC_SHA256_256\n");

                uint8_t calculatedMac[32];

                RSessionCrypto_createHMAC(macStart, macEnd, self->currentKey, self->currentKeySize, calculatedMac, sizeof(calculatedMac));

                if (memcmp(calculatedMac, mACBuffer, 32) != 0)
                {
                    printf("L2_SECURITY: MAC mismatch\n");
                    return false;
                }
            }
            else if (self->currentSigAlgo == MC_SEC_SIG_ALGO_AES_GMAC_128)
            {
                printf("Algo: AES_GMAC_128\n");

                uint8_t calculatedMac[16];

                printf("IV: ");
                printBuffer(ivBuffer, ivSize);

                printf("MAC: ");
                printBuffer(mACBuffer, mACSize);

                if (RSessionCrypto_createAES_GMAC(self->currentKey, self->currentKeySize, ivBuffer, ivSize, macStart, macEnd, calculatedMac, 16) == false)
                {
                    printf("L2_SECURITY: GMAC calculation failed\n");
                    return false;
                }

                if (memcmp(calculatedMac, mACBuffer, 16) != 0)
                {
                    printf("L2_SECURITY: GMAC mismatch\n");
                    return false;
                }
            }
            else if (self->currentSigAlgo == MC_SEC_SIG_ALGO_AES_GMAC_256)
            {
                printf("Algo: AES_GMAC_256\n");

                printf("IV: ");
                printBuffer(ivBuffer, ivSize);

                printf("MAC: ");
                printBuffer(mACBuffer, mACSize);

                uint8_t calculatedMac[8];

                if (RSessionCrypto_createAES_GMAC(self->currentKey, self->currentKeySize, ivBuffer, ivSize, macStart, macEnd, calculatedMac, 32) == false)
                {
                    printf("L2_SECURITY: GMAC calculation failed\n");
                    return false;
                }

                if (memcmp(calculatedMac, mACBuffer, 8) != 0)
                {
                    printf("L2_SECURITY: GMAC mismatch\n");
                    return false;
                }
            }
            else
            {
                printf("L2_SECURITY: signature algorithm not supported\n");
                return false;
            }
        }
        else {
            printf("L2_SECURITY: invalid tag %02x in security extension\n", tag);

            return false;
        }

        bufPos += len;
    }


    return true;
}

bool
L2Security_checkSecurityExtension(L2Security self, uint8_t* buffer, int start, int length, int secExtSize)
{
    if (self->currentSigAlgo == MC_SEC_SIG_ALGO_NONE)
    {
        if (secExtSize > 0)
        {
            printf("L2_SECURITY: security extension found but no security association\n");
            return false;
        }
        else
        {
            return true;
        }
    }
    else 
    {
        uint8_t* secExtBuf = buffer + start + length;

        int bufPos = 0;

        while (bufPos < secExtSize)
        {
            uint8_t tag = secExtBuf[bufPos++];
            int len = 0;

            bufPos = BerDecoder_decodeLength(secExtBuf, &len, bufPos, secExtSize);

            if (bufPos == -1)
            {
                printf("L2_SECURITY: [2] invalid len for tag %02x\n", tag);
                return false;
            }

            if (tag == 0xa0)
            {
                /* SecurityExtension */
                printf("L2_SECURITY: found security extension\n");
                return checkSecurityExtension(self, secExtBuf + bufPos, len, buffer + start - 2);
            }
            else {
                printf("L2_SECURITY: invalid tag %02x in security extension\n", tag);
                return false;
            }
        }

        return false;
    }
}

L2Security
L2Security_create()
{
    L2Security self = (L2Security)GLOBAL_CALLOC(1, sizeof(struct sL2Security));

    if (self) {

    }

    return self;
}

MCKeyError
L2Security_addKey(L2Security self, uint32_t keyId, uint8_t* key, int keyLength, MCSecurityAlgorithm secAlgo, MCSignatureAlgorithm sigAlgo)
{
    self->currentKeyId = keyId;
    self->currentSigAlgo = sigAlgo;
    self->currentKey = key;
    self->currentKeySize = keyLength;
}

MCKeyError
L2Security_removeKey(L2Security self, uint32_t keyId)
{

}

void
L2Security_removeAllKeys(L2Security self)
{

}

RSessionError
L2Security_setActiveKey(L2Security self, uint32_t keyId)
{
    self->currentKeyId = keyId;
}

void
L2Security_destroy(L2Security self)
{
    if (self) {
        GLOBAL_FREEMEM(self);
    }
}
