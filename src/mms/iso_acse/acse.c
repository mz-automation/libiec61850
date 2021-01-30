/*
 *  acse.c
 *
 *  Copyright 2013 Michael Zillgith
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

#include "libiec61850_platform_includes.h"
#include "acse.h"
#include "ber_encoder.h"
#include "ber_decode.h"

#if ((DEBUG_ISO_CLIENT == 1) || (DEBUG_ISO_SERVER == 1))
#define DEBUG_ACSE 1
#else
#define DEBUG_ACSE 0
#endif

static uint8_t appContextNameMms[] = { 0x28, 0xca, 0x22, 0x02, 0x03 };

static uint8_t auth_mech_password_oid[] = { 0x52, 0x03, 0x01 };

static uint8_t requirements_authentication[] = { 0x80 };

static AcseAuthenticationMechanism
checkAuthMechanismName(uint8_t* authMechanism, int authMechLen)
{
    AcseAuthenticationMechanism authenticationMechanism = ACSE_AUTH_NONE;

    if (authMechanism != NULL) {

        if (authMechLen == 3) {
            if (memcmp(auth_mech_password_oid, authMechanism, 3) == 0) {
                authenticationMechanism = ACSE_AUTH_PASSWORD;
            }
        }
    }

    return authenticationMechanism;
}

static bool
authenticateClient(AcseConnection* self, AcseAuthenticationMechanism mechanism, uint8_t* authValue, int authValueLen)
{
    struct sAcseAuthenticationParameter authParamStruct;

    AcseAuthenticationParameter authParameter = &authParamStruct;

    authParameter->mechanism = mechanism;

    if (mechanism == ACSE_AUTH_PASSWORD) {
        authParameter->value.password.octetString = authValue;
        authParameter->value.password.passwordLength = authValueLen;
    }
    else if (mechanism == ACSE_AUTH_TLS) {
        authParameter->value.certificate.buf = authValue;
        authParameter->value.certificate.length = authValueLen;
    }

    return self->authenticator(self->authenticatorParameter, authParameter, &(self->securityToken), &(self->applicationReference));
}

static bool
checkAuthentication(AcseConnection* self, uint8_t* authMechanism, int authMechLen, uint8_t* authValue, int authValueLen)
{
    self->securityToken = NULL;

    if (self->authenticator != NULL) {

        AcseAuthenticationMechanism mechanism = checkAuthMechanismName(authMechanism, authMechLen);

        if (mechanism == ACSE_AUTH_NONE) {

#if (CONFIG_MMS_SUPPORT_TLS == 1)
            if (self->tlsSocket) {

                int certLen;

                uint8_t* certBuf = TLSSocket_getPeerCertificate(self->tlsSocket, &certLen);

                if (certBuf) {
                    mechanism = ACSE_AUTH_TLS;
                    authValue = certBuf;
                    authValueLen = certLen;
                }

            }
#endif /* (CONFIG_MMS_SUPPORT_TLS == 1) */

        }

        return authenticateClient(self, mechanism, authValue, authValueLen);
    }
    else
        return true;
}

static int
parseUserInformation(AcseConnection* self, uint8_t* buffer, int bufPos, int maxBufPos, bool* userInfoValid)
{
    if (DEBUG_ACSE)
        printf("ACSE: parseUserInformation %i %i\n", bufPos, maxBufPos);

    bool hasindirectReference = false;
    bool isDataValid = false;

    while (bufPos < maxBufPos) {
        uint8_t tag = buffer[bufPos++];
        int len;

        bufPos = BerDecoder_decodeLength(buffer, &len, bufPos, maxBufPos);

        if (bufPos < 0) {
            *userInfoValid = false;
            return -1;
        }

        switch (tag)
        {

        case 0x02: /* indirect-reference */
            self->nextReference = BerDecoder_decodeUint32(buffer, len, bufPos);
            bufPos += len;
            hasindirectReference = true;
            break;

        case 0xa0: /* encoding */
            isDataValid = true;

            self->userDataBufferSize = len;
            self->userDataBuffer = buffer + bufPos;

            bufPos += len;

            break;

        default: /* ignore unknown tag */
            bufPos += len;
        }
    }

    if (DEBUG_ACSE) {
        if (!hasindirectReference)
            printf("ACSE: User data has no indirect reference!\n");

        if (!isDataValid)
            printf("ACSE: No valid user data\n");
    }

    if (hasindirectReference && isDataValid)
        *userInfoValid = true;
    else
        *userInfoValid = false;

    return bufPos;
}

