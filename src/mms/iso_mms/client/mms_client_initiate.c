/*
 *  mms_client_initiate.c
 *
 *  Copyright 2013, 2014 Michael Zillgith
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
#include "mms_common_internal.h"
#include "mms_indication.h"
#include "mms_client_connection.h"
#include "byte_buffer.h"

#include "stack_config.h"

#include "mms_client_internal.h"

/* servicesSupported = {GetNameList} - required by initiate request, other services are required by some servers to work properly */
static uint8_t servicesSupported[] = { 0xee, 0x1c, 0x00, 0x00, 0x04, 0x08, 0x00, 0x00, 0x79, 0xef, 0x18 };

static InitiateRequestPdu_t
createInitiateRequestPdu(MmsConnection self)
{
    InitiateRequestPdu_t request;

    request.localDetailCalling = (Integer32_t*) GLOBAL_CALLOC(1, sizeof(Integer32_t));
    *(request.localDetailCalling) = self->parameters.maxPduSize;

    request.proposedMaxServOutstandingCalled = DEFAULT_MAX_SERV_OUTSTANDING_CALLED;
    request.proposedMaxServOutstandingCalling = DEFAULT_MAX_SERV_OUTSTANDING_CALLING;

    request.proposedDataStructureNestingLevel = (Integer8_t*) GLOBAL_CALLOC(1, sizeof(Integer8_t));
    *(request.proposedDataStructureNestingLevel) = DEFAULT_DATA_STRUCTURE_NESTING_LEVEL;

    request.mmsInitRequestDetail.proposedParameterCBB.size = 2;
    request.mmsInitRequestDetail.proposedParameterCBB.bits_unused = 5;
    request.mmsInitRequestDetail.proposedParameterCBB.buf = (uint8_t*) GLOBAL_CALLOC(2, sizeof(uint8_t));
    request.mmsInitRequestDetail.proposedParameterCBB.buf[0] = 0xf1;
    request.mmsInitRequestDetail.proposedParameterCBB.buf[1] = 0x00;

    request.mmsInitRequestDetail.proposedVersionNumber = 1;

    request.mmsInitRequestDetail.servicesSupportedCalling.size = 11;
    request.mmsInitRequestDetail.servicesSupportedCalling.bits_unused = 3;

    request.mmsInitRequestDetail.servicesSupportedCalling.buf = servicesSupported;

    return request;
}

static void
freeInitiateRequestPdu(InitiateRequestPdu_t request)
{
    GLOBAL_FREEMEM(request.localDetailCalling);
    GLOBAL_FREEMEM(request.proposedDataStructureNestingLevel);
    GLOBAL_FREEMEM(request.mmsInitRequestDetail.proposedParameterCBB.buf);
}

int
mmsClient_createInitiateRequest(MmsConnection self, ByteBuffer* message)
{
    MmsPdu_t* mmsPdu = (MmsPdu_t*) GLOBAL_CALLOC(1, sizeof(MmsPdu_t));

    mmsPdu->present = MmsPdu_PR_initiateRequestPdu;

    mmsPdu->choice.initiateRequestPdu = createInitiateRequestPdu(self);

    asn_enc_rval_t rval = der_encode(&asn_DEF_MmsPdu, mmsPdu,
            (asn_app_consume_bytes_f*) mmsClient_write_out, (void*) message);

    freeInitiateRequestPdu(mmsPdu->choice.initiateRequestPdu);
    GLOBAL_FREEMEM(mmsPdu);

    return rval.encoded;
}

int
mmsClient_createConcludeRequest(MmsConnection self, ByteBuffer* message)
{
    if (message->maxSize > 1) {
        message->buffer[0] = 0x8b;
        message->buffer[1] = 0;
        message->size = 2;
        return 2;
    }
    else
        return -1;
}

bool
mmsClient_parseInitiateResponse(MmsConnection self)
{
    bool result = false;
    MmsPdu_t* mmsPdu = 0;

    asn_dec_rval_t rval = ber_decode(NULL, &asn_DEF_MmsPdu,
            (void**) &mmsPdu, ByteBuffer_getBuffer(self->lastResponse),
            ByteBuffer_getSize(self->lastResponse));

    if (rval.code != RC_OK) goto exit_function;

    if (mmsPdu->present == MmsPdu_PR_initiateResponsePdu) {
        InitiateResponsePdu_t* initiateResponse = &(mmsPdu->choice.initiateResponsePdu);

        if (initiateResponse->localDetailCalled != NULL)
            self->parameters.maxPduSize = *(initiateResponse->localDetailCalled);

        if (initiateResponse->negotiatedDataStructureNestingLevel != NULL)
            self->parameters.dataStructureNestingLevel = *(initiateResponse->negotiatedDataStructureNestingLevel);;

        self->parameters.maxServOutstandingCalled = initiateResponse->negotiatedMaxServOutstandingCalled;

        self->parameters.maxServOutstandingCalling = initiateResponse->negotiatedMaxServOutstandingCalling;

        result = true;
    }
    else
        result = false;

    asn_DEF_MmsPdu.free_struct(&asn_DEF_MmsPdu, mmsPdu, 0);

exit_function:
    return result;
}
