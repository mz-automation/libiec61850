/*
 * tls_mbedtls.c
 *
 * TLS API for TCP/IP protocol stacks
 *
 * Copyright 2017-2024 Michael Zillgith
 *
 * Implementation of the TLS abstraction layer for mbedtls
 *
 */

#include <string.h>

#include "tls_socket.h"
#include "hal_thread.h"
#include "lib_memory.h"
#include "hal_time.h"
#include "linked_list.h"

#include "mbedtls/platform.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/x509.h"
#include "mbedtls/ssl.h"
#include "mbedtls/net_sockets.h"
#include "mbedtls/error.h"
#include "mbedtls/debug.h"
#include "mbedtls/ssl_cache.h"

#define SEC_EVENT_ALARM 2
#define SEC_EVENT_WARNING 1
#define SEC_EVENT_INFO 0

#ifndef CONFIG_DEBUG_TLS
#define CONFIG_DEBUG_TLS 0
#endif

#if (CONFIG_DEBUG_TLS == 1)
#define DEBUG_PRINT(appId, fmt, ...) fprintf(stderr, "%s: " fmt, appId, ## __VA_ARGS__)
#else
#define DEBUG_PRINT(fmt, ...) do {} while(0)
#endif


struct sTLSConfiguration {
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;

    mbedtls_x509_crt ownCertificate;
    mbedtls_pk_context ownKey;

    mbedtls_x509_crt cacerts;

    mbedtls_x509_crl crl;

    mbedtls_ssl_config conf;
    LinkedList /* <mbedtls_x509_crt*> */ allowedCertificates;

    /* session cache for server */
    mbedtls_ssl_cache_context cache;

    /* client side cached session */
    mbedtls_ssl_session* savedSession;
    uint64_t savedSessionTime;

    bool chainValidation;
    bool allowOnlyKnownCertificates;
    bool timeValidation;

    /* TLS session renegotiation interval in milliseconds */
    int renegotiationTimeInMs;

    /* TLS minimum version allowed (default: TLS_VERSION_TLS_1_0) */
    TLSConfigVersion minVersion;
    
    /* TLS maximum version allowed (default: TLS_VERSION_TLS_1_2) */
    TLSConfigVersion maxVersion;

    TLSConfiguration_EventHandler eventHandler;
    void* eventHandlerParameter;

    /* time of the last CRL update */
    uint64_t crlUpdated;

    bool setupComplete;

    bool useSessionResumption;
    int sessionResumptionInterval; /* session resumption interval in seconds */

    int* ciphersuites;
    int maxCiphersuites;
};

struct sTLSSocket {
    mbedtls_ssl_context ssl;
    Socket socket;
    mbedtls_ssl_config conf;
    TLSConfiguration tlsConfig;
    bool storePeerCert;
    uint8_t* peerCert;
    int peerCertLength;

    /* time of last session renegotiation (used to calculate next renegotiation time) */
    uint64_t lastRenegotiationTime;

    /* time of the last CRL update */
    uint64_t crlUpdated;
};

static void
raiseSecurityEvent(TLSConfiguration config, TLSEventLevel eventCategory, int eventCode, const char* message, TLSSocket socket)
{
    if (config->eventHandler) {
        config->eventHandler(config->eventHandlerParameter, eventCategory, eventCode, message, (TLSConnection)socket);
    }
}

static bool
compareCertificates(mbedtls_x509_crt *crt1, mbedtls_x509_crt *crt2)
{
    if (crt1 != NULL && crt2 != NULL)
    {
        if (crt1->sig.len == crt2->sig.len)
        {
            if (memcmp(crt1->sig.p, crt2->sig.p, crt1->sig.len) == 0)
                return true;
        }

    }

    return false;
}

