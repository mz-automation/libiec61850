/*
 *  mms_client_get_namelist.c
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

#include "libiec61850_platform_includes.h"
#include <MmsPdu.h>
#include "stack_config.h"
#include "mms_common.h"
#include "mms_client_connection.h"
#include "byte_buffer.h"
#include "mms_client_internal.h"

#include "ber_decode.h"

int
mmsClient_createMmsGetNameListRequestVMDspecific(long invokeId, ByteBuffer* writeBuffer,
		const char* continueAfter)
{
	MmsPdu_t* mmsPdu = mmsClient_createConfirmedRequestPdu(invokeId);

	mmsPdu->choice.confirmedRequestPdu.confirmedServiceRequest.present =
				ConfirmedServiceRequest_PR_getNameList;

	GetNameListRequest_t* request;

	request = &(mmsPdu->choice.confirmedRequestPdu.confirmedServiceRequest.choice.getNameList);

	if (continueAfter != NULL) {
		request->continueAfter = (Identifier_t*) GLOBAL_CALLOC(1, sizeof(Identifier_t));
		request->continueAfter->buf = (uint8_t*) StringUtils_copyString(continueAfter);
		request->continueAfter->size = strlen(continueAfter);
	}
	else
		request->continueAfter = NULL;

	request->objectScope.present = GetNameListRequest__objectScope_PR_vmdSpecific;
	request->objectClass.present = ObjectClass_PR_basicObjectClass;

	asn_long2INTEGER(&request->objectClass.choice.basicObjectClass,
			ObjectClass__basicObjectClass_domain);

	asn_enc_rval_t rval;

	rval = der_encode(&asn_DEF_MmsPdu, mmsPdu,
		(asn_app_consume_bytes_f*) mmsClient_write_out, (void*) writeBuffer);

	asn_DEF_MmsPdu.free_struct(&asn_DEF_MmsPdu, mmsPdu, 0);

	return rval.encoded;
}

int
mmsClient_createMmsGetNameListRequestAssociationSpecific(long invokeId, ByteBuffer* writeBuffer,
		const char* continueAfter)
{
	MmsPdu_t* mmsPdu = mmsClient_createConfirmedRequestPdu(invokeId);

	mmsPdu->choice.confirmedRequestPdu.confirmedServiceRequest.present =
				ConfirmedServiceRequest_PR_getNameList;

	GetNameListRequest_t* request;

	request = &(mmsPdu->choice.confirmedRequestPdu.confirmedServiceRequest.choice.getNameList);


	if (continueAfter != NULL) {
		request->continueAfter = (Identifier_t*) GLOBAL_CALLOC(1, sizeof(Identifier_t));
		request->continueAfter->buf = (uint8_t*) StringUtils_copyString(continueAfter);
		request->continueAfter->size = strlen(continueAfter);
	}
	else
		request->continueAfter = NULL;


	request->objectScope.present = GetNameListRequest__objectScope_PR_aaSpecific;
	request->objectClass.present = ObjectClass_PR_basicObjectClass;

	asn_long2INTEGER(&request->objectClass.choice.basicObjectClass,
				ObjectClass__basicObjectClass_namedVariableList);

	asn_enc_rval_t rval;

	rval = der_encode(&asn_DEF_MmsPdu, mmsPdu,
		(asn_app_consume_bytes_f*) mmsClient_write_out, (void*) writeBuffer);

	asn_DEF_MmsPdu.free_struct(&asn_DEF_MmsPdu, mmsPdu, 0);

	return rval.encoded;
}

bool
mmsClient_parseGetNameListResponse(LinkedList* nameList, ByteBuffer* message)
{
    /* TODO only parse get name list specific part here */

    bool moreFollows = true;

    uint8_t* buffer = message->buffer;
    int maxBufPos = message->size;
    int bufPos = 0;
    int length;

    uint8_t tag = buffer[bufPos++];
    if (tag == 0xa2) {
        /* TODO parse confirmed error PDU */
        goto exit_error;
    }
    if (tag != 0xa1) goto exit_error;

    bufPos = BerDecoder_decodeLength(buffer, &length, bufPos, maxBufPos);
    if (bufPos < 0) goto exit_error;

    /* get invokeId */
    tag = buffer[bufPos++];
    if (tag != 0x02) goto exit_error;

    bufPos = BerDecoder_decodeLength(buffer, &length, bufPos, maxBufPos);
    if (bufPos < 0) goto exit_error;

    bufPos += length;

    tag = buffer[bufPos++];
    if (tag != 0xa1) goto exit_error;

    bufPos = BerDecoder_decodeLength(buffer, &length, bufPos, maxBufPos);
    if (bufPos < 0) goto exit_error;

    tag = buffer[bufPos++];
    if (tag != 0xa0) goto exit_error;

    bufPos = BerDecoder_decodeLength(buffer, &length, bufPos, maxBufPos);
    if (bufPos < 0) goto exit_error;

    int listEndPos = bufPos + length;

    if (*nameList == NULL)
        *nameList = LinkedList_create();

    LinkedList element = LinkedList_getLastElement(*nameList);

    while (bufPos < listEndPos) {
        tag = buffer[bufPos++];
        if (tag != 0x1a) goto exit_error;

        bufPos = BerDecoder_decodeLength(buffer, &length, bufPos, maxBufPos);
        if (bufPos < 0) goto exit_error;

        char* variableName = StringUtils_createStringFromBuffer(buffer + bufPos, length);

        element = LinkedList_insertAfter(element, variableName);

        bufPos += length;
    }

    if (bufPos < maxBufPos) {
		tag = buffer[bufPos++];

		if (tag != 0x81) goto exit_error;

		bufPos = BerDecoder_decodeLength(buffer, &length, bufPos, maxBufPos);

		if (bufPos < 0) goto exit_error;

		if (length != 1) goto exit_error;

		if (buffer[bufPos++] > 0)
			moreFollows = true;
		else
			moreFollows = false;
    }

	return moreFollows;

