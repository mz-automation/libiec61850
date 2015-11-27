/*
 *  mms_client_read.c
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

#include "libiec61850_platform_includes.h"
#include <MmsPdu.h>
#include "mms_common.h"
#include "mms_client_connection.h"
#include "byte_buffer.h"
#include "stack_config.h"

#include "mms_client_internal.h"
#include "mms_common_internal.h"
#include "mms_value_internal.h"

MmsValue*
mmsClient_parseListOfAccessResults(AccessResult_t** accessResultList, int listSize, bool createArray)
{
    MmsValue* valueList = NULL;
    MmsValue* value = NULL;

    int elementCount = listSize;

    if ((elementCount > 1) || createArray)
        valueList = MmsValue_createEmptyArray(elementCount);

    int i = 0;

    for (i = 0; i < elementCount; i++) {
        AccessResult_PR presentType = accessResultList[i]->present;

        if (presentType == AccessResult_PR_failure) {
            if (DEBUG_MMS_CLIENT) printf("access error!\n");

            if (accessResultList[i]->choice.failure.size > 0) {
                int errorCode = (int) accessResultList[i]->choice.failure.buf[0];

                MmsDataAccessError dataAccessError = DATA_ACCESS_ERROR_UNKNOWN;

                if ((errorCode >= 0) && (errorCode < 12))
                    dataAccessError = (MmsDataAccessError) errorCode;

                value = MmsValue_newDataAccessError(dataAccessError);
            }
            else
                value = MmsValue_newDataAccessError(DATA_ACCESS_ERROR_UNKNOWN);
        }
        else if (presentType == AccessResult_PR_array) {
            value = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));
            value->type = MMS_ARRAY;

            int arrayElementCount =
                    accessResultList[i]->choice.array.list.count;

            value->value.structure.size = arrayElementCount;
            value->value.structure.components = (MmsValue**) GLOBAL_CALLOC(arrayElementCount, sizeof(MmsValue*));

            int j;

            for (j = 0; j < arrayElementCount; j++) {
                value->value.structure.components[j] = mmsMsg_parseDataElement(
                        accessResultList[i]->choice.array.list.array[j]);
            }
        }
        else if (presentType == AccessResult_PR_structure) {
            value = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));
            value->type = MMS_STRUCTURE;

            int componentCount =
                    accessResultList[i]->choice.structure.list.count;

            value->value.structure.size = componentCount;
            value->value.structure.components = (MmsValue**) GLOBAL_CALLOC(componentCount, sizeof(MmsValue*));

            int j;
            for (j = 0; j < componentCount; j++) {
                value->value.structure.components[j] = mmsMsg_parseDataElement(
                        accessResultList[i]->choice.structure.list.array[j]);
            }
        }
        else if (presentType == AccessResult_PR_bitstring) {
            value = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));
            value->type = MMS_BIT_STRING;
            int size = accessResultList[i]->choice.bitstring.size;

            value->value.bitString.size = (size * 8)
               - accessResultList[i]->choice.bitstring.bits_unused;

            value->value.bitString.buf = (uint8_t*) GLOBAL_MALLOC(size);
            memcpy(value->value.bitString.buf,
                    accessResultList[i]->choice.bitstring.buf, size);

        }
        else if (presentType == AccessResult_PR_integer) {
            Asn1PrimitiveValue* berInteger =
                    BerInteger_createFromBuffer(accessResultList[i]->choice.integer.buf,
                                                accessResultList[i]->choice.integer.size);

            value = MmsValue_newIntegerFromBerInteger(berInteger);
        }
        else if (presentType == AccessResult_PR_unsigned) {
            Asn1PrimitiveValue* berInteger =
                    BerInteger_createFromBuffer(accessResultList[i]->choice.Unsigned.buf,
                                                                    accessResultList[i]->choice.Unsigned.size);

            value = MmsValue_newUnsignedFromBerInteger(berInteger);
        }
        else if (presentType == AccessResult_PR_floatingpoint) {
            int size = accessResultList[i]->choice.floatingpoint.size;

            value = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));
            value->type = MMS_FLOAT;

            if (size == 5) { /* FLOAT32 */
                value->value.floatingPoint.formatWidth = 32;
                value->value.floatingPoint.exponentWidth = accessResultList[i]->choice.floatingpoint.buf[0];

                uint8_t* floatBuf = (accessResultList[i]->choice.floatingpoint.buf + 1);

                value->value.floatingPoint.buf = (uint8_t*) GLOBAL_MALLOC(4);

