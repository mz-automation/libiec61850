/*
 * tls_socket.h
 *
 * TLS socket API for protocol libraries using TCP/IP
 *
 * Copyright 2017-2020 Michael Zillgith, MZ Automation GmbH
 *
 * Abstraction layer for different TLS implementations
 *
 * The implementation has to connect the TLS API layer with the socket API layer
 * and perform all TLS tasks like handshake, encryption/decryption.
 *
 */

#ifndef SRC_TLS_SOCKET_API_H_
#define SRC_TLS_SOCKET_API_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \file tls_socket.h
 * \brief Abstraction layer for different TLS implementations.
 *
 * The implementation has to connect the TLS API layer with the socket API layer
 * and perform all TLS tasks like handshake, encryption/decryption.
 */

/*! \addtogroup hal Platform (Hardware/OS) abstraction layer
   *
   *  @{
   */

/**
 * @defgroup HAL_TLS_SOCKET Abstraction layer for different TLS implementations.
 *
 * The implementation has to connect the TLS API layer with the socket API layer
 * and perform all TLS tasks like handshake, encryption/decryption.
 *
 * @{
 */

#include <stdint.h>
#include "tls_config.h"
#include "hal_socket.h"

typedef struct sTLSSocket* TLSSocket;

/**
 * \brief This function create a new TLSSocket instance using the given Socket instance
 *
 * NOTE: This function also has to perform the TLS handshake
 *
 * \param socket the socket instance to use for the TLS connection
 * \param configuration the TLS configuration object to use
 * \param storeClientCert if true, the client certificate will be stored
 *                        for later access by \ref TLSSocket_getPeerCertificate
 *
 * \return new TLS connection instance
 */
PAL_API TLSSocket
TLSSocket_create(Socket socket, TLSConfiguration configuration, bool storeClientCert);

/**
 * \brief Perform a new TLS handshake/session renegotiation
 */
PAL_API bool
TLSSocket_performHandshake(TLSSocket self);

/**
 * \brief Access the certificate used by the peer
 *
 * \param[out] certSize the size of the certificate in bytes
 *
 * \return the certificate byte buffer
 */
PAL_API uint8_t*
TLSSocket_getPeerCertificate(TLSSocket self, int* certSize);

/**
 * \brief read from socket to local buffer (non-blocking)
 *
 * The function shall return immediately if no data is available. In this case
 * the function returns 0. If an error happens the function shall return -1.
 *
 * \param self the client, connection or server socket instance
 * \param buf the buffer where the read bytes are copied to
 * \param size the maximum number of bytes to read (size of the provided buffer)
 *
 * \return the number of bytes read or -1 if an error occurred
 */
PAL_API int
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
PAL_API int
TLSSocket_write(TLSSocket self, uint8_t* buf, int size);

/**
 * \brief Closes the TLS connection and released all resources
 */
PAL_API void
TLSSocket_close(TLSSocket self);

/*! @} */

/*! @} */

#ifdef __cplusplus
}
#endif

#endif /* SRC_TLS_SOCKET_API_H_ */