static int
verifyCertificate (void* parameter, mbedtls_x509_crt *crt, int certificate_depth, uint32_t *flags)
{
    TLSSocket self = (TLSSocket) parameter;

    DEBUG_PRINT("TLS", "Verify cert: depth %i\n", certificate_depth);

    DEBUG_PRINT("TLS", "   flags: %08x\n", *flags);

    char buffer[1024];

    mbedtls_x509_crt_info(buffer, 1023, "  ", crt);

    DEBUG_PRINT("TLS", "%s\n", buffer);

    if (self->tlsConfig->chainValidation == false)
    {
        if (certificate_depth != 0)
            *flags = 0;
    }

    if (certificate_depth == 0)
    {
        if (self->tlsConfig->allowOnlyKnownCertificates)
        {
            DEBUG_PRINT("TLS", "Check against list of allowed certs\n");

            bool certMatches = false;

            LinkedList certList = LinkedList_getNext(self->tlsConfig->allowedCertificates);

            while (certList)
            {
                mbedtls_x509_crt* allowedCert = (mbedtls_x509_crt*) LinkedList_getData(certList);

                DEBUG_PRINT("TLS", "Compare With:\n");
                mbedtls_x509_crt_info(buffer, 1023, "     ", allowedCert);
                DEBUG_PRINT("TLS", "%s\n", buffer);

                if (compareCertificates(allowedCert, crt))
                {
                    certMatches = true;
                    break;
                }

                certList = LinkedList_getNext(certList);
            }

            if (certMatches)
            {
                if (self->tlsConfig->chainValidation == false)
                    *flags = 0;
            }
            else
            {
                raiseSecurityEvent(self->tlsConfig, TLS_SEC_EVT_INCIDENT, TLS_EVENT_CODE_ALM_CERT_NOT_CONFIGURED, "Alarm: certificate validation: trusted individual certificate not available", self);

                *flags |= MBEDTLS_X509_BADCERT_OTHER;
                return 1;
            }
        }

        if (self->tlsConfig->timeValidation == false)
        {
            if (*flags & MBEDTLS_X509_BADCERT_EXPIRED)
            {
                *flags = *flags - MBEDTLS_X509_BADCERT_EXPIRED;

                raiseSecurityEvent(self->tlsConfig, TLS_SEC_EVT_WARNING, TLS_EVENT_CODE_WRN_CERT_EXPIRED, "Warning: certificate validation: using expired certificate", self);
            }

            if (*flags & MBEDTLS_X509_BADCRL_EXPIRED)
            {
                *flags = *flags - MBEDTLS_X509_BADCRL_EXPIRED;

                raiseSecurityEvent(self->tlsConfig, TLS_SEC_EVT_WARNING, TLS_EVENT_CODE_WRN_CRL_EXPIRED, "Warning: certificate validation: using expired CRL", self);
            }

            if (*flags & MBEDTLS_X509_BADCERT_FUTURE)
            {
                *flags = *flags - MBEDTLS_X509_BADCERT_FUTURE;

                raiseSecurityEvent(self->tlsConfig, TLS_SEC_EVT_WARNING, TLS_EVENT_CODE_WRN_CERT_NOT_YET_VALID, "Warning: certificate validation: using certificate with validity in future", self);
            }

            if (*flags & MBEDTLS_X509_BADCRL_FUTURE)
            {
                *flags = *flags - MBEDTLS_X509_BADCRL_FUTURE;

                raiseSecurityEvent(self->tlsConfig, TLS_SEC_EVT_WARNING, TLS_EVENT_CODE_WRN_CRL_NOT_YET_VALID, "Warning: certificate validation: using CRL with validity in future", self);
            }
        }

        if (self->storePeerCert)
        {
            if (*flags == 0)
            {
                self->peerCertLength = 0;
                self->peerCert = (uint8_t*) GLOBAL_MALLOC(crt->raw.len);

                if (self->peerCert)
                {
                    self->peerCertLength = (int)crt->raw.len;
                    memcpy(self->peerCert, crt->raw.p, self->peerCertLength);
                }
            }
        }
    }

    printf(" flags: %u\n", *flags);

    return 0;
}

/*
 * Finish configuration when used the first time.
 */
static bool
TLSConfiguration_setupComplete(TLSConfiguration self)
{
    if (self->setupComplete == false)
    {
        mbedtls_ssl_conf_ca_chain( &(self->conf), &(self->cacerts), &(self->crl) );

        if (self->ownCertificate.version > 0)
        {
            int ret = mbedtls_ssl_conf_own_cert( &(self->conf), &(self->ownCertificate), &(self->ownKey));

            if (ret != 0) {
                DEBUG_PRINT("TLS", "mbedtls_ssl_conf_own_cert returned -0x%x\n", -ret);
                return false;
            }
        }

        if (self->useSessionResumption)
        {
            if (self->conf.endpoint == MBEDTLS_SSL_IS_CLIENT) {

            }
            else
            {
                mbedtls_ssl_cache_init( &(self->cache) );

                self->cache.timeout = self->sessionResumptionInterval;

                mbedtls_ssl_conf_session_cache( &(self->conf), &(self->cache),
                                   mbedtls_ssl_cache_get,
                                   mbedtls_ssl_cache_set );
            }
        }

        mbedtls_ssl_conf_ciphersuites(&(self->conf), self->ciphersuites);

        self->setupComplete = true;
    }

    return true;
}

void
TLSConfiguration_addCipherSuite(TLSConfiguration self, int ciphersuite)
{
    /* search last index */
    int nextIndex = 0;
    int i;

    for (i = 0; i < self->maxCiphersuites; i++)
    {
        if (self->ciphersuites[i] == 0)
        {
            nextIndex = i;
            break;
        }
    }

    if (nextIndex == self->maxCiphersuites)
    {
        /* reallocate space for ciphersuites list */
        int newMaxCiphersuites = self->maxCiphersuites + 10;

        int* newCiphersuites = (int*)GLOBAL_CALLOC(newMaxCiphersuites, sizeof(int));

        if (newCiphersuites)
        {
            for (i = 0; i < self->maxCiphersuites; i++)
            {
                newCiphersuites[i] = self->ciphersuites[i];
            }

            GLOBAL_FREEMEM(self->ciphersuites);
            self->ciphersuites = newCiphersuites;
            self->maxCiphersuites = newMaxCiphersuites;

            TLSConfiguration_addCipherSuite(self, ciphersuite);
        }
    }
    else
    {
        self->ciphersuites[nextIndex] = ciphersuite;
        self->ciphersuites[nextIndex + 1] = 0;
    }
}

void
TLSConfiguration_clearCipherSuiteList(TLSConfiguration self)
{
    self->ciphersuites[0] = 0;
}

