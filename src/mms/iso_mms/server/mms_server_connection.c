/*
 *  mms_server_connection.c
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
 *
 *
 *  MMS client connection handling code for libiec61850.
 *
 *  Handles a MMS client connection.
 */

#include "libiec61850_platform_includes.h"
#include "mms_server_internal.h"
#include "iso_server.h"
#include "ber_encoder.h"
#include "ber_decode.h"

/**********************************************************************************************
 * MMS Common support functions
 *********************************************************************************************/

void
mmsServer_writeMmsRejectPdu(uint32_t* invokeId, int reason, ByteBuffer* response) {
	MmsPdu_t* mmsPdu = (MmsPdu_t*) GLOBAL_CALLOC(1, sizeof(MmsPdu_t));

	mmsPdu->present = MmsPdu_PR_rejectPDU;

	if (invokeId != NULL) {
		mmsPdu->choice.rejectPDU.originalInvokeID = (Unsigned32_t*) GLOBAL_CALLOC(1, sizeof(Unsigned32_t));
		asn_long2INTEGER(mmsPdu->choice.rejectPDU.originalInvokeID, *invokeId);
	}

	if (reason == MMS_ERROR_REJECT_UNRECOGNIZED_SERVICE) {
		mmsPdu->choice.rejectPDU.rejectReason.present = RejectPDU__rejectReason_PR_confirmedRequestPDU;
		mmsPdu->choice.rejectPDU.rejectReason.choice.confirmedResponsePDU =
			RejectPDU__rejectReason__confirmedRequestPDU_unrecognizedService;
	}
	else if(reason == MMS_ERROR_REJECT_UNKNOWN_PDU_TYPE) {
		mmsPdu->choice.rejectPDU.rejectReason.present = RejectPDU__rejectReason_PR_pduError;
		asn_long2INTEGER(&mmsPdu->choice.rejectPDU.rejectReason.choice.pduError,
				RejectPDU__rejectReason__pduError_unknownPduType);
	}
	else if (reason == MMS_ERROR_REJECT_REQUEST_INVALID_ARGUMENT) {
	    mmsPdu->choice.rejectPDU.rejectReason.present = RejectPDU__rejectReason_PR_confirmedRequestPDU;
	    mmsPdu->choice.rejectPDU.rejectReason.choice.confirmedResponsePDU =
	                RejectPDU__rejectReason__confirmedRequestPDU_invalidArgument;
	}
	else if (reason == MMS_ERROR_REJECT_INVALID_PDU) {
		mmsPdu->choice.rejectPDU.rejectReason.present = RejectPDU__rejectReason_PR_pduError;
		asn_long2INTEGER(&mmsPdu->choice.rejectPDU.rejectReason.choice.pduError,
		                RejectPDU__rejectReason__pduError_invalidPdu);
	}
	else {
		mmsPdu->choice.rejectPDU.rejectReason.present = RejectPDU__rejectReason_PR_confirmedRequestPDU;
		mmsPdu->choice.rejectPDU.rejectReason.choice.confirmedResponsePDU =
			RejectPDU__rejectReason__confirmedRequestPDU_other;
	}

	der_encode(&asn_DEF_MmsPdu, mmsPdu,	mmsServer_write_out, (void*) response);

	asn_DEF_MmsPdu.free_struct(&asn_DEF_MmsPdu, mmsPdu, 0);
}

/**********************************************************************************************
 * MMS General service handling functions
 *********************************************************************************************/