static AcseIndication
parseAarePdu(AcseConnection* self, uint8_t* buffer, int bufPos, int maxBufPos)
{
    if (DEBUG_ACSE)
        printf("ACSE: parse AARE PDU\n");

    bool userInfoValid = false;

    uint32_t result = 99;

    while (bufPos < maxBufPos) {
        uint8_t tag = buffer[bufPos++];
        int len;

        bufPos = BerDecoder_decodeLength(buffer, &len, bufPos, maxBufPos);
        if (bufPos < 0)
            return ACSE_ERROR;

        switch (tag)
        {
        case 0xa1: /* application context name */
            bufPos += len;
            break;

        case 0xa2: /* result */
            bufPos++;

            bufPos = BerDecoder_decodeLength(buffer, &len, bufPos, maxBufPos);
            if (bufPos < 0)
                return ACSE_ERROR;

            result = BerDecoder_decodeUint32(buffer, len, bufPos);

            bufPos += len;
            break;

        case 0xa3: /* result source diagnostic */
            bufPos += len;
            break;

        case 0xbe: /* user information */
            if (buffer[bufPos] != 0x28) {
                if (DEBUG_ACSE)
                    printf("ACSE: invalid user info\n");
                bufPos += len;
            }
            else {
                bufPos++;

                bufPos = BerDecoder_decodeLength(buffer, &len, bufPos, maxBufPos);
                if (bufPos < 0)
                    return ACSE_ERROR;

                bufPos = parseUserInformation(self, buffer, bufPos, bufPos + len, &userInfoValid);
                if (bufPos < 0)
                    return ACSE_ERROR;
            }
            break;

        case 0x00: /* indefinite length end tag -> ignore */
            break;

        default: /* ignore unknown tag */
            if (DEBUG_ACSE)
                printf("ACSE: parseAarePdu: unknown tag %02x\n", tag);

            bufPos += len;
            break;
        }
    }

    if (!userInfoValid)
        return ACSE_ERROR;

    if (result != 0)
        return ACSE_ASSOCIATE_FAILED;

    return ACSE_ASSOCIATE;
}

