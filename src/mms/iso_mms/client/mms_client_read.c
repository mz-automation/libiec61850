/*
 *  mms_client_read.c
 *
 *  Copyright 2013-2024 Michael Zillgith
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

    for (i = 0; i < elementCount; i++)
    {
        value = NULL;

        AccessResult_PR presentType = accessResultList[i]->present;

        if (presentType == AccessResult_PR_failure)
        {
            if (DEBUG_MMS_CLIENT)
                printf("MMS CLIENT: received access error!\n");

            if (accessResultList[i]->choice.failure.size > 0)
            {
                int errorCode = (int) accessResultList[i]->choice.failure.buf[0];

                MmsDataAccessError dataAccessError = DATA_ACCESS_ERROR_UNKNOWN;

                if ((errorCode >= 0) && (errorCode < 12))
                    dataAccessError = (MmsDataAccessError) errorCode;

                value = MmsValue_newDataAccessError(dataAccessError);
            }
            else
                value = MmsValue_newDataAccessError(DATA_ACCESS_ERROR_UNKNOWN);
        }
        else if (presentType == AccessResult_PR_array)
        {
            int arrayElementCount =
                    accessResultList[i]->choice.array.list.count;

            if (arrayElementCount > 0)
            {
                value = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));
                value->type = MMS_ARRAY;
                value->value.structure.size = arrayElementCount;
                value->value.structure.components = (MmsValue**) GLOBAL_CALLOC(arrayElementCount, sizeof(MmsValue*));

                if (value->value.structure.components)
                {
                    int j;

                    for (j = 0; j < arrayElementCount; j++)
                    {
                        value->value.structure.components[j] = mmsMsg_parseDataElement(
                                accessResultList[i]->choice.array.list.array[j]);

                        if (value->value.structure.components[j] == NULL)
                        {
                            if (DEBUG_MMS_CLIENT)
                                printf("MMS CLIENT: failed to parse array element %i\n", j);

                            MmsValue_delete(value);
                            value = NULL;
                            break;
                        }
                    }
                }
            }
            else
            {
                if (DEBUG_MMS_CLIENT)
                    printf("MMS CLIENT: error parsing access result (invalid array size)!\n");
            }
        }
        else if (presentType == AccessResult_PR_structure)
        {
            int componentCount =
                    accessResultList[i]->choice.structure.list.count;

            if (componentCount > 0)
            {
                value = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));

                if (value)
                {
                    value->type = MMS_STRUCTURE;
                    value->value.structure.size = componentCount;
                    value->value.structure.components = (MmsValue**) GLOBAL_CALLOC(componentCount, sizeof(MmsValue*));

                    if (value->value.structure.components)
                    {
                        int j;
                        for (j = 0; j < componentCount; j++)
                        {
                            value->value.structure.components[j] = mmsMsg_parseDataElement(
                                    accessResultList[i]->choice.structure.list.array[j]);

                            if (value->value.structure.components[j] == NULL)
                            {
                                if (DEBUG_MMS_CLIENT)
                                    printf("MMS CLIENT: failed to parse struct element %i\n", j);

                                MmsValue_delete(value);
                                value = NULL;
                                break;
                            }
                        }
                    }
                }
            }
            else
            {
                if (DEBUG_MMS_CLIENT)
                    printf("MMS CLIENT: error parsing access result (invalid structure size)!\n");
            }
        }
        else if (presentType == AccessResult_PR_bitstring)
        {
            int size = accessResultList[i]->choice.bitstring.size;

            if (size > 0)
            {
                int maxSize = (size * 8);
                int bitSize = maxSize - accessResultList[i]->choice.bitstring.bits_unused;

                if ((bitSize > 0) && (maxSize >= bitSize))
                {
                    value = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));

                    if (value)
                    {
                        value->type = MMS_BIT_STRING;

                        value->value.bitString.size = (size * 8)
                                - accessResultList[i]->choice.bitstring.bits_unused;

                        value->value.bitString.buf = (uint8_t*) GLOBAL_MALLOC(size);
                        memcpy(value->value.bitString.buf,
                                accessResultList[i]->choice.bitstring.buf, size);
                    }
                }
                else
                {
                    if (DEBUG_MMS_CLIENT)
                        printf("MMS CLIENT: error parsing access result (bit string padding problem)!\n");
                }
            }
            else
            {
                if (DEBUG_MMS_CLIENT)
                    printf("MMS CLIENT: error parsing access result (bit string size 0 or negative)!\n");
            }
        }
        else if (presentType == AccessResult_PR_integer)
        {
            int size = accessResultList[i]->choice.integer.size;

            if (size > 0)
            {
                Asn1PrimitiveValue* berInteger =
                        BerInteger_createFromBuffer(accessResultList[i]->choice.integer.buf, size);

                value = MmsValue_newIntegerFromBerInteger(berInteger);
            }
            else
            {
                if (DEBUG_MMS_CLIENT)
                    printf("MMS CLIENT: error parsing access result (invalid integer size)!\n");
            }
        }
        else if (presentType == AccessResult_PR_unsigned)
        {
            int size = accessResultList[i]->choice.Unsigned.size;

            if (size > 0)
            {
                Asn1PrimitiveValue* berInteger =
                        BerInteger_createFromBuffer(accessResultList[i]->choice.Unsigned.buf,
                                accessResultList[i]->choice.Unsigned.size);

                value = MmsValue_newUnsignedFromBerInteger(berInteger);
            }
            else
            {
                if (DEBUG_MMS_CLIENT)
                    printf("MMS CLIENT: error parsing access result (invalid unsigned size)!\n");
            }
        }
        else if (presentType == AccessResult_PR_floatingpoint)
        {
            int size = accessResultList[i]->choice.floatingpoint.size;

            if (size == 5) /* FLOAT32 */
            { 
                value = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));

                if (value)
                {
                    value->type = MMS_FLOAT;

                    value->value.floatingPoint.formatWidth = 32;
                    value->value.floatingPoint.exponentWidth = accessResultList[i]->choice.floatingpoint.buf[0];

                    uint8_t* floatBuf = (accessResultList[i]->choice.floatingpoint.buf + 1);

#if (ORDER_LITTLE_ENDIAN == 1)
                    memcpyReverseByteOrder(value->value.floatingPoint.buf, floatBuf, 4);
#else
                    memcpy(value->value.floatingPoint.buf, floatBuf, 4);
#endif
                }
            }
            else if (size == 9) /* FLOAT64 */
            {
                value = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));

                if (value)
                {
                    value->type = MMS_FLOAT;

                    value->value.floatingPoint.formatWidth = 64;
                    value->value.floatingPoint.exponentWidth = accessResultList[i]->choice.floatingpoint.buf[0];

                    uint8_t* floatBuf = (accessResultList[i]->choice.floatingpoint.buf + 1);

#if (ORDER_LITTLE_ENDIAN == 1)
                    memcpyReverseByteOrder(value->value.floatingPoint.buf, floatBuf, 8);
#else
                    memcpy(value->value.floatingPoint.buf, floatBuf, 8);
#endif
                }
            }
            else
            {
                if (DEBUG_MMS_CLIENT)
                    printf("MMS CLIENT: error parsing float (size must be 5 or 9, is %i)\n", size);
            }
        }
        else if (presentType == AccessResult_PR_visiblestring)
        {
            int strSize = accessResultList[i]->choice.visiblestring.size;

            if (strSize >= 0)
            {
                value = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));

                if (value)
                {
                    value->type = MMS_VISIBLE_STRING;
                    value->value.visibleString.buf = (char*) GLOBAL_MALLOC(strSize + 1);
                    value->value.visibleString.size = strSize;

                    memcpy(value->value.visibleString.buf,
                            accessResultList[i]->choice.visiblestring.buf,
                            strSize);

                    value->value.visibleString.buf[strSize] = 0;
                }
            }
            else
            {
                if (DEBUG_MMS_CLIENT)
                    printf("MMS CLIENT: error parsing access result (invalid visible-string size)\n");
            }
        }
        else if (presentType == AccessResult_PR_mMSString)
        {
            int strSize = accessResultList[i]->choice.mMSString.size;

            if (strSize >= 0)
            {
                value = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));

                if (value)
                {
                    value->type = MMS_STRING;
                    value->value.visibleString.buf = (char*) GLOBAL_MALLOC(strSize + 1);
                    value->value.visibleString.size = strSize;

                    memcpy(value->value.visibleString.buf,
                            accessResultList[i]->choice.mMSString.buf, strSize);

                    value->value.visibleString.buf[strSize] = 0;
                }
            }
            else
            {
                if (DEBUG_MMS_CLIENT)
                    printf("MMS CLIENT: error parsing access result (invalid mms-string size)\n");
            }
        }
        else if (presentType == AccessResult_PR_utctime)
        {
            int size = accessResultList[i]->choice.utctime.size;

            if (size == 8)
            {
                value = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));

                if (value)
                {
                    value->type = MMS_UTC_TIME;
                    memcpy(value->value.utcTime, accessResultList[i]->choice.utctime.buf, 8);
                }
            }
            else
            {
                if (DEBUG_MMS_CLIENT)
                    printf("MMS CLIENT: error parsing UTC time (size is %i instead of 8\n", size);
            }
        }
        else if (presentType == AccessResult_PR_boolean)
        {
            value = MmsValue_newBoolean(accessResultList[i]->choice.boolean);
        }
        else if (presentType == AccessResult_PR_binarytime)
        {
            int size = accessResultList[i]->choice.binarytime.size;

            if ((size == 4) || (size == 6))
            {
                value = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));

                if (value)
                {
                    value->type = MMS_BINARY_TIME;
                    value->value.binaryTime.size = size;
                    memcpy(value->value.binaryTime.buf, accessResultList[i]->choice.binarytime.buf, size);
                }
            }
            else
            {
                if (DEBUG_MMS_CLIENT)
                    printf("MMS CLIENT: error parsing binary time (size must be 4 or 6, is %i\n", size);
            }
        }
        else if (presentType == AccessResult_PR_octetstring)
        {
            int size = accessResultList[i]->choice.octetstring.size;

            if (size >= 0)
            {
                value = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));

                if (value)
                {
                    value->type = MMS_OCTET_STRING;
                    value->value.octetString.maxSize = -size;
                    value->value.octetString.size = size;
                    value->value.octetString.buf = (uint8_t*) GLOBAL_MALLOC(size);
                    memcpy(value->value.octetString.buf, accessResultList[i]->choice.octetstring.buf, size);
                }
            }
            else
            {
                if (DEBUG_MMS_CLIENT)
                    printf("MMS CLIENT: error parsing access result (invalid octet-string size)\n");
            }
        }
        else
        {
            if (DEBUG_MMS_CLIENT)
                printf("MMS CLIENT: unknown type %i in access result\n", presentType);
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
    MmsPdu_t* mmsPdu = NULL; /* allow asn1c to allocate structure */

    MmsValue* valueList = NULL;

    asn_dec_rval_t rval = ber_decode(NULL, &asn_DEF_MmsPdu,
            (void**) &mmsPdu, ByteBuffer_getBuffer(message), ByteBuffer_getSize(message));

    if (rval.code == RC_OK) {
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
    }

    asn_DEF_MmsPdu.free_struct(&asn_DEF_MmsPdu, mmsPdu, 0);

    return valueList;
}

