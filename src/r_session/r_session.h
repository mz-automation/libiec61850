/*
 *  r_session.h
 *
 *  Copyright 2013-2022 Michael Zillgith
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

#ifndef LIBIEC61850_R_SESSION_H_
#define LIBIEC61850_R_SESSION_H_

#include "libiec61850_common_api.h"
#include "hal_socket.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct sRSession* RSession;

typedef enum {
    R_SESSION_SEC_ALGO_NONE = 0,
    R_SESSION_SEC_ALGO_AES_128_GCM = 1,
    R_SESSION_SEC_ALGO_AES_256_GCM = 2
} RSecurityAlgorithm;

typedef enum {
    R_SESSION_SIG_ALGO_NONE = 0,
    R_SESSION_SIG_ALGO_HMAC_SHA256_80 = 1,
    R_SESSION_SIG_ALGO_HMAC_SHA256_128 = 2,
    R_SESSION_SIG_ALGO_HMAC_SHA256_256 = 3,
    R_SESSION_SIG_ALGO_AES_GMAC_64 = 4,
    R_SESSION_SIG_ALGO_AES_GMAC_128 = 5,
    R_SESSION_SIG_ALGO_HMAC_SHA3_80 = 6,
    R_SESSION_SIG_ALGO_HMAC_SHA3_128 = 7,
    R_SESSION_SIG_ALGO_HMAC_SHA3_256 = 8
} RSignatureAlgorithm;

typedef enum {
    R_SESSION_ERROR_OK = 0,
    R_SESSION_ERROR_INVALID_KEY = 1,
    R_SESSION_ERROR_KEY_QUEUE_FULL = 2,
    R_SESSION_ERROR_NO_SOCKET = 3,
    R_SESSION_ERROR_OUT_OF_MEMORY = 4,
    R_SESSION_ERROR_FAILED_TO_SEND = 5,
    R_SESSION_ERROR_FAILED_TO_RECEIVE = 6,
    R_SESSION_ERROR_INVALID_MESSAGE = 7,
    R_SESSION_ERROR_SET_FAILED = 8
} RSessionError;

typedef struct sRSessionPayloadElement* RSessionPayloadElement;

struct sRSessionPayloadElement
{
    bool simulation;
    uint16_t appId;
    uint8_t payloadType;
    uint8_t* payload;
    int payloadSize;
    RSessionPayloadElement nextElement; /* NULL when no more elements follow */
};

/**
 * \brief Create a new RSession instance to provide R-GOOSE/R-SMV support for GOOSE/SMV publisher/subscriber
 *
 * \return new RSession instance
 */
LIB61850_API RSession
RSession_create(void);

/**
 * \brief Set the maximum buffer size for session messages (range: 128 - 65535)
 *
 * \param self the RSession instance
 * \param bufferSize the size of the buffer for RSession UDP messages (range: 128 - 65535)
 */
LIB61850_API void
RSession_setBufferSize(RSession self, uint16_t bufferSize);

/**
 * \brief Set the security algorithms for the session instance
 * 
 * \note only for version 1 of the protocol!
 * 
 * \param secAlgo encryption algorithm to be used for the session instance
 * \param sigAlgo signature algorithm to be used for the session instance
 * 
 * \return returns R_SESSION_ERROR_OK
 */
LIB61850_API RSessionError
RSession_setSecurity(RSession self, RSecurityAlgorithm secAlgo, RSignatureAlgorithm sigAlgo);

/**
 * \brief Bind the RSession instance to a specific local IP address and UDP port
 * 
 * \param self the RSession instance
 * \param localAddress the local IP address to use
 * \param localPort the local UDP port to use (default is 102)
*/
LIB61850_API RSessionError
RSession_setLocalAddress(RSession self, const char* localAddress, int localPort);

