/*
 *  r_session_crpyto_mbedtls.c
 *
 *  Implementation of RSessionCrypto interface using mbedtls
 *
 *  Copyright 2013-2023 Michael Zillgith
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


#include "mbedtls/cipher.h"
#include "mbedtls/md.h"
#include "mbedtls/md_internal.h"
#include "mbedtls/platform_util.h"
#include "mbedtls/gcm.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"

#include "libiec61850_platform_includes.h"

#include "r_session_crypto.h"

bool
RSessionCrypto_createHMAC(uint8_t* buffer, int bufSize, uint8_t* key, int keySize, uint8_t* hmac, int hmacMaxSize)
{
    const mbedtls_md_info_t* md_info = &mbedtls_sha256_info;

    mbedtls_md_context_t md_ctx;

    mbedtls_md_init( &md_ctx );

    mbedtls_md_setup(&md_ctx, md_info, 1);

    if (mbedtls_md_hmac_starts(&md_ctx, key, keySize)) {
        printf("Error in initializing HMAC\n");
        return false;
    }

    if (mbedtls_md_hmac_update(&md_ctx, buffer, bufSize)) {
        printf("Failed to calculate HMAC\n");
        return false;
    }

    if (mbedtls_md_hmac_finish(&md_ctx, hmac)) {
        printf("Failed to finish HMAC\n");
        return false;
    }

    mbedtls_md_free(&md_ctx);

    return true;
}

bool
RSessionCrypto_gcmEncryptAndTag(uint8_t* key, int keySize, uint8_t* iv, int ivSize, uint8_t* addData, int addDataSize, uint8_t* encryptData, int encryptDataSize, uint8_t* tag, int tagSize)
{
    mbedtls_gcm_context gcmCtx;

    mbedtls_gcm_init(&gcmCtx);

    if (mbedtls_gcm_setkey(&gcmCtx, MBEDTLS_CIPHER_ID_AES , (const unsigned char*) key, keySize * 8)) {
        printf("AES-GCM: Failed to set key\n");
        mbedtls_gcm_free(&gcmCtx);
        return false;
    }

    if (mbedtls_gcm_crypt_and_tag(&gcmCtx, MBEDTLS_GCM_ENCRYPT, (size_t) encryptDataSize, iv, (size_t) ivSize, addData, (size_t) addDataSize, encryptData, encryptData, (size_t)tagSize, tag)) {
        printf("AES-GCM: Failed to authenticate/encrypt data\n");
        mbedtls_gcm_free(&gcmCtx);
        return false;
    }

    mbedtls_gcm_free(&gcmCtx);

    return true;
}

bool
RSessionCrypto_gcmAuthAndDecrypt(uint8_t* key, int keySize, uint8_t* iv, int ivSize, uint8_t* addData, int addDataSize, uint8_t* encryptData, int encryptDataSize, uint8_t* decryptedData, uint8_t* tag, int tagSize)
{
    mbedtls_gcm_context gcmCtx;

    mbedtls_gcm_init(&gcmCtx);

    if (mbedtls_gcm_setkey(&gcmCtx, MBEDTLS_CIPHER_ID_AES , (const unsigned char*) key, keySize * 8)) {
        printf("AES-GCM: Failed to set key\n");
        mbedtls_gcm_free(&gcmCtx);
        return false;
    }

    if (mbedtls_gcm_auth_decrypt(&gcmCtx, (size_t) encryptDataSize, iv, (size_t) ivSize, addData, (size_t) addDataSize, tag, (size_t) tagSize, encryptData, decryptedData)) {
        printf("AES-GCM: Failed to authentication and decrypt!\n");
        mbedtls_gcm_free(&gcmCtx);
        return false;
    }

    mbedtls_gcm_free(&gcmCtx);

    return true;
}

bool
RSessionCrypto_createRandomData(uint8_t* data, int dataSize)
{
    int ret;
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_entropy_context entropy;

    mbedtls_entropy_init( &entropy );
    mbedtls_ctr_drbg_init( &ctr_drbg );

    if( ( ret = mbedtls_ctr_drbg_seed( &ctr_drbg, mbedtls_entropy_func, &entropy,
        NULL, 0) ) != 0 )
    {
        printf( " failed\n ! mbedtls_ctr_drbg_init returned -0x%04x\n", -ret );
        return false;
    }

    if( ( ret = mbedtls_ctr_drbg_random( &ctr_drbg, data, dataSize ) ) != 0 )
    {
        printf( " failed\n ! mbedtls_ctr_drbg_random returned -0x%04x\n", -ret );
        return false;
    }

    return true;
}

