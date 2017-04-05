/*
 *  mms_mapping.c
 *
 *  Copyright 2013-2016 Michael Zillgith
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
#include "mms_mapping.h"
#include "mms_mapping_internal.h"
#include "array_list.h"
#include "stack_config.h"

#include "mms_goose.h"
#include "mms_sv.h"
#include "reporting.h"
#include "logging.h"
#include "control.h"
#include "ied_server_private.h"

#ifndef CONFIG_IEC61850_SG_RESVTMS
#define CONFIG_IEC61850_SG_RESVTMS 100
#endif

#ifndef DEBUG_IED_SERVER
#define DEBUG_IED_SERVER 0
#endif

typedef struct
{
    DataAttribute* attribute;
    AttributeChangedHandler handler;
} AttributeObserver;

typedef struct
{
    DataAttribute* attribute;
    WriteAccessHandler handler;
    void* parameter;
} AttributeAccessHandler;


typedef struct
{
    SettingGroupControlBlock* sgcb;
    MmsValue* sgcbMmsValues;
    MmsDomain* mmsDomain;

    ActiveSettingGroupChangedHandler actSgChangedHandler;
    void* actSgChangedHandlerParameter;

    EditSettingGroupChangedHandler editSgChangedHandler;
    void* editSgChangedHandlerParameter;

    EditSettingGroupConfirmationHandler editSgConfirmedHandler;
    void* editSgConfirmedHandlerParameter;

    ClientConnection editingClient;
    uint64_t reservationTimeout;
} SettingGroup;

#if (CONFIG_IEC61850_CONTROL_SERVICE == 1)
MmsValue*
Control_readAccessControlObject(MmsMapping* self, MmsDomain* domain, char* variableIdOrig,
        MmsServerConnection connection);
#endif

void /* Create PHYCOMADDR ACSI type instance */
MmsMapping_createPhyComAddrStructure(MmsVariableSpecification* namedVariable)
{
    namedVariable->type = MMS_STRUCTURE;
    namedVariable->typeSpec.structure.elementCount = 4;
    namedVariable->typeSpec.structure.elements = (MmsVariableSpecification**) GLOBAL_CALLOC(4,
            sizeof(MmsVariableSpecification*));

    MmsVariableSpecification* element;

    element = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    element->name = StringUtils_copyString("Addr");
    element->type = MMS_OCTET_STRING;
    element->typeSpec.octetString = 6;
    namedVariable->typeSpec.structure.elements[0] = element;

    element = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    element->name = StringUtils_copyString("PRIORITY");
    element->type = MMS_UNSIGNED;
    element->typeSpec.unsignedInteger = 8;
    namedVariable->typeSpec.structure.elements[1] = element;

    element = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    element->name = StringUtils_copyString("VID");
    element->type = MMS_UNSIGNED;
    element->typeSpec.unsignedInteger = 16;
    namedVariable->typeSpec.structure.elements[2] = element;

    element = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    element->name = StringUtils_copyString("APPID");
    element->type = MMS_UNSIGNED;
    element->typeSpec.unsignedInteger = 16;
    namedVariable->typeSpec.structure.elements[3] = element;
}

static MmsVariableSpecification*
createNamedVariableFromDataAttribute(DataAttribute* attribute)
{
    MmsVariableSpecification* origNamedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1,
            sizeof(MmsVariableSpecification));
    origNamedVariable->name = StringUtils_copyString(attribute->name);

    MmsVariableSpecification* namedVariable = origNamedVariable;

    if (attribute->elementCount > 0) {
        namedVariable->type = MMS_ARRAY;
        namedVariable->typeSpec.array.elementCount = attribute->elementCount;
        namedVariable->typeSpec.array.elementTypeSpec = (MmsVariableSpecification*) GLOBAL_CALLOC(1,
                sizeof(MmsVariableSpecification));
        namedVariable = namedVariable->typeSpec.array.elementTypeSpec;
    }

    if (attribute->firstChild != NULL) {
        namedVariable->type = MMS_STRUCTURE;

        int componentCount = ModelNode_getChildCount((ModelNode*) attribute);

        namedVariable->typeSpec.structure.elements = (MmsVariableSpecification**) GLOBAL_CALLOC(componentCount,
                sizeof(MmsVariableSpecification*));

        DataAttribute* subDataAttribute = (DataAttribute*) attribute->firstChild;

        int i = 0;
        while (subDataAttribute != NULL) {
            namedVariable->typeSpec.structure.elements[i] =
                    createNamedVariableFromDataAttribute(subDataAttribute);

            subDataAttribute = (DataAttribute*) subDataAttribute->sibling;
            i++;
        }

        namedVariable->typeSpec.structure.elementCount = i;
    }
    else {
        switch (attribute->type) {
        case IEC61850_BOOLEAN:
            namedVariable->type = MMS_BOOLEAN;
            break;
        case IEC61850_INT8:
            namedVariable->typeSpec.integer = 8;
            namedVariable->type = MMS_INTEGER;
            break;
        case IEC61850_INT16:
            namedVariable->typeSpec.integer = 16;
            namedVariable->type = MMS_INTEGER;
            break;
        case IEC61850_INT32:
            namedVariable->typeSpec.integer = 32;
            namedVariable->type = MMS_INTEGER;
            break;
        case IEC61850_INT64:
            namedVariable->typeSpec.integer = 64;
            namedVariable->type = MMS_INTEGER;
            break;
        case IEC61850_INT128:
            namedVariable->typeSpec.integer = 128;
            namedVariable->type = MMS_INTEGER;
            break;
        case IEC61850_INT8U:
            namedVariable->typeSpec.unsignedInteger = 8;
            namedVariable->type = MMS_UNSIGNED;
            break;
        case IEC61850_INT16U:
            namedVariable->typeSpec.unsignedInteger = 16;
            namedVariable->type = MMS_UNSIGNED;
            break;
        case IEC61850_INT24U:
            namedVariable->typeSpec.unsignedInteger = 24;
            namedVariable->type = MMS_UNSIGNED;
            break;
        case IEC61850_INT32U:
            namedVariable->typeSpec.unsignedInteger = 32;
            namedVariable->type = MMS_UNSIGNED;
            break;
        case IEC61850_FLOAT32:
            namedVariable->typeSpec.floatingpoint.formatWidth = 32;
            namedVariable->typeSpec.floatingpoint.exponentWidth = 8;
            namedVariable->type = MMS_FLOAT;
            break;
        case IEC61850_FLOAT64:
            namedVariable->typeSpec.floatingpoint.formatWidth = 64;
            namedVariable->typeSpec.floatingpoint.exponentWidth = 11;
            namedVariable->type = MMS_FLOAT;
            break;
        case IEC61850_ENUMERATED:
            namedVariable->typeSpec.integer = 8; /* 8 bit integer should be enough for all enumerations */
            namedVariable->type = MMS_INTEGER;
            break;
        case IEC61850_CHECK:
            namedVariable->typeSpec.bitString = -2;
            namedVariable->type = MMS_BIT_STRING;
            break;
        case IEC61850_CODEDENUM:
            namedVariable->typeSpec.bitString = 2;
            namedVariable->type = MMS_BIT_STRING;
            break;
        case IEC61850_OCTET_STRING_6:
            namedVariable->typeSpec.octetString = -6;
            namedVariable->type = MMS_OCTET_STRING;
            break;
        case IEC61850_OCTET_STRING_8:
            namedVariable->typeSpec.octetString = 8;
            namedVariable->type = MMS_OCTET_STRING;
            break;
        case IEC61850_OCTET_STRING_64:
            namedVariable->typeSpec.octetString = -64;
            namedVariable->type = MMS_OCTET_STRING;
            break;
        case IEC61850_CURRENCY:  /* mapping of Currency BasicType (see tissue 1047) */
            namedVariable->typeSpec.visibleString = -3;
            namedVariable->type = MMS_VISIBLE_STRING;
            break;
        case IEC61850_VISIBLE_STRING_32:
            namedVariable->typeSpec.visibleString = -32;
            namedVariable->type = MMS_VISIBLE_STRING;
            break;
        case IEC61850_VISIBLE_STRING_64:
            namedVariable->typeSpec.visibleString = -64;
            namedVariable->type = MMS_VISIBLE_STRING;
            break;
        case IEC61850_VISIBLE_STRING_65:
            namedVariable->typeSpec.visibleString = -65;
            namedVariable->type = MMS_VISIBLE_STRING;
            break;
        case IEC61850_VISIBLE_STRING_129:
            namedVariable->typeSpec.visibleString = -129;
            namedVariable->type = MMS_VISIBLE_STRING;
            break;
        case IEC61850_VISIBLE_STRING_255:
            namedVariable->typeSpec.visibleString = -255;
            namedVariable->type = MMS_VISIBLE_STRING;
            break;
        case IEC61850_UNICODE_STRING_255:
            namedVariable->typeSpec.mmsString = -255;
            namedVariable->type = MMS_STRING;
            break;
        case IEC61850_GENERIC_BITSTRING:
            namedVariable->type = MMS_BIT_STRING;
            break;
        case IEC61850_TIMESTAMP:
            namedVariable->type = MMS_UTC_TIME;
            break;
        case IEC61850_QUALITY:
            namedVariable->typeSpec.bitString = -13; // -13 = up to 13 bits
            namedVariable->type = MMS_BIT_STRING;
            break;
        case IEC61850_ENTRY_TIME:
            namedVariable->type = MMS_BINARY_TIME;
            namedVariable->typeSpec.binaryTime = 6;
            break;
        case IEC61850_PHYCOMADDR:
            MmsMapping_createPhyComAddrStructure(namedVariable);
            break;
        default:
            if (DEBUG_IED_SERVER)
                printf("MMS-MAPPING: type cannot be mapped %i\n", attribute->type);
            break;
        }
    }

    return origNamedVariable;
}

static int
countChildrenWithFc(DataObject* dataObject, FunctionalConstraint fc)
{
    int elementCount = 0;

    ModelNode* child = dataObject->firstChild;

    while (child != NULL) {
        if (child->modelType == DataAttributeModelType) {
            DataAttribute* dataAttribute = (DataAttribute*) child;

            if (dataAttribute->fc == fc)
                elementCount++;
        }
        else if (child->modelType == DataObjectModelType) {
            DataObject* subDataObject = (DataObject*) child;

            if (DataObject_hasFCData(subDataObject, fc))
                elementCount++;
        }

        child = child->sibling;
    }

    return elementCount;
}

static MmsVariableSpecification*
createFCNamedVariableFromDataObject(DataObject* dataObject,
        FunctionalConstraint fc)
{
    MmsVariableSpecification* namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1,
            sizeof(MmsVariableSpecification));

    MmsVariableSpecification* completeNamedVariable = namedVariable;

    namedVariable->name = StringUtils_copyString(dataObject->name);

    if (dataObject->elementCount > 0) {
        namedVariable->type = MMS_ARRAY;
        namedVariable->typeSpec.array.elementCount = dataObject->elementCount;
        namedVariable->typeSpec.array.elementTypeSpec = (MmsVariableSpecification*) GLOBAL_CALLOC(1,
                sizeof(MmsVariableSpecification));
        namedVariable = namedVariable->typeSpec.array.elementTypeSpec;
    }

    namedVariable->type = MMS_STRUCTURE;

    int elementCount = countChildrenWithFc(dataObject, fc);

    /* Allocate memory for components */
    namedVariable->typeSpec.structure.elements = (MmsVariableSpecification**) GLOBAL_CALLOC(elementCount,
            sizeof(MmsVariableSpecification*));

    int i = 0;
    ModelNode* component = dataObject->firstChild;

    while (component != NULL) {
        if (component->modelType == DataAttributeModelType) {
            DataAttribute* dataAttribute = (DataAttribute*) component;

            if (dataAttribute->fc == fc) {
                namedVariable->typeSpec.structure.elements[i] =
                        createNamedVariableFromDataAttribute(dataAttribute);
                i++;
            }
        }
        else if (component->modelType == DataObjectModelType) {
            DataObject* subDataObject = (DataObject*) component;

            if (DataObject_hasFCData(subDataObject, fc)) {
                namedVariable->typeSpec.structure.elements[i] =
                        createFCNamedVariableFromDataObject(subDataObject, fc);
                i++;
            }

        }

        component = component->sibling;
    }

    namedVariable->typeSpec.structure.elementCount = elementCount;

    return completeNamedVariable;
}

static MmsVariableSpecification*
createFCNamedVariable(LogicalNode* logicalNode, FunctionalConstraint fc)
{
    MmsVariableSpecification* namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1,
            sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString(FunctionalConstraint_toString(fc));
    namedVariable->type = MMS_STRUCTURE;

    int dataObjectCount = 0;

    DataObject* dataObject = (DataObject*) logicalNode->firstChild;

    while (dataObject != NULL) {
        if (DataObject_hasFCData(dataObject, fc))
            dataObjectCount++;

        dataObject = (DataObject*) dataObject->sibling;
    }

    namedVariable->typeSpec.structure.elementCount = dataObjectCount;
    namedVariable->typeSpec.structure.elements = (MmsVariableSpecification**) GLOBAL_CALLOC(dataObjectCount,
            sizeof(MmsVariableSpecification*));

    dataObjectCount = 0;

    dataObject = (DataObject*) logicalNode->firstChild;

    while (dataObject != NULL) {
        if (DataObject_hasFCData(dataObject, fc)) {

            namedVariable->typeSpec.structure.elements[dataObjectCount] =
                    createFCNamedVariableFromDataObject(dataObject, fc);

            dataObjectCount++;
        }

        dataObject = (DataObject*) dataObject->sibling;
    }

    return namedVariable;
}