static void
handleConfirmedRequestPdu(
		MmsServerConnection self,
		uint8_t* buffer, int bufPos, int maxBufPos,
		ByteBuffer* response)
{
	uint32_t invokeId = 0;

	while (bufPos < maxBufPos) {
		uint8_t tag = buffer[bufPos++];
		int length;

		bool extendedTag = false;

		if ((tag & 0x1f) == 0x1f) {
		    extendedTag = true;
		    tag = buffer[bufPos++];
		}

		bufPos = BerDecoder_decodeLength(buffer, &length, bufPos, maxBufPos);

		if (bufPos < 0)  {
			mmsServer_writeMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_UNRECOGNIZED_SERVICE, response);
			return;
		}

		if (DEBUG_MMS_SERVER) printf("tag %02x extended tag: %i size: %i\n", tag, extendedTag, length);

		if (extendedTag) {
		    switch(tag) {

#if (MMS_JOURNAL_SERVICE == 1)
            case 0x41: /* read-journal */
                mmsServer_handleReadJournalRequest(self, buffer, bufPos, bufPos + length, invokeId, response);
                break;
#endif /* (MMS_JOURNAL_SERVICE == 1) */

#if (MMS_FILE_SERVICE == 1)
		    case 0x48: /* file-open-request */
		        mmsServer_handleFileOpenRequest(self, buffer, bufPos, bufPos + length, invokeId, response);
		        break;

		    case 0x49: /* file-read-request */
		        mmsServer_handleFileReadRequest(self, buffer, bufPos, bufPos + length, invokeId, response);
		        break;

		    case 0x4a: /* file-close-request */
		        mmsServer_handleFileCloseRequest(self, buffer, bufPos, bufPos + length, invokeId, response);
		        break;

		    case 0x4b: /* file-rename-request */
                mmsServer_handleFileRenameRequest(self, buffer, bufPos, bufPos + length, invokeId, response);
                break;

		    case 0x4c: /* file-delete-request */
		        mmsServer_handleFileDeleteRequest(self, buffer, bufPos, bufPos + length, invokeId, response);
		        break;

		    case 0x4d: /* file-directory-request */
		        mmsServer_handleFileDirectoryRequest(self, buffer, bufPos, bufPos + length, invokeId, response);
		        break;
#endif /* MMS_FILE_SERVICE == 1 */

            default:
                mmsServer_writeMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_UNRECOGNIZED_SERVICE, response);
                return;
                break;
		    }
		}
		else {
            switch(tag) {
            case 0x02: /* invoke Id */
                invokeId = BerDecoder_decodeUint32(buffer, length, bufPos);
                if (DEBUG_MMS_SERVER) printf("invokeId: %i\n", invokeId);
                self->lastInvokeId = invokeId;
                break;

#if (MMS_STATUS_SERVICE == 1)
            case 0x80: /* status-request */
                mmsServer_handleStatusRequest(self, buffer, bufPos, invokeId, response);
                break;
#endif /* MMS_STATUS_SERVICE == 1 */

#if (MMS_GET_NAME_LIST == 1)
            case 0xa1: /* get-name-list-request */
                mmsServer_handleGetNameListRequest(self, buffer, bufPos, bufPos + length,
                        invokeId, response);
                break;
#endif /* (MMS_GET_NAME_LIST == 1) */

#if (MMS_IDENTIFY_SERVICE == 1)
            case 0x82: /* identify */
                mmsServer_handleIdentifyRequest(self, invokeId, response);
                break;
#endif /* MMS_IDENTIFY_SERVICE == 1 */

            case 0xa4: /* read-request */
                mmsServer_handleReadRequest(self, buffer, bufPos, bufPos + length,
                        invokeId, response);
                break;

#if (MMS_WRITE_SERVICE == 1)
            case 0xa5: /* write-request */
                mmsServer_handleWriteRequest(self, buffer, bufPos, bufPos + length,
                                invokeId, response);
                break;
#endif /* (MMS_WRITE_SERVICE == 1) */

#if (MMS_GET_VARIABLE_ACCESS_ATTRIBUTES == 1)
            case 0xa6: /* get-variable-access-attributes-request */
                mmsServer_handleGetVariableAccessAttributesRequest(self,
                        buffer, bufPos, bufPos + length,
                        invokeId, response);
                break;
#endif /* MMS_GET_VARIABLE_ACCESS_ATTRIBUTES == 1 */


#if (MMS_DYNAMIC_DATA_SETS == 1)
            case 0xab: /* define-named-variable-list */
                mmsServer_handleDefineNamedVariableListRequest(self,
                        buffer, bufPos, bufPos + length,
                        invokeId, response);
                break;
#endif /* (MMS_DYNAMIC_DATA_SETS == 1) */


#if (MMS_GET_DATA_SET_ATTRIBUTES == 1)
            case 0xac: /* get-named-variable-list-attributes-request */
                mmsServer_handleGetNamedVariableListAttributesRequest(self,
                        buffer, bufPos, bufPos + length,
                        invokeId, response);
                break;
#endif /* (MMS_GET_DATA_SET_ATTRIBUTES == 1) */

#if (MMS_DYNAMIC_DATA_SETS == 1)
            case 0xad: /* delete-named-variable-list-request */
                mmsServer_handleDeleteNamedVariableListRequest(self,
                        buffer, bufPos, bufPos + length,
                        invokeId, response);
                break;
#endif /* (MMS_DYNAMIC_DATA_SETS == 1) */

            default:
                mmsServer_writeMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_UNRECOGNIZED_SERVICE, response);
                return;
                break;
            }
		}

		bufPos += length;
	}
}