#if (ORDER_LITTLE_ENDIAN == 1)
                    memcpyReverseByteOrder(value->value.floatingPoint.buf, floatBuf, 4);
#else
                    memcpy(value->value.floatingPoint.buf, floatBuf, 4);
#endif

            }

            if (size == 9) { /* FLOAT64 */
                value->value.floatingPoint.formatWidth = 64;
                value->value.floatingPoint.exponentWidth = accessResultList[i]->choice.floatingpoint.buf[0];

                uint8_t* floatBuf = (accessResultList[i]->choice.floatingpoint.buf + 1);

                value->value.floatingPoint.buf = (uint8_t*) GLOBAL_MALLOC(8);

#if (ORDER_LITTLE_ENDIAN == 1)
                memcpyReverseByteOrder(value->value.floatingPoint.buf, floatBuf, 8);
#else
                memcpy(value->value.floatingPoint.buf, floatBuf, 8);
#endif
            }

        }
        else if (presentType == AccessResult_PR_visiblestring) {
            value = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));

            value->type = MMS_VISIBLE_STRING;

            int strSize = accessResultList[i]->choice.visiblestring.size;

            value->value.visibleString.buf = (char*) GLOBAL_MALLOC(strSize + 1);
            value->value.visibleString.size = strSize;

            memcpy(value->value.visibleString.buf,
                    accessResultList[i]->choice.visiblestring.buf,
                    strSize);

            value->value.visibleString.buf[strSize] = 0;
        }
        else if (presentType == AccessResult_PR_mMSString) {
        	value = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));

        	value->type = MMS_STRING;

        	int strSize = accessResultList[i]->choice.mMSString.size;

        	value->value.visibleString.buf = (char*) GLOBAL_MALLOC(strSize + 1);
        	value->value.visibleString.size = strSize;

        	memcpy(value->value.visibleString.buf,
        			accessResultList[i]->choice.mMSString.buf, strSize);

        	value->value.visibleString.buf[strSize] = 0;

        }
        else if (presentType == AccessResult_PR_utctime) {
            value = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));

            value->type = MMS_UTC_TIME;
            memcpy(value->value.utcTime,
                    accessResultList[i]->choice.utctime.buf, 8);
        }
        else if (presentType == AccessResult_PR_boolean) {
            value = MmsValue_newBoolean(accessResultList[i]->choice.boolean);
        }
        else if (presentType == AccessResult_PR_binarytime) {
            int size = accessResultList[i]->choice.binarytime.size;

            if (size <= 6) {
                value = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));
                value->type = MMS_BINARY_TIME;
                value->value.binaryTime.size = size;
                memcpy(value->value.binaryTime.buf, accessResultList[i]->choice.binarytime.buf, size);
            }
        }
        else if (presentType == AccessResult_PR_octetstring) {
        	int size = accessResultList[i]->choice.octetstring.size;

        	value = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));
        	value->type = MMS_OCTET_STRING;
        	value->value.octetString.maxSize = size;
        	value->value.octetString.size = size;
        	value->value.octetString.buf = (uint8_t*) GLOBAL_MALLOC(size);
        	memcpy(value->value.octetString.buf, accessResultList[i]->choice.octetstring.buf, size);
        }
        else {
            printf("unknown type %i\n", presentType);
            value = MmsValue_newDataAccessError(DATA_ACCESS_ERROR_OBJECT_VALUE_INVALID);
        }

        if ((elementCount > 1) || createArray)
            MmsValue_setElement(valueList, i, value);
    }

    if (valueList == NULL)
        valueList = value;

    return valueList;
}