TLSConfiguration
TLSConfiguration_create()
{
    TLSConfiguration self = (TLSConfiguration) GLOBAL_CALLOC(1, sizeof(struct sTLSConfiguration));

    if (self)
    {
        mbedtls_ssl_config_init( &(self->conf) );
        mbedtls_x509_crt_init( &(self->ownCertificate) );
        mbedtls_x509_crt_init( &(self->cacerts) );
        mbedtls_x509_crl_init( &(self->crl) );
        mbedtls_pk_init( &(self->ownKey) );
        mbedtls_entropy_init( &(self->entropy) );
        mbedtls_ctr_drbg_init( &(self->ctr_drbg) );

        /* WARINING is fixed to server! */
        mbedtls_ssl_config_defaults( &(self->conf),
                MBEDTLS_SSL_IS_SERVER,
                MBEDTLS_SSL_TRANSPORT_STREAM,
                MBEDTLS_SSL_PRESET_DEFAULT );

        mbedtls_ctr_drbg_seed( &(self->ctr_drbg), mbedtls_entropy_func, &(self->entropy), NULL, 0);
        mbedtls_ssl_conf_rng( &(self->conf), mbedtls_ctr_drbg_random, &(self->ctr_drbg) );

        mbedtls_ssl_conf_authmode(&(self->conf), MBEDTLS_SSL_VERIFY_REQUIRED);

        mbedtls_ssl_conf_renegotiation(&(self->conf), MBEDTLS_SSL_RENEGOTIATION_ENABLED);

        self->minVersion = TLS_VERSION_TLS_1_2;
        self->maxVersion = TLS_VERSION_NOT_SELECTED;

        self->renegotiationTimeInMs = -1; /* no automatic renegotiation */

        self->allowedCertificates = LinkedList_create();

        /* default behavior is to allow all certificates that are signed by the CA */
        self->chainValidation = true;
        self->allowOnlyKnownCertificates = false;

        /* default behaviour is to check for valid-from and expiration times */
        self->timeValidation = true;
        self->setupComplete = false;

        self->eventHandler = NULL;
        self->eventHandlerParameter = NULL;

        self->useSessionResumption = true;
        self->sessionResumptionInterval = 21600; /* default value: 6h */
        self->savedSession = NULL;
        self->savedSessionTime = 0;

        self->ciphersuites = (int*)GLOBAL_CALLOC(20, sizeof(int));

        if (self->ciphersuites)
        {
            self->maxCiphersuites = 20;

            /* mandatory cipher suites by IEC 62351-4:2018 */
            self->ciphersuites[0] = MBEDTLS_TLS_RSA_WITH_AES_128_CBC_SHA256;
            /* self->ciphersuites[1] = MBEDTLS_TLS_DH_RSA_WITH_AES_128_GCM_SHA256; */ /* weak - not supported? */
            self->ciphersuites[1] = MBEDTLS_TLS_DHE_RSA_WITH_AES_128_GCM_SHA256;
            self->ciphersuites[2] = MBEDTLS_TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256;

            /* recommended cipher suites by IEC 62351-4:2018 */

            /* self->ciphersuites[1] = MBEDTLS_TLS_DH_RSA_WITH_AES_128_CBC_SHA256; */ /* weak - not supported?*/
            /* self->ciphersuites[1] = MBEDTLS_TLS_DH_RSA_WITH_AES_256_GCM_SHA384; */ /* not supported?*/
            self->ciphersuites[3] = MBEDTLS_TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256;
            self->ciphersuites[4] = MBEDTLS_TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384;
            self->ciphersuites[5] = MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384;

            /* additional ciphersuites */
            self->ciphersuites[6] = MBEDTLS_TLS_RSA_WITH_NULL_SHA256;
            self->ciphersuites[7] = MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384;
        }
    }

    return self;
}

void
TLSConfiguration_setClientMode(TLSConfiguration self)
{
    mbedtls_ssl_conf_endpoint(&(self->conf), MBEDTLS_SSL_IS_CLIENT);
}

void
TLSConfiguration_enableSessionResumption(TLSConfiguration self, bool enable)
{
    self->useSessionResumption = enable;
}

void
TLSConfiguration_setSessionResumptionInterval(TLSConfiguration self, int intervalInSeconds)
{
    self->sessionResumptionInterval = intervalInSeconds;
}

void
TLSConfiguration_setEventHandler(TLSConfiguration self, TLSConfiguration_EventHandler handler, void* parameter)
{
    self->eventHandler = handler;
    self->eventHandlerParameter = parameter;
}

void
TLSConfiguration_setMinTlsVersion(TLSConfiguration self, TLSConfigVersion version)
{
    self->minVersion = version;
}

void
TLSConfiguration_setMaxTlsVersion(TLSConfiguration self, TLSConfigVersion version)
{
    self->maxVersion = version;
}

void
TLSConfiguration_setChainValidation(TLSConfiguration self, bool value)
{
    self->chainValidation = value;
}

void
TLSConfiguration_setTimeValidation(TLSConfiguration self, bool value)
{
    self->timeValidation = value;
}

void
TLSConfiguration_setAllowOnlyKnownCertificates(TLSConfiguration self, bool value)
{
    self->allowOnlyKnownCertificates = value;
}

bool
TLSConfiguration_setOwnCertificate(TLSConfiguration self, uint8_t* certificate, int certLen)
{
    int ret = mbedtls_x509_crt_parse(&(self->ownCertificate), certificate, certLen);

    if (ret != 0)
        DEBUG_PRINT("TLS", "mbedtls_x509_crt_parse returned -0x%x\n", -ret);

    return (ret == 0);
}

bool
TLSConfiguration_setOwnCertificateFromFile(TLSConfiguration self, const char* filename)
{
    int ret =  mbedtls_x509_crt_parse_file(&(self->ownCertificate), filename);

    if (ret != 0)
        DEBUG_PRINT("TLS", "mbedtls_x509_crt_parse_file returned -0x%x\n", -ret);

    return (ret == 0);
}