static ReadRequest_t*
createReadRequest(MmsPdu_t* mmsPdu)
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

        objectName->choice.domainspecific.domainId.buf = (uint8_t*) StringUtils_copyString(domainId);
        objectName->choice.domainspecific.domainId.size = (int)strlen(domainId);

        objectName->choice.domainspecific.itemId.buf = (uint8_t*) StringUtils_copyString(itemId);
        objectName->choice.domainspecific.itemId.size = (int)strlen(itemId);
    }
    else {
        objectName->present = ObjectName_PR_vmdspecific;

        objectName->choice.vmdspecific.buf = (uint8_t*) StringUtils_copyString(itemId);
        objectName->choice.vmdspecific.size = (int)strlen(itemId);
    }

    asn_enc_rval_t rval;

    rval = der_encode(&asn_DEF_MmsPdu, mmsPdu,
            (asn_app_consume_bytes_f*) mmsClient_write_out, (void*) writeBuffer);

    asn_DEF_MmsPdu.free_struct(&asn_DEF_MmsPdu, mmsPdu, 0);

    return (int)rval.encoded;
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

    objectName->choice.aaspecific.buf = (uint8_t*) StringUtils_copyString(itemId);
    objectName->choice.aaspecific.size = (int)strlen(itemId);

    asn_enc_rval_t rval;

    rval = der_encode(&asn_DEF_MmsPdu, mmsPdu,
            (asn_app_consume_bytes_f*) mmsClient_write_out, (void*) writeBuffer);

    asn_DEF_MmsPdu.free_struct(&asn_DEF_MmsPdu, mmsPdu, 0);

    return (int)rval.encoded;
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
        listOfVars->variableSpecification.choice.name.choice.domainspecific.domainId.size = (int)strlen(domainId);
        listOfVars->variableSpecification.choice.name.choice.domainspecific.itemId.buf = (uint8_t*) itemId;
        listOfVars->variableSpecification.choice.name.choice.domainspecific.itemId.size = (int)strlen(itemId);
    }
    else {
        listOfVars->variableSpecification.choice.name.present = ObjectName_PR_vmdspecific;
        listOfVars->variableSpecification.choice.name.choice.vmdspecific.buf = (uint8_t*) itemId;
        listOfVars->variableSpecification.choice.name.choice.vmdspecific.size = (int)strlen(itemId);
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

    return (int)rval.encoded;
}

