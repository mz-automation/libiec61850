/*
 *  dynamic_model.c
 *
 *  Copyright 2014 Michael Zillgith
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


#include "iec61850_server.h"
#include "string_utilities.h"

static void
iedModel_emptyVariableInitializer(void)
{
    return;
}

IedModel*
IedModel_create(const char* name/*, MemoryAllocator allocator*/)
{
    IedModel* self = (IedModel*) calloc(1, sizeof(IedModel));

    if (name)
        self->name = copyString(name);
    else
        self->name = NULL;

    self->rcbs = NULL;

    self->dataSets = NULL;

    self->gseCBs = NULL;

    self->initializer = iedModel_emptyVariableInitializer;

    return self;
}

static void
IedModel_addDataSet(IedModel* self, DataSet* dataSet)
{
    if (self->dataSets == NULL)
        self->dataSets = dataSet;
    else {
        DataSet* lastDataSet = self->dataSets;

        while (lastDataSet != NULL) {
            if (lastDataSet->sibling == NULL) {
                lastDataSet->sibling = dataSet;
                break;
            }

            lastDataSet = lastDataSet->sibling;
        }
    }
}

static void
IedModel_addLogicalDevice(IedModel* self, LogicalDevice* lDevice)
{
    if (self->firstChild == NULL)
        self->firstChild = lDevice;
    else {
        LogicalDevice* sibling = self->firstChild;

        while (sibling->sibling != NULL)
            sibling = (LogicalDevice*) sibling->sibling;

        sibling->sibling = (ModelNode*) lDevice;
    }
}

static void
IedModel_addReportControlBlock(IedModel* self, ReportControlBlock* rcb)
{
    if (self->rcbs == NULL)
        self->rcbs = rcb;
    else {
        ReportControlBlock* lastRcb = self->rcbs;

        while (lastRcb->sibling != NULL)
            lastRcb = lastRcb->sibling;

        lastRcb->sibling = rcb;
    }
}

static void
IedModel_addGSEControlBlock(IedModel* self, GSEControlBlock* gcb)
{
    if (self->gseCBs == NULL)
        self->gseCBs = gcb;
    else {
        GSEControlBlock* lastGcb = self->gseCBs;

        while (lastGcb->sibling != NULL)
            lastGcb = lastGcb->sibling;

        lastGcb->sibling = gcb;
    }
}

LogicalDevice*
LogicalDevice_create(const char* name, IedModel* parent)
{
    LogicalDevice* self = (LogicalDevice*) calloc(1, sizeof(LogicalDevice));

    self->name = copyString(name);
    self->modelType = LogicalDeviceModelType;
    self->parent = (ModelNode*) parent;
    self->sibling = NULL;

    IedModel_addLogicalDevice(parent, self);

    return self;
}

static LogicalNode*
LogicalDevice_getLastLogicalNode(LogicalDevice* self)
{
    LogicalNode* lastNode = (LogicalNode*) self->firstChild;

    LogicalNode* nextNode = lastNode;

    while (nextNode != NULL) {
        lastNode = nextNode;
        nextNode = (LogicalNode*) nextNode->sibling;
    }

    return lastNode;
}

static void
LogicalDevice_addLogicalNode(LogicalDevice* self, LogicalNode* lNode)
{
    if (self->firstChild == NULL)
        self->firstChild = (ModelNode*) lNode;
    else {
        LogicalNode* lastNode = LogicalDevice_getLastLogicalNode(self);

        lastNode->sibling = (ModelNode*) lNode;
    }
}

LogicalNode*
LogicalNode_create(const char* name, LogicalDevice* parent)
{
    LogicalNode* self = (LogicalNode*) malloc(sizeof(LogicalNode));

    self->name = copyString(name);
    self->parent = (ModelNode*) parent;
    self->modelType = LogicalNodeModelType;
    self->firstChild = NULL;
    self->sibling = NULL;

    LogicalDevice_addLogicalNode(parent, self);

    return self;
}

static DataObject*
LogicalNode_getLastDataObject(LogicalNode* self)
{
    DataObject* lastNode = (DataObject*) self->firstChild;

    DataObject* nextNode = lastNode;

    while (nextNode != NULL) {
        lastNode = nextNode;
        nextNode = (DataObject*) nextNode->sibling;
    }

    return lastNode;

}

static void
LogicalNode_addDataObject(LogicalNode* self, DataObject* dataObject)
{
    if (self->firstChild == NULL)
        self->firstChild = (ModelNode*) dataObject;
    else {
        DataObject* lastDataObject = LogicalNode_getLastDataObject(self);

        lastDataObject->sibling = (ModelNode*) dataObject;
    }
}

static void
LogicalNode_addReportControlBlock(LogicalNode* self, ReportControlBlock* rcb)
{
    IedModel* model = (IedModel*) self->parent->parent;

    IedModel_addReportControlBlock(model, rcb);
}

