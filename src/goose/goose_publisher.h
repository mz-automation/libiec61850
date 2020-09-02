/*
 *  goose_publisher.h
 *
 *  Copyright 2013-2020 Michael Zillgith
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

#ifndef GOOSE_PUBLISHER_H_
#define GOOSE_PUBLISHER_H_

#include "iec61850_common.h"
#include "linked_list.h"
#include "mms_value.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef GOOSE_SV_COMM_PARAMETERS
#define GOOSE_SV_COMM_PARAMETERS

typedef struct sCommParameters {
    uint8_t vlanPriority;
    uint16_t vlanId;
    uint16_t appId;
    uint8_t dstAddress[6];
} CommParameters;

#endif

typedef struct sGoosePublisher* GoosePublisher;

/**
 * \brief Create a new GoosePublisher instance
 *
 * NOTE: The created GoosePublisher instance uses VLAN tags
 *
 * \param parameters GOOSE communication parameters
 * \param interfaceId name of the Ethernet interface to use (e.g. "eth0")
 */
LIB61850_API GoosePublisher
GoosePublisher_create(CommParameters* parameters, const char* interfaceID);

/**
 * \brief Create a new GoosePublisher instance
 *
 * \param parameters GOOSE communication parameters
 * \param interfaceId name of the Ethernet interface to use (e.g. "eth0")
 * \param useVlanTag enable or disable the usage of VLAN tags in GOOSE messages
 */
LIB61850_API GoosePublisher
GoosePublisher_createEx(CommParameters* parameters, const char* interfaceID, bool useVlanTag);

/**
 * \brief Release all resources of the GoosePublisher instance
 *
 * \param self GoosePublisher instance
 */
LIB61850_API void
GoosePublisher_destroy(GoosePublisher self);

/**
 * \brief Publish a GOOSE message
 *
 * NOTE: This function also increased the sequence number of the GOOSE publisher
 *
 * \param self GoosePublisher instance
 * \param dataSet the GOOSE data set to send
 */
LIB61850_API int
GoosePublisher_publish(GoosePublisher self, LinkedList dataSet);

/**
 * \brief Publish a GOOSE message and store the sent message in the provided buffer
 *
 * \param self GoosePublisher instance
 * \param dataSet the GOOSE data set to send
 * \param msgBuf to store the sent message
 * \param[out] msgLen the length of the sent message
 * \param bufSize the size of the buffer to store the sent message
 */
LIB61850_API int
GoosePublisher_publishAndDump(GoosePublisher self, LinkedList dataSet, char* msgBuf, int32_t* msgLen, int32_t bufSize);

/**
 * \brief Sets the GoID used by the GoosePublisher instance
 *
 * \param self GoosePublisher instance
 * \param goID the GoId string
 */
LIB61850_API void
GoosePublisher_setGoID(GoosePublisher self, char* goID);

/**
 * \brief Sets the GoCB reference used by the GoosePublisher instance
 *
 * \param self GoosePublisher instance
 * \param goCbRef the GoCB reference string
 */
LIB61850_API void
GoosePublisher_setGoCbRef(GoosePublisher self, char* goCbRef);

/**
 * \brief Sets the time allowed to live value of the GOOSE messages
 *
 * \param self GoosePublisher instance
 * \param timeAllowedToLive the time allowed to live value in milliseconds
 */
LIB61850_API void
GoosePublisher_setTimeAllowedToLive(GoosePublisher self, uint32_t timeAllowedToLive);

/**
 * \brief Sets the data set reference used by the GoosePublisher instance
 *
 * \param self GoosePublisher instance
 * \param dataSetRef the data set reference string
 */
LIB61850_API void
GoosePublisher_setDataSetRef(GoosePublisher self, char* dataSetRef);

/**
 * \brief Sets the configuration revision used by the GoosePublisher instance
 *
 * \param self GoosePublisher instance
 * \param confRev the configuration revision value
 */
LIB61850_API void
GoosePublisher_setConfRev(GoosePublisher self, uint32_t confRev);

/**
 * \brief Sets simulation flag in sent GOOSE messages
 *
 * \param self GoosePublisher instance
 * \param simulation the value of the simulation flag
 */
LIB61850_API void
GoosePublisher_setSimulation(GoosePublisher self, bool simulation);

/**
 * \brief Manually sets the state number (stNum) of the GoosePublisher instance
 *
 * NOTE: Only for testing! Use \ref GoosePublisher_increaseStNum instead whenever
 * a data set member changes.
 *
 * \param self GoosePublisher instance
 * \param stNum the state number of the next GOOSE message to send
 */
LIB61850_API void
GoosePublisher_setStNum(GoosePublisher self, uint32_t stNum);

/**
 * \brief Manually sets the sequence number (sqNum) of the GoosePublisher instance
 *
 * NOTE: Only for testing! The sequence number is increase manually whenever \ref GoosePublisher_publish is called.
 *
 * \param self GoosePublisher instance
 * \param stNum the state number of the next GOOSE message to send
 */
LIB61850_API void
GoosePublisher_setSqNum(GoosePublisher self, uint32_t sqNum);

/**
 * \brief Sets the needs commission flag in sent GOOSE messages
 *
 * \param self GoosePublisher instance
 * \param ndsCom the value of the needs commission flag
 */
LIB61850_API void
GoosePublisher_setNeedsCommission(GoosePublisher self, bool ndsCom);

/**
 * \brief Increase the state number (stNum) of the GoosePublisher instance
 *
 * The state number should be increased whenever a member of the GOOSE data set changed
 *
 * NOTE: This function also resets the sequence number (sqNum)
 *
 * \param self GoosePublisher instance
 */
LIB61850_API uint64_t
GoosePublisher_increaseStNum(GoosePublisher self);

/**
 * \brief Reset state and sequence number of the GoosePublisher instance
 *
 * This function will set the state number (stNum) to 1 and the sequence number (sqNum) to 0.
 *
 * \param self GoosePublisher instance
 */
LIB61850_API void
GoosePublisher_reset(GoosePublisher self);

#ifdef __cplusplus
}
#endif

#endif /* GOOSE_PUBLISHER_H_ */
