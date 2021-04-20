/*
 *  goose_subscriber.c
 *
 *  Copyright 2013, 2014 Michael Zillgith
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

#include "stack_config.h"
#include "goose_subscriber.h"
#include "hal_ethernet.h"
#include "hal_thread.h"

#include "ber_decode.h"

#include "mms_value.h"
#include "mms_value_internal.h"

#include "goose_receiver_internal.h"

GooseSubscriber
GooseSubscriber_create(char* goCbRef, MmsValue* dataSetValues)
{
    GooseSubscriber self = (GooseSubscriber) GLOBAL_CALLOC(1, sizeof(struct sGooseSubscriber));

    strncpy(self->goCBRef, goCbRef, 129);
    self->goCBRef[129] = 0;

    self->goCBRefLen = strlen(goCbRef);
    self->timestamp = MmsValue_newUtcTime(0);
    self->dataSetValues = dataSetValues;

    if (dataSetValues)
        self->dataSetValuesSelfAllocated = false;
    else
        self->dataSetValuesSelfAllocated = true;

    memset(self->dstMac, 0xFF, 6);
    self->dstMacSet = false;
    self->appId = -1;
    self->isObserver = false;
    self->vlanSet = false;
    self->parseError = GOOSE_PARSE_ERROR_NO_ERROR;

    return self;
}

bool
GooseSubscriber_isValid(GooseSubscriber self)
{
    if (self->stateValid == false)
        return false;

    if (Hal_getTimeInMs() > self->invalidityTime)
        return false;

    return true;
}

GooseParseError
GooseSubscriber_getParseError(GooseSubscriber self)
{
    return self->parseError;
}

void
GooseSubscriber_setDstMac(GooseSubscriber self, uint8_t dstMac[6])
{
  memcpy(self->dstMac, dstMac,6);
  self->dstMacSet = true;
}

void
GooseSubscriber_setAppId(GooseSubscriber self, uint16_t appId)
{
    self->appId = (int32_t) appId;
}

void
GooseSubscriber_destroy(GooseSubscriber self)
{
    MmsValue_delete(self->timestamp);

    if (self->dataSetValuesSelfAllocated)
        MmsValue_delete(self->dataSetValues);

    GLOBAL_FREEMEM(self);
}

void
GooseSubscriber_setListener(GooseSubscriber self, GooseListener listener, void* parameter)
{
    self->listener = listener;
    self->listenerParameter = parameter;
}

int32_t
GooseSubscriber_getAppId(GooseSubscriber self)
{
    return self->appId;
}

char *
GooseSubscriber_getGoId(GooseSubscriber self)
{
    return self->goId;
}

char *
GooseSubscriber_getGoCbRef(GooseSubscriber self)
{
    return self->goCBRef;
}

char *
GooseSubscriber_getDataSet(GooseSubscriber self)
{
    return self->datSet;
}

void 
GooseSubscriber_getSrcMac(GooseSubscriber self, uint8_t *buffer)
{
    memcpy(buffer, self->srcMac,6);
}

void 
GooseSubscriber_getDstMac(GooseSubscriber self, uint8_t *buffer)
{
    memcpy(buffer, self->dstMac,6);
}

uint32_t
GooseSubscriber_getStNum(GooseSubscriber self)
{
    return self->stNum;
}

uint32_t
GooseSubscriber_getSqNum(GooseSubscriber self)
{
    return self->sqNum;
}

bool
GooseSubscriber_isTest(GooseSubscriber self)
{
    return self->simulation;
}

uint32_t
GooseSubscriber_getConfRev(GooseSubscriber self)
{
    return self->confRev;
}

bool
GooseSubscriber_needsCommission(GooseSubscriber self)
{
    return self->ndsCom;
}

uint32_t
GooseSubscriber_getTimeAllowedToLive(GooseSubscriber self)
{
    return self->timeAllowedToLive;
}

uint64_t
GooseSubscriber_getTimestamp(GooseSubscriber self)
{
    return MmsValue_getUtcTimeInMs(self->timestamp);
}

MmsValue*
GooseSubscriber_getDataSetValues(GooseSubscriber self)
{
    return self->dataSetValues;
}

bool
GooseSubscriber_isVlanSet(GooseSubscriber self)
{
    return self->vlanSet;
}

uint16_t
GooseSubscriber_getVlanId(GooseSubscriber self)
{
    return self->vlanId;
}

uint8_t
GooseSubscriber_getVlanPrio(GooseSubscriber self)
{
    return self->vlanPrio;
}

void
GooseSubscriber_setObserver(GooseSubscriber self)
{
    self->isObserver = true;
}
