/*
 *  model.h
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

#ifndef MODEL_H_
#define MODEL_H_

#include "iec61850_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup server_api_group
 *  @{
 */

/**
 * @defgroup DATA_MODEL General data model definitions, access and iteration functions
 *
 * @{
 */

/**
 * \brief abstract base type for IEC 61850 data model nodes
 */
typedef struct sModelNode ModelNode;

/**
 * \brief IEC 61850 data model element of type data attribute
 */
typedef struct sDataAttribute DataAttribute;

/**
 * \brief IEC 61850 data model element of type data object
 */
typedef struct sDataObject DataObject;

/**
 * \brief IEC 61850 data model element of type logical node
 */
typedef struct sLogicalNode LogicalNode;

/**
 * \brief IEC 61850 data model element of type logical device
 */
typedef struct sLogicalDevice LogicalDevice;

/**
 * \brief Root node of the IEC 61850 data model. This is usually created by the model generator tool (genmodel.jar)
 */
typedef struct sIedModel IedModel;

typedef struct sDataSet DataSet;
typedef struct sReportControlBlock ReportControlBlock;

/**
 * \brief IEC 61850 data model of setting group control block (SGCB)
 */
typedef struct sSettingGroupControlBlock SettingGroupControlBlock;

typedef struct sGSEControlBlock GSEControlBlock;

typedef struct sSVControlBlock SVControlBlock;

typedef struct sLogControlBlock LogControlBlock;

typedef struct sLog Log;

typedef enum {
	IEC61850_BOOLEAN = 0,/* int */
	IEC61850_INT8 = 1,   /* int8_t */
	IEC61850_INT16 = 2,  /* int16_t */
	IEC61850_INT32 = 3,  /* int32_t */
	IEC61850_INT64 = 4,  /* int64_t */
	IEC61850_INT128 = 5, /* no native mapping! */
	IEC61850_INT8U = 6,  /* uint8_t */
	IEC61850_INT16U = 7, /* uint16_t */
	IEC61850_INT24U = 8, /* uint32_t */
	IEC61850_INT32U = 9, /* uint32_t */
	IEC61850_FLOAT32 = 10, /* float */
	IEC61850_FLOAT64 = 11, /* double */
	IEC61850_ENUMERATED = 12,
	IEC61850_OCTET_STRING_64 = 13,
	IEC61850_OCTET_STRING_6 = 14,
	IEC61850_OCTET_STRING_8 = 15,
	IEC61850_VISIBLE_STRING_32 = 16,
	IEC61850_VISIBLE_STRING_64 = 17,
	IEC61850_VISIBLE_STRING_65 = 18,
	IEC61850_VISIBLE_STRING_129 = 19,
	IEC61850_VISIBLE_STRING_255 = 20,
	IEC61850_UNICODE_STRING_255 = 21,
	IEC61850_TIMESTAMP = 22,
	IEC61850_QUALITY = 23,
	IEC61850_CHECK = 24,
	IEC61850_CODEDENUM = 25,
	IEC61850_GENERIC_BITSTRING = 26,
	IEC61850_CONSTRUCTED = 27,
	IEC61850_ENTRY_TIME = 28,
	IEC61850_PHYCOMADDR = 29,
	IEC61850_CURRENCY = 30


#if (CONFIG_IEC61850_USE_COMPAT_TYPE_DECLARATIONS == 1)
	,
    BOOLEAN = 0,/* int */
    INT8 = 1,   /* int8_t */
    INT16 = 2,  /* int16_t */
    INT32 = 3,  /* int32_t */
    INT64 = 4,  /* int64_t */
    INT128 = 5, /* no native mapping! */
    INT8U = 6,  /* uint8_t */
    INT16U = 7, /* uint16_t */
    INT24U = 8, /* uint32_t */
    INT32U = 9, /* uint32_t */
    FLOAT32 = 10, /* float */
    FLOAT64 = 11, /* double */
    ENUMERATED = 12,
    OCTET_STRING_64 = 13,
    OCTET_STRING_6 = 14,
    OCTET_STRING_8 = 15,
    VISIBLE_STRING_32 = 16,
    VISIBLE_STRING_64 = 17,
    VISIBLE_STRING_65 = 18,
    VISIBLE_STRING_129 = 19,
    VISIBLE_STRING_255 = 20,
    UNICODE_STRING_255 = 21,
    TIMESTAMP = 22,
    QUALITY = 23,
    CHECK = 24,
    CODEDENUM = 25,
    GENERIC_BITSTRING = 26,
    CONSTRUCTED = 27,
    ENTRY_TIME = 28,
    PHYCOMADDR = 29
#endif
} DataAttributeType;

typedef enum {
    LogicalDeviceModelType,
    LogicalNodeModelType,
    DataObjectModelType,
    DataAttributeModelType
} ModelNodeType;

struct sIedModel {
    char* name;
    LogicalDevice* firstChild;
    DataSet* dataSets;
    ReportControlBlock* rcbs;
    GSEControlBlock* gseCBs;
    SVControlBlock* svCBs;
    SettingGroupControlBlock* sgcbs;
    LogControlBlock* lcbs;
    Log* logs;
    void (*initializer) (void);
};

