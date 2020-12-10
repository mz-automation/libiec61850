/*
 *  mms_server_common.c
 *
 *  Copyright 2013-2020 Michael Zillgith
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
#include "mms_server_internal.h"

/* write_out function required for ASN.1 encoding */
int
mmsServer_write_out(const void *buffer, size_t size, void *app_key)
{
    ByteBuffer* writeBuffer = (ByteBuffer*) app_key;

    int appendedBytes = ByteBuffer_append(writeBuffer, (uint8_t*) buffer, size);

    if (appendedBytes == -1) {
        if (DEBUG_MMS_SERVER)
            printf("MMS_SERVER: message exceeds maximum PDU size!\n");
    }

    return appendedBytes;
}

MmsPdu_t*
mmsServer_createConfirmedResponse(uint32_t invokeId)
{
	MmsPdu_t* mmsPdu = (MmsPdu_t*) GLOBAL_CALLOC(1, sizeof(MmsPdu_t));

	mmsPdu->present = MmsPdu_PR_confirmedResponsePdu;

	asn_long2INTEGER(&(mmsPdu->choice.confirmedResponsePdu.invokeID),
		invokeId);

	return mmsPdu;
}

static void
mapErrorTypeToErrorClass(MmsError errorType, uint8_t* tag, uint8_t* value)
{
    switch (errorType) {

    case MMS_ERROR_ACCESS_OBJECT_ACCESS_UNSUPPORTED:
        *tag = 0x87; /* access */
        *value = 1;
        break;

    case MMS_ERROR_ACCESS_OBJECT_NON_EXISTENT:
        *tag = 0x87; /* access */
        *value = 2;
        break;

    case MMS_ERROR_ACCESS_OBJECT_ACCESS_DENIED:
        *tag = 0x87; /* access */
        *value = 3;
        break;

    case MMS_ERROR_SERVICE_OTHER:
        *tag = 0x84; /* service */
        *value = 0;
        break;

    case MMS_ERROR_SERVICE_OBJECT_CONSTRAINT_CONFLICT:
         *tag = 0x84; /* service */
         *value = 5;
         break;

    case MMS_ERROR_DEFINITION_OTHER:
        *tag = 0x82; /* definition */
        *value = 0;
        break;

    case MMS_ERROR_DEFINITION_OBJECT_UNDEFINED:
        *tag = 0x82; /* definition */
        *value = 1;
        break;

    case MMS_ERROR_DEFINITION_TYPE_UNSUPPORTED:
        *tag = 0x82; /* definition */
        *value = 3;
        break;

    case MMS_ERROR_DEFINITION_OBJECT_EXISTS:
        *tag = 0x82; /* definition */
        *value = 5;
        break;

    case MMS_ERROR_FILE_OTHER:
        *tag = 0x8b; /* file */
        *value = 0;
        break;

    case MMS_ERROR_FILE_FILENAME_AMBIGUOUS:
        *tag = 0x8b; /* file */
        *value = 1;
        break;

    case MMS_ERROR_FILE_POSITION_INVALID:
        *tag = 0x8b; /* file */
        *value = 5;
        break;

    case MMS_ERROR_FILE_FILE_ACCESS_DENIED:
        *tag = 0x8b; /* file */
        *value = 6;
        break;

    case MMS_ERROR_FILE_FILE_NON_EXISTENT:
        *tag = 0x8b; /* file */
        *value = 7;
        break;

    case MMS_ERROR_FILE_DUPLICATE_FILENAME:
        *tag = 0x8b; /* file */
        *value = 8;
        break;

    case MMS_ERROR_FILE_INSUFFICIENT_SPACE_IN_FILESTORE:
        *tag = 0x8b; /* file */
        *value = 9;
        break;

    case MMS_ERROR_RESOURCE_OTHER:
        *tag = 0x83; /* resource */
        *value = 0;
        break;

    case MMS_ERROR_RESOURCE_CAPABILITY_UNAVAILABLE:
        *tag = 0x83; /* resource */
        *value = 4;
        break;

    default:

        if (DEBUG_MMS_SERVER)
            printf("MMS_SERVER: unknown errorType!\n");

        *tag = 0x8c; /* others */
        *value = 0;
        break;

    }

}

void
mmsServer_createServiceErrorPduWithServiceSpecificInfo(uint32_t invokeId, ByteBuffer* response,
        MmsError errorType, uint8_t* serviceSpecificInfo, int serviceSpecficInfoLength)
{
    /* determine encoded size */

    uint32_t invokeIdSize = BerEncoder_UInt32determineEncodedSize(invokeId) + 2;

    uint32_t specificInfoSize = 0;

    if (serviceSpecificInfo != NULL)
        specificInfoSize = 1 + BerEncoder_determineLengthSize(serviceSpecficInfoLength)
                + serviceSpecficInfoLength;

    uint32_t serviceErrorContentSize = 5 /* errorClass */ + specificInfoSize;

    uint32_t serviceErrorSize = 1 + BerEncoder_determineLengthSize(serviceErrorContentSize) +
            serviceErrorContentSize;

    uint32_t confirmedErrorContentSize = serviceErrorSize + invokeIdSize;

    /* encode */
    uint8_t* buffer = response->buffer;
    int bufPos = response->size;

    bufPos = BerEncoder_encodeTL(0xa2, confirmedErrorContentSize, buffer, bufPos);

    bufPos = BerEncoder_encodeTL(0x80, invokeIdSize - 2, buffer, bufPos); /* invokeID */
    bufPos = BerEncoder_encodeUInt32(invokeId, buffer, bufPos);

    bufPos = BerEncoder_encodeTL(0xa2, serviceErrorContentSize, buffer, bufPos); /* serviceError */
    bufPos = BerEncoder_encodeTL(0xa0, 3, buffer, bufPos); /* serviceError */

    uint8_t errorCodeTag;
    uint8_t errorCodeValue;

    mapErrorTypeToErrorClass(errorType, &errorCodeTag, &errorCodeValue);

    buffer[bufPos++] = errorCodeTag;
    buffer[bufPos++] = 1;
    buffer[bufPos++] = errorCodeValue;

    if (serviceSpecificInfo != NULL)
        bufPos = BerEncoder_encodeOctetString(0xa3, serviceSpecificInfo, serviceSpecficInfoLength,
                buffer, bufPos);

    response->size = bufPos;
}

