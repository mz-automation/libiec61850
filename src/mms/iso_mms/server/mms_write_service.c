/*
 *  mms_write_service.c
 *
 *  Copyright 2013-2023 Michael Zillgith
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

void
mmsServer_createMmsWriteResponse(MmsServerConnection connection,
        uint32_t invokeId, ByteBuffer* response, int numberOfItems, MmsDataAccessError* accessResults)
{
    (void)connection;

    int bufPos = 0;
    uint8_t* buffer = response->buffer;

    /* Determine length fields */

    uint32_t invokeIdLength = BerEncoder_UInt32determineEncodedSize(invokeId);

    uint32_t accessResultsLength = 0;

    int i;
    for (i = 0; i < numberOfItems; i++)
    {
        if (accessResults[i] < 0)
            accessResultsLength += 2;
        else
            accessResultsLength += 3;
    }

    uint32_t writeResponseLength = 2 + invokeIdLength
                                 + 1 + BerEncoder_determineLengthSize(accessResultsLength)
                                 + accessResultsLength;

    if ((int)(writeResponseLength + 1) > response->maxSize)
    {
        /* TODO add log message */

        response->size = 0;
        return;
    }

    /* Encode write response */

    bufPos = BerEncoder_encodeTL(0xa1, writeResponseLength, buffer, bufPos);

    /* invokeId */
    bufPos = BerEncoder_encodeTL(0x02, invokeIdLength, buffer, bufPos);
    bufPos = BerEncoder_encodeUInt32(invokeId, buffer, bufPos);

    bufPos = BerEncoder_encodeTL(0xa5, accessResultsLength, buffer, bufPos);

    for (i = 0; i < numberOfItems; i++) 
    {
        if (accessResults[i] < 0)
        {
            buffer[bufPos++] = 0x81;
            buffer[bufPos++] = 0x00;
        }
        else
        {
            buffer[bufPos++] = 0x80;
            buffer[bufPos++] = 0x01;
            buffer[bufPos++] = (uint8_t) accessResults[i];
        }
    }

    response->size = bufPos;
}

void
MmsServerConnection_sendWriteResponse(MmsServerConnection self, uint32_t invokeId, MmsDataAccessError indication, bool handlerMode)
{
    if (handlerMode == false)
        IsoConnection_lock(self->isoConnection);

    ByteBuffer* response = MmsServer_reserveTransmitBuffer(self->server);

    ByteBuffer_setSize(response, 0);

    mmsServer_createMmsWriteResponse(self, invokeId, response, 1, &indication);

    IsoConnection_sendMessage(self->isoConnection, response);

    MmsServer_releaseTransmitBuffer(self->server);

    if (handlerMode == false)
        IsoConnection_unlock(self->isoConnection);
}

#if 0
typedef struct {
    uint8_t type; /* 0 = vmd-specific, 1 = domain-specific, 2 = association-specific */
    uint8_t* name;
    uint8_t nameLength;
    uint8_t* domain;
    uint8_t domainLength;
} _MmsObjectName;

/**
 * \brief Decode MMS ObjectName
 *
 * \return true = valid data, false = decoding error
 */
static bool
decodeObjectName(uint8_t* buffer, int bufPos, int length, int* endPos, _MmsObjectName* objName)
{
    int dataEndBufPos = bufPos + length;

    uint8_t tag = buffer[bufPos++];

    int dataLength;

    bufPos = BerDecoder_decodeLength(buffer, &dataLength, bufPos, dataEndBufPos);

    printf("  decodeObjectName - bufPos: %i endPos: %i tag: %02x\n", bufPos, bufPos + dataLength, tag);

    if (bufPos == -1)
        return false;

    switch (tag) {
    case 0x80: /* VMD specific */
        objName->type = 0;
        objName->name = buffer + bufPos;
        objName->nameLength = dataLength;
        objName->domain = NULL;
        if (bufPos + dataLength > dataEndBufPos)
            return false;

        bufPos += dataLength;
        break;

    case 0xa1: /* domain specific */
        objName->type = 1;
        {
            if (buffer[bufPos++] != 0x1a)
                return false;

            int nameLength;

            bufPos = BerDecoder_decodeLength(buffer, &nameLength, bufPos, dataEndBufPos);

            if (bufPos == -1)
                return false;

            objName->domainLength = nameLength;
            objName->domain = buffer + bufPos;

            if (bufPos + nameLength >= dataEndBufPos)
                return false;

            bufPos += nameLength;

            if (buffer[bufPos++] != 0x1a)
                return false;

            bufPos = BerDecoder_decodeLength(buffer, &nameLength, bufPos, dataEndBufPos);

            objName->nameLength = nameLength;
            objName->name = buffer + bufPos;

            if (bufPos + nameLength > dataEndBufPos)
                return false;

            bufPos += nameLength;
        }
        break;

    case 0x82: /* association specific */
        objName->type = 2;
        objName->name = buffer + bufPos;
        objName->nameLength = dataLength;
        objName->domain = NULL;
        if (bufPos + dataLength > dataEndBufPos)
            return false;

        bufPos += dataLength;
        break;
    }

    if (endPos != NULL)
        *endPos = bufPos;

    return true;
}