struct sLogicalDevice {
    ModelNodeType modelType;
    char* name;
    ModelNode* parent;
    ModelNode* sibling;
    ModelNode* firstChild;
};

struct sModelNode {
    ModelNodeType modelType;
    char* name;
    ModelNode* parent;
    ModelNode* sibling;
    ModelNode* firstChild;
};

struct sLogicalNode {
    ModelNodeType modelType;
    char* name;
    ModelNode* parent;
    ModelNode* sibling;
    ModelNode* firstChild;
};

struct sDataObject {
    ModelNodeType modelType;
    char* name;
    ModelNode* parent;
    ModelNode* sibling;
    ModelNode* firstChild;

    int elementCount; /* > 0 if this is an array */
};

struct sDataAttribute {
    ModelNodeType modelType;
    char* name;
    ModelNode* parent;
    ModelNode* sibling;
    ModelNode* firstChild;

    int elementCount; /* > 0 if this is an array */

    FunctionalConstraint fc;
    DataAttributeType type;

    uint8_t triggerOptions; /* TRG_OPT_DATA_CHANGED | TRG_OPT_QUALITY_CHANGED | TRG_OPT_DATA_UPDATE */

    MmsValue* mmsValue;

    uint32_t sAddr;
};

typedef struct sDataSetEntry {
    char* logicalDeviceName;
    bool isLDNameDynamicallyAllocated;
    char* variableName;
    int index;
    char* componentName;
    MmsValue* value;
    struct sDataSetEntry* sibling;
} DataSetEntry;

struct sDataSet {
    char* logicalDeviceName;
    char* name; /* eg. MMXU1$dataset1 */
    int elementCount;
    DataSetEntry* fcdas;
    DataSet* sibling;
};

struct sReportControlBlock {
    LogicalNode* parent;
    char* name;
    char* rptId;
    bool buffered;
    char* dataSetName; /* pre loaded with relative name in logical node */

    uint32_t confRef; /* ConfRef - configuration revision */
    uint8_t trgOps; /* TrgOps - trigger conditions */
    uint8_t options; /* OptFlds */
    uint32_t bufferTime; /* BufTm - time to buffer events until a report is generated */
    uint32_t intPeriod; /* IntgPd - integrity period */

    ReportControlBlock* sibling; /* next control block in list or NULL if this is the last entry */
};

struct sLogControlBlock {
    LogicalNode* parent;

    char* name;

    char* dataSetName;
    char* logRef;        /* object reference to the journal. Defaults to <MMS DomainName>/<LNName>$GeneralLog */

    uint8_t trgOps;      /* TrgOps - trigger conditions */
    uint32_t intPeriod;  /* IntgPd - integrity period */

    bool logEna;         /* enable log by default */
    bool reasonCode;     /* include reason code in log */

    LogControlBlock* sibling; /* next control block in list or NULL if this is the last entry */
};

struct sLog {
    LogicalNode* parent;

    char* name;

    Log* sibling; /* next log instance in list or NULL if this is the last entry */
};

struct sSettingGroupControlBlock {
    LogicalNode* parent;

    uint8_t actSG;       /* value from SCL file */
    uint8_t numOfSGs;    /* value from SCL file */

    uint8_t editSG;      /* 0 at power-up */
    bool cnfEdit;        /* false at power-up */
    uint64_t timestamp;
    uint16_t resvTms;

    SettingGroupControlBlock* sibling; /* next control block in list or NULL if this is the last entry */
};

struct sGSEControlBlock {
    LogicalNode* parent;
    char* name;
    char* appId;
    char* dataSetName; /* pre loaded with relative name in logical node */
    uint32_t confRev;  /* ConfRev - configuration revision */
    bool fixedOffs;    /* fixed offsets */
    PhyComAddress* address; /* GSE communication parameters */
    int minTime; /* optional minTime parameter --> -1 if not present */
    int maxTime; /* optional maxTime parameter --> -1 if not present */
    GSEControlBlock* sibling; /* next control block in list or NULL if this is the last entry */
};

struct sSVControlBlock {
    LogicalNode* parent;
    char* name;

    char* svId; /* MsvUD/UsvID */
    char* dataSetName; /* pre loaded with relative name in logical node */

    uint8_t optFlds;

    uint8_t smpMod;
    uint16_t smpRate;

    uint32_t confRev;  /* ConfRev - configuration revision */

    PhyComAddress* dstAddress; /* SV communication parameters */

    bool isUnicast;

    int noASDU;

    SVControlBlock* sibling; /* next control block in list or NULL if this is the last entry */
};

/**
 * \brief get the number of direct children of a model node
 *
 * \param self the model node instance
 *
 * \return the number of children of the model node
 * ¸
 */
int
ModelNode_getChildCount(ModelNode* self);

/**
 * \brief return a child model node
 *
 * \param self the model node instance
 * \param name the name of the child model node
 *
 * \return  the model node instance or NULL if model node does not exist.
 */
ModelNode*
ModelNode_getChild(ModelNode* self, const char* name);

