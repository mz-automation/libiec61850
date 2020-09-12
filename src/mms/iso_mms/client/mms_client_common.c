/*
 *  mms_client_common.c
 *
 *  Copyright 2013-2018 Michael Zillgith
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
#include <MmsPdu.h>
#include "mms_common.h"
#include "mms_client_connection.h"
#include "byte_buffer.h"

#include "mms_client_internal.h"

int
mmsClient_write_out(void *buffer, size_t size, void *app_key)
{
    ByteBuffer* writeBuffer = (ByteBuffer*) app_key;

    int appendedBytes = ByteBuffer_append(writeBuffer, (uint8_t*) buffer, size);

    if (appendedBytes == -1) {
        if (DEBUG_MMS_CLIENT)
            printf("MMS_CLIENT: message exceeds maximum PDU size!\n");
    }

    return appendedBytes;
}


uint32_t
mmsClient_getInvokeId(ConfirmedResponsePdu_t* confirmedResponse)
{
	long invokeId;

	asn_INTEGER2long(&confirmedResponse->invokeID, &invokeId);

	return (uint32_t) invokeId;
}


MmsPdu_t*
mmsClient_createConfirmedRequestPdu(uint32_t invokeId)
{
	MmsPdu_t* mmsPdu = (MmsPdu_t*) GLOBAL_CALLOC(1, sizeof(MmsPdu_t));
	mmsPdu->present = MmsPdu_PR_confirmedRequestPdu;

	asn_long2INTEGER(&(mmsPdu->choice.confirmedRequestPdu.invokeID), invokeId);

	return mmsPdu;
}

AlternateAccess_t*
mmsClient_createAlternateAccess(uint32_t index, uint32_t elementCount)
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

void
mmsClient_deleteAlternateAccess(AlternateAccess_t* alternateAccess)
{
    if (alternateAccess->list.array[0]->choice.unnamed->choice.selectAccess.choice.indexRange.lowIndex.buf != NULL) {
         GLOBAL_FREEMEM(alternateAccess->list.array[0]->choice.unnamed->choice.selectAccess.choice.indexRange.lowIndex.buf);
         alternateAccess->list.array[0]->choice.unnamed->choice.selectAccess.choice.indexRange.lowIndex.buf = NULL;
    }

    if (alternateAccess->list.array[0]->choice.unnamed->choice.selectAccess.choice.indexRange.numberOfElements.buf != NULL) {
        GLOBAL_FREEMEM(alternateAccess->list.array[0]->choice.unnamed->choice.selectAccess.choice.indexRange.numberOfElements.buf);
        alternateAccess->list.array[0]->choice.unnamed->choice.selectAccess.choice.indexRange.numberOfElements.buf = NULL;
    }

    if (alternateAccess->list.array[0]->choice.unnamed->choice.selectAccess.choice.index.buf != NULL) {
        GLOBAL_FREEMEM(alternateAccess->list.array[0]->choice.unnamed->choice.selectAccess.choice.index.buf);
        alternateAccess->list.array[0]->choice.unnamed->choice.selectAccess.choice.index.buf = NULL;
    }

    if (alternateAccess->list.array[0]->choice.unnamed->present == AlternateAccessSelection_PR_selectAlternateAccess) {
        if (alternateAccess->list.array[0]->choice.unnamed->choice.selectAlternateAccess.alternateAccess != NULL) {
            mmsClient_deleteAlternateAccess(alternateAccess->list.array[0]->choice.unnamed->choice.selectAlternateAccess.alternateAccess);
        }
    }

    GLOBAL_FREEMEM(alternateAccess->list.array[0]->choice.unnamed);
    GLOBAL_FREEMEM(alternateAccess->list.array[0]);
    GLOBAL_FREEMEM(alternateAccess->list.array);
    GLOBAL_FREEMEM(alternateAccess);
}

AlternateAccess_t*
mmsClient_createAlternateAccessComponent(const char* componentName)
{
    AlternateAccess_t* alternateAccess = (AlternateAccess_t*) GLOBAL_CALLOC(1, sizeof(AlternateAccess_t));
    alternateAccess->list.count = 1;
    alternateAccess->list.array = (struct AlternateAccess__Member**) GLOBAL_CALLOC(1, sizeof(struct AlternateAccess__Member*));
    alternateAccess->list.array[0] = (struct AlternateAccess__Member*) GLOBAL_CALLOC(1, sizeof(struct AlternateAccess__Member));
    alternateAccess->list.array[0]->present = AlternateAccess__Member_PR_unnamed;

    alternateAccess->list.array[0]->choice.unnamed = (AlternateAccessSelection_t*) GLOBAL_CALLOC(1, sizeof(AlternateAccessSelection_t));

    const char* separator = strchr(componentName, '$');

    if (separator) {
        int size = separator - componentName;

        alternateAccess->list.array[0]->choice.unnamed->present = AlternateAccessSelection_PR_selectAlternateAccess;
        alternateAccess->list.array[0]->choice.unnamed->choice.selectAlternateAccess.accessSelection.present =
                AlternateAccessSelection__selectAlternateAccess__accessSelection_PR_component;

        alternateAccess->list.array[0]->choice.unnamed->choice.selectAlternateAccess.accessSelection.choice.component.buf =
            (uint8_t*) StringUtils_copySubString((char*) componentName, (char*) separator);
        alternateAccess->list.array[0]->choice.unnamed->choice.selectAlternateAccess.accessSelection.choice.component.size = size;

        alternateAccess->list.array[0]->choice.unnamed->choice.selectAlternateAccess.alternateAccess = mmsClient_createAlternateAccessComponent(separator + 1);
    }
    else {
        int size = strlen(componentName);

        alternateAccess->list.array[0]->choice.unnamed->present = AlternateAccessSelection_PR_selectAccess;

        alternateAccess->list.array[0]->choice.unnamed->choice.selectAccess.present =
                AlternateAccessSelection__selectAccess_PR_component;

        alternateAccess->list.array[0]->choice.unnamed->choice.selectAccess.choice.component.buf =
            (uint8_t*) StringUtils_copyString(componentName);
        alternateAccess->list.array[0]->choice.unnamed->choice.selectAccess.choice.component.size = size;
    }

    return alternateAccess;
}

AlternateAccess_t*
mmsClient_createAlternateAccessIndexComponent(uint32_t index, const char* componentName)
{
    AlternateAccess_t* alternateAccess = (AlternateAccess_t*) GLOBAL_CALLOC(1, sizeof(AlternateAccess_t));
    alternateAccess->list.count = 1;
    alternateAccess->list.array = (struct AlternateAccess__Member**) GLOBAL_CALLOC(1, sizeof(struct AlternateAccess__Member*));
    alternateAccess->list.array[0] = (struct AlternateAccess__Member*) GLOBAL_CALLOC(1, sizeof(struct AlternateAccess__Member));
    alternateAccess->list.array[0]->present = AlternateAccess__Member_PR_unnamed;

    alternateAccess->list.array[0]->choice.unnamed = (AlternateAccessSelection_t*) GLOBAL_CALLOC(1, sizeof(AlternateAccessSelection_t));

    if (componentName) {
        alternateAccess->list.array[0]->choice.unnamed->present = AlternateAccessSelection_PR_selectAlternateAccess;

        alternateAccess->list.array[0]->choice.unnamed->choice.selectAlternateAccess.accessSelection.present =
                AlternateAccessSelection__selectAlternateAccess__accessSelection_PR_index;

        INTEGER_t* asnIndex =
                &(alternateAccess->list.array[0]->choice.unnamed->choice.selectAccess.choice.index);

        asn_long2INTEGER(asnIndex, index);

        alternateAccess->list.array[0]->choice.unnamed->choice.selectAlternateAccess.alternateAccess = mmsClient_createAlternateAccessComponent(componentName);
    }
    else {
        alternateAccess->list.array[0]->choice.unnamed->present = AlternateAccessSelection_PR_selectAccess;

        alternateAccess->list.array[0]->choice.unnamed->choice.selectAccess.present =
                        AlternateAccessSelection__selectAccess_PR_index;

        INTEGER_t* asnIndex =
                &(alternateAccess->list.array[0]->choice.unnamed->choice.selectAccess.choice.index);

        asn_long2INTEGER(asnIndex, index);
    }

    return alternateAccess;
}

