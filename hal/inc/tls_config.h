/*
 * tls_config.h
 *
 * TLS Configuration API for protocol stacks using TCP/IP
 *
 * Copyright 2017-2021 Michael Zillgith
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

/**
 * \brief Enables the validation of the certificate trust chain (enabled by default)
 *
 * \param value true to enable chain validation, false to disable
 */
PAL_API void
TLSConfiguration_setChainValidation(TLSConfiguration self, bool value);

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

typedef enum {
   TLS_VERSION_NOT_SELECTED = 0,
   TLS_VERSION_SSL_3_0 = 3,
   TLS_VERSION_TLS_1_0 = 4,
   TLS_VERSION_TLS_1_1 = 5,
   TLS_VERSION_TLS_1_2 = 6,
   TLS_VERSION_TLS_1_3 = 7
} TLSConfigVersion;

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
