/*
 *  mms_sv.h
 *
 *  Copyright 2015 Michael Zillgith
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

#ifndef LIBIEC61850_SRC_IEC61850_INC_PRIVATE_MMS_SV_H_
#define LIBIEC61850_SRC_IEC61850_INC_PRIVATE_MMS_SV_H_


typedef struct sMmsSampledValueControlBlock* MmsSampledValueControlBlock;

LIB61850_INTERNAL MmsSampledValueControlBlock
MmsSampledValueControlBlock_create(void);

LIB61850_INTERNAL void
MmsSampledValueControlBlock_destroy(MmsSampledValueControlBlock self);

LIB61850_INTERNAL MmsVariableSpecification*
LIBIEC61850_SV_createSVControlBlocks(MmsMapping* self, MmsDomain* domain,
        LogicalNode* logicalNode, int svCount, bool unicast);

LIB61850_INTERNAL MmsValue*
LIBIEC61850_SV_readAccessSampledValueControlBlock(MmsMapping* self, MmsDomain* domain, char* variableIdOrig);

LIB61850_INTERNAL MmsDataAccessError
LIBIEC61850_SV_writeAccessSVControlBlock(MmsMapping* self, MmsDomain* domain, char* variableIdOrig,
        MmsValue* value, MmsServerConnection connection);

LIB61850_INTERNAL void
LIBIEC61850_SV_setSVCBHandler(MmsMapping* self, SVControlBlock* svcb, SVCBEventHandler handler, void* parameter);

#endif /* LIBIEC61850_SRC_IEC61850_INC_PRIVATE_MMS_SV_H_ */
