/*
 *  dynamic_model.h
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

#ifndef DYNAMIC_MODEL_H_
#define DYNAMIC_MODEL_H_

#include "model.h"
#include "cdc.h"

#ifdef __cplusplus
extern "C" {
#endif


/** \addtogroup server_api_group
 *  @{
 */

/**
 * @defgroup DYNAMIC_MODEL General dynamic model creation functions
 *
 * @{
 */


/**
 * \brief create a new IedModel instance
 *
 * The IedModel object is the root node of an IEC 61850 service data model.
 *
 * \param name the name of the IedModel or NULL (optional - NOT YET USED)
 *
 * \return
 */
IedModel*
IedModel_create(const char* name/*, MemoryAllocator allocator*/);

/**
 * \brief destroy a dynamically created data model
 *
 * This function will free all the memory allocated for the data model.
 *
 * NOTE: Do not use this function when using a static data model (static_model.c create by static model generator).
 *
 * \param model the model instance to destroy
 */
void
IedModel_destroy(IedModel* model);

/**
 * \brief Create a new logical device model and add it to the IED model
 *
 * \param name the name of the new logical device
 * \param parent the parent IED model
 *
 * \return the newly created LogicalDevice instance
 */
LogicalDevice*
LogicalDevice_create(const char* name, IedModel* parent);


/**
 * \brief Create a new logical mode  and add it to a logical device
 *
 * \param name the name of the new logical node
 * \param parent the parent logical device
 *
 * \return the newly created LogicalNode instance
 */
LogicalNode*
LogicalNode_create(const char* name, LogicalDevice* parent);

/**
 * \brief create a new data object and add it to a parent model node
 *
 * The parent model node has to be of type DataObject or LogicalNode.
 *
 * \param name the name of the data object (e.h. "Mod", "Health" ...)
 * \param parent the parent model node
 * \param arrayElements the number of array elements if the data object is an array or 0
 *
 * \return the newly create DataObject instance
 */
DataObject*
DataObject_create(const char* name, ModelNode* parent, int arrayElements);

/**
 * \brief create a new data attribute and add it to a parent model node
 *
 * The parent model node has to be of type LogicalNode or DataObject
 *
 * \param name the name of the data attribute (e.g. "stVal")
 * \param parent the parent model node
 * \param type the type of the data attribute (CONSTRUCTED if the type contains sub data attributes)
 * \param fc the functional constraint (FC) of the data attribte
 * \param triggerOptions the trigger options (dupd, dchg, qchg) that cause an event notification
 * \param arrayElements the number of array elements if the data attribute is an array or 0
 * \param sAddr an optional short address
 *
 * \return the newly create DataAttribute instance
 */
DataAttribute*
DataAttribute_create(const char* name, ModelNode* parent, DataAttributeType type, FunctionalConstraint fc,
        uint8_t triggerOptions, int arrayElements, uint32_t sAddr);

/**
 * \brief create a new report control block (RCB)
 *
 * Create a new report control block (RCB) and add it to the given logical node (LN).
 *
 * \param name name of the RCB relative to the parent LN
 * \param parent the parent LN.
 * \param rptId of the report. If NULL the default report ID (object reference) is used.
 * \param isBuffered true for a buffered RCB - false for unbuffered RCB
 * \param dataSetName name (object reference) of the default data set or NULL if no data
 *        is set by default
 * \param confRef the configuration revision
 * \param trgOps the trigger options supported by this RCB (bit set)
 * \param options the inclusion options. Specifies what elements are included in a report (bit set)
 * \param bufTm the buffering time of the RCB in milliseconds (time between the first event and the preparation of the report).
 * \param intgPd integrity period in milliseconds
 *
 * \return the new RCB instance.
 */
ReportControlBlock*
ReportControlBlock_create(const char* name, LogicalNode* parent, char* rptId, bool isBuffered, char*
        dataSetName, uint32_t confRef, uint8_t trgOps, uint8_t options, uint32_t bufTm, uint32_t intgPd);

/**
 * \brief create a new GSE/GOOSE control block (GoCB)
 *
 * Create a new GOOSE control block (GoCB) and add it to the given logical node (LN)
 *
 * \param name name of the GoCB relative to the parent LN
 * \param parent the parent LN
 * \param appId the application ID of the GoCB
 * \param dataSet the data set reference to be used by the GoCB
 * \param confRef the configuration revision
 * \param fixedOffs indicates if GOOSE publisher shall use fixed offsets (NOT YET SUPPORTED)
 *
 * \return the new GoCB instance
 */
GSEControlBlock*
GSEControlBlock_create(const char* name, LogicalNode* parent, char* appId, char* dataSet, uint32_t confRef, bool fixedOffs);

/**
 * \brief create a PhyComAddress object and add it to a GoCB
 *
 * A PhyComAddress object contains all required addressing informations for a GOOSE publisher.
 *
 * \param parent the parent GSEControlBlock object
 * \param vlanPriority the priority field of the VLAN tag
 * \param vlanId the ID field of the VLAN tag
 * \param appId the application identifier
 * \param dstAddress the 6 byte multicast MAC address to specify the destination
 *
 * \return the new PhyComAddress object
 */
PhyComAddress*
PhyComAddress_create(GSEControlBlock* parent, uint8_t vlanPriority, uint16_t vlanId, uint16_t appId, uint8_t dstAddress[]);

/**
 * \brief create a new data set
 *
 * \param name the name of the data set
 * \param parent the logical node that hosts the data set (typically a LLN0)
 *
 * \return the new data set instance
 */
DataSet*
DataSet_create(const char* name, LogicalNode* parent);

/**
 * \brief returns the number of elements (entries) of the data set
 *
 * \param self the instance of the data set
 *
 * \returns the number of data set elements
 */
int
DataSet_getSize(DataSet* self);

DataSetEntry*
DataSet_getFirstEntry(DataSet* self);

DataSetEntry*
DataSetEntry_getNext(DataSetEntry* self);

/**
 * \brief create a new data set entry (FCDA)
 *
 * Create a new FCDA reference and add it to the given data set as a new data set member.
 *
 * Note: Be aware that data set entries are not IEC 61850 object reference but MMS variable names
 * that have to contain the LN name, the FC and subsequent path elements separated by "$" instead of ".".
 * This is due to efficiency reasons to avoid the creation of additional strings.
 *
 * \param dataSet the data set to which the new entry will be added
 * \param variable the name of the variable as MMS variable name  including FC ("$" used as separator!)
 * \param index the index if the FCDA is an array element, otherwise -1
 * \param component the name of the component of the variable if the FCDA is a sub element of an array
 *        element. If this is not the case then NULL should be given here.
 *
 * \return the new data set entry instance
 */
DataSetEntry*
DataSetEntry_create(DataSet* dataSet, char* variable, int index, char* component);

/**@}*/

/**@}*/

#ifdef __cplusplus
}
#endif

#endif /* DYNAMIC_MODEL_H_ */
