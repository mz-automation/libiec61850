/*
 *  mms_read_service.c
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

#include "mms_server_internal.h"
#include "mms_common_internal.h"
#include "mms_value_internal.h"

#include "linked_list.h"

#include "ber_encoder.h"

/**********************************************************************************************
 * MMS Read Service
 *********************************************************************************************/

typedef struct sVarAccessSpec {
	bool isNamedVariableList;
	int specific; /* 0 - vmd, 1 - domain, 2 - association */

	char* itemId;
	char* domainId;
} VarAccessSpec;

static MmsValue*
addNamedVariableValue(MmsVariableSpecification* namedVariable, MmsServerConnection connection,
        MmsDomain* domain, char* itemId)
{
    MmsValue* value = NULL;

    if (namedVariable->type == MMS_STRUCTURE) {

        value = mmsServer_getValue(connection->server, domain, itemId, connection, false);

        if (value != NULL)
            goto exit_function;
        else {

            int componentCount = namedVariable->typeSpec.structure.elementCount;

            value = MmsValue_createEmptyStructure(componentCount);

            value->deleteValue = 1;

            int i;

            for (i = 0; i < componentCount; i++) {
                char newNameIdStr[65];

                StringUtils_createStringInBuffer(newNameIdStr, 3, itemId, "$",
                        namedVariable->typeSpec.structure.elements[i]->name);

                MmsValue* element =
                        addNamedVariableValue(namedVariable->typeSpec.structure.elements[i],
                                connection, domain, newNameIdStr);

                if (element == NULL) {
                    MmsValue_delete(value);
                    value = NULL;
                    break;
                }

                MmsValue_setElement(value, i, element);
            }
        }
    }
    else {
        value = mmsServer_getValue(connection->server, domain, itemId, connection, false);
    }

exit_function:
    return value;
}

static void
addComplexValueToResultList(MmsVariableSpecification* namedVariable,
                                LinkedList typedValues, MmsServerConnection connection,
                                MmsDomain* domain, char* nameIdStr)
{

    MmsValue* value = addNamedVariableValue(namedVariable, connection, domain, nameIdStr);

    if (value != NULL)
        LinkedList_add(typedValues, value);
}


static void
appendValueToResultList(MmsValue* value, LinkedList values)
{

	if (value != NULL )
		LinkedList_add(values, value);
}

static void
appendErrorToResultList(LinkedList values, MmsDataAccessError errorType) {
    MmsValue* value = MmsValue_newDataAccessError(errorType);
    MmsValue_setDeletable(value);
    appendValueToResultList(value, values);
}

static void
deleteValueList(LinkedList values)
{
    LinkedList value = LinkedList_getNext(values);

	while (value != NULL ) {
	    MmsValue* typedValue = (MmsValue*) (value->data);

		MmsValue_deleteConditional(typedValue);

		value = LinkedList_getNext(value);
	}

	LinkedList_destroyStatic(values);
}

static bool
isAccessToArrayComponent(AlternateAccess_t* alternateAccess)
{
    if (alternateAccess->list.array[0]->choice.unnamed->choice.selectAlternateAccess.alternateAccess != NULL)
    {
        if (alternateAccess->list.array[0]->choice.unnamed->
                choice.selectAlternateAccess.alternateAccess->list.array[0]->choice.unnamed->choice.selectAlternateAccess.accessSelection.present ==
                        AlternateAccessSelection__selectAlternateAccess__accessSelection_PR_component)
        {
            return true;
        }
        else
            return false;
    }
    else
        return false;
}

