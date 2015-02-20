/*
 *  goose_publisher.h
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

#ifndef GOOSE_PUBLISHER_H_
#define GOOSE_PUBLISHER_H_

#include "linked_list.h"
#include "mms_value.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct sCommParameters {
    uint8_t vlanPriority;
    uint16_t vlanId;
    uint16_t appId;
    uint8_t dstAddress[6];
} CommParameters;

typedef struct sGoosePublisher* GoosePublisher;

GoosePublisher
GoosePublisher_create(CommParameters* parameters, const char* interfaceID);

void
GoosePublisher_destroy(GoosePublisher self);

int
GoosePublisher_publish(GoosePublisher self, LinkedList dataSet);

void
GoosePublisher_setGoID(GoosePublisher self, char* goID);

void
GoosePublisher_setGoCbRef(GoosePublisher self, char* goCbRef);

void
GoosePublisher_setTimeAllowedToLive(GoosePublisher self, uint32_t timeAllowedToLive);

void
GoosePublisher_setDataSetRef(GoosePublisher self, char* dataSetRef);

void
GoosePublisher_setConfRev(GoosePublisher self, uint32_t confRev);

void
GoosePublisher_setSimulation(GoosePublisher self, bool simulation);

void
GoosePublisher_setNeedsCommission(GoosePublisher self, bool ndsCom);

uint64_t
GoosePublisher_increaseStNum(GoosePublisher self);

void
GoosePublisher_reset(GoosePublisher self);

#ifdef __cplusplus
}
#endif

#endif /* GOOSE_PUBLISHER_H_ */
