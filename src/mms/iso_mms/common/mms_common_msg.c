/*
 *  mms_common_msg.c
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
#include "mms_common_internal.h"
#include "platform_endian.h"
#include "stack_config.h"
#include "string_utilities.h"
#include "mms_value_internal.h"

void
mmsMsg_createFloatData(MmsValue* value, int* size, uint8_t** buf)
{
    if (value->value.floatingPoint.formatWidth == 64) {
        *size = 9;
        *buf = (uint8_t*) malloc(9);
        (*buf)[0] = 11;
#if (ORDER_LITTLE_ENDIAN == 1)
        memcpyReverseByteOrder((*buf) + 1, value->value.floatingPoint.buf, 8);
#else
        memcpy((*buf) + 1, value->value.floatingPoint.buf, 8);
#endif
    } else {
        *size = 5;
        *buf = (uint8_t*) malloc(5);
        (*buf)[0] = 8;
#if (ORDER_LITTLE_ENDIAN == 1)
        memcpyReverseByteOrder((*buf) + 1, value->value.floatingPoint.buf, 4);
#else
        memcpy((*buf) + 1, value->value.floatingPoint.buf, 4);
#endif
    }
}

Data_t*
mmsMsg_createBasicDataElement(MmsValue* value)
{
    Data_t* dataElement = (Data_t*) calloc(1, sizeof(Data_t));

    switch (value->type) {
    case MMS_ARRAY:
        {
            int size = MmsValue_getArraySize(value);
            dataElement->present = Data_PR_array;
            dataElement->choice.array = (DataSequence_t*) calloc(1, sizeof(DataSequence_t));
            dataElement->choice.array->list.count = size;
            dataElement->choice.array->list.size = size;
            dataElement->choice.array->list.array = (Data_t**) calloc(size, sizeof(Data_t*));
            int i;
            for (i = 0; i < size; i++) {
                dataElement->choice.array->list.array[i] =
                        mmsMsg_createBasicDataElement(MmsValue_getElement(value, i));
            }
        }
        break;

    case MMS_STRUCTURE:
        {
            int size = value->value.structure.size;

            dataElement->present = Data_PR_structure;
            dataElement->choice.structure = (DataSequence_t*) calloc(1, sizeof(DataSequence_t));
            dataElement->choice.structure->list.count = size;
            dataElement->choice.structure->list.size = size;
            dataElement->choice.structure->list.array = (Data_t**) calloc(size, sizeof(Data_t*));
            int i;
            for (i = 0; i < size; i++) {
                dataElement->choice.structure->list.array[i] = mmsMsg_createBasicDataElement(
                        value->value.structure.components[i]);
            }
        }
        break;

    case MMS_BIT_STRING: 
		{
			dataElement->present = Data_PR_bitstring;
			dataElement->choice.bitstring.buf = value->value.bitString.buf;
			int size = (value->value.bitString.size / 8) + ((value->value.bitString.size % 8) > 0);
			int unused = 8 - (value->value.bitString.size % 8);
			dataElement->choice.bitstring.size = size; /* size in bytes */
			dataElement->choice.bitstring.bits_unused = unused;
		}
        break;

    case MMS_BOOLEAN:
        dataElement->present = Data_PR_boolean;
        dataElement->choice.boolean = value->value.boolean;
        break;

    case MMS_FLOAT:
        dataElement->present = Data_PR_floatingpoint;

        mmsMsg_createFloatData(value, &dataElement->choice.floatingpoint.size,
                &dataElement->choice.floatingpoint.buf);
        break;

    case MMS_UTC_TIME:
        dataElement->present = Data_PR_utctime;

        dataElement->choice.utctime.buf = (uint8_t*) malloc(8);
        memcpy(dataElement->choice.utctime.buf, value->value.utcTime, 8);
        dataElement->choice.utctime.size = 8;
        break;

    case MMS_INTEGER:
        dataElement->present = Data_PR_integer;

        dataElement->choice.integer.size = value->value.integer->size;
        dataElement->choice.integer.buf = value->value.integer->octets;

        break;

    case MMS_UNSIGNED:
        dataElement->present = Data_PR_unsigned;

        dataElement->choice.Unsigned.size = value->value.integer->size;
        dataElement->choice.Unsigned.buf = value->value.integer->octets;

        break;

    case MMS_VISIBLE_STRING:
        dataElement->present = Data_PR_visiblestring;
        if (value->value.visibleString.buf != NULL ) {
            dataElement->choice.visiblestring.buf = (uint8_t*) value->value.visibleString.buf;
            dataElement->choice.visiblestring.size = strlen(value->value.visibleString.buf);
        } else
            dataElement->choice.visiblestring.size = 0;
        break;

    case MMS_OCTET_STRING:
        dataElement->present = Data_PR_octetstring;
        if (value->value.octetString.buf != NULL ) {
            dataElement->choice.octetstring.buf = value->value.octetString.buf;
            dataElement->choice.octetstring.size =
                    value->value.octetString.size;
        } else
            dataElement->choice.octetstring.size = 0;
        break;

    case MMS_STRING:
        dataElement->present = Data_PR_mMSString;
        if (value->value.visibleString.buf != NULL ) {
            dataElement->choice.mMSString.buf = (uint8_t*) value->value.visibleString.buf;
            dataElement->choice.mMSString.size = strlen(value->value.visibleString.buf);
        } else
            dataElement->choice.mMSString.size = 0;
        break;

    case MMS_BINARY_TIME:
        dataElement->present = Data_PR_binarytime;
        dataElement->choice.binarytime.size = value->value.binaryTime.size;
        dataElement->choice.binarytime.buf = value->value.binaryTime.buf;
        break;

    default:
        dataElement->present = Data_PR_NOTHING;
        printf("MMS read: unknown value type %i in result\n", value->type);
        break;
    }

    return dataElement;
}

