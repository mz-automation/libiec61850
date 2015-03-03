/*
 *  mms_write_service.c
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
#include "mms_server_internal.h"
#include "mms_common_internal.h"
#include "mms_types.h"

#if (MMS_WRITE_SERVICE == 1)

#define CONFIG_MMS_WRITE_SERVICE_MAX_NUMBER_OF_WRITE_ITEMS 100

/**********************************************************************************************
 * MMS Write Service
 *********************************************************************************************/

int
mmsServer_createMmsWriteResponse(MmsServerConnection connection,
		int invokeId, ByteBuffer* response, int numberOfItems, MmsDataAccessError* accessResults)
{
	MmsPdu_t* mmsPdu = mmsServer_createConfirmedResponse(invokeId);

	mmsPdu->choice.confirmedResponsePdu.confirmedServiceResponse.present =
			ConfirmedServiceResponse_PR_write;

	WriteResponse_t* writeResponse =
			&(mmsPdu->choice.confirmedResponsePdu.confirmedServiceResponse.choice.write);

	writeResponse->list.count = numberOfItems;
	writeResponse->list.size = numberOfItems;
	writeResponse->list.array = (struct WriteResponse__Member**) GLOBAL_CALLOC(numberOfItems,
	        sizeof(struct WriteResponse__Member*));

	int i;

	for (i = 0; i < numberOfItems; i++) {
	    writeResponse->list.array[i] =  (struct WriteResponse__Member*) GLOBAL_CALLOC(1, sizeof(struct WriteResponse__Member));

	    if (accessResults[i] == DATA_ACCESS_ERROR_SUCCESS)
	        writeResponse->list.array[i]->present = WriteResponse__Member_PR_success;
	    else {
	        writeResponse->list.array[i]->present = WriteResponse__Member_PR_failure;
	        asn_long2INTEGER(&writeResponse->list.array[i]->choice.failure, (long) accessResults[i]);
	    }
	}

	der_encode(&asn_DEF_MmsPdu, mmsPdu, mmsServer_write_out, (void*) response);

	asn_DEF_MmsPdu.free_struct(&asn_DEF_MmsPdu, mmsPdu, 0);

 	return 0;
}


void
MmsServerConnection_sendWriteResponse(MmsServerConnection self, uint32_t invokeId, MmsDataAccessError indication, bool handlerMode)
{
    ByteBuffer* response = ByteBuffer_create(NULL, self->maxPduSize);

    mmsServer_createMmsWriteResponse(self, invokeId, response, 1, &indication);

    IsoConnection_sendMessage(self->isoConnection, response, handlerMode);

    ByteBuffer_destroy(response);
}

