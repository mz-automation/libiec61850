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

typedef struct sGooseReceiver* GooseReceiver;

GooseReceiver
GooseReceiver_create(void);

void
GooseReceiver_setInterfaceId(GooseReceiver self, const char* interfaceId);

void
GooseReceiver_addSubscriber(GooseReceiver self, GooseSubscriber subscriber);

void
GooseReceiver_removeSubscriber(GooseReceiver self, GooseSubscriber subscriber);

// call backup listener if message is not handled by a subscriber
void
GooseReceiver_setBackupListener(GooseReceiver self);

// start GOOSE receiver in a separate thread
void
GooseReceiver_start(GooseReceiver self);

void
GooseReceiver_stop(GooseReceiver self);

void
GooseReceiver_destroy(GooseReceiver self);

/***************************************
 * Functions for non-threaded operation
 ***************************************/
void
GooseReceiver_startThreadless(GooseReceiver self);

void
GooseReceiver_stopThreadless(GooseReceiver self);

// call after reception of ethernet frame and periodically to to house keeping tasks
void
GooseReceiver_tick(GooseReceiver self);

#endif /* GOOSE_RECEIVER_H_ */
