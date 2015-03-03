/*
 *  mms_server_common.c
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
#include "mms_server_internal.h"

/* write_out function required for ASN.1 encoding */
int
mmsServer_write_out(const void *buffer, size_t size, void *app_key)
{
    ByteBuffer* writeBuffer = (ByteBuffer*) app_key;
    return ByteBuffer_append(writeBuffer, (uint8_t*) buffer, size);
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


void
mmsServer_createConfirmedErrorPdu(uint32_t invokeId, ByteBuffer* response, MmsError errorType)
{
	MmsPdu_t* mmsPdu = (MmsPdu_t*) GLOBAL_CALLOC(1, sizeof(MmsPdu_t));
	mmsPdu->present = MmsPdu_PR_confirmedErrorPDU;

	asn_long2INTEGER(&(mmsPdu->choice.confirmedErrorPDU.invokeID),
			invokeId);

	if (errorType == MMS_ERROR_ACCESS_OBJECT_NON_EXISTENT) {
		mmsPdu->choice.confirmedErrorPDU.serviceError.errorClass.present =
				ServiceError__errorClass_PR_access;

		asn_long2INTEGER(&mmsPdu->choice.confirmedErrorPDU.serviceError.errorClass.choice.access,
						ServiceError__errorClass__access_objectnonexistent);
	}
	else if (errorType == MMS_ERROR_ACCESS_OBJECT_ACCESS_DENIED) {
		mmsPdu->choice.confirmedErrorPDU.serviceError.errorClass.present =
				ServiceError__errorClass_PR_access;

		asn_long2INTEGER(&mmsPdu->choice.confirmedErrorPDU.serviceError.errorClass.choice.access,
				ServiceError__errorClass__access_objectaccessdenied);
	}
	else if (errorType == MMS_ERROR_ACCESS_OBJECT_ACCESS_UNSUPPORTED) {
		mmsPdu->choice.confirmedErrorPDU.serviceError.errorClass.present =
				ServiceError__errorClass_PR_access;

		asn_long2INTEGER(&mmsPdu->choice.confirmedErrorPDU.serviceError.errorClass.choice.access,
				ServiceError__errorClass__access_objectaccessunsupported);
	}
	else if (errorType == MMS_ERROR_SERVICE_OTHER) {
	    mmsPdu->choice.confirmedErrorPDU.serviceError.errorClass.present =
	                            ServiceError__errorClass_PR_service;
        asn_long2INTEGER(&mmsPdu->choice.confirmedErrorPDU.serviceError.errorClass.choice.access,
                        ServiceError__errorClass__service_other);
	}
	else if (errorType == MMS_ERROR_DEFINITION_OTHER) {
	    mmsPdu->choice.confirmedErrorPDU.serviceError.errorClass.present =
                        ServiceError__errorClass_PR_definition;
        asn_long2INTEGER(&mmsPdu->choice.confirmedErrorPDU.serviceError.errorClass.choice.access,
                        ServiceError__errorClass__definition_other);
	}
	else if (errorType == MMS_ERROR_DEFINITION_OBJECT_EXISTS) {
	    mmsPdu->choice.confirmedErrorPDU.serviceError.errorClass.present =
                        ServiceError__errorClass_PR_definition;
        asn_long2INTEGER(&mmsPdu->choice.confirmedErrorPDU.serviceError.errorClass.choice.access,
                        ServiceError__errorClass__definition_objectexists);
	}
	else if (errorType == MMS_ERROR_DEFINITION_OBJECT_UNDEFINED) {
        mmsPdu->choice.confirmedErrorPDU.serviceError.errorClass.present =
                        ServiceError__errorClass_PR_definition;
        asn_long2INTEGER(&mmsPdu->choice.confirmedErrorPDU.serviceError.errorClass.choice.access,
                        ServiceError__errorClass__definition_objectundefined);
	}
	else if (errorType == MMS_ERROR_DEFINITION_TYPE_UNSUPPORTED) {
        mmsPdu->choice.confirmedErrorPDU.serviceError.errorClass.present =
                        ServiceError__errorClass_PR_definition;
        asn_long2INTEGER(&mmsPdu->choice.confirmedErrorPDU.serviceError.errorClass.choice.access,
                        ServiceError__errorClass__definition_typeunsupported);
    }
	else if (errorType == MMS_ERROR_FILE_FILE_NON_EXISTENT) {
        mmsPdu->choice.confirmedErrorPDU.serviceError.errorClass.present =
                        ServiceError__errorClass_PR_file;
        asn_long2INTEGER(&mmsPdu->choice.confirmedErrorPDU.serviceError.errorClass.choice.access,
                        ServiceError__errorClass__file_filenonexistent);
	}
	else if (errorType == MMS_ERROR_FILE_OTHER) {
        mmsPdu->choice.confirmedErrorPDU.serviceError.errorClass.present =
                        ServiceError__errorClass_PR_file;
        asn_long2INTEGER(&mmsPdu->choice.confirmedErrorPDU.serviceError.errorClass.choice.access,
                        ServiceError__errorClass__file_other);
	}
    else if (errorType == MMS_ERROR_RESOURCE_OTHER) {
        mmsPdu->choice.confirmedErrorPDU.serviceError.errorClass.present =
                        ServiceError__errorClass_PR_resource;
        asn_long2INTEGER(&mmsPdu->choice.confirmedErrorPDU.serviceError.errorClass.choice.access,
                        ServiceError__errorClass__resource_other);
    }
    else if (errorType == MMS_ERROR_RESOURCE_CAPABILITY_UNAVAILABLE) {
        mmsPdu->choice.confirmedErrorPDU.serviceError.errorClass.present =
                        ServiceError__errorClass_PR_resource;
        asn_long2INTEGER(&mmsPdu->choice.confirmedErrorPDU.serviceError.errorClass.choice.access,
                        ServiceError__errorClass__resource_capabilityunavailable);
    }

	der_encode(&asn_DEF_MmsPdu, mmsPdu,
			mmsServer_write_out, (void*) response);

	asn_DEF_MmsPdu.free_struct(&asn_DEF_MmsPdu, mmsPdu, 0);
}

int
mmsServer_isIndexAccess(AlternateAccess_t* alternateAccess)
{
	if (alternateAccess->list.array[0]->present == AlternateAccess__Member_PR_unnamed) {
		if ((alternateAccess->list.array[0]->choice.unnamed->choice.selectAccess.present
				== AlternateAccessSelection__selectAccess_PR_index) ||
			(alternateAccess->list.array[0]->choice.unnamed->choice.selectAccess.present
				== AlternateAccessSelection__selectAccess_PR_indexRange))
		{
			return 1;
		}
		else
			return 0;
	}
	else
		return 0;
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


