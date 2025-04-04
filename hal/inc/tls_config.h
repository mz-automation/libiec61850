/*
 * tls_config.h
 *
 * TLS Configuration API for protocol stacks using TCP/IP
 *
 * Copyright 2017-2024 Michael Zillgith
 *
 * Abstraction layer for configuration of different TLS implementations
 *
 */

#ifndef SRC_TLS_CONFIG_H_
#define SRC_TLS_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hal_base.h"
#include "tls_ciphers.h"

/**
 * \file tls_config.h
 * \brief TLS API functions
 */

/*! \addtogroup hal Platform (Hardware/OS) abstraction layer
   *
   *  @{
   */

/**
 * @defgroup TLS_CONFIG_API TLS configuration
 *
 * @{
 */

typedef struct sTLSConfiguration* TLSConfiguration;

/**
 * \brief Create a new \ref TLSConfiguration object to represent TLS configuration and certificates
 *
 * WARNING: Configuration cannot be changed after using for the first time.
 *
 * \return the new TLS configuration
 */
PAL_API TLSConfiguration
TLSConfiguration_create(void);

/* will be called by stack automatically when appropriate */
PAL_API void
TLSConfiguration_setClientMode(TLSConfiguration self);

typedef enum {
   TLS_VERSION_NOT_SELECTED = 0,
   TLS_VERSION_SSL_3_0 = 3,
   TLS_VERSION_TLS_1_0 = 4,
   TLS_VERSION_TLS_1_1 = 5,
   TLS_VERSION_TLS_1_2 = 6,
   TLS_VERSION_TLS_1_3 = 7
} TLSConfigVersion;

/**
 * \brief Convert TLS version number to string
 * 
 * \param version TLS version number
 * 
 * \return the TLS version as null terminated string 
 */
PAL_API const char*
TLSConfigVersion_toString(TLSConfigVersion version);

typedef enum {
    TLS_SEC_EVT_INFO = 0,
    TLS_SEC_EVT_WARNING = 1,
    TLS_SEC_EVT_INCIDENT = 2
} TLSEventLevel;

#define TLS_EVENT_CODE_ALM_ALGO_NOT_SUPPORTED 1
#define TLS_EVENT_CODE_ALM_UNSECURE_COMMUNICATION 2
#define TLS_EVENT_CODE_ALM_CERT_UNAVAILABLE 3
#define TLS_EVENT_CODE_ALM_BAD_CERT 4
#define TLS_EVENT_CODE_ALM_CERT_SIZE_EXCEEDED 5
#define TLS_EVENT_CODE_ALM_CERT_VALIDATION_FAILED 6
#define TLS_EVENT_CODE_ALM_CERT_REQUIRED 7
#define TLS_EVENT_CODE_ALM_HANDSHAKE_FAILED_UNKNOWN_REASON 8
#define TLS_EVENT_CODE_WRN_INSECURE_TLS_VERSION 9
#define TLS_EVENT_CODE_INF_SESSION_RENEGOTIATION 10
#define TLS_EVENT_CODE_ALM_CERT_EXPIRED 11
#define TLS_EVENT_CODE_ALM_CERT_REVOKED 12
#define TLS_EVENT_CODE_ALM_CERT_NOT_CONFIGURED 13
#define TLS_EVENT_CODE_ALM_CERT_NOT_TRUSTED 14
#define TLS_EVENT_CODE_ALM_NO_CIPHER 15
#define TLS_EVENT_CODE_INF_SESSION_ESTABLISHED 16
#define TLS_EVENT_CODE_WRN_CERT_EXPIRED 17
#define TLS_EVENT_CODE_WRN_CERT_NOT_YET_VALID 18
#define TLS_EVENT_CODE_WRN_CRL_EXPIRED 19
#define TLS_EVENT_CODE_WRN_CRL_NOT_YET_VALID 20

typedef struct sTLSConnection* TLSConnection;

