/*
 * tls_mbedtls.c
 *
 * TLS API for TCP/IP protocol stacks
 *
 * Copyright 2017 MZ Automation GmbH
 *
 * Implementation of the TLS abstraction layer for mbedtls
 *
 */

#include <string.h>

#include "tls_socket.h"
#include "hal_thread.h"
#include "lib_memory.h"
#include "linked_list.h"

#include "mbedtls/platform.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/certs.h"
#include "mbedtls/x509.h"
#include "mbedtls/ssl.h"
#include "mbedtls/net_sockets.h"
#include "mbedtls/error.h"
#include "mbedtls/debug.h"

#if (CONFIG_DEBUG_TLS == 1)
#define DEBUG_PRINT(appId, fmt, ...) fprintf(stderr, "%s: " fmt, \
    appId, \
    __VA_ARGS__)
#else
#define DEBUG_PRINT(fmt, ...) {do {} while(0);}
#endif


struct sTLSConfiguration {
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;

    mbedtls_x509_crt ownCertificate;
    mbedtls_pk_context ownKey;

    mbedtls_x509_crt cacerts;

    mbedtls_ssl_config conf;
    LinkedList /* <mbedtls_x509_crt*> */ allowedCertificates;

    bool chainValidation;
    bool allowOnlyKnownCertificates;

    /* TLS session renegotioation time in milliseconds */
    int renegotiationTimeInMs;
};

struct sTLSSocket {
    mbedtls_ssl_context ssl;
    Socket socket;
    mbedtls_ssl_config conf;
    TLSConfiguration tlsConfig;
    bool storePeerCert;
    uint8_t* peerCert;
    int peerCertLength;
};

