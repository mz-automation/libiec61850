/*
 *  model.c
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

#include "iec61850_model.h"

#include "stack_config.h"
#include "libiec61850_platform_includes.h"

static void
setAttributeValuesToNull(ModelNode* node)
{
    if (node->modelType == DataAttributeModelType) {
        DataAttribute* da = (DataAttribute*) node;

        da->mmsValue = NULL;
    }

    ModelNode* child = node->firstChild;

    while (child != NULL) {
        setAttributeValuesToNull(child);
        child = child->sibling;
    }
}

void
IedModel_setIedName(IedModel* self, const char* name)
{
    self->name = (char*) name;
}

void
IedModel_setAttributeValuesToNull(IedModel* iedModel)
{
    LogicalDevice* ld = iedModel->firstChild;

    while (ld != NULL) {

        LogicalNode* ln = (LogicalNode*) ld->firstChild;

        while (ln != NULL) {
            ModelNode* node = ln->firstChild;

            while (node != NULL) {
                setAttributeValuesToNull(node);
                node = node->sibling;
            }

            ln = (LogicalNode*) ln->sibling;
        }

        ld = (LogicalDevice*) ld->sibling;
    }
}



int
IedModel_getLogicalDeviceCount(IedModel* self)
{
	if (self->firstChild == NULL)
		return 0;

	LogicalDevice* logicalDevice = self->firstChild;

	int ldCount = 1;

	while (logicalDevice->sibling != NULL) {
		logicalDevice = (LogicalDevice*) logicalDevice->sibling;
		ldCount++;
	}

	return ldCount;
}

DataSet*
IedModel_lookupDataSet(IedModel* self, const char* dataSetReference  /* e.g. ied1Inverter/LLN0$dataset1 */)
{
	DataSet* dataSet = self->dataSets;

	const char* separator = strchr(dataSetReference, '/');

	if (separator == NULL)
		return NULL;

	int ldNameLen = separator - dataSetReference;

	char domainName[65];

	int modelNameLen = strlen(self->name);

	memcpy(domainName, self->name, modelNameLen);

	while (dataSet != NULL) {

	    domainName[modelNameLen] = 0;

	    strncat(domainName, dataSet->logicalDeviceName, 64);

		if (strncmp(domainName, dataSetReference, ldNameLen) == 0) {
			if (strcmp(dataSet->name, separator + 1) == 0) {
				return dataSet;
			}
		}

		dataSet = dataSet->sibling;
	}

	return NULL;
}

LogicalDevice*
IedModel_getDevice(IedModel* self, const char* deviceName)
{
    LogicalDevice* device = self->firstChild;

    while (device != NULL) {

        char domainName[65];

        strncpy(domainName, self->name, 64);
        strncat(domainName, device->name, 64);

        if (strcmp(domainName, deviceName) == 0)
            return device;

        device = (LogicalDevice*) device->sibling;
    }

    return NULL;
}

LogicalDevice*
IedModel_getDeviceByInst(IedModel* self, const char* ldInst)
{
    LogicalDevice* device = self->firstChild;

    while (device != NULL) {

      if (strcmp(device->name, ldInst) == 0)
          return device;

      device = (LogicalDevice*) device->sibling;
    }

    return NULL;
}


LogicalDevice*
IedModel_getDeviceByIndex(IedModel* self, int index)
{
    LogicalDevice* logicalDevice = self->firstChild;

    int currentIndex = 0;

    while (logicalDevice) {

        if (currentIndex == index)
            return logicalDevice;

        currentIndex++;

        logicalDevice = (LogicalDevice*) logicalDevice->sibling;
    }

    return NULL;
}

static DataAttribute*
ModelNode_getDataAttributeByMmsValue(ModelNode* self, MmsValue* value)
{
    ModelNode* node = self->firstChild;

    while (node != NULL) {
        if (node->modelType == DataAttributeModelType) {
            DataAttribute* da = (DataAttribute*) node;

            if (da->mmsValue == value)
                return da;
        }

        DataAttribute* da = ModelNode_getDataAttributeByMmsValue(node, value);

        if (da != NULL)
            return da;

        node = node->sibling;
    }

    return NULL;
}