/**
 * \brief Get the peer address of the TLS connection
 * 
 * \param self the TLS connection instance
 * \param peerAddrBuf user provided buffer that can hold at least 60 characters, or NULL to allow the function to allocate the memory for the buffer
 * 
 * \returns peer address:port as null terminated string 
 */
PAL_API char*
TLSConnection_getPeerAddress(TLSConnection self, char* peerAddrBuf);

/**
 * \brief Get the TLS certificate used by the peer
 * 
 * \param self the TLS connection instance
 * \param certSize[OUT] the certificate size in bytes
 * 
 * \return address of the certificate buffer 
 */
PAL_API uint8_t*
TLSConnection_getPeerCertificate(TLSConnection self, int* certSize);

/**
 * \brief Get the TLS version used by the connection
 * 
 * \param self the TLS connection instance
 * 
 * \return TLS version
 */
PAL_API TLSConfigVersion
TLSConnection_getTLSVersion(TLSConnection self);

typedef void (*TLSConfiguration_EventHandler)(void* parameter, TLSEventLevel eventLevel, int eventCode, const char* message, TLSConnection con);

/**
 * \brief Set the security event handler
 * 
 * \param handler the security event callback handler
 * \param parameter user provided parameter to be passed to the callback handler
 */
PAL_API void
TLSConfiguration_setEventHandler(TLSConfiguration self, TLSConfiguration_EventHandler handler, void* parameter);

/**
 * \brief enable or disable TLS session resumption (default: enabled)
 * 
 * NOTE: Depending on the used TLS version this is implemented by
 * session IDs or by session tickets.
 * 
 * \param enable true to enable session resumption, false otherwise
 */
PAL_API void
TLSConfiguration_enableSessionResumption(TLSConfiguration self, bool enable);

/**
 * \brief Set the maximum life time of a cached TLS session for session resumption in seconds
 *
 * \param intervalInSeconds the maximum lifetime of a cached TLS session
 */
PAL_API void
TLSConfiguration_setSessionResumptionInterval(TLSConfiguration self, int intervalInSeconds);

/**
 * \brief Enables the validation of the certificate trust chain (enabled by default)
 *
 * \param value true to enable chain validation, false to disable
 */
PAL_API void
TLSConfiguration_setChainValidation(TLSConfiguration self, bool value);

/**
 * \brief Enabled or disables the verification of validity times for certificates and CRLs
 *
 * \param value true to enable time validation, false to disable (enabled by default)
 */
PAL_API void
TLSConfiguration_setTimeValidation(TLSConfiguration self, bool value);

/**
 * \brief Set if only known certificates are accepted.
 *
 * If set to true only known certificates are accepted. Connections with unknown certificates
 * are rejected even if they are signed by a trusted authority.
 *
 * \param value true to enable setting, false otherwise
 */
PAL_API void
TLSConfiguration_setAllowOnlyKnownCertificates(TLSConfiguration self, bool value);

/**
 * \brief Set own certificate (identity) from a byte buffer
 *
 * \param certificate the certificate buffer
 * \param certLen the lenght of the certificate
 *
 * \return true, when the certificate was set, false otherwise (e.g. unknown certificate format)
 */
PAL_API bool
TLSConfiguration_setOwnCertificate(TLSConfiguration self, uint8_t* certificate, int certLen);

/**
 * \brief Set own certificate (identity) from a certificate file
 *
 * \param filename of the certificate file
 *
 * \return true, when the certificate was set, false otherwise (e.g. unknown certificate format)
 */
PAL_API bool
TLSConfiguration_setOwnCertificateFromFile(TLSConfiguration self, const char* filename);

/**
 * \brief Set the own private key from a byte buffer
 *
 * \param key the private key to use
 * \param keyLen the length of the key
 * \param password the password of the key or null if the key is not password protected
 *
 * \return true, when the key was set, false otherwise (e.g. unknown key format)
 */
PAL_API bool
TLSConfiguration_setOwnKey(TLSConfiguration self, uint8_t* key, int keyLen, const char* keyPassword);

/**
 * \brief Set the own private key from a key file
 *
 * \param filename filename/path of the key file
 * \param password the password of the key or null if the key is not password protected
 *
 * \return true, when the key was set, false otherwise (e.g. unknown key format)
 */