static MmsValue*
getComponent(MmsServerConnection connection, MmsDomain* domain, AlternateAccess_t* alternateAccess, MmsVariableSpecification* namedVariable, char* variableName)
{
    MmsValue* retValue = NULL;

    if (mmsServer_isComponentAccess(alternateAccess)) {
        Identifier_t component =
                alternateAccess->list.array[0]->choice.unnamed->choice.selectAccess.choice.component;

        if (component.size > 129)
            goto exit_function;

        if (namedVariable->type == MMS_STRUCTURE) {

            int i;

            for (i = 0; i < namedVariable->typeSpec.structure.elementCount; i++) {

                if ((int) strlen(namedVariable->typeSpec.structure.elements[i]->name)
                        == component.size) {
                    if (!strncmp(namedVariable->typeSpec.structure.elements[i]->name,
                            (char*) component.buf, component.size))
                    {
                        if (strlen(variableName) + component.size < 199) {

                            strcat(variableName, "$");
                            strncat(variableName, (const char*) component.buf, component.size);

                            if (alternateAccess->list.array[0]->choice.unnamed->choice.selectAlternateAccess.alternateAccess
                                    != NULL) {
                                retValue =
                                        getComponent(connection, domain,
                                                alternateAccess->list.array[0]->choice.unnamed->choice.selectAlternateAccess.alternateAccess,
                                                namedVariable->typeSpec.structure.elements[i],
                                                variableName);
                            }
                            else {
                                retValue = mmsServer_getValue(connection->server, domain, variableName, connection, false);
                            }
                        }
                    }
                }
            }
        }
    }

exit_function:
    return retValue;
}

static MmsValue*
getComponentOfArrayElement(AlternateAccess_t* alternateAccess, MmsVariableSpecification* namedVariable,
        MmsValue* structuredValue)
{
    MmsValue* retValue = NULL;

    if (isAccessToArrayComponent(alternateAccess))
    {
        Identifier_t component = alternateAccess->list.array[0]->choice.unnamed->choice.selectAlternateAccess.alternateAccess
                ->list.array[0]->choice.unnamed->choice.selectAccess.choice.component;

        if (component.size > 129)
            goto exit_function;

        MmsVariableSpecification* structSpec;

        if (namedVariable->type == MMS_ARRAY)
            structSpec = namedVariable->typeSpec.array.elementTypeSpec;
        else if (namedVariable->type == MMS_STRUCTURE)
            structSpec = namedVariable;
        else
            goto exit_function;

        int i;
        for (i = 0; i < structSpec->typeSpec.structure.elementCount; i++) {

            if ((int) strlen(structSpec->typeSpec.structure.elements[i]->name)
                    == component.size) {
                if (strncmp(structSpec->typeSpec.structure.elements[i]->name,
                        (char*) component.buf, component.size) == 0) {
                    MmsValue* value = MmsValue_getElement(structuredValue, i);

                    if (isAccessToArrayComponent(
                            alternateAccess->list.array[0]->choice.unnamed->choice.selectAlternateAccess.alternateAccess)) {
                        retValue =
                                getComponentOfArrayElement(
                                        alternateAccess->list.array[0]->choice.unnamed->choice.selectAlternateAccess.alternateAccess,
                                        structSpec->typeSpec.structure.elements[i],
                                        value);
                    }
                    else
                        retValue = value;

                    goto exit_function;
                }
            }
        }
    }

exit_function:
    return retValue;
}

static void
alternateArrayAccess(MmsServerConnection connection,
		AlternateAccess_t* alternateAccess, MmsDomain* domain,
		char* itemId, LinkedList values,
		MmsVariableSpecification* namedVariable)
{
	if (mmsServer_isIndexAccess(alternateAccess))
	{
		int lowIndex = mmsServer_getLowIndex(alternateAccess);
		int numberOfElements = mmsServer_getNumberOfElements(alternateAccess);

		int index = lowIndex;

		MmsValue* arrayValue = mmsServer_getValue(connection->server, domain, itemId, connection, false);

		if (arrayValue != NULL) {

	        MmsValue* value = NULL;

			if (numberOfElements == 0)
			    if (isAccessToArrayComponent(alternateAccess)) {
			        if (namedVariable->typeSpec.array.elementTypeSpec->type == MMS_STRUCTURE) {
			            MmsValue* structValue = MmsValue_getElement(arrayValue, index);

			            if (structValue != NULL)
			                value = getComponentOfArrayElement(alternateAccess,
			                        namedVariable, structValue);
			        }
			    }
			    else {
			        value = MmsValue_getElement(arrayValue, index);
			    }
			else {
				value = MmsValue_createEmptyArray(numberOfElements);

				MmsValue_setDeletable(value);

				int resultIndex = 0;
				while (index < lowIndex + numberOfElements) {
					MmsValue* elementValue = NULL;

					elementValue = MmsValue_getElement(arrayValue, index);

					if (!MmsValue_isDeletable(elementValue)) {
						elementValue = MmsValue_clone(elementValue);
						elementValue->deleteValue = 1;
					}

					MmsValue_setElement(value, resultIndex, elementValue);

					index++;
					resultIndex++;
				}
			}

			if (value)
			    appendValueToResultList(value, values);
			else
			    appendErrorToResultList(values, DATA_ACCESS_ERROR_OBJECT_NONE_EXISTENT);
		}
		else  /* access error */
			appendErrorToResultList(values, DATA_ACCESS_ERROR_OBJECT_NONE_EXISTENT);

	}
	else { /* invalid access */
		if (DEBUG_MMS_SERVER) printf("Invalid alternate access\n");

        appendErrorToResultList(values, DATA_ACCESS_ERROR_OBJECT_NONE_EXISTENT);
	}
}