bool
TLSConfiguration_setOwnKey(TLSConfiguration self, uint8_t* key, int keyLen, const char* keyPassword)
{
    int ret = mbedtls_pk_parse_key(&(self->ownKey), key, keyLen, (const uint8_t*) keyPassword, (keyPassword == NULL) ? 0 : strlen(keyPassword));

    if (ret != 0)
        DEBUG_PRINT("TLS", "mbedtls_pk_parse_key returned -0x%x\n", -ret);

    return (ret == 0);
}

bool
TLSConfiguration_setOwnKeyFromFile(TLSConfiguration self, const char* filename, const char* keyPassword)
{
    int ret =  mbedtls_pk_parse_keyfile(&(self->ownKey), filename, keyPassword);

    if (ret != 0)
        DEBUG_PRINT("TLS", "mbedtls_pk_parse_keyfile returned -0x%x\n", -ret);

    return (ret == 0);
}

bool
TLSConfiguration_addAllowedCertificate(TLSConfiguration self, uint8_t* certificate, int certLen)
{
    mbedtls_x509_crt* cert = (mbedtls_x509_crt*) GLOBAL_CALLOC(1, sizeof(mbedtls_x509_crt));

    int ret = mbedtls_x509_crt_parse(cert, certificate, certLen);

    if (ret == 0)
    {
        LinkedList_add(self->allowedCertificates, cert);
        return true;
    }
    else
    {
        GLOBAL_FREEMEM(cert);
        return false;
    }
}

bool
TLSConfiguration_addAllowedCertificateFromFile(TLSConfiguration self, const char* filename)
{
    mbedtls_x509_crt* cert = (mbedtls_x509_crt*) GLOBAL_CALLOC(1, sizeof(mbedtls_x509_crt));

    int ret = mbedtls_x509_crt_parse_file(cert, filename);

    if (ret == 0)
    {
        LinkedList_add(self->allowedCertificates, cert);
        return true;
    }
    else
    {
        GLOBAL_FREEMEM(cert);
        return false;
    }
}

bool
TLSConfiguration_addCACertificate(TLSConfiguration self, uint8_t* certificate, int certLen)
{
    int ret =  mbedtls_x509_crt_parse(&(self->cacerts), certificate, certLen);

    if (ret != 0)
    {
        DEBUG_PRINT("TLS", "mbedtls_x509_crt_parse returned -0x%x\n", -ret);
        return false;
    }

    return (ret == 0);
}

bool
TLSConfiguration_addCACertificateFromFile(TLSConfiguration self, const char* filename)
{
    int ret =  mbedtls_x509_crt_parse_file(&(self->cacerts), filename);

    if (ret != 0)
        DEBUG_PRINT("TLS", "mbedtls_x509_crt_parse returned -0x%x\n", -ret);

    return (ret == 0);
}

static void
udpatedCRL(TLSConfiguration self)
{
    self->crlUpdated = Hal_getTimeInMs();

    /* We need to clean-up resumption cache (if enabled) to make sure we renegotiate as CRL may have changed data */
    if (self->useSessionResumption == false)
        return;

    if (self->conf.endpoint == MBEDTLS_SSL_IS_SERVER)
    {
        mbedtls_ssl_cache_entry *cur = self->cache.chain;

        while (cur)
        {
            cur->timestamp = 0;
            cur = cur->next;
        }
    }
}

bool
TLSConfiguration_addCRL(TLSConfiguration self, uint8_t* crl, int crlLen)
{
    int ret = mbedtls_x509_crl_parse(&(self->crl), crl, crlLen);

    if (ret != 0) {
        DEBUG_PRINT("TLS", "mbedtls_x509_crl_parse returned -0x%x\n", -ret);
    }
    else {
        udpatedCRL(self);
    }

    return (ret == 0);
}

bool
TLSConfiguration_addCRLFromFile(TLSConfiguration self, const char* filename)
{
    int ret =  mbedtls_x509_crl_parse_file(&(self->crl), filename);

    if (ret != 0) {
        DEBUG_PRINT("TLS", "mbedtls_x509_crl_parse_file returned %d\n", ret);
    }
    else {
        udpatedCRL(self);
    }

    return (ret == 0);
}

void
TLSConfiguration_resetCRL(TLSConfiguration self)
{
    mbedtls_x509_crl_free(&(self->crl));
    mbedtls_x509_crl_init(&(self->crl));
    self->crlUpdated = Hal_getTimeInMs();
}

void
TLSConfiguration_setRenegotiationTime(TLSConfiguration self, int timeInMs)
{
    self->renegotiationTimeInMs = timeInMs;
}

void
TLSConfiguration_destroy(TLSConfiguration self)
{
    if (self)
    {
        if (self->useSessionResumption)
        {
            if (self->conf.endpoint == MBEDTLS_SSL_IS_CLIENT)
            {
                if (self->savedSession)
                {
                    mbedtls_ssl_session_free(self->savedSession);
                    GLOBAL_FREEMEM(self->savedSession);
                }
            }
            else
            {
                mbedtls_ssl_cache_free(&(self->cache));
            }
        }

        mbedtls_x509_crt_free(&(self->ownCertificate));
        mbedtls_x509_crt_free(&(self->cacerts));
        mbedtls_x509_crl_free(&(self->crl));
        mbedtls_pk_free(&(self->ownKey));
        mbedtls_ssl_config_free(&(self->conf));

        LinkedList certElem = LinkedList_getNext(self->allowedCertificates);

        while (certElem)
        {
            mbedtls_x509_crt* cert = (mbedtls_x509_crt*) LinkedList_getData(certElem);

            mbedtls_x509_crt_free(cert);

            certElem = LinkedList_getNext(certElem);
        }

        LinkedList_destroy(self->allowedCertificates);

        GLOBAL_FREEMEM(self->ciphersuites);

        GLOBAL_FREEMEM(self);
    }
}