static AlternateAccess_t*
createAlternateAccessComponent(const char* componentName)
{
    AlternateAccess_t* alternateAccess = (AlternateAccess_t*) GLOBAL_CALLOC(1, sizeof(AlternateAccess_t));
    alternateAccess->list.count = 1;
    alternateAccess->list.array = (struct AlternateAccess__Member**) GLOBAL_CALLOC(1, sizeof(struct AlternateAccess__Member*));
    alternateAccess->list.array[0] = (struct AlternateAccess__Member*) GLOBAL_CALLOC(1, sizeof(struct AlternateAccess__Member));
    alternateAccess->list.array[0]->present = AlternateAccess__Member_PR_unnamed;

    alternateAccess->list.array[0]->choice.unnamed = (AlternateAccessSelection_t*) GLOBAL_CALLOC(1, sizeof(AlternateAccessSelection_t));

    const char* separator = strchr(componentName, '$');

    if (separator) {
        int size = (int)(separator - componentName);

        alternateAccess->list.array[0]->choice.unnamed->present = AlternateAccessSelection_PR_selectAlternateAccess;
        alternateAccess->list.array[0]->choice.unnamed->choice.selectAlternateAccess.accessSelection.present =
                AlternateAccessSelection__selectAlternateAccess__accessSelection_PR_component;

        alternateAccess->list.array[0]->choice.unnamed->choice.selectAlternateAccess.accessSelection.choice.component.buf =
			(uint8_t*) StringUtils_copySubString((char*) componentName, (char*) separator);
        alternateAccess->list.array[0]->choice.unnamed->choice.selectAlternateAccess.accessSelection.choice.component.size = size;

        alternateAccess->list.array[0]->choice.unnamed->choice.selectAlternateAccess.alternateAccess = createAlternateAccessComponent(separator + 1);
    }
    else {
        int size = (int)strlen(componentName);

        alternateAccess->list.array[0]->choice.unnamed->present = AlternateAccessSelection_PR_selectAccess;

        alternateAccess->list.array[0]->choice.unnamed->choice.selectAccess.present =
                AlternateAccessSelection__selectAccess_PR_component;

		alternateAccess->list.array[0]->choice.unnamed->choice.selectAccess.choice.component.buf =
			(uint8_t*) StringUtils_copyString(componentName);
        alternateAccess->list.array[0]->choice.unnamed->choice.selectAccess.choice.component.size = size;
    }

    return alternateAccess;
}