static void
addNamedVariableToResultList(MmsVariableSpecification* namedVariable, MmsDomain* domain, char* nameIdStr,
		LinkedList /*<MmsValue>*/ values, MmsServerConnection connection, AlternateAccess_t* alternateAccess, bool isAccessToSingleVariable)
{
    if (namedVariable != NULL) {

        if (DEBUG_MMS_SERVER)
            printf("MMS read: found named variable %s with search string %s\n",
                    namedVariable->name, nameIdStr);

        if (namedVariable->type == MMS_STRUCTURE) {

            MmsValue* value = mmsServer_getValue(connection->server, domain, nameIdStr, connection, isAccessToSingleVariable);

            if (alternateAccess != NULL) {

                char variableName[200];
                variableName[0] = 0;
                strcat(variableName, nameIdStr);

                value = getComponent(connection, domain, alternateAccess, namedVariable, variableName);

                if (value != NULL) {
                    appendValueToResultList(value, values);
                }
                else {
                    appendErrorToResultList(values, DATA_ACCESS_ERROR_OBJECT_NONE_EXISTENT);
                }
            }
            else {
                if (value != NULL) {
                    appendValueToResultList(value, values);
                }
                else {
                    addComplexValueToResultList(namedVariable,
                            values, connection, domain, nameIdStr);
                }
            }
        }
        else if (namedVariable->type == MMS_ARRAY) {

            if (alternateAccess != NULL) {
                alternateArrayAccess(connection, alternateAccess, domain,
                        nameIdStr, values, namedVariable);
            }
            else { /* return complete array */
                MmsValue* value = mmsServer_getValue(connection->server, domain, nameIdStr, connection, isAccessToSingleVariable);
                appendValueToResultList(value, values);
            }
        }
        else {

            if (alternateAccess != NULL) {
                appendErrorToResultList(values, DATA_ACCESS_ERROR_OBJECT_NONE_EXISTENT);
            }
            else {
                MmsValue* value = mmsServer_getValue(connection->server, domain, nameIdStr, connection, isAccessToSingleVariable);

                if (value == NULL) {
                    if (DEBUG_MMS_SERVER)
                        printf("MMS read: value of known variable is not found. Maybe illegal access to array element!\n");

                    appendErrorToResultList(values, DATA_ACCESS_ERROR_OBJECT_NONE_EXISTENT);
                }
                else
                    appendValueToResultList(value, values);
            }
        }
    }
    else
        appendErrorToResultList(values, DATA_ACCESS_ERROR_OBJECT_NONE_EXISTENT);
}


static bool
isSpecWithResult(ReadRequest_t* read)
{
	if (read->specificationWithResult != NULL)
		if (*(read->specificationWithResult) != false)
			return true;

	return false;
}

