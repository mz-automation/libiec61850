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

#include "iec61850_model.h"
#include "iec61850_cdc.h"

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
LIB61850_API IedModel*
IedModel_create(const char* name/*, MemoryAllocator allocator*/);

/**
 * \brief Set the name of the IED (use only for dynamic model!)
 *
 * This will change the default name (usually "TEMPLATE") to a user configured values.
 * NOTE: This function has to be called before IedServer_create !
 * NOTE: For dynamic model (and configuration file date model) this function has to be
 * used instead of IedModel_setIedName.
 *
 * \param model the IedModel instance
 * \param the name of the configured IED
 */
LIB61850_API void
IedModel_setIedNameForDynamicModel(IedModel* self, const char* name);

/**
 * \brief destroy a dynamically created data model
 *
 * This function will free all the memory allocated for the data model.
 *
 * NOTE: Do not use this function when using a static data model (static_model.c create by static model generator).
 *
 * \param model the model instance to destroy
 */
LIB61850_API void
IedModel_destroy(IedModel* model);

/**
 * \brief Create a new logical device model and add it to the IED model
 *
 * \param name the name of the new logical device
 * \param parent the parent IED model
 *
 * \return the newly created LogicalDevice instance
 */
LIB61850_API LogicalDevice*
LogicalDevice_create(const char* name, IedModel* parent);


/**
 * \brief Create a new logical mode  and add it to a logical device
 *
 * \param name the name of the new logical node
 * \param parent the parent logical device
 *
 * \return the newly created LogicalNode instance
 */
LIB61850_API LogicalNode*
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
LIB61850_API DataObject*
DataObject_create(const char* name, ModelNode* parent, int arrayElements);

/**
 * \brief create a new data attribute and add it to a parent model node
 *
 * The parent model node has to be of type DataObject or DataAttribute
 *
 * \param name the name of the data attribute (e.g. "stVal")
 * \param parent the parent model node
 * \param type the type of the data attribute (CONSTRUCTED if the type contains sub data attributes)
 * \param fc the functional constraint (FC) of the data attribute
 * \param triggerOptions the trigger options (dupd, dchg, qchg) that cause an event notification
 * \param arrayElements the number of array elements if the data attribute is an array or 0
 * \param sAddr an optional short address
 *
 * \return the newly create DataAttribute instance
 */
LIB61850_API DataAttribute*
DataAttribute_create(const char* name, ModelNode* parent, DataAttributeType type, FunctionalConstraint fc,
        uint8_t triggerOptions, int arrayElements, uint32_t sAddr);

/**
 * \brief Get the data type of the data attribute
 *
 * \param self the data attribute instance
 *
 * \return the data attribute type
 */
LIB61850_API DataAttributeType
DataAttribute_getType(DataAttribute* self);

/**
 * \brief Get the functional constraint (FC) of the data attribute
 *
 * \param self the data attribute instance
 *
 * \return the functional constraint (FC) of the data attribute
 */
LIB61850_API FunctionalConstraint
DataAttribute_getFC(DataAttribute* self);

/**
 * \brief Get the trigger options of the data attribute
 *
 * \param self the data attribute instance
 *
 * \return the trigger options (dupd, dchg, qchg) that cause an event notification
 */
LIB61850_API uint8_t
DataAttribute_getTrgOps(DataAttribute* self);

/**
 * \brief Set the value of the data attribute (can be used to set default values before server is created)
 * 
 * \param self the data attribute instance
 * \param value the new default value
 */
LIB61850_API void
DataAttribute_setValue(DataAttribute* self, MmsValue* value);

/**
 * \brief create a new report control block (RCB)
 *
 * Create a new report control block (RCB) and add it to the given logical node (LN).
 *
 * \param name name of the RCB relative to the parent LN
 * \param parent the parent LN.
 * \param rptId of the report. If NULL the default report ID (object reference) is used.
 * \param isBuffered true for a buffered RCB - false for unbuffered RCB
 * \param dataSetName name (object reference) of the default data set or NULL if no data set
 *        is set by default
 * \param confRef the configuration revision
 * \param trgOps the trigger options supported by this RCB (bit set)
 * \param options the inclusion options. Specifies what elements are included in a report (bit set)
 * \param bufTm the buffering time of the RCB in milliseconds (time between the first event and the preparation of the report).
 * \param intgPd integrity period in milliseconds
 *
 * \return the new RCB instance.
 */
LIB61850_API ReportControlBlock*
ReportControlBlock_create(const char* name, LogicalNode* parent, char* rptId, bool isBuffered, char*
        dataSetName, uint32_t confRef, uint8_t trgOps, uint8_t options, uint32_t bufTm, uint32_t intgPd);

/**
 * \brief Set a pre-configured client for the RCB
 *
 * If set only the pre configured client should use this RCB instance
 *
 * \param self the RCB instance
 * \param clientType the type of the client (0 = no client, 4 = IPv4 client, 6 = IPv6 client)
 * \param clientAddress buffer containing the client address (4 byte in case of an IPv4 address, 16 byte in case of an IPv6 address, NULL for no client)
 */
LIB61850_API void
ReportControlBlock_setPreconfiguredClient(ReportControlBlock* self, uint8_t clientType, uint8_t* clientAddress);

/**
 * \brief create a new log control block (LCB)
 *
 * Create a new log control block (LCB) and add it to the given logical node (LN).
 *
 * \param name name of the LCB relative to the parent LN
 * \param parent the parent LN.
 * \param dataSetName name (object reference) of the default data set or NULL if no data set
 *        is set by default
 * \param logRef name (object reference) of the default log or NULL if no log is set by default. THe LDname doesn't contain the IED name!
 * \param trgOps the trigger options supported by this LCB (bit set)
 * \param intgPd integrity period in milliseconds
 * \param logEna if true the log will be enabled by default, false otherwise
 * \param reasonCode if true the reasonCode will be included in the log (this is always true in MMS mapping)
 *
 * \return the new LCB instance
 */