static void
createSecurityEvents(TLSConfiguration config, int ret, uint32_t flags, TLSSocket socket)
{
    if (config->eventHandler == NULL)
        return;

    switch (ret) {
    case MBEDTLS_ERR_X509_UNKNOWN_SIG_ALG:
        raiseSecurityEvent(config, TLS_SEC_EVT_INCIDENT, TLS_EVENT_CODE_ALM_NO_CIPHER, "Alarm: Algorithm not supported", socket);
        break;

    case MBEDTLS_ERR_SSL_NO_CIPHER_CHOSEN:
        raiseSecurityEvent(config, TLS_SEC_EVT_INCIDENT, TLS_EVENT_CODE_ALM_NO_CIPHER, "Alarm: no matching TLS ciphers", socket);
        break;

    case MBEDTLS_ERR_SSL_NO_USABLE_CIPHERSUITE:
        raiseSecurityEvent(config, TLS_SEC_EVT_INCIDENT, TLS_EVENT_CODE_ALM_ALGO_NOT_SUPPORTED, "Alarm: Algorithm not supported", socket);
        break;

    case MBEDTLS_ERR_SSL_BAD_HS_PROTOCOL_VERSION:
        raiseSecurityEvent(config,TLS_SEC_EVT_INCIDENT, TLS_EVENT_CODE_ALM_UNSECURE_COMMUNICATION, "Alarm: Unsecure communication", socket);
        break;

    case MBEDTLS_ERR_SSL_NO_CLIENT_CERTIFICATE:
        raiseSecurityEvent(config,TLS_SEC_EVT_INCIDENT, TLS_EVENT_CODE_ALM_CERT_UNAVAILABLE, "Alarm: certificate unavailable", socket);
        break;

    case MBEDTLS_ERR_SSL_BAD_HS_CERTIFICATE:
        raiseSecurityEvent(config,TLS_SEC_EVT_INCIDENT, TLS_EVENT_CODE_ALM_BAD_CERT, "Alarm: Bad certificate", socket);
        break;

    case MBEDTLS_ERR_SSL_CERTIFICATE_TOO_LARGE:
        raiseSecurityEvent(config,TLS_SEC_EVT_INCIDENT, TLS_EVENT_CODE_ALM_CERT_SIZE_EXCEEDED, "Alarm: TLS certificate size exceeded", socket);
        break;

    case MBEDTLS_ERR_SSL_PEER_VERIFY_FAILED:
        raiseSecurityEvent(config,TLS_SEC_EVT_INCIDENT, TLS_EVENT_CODE_ALM_CERT_VALIDATION_FAILED, "Alarm: certificate validation: certificate signature could not be validated", socket);
        break;

    case MBEDTLS_ERR_SSL_CERTIFICATE_REQUIRED:
        raiseSecurityEvent(config,TLS_SEC_EVT_INCIDENT, TLS_EVENT_CODE_ALM_CERT_REQUIRED, "Alarm: Certificate required", socket);
        break;

    case MBEDTLS_ERR_X509_CERT_VERIFY_FAILED:
        {
            if (flags & MBEDTLS_X509_BADCERT_EXPIRED) {
                raiseSecurityEvent(config,TLS_SEC_EVT_INCIDENT, TLS_EVENT_CODE_ALM_CERT_EXPIRED, "Alarm: expired certificate", socket);
            }
            else if (flags & MBEDTLS_X509_BADCERT_REVOKED) {
                raiseSecurityEvent(config,TLS_SEC_EVT_INCIDENT, TLS_EVENT_CODE_ALM_CERT_REVOKED, "Alarm: revoked certificate", socket);
            }
            else if (flags & MBEDTLS_X509_BADCERT_NOT_TRUSTED) {
                raiseSecurityEvent(config,TLS_SEC_EVT_INCIDENT, TLS_EVENT_CODE_ALM_CERT_NOT_TRUSTED, "Alarm: Certificate validation: CA certificate not available", socket);
            }
            else if (flags & MBEDTLS_X509_BADCERT_OTHER) {
                raiseSecurityEvent(config,TLS_SEC_EVT_INCIDENT, TLS_EVENT_CODE_ALM_CERT_NOT_CONFIGURED, "Alarm: Certificate not configured", socket);
            }
            else if (flags & MBEDTLS_X509_BADCERT_BAD_KEY) {
                raiseSecurityEvent(config,TLS_SEC_EVT_INCIDENT, TLS_EVENT_CODE_ALM_CERT_NOT_CONFIGURED, "Alarm: Insufficient key length", socket);
            }

            raiseSecurityEvent(config,TLS_SEC_EVT_INCIDENT, TLS_EVENT_CODE_ALM_CERT_VALIDATION_FAILED, "Alarm: Certificate verification failed", socket);
        }
        break;

    default:
        raiseSecurityEvent(config,TLS_SEC_EVT_INCIDENT, TLS_EVENT_CODE_ALM_HANDSHAKE_FAILED_UNKNOWN_REASON, "Alarm: handshake failed for unknown reason", socket);
        break;
    }
}