MmsValue*
mmsMsg_parseDataElement(Data_t* dataElement)
{
    MmsValue* value = NULL;

    if (dataElement->present == Data_PR_structure) {
        value = (MmsValue*) calloc(1, sizeof(MmsValue));

        int componentCount = dataElement->choice.structure->list.count;

        value->type = MMS_STRUCTURE;
        value->value.structure.size = componentCount;
        value->value.structure.components = (MmsValue**) calloc(componentCount, sizeof(MmsValue*));

        int i;

        for (i = 0; i < componentCount; i++) {
            value->value.structure.components[i] =
                    mmsMsg_parseDataElement(dataElement->choice.structure->list.array[i]);
        }
    }
    else if (dataElement->present == Data_PR_array) {
        value = (MmsValue*) calloc(1, sizeof(MmsValue));

        int componentCount = dataElement->choice.array->list.count;

        value->type = MMS_ARRAY;
        value->value.structure.size = componentCount;
        value->value.structure.components = (MmsValue**) calloc(componentCount, sizeof(MmsValue*));

        int i;

        for (i = 0; i < componentCount; i++) {
            value->value.structure.components[i] =
                    mmsMsg_parseDataElement(dataElement->choice.array->list.array[i]);
        }
    }
    else {
        if (dataElement->present == Data_PR_integer) {
            Asn1PrimitiveValue* berInteger = BerInteger_createFromBuffer(
                    dataElement->choice.integer.buf, dataElement->choice.integer.size);

            value = MmsValue_newIntegerFromBerInteger(berInteger);
        }
        else if (dataElement->present == Data_PR_unsigned) {
            Asn1PrimitiveValue* berInteger = BerInteger_createFromBuffer(
                    dataElement->choice.Unsigned.buf, dataElement->choice.Unsigned.size);

            value = MmsValue_newUnsignedFromBerInteger(berInteger);
        }
        else if (dataElement->present == Data_PR_visiblestring) {
            value = MmsValue_newVisibleStringFromByteArray(dataElement->choice.visiblestring.buf,
                    dataElement->choice.visiblestring.size);
        }
        else if (dataElement->present == Data_PR_mMSString) {
            value = MmsValue_newMmsStringFromByteArray(dataElement->choice.mMSString.buf,
                    dataElement->choice.mMSString.size);
        }
        else if (dataElement->present == Data_PR_bitstring) {
            value = (MmsValue*) calloc(1, sizeof(MmsValue));

            value->type = MMS_BIT_STRING;
            int size = dataElement->choice.bitstring.size;

            value->value.bitString.size = (size * 8)
                    - dataElement->choice.bitstring.bits_unused;

            value->value.bitString.buf = (uint8_t*) malloc(size);
            memcpy(value->value.bitString.buf,
                    dataElement->choice.bitstring.buf, size);

        }
        else if (dataElement->present == Data_PR_floatingpoint) {
            value = (MmsValue*) calloc(1, sizeof(MmsValue));
            int size = dataElement->choice.floatingpoint.size;

            value->type = MMS_FLOAT;

            if (size == 5) { /* FLOAT32 */
                value->value.floatingPoint.formatWidth = 32;
                value->value.floatingPoint.exponentWidth = dataElement->choice.floatingpoint.buf[0];

                uint8_t* floatBuf = (dataElement->choice.floatingpoint.buf + 1);

                value->value.floatingPoint.buf = (uint8_t*) malloc(4);
#if (ORDER_LITTLE_ENDIAN == 1)
                memcpyReverseByteOrder(value->value.floatingPoint.buf, floatBuf, 4);
#else
                memcpy(value->value.floatingPoint.buf, floatBuf, 4);
#endif
            }

            if (size == 9) { /* FLOAT64 */
                value->value.floatingPoint.formatWidth = 64;
                value->value.floatingPoint.exponentWidth = dataElement->choice.floatingpoint.buf[0];

                uint8_t* floatBuf = (dataElement->choice.floatingpoint.buf + 1);

                value->value.floatingPoint.buf = (uint8_t*) malloc(8);
#if (ORDER_LITTLE_ENDIAN == 1)
                memcpyReverseByteOrder(value->value.floatingPoint.buf, floatBuf, 8);
#else
                memcpy(value->value.floatingPoint.buf, floatBuf, 8);
#endif
            }
        }
        else if (dataElement->present == Data_PR_utctime) {
            value = (MmsValue*) calloc(1, sizeof(MmsValue));
            value->type = MMS_UTC_TIME;
            memcpy(value->value.utcTime, dataElement->choice.utctime.buf, 8);
        }
        else if (dataElement->present == Data_PR_octetstring) {
            value = (MmsValue*) calloc(1, sizeof(MmsValue));
            value->type = MMS_OCTET_STRING;
            int size = dataElement->choice.octetstring.size;
            value->value.octetString.size = size;
            value->value.octetString.maxSize = size;
            value->value.octetString.buf = (uint8_t*) malloc(size);
            memcpy(value->value.octetString.buf, dataElement->choice.octetstring.buf, size);
        }
        else if (dataElement->present == Data_PR_binarytime) {
            int size = dataElement->choice.binarytime.size;

            if (size <= 6) {
                value = (MmsValue*) calloc(1, sizeof(MmsValue));
                value->type = MMS_BINARY_TIME;
                value->value.binaryTime.size = size;
                memcpy(value->value.binaryTime.buf, dataElement->choice.binarytime.buf, size);
            }
        }
        else if (dataElement->present == Data_PR_boolean) {
            value = MmsValue_newBoolean(dataElement->choice.boolean);
        }

    }

    return value;
}