#if (CONFIG_IEC61850_SETTING_GROUPS == 1)

static MmsVariableSpecification*
createSGCB(void)
{
    MmsVariableSpecification* namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1,
            sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("SGCB");
    namedVariable->type = MMS_STRUCTURE;

    namedVariable->typeSpec.structure.elementCount = 6;
    namedVariable->typeSpec.structure.elements = (MmsVariableSpecification**) GLOBAL_CALLOC(6,
            sizeof(MmsVariableSpecification*));

    MmsVariableSpecification* element;

    element = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    element->name = StringUtils_copyString("NumOfSG");
    element->type = MMS_UNSIGNED;
    element->typeSpec.integer = 8;
    namedVariable->typeSpec.structure.elements[0] = element;

    element = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    element->name = StringUtils_copyString("ActSG");
    element->type = MMS_UNSIGNED;
    element->typeSpec.integer = 8;
    namedVariable->typeSpec.structure.elements[1] = element;

    element = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    element->name = StringUtils_copyString("EditSG");
    element->type = MMS_UNSIGNED;
    element->typeSpec.integer = 8;
    namedVariable->typeSpec.structure.elements[2] = element;

    element = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    element->name = StringUtils_copyString("CnfEdit");
    element->type = MMS_BOOLEAN;
    namedVariable->typeSpec.structure.elements[3] = element;

    element = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    element->name = StringUtils_copyString("LActTm");
    element->type = MMS_UTC_TIME;
    namedVariable->typeSpec.structure.elements[4] = element;

    element = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
    element->name = StringUtils_copyString("ResvTms");
    element->type = MMS_UNSIGNED;
    element->typeSpec.integer = 16;
    namedVariable->typeSpec.structure.elements[5] = element;

    return namedVariable;
}


static MmsVariableSpecification*
createFCNamedVariableSPWithSGCB(LogicalNode* logicalNode)
{
    MmsVariableSpecification* namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1,
            sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("SP");
    namedVariable->type = MMS_STRUCTURE;

    int dataObjectCount = 1;

    DataObject* dataObject = (DataObject*) logicalNode->firstChild;

    while (dataObject != NULL) {
        if (DataObject_hasFCData(dataObject, IEC61850_FC_SP))
            dataObjectCount++;

        dataObject = (DataObject*) dataObject->sibling;
    }

    namedVariable->typeSpec.structure.elementCount = dataObjectCount;
    namedVariable->typeSpec.structure.elements = (MmsVariableSpecification**) GLOBAL_CALLOC(dataObjectCount,
            sizeof(MmsVariableSpecification*));

    dataObjectCount = 1;

    namedVariable->typeSpec.structure.elements[0] = createSGCB();

    dataObject = (DataObject*) logicalNode->firstChild;

    while (dataObject != NULL) {
        if (DataObject_hasFCData(dataObject, IEC61850_FC_SP)) {

            namedVariable->typeSpec.structure.elements[dataObjectCount] =
                    createFCNamedVariableFromDataObject(dataObject, IEC61850_FC_SP);

            dataObjectCount++;
        }

        dataObject = (DataObject*) dataObject->sibling;
    }

    return namedVariable;
}

static bool
isSettingGroupControlBlock(char* separator)
{
    if (strncmp(separator + 1, "SP$SGCB", 7) == 0)
        return true;

    return false;
}

static SettingGroup*
getSettingGroupByMmsDomain(MmsMapping* self, MmsDomain* domain)
{
    LinkedList settingGroupElement = LinkedList_getNext(self->settingGroups);

    while (settingGroupElement != NULL) {
        SettingGroup* settingGroup = (SettingGroup*) LinkedList_getData(settingGroupElement);

        if (settingGroup->mmsDomain == domain)
            return settingGroup;

        settingGroupElement = LinkedList_getNext(settingGroupElement);
    }

    return NULL;
}

static SettingGroup*
getSettingGroupBySGCB(MmsMapping* self, SettingGroupControlBlock* sgcb)
{
    LinkedList settingGroupElement = LinkedList_getNext(self->settingGroups);

    while (settingGroupElement != NULL) {
        SettingGroup* settingGroup = (SettingGroup*) LinkedList_getData(settingGroupElement);

        if (settingGroup->sgcb == sgcb)
            return settingGroup;

        settingGroupElement = LinkedList_getNext(settingGroupElement);
    }

    return NULL;
}

static void
unselectEditSettingGroup(SettingGroup* settingGroup)
{
	if (DEBUG_IED_SERVER)
		printf("IED_SERVER: Unselect setting group\n");

	settingGroup->sgcb->editSG = 0;
    settingGroup->editingClient = NULL;
    MmsValue* editSg = MmsValue_getElement(settingGroup->sgcbMmsValues, 2);
    MmsValue_setUint8(editSg, 0U);
    MmsValue* resvTms = MmsValue_getElement(settingGroup->sgcbMmsValues, 5);
    MmsValue_setUint16(resvTms, 0U);
}

static void
unselectAllSettingGroups(MmsMapping* self, MmsServerConnection serverCon)
{
    LinkedList settingGroupElement = LinkedList_getNext(self->settingGroups);

    while (settingGroupElement != NULL) {
        SettingGroup* settingGroup = (SettingGroup*) LinkedList_getData(settingGroupElement);

        if (settingGroup->editingClient == (ClientConnection) serverCon)
            unselectEditSettingGroup(settingGroup);

        settingGroupElement = LinkedList_getNext(settingGroupElement);
    }
}

void
MmsMapping_checkForSettingGroupReservationTimeouts(MmsMapping* self, uint64_t currentTime)
{
    LinkedList settingGroupElement = LinkedList_getNext(self->settingGroups);

    while (settingGroupElement != NULL) {
        SettingGroup* settingGroup = (SettingGroup*) LinkedList_getData(settingGroupElement);

        if (settingGroup->sgcb->editSG != 0)
            if (currentTime >= settingGroup->reservationTimeout)
                unselectEditSettingGroup(settingGroup);

        settingGroupElement = LinkedList_getNext(settingGroupElement);
    }
}

void
MmsMapping_configureSettingGroups(MmsMapping* self)
{

    LinkedList settingGroupElement = LinkedList_getNext(self->settingGroups);

    while (settingGroupElement != NULL) {
        SettingGroup* settingGroup = (SettingGroup*) LinkedList_getData(settingGroupElement);

        if (DEBUG_IED_SERVER)
            printf("IED_SERVER: Configure setting group\n");

        MmsValue* values =
                MmsServer_getValueFromCache(self->mmsServer, settingGroup->mmsDomain, "LLN0$SP$SGCB");

        if (values != NULL) {
            settingGroup->sgcb->resvTms = CONFIG_IEC61850_SG_RESVTMS;

            MmsValue* numOfSg = MmsValue_getElement(values, 0);
            MmsValue* actSg = MmsValue_getElement(values, 1);
            MmsValue* resvTms = MmsValue_getElement(values, 5);

            MmsValue_setUint8(numOfSg, settingGroup->sgcb->numOfSGs);
            MmsValue_setUint8(actSg, settingGroup->sgcb->actSG);
            MmsValue_setUint16(resvTms, 0U);

            settingGroup->sgcbMmsValues = values;
        }
        else if (DEBUG_IED_SERVER)
            printf("IED_SERVER: Values for SGCB do not exist\n");

        settingGroupElement = LinkedList_getNext(settingGroupElement);
    }
}

void
MmsMapping_setSgChangedHandler(MmsMapping* self, SettingGroupControlBlock* sgcb,
        ActiveSettingGroupChangedHandler handler, void* parameter)
{
    SettingGroup* sg = getSettingGroupBySGCB(self, sgcb);

    if (sg != NULL) {
        sg->actSgChangedHandler = handler;
        sg->actSgChangedHandlerParameter = parameter;
    }
}

void
MmsMapping_setEditSgChangedHandler(MmsMapping* self, SettingGroupControlBlock* sgcb,
        EditSettingGroupChangedHandler handler, void* parameter)
{
    SettingGroup* sg = getSettingGroupBySGCB(self, sgcb);

    if (sg != NULL) {
        sg->editSgChangedHandler = handler;
        sg->editSgChangedHandlerParameter = parameter;
    }
}

void
MmsMapping_setConfirmEditSgHandler(MmsMapping* self, SettingGroupControlBlock* sgcb,
       EditSettingGroupConfirmationHandler handler, void* parameter)
{
    SettingGroup* sg = getSettingGroupBySGCB(self, sgcb);

    if (sg != NULL) {
        sg->editSgConfirmedHandler = handler;
        sg->editSgConfirmedHandlerParameter = parameter;
    }
}

void
MmsMapping_changeActiveSettingGroup(MmsMapping* self, SettingGroupControlBlock* sgcb, uint8_t newActiveSg)
{
    SettingGroup* sg = getSettingGroupBySGCB(self, sgcb);

    if (sg != NULL) {
        if (newActiveSg > 0 && newActiveSg <= sgcb->numOfSGs) {
            sgcb->actSG = newActiveSg;

            MmsValue* actSg = MmsValue_getElement(sg->sgcbMmsValues, 1);
            MmsValue* lActTm = MmsValue_getElement(sg->sgcbMmsValues, 4);

            MmsValue_setUint8(actSg, sgcb->actSG);
            MmsValue_setUtcTimeMs(lActTm, Hal_getTimeInMs());
        }
    }
}

#endif /* (CONFIG_IEC61850_SETTING_GROUPS == 1) */

static int
determineLogicalNodeComponentCount(LogicalNode* logicalNode)
{
    int componentCount = 0;

    if (LogicalNode_hasFCData(logicalNode, IEC61850_FC_ST))
        componentCount++;

    if (LogicalNode_hasFCData(logicalNode, IEC61850_FC_MX))
        componentCount++;

    if (LogicalNode_hasFCData(logicalNode, IEC61850_FC_SP))
        componentCount++;

    if (LogicalNode_hasFCData(logicalNode, IEC61850_FC_SV))
        componentCount++;

    if (LogicalNode_hasFCData(logicalNode, IEC61850_FC_CF))
        componentCount++;

    if (LogicalNode_hasFCData(logicalNode, IEC61850_FC_DC))
        componentCount++;

    if (LogicalNode_hasFCData(logicalNode, IEC61850_FC_SG))
        componentCount++;

    if (LogicalNode_hasFCData(logicalNode, IEC61850_FC_SE))
        componentCount++;

    if (LogicalNode_hasFCData(logicalNode, IEC61850_FC_SR))
        componentCount++;

    if (LogicalNode_hasFCData(logicalNode, IEC61850_FC_OR))
        componentCount++;

    if (LogicalNode_hasFCData(logicalNode, IEC61850_FC_BL))
        componentCount++;

    if (LogicalNode_hasFCData(logicalNode, IEC61850_FC_EX))
        componentCount++;

    if (LogicalNode_hasFCData(logicalNode, IEC61850_FC_CO))
        componentCount++;

    return componentCount;
}


#if (CONFIG_IEC61850_REPORT_SERVICE == 1)
static int
countReportControlBlocksForLogicalNode(MmsMapping* self, LogicalNode* logicalNode, bool buffered)
{
    int rcbCount = 0;

    ReportControlBlock* rcb = self->model->rcbs;

    /* Iterate list of RCBs */
    while (rcb != NULL) {
        if (rcb->parent == logicalNode) {
            if (rcb->buffered == buffered)
                rcbCount++;
        }

        rcb = rcb->sibling;
    }

    return rcbCount;
}
#endif /* (CONFIG_IEC61850_CONTROL_SERVICE == 1) */

#if (CONFIG_IEC61850_LOG_SERVICE == 1)
static int
countLogControlBlocksForLogicalNode (MmsMapping* self, LogicalNode* logicalNode)
{
    int lcbCount = 0;

    LogControlBlock* lcb = self->model->lcbs;

    while (lcb != NULL) {
        if (lcb->parent == logicalNode)
            lcbCount++;

        lcb = lcb->sibling;
    }

    return lcbCount;
}
#endif /* (CONFIG_IEC61850_LOG_SERVICE == 1) */


#if (CONFIG_INCLUDE_GOOSE_SUPPORT == 1)

static int
countGSEControlBlocksForLogicalNode(MmsMapping* self, LogicalNode* logicalNode)
{
    int gseCount = 0;

    GSEControlBlock* gcb = self->model->gseCBs;

    while (gcb != NULL) {
        if (gcb->parent == logicalNode)
            gseCount++;

        gcb = gcb->sibling;
    }

    return gseCount;
}

#endif /* (CONFIG_INCLUDE_GOOSE_SUPPORT == 1) */

#if (CONFIG_IEC61850_SAMPLED_VALUES_SUPPORT == 1)

static int
countSVControlBlocksForLogicalNode(MmsMapping* self, LogicalNode* logicalNode, bool unicast)
{
    int svCount = 0;

    SVControlBlock* svCb = self->model->svCBs;

    while (svCb != NULL) {
        if ((svCb->parent == logicalNode) && (svCb->isUnicast == unicast))
            svCount++;

        svCb = svCb->sibling;
    }

    return svCount;
}