static bool
decodeVarSpec(uint8_t* buffer, int bufPos, int length, int* endPos)
{
    int dataEndBufPos = bufPos + length;

    uint8_t tag = buffer[bufPos++];

    printf("  varSpec - bufPos: %i endPos: %i tag: %02x\n", bufPos - 1, bufPos + length, tag);

    if (tag != 0x30)
        return false;

    int dataLength;

    bufPos = BerDecoder_decodeLength(buffer, &dataLength, bufPos, dataEndBufPos);

    printf("dataLength = %i - bufPos: %i\n", dataLength, bufPos);

    if (bufPos < 0)
        return false;

    tag = buffer[bufPos++];

    printf("  varSpec - bufPos: %i endPos: %i tag: %02x\n", bufPos - 1, bufPos + length, tag);

    if (tag != 0xa0)
        return false;

    bufPos = BerDecoder_decodeLength(buffer, &dataLength, bufPos, dataEndBufPos);

    if (bufPos < 0)
        return false;

    printf("dataLength = %i - bufPos: %i\n", dataLength, bufPos);

    _MmsObjectName objName;

    if (decodeObjectName(buffer, bufPos, bufPos + dataLength, &bufPos, &objName) == false)
        return false;

    if (objName.domain != NULL)
        printf("domain name: %.*s\n", objName.domainLength, objName.domain);
    if (objName.name != NULL)
        printf("item name: %.*s\n", objName.nameLength, objName.name);

    if (endPos != NULL)
        *endPos = bufPos;

    return true;
}

void
mmsServer_handleWriteRequest2(
        MmsServerConnection connection,
        uint8_t* buffer, int bufPos, int maxBufPos,
        uint32_t invokeId,
        ByteBuffer* response)
{

    bool isAccessSpecification = true;

    while (bufPos < maxBufPos) {
         uint8_t tag = buffer[bufPos++];
         int length;

         bufPos = BerDecoder_decodeLength(buffer, &length, bufPos, maxBufPos);

         if (bufPos < 0)  {
             mmsMsg_createMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_INVALID_PDU, response);
             return;
         }

         if (tag == 0xa1) { /* variable access specification - variable list name (data set) */
             isAccessSpecification = false;


         }
         if (tag == 0xa0) {
             if (isAccessSpecification) { /* variable access specification - list of variable names */
                 printf("VAR ACCESS SPEC\n");

                 isAccessSpecification = false;

                 int dataBufPos = bufPos;

                 while (dataBufPos < (bufPos + length)) {


                     if (decodeVarSpec(buffer, dataBufPos, length, &dataBufPos) == false) {
                         printf("Failed to decode MMS var access spec\n");
                         mmsMsg_createMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_INVALID_PDU, response);
                         return;
                     }
                 }
             }
             else {
                 printf("LIST OF DATA\n");


                 int dataBufPos = bufPos;

                 while (dataBufPos < (bufPos + length)) {
                     printf("lod - dataBufPos: %i endPos: %i\n", dataBufPos, bufPos + length);

                     MmsValue* newValue = MmsValue_decodeMmsData(buffer, dataBufPos, length, &dataBufPos);

                     if (newValue != NULL) {
                         printf("  Decoded MMS data value:\n");

                         uint8_t printBuf[1024];

                         MmsValue_printToBuffer(newValue, printBuf, 1024);

                         printf("    %s\n", printBuf);
                     }
                     else {
                         /* TODO cleanup already decoded MmsValue instances */
                         printf("  Failed to decode MMS data value\n");
                         mmsMsg_createMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_INVALID_PDU, response);
                         return;
                     }
                 }
             }
         }

         bufPos += length;
    }
}
#endif