static AcseIndication
parseAarqPdu(AcseConnection* self, uint8_t* buffer, int bufPos, int maxBufPos)
{
    if (DEBUG_ACSE)
        printf("ACSE: parse AARQ PDU\n");

    uint8_t* authValue = NULL;
    int authValueLen = 0;
    uint8_t* authMechanism = NULL;
    int authMechLen = 0;
    bool userInfoValid = false;

    while (bufPos < maxBufPos) {
        uint8_t tag = buffer[bufPos++];
        int len;

        bufPos = BerDecoder_decodeLength(buffer, &len, bufPos, maxBufPos);

        if (bufPos < 0) {
            if (DEBUG_ACSE)
                printf("ACSE: Invalid PDU!\n");
            return ACSE_ASSOCIATE_FAILED;
        }

        switch (tag)
        {
        case 0xa1: /* application context name */
            bufPos += len;
            break;

        case 0xa2: /* called AP title */
            bufPos += len;
            break;
        case 0xa3: /* called AE qualifier */
            bufPos += len;
            break;

        case 0xa6: /* calling AP title */
            {
                if (buffer[bufPos] == 0x06) { /* ap-title-form2 */

                    int innerLength = buffer[bufPos + 1];

                    if (innerLength == len - 2)
                        BerDecoder_decodeOID(buffer, bufPos + 2, innerLength, &(self->applicationReference.apTitle));
                }
            }
            bufPos += len;
            break;

        case 0xa7: /* calling AE qualifier */
            {
                if (buffer[bufPos] == 0x02) { /* ae-qualifier-form2 */

                    int innerLength = buffer[bufPos + 1];

                    if (innerLength == len - 2)
                        self->applicationReference.aeQualifier = BerDecoder_decodeInt32(buffer + 2, buffer[bufPos + 1], bufPos);
                }
            }
            bufPos += len;
            break;

        case 0x8a: /* sender ACSE requirements */
            bufPos += len;
            break;

        case 0x8b: /* (authentication) mechanism name */
            authMechLen = len;
            authMechanism = buffer + bufPos;
            bufPos += len;
            break;

        case 0xac: /* authentication value */
            bufPos++;

            bufPos = BerDecoder_decodeLength(buffer, &len, bufPos, maxBufPos);
            if (bufPos < 0) {
                if (DEBUG_ACSE)
                    printf("ACSE: Invalid PDU!\n");
                return ACSE_ASSOCIATE_FAILED;
            }

            authValueLen = len;
            authValue = buffer + bufPos;
            bufPos += len;
            break;

        case 0xbe: /* user information */
            if (buffer[bufPos] != 0x28) {
                if (DEBUG_ACSE)
                    printf("ACSE: invalid user info\n");
                bufPos += len;
            }
            else {
                bufPos++;

                bufPos = BerDecoder_decodeLength(buffer, &len, bufPos, maxBufPos);

                if (bufPos < 0) {
                    if (DEBUG_ACSE)
                        printf("ACSE: Invalid PDU!\n");
                    return ACSE_ASSOCIATE_FAILED;
                }

                bufPos = parseUserInformation(self, buffer, bufPos, bufPos + len, &userInfoValid);

                if (bufPos < 0) {
                    if (DEBUG_ACSE)
                        printf("ACSE: Invalid PDU!\n");
                    return ACSE_ASSOCIATE_FAILED;
                }
            }
            break;

        case 0x00: /* indefinite length end tag -> ignore */
            break;

        default: /* ignore unknown tag */
            if (DEBUG_ACSE)
                printf("ACSE: parseAarqPdu: unknown tag %02x\n", tag);

            bufPos += len;
            break;
        }
    }

    if (checkAuthentication(self, authMechanism, authMechLen, authValue, authValueLen) == false) {
        if (DEBUG_ACSE)
            printf("ACSE: parseAarqPdu: check authentication failed!\n");

        return ACSE_ASSOCIATE_FAILED;
    }

    if (userInfoValid == false) {
        if (DEBUG_ACSE)
            printf("ACSE: parseAarqPdu: user info invalid!\n");

        return ACSE_ASSOCIATE_FAILED;
    }

    return ACSE_ASSOCIATE;
}

void
AcseConnection_init(AcseConnection* self, AcseAuthenticator authenticator, void* parameter, TLSSocket tlsSocket)
{
    self->state = idle;
    self->nextReference = 0;
    self->userDataBuffer = NULL;
    self->userDataBufferSize = 0;
    self->authenticator = authenticator;
    self->authenticatorParameter = parameter;

#if (CONFIG_MMS_SUPPORT_TLS == 1)
    self->tlsSocket = tlsSocket;
#else
    (void)tlsSocket;
#endif

    memset(&(self->applicationReference), 0,
            sizeof(self->applicationReference));
}

AcseIndication
AcseConnection_parseMessage(AcseConnection* self, ByteBuffer* message)
{
    AcseIndication indication = ACSE_ERROR;

    uint8_t* buffer = message->buffer;

    int messageSize = message->size;

    int bufPos = 0;

    uint8_t messageType = buffer[bufPos++];

    int len;

    bufPos = BerDecoder_decodeLength(buffer, &len, bufPos, messageSize);

    if (bufPos < 0)
    {
        if (DEBUG_ACSE)
            printf("ACSE: AcseConnection_parseMessage: invalid ACSE message!\n");

        return ACSE_ERROR;
    }

    switch (messageType)
    {
    case 0x60:
        indication = parseAarqPdu(self, buffer, bufPos, messageSize);
        break;
    case 0x61:
        indication = parseAarePdu(self, buffer, bufPos, messageSize);
        break;
    case 0x62: /* A_RELEASE.request RLRQ-apdu */
        indication = ACSE_RELEASE_REQUEST;
        break;
    case 0x63: /* A_RELEASE.response RLRE-apdu */
        indication = ACSE_RELEASE_RESPONSE;
        break;
    case 0x64: /* A_ABORT */
        indication = ACSE_ABORT;
        break;
    case 0x00: /* indefinite length end tag -> ignore */
        break;
    default:
        if (DEBUG_ACSE)
            printf("ACSE: Unknown ACSE message\n");
        indication = ACSE_ERROR;
        break;
    }

    return indication;
}

