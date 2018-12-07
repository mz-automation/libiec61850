/*
 *  mms_sv.c
 *
 *  Copyright 2015 Michael Zillgith
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

#include "stack_config.h"

#if (CONFIG_IEC61850_SAMPLED_VALUES_SUPPORT == 1)

#include "libiec61850_platform_includes.h"
#include "mms_mapping.h"
#include "linked_list.h"

#include "mms_sv.h"

#include "mms_mapping_internal.h"

struct sMmsSampledValueControlBlock {
    SVControlBlock* svcb;

    bool svEna;
    MmsServerConnection reservedByClient;

    char* dstAddress;

    MmsDomain* domain;
    LogicalNode* logicalNode;

    MmsVariableSpecification* mmsType;
    MmsValue* mmsValue;

    MmsValue* svEnaValue;
    MmsValue* resvValue;


    SVCBEventHandler eventHandler;
    void* eventHandlerParameter;
};

MmsSampledValueControlBlock
MmsSampledValueControlBlock_create()
{
    MmsSampledValueControlBlock self = (MmsSampledValueControlBlock) GLOBAL_CALLOC(1, sizeof(struct sMmsSampledValueControlBlock));

    return self;
}


void
MmsSampledValueControlBlock_destroy(MmsSampledValueControlBlock self)
{
    MmsValue_delete(self->mmsValue);

    GLOBAL_FREEMEM(self);
}

static MmsSampledValueControlBlock
lookupSVCB(MmsMapping* self, MmsDomain* domain, char* lnName, char* objectName)
{
    LinkedList element = LinkedList_getNext(self->svControls);

    while (element != NULL) {
        MmsSampledValueControlBlock mmsSVCB = (MmsSampledValueControlBlock) element->data;

        if (mmsSVCB->domain == domain) {
            if (strcmp(mmsSVCB->logicalNode->name, lnName) == 0) {
                if (strcmp(mmsSVCB->svcb->name, objectName) == 0) {
                    return mmsSVCB;
                }
            }
        }

        element = LinkedList_getNext(element);
    }

    return NULL;
}

static void
MmsSampledValueControlBlock_enable(MmsSampledValueControlBlock self)
{
    self->svEna = true;
    MmsValue_setBoolean(self->svEnaValue, true);

    if (DEBUG_IED_SERVER)
        printf("IED_SERVER: enable SVCB %s\n", self->svcb->name);

    self->eventHandler(self->svcb, IEC61850_SVCB_EVENT_ENABLE, self->eventHandlerParameter);
}

static void
MmsSampledValueControlBlock_disable(MmsSampledValueControlBlock self)
{
    self->svEna = false;
    MmsValue_setBoolean(self->svEnaValue, false);

    if (DEBUG_IED_SERVER)
        printf("IED_SERVER: disable SVCB %s\n", self->svcb->name);

    self->eventHandler(self->svcb, IEC61850_SVCB_EVENT_DISABLE, self->eventHandlerParameter);
}

static bool
MmsSampledValueControlBlock_isEnabled(MmsSampledValueControlBlock self)
{
    return self->svEna;
}

MmsDataAccessError
LIBIEC61850_SV_writeAccessSVControlBlock(MmsMapping* self, MmsDomain* domain, char* variableIdOrig,
        MmsValue* value, MmsServerConnection connection)
{
    char variableId[130];

    strncpy(variableId, variableIdOrig, 129);

    char* separator = strchr(variableId, '$');

    *separator = 0;

    char* lnName = variableId;

    if (lnName == NULL)
        return DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED;

    char* objectName = MmsMapping_getNextNameElement(separator + 1);

    if (objectName == NULL)
        return DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED;

    char* varName = MmsMapping_getNextNameElement(objectName);

    if (varName != NULL)
        *(varName - 1) = 0;
    else
       return DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED;

    MmsSampledValueControlBlock mmsSVCB = lookupSVCB(self, domain, lnName, objectName);

    if (mmsSVCB == NULL)
        return DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED;

    if (mmsSVCB->reservedByClient != NULL) {
        if (mmsSVCB->reservedByClient != connection)
            return DATA_ACCESS_ERROR_TEMPORARILY_UNAVAILABLE;
    }

    if (strcmp(varName, "Resv") == 0) {
        if (MmsValue_getType(value) != MMS_BOOLEAN)
            return DATA_ACCESS_ERROR_TYPE_INCONSISTENT;

        if (MmsValue_getBoolean(value)) {
            mmsSVCB->reservedByClient = connection;
            MmsValue_setBoolean(mmsSVCB->resvValue, true);
        }
        else {
            mmsSVCB->reservedByClient = NULL;
            MmsValue_setBoolean(mmsSVCB->resvValue, false);
        }

        return DATA_ACCESS_ERROR_SUCCESS;
    }
    else if (strcmp(varName, "SvEna") == 0) {
        if (MmsValue_getType(value) != MMS_BOOLEAN)
            return DATA_ACCESS_ERROR_TYPE_INCONSISTENT;

        if (MmsValue_getBoolean(value))
            MmsSampledValueControlBlock_enable(mmsSVCB);
        else
            MmsSampledValueControlBlock_disable(mmsSVCB);

        return DATA_ACCESS_ERROR_SUCCESS;
    }
    else {
        if (MmsSampledValueControlBlock_isEnabled(mmsSVCB))
            return DATA_ACCESS_ERROR_TEMPORARILY_UNAVAILABLE;
        else {
            bool allowAccess = false;

            /* In 61850-9-2 mapping only Resv and SvEna are writable! */

            if (allowAccess)
                return DATA_ACCESS_ERROR_SUCCESS;
            else
                return DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED;

        }
    }
}

