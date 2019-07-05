/*
 *  iso_connection_parameters.h
 *
 *  Copyright 2013-2018 Michael Zillgith
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

#ifndef ISO_CONNECTION_PARAMETERS_H_
#define ISO_CONNECTION_PARAMETERS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "tls_config.h"

/**
 * \addtogroup mms_client_api_group
 */
/**@{*/


/**
 * \brief authentication mechanism used by AcseAuthenticator
 */
typedef enum
{
    /** Neither ACSE nor TLS authentication used */
    ACSE_AUTH_NONE = 0,

    /** Use ACSE password for client authentication */
    ACSE_AUTH_PASSWORD = 1,

    /** Use ACSE certificate for client authentication */
    ACSE_AUTH_CERTIFICATE = 2,

    /** Use TLS certificate for client authentication */
    ACSE_AUTH_TLS = 3
} AcseAuthenticationMechanism;


typedef struct sAcseAuthenticationParameter* AcseAuthenticationParameter;

struct sAcseAuthenticationParameter
{
    AcseAuthenticationMechanism mechanism;

    union
    {
        struct
        {
            uint8_t* octetString;
            int passwordLength;
        } password; /* for mechanism = ACSE_AUTH_PASSWORD */

        struct
        {
            uint8_t* buf;
            int length;
        } certificate; /* for mechanism = ACSE_AUTH_CERTIFICATE or ACSE_AUTH_TLS */

    } value;
};

LIB61850_API AcseAuthenticationParameter
AcseAuthenticationParameter_create(void);

LIB61850_API void
AcseAuthenticationParameter_destroy(AcseAuthenticationParameter self);

LIB61850_API void
AcseAuthenticationParameter_setAuthMechanism(AcseAuthenticationParameter self, AcseAuthenticationMechanism mechanism);

LIB61850_API void
AcseAuthenticationParameter_setPassword(AcseAuthenticationParameter self, char* password);


/**
 * \brief Callback function to authenticate a client
 *
 * \param parameter user provided parameter - set when user registers the authenticator
 * \param authParameter the authentication parameters provided by the client
 * \param securityToken pointer where to store an application specific security token - can be ignored if not used.
 * \param appReference ISO application reference (ap-title + ae-qualifier)
 *
 * \return true if client connection is accepted, false otherwise
 */
typedef bool
(*AcseAuthenticator)(void* parameter, AcseAuthenticationParameter authParameter, void** securityToken, IsoApplicationReference* appReference);

/**
 * \brief COTP T selector
 *
 * To not use T SEL set size to 0.
 */
typedef struct {
    uint8_t size; /** 0 .. 4 - 0 means T-selector is not present */
    uint8_t value[4]; /** T-selector value */
} TSelector;

/**
 * \brief OSI session selector
 *
 * To not use S SEL set size to 0
 */
typedef struct {
    uint8_t size; /** 0 .. 16 - 0 means S-selector is not present */
    uint8_t value[16]; /** S-selector value */
} SSelector;

/**
 * \brief OSI presentation (P) selector
 *
 * To not use P SEL set size to 0
 */
typedef struct {
    uint8_t size; /** 0 .. 16 - 0 means P-selector is not present */
    uint8_t value[16]; /** P-selector value */
} PSelector;

struct sIsoConnectionParameters
{
    AcseAuthenticationParameter acseAuthParameter;

#if (CONFIG_MMS_SUPPORT_TLS == 1)
    TLSConfiguration tlsConfiguration;
#endif

    const char* hostname;
    int tcpPort;

    uint8_t remoteApTitle[10];
    int remoteApTitleLen;
    int remoteAEQualifier;
    PSelector remotePSelector;
    SSelector remoteSSelector;
    TSelector remoteTSelector;


    uint8_t localApTitle[10];
    int localApTitleLen;
    int localAEQualifier;
    PSelector localPSelector;
    SSelector localSSelector;
    TSelector localTSelector;

};

typedef struct sIsoConnectionParameters* IsoConnectionParameters;

/**
 * \brief create a new IsoConnectionParameters instance (FOR LIBRARY INTERNAL USE)
 *
 * NOTE: This function used internally by the MMS client library. When using the MMS or IEC 61850 API
 * there should be no reason for the user to call this function.
 *
 * \return new IsoConnectionParameters instance
 */