LIB61850_API LogControlBlock*
LogControlBlock_create(const char* name, LogicalNode* parent, char* dataSetName, char* logRef, uint8_t trgOps,
        uint32_t intgPd, bool logEna, bool reasonCode);

/**
 * \brief create a log (used by the IEC 61850 log service)
 *
 * \param name name of the LOG relative to the parent LN
 * \param parent the parent LN
 *
 * \return the new LOG instance
 */
LIB61850_API Log*
Log_create(const char* name, LogicalNode* parent);

/**
 * \brief create a setting group control block (SGCB)
 *
 * Create a new setting group control block (SGCB) and add it to the given logical node (LN).
 *
 * \param parent the parent LN.
 * \param the active setting group on server startup (1..N)
 * \param the number of setting groups (N)
 *
 * \return the new SGCB instance
 */
LIB61850_API SettingGroupControlBlock*
SettingGroupControlBlock_create(LogicalNode* parent, uint8_t actSG, uint8_t numOfSGs);

/**
 * \brief create a new GSE/GOOSE control block (GoCB)
 *
 * Create a new GOOSE control block (GoCB) and add it to the given logical node (LN)
 *
 * \param name name of the GoCB relative to the parent LN
 * \param parent the parent LN
 * \param appId the application ID of the GoCB
 * \param dataSet the data set reference to be used by the GoCB
 * \param confRev the configuration revision
 * \param fixedOffs indicates if GOOSE publisher shall use fixed offsets (NOT YET SUPPORTED)
 * \param minTime minimum GOOSE retransmission time (-1 if not specified - uses stack default then)
 * \param maxTime GOOSE retransmission time in stable state (-1 if not specified - uses stack default then)
 *
 * \return the new GoCB instance
 */
LIB61850_API GSEControlBlock*
GSEControlBlock_create(const char* name, LogicalNode* parent, char* appId, char* dataSet, uint32_t confRev,
        bool fixedOffs, int minTime, int maxTime);

/**
 * \brief create a new Multicast/Unicast Sampled Value (SV) control block (SvCB)
 *
 * Create a new Sampled Value control block (SvCB) and add it to the given logical node (LN)
 *
 * \param name name of the SvCB relative to the parent LN
 * \param parent the parent LN
 * \param svID the application ID of the SvCB
 * \param dataSet the data set reference to be used by the SVCB
 * \param confRev the configuration revision
 * \param smpMod the sampling mode used
 * \param smpRate the sampling rate used
 * \param optFlds the optional element configuration
 *
 * \return the new SvCB instance
 */
LIB61850_API SVControlBlock*
SVControlBlock_create(const char* name, LogicalNode* parent, char* svID, char* dataSet, uint32_t confRev, uint8_t smpMod,
        uint16_t smpRate, uint8_t optFlds, bool isUnicast);

LIB61850_API void
SVControlBlock_addPhyComAddress(SVControlBlock* self, PhyComAddress* phyComAddress);

LIB61850_API void
GSEControlBlock_addPhyComAddress(GSEControlBlock* self, PhyComAddress* phyComAddress);

/**
 * \brief create a PhyComAddress object
 *
 * A PhyComAddress object contains all required addressing information for a GOOSE publisher.
 *
 * \param vlanPriority the priority field of the VLAN tag
 * \param vlanId the ID field of the VLAN tag
 * \param appId the application identifier
 * \param dstAddress the 6 byte multicast MAC address to specify the destination
 *
 * \return the new PhyComAddress object
 */
LIB61850_API PhyComAddress*
PhyComAddress_create(uint8_t vlanPriority, uint16_t vlanId, uint16_t appId, uint8_t dstAddress[]);

/**
 * \brief create a new data set
 *
 * \param name the name of the data set
 * \param parent the logical node that hosts the data set (typically a LLN0)
 *
 * \return the new data set instance
 */
LIB61850_API DataSet*
DataSet_create(const char* name, LogicalNode* parent);

/**
 * \brief Get the name of the data set
 *
 * \param self the instance of the data set
 *
 * \returns the name of the data set (not the object reference).
 */
LIB61850_API const char*
DataSet_getName(DataSet* self);

/**
 * \brief returns the number of elements (entries) of the data set
 *
 * \param self the instance of the data set
 *
 * \returns the number of data set elements
 */
LIB61850_API int
DataSet_getSize(DataSet* self);

LIB61850_API DataSetEntry*
DataSet_getFirstEntry(DataSet* self);

LIB61850_API DataSetEntry*
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
 * If the variable parameter does not contain a logical device name (separated from the remaining variable
 * name by the "/" character) the logical device where the data set resides is used automatically.
 *
 * \param dataSet the data set to which the new entry will be added
 * \param variable the name of the variable as MMS variable name  including FC ("$" used as separator!)
 * \param index the index if the FCDA is an array element, otherwise -1
 * \param component the name of the component of the variable if the FCDA is a sub element of an array
 *        element. If this is not the case then NULL should be given here.
 *
 * \return the new data set entry instance
 */
LIB61850_API DataSetEntry*
DataSetEntry_create(DataSet* dataSet, const char* variable, int index, const char* component);

/**@}*/

/**@}*/

#ifdef __cplusplus
}
#endif

#endif /* DYNAMIC_MODEL_H_ */
