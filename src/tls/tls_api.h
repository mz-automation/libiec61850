/*
 * tls_api.h
 *
 * TLS API for TCP/IP protocol stacks
 *
 * Copyright 2017 MZ Automation GmbH
 *
 * Abstraction layer for different TLS implementations
 *
 * Implementation connects the TLS API layer with the socket API layer
 * and performs all TLS tasks like handshake, encryption/decryption.
 *
 */

#ifndef SRC_TLS_TLS_API_H_
#define SRC_TLS_TLS_API_H_

#include "hal_socket.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct sTLSConfiguration* TLSConfiguration;

typedef struct sTLSSocket* TLSSocket;

TLSConfiguration
TLSConfiguration_create(void);

/* will be called by stack automatically */
void
TLSConfiguration_setClientMode(TLSConfiguration self);

void
TLSConfiguration_setChainValidation(TLSConfiguration self, bool value);

void
TLSConfiguration_setAllowOnlyKnownCertificates(TLSConfiguration self, bool value);

bool
TLSConfiguration_setOwnCertificate(TLSConfiguration self, uint8_t* certificate, int certLen);

bool
TLSConfiguration_setOwnCertificateFromFile(TLSConfiguration self, const char* filename);

bool
TLSConfiguration_setOwnKey(TLSConfiguration self, uint8_t* key, int keyLen, const char* keyPassword);

bool
TLSConfiguration_setOwnKeyFromFile(TLSConfiguration self, const char* filename, const char* keyPassword);

bool
TLSConfiguration_addAllowedCertificate(TLSConfiguration self, uint8_t* certifcate, int certLen);

bool
TLSConfiguration_addAllowedCertificateFromFile(TLSConfiguration self, const char* filename);

bool
TLSConfiguration_addCACertificate(TLSConfiguration self, uint8_t* certifcate, int certLen);

bool
TLSConfiguration_addCACertificateFromFile(TLSConfiguration self, const char* filename);

void
TLSConfiguration_destroy(TLSConfiguration self);

TLSSocket
TLSSocket_create(Socket socket, TLSConfiguration configuration, bool storeClientCert);

bool
TLSSocket_performHandshake(TLSSocket self);

uint8_t*
TLSSocket_getPeerCertificate(TLSSocket self, int* certSize);

/**
 * \brief read from socket to local buffer (non-blocking)
 *
 * The function shall return immediately if no data is available. In this case
 * the function returns 0. If an error happens the function shall return -1.
 *
 * Implementation of this function is MANDATORY
 *
 * NOTE: The behaviour of this function changed with version 0.8!
 *
 * \param self the client, connection or server socket instance
 * \param buf the buffer where the read bytes are copied to
 * \param size the maximum number of bytes to read (size of the provided buffer)
 *
 * \return the number of bytes read or -1 if an error occurred
 */
int
TLSSocket_read(TLSSocket self, uint8_t* buf, int size);

/**
 * \brief send a message through the socket
 *
 * Implementation of this function is MANDATORY
 *
 * \param self client, connection or server socket instance
 *
 * \return number of bytes transmitted of -1 in case of an error
 */
int
TLSSocket_write(TLSSocket self, uint8_t* buf, int size);

void
TLSSocket_close(TLSSocket self);

#ifdef __cplusplus
}
#endif

#endif /* SRC_TLS_TLS_API_H_ */