DataAttribute*
IedModel_lookupDataAttributeByMmsValue(IedModel* model, MmsValue* value)
{
    LogicalDevice* ld = model->firstChild;

    while (ld != NULL) {

        DataAttribute* da =
                ModelNode_getDataAttributeByMmsValue((ModelNode*) ld, value);

        if (da != NULL)
            return da;


        ld = (LogicalDevice*) ld->sibling;
    }

    return NULL;
}

static ModelNode*
getChildWithShortAddress(ModelNode* node, uint32_t sAddr)
{
    ModelNode* child;

    child = node->firstChild;

    while (child != NULL) {
        if (child->modelType == DataAttributeModelType) {
            DataAttribute* da = (DataAttribute*) child;

            if (da->sAddr == sAddr)
                return child;
        }

        ModelNode* childChild = getChildWithShortAddress(child, sAddr);

        if (childChild != NULL)
            return childChild;

        child = child->sibling;
    }

    return NULL;
}

ModelNode*
IedModel_getModelNodeByShortAddress(IedModel* model, uint32_t sAddr)
{
    ModelNode* node = NULL;

    LogicalDevice* ld = (LogicalDevice*) model->firstChild;

    while (ld != NULL) {

        LogicalNode* ln = (LogicalNode*) ld->firstChild;

        while (ln != NULL) {

            ModelNode* doNode = ln->firstChild;

            while (doNode != NULL) {
                ModelNode* matchingNode = getChildWithShortAddress(doNode, sAddr);

                if (matchingNode != NULL)
                    return matchingNode;

                doNode = doNode->sibling;
            }

            ln = (LogicalNode*) ln->sibling;
        }

        ld = (LogicalDevice*) ld->sibling;
    }

    return node;
}

ModelNode*
IedModel_getModelNodeByObjectReference(IedModel* model, const char* objectReference)
{
    assert(strlen(objectReference) < 129);

    char objRef[130];

    strncpy(objRef, objectReference, 129);
    objRef[129] = 0;

    char* separator = strchr(objRef, '/');

    if (separator != NULL)
        *separator = 0;

    LogicalDevice* ld = IedModel_getDevice(model, objRef);

    if (ld == NULL) return NULL;

    if ((separator == NULL) || (*(separator + 1) == 0))
        return (ModelNode*) ld;

    return ModelNode_getChild((ModelNode*) ld, separator + 1);
}

#if (CONFIG_IEC61850_SAMPLED_VALUES_SUPPORT == 1)

SVControlBlock*
IedModel_getSVControlBlock(IedModel* self, LogicalNode* parentLN, const char* svcbName)
{
    SVControlBlock* retVal = NULL;

    SVControlBlock* svCb = self->svCBs;

    while (svCb != NULL) {
        if ((svCb->parent == parentLN) && (strcmp(svCb->name, svcbName) == 0)) {
            retVal = svCb;
            break;
        }


        svCb = svCb->sibling;
    }

    return retVal;
}

#endif /* (CONFIG_IEC61850_SAMPLED_VALUES_SUPPORT == 1) */

ModelNode*
IedModel_getModelNodeByShortObjectReference(IedModel* model, const char* objectReference)
{
    assert((strlen(model->name) + strlen(objectReference)) < 130);

    char objRef[130];

    strncpy(objRef, objectReference, 129);
    objRef[129] = 0;

    char* separator = strchr(objRef, '/');

    if (separator != NULL)
        *separator = 0;

    char ldName[65];
    strcpy(ldName, model->name);
    strcat(ldName, objRef);

    LogicalDevice* ld = IedModel_getDevice(model, ldName);

    if (ld == NULL) return NULL;

    if ((separator == NULL) || (*(separator + 1) == 0))
        return (ModelNode*) ld;

    return ModelNode_getChild((ModelNode*) ld, separator + 1);
}


bool
DataObject_hasFCData(DataObject* dataObject, FunctionalConstraint fc)
{
	ModelNode* modelNode = dataObject->firstChild;

	while (modelNode != NULL) {

		if (modelNode->modelType == DataAttributeModelType) {
			DataAttribute* dataAttribute = (DataAttribute*) modelNode;

			if (dataAttribute->fc == fc)
				return true;
		}
		else if (modelNode->modelType == DataObjectModelType) {

			if (DataObject_hasFCData((DataObject*) modelNode, fc))
				return true;
		}

		modelNode = modelNode->sibling;
	}

	return false;
}

