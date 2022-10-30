/*
 *  goose_receiver.h
 *
 *  Copyright 2014-2022 Michael Zillgith
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

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "hal_ethernet.h"
#include "goose_subscriber.h"
#include "r_session.h"

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
LIB61850_API GooseReceiver
GooseReceiver_create(void);

/**
 * \brief Create a new receiver instance using the provided buffer instead of allocating an own buffer
 *
 * A GooseReceiver instance is used to handle all GOOSE messages received on a specific
 * network interface.
 *
 * \param buffer buffer to store Ethernet messages or NULL when using \ref GooseReceiver_handleMessage
 *
 * \return the new GooseReceiver instance
 */
LIB61850_API GooseReceiver
GooseReceiver_createEx(uint8_t* buffer);

/**
 * \brief Create a new R-GOOSE receiver instance.
 *
 * \param session the remote session protocol instance
 *
 * \return the newly created receiver instance
 */
LIB61850_API GooseReceiver
GooseReceiver_createRemote(RSession session);

/**
 * \brief sets the interface for the GOOSE receiver
 *
 * \param self the GooseReceiver instance
 * \param interfaceId
 */
LIB61850_API void
GooseReceiver_setInterfaceId(GooseReceiver self, const char* interfaceId);

/**
 * \brief return the interface ID used by the GOOSE receiver
 *
 * \param self the GosseReceiver instance
 *
 * \return the Ethernet interface ID string
 */
LIB61850_API const char*
GooseReceiver_getInterfaceId(GooseReceiver self);

/**
 * \brief Add a subscriber to this receiver instance
 *
 * NOTE: Do not call this function while the receiver is running (after GooseReceiver_start
 * has been called)!
 *
 * \param self the GooseReceiver instance
 * \param subscriber the GooseSubscriber instance to add
 */
LIB61850_API void
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
LIB61850_API void
GooseReceiver_removeSubscriber(GooseReceiver self, GooseSubscriber subscriber);

/**
 * \brief start the GOOSE receiver in a separate thread
 *
 * \param self the GooseReceiver instance
 */
LIB61850_API void
GooseReceiver_start(GooseReceiver self);

/**
 * \brief stop the GOOSE receiver running in a separate thread
 *
 * This function is used to stop the receiver thread started with GooseReceiver_start
 *
 * \param self the GooseReceiver instance
 */
LIB61850_API void
GooseReceiver_stop(GooseReceiver self);

/**
 * \brief Check if GOOSE receiver is running
 *
 * Can be used to check if \ref GooseReceiver_start has been successful.
 *
 * \param self the GooseReceiver instance
 *
 * \return true if GOOSE receiver is running, false otherwise
 */
LIB61850_API bool
GooseReceiver_isRunning(GooseReceiver self);

/**
 * \brief Free all resource of the GooseReceiver and all installed GooseSubscribers
 *
 * \param self the GooseReceiver instance
 */
LIB61850_API void
GooseReceiver_destroy(GooseReceiver self);

/***************************************
 * Functions for non-threaded operation
 ***************************************/
LIB61850_API EthernetSocket
GooseReceiver_startThreadless(GooseReceiver self);

LIB61850_API void
GooseReceiver_stopThreadless(GooseReceiver self);

/**
 * \brief Parse GOOSE messages if they are available
 *
 * Call after reception of an Ethernet frame or periodically
 *
 * \param self the receiver object
 *
 * \return true if a message was available and has been parsed, false otherwise
 */
LIB61850_API bool
GooseReceiver_tick(GooseReceiver self);

/**
 * \brief Parse a GOOSE message
 *
 * Call after reception of an Ethernet frame (can be used as an alternative to \ref GooseReceiver_tick
 * to avoid implementing the Ethernet HAL)
 *
 * \param self the receiver object
 * \param buffer a buffer containing the complete Ethernet message
 * \param size size of the Ethernet message
 */
LIB61850_API void
GooseReceiver_handleMessage(GooseReceiver self, uint8_t* buffer, int size);

/**@}*/

#ifdef __cplusplus
}
#endif


#endif /* GOOSE_RECEIVER_H_ */
