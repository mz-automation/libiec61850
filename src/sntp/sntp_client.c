/*
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

#include "hal_socket.h"
#include "hal_time.h"
#include "hal_thread.h"
#include "libiec61850_platform_includes.h"
#include "sntp_client.h"
#include <time.h>
#include <errno.h>

#define SNTP_DEFAULT_PORT 123

#ifndef SNTP_DEBUG
#define SNTP_DEBUG 1
#endif

typedef struct sSNTPClient* SNTPClient;

/* new time types */

typedef struct
{
    uint32_t coarse;
    uint32_t fine;
} NtpTime;


struct sSNTPClient
{
    UdpSocket socket;
    HandleSet handleSet;
    nsSinceEpoch lastReceivedMessage; /* time of last received synchronization message */
    uint64_t nextMessageExpected;
    bool outStandingRequest;

    uint64_t pollInterval; /* poll interval in ns */

    nsSinceEpoch lastRequestTimestamp; /* timestamp used in the last request message */

    SNTPClient_UserCallback userCallback;
    void* userCallbackParameter;

    Thread thread;
    bool running;

    bool clockSynced;

    char* serverAddr;
    int serverPort;
};

msSinceEpoch
nsTimeToMsTime(nsSinceEpoch nsTime)
{
    return nsTime / 1000000UL;
}

#define UNIX_EPOCH_OFFSET 2208988800  /* offset between NTP seconds and UNIX time */

static nsSinceEpoch
ntpTimeToNsTime(uint32_t coarse, uint32_t fine)
{
    if ((coarse == 0) && (fine == 0))
        return 0;

    uint64_t nsTime = (coarse - UNIX_EPOCH_OFFSET) * 1000000000UL;

    uint64_t nsPart = fine * 1000000000UL;
    nsPart = nsPart >> 32;
    nsTime += nsPart;

    return nsTime;
}

static void
nsTimeToNtpTime(nsSinceEpoch nsTime, uint32_t* coarse, uint32_t* fine)
{
    uint32_t secondsPart = (uint32_t)((nsTime / 1000000000UL) + UNIX_EPOCH_OFFSET);

    *coarse = secondsPart;

    uint64_t nsPart = nsTime % 1000000000UL;
    nsPart = nsPart << 32;
    nsPart = nsPart / 1000000000UL;

    *fine = (uint32_t) nsPart;
}

static uint32_t
nsTimeToSeconds(nsSinceEpoch nsTime)
{
    uint32_t secondsSinceEpoch = (uint32_t)(nsTime / 1000000000UL);

    return secondsSinceEpoch;
}

static uint32_t
getUsPartFromNsTime(nsSinceEpoch nsTime)
{
    uint64_t nsPart = nsTime % 1000000000UL;

    uint32_t msPart = (uint32_t)(nsPart / 1000UL);

    return msPart;
}

static int
encodeUint32(uint8_t* buffer, int bufPos, uint32_t value)
{
    buffer[bufPos++] = (uint8_t) (value / 0x1000000);
    buffer[bufPos++] = (uint8_t) ((value / 0x10000) % 0x100);
    buffer[bufPos++] = (uint8_t) ((value / 0x100) % 0x100);
    buffer[bufPos++] = (uint8_t) (value % 0x100);

    return bufPos;
}

static int
decodeUint32(uint8_t* buffer, int bufPos, uint32_t* value)
{
    uint32_t val;

    val  = buffer[bufPos++] * 0x1000000;
    val += buffer[bufPos++] * 0x10000;
    val += buffer[bufPos++] * 0x100;
    val += buffer[bufPos++];

    *value = val;

    return bufPos;
}