PAL_API bool
TLSConfiguration_setOwnKeyFromFile(TLSConfiguration self, const char* filename, const char* keyPassword);

/**
 * Add a certificate to the list of allowed peer certificates from a byte buffer
 *
 * \param certificate the certificate buffer
 * \param certLen the length of the certificate buffer
 * \return true, when the certificate was set, false otherwise (e.g. unknown certificate format)
 */
PAL_API bool
TLSConfiguration_addAllowedCertificate(TLSConfiguration self, uint8_t* certificate, int certLen);

/**
 * \brief Add a certificate to the list of allowed peer certificates
 *
 * \param filename filename of the certificate file
 * \return true, when the certificate was set, false otherwise (e.g. unknown certificate format)
 */
PAL_API bool
TLSConfiguration_addAllowedCertificateFromFile(TLSConfiguration self, const char* filename);

/**
 * \brief Add a CA certificate used to validate peer certificates from a byte buffer
 *
 * \param certificate the certificate buffer
 * \param certLen the length of the certificate buffer
 * \return true, when the certificate was set, false otherwise (e.g. unknown certificate format)
 */
PAL_API bool
TLSConfiguration_addCACertificate(TLSConfiguration self, uint8_t* certificate, int certLen);

/**
 * \brief Add a CA certificate used to validate peer certificates from a file
 *
 * \param filename filename of the certificate file
 * \return true, when the certificate was set, false otherwise (e.g. unknown certificate format)
 */
PAL_API bool
TLSConfiguration_addCACertificateFromFile(TLSConfiguration self, const char* filename);

/**
 * \brief Set the renegotiation timeout.
 *
 * After the timeout elapsed a TLS session renegotiation has to occur.
 *
 * \param timeInMs session renegotiation timeout in milliseconds
 */
PAL_API void
TLSConfiguration_setRenegotiationTime(TLSConfiguration self, int timeInMs);

/**
 * \brief Set minimal allowed TLS version to use
 */
PAL_API void
TLSConfiguration_setMinTlsVersion(TLSConfiguration self, TLSConfigVersion version);

/**
 * \brief Set maximal allowed TLS version to use
 */
PAL_API void
TLSConfiguration_setMaxTlsVersion(TLSConfiguration self, TLSConfigVersion version);

/**
 * \brief Add a CRL (certificate revocation list) from buffer
 *
 * \param crl the buffer containing the CRL
 * \param crlLen the length of the CRL buffer
 * \return true, when the CRL was imported, false otherwise (e.g. unknown format)
 */
PAL_API bool
TLSConfiguration_addCRL(TLSConfiguration self, uint8_t* crl, int crlLen);

/**
 * \brief Add a CRL (certificate revocation list) from a file
 *
 * \param filename filename of the CRL file
 * \return true, when the CRL was imported, false otherwise (e.g. unknown format)
 */
PAL_API bool
TLSConfiguration_addCRLFromFile(TLSConfiguration self, const char* filename);

/**
 * \brief Removes any CRL (certificate revocation list) currently in use
 */
PAL_API void
TLSConfiguration_resetCRL(TLSConfiguration self);

/**
 * \brief Add an allowed ciphersuite to the list of allowed ciphersuites
 *
 * \param self the TLS configuration instance
 * \param ciphersuite the ciphersuite to add (IANA cipher suite ID)
 */
PAL_API void
TLSConfiguration_addCipherSuite(TLSConfiguration self, int ciphersuite);

/**
 * \brief Clear the list of allowed ciphersuites
 *
 * \param self the TLS configuration instance
 */
PAL_API void
TLSConfiguration_clearCipherSuiteList(TLSConfiguration self);

/**
 * Release all resource allocated by the TLSConfiguration instance
 *
 * NOTE: Do not use the object after calling this function!
 */
PAL_API void
TLSConfiguration_destroy(TLSConfiguration self);

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* SRC_TLS_CONFIG_H_ */