#endif /* (CONFIG_IEC61850_SAMPLED_VALUES_SUPPORT == 1) */

static SettingGroupControlBlock*
checkForSgcb(MmsMapping* self, LogicalNode* logicalNode)
{
    SettingGroupControlBlock* sgcb = self->model->sgcbs;

    while (sgcb != NULL) {
        if (sgcb->parent == logicalNode)
            return sgcb;

        sgcb = sgcb->sibling;
    }

    return NULL;
}

static MmsVariableSpecification*
createNamedVariableFromLogicalNode(MmsMapping* self, MmsDomain* domain,
        LogicalNode* logicalNode)
{
    MmsVariableSpecification* namedVariable = (MmsVariableSpecification*)
            GLOBAL_MALLOC(sizeof(MmsVariableSpecification));

    namedVariable->name = StringUtils_copyString(logicalNode->name);

    namedVariable->type = MMS_STRUCTURE;

    int componentCount = determineLogicalNodeComponentCount(logicalNode);

    SettingGroupControlBlock* sgControlBlock = NULL;

#if (CONFIG_IEC61850_SETTING_GROUPS == 1)

    sgControlBlock = checkForSgcb(self, logicalNode);

    if (sgControlBlock != NULL) {
        if (LogicalNode_hasFCData(logicalNode, IEC61850_FC_SP) == false)
            componentCount++;

        SettingGroup* settingGroup = (SettingGroup*) GLOBAL_CALLOC(1, sizeof(SettingGroup));

        settingGroup->sgcb = sgControlBlock;
        settingGroup->mmsDomain = domain;

        LinkedList_add(self->settingGroups, (void*) settingGroup);
    }
#endif /* (CONFIG_IEC61850_SETTING_GROUPS == 1) */

    if (DEBUG_IED_SERVER)
        printf("LogicalNode %s has %i fc components\n", logicalNode->name,
                componentCount);

#if (CONFIG_IEC61850_REPORT_SERVICE == 1)
    int brcbCount = countReportControlBlocksForLogicalNode(self, logicalNode,
    true);

    if (brcbCount > 0) {
        if (DEBUG_IED_SERVER)
            printf("  and %i buffered RCBs\n", brcbCount);
        componentCount++;
    }

    int urcbCount = countReportControlBlocksForLogicalNode(self, logicalNode,
    false);

    if (urcbCount > 0) {
        if (DEBUG_IED_SERVER)
            printf("  and %i unbuffered RCBs\n", urcbCount);
        componentCount++;
    }
#endif /* (CONFIG_IEC61850_REPORT_SERVICE == 1) */

#if (CONFIG_IEC61850_LOG_SERVICE == 1)

    int lcbCount = countLogControlBlocksForLogicalNode(self, logicalNode);

    if (lcbCount > 0) {
        if (DEBUG_IED_SERVER)
            printf("   and %i LOG control blocks\n", lcbCount);

        componentCount++;
    }

#endif /* (CONFIG_IEC61850_LOG_SERVICE == 1) */

#if (CONFIG_INCLUDE_GOOSE_SUPPORT == 1)

    int gseCount = countGSEControlBlocksForLogicalNode(self, logicalNode);

    if (gseCount > 0) {
        if (DEBUG_IED_SERVER)
            printf("   and %i GSE control blocks\n", gseCount);
        componentCount++;
    }

#endif /* (CONFIG_INCLUDE_GOOSE_SUPPORT == 1) */

#if (CONFIG_IEC61850_SAMPLED_VALUES_SUPPORT == 1)

    int msvcbCount = countSVControlBlocksForLogicalNode(self, logicalNode, false);

    if (msvcbCount > 0) {
        if (DEBUG_IED_SERVER)
            printf("   and %i MSV control blocks\n", msvcbCount);

        componentCount++;
    }

    int usvcbCount = countSVControlBlocksForLogicalNode(self, logicalNode, true);

    if (usvcbCount > 0) {
        if (DEBUG_IED_SERVER)
            printf("   and %i USV control blocks\n", usvcbCount);

        componentCount++;
    }

#endif /* (CONFIG_IEC61850_SAMPLED_VALUES_SUPPORT == 1) */

    namedVariable->typeSpec.structure.elements = (MmsVariableSpecification**) GLOBAL_CALLOC(componentCount,
            sizeof(MmsVariableSpecification*));

    /* Create a named variable of type structure for each functional constrained */
    int currentComponent = 0;

    if (LogicalNode_hasFCData(logicalNode, IEC61850_FC_MX)) {
        namedVariable->typeSpec.structure.elements[currentComponent] =
                createFCNamedVariable(logicalNode, IEC61850_FC_MX);
        currentComponent++;
    }

    if (LogicalNode_hasFCData(logicalNode, IEC61850_FC_ST)) {
        namedVariable->typeSpec.structure.elements[currentComponent] =
                createFCNamedVariable(logicalNode, IEC61850_FC_ST);
        currentComponent++;
    }

    if (LogicalNode_hasFCData(logicalNode, IEC61850_FC_CO)) {
        namedVariable->typeSpec.structure.elements[currentComponent] =
                createFCNamedVariable(logicalNode, IEC61850_FC_CO);
        currentComponent++;
    }

    if (LogicalNode_hasFCData(logicalNode, IEC61850_FC_CF)) {
        namedVariable->typeSpec.structure.elements[currentComponent] =
                createFCNamedVariable(logicalNode, IEC61850_FC_CF);
        currentComponent++;
    }

    if (LogicalNode_hasFCData(logicalNode, IEC61850_FC_DC)) {
        namedVariable->typeSpec.structure.elements[currentComponent] =
                createFCNamedVariable(logicalNode, IEC61850_FC_DC);
        currentComponent++;
    }

#if (CONFIG_IEC61850_SETTING_GROUPS == 1)
    if (sgControlBlock != NULL) {
        namedVariable->typeSpec.structure.elements[currentComponent] =
                createFCNamedVariableSPWithSGCB(logicalNode);
        currentComponent++;
    }
    else
#endif /* (CONFIG_IEC61850_SETTING_GROUPS == 1) */
    if (LogicalNode_hasFCData(logicalNode, IEC61850_FC_SP)) {
        namedVariable->typeSpec.structure.elements[currentComponent] =
                createFCNamedVariable(logicalNode, IEC61850_FC_SP);
        currentComponent++;
    }

    if (LogicalNode_hasFCData(logicalNode, IEC61850_FC_SG)) {
        namedVariable->typeSpec.structure.elements[currentComponent] =
                createFCNamedVariable(logicalNode, IEC61850_FC_SG);
        currentComponent++;
    }

#if (CONFIG_IEC61850_REPORT_SERVICE == 1)
    if (urcbCount > 0) {
        namedVariable->typeSpec.structure.elements[currentComponent] =
                Reporting_createMmsUnbufferedRCBs(self, domain, logicalNode,
                        urcbCount);
        currentComponent++;
    }
#endif /* (CONFIG_IEC61850_REPORT_SERVICE == 1) */

#if (CONFIG_IEC61850_LOG_SERVICE == 1)
    if (lcbCount > 0) {
        namedVariable->typeSpec.structure.elements[currentComponent] =
                Logging_createLCBs(self, domain, logicalNode, lcbCount);

        currentComponent++;
    }
#endif /* (CONFIG_IEC61850_LOG_SERVICE == 1) */


#if (CONFIG_IEC61850_REPORT_SERVICE == 1)
    if (brcbCount > 0) {
        namedVariable->typeSpec.structure.elements[currentComponent] =
                Reporting_createMmsBufferedRCBs(self, domain, logicalNode,
                        brcbCount);
        currentComponent++;
    }
#endif /* (CONFIG_IEC61850_REPORT_SERVICE == 1) */

#if (CONFIG_INCLUDE_GOOSE_SUPPORT == 1)
    if (gseCount > 0) {
        namedVariable->typeSpec.structure.elements[currentComponent] =
                GOOSE_createGOOSEControlBlocks(self, domain, logicalNode, gseCount);

        currentComponent++;
    }
#endif /* (CONFIG_INCLUDE_GOOSE_SUPPORT == 1) */



    if (LogicalNode_hasFCData(logicalNode, IEC61850_FC_SV)) {
        namedVariable->typeSpec.structure.elements[currentComponent] =
                createFCNamedVariable(logicalNode, IEC61850_FC_SV);
        currentComponent++;
    }

    if (LogicalNode_hasFCData(logicalNode, IEC61850_FC_SE)) {
        namedVariable->typeSpec.structure.elements[currentComponent] =
                createFCNamedVariable(logicalNode, IEC61850_FC_SE);
        currentComponent++;
    }

#if (CONFIG_IEC61850_SAMPLED_VALUES_SUPPORT == 1)

    /* Add MS and US named variables */
    if (msvcbCount > 0) {
        namedVariable->typeSpec.structure.elements[currentComponent] =
                LIBIEC61850_SV_createSVControlBlocks(self, domain, logicalNode, msvcbCount, false);

        currentComponent++;
    }

    if (usvcbCount > 0) {
        namedVariable->typeSpec.structure.elements[currentComponent] =
                LIBIEC61850_SV_createSVControlBlocks(self, domain, logicalNode, usvcbCount, true);

        currentComponent++;
    }
#endif

    if (LogicalNode_hasFCData(logicalNode, IEC61850_FC_EX)) {
        namedVariable->typeSpec.structure.elements[currentComponent] =
                createFCNamedVariable(logicalNode, IEC61850_FC_EX);
        currentComponent++;
    }

    if (LogicalNode_hasFCData(logicalNode, IEC61850_FC_SR)) {
        namedVariable->typeSpec.structure.elements[currentComponent] =
                createFCNamedVariable(logicalNode, IEC61850_FC_SR);
        currentComponent++;
    }

    if (LogicalNode_hasFCData(logicalNode, IEC61850_FC_OR)) {
        namedVariable->typeSpec.structure.elements[currentComponent] =
                createFCNamedVariable(logicalNode, IEC61850_FC_OR);
        currentComponent++;
    }

    if (LogicalNode_hasFCData(logicalNode, IEC61850_FC_BL)) {
        namedVariable->typeSpec.structure.elements[currentComponent] =
                createFCNamedVariable(logicalNode, IEC61850_FC_BL);
        currentComponent++;
    }

    namedVariable->typeSpec.structure.elementCount = currentComponent;

    return namedVariable;
}

static MmsDomain*
createMmsDomainFromIedDevice(MmsMapping* self, LogicalDevice* logicalDevice)
{
    MmsDomain* domain = NULL;
    char domainName[65];

    int modelNameLength = strlen(self->model->name);

    if (modelNameLength > 64)
        goto exit_function;

    strncpy(domainName, self->model->name, 64);
    strncat(domainName, logicalDevice->name, 64 - modelNameLength);

    domain = MmsDomain_create(domainName);

    if (domain == NULL)
        goto exit_function;

#if (CONFIG_IEC61850_LOG_SERVICE == 1)
    /* add logs (journals) */
    Log* log = self->model->logs;

    while (log != NULL) {

        char journalName[65];

        int nameLength = strlen(log->parent->name) + strlen(log->name);

        if (nameLength > 63) {
            if (DEBUG_IED_SERVER)
                printf("IED_SERVER: Log name %s invalid! Resulting journal name too long! Skip log\n", log->name);
        }
        else {
            strcpy(journalName, log->parent->name);
            strcat(journalName, "$");
            strcat(journalName, log->name);

            MmsDomain_addJournal(domain, journalName);

            LogInstance* logInstance = LogInstance_create(log->parent, log->name);

            LinkedList_add(self->logInstances, (void*) logInstance);
        }

        log = log->sibling;
    }
#endif /* (CONFIG_IEC61850_LOG_SERVICE == 1) */

    int nodesCount = LogicalDevice_getLogicalNodeCount(logicalDevice);

    /* Logical nodes are first level named variables */
    domain->namedVariablesCount = nodesCount;
    domain->namedVariables = (MmsVariableSpecification**) GLOBAL_MALLOC(nodesCount * sizeof(MmsVariableSpecification*));

    LogicalNode* logicalNode = (LogicalNode*) logicalDevice->firstChild;

    int i = 0;
    while (logicalNode != NULL) {
        domain->namedVariables[i] = createNamedVariableFromLogicalNode(self,
                domain, logicalNode);

        logicalNode = (LogicalNode*) logicalNode->sibling;
        i++;
    }

exit_function:
    return domain;
}

static void
createMmsDataModel(MmsMapping* self, int iedDeviceCount,
        MmsDevice* mmsDevice, IedModel* iedModel)
{
    mmsDevice->domains = (MmsDomain**) GLOBAL_MALLOC((iedDeviceCount) * sizeof(MmsDomain*));
    mmsDevice->domainCount = iedDeviceCount;

    LogicalDevice* logicalDevice = iedModel->firstChild;

    int i = 0;
    while (logicalDevice != NULL) {
        mmsDevice->domains[i] = createMmsDomainFromIedDevice(self,
                logicalDevice);
        i++;
        logicalDevice = (LogicalDevice*) logicalDevice->sibling;
    }
}