static void
createWriteNamedVariableListResponse(
        MmsServerConnection connection,
        WriteRequest_t* writeRequest,
        uint32_t invokeId,
        MmsNamedVariableList namedList,
        ByteBuffer* response)
{
    bool sendResponse = true;

    LinkedList variables = MmsNamedVariableList_getVariableList(namedList);

    int numberOfWriteItems = LinkedList_size(variables);

    if (numberOfWriteItems > CONFIG_MMS_WRITE_SERVICE_MAX_NUMBER_OF_WRITE_ITEMS)
    {
        mmsMsg_createMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_OTHER, response);
        return;
    }

    /* write variables and send response */

    MmsDataAccessError accessResults[CONFIG_MMS_WRITE_SERVICE_MAX_NUMBER_OF_WRITE_ITEMS * sizeof(MmsDataAccessError)];

    LinkedList element;

    int i = 0;

    for (element = LinkedList_getNext(variables); element != NULL; element = LinkedList_getNext(element))
    {
        MmsNamedVariableListEntry variableListEntry = (MmsNamedVariableListEntry) LinkedList_getData(element);

        MmsDomain* variableDomain = MmsNamedVariableListEntry_getDomain(variableListEntry);
        char* variableName = MmsNamedVariableListEntry_getVariableName(variableListEntry);

        MmsValue* oldValue = mmsServer_getValue(connection->server, variableDomain, variableName, connection, false);

        Data_t* dataElement = writeRequest->listOfData.list.array[i];

        MmsValue* newValue = mmsMsg_parseDataElement(dataElement);

        if (newValue == NULL)
        {
            accessResults[i] = DATA_ACCESS_ERROR_OBJECT_ATTRIBUTE_INCONSISTENT;
        }
        else if (MmsValue_equalTypes(oldValue, newValue) == false)
        {
            MmsValue_delete(newValue);
            accessResults[i] = DATA_ACCESS_ERROR_TYPE_INCONSISTENT;
        }
        else
        {
            MmsDataAccessError valueIndication =
                    mmsServer_setValue(connection->server, variableDomain, variableName, newValue, connection);

            accessResults[i] = valueIndication;

            if (valueIndication == DATA_ACCESS_ERROR_NO_RESPONSE)
                sendResponse = false;

            MmsValue_delete(newValue);
        }

        i++;
    }

    if (sendResponse)
        mmsServer_createMmsWriteResponse(connection, invokeId, response, numberOfWriteItems, accessResults);
}