static int
encodeVariableAccessSpecification(VarAccessSpec* accessSpec, uint8_t* buffer, int bufPos, bool encode)
{
	/* determine size */
    uint32_t varAccessSpecSize = 0;

	uint32_t itemIdLen = strlen(accessSpec->itemId);

	varAccessSpecSize += itemIdLen + BerEncoder_determineLengthSize(itemIdLen) + 1;

	if (accessSpec->domainId != NULL) {
	    uint32_t domainIdLen = strlen(accessSpec->domainId);

		varAccessSpecSize += domainIdLen + BerEncoder_determineLengthSize(domainIdLen) + 1;
	}

	uint32_t specificityLength = varAccessSpecSize;

	varAccessSpecSize += 1 + BerEncoder_determineLengthSize(specificityLength);

	uint32_t variableListNameLength = varAccessSpecSize;

	varAccessSpecSize += 1 + BerEncoder_determineLengthSize(variableListNameLength);

	uint32_t varAccessSpecLength = varAccessSpecSize;

	varAccessSpecSize += 1 + BerEncoder_determineLengthSize(varAccessSpecLength);

	if (encode == false) {
	    bufPos = varAccessSpecSize;
	    goto exit_function;
	}

	/* encode to buffer */
	bufPos = BerEncoder_encodeTL(0xa0, varAccessSpecLength, buffer, bufPos);

	if (accessSpec->isNamedVariableList == true) {

		bufPos = BerEncoder_encodeTL(0xa1, variableListNameLength, buffer, bufPos);

		if (accessSpec->specific == 0) { /* vmd-specific */
			bufPos = BerEncoder_encodeTL(0xa0, specificityLength, buffer, bufPos);
		}
		else if (accessSpec->specific == 1) { /* domain-specific */
			bufPos = BerEncoder_encodeTL(0xa1, specificityLength, buffer, bufPos);

		}
		else { /* association-specific */
			bufPos = BerEncoder_encodeTL(0xa2, specificityLength, buffer, bufPos);
		}


		if (accessSpec->domainId != NULL)
			bufPos = BerEncoder_encodeStringWithTag(0x1a, accessSpec->domainId, buffer, bufPos);

		bufPos = BerEncoder_encodeStringWithTag(0x1a, accessSpec->itemId, buffer, bufPos);
	}

exit_function:
	return bufPos;
}

static void
encodeReadResponse(MmsServerConnection connection,
        uint32_t invokeId, ByteBuffer* response, LinkedList values,
        VarAccessSpec* accessSpec)
{
    int i;

    int variableCount = LinkedList_size(values);

    uint32_t varAccessSpecSize = 0;

    if (accessSpec != NULL) {
        varAccessSpecSize = encodeVariableAccessSpecification(accessSpec, NULL, 0, false);
    }

    /* determine BER encoded message sizes */
    uint32_t accessResultSize = 0;

    /* iterate values list to determine encoded size  */
    LinkedList value = LinkedList_getNext(values);

    for (i = 0; i < variableCount; i++) {

        MmsValue* data = (MmsValue*) value->data;

        accessResultSize += MmsValue_encodeMmsData(data, NULL, 0, false);

        value = LinkedList_getNext(value);
    }

    uint32_t listOfAccessResultsLength = 1 +
            BerEncoder_determineLengthSize(accessResultSize) +
            accessResultSize;

    uint32_t confirmedServiceResponseContentLength = listOfAccessResultsLength + varAccessSpecSize;

    uint32_t confirmedServiceResponseLength = 1 +
            BerEncoder_determineLengthSize(confirmedServiceResponseContentLength) +
            confirmedServiceResponseContentLength;

    uint32_t invokeIdSize = BerEncoder_UInt32determineEncodedSize(invokeId) + 2;

    uint32_t confirmedResponseContentSize = confirmedServiceResponseLength + invokeIdSize;

    uint32_t mmsPduSize = 1 + BerEncoder_determineLengthSize(confirmedResponseContentSize) +
            confirmedResponseContentSize;

    /* Check if message would fit in the MMS PDU */
    if (mmsPduSize > connection->maxPduSize) {
        if (DEBUG_MMS_SERVER)
            printf("MMS read: message to large! send error PDU!\n");

        mmsMsg_createServiceErrorPdu(invokeId, response,
                MMS_ERROR_SERVICE_OTHER);

        goto exit_function;
    }

    /* encode message */

    uint8_t* buffer = response->buffer;
    int bufPos = 0;

    /* confirmed response PDU */
    bufPos = BerEncoder_encodeTL(0xa1, confirmedResponseContentSize, buffer, bufPos);

    /* invoke id */
    bufPos = BerEncoder_encodeTL(0x02, invokeIdSize - 2, buffer, bufPos);
    bufPos = BerEncoder_encodeUInt32(invokeId, buffer, bufPos);

    /* confirmed-service-response read */
    bufPos = BerEncoder_encodeTL(0xa4, confirmedServiceResponseContentLength, buffer, bufPos);

    /* encode variable access specification */
    if (accessSpec != NULL)
        bufPos = encodeVariableAccessSpecification(accessSpec, buffer, bufPos, true);

    /* encode list of access results */
    bufPos = BerEncoder_encodeTL(0xa1, accessResultSize, buffer, bufPos);

    /* encode access results */
    value = LinkedList_getNext(values);

    for (i = 0; i < variableCount; i++) {
        MmsValue* data = (MmsValue*) value->data;

        bufPos = MmsValue_encodeMmsData(data, buffer, bufPos, true);

        value = LinkedList_getNext(value);
    }

    response->size = bufPos;

    if (DEBUG_MMS_SERVER)
        printf("MMS read: sent message for request with id %u (size = %i)\n", invokeId, bufPos);

exit_function:
    return;
}

