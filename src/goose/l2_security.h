/*
 *  l2_security.h
 *
 *  Copyright 2022 Michael Zillgith
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

#ifndef LIBIEC61850_L2_SECURITY_H_
#define LIBIEC61850_L2_SECURITY_H_

#include "libiec61850_common_api.h"
#include "r_session.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    MULTICAST_KEY_ERROR_OK = 0,
    MULTICAST_KEY_ERROR_INVALID_KEY = 1,
    MULTICAST_KEY_ERROR_KEY_QUEUE_FULL = 2,
    MULTICAST_KEY_ERROR_SEC_ALGO_NOT_SUPPORTED = 3,
    MULTICAST_KEY_ERROR_SIG_ALGO_NOT_SUPPORTED = 4,
    MULTICAST_KEY_ERROR_OUT_OF_MEMORY = 5,
    MULTICAST_KEY_ERROR_INVALID_MESSAGE = 7,
    MULTICAST_KEY_ERROR_SET_FAILED = 8
} MCKeyError;

typedef enum {
    MC_SEC_SEC_ALGO_NONE = 0,
    MC_SEC_SEC_ALGO_AES_128_GCM = 1,
    MC_SEC_SEC_ALGO_AES_256_GCM = 2
} MCSecurityAlgorithm;

typedef enum {
    MC_SEC_SIG_ALGO_NONE = 0,
    MC_SEC_SIG_ALGO_HMAC_SHA256_80 = 1,
    MC_SEC_SIG_ALGO_HMAC_SHA256_128 = 2,
    MC_SEC_SIG_ALGO_HMAC_SHA256_256 = 3,
    MC_SEC_SIG_ALGO_AES_GMAC_64 = 4,
    MC_SEC_SIG_ALGO_AES_GMAC_128 = 5,
    MC_SEC_SIG_ALGO_HMAC_SHA3_80 = 6,
    MC_SEC_SIG_ALGO_HMAC_SHA3_128 = 7,
    MC_SEC_SIG_ALGO_HMAC_SHA3_256 = 8
} MCSignatureAlgorithm;

typedef enum
{
    MC_SEC_KEY_EVENT__NEED_KEY = 1
} MCKeyEvent;

typedef void (*MC_KeyEventHandler) (void* parameter, RSession rSession, MCKeyEvent event, uint32_t keyID);



typedef struct sL2Security* L2Security;

LIB61850_API L2Security
L2Security_create();

LIB61850_API MCKeyError
L2Security_addKey(L2Security self, uint32_t keyId, uint8_t* key, int keyLength, MCSecurityAlgorithm secAlgo, MCSignatureAlgorithm sigAlgo);

/**
 * \brief Remove key from the list of accepted keys
 * 
 * \param self the MulticastKeystore instance
 * \param keyId the key ID is unique for the security association
 */
LIB61850_API MCKeyError
L2Security_removeKey(L2Security self, uint32_t keyId);

/**
 * \brief Remove all keys from the list of accepted keys
 * 
 * \param self the L2Security instance 
 */
void
L2Security_removeAllKeys(L2Security self);

LIB61850_API RSessionError
L2Security_setActiveKey(L2Security self, uint32_t keyId);

uint16_t
L2Security_calculateCRC16(uint8_t* data, int size);

/**
 * \brief Create the security extension
 * 
 * \param buffer buffer with the encoded GOOSE message (security extension will be added)
 * \param start start of the message payload used to calculate the MAC
 * \param length length of the message payload
 * \param encode true if the security extension should be encoded, false only length calculation without modifying the buffer
 * 
 * \return length of the security extension
 */
uint16_t
L2Security_addSecurityExtension(L2Security self, uint8_t* buffer, int start, int length, int maxBufSize, bool encode);

/**
 * \brief Check the security extension
 * 
 * \param buffer buffer with the encoded GOOSE message
 * \param start start of the message payload used to calculate the MAC
 * \param length length of the message payload
 * \param secExtSize size of the security extension
 * 
 * \return true if the security extension is valid
 */
bool
L2Security_checkSecurityExtension(L2Security self, uint8_t* buffer, int start, int length, int secExtSize);

LIB61850_API void
L2Security_destroy(L2Security self);

#ifdef __cplusplus
}
#endif

#endif /* LIBIEC61850_L2_SECURITY_H_ */