static void
handleWriteNamedVariableListRequest(
        MmsServerConnection connection,
        WriteRequest_t* writeRequest,
        uint32_t invokeId,
        ByteBuffer* response)
{
    if (writeRequest->variableAccessSpecification.choice.variableListName.present == ObjectName_PR_domainspecific)
    {
        char domainIdStr[65];
        char nameIdStr[65];

        mmsMsg_copyAsn1IdentifierToStringBuffer(writeRequest->variableAccessSpecification.choice.variableListName.choice.domainspecific.domainId,
                domainIdStr, 65);

        mmsMsg_copyAsn1IdentifierToStringBuffer(writeRequest->variableAccessSpecification.choice.variableListName.choice.domainspecific.itemId,
                nameIdStr, 65);

        MmsDomain* domain = MmsDevice_getDomain(MmsServer_getDevice(connection->server), domainIdStr);

        if (domain == NULL)
        {
            if (DEBUG_MMS_SERVER)
                printf("MMS write: domain %s not found!\n", domainIdStr);
            mmsMsg_createServiceErrorPdu(invokeId, response, MMS_ERROR_ACCESS_OBJECT_NON_EXISTENT);

            return;
        }
        else
        {
            MmsNamedVariableList namedList = MmsDomain_getNamedVariableList(domain, nameIdStr);

            if (namedList)
            {
                MmsError accessError = mmsServer_callVariableListChangedHandler(MMS_VARLIST_WRITE, MMS_DOMAIN_SPECIFIC, domain, namedList->name, connection);

                if (accessError == MMS_ERROR_NONE)
                {
                    createWriteNamedVariableListResponse(connection, writeRequest, invokeId, namedList, response);
                }
                else
                {
                    if (DEBUG_MMS_SERVER) printf("MMS write: named variable list %s access error: %i\n", nameIdStr, accessError);

                    mmsMsg_createServiceErrorPdu(invokeId, response, accessError);
                }
            }
            else
            {
                if (DEBUG_MMS_SERVER) printf("MMS write: named variable list %s not found!\n", nameIdStr);
                mmsMsg_createServiceErrorPdu(invokeId, response, MMS_ERROR_ACCESS_OBJECT_NON_EXISTENT);
            }
        }
    }
    else if (writeRequest->variableAccessSpecification.choice.variableListName.present == ObjectName_PR_vmdspecific)
    {
        char listName[65];

        mmsMsg_copyAsn1IdentifierToStringBuffer(writeRequest->variableAccessSpecification.choice.variableListName.choice.vmdspecific,
                listName, 65);

        MmsNamedVariableList namedList = mmsServer_getNamedVariableListWithName(connection->server->device->namedVariableLists, listName);

        if (namedList)
        {
            MmsError accessError = mmsServer_callVariableListChangedHandler(MMS_VARLIST_WRITE, MMS_VMD_SPECIFIC, NULL, namedList->name, connection);

            if (accessError == MMS_ERROR_NONE)
            {
                createWriteNamedVariableListResponse(connection, writeRequest, invokeId, namedList, response);
            }
            else
            {
                if (DEBUG_MMS_SERVER) printf("MMS write: vmd specific named variable list %s access error: %i\n", namedList->name, accessError);

                mmsMsg_createServiceErrorPdu(invokeId, response, accessError);
            }

        }
        else
        {
            if (DEBUG_MMS_SERVER) printf("MMS write: vmd specific named variable list %s not found!\n", listName);
            mmsMsg_createServiceErrorPdu(invokeId, response, MMS_ERROR_ACCESS_OBJECT_NON_EXISTENT);
        }
    }
#if (MMS_DYNAMIC_DATA_SETS == 1)
    else if (writeRequest->variableAccessSpecification.choice.variableListName.present == ObjectName_PR_aaspecific)
    {
        char listName[65];

        mmsMsg_copyAsn1IdentifierToStringBuffer(writeRequest->variableAccessSpecification.choice.variableListName.choice.aaspecific,
                listName, 65);

        MmsNamedVariableList namedList = MmsServerConnection_getNamedVariableList(connection, listName);

        if (namedList)
        {
            MmsError accessError = mmsServer_callVariableListChangedHandler(MMS_VARLIST_WRITE, MMS_ASSOCIATION_SPECIFIC, NULL, namedList->name, connection);

            if (accessError == MMS_ERROR_NONE)
            {
                createWriteNamedVariableListResponse(connection, writeRequest, invokeId, namedList, response);
            }
            else
            {
                if (DEBUG_MMS_SERVER) printf("MMS write: association specific named variable list %s access error: %i\n", namedList->name, accessError);

                mmsMsg_createServiceErrorPdu(invokeId, response, accessError);
            }
        }
        else
        {
            if (DEBUG_MMS_SERVER) printf("MMS write: association specific named variable list %s not found!\n", listName);
            mmsMsg_createServiceErrorPdu(invokeId, response, MMS_ERROR_ACCESS_OBJECT_NON_EXISTENT);
        }
    }
#endif /* (MMS_DYNAMIC_DATA_SETS == 1) */
    else
        mmsMsg_createServiceErrorPdu(invokeId, response, MMS_ERROR_ACCESS_OBJECT_ACCESS_UNSUPPORTED);

}