static void
createDataSets(MmsDevice* mmsDevice, IedModel* iedModel)
{
    DataSet* dataset = iedModel->dataSets;

    char domainName[65];

    int iedModelNameLength = strlen(iedModel->name);

    if (iedModelNameLength > 64)
        goto exit_function; //TODO call exception handler!

    while (dataset != NULL) {
        strncpy(domainName, iedModel->name, 64);
        strncat(domainName, dataset->logicalDeviceName, 64 - iedModelNameLength);

        MmsDomain* dataSetDomain = MmsDevice_getDomain(mmsDevice, domainName);

        if (dataSetDomain == NULL)
            goto exit_function; //TODO call exception handler!

        MmsNamedVariableList varList = MmsNamedVariableList_create(dataSetDomain, dataset->name, false);

        DataSetEntry* dataSetEntry = dataset->fcdas;

        while (dataSetEntry != NULL) {

            MmsAccessSpecifier accessSpecifier;

            strncpy(domainName, iedModel->name, 64);
            strncat(domainName, dataSetEntry->logicalDeviceName, 64 - iedModelNameLength);

            accessSpecifier.domain = MmsDevice_getDomain(mmsDevice, domainName);

            accessSpecifier.variableName = dataSetEntry->variableName;
            accessSpecifier.arrayIndex = dataSetEntry->index;
            accessSpecifier.componentName = dataSetEntry->componentName;

            MmsNamedVariableListEntry variableListEntry =
                    MmsNamedVariableListEntry_create(accessSpecifier);

            MmsNamedVariableList_addVariable(varList, variableListEntry);

            dataSetEntry = dataSetEntry->sibling;
        }

        MmsDomain_addNamedVariableList(dataSetDomain, varList);

        dataset = dataset->sibling;
    }

exit_function:
    return;
}

static MmsDevice*
createMmsModelFromIedModel(MmsMapping* self, IedModel* iedModel)
{
    MmsDevice* mmsDevice = NULL;

    if (iedModel->firstChild != NULL) {

        mmsDevice = MmsDevice_create(iedModel->name);

        int iedDeviceCount = IedModel_getLogicalDeviceCount(iedModel);

        createMmsDataModel(self, iedDeviceCount, mmsDevice, iedModel);

        createDataSets(mmsDevice, iedModel);
    }

    return mmsDevice;
}

MmsMapping*
MmsMapping_create(IedModel* model)
{
    MmsMapping* self = (MmsMapping*) GLOBAL_CALLOC(1, sizeof(struct sMmsMapping));

    self->model = model;

#if (CONFIG_IEC61850_REPORT_SERVICE == 1)
    self->reportControls = LinkedList_create();
#endif

#if (CONFIG_IEC61850_LOG_SERVICE == 1)
    self->logControls = LinkedList_create();
    self->logInstances = LinkedList_create();
#endif

#if (CONFIG_INCLUDE_GOOSE_SUPPORT == 1)
    self->gseControls = LinkedList_create();
    self->gooseInterfaceId = NULL;
#endif

#if (CONFIG_IEC61850_SAMPLED_VALUES_SUPPORT == 1)
    self->svControls = LinkedList_create();
    self->svInterfaceId = NULL;
#endif

#if (CONFIG_IEC61850_CONTROL_SERVICE == 1)
    self->controlObjects = LinkedList_create();
#endif

#if (CONFIG_IEC61850_SETTING_GROUPS == 1)
    self->settingGroups = LinkedList_create();
#endif

    self->observedObjects = LinkedList_create();

    self->attributeAccessHandlers = LinkedList_create();

    self->mmsDevice = createMmsModelFromIedModel(self, model);

    return self;
}

void
MmsMapping_destroy(MmsMapping* self)
{

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    if (self->reportWorkerThread != NULL) {
        self->reportThreadRunning = false;
        Thread_destroy(self->reportWorkerThread);
    }
#endif

    if (self->mmsDevice != NULL)
        MmsDevice_destroy(self->mmsDevice);

#if (CONFIG_IEC61850_REPORT_SERVICE == 1)
    LinkedList_destroyDeep(self->reportControls, (LinkedListValueDeleteFunction) ReportControl_destroy);
#endif

#if (CONFIG_INCLUDE_GOOSE_SUPPORT == 1)
    LinkedList_destroyDeep(self->gseControls, (LinkedListValueDeleteFunction) MmsGooseControlBlock_destroy);
#endif

#if (CONFIG_IEC61850_SAMPLED_VALUES_SUPPORT == 1)
    LinkedList_destroyDeep(self->svControls, (LinkedListValueDeleteFunction) MmsSampledValueControlBlock_destroy);
#endif

#if (CONFIG_IEC61850_CONTROL_SERVICE == 1)
    LinkedList_destroyDeep(self->controlObjects, (LinkedListValueDeleteFunction) ControlObject_destroy);
#endif

#if (CONFIG_IEC61850_SETTING_GROUPS == 1)
    LinkedList_destroy(self->settingGroups);
#endif

#if (CONFIG_IEC61850_LOG_SERVICE == 1)
    LinkedList_destroyDeep(self->logControls, (LinkedListValueDeleteFunction) LogControl_destroy);
    LinkedList_destroyDeep(self->logInstances, (LinkedListValueDeleteFunction) LogInstance_destroy);
#endif

    LinkedList_destroy(self->observedObjects);

    LinkedList_destroy(self->attributeAccessHandlers);

    IedModel_setAttributeValuesToNull(self->model);

    GLOBAL_FREEMEM(self);
}

MmsDevice*
MmsMapping_getMmsDeviceModel(MmsMapping* mapping)
{
    return mapping->mmsDevice;
}

#if (CONFIG_IEC61850_REPORT_SERVICE == 1)
static bool
isReportControlBlock(char* separator)
{
    if (strncmp(separator + 1, "RP", 2) == 0)
        return true;

    if (strncmp(separator + 1, "BR", 2) == 0)
        return true;

    return false;
}
#endif /* (CONFIG_IEC61850_REPORT_SERVICE == 1) */

static bool
isFunctionalConstraintCF(char* separator)
{
    if (strncmp(separator + 1, "CF", 2) == 0)
        return true;
    else
        return false;
}

static bool
isFunctionalConstraintDC(char* separator)
{
    if (strncmp(separator + 1, "DC", 2) == 0)
        return true;
    else
        return false;
}

static bool
isFunctionalConstraintSP(char* separator)
{
    if (strncmp(separator + 1, "SP", 2) == 0)
        return true;
    else
        return false;
}

static bool
isFunctionalConstraintSV(char* separator)
{
    if (strncmp(separator + 1, "SV", 2) == 0)
        return true;
    else
        return false;
}

static bool
isFunctionalConstraintSE(char* separator)
{
    if (strncmp(separator + 1, "SE", 2) == 0)
        return true;
    else
        return false;
}


#if (CONFIG_IEC61850_CONTROL_SERVICE == 1)
static bool
isControllable(char* separator)
{
    if (strncmp(separator + 1, "CO", 2) == 0)
        return true;
    else
        return false;
}
#endif /* (CONFIG_IEC61850_CONTROL_SERVICE == 1) */

#if (CONFIG_INCLUDE_GOOSE_SUPPORT == 1)

static bool
isGooseControlBlock(char* separator)
{
    if (strncmp(separator + 1, "GO", 2) == 0)
        return true;
    else
        return false;
}

#endif /* (CONFIG_INCLUDE_GOOSE_SUPPORT == 1) */


#if (CONFIG_IEC61850_SAMPLED_VALUES_SUPPORT == 1)

static bool
isSampledValueControlBlock(char* separator)
{
    if (strncmp(separator + 1, "MS", 2) == 0)
        return true;

    if (strncmp(separator + 1, "US", 2) == 0)
        return true;

    return false;
}

#endif /* (CONFIG_IEC61850_SAMPLED_VALUES_SUPPORT == 1) */

#if (CONFIG_IEC61850_LOG_SERVICE == 1)

static bool
isLogControlBlock(char* separator)
{
    if (strncmp(separator + 1, "LG", 2) == 0)
        return true;

    return false;
}

#endif /* (CONFIG_IEC61850_LOG_SERVICE == 1) */

char*
MmsMapping_getNextNameElement(char* name)
{
    char* separator = strchr(name, '$');

    if (separator == NULL)
        return NULL;

    separator++;

    if (*separator == 0)
        return NULL;

    return separator;
}

#if (CONFIG_INCLUDE_GOOSE_SUPPORT == 1)

static MmsGooseControlBlock
lookupGCB(MmsMapping* self, MmsDomain* domain, char* lnName, char* objectName)
{
    LinkedList element = LinkedList_getNext(self->gseControls);

    while (element != NULL) {
        MmsGooseControlBlock mmsGCB = (MmsGooseControlBlock) element->data;

        if (MmsGooseControlBlock_getDomain(mmsGCB) == domain) {
            if (strcmp(MmsGooseControlBlock_getLogicalNodeName(mmsGCB), lnName) == 0) {
                if (strcmp(MmsGooseControlBlock_getName(mmsGCB), objectName) == 0) {
                    return mmsGCB;
                }
            }
        }

        element = LinkedList_getNext(element);
    }

    return NULL;
}

#ifndef CONFIG_GOOSE_GOID_WRITABLE
#define CONFIG_GOOSE_GOID_WRITABLE 0
#endif

#ifndef CONFIG_GOOSE_DATSET_WRITABLE
#define CONFIG_GOOSE_DATSET_WRITABLE 0
#endif

#ifndef CONFIG_GOOSE_CONFREV_WRITABLE
#define CONFIG_GOOSE_CONFREV_WRITABLE 0
#endif

#ifndef CONFIG_GOOSE_NDSCOM_WRITABLE
#define CONFIG_GOOSE_NDSCOM_WRITABLE 0
#endif

#ifndef CONFIG_GOOSE_DSTADDRESS_WRITABLE
#define CONFIG_GOOSE_DSTADDRESS_WRITABLE 0
#endif

#ifndef CONFIG_GOOSE_MINTIME_WRITABLE
#define CONFIG_GOOSE_MINTIME_WRITABLE 0
#endif

#ifndef CONFIG_GOOSE_MAXTIME_WRITABLE
#define CONFIG_GOOSE_MAXTIME_WRITABLE 0
#endif

#ifndef CONFIG_GOOSE_FIXEDOFFS_WRITABLE
#define CONFIG_GOOSE_FIXEDOFFS_WRITABLE 0
#endif

static MmsDataAccessError
writeAccessGooseControlBlock(MmsMapping* self, MmsDomain* domain, char* variableIdOrig,
        MmsValue* value)
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

    MmsGooseControlBlock mmsGCB = lookupGCB(self, domain, lnName, objectName);

    if (mmsGCB == NULL)
        return DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED;

    if (strcmp(varName, "GoEna") == 0) {
        if (MmsValue_getType(value) != MMS_BOOLEAN)
            return DATA_ACCESS_ERROR_TYPE_INCONSISTENT;

        if (MmsValue_getBoolean(value))
            MmsGooseControlBlock_enable(mmsGCB);
        else
            MmsGooseControlBlock_disable(mmsGCB);

        return DATA_ACCESS_ERROR_SUCCESS;
    }
    else {
        if (MmsGooseControlBlock_isEnabled(mmsGCB))
            return DATA_ACCESS_ERROR_TEMPORARILY_UNAVAILABLE;
        else {
            bool allowAccess = false;

#if (CONFIG_GOOSE_GOID_WRITABLE == 1)
            if (strcmp(varName, "GoID") == 0) {
                MmsValue_update(MmsValue_getElement(MmsGooseControlBlock_getMmsValues(mmsGCB), 1), value);
                allowAccess = true;
            }
#endif

#if (CONFIG_GOOSE_DATSET_WRITABLE == 1)
            if (strcmp(varName, "DatSet") == 0) {
            	// allow to set non-existing data set?

                MmsValue_update(MmsValue_getElement(MmsGooseControlBlock_getMmsValues(mmsGCB), 2), value);
                allowAccess = true;
            }
#endif

#if (CONFIG_GOOSE_CONFREV_WRITABLE == 1)
            if (strcmp(varName, "ConfRev") == 0) {
                MmsValue_update(MmsValue_getElement(MmsGooseControlBlock_getMmsValues(mmsGCB), 3), value);
                allowAccess = true;
            }
#endif

#if (CONFIG_GOOSE_NDSCOM_WRITABLE == 1)
            if (strcmp(varName, "NdsCom") == 0) {
                MmsValue_update(MmsValue_getElement(MmsGooseControlBlock_getMmsValues(mmsGCB), 4), value);
                allowAccess = true;
            }
#endif

#if (CONFIG_GOOSE_DSTADDRESS_WRITABLE == 1)
            if (memcmp(varName, "DstAddress", 9) == 0) {
                MmsValue* subValue = MmsValue_getSubElement(MmsGooseControlBlock_getMmsValues(mmsGCB),
                    MmsGooseControlBlock_getVariableSpecification(mmsGCB), varName);

                if (subValue == NULL)
                    return DATA_ACCESS_ERROR_INVALID_ADDRESS;

                if (MmsValue_update(subValue, value))
                    return DATA_ACCESS_ERROR_SUCCESS;
                else
                    return DATA_ACCESS_ERROR_OBJECT_VALUE_INVALID;
            }
#endif

#if (CONFIG_GOOSE_MINTIME_WRITABLE == 1)
            if (strcmp(varName, "MinTime") == 0) {
                MmsValue_update(MmsValue_getElement(MmsGooseControlBlock_getMmsValues(mmsGCB), 6), value);
                allowAccess = true;
            }
#endif

#if (CONFIG_GOOSE_MAXTIME_WRITABLE == 1)
            if (strcmp(varName, "MaxTime") == 0) {
                MmsValue_update(MmsValue_getElement(MmsGooseControlBlock_getMmsValues(mmsGCB), 7), value);
                allowAccess = true;
            }
#endif

#if (CONFIG_GOOSE_FIXEDOFFS_WRITABLE == 1)
            if (strcmp(varName, "FixedOffs") == 0) {
                MmsValue_update(MmsValue_getElement(MmsGooseControlBlock_getMmsValues(mmsGCB), 8), value);
                allowAccess = true;
            }
#endif

            if (allowAccess)
                return DATA_ACCESS_ERROR_SUCCESS;
            else
                return DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED;

        }
    }
}