/**
 * \brief implements access to list of variables (multiple MMS variables)
 *
 * \param connection the client connection that received the request
 * \param read read request information
 * \param invokeId the invoke ID of the confirmed request PDU
 * \param response byte buffer to encode the response
 */
static void
handleReadListOfVariablesRequest(
		MmsServerConnection connection,
		ReadRequest_t* read,
		uint32_t invokeId,
		ByteBuffer* response)
{
	int variableCount = read->variableAccessSpecification.choice.listOfVariable.list.count;

	LinkedList /*<MmsValue>*/ values = LinkedList_create();

	if (isSpecWithResult(read)) { /* add specification to result */
		/* ignore - not required for IEC 61850 */
	}

	int i;

	for (i = 0; i < variableCount; i++) {
		VariableSpecification_t varSpec =
			read->variableAccessSpecification.choice.listOfVariable.list.array[i]->variableSpecification;

		AlternateAccess_t* alternateAccess =
			read->variableAccessSpecification.choice.listOfVariable.list.array[i]->alternateAccess;

		if (varSpec.present == VariableSpecification_PR_name) {

			if (varSpec.choice.name.present == ObjectName_PR_domainspecific) {
				char domainIdStr[65];
				char nameIdStr[65];

				mmsMsg_copyAsn1IdentifierToStringBuffer(varSpec.choice.name.choice.domainspecific.domainId,
				        domainIdStr, 65);

				mmsMsg_copyAsn1IdentifierToStringBuffer(varSpec.choice.name.choice.domainspecific.itemId,
				        nameIdStr, 65);

				MmsDomain* domain = MmsDevice_getDomain(MmsServer_getDevice(connection->server), domainIdStr);

				if (DEBUG_MMS_SERVER)
				    printf("MMS_SERVER: READ domainId: (%s) nameId: (%s)\n", domainIdStr, nameIdStr);

				if (domain == NULL) {
					if (DEBUG_MMS_SERVER)
					    printf("MMS_SERVER: READ domain %s not found!\n", domainIdStr);

					appendErrorToResultList(values, DATA_ACCESS_ERROR_OBJECT_NONE_EXISTENT);
				}
				else {
                    MmsVariableSpecification* namedVariable = MmsDomain_getNamedVariable(domain, nameIdStr);

                    if (namedVariable == NULL)
                        appendErrorToResultList(values, DATA_ACCESS_ERROR_OBJECT_NONE_EXISTENT);
                    else
                        addNamedVariableToResultList(namedVariable, domain, nameIdStr,
                            values, connection, alternateAccess, variableCount == 1);
				}
			}
#if (CONFIG_MMS_SUPPORT_VMD_SCOPE_NAMED_VARIABLES == 1)

			else if (varSpec.choice.name.present == ObjectName_PR_vmdspecific) {
			    char nameIdStr[65];

			    mmsMsg_copyAsn1IdentifierToStringBuffer(varSpec.choice.name.choice.vmdspecific, nameIdStr, 65);

			    if (DEBUG_MMS_SERVER)
			        printf("MMS_SERVER: READ vmd-specific nameId:%s\n", nameIdStr);

			    MmsVariableSpecification* namedVariable = MmsDevice_getNamedVariable(MmsServer_getDevice(connection->server), nameIdStr);

                if (namedVariable == NULL)
                    appendErrorToResultList(values, DATA_ACCESS_ERROR_OBJECT_NONE_EXISTENT);
                else
                    addNamedVariableToResultList(namedVariable, (MmsDomain*) MmsServer_getDevice(connection->server), nameIdStr,
                            values, connection, alternateAccess);

			}
#endif /* (CONFIG_MMS_SUPPORT_VMD_SCOPE_NAMED_VARIABLES == 1) */

			else {
                appendErrorToResultList(values, DATA_ACCESS_ERROR_OBJECT_NONE_EXISTENT);

				if (DEBUG_MMS_SERVER) printf("MMS_SERVER: READ object name type not supported!\n");
			}
		}
		else {
			if (DEBUG_MMS_SERVER) printf("MMS_SERVER: READ varspec type not supported!\n");
			mmsMsg_createMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_REQUEST_INVALID_ARGUMENT, response);
			goto exit;
		}
	}

	bool sendResponse = true;

	LinkedList valueElement = LinkedList_getNext(values);

	while (valueElement) {

	    MmsValue* value = (MmsValue*) LinkedList_getData(valueElement);

	    if (value) {
	        if (MmsValue_getType(value) == MMS_DATA_ACCESS_ERROR) {
	            if (MmsValue_getDataAccessError(value) == DATA_ACCESS_ERROR_NO_RESPONSE) {
	                sendResponse = false;
	                break;
	            }
	        }
	    }

	    valueElement = LinkedList_getNext(valueElement);
	}

	if (sendResponse)
	    encodeReadResponse(connection, invokeId, response, values, NULL);