/**
 * \brief return a child model node with a given functional constraint
 *
 * Sometimes the name is not enough to identify a model node. This is the case when
 * editable setting groups are used. In this case the setting group members have two different
 * model nodes associated that differ in their FC (SG and SE).
 *
 * \param self the model node instance
 * \param name the name of the child model node
 * \param fc the functional constraint of the model node
 *
 * \return  the model node instance or NULL if model node does not exist.
 */
ModelNode*
ModelNode_getChildWithFc(ModelNode* self, const char* name, FunctionalConstraint fc);

/**
 * \brief Return the IEC 61850 object reference of a model node
 *
 * \param self the model node instance
 * \param objectReference pointer to a buffer where to write the object reference string. If NULL
 *        is given the buffer is allocated by the function.
 *
 * \return the object reference string
 */
char*
ModelNode_getObjectReference(ModelNode* self, char* objectReference);

/**
 *
 */
ModelNodeType
ModelNode_getType(ModelNode* self);

/**
 * \brief Set the name of the IED
 *
 * This will change the default name (usualy "TEMPLATE") to a user configured values.
 * NOTE: This function has to be called before IedServer_create !
 *
 * \param model the IedModel instance
 * \param the name of the configured IED
 */
void
IedModel_setIedName(IedModel* self, const char* iedName);

/**
 * \brief Lookup a model node by its object reference
 *
 * This function uses the full logical device name as part of the object reference
 * as it happens to appear on the wire. E.g. if IED name in SCL file would be "IED1"
 * and the logical device "WD1" the resulting LD name would be "IED1WD".
 *
 * \param model the IedModel instance that holds the model node
 * \param objectReference the IEC 61850 object reference
 *
 * \return the model node instance or NULL if model node does not exist.
 */
ModelNode*
IedModel_getModelNodeByObjectReference(IedModel* model, const char* objectReference);

SVControlBlock*
IedModel_getSVControlBlock(IedModel* self, LogicalNode* parentLN, const char* svcbName);

/**
 * \brief Lookup a model node by its short (normalized) reference
 *
 * This version uses the object reference that does not contain the
 * IED name as part of the logical device name. This function is useful for
 * devices where the IED name can be configured.
 *
 * \param model the IedModel instance that holds the model node
 * \param objectReference the IEC 61850 object reference
 *
 * \return the model node instance or NULL if model node does not exist.
 */
ModelNode*
IedModel_getModelNodeByShortObjectReference(IedModel* model, const char* objectReference);

/**
 * \brief Lookup a model node by its short address
 *
 * Short address is a 32 bit unsigned integer as specified in the "sAddr" attribute of
 * the ICD file or in the configuration file.
 *
 * \param model the IedModel instance that holds the model node
 * \param shortAddress
 *
 * \return the model node instance or NULL if model node does not exist.
 */
ModelNode*
IedModel_getModelNodeByShortAddress(IedModel* model, uint32_t shortAddress);

/**
 * \brief Lookup a logical node by name that is part of the given logical device
 *
 * \param device the logical device instance
 * \param lnName the logical node name
 *
 * \return the logical device instance or NULL if it does not exist
 */
LogicalNode*
LogicalDevice_getLogicalNode(LogicalDevice* device, const char* lnName);

/**
 * \brief Get the setting group control block (SGCB) of the logical device
 *
 * \param device the logical device instance
 *
 * \return the SGCB instance or NULL if no SGCB is available
 */
SettingGroupControlBlock*
LogicalDevice_getSettingGroupControlBlock(LogicalDevice* device);

/**@}*/

/**@}*/


/**
 * \brief unset all MmsValue references in the data model
 *
 * \param model the IedModel instance that holds the model node
 */
void
IedModel_setAttributeValuesToNull(IedModel* iedModel);

LogicalDevice*
IedModel_getDevice(IedModel* model, const char* deviceName);

/*
 *  \param dataSetReference MMS mapping object reference! e.g. ied1Inverter/LLN0$dataset1
 */
DataSet*
IedModel_lookupDataSet(IedModel* model, const char* dataSetReference);

int
IedModel_getLogicalDeviceCount(IedModel* iedModel);

int
LogicalDevice_getLogicalNodeCount(LogicalDevice* logicalDevice);

ModelNode*
LogicalDevice_getChildByMmsVariableName(LogicalDevice* logicalDevice, const char* mmsVariableName);

bool
LogicalNode_hasFCData(LogicalNode* node, FunctionalConstraint fc);

bool
LogicalNode_hasBufferedReports(LogicalNode* node);

bool
LogicalNode_hasUnbufferedReports(LogicalNode* node);

/**
 * \brief get a data set instance
 *
 * \param self the logical node instance of the data set
 * \param dataSetName the name of the data set
 *
 * \return the data set instance or NULL if the data set does not exist
 */
DataSet*
LogicalNode_getDataSet(LogicalNode* self, const char* dataSetName);

bool
DataObject_hasFCData(DataObject* dataObject, FunctionalConstraint fc);

DataAttribute*
IedModel_lookupDataAttributeByMmsValue(IedModel* model, MmsValue* value);

#ifdef __cplusplus
}
#endif


#endif /* MODEL_H_ */