static void
deleteAlternateAccessComponent(AlternateAccess_t* alternateAccess)
{
    if (alternateAccess->list.array[0]->choice.unnamed->choice.selectAlternateAccess.alternateAccess)
        deleteAlternateAccessComponent(alternateAccess->list.array[0]->choice.unnamed->choice.selectAlternateAccess.alternateAccess);

    if (alternateAccess->list.array[0]->choice.unnamed->present == AlternateAccessSelection_PR_selectAlternateAccess)
        GLOBAL_FREEMEM(alternateAccess->list.array[0]->choice.unnamed->choice.selectAlternateAccess.accessSelection.choice.component.buf);
    else if (alternateAccess->list.array[0]->choice.unnamed->present == AlternateAccessSelection_PR_selectAccess)
        GLOBAL_FREEMEM(alternateAccess->list.array[0]->choice.unnamed->choice.selectAccess.choice.component.buf);

    GLOBAL_FREEMEM(alternateAccess->list.array[0]->choice.unnamed);
    GLOBAL_FREEMEM(alternateAccess->list.array[0]);
    GLOBAL_FREEMEM(alternateAccess->list.array);
    GLOBAL_FREEMEM(alternateAccess);
}

static ListOfVariableSeq_t*
createNewVariableSpecification(const char* domainId, const char* itemId, const char* componentName, bool associationSpecific)
{
    ListOfVariableSeq_t* varSpec = (ListOfVariableSeq_t*) GLOBAL_CALLOC(1, sizeof(ListOfVariableSeq_t));

    varSpec->variableSpecification.present = VariableSpecification_PR_name;

    if (domainId) {
        varSpec->variableSpecification.choice.name.present = ObjectName_PR_domainspecific;
        varSpec->variableSpecification.choice.name.choice.domainspecific.domainId.buf = (uint8_t*) domainId;
        varSpec->variableSpecification.choice.name.choice.domainspecific.domainId.size = (int)strlen(domainId);
        varSpec->variableSpecification.choice.name.choice.domainspecific.itemId.buf = (uint8_t*) itemId;
        varSpec->variableSpecification.choice.name.choice.domainspecific.itemId.size = (int)strlen(itemId);
    }
    else if (associationSpecific) {
        varSpec->variableSpecification.choice.name.present = ObjectName_PR_aaspecific;
        varSpec->variableSpecification.choice.name.choice.aaspecific.buf = (uint8_t*) itemId;
        varSpec->variableSpecification.choice.name.choice.aaspecific.size = (int)strlen(itemId);
    }
    else {
        varSpec->variableSpecification.choice.name.present = ObjectName_PR_vmdspecific;
        varSpec->variableSpecification.choice.name.choice.vmdspecific.buf = (uint8_t*) itemId;
        varSpec->variableSpecification.choice.name.choice.vmdspecific.size = (int)strlen(itemId);
    }

    if (componentName)
        varSpec->alternateAccess = createAlternateAccessComponent(componentName);

    return varSpec;
}