exit:

    deleteValueList(values);
}

#if (MMS_DATA_SET_SERVICE == 1)

static void
addNamedVariableToNamedVariableListResultList(MmsVariableSpecification* namedVariable, MmsDomain* domain, char* nameIdStr,
        LinkedList /*<MmsValue>*/ values, MmsServerConnection connection, MmsNamedVariableListEntry listEntry)
{
    if (namedVariable != NULL) {

        if (DEBUG_MMS_SERVER)
            printf("MMS read: found named variable %s with search string %s\n",
                    namedVariable->name, nameIdStr);

        MmsValue* value = mmsServer_getValue(connection->server, domain, nameIdStr, connection, false);

        if (value) {
            if (listEntry->arrayIndex != -1) {
                if (MmsValue_getType(value) == MMS_ARRAY) {

                    MmsValue* elementValue = MmsValue_getElement(value, listEntry->arrayIndex);

                    if (listEntry->componentName) {
                        MmsVariableSpecification* elementType = namedVariable->typeSpec.array.elementTypeSpec;

                        MmsValue* subElementValue = MmsVariableSpecification_getChildValue(elementType, elementValue, listEntry->componentName);

                        if (subElementValue) {
                            appendValueToResultList(subElementValue, values);
                        }
                        else {
                            if (DEBUG_IED_SERVER)
                                printf("IED_SERVER: ERROR - component %s of array element not found\n", listEntry->componentName);
                        }
                    }
                    else {
                        appendValueToResultList(elementValue, values);
                    }

                }
                else {
                    if (DEBUG_MMS_SERVER)
                        printf("MMS_SERVER: data set entry of unexpected type!\n");

                    appendErrorToResultList(values, DATA_ACCESS_ERROR_OBJECT_NONE_EXISTENT);
                }
            }
            else {
                appendValueToResultList(value, values);
            }
        }
        else {
            appendErrorToResultList(values, DATA_ACCESS_ERROR_OBJECT_NONE_EXISTENT);
        }
    }
    else
        appendErrorToResultList(values, DATA_ACCESS_ERROR_OBJECT_NONE_EXISTENT);
}

