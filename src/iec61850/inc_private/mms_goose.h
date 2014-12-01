/*
 *  mms_goose.h
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

#ifndef MMS_GOOSE_H_
#define MMS_GOOSE_H_

typedef struct sMmsGooseControlBlock* MmsGooseControlBlock;

MmsGooseControlBlock
MmsGooseControlBlock_create(void);

void
MmsGooseControlBlock_destroy(MmsGooseControlBlock self);

MmsDomain*
MmsGooseControlBlock_getDomain(MmsGooseControlBlock self);

char*
MmsGooseControlBlock_getLogicalNodeName(MmsGooseControlBlock self);

char*
MmsGooseControlBlock_getName(MmsGooseControlBlock self);

MmsValue*
MmsGooseControlBlock_getGCBValue(MmsGooseControlBlock self, char* elementName);

MmsValue*
MmsGooseControlBlock_getMmsValues(MmsGooseControlBlock self);

MmsVariableSpecification*
MmsGooseControlBlock_getVariableSpecification(MmsGooseControlBlock self);

DataSet*
MmsGooseControlBlock_getDataSet(MmsGooseControlBlock self);

bool
MmsGooseControlBlock_isEnabled(MmsGooseControlBlock self);

void
MmsGooseControlBlock_checkAndPublish(MmsGooseControlBlock self, uint64_t currentTime);

void
MmsGooseControlBlock_observedObjectChanged(MmsGooseControlBlock self);

void
MmsGooseControlBlock_enable(MmsGooseControlBlock self);

void
MmsGooseControlBlock_disable(MmsGooseControlBlock self);

MmsVariableSpecification*
GOOSE_createGOOSEControlBlocks(MmsMapping* self, MmsDomain* domain,
        LogicalNode* logicalNode, int gseCount);

#endif /* MMS_GOOSE_H_ */