/**
 * \brief Add this instance to an IPv4/IPv6 multicast group
 *
 * \param self the RSession instance
 * \param multicastAddress IPv4 or IPv6 multicast address
 *
 * \return R_SESSION_ERROR_OK on success, R_SESSION_ERROR_SET_FAILED otherwise
 */
LIB61850_API RSessionError
RSession_addMulticastGroup(RSession self, const char* multicastAddress);

/**
 * \brief Sets the multicast TTL (number of hops) for this RSession instance
 *
 * \param self the RSession instance
 * \param ttl number of hops for multicast messages. Default is 1 (not routable!)
 *
 * \return R_SESSION_ERROR_OK on success, error code otherwise
 */
LIB61850_API RSessionError
RSession_setMulticastTtl(RSession self, int ttl);

/**
 * \brief Set the destionation address and port for publishers
 *
 * \param self the RSession instance
 * \param remoteAddress remote IP address
 * \param remotePort remote UDP port
 *
 * \return R_SESSION_ERROR_OK on success, error code otherwise
 */
LIB61850_API RSessionError
RSession_setRemoteAddress(RSession self, const char* remoteAddress, int remotePort);

/**
 * \brief Start sending and receiving messages (bind to a local UDP port/interface)
 *
 * \param self the RSession instance
 *
 * \return R_SESSION_ERROR_OK on success, error code otherwise
 */
LIB61850_API RSessionError
RSession_start(RSession self);

/**
 * \brief Stop sending and receiving messages.
 *
 * \param self the RSession instance
 *
 * \return R_SESSION_ERROR_OK on success, error code otherwise
 */
LIB61850_API RSessionError
RSession_stop(RSession self);

/**
 * \brief Manually add a key to the RSession instance
 *
 * \param self the RSession instance
 * \param keyId the key ID is unique for the security association
 * \param key the key data
 * \param keyLength the length of the key in bytes
 * \param secAlgo the applicable security (encryption) algorithm
 * \param sigAlgo the applicable signature algorithm
 */
LIB61850_API RSessionError
RSession_addKey(RSession self, uint32_t keyId, uint8_t* key, int keyLength, RSecurityAlgorithm secAlgo, RSignatureAlgorithm sigAlgo);

/**
 * \brief Remove key from the list of accepted keys
 * 
 * \param self the RSession instance
 * \param keyId the key ID is unique for the security association
 */
LIB61850_API RSessionError
RSession_removeKey(RSession self, uint32_t keyId);

/**
 * \brief Remove all keys from the list of accepted keys
 * 
 * \param self the RSession instance 
 */
void
RSession_removeAllKeys(RSession self);

typedef enum
{
    RSESSION_KEY_EVENT__NEED_KEY = 1
} RSessionKeyEvent;

typedef void (*RSession_KeyEventHandler) (void* parameter, RSession rSession, RSessionKeyEvent event, uint32_t keyID);

/**
 * \brief Set a callback handler to receive key events from the RSession instance
 *
 * e.g. when the RSession instance has no valid key for the received messages or to publish messages.
 * 
 * \param self the RSession instance
 * \param handler the callback that is called when a new event happens
 * \param parameter user provided parameter that is passed to the user callback
 */
LIB61850_API void
RSession_setKeyEventHandler(RSession self, RSession_KeyEventHandler handler, void* parameter);

/**
 * \brief Set the active key for the sender/publisher
 *
 * \param self the RSession instance
 * \param keyId the key ID of the new active key (has to be added with \ref RSession_addKey before).
 * 
 * \return R_SESSION_ERROR_INVALID_KEY when no valid key with the given keyId is avialable, R_SESSION_ERROR_OK otherwise
 */
LIB61850_API RSessionError
RSession_setActiveKey(RSession self, uint32_t keyId);

/**
 * \brief Destroy the RSession instance and free all resource
 *
 * \param self the RSession instance
 */
LIB61850_API void
RSession_destroy(RSession self);

#ifdef __cplusplus
}
#endif

#endif /* LIBIEC61850_R_SESSION_H_ */
