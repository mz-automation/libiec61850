/*
 * tls_socket.h
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

#ifndef SRC_TLS_TLS_SOCKET_H_
#define SRC_TLS_TLS_SOCKET_H_

#include "tls_api.h"

#include "hal_socket.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct sTLSSocket* TLSSocket;

TLSSocket
TLSSocket_create(Socket socket, TLSConfiguration configuration, bool storeClientCert);

/**
 * \brief Perform a new TLS handshake/session renegotiation
 */
bool
TLSSocket_performHandshake(TLSSocket self);

/**
 * \brief Access the certificate used by the peer
 *
 * \param[out] certSize the size of the certificate in bytes
 *
 * \return the certificate byte buffer
 */
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

/**
 * \brief Close the TLS socket and release all resources
 */
void
TLSSocket_close(TLSSocket self);

#ifdef __cplusplus
}
#endif

#endif /* SRC_TLS_TLS_SOCKET_H_ */
