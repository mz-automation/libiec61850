/*
 *  r_session_crypto.h
 *
 *  Copyright 2013-2021 Michael Zillgith
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

#ifndef SRC_R_SESSION_R_SESSION_CRYPTO_H_
#define SRC_R_SESSION_R_SESSION_CRYPTO_H_

#include <stdint.h>
#include <stdbool.h>

#include "libiec61850_common_api.h"

LIB61850_INTERNAL bool
RSessionCrypto_createHMAC(uint8_t* buffer, int bufSize, uint8_t* key, int keySize, uint8_t* hmac, int hmacMaxSize);

LIB61850_INTERNAL bool
RSessionCrypto_gcmEncryptAndTag(uint8_t* key, int keySize, uint8_t* iv, int ivSize, uint8_t* addData, int addDataSize, uint8_t* encryptData, int encryptDataSize, uint8_t* tag, int tagSize);

LIB61850_INTERNAL bool
RSessionCrypto_gcmAuthAndDecrypt(uint8_t* key, int keySize, uint8_t* iv, int ivSize, uint8_t* addData, int addDataSize, uint8_t* encryptData, int encryptDataSize, uint8_t* decryptedData, uint8_t* tag, int tagSize);

LIB61850_INTERNAL bool
RSessionCrypto_createRandomData(uint8_t* data, int dataSize);

#endif /* SRC_R_SESSION_R_SESSION_CRYPTO_H_ */