void
AcseConnection_createAssociateFailedMessage(AcseConnection* self, BufferChain writeBuffer)
{
    AcseConnection_createAssociateResponseMessage(self, ACSE_RESULT_REJECT_PERMANENT, writeBuffer, NULL);
}

void
AcseConnection_createAssociateResponseMessage(AcseConnection* self,
        uint8_t acseResult,
        BufferChain writeBuffer,
        BufferChain payload
        )
{
    assert(self != NULL);
    assert(writeBuffer != NULL);
    assert(payload != NULL);

    int appContextLength = 9;
    int resultLength = 5;
    int resultDiagnosticLength = 5;

    int fixedContentLength = appContextLength + resultLength
            + resultDiagnosticLength;

    int variableContentLength = 0;

    int assocDataLength;
    int userInfoLength;
    int nextRefLength;

    int payloadLength = payload->length;

    /* single ASN1 type tag */
    variableContentLength += payloadLength;
    variableContentLength += 1;
    variableContentLength += BerEncoder_determineLengthSize(payloadLength);

    /* indirect reference */
    nextRefLength = BerEncoder_UInt32determineEncodedSize(self->nextReference);
    variableContentLength += nextRefLength;
    variableContentLength += 2;

    /* association data */
    assocDataLength = variableContentLength;
    variableContentLength += BerEncoder_determineLengthSize(assocDataLength);
    variableContentLength += 1;

    /* user information */
    userInfoLength = variableContentLength;
    variableContentLength += BerEncoder_determineLengthSize(userInfoLength);
    variableContentLength += 1;

    variableContentLength += 2;

    int contentLength = fixedContentLength + variableContentLength;

    uint8_t* buffer = writeBuffer->buffer;
    int bufPos = 0;

    bufPos = BerEncoder_encodeTL(0x61, contentLength, buffer, bufPos);

    /* application context name */
    bufPos = BerEncoder_encodeTL(0xa1, 7, buffer, bufPos);
    bufPos = BerEncoder_encodeTL(0x06, 5, buffer, bufPos);
    memcpy(buffer + bufPos, appContextNameMms, 5);
    bufPos += 5;

    /* result */
    bufPos = BerEncoder_encodeTL(0xa2, 3, buffer, bufPos);
    bufPos = BerEncoder_encodeTL(0x02, 1, buffer, bufPos);
    buffer[bufPos++] = acseResult;

    /* result source diagnostics */
    bufPos = BerEncoder_encodeTL(0xa3, 5, buffer, bufPos);
    bufPos = BerEncoder_encodeTL(0xa1, 3, buffer, bufPos);
    bufPos = BerEncoder_encodeTL(0x02, 1, buffer, bufPos);
    buffer[bufPos++] = 0;

    /* user information */
    bufPos = BerEncoder_encodeTL(0xbe, userInfoLength, buffer, bufPos);

    /* association data */
    bufPos = BerEncoder_encodeTL(0x28, assocDataLength, buffer, bufPos);

    /* indirect-reference */
    bufPos = BerEncoder_encodeTL(0x02, nextRefLength, buffer, bufPos);
    bufPos = BerEncoder_encodeUInt32(self->nextReference, buffer, bufPos);

    /* single ASN1 type */
    bufPos = BerEncoder_encodeTL(0xa0, payloadLength, buffer, bufPos);

    writeBuffer->partLength = bufPos;
    writeBuffer->length = bufPos + payloadLength;
    writeBuffer->nextPart = payload;
}