LIB61850_API IsoConnectionParameters
IsoConnectionParameters_create(void);

/**
 * \brief Destroy an IsoConnectionParameters instance (FOR LIBRARY INTERNAL USE)
 *
 * NOTE: This function used internally by the MMS client library. When using the MMS or IEC 61850 API
 * there should be no reason for the user to call this function.
 *
 * \param self the IsoConnectionParameters instance
 */
LIB61850_API void
IsoConnectionParameters_destroy(IsoConnectionParameters self);


LIB61850_API void
IsoConnectionParameters_setTlsConfiguration(IsoConnectionParameters self, TLSConfiguration tlsConfig);

/**
 * \brief set the authentication parameter
 *
 * This will set the authentication parameter and activates authentication.
 *
 * \param self the IsoConnectionParameters instance
 * \param acseAuthParameter
 */
LIB61850_API void
IsoConnectionParameters_setAcseAuthenticationParameter(IsoConnectionParameters self,
        AcseAuthenticationParameter acseAuthParameter);

/**
 * \brief Set TCP parameters (FOR LIBRARY INTERNAL USE)
 *
 * NOTE: This function used internally by the MMS client library. When using the MMS or IEC 61850 API
 * there should be no reason for the user to call this function
 *
 * \param self the IsoConnectionParameters instance
 * \param hostname the hostname of IP address if the server
 * \param tcpPort the TCP port number of the server
 */
LIB61850_API void
IsoConnectionParameters_setTcpParameters(IsoConnectionParameters self, const char* hostname, int tcpPort);

/**
 * \brief set the remote AP-Title and AE-Qualifier
 *
 * Calling this function is optional and not recommended. If not called the default
 * parameters are used.
 * If apTitle is NULL the parameter the AP-Title and AE-Qualifier will not be transmitted.
 * This seems to be required by some server devices.
 *
 * \param self the IsoConnectionParameters instance
 * \param apTitle the AP-Title OID as string.
 * \param aeQualifier the AP-qualifier
 */
LIB61850_API void
IsoConnectionParameters_setRemoteApTitle(IsoConnectionParameters self, const char* apTitle, int aeQualifier);

/**
 * \brief set remote addresses for the lower layers
 *
 * This function can be used to set the addresses for the lower layer protocols (presentation, session, and transport
 * layer). Calling this function is optional and not recommended. If not called the default
 * parameters are used.
 *
 *  \param self the IsoConnectionParameters instance
 *  \param pSelector the P-Selector (presentation layer address)
 *  \param sSelector the S-Selector (session layer address)
 *  \param tSelector the T-Selector (ISO transport layer address)
 */
LIB61850_API void
IsoConnectionParameters_setRemoteAddresses(IsoConnectionParameters self, PSelector pSelector, SSelector sSelector, TSelector tSelector);

/**
 * \brief set the local AP-Title and AE-Qualifier
 *
 * Calling this function is optional and not recommended. If not called the default
 * parameters are used.
 * If apTitle is NULL the parameter the AP-Title and AE-Qualifier will not be transmitted.
 * This seems to be required by some server devices.
 *
 * \param self the IsoConnectionParameters instance
 * \param apTitle the AP-Title OID as string.
 * \param aeQualifier the AP-qualifier
 */
LIB61850_API void
IsoConnectionParameters_setLocalApTitle(IsoConnectionParameters self, const char* apTitle, int aeQualifier);

/**
 * \brief set local addresses for the lower layers
 *
 * This function can be used to set the addresses for the lower layer protocols (presentation, session, and transport
 * layer). Calling this function is optional and not recommended. If not called the default
 * parameters are used.
 *
 *  \param self the IsoConnectionParameters instance
 *  \param pSelector the P-Selector (presentation layer address)
 *  \param sSelector the S-Selector (session layer address)
 *  \param tSelector the T-Selector (ISO transport layer address)
 */
LIB61850_API void
IsoConnectionParameters_setLocalAddresses(IsoConnectionParameters self, PSelector pSelector, SSelector sSelector, TSelector tSelector);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif /* ISO_CONNECTION_PARAMETERS_H_ */
