/*
 *  mms_client_common.c
 *
 *  Copyright 2013 Michael Zillgith
 *
 *	This file is part of libIEC61850.
 *
 *	libIEC61850 is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	libIEC61850 is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with libIEC61850.  If not, see <http://www.gnu.org/licenses/>.
 *
 *	See COPYING file for the complete license text.
 */

#include "libiec61850_platform_includes.h"
#include <MmsPdu.h>
#include "mms_common.h"
#include "mms_client_connection.h"
#include "byte_buffer.h"

#include "mms_client_internal.h"

int
mmsClient_write_out(void *buffer, size_t size, void *app_key)
{
    ByteBuffer* writeBuffer = (ByteBuffer*) app_key;
    return ByteBuffer_append(writeBuffer, (uint8_t*) buffer, size);
}


uint32_t
mmsClient_getInvokeId(ConfirmedResponsePdu_t* confirmedResponse)
{
	long invokeId;

	asn_INTEGER2long(&confirmedResponse->invokeID, &invokeId);

	return (uint32_t) invokeId;
}


MmsPdu_t*
mmsClient_createConfirmedRequestPdu(uint32_t invokeId)
{
	MmsPdu_t* mmsPdu = (MmsPdu_t*) GLOBAL_CALLOC(1, sizeof(MmsPdu_t));
	mmsPdu->present = MmsPdu_PR_confirmedRequestPdu;

	asn_long2INTEGER(&(mmsPdu->choice.confirmedRequestPdu.invokeID), invokeId);

	return mmsPdu;
}
