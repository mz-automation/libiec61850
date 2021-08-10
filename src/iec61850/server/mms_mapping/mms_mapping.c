/*
 *  mms_mapping.c
 *
 *  Copyright 2013-2021 Michael Zillgith
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
#include "mms_server_internal.h"
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

    MmsServerConnection editingClient;
    uint64_t reservationTimeout;
} SettingGroup;

#if (CONFIG_IEC61850_CONTROL_SERVICE == 1)
MmsValue*
Control_readAccessControlObject(MmsMapping* self, MmsDomain* domain, char* variableIdOrig,
        MmsServerConnection connection, bool isDirectAccess);
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
            namedVariable->typeSpec.bitString = -13; /* -13 = up to 13 bits */
            namedVariable->type = MMS_BIT_STRING;
            break;
        case IEC61850_ENTRY_TIME:
            namedVariable->type = MMS_BINARY_TIME;
            namedVariable->typeSpec.binaryTime = 6;
            break;
        case IEC61850_PHYCOMADDR:
            MmsMapping_createPhyComAddrStructure(namedVariable);
            break;
        case IEC61850_OPTFLDS:
            namedVariable->typeSpec.bitString = -10;
            namedVariable->type = MMS_BIT_STRING;
            break;
        case IEC61850_TRGOPS:
            namedVariable->typeSpec.bitString = -6;
            namedVariable->type = MMS_BIT_STRING;
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
createSGCB(bool withResvTms)
{
    int numberOfElements = 5;

    if (withResvTms)
        numberOfElements = 6;

    MmsVariableSpecification* namedVariable = (MmsVariableSpecification*) GLOBAL_CALLOC(1,
            sizeof(MmsVariableSpecification));
    namedVariable->name = StringUtils_copyString("SGCB");
    namedVariable->type = MMS_STRUCTURE;

    namedVariable->typeSpec.structure.elementCount = numberOfElements;
    namedVariable->typeSpec.structure.elements = (MmsVariableSpecification**) GLOBAL_CALLOC(numberOfElements,
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

    if (withResvTms) {
        element = (MmsVariableSpecification*) GLOBAL_CALLOC(1, sizeof(MmsVariableSpecification));
        element->name = StringUtils_copyString("ResvTms");
        element->type = MMS_UNSIGNED;
        element->typeSpec.integer = 16;
        namedVariable->typeSpec.structure.elements[5] = element;
    }

    return namedVariable;
}

static MmsVariableSpecification*
createFCNamedVariableSPWithSGCB(LogicalNode* logicalNode, bool withResvTms)
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

    namedVariable->typeSpec.structure.elements[0] = createSGCB(withResvTms);

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
    if (editSg)
        MmsValue_setUint8(editSg, 0U);

    MmsValue* resvTms = MmsValue_getElement(settingGroup->sgcbMmsValues, 5);
    if (resvTms)
        MmsValue_setUint16(resvTms, 0U);
}

static void
unselectAllSettingGroups(MmsMapping* self, MmsServerConnection serverCon)
{
    LinkedList settingGroupElement = LinkedList_getNext(self->settingGroups);

    while (settingGroupElement != NULL) {
        SettingGroup* settingGroup = (SettingGroup*) LinkedList_getData(settingGroupElement);

        if (settingGroup->editingClient == serverCon)
            unselectEditSettingGroup(settingGroup);

        settingGroupElement = LinkedList_getNext(settingGroupElement);
    }
}

#if (CONFIG_IEC61850_SERVICE_TRACKING == 1)

static void
copySGCBValuesToTrackingObject(MmsMapping* self, SettingGroupControlBlock* sgcb)
{
    if (self->sgcbTrk) {
        SgcbTrkInstance trkInst = self->sgcbTrk;

        if (trkInst->numOfSG)
            MmsValue_setUint8(trkInst->numOfSG->mmsValue, sgcb->numOfSGs);

        if (trkInst->actSG)
            MmsValue_setUint8(trkInst->actSG->mmsValue, sgcb->actSG);

        if (trkInst->editSG)
            MmsValue_setUint8(trkInst->editSG->mmsValue, sgcb->editSG);

        if (trkInst->cnfEdit)
            MmsValue_setBoolean(trkInst->cnfEdit->mmsValue, sgcb->cnfEdit);

        if (trkInst->lActTm) {
            SettingGroup* sg = getSettingGroupBySGCB(self, sgcb);
            MmsValue* lActTm = MmsValue_getElement(sg->sgcbMmsValues, 4);
            MmsValue_update(trkInst->lActTm->mmsValue, lActTm);
        }
    }
}

IEC61850_ServiceError
private_IedServer_convertMmsDataAccessErrorToServiceError(MmsDataAccessError mmsError)
{
    IEC61850_ServiceError errVal = IEC61850_SERVICE_ERROR_NO_ERROR;

    switch (mmsError) {
    case DATA_ACCESS_ERROR_SUCCESS:
        break;
    case DATA_ACCESS_ERROR_TEMPORARILY_UNAVAILABLE:
        errVal = IEC61850_SERVICE_ERROR_INSTANCE_LOCKED_BY_OTHER_CLIENT;
        break;
    case DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED:
        errVal = IEC61850_SERVICE_ERROR_ACCESS_VIOLATION;
        break;
    case DATA_ACCESS_ERROR_OBJECT_VALUE_INVALID:
        errVal = IEC61850_SERVICE_ERROR_PARAMETER_VALUE_INAPPROPRIATE;
        break;
    case DATA_ACCESS_ERROR_TYPE_INCONSISTENT:
    case DATA_ACCESS_ERROR_OBJECT_ATTRIBUTE_INCONSISTENT:
        errVal = IEC61850_SERVICE_ERROR_PARAMETER_VALUE_INCONSISTENT;
        break;
    case DATA_ACCESS_ERROR_OBJECT_NONE_EXISTENT:
        errVal = IEC61850_SERVICE_ERROR_INSTANCE_NOT_AVAILABLE;
        break;
    default:
        if (DEBUG_IED_SERVER)
            printf("IED_SERVER: Data access error %i not mapped!\n", mmsError);
        errVal = IEC61850_SERVICE_ERROR_FAILED_DUE_TO_SERVER_CONSTRAINT;
        break;
    }

    return errVal;
}

static void
updateGenericTrackingObjectValues(MmsMapping* self, SettingGroupControlBlock* sgcb, IEC61850_ServiceType serviceType, MmsDataAccessError errVal)
{
    ServiceTrkInstance trkInst = NULL;

    if (self->sgcbTrk) {
        trkInst = (ServiceTrkInstance) self->sgcbTrk;
    }

    if (trkInst) {
        if (trkInst->serviceType)
            MmsValue_setInt32(trkInst->serviceType->mmsValue, (int) serviceType);

        if (trkInst->t)
            MmsValue_setUtcTimeMs(trkInst->t->mmsValue, Hal_getTimeInMs());

        if (trkInst->errorCode)
            MmsValue_setInt32(trkInst->errorCode->mmsValue,
                    private_IedServer_convertMmsDataAccessErrorToServiceError(errVal));

        char objRef[129];

        /* create object reference */
        LogicalNode* ln = (LogicalNode*) sgcb->parent;
        LogicalDevice* ld = (LogicalDevice*) ln->parent;

        char* iedName = self->iedServer->mmsDevice->deviceName;

        snprintf(objRef, 129, "%s%s/%s.SGCB", iedName, ld->name, sgcb->parent->name);

        if (trkInst->objRef) {
            IedServer_updateVisibleStringAttributeValue(self->iedServer, trkInst->objRef, objRef);
        }
    }
}

#endif /* (CONFIG_IEC61850_SERVICE_TRACKING == 1) */

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
MmsMapping_initializeControlObjects(MmsMapping* self)
{
    LinkedList element = LinkedList_getNext(self->controlObjects);

    while (element) {
        ControlObject* controlObject = (ControlObject*) LinkedList_getData(element);

        ControlObject_initialize(controlObject);

        element = LinkedList_getNext(element);
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

            if (resvTms)
                MmsValue_setUint16(resvTms, 0U);

            settingGroup->sgcbMmsValues = values;
        }
        else if (DEBUG_IED_SERVER)
            printf("IED_SERVER: Values for SGCB do not exist\n");

        settingGroupElement = LinkedList_getNext(settingGroupElement);
    }
}