static void
parseResponseMessage(SNTPClient self, uint8_t* buffer, int bufSize)
{
    self->lastReceivedMessage = Hal_getTimeInNs();

    int bufPos = 0;

    uint8_t header = buffer[bufPos++];
    int8_t stratum = (int8_t) buffer[bufPos++];
    int8_t poll = (int8_t) buffer[bufPos++];
    int8_t precision = (int8_t) buffer[bufPos++];

    int li = (header & 0xc0)>> 6;

    /* check for "clock-not-synchronized" */
    if (li == 3)
    {
        /* ignore time message */
        if (SNTP_DEBUG)
            printf("WARNING: received clock-not-synchronized from server\n");

        /* TODO call user callback? */

        return;
    }

    int version = (header & 0x38) >> 3;

    int mode = (header & 0x7);

    /* TODO: expect mode 4 - server */

    int timeoutInSeconds = 1 << poll;

    if (SNTP_DEBUG)
        printf("SNTP: response - version: %i mode: %i (4 = server, 3 = client) timeout(in s): %i\n", version, mode, timeoutInSeconds);

    /* root delay */
    bufPos += 4;

    /* root dispersion */
    bufPos += 4;

    /* reference */
    bufPos += 4;

    /* reference timestamp */
    uint32_t coarse;
    uint32_t fine;

    bufPos = decodeUint32(buffer, bufPos, &coarse);
    bufPos = decodeUint32(buffer, bufPos, &fine);

    uint64_t refTime = ntpTimeToNsTime(coarse, fine);

    /* originate timestamp */

    bufPos = decodeUint32(buffer, bufPos, &coarse);
    bufPos = decodeUint32(buffer, bufPos, &fine);

    uint64_t origTime = ntpTimeToNsTime(coarse, fine);

    /* receive timestamp */

    bufPos = decodeUint32(buffer, bufPos, &coarse);
    bufPos = decodeUint32(buffer, bufPos, &fine);

    uint64_t recvTime = ntpTimeToNsTime(coarse, fine);

    /* transmit timestamp */

    bufPos = decodeUint32(buffer, bufPos, &coarse);
    bufPos = decodeUint32(buffer, bufPos, &fine);

    uint64_t trnsTime = ntpTimeToNsTime(coarse, fine);

    /* set system time */
    if (Hal_setTimeInNs(trnsTime) == false)
    {
        if (SNTP_DEBUG)
            printf("SNTP: failed to set system clock!\n");
    }

    self->clockSynced = true;
    self->outStandingRequest = false;

    if (self->userCallback)
    {
        self->userCallback(self->userCallbackParameter, true);
    }

    if (SNTP_DEBUG)
    {
        printf("SNTP:   reference time: %u.%.6u\n", nsTimeToSeconds(refTime), getUsPartFromNsTime(refTime));
        printf("SNTP:   original time: %u.%.9u\n", nsTimeToSeconds(origTime), getUsPartFromNsTime(origTime));
        printf("SNTP:   receive time: %u.%.6u\n", nsTimeToSeconds(recvTime), getUsPartFromNsTime(recvTime));
        printf("SNTP:   transmit time: %u.%.6u\n", nsTimeToSeconds(trnsTime), getUsPartFromNsTime(trnsTime));
    }
}

static void
sendRequestMessage(SNTPClient self, const char* serverAddr, int serverPort)
{
    uint8_t buffer[100];

    memset(buffer, 0, sizeof(buffer));

    uint8_t li = 0;
    int8_t version = 3;
    int8_t mode = 3; /* mode: client */
    int8_t stratum = 0; /* stratum: not specified */
    int8_t poll = 4;
    int8_t precision = -6;

    int bufPos = 0;

    buffer[bufPos++] = (li << 6) | (version << 3) | mode;
    buffer[bufPos++] = stratum;
    buffer[bufPos++] = poll;
    buffer[bufPos++] = precision;

    /* root delay */
    bufPos = encodeUint32(buffer, bufPos, 1 << 16);

    /* root dispersion */
    bufPos = encodeUint32(buffer, bufPos, 1 << 16);

    /* reference */
    bufPos += 4;

    /* skip reference timestamp */
    bufPos += 8;

    /* skip originate timestamp */
    bufPos += 8;

    /* skip receive timestamp */
    bufPos += 8;

    uint32_t time_coarse;
    uint32_t time_fine;

    nsSinceEpoch nsTime = Hal_getTimeInNs();

    nsTimeToNtpTime(nsTime, &time_coarse, &time_fine);

    /* transmit timestamp */
    bufPos = encodeUint32(buffer, bufPos, time_coarse);
    bufPos = encodeUint32(buffer, bufPos, time_fine);

    UdpSocket_sendTo(self->socket, serverAddr ,serverPort, buffer, bufPos);

    if (SNTP_DEBUG)
        printf("SNTP: sent request to %s:%i\n", serverAddr, serverPort);

    self->lastRequestTimestamp = nsTime;
    self->outStandingRequest = true;
}