static int
readFunction(void* ctx, unsigned char* buf, size_t len)
{
    int ret = Socket_read((Socket) ctx, buf, (int)len);

    if ((ret == 0) && (len > 0))
    {
        return MBEDTLS_ERR_SSL_WANT_READ;
    }

    return ret;
}

static int
writeFunction(void* ctx, unsigned char* buf, size_t len)
{
    int ret = Socket_write((Socket)ctx, buf, (int)len);

    if ((ret == 0) && (len > 0))
    {
        return MBEDTLS_ERR_SSL_WANT_WRITE;
    }

    return ret;
}

static TLSConfigVersion
getTLSVersion(int majorVersion, int minorVersion)
{
    if (majorVersion != 3) {
        return TLS_VERSION_NOT_SELECTED;
    }
    else
    {
        switch (minorVersion) {
        case 0:
            return TLS_VERSION_SSL_3_0;
        case 1:
            return TLS_VERSION_TLS_1_0;
        case 2:
            return TLS_VERSION_TLS_1_1;
        case 3:
            return TLS_VERSION_TLS_1_2;
        case 4:
            return TLS_VERSION_TLS_1_3;
        default:
            return TLS_VERSION_NOT_SELECTED;
        }
    }
}

static int
getMajorVersion(TLSConfigVersion version)
{
    switch(version) {
    case TLS_VERSION_NOT_SELECTED:
        return 0;
    case TLS_VERSION_SSL_3_0:
    case TLS_VERSION_TLS_1_0:
    case TLS_VERSION_TLS_1_1:
    case TLS_VERSION_TLS_1_2:
    case TLS_VERSION_TLS_1_3:
        return 3;
    default:
        return 0;
    }
}

static int
getMinorVersion(TLSConfigVersion version)
{
    switch(version) {
    case TLS_VERSION_NOT_SELECTED:
        return 0;
    case TLS_VERSION_SSL_3_0:
        return 0;
    case TLS_VERSION_TLS_1_0:
        return 1;
    case TLS_VERSION_TLS_1_1:
        return 2;
    case TLS_VERSION_TLS_1_2:
        return 3;
    case TLS_VERSION_TLS_1_3:
        return 4;
    default:
        return 0;
    }
}

TLSSocket
TLSSocket_create(Socket socket, TLSConfiguration configuration, bool storeClientCert)
{
    TLSSocket self = (TLSSocket) GLOBAL_CALLOC(1, sizeof(struct sTLSSocket));

    if (self)
    {
        self->socket = socket;
        self->tlsConfig = configuration;
        self->storePeerCert = storeClientCert;
        self->peerCert = NULL;
        self->peerCertLength = 0;

        TLSConfiguration_setupComplete(configuration);

        memcpy(&(self->conf), &(configuration->conf), sizeof(mbedtls_ssl_config));

        mbedtls_ssl_conf_verify(&(self->conf), verifyCertificate, (void*) self);

        int ret;

        mbedtls_ssl_conf_ca_chain( &(self->conf), &(configuration->cacerts), &(configuration->crl) );

        self->crlUpdated = configuration->crlUpdated;

        if (configuration->minVersion != TLS_VERSION_NOT_SELECTED)
        {
            /* set minimum TLS version */

            int majorVer = getMajorVersion(configuration->minVersion);
            int minorVer = getMinorVersion(configuration->minVersion);

            mbedtls_ssl_conf_min_version( &(self->conf), majorVer, minorVer);
        }

        if (configuration->maxVersion != TLS_VERSION_NOT_SELECTED)
        {
            /* set maximum TLS version */

            int majorVer = getMajorVersion(configuration->maxVersion);
            int minorVer = getMinorVersion(configuration->maxVersion);

            mbedtls_ssl_conf_max_version( &(self->conf), majorVer, minorVer);
        }

        if (configuration->ownCertificate.version > 0)
        {
            ret = mbedtls_ssl_conf_own_cert( &(self->conf), &(configuration->ownCertificate), &(configuration->ownKey));

            if (ret != 0)
                DEBUG_PRINT("TLS", "mbedtls_ssl_conf_own_cert returned %d\n", ret);
        }

        ret = mbedtls_ssl_setup( &(self->ssl), &(self->conf) );

        if (ret != 0)
            DEBUG_PRINT("TLS", "mbedtls_ssl_setup returned %d\n", ret);

        mbedtls_ssl_set_bio(&(self->ssl), socket, (mbedtls_ssl_send_t*) writeFunction,
                (mbedtls_ssl_recv_t*) readFunction, NULL);

        if (configuration->useSessionResumption)
        {
            if (configuration->conf.endpoint == MBEDTLS_SSL_IS_CLIENT)
            {
                if (configuration->savedSession && configuration->savedSessionTime > 0)
                {
                    if (Hal_getTimeInMs() < (configuration->savedSessionTime + configuration->sessionResumptionInterval * 1000))
                    {
                        ret = mbedtls_ssl_set_session(&(self->ssl), configuration->savedSession);

                        if (ret != 0)
                        {
                            DEBUG_PRINT("TLS", "mbedtls_ssl_set_session returned %d\n", ret);
                            configuration->savedSessionTime = 0;
                        }
                        else
                        {
                            DEBUG_PRINT("TLS", "resume TLS session\n");
                        }
                    }
                    else
                    {
                        configuration->savedSessionTime = 0;
                        DEBUG_PRINT("TLS", "cached session expired\n");
                    }
                }
            }
        }

        while( (ret = mbedtls_ssl_handshake(&(self->ssl)) ) != 0 )
        {
            if( ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE )
            {
                DEBUG_PRINT("TLS", "handshake failed - mbedtls_ssl_handshake returned -0x%x\n", -ret );

                uint32_t flags = mbedtls_ssl_get_verify_result(&(self->ssl));

                createSecurityEvents(configuration, ret, flags, self);

                mbedtls_ssl_free(&(self->ssl));

                if (self->peerCert) {
                    GLOBAL_FREEMEM(self->peerCert);
                }

                GLOBAL_FREEMEM(self);

                return NULL;
            }
        }

        if (configuration->useSessionResumption)
        {
            if (configuration->conf.endpoint == MBEDTLS_SSL_IS_CLIENT)
            {
                if (configuration->savedSession == NULL)
                {
                    configuration->savedSession = (mbedtls_ssl_session*)GLOBAL_CALLOC(1, sizeof(mbedtls_ssl_session));
                }

                if (configuration->savedSession)
                {
                    if (configuration->savedSessionTime == 0)
                    {
                        ret = mbedtls_ssl_get_session(&(self->ssl), configuration->savedSession);

                        if (ret != 0)
                        {
                            DEBUG_PRINT("TLS", "mbedtls_ssl_get_session returned %d\n", ret);
                        }
                        else
                        {
                            configuration->savedSessionTime = Hal_getTimeInMs();
                        }
                    }
                }
            }
        }

        self->lastRenegotiationTime = Hal_getTimeInMs();

        if (getTLSVersion(self->ssl.major_ver, self->ssl.minor_ver) < TLS_VERSION_TLS_1_2) {
            raiseSecurityEvent(configuration, TLS_SEC_EVT_WARNING, TLS_EVENT_CODE_WRN_INSECURE_TLS_VERSION,  "Warning: Insecure TLS version", self);
        }

        /* create event that TLS session is established */
        {
            char msg[256];

            const char* cipherSuite = mbedtls_ssl_get_ciphersuite(&(self->ssl));

            snprintf(msg, 255, "Info: Session established with cipher suite %s", cipherSuite);

            raiseSecurityEvent(configuration, TLS_SEC_EVT_INFO, TLS_EVENT_CODE_INF_SESSION_ESTABLISHED, msg, self);
        }
    }

    return self;
}