bool
LogicalNode_hasFCData(LogicalNode* node, FunctionalConstraint fc)
{
	DataObject* dataObject = (DataObject*) node->firstChild;

	while (dataObject != NULL) {
		if (DataObject_hasFCData(dataObject, fc))
			return true;

		dataObject = (DataObject*) dataObject->sibling;
	}

	return false;
}

DataSet*
LogicalNode_getDataSet(LogicalNode* self, const char* dataSetName)
{
    assert(self->modelType == LogicalNodeModelType);
	assert(dataSetName != NULL);

	char dsName[66];

	LogicalDevice* ld = (LogicalDevice*) self->parent;

	if (strlen(dataSetName) > 32) {

		if (DEBUG_IED_SERVER) {
			printf("IED_SERVER: LogicalNode_getDataSet - data set name %s too long!\n", dataSetName);
		}

		goto exit_error;
	}

	StringUtils_createStringInBuffer(dsName, 3, self->name, "$", dataSetName);

	IedModel* iedModel = (IedModel*) ld->parent;

	DataSet* ds = iedModel->dataSets;

	while (ds != NULL) {
		if (strcmp(ds->logicalDeviceName, ld->name) == 0) {
			if (strcmp(ds->name, dsName) == 0) {
				return ds;
			}
		}

		ds = ds->sibling;
	}


exit_error:
	return NULL;
}

int
LogicalDevice_getLogicalNodeCount(LogicalDevice* logicalDevice)
{
	int lnCount = 0;

	LogicalNode* logicalNode = (LogicalNode*) logicalDevice->firstChild;

	while (logicalNode != NULL) {
		logicalNode = (LogicalNode*) logicalNode->sibling;
		lnCount++;
	}

	return lnCount;
}

ModelNode*
LogicalDevice_getChildByMmsVariableName(LogicalDevice* logicalDevice, const char* mmsVariableName)
{


	char fcString[3];
	char nameRef[65];

	const char* separator = strchr(mmsVariableName,'$');

	if (separator == NULL)
		return NULL;

	if (strlen(separator) > 4) {
		fcString[0] = separator[1];
		fcString[1] = separator[2];
		fcString[2] = 0;

		const char* strpos = mmsVariableName;

		int targetPos = 0;

		while (strpos < separator) {
			nameRef[targetPos++] = strpos[0];
			strpos++;
		}

		nameRef[targetPos++] = '.';

		strpos = separator + 4;

		while (strpos[0] != 0) {
			nameRef[targetPos++] = strpos[0];
			strpos++;
		}

		nameRef[targetPos++] = 0;

		StringUtils_replace(nameRef, '$', '.');

		FunctionalConstraint fc = FunctionalConstraint_fromString(fcString);

		return ModelNode_getChildWithFc((ModelNode*) logicalDevice, nameRef, fc);
	}

	return NULL;
}

static int
createObjectReference(ModelNode* node, char* objectReference, bool withoutIedName)
{
    int bufPos;

    if (node->modelType != LogicalNodeModelType) {
        bufPos = createObjectReference(node->parent, objectReference, withoutIedName);

        objectReference[bufPos++] = '.';
    }
    else {
        LogicalNode* lNode = (LogicalNode*) node;

        LogicalDevice* lDevice = (LogicalDevice*) lNode->parent;

        IedModel* iedModel = (IedModel*) lDevice->parent;

        bufPos = 0;

        int nameLength = 0;

        if (withoutIedName) {
            nameLength = strlen(lDevice->name);
            strncpy(objectReference, lDevice->name, 64);
        }
        else {
            nameLength = strlen (iedModel->name) + strlen(lDevice->name);

            strncpy(objectReference, iedModel->name, 64);
            strncat(objectReference, lDevice->name, 64);
        }

        bufPos += nameLength;

        objectReference[bufPos++] = '/';
    }

    /* append own name */
    int nameLength = strlen(node->name);

    int i;
    for (i = 0; i < nameLength; i++) {
        objectReference[bufPos++] = node->name[i];
    }

    return bufPos;
}

char*
ModelNode_getObjectReference(ModelNode* node, char* objectReference)
{
    if (objectReference == NULL)
        objectReference = (char*) GLOBAL_MALLOC(130);

    int bufPos = createObjectReference(node, objectReference, false);

    objectReference[bufPos] = 0;

    return objectReference;
}