void
AcseConnection_createAssociateRequestMessage(AcseConnection* self,
        IsoConnectionParameters isoParameters,
        BufferChain writeBuffer,
        BufferChain payload,
        AcseAuthenticationParameter authParameter)
{
    (void)self;

    assert(self != NULL);
    assert(writeBuffer != NULL);
    assert(payload != NULL);

    int payloadLength = payload->length;
    int authValueLength;
    int authValueStringLength = 0;

    int passwordLength = 0;

    int contentLength = 0;

    /* application context name */
    contentLength += 9;

    int calledAEQualifierLength = 0;

    if (isoParameters->remoteApTitleLen > 0)
    {
        /* called AP title */
        contentLength += (4 + isoParameters->remoteApTitleLen);

        calledAEQualifierLength = BerEncoder_UInt32determineEncodedSize(
                isoParameters->remoteAEQualifier);

        /* called AE qualifier */
        contentLength += (4 + calledAEQualifierLength);
    }

    int callingAEQualifierLength = 0;

    if (isoParameters->localApTitleLen > 0)
    {
        /* calling AP title */
        contentLength += (4 + isoParameters->localApTitleLen);

        callingAEQualifierLength = BerEncoder_UInt32determineEncodedSize(
                isoParameters->localAEQualifier);

        /* calling AE qualifier */
        contentLength += (4 + callingAEQualifierLength);
    }

    if (authParameter != NULL)
    {
        /* sender ACSE requirements */
        contentLength += 4;

        /* mechanism name */
        contentLength += 5;

        /* authentication value */
        if (authParameter->mechanism == ACSE_AUTH_PASSWORD)
        {
            contentLength += 2;

            passwordLength = authParameter->value.password.passwordLength;

            authValueStringLength = BerEncoder_determineLengthSize(
                    passwordLength);

            contentLength += passwordLength + authValueStringLength;

            authValueLength = BerEncoder_determineLengthSize(
                    passwordLength + authValueStringLength + 1);

            contentLength += authValueLength;
        }
        else
        {
            contentLength += 2;
        }
    }

    /* user information */
    int userInfoLength = 0;

    /* single ASN1 type tag */
    userInfoLength += payloadLength;
    userInfoLength += 1;
    userInfoLength += BerEncoder_determineLengthSize(payloadLength);

    /* indirect reference */
    userInfoLength += 1;
    userInfoLength += 2;

    /* association data */
    int assocDataLength = userInfoLength;
    userInfoLength += BerEncoder_determineLengthSize(assocDataLength);
    userInfoLength += 1;

    /* user information */
    int userInfoLen = userInfoLength;
    userInfoLength += BerEncoder_determineLengthSize(userInfoLength);
    userInfoLength += 1;

    contentLength += userInfoLength;

    uint8_t* buffer = writeBuffer->buffer;
    int bufPos = 0;

    bufPos = BerEncoder_encodeTL(0x60, contentLength, buffer, bufPos);

    /* application context name */
    bufPos = BerEncoder_encodeTL(0xa1, 7, buffer, bufPos);
    bufPos = BerEncoder_encodeTL(0x06, 5, buffer, bufPos);
    memcpy(buffer + bufPos, appContextNameMms, 5);
    bufPos += 5;

    if (isoParameters->remoteApTitleLen > 0)
    {
        /* called AP title */
        bufPos = BerEncoder_encodeTL(0xa2, isoParameters->remoteApTitleLen + 2,
                buffer, bufPos);
        bufPos = BerEncoder_encodeTL(0x06, isoParameters->remoteApTitleLen,
                buffer, bufPos);

        memcpy(buffer + bufPos, isoParameters->remoteApTitle,
                isoParameters->remoteApTitleLen);
        bufPos += isoParameters->remoteApTitleLen;

        /* called AE qualifier */
        bufPos = BerEncoder_encodeTL(0xa3, calledAEQualifierLength + 2, buffer,
                bufPos);
        bufPos = BerEncoder_encodeTL(0x02, calledAEQualifierLength, buffer,
                bufPos);
        bufPos = BerEncoder_encodeUInt32(isoParameters->remoteAEQualifier,
                buffer, bufPos);
    }

    if (isoParameters->localApTitleLen > 0)
    {
        /* calling AP title */
        bufPos = BerEncoder_encodeTL(0xa6, isoParameters->localApTitleLen + 2,
                buffer, bufPos);
        bufPos = BerEncoder_encodeTL(0x06, isoParameters->localApTitleLen,
                buffer, bufPos);
        memcpy(buffer + bufPos, isoParameters->localApTitle,
                isoParameters->localApTitleLen);
        bufPos += isoParameters->localApTitleLen;

        /* calling AE qualifier */
        bufPos = BerEncoder_encodeTL(0xa7, callingAEQualifierLength + 2, buffer,
                bufPos);
        bufPos = BerEncoder_encodeTL(0x02, callingAEQualifierLength, buffer,
                bufPos);
        bufPos = BerEncoder_encodeUInt32(isoParameters->localAEQualifier,
                buffer, bufPos);
    }

    if (authParameter != NULL)
    {
        /* sender requirements */
        bufPos = BerEncoder_encodeTL(0x8a, 2, buffer, bufPos);
        buffer[bufPos++] = 0x04;

        if (authParameter->mechanism == ACSE_AUTH_PASSWORD)
        {
            buffer[bufPos++] = requirements_authentication[0];

            bufPos = BerEncoder_encodeTL(0x8b, 3, buffer, bufPos);
            memcpy(buffer + bufPos, auth_mech_password_oid, 3);
            bufPos += 3;

            /* authentication value */
            bufPos = BerEncoder_encodeTL(0xac,
                    authValueStringLength + passwordLength + 1, buffer, bufPos);
            bufPos = BerEncoder_encodeTL(0x80, passwordLength, buffer, bufPos);
            memcpy(buffer + bufPos, authParameter->value.password.octetString,
                    passwordLength);
            bufPos += passwordLength;
        }
        else
        { /* AUTH_NONE */
            buffer[bufPos++] = 0;
        }
    }

    /* user information */
    bufPos = BerEncoder_encodeTL(0xbe, userInfoLen, buffer, bufPos);

    /* association data */
    bufPos = BerEncoder_encodeTL(0x28, assocDataLength, buffer, bufPos);

    /* indirect-reference */
    bufPos = BerEncoder_encodeTL(0x02, 1, buffer, bufPos);
    buffer[bufPos++] = 3;

    /* single ASN1 type */
    bufPos = BerEncoder_encodeTL(0xa0, payloadLength, buffer, bufPos);

    writeBuffer->partLength = bufPos;
    writeBuffer->length = bufPos + payload->length;
    writeBuffer->nextPart = payload;
}