MmsValue*
LIBIEC61850_SV_readAccessSampledValueControlBlock(MmsMapping* self, MmsDomain* domain, char* variableIdOrig)
{
    MmsValue* value = NULL;

    char variableId[130];

    strncpy(variableId, variableIdOrig, 129);

    char* separator = strchr(variableId, '$');

    *separator = 0;

    char* lnName = variableId;

    if (lnName == NULL)
        return NULL;

    char* objectName = MmsMapping_getNextNameElement(separator + 1);

    if (objectName == NULL)
        return NULL;

    char* varName = MmsMapping_getNextNameElement(objectName);

    if (varName != NULL)
        *(varName - 1) = 0;

    MmsSampledValueControlBlock mmsSVCB = lookupSVCB(self, domain, lnName, objectName);

    if (mmsSVCB != NULL) {
        if (varName != NULL) {
            value = MmsValue_getSubElement(mmsSVCB->mmsValue, mmsSVCB->mmsType, varName);
        }
        else {
            value = mmsSVCB->mmsValue;
        }
    }

    return value;
}

static SVControlBlock*
getSVCBForLogicalNodeWithIndex(MmsMapping* self, LogicalNode* logicalNode, int index, bool isUnicast)
{
    int svCount = 0;

    SVControlBlock* svcb = self->model->svCBs;

    /* Iterate list of SvCBs */
    while (svcb != NULL ) {
        if ((svcb->parent == logicalNode) && (svcb->isUnicast == isUnicast)) {
            if (svCount == index)
                return svcb;

            svCount++;
        }

        svcb = svcb->sibling;
    }

    return NULL ;
}