uint8_t*
TLSSocket_getPeerCertificate(TLSSocket self, int* certSize)
{
    if (certSize)
        *certSize = self->peerCertLength;

    return self->peerCert;
}

bool
TLSSocket_performHandshake(TLSSocket self)
{
    int ret = mbedtls_ssl_renegotiate(&(self->ssl));

    if (ret == 0 || ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE ||
        ret == MBEDTLS_ERR_SSL_ASYNC_IN_PROGRESS || ret == MBEDTLS_ERR_SSL_CRYPTO_IN_PROGRESS)
    {
        if (getTLSVersion(self->ssl.major_ver, self->ssl.minor_ver) < TLS_VERSION_TLS_1_2) {
            raiseSecurityEvent(self->tlsConfig, TLS_SEC_EVT_WARNING, TLS_EVENT_CODE_WRN_INSECURE_TLS_VERSION, "Warning: Insecure TLS version", self);
        }

        DEBUG_PRINT("TLS", "TLSSocket_performHandshake Success -> ret=%i\n", ret);
        raiseSecurityEvent(self->tlsConfig, TLS_SEC_EVT_INFO, TLS_EVENT_CODE_INF_SESSION_RENEGOTIATION, "TLS session renegotiation completed", self);
        return true;
    }
    else
    {
        DEBUG_PRINT("TLS", "TLSSocket_performHandshake failed -> ret=%i\n", ret);

        raiseSecurityEvent(self->tlsConfig, TLS_SEC_EVT_WARNING, TLS_EVENT_CODE_INF_SESSION_RENEGOTIATION, "Alarm: TLS session renegotiation failed", self);

        /* mbedtls_ssl_renegotiate mandates to reset the ssl session in case of errors */
        ret = mbedtls_ssl_session_reset(&(self->ssl));
        if (ret != 0) {
            DEBUG_PRINT("TLS", "mbedtls_ssl_session_reset failed -> ret: -0x%X\n", -ret);
        }

        return false;
    }
}

static void
checkForCRLUpdate(TLSSocket self)
{
    if (self->crlUpdated == self->tlsConfig->crlUpdated)
        return;

    DEBUG_PRINT("TLS", "CRL updated -> refresh CA chain\n");

    mbedtls_ssl_conf_ca_chain( &(self->conf), &( self->tlsConfig->cacerts), &( self->tlsConfig->crl) );

    self->crlUpdated = self->tlsConfig->crlUpdated;

    /* IEC TS 62351-100-3 Conformance test 6.2.6 requires that upon CRL update a TLS renegotiation should occur */
    self->lastRenegotiationTime = 0;
}