/**
 * Request a single value with optional component
 */
int
mmsClient_createReadRequestComponent(uint32_t invokeId, const char* domainId, const char* itemId, const char* component, ByteBuffer* writeBuffer)
{
    MmsPdu_t* mmsPdu = mmsClient_createConfirmedRequestPdu(invokeId);

    ReadRequest_t* readRequest = createReadRequest(mmsPdu);

    readRequest->specificationWithResult = NULL;

    readRequest->variableAccessSpecification.present = VariableAccessSpecification_PR_listOfVariable;
    readRequest->variableAccessSpecification.choice.listOfVariable.list.count = 1;
    readRequest->variableAccessSpecification.choice.listOfVariable.list.size = 1;
    readRequest->variableAccessSpecification.choice.listOfVariable.list.array =
            (ListOfVariableSeq_t**) GLOBAL_CALLOC(1, sizeof(ListOfVariableSeq_t*));
    readRequest->variableAccessSpecification.choice.listOfVariable.list.array[0] = createNewVariableSpecification(domainId, itemId, component, false);

    asn_enc_rval_t rval;

    rval = der_encode(&asn_DEF_MmsPdu, mmsPdu,
            (asn_app_consume_bytes_f*) mmsClient_write_out, (void*) writeBuffer);

    /* clean up data structures */
    deleteAlternateAccessComponent(readRequest->variableAccessSpecification.choice.listOfVariable.list.array[0]->alternateAccess);

    GLOBAL_FREEMEM(readRequest->variableAccessSpecification.choice.listOfVariable.list.array[0]);
    GLOBAL_FREEMEM(readRequest->variableAccessSpecification.choice.listOfVariable.list.array);
    readRequest->variableAccessSpecification.choice.listOfVariable.list.array = NULL;
    readRequest->variableAccessSpecification.choice.listOfVariable.list.count = 0;
    asn_DEF_MmsPdu.free_struct(&asn_DEF_MmsPdu, mmsPdu, 0);

    return (int)rval.encoded;
}