char*
ModelNode_getObjectReferenceEx(ModelNode* node, char* objectReference, bool withoutIedName)
{
    if (objectReference == NULL)
        objectReference = (char*) GLOBAL_MALLOC(130);

    int bufPos = createObjectReference(node, objectReference, withoutIedName);

    objectReference[bufPos] = 0;

    return objectReference;
}

int
ModelNode_getChildCount(ModelNode* modelNode) {
	int childCount = 0;

	ModelNode* child = modelNode->firstChild;

	while (child != NULL) {
		childCount++;
		child = child->sibling;
	}

	return childCount;
}


ModelNode*
ModelNode_getChild(ModelNode* self, const char* name)
{
   /* check for separator */
   const char* separator = strchr(name, '.');

   int nameElementLength = 0;

   if (separator != NULL)
       nameElementLength = (separator - name);
   else
       nameElementLength = strlen(name);

   ModelNode* nextNode = self->firstChild;

   ModelNode* matchingNode = NULL;

   while (nextNode != NULL) {
       int nodeNameLen = strlen(nextNode->name);

       if (nodeNameLen == nameElementLength) {
           if (memcmp(nextNode->name, name, nodeNameLen) == 0) {
               matchingNode = nextNode;
               break;
           }
       }

       nextNode = nextNode->sibling;
   }

   if ((separator != NULL) && (matchingNode != NULL)) {
       return ModelNode_getChild(matchingNode, separator + 1);
   }
   else
       return matchingNode;
}

ModelNode*
ModelNode_getChildWithFc(ModelNode* self, const char* name, FunctionalConstraint fc)
{
   /* check for separator */
   const char* separator = strchr(name, '.');

   int nameElementLength = 0;

   if (separator != NULL)
       nameElementLength = (separator - name);
   else
       nameElementLength = strlen(name);

   ModelNode* nextNode = self->firstChild;

   ModelNode* matchingNode = NULL;

   while (nextNode != NULL) {
       int nodeNameLen = strlen(nextNode->name);

       if (nodeNameLen == nameElementLength) {
           if (memcmp(nextNode->name, name, nodeNameLen) == 0) {

               if (separator == NULL) {
                   if (nextNode->modelType == DataAttributeModelType) {
                       DataAttribute* da = (DataAttribute*) nextNode;

                       if (da->fc == fc) {
                           matchingNode = nextNode;
                           break;
                       }
                   }
               }
               else {

                   if (nextNode->modelType == DataAttributeModelType) {
                        DataAttribute* da = (DataAttribute*) nextNode;

                        if (da->fc == fc) {
                          matchingNode = nextNode;
                          break;
                        }
                   }
                   else {
                       matchingNode = nextNode;
                       break;
                   }

               }
           }
       }

       nextNode = nextNode->sibling;
   }

   if ((separator != NULL) && (matchingNode != NULL)) {
       return ModelNode_getChildWithFc(matchingNode, separator + 1, fc);
   }
   else
       return matchingNode;
}

ModelNodeType
ModelNode_getType(ModelNode* self)
{
    return self->modelType;
}

const char*
ModelNode_getName(ModelNode* self)
{
    return self->name;
}

ModelNode*
ModelNode_getParent(ModelNode* self)
{
    return self->parent;
}

LinkedList
ModelNode_getChildren(ModelNode* self)
{
    LinkedList childNodes = NULL;

    if (self->firstChild)
        childNodes = LinkedList_create();

    ModelNode* childNode = self->firstChild;

    while (childNode) {
        LinkedList_add(childNodes, childNode);

        childNode = childNode->sibling;
    }

    return childNodes;
}

LogicalNode*
LogicalDevice_getLogicalNode(LogicalDevice* self, const char* nodeName)
{
    return (LogicalNode*) ModelNode_getChild((ModelNode*) self, nodeName);
}

SettingGroupControlBlock*
LogicalDevice_getSettingGroupControlBlock(LogicalDevice* self)
{
    IedModel* model = (IedModel*) self->parent;

    if (model == NULL)
        return NULL;

    LogicalNode* ln = LogicalDevice_getLogicalNode(self, "LLN0");

    if (ln == NULL) {
        if (DEBUG_IED_SERVER)
            printf("IED_SERVER: logical node LLN0 not found!\n");

        return NULL;
    }

    SettingGroupControlBlock* sgcb = model->sgcbs;

    while (sgcb != NULL) {
        if (sgcb->parent == ln)
            return sgcb;

        sgcb = sgcb->sibling;
    }

    return NULL;
}