/*
 * \param createArray if multiple variables should be read (e.g. if a data set is read) an array should
 *                    be created that contains the access results.
 */
MmsValue*
mmsClient_parseReadResponse(ByteBuffer* message, uint32_t* invokeId, bool createArray)
{
	MmsPdu_t* mmsPdu = 0; /* allow asn1c to allocate structure */

	MmsValue* valueList = NULL;

	asn_dec_rval_t rval = ber_decode(NULL, &asn_DEF_MmsPdu,
			(void**) &mmsPdu, ByteBuffer_getBuffer(message), ByteBuffer_getSize(message));

	if (rval.code != RC_OK)
	    return NULL;

	if (mmsPdu->present == MmsPdu_PR_confirmedResponsePdu) {

	    if (invokeId != NULL)
	        *invokeId = mmsClient_getInvokeId(&mmsPdu->choice.confirmedResponsePdu);

		if (mmsPdu->choice.confirmedResponsePdu.confirmedServiceResponse.present == ConfirmedServiceResponse_PR_read) {
			ReadResponse_t* response = &(mmsPdu->choice.confirmedResponsePdu.confirmedServiceResponse.choice.read);

			int elementCount = response->listOfAccessResult.list.count;

			valueList = mmsClient_parseListOfAccessResults(response->listOfAccessResult.list.array,
			        elementCount, createArray);
		}
	}

	asn_DEF_MmsPdu.free_struct(&asn_DEF_MmsPdu, mmsPdu, 0);

	return valueList;
}


static ReadRequest_t*
createReadRequest (MmsPdu_t* mmsPdu)
{
	mmsPdu->choice.confirmedRequestPdu.confirmedServiceRequest.present =
				ConfirmedServiceRequest_PR_read;

	return &(mmsPdu->choice.confirmedRequestPdu.confirmedServiceRequest.choice.read);
}


int
mmsClient_createReadNamedVariableListRequest(uint32_t invokeId, const char* domainId, const char* itemId,
		ByteBuffer* writeBuffer, bool specWithResult)
{
	MmsPdu_t* mmsPdu = mmsClient_createConfirmedRequestPdu(invokeId);

	ReadRequest_t* readRequest = createReadRequest(mmsPdu);

	if (specWithResult) {
		readRequest->specificationWithResult = (BOOLEAN_t*) GLOBAL_CALLOC(1, sizeof(BOOLEAN_t));
		(*(readRequest->specificationWithResult)) = true;
	}
	else
		readRequest->specificationWithResult = NULL;

	readRequest->variableAccessSpecification.present = VariableAccessSpecification_PR_variableListName;

	ObjectName_t* objectName = &(readRequest->variableAccessSpecification.choice.variableListName);


	if (domainId != NULL) {
        objectName->present = ObjectName_PR_domainspecific;

        objectName->choice.domainspecific.domainId.buf = (uint8_t*) copyString(domainId);
        objectName->choice.domainspecific.domainId.size = strlen(domainId);

        objectName->choice.domainspecific.itemId.buf = (uint8_t*) copyString(itemId);
        objectName->choice.domainspecific.itemId.size = strlen(itemId);
	}
	else {
        objectName->present = ObjectName_PR_vmdspecific;

        objectName->choice.vmdspecific.buf = (uint8_t*) copyString(itemId);
        objectName->choice.vmdspecific.size = strlen(itemId);
	}

	asn_enc_rval_t rval;

	rval = der_encode(&asn_DEF_MmsPdu, mmsPdu,
		(asn_app_consume_bytes_f*) mmsClient_write_out, (void*) writeBuffer);

	asn_DEF_MmsPdu.free_struct(&asn_DEF_MmsPdu, mmsPdu, 0);

	return rval.encoded;
}