Data_t*
mmsMsg_createDataElement(MmsValue* value)
{
    if (value->type == MMS_STRUCTURE) {
        Data_t* dataElement = (Data_t*) calloc(1, sizeof(Data_t));

        dataElement->present = Data_PR_structure;

        int elementCount = value->value.structure.size;

        dataElement->choice.structure = (DataSequence_t*) calloc(1, sizeof(DataSequence_t));

        dataElement->choice.structure->list.size = elementCount;
        dataElement->choice.structure->list.count = elementCount;

        dataElement->choice.structure->list.array = (Data_t**) calloc(elementCount, sizeof(Data_t*));

        int i;

        for (i = 0; i < elementCount; i++) {
            dataElement->choice.structure->list.array[i] =
                    mmsMsg_createDataElement(value->value.structure.components[i]);
        }

        return dataElement;
    }
    else {
        return mmsMsg_createBasicDataElement(value);
    }
}

void
mmsMsg_addResultToResultList(AccessResult_t* accessResult, MmsValue* value)
{
    if (value == NULL) {
        accessResult->present = AccessResult_PR_failure;

        asn_long2INTEGER(&accessResult->choice.failure, DataAccessError_objectnonexistent);

        if (DEBUG)
            printf("ACCESS ERROR\n");
    }
    else {
        switch (value->type) {
        case MMS_ARRAY:
            {
            int size = value->value.structure.size;
            accessResult->present = AccessResult_PR_array;
            accessResult->choice.array.list.count = size;
            accessResult->choice.array.list.size = size;
            accessResult->choice.array.list.array = (Data_t**) calloc(size, sizeof(Data_t*));
            int i;
            for (i = 0; i < size; i++) {
                accessResult->choice.array.list.array[i] =
                        mmsMsg_createDataElement(value->value.structure.components[i]);
            }
        }
            break;
        case MMS_STRUCTURE:
            {
            int size = value->value.structure.size;
            accessResult->present = AccessResult_PR_structure;
            accessResult->choice.structure.list.count = size;
            accessResult->choice.structure.list.size = size;
            accessResult->choice.structure.list.array = (Data_t**) calloc(size, sizeof(Data_t*));
            int i;
            for (i = 0; i < size; i++) {
                accessResult->choice.structure.list.array[i] =
                        mmsMsg_createDataElement(value->value.structure.components[i]);
            }

        }
            break;
        case MMS_BIT_STRING:
			{
				accessResult->present = AccessResult_PR_bitstring;
				accessResult->choice.bitstring.buf = value->value.bitString.buf;
				int size = (value->value.bitString.size / 8) + ((value->value.bitString.size % 8) > 0);
				int unused = 8 - (value->value.bitString.size % 8);
				accessResult->choice.bitstring.size = size; /* size in bytes */
				accessResult->choice.bitstring.bits_unused = unused;
			}
            break;
        case MMS_BOOLEAN:
            accessResult->present = AccessResult_PR_boolean;
            accessResult->choice.boolean = value->value.boolean;
            break;
        case MMS_FLOAT:
            accessResult->present = AccessResult_PR_floatingpoint;

            mmsMsg_createFloatData(value, &accessResult->choice.floatingpoint.size,
                    &accessResult->choice.floatingpoint.buf);

            break;
        case MMS_UTC_TIME:
            accessResult->present = AccessResult_PR_utctime;
            accessResult->choice.utctime.buf = (uint8_t*) malloc(8);
            memcpy(accessResult->choice.utctime.buf, value->value.utcTime, 8);
            accessResult->choice.utctime.size = 8;
            break;
        case MMS_INTEGER:
            accessResult->present = AccessResult_PR_integer;
            asn_long2INTEGER(&accessResult->choice.integer, (long) MmsValue_toInt32(value));
            break;
        case MMS_UNSIGNED:
            accessResult->present = AccessResult_PR_unsigned;
            asn_long2INTEGER(&accessResult->choice.Unsigned, (long) MmsValue_toInt32(value));
            break;
        case MMS_VISIBLE_STRING:
            accessResult->present = AccessResult_PR_visiblestring;
            if (value->value.visibleString.buf == NULL )
                accessResult->choice.visiblestring.size = 0;
            else {
                accessResult->choice.visiblestring.buf = (uint8_t*) value->value.visibleString.buf;
                accessResult->choice.visiblestring.size = strlen(value->value.visibleString.buf);
            }
            break;

        case MMS_STRING:
            accessResult->present = AccessResult_PR_mMSString;
            if (value->value.visibleString.buf == NULL ) {
                accessResult->choice.mMSString.size = 0;
            }
            else {
                accessResult->choice.mMSString.buf = (uint8_t*) value->value.visibleString.buf;
                accessResult->choice.mMSString.size = strlen(value->value.visibleString.buf);
            }
            break;

        case MMS_BINARY_TIME:
            accessResult->present = AccessResult_PR_binarytime;
            accessResult->choice.binarytime.size = value->value.binaryTime.size;
            accessResult->choice.binarytime.buf = value->value.binaryTime.buf;
            break;

        case MMS_OCTET_STRING:
            accessResult->present = AccessResult_PR_octetstring;
            if (value->value.octetString.buf != NULL ) {
                accessResult->choice.octetstring.buf = value->value.octetString.buf;
                accessResult->choice.octetstring.size = value->value.octetString.size;
            }
            else
                accessResult->choice.octetstring.size = 0;
            break;

        default:
            accessResult->present = AccessResult_PR_failure;

            asn_long2INTEGER(&accessResult->choice.failure, DataAccessError_typeinconsistent);

            if (1)
                printf("MMS read: unknown value type %i in result\n", value->type);
            break;
        }
    }
}