/* true = renegotiation is not needed or it is successfull, false = Failed */
static bool
startRenegotiationIfRequired(TLSSocket self)
{
    if (self->tlsConfig->renegotiationTimeInMs <= 0)
        return true;

    if (Hal_getTimeInMs() <= self->lastRenegotiationTime + self->tlsConfig->renegotiationTimeInMs)
        return true;

    raiseSecurityEvent(self->tlsConfig, TLS_SEC_EVT_INFO, TLS_EVENT_CODE_INF_SESSION_RENEGOTIATION, "Info: session renegotiation started", self);

    if (TLSSocket_performHandshake(self) == false)
    {
        DEBUG_PRINT("TLS", " renegotiation failed\n");
        return false;
    }

    DEBUG_PRINT("TLS", " started renegotiation\n");
    self->lastRenegotiationTime = Hal_getTimeInMs();

    return true;
}

int
TLSSocket_read(TLSSocket self, uint8_t* buf, int size)
{
    checkForCRLUpdate(self);

    if (startRenegotiationIfRequired(self) == false) {
        return -1;
    }

    int len = 0;
    while (len < size) {
        int ret = mbedtls_ssl_read(&(self->ssl), (buf + len), (size - len));
        if (ret > 0) {
            len += ret;
            continue;
        }

        switch (ret) {
        case 0: // falling through
        case MBEDTLS_ERR_SSL_WANT_READ:
        case MBEDTLS_ERR_SSL_WANT_WRITE:
        case MBEDTLS_ERR_SSL_ASYNC_IN_PROGRESS:
        case MBEDTLS_ERR_SSL_CRYPTO_IN_PROGRESS:
            // Known "good" cases indicating the read is done
            return len;

        case MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY:
            DEBUG_PRINT("TLS", " connection was closed gracefully\n");
            break;

        case MBEDTLS_ERR_NET_CONN_RESET:
            DEBUG_PRINT("TLS", " connection was reset by peer\n");
            break;

        default:
            DEBUG_PRINT("TLS", " mbedtls_ssl_read returned -0x%x\n", -ret);

            {
                uint32_t flags = mbedtls_ssl_get_verify_result(&(self->ssl));

                createSecurityEvents(self->tlsConfig, ret, flags, self);
            }
        }

        int reset_err = mbedtls_ssl_session_reset(&(self->ssl));
        if (0 != reset_err) {
            DEBUG_PRINT("TLS", "mbedtls_ssl_session_reset failed -0x%X\n", -reset_err);
        }

        return ret;
    }

    return len;
}

int
TLSSocket_write(TLSSocket self, uint8_t* buf, int size)
{
    int len = 0;

    checkForCRLUpdate(self);

    if (startRenegotiationIfRequired(self) == false) {
        return -1;
    }

    while (len < size)
    {
        int ret = mbedtls_ssl_write(&(self->ssl), (buf + len), (size -len));
        if ((ret == MBEDTLS_ERR_SSL_WANT_READ) || (ret == MBEDTLS_ERR_SSL_WANT_WRITE) ||
            (ret == MBEDTLS_ERR_SSL_ASYNC_IN_PROGRESS) || (ret == MBEDTLS_ERR_SSL_CRYPTO_IN_PROGRESS)) {
            continue;
        }

        if (ret < 0) {
            DEBUG_PRINT("TLS", "mbedtls_ssl_write returned -0x%X\n", -ret);

            if (0 != (ret = mbedtls_ssl_session_reset(&(self->ssl)))) {
                DEBUG_PRINT("TLS", "mbedtls_ssl_session_reset failed -0x%X\n", -ret);
            }

            return -1;
        }

        len += ret;
    }

    return len;
}

void
TLSSocket_close(TLSSocket self)
{
    int ret;

    /* TODO add timeout? */

    while ((ret = mbedtls_ssl_close_notify(&(self->ssl))) < 0)
    {
        if ((ret != MBEDTLS_ERR_SSL_WANT_READ) && (ret != MBEDTLS_ERR_SSL_WANT_WRITE))
        {
            DEBUG_PRINT("TLS", "mbedtls_ssl_close_notify returned -0x%x\n", -ret);
            break;
        }
    }

    Thread_sleep(10);

    mbedtls_ssl_free(&(self->ssl));

    if (self->peerCert)
        GLOBAL_FREEMEM(self->peerCert);

    GLOBAL_FREEMEM(self);
}

char*
TLSConnection_getPeerAddress(TLSConnection self, char* peerAddrBuf)
{
    TLSSocket socket = (TLSSocket)self;

    if (peerAddrBuf == NULL) {
        peerAddrBuf = (char*)GLOBAL_MALLOC(61);
    }

    if (peerAddrBuf)
        return Socket_getPeerAddressStatic(socket->socket, peerAddrBuf);
    else
        return NULL; 
}

uint8_t*
TLSConnection_getPeerCertificate(TLSConnection self, int* certSize)
{
    TLSSocket socket = (TLSSocket)self;

    return TLSSocket_getPeerCertificate(socket, certSize);
}

TLSConfigVersion
TLSConnection_getTLSVersion(TLSConnection self)
{
    TLSSocket socket = (TLSSocket)self;

    return getTLSVersion(socket->ssl.major_ver, socket->ssl.minor_ver);
}

const char*
TLSConfigVersion_toString(TLSConfigVersion version)
{
    switch (version)
    {
        case TLS_VERSION_SSL_3_0:
            return "SSL 3.0";
        case TLS_VERSION_TLS_1_0:
            return "TLS 1.0";
        case TLS_VERSION_TLS_1_1:
            return "TLS 1.1";
        case TLS_VERSION_TLS_1_2:
            return "TLS 1.2";
        case TLS_VERSION_TLS_1_3:
            return "TLS 1.3";
        default:
            return "unknown TLS version";
    }
}
