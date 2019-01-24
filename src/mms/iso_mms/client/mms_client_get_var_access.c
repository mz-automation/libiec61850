/*
 *  mms_client_get_var_access.c
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
 *  sSee COPYING file for the complete license text.
 */

#include "libiec61850_platform_includes.h"
#include <MmsPdu.h>
#include "stack_config.h"
#include "mms_common.h"
#include "mms_client_connection.h"
#include "byte_buffer.h"

#include "mms_client_internal.h"

static MmsVariableSpecification*
createTypeSpecification(TypeSpecification_t* asnTypeSpec)
{
    MmsVariableSpecification* typeSpec = (MmsVariableSpecification*)
            GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));

    switch (asnTypeSpec->present)
    {
    case TypeSpecification_PR_structure:
        {
            typeSpec->type = MMS_STRUCTURE;

            int elementCount = asnTypeSpec->choice.structure.components.list.count;
            typeSpec->typeSpec.structure.elementCount = elementCount;

            typeSpec->typeSpec.structure.elements = (MmsVariableSpecification**)
                    GLOBAL_CALLOC(elementCount, sizeof(MmsVariableSpecification*));

            int i;

            for (i = 0; i < elementCount; i++) {

                char* name = StringUtils_createStringFromBuffer(
                        asnTypeSpec->choice.structure.components.list.array[i]->componentName->buf,
                        asnTypeSpec->choice.structure.components.list.array[i]->componentName->size);

                typeSpec->typeSpec.structure.elements[i] =
                        createTypeSpecification(asnTypeSpec->choice.structure.components.
                                list.array[i]->componentType);

                typeSpec->typeSpec.structure.elements[i]->name = name;
            }
        }
        break;
    case TypeSpecification_PR_array:
        {
            typeSpec->type = MMS_ARRAY;

            long elementCount;
            asn_INTEGER2long(&asnTypeSpec->choice.array.numberOfElements, &elementCount);

            typeSpec->typeSpec.array.elementCount = elementCount;

            typeSpec->typeSpec.array.elementTypeSpec =
                    createTypeSpecification(asnTypeSpec->choice.array.elementType);
        }
        break;
    case TypeSpecification_PR_boolean:
        typeSpec->type = MMS_BOOLEAN;
        break;
    case TypeSpecification_PR_bitstring:
        typeSpec->type = MMS_BIT_STRING;
        typeSpec->typeSpec.bitString = asnTypeSpec->choice.bitstring;
        break;
    case TypeSpecification_PR_integer:
        typeSpec->type = MMS_INTEGER;
        typeSpec->typeSpec.integer = asnTypeSpec->choice.integer;
        break;
    case TypeSpecification_PR_unsigned:
        typeSpec->type = MMS_UNSIGNED;
        typeSpec->typeSpec.unsignedInteger = asnTypeSpec->choice.Unsigned;
        break;
    case TypeSpecification_PR_floatingpoint:
        typeSpec->type = MMS_FLOAT;
        typeSpec->typeSpec.floatingpoint.exponentWidth =
                asnTypeSpec->choice.floatingpoint.exponentwidth;
        typeSpec->typeSpec.floatingpoint.formatWidth =
                asnTypeSpec->choice.floatingpoint.formatwidth;
        break;
    case TypeSpecification_PR_octetstring:
        typeSpec->type = MMS_OCTET_STRING;
        typeSpec->typeSpec.octetString = asnTypeSpec->choice.octetstring;
        break;
    case TypeSpecification_PR_visiblestring:
        typeSpec->type = MMS_VISIBLE_STRING;
        typeSpec->typeSpec.visibleString = asnTypeSpec->choice.visiblestring;
        break;
    case TypeSpecification_PR_mMSString:
        typeSpec->type = MMS_STRING;
        typeSpec->typeSpec.mmsString = asnTypeSpec->choice.mMSString;
        break;
    case TypeSpecification_PR_utctime:
        typeSpec->type = MMS_UTC_TIME;
        break;
    case TypeSpecification_PR_binarytime:
        typeSpec->type = MMS_BINARY_TIME;
        if (asnTypeSpec->choice.binarytime == 0)
            typeSpec->typeSpec.binaryTime = 4;
        else
            typeSpec->typeSpec.binaryTime = 6;
        break;
    default:
        printf("ERROR: unknown type in type specification\n");
        break;
    }

    return typeSpec;
}