ReportControlBlock*
ReportControlBlock_create(const char* name, LogicalNode* parent, char* rptId, bool isBuffered, char*
        dataSetName, uint32_t confRef, uint8_t trgOps, uint8_t options, uint32_t bufTm, uint32_t intgPd)
{
    ReportControlBlock* self = (ReportControlBlock*) malloc(sizeof(ReportControlBlock));

    self->name = copyString(name);
    self->parent = parent;

    if (rptId)
        self->rptId = copyString(rptId);
    else
        self->rptId = NULL;

    self->buffered = isBuffered;

    if (dataSetName)
        self->dataSetName = copyString(dataSetName);
    else
        self->dataSetName = NULL;

    self->confRef = confRef;
    self->trgOps = trgOps;
    self->options = options;
    self->bufferTime = bufTm;
    self->intPeriod = intgPd;
    self->sibling = NULL;

    LogicalNode_addReportControlBlock(parent, self);

    return self;
}

static void
LogicalNode_addGSEControlBlock(LogicalNode* self, GSEControlBlock* gcb)
{
    IedModel* model = (IedModel*) self->parent->parent;

    IedModel_addGSEControlBlock(model, gcb);
}

GSEControlBlock*
GSEControlBlock_create(const char* name, LogicalNode* parent, char* appId, char* dataSet, uint32_t confRef, bool fixedOffs)
{
    GSEControlBlock* self = (GSEControlBlock*) malloc(sizeof(GSEControlBlock));

    self->name = copyString(name);
    self->parent = parent;

    if (appId)
        self->appId = copyString(appId);
    else
        self->appId = NULL;

    if (dataSet)
        self->dataSetName = copyString(dataSet);
    else
        self->dataSetName = NULL;

    self->confRef = confRef;
    self->fixedOffs = fixedOffs;

    self->address = NULL;

    self->sibling = NULL;

    if (parent != NULL)
        LogicalNode_addGSEControlBlock(parent, self);

    return self;
}

static void
GSEControlBlock_addPhyComAddress(GSEControlBlock* self, PhyComAddress* phyComAddress)
{
    self->address = phyComAddress;
}

PhyComAddress*
PhyComAddress_create(GSEControlBlock* parent, uint8_t vlanPriority, uint16_t vlanId, uint16_t appId, uint8_t dstAddress[])
{
    PhyComAddress* self = (PhyComAddress*) malloc(sizeof(PhyComAddress));

    self->vlanPriority = vlanPriority;
    self->vlanId = vlanId;
    self->appId = appId;

    memcpy(self->dstAddress, dstAddress, 6);

    if (parent != NULL)
        GSEControlBlock_addPhyComAddress(parent, self);

    return self;
}

static ModelNode*
DataObject_getLastChild(DataObject* self)
{
    ModelNode* lastNode = self->firstChild;

    ModelNode* nextNode = lastNode;

    while (nextNode != NULL) {
        lastNode = nextNode;
        nextNode = (ModelNode*) nextNode->sibling;
    }

    return lastNode;
}

static void
DataObject_addChild(DataObject* self, ModelNode* child)
{
    if (self->firstChild == NULL)
        self->firstChild = child;
    else {
        ModelNode* lastChild = DataObject_getLastChild(self);

        lastChild->sibling = child;
    }
}

DataObject*
DataObject_create(const char* name, ModelNode* parent, int arrayElements)
{
    DataObject* self = (DataObject*) malloc(sizeof(DataObject));

    self->name = copyString(name);
    self->modelType = DataObjectModelType;
    self->elementCount = arrayElements;
    self->firstChild = NULL;
    self->parent = parent;
    self->sibling = NULL;

    if (parent->modelType == LogicalNodeModelType)
        LogicalNode_addDataObject((LogicalNode*) parent, self);
    else if (parent->modelType == DataObjectModelType)
        DataObject_addChild((DataObject*) parent, (ModelNode*) self);

    return self;
}

static ModelNode*
DataAttribute_getLastChild(DataAttribute* self)
{
    ModelNode* lastNode = self->firstChild;

    ModelNode* nextNode = lastNode;

    while (nextNode != NULL) {
        lastNode = nextNode;
        nextNode = (ModelNode*) nextNode->sibling;
    }

    return lastNode;
}

static void
DataAttribute_addChild(DataAttribute* self, ModelNode* child)
{
    if (self->firstChild == NULL)
        self->firstChild = child;
    else {
        ModelNode* lastChild = DataAttribute_getLastChild(self);

        lastChild->sibling = child;
    }
}

DataAttribute*
DataAttribute_create(const char* name, ModelNode* parent, DataAttributeType type, FunctionalConstraint fc,
        uint8_t triggerOptions, int arrayElements, uint32_t sAddr)
{
    DataAttribute* self = (DataAttribute*) malloc(sizeof(DataAttribute));

    self->name = copyString(name);
    self->elementCount = arrayElements;
    self->modelType = DataAttributeModelType;
    self->type = type;
    self->fc = fc;
    self->firstChild = NULL;
    self->mmsValue = NULL;
    self->parent = parent;
    self->sibling = NULL;
    self->triggerOptions = triggerOptions;
    self->sAddr = sAddr;

    if (parent->modelType == DataObjectModelType)
        DataObject_addChild((DataObject*) parent, (ModelNode*) self);
    else if (parent->modelType == DataAttributeModelType)
        DataAttribute_addChild((DataAttribute*) parent, (ModelNode*) self);

    return self;
}

