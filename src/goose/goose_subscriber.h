/*
 *  goose_subscriber.h
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

#ifndef GOOSE_SUBSCRIBER_H_
#define GOOSE_SUBSCRIBER_H_

#include "libiec61850_common_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup goose_api_group IEC 61850 GOOSE subscriber API
 */
/**@{*/

#include "mms_value.h"

typedef enum
{
    GOOSE_PARSE_ERROR_NO_ERROR = 0,
    GOOSE_PARSE_ERROR_UNKNOWN_TAG,
    GOOSE_PARSE_ERROR_TAGDECODE,
    GOOSE_PARSE_ERROR_SUBLEVEL,
    GOOSE_PARSE_ERROR_OVERFLOW,
    GOOSE_PARSE_ERROR_UNDERFLOW,
    GOOSE_PARSE_ERROR_TYPE_MISMATCH,
    GOOSE_PARSE_ERROR_LENGTH_MISMATCH,
    GOOSE_PARSE_ERROR_INVALID_PADDING
} GooseParseError;

typedef struct sGooseSubscriber* GooseSubscriber;

/**
 * \brief user provided callback function that will be invoked when a GOOSE message is received.
 *
 * \param subscriber the subscriber object that invoked the callback function,
 * \param parameter a user provided parameter that will be passed to the callback function
 */
typedef void (*GooseListener)(GooseSubscriber subscriber, void* parameter);

/**
 * \brief create a new GOOSE subscriber instance.
 *
 * A new GOOSE subscriber will be created and connected to a specific GOOSE control block reference.
 *
 * The parameter goCbRef has to be given in MMS like notation (as it also will appear in the GOOSE message
 * sent by the publisher). An example could be "simpleIOGenericIO/LLN0$GO$gcbEvents".
 *
 * The data set values contained in a GOOSE message will be written to the optionally provided MmsValue instance.
 * The MmsValue object has to be of type MMS_ARRAY. The array elements need to be of the same type as
 * the data set elements. It is intended that the provided MmsValue instance has been created by the
 * IedConnection_getDataSet() method before.
 *
 * If NULL is given as dataSetValues it will be created the first time when a appropriate GOOSE message
 * is received.
 *
 * \param goCbRef a string containing the object reference of the GOOSE Control Block (GoCB) in MMS notation the
 *        GOOSE publisher uses.
 * \param dataSetValues the MmsValue object where the data set values will be written or NULL.
 */
LIB61850_API GooseSubscriber
GooseSubscriber_create(char* goCbRef, MmsValue* dataSetValues);

/**
 * \brief Get the GoId value of the received GOOSE message
 *
 * \param self GooseSubscriber instance to operate on.
 */
LIB61850_API char*
GooseSubscriber_getGoId(GooseSubscriber self);

/**
 * \brief Get the GOOSE Control Block reference value of the received GOOSE message
 *
 * \param self GooseSubscriber instance to operate on.
 */
LIB61850_API char*
GooseSubscriber_getGoCbRef(GooseSubscriber self);

/**
 * \brief Get the DatSet value of the received GOOSE message
 *
 * \param self GooseSubscriber instance to operate on.
 */
LIB61850_API char*
GooseSubscriber_getDataSet(GooseSubscriber self);

/**
 * \brief set the destination mac address used by the subscriber to filter relevant messages.
 *
 * If dstMac is set the subscriber will ignore all messages with other dstMac values.
 *
 * \param self GooseSubscriber instance to operate on.
 * \param dstMac the destination mac address
 */
LIB61850_API void
GooseSubscriber_setDstMac(GooseSubscriber self, uint8_t dstMac[6]);

/**
 * \brief set the APPID used by the subscriber to filter relevant messages.
 *
 * If APPID is set the subscriber will ignore all messages with other APPID values.
 *
 * \param self GooseSubscriber instance to operate on.
 * \param appId the APPID value the subscriber should use to filter messages
 */
LIB61850_API void
GooseSubscriber_setAppId(GooseSubscriber self, uint16_t appId);

/**
 * \brief Check if subscriber state is valid
 *
 * A GOOSE subscriber is valid if TimeAllowedToLive timeout is not elapsed and GOOSE
 * message were received with correct state and sequence ID.
 *
 */
LIB61850_API bool
GooseSubscriber_isValid(GooseSubscriber self);

/**
 * \brief Get parse error in case of invalid subscriber state
 *
 * \param self GooseSubscriber instance to operate on.
 *
 * \return the error code representing a message parse problem of the last received message
 */
LIB61850_API GooseParseError
GooseSubscriber_getParseError(GooseSubscriber self);

/**
 * \brief Destroy the GooseSubscriber instance
 *
 * Do not call this function when the GooseSubscriber instance was added to a GooseReceiver.
 * The GooseReceiver will call the destructor when \ref GooseReceiver_destroy is called!
 *
 * \param self GooseSubscriber instance to operate on.
 */
LIB61850_API void
GooseSubscriber_destroy(GooseSubscriber self);