exit_error:
    if (*nameList != NULL) {
        LinkedList_destroy(*nameList);
        *nameList = NULL;
    }

    if (DEBUG) printf("parseNameListResponse: error parsing message!\n");
    return false;
}

int
mmsClient_createGetNameListRequestDomainOrVMDSpecific(long invokeId, const char* domainName,
		ByteBuffer* writeBuffer, MmsObjectClass objectClass, const char* continueAfter)
{
	MmsPdu_t* mmsPdu = mmsClient_createConfirmedRequestPdu(invokeId);

	mmsPdu->choice.confirmedRequestPdu.confirmedServiceRequest.present =
				ConfirmedServiceRequest_PR_getNameList;

	GetNameListRequest_t* request;

	request = &(mmsPdu->choice.confirmedRequestPdu.confirmedServiceRequest.choice.getNameList);

	if (continueAfter != NULL) {
		request->continueAfter = (Identifier_t*) GLOBAL_CALLOC(1, sizeof(Identifier_t));
		request->continueAfter->buf = (uint8_t*) StringUtils_copyString(continueAfter);
		request->continueAfter->size = strlen(continueAfter);
	}
	else
		request->continueAfter = NULL;


	if (domainName != NULL) {
        request->objectScope.present = GetNameListRequest__objectScope_PR_domainSpecific;
        request->objectScope.choice.domainSpecific.buf = (uint8_t*) domainName;
        request->objectScope.choice.domainSpecific.size = strlen(domainName);
	}
	else {
	    request->objectScope.present = GetNameListRequest__objectScope_PR_vmdSpecific;
	}

	request->objectClass.present = ObjectClass_PR_basicObjectClass;

	asn_long2INTEGER(&request->objectClass.choice.basicObjectClass, objectClass);

	asn_enc_rval_t rval;

	rval = der_encode(&asn_DEF_MmsPdu, mmsPdu,
		(asn_app_consume_bytes_f*) mmsClient_write_out, (void*) writeBuffer);

	request->objectScope.choice.domainSpecific.buf = 0;
	request->objectScope.choice.domainSpecific.size = 0;

	asn_DEF_MmsPdu.free_struct(&asn_DEF_MmsPdu, mmsPdu, 0);

	return rval.encoded;
}
