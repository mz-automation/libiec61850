/*
 *  goose_receiver.h
 *
 *  Copyright 2014 Michael Zillgith
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

#ifndef GOOSE_RECEIVER_H_
#define GOOSE_RECEIVER_H_

#include <goose_subscriber.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * \addtogroup goose_api_group
 */
/**@{*/


typedef struct sGooseReceiver* GooseReceiver;

/**
 * \brief Create a new receiver instance
 *
 * A GooseReceiver instance is used to handle all GOOSE messages received on a specific
 * network interface.
 *
 * \return the new GooseReceiver instance
 */
GooseReceiver
GooseReceiver_create(void);

/**
 * \brief sets the interface for the GOOSE receiver
 *
 * \param self the GooseReceiver instance
 * \param interfaceId
 */
void
GooseReceiver_setInterfaceId(GooseReceiver self, const char* interfaceId);

/**
 * \brief Add a subscriber to this receiver instance
 *
 * NOTE: Do not call this function while the receiver is running (after GooseReceiver_start
 * has been called)!
 *
 * \param self the GooseReceiver instance
 * \param subscriber the GooseSubscriber instance to add
 */
void
GooseReceiver_addSubscriber(GooseReceiver self, GooseSubscriber subscriber);

/**
 * \brief Remove a subscriber from this receiver instance
 *
 * NOTE: Do not call this function while the receiver is running (after GooseReceiver_start
 * has been called)!
 *
 * \param self the GooseReceiver instance
 * \param subscriber the GooseSubscriber instance to remove
 */
void
GooseReceiver_removeSubscriber(GooseReceiver self, GooseSubscriber subscriber);

/**
 * \brief start the GOOSE receiver in a separate thread
 *
 * \param self the GooseReceiver instance
 */
void
GooseReceiver_start(GooseReceiver self);

/**
 * \brief stop the GOOSE receiver running in a separate thread
 *
 * This function is used to stop the receiver thread started with GooseReceiver_start
 *
 * \param self the GooseReceiver instance
 */
void
GooseReceiver_stop(GooseReceiver self);

bool
GooseReceiver_isRunning(GooseReceiver self);

/**
 * \brief Free all resource of the GooseReceiver and all installed GooseSubscribers
 *
 * \param self the GooseReceiver instance
 */
void
GooseReceiver_destroy(GooseReceiver self);

/***************************************
 * Functions for non-threaded operation
 ***************************************/
void
GooseReceiver_startThreadless(GooseReceiver self);

void
GooseReceiver_stopThreadless(GooseReceiver self);

/**
 * \brief Parse GOOSE messages if they are available
 *
 * Call after reception of ethernet frame and periodically to to house keeping tasks
 *
 * \param self the receiver object
 *
 * \return true if a message was available and has been parsed, false otherwise
 */
bool
GooseReceiver_tick(GooseReceiver self);

/* Forward declaration */
typedef struct sEthernetHandleSet* EthernetHandleSet;

/**
 * \brief Add the receiver to a handleset for multiplexed asynchronous IO.
 *
 * Note: This function must only be called after GooseReceiver_startThreadless().
 *
 * \param[in] self The SVReceiver instance.
 * \param[inout] handles The EthernetHandleSet to which the EthernetSocket of this receiver should be added.
 */
void
GooseReceiver_addHandleSet(GooseReceiver self, EthernetHandleSet handles);

/**@}*/

#ifdef __cplusplus
}
#endif


#endif /* GOOSE_RECEIVER_H_ */