AccessResult_t**
mmsMsg_createAccessResultsList(MmsPdu_t* mmsPdu, int resultsCount)
{
    ReadResponse_t* readResponse =
            &(mmsPdu->choice.confirmedResponsePdu.confirmedServiceResponse.choice.read);

    readResponse->listOfAccessResult.list.size = resultsCount;
    readResponse->listOfAccessResult.list.count = resultsCount;
    readResponse->listOfAccessResult.list.array = (AccessResult_t**) calloc(resultsCount, sizeof(AccessResult_t*));

    int i;

    for (i = 0; i < resultsCount; i++) {
        readResponse->listOfAccessResult.list.array[i] = (AccessResult_t*) calloc(1, sizeof(AccessResult_t));
    }

    AccessResult_t** accessResultList = readResponse->listOfAccessResult.list.array;

    return accessResultList;
}

static void
deleteDataElement(Data_t* dataElement)
{
    if (dataElement == NULL ) {
        printf("deleteDataElement NULL argument\n");
        return;
    }

    if (dataElement->present == Data_PR_structure) {
        int elementCount = dataElement->choice.structure->list.count;

        int i;
        for (i = 0; i < elementCount; i++) {
            deleteDataElement(dataElement->choice.structure->list.array[i]);
        }

        free(dataElement->choice.structure->list.array);
        free(dataElement->choice.structure);
    }
    else if (dataElement->present == Data_PR_array) {
        int elementCount = dataElement->choice.array->list.count;

        int i;
        for (i = 0; i < elementCount; i++) {
            deleteDataElement(dataElement->choice.array->list.array[i]);
        }

        free(dataElement->choice.array->list.array);
        free(dataElement->choice.array);
    }
    else if (dataElement->present == Data_PR_floatingpoint) {
        free(dataElement->choice.floatingpoint.buf);
    }
    else if (dataElement->present == Data_PR_utctime) {
        free(dataElement->choice.utctime.buf);
    }

    free(dataElement);
}