SNTPClient
SNTPClient_create()
{
    SNTPClient self = (SNTPClient) GLOBAL_CALLOC(1, sizeof(struct sSNTPClient));

    if (self)
    {
        self->socket = UdpSocket_create();

        if (self->socket == NULL)
        {
            GLOBAL_FREEMEM(self);
            self = NULL;
        }
        else
        {
            self->handleSet = Handleset_new();
            Handleset_addSocket(self->handleSet, (Socket) self->socket);
            self->pollInterval = 30000000000UL; /* 30 s */
        }
    }

    return self;
}

HandleSet
SNTPClient_getHandleSet(SNTPClient self)
{
    return self->handleSet;
}

void
SNTPClient_addServer(SNTPClient self, const char* serverAddr, int serverPort)
{
    if (self) {
        self->serverAddr = StringUtils_copyString(serverAddr);
        self->serverPort = serverPort;
    }
}

bool
SNTPClient_isSynchronized(SNTPClient self)
{
    return self->clockSynced;
}

void
SNTPClient_setUserCallback(SNTPClient self, SNTPClient_UserCallback callback, void* parameter)
{
    if (self)
    {
        self->userCallback = callback;
        self->userCallbackParameter = parameter;
    }
}

void
SNTPClient_setPollInterval(SNTPClient self, uint32_t intervalInSeconds)
{
    if (self)
        self->pollInterval = intervalInSeconds * 1000000000UL;
}

void
SNTPClient_tick(SNTPClient self)
{
    nsSinceEpoch now = Hal_getTimeInNs();

    if (self->lastReceivedMessage > now)
        self->lastReceivedMessage = now;

    if (self->lastRequestTimestamp > now)
        self->lastRequestTimestamp = now;

    if (self->lastRequestTimestamp > 0)
    {
        /* check for timeout */
        if ((now - self->lastReceivedMessage) > 300000000000UL)
        {
            if (self->clockSynced)
            {
                self->clockSynced = false;
                printf("SNTP: request timeout\n");

                if (self->userCallback)
                {
                    self->userCallback(self->userCallbackParameter, false);
                }
            }
        }
    }

    if (self->serverAddr)
    {
        if ((now - self->lastRequestTimestamp) > self->pollInterval)
        {
            sendRequestMessage(self, self->serverAddr, self->serverPort);
        }
    }
}

void
SNTPClient_handleIncomingMessage(SNTPClient self)
{
    char ipAddress[200];

    uint8_t buffer[200];
    int rcvdBytes = UdpSocket_receiveFrom(self->socket, ipAddress, 200, buffer, sizeof(buffer));

    if (rcvdBytes > 0)
    {
        if (SNTP_DEBUG)
            printf("SNTP: received response from %s\n", ipAddress);

        parseResponseMessage(self, buffer, rcvdBytes);
    }
    else if (rcvdBytes == -1)
    {
        printf("UDP socket error\n");
    }
    else
    {
        printf("No data!\n");
    }
}

static void*
handleThread(void* parameter)
{
    SNTPClient self = (SNTPClient) parameter;

    self->running = true;

    while (self->running) {

        SNTPClient_tick(self);

        if (Handleset_waitReady(self->handleSet, 1000) > 0) {
            SNTPClient_handleIncomingMessage(self);
        }
    }

    return NULL;
}

void
SNTPClient_start(SNTPClient self)
{
    if (self)
    {
        int sntpPoirt = SNTP_DEFAULT_PORT;

        if (UdpSocket_bind(self->socket, "0.0.0.0", SNTP_DEFAULT_PORT))
        {
            printf("Start NTP thread\n");

            self->thread = Thread_create(handleThread, self, false);

            if (self->thread)
                Thread_start(self->thread);
        }
        else
        {
            if (SNTP_DEBUG)
                printf("SNTP: Failed to bind to port %i\n", sntpPoirt);
        }
    }
}

void
SNTPClient_stop(SNTPClient self)
{
    if (self->thread)
    {
        self->running = false;
        Thread_destroy(self->thread);
        self->thread = NULL;
    }
}

void
SNTPClient_destroy(SNTPClient self)
{
    if (self)
    {
        SNTPClient_stop(self);

        if (self->serverAddr)
            GLOBAL_FREEMEM(self->serverAddr);

        if (self->socket)
            Socket_destroy((Socket) self->socket);

        GLOBAL_FREEMEM(self);
    }
}