MmsVariableSpecification*
mmsClient_parseGetVariableAccessAttributesResponse(ByteBuffer* message, uint32_t* invokeId)
{
    MmsPdu_t* mmsPdu = 0; /* allow asn1c to allocate structure */
    MmsVariableSpecification* typeSpec = NULL;

    asn_dec_rval_t rval = ber_decode(NULL, &asn_DEF_MmsPdu,
            (void**) &mmsPdu, ByteBuffer_getBuffer(message), ByteBuffer_getSize(message));

    if (rval.code == RC_OK) {

        if (mmsPdu->present == MmsPdu_PR_confirmedResponsePdu) {

            if (invokeId != NULL)
                *invokeId = mmsClient_getInvokeId(&mmsPdu->choice.confirmedResponsePdu);

            if (mmsPdu->choice.confirmedResponsePdu.confirmedServiceResponse.present ==
                    ConfirmedServiceResponse_PR_getVariableAccessAttributes)
                    {
                GetVariableAccessAttributesResponse_t* response;

                response = &(mmsPdu->choice.confirmedResponsePdu.confirmedServiceResponse.choice.getVariableAccessAttributes);
                TypeSpecification_t* asnTypeSpec = &response->typeSpecification;

                typeSpec = createTypeSpecification(asnTypeSpec);
            }
        }

    }

    asn_DEF_MmsPdu.free_struct(&asn_DEF_MmsPdu, mmsPdu, 0);

    return typeSpec;
}

int
mmsClient_createGetVariableAccessAttributesRequest(
        uint32_t invokeId,
        const char* domainId, const char* itemId,
        ByteBuffer* writeBuffer)
{
    MmsPdu_t* mmsPdu = mmsClient_createConfirmedRequestPdu(invokeId);

    mmsPdu->choice.confirmedRequestPdu.confirmedServiceRequest.present =
            ConfirmedServiceRequest_PR_getVariableAccessAttributes;

    GetVariableAccessAttributesRequest_t* request;

    request =
            &(mmsPdu->choice.confirmedRequestPdu.confirmedServiceRequest.choice.getVariableAccessAttributes);

    request->present = GetVariableAccessAttributesRequest_PR_name;

    if (domainId != NULL) {
        request->choice.name.present = ObjectName_PR_domainspecific;

        request->choice.name.choice.domainspecific.domainId.buf = (uint8_t*) domainId;
        request->choice.name.choice.domainspecific.domainId.size = strlen(domainId);
        request->choice.name.choice.domainspecific.itemId.buf = (uint8_t*) itemId;
        request->choice.name.choice.domainspecific.itemId.size = strlen(itemId);
    }
    else {
        request->choice.name.present = ObjectName_PR_vmdspecific;
        request->choice.name.choice.vmdspecific.buf = (uint8_t*) itemId;
        request->choice.name.choice.vmdspecific.size = strlen(itemId);
    }

    asn_enc_rval_t rval;

    rval = der_encode(&asn_DEF_MmsPdu, mmsPdu,
            (asn_app_consume_bytes_f*) mmsClient_write_out, (void*) writeBuffer);

    if (domainId != NULL) {
        request->choice.name.choice.domainspecific.domainId.buf = 0;
        request->choice.name.choice.domainspecific.domainId.size = 0;
        request->choice.name.choice.domainspecific.itemId.buf = 0;
        request->choice.name.choice.domainspecific.itemId.size = 0;
    }
    else {
        request->choice.name.choice.vmdspecific.buf = 0;
        request->choice.name.choice.vmdspecific.size = 0;
    }

    asn_DEF_MmsPdu.free_struct(&asn_DEF_MmsPdu, mmsPdu, 0);

    return rval.encoded;
}