#endif /* (CONFIG_INCLUDE_GOOSE_SUPPORT == 1) */

static MmsValue*
checkIfValueBelongsToModelNode(DataAttribute* dataAttribute, MmsValue* value, MmsValue* newValue)
{
    if (dataAttribute->mmsValue == value)
        return newValue;

    DataAttribute* child = (DataAttribute*) dataAttribute->firstChild;

    while (child != NULL) {
        MmsValue* tmpValue = checkIfValueBelongsToModelNode(child, value, newValue);

        if (tmpValue != NULL)
            return tmpValue;
        else
            child = (DataAttribute*) child->sibling;
    }

    if (MmsValue_getType(value) == MMS_STRUCTURE) {
        int elementCount = MmsValue_getArraySize(value);

        int i;
        for (i = 0; i < elementCount; i++) {
            MmsValue* childValue = MmsValue_getElement(value, i);
            MmsValue* childNewValue = MmsValue_getElement(newValue, i);

            MmsValue* tmpValue = checkIfValueBelongsToModelNode(dataAttribute, childValue, childNewValue);

            if (tmpValue != NULL)
                return tmpValue;
        }
    }

    return NULL;
}

static FunctionalConstraint
getFunctionalConstraintForWritableNode(MmsMapping* self, char* separator)
{
    if (isFunctionalConstraintCF(separator))
        return IEC61850_FC_CF;
    if (isFunctionalConstraintDC(separator))
        return IEC61850_FC_DC;
    if (isFunctionalConstraintSP(separator))
        return IEC61850_FC_SP;
    if (isFunctionalConstraintSV(separator))
        return IEC61850_FC_SV;
    if (isFunctionalConstraintSE(separator))
        return IEC61850_FC_SE;

    return IEC61850_FC_NONE;
}

static AccessPolicy
getAccessPolicyForFC(MmsMapping* self, FunctionalConstraint fc)
{
    if (fc == IEC61850_FC_CF) {
        if (self->iedServer->writeAccessPolicies & ALLOW_WRITE_ACCESS_CF)
            return ACCESS_POLICY_ALLOW;
        else
            return ACCESS_POLICY_DENY;
    }

    if (fc == IEC61850_FC_DC) {
        if (self->iedServer->writeAccessPolicies & ALLOW_WRITE_ACCESS_DC)
            return ACCESS_POLICY_ALLOW;
        else
            return ACCESS_POLICY_DENY;
    }

    if (fc == IEC61850_FC_SP) {
        if (self->iedServer->writeAccessPolicies & ALLOW_WRITE_ACCESS_SP)
            return ACCESS_POLICY_ALLOW;
        else
            return ACCESS_POLICY_DENY;
    }

    if (fc == IEC61850_FC_SV) {
        if (self->iedServer->writeAccessPolicies & ALLOW_WRITE_ACCESS_SV)
            return ACCESS_POLICY_ALLOW;
        else
            return ACCESS_POLICY_DENY;
    }

    if (fc == IEC61850_FC_SE) {
        if (self->iedServer->writeAccessPolicies & ALLOW_WRITE_ACCESS_SE)
            return ACCESS_POLICY_ALLOW;
        else
            return ACCESS_POLICY_DENY;
    }

    return ACCESS_POLICY_DENY;
}

static MmsDataAccessError
mmsWriteHandler(void* parameter, MmsDomain* domain,
        char* variableId, MmsValue* value, MmsServerConnection connection)
{
    MmsMapping* self = (MmsMapping*) parameter;

    if (DEBUG_IED_SERVER)
        printf("IED_SERVER: Write requested %s\n", variableId);

    /* Access control based on functional constraint */

    char* separator = strchr(variableId, '$');

    if (separator == NULL)
        return DATA_ACCESS_ERROR_INVALID_ADDRESS;

    int lnNameLength = separator - variableId;

#if (CONFIG_IEC61850_CONTROL_SERVICE == 1)
    /* Controllable objects - CO */
    if (isControllable(separator)) {
        return Control_writeAccessControlObject(self, domain, variableId, value,
                connection);
    }
#endif /* (CONFIG_IEC61850_CONTROL_SERVICE == 1) */

#if (CONFIG_INCLUDE_GOOSE_SUPPORT == 1)

    /* Goose control block - GO */
    if (isGooseControlBlock(separator))
        return writeAccessGooseControlBlock(self, domain, variableId, value);

#endif /* (CONFIG_INCLUDE_GOOSE_SUPPORT == 1) */

#if (CONFIG_IEC61850_SAMPLED_VALUES_SUPPORT == 1)

    /* Sampled Value control block - MS/US */
    if (isSampledValueControlBlock(separator))
        return LIBIEC61850_SV_writeAccessSVControlBlock(self, domain, variableId, value, connection);

#endif /* (CONFIG_IEC61850_SAMPLED_VALUES_SUPPORT == 1) */

#if (CONFIG_IEC61850_LOG_SERVICE == 1)

    /* Log control block - LG */
    if (isLogControlBlock(separator))
        return LIBIEC61850_LOG_SVC_writeAccessLogControlBlock(self, domain, variableId, value, connection);

#endif /* (CONFIG_IEC61850_LOG_SERVICE == 1) */

#if (CONFIG_IEC61850_REPORT_SERVICE == 1)
    /* Report control blocks - BR, RP */
    if (isReportControlBlock(separator)) {

        char* reportName = MmsMapping_getNextNameElement(separator + 1);

        if (reportName == NULL)
            return DATA_ACCESS_ERROR_OBJECT_NONE_EXISTENT;

        separator = strchr(reportName, '$');

        int variableIdLen;

        if (separator != NULL)
            variableIdLen = separator - variableId;
        else
            variableIdLen = strlen(variableId);

        LinkedList nextElement = self->reportControls;

        while ((nextElement = LinkedList_getNext(nextElement)) != NULL) {
            ReportControl* rc = (ReportControl*) nextElement->data;

            if (rc->domain == domain) {

                int parentLNNameStrLen = strlen(rc->parentLN->name);

                if (parentLNNameStrLen != lnNameLength)
                    continue;

                if (memcmp(rc->parentLN->name, variableId, parentLNNameStrLen) != 0)
                    continue;

                int rcNameLen = strlen(rc->name);

                if (rcNameLen == variableIdLen) {

                    if (strncmp(variableId, rc->name, variableIdLen) == 0) {
                        char* elementName = variableId + rcNameLen + 1;

                        return Reporting_RCBWriteAccessHandler(self, rc, elementName, value, connection);
                    }
                }
            }
        }

        return DATA_ACCESS_ERROR_OBJECT_NONE_EXISTENT;
    }
#endif /* (CONFIG_IEC61850_REPORT_SERVICE == 1) */

#if (CONFIG_IEC61850_SETTING_GROUPS == 1)
    if (isSettingGroupControlBlock(separator)) {
        if (DEBUG_IED_SERVER)
            printf("IED_SERVER: Access to SGCB\n");

        char* nextSep = strchr(separator + 1, '$');

        if (nextSep != NULL) {
            nextSep = strchr(nextSep + 1, '$');

            char* nameId = nextSep + 1;

            if (strcmp(nameId, "ActSG") == 0) {
                SettingGroup* sg = getSettingGroupByMmsDomain(self, domain);

                if (sg != NULL) {
                    uint32_t val = MmsValue_toUint32(value);

                    if ((val > 0) && (val <= sg->sgcb->numOfSGs)) {
                        if (val != sg->sgcb->actSG) {

                            if (sg->actSgChangedHandler != NULL) {
                                if (sg->actSgChangedHandler(sg->actSgChangedHandlerParameter, sg->sgcb,
                                        (uint8_t) val, (ClientConnection) connection))
                                {
                                    sg->sgcb->actSG = val;

                                    MmsValue* actSg = MmsValue_getElement(sg->sgcbMmsValues, 1);
                                    MmsValue* lActTm = MmsValue_getElement(sg->sgcbMmsValues, 4);

                                    MmsValue_setUint8(actSg, sg->sgcb->actSG);
                                    MmsValue_setUtcTimeMs(lActTm, Hal_getTimeInMs());
                                }

                                else
                                    return DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED;
                            }
                            else
                                return DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED;

                        }

                        return DATA_ACCESS_ERROR_SUCCESS;
                    }
                    else
                        return DATA_ACCESS_ERROR_OBJECT_VALUE_INVALID;
                }
            }
            else if (strcmp(nameId, "EditSG") == 0) {
                SettingGroup* sg = getSettingGroupByMmsDomain(self, domain);

                if (sg != NULL) {
                    uint32_t val = MmsValue_toUint32(value);

                    if ((sg->editingClient != NULL) && ( sg->editingClient != (ClientConnection) connection))
                    	/* Edit SG was set by other client */
                    	return DATA_ACCESS_ERROR_TEMPORARILY_UNAVAILABLE;

					if (val == 0) {
						unselectEditSettingGroup(sg);
						return DATA_ACCESS_ERROR_SUCCESS;
					}

					if ((val > 0) && (val <= sg->sgcb->numOfSGs)) {

						if (sg->editSgChangedHandler != NULL) {

							if (sg->editSgChangedHandler(sg->editSgChangedHandlerParameter, sg->sgcb,
									(uint8_t) val, (ClientConnection) connection))
							{
								sg->sgcb->editSG = val;
								sg->editingClient = (ClientConnection) connection;

								sg->reservationTimeout = Hal_getTimeInMs() + (sg->sgcb->resvTms * 1000);

								MmsValue* editSg = MmsValue_getElement(sg->sgcbMmsValues, 2);
								MmsValue* resvTms = MmsValue_getElement(sg->sgcbMmsValues, 5);

								MmsValue_setUint16(resvTms, sg->sgcb->resvTms);
								MmsValue_setUint8(editSg, sg->sgcb->editSG);

								return DATA_ACCESS_ERROR_SUCCESS;
							}
							else
								return DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED;
						}
						else
							return DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED;

					}
					else
						return DATA_ACCESS_ERROR_OBJECT_VALUE_INVALID;

                }
            }
            else if (strcmp(nameId, "CnfEdit") == 0) {
                SettingGroup* sg = getSettingGroupByMmsDomain(self, domain);

                if (sg != NULL) {
                    bool val = MmsValue_getBoolean(value);

                    if (val == true) {
                        if (sg->sgcb->editSG != 0) {
                            if (sg->editingClient == (ClientConnection) connection) {
                                if (sg->editSgConfirmedHandler != NULL) {
                                    sg->editSgConfirmedHandler(sg->editSgConfirmedHandlerParameter, sg->sgcb,
                                            sg->sgcb->editSG);

                                    unselectEditSettingGroup(sg);

                                    return DATA_ACCESS_ERROR_SUCCESS;
                                }
                                else
                                    return DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED;
                            }
                            else
                                return DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED;
                        }
                        else
                            return DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED;
                    }
                    else
                        return DATA_ACCESS_ERROR_OBJECT_VALUE_INVALID;
                }
            }
        }

        return DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED;
    }
#endif /* (CONFIG_IEC61850_SETTING_GROUPS == 1) */

    FunctionalConstraint fc = getFunctionalConstraintForWritableNode(self, separator);

#if (CONFIG_IEC61850_SETTING_GROUPS == 1)
	if (fc == IEC61850_FC_SE) {
		SettingGroup* sg = getSettingGroupByMmsDomain(self, domain);

		if (sg->editingClient != (ClientConnection) connection)
		    return DATA_ACCESS_ERROR_TEMPORARILY_UNAVAILABLE;
	}
#endif /* (CONFIG_IEC61850_SETTING_GROUPS == 1) */

    /* writable data model elements - SP, SV, CF, DC */
    if (fc != IEC61850_FC_NONE) {
        MmsValue* cachedValue;

        cachedValue = MmsServer_getValueFromCache(self->mmsServer, domain, variableId);

        if (cachedValue != NULL) {

            if (!MmsValue_equalTypes(cachedValue, value))
                return DATA_ACCESS_ERROR_OBJECT_VALUE_INVALID;

            bool handlerFound = false;

            AccessPolicy nodeAccessPolicy = getAccessPolicyForFC(self, fc);

            if (DEBUG_IED_SERVER)
                printf("IED_SERVER: write to %s policy:%i\n", variableId, nodeAccessPolicy);

#if (CONFIG_IEC61850_SETTING_GROUPS == 1)
            if (isFunctionalConstraintSE(separator)) {
                SettingGroup* sg = getSettingGroupByMmsDomain(self, domain);

                if (sg != NULL) {
                    if (sg->sgcb->editSG == 0)
                        return DATA_ACCESS_ERROR_TEMPORARILY_UNAVAILABLE;
                }
                else
                    return DATA_ACCESS_ERROR_OBJECT_NONE_EXISTENT;
            }
#endif

            /* Call writer access handlers */
            LinkedList writeHandlerListElement = LinkedList_getNext(self->attributeAccessHandlers);

            while (writeHandlerListElement != NULL) {
                AttributeAccessHandler* accessHandler = (AttributeAccessHandler*) writeHandlerListElement->data;
                DataAttribute* dataAttribute = accessHandler->attribute;

                if (nodeAccessPolicy == ACCESS_POLICY_ALLOW) {

                    MmsValue* matchingValue = checkIfValueBelongsToModelNode(dataAttribute, cachedValue, value);

                    if (matchingValue != NULL) {
                        MmsDataAccessError handlerResult =
                            accessHandler->handler(dataAttribute, matchingValue, (ClientConnection) connection,
                            		accessHandler->parameter);

                        if (handlerResult == DATA_ACCESS_ERROR_SUCCESS)
                            handlerFound = true;
                        else
                            return handlerResult;
                    }

                }
                else { /* if ACCESS_POLICY_DENY only allow direct access to handled data attribute */
                    if (dataAttribute->mmsValue == cachedValue) {
                        MmsDataAccessError handlerResult =
                            accessHandler->handler(dataAttribute, value, (ClientConnection) connection,
                            		accessHandler->parameter);

                        if (handlerResult == DATA_ACCESS_ERROR_SUCCESS) {
                            handlerFound = true;
                            break;
                        }
                        else
                            return handlerResult;
                    }

                }

                writeHandlerListElement = LinkedList_getNext(writeHandlerListElement);
            }

            /* DENY access if no handler is found and default policy is DENY */
            if (!handlerFound) {

                if (nodeAccessPolicy == ACCESS_POLICY_DENY)
                    return DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED;

            }

            DataAttribute* da = IedModel_lookupDataAttributeByMmsValue(self->model, cachedValue);

            if (da != NULL)
                IedServer_updateAttributeValue(self->iedServer, da, value);
            else
                return DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED;

            /* Call observer callback */
            LinkedList observerListElement = LinkedList_getNext(self->observedObjects);

            while (observerListElement != NULL) {
                AttributeObserver* observer = (AttributeObserver*) observerListElement->data;
                DataAttribute* dataAttribute = observer->attribute;

                if (checkIfValueBelongsToModelNode(dataAttribute, cachedValue, value) != NULL) {
                    observer->handler(dataAttribute, (ClientConnection) connection);
                    break; /* only all one handler per data attribute */
                }

                observerListElement = LinkedList_getNext(observerListElement);
            }

            return DATA_ACCESS_ERROR_SUCCESS;
        }
        else
        	return DATA_ACCESS_ERROR_OBJECT_NONE_EXISTENT;
    }

    return DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED;
}