/**
 * \brief set a callback function that will be invoked when a GOOSE message has been received.
 *
 * \param self GooseSubscriber instance to operate on.
 * \param listener user provided callback function
 * \param parameter a user provided parameter that will be passed to the callback function
 */
LIB61850_API void
GooseSubscriber_setListener(GooseSubscriber self, GooseListener listener, void* parameter);

/**
 * \brief Get the APPID value of the received GOOSE message
 *
 * \param self GooseSubscriber instance to operate on.
 */
LIB61850_API int32_t
GooseSubscriber_getAppId(GooseSubscriber self);

/**
 * \brief Get the source MAC address of the received GOOSE message
 *
 * \param self GooseSubscriber instance to operate on.
 * \param buffer buffer to store the MAC address (at least 6 byte)
 */
LIB61850_API void
GooseSubscriber_getSrcMac(GooseSubscriber self, uint8_t* buffer);

/**
 * \brief Get the destination MAC address of the received GOOSE message
 *
 * \param self GooseSubscriber instance to operate on.
 * \param buffer buffer to store the MAC address (at least 6 byte)
 */
LIB61850_API void
GooseSubscriber_getDstMac(GooseSubscriber self, uint8_t* buffer);

/**
 * \brief return the state number (stNum) of the last received GOOSE message.
 *
 * The state number is increased if any of the values in the GOOSE data set changed due to a valid trigger condition
 *
 * \param self GooseSubscriber instance to operate on.
 *
 * \return the state number of the last received GOOSE message
 */
LIB61850_API uint32_t
GooseSubscriber_getStNum(GooseSubscriber self);

/**
 * \brief return the sequence number (sqNum) of the last received GOOSE message.
 *
 * The sequence number is increased for every consecutive GOOSE message without state change. When a state change occurs (stNum is increased)
 * then the sequence number (sqNum) will be reset.
 *
 * \param self GooseSubscriber instance to operate on.
 *
 * \return the sequence number of the last received GOOSE message
 */
LIB61850_API uint32_t
GooseSubscriber_getSqNum(GooseSubscriber self);

/**
 * \brief returns the test flag of the last received GOOSE message
 *
 * IMPORTANT: Goose messages with test=TRUE have to be ignored to be standard compliant!
 *
 * \param self GooseSubscriber instance to operate on.
 *
 * \return the state of the test flag of the last received GOOSE message.
 */
LIB61850_API bool
GooseSubscriber_isTest(GooseSubscriber self);

/**
 * \brief returns the confRev value of the last received GOOSE message
 *
 * \param self GooseSubscriber instance to operate on.
 *
 * \return the confRev value of the last received GOOSE message. If the message does not contain such
 *         a value the result is always 0
 */
LIB61850_API uint32_t
GooseSubscriber_getConfRev(GooseSubscriber self);

/**
 * \brief returns the value of the ndsCom (needs commission) flag of the last received GOOSE message.
 *
 * IMPORTANT: Goose messages with ndsCom=TRUE have to be ignored to be standard compliant!
 *
 * \param self GooseSubscriber instance to operate on.
 *
 * \return the state of the ndsCom flag of the last received GOOSE message.
 *
 */
LIB61850_API bool
GooseSubscriber_needsCommission(GooseSubscriber self);

/**
 * \brief Get the TimeAllowedToLive value of the last received message.
 *
 * \param self GooseSubscriber instance to operate on.
 *
 * \return the TimeAllowedToLive value of the last received GOOSE message in milliseconds.
 */
LIB61850_API uint32_t
GooseSubscriber_getTimeAllowedToLive(GooseSubscriber self);

/**
 * \brief Get the timestamp of the last received message.
 *
 * \param self GooseSubscriber instance to operate on.
 *
 * \return the timestamp value of the last received GOOSE message in milliseconds since epoch (1.1.1970 UTC).
 */
LIB61850_API uint64_t
GooseSubscriber_getTimestamp(GooseSubscriber self);

/**
 * \brief get the data set values received with the last report
 *
 * Note: To prevent data corruption. The MmsValue instance received should
 * only be used inside of the callback function, when the GOOSE receiver is
 * running in a separate thread.
 *
 * \param self GooseSubscriber instance to operate on.
 *
 * \return MmsValue instance of the report data set
 */
LIB61850_API MmsValue*
GooseSubscriber_getDataSetValues(GooseSubscriber self);

LIB61850_API bool
GooseSubscriber_isVlanSet(GooseSubscriber self);

LIB61850_API uint16_t
GooseSubscriber_getVlanId(GooseSubscriber self);

LIB61850_API uint8_t
GooseSubscriber_getVlanPrio(GooseSubscriber self);

/**
 * \brief Configure the Subscriber to listen to any received GOOSE message
 *
 * NOTE: When the observer flag is set the subscriber also has access to the
 * goCbRef, goId, and datSet values of the received GOOSE message
 */
LIB61850_API void
GooseSubscriber_setObserver(GooseSubscriber self);
#ifdef __cplusplus
}
#endif


/**@}*/

#endif /* GOOSE_SUBSCRIBER_H_ */