static MmsVariableSpecification*
createSVControlBlockMmsStructure(char* gcbName, bool isUnicast)
{
    MmsVariableSpecification* gcb = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    gcb->name = StringUtils_copyString(gcbName);
    gcb->type = MMS_STRUCTURE;

    int elementCount;

    if (isUnicast)
        elementCount = 10;
    else
        elementCount = 9;

    gcb->typeSpec.structure.elementCount = elementCount;
    gcb->typeSpec.structure.elements = (MmsVariableSpecification**)
            GLOBAL_CALLOC(elementCount, sizeof(MmsVariableSpecification*));

    int currentElement = 0;

    MmsVariableSpecification* namedVariable;

    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("SvEna");
    namedVariable->type = MMS_BOOLEAN;

    gcb->typeSpec.structure.elements[currentElement++] = namedVariable;

    if (isUnicast) {
        namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
        namedVariable->name = StringUtils_copyString("Resv");
        namedVariable->type = MMS_BOOLEAN;

        gcb->typeSpec.structure.elements[currentElement++] = namedVariable;
    }

    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    if (isUnicast)
        namedVariable->name = StringUtils_copyString("UsvID");
    else
        namedVariable->name = StringUtils_copyString("MsvID");
    namedVariable->typeSpec.visibleString = -129;
    namedVariable->type = MMS_VISIBLE_STRING;

    gcb->typeSpec.structure.elements[currentElement++] = namedVariable;

    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("DatSet");
    namedVariable->typeSpec.visibleString = -129;
    namedVariable->type = MMS_VISIBLE_STRING;

    gcb->typeSpec.structure.elements[currentElement++] = namedVariable;

    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("ConfRev");
    namedVariable->type = MMS_INTEGER;
    namedVariable->typeSpec.integer = 32;

    gcb->typeSpec.structure.elements[currentElement++] = namedVariable;

    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("SmpRate");
    namedVariable->type = MMS_INTEGER;
    namedVariable->typeSpec.unsignedInteger = 32;

    gcb->typeSpec.structure.elements[currentElement++] = namedVariable;

    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("OptFlds");
    namedVariable->type = MMS_BIT_STRING;
    namedVariable->typeSpec.bitString = 5;

    gcb->typeSpec.structure.elements[currentElement++] = namedVariable;

    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("SmpMod");
    namedVariable->type = MMS_INTEGER;
    namedVariable->typeSpec.integer = 8;

    gcb->typeSpec.structure.elements[currentElement++] = namedVariable;

    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("DstAddress");
    MmsMapping_createPhyComAddrStructure(namedVariable);

    gcb->typeSpec.structure.elements[currentElement++] = namedVariable;

    namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("noASDU");
    namedVariable->type = MMS_INTEGER;
    namedVariable->typeSpec.integer = 32;

    gcb->typeSpec.structure.elements[currentElement++] = namedVariable;

    return gcb;
}

static void
createDataSetReference(char* buffer, char* domainName, char* lnName, char* dataSetName)
{
    StringUtils_createStringInBuffer(buffer, 5, domainName, "/", lnName, "$", dataSetName);
}

void
LIBIEC61850_SV_setSVCBHandler(MmsMapping* self, SVControlBlock* svcb, SVCBEventHandler handler, void* parameter)
{
    LinkedList svcbElement = LinkedList_getNext(self->svControls);

    while (svcbElement != NULL) {
        MmsSampledValueControlBlock mmsSVCB = (MmsSampledValueControlBlock) svcbElement->data;

        if (mmsSVCB->svcb == svcb) {
            mmsSVCB->eventHandler = handler;
            mmsSVCB->eventHandlerParameter = parameter;
            break;
        }

        svcbElement = LinkedList_getNext(svcbElement);
    }

    if (DEBUG_IED_SERVER) {
        if (svcbElement == NULL)
            printf("IED_SERVER: setSVCBHandler failed\n");
    }
}

