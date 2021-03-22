/*
 *  mms_goose.h
 *
 *  Copyright 2013-2019 Michael Zillgith
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

#ifndef MMS_GOOSE_H_
#define MMS_GOOSE_H_

LIB61850_INTERNAL MmsGooseControlBlock
MmsGooseControlBlock_create(void);

LIB61850_INTERNAL void
MmsGooseControlBlock_destroy(MmsGooseControlBlock self);

LIB61850_INTERNAL MmsDomain*
MmsGooseControlBlock_getDomain(MmsGooseControlBlock self);

LIB61850_INTERNAL void
MmsGooseControlBlock_useGooseVlanTag(MmsGooseControlBlock self, bool useVlanTag);

LIB61850_INTERNAL void
MmsGooseControlBlock_setGooseInterfaceId(MmsGooseControlBlock self, const char* interfaceId);

LIB61850_INTERNAL char*
MmsGooseControlBlock_getLogicalNodeName(MmsGooseControlBlock self);

LIB61850_INTERNAL MmsValue*
MmsGooseControlBlock_getGCBValue(MmsGooseControlBlock self, char* elementName);

LIB61850_INTERNAL MmsValue*
MmsGooseControlBlock_getMmsValues(MmsGooseControlBlock self);

LIB61850_INTERNAL MmsVariableSpecification*
MmsGooseControlBlock_getVariableSpecification(MmsGooseControlBlock self);

LIB61850_INTERNAL bool
MmsGooseControlBlock_isEnabled(MmsGooseControlBlock self);

LIB61850_INTERNAL void
MmsGooseControlBlock_checkAndPublish(MmsGooseControlBlock self, uint64_t currentTime, MmsMapping* mapping);

LIB61850_INTERNAL void
MmsGooseControlBlock_setStateChangePending(MmsGooseControlBlock self);

LIB61850_INTERNAL void
MmsGooseControlBlock_publishNewState(MmsGooseControlBlock self);

LIB61850_INTERNAL bool
MmsGooseControlBlock_enable(MmsGooseControlBlock self, MmsMapping* mmsMapping);

LIB61850_INTERNAL void
MmsGooseControlBlock_disable(MmsGooseControlBlock self, MmsMapping* mmsMapping);

LIB61850_INTERNAL void
GOOSE_sendPendingEvents(MmsMapping* self);

LIB61850_INTERNAL MmsVariableSpecification*
GOOSE_createGOOSEControlBlocks(MmsMapping* self, MmsDomain* domain,
        LogicalNode* logicalNode, int gseCount);

#endif /* MMS_GOOSE_H_ */