static bool
compareCertificates(mbedtls_x509_crt *crt1, mbedtls_x509_crt *crt2)
{
    if (crt1 != NULL && crt2 != NULL) {

        if (crt1->sig.len == crt2->sig.len) {
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

    if (self->tlsConfig->chainValidation == false) {
        if (certificate_depth != 0)
            *flags = 0;
    }

    if (certificate_depth == 0) {
        if (self->tlsConfig->allowOnlyKnownCertificates) {

            DEBUG_PRINT("TLS", "Check against list of allowed certs\n");

            bool certMatches = false;

            LinkedList certList = LinkedList_getNext(self->tlsConfig->allowedCertificates);

            while (certList) {
                mbedtls_x509_crt* allowedCert = (mbedtls_x509_crt*) LinkedList_getData(certList);

                DEBUG_PRINT("TLS", "Compare With:\n");
                mbedtls_x509_crt_info(buffer, 1023, "     ", allowedCert);
                DEBUG_PRINT("TLS", "%s\n", buffer);

                if (compareCertificates(allowedCert, crt)) {
                    certMatches = true;
                    break;
                }

                certList = LinkedList_getNext(certList);
            }

            if (certMatches)
                *flags = 0;
            else
                return 1;
        }

        if (self->storePeerCert) {

            if (*flags == 0) {

                self->peerCertLength = crt->raw.len;
                self->peerCert = (uint8_t*) GLOBAL_MALLOC(self->peerCertLength);
                memcpy(self->peerCert, crt->raw.p, self->peerCertLength);

            }

        }
    }

    return 0;
}



TLSConfiguration
TLSConfiguration_create()
{
    TLSConfiguration self = (TLSConfiguration) GLOBAL_MALLOC(sizeof(struct sTLSConfiguration));

    if (self != NULL) {
        mbedtls_ssl_config_init( &(self->conf) );
        mbedtls_x509_crt_init( &(self->ownCertificate) );
        mbedtls_x509_crt_init( &(self->cacerts));
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

        mbedtls_ssl_conf_renegotiation(&(self->conf), MBEDTLS_SSL_RENEGOTIATION_DISABLED);

        /* static int hashes[] = {3,4,5,6,7,8,0}; */
        /* mbedtls_ssl_conf_sig_hashes(&(self->conf), hashes); */

        self->allowedCertificates = LinkedList_create();

        /* default behavior is to allow all certificates that are signed by the CA */
        self->chainValidation = true;
        self->allowOnlyKnownCertificates = false;
    }

    return self;
}

void
TLSConfiguration_setClientMode(TLSConfiguration self)
{
    self->conf.endpoint = MBEDTLS_SSL_IS_CLIENT;
}

void
TLSConfiguration_setChainValidation(TLSConfiguration self, bool value)
{
    self->chainValidation = value;
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
        DEBUG_PRINT("mbedtls_x509_crt_parse returned %d\n", ret);

    return (ret == 0);
}

bool
TLSConfiguration_setOwnCertificateFromFile(TLSConfiguration self, const char* filename)
{
    int ret =  mbedtls_x509_crt_parse_file(&(self->ownCertificate), filename);

    if (ret != 0)
        DEBUG_PRINT("TLS", "mbedtls_x509_crt_parse_file returned %d\n", ret);

    return (ret == 0);
}

bool
TLSConfiguration_setOwnKey(TLSConfiguration self, uint8_t* key, int keyLen, const char* keyPassword)
{
    int ret = mbedtls_pk_parse_key(&(self->ownKey), key, keyLen, (const uint8_t*) keyPassword, (keyPassword == NULL) ? 0 : strlen(keyPassword));

    if (ret != 0)
        DEBUG_PRINT("TLS", "mbedtls_pk_parse_key returned %d\n", ret);

    return (ret == 0);
}

bool
TLSConfiguration_setOwnKeyFromFile(TLSConfiguration self, const char* filename, const char* keyPassword)
{
    int ret =  mbedtls_pk_parse_keyfile(&(self->ownKey), filename, keyPassword);

    if (ret != 0)
        DEBUG_PRINT("TLS", "mbedtls_pk_parse_keyfile returned %d\n", ret);

    return (ret == 0);
}

bool
TLSConfiguration_addAllowedCertificate(TLSConfiguration self, uint8_t* certificate, int certLen)
{
    mbedtls_x509_crt* cert = (mbedtls_x509_crt*) GLOBAL_CALLOC(1, sizeof(mbedtls_x509_crt));

    int ret = mbedtls_x509_crt_parse(cert, certificate, certLen);

    if (ret == 0) {
        LinkedList_add(self->allowedCertificates, cert);
        return true;
    }
    else {
        GLOBAL_FREEMEM(cert);
        return false;
    }
}

bool
TLSConfiguration_addAllowedCertificateFromFile(TLSConfiguration self, const char* filename)
{
    mbedtls_x509_crt* cert = (mbedtls_x509_crt*) GLOBAL_CALLOC(1, sizeof(mbedtls_x509_crt));

    int ret = mbedtls_x509_crt_parse_file(cert, filename);

    if (ret == 0) {
        LinkedList_add(self->allowedCertificates, cert);
        return true;
    }
    else {
        GLOBAL_FREEMEM(cert);
        return false;
    }
}

bool
TLSConfiguration_addCACertificate(TLSConfiguration self, uint8_t* certificate, int certLen)
{
    int ret =  mbedtls_x509_crt_parse(&(self->cacerts), certificate, certLen);

    if (ret != 0) {
        DEBUG_PRINT("TLS", "mbedtls_x509_crt_parse returned %d\n", ret);
        return false;
    }

    return (ret == 0);
}

bool
TLSConfiguration_addCACertificateFromFile(TLSConfiguration self, const char* filename)
{
    int ret =  mbedtls_x509_crt_parse_file(&(self->cacerts), filename);

    if (ret != 0)
        DEBUG_PRINT("TLS", "mbedtls_x509_crt_parse returned %d\n", ret);

    return (ret == 0);
}

void
TLSConfiguration_setRenegotiationTime(TLSConfiguration self, int timeInMs)
{
    self->renegotiationTimeInMs = timeInMs;
}

void
TLSConfiguration_destroy(TLSConfiguration self)
{
    mbedtls_x509_crt_free(&(self->ownCertificate));
    mbedtls_x509_crt_free(&(self->cacerts));
    mbedtls_pk_free(&(self->ownKey));
    mbedtls_ssl_config_free(&(self->conf));

    LinkedList certElem = LinkedList_getNext(self->allowedCertificates);

    while (certElem) {
        mbedtls_x509_crt* cert = (mbedtls_x509_crt*) LinkedList_getData(certElem);

        mbedtls_x509_crt_free(cert);

        certElem = LinkedList_getNext(certElem);
    }

    LinkedList_destroy(self->allowedCertificates);

    GLOBAL_FREEMEM(self);
}


static int
readFunction(void* ctx, unsigned char* buf, size_t len)
{
    int ret = Socket_read((Socket) ctx, buf, len);

    if ((ret == 0) && (len > 0)) {
        return MBEDTLS_ERR_SSL_WANT_READ;
    }

    return ret;
}

TLSSocket
TLSSocket_create(Socket socket, TLSConfiguration configuration, bool storeClientCert)
{
    TLSSocket self = (TLSSocket) GLOBAL_CALLOC(1, sizeof(struct sTLSSocket));

    if (self != NULL) {

        self->socket = socket;
        self->tlsConfig = configuration;
        self->storePeerCert = storeClientCert;
        self->peerCert = NULL;
        self->peerCertLength = 0;

        memcpy(&(self->conf), &(configuration->conf), sizeof(mbedtls_ssl_config));

        mbedtls_ssl_conf_verify(&(self->conf), verifyCertificate, (void*) self);

        int ret;

        mbedtls_ssl_conf_ca_chain( &(self->conf), &(configuration->cacerts), NULL );

        ret = mbedtls_ssl_conf_own_cert( &(self->conf), &(configuration->ownCertificate), &(configuration->ownKey));

        if (ret != 0)
            DEBUG_PRINT("TLS", "mbedtls_ssl_conf_own_cert returned %d\n", ret);

        ret = mbedtls_ssl_setup( &(self->ssl), &(self->conf) );

        if (ret != 0)
            DEBUG_PRINT("TLS", "mbedtls_ssl_setup returned %d\n", ret);

        mbedtls_ssl_set_bio(&(self->ssl), socket, (mbedtls_ssl_send_t*) Socket_write,
                (mbedtls_ssl_recv_t*) readFunction, NULL);

        while( (ret = mbedtls_ssl_handshake(&(self->ssl)) ) != 0 )
        {
            if( ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE )
            {
                DEBUG_PRINT("TLS", "mbedtls_ssl_handshake returned %d\n\n", ret );

                mbedtls_ssl_config_free(&(self->conf));
                mbedtls_ssl_free(&(self->ssl));

                GLOBAL_FREEMEM(self);

                return NULL;
            }
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
    if (mbedtls_ssl_renegotiate(&(self->ssl)) == 0)
        return true;
    else
        return false;
}

int
TLSSocket_read(TLSSocket self, uint8_t* buf, int size)
{
    int ret = mbedtls_ssl_read(&(self->ssl), buf, size);

    if ((ret == MBEDTLS_ERR_SSL_WANT_READ) || (ret == MBEDTLS_ERR_SSL_WANT_WRITE))
        return 0;

    if (ret < 0) {

        switch (ret)
        {
        case MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY:
            DEBUG_PRINT("TLS", " connection was closed gracefully\n");
            return -1;

        case MBEDTLS_ERR_NET_CONN_RESET:
            DEBUG_PRINT("TLS", " connection was reset by peer\n");
            return -1;

        default:
            DEBUG_PRINT("TLS", " mbedtls_ssl_read returned -0x%x\n", -ret);
            return -1;
        }
    }

    return ret;
}

int
TLSSocket_write(TLSSocket self, uint8_t* buf, int size)
{
    int ret;
    int len = size;

    while ((ret = mbedtls_ssl_write(&(self->ssl), buf, len)) <= 0)
    {
        if (ret == MBEDTLS_ERR_NET_CONN_RESET)
        {
            DEBUG_PRINT("TLS", "peer closed the connection\n");
            return -1;
        }

        if ((ret != MBEDTLS_ERR_SSL_WANT_READ) && (ret != MBEDTLS_ERR_SSL_WANT_WRITE))
        {
            DEBUG_PRINT("TLS", "mbedtls_ssl_write returned %d\n", ret);
            return -1;
        }
    }

    len = ret;

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
            DEBUG_PRINT("TLS", "mbedtls_ssl_close_notify returned %d\n", ret);
            break;
        }
    }

    Thread_sleep(10);

    mbedtls_ssl_config_free(&(self->conf));
    mbedtls_ssl_free(&(self->ssl));

    if (self->peerCert)
        GLOBAL_FREEMEM(self->peerCert);

    GLOBAL_FREEMEM(self);
}