int
mmsClient_createReadAssociationSpecificNamedVariableListRequest(
		uint32_t invokeId,
		const char* itemId,
		ByteBuffer* writeBuffer,
		bool specWithResult)
{
	MmsPdu_t* mmsPdu = mmsClient_createConfirmedRequestPdu(invokeId);

	ReadRequest_t* readRequest = createReadRequest(mmsPdu);

	if (specWithResult) {
		readRequest->specificationWithResult = (BOOLEAN_t*) GLOBAL_CALLOC(1, sizeof(BOOLEAN_t));
		(*(readRequest->specificationWithResult)) = true;
	}
	else
		readRequest->specificationWithResult = NULL;

	readRequest->variableAccessSpecification.present = VariableAccessSpecification_PR_variableListName;

	ObjectName_t* objectName = &(readRequest->variableAccessSpecification.choice.variableListName);

	objectName->present = ObjectName_PR_aaspecific;

	objectName->choice.aaspecific.buf = (uint8_t*) copyString(itemId);
	objectName->choice.aaspecific.size = strlen(itemId);

	asn_enc_rval_t rval;

	rval = der_encode(&asn_DEF_MmsPdu, mmsPdu,
	           (asn_app_consume_bytes_f*) mmsClient_write_out, (void*) writeBuffer);

	asn_DEF_MmsPdu.free_struct(&asn_DEF_MmsPdu, mmsPdu, 0);

	return rval.encoded;
}

/**
 * Request a single value
 */
int
mmsClient_createReadRequest(uint32_t invokeId, const char* domainId, const char* itemId, ByteBuffer* writeBuffer)
{
	MmsPdu_t* mmsPdu = mmsClient_createConfirmedRequestPdu(invokeId);

	ReadRequest_t* readRequest = createReadRequest(mmsPdu);

	readRequest->specificationWithResult = NULL;

	readRequest->variableAccessSpecification.present = VariableAccessSpecification_PR_listOfVariable;

	readRequest->variableAccessSpecification.choice.listOfVariable.list.array = 
			(ListOfVariableSeq_t**) GLOBAL_CALLOC(1, sizeof(ListOfVariableSeq_t*));
	readRequest->variableAccessSpecification.choice.listOfVariable.list.count = 1;

	ListOfVariableSeq_t* listOfVars = (ListOfVariableSeq_t*) GLOBAL_CALLOC(1, sizeof(ListOfVariableSeq_t));

	readRequest->variableAccessSpecification.choice.listOfVariable.list.array[0] = listOfVars;

	listOfVars->alternateAccess = NULL;
	listOfVars->variableSpecification.present = VariableSpecification_PR_name;

	if (domainId != NULL) {
        listOfVars->variableSpecification.choice.name.present = ObjectName_PR_domainspecific;
        listOfVars->variableSpecification.choice.name.choice.domainspecific.domainId.buf = (uint8_t*) domainId;
        listOfVars->variableSpecification.choice.name.choice.domainspecific.domainId.size = strlen(domainId);
        listOfVars->variableSpecification.choice.name.choice.domainspecific.itemId.buf = (uint8_t*) itemId;
        listOfVars->variableSpecification.choice.name.choice.domainspecific.itemId.size = strlen(itemId);
	}
	else {
	    listOfVars->variableSpecification.choice.name.present = ObjectName_PR_vmdspecific;
	    listOfVars->variableSpecification.choice.name.choice.vmdspecific.buf = (uint8_t*) itemId;
	    listOfVars->variableSpecification.choice.name.choice.vmdspecific.size = strlen(itemId);
	}

	asn_enc_rval_t rval;

	rval = der_encode(&asn_DEF_MmsPdu, mmsPdu,
	            (asn_app_consume_bytes_f*) mmsClient_write_out, (void*) writeBuffer);

	/* clean up data structures */
	GLOBAL_FREEMEM(listOfVars);
	GLOBAL_FREEMEM(readRequest->variableAccessSpecification.choice.listOfVariable.list.array);
	readRequest->variableAccessSpecification.choice.listOfVariable.list.array = NULL;
	readRequest->variableAccessSpecification.choice.listOfVariable.list.count = 0;
	asn_DEF_MmsPdu.free_struct(&asn_DEF_MmsPdu, mmsPdu, 0);

	return rval.encoded;
}