/**
 * \param isProvider specifies abort source (false = user/client; true = provider/server)
 */
void
AcseConnection_createAbortMessage(AcseConnection* self, BufferChain writeBuffer, bool isProvider)
{
    (void)self;

    uint8_t* buffer = writeBuffer->buffer;

    buffer[0] = 0x64; /* [APPLICATION 4] */
    buffer[1] = 3;
    buffer[2] = 0x80;
    buffer[3] = 1;

    if (isProvider)
        buffer[4] = 1;
    else
        buffer[4] = 0;

    writeBuffer->partLength = 5;
    writeBuffer->length = 5;
    writeBuffer->nextPart = NULL;
}

void
AcseConnection_createReleaseRequestMessage(AcseConnection* self, BufferChain writeBuffer)
{
    (void)self;

    uint8_t* buffer = writeBuffer->buffer;

    buffer[0] = 0x62;
    buffer[1] = 3;
    buffer[2] = 0x80;
    buffer[3] = 1;
    buffer[4] = 0;

    writeBuffer->partLength = 5;
    writeBuffer->length = 5;
    writeBuffer->nextPart = NULL;
}

void
AcseConnection_createReleaseResponseMessage(AcseConnection* self, BufferChain writeBuffer)
{
    (void)self;

    uint8_t* buffer = writeBuffer->buffer;

    buffer[0] = 0x63;
    buffer[1] = 0;

    writeBuffer->partLength = 2;
    writeBuffer->length = 2;
    writeBuffer->nextPart = NULL;
}