void
MmsMapping_addObservedAttribute(MmsMapping* self, DataAttribute* dataAttribute,
        AttributeChangedHandler handler)
{
    AttributeObserver* observer = (AttributeObserver*) GLOBAL_MALLOC(sizeof(AttributeObserver));

    observer->attribute = dataAttribute;
    observer->handler = handler;

    LinkedList_add(self->observedObjects, observer);
}

static AttributeAccessHandler*
getAccessHandlerForAttribute(MmsMapping* self, DataAttribute* dataAttribute)
{
    LinkedList element = LinkedList_getNext(self->attributeAccessHandlers);

    while (element != NULL) {
        AttributeAccessHandler* accessHandler = (AttributeAccessHandler*) element->data;

        if (accessHandler->attribute == dataAttribute)
            return accessHandler;

        element = LinkedList_getNext(element);
    }

    return NULL;
}

void
MmsMapping_installWriteAccessHandler(MmsMapping* self, DataAttribute* dataAttribute, WriteAccessHandler handler, void* parameter)
{
    AttributeAccessHandler* accessHandler = getAccessHandlerForAttribute(self, dataAttribute);

    if (accessHandler == NULL) {
        accessHandler = (AttributeAccessHandler*) GLOBAL_MALLOC(sizeof(AttributeAccessHandler));

        accessHandler->attribute = dataAttribute;
        accessHandler->parameter = parameter;
        LinkedList_add(self->attributeAccessHandlers, (void*) accessHandler);
    }

    accessHandler->handler = handler;
}

#if (CONFIG_INCLUDE_GOOSE_SUPPORT == 1)

static MmsValue*
readAccessGooseControlBlock(MmsMapping* self, MmsDomain* domain, char* variableIdOrig)
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

    MmsGooseControlBlock mmsGCB = lookupGCB(self, domain, lnName, objectName);

    if (mmsGCB != NULL) {
        if (varName != NULL) {
            value = MmsValue_getSubElement(MmsGooseControlBlock_getMmsValues(mmsGCB),
                    MmsGooseControlBlock_getVariableSpecification(mmsGCB), varName);
        }
        else {
            value = MmsGooseControlBlock_getMmsValues(mmsGCB);
        }
    }

    return value;
}

#endif /* (CONFIG_INCLUDE_GOOSE_SUPPORT == 1) */


static MmsValue*
mmsReadHandler(void* parameter, MmsDomain* domain, char* variableId, MmsServerConnection connection)
{
    MmsMapping* self = (MmsMapping*) parameter;

    MmsValue* retValue = NULL;

    if (DEBUG_IED_SERVER)
        printf("IED_SERVER: mmsReadHandler: Requested %s\n", variableId);

    char* separator = strchr(variableId, '$');

    if (separator == NULL)
        goto exit_function;

    int lnNameLength = separator - variableId;

#if (CONFIG_IEC61850_CONTROL_SERVICE == 1)
    /* Controllable objects - CO */
    if (isControllable(separator)) {
        retValue = Control_readAccessControlObject(self, domain, variableId, connection);
        goto exit_function;
    }
#endif


#if (CONFIG_INCLUDE_GOOSE_SUPPORT == 1)
    /* GOOSE control blocks - GO */
    if (isGooseControlBlock(separator)) {
        retValue = readAccessGooseControlBlock(self, domain, variableId);
        goto exit_function;
    }
#endif

#if (CONFIG_IEC61850_SAMPLED_VALUES_SUPPORT == 1)
    /* Sampled Value control blocks - MS/US */
    if (isSampledValueControlBlock(separator)) {
        retValue = LIBIEC61850_SV_readAccessSampledValueControlBlock(self, domain, variableId);
        goto exit_function;
    }
#endif

#if (CONFIG_IEC61850_LOG_SERVICE == 1)
    /* LOG control block - LG */
    if (isLogControlBlock(separator)) {
        retValue = LIBIEC61850_LOG_SVC_readAccessControlBlock(self, domain, variableId);
        goto exit_function;
    }
#endif

#if (CONFIG_IEC61850_REPORT_SERVICE == 1)
    /* Report control blocks - BR, RP */
    if (isReportControlBlock(separator)) {

        LinkedList reportControls = self->reportControls;

        LinkedList nextElement = reportControls;

        char* reportName = MmsMapping_getNextNameElement(separator + 1);

        if (reportName == NULL) {
            retValue = NULL;
            goto exit_function;
        }

        separator = strchr(reportName, '$');

        size_t variableIdLen;

        if (separator != NULL)
            variableIdLen = separator - variableId;
        else
            variableIdLen = strlen(variableId);

        while ((nextElement = LinkedList_getNext(nextElement)) != NULL) {
            ReportControl* rc = (ReportControl*) nextElement->data;

            if (rc->domain == domain) {

                int parentLNNameStrLen = strlen(rc->parentLN->name);

                if (parentLNNameStrLen != lnNameLength)
                    continue;

                if (memcmp(rc->parentLN->name, variableId, parentLNNameStrLen) != 0)
                    continue;

                if (strlen(rc->name) == variableIdLen) {
                    if (strncmp(variableId, rc->name, variableIdLen) == 0) {

                        char* elementName = MmsMapping_getNextNameElement(reportName);

                        MmsValue* value = NULL;

                        if (elementName != NULL)
                            value = ReportControl_getRCBValue(rc, elementName);
                        else
                            value = rc->rcbValues;

                        retValue = value;

                        goto exit_function;
                    }
                }

            }

        }
    }
#endif /* (CONFIG_IEC61850_REPORT_SERVICE == 1) */

exit_function:
    return retValue;
}

void
MmsMapping_setMmsServer(MmsMapping* self, MmsServer server)
{
    self->mmsServer = server;
}

#if (CONFIG_IEC61850_CONTROL_SERVICE == 1)
static void
unselectControlsForConnection(MmsMapping* self, MmsServerConnection connection)
{
    LinkedList controlObjectElement = LinkedList_getNext(self->controlObjects);

    while (controlObjectElement != NULL) {
        ControlObject* controlObject = (ControlObject*) controlObjectElement->data;

        if (ControlObject_unselect(controlObject, connection))
            break;

        controlObjectElement = LinkedList_getNext(controlObjectElement);
    }
}
#endif /* (CONFIG_IEC61850_CONTROL_SERVICE == 1) */

static void /* is called by MMS server layer */
mmsConnectionHandler(void* parameter, MmsServerConnection connection, MmsServerEvent event)
{
    MmsMapping* self = (MmsMapping*) parameter;

    if (event == MMS_SERVER_CONNECTION_CLOSED) {
        ClientConnection clientConnection = private_IedServer_getClientConnectionByHandle(self->iedServer, connection);

        /* call user provided handler function */
        if (self->connectionIndicationHandler != NULL)
            self->connectionIndicationHandler(self->iedServer, clientConnection, false,
                    self->connectionIndicationHandlerParameter);

        private_IedServer_removeClientConnection(self->iedServer, clientConnection);

#if (CONFIG_MMS_THREADLESS_STACK != 1)
        /* wait until control threads are finished */
        while (private_ClientConnection_getTasksCount(clientConnection) > 0)
            Thread_sleep(10);
#endif /* (CONFIG_MMS_THREADLESS_STACK != 1) */

#if (CONFIG_IEC61850_REPORT_SERVICE == 1)
        Reporting_deactivateReportsForConnection(self, connection);
#endif

#if (CONFIG_IEC61850_CONTROL_SERVICE == 1)
        unselectControlsForConnection(self, connection);
#endif

#if (CONFIG_IEC61850_SETTING_GROUPS == 1)
        unselectAllSettingGroups(self, connection);
#endif

        private_ClientConnection_destroy(clientConnection);
    }
    else if (event == MMS_SERVER_NEW_CONNECTION) {
        /* call user provided handler function */
        ClientConnection newClientConnection = private_ClientConnection_create(connection);

        private_IedServer_addNewClientConnection(self->iedServer, newClientConnection);

        /* call user provided handler function */
        if (self->connectionIndicationHandler != NULL)
            self->connectionIndicationHandler(self->iedServer, newClientConnection, true,
                    self->connectionIndicationHandlerParameter);
    }
}

static MmsDataAccessError
mmsReadAccessHandler (void* parameter, MmsDomain* domain, char* variableId, MmsServerConnection connection)
{
    MmsMapping* self = (MmsMapping*) parameter;

    if (DEBUG_IED_SERVER)
        printf("IED_SERVER: mmsReadAccessHandler: Requested %s\n", variableId);

    char* separator = strchr(variableId, '$');

    if (separator == NULL)
        return DATA_ACCESS_ERROR_SUCCESS;

#if (CONFIG_IEC61850_SETTING_GROUPS == 1)

    if (isFunctionalConstraintSE(separator)) {
        SettingGroup* sg = getSettingGroupByMmsDomain(self, domain);

        if (sg != NULL) {
            if (sg->sgcb->editSG == 0)
                return DATA_ACCESS_ERROR_TEMPORARILY_UNAVAILABLE;
        }
        else
            return DATA_ACCESS_ERROR_OBJECT_NONE_EXISTENT;
    }

#endif /* (CONFIG_IEC61850_SETTING_GROUPS == 1) */

    return DATA_ACCESS_ERROR_SUCCESS;
}