void
mmsServer_handleWriteRequest(
		MmsServerConnection connection,
		uint8_t* buffer, int bufPos, int maxBufPos,
		uint32_t invokeId,
		ByteBuffer* response)
{
	WriteRequest_t* writeRequest = 0;

	MmsPdu_t* mmsPdu = 0;

	asn_dec_rval_t rval; /* Decoder return value  */

	rval = ber_decode(NULL, &asn_DEF_MmsPdu, (void**) &mmsPdu, buffer, CONFIG_MMS_MAXIMUM_PDU_SIZE);

	if (rval.code != RC_OK) {
	    mmsServer_writeMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_INVALID_PDU, response);
	    return;
	}

	writeRequest = &(mmsPdu->choice.confirmedRequestPdu.confirmedServiceRequest.choice.write);

	int numberOfWriteItems = writeRequest->variableAccessSpecification.choice.listOfVariable.list.count;

	if (numberOfWriteItems < 1) {
        mmsServer_writeMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_REQUEST_INVALID_ARGUMENT, response);
        return;
	}

	if (numberOfWriteItems > CONFIG_MMS_WRITE_SERVICE_MAX_NUMBER_OF_WRITE_ITEMS) {
	    mmsServer_writeMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_OTHER, response);
	    return;
	}

    if (writeRequest->listOfData.list.count != numberOfWriteItems) {
        mmsServer_writeMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_REQUEST_INVALID_ARGUMENT, response);
        return;
    }

    MmsDataAccessError accessResults[CONFIG_MMS_WRITE_SERVICE_MAX_NUMBER_OF_WRITE_ITEMS * sizeof(MmsDataAccessError)];

	bool sendResponse = true;

	int i;

	for (i = 0; i < numberOfWriteItems; i++) {
	    ListOfVariableSeq_t* varSpec =
                writeRequest->variableAccessSpecification.choice.listOfVariable.list.array[i];

        if (varSpec->variableSpecification.present != VariableSpecification_PR_name) {
            accessResults[i] = DATA_ACCESS_ERROR_OBJECT_ACCESS_UNSUPPORTED;
            continue;
        }

        MmsVariableSpecification* variable;

        MmsDevice* device = MmsServer_getDevice(connection->server);

        MmsDomain* domain = NULL;

        char* nameIdStr;

        if (varSpec->variableSpecification.choice.name.present == ObjectName_PR_domainspecific) {
            Identifier_t domainId = varSpec->variableSpecification.choice.name.choice.domainspecific.domainId;
            char* domainIdStr = createStringFromBuffer(domainId.buf, domainId.size);

            domain = MmsDevice_getDomain(device, domainIdStr);

            GLOBAL_FREEMEM(domainIdStr);

            if (domain == NULL) {
                accessResults[i] = DATA_ACCESS_ERROR_OBJECT_NONE_EXISTENT;
                continue;
            }

            Identifier_t nameId = varSpec->variableSpecification.choice.name.choice.domainspecific.itemId;
            nameIdStr = createStringFromBuffer(nameId.buf, nameId.size);

            variable = MmsDomain_getNamedVariable(domain, nameIdStr);
        }

#if (CONFIG_MMS_SUPPORT_VMD_SCOPE_NAMED_VARIABLES == 1)
        else if (varSpec->variableSpecification.choice.name.present == ObjectName_PR_vmdspecific) {

            Identifier_t nameId = varSpec->variableSpecification.choice.name.choice.vmdspecific;
            nameIdStr = createStringFromBuffer(nameId.buf, nameId.size);

            variable = MmsDevice_getNamedVariable(device, nameIdStr);
        }
#endif /* (CONFIG_MMS_SUPPORT_VMD_SCOPE_NAMED_VARIABLES == 1) */

        else {
            accessResults[i] = DATA_ACCESS_ERROR_OBJECT_ACCESS_UNSUPPORTED;
            continue;
        }

        if (variable == NULL) {
            GLOBAL_FREEMEM(nameIdStr);
            accessResults[i] = DATA_ACCESS_ERROR_OBJECT_NONE_EXISTENT;
            continue;
        }

        AlternateAccess_t* alternateAccess = varSpec->alternateAccess;

        if (alternateAccess != NULL) {
            if (variable->type != MMS_ARRAY) {
                GLOBAL_FREEMEM(nameIdStr);
                accessResults[i] = DATA_ACCESS_ERROR_OBJECT_ATTRIBUTE_INCONSISTENT;
                continue;
            }

            if (!mmsServer_isIndexAccess(alternateAccess)) {
                GLOBAL_FREEMEM(nameIdStr);
                accessResults[i] = DATA_ACCESS_ERROR_OBJECT_ACCESS_UNSUPPORTED;
                continue;
            }
        }

        Data_t* dataElement = writeRequest->listOfData.list.array[i];

        MmsValue* value = mmsMsg_parseDataElement(dataElement);

        if (value == NULL) {
            GLOBAL_FREEMEM(nameIdStr);
            accessResults[i] = DATA_ACCESS_ERROR_OBJECT_ATTRIBUTE_INCONSISTENT;
            continue;
        }

        if (alternateAccess != NULL) {

            if (domain != NULL)
                domain = (MmsDomain*) device;

            MmsValue* cachedArray = MmsServer_getValueFromCache(connection->server, domain, nameIdStr);

            if (cachedArray == NULL) {
                GLOBAL_FREEMEM(nameIdStr);
                MmsValue_delete(value);
                accessResults[i] = DATA_ACCESS_ERROR_OBJECT_ATTRIBUTE_INCONSISTENT;
                continue;
            }

            int index = mmsServer_getLowIndex(alternateAccess);

            MmsValue* elementValue = MmsValue_getElement(cachedArray, index);

            if (elementValue == NULL) {
                GLOBAL_FREEMEM(nameIdStr);
                MmsValue_delete(value);
                accessResults[i] = DATA_ACCESS_ERROR_OBJECT_ATTRIBUTE_INCONSISTENT;
                continue;
            }

            if (MmsValue_update(elementValue, value) == false) {
                GLOBAL_FREEMEM(nameIdStr);
                MmsValue_delete(value);
                accessResults[i] = DATA_ACCESS_ERROR_TYPE_INCONSISTENT;
                continue;
            }

            GLOBAL_FREEMEM(nameIdStr);
            MmsValue_delete(value);
            accessResults[i] = DATA_ACCESS_ERROR_SUCCESS;
            continue;

        }

        MmsDataAccessError valueIndication =
                mmsServer_setValue(connection->server, domain, nameIdStr, value, connection);

        if (valueIndication == DATA_ACCESS_ERROR_NO_RESPONSE)
            sendResponse = false;

        accessResults[i] = valueIndication;

        MmsValue_delete(value);

        GLOBAL_FREEMEM(nameIdStr);
	}

	if (sendResponse) {
	    mmsServer_createMmsWriteResponse(connection, invokeId, response, numberOfWriteItems, accessResults);
	}

	asn_DEF_MmsPdu.free_struct(&asn_DEF_MmsPdu, mmsPdu, 0);
}

#endif /* (MMS_WRITE_SERVICE == 1) */
