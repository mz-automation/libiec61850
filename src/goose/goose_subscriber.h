/*
 *  goose_subscriber.h
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
 * is recevied.
 *
 * \param goCbRef a string containing the object reference of the GOOSE Control Block (GoCB) in MMS notation the
 *        GOOSE publisher uses.
 * \param dataSetValues the MmsValue object where the data set values will be written or NULL.
 */
GooseSubscriber
GooseSubscriber_create(char* goCbRef, MmsValue* dataSetValues);

/**
 * \brief set the APPID used by the subscriber to filter relevant messages.
 *
 * If APPID is set the subscriber will ignore all messages with other APPID values.
 *
 * \param self GooseSubscriber instance to operate on.
 * \param the APPID value the subscriber should use to filter messages
 */
void
GooseSubscriber_setAppId(GooseSubscriber self, uint16_t appId);

/**
 * \brief set the ethernet interface that should be used.
 *
 * \param self GooseSubscriber instance to operate on.
 * \param interfaceId the id of the interface (e.g. a network device name like eth0
 *        for linux or a numerical index for windows)
 */
void
GooseSubscriber_setInterfaceId(GooseSubscriber self, char* interfaceId);

/**
 * \brief Start listening to GOOSE messages
 *
 * \param self GooseSubscriber instance to operate on.
 */
void
GooseSubscriber_subscribe(GooseSubscriber self);

/**
 * \brief Stop listening to GOOSE messages
 *
 * \param self GooseSubscriber instance to operate on.
 */
void
GooseSubscriber_unsubscribe(GooseSubscriber self);

void
GooseSubscriber_destroy(GooseSubscriber self);

/**
 * \brief set a callback function that will be invoked when a GOOSE message has been received.
 *
 * \param self GooseSubscriber instance to operate on.
 * \param listener user provided callback function
 * \param parameter a user provided parameter that will be passed to the callback function
 */
void
GooseSubscriber_setListener(GooseSubscriber self, GooseListener listener, void* parameter);

uint32_t
GooseSubscriber_getStNum(GooseSubscriber self);

uint32_t
GooseSubscriber_getSqNum(GooseSubscriber self);

bool
GooseSubscriber_isTest(GooseSubscriber self);

bool
GooseSubscriber_needsCommission(GooseSubscriber self);

uint32_t
GooseSubscriber_getTimeAllowedToLive(GooseSubscriber self);

uint64_t
GooseSubscriber_getTimestamp(GooseSubscriber self);

MmsValue*
GooseSubscriber_getDataSetValues(GooseSubscriber self);

#ifdef __cplusplus
}
#endif


/**@}*/

#endif /* GOOSE_SUBSCRIBER_H_ */