static MmsError
variableListChangedHandler (void* parameter, bool create, MmsVariableListType listType, MmsDomain* domain,
        char* listName, MmsServerConnection connection)
{
    MmsError allow = MMS_ERROR_NONE;

#if (DEBUG_IED_SERVER == 1)
    if (create)
        printf("IED_SERVER: create data set ");
    else
        printf("IED_SERVER: delete data set ");

    switch (listType) {
    case MMS_VMD_SPECIFIC:
        printf("VMD ");
        break;
    case MMS_ASSOCIATION_SPECIFIC:
        printf("association ");
        break;
    case MMS_DOMAIN_SPECIFIC:
        printf("domain ");
        break;
    }

    printf("specific (name=%s)\n", listName);
#endif /* (DEBUG_IED_SERVER == 1) */

    MmsMapping* self = (MmsMapping*) parameter;

    if (create) {
        if (listType == MMS_DOMAIN_SPECIFIC) {
            // check if LN exists - otherwise reject request (to fulfill test case sDsN1c)

            allow = MMS_ERROR_ACCESS_OBJECT_NON_EXISTENT;

            IedModel* model = self->model;

            LogicalDevice* ld = IedModel_getDevice(model, domain->domainName);

            if (ld != NULL) {

                char lnName[129];

                char* separator = strchr(listName, '$');

                if (separator != NULL) {
                    int lnNameLen = separator - listName;

                    memcpy(lnName, listName, lnNameLen);
                    lnName[lnNameLen] = 0;

                    if (LogicalDevice_getLogicalNode(ld, lnName) != NULL)
                        allow = MMS_ERROR_NONE;
                }

            }

        }
    }
    else {
        /* Check if data set is referenced in a report */

        LinkedList rcElement = self->reportControls;

        while ((rcElement = LinkedList_getNext(rcElement)) != NULL) {
            ReportControl* rc = (ReportControl*) rcElement->data;

            if (rc->isDynamicDataSet) {
                if (rc->dataSet != NULL) {

                    if (listType == MMS_DOMAIN_SPECIFIC) {
                        if (rc->dataSet->logicalDeviceName != NULL) {
                            if (strcmp(rc->dataSet->name, listName) == 0) {
                                if (strcmp(rc->dataSet->logicalDeviceName, MmsDomain_getName(domain)) == 0) {
                                    allow = MMS_ERROR_SERVICE_OBJECT_CONSTRAINT_CONFLICT;
                                    break;
                                }
                            }
                        }
                    }
                    else if (listType == MMS_VMD_SPECIFIC) {
                        if (rc->dataSet->logicalDeviceName == NULL) {
                            if (strcmp(rc->dataSet->name, listName) == 0) {
                                allow = MMS_ERROR_SERVICE_OBJECT_CONSTRAINT_CONFLICT;
                               break;
                            }
                        }
                    }
                    else if (listType == MMS_ASSOCIATION_SPECIFIC) {
                        if (rc->dataSet->logicalDeviceName == NULL) {
                            if (strcmp(rc->dataSet->name, listName) == 0) {
                                allow = MMS_ERROR_SERVICE_OBJECT_CONSTRAINT_CONFLICT;
                                break;
                            }
                        }
                    }

                }
            }
        }


#if (CONFIG_IEC61850_LOG_SERVICE == 1)
        /* check if data set is referenced in a log control block*/
        LinkedList logElement = self->logControls;

        while ((logElement = LinkedList_getNext(logElement)) != NULL) {
            LogControl* lc = (LogControl*) logElement->data;

            if (lc->isDynamicDataSet) {
                if (lc->dataSet != NULL) {

                    if (listType == MMS_DOMAIN_SPECIFIC) {
                        if (lc->dataSet->logicalDeviceName != NULL) {
                            if (strcmp(lc->dataSet->name, listName) == 0) {
                                if (strcmp(lc->dataSet->logicalDeviceName, MmsDomain_getName(domain)) == 0) {
                                    allow = MMS_ERROR_SERVICE_OBJECT_CONSTRAINT_CONFLICT;
                                    break;
                                }
                            }
                        }
                    }
                    else if (listType == MMS_VMD_SPECIFIC) {
                        if (lc->dataSet->logicalDeviceName == NULL) {
                            if (strcmp(lc->dataSet->name, listName) == 0) {
                                allow = MMS_ERROR_SERVICE_OBJECT_CONSTRAINT_CONFLICT;
                               break;
                            }
                        }
                    }

                }
            }
        }

#endif /* (CONFIG_IEC61850_LOG_SERVICE == 1) */
    }

    return allow;
}

void
MmsMapping_installHandlers(MmsMapping* self)
{
    MmsServer_installReadHandler(self->mmsServer, mmsReadHandler, (void*) self);
    MmsServer_installWriteHandler(self->mmsServer, mmsWriteHandler, (void*) self);
    MmsServer_installReadAccessHandler(self->mmsServer, mmsReadAccessHandler, (void*) self);
    MmsServer_installConnectionHandler(self->mmsServer, mmsConnectionHandler, (void*) self);
    MmsServer_installVariableListChangedHandler(self->mmsServer, variableListChangedHandler, (void*) self);
}

void
MmsMapping_setIedServer(MmsMapping* self, IedServer iedServer)
{
    self->iedServer = iedServer;
}

void
MmsMapping_setConnectionIndicationHandler(MmsMapping* self, IedConnectionIndicationHandler handler, void* parameter)
{
    self->connectionIndicationHandler = handler;
    self->connectionIndicationHandlerParameter = parameter;
}



static bool
isMemberValueRecursive(MmsValue* container, MmsValue* value)
{
    bool isMemberValue = false;

    if (container == value)
        isMemberValue = true;
    else {
        if ((MmsValue_getType(container) == MMS_STRUCTURE) ||
                (MmsValue_getType(container) == MMS_ARRAY))
        {

            int compCount = MmsValue_getArraySize(container);
            int i;
            for (i = 0; i < compCount; i++) {
                if (isMemberValueRecursive(MmsValue_getElement(container, i), value)) {
                    isMemberValue = true;
                    break;
                }
            }
        }
    }

    return isMemberValue;
}

#if ((CONFIG_IEC61850_REPORT_SERVICE == 1) || (CONFIG_INCLUDE_GOOSE_SUPPORT == 1))

static bool
DataSet_isMemberValue(DataSet* dataSet, MmsValue* value, int* index)
{
    int i = 0;

    DataSetEntry* dataSetEntry = dataSet->fcdas;

    while (dataSetEntry != NULL) {

        MmsValue* dataSetValue = dataSetEntry->value;

        if (dataSetValue != NULL) { /* prevent invalid data set members */
            if (isMemberValueRecursive(dataSetValue, value)) {
                if (index != NULL)
                    *index = i;

                return true;
            }
        }

        i++;

        dataSetEntry = dataSetEntry->sibling;
    }

    return false;
}
#endif /* ((CONFIG_IEC61850_REPORT_SERVICE == 1) || (CONFIG_INCLUDE_GOOSE_SUPPORT)) */

#if (CONFIG_IEC61850_LOG_SERVICE == 1)

static bool
DataSet_isMemberValueWithRef(DataSet* dataSet, MmsValue* value, char* dataRef, const char* iedName)
{
    int i = 0;

     DataSetEntry* dataSetEntry = dataSet->fcdas;

     while (dataSetEntry != NULL) {

         MmsValue* dataSetValue = dataSetEntry->value;

         if (dataSetValue != NULL) { /* prevent invalid data set members */
             if (isMemberValueRecursive(dataSetValue, value)) {
                 if (dataRef != NULL)
                     sprintf(dataRef, "%s%s/%s", iedName, dataSetEntry->logicalDeviceName, dataSetEntry->variableName);

                 return true;
             }
         }

         i++;

         dataSetEntry = dataSetEntry->sibling;
     }

     return false;
}

void
MmsMapping_triggerLogging(MmsMapping* self, MmsValue* value, LogInclusionFlag flag)
{
    LinkedList element = self->logControls;

    while ((element = LinkedList_getNext(element)) != NULL) {
        LogControl* lc = (LogControl*) element->data;

        if ((lc->enabled) && (lc->dataSet != NULL)) {

            uint8_t reasonCode;

            switch (flag) {

            case LOG_CONTROL_VALUE_UPDATE:
                if ((lc->triggerOps & TRG_OPT_DATA_UPDATE) == 0)
                    continue;

                reasonCode = TRG_OPT_DATA_UPDATE * 2;

                break;

            case LOG_CONTROL_VALUE_CHANGED:
                if (((lc->triggerOps & TRG_OPT_DATA_CHANGED) == 0) &&
                        ((lc->triggerOps & TRG_OPT_DATA_UPDATE) == 0))
                    continue;

                reasonCode = TRG_OPT_DATA_CHANGED * 2;

                break;

            case LOG_CONTROL_QUALITY_CHANGED:
                if ((lc->triggerOps & TRG_OPT_QUALITY_CHANGED) == 0)
                    continue;

                reasonCode = TRG_OPT_QUALITY_CHANGED * 2;

                break;

            default:
                continue;
            }

            char dataRef[130];

            if (DataSet_isMemberValueWithRef(lc->dataSet, value, dataRef, self->model->name)) {

                if (lc->logInstance != NULL) {
                    LogInstance_logSingleData(lc->logInstance, dataRef, value, reasonCode);
                }
                else
                    printf("No log instance available!\n");
            }


        }
    }
}

#endif /* (CONFIG_IEC61850_LOG_SERVICE == 1) */

#if (CONFIG_IEC61850_REPORT_SERVICE == 1)
void
MmsMapping_triggerReportObservers(MmsMapping* self, MmsValue* value, ReportInclusionFlag flag)
{
    LinkedList element = self->reportControls;

    while ((element = LinkedList_getNext(element)) != NULL) {
        ReportControl* rc = (ReportControl*) element->data;

        if (rc->enabled || (rc->buffered && rc->dataSet != NULL)) {
            int index;

            switch (flag) {
            case REPORT_CONTROL_VALUE_UPDATE:
                if ((rc->triggerOps & TRG_OPT_DATA_UPDATE) == 0)
                    continue;
                break;
            case REPORT_CONTROL_VALUE_CHANGED:
                if (((rc->triggerOps & TRG_OPT_DATA_CHANGED) == 0) &&
                        ((rc->triggerOps & TRG_OPT_DATA_UPDATE) == 0))
                    continue;
                break;
            case REPORT_CONTROL_QUALITY_CHANGED:
                if ((rc->triggerOps & TRG_OPT_QUALITY_CHANGED) == 0)
                    continue;
                break;
            default:
                continue;
            }

            if (DataSet_isMemberValue(rc->dataSet, value, &index)) {
                ReportControl_valueUpdated(rc, index, flag);
            }
        }
    }
}

#endif /* (CONFIG_IEC61850_REPORT_SERVICE == 1) */

#if (CONFIG_INCLUDE_GOOSE_SUPPORT == 1)

void
MmsMapping_triggerGooseObservers(MmsMapping* self, MmsValue* value)
{
    LinkedList element = self->gseControls;

    while ((element = LinkedList_getNext(element)) != NULL) {
        MmsGooseControlBlock gcb = (MmsGooseControlBlock) element->data;

        if (MmsGooseControlBlock_isEnabled(gcb)) {
            DataSet* dataSet = MmsGooseControlBlock_getDataSet(gcb);

            if (DataSet_isMemberValue(dataSet, value, NULL)) {
                MmsGooseControlBlock_observedObjectChanged(gcb);
            }
        }
    }
}

void
MmsMapping_enableGoosePublishing(MmsMapping* self)
{

    LinkedList element = self->gseControls;

    while ((element = LinkedList_getNext(element)) != NULL) {
        MmsGooseControlBlock gcb = (MmsGooseControlBlock) element->data;

        MmsGooseControlBlock_enable(gcb);
    }

}

void
MmsMapping_disableGoosePublishing(MmsMapping* self)
{
    LinkedList element = self->gseControls;

    while ((element = LinkedList_getNext(element)) != NULL) {
        MmsGooseControlBlock gcb = (MmsGooseControlBlock) element->data;

        MmsGooseControlBlock_disable(gcb);
    }
}

#endif /* (CONFIG_INCLUDE_GOOSE_SUPPORT == 1) */

#if (CONFIG_IEC61850_CONTROL_SERVICE == 1)
void
MmsMapping_addControlObject(MmsMapping* self, ControlObject* controlObject)
{
    LinkedList_add(self->controlObjects, controlObject);
}

ControlObject*
MmsMapping_getControlObject(MmsMapping* self, MmsDomain* domain, char* lnName, char* coName)
{
    return Control_lookupControlObject(self, domain, lnName, coName);
}
#endif /* (CONFIG_IEC61850_CONTROL_SERVICE == 1) */


char*
MmsMapping_getMmsDomainFromObjectReference(const char* objectReference, char* buffer)
{
    int objRefLength = strlen(objectReference);
    char* domainName = NULL;

    /* check for object reference size limit VISIBLESTRING129 */
    if (objRefLength > 129)
        goto exit_function;

    /* check if LD name is present */
    int i;
    for (i = 0; i < objRefLength; i++) {
        if (objectReference[i] == '/') {
            break;
        }
    }

    /* check for LD name limit (=64 characters) */
    if (i > 64)
        goto exit_function;

    if (i == objRefLength)
        goto exit_function;

    if (buffer == NULL)
        domainName = (char*) GLOBAL_MALLOC(i + 1);
    else
        domainName = buffer;

    int j;
    for (j = 0; j < i; j++) {
        domainName[j] = objectReference[j];
    }

    domainName[j] = 0;

exit_function:
    return domainName;
}