void /* Confirmed service error (ServiceError) */
mmsMsg_createServiceErrorPdu(uint32_t invokeId, ByteBuffer* response, MmsError errorType)
{
    mmsServer_createServiceErrorPduWithServiceSpecificInfo(invokeId, response, errorType, NULL, 0);
}

void
mmsMsg_createInitiateErrorPdu(ByteBuffer* response, uint8_t initiateErrorCode)
{
    /* determine encoded size */

    uint32_t serviceErrorContentSize = 5; /* errorClass */

    /* encode */
    uint8_t* buffer = response->buffer;
    int bufPos = response->size;

    bufPos = BerEncoder_encodeTL(0xaa, serviceErrorContentSize, buffer, bufPos); /* serviceError */
    bufPos = BerEncoder_encodeTL(0xa0, 3, buffer, bufPos); /* serviceError */

    buffer[bufPos++] = 8; /* initiate */
    buffer[bufPos++] = 1;
    buffer[bufPos++] = initiateErrorCode;

    response->size = bufPos;
}

bool
mmsServer_isIndexAccess(AlternateAccess_t* alternateAccess)
{
    if (alternateAccess->list.array[0]->present == AlternateAccess__Member_PR_unnamed) {
        if ((alternateAccess->list.array[0]->choice.unnamed->choice.selectAccess.present
                == AlternateAccessSelection__selectAccess_PR_index) ||
                (alternateAccess->list.array[0]->choice.unnamed->choice.selectAccess.present
                        == AlternateAccessSelection__selectAccess_PR_indexRange))
        {
            return true;
        }
    }

    return false;
}

bool
mmsServer_isComponentAccess(AlternateAccess_t* alternateAccess)
{
    if (alternateAccess->list.array[0]->present
            == AlternateAccess__Member_PR_unnamed) {
        if (alternateAccess->list.array[0]->choice.unnamed->choice.selectAccess.present
                == AlternateAccessSelection__selectAccess_PR_component) {
            return true;
        }
    }

    return false;
}

int
mmsServer_getLowIndex(AlternateAccess_t* alternateAccess)
{
	if (alternateAccess->list.array[0]->choice.unnamed->choice.selectAccess.present
				== AlternateAccessSelection__selectAccess_PR_index)
	{
		long index;
		asn_INTEGER2long(
				&alternateAccess->list.array[0]->choice.unnamed->choice.selectAccess.choice.index,
				&index);

		return (int) index;
	}

	if (alternateAccess->list.array[0]->choice.unnamed->choice.selectAccess.present
				== AlternateAccessSelection__selectAccess_PR_indexRange)
	{
		long index;
		asn_INTEGER2long(
				&alternateAccess->list.array[0]->choice.unnamed->choice.selectAccess.choice.indexRange.lowIndex,
				&index);

		return (int) index;
	}

	return -1;
}

int
mmsServer_getNumberOfElements(AlternateAccess_t* alternateAccess)
{
	if (alternateAccess->list.array[0]->choice.unnamed->choice.selectAccess.present
				== AlternateAccessSelection__selectAccess_PR_indexRange)
	{
		long number;

		asn_INTEGER2long(
				&alternateAccess->list.array[0]->choice.unnamed->choice.selectAccess.choice.indexRange.numberOfElements,
				&number);

		return (int) number;
	}

	return 0;
}

MmsNamedVariableList
mmsServer_getNamedVariableListWithName(LinkedList namedVariableLists, const char* variableListName)
{
    MmsNamedVariableList variableList = NULL;

    LinkedList element = LinkedList_getNext(namedVariableLists);

    while (element != NULL) {
        MmsNamedVariableList varList = (MmsNamedVariableList) element->data;

        if (strcmp(MmsNamedVariableList_getName(varList), variableListName) == 0) {
            variableList = varList;
            break;
        }

        element = LinkedList_getNext(element);
    }

    return variableList;
}


void
mmsServer_deleteVariableList(LinkedList namedVariableLists, char* variableListName)
{
	LinkedList previousElement = namedVariableLists;
	LinkedList element = LinkedList_getNext(namedVariableLists);

	while (element != NULL ) {
		MmsNamedVariableList varList = (MmsNamedVariableList) element->data;

		if (strcmp(MmsNamedVariableList_getName(varList), variableListName)
				== 0) {
			previousElement->next = element->next;
			GLOBAL_FREEMEM(element);
			MmsNamedVariableList_destroy(varList);

			break;
		}

		previousElement = element;
		element = LinkedList_getNext(element);
	}
}