MmsVariableSpecification*
LIBIEC61850_SV_createSVControlBlocks(MmsMapping* self, MmsDomain* domain,
        LogicalNode* logicalNode, int svCount, bool unicast)
{
    MmsVariableSpecification* namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1,
            sizeof(MmsVariableSpecification));

    if (unicast)
        namedVariable->name = StringUtils_copyString("US");
    else
        namedVariable->name = StringUtils_copyString("MS");

    namedVariable->type = MMS_STRUCTURE;

    namedVariable->typeSpec.structure.elementCount = svCount;
    namedVariable->typeSpec.structure.elements = (MmsVariableSpecification**) GLOBAL_CALLOC(svCount,
            sizeof(MmsVariableSpecification*));

    int currentSVCB = 0;

    char dataRefBuffer[130];

    while (currentSVCB < svCount) {
        SVControlBlock* svControlBlock = getSVCBForLogicalNodeWithIndex(
                self, logicalNode, currentSVCB, unicast);

        MmsVariableSpecification* svTypeSpec = createSVControlBlockMmsStructure(svControlBlock->name, unicast);

        MmsValue* svValues = MmsValue_newStructure(svTypeSpec);

        namedVariable->typeSpec.structure.elements[currentSVCB] = svTypeSpec;

        int currentIndex = 0;

        /* SvEna */
        MmsValue* svEna = MmsValue_getElement(svValues, currentIndex++);

        MmsValue* resv = NULL;

        if (unicast) {
            /* Resv */
            resv = MmsValue_getElement(svValues, currentIndex++);
        }


        /* SvID */
        MmsValue* svID = MmsValue_getElement(svValues, currentIndex++);
        MmsValue_setVisibleString(svID, svControlBlock->svId);

        /* DatSet */
        MmsValue* dataSetRef = MmsValue_getElement(svValues, currentIndex++);

        createDataSetReference(dataRefBuffer, MmsDomain_getName(domain),
                logicalNode->name, svControlBlock->dataSetName);

        MmsValue_setVisibleString(dataSetRef, dataRefBuffer);

        /* ConfRev */
        MmsValue* confRev = MmsValue_getElement(svValues, currentIndex++);
        MmsValue_setInt32(confRev, svControlBlock->confRev);

        /* SmpRate */
        MmsValue* smpRate = MmsValue_getElement(svValues, currentIndex++);
        MmsValue_setInt32(smpRate, svControlBlock->smpRate);

        /* OptFlds */
        MmsValue* optFlds = MmsValue_getElement(svValues, currentIndex++);
        MmsValue_setBitStringFromInteger(optFlds, svControlBlock->optFlds);

        /* SmpMod */
        MmsValue* smpMod = MmsValue_getElement(svValues, currentIndex++);
        MmsValue_setInt32(smpMod, svControlBlock->smpMod);

        /* Set communication parameters - DstAddress */
        uint8_t priority = CONFIG_GOOSE_DEFAULT_PRIORITY;
        uint8_t dstAddr[] = CONFIG_GOOSE_DEFAULT_DST_ADDRESS;
        uint16_t vid = CONFIG_GOOSE_DEFAULT_VLAN_ID;
        uint16_t appId = CONFIG_GOOSE_DEFAULT_APPID;

        if (svControlBlock->dstAddress != NULL) {
            priority = svControlBlock->dstAddress->vlanPriority;
            vid = svControlBlock->dstAddress->vlanId;
            appId = svControlBlock->dstAddress->appId;

            int i;
            for (i = 0; i < 6; i++) {
                dstAddr[i] = svControlBlock->dstAddress->dstAddress[i];
            }
        }

        MmsValue* dstAddress = MmsValue_getElement(svValues, currentIndex++);

        MmsValue* addr = MmsValue_getElement(dstAddress, 0);
        MmsValue_setOctetString(addr, dstAddr, 6);

        MmsValue* prio = MmsValue_getElement(dstAddress, 1);
        MmsValue_setUint8(prio, priority);

        MmsValue* vlanId = MmsValue_getElement(dstAddress, 2);
        MmsValue_setUint16(vlanId, vid);

        MmsValue* appIdVal = MmsValue_getElement(dstAddress, 3);
        MmsValue_setUint16(appIdVal, appId);

        /* noASDU */
        MmsValue* noASDU = MmsValue_getElement(svValues, currentIndex++);
        MmsValue_setInt32(noASDU, svControlBlock->noASDU);

        MmsSampledValueControlBlock mmsSvCb = MmsSampledValueControlBlock_create();

        mmsSvCb->mmsValue = svValues;
        mmsSvCb->svEnaValue = svEna;
        mmsSvCb->resvValue = resv;
        mmsSvCb->mmsType = svTypeSpec;
        mmsSvCb->domain = domain;
        mmsSvCb->logicalNode = logicalNode;
        mmsSvCb->svcb = svControlBlock;

        LinkedList_add(self->svControls, (void*) mmsSvCb);

        currentSVCB++;
    }

    return namedVariable;
}

#endif /* (CONFIG_IEC61850_SAMPLED_VALUES_SUPPORT == 1) */