static AlternateAccess_t*
createAlternateAccess(uint32_t index, uint32_t elementCount)
{
	AlternateAccess_t* alternateAccess = (AlternateAccess_t*) GLOBAL_CALLOC(1, sizeof(AlternateAccess_t));
	alternateAccess->list.count = 1;
	alternateAccess->list.array = (struct AlternateAccess__Member**) GLOBAL_CALLOC(1, sizeof(struct AlternateAccess__Member*));
	alternateAccess->list.array[0] = (struct AlternateAccess__Member*) GLOBAL_CALLOC(1, sizeof(struct AlternateAccess__Member));
	alternateAccess->list.array[0]->present = AlternateAccess__Member_PR_unnamed;

	alternateAccess->list.array[0]->choice.unnamed = (AlternateAccessSelection_t*) GLOBAL_CALLOC(1, sizeof(AlternateAccessSelection_t));

	alternateAccess->list.array[0]->choice.unnamed->present = AlternateAccessSelection_PR_selectAccess;

	if (elementCount > 0) {
		alternateAccess->list.array[0]->choice.unnamed->choice.selectAccess.present =
				AlternateAccessSelection__selectAccess_PR_indexRange;

		INTEGER_t* asnIndex =
			&(alternateAccess->list.array[0]->choice.unnamed->choice.selectAccess.choice.indexRange.lowIndex);

		asn_long2INTEGER(asnIndex, index);

		asnIndex =
			&(alternateAccess->list.array[0]->choice.unnamed->choice.selectAccess.choice.indexRange.numberOfElements);

		asn_long2INTEGER(asnIndex, elementCount);
	}
	else {
		alternateAccess->list.array[0]->choice.unnamed->choice.selectAccess.present =
				AlternateAccessSelection__selectAccess_PR_index;

		INTEGER_t* asnIndex =
			&(alternateAccess->list.array[0]->choice.unnamed->choice.selectAccess.choice.index);

		asn_long2INTEGER(asnIndex, index);
	}

	return alternateAccess;
}

static ListOfVariableSeq_t*
createVariableIdentifier(const char* domainId, const char* itemId)
{
	ListOfVariableSeq_t* variableIdentifier = (ListOfVariableSeq_t*) GLOBAL_CALLOC(1, sizeof(ListOfVariableSeq_t));

	variableIdentifier->variableSpecification.present = VariableSpecification_PR_name;
	variableIdentifier->variableSpecification.choice.name.present = ObjectName_PR_domainspecific;
	variableIdentifier->variableSpecification.choice.name.choice.domainspecific.domainId.buf = (uint8_t*) domainId;
	variableIdentifier->variableSpecification.choice.name.choice.domainspecific.domainId.size = strlen(domainId);
	variableIdentifier->variableSpecification.choice.name.choice.domainspecific.itemId.buf = (uint8_t*) itemId;
	variableIdentifier->variableSpecification.choice.name.choice.domainspecific.itemId.size = strlen(itemId);

	return variableIdentifier;
}