char*
MmsMapping_createMmsVariableNameFromObjectReference(const char* objectReference,
        FunctionalConstraint fc, char* buffer)
{
    int objRefLength = strlen(objectReference);

    /* check for object reference size limit VISIBLESTRING129 */
    if (objRefLength > 129)
        return NULL;

    /* check if LD name is present */
    int i;
    for (i = 0; i < objRefLength; i++) {
        if (objectReference[i] == '/') {
            break;
        }
    }

    /* check for LD name limit (= 64 characters) */
    if (i > 64)
        return NULL;

    if (i == objRefLength)
        i = 0; /* for the case when no LD name is present */
    else
        i++;


    if (fc == IEC61850_FC_NONE) {

        int len = objRefLength - i;

        char* mmsVariableName;

        if (buffer == NULL)
            mmsVariableName = (char*) GLOBAL_MALLOC(len);
        else
            mmsVariableName = buffer;

        strcpy(mmsVariableName, objectReference + i);

        return mmsVariableName;
    }

    char* fcString = FunctionalConstraint_toString(fc);

    if (fcString == NULL)
        return NULL;

    int namePartLength = objRefLength - i - 1;

    /* ensure that limit due to MMS name part length = 64 is not exceeded */
    if (namePartLength > 61)
        return NULL;

    char* mmsVariableName;

    if (buffer == NULL)
        mmsVariableName = (char*) GLOBAL_MALLOC(namePartLength + 5);
    else
        mmsVariableName = buffer;

    int sourceIndex = i;
    int destIndex = 0;

    bool fcAdded = false;

    while (sourceIndex < objRefLength) {

        if (objectReference[sourceIndex] != '.')
            mmsVariableName[destIndex++] = objectReference[sourceIndex++];
        else {

            if (!fcAdded) {
                mmsVariableName[destIndex++] = '$';
                mmsVariableName[destIndex++] = fcString[0];
                mmsVariableName[destIndex++] = fcString[1];
                mmsVariableName[destIndex++] = '$';

                fcAdded = true;
            }
            else
                mmsVariableName[destIndex++] = '$';

            sourceIndex++;
        }
    }

    if (!fcAdded) {
        mmsVariableName[destIndex++] = '$';
        mmsVariableName[destIndex++] = fcString[0];
        mmsVariableName[destIndex++] = fcString[1];
    }

    mmsVariableName[destIndex] = 0;

    return mmsVariableName;
}

#if (CONFIG_INCLUDE_GOOSE_SUPPORT == 1)

static void
GOOSE_processGooseEvents(MmsMapping* self, uint64_t currentTimeInMs)
{
    LinkedList element = LinkedList_getNext(self->gseControls);

    while (element != NULL) {
        MmsGooseControlBlock mmsGCB = (MmsGooseControlBlock) element->data;

        if (MmsGooseControlBlock_isEnabled(mmsGCB)) {
            MmsGooseControlBlock_checkAndPublish(mmsGCB, currentTimeInMs);
        }

        element = LinkedList_getNext(element);
    }
}

#endif /* (CONFIG_INCLUDE_GOOSE_SUPPORT == 1) */



static void
processPeriodicTasks(MmsMapping* self)
{
    uint64_t currentTimeInMs = Hal_getTimeInMs();

#if (CONFIG_INCLUDE_GOOSE_SUPPORT == 1)
    GOOSE_processGooseEvents(self, currentTimeInMs);
#endif

#if (CONFIG_IEC61850_CONTROL_SERVICE == 1)
    Control_processControlActions(self, currentTimeInMs);
#endif

#if (CONFIG_IEC61850_REPORT_SERVICE == 1)
    Reporting_processReportEvents(self, currentTimeInMs);
#endif

#if (CONFIG_IEC61850_SETTING_GROUPS == 1)
    MmsMapping_checkForSettingGroupReservationTimeouts(self, currentTimeInMs);
#endif

#if (CONFIG_IEC61850_LOG_SERVICE == 1)
    Logging_processIntegrityLogs(self, currentTimeInMs);
#endif

    /* handle low priority MMS backgound tasks (like file upload...) */
    MmsServer_handleBackgroundTasks(self->mmsServer);
}

void
IedServer_performPeriodicTasks(IedServer self)
{
    processPeriodicTasks(self->mmsMapping);
}

#if (CONFIG_MMS_THREADLESS_STACK != 1)
/* single worker thread for all enabled GOOSE and report control blocks */
static void
eventWorkerThread(MmsMapping* self)
{
    bool running = true;
    self->reportThreadFinished = false;

    while (running) {

        processPeriodicTasks(self);

        Thread_sleep(1); /* hand-over control to other threads */

        running = self->reportThreadRunning;
    }

    if (DEBUG_IED_SERVER)
        printf("IED_SERVER: event worker thread finished!\n");

    self->reportThreadFinished = true;
}

void
MmsMapping_startEventWorkerThread(MmsMapping* self)
{
    self->reportThreadRunning = true;

    Thread thread = Thread_create((ThreadExecutionFunction) eventWorkerThread, self, false);
    self->reportWorkerThread = thread;
    Thread_start(thread);
}

void
MmsMapping_stopEventWorkerThread(MmsMapping* self)
{
    if (self->reportThreadRunning) {

        self->reportThreadRunning = false;

        while (self->reportThreadFinished == false)
            Thread_sleep(1);
    }
}
#endif /* (CONFIG_MMS_THREADLESS_STACK != 1) */

DataSet*
MmsMapping_createDataSetByNamedVariableList(MmsMapping* self, MmsNamedVariableList variableList)
{
    DataSet* dataSet = (DataSet*) GLOBAL_MALLOC(sizeof(DataSet));

    if (variableList->domain != NULL)
        dataSet->logicalDeviceName = MmsDomain_getName(variableList->domain);
    else
        dataSet->logicalDeviceName = NULL; /* name is not relevant for association specific data sets */

    dataSet->name = variableList->name;
    dataSet->elementCount = LinkedList_size(variableList->listOfVariables);

    LinkedList element = LinkedList_getNext(variableList->listOfVariables);

    DataSetEntry* lastDataSetEntry = NULL;

    while (element != NULL) {
        MmsAccessSpecifier* listEntry = (MmsAccessSpecifier*) element->data;

        DataSetEntry* dataSetEntry = (DataSetEntry*) GLOBAL_MALLOC(sizeof(DataSetEntry));

        /* use variable name part of domain name as logicalDeviceName */
        dataSetEntry->logicalDeviceName = MmsDomain_getName(listEntry->domain) + strlen(self->model->name);

        dataSetEntry->variableName = listEntry->variableName;
        dataSetEntry->index = listEntry->arrayIndex;
        dataSetEntry->componentName = listEntry->componentName;
        dataSetEntry->sibling = NULL;

        if (lastDataSetEntry == NULL)
            dataSet->fcdas =dataSetEntry;
        else
            lastDataSetEntry->sibling = dataSetEntry;

        dataSetEntry->value =
                MmsServer_getValueFromCache(self->mmsServer, listEntry->domain, listEntry->variableName);

        lastDataSetEntry = dataSetEntry;

        element = LinkedList_getNext(element);
    }

    return dataSet;
}

MmsNamedVariableList
MmsMapping_getDomainSpecificVariableList(MmsMapping* self, const char* variableListReference)
{
    char variableListReferenceCopy[193];

    strncpy(variableListReferenceCopy, variableListReference, 192);
	variableListReferenceCopy[192] = 0;

    char* separator = strchr(variableListReferenceCopy, '/');

    if (separator == NULL)
        return NULL;

    char* domainName = variableListReferenceCopy;

    char* variableListName = separator + 1;

    *separator = 0;

    MmsDomain* domain = MmsDevice_getDomain(self->mmsDevice, domainName);

    if (domain == NULL)
        return NULL;

    MmsNamedVariableList variableList = MmsDomain_getNamedVariableList(domain, variableListName);

    return variableList;
}

DataSet*
MmsMapping_getDomainSpecificDataSet(MmsMapping* self, const char* dataSetName)
{
    MmsNamedVariableList variableList = MmsMapping_getDomainSpecificVariableList(self, dataSetName);

    if (variableList == NULL)
        return NULL;

    return MmsMapping_createDataSetByNamedVariableList(self, variableList);
}

void
MmsMapping_freeDynamicallyCreatedDataSet(DataSet* dataSet)
{
    DataSetEntry* dataSetEntry = dataSet->fcdas;

    while (dataSetEntry != NULL) {
        DataSetEntry* nextEntry = dataSetEntry->sibling;

        GLOBAL_FREEMEM (dataSetEntry);

        dataSetEntry = nextEntry;
    }

    GLOBAL_FREEMEM(dataSet);
}

MmsVariableAccessSpecification*
MmsMapping_ObjectReferenceToVariableAccessSpec(char* objectReference)
{
    char* domainIdEnd = strchr(objectReference, '/');

    if (domainIdEnd == NULL) /* no logical device name present */
        return NULL;

    int domainIdLen = domainIdEnd - objectReference;

    if (domainIdLen > 64)
        return NULL;

    char* fcStart = strchr(objectReference, '[');

    if (fcStart == NULL) /* no FC present */
        return NULL;

    char* fcEnd = strchr(fcStart, ']');

    if (fcEnd == NULL) /* syntax error in FC */
        return NULL;

    if ((fcEnd - fcStart) != 3) /* syntax error in FC */
        return NULL;

    FunctionalConstraint fc = FunctionalConstraint_fromString(fcStart + 1);

    MmsVariableAccessSpecification* accessSpec =
            (MmsVariableAccessSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableAccessSpecification));

    accessSpec->domainId = StringUtils_createStringFromBuffer((uint8_t*) objectReference, domainIdLen);

    char* indexBrace = strchr(domainIdEnd, '(');

    char* itemIdEnd = indexBrace;

    if (itemIdEnd == NULL)
        itemIdEnd = strchr(domainIdEnd, '[');

    int objRefLen = strlen(objectReference);

    accessSpec->arrayIndex = -1; /* -1 --> not present */

    if (itemIdEnd != NULL) {
        int itemIdLen = itemIdEnd - domainIdEnd - 1;

        char itemIdStr[129];

        memcpy(itemIdStr, (domainIdEnd + 1), itemIdLen);
        itemIdStr[itemIdLen] = 0;

        accessSpec->itemId = MmsMapping_createMmsVariableNameFromObjectReference(itemIdStr, fc, NULL);

        if (indexBrace != NULL) {

            char* indexStart = itemIdEnd + 1;

            char* indexEnd = strchr(indexStart, ')');

            int indexLen = indexEnd - indexStart;

            int index = StringUtils_digitsToInt(indexStart, indexLen);

            accessSpec->arrayIndex = (int32_t) index;

            int componentNameLen = objRefLen - ((indexEnd + 2) - objectReference) - 4;

            if (componentNameLen > 0) {
                accessSpec->componentName = StringUtils_createStringFromBuffer((uint8_t*) (indexEnd + 2), componentNameLen);
                StringUtils_replace(accessSpec->componentName, '.', '$');
            }
        }
    }

    return accessSpec;
}

static int
getNumberOfDigits(int value)
{
    int numberOfDigits = 1;

    while (value > 9) {
        numberOfDigits++;
        value /= 10;
    }

    return numberOfDigits;
}

char*
MmsMapping_varAccessSpecToObjectReference(MmsVariableAccessSpecification* varAccessSpec)
{
    char* domainId = varAccessSpec->domainId;

    int domainIdLen = strlen(domainId);

    char* itemId = varAccessSpec->itemId;

    char* separator = strchr(itemId, '$');

    int itemIdLen = strlen(itemId);

    int arrayIndexLen = 0;

    int componentPartLen = 0;

    if (varAccessSpec->componentName != NULL)
        componentPartLen = strlen(varAccessSpec->componentName);

    if (varAccessSpec->arrayIndex > -1)
        arrayIndexLen = 2 + getNumberOfDigits(varAccessSpec->arrayIndex);

    int newStringLen = (domainIdLen + 1) + (itemIdLen - 2) + arrayIndexLen + 4 /* for FC */+ componentPartLen + 1;

    char* newString = (char*) GLOBAL_MALLOC(newStringLen);

    char* targetPos = newString;

    /* Copy domain id part */
    char* currentPos = domainId;

    while (currentPos < (domainId + domainIdLen)) {
        *targetPos = *currentPos;
        targetPos++;
        currentPos++;
    }

    *targetPos = '/';
    targetPos++;

    /* Copy item id parts */
    currentPos = itemId;

    while (currentPos < separator) {
        *targetPos = *currentPos;
        targetPos++;
        currentPos++;
    }

    *targetPos = '.';
    targetPos++;

    currentPos = separator + 4;

    while (currentPos < (itemId + itemIdLen)) {
        if (*currentPos == '$')
            *targetPos = '.';
        else
            *targetPos = *currentPos;

        targetPos++;
        currentPos++;
    }

    /* Add array index part */
    if (varAccessSpec->arrayIndex > -1) {
        sprintf(targetPos, "(%i)", varAccessSpec->arrayIndex);
        targetPos += arrayIndexLen;
    }

    /* Add component part */
    if (varAccessSpec->componentName != NULL) {
        *targetPos = '.';
        targetPos++;

        int i;
        for (i = 0; i < componentPartLen; i++) {
            if (varAccessSpec->componentName[i] == '$')
                *targetPos = '.';
            else
                *targetPos = varAccessSpec->componentName[i];

            targetPos++;
        }
    }

    /* add FC part */
    *targetPos = '[';
    targetPos++;
    *targetPos = *(separator + 1);
    targetPos++;
    *targetPos = *(separator + 2);
    targetPos++;
    *targetPos = ']';
    targetPos++;

    *targetPos = 0; /* add terminator */

    return newString;
}