static void
createNamedVariableListResponse(MmsServerConnection connection, MmsNamedVariableList namedList,
		int invokeId, ByteBuffer* response, bool isSpecWithResult, VarAccessSpec* accessSpec)
{

	LinkedList /*<MmsValue>*/ values = LinkedList_create();
	LinkedList variables = MmsNamedVariableList_getVariableList(namedList);

	int variableCount = LinkedList_size(variables);

	int i;

	LinkedList variable = LinkedList_getNext(variables);

	for (i = 0; i < variableCount; i++) {

		MmsNamedVariableListEntry variableListEntry = (MmsNamedVariableListEntry) variable->data;

		MmsDomain* variableDomain = MmsNamedVariableListEntry_getDomain(variableListEntry);
		char* variableName = MmsNamedVariableListEntry_getVariableName(variableListEntry);

		MmsVariableSpecification* namedVariable = MmsDomain_getNamedVariable(variableDomain,
				variableName);

		addNamedVariableToNamedVariableListResultList(namedVariable, variableDomain, variableName,
								values, connection, variableListEntry);

		variable = LinkedList_getNext(variable);
	}

	if (isSpecWithResult) /* add specification to result */
		encodeReadResponse(connection, invokeId, response, values, accessSpec);
	else
		encodeReadResponse(connection, invokeId, response, values, NULL);

	deleteValueList(values);
}

/**
 * \brief implements access to named variable lists (data sets)
 *
 * \param connection the client connection that received the request
 * \param read read request information
 * \param invokeId the invoke ID of the confirmed request PDU
 * \param response byte buffer to encode the response
 */
static void
handleReadNamedVariableListRequest(
		MmsServerConnection connection,
		ReadRequest_t* read,
		int invokeId,
		ByteBuffer* response)
{
	if (read->variableAccessSpecification.choice.variableListName.present ==
			ObjectName_PR_domainspecific)
	{
        char domainIdStr[65];
        char nameIdStr[65];

        mmsMsg_copyAsn1IdentifierToStringBuffer(read->variableAccessSpecification.choice.variableListName.choice.domainspecific.domainId,
                domainIdStr, 65);

        mmsMsg_copyAsn1IdentifierToStringBuffer(read->variableAccessSpecification.choice.variableListName.choice.domainspecific.itemId,
                nameIdStr, 65);

		VarAccessSpec accessSpec;

		accessSpec.isNamedVariableList = true;
		accessSpec.specific = 1;
		accessSpec.domainId = domainIdStr;
		accessSpec.itemId = nameIdStr;

		MmsDomain* domain = MmsDevice_getDomain(MmsServer_getDevice(connection->server), domainIdStr);

		if (domain == NULL) {
			if (DEBUG_MMS_SERVER) printf("MMS read: domain %s not found!\n", domainIdStr);
			mmsMsg_createServiceErrorPdu(invokeId, response, MMS_ERROR_ACCESS_OBJECT_NON_EXISTENT);
		}
		else {
			MmsNamedVariableList namedList = MmsDomain_getNamedVariableList(domain, nameIdStr);

			if (namedList != NULL) {
				createNamedVariableListResponse(connection, namedList, invokeId, response, isSpecWithResult(read),
						&accessSpec);
			}
			else {
				if (DEBUG_MMS_SERVER) printf("MMS read: named variable list %s not found!\n", nameIdStr);
				mmsMsg_createServiceErrorPdu(invokeId, response, MMS_ERROR_ACCESS_OBJECT_NON_EXISTENT);
			}
		}
	}
	else if (read->variableAccessSpecification.choice.variableListName.present == ObjectName_PR_vmdspecific)
	{
	    char listName[65];

        mmsMsg_copyAsn1IdentifierToStringBuffer(read->variableAccessSpecification.choice.variableListName.choice.vmdspecific,
                listName, 65);

        MmsNamedVariableList namedList = mmsServer_getNamedVariableListWithName(connection->server->device->namedVariableLists, listName);

        if (namedList == NULL)
            mmsMsg_createServiceErrorPdu(invokeId, response, MMS_ERROR_ACCESS_OBJECT_NON_EXISTENT);
        else {

            VarAccessSpec accessSpec;

            accessSpec.isNamedVariableList = true;
            accessSpec.specific = 0;
            accessSpec.domainId = NULL;
            accessSpec.itemId = listName;

            createNamedVariableListResponse(connection, namedList, invokeId, response, isSpecWithResult(read), &accessSpec);
        }
	}
#if (MMS_DYNAMIC_DATA_SETS == 1)
	else if (read->variableAccessSpecification.choice.variableListName.present ==
				ObjectName_PR_aaspecific)
	{
        char listName[65];

        mmsMsg_copyAsn1IdentifierToStringBuffer(read->variableAccessSpecification.choice.variableListName.choice.aaspecific,
                listName, 65);

		MmsNamedVariableList namedList = MmsServerConnection_getNamedVariableList(connection, listName);

		if (namedList == NULL)
			mmsMsg_createServiceErrorPdu(invokeId, response, MMS_ERROR_ACCESS_OBJECT_NON_EXISTENT);
		else {
            VarAccessSpec accessSpec;

            accessSpec.isNamedVariableList = true;
            accessSpec.specific = 2;
            accessSpec.domainId = NULL;
            accessSpec.itemId = listName;

			createNamedVariableListResponse(connection, namedList, invokeId, response, isSpecWithResult(read), &accessSpec);
		}
	}
#endif /* (MMS_DYNAMIC_DATA_SETS == 1) */
	else
		mmsMsg_createServiceErrorPdu(invokeId, response, MMS_ERROR_ACCESS_OBJECT_ACCESS_UNSUPPORTED);
}