DataSet*
DataSet_create(const char* name, LogicalNode* parent)
{
    DataSet* self = (DataSet*) malloc(sizeof(DataSet));

    LogicalDevice* ld = (LogicalDevice*) parent->parent;

    self->name = createString(3, parent->name, "$", name);
    self->elementCount = 0;
    self->sibling = NULL;
    self->logicalDeviceName = ld->name;
    self->fcdas = NULL;

    IedModel_addDataSet((IedModel*) ld->parent, self);

    return self;
}

int
DataSet_getSize(DataSet* self)
{
    return self->elementCount;
}

DataSetEntry*
DataSet_getFirstEntry(DataSet* self)
{
    return self->fcdas;
}

DataSetEntry*
DataSetEntry_getNext(DataSetEntry* self)
{
    return self->sibling;
}

static void
DataSet_addEntry(DataSet* self, DataSetEntry* newEntry)
{
    self->elementCount++;

    if (self->fcdas == NULL)
        self->fcdas = newEntry;
    else {
        DataSetEntry* lastEntry = self->fcdas;

        while (lastEntry != NULL) {

            if (lastEntry->sibling == NULL) {
                lastEntry->sibling = newEntry;
                break;
            }

            lastEntry = lastEntry->sibling;
        }
    }
}

DataSetEntry*
DataSetEntry_create(DataSet* dataSet, char* variable, int index, char* component)
{
    DataSetEntry* self = (DataSetEntry*) malloc(sizeof(DataSetEntry));

    self->variableName = copyString(variable);

    if (component != NULL)
        self->componentName = copyString(component);
    else
        self->componentName = NULL;

    self->index = index;
    self->logicalDeviceName = dataSet->logicalDeviceName;
    self->sibling = NULL;

    DataSet_addEntry(dataSet, self);

    return self;
}

static void
ModelNode_destroy(ModelNode* modelNode)
{
    free(modelNode->name);

    ModelNode* currentChild = modelNode->firstChild;

    while (currentChild != NULL) {
        ModelNode* nextChild = currentChild->sibling;

        ModelNode_destroy(currentChild);

        currentChild = nextChild;
    }

    if (modelNode->modelType == DataAttributeModelType) {
        DataAttribute* dataAttribute = (DataAttribute*) modelNode;

        if (dataAttribute->mmsValue != NULL) {
            MmsValue_delete(dataAttribute->mmsValue);
            dataAttribute->mmsValue = NULL;
        }
    }

    free(modelNode);
}

void
IedModel_destroy(IedModel* model)
{
    // delete all model nodes and dynamically created strings

    /* delete all logical devices */

    LogicalDevice* ld = model->firstChild;

    while (ld != NULL) {
        free (ld->name);

        LogicalNode* ln = (LogicalNode*) ld->firstChild;

        while (ln != NULL) {
            free(ln->name);

            /* delete all data objects */

            DataObject* currentDataObject = (DataObject*) ln->firstChild;

            while (currentDataObject != NULL) {
                DataObject* nextDataObject = (DataObject*) currentDataObject->sibling;

                ModelNode_destroy((ModelNode*) currentDataObject);

                currentDataObject = nextDataObject;
            }

            LogicalNode* currentLn = ln;
            ln = (LogicalNode*) ln->sibling;

            free(currentLn);
        }


        LogicalDevice* currentLd = ld;
        ld = (LogicalDevice*) ld->sibling;

        free(currentLd);
    }

    /*  delete all data sets */

    DataSet* dataSet = model->dataSets;

    while (dataSet != NULL) {
        DataSet* nextDataSet = dataSet->sibling;

        free(dataSet->name);

        DataSetEntry* dse = dataSet->fcdas;

        while (dse != NULL) {
            DataSetEntry* nextDse = dse->sibling;

            if (dse->componentName != NULL)
                free(dse->componentName);

            free(dse->variableName);

            free(dse);

            dse = nextDse;
        }

        free(dataSet);

        dataSet = nextDataSet;
    }

    /* delete all RCBs */

    ReportControlBlock* rcb = model->rcbs;

    while (rcb != NULL) {
        ReportControlBlock* nextRcb = rcb->sibling;

        free(rcb->name);

        if (rcb->rptId)
            free(rcb->rptId);

        if (rcb->dataSetName)
            free(rcb->dataSetName);

        free(rcb);

        rcb = nextRcb;
    }

    /* delete all GoCBs */

    GSEControlBlock* gcb = model->gseCBs;

    while (gcb != NULL) {
        GSEControlBlock* nextGcb = gcb->sibling;

        free(gcb->name);
        free(gcb->appId);
        free(gcb->dataSetName);

        if (gcb->address)
            free(gcb->address);

        free(gcb);

        gcb = nextGcb;
    }

    if (model->name)
        free(model->name);

    free(model);

}