static MmsVariableSpecification*
getComponent(MmsServerConnection connection, MmsDomain* domain, AlternateAccess_t* alternateAccess, MmsVariableSpecification* namedVariable, char* variableName)
{
    MmsVariableSpecification* retValue = NULL;

    if (mmsServer_isComponentAccess(alternateAccess))
    {
        Identifier_t component =
                alternateAccess->list.array[0]->choice.unnamed->choice.selectAccess.choice.component;

        if (component.size > 129)
            goto exit_function;

        if (namedVariable->type == MMS_STRUCTURE)
        {
            int i;

            for (i = 0; i < namedVariable->typeSpec.structure.elementCount; i++)
            {
                if ((int) strlen(namedVariable->typeSpec.structure.elements[i]->name)
                        == component.size)
                {
                    if (!strncmp(namedVariable->typeSpec.structure.elements[i]->name,
                            (char*) component.buf, component.size))
                    {
                        if (strlen(variableName) + component.size < 199)
                        {
                            StringUtils_appendString(variableName, 200, "$");

                            /* here we need strncat because component.buf is not null terminated! */
                            strncat(variableName, (const char*)component.buf, (size_t)component.size);

                            if (alternateAccess->list.array[0]->choice.unnamed->choice.selectAlternateAccess.alternateAccess
                                    != NULL)
                            {
                                retValue =
                                        getComponent(connection, domain,
                                                alternateAccess->list.array[0]->choice.unnamed->choice.selectAlternateAccess.alternateAccess,
                                                namedVariable->typeSpec.structure.elements[i],
                                                variableName);
                            }
                            else {
                                retValue = namedVariable->typeSpec.structure.elements[i];
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

void
mmsServer_handleWriteRequest(
        MmsServerConnection connection,
        uint8_t* buffer, int bufPos, int maxBufPos,
        uint32_t invokeId,
        ByteBuffer* response)
{
    (void)bufPos;

    MmsPdu_t* mmsPdu = NULL;
    WriteRequest_t* writeRequest = NULL;

    asn_dec_rval_t rval; /* Decoder return value  */

    rval = ber_decode(NULL, &asn_DEF_MmsPdu, (void**) &mmsPdu, buffer, maxBufPos);

    if (rval.code != RC_OK)
    {
        mmsMsg_createMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_INVALID_PDU, response);
        goto exit_function;
    }

    if ((mmsPdu->present == MmsPdu_PR_confirmedRequestPdu) && 
        (mmsPdu->choice.confirmedRequestPdu.confirmedServiceRequest.present 
            == ConfirmedServiceRequest_PR_write))
    {
        writeRequest = &(mmsPdu->choice.confirmedRequestPdu.confirmedServiceRequest.choice.write);
    }
    else {
        mmsMsg_createMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_INVALID_PDU, response);
	    goto exit_function;
    }

    MmsServer_lockModel(connection->server);

    if (writeRequest->variableAccessSpecification.present == VariableAccessSpecification_PR_variableListName)
    {
        handleWriteNamedVariableListRequest(connection, writeRequest, invokeId, response);
        goto exit_function;
    }
    else if (writeRequest->variableAccessSpecification.present == VariableAccessSpecification_PR_listOfVariable)
    {
        int numberOfWriteItems = writeRequest->variableAccessSpecification.choice.listOfVariable.list.count;

        if (numberOfWriteItems < 1)
        {
            mmsMsg_createMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_REQUEST_INVALID_ARGUMENT, response);
            goto exit_function;
        }

        if (numberOfWriteItems > CONFIG_MMS_WRITE_SERVICE_MAX_NUMBER_OF_WRITE_ITEMS)
        {
            mmsMsg_createMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_OTHER, response);
            goto exit_function;
        }

        if (writeRequest->listOfData.list.count != numberOfWriteItems)
        {
            mmsMsg_createMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_REQUEST_INVALID_ARGUMENT, response);
            goto exit_function;
        }

        MmsDataAccessError accessResults[CONFIG_MMS_WRITE_SERVICE_MAX_NUMBER_OF_WRITE_ITEMS * sizeof(MmsDataAccessError)];

        bool sendResponse = true;

        int i;

        for (i = 0; i < numberOfWriteItems; i++)
        {
            ListOfVariableSeq_t* varSpec =
                    writeRequest->variableAccessSpecification.choice.listOfVariable.list.array[i];

            if (varSpec->variableSpecification.present != VariableSpecification_PR_name)
            {
                accessResults[i] = DATA_ACCESS_ERROR_OBJECT_ACCESS_UNSUPPORTED;
                continue;
            }

            MmsVariableSpecification* variable;

            MmsDevice* device = MmsServer_getDevice(connection->server);

            MmsDomain* domain = NULL;

            char nameIdStr[65];

            if (varSpec->variableSpecification.choice.name.present == ObjectName_PR_domainspecific)
            {
                Identifier_t domainId = varSpec->variableSpecification.choice.name.choice.domainspecific.domainId;

                char domainIdStr[65];

                mmsMsg_copyAsn1IdentifierToStringBuffer(domainId, domainIdStr, 65);

                domain = MmsDevice_getDomain(device, domainIdStr);

                if (domain == NULL)
                {
                    accessResults[i] = DATA_ACCESS_ERROR_OBJECT_NONE_EXISTENT;
                    continue;
                }

                Identifier_t nameId = varSpec->variableSpecification.choice.name.choice.domainspecific.itemId;

                mmsMsg_copyAsn1IdentifierToStringBuffer(nameId, nameIdStr, 65);

                variable = MmsDomain_getNamedVariable(domain, nameIdStr);
            }

#if (CONFIG_MMS_SUPPORT_VMD_SCOPE_NAMED_VARIABLES == 1)
            else if (varSpec->variableSpecification.choice.name.present == ObjectName_PR_vmdspecific)
            {
                Identifier_t nameId = varSpec->variableSpecification.choice.name.choice.vmdspecific;

                mmsMsg_copyAsn1IdentifierToStringBuffer(nameId, nameIdStr, 65);

                variable = MmsDevice_getNamedVariable(device, nameIdStr);
            }
#endif /* (CONFIG_MMS_SUPPORT_VMD_SCOPE_NAMED_VARIABLES == 1) */

            else
            {
                accessResults[i] = DATA_ACCESS_ERROR_OBJECT_ACCESS_UNSUPPORTED;
                continue;
            }

            if (variable == NULL) 
            {
                accessResults[i] = DATA_ACCESS_ERROR_OBJECT_NONE_EXISTENT;
                continue;
            }

            AlternateAccess_t* alternateAccess = varSpec->alternateAccess;

            if (alternateAccess)
            {
                if ((variable->type == MMS_STRUCTURE) && (mmsServer_isComponentAccess(alternateAccess) == false)) {
                    accessResults[i] = DATA_ACCESS_ERROR_OBJECT_ATTRIBUTE_INCONSISTENT;
                    continue;
                }

                if ((variable->type == MMS_ARRAY) && (mmsServer_isIndexAccess(alternateAccess) == false)) {
                    accessResults[i] = DATA_ACCESS_ERROR_OBJECT_ACCESS_UNSUPPORTED;
                    continue;
                }

                if (variable->type != MMS_ARRAY && variable->type != MMS_STRUCTURE) {
                    accessResults[i] = DATA_ACCESS_ERROR_OBJECT_ATTRIBUTE_INCONSISTENT;
                    continue;
                }
            }

            Data_t* dataElement = writeRequest->listOfData.list.array[i];

            MmsValue* value = mmsMsg_parseDataElement(dataElement);

            if (value == NULL)
            {
                accessResults[i] = DATA_ACCESS_ERROR_OBJECT_ATTRIBUTE_INCONSISTENT;
                continue;
            }

            if (alternateAccess)
            {
                if (domain == NULL)
                    domain = (MmsDomain*) device;

                if (mmsServer_isIndexAccess(alternateAccess))
                {
                    MmsValue* cachedArray = MmsServer_getValueFromCache(connection->server, domain, nameIdStr);

                    if (cachedArray == NULL)
                    {
                        accessResults[i] = DATA_ACCESS_ERROR_OBJECT_ATTRIBUTE_INCONSISTENT;
                        goto end_of_main_loop;
                    }

                    int index = mmsServer_getLowIndex(alternateAccess);
                    int numberOfElements = mmsServer_getNumberOfElements(alternateAccess);

                    if (numberOfElements == 0) /* select single array element with index */
                    {
                        MmsValue* elementValue = MmsValue_getElement(cachedArray, index);

                        if (elementValue == NULL) {
                            accessResults[i] = DATA_ACCESS_ERROR_OBJECT_ATTRIBUTE_INCONSISTENT;
                            goto end_of_main_loop;
                        }

                        if (mmsServer_isAccessToArrayComponent(alternateAccess))
                        {
                            MmsVariableSpecification* namedVariable = MmsDomain_getNamedVariable(domain, nameIdStr);

                            char componentId[65];
                            componentId[0] = 0;

                            if (namedVariable) {
                                elementValue = mmsServer_getComponentOfArrayElement(alternateAccess, namedVariable, elementValue, componentId);
                            }

                            if ((namedVariable == NULL) || (elementValue == NULL)) {
                                accessResults[i] = DATA_ACCESS_ERROR_OBJECT_NONE_EXISTENT;
                            }
                            else
                            {
                                accessResults[i] = mmsServer_setValueEx(connection->server, domain, nameIdStr, value, connection, index, componentId);
                            }

                            goto end_of_main_loop;
                        }
                        else
                        {
                            accessResults[i] = mmsServer_setValueEx(connection->server, domain, nameIdStr, value, connection, index, NULL);
                            goto end_of_main_loop;
                        }
                    }
                    else /* select sub-array with start-index and number-of-elements */
                    {
                        if (MmsValue_getType(value) != MMS_ARRAY)
                        {
                            accessResults[i] = DATA_ACCESS_ERROR_TYPE_INCONSISTENT;
                            goto end_of_main_loop;
                        }

                        int elementNo;

                        for (elementNo = 0; elementNo < numberOfElements; elementNo++)
                        {
                            MmsValue* newElement = MmsValue_getElement(value, elementNo);
                            MmsValue* elementValue = MmsValue_getElement(cachedArray, index++);

                            if ((elementValue == NULL) || (newElement == NULL) )
                            {
                                accessResults[i] = DATA_ACCESS_ERROR_TYPE_INCONSISTENT;
                                goto end_of_main_loop;
                            }

                            if (MmsValue_update(elementValue, newElement) == false)
                            {
                                accessResults[i] = DATA_ACCESS_ERROR_TYPE_INCONSISTENT;
                                goto end_of_main_loop;
                            }
                        }
                    }

                    accessResults[i] = DATA_ACCESS_ERROR_SUCCESS;
                    goto end_of_main_loop;
                }
                else if (mmsServer_isComponentAccess(alternateAccess))
                {
                    variable = getComponent(connection, domain, alternateAccess, variable, nameIdStr);

                    if (variable == NULL)
                    {
                        accessResults[i] = DATA_ACCESS_ERROR_OBJECT_NONE_EXISTENT;
                        goto end_of_main_loop;
                    }
                }
                else
                {
                    accessResults[i] = DATA_ACCESS_ERROR_SUCCESS;
                    goto end_of_main_loop;
                }
            }

            /* Check for correct type */
            if (MmsVariableSpecification_isValueOfType(variable, value) == false)
            {
                accessResults[i] = DATA_ACCESS_ERROR_TYPE_INCONSISTENT;
                goto end_of_main_loop;
            }

            accessResults[i] = mmsServer_setValue(connection->server, domain, nameIdStr, value, connection);

            end_of_main_loop:

            if (accessResults[i] == DATA_ACCESS_ERROR_NO_RESPONSE)
                sendResponse = false;

            MmsValue_delete(value);
        }

        if (sendResponse)
            mmsServer_createMmsWriteResponse(connection, invokeId, response, numberOfWriteItems, accessResults);
    }
    else
    {
        /* unknown request type */
        mmsMsg_createMmsRejectPdu(&invokeId, MMS_ERROR_REJECT_REQUEST_INVALID_ARGUMENT, response);
        goto exit_function;
    }

exit_function:

    MmsServer_unlockModel(connection->server);

    asn_DEF_MmsPdu.free_struct(&asn_DEF_MmsPdu, mmsPdu, 0);
}

#endif /* (MMS_WRITE_SERVICE == 1) */