void
mmsMsg_deleteAccessResultList(AccessResult_t** accessResult, int variableCount)
{
    int i;

    for (i = 0; i < variableCount; i++) {

        if (accessResult[i]->present == AccessResult_PR_structure) {
            int elementCount = accessResult[i]->choice.structure.list.count;

            int j;

            for (j = 0; j < elementCount; j++) {
                deleteDataElement(accessResult[i]->choice.structure.list.array[j]);
            }

            free(accessResult[i]->choice.structure.list.array);
        }
        else if (accessResult[i]->present == AccessResult_PR_array) {
            int elementCount = accessResult[i]->choice.array.list.count;

            int j;

            for (j = 0; j < elementCount; j++) {
                deleteDataElement(accessResult[i]->choice.array.list.array[j]);
            }

            free(accessResult[i]->choice.array.list.array);
        }
        else if (accessResult[i]->present == AccessResult_PR_integer)
            free(accessResult[i]->choice.integer.buf);
        else if (accessResult[i]->present == AccessResult_PR_unsigned)
            free(accessResult[i]->choice.Unsigned.buf);
        else if (accessResult[i]->present == AccessResult_PR_floatingpoint)
            free(accessResult[i]->choice.floatingpoint.buf);
        else if (accessResult[i]->present == AccessResult_PR_utctime)
            free(accessResult[i]->choice.utctime.buf);
        else if (accessResult[i]->present == AccessResult_PR_failure)
            free(accessResult[i]->choice.failure.buf);

        free(accessResult[i]);
    }

    free(accessResult);
}


char*
mmsMsg_createStringFromAsnIdentifier(Identifier_t identifier)
{
    char* str = createStringFromBuffer(identifier.buf, identifier.size);

    return str;
}


void
mmsMsg_copyAsn1IdentifierToStringBuffer(Identifier_t identifier, char* buffer, int bufSize)
{
    if (identifier.size < bufSize) {
        memcpy(buffer, identifier.buf, identifier.size);
        buffer[identifier.size] = 0;
    }
    else {

        if (DEBUG_MMS_SERVER || DEBUG_MMS_CLIENT)
            printf("MMS_COMMON: mms_common_msg.c: ASN1 identifier to long!\n");

        buffer[0] = 0;
    }
}