static ListOfVariableSeq_t*
createVariableIdentifier(const char* domainId, const char* itemId)
{
    ListOfVariableSeq_t* variableIdentifier = (ListOfVariableSeq_t*) GLOBAL_CALLOC(1, sizeof(ListOfVariableSeq_t));

    variableIdentifier->variableSpecification.present = VariableSpecification_PR_name;
    variableIdentifier->variableSpecification.choice.name.present = ObjectName_PR_domainspecific;
    variableIdentifier->variableSpecification.choice.name.choice.domainspecific.domainId.buf = (uint8_t*) domainId;
    variableIdentifier->variableSpecification.choice.name.choice.domainspecific.domainId.size = (int)strlen(domainId);
    variableIdentifier->variableSpecification.choice.name.choice.domainspecific.itemId.buf = (uint8_t*) itemId;
    variableIdentifier->variableSpecification.choice.name.choice.domainspecific.itemId.size = (int)strlen(itemId);

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

    variableIdentifier->alternateAccess = mmsClient_createAlternateAccess(index, elementCount);

    asn_enc_rval_t rval;

    rval = der_encode(&asn_DEF_MmsPdu, mmsPdu,
            (asn_app_consume_bytes_f*) mmsClient_write_out, (void*) writeBuffer);

    variableIdentifier->variableSpecification.choice.name.choice.domainspecific.domainId.buf = 0;
    variableIdentifier->variableSpecification.choice.name.choice.domainspecific.domainId.size = 0;
    variableIdentifier->variableSpecification.choice.name.choice.domainspecific.itemId.buf = 0;
    variableIdentifier->variableSpecification.choice.name.choice.domainspecific.itemId.size = 0;

    asn_DEF_MmsPdu.free_struct(&asn_DEF_MmsPdu, mmsPdu, 0);

    return (int)rval.encoded;
}

int
mmsClient_createReadRequestAlternateAccessSingleIndexComponent(uint32_t invokeId, const char* domainId, const char* itemId,
        uint32_t arrayIndex, const char* component, ByteBuffer* writeBuffer)
{
    MmsPdu_t* mmsPdu = mmsClient_createConfirmedRequestPdu(invokeId);
    ReadRequest_t* readRequest = createReadRequest(mmsPdu);

    readRequest->specificationWithResult = NULL;

    readRequest->variableAccessSpecification.present = VariableAccessSpecification_PR_listOfVariable;

    readRequest->variableAccessSpecification.choice.listOfVariable.list.array = (ListOfVariableSeq_t**) GLOBAL_CALLOC(1, sizeof(ListOfVariableSeq_t*));
    readRequest->variableAccessSpecification.choice.listOfVariable.list.count = 1;

    ListOfVariableSeq_t* variableIdentifier = createVariableIdentifier(domainId, itemId);

    readRequest->variableAccessSpecification.choice.listOfVariable.list.array[0] = variableIdentifier;

    variableIdentifier->alternateAccess = mmsClient_createAlternateAccessIndexComponent(arrayIndex, component);

    asn_enc_rval_t rval;

    rval = der_encode(&asn_DEF_MmsPdu, mmsPdu,
            (asn_app_consume_bytes_f*) mmsClient_write_out, (void*) writeBuffer);

    variableIdentifier->variableSpecification.choice.name.choice.domainspecific.domainId.buf = 0;
    variableIdentifier->variableSpecification.choice.name.choice.domainspecific.domainId.size = 0;
    variableIdentifier->variableSpecification.choice.name.choice.domainspecific.itemId.buf = 0;
    variableIdentifier->variableSpecification.choice.name.choice.domainspecific.itemId.size = 0;

    asn_DEF_MmsPdu.free_struct(&asn_DEF_MmsPdu, mmsPdu, 0);

    return (int)rval.encoded;
}

static ListOfVariableSeq_t**
createListOfVariables(ReadRequest_t* readRequest, int valuesCount)
{
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

    return (int)rval.encoded;
}