MmsIndication
MmsServerConnection_parseMessage(MmsServerConnection self, ByteBuffer* message, ByteBuffer* response)
{
	MmsIndication retVal;

	uint8_t* buffer = message->buffer;

	if (message->size < 2)
		return MMS_ERROR;

	int bufPos = 0;

	uint8_t pduType = buffer[bufPos++];
	int pduLength;

	bufPos = BerDecoder_decodeLength(buffer, &pduLength, bufPos, message->size);

	if (bufPos < 0)
		return MMS_ERROR;

	if (DEBUG_MMS_SERVER)
	    printf("mms_server: recvd MMS-PDU type: %02x size: %i\n", pduType, pduLength);

	switch (pduType) {
	case 0xa8: /* Initiate request PDU */
		mmsServer_handleInitiateRequest(self, buffer, bufPos, bufPos + pduLength, response);
		retVal = MMS_INITIATE;
		break;
	case 0xa0: /* Confirmed request PDU */
		handleConfirmedRequestPdu(self, buffer, bufPos, bufPos + pduLength, response);
		retVal = MMS_CONFIRMED_REQUEST;
		break;
	case 0x8b: /* Conclude request PDU */
		mmsServer_writeConcludeResponsePdu(response);
		retVal = MMS_CONCLUDE;
		break;
	case 0xa4: /* Reject PDU - silently ignore */
	    if (DEBUG) printf("received reject PDU!\n");
		retVal = MMS_OK;
		break;
	default:
		mmsServer_writeMmsRejectPdu(NULL, MMS_ERROR_REJECT_UNKNOWN_PDU_TYPE, response);
		retVal = MMS_ERROR;
		break;
	}

	return retVal;
}

static void /* will be called by IsoConnection */
messageReceived(void* parameter, ByteBuffer* message, ByteBuffer* response)
{
	MmsServerConnection self = (MmsServerConnection) parameter;

	MmsServerConnection_parseMessage(self,  message, response);
}

/**********************************************************************************************
 * MMS server connection public API functions
 *********************************************************************************************/

MmsServerConnection
MmsServerConnection_init(MmsServerConnection connection, MmsServer server, IsoConnection isoCon)
{
	MmsServerConnection self;

	if (connection == NULL)
		self = (MmsServerConnection) GLOBAL_CALLOC(1, sizeof(struct sMmsServerConnection));
	else
		self = connection;

	self->maxServOutstandingCalled = 0;
	self->maxServOutstandingCalling = 0;
	self->maxPduSize = CONFIG_MMS_MAXIMUM_PDU_SIZE;
	self->dataStructureNestingLevel = 0;
	self->server = server;
	self->isoConnection = isoCon;

#if (MMS_DYNAMIC_DATA_SETS == 1)
	self->namedVariableLists = LinkedList_create();
#endif

	IsoConnection_installListener(isoCon, messageReceived, (void*) self);

	return self;
}

void
MmsServerConnection_destroy(MmsServerConnection self)
{

#if (MMS_FILE_SERVICE == 1)
    int frsmIndex = 0;

    for (frsmIndex = 0; frsmIndex < CONFIG_MMS_MAX_NUMBER_OF_OPEN_FILES_PER_CONNECTION; frsmIndex++)
        if (self->frsms[frsmIndex].fileHandle != NULL)
            FileSystem_closeFile(self->frsms[frsmIndex].fileHandle);
#endif

#if (MMS_DYNAMIC_DATA_SETS == 1)
	LinkedList_destroyDeep(self->namedVariableLists, (LinkedListValueDeleteFunction) MmsNamedVariableList_destroy);
#endif

	GLOBAL_FREEMEM(self);
}

#if (MMS_DYNAMIC_DATA_SETS == 1)
bool
MmsServerConnection_addNamedVariableList(MmsServerConnection self, MmsNamedVariableList variableList)
{
	//TODO check if operation is allowed!

	LinkedList_add(self->namedVariableLists, variableList);

	return true;
}

void
MmsServerConnection_deleteNamedVariableList(MmsServerConnection self, char* listName)
{
	mmsServer_deleteVariableList(self->namedVariableLists, listName);
}

MmsNamedVariableList
MmsServerConnection_getNamedVariableList(MmsServerConnection self, const char* variableListName)
{
    return mmsServer_getNamedVariableListWithName(self->namedVariableLists, variableListName);
}
#endif /* (MMS_DYNAMIC_DATA_SETS == 1) */


char*
MmsServerConnection_getClientAddress(MmsServerConnection self)
{
	return IsoConnection_getPeerAddress(self->isoConnection);
}

IsoConnection
MmsServerConnection_getIsoConnection(MmsServerConnection self)
{
    return self->isoConnection;
}

#if (MMS_DYNAMIC_DATA_SETS == 1)
LinkedList
MmsServerConnection_getNamedVariableLists(MmsServerConnection self)
{
	return self->namedVariableLists;
}
#endif /* (MMS_DYNAMIC_DATA_SETS == 1) */

uint32_t
MmsServerConnection_getLastInvokeId(MmsServerConnection self)
{
    return self->lastInvokeId;
}
