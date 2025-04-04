// https://github.com/Mbed-TLS/mbedtls/blob/development/docs/3.0-migration-guide.md#introduce-a-level-of-indirection-and-versioning-in-the-config-files
// #ifndef MBEDTLS_CONFIG_H
// #define MBEDTLS_CONFIG_H

/* System support */
#define MBEDTLS_HAVE_ASM
#define MBEDTLS_HAVE_TIME
#define MBEDTLS_HAVE_TIME_DATE
#define MBEDTLS_NO_UDBL_DIVISION
#define MBEDTLS_PLATFORM_C
#define MBEDTLS_DEBUG_C

/* mbed TLS feature support */
#define MBEDTLS_CIPHER_MODE_CBC
#define MBEDTLS_PKCS1_V15
#define MBEDTLS_KEY_EXCHANGE_RSA_ENABLED
#define MBEDTLS_SSL_PROTO_TLS1_2
#define MBEDTLS_SSL_PROTO_TLS1_3
// MIGRATE 2.28->3.x.x: https://github.com/Mbed-TLS/mbedtls/blob/development/docs/3.0-migration-guide.md#remove-support-for-tls-10-11-and-dtls-10
// #define MBEDTLS_SSL_PROTO_TLS1_1
// #define MBEDTLS_SSL_PROTO_TLS1
#define MBEDTLS_SSL_RENEGOTIATION

#error "MBEDTLS"

#define MBEDTLS_TLS_DEFAULT_ALLOW_SHA1_IN_CERTIFICATES

/* mbed TLS modules */
#define MBEDTLS_GCM_C
#define MBEDTLS_AES_C
#define MBEDTLS_ASN1_PARSE_C
#define MBEDTLS_ASN1_WRITE_C
#define MBEDTLS_BIGNUM_C
#define MBEDTLS_CIPHER_C
#define MBEDTLS_CTR_DRBG_C
/* #define MBEDTLS_DES_C */
#define MBEDTLS_ENTROPY_C
#define MBEDTLS_MD_C
#define MBEDTLS_MD5_C
#define MBEDTLS_NET_C
#define MBEDTLS_OID_C
#define MBEDTLS_PK_C
#define MBEDTLS_PK_PARSE_C
#define MBEDTLS_RSA_C
#define MBEDTLS_SHA1_C
#define MBEDTLS_SHA256_C
#define MBEDTLS_SSL_CLI_C
#define MBEDTLS_SSL_SRV_C
#define MBEDTLS_SSL_TLS_C
#define MBEDTLS_X509_CRT_PARSE_C
#define MBEDTLS_X509_CRL_PARSE_C
#define MBEDTLS_X509_USE_C
#define MBEDTLS_SSL_CACHE_C

/* For test certificates */
#define MBEDTLS_BASE64_C
#define MBEDTLS_CERTS_C
#define MBEDTLS_PEM_PARSE_C

#define MBEDTLS_PKCS12_C
#define MBEDTLS_PKCS5_C

/* For testing with compat.sh */
#define MBEDTLS_FS_IO

// MIGRATE 2.28->3.x.x: https://github.com/Mbed-TLS/mbedtls/blob/development/docs/3.0-migration-guide.md#remove-mbedtls_x509_check__key_usage-options-from-mbedtls_configh
// #define MBEDTLS_X509_CHECK_KEY_USAGE
// #define MBEDTLS_X509_CHECK_EXTENDED_KEY_USAGE

// MIGRATE 2.28->3.x.x: https://github.com/Mbed-TLS/mbedtls/blob/development/docs/3.0-migration-guide.md#introduce-a-level-of-indirection-and-versioning-in-the-config-files
// #include "mbedtls/check_config.h"

// #endif /* MBEDTLS_CONFIG_H */