void
MmsMapping_useIntegratedGoosePublisher(MmsMapping* self, bool enable)
{
#if (CONFIG_INCLUDE_GOOSE_SUPPORT == 1)
    self->useIntegratedPublisher = enable;
#endif
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

#if (CONFIG_IEC61850_SERVICE_TRACKING == 1)
            copySGCBValuesToTrackingObject(self, sgcb);
            updateGenericTrackingObjectValues(self, sgcb, IEC61850_SERVICE_TYPE_SELECT_ACTIVE_SG, DATA_ACCESS_ERROR_SUCCESS);
#endif /* (CONFIG_IEC61850_SERVICE_TRACKING == 1) */

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

#if (CONFIG_IEC61850_SETTING_GROUPS == 1)

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

#endif /* (CONFIG_IEC61850_SETTING_GROUPS == 1) */

#if (CONFIG_IEC61850_SERVICE_TRACKING == 1)

static void
getCommonTrackingAttributes(ServiceTrkInstance svcTrkInst, DataObject* trkObj)
{
    ModelNode* modelNode = trkObj->firstChild;

    while (modelNode) {
        if (modelNode->modelType == DataAttributeModelType) {
            DataAttribute* da = (DataAttribute*) modelNode;

            if (!strcmp(da->name, "objRef")) {
                svcTrkInst->objRef = da;
            }
            else if (!strcmp(da->name, "serviceType")) {
                svcTrkInst->serviceType = da;
            }
            else if (!strcmp(da->name, "errorCode")) {
                svcTrkInst->errorCode = da;
            }
            else if (!strcmp(da->name, "originatorID")) {
                svcTrkInst->originatorID = da;
            }
            else if (!strcmp(da->name, "t")) {
                svcTrkInst->t = da;
            }
        }

        modelNode = modelNode->sibling;
    }

    /* check if all mandatory attributes are present */
    if (svcTrkInst->objRef && svcTrkInst->serviceType && svcTrkInst->errorCode && svcTrkInst->t) {

    }
    else {
        if (DEBUG_IED_SERVER)
            printf("IED_SERVER: required attribute missing in service tracking object %s\n",trkObj->name);
    }
}

static void
getBrcbTrackingAttributes(BrcbTrkInstance svcTrkInst, DataObject* trkObj)
{
    ModelNode* modelNode = trkObj->firstChild;

    while (modelNode) {
        if (modelNode->modelType == DataAttributeModelType) {
            DataAttribute* da = (DataAttribute*) modelNode;

            if (!strcmp(da->name, "rptID")) {
                svcTrkInst->rptID = da;
            }
            else if (!strcmp(da->name, "rptEna")) {
                svcTrkInst->rptEna = da;
            }
            else if (!strcmp(da->name, "datSet")) {
                svcTrkInst->datSet = da;
            }
            else if (!strcmp(da->name, "confRev")) {
                svcTrkInst->confRev = da;
            }
            else if (!strcmp(da->name, "optFlds")) {
                svcTrkInst->optFlds = da;
            }
            else if (!strcmp(da->name, "bufTm")) {
                svcTrkInst->bufTm = da;
            }
            else if (!strcmp(da->name, "sqNum")) {
                svcTrkInst->sqNum = da;
            }
            else if (!strcmp(da->name, "trgOps")) {
                svcTrkInst->trgOps = da;
            }
            else if (!strcmp(da->name, "intgPd")) {
                svcTrkInst->intgPd = da;
            }
            else if (!strcmp(da->name, "gi")) {
                svcTrkInst->gi = da;
            }
            else if (!strcmp(da->name, "purgeBuf")) {
                svcTrkInst->purgeBuf = da;
            }
            else if (!strcmp(da->name, "entryID")) {
                svcTrkInst->entryID = da;
            }
            else if (!strcmp(da->name, "timeOfEntry")) {
                svcTrkInst->timeOfEntry = da;
            }
            else if (!strcmp(da->name, "resvTms")) {
                svcTrkInst->resvTms = da;
            }
        }

        modelNode = modelNode->sibling;
    }

    /* check if all mandatory attributes are present */
    if (svcTrkInst->rptID && svcTrkInst->rptEna && svcTrkInst->datSet && svcTrkInst->confRev &&
        svcTrkInst->optFlds && svcTrkInst->bufTm && svcTrkInst->sqNum && svcTrkInst->trgOps &&
        svcTrkInst->intgPd && svcTrkInst->gi && svcTrkInst->purgeBuf && svcTrkInst->entryID &&
        svcTrkInst->timeOfEntry) {

    }
    else {
        if (DEBUG_IED_SERVER)
            printf("IED_SERVER: required attribute missing in brcbTrk service tracking object %s\n",trkObj->name);
    }
}

static void
getUrcbTrackingAttributes(UrcbTrkInstance svcTrkInst, DataObject* trkObj)
{
    ModelNode* modelNode = trkObj->firstChild;

    while (modelNode) {
        if (modelNode->modelType == DataAttributeModelType) {
            DataAttribute* da = (DataAttribute*) modelNode;

            if (!strcmp(da->name, "rptID")) {
                svcTrkInst->rptID = da;
            }
            else if (!strcmp(da->name, "rptEna")) {
                svcTrkInst->rptEna = da;
            }
            else if (!strcmp(da->name, "resv")) {
                svcTrkInst->resv = da;
            }
            else if (!strcmp(da->name, "datSet")) {
                svcTrkInst->datSet = da;
            }
            else if (!strcmp(da->name, "confRev")) {
                svcTrkInst->confRev = da;
            }
            else if (!strcmp(da->name, "optFlds")) {
                svcTrkInst->optFlds = da;
            }
            else if (!strcmp(da->name, "bufTm")) {
                svcTrkInst->bufTm = da;
            }
            else if (!strcmp(da->name, "sqNum")) {
                svcTrkInst->sqNum = da;
            }
            else if (!strcmp(da->name, "trgOps")) {
                svcTrkInst->trgOps = da;
            }
            else if (!strcmp(da->name, "intgPd")) {
                svcTrkInst->intgPd = da;
            }
            else if (!strcmp(da->name, "gi")) {
                svcTrkInst->gi = da;
            }
        }

        modelNode = modelNode->sibling;
    }

    /* check if all mandatory attributes are present */
    if (svcTrkInst->rptID && svcTrkInst->rptEna && svcTrkInst->datSet && svcTrkInst->confRev &&
        svcTrkInst->optFlds && svcTrkInst->bufTm && svcTrkInst->sqNum && svcTrkInst->trgOps &&
        svcTrkInst->intgPd && svcTrkInst->gi && svcTrkInst->resv) {

    }
    else {
        if (DEBUG_IED_SERVER)
            printf("IED_SERVER: required attribute missing in urcbTrk service tracking object %s\n",trkObj->name);
    }
}

static void
getGocbTrackingAttributes(GocbTrkInstance svcTrkInst, DataObject* trkObj)
{
    ModelNode* modelNode = trkObj->firstChild;

    while (modelNode) {
        if (modelNode->modelType == DataAttributeModelType) {
            DataAttribute* da = (DataAttribute*) modelNode;

            if (!strcmp(da->name, "goEna")) {
                svcTrkInst->goEna = da;
            }
            if (!strcmp(da->name, "goID")) {
                svcTrkInst->goID = da;
            }
            else if (!strcmp(da->name, "datSet")) {
                svcTrkInst->datSet = da;
            }
            else if (!strcmp(da->name, "confRev")) {
                svcTrkInst->confRev = da;
            }
            else if (!strcmp(da->name, "ndsCom")) {
                svcTrkInst->ndsCom = da;
            }
            else if (!strcmp(da->name, "dstAddress")) {
                svcTrkInst->dstAddress = da;
            }
        }

        modelNode = modelNode->sibling;
    }

    /* check if all mandatory attributes are present */
    if (svcTrkInst->goEna && svcTrkInst->goID && svcTrkInst->datSet && svcTrkInst->confRev &&
        svcTrkInst->ndsCom && svcTrkInst->dstAddress) {

    }
    else {
        if (DEBUG_IED_SERVER)
            printf("IED_SERVER: required attribute missing in gocbTrk service tracking object %s\n",trkObj->name);
    }
}

static void
getSgcbTrackingAttributes(SgcbTrkInstance svcTrkInst, DataObject* trkObj)
{
    ModelNode* modelNode = trkObj->firstChild;

    while (modelNode) {
        if (modelNode->modelType == DataAttributeModelType) {
            DataAttribute* da = (DataAttribute*) modelNode;

            if (!strcmp(da->name, "numOfSG")) {
                svcTrkInst->numOfSG = da;
            }
            if (!strcmp(da->name, "actSG")) {
                svcTrkInst->actSG = da;
            }
            else if (!strcmp(da->name, "editSG")) {
                svcTrkInst->editSG = da;
            }
            else if (!strcmp(da->name, "cnfEdit")) {
                svcTrkInst->cnfEdit = da;
            }
            else if (!strcmp(da->name, "lActTm")) {
                svcTrkInst->lActTm = da;
            }
        }

        modelNode = modelNode->sibling;
    }

    /* check if all mandatory attributes are present */
    if (svcTrkInst->numOfSG && svcTrkInst->actSG && svcTrkInst->editSG && svcTrkInst->cnfEdit &&
        svcTrkInst->lActTm) {

    }
    else {
        if (DEBUG_IED_SERVER)
            printf("IED_SERVER: required attribute missing in sgcbTrk service tracking object %s\n",trkObj->name);
    }
}

static void
getLocbTrackingAttributes(LocbTrkInstance svcTrkInst, DataObject* trkObj)
{
    ModelNode* modelNode = trkObj->firstChild;

    while (modelNode) {
        if (modelNode->modelType == DataAttributeModelType) {
            DataAttribute* da = (DataAttribute*) modelNode;

            if (!strcmp(da->name, "logEna")) {
                svcTrkInst->logEna = da;
            }
            if (!strcmp(da->name, "datSet")) {
                svcTrkInst->datSet = da;
            }
            else if (!strcmp(da->name, "trgOps")) {
                svcTrkInst->trgOps = da;
            }
            else if (!strcmp(da->name, "intgPd")) {
                svcTrkInst->intgPd = da;
            }
            else if (!strcmp(da->name, "logRef")) {
                svcTrkInst->logRef = da;
            }
        }

        modelNode = modelNode->sibling;
    }

}

static void
getControlTrackingAttributes(ControlTrkInstance svcTrkInst, DataObject* trkObj)
{
    ModelNode* modelNode = trkObj->firstChild;

    while (modelNode) {
        if (modelNode->modelType == DataAttributeModelType) {
            DataAttribute* da = (DataAttribute*) modelNode;

            if (!strcmp(da->name, "ctlVal")) {
                svcTrkInst->ctlVal = da;
            }
            else if (!strcmp(da->name, "operTm")) {
                svcTrkInst->operTm = da;
            }
            else if (!strcmp(da->name, "origin")) {
                svcTrkInst->origin = da;
            }
            else if (!strcmp(da->name, "ctlNum")) {
                svcTrkInst->ctlNum = da;
            }
            else if (!strcmp(da->name, "T")) {
                svcTrkInst->T = da;
            }
            else if (!strcmp(da->name, "Test")) {
                svcTrkInst->Test = da;
            }
            else if (!strcmp(da->name, "Check")) {
                svcTrkInst->Check = da;
            }
            else if (!strcmp(da->name, "respAddCause")) {
                svcTrkInst->respAddCause = da;
            }
        }

        modelNode = modelNode->sibling;
    }
}

static void
checkForServiceTrackingVariables(MmsMapping* self, LogicalNode* logicalNode)
{
    ModelNode* modelNode = logicalNode->firstChild;

    while (modelNode) {

        if (!strcmp(modelNode->name, "SpcTrk") || !strcmp(modelNode->name, "DpcTrk") || 
                !strcmp(modelNode->name, "IncTrk") || !strcmp(modelNode->name, "EncTrk1") || 
                !strcmp(modelNode->name, "ApcFTrk") || !strcmp(modelNode->name, "ApcIntTrk") ||
                !strcmp(modelNode->name, "BscTrk") || !strcmp(modelNode->name, "IscTrk") || 
                !strcmp(modelNode->name, "BacIntTrk"))
        {
            if (DEBUG_IED_SERVER)
                printf("IED_SERVER: %s data object found!\n", modelNode->name);

            DataObject* actTrk = (DataObject*) modelNode;
            ControlTrkInstance* actInstance = NULL;

            if (!strcmp(modelNode->name, "SpcTrk"))
                actInstance = &self->spcTrk;
            else if (!strcmp(modelNode->name, "DpcTrk"))
                actInstance = &self->dpcTrk;
            else if (!strcmp(modelNode->name, "IncTrk"))
                actInstance = &self->incTrk;
            else if (!strcmp(modelNode->name, "EncTrk1"))
                actInstance = &self->encTrk1;
            else if (!strcmp(modelNode->name, "ApcFTrk"))
                actInstance = &self->apcFTrk;
            else if (!strcmp(modelNode->name, "ApcIntTrk"))
                actInstance = &self->apcIntTrk;
            else if (!strcmp(modelNode->name, "BscTrk"))
                actInstance = &self->bscTrk;
            else if (!strcmp(modelNode->name, "IscTrk"))
                actInstance = &self->iscTrk;
            else if (!strcmp(modelNode->name, "BacTrk"))
                actInstance = &self->bacTrk;

            if (*actInstance != NULL) {
                if (DEBUG_IED_SERVER)
                    printf("IED_SERVER: ERROR: multiple %s instances found in server\n", modelNode->name);
            }
            else {
                *actInstance = (ControlTrkInstance) GLOBAL_CALLOC(1, sizeof(struct sControlTrkInstance));

                if (*actInstance != NULL) {
                    (*actInstance)->dobj = actTrk;

                    getCommonTrackingAttributes((ServiceTrkInstance) *actInstance, actTrk);
                    getControlTrackingAttributes(*actInstance, actTrk);
                }
            }

        }
        else if (!strcmp(modelNode->name, "BrcbTrk")) {
            if (DEBUG_IED_SERVER)
                printf("IED_SERVER: BrcbTrk data object found!\n");

            DataObject* brcbTrk = (DataObject*) modelNode;

            if (self->brcbTrk) {
                if (DEBUG_IED_SERVER)
                    printf("IED_SERVER: ERROR: multiple BrcbTrk instances found in server\n");
            }
            else {
                /* Setup BrcbTrk references */
                self->brcbTrk = (BrcbTrkInstance) GLOBAL_CALLOC(1, sizeof(struct sBrcbTrkInstance));

                if (self->brcbTrk) {
                    self->brcbTrk->dobj = brcbTrk;

                    getCommonTrackingAttributes((ServiceTrkInstance) self->brcbTrk, brcbTrk);
                    getBrcbTrackingAttributes(self->brcbTrk, brcbTrk);
                }

            }

        }
        else if (!strcmp(modelNode->name, "UrcbTrk")) {
            if (DEBUG_IED_SERVER)
                printf("IED_SERVER: UrcbTrk data object found!\n");

            DataObject* urcbTrk = (DataObject*) modelNode;

            if (self->urcbTrk) {
                if (DEBUG_IED_SERVER)
                    printf("IED_SERVER: ERROR: multiple UrcbTrk instances found in server\n");
            }
            else {
                /* Setup UrcbTrk references */
                self->urcbTrk = (UrcbTrkInstance) GLOBAL_CALLOC(1, sizeof(struct sUrcbTrkInstance));

                if (self->urcbTrk) {
                    self->urcbTrk->dobj = urcbTrk;

                    getCommonTrackingAttributes((ServiceTrkInstance) self->urcbTrk, urcbTrk);
                    getUrcbTrackingAttributes(self->urcbTrk, urcbTrk);
                }

            }
        }
        else if (!strcmp(modelNode->name, "GocbTrk")) {
            if (DEBUG_IED_SERVER)
                printf("IED_SERVER: GocbTrk data object found!\n");

            DataObject* gocbTrk = (DataObject*) modelNode;

            if (self->gocbTrk) {
                if (DEBUG_IED_SERVER)
                    printf("IED_SERVER: ERROR: multiple GocbTrk instances found in server\n");
            }
            else {
                /* Setup GocbTrk references */
                self->gocbTrk = (GocbTrkInstance) GLOBAL_CALLOC(1, sizeof(struct sGocbTrkInstance));

                if (self->gocbTrk) {
                    self->gocbTrk->dobj = gocbTrk;

                    getCommonTrackingAttributes((ServiceTrkInstance) self->gocbTrk, gocbTrk);
                    getGocbTrackingAttributes(self->gocbTrk, gocbTrk);
                }

            }
        }
        else if (!strcmp(modelNode->name, "SgcbTrk")) {
            if (DEBUG_IED_SERVER)
                printf("IED_SERVER: SgcbTrk data object found!\n");

            DataObject* sgcbTrk = (DataObject*) modelNode;

            if (self->sgcbTrk) {
                if (DEBUG_IED_SERVER)
                    printf("IED_SERVER: ERROR: multiple SgcbTrk instances found in server\n");
            }
            else {
                /* Setup SgcbTrk references */
                self->sgcbTrk = (SgcbTrkInstance) GLOBAL_CALLOC(1, sizeof(struct sSgcbTrkInstance));

                if (self->sgcbTrk) {
                    self->sgcbTrk->dobj = sgcbTrk;

                    getCommonTrackingAttributes((ServiceTrkInstance) self->sgcbTrk, sgcbTrk);
                    getSgcbTrackingAttributes(self->sgcbTrk, sgcbTrk);
                }
            }
        }
        else if (!strcmp(modelNode->name, "LocbTrk")) {
            if (DEBUG_IED_SERVER)
                printf("IED_SERVER: LocbTrk data object found!\n");

            DataObject* locbTrk = (DataObject*) modelNode;

            if (self->locbTrk) {
                if (DEBUG_IED_SERVER)
                    printf("IED_SERVER: ERROR: multiple LocbTrk instances found in server\n");
            }
            else {
                /* Setup LocbTrk references */
                self->locbTrk = (LocbTrkInstance) GLOBAL_CALLOC(1, sizeof(struct sLocbTrkInstance));

                if (self->locbTrk) {
                    self->locbTrk->dobj = locbTrk;

                    getCommonTrackingAttributes((ServiceTrkInstance) self->locbTrk, locbTrk);
                    getLocbTrackingAttributes(self->locbTrk, locbTrk);
                }
            }
        }
        else if (!strcmp(modelNode->name, "GenTrk")) {
            if (DEBUG_IED_SERVER)
                printf("IED_SERVER: GenTrk data object found!\n");

            DataObject* genTrk = (DataObject*) modelNode;

            if (self->genTrk) {
                if (DEBUG_IED_SERVER)
                    printf("IED_SERVER: ERROR: multiple GenTrk instances found in server\n");
            }
            else {
                /* Setup GenTrk references */
                self->genTrk = (ServiceTrkInstance) GLOBAL_CALLOC(1, sizeof(struct sServiceTrkInstance));

                if (self->genTrk) {
                    self->genTrk->dobj = genTrk;

                    getCommonTrackingAttributes((ServiceTrkInstance) self->genTrk, genTrk);
                }
            }
        }

        modelNode = modelNode->sibling;
    }

}

#endif /* (CONFIG_IEC61850_SERVICE_TRACKING == 1) */

static MmsVariableSpecification*
createNamedVariableFromLogicalNode(MmsMapping* self, MmsDomain* domain,
        LogicalNode* logicalNode)
{
    MmsVariableSpecification* namedVariable = (MmsVariableSpecification*)
            GLOBAL_MALLOC(sizeof(MmsVariableSpecification));

    namedVariable->name = StringUtils_copyString(logicalNode->name);

    namedVariable->type = MMS_STRUCTURE;

    int componentCount = determineLogicalNodeComponentCount(logicalNode);

#if (CONFIG_IEC61850_SETTING_GROUPS == 1)
    SettingGroupControlBlock* sgControlBlock = NULL;

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

        bool withResvTms = false;

        if (self->iedServer->enableEditSG) {
            withResvTms = self->iedServer->hasSGCBResvTms;
        }

        namedVariable->typeSpec.structure.elements[currentComponent] =
                createFCNamedVariableSPWithSGCB(logicalNode, withResvTms);
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

#if (CONFIG_MMS_SERVER_CONFIG_SERVICES_AT_RUNTIME == 1)
    if (self->iedServer->logServiceEnabled) {
#endif

        if (lcbCount > 0) {
            namedVariable->typeSpec.structure.elements[currentComponent] =
                    Logging_createLCBs(self, domain, logicalNode, lcbCount);

            currentComponent++;
        }

#if (CONFIG_MMS_SERVER_CONFIG_SERVICES_AT_RUNTIME == 1)
    }
#endif

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

#if (CONFIG_IEC61850_SERVICE_TRACKING == 1)

        if (DEBUG_IED_SERVER)
            printf("Service tracking elements detected\n");

        /* TODO set flag to enable service tracking */

        checkForServiceTrackingVariables(self, logicalNode);

#endif /* (CONFIG_IEC61850_SERVICE_TRACKING == 1) */

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
    int ldInstName = strlen(logicalDevice->name);

    if ((modelNameLength + ldInstName) > 64) {

        if (DEBUG_IED_SERVER)
            printf("IED_SERVER: Resulting domain name (IEDName+LDInst) too long (%i)\n", modelNameLength + ldInstName);

        goto exit_function;
    }

    strncpy(domainName, self->model->name, 64);
    strncat(domainName, logicalDevice->name, 64 - modelNameLength);
    domainName[64] = 0;

    domain = MmsDomain_create(domainName);

    if (domain == NULL)
        goto exit_function;

#if (CONFIG_IEC61850_LOG_SERVICE == 1)

#if (CONFIG_MMS_SERVER_CONFIG_SERVICES_AT_RUNTIME == 1)
    if (self->iedServer->logServiceEnabled) {
#endif
        /* add logs (journals) */
        Log* log = self->model->logs;

        while (log) {

            /* Check if log belongs to this logical device */
            if (log->parent->parent == (ModelNode*)logicalDevice) {
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
            }

            log = log->sibling;
        }

#if (CONFIG_MMS_SERVER_CONFIG_SERVICES_AT_RUNTIME == 1)
    }
#endif

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

static bool
createMmsDataModel(MmsMapping* self, int iedDeviceCount,
        MmsDevice* mmsDevice, IedModel* iedModel)
{
    mmsDevice->domains = (MmsDomain**) GLOBAL_CALLOC(1, (iedDeviceCount) * sizeof(MmsDomain*));
    mmsDevice->domainCount = iedDeviceCount;

    LogicalDevice* logicalDevice = iedModel->firstChild;

    int i = 0;
    while (logicalDevice != NULL) {
        mmsDevice->domains[i] = createMmsDomainFromIedDevice(self, logicalDevice);

        if (mmsDevice->domains[i] == NULL) {
        	mmsDevice->domainCount = i;
        	return false;
        }

        i++;
        logicalDevice = (LogicalDevice*) logicalDevice->sibling;
    }

    return true;
}

static void
createDataSets(MmsDevice* mmsDevice, IedModel* iedModel)
{
    DataSet* dataset = iedModel->dataSets;

    char domainName[65];

    int iedModelNameLength = strlen(iedModel->name);

    if (iedModelNameLength > 64) {

        if (DEBUG_IED_SERVER)
            printf("IED_SERVER: IED model name too long!\n");

        goto exit_function;
    }

    while (dataset != NULL) {
        strncpy(domainName, iedModel->name, 64);
        strncat(domainName, dataset->logicalDeviceName, 64 - iedModelNameLength);

        MmsDomain* dataSetDomain = MmsDevice_getDomain(mmsDevice, domainName);

        if (dataSetDomain == NULL) {

            if (DEBUG_IED_SERVER)
                printf("IED_SERVER: LD for dataset does not exist!\n");

            goto exit_function;
        }

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

    mmsDevice = MmsDevice_create(iedModel->name);

    if (iedModel->firstChild != NULL) {

        int iedDeviceCount = IedModel_getLogicalDeviceCount(iedModel);

        if (createMmsDataModel(self, iedDeviceCount, mmsDevice, iedModel)) {
            createDataSets(mmsDevice, iedModel);
        }
        else {
        	MmsDevice_destroy(mmsDevice);
        	mmsDevice = NULL;
        }
    }

    return mmsDevice;
}

MmsMapping*
MmsMapping_create(IedModel* model, IedServer iedServer)
{
    MmsMapping* self = (MmsMapping*) GLOBAL_CALLOC(1, sizeof(struct sMmsMapping));

    self->model = model;
    self->iedServer = iedServer;

#if (CONFIG_IEC61850_REPORT_SERVICE == 1)
    self->reportControls = LinkedList_create();
#endif

#if (CONFIG_IEC61850_LOG_SERVICE == 1)
    self->logControls = LinkedList_create();
    self->logInstances = LinkedList_create();
#endif

#if (CONFIG_INCLUDE_GOOSE_SUPPORT == 1)
    self->useIntegratedPublisher = true;

    self->gseControls = LinkedList_create();
    self->gooseInterfaceId = NULL;

    self->goCbHandler = NULL;
    self->goCbHandlerParameter = NULL;
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

    self->attributeAccessHandlers = LinkedList_create();

    /* create data model specification */
    self->mmsDevice = createMmsModelFromIedModel(self, model);

    if (self->mmsDevice == false) {
    	MmsMapping_destroy(self);
    	self = NULL;
    }

    return self;
}

void
MmsMapping_destroy(MmsMapping* self)
{

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    if (self->reportWorkerThread) {
        self->reportThreadRunning = false;
        Thread_destroy(self->reportWorkerThread);
    }
#endif

    if (self->mmsDevice)
        MmsDevice_destroy(self->mmsDevice);

#if (CONFIG_IEC61850_REPORT_SERVICE == 1)
    LinkedList_destroyDeep(self->reportControls, (LinkedListValueDeleteFunction) ReportControl_destroy);
#endif

#if (CONFIG_INCLUDE_GOOSE_SUPPORT == 1)
    LinkedList_destroyDeep(self->gseControls, (LinkedListValueDeleteFunction) MmsGooseControlBlock_destroy);
    if (self->gooseInterfaceId) GLOBAL_FREEMEM(self->gooseInterfaceId);
#endif

#if (CONFIG_IEC61850_SAMPLED_VALUES_SUPPORT == 1)
    LinkedList_destroyDeep(self->svControls, (LinkedListValueDeleteFunction) MmsSampledValueControlBlock_destroy);
#endif

#if (CONFIG_IEC61850_CONTROL_SERVICE == 1)
    LinkedList_destroyDeep(self->controlObjects, (LinkedListValueDeleteFunction) ControlObject_destroy);
#endif

#if (CONFIG_IEC61850_SETTING_GROUPS == 1)
    LinkedList_destroy(self->settingGroups);
    self->allowEditSg = true;
#endif

#if (CONFIG_IEC61850_LOG_SERVICE == 1)
    LinkedList_destroyDeep(self->logControls, (LinkedListValueDeleteFunction) LogControl_destroy);
    LinkedList_destroyDeep(self->logInstances, (LinkedListValueDeleteFunction) LogInstance_destroy);
#endif

#if (CONFIG_IEC61850_SERVICE_TRACKING == 1)
    if (self->brcbTrk) GLOBAL_FREEMEM(self->brcbTrk);
    if (self->urcbTrk) GLOBAL_FREEMEM(self->urcbTrk);
    if (self->gocbTrk) GLOBAL_FREEMEM(self->gocbTrk);
    if (self->spcTrk) GLOBAL_FREEMEM(self->spcTrk);
    if (self->dpcTrk) GLOBAL_FREEMEM(self->dpcTrk);
    if (self->incTrk) GLOBAL_FREEMEM(self->incTrk);
    if (self->encTrk1) GLOBAL_FREEMEM(self->encTrk1);
    if (self->apcFTrk) GLOBAL_FREEMEM(self->apcFTrk);
    if (self->apcIntTrk) GLOBAL_FREEMEM(self->apcIntTrk);
    if (self->bscTrk) GLOBAL_FREEMEM(self->bscTrk);
    if (self->iscTrk) GLOBAL_FREEMEM(self->iscTrk);
    if (self->bacTrk) GLOBAL_FREEMEM(self->bacTrk);
    if (self->sgcbTrk) GLOBAL_FREEMEM(self->sgcbTrk);
    if (self->genTrk) GLOBAL_FREEMEM(self->genTrk);
    if (self->locbTrk) GLOBAL_FREEMEM(self->locbTrk);
#endif

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
isFunctionalConstraint(const char* fcStr, char* separator)
{
    if (strncmp(separator + 1, fcStr, 2) == 0)
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

        if (MmsGooseControlBlock_getNdsCom(mmsGCB))
            return DATA_ACCESS_ERROR_OBJECT_VALUE_INVALID;

        if (MmsValue_getBoolean(value)) {
            if (MmsGooseControlBlock_enable(mmsGCB, self)) {
                if (self->goCbHandler)
                    self->goCbHandler(mmsGCB, IEC61850_GOCB_EVENT_ENABLE, self->goCbHandlerParameter);
            }
            else {
                return DATA_ACCESS_ERROR_OBJECT_VALUE_INVALID;
            }
        }
        else {
            MmsGooseControlBlock_disable(mmsGCB, self);

            if (self->goCbHandler)
                self->goCbHandler(mmsGCB, IEC61850_GOCB_EVENT_DISABLE, self->goCbHandlerParameter);
        }

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
            	/* allow to set non-existing data set? */

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

#if 0
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
#endif

static FunctionalConstraint
getFunctionalConstraintForWritableNode(char* separator)
{
    if (isFunctionalConstraint("CF", separator))
        return IEC61850_FC_CF;
    if (isFunctionalConstraint("DC", separator))
        return IEC61850_FC_DC;
    if (isFunctionalConstraint("SP", separator))
        return IEC61850_FC_SP;
    if (isFunctionalConstraint("SV", separator))
        return IEC61850_FC_SV;
    if (isFunctionalConstraint("SE", separator))
        return IEC61850_FC_SE;
    if (isFunctionalConstraint("BL", separator))
        return IEC61850_FC_BL;

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

    if (fc == IEC61850_FC_BL) {
        if (self->iedServer->writeAccessPolicies & ALLOW_WRITE_ACCESS_BL)
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
                MmsDataAccessError retVal = DATA_ACCESS_ERROR_SUCCESS;

                if (sg != NULL) {
                    uint32_t val = MmsValue_toUint32(value);

                    if ((val > 0) && (val <= sg->sgcb->numOfSGs)) {
                        if (val != sg->sgcb->actSG) {

                            if (sg->actSgChangedHandler) {

                                ClientConnection clientConnection = private_IedServer_getClientConnectionByHandle(self->iedServer, connection);

                                if (sg->actSgChangedHandler(sg->actSgChangedHandlerParameter, sg->sgcb,
                                        (uint8_t) val, clientConnection))
                                {
                                    sg->sgcb->actSG = val;

                                    MmsValue* actSg = MmsValue_getElement(sg->sgcbMmsValues, 1);
                                    MmsValue* lActTm = MmsValue_getElement(sg->sgcbMmsValues, 4);

                                    MmsValue_setUint8(actSg, sg->sgcb->actSG);
                                    MmsValue_setUtcTimeMs(lActTm, Hal_getTimeInMs());
                                }
                                else
                                    retVal = DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED;
                            }
                            else
                                retVal = DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED;
                        }
                    }
                    else
                        retVal = DATA_ACCESS_ERROR_OBJECT_VALUE_INVALID;
                }
#if (CONFIG_IEC61850_SERVICE_TRACKING == 1)
                copySGCBValuesToTrackingObject(self, sg->sgcb);
                updateGenericTrackingObjectValues(self, sg->sgcb, IEC61850_SERVICE_TYPE_SELECT_ACTIVE_SG, retVal);
#endif /* (CONFIG_IEC61850_SERVICE_TRACKING == 1) */
                return retVal;
            }
            else if (strcmp(nameId, "EditSG") == 0) {

                SettingGroup* sg = getSettingGroupByMmsDomain(self, domain);
                MmsDataAccessError retVal = DATA_ACCESS_ERROR_SUCCESS;

                if (self->iedServer->enableEditSG) {

                    if (sg != NULL) {
                        uint32_t val = MmsValue_toUint32(value);

                        if ((sg->editingClient != NULL) && ( sg->editingClient != connection)) {
                            /* Edit SG was set by other client */
                            retVal = DATA_ACCESS_ERROR_TEMPORARILY_UNAVAILABLE;
                        }
                        else {
                            if (val == 0) {
                                unselectEditSettingGroup(sg);
                                retVal = DATA_ACCESS_ERROR_SUCCESS;
                            }
                            else {

                                if ((val > 0) && (val <= sg->sgcb->numOfSGs)) {

                                    if (sg->editSgChangedHandler) {

                                        ClientConnection clientConnection = private_IedServer_getClientConnectionByHandle(self->iedServer, connection);

                                        if (sg->editSgChangedHandler(sg->editSgChangedHandlerParameter, sg->sgcb,
                                                (uint8_t) val, clientConnection))
                                        {
                                            sg->sgcb->editSG = val;
                                            sg->editingClient = connection;

                                            sg->reservationTimeout = Hal_getTimeInMs() + (sg->sgcb->resvTms * 1000);

                                            MmsValue* editSg = MmsValue_getElement(sg->sgcbMmsValues, 2);

                                            if (editSg)
                                                MmsValue_setUint8(editSg, sg->sgcb->editSG);

                                            MmsValue* resvTms = MmsValue_getElement(sg->sgcbMmsValues, 5);

                                            if (resvTms)
                                                MmsValue_setUint16(resvTms, sg->sgcb->resvTms);


                                            retVal = DATA_ACCESS_ERROR_SUCCESS;
                                        }
                                        else
                                            retVal = DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED;
                                    }
                                    else
                                        retVal = DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED;

                                }
                                else
                                    retVal = DATA_ACCESS_ERROR_OBJECT_VALUE_INVALID;

                            }
                        }
                    }
                    else {
                        retVal = DATA_ACCESS_ERROR_OBJECT_ACCESS_UNSUPPORTED;
                    }
                }
                else {
                    retVal = DATA_ACCESS_ERROR_OBJECT_ACCESS_UNSUPPORTED;
                }

#if (CONFIG_IEC61850_SERVICE_TRACKING == 1)
                copySGCBValuesToTrackingObject(self, sg->sgcb);
                updateGenericTrackingObjectValues(self, sg->sgcb, IEC61850_SERVICE_TYPE_SELECT_EDIT_SG, retVal);
#endif /* (CONFIG_IEC61850_SERVICE_TRACKING == 1) */
                return retVal;

            }
            else if (strcmp(nameId, "CnfEdit") == 0) {
                SettingGroup* sg = getSettingGroupByMmsDomain(self, domain);
                MmsDataAccessError retVal = DATA_ACCESS_ERROR_SUCCESS;

                if (self->iedServer->enableEditSG) {

                    if (sg != NULL) {
                        bool val = MmsValue_getBoolean(value);

                        if (val == true) {
                            if (sg->sgcb->editSG != 0) {
                                if (sg->editingClient == connection) {
                                    if (sg->editSgConfirmedHandler) {
                                        sg->editSgConfirmedHandler(sg->editSgConfirmedHandlerParameter, sg->sgcb,
                                                sg->sgcb->editSG);

                                        unselectEditSettingGroup(sg);

                                        retVal = DATA_ACCESS_ERROR_SUCCESS;
                                    }
                                    else
                                        retVal = DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED;
                                }
                                else
                                    retVal = DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED;
                            }
                            else
                                retVal = DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED;
                        }
                        else
                            retVal = DATA_ACCESS_ERROR_OBJECT_VALUE_INVALID;
                    }
                    else {
                        retVal = DATA_ACCESS_ERROR_OBJECT_ACCESS_UNSUPPORTED;
                    }
                }
                else {
                    retVal = DATA_ACCESS_ERROR_OBJECT_ACCESS_UNSUPPORTED;
                }

#if (CONFIG_IEC61850_SERVICE_TRACKING == 1)
                copySGCBValuesToTrackingObject(self, sg->sgcb);
                updateGenericTrackingObjectValues(self, sg->sgcb, IEC61850_SERVICE_TYPE_CONFIRM_EDIT_SG_VALUES, retVal);
#endif /* (CONFIG_IEC61850_SERVICE_TRACKING == 1) */
                return retVal;
            }
        }

        return DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED;
    }
#endif /* (CONFIG_IEC61850_SETTING_GROUPS == 1) */

    FunctionalConstraint fc = getFunctionalConstraintForWritableNode(separator);

#if (CONFIG_IEC61850_SETTING_GROUPS == 1)
     if (fc == IEC61850_FC_SE) {
         SettingGroup* sg = getSettingGroupByMmsDomain(self, domain);

         if (sg != NULL) {
             if (sg->editingClient != connection)
                 return DATA_ACCESS_ERROR_TEMPORARILY_UNAVAILABLE;
         }
         else
             return DATA_ACCESS_ERROR_OBJECT_NONE_EXISTENT;
    }
#endif /* (CONFIG_IEC61850_SETTING_GROUPS == 1) */

    /* writable data model elements - SP, SV, CF, DC, BL */
    if (fc != IEC61850_FC_NONE) {
        MmsValue* cachedValue;

        cachedValue = MmsServer_getValueFromCache(self->mmsServer, domain, variableId);

        if (cachedValue) {

            if (!MmsValue_equalTypes(cachedValue, value)) {
                return DATA_ACCESS_ERROR_OBJECT_VALUE_INVALID;
            }

            bool handlerFound = false;

            AccessPolicy nodeAccessPolicy = getAccessPolicyForFC(self, fc);

            if (DEBUG_IED_SERVER)
                printf("IED_SERVER: write to %s policy:%i\n", variableId, nodeAccessPolicy);

#if (CONFIG_IEC61850_SETTING_GROUPS == 1)
            if (isFunctionalConstraint("SE", separator)) {
                SettingGroup* sg = getSettingGroupByMmsDomain(self, domain);

                if (sg != NULL) {
                    if (sg->sgcb->editSG == 0)
                        return DATA_ACCESS_ERROR_TEMPORARILY_UNAVAILABLE;
                }
                else
                    return DATA_ACCESS_ERROR_OBJECT_NONE_EXISTENT;
            }
#endif

            bool updateValue = true;

            /* Call write access handlers */
            LinkedList writeHandlerListElement = LinkedList_getNext(self->attributeAccessHandlers);

            while (writeHandlerListElement != NULL) {
                AttributeAccessHandler* accessHandler = (AttributeAccessHandler*) writeHandlerListElement->data;
                DataAttribute* dataAttribute = accessHandler->attribute;

                if (dataAttribute->mmsValue == cachedValue) {

                    ClientConnection clientConnection = private_IedServer_getClientConnectionByHandle(self->iedServer,
                            connection);

                    MmsDataAccessError handlerResult =
                        accessHandler->handler(dataAttribute, value, clientConnection,
                                accessHandler->parameter);

                    if ((handlerResult == DATA_ACCESS_ERROR_SUCCESS) || (handlerResult == DATA_ACCESS_ERROR_SUCCESS_NO_UPDATE)) {
                        handlerFound = true;

                        if (handlerResult == DATA_ACCESS_ERROR_SUCCESS_NO_UPDATE)
                            updateValue = false;

                        break;
                    }
                    else
                        return handlerResult;
                }

                writeHandlerListElement = LinkedList_getNext(writeHandlerListElement);
            }

            /* DENY access if no handler is found and default policy is DENY */
            if (!handlerFound) {

                if (nodeAccessPolicy == ACCESS_POLICY_DENY)
                    return DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED;

            }

            if (updateValue) {
                DataAttribute* da = IedModel_lookupDataAttributeByMmsValue(self->model, cachedValue);

                if (da)
                    IedServer_updateAttributeValue(self->iedServer, da, value);
                else
                    return DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED;
            }

            return DATA_ACCESS_ERROR_SUCCESS;
        }
        else
        	return DATA_ACCESS_ERROR_OBJECT_NONE_EXISTENT;
    }

    return DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED;
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

void
MmsMapping_installReadAccessHandler(MmsMapping* self, ReadAccessHandler handler, void* parameter)
{
#if (CONFIG_IEC61850_SUPPORT_USER_READ_ACCESS_CONTROL == 1)
    self->readAccessHandler = handler;
    self->readAccessHandlerParameter = parameter;
#endif
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
mmsReadHandler(void* parameter, MmsDomain* domain, char* variableId, MmsServerConnection connection, bool isDirectAccess)
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
        retValue = Control_readAccessControlObject(self, domain, variableId, connection, isDirectAccess);
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

                        ReportControl_readAccess(rc, self, elementName);

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

    /* handle read access to other objects */

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

        ControlObject_unselect(controlObject, connection);

        controlObjectElement = LinkedList_getNext(controlObjectElement);
    }
}
#endif /* (CONFIG_IEC61850_CONTROL_SERVICE == 1) */

static void /* is called by MMS server layer and runs in the connection handling thread */
mmsConnectionHandler(void* parameter, MmsServerConnection connection, MmsServerEvent event)
{
    MmsMapping* self = (MmsMapping*) parameter;

    if (event == MMS_SERVER_CONNECTION_TICK) {
        Reporting_sendReports(self, connection);
    }
    else if (event == MMS_SERVER_CONNECTION_CLOSED) {
        ClientConnection clientConnection = private_IedServer_getClientConnectionByHandle(self->iedServer, connection);

        /* call user provided handler function */
        if (self->connectionIndicationHandler != NULL)
            self->connectionIndicationHandler(self->iedServer, clientConnection, false,
                    self->connectionIndicationHandlerParameter);

        private_IedServer_removeClientConnection(self->iedServer, clientConnection);

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
mmsReadAccessHandler (void* parameter, MmsDomain* domain, char* variableId, MmsServerConnection connection, bool isDirectAccess)
{
    MmsMapping* self = (MmsMapping*) parameter;

    (void)isDirectAccess;

    if (DEBUG_IED_SERVER)
        printf("IED_SERVER: mmsReadAccessHandler: Requested %s\n", variableId);

    char* separator = strchr(variableId, '$');

#if (CONFIG_IEC61850_SETTING_GROUPS == 1)

    if (separator) {
        if (isFunctionalConstraint("SE", separator)) {
            SettingGroup* sg = getSettingGroupByMmsDomain(self, domain);

            if (sg != NULL) {
                if (sg->sgcb->editSG == 0)
                    return DATA_ACCESS_ERROR_TEMPORARILY_UNAVAILABLE;
            }
            else
                return DATA_ACCESS_ERROR_OBJECT_NONE_EXISTENT;
        }
    }

#endif /* (CONFIG_IEC61850_SETTING_GROUPS == 1) */

#if (CONFIG_IEC61850_SUPPORT_USER_READ_ACCESS_CONTROL == 1)
    if (self->readAccessHandler != NULL)
    {
        char* ldName = MmsDomain_getName(domain);

        LogicalDevice* ld = IedModel_getDevice(self->model, ldName);

        if (ld != NULL) {

            char str[65];

            FunctionalConstraint fc;

            if (separator != NULL) {
                fc = FunctionalConstraint_fromString(separator + 1);

                if (fc == IEC61850_FC_BR || fc == IEC61850_FC_US ||
                    fc == IEC61850_FC_MS || fc == IEC61850_FC_RP ||
                    fc == IEC61850_FC_LG || fc == IEC61850_FC_GO)
                {
                    return DATA_ACCESS_ERROR_SUCCESS;
                }
                else {

                    StringUtils_createStringFromBufferInBuffer(str, (uint8_t*) variableId, separator - variableId);

                    LogicalNode* ln = LogicalDevice_getLogicalNode(ld, str);

                    if (ln != NULL) {


                        char* doStart = strchr(separator + 1, '$');


                        if (doStart != NULL) {

                            char* doEnd = strchr(doStart + 1, '$');

                            if (doEnd == NULL) {
                                StringUtils_copyStringToBuffer(doStart + 1, str);
                            }
                            else {
                                doEnd--;

                                StringUtils_createStringFromBufferInBuffer(str, (uint8_t*) (doStart + 1), doEnd - doStart);
                            }

                            if (fc == IEC61850_FC_SP) {
                                if (!strcmp(str, "SGCB"))
                                    return DATA_ACCESS_ERROR_SUCCESS;
                            }

                            ModelNode* dobj = ModelNode_getChild((ModelNode*) ln, str);

                            if (dobj != NULL) {

                                if (dobj->modelType == DataObjectModelType) {

                                    ClientConnection clientConnection = private_IedServer_getClientConnectionByHandle(self->iedServer,
                                                                                                                      connection);

                                    return self->readAccessHandler(ld, ln, (DataObject*) dobj, fc, clientConnection,
                                            self->readAccessHandlerParameter);
                                }
                            }
                        }
                    }
                }
            }
        }

        return DATA_ACCESS_ERROR_OBJECT_ACCESS_UNSUPPORTED;
    }
#endif /* CONFIG_IEC61850_SUPPORT_USER_READ_ACCESS_CONTROL */

    return DATA_ACCESS_ERROR_SUCCESS;
}

static MmsError
variableListChangedHandler (void* parameter, bool create, MmsVariableListType listType, MmsDomain* domain,
        char* listName, MmsServerConnection connection)
{
    MmsError allow = MMS_ERROR_NONE;

    (void)connection;

    /* TODO add log message */

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
            /* check if LN exists - otherwise reject request (to fulfill test case sDsN1c) */

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
                                if (strcmp(rc->dataSet->logicalDeviceName, MmsDomain_getName(domain) + strlen(self->model->name)) == 0) {
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
                                if (strcmp(lc->dataSet->logicalDeviceName, MmsDomain_getName(domain) + strlen(self->model->name)) == 0) {
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
DataSet_isMemberValueWithRef(DataSet* dataSet, MmsValue* value, char* dataRef, const char* iedName, int* index)
{
    int i = 0;

    DataSetEntry* dataSetEntry = dataSet->fcdas;

    while (dataSetEntry != NULL) {

        MmsValue *dataSetValue = dataSetEntry->value;

        if (dataSetValue != NULL) { /* prevent invalid data set members */
            if (isMemberValueRecursive(dataSetValue, value)) {
                if (dataRef != NULL)
                    sprintf(dataRef, "%s%s/%s", iedName, dataSetEntry->logicalDeviceName, dataSetEntry->variableName);

                if (index)
                    *index = i;

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

            int dsEntryIdx = 0;

            if (DataSet_isMemberValueWithRef(lc->dataSet, value, dataRef, self->model->name, &dsEntryIdx)) {

                if (lc->logInstance != NULL) {

                    if (lc->dataSet) {

                        DataSetEntry* dsEntry = lc->dataSet->fcdas;

                        while (dsEntry && (dsEntryIdx > 0)) {
                            dsEntry = dsEntry->sibling;

                            if (dsEntry == NULL)
                                break;

                            dsEntryIdx--;
                        }

                        if (dsEntry) {
                            MmsValue* dsValue = dsEntry->value;

                            LogInstance_logSingleData(lc->logInstance, dataRef, dsValue, reasonCode);
                        }

                    }

                }
                else {
                    if (DEBUG_IED_SERVER)
                        printf("IED_SERVER: No log instance available!\n");
                }
            }

        }
    }
}

#endif /* (CONFIG_IEC61850_LOG_SERVICE == 1) */

#if (CONFIG_IEC61850_REPORT_SERVICE == 1)
void
MmsMapping_triggerReportObservers(MmsMapping* self, MmsValue* value, int flag)
{
    LinkedList element = self->reportControls;

    bool modelLocked = self->isModelLocked;

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
                ReportControl_valueUpdated(rc, index, flag, modelLocked);
            }
        }
    }

    if (modelLocked == false) {
        Reporting_processReportEventsAfterUnlock(self);
    }
}

#endif /* (CONFIG_IEC61850_REPORT_SERVICE == 1) */

#if (CONFIG_INCLUDE_GOOSE_SUPPORT == 1)

void
MmsMapping_triggerGooseObservers(MmsMapping* self, MmsValue* value)
{
    LinkedList element = self->gseControls;

    bool modelLocked = self->isModelLocked;

    while ((element = LinkedList_getNext(element)) != NULL) {
        MmsGooseControlBlock gcb = (MmsGooseControlBlock) element->data;

        if (MmsGooseControlBlock_isEnabled(gcb)) {
            DataSet* dataSet = MmsGooseControlBlock_getDataSet(gcb);

            if (DataSet_isMemberValue(dataSet, value, NULL)) {
                MmsGooseControlBlock_setStateChangePending(gcb);

                if (modelLocked == false) {
                    MmsGooseControlBlock_publishNewState(gcb);
                }
            }
        }
    }
}

void
MmsMapping_enableGoosePublishing(MmsMapping* self)
{
    LinkedList element = LinkedList_getNext(self->gseControls);

    while (element) {
        MmsGooseControlBlock gcb = (MmsGooseControlBlock) LinkedList_getData(element);

        if (MmsGooseControlBlock_enable(gcb, self) == false) {
            if (DEBUG_IED_SERVER)
                printf("IED_SERVER: failed to enable GoCB %s\n", MmsGooseControlBlock_getName(gcb));
        }

        element = LinkedList_getNext(element);
    }
}

void
MmsMapping_useGooseVlanTag(MmsMapping* self, LogicalNode* ln, const char* gcbName, bool useVlanTag)
{
    LinkedList element = self->gseControls;

    while ((element = LinkedList_getNext(element)) != NULL) {
        MmsGooseControlBlock gcb = (MmsGooseControlBlock) element->data;

        if (ln == NULL) {
            MmsGooseControlBlock_useGooseVlanTag(gcb, useVlanTag);
        }
        else {
            if ((MmsGooseControlBlock_getLogicalNode(gcb) == ln) && !strcmp(MmsGooseControlBlock_getName(gcb), gcbName)) {
                MmsGooseControlBlock_useGooseVlanTag(gcb, useVlanTag);
            }
        }
    }
}

void
MmsMapping_setGooseInterfaceId(MmsMapping* self,  LogicalNode* ln, const char* gcbName, const char* interfaceId)
{
    LinkedList element = self->gseControls;

    while ((element = LinkedList_getNext(element)) != NULL) {
        MmsGooseControlBlock gcb = (MmsGooseControlBlock) element->data;

        if (ln == NULL) {
            MmsGooseControlBlock_setGooseInterfaceId(gcb, interfaceId);
        }
        else {
            if ((MmsGooseControlBlock_getLogicalNode(gcb) == ln) && !strcmp(MmsGooseControlBlock_getName(gcb), gcbName)) {
                MmsGooseControlBlock_setGooseInterfaceId(gcb, interfaceId);
            }
        }
    }
}

void
MmsMapping_disableGoosePublishing(MmsMapping* self)
{
    LinkedList element = self->gseControls;

    while ((element = LinkedList_getNext(element)) != NULL) {
        MmsGooseControlBlock gcb = (MmsGooseControlBlock) element->data;

        MmsGooseControlBlock_disable(gcb, self);
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

#if (CONFIG_INCLUDE_GOOSE_SUPPORT == 1)

static void
GOOSE_processGooseEvents(MmsMapping* self, uint64_t currentTimeInMs)
{
    LinkedList element = LinkedList_getNext(self->gseControls);

    while (element != NULL) {
        MmsGooseControlBlock mmsGCB = (MmsGooseControlBlock) element->data;

        if (MmsGooseControlBlock_isEnabled(mmsGCB)) {
            MmsGooseControlBlock_checkAndPublish(mmsGCB, currentTimeInMs, self);
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
    if (self->useIntegratedPublisher)
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
static void*
eventWorkerThread(MmsMapping* self)
{
    bool running = true;

    while (running) {

        processPeriodicTasks(self);

        Thread_sleep(1); /* hand-over control to other threads */

        running = self->reportThreadRunning;
    }

    if (DEBUG_IED_SERVER)
        printf("IED_SERVER: event worker thread finished!\n");

    return NULL;
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

        if (self->reportWorkerThread) {
            Thread_destroy(self->reportWorkerThread);
            self->reportWorkerThread = NULL;
        }
    }
}
#endif /* (CONFIG_MMS_THREADLESS_STACK != 1) */

DataSet*
MmsMapping_createDataSetByNamedVariableList(MmsMapping* self, MmsNamedVariableList variableList)
{
    DataSet* dataSet = (DataSet*) GLOBAL_MALLOC(sizeof(DataSet));

    if (variableList->domain != NULL)
        dataSet->logicalDeviceName = MmsDomain_getName(variableList->domain) + strlen(self->model->name);
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
        dataSetEntry->value = NULL;

        if (lastDataSetEntry == NULL)
            dataSet->fcdas =dataSetEntry;
        else
            lastDataSetEntry->sibling = dataSetEntry;

        MmsVariableSpecification* dataSetEntryVarSpec = NULL;

        MmsValue* dataSetEntryValue = MmsServer_getValueFromCacheEx(self->mmsServer, listEntry->domain, listEntry->variableName, &dataSetEntryVarSpec);

        if (dataSetEntryValue) {
            if (dataSetEntry->index != -1) {
                if (dataSetEntryVarSpec->type == MMS_ARRAY) {
                    MmsValue* elementValue = MmsValue_getElement(dataSetEntryValue, dataSetEntry->index);

                    if (elementValue) {

                        if (dataSetEntry->componentName) {
                            MmsVariableSpecification* elementType = dataSetEntryVarSpec->typeSpec.array.elementTypeSpec;

                            MmsValue* subElementValue = MmsVariableSpecification_getChildValue(elementType, elementValue, dataSetEntry->componentName);

                            if (subElementValue) {
                                dataSetEntry->value = subElementValue;
                            }
                            else {
                                if (DEBUG_IED_SERVER)
                                    printf("IED_SERVER: ERROR - component %s of array element not found\n", dataSetEntry->componentName);
                            }

                        }
                        else {
                            dataSetEntry->value = elementValue;
                        }
                    }
                    else {
                        if (DEBUG_IED_SERVER)
                            printf("IED_SERVER: ERROR - array element %i not found\n", dataSetEntry->index);
                    }
                }
                else {
                    if (DEBUG_IED_SERVER)
                        printf("IED_SERVER: ERROR - variable %s/%s is not an array\n", dataSetEntry->logicalDeviceName, dataSetEntry->variableName);
                }
            }
            else {
                dataSetEntry->value = dataSetEntryValue;
            }
        }

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
