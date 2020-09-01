/*
 *  goose_publisher.h
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

#ifndef GOOSE_PUBLISHER_H_
#define GOOSE_PUBLISHER_H_

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

LIB61850_API GoosePublisher
GoosePublisher_create(CommParameters* parameters, const char* interfaceID);

LIB61850_API GoosePublisher
GoosePublisher_createEx(CommParameters* parameters, const char* interfaceID, bool useVlanTag);

LIB61850_API void
GoosePublisher_destroy(GoosePublisher self);

LIB61850_API int
GoosePublisher_publish(GoosePublisher self, LinkedList dataSet);

LIB61850_API void
GoosePublisher_setGoID(GoosePublisher self, char* goID);

LIB61850_API void
GoosePublisher_setGoCbRef(GoosePublisher self, char* goCbRef);

LIB61850_API void
GoosePublisher_setTimeAllowedToLive(GoosePublisher self, uint32_t timeAllowedToLive);

LIB61850_API void
GoosePublisher_setDataSetRef(GoosePublisher self, char* dataSetRef);

LIB61850_API void
GoosePublisher_setConfRev(GoosePublisher self, uint32_t confRev);

LIB61850_API void
GoosePublisher_setSimulation(GoosePublisher self, bool simulation);

LIB61850_API void
GoosePublisher_setNeedsCommission(GoosePublisher self, bool ndsCom);

LIB61850_API uint64_t
GoosePublisher_increaseStNum(GoosePublisher self);

LIB61850_API void
GoosePublisher_reset(GoosePublisher self);

#ifdef __cplusplus
}
#endif

#endif /* GOOSE_PUBLISHER_H_ */