#endif /* MMS_DATA_SET_SERVICE == 1 */

void
mmsServer_handleReadRequest(
		MmsServerConnection connection,
		uint8_t* buffer, int bufPos, int maxBufPos,
		uint32_t invokeId,
		ByteBuffer* response)
{
    (void)bufPos;
    (void)maxBufPos;

	ReadRequest_t* request = 0; /* allow asn1c to allocate structure */

	MmsPdu_t* mmsPdu = 0;

	asn_dec_rval_t rval = ber_decode(NULL, &asn_DEF_MmsPdu, (void**) &mmsPdu, buffer, CONFIG_MMS_MAXIMUM_PDU_SIZE);

	if (rval.code != RC_OK) {
	    mmsMsg_createMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_INVALID_PDU, response);
	    goto exit_function;
	}

	request = &(mmsPdu->choice.confirmedRequestPdu.confirmedServiceRequest.choice.read);

	if (request->variableAccessSpecification.present == VariableAccessSpecification_PR_listOfVariable) {
		MmsServer_lockModel(connection->server);

		handleReadListOfVariablesRequest(connection, request, invokeId, response);

		MmsServer_unlockModel(connection->server);
	}
#if (MMS_DATA_SET_SERVICE == 1)
	else if (request->variableAccessSpecification.present == VariableAccessSpecification_PR_variableListName) {
		MmsServer_lockModel(connection->server);

		handleReadNamedVariableListRequest(connection, request, invokeId, response);

		MmsServer_unlockModel(connection->server);
	}
#endif
	else {
		mmsMsg_createServiceErrorPdu(invokeId, response, MMS_ERROR_ACCESS_OBJECT_ACCESS_UNSUPPORTED);
	}

exit_function:	
	asn_DEF_MmsPdu.free_struct(&asn_DEF_MmsPdu, mmsPdu, 0);
}

void
MmsServerConnection_sendReadResponse(MmsServerConnection self, uint32_t invokeId, LinkedList values, bool handlerMode)
{
    if (handlerMode == false)
        IsoConnection_lock(self->isoConnection);

    ByteBuffer* response = MmsServer_reserveTransmitBuffer(self->server);

    ByteBuffer_setSize(response, 0);

    encodeReadResponse(self, invokeId, response, values, NULL);

    IsoConnection_sendMessage(self->isoConnection, response);

    MmsServer_releaseTransmitBuffer(self->server);

    if (handlerMode == false)
        IsoConnection_unlock(self->isoConnection);
}

