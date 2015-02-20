/*
 *  mms_common_internal.h
 *
 *  Copyright 2013, 2014, 2015 Michael Zillgith
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

#ifndef MMS_COMMON_INTERNAL_H_
#define MMS_COMMON_INTERNAL_H_

#include "mms_value.h"
#include "MmsPdu.h"
#include "mms_access_result.h"
#include "conversions.h"




MmsValue*
mmsMsg_parseDataElement(Data_t* dataElement);

void
mmsMsg_createFloatData(MmsValue* value, int* size,  uint8_t** buf);

Data_t*
mmsMsg_createBasicDataElement(MmsValue* value);

Data_t*
mmsMsg_createDataElement(MmsValue* value);

void
mmsMsg_addResultToResultList(AccessResult_t* accessResult, MmsValue* value);

AccessResult_t**
mmsMsg_createAccessResultsList(MmsPdu_t* mmsPdu, int resultsCount);

char*
mmsMsg_createStringFromAsnIdentifier(Identifier_t identifier);

void
mmsMsg_copyAsn1IdentifierToStringBuffer(Identifier_t identifier, char* buffer, int bufSize);

void
mmsMsg_deleteAccessResultList(AccessResult_t** accessResult, int variableCount);

#endif /* MMS_COMMON_INTERNAL */