int
mmsClient_createReadRequestAlternateAccessIndex(uint32_t invokeId, const char* domainId, const char* itemId,
		uint32_t index, uint32_t elementCount, ByteBuffer* writeBuffer)
{
	MmsPdu_t* mmsPdu = mmsClient_createConfirmedRequestPdu(invokeId);
	ReadRequest_t* readRequest = createReadRequest(mmsPdu);

	readRequest->specificationWithResult = NULL;

	readRequest->variableAccessSpecification.present = VariableAccessSpecification_PR_listOfVariable;

	readRequest->variableAccessSpecification.choice.listOfVariable.list.array = (ListOfVariableSeq_t**) GLOBAL_CALLOC(1, sizeof(ListOfVariableSeq_t*));
	readRequest->variableAccessSpecification.choice.listOfVariable.list.count = 1;

	ListOfVariableSeq_t* variableIdentifier = createVariableIdentifier(domainId, itemId);

	readRequest->variableAccessSpecification.choice.listOfVariable.list.array[0] = variableIdentifier;

	variableIdentifier->alternateAccess = createAlternateAccess(index, elementCount);

	asn_enc_rval_t rval;

	rval = der_encode(&asn_DEF_MmsPdu, mmsPdu,
		(asn_app_consume_bytes_f*) mmsClient_write_out, (void*) writeBuffer);

	variableIdentifier->variableSpecification.choice.name.choice.domainspecific.domainId.buf = 0;
	variableIdentifier->variableSpecification.choice.name.choice.domainspecific.domainId.size = 0;
	variableIdentifier->variableSpecification.choice.name.choice.domainspecific.itemId.buf = 0;
	variableIdentifier->variableSpecification.choice.name.choice.domainspecific.itemId.size = 0;

	asn_DEF_MmsPdu.free_struct(&asn_DEF_MmsPdu, mmsPdu, 0);

	return rval.encoded;
}

static ListOfVariableSeq_t**
createListOfVariables(ReadRequest_t* readRequest, int valuesCount) {
	readRequest->variableAccessSpecification.present = VariableAccessSpecification_PR_listOfVariable;

	readRequest->variableAccessSpecification.choice.listOfVariable.list.array = (ListOfVariableSeq_t**)
        GLOBAL_CALLOC(valuesCount, sizeof(ListOfVariableSeq_t*));
	readRequest->variableAccessSpecification.choice.listOfVariable.list.count = valuesCount;
	readRequest->variableAccessSpecification.choice.listOfVariable.list.size = valuesCount;

	return readRequest->variableAccessSpecification.choice.listOfVariable.list.array;
}

/**
 * Request multiple values of a single domain
 */
int
mmsClient_createReadRequestMultipleValues(uint32_t invokeId, const char* domainId, LinkedList items,
		ByteBuffer* writeBuffer)
{
	MmsPdu_t* mmsPdu = mmsClient_createConfirmedRequestPdu(invokeId);

	ReadRequest_t* readRequest = createReadRequest(mmsPdu);

	readRequest->specificationWithResult = NULL;

	int valuesCount = LinkedList_size(items);

	ListOfVariableSeq_t** listOfVars = createListOfVariables(readRequest, valuesCount);

	LinkedList item = items;

	int i = 0;

	while ((item = LinkedList_getNext(item)) != NULL) {
		listOfVars[i] = createVariableIdentifier(domainId, (char*) (item->data));
		i++;
	}

	asn_enc_rval_t rval;

	rval = der_encode(&asn_DEF_MmsPdu, mmsPdu,
		(asn_app_consume_bytes_f*) mmsClient_write_out, (void*) writeBuffer);

	for (i = 0; i < valuesCount; i++) {
		GLOBAL_FREEMEM(listOfVars[i]);
	}
	GLOBAL_FREEMEM(listOfVars);

	readRequest->variableAccessSpecification.choice.listOfVariable.list.count = 0;
	readRequest->variableAccessSpecification.choice.listOfVariable.list.size = 0;
	readRequest->variableAccessSpecification.choice.listOfVariable.list.array = NULL;


	asn_DEF_MmsPdu.free_struct(&asn_DEF_MmsPdu, mmsPdu, 0);

	return rval.encoded;
}

