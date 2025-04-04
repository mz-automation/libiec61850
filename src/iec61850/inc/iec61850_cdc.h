/*
 *  cdc.h
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

#ifndef CDC_H_
#define CDC_H_

#ifdef __cplusplus
extern "C" {
#endif


/** \addtogroup server_api_group
 *  @{
 */

/**
 * @defgroup COMMON_DATA_CLASSES Helper functions to create common data classes (CDC) using the dynamic model API
 *
 * \brief Helper functions to create compliant common data classes (CDC) using the dynamic model API.
 *
 * Currently supports CDCs from IEC 61850-7-3:2010 (Edition 2)
 *
 * @{
 */

/**
 * \brief optional parts of CDCs
 */
#define CDC_OPTION_PICS_SUBST (1 << 0)
#define CDC_OPTION_BLK_ENA (1 << 1)

/** Add d (description) data attribute */
#define CDC_OPTION_DESC (1 << 2)

/** Add dU (unicode description) data attribute */
#define CDC_OPTION_DESC_UNICODE (1 << 3)

/** Add cdcNs and cdcName required when a CDC is an extension to the standard */
#define CDC_OPTION_AC_DLNDA (1 << 4)

/** Add dataNs (data namespace) required for extended CDCs */
#define CDC_OPTION_AC_DLN (1 << 5)

/** Add the unit data attribute */
#define CDC_OPTION_UNIT (1 << 6)

#define CDC_OPTION_FROZEN_VALUE (1 << 7)

#define CDC_OPTION_ADDR (1 << 8)
#define CDC_OPTION_ADDINFO (1 << 9)

#define CDC_OPTION_INST_MAG (1 << 10)
#define CDC_OPTION_RANGE (1 << 11)

#define CDC_OPTION_UNIT_MULTIPLIER (1 << 12)

#define CDC_OPTION_AC_SCAV (1 << 13)

#define CDC_OPTION_MIN (1 << 14)
#define CDC_OPTION_MAX (1 << 15)

#define CDC_OPTION_AC_CLC_O (1 << 16)

#define CDC_OPTION_RANGE_ANG (1 << 17)

#define CDC_OPTION_PHASE_A (1 << 18)
#define CDC_OPTION_PHASE_B (1 << 19)
#define CDC_OPTION_PHASE_C (1 << 20)

#define CDC_OPTION_PHASE_NEUT (1 << 21)

#define CDC_OPTION_PHASES_ABC (CDC_OPTION_PHASE_A | CDC_OPTION_PHASE_B | CDC_OPTION_PHASE_C)

#define CDC_OPTION_PHASES_ALL (CDC_OPTION_PHASE_A | CDC_OPTION_PHASE_B | CDC_OPTION_PHASE_C | CDC_OPTION_PHASE_NEUT)

#define CDC_OPTION_STEP_SIZE (1 << 22)

#define CDC_OPTION_ANGLE_REF (1 << 23)

/** Options that are only valid for DPL CDC */
#define CDC_OPTION_DPL_HWREV (1 << 17)
#define CDC_OPTION_DPL_SWREV (1 << 18)
#define CDC_OPTION_DPL_SERNUM (1 << 19)
#define CDC_OPTION_DPL_MODEL (1 << 20)
#define CDC_OPTION_DPL_LOCATION (1 << 21)

/** Add mandatory data attributes for LLN0 (e.g. LBL configRef) */
#define CDC_OPTION_AC_LN0_M (1 << 24)
#define CDC_OPTION_AC_LN0_EX (1 << 25)
#define CDC_OPTION_AC_DLD_M (1 << 26)

/**
 * \brief Control model types
 */
#define CDC_CTL_MODEL_NONE 0
#define CDC_CTL_MODEL_DIRECT_NORMAL 1
#define CDC_CTL_MODEL_SBO_NORMAL 2
#define CDC_CTL_MODEL_DIRECT_ENHANCED 3
#define CDC_CTL_MODEL_SBO_ENHANCED 4

#define CDC_CTL_MODEL_HAS_CANCEL (1 << 4)
#define CDC_CTL_MODEL_IS_TIME_ACTIVATED (1 << 5)

#define CDC_CTL_OPTION_ORIGIN (1 << 6)
#define CDC_CTL_OPTION_CTL_NUM (1 << 7)
#define CDC_CTL_OPTION_ST_SELD (1 << 8)
#define CDC_CTL_OPTION_OP_RCVD (1 << 9)
#define CDC_CTL_OPTION_OP_OK (1 << 10)
#define CDC_CTL_OPTION_T_OP_OK (1 << 11)
#define CDC_CTL_OPTION_SBO_TIMEOUT (1 << 12)
#define CDC_CTL_OPTION_SBO_CLASS (1 << 13)
#define CDC_CTL_OPTION_OPER_TIMEOUT (1 << 14)

/****************************************************
 * Constructed Attribute Classes (CAC)
 ***************************************************/

LIB61850_API DataAttribute*
CAC_AnalogueValue_create(const char* name, ModelNode* parent, FunctionalConstraint fc, uint8_t triggerOptions,
        bool isIntegerNotFloat);


/**
 * \brief create a ValWithTrans constructed data attribute
 *
 * \param hasTransInd
 */
LIB61850_API DataAttribute*
CAC_ValWithTrans_create(const char* name, ModelNode* parent, FunctionalConstraint fc, uint8_t triggerOptions, bool hasTransientIndicator);


/**
 * CDC_OPTION_AC_CLC_O
 */
LIB61850_API DataAttribute*
CAC_Vector_create(const char* name, ModelNode* parent, uint32_t options, FunctionalConstraint fc, uint8_t triggerOptions);

LIB61850_API DataAttribute*
CAC_Point_create(const char* name, ModelNode* parent, FunctionalConstraint fc, uint8_t triggerOptions, bool hasZVal);

LIB61850_API DataAttribute*
CAC_ScaledValueConfig_create(const char* name, ModelNode* parent);

LIB61850_API DataAttribute*
CAC_Unit_create(const char* name, ModelNode* parent, bool hasMagnitude);

LIB61850_API DataAttribute*
CDA_OperBoolean(ModelNode* parent, bool isTImeActivated);

/****************************************************
 * Common Data Classes (CDC)
 ***************************************************/

LIB61850_API DataObject*
CDC_SPS_create(const char* dataObjectName, ModelNode* parent, uint32_t options);

LIB61850_API DataObject*
CDC_DPS_create(const char* dataObjectName, ModelNode* parent, uint32_t options);

LIB61850_API DataObject*
CDC_INS_create(const char* dataObjectName, ModelNode* parent, uint32_t options);

LIB61850_API DataObject*
CDC_ENS_create(const char* dataObjectName, ModelNode* parent, uint32_t options);

LIB61850_API DataObject*
CDC_BCR_create(const char* dataObjectName, ModelNode* parent, uint32_t options);

LIB61850_API DataObject*
CDC_VSS_create(const char* dataObjectName, ModelNode* parent, uint32_t options);

/**
 * \brief create a new SEC (Security violation) CDC instance (data object)
 *
 * Allowed parent types are LogicalNode and DataObject.
 *
 * options:
 *   standard (include standard optional elements like extension namespaces and descriptions (d, dU).
 *
 *   CDC_OPTION_ADDR (address of the client causing the security violation)
 *   CDC_OPTION_ADDINFO (additional info text)
 *
 * \param dataObjectName the name of the new object
 * \param parent the parent of the new data object (either a LogicalNode or another DataObject)
 * \param options bit mask to encode required optional elements
 */
LIB61850_API DataObject*
CDC_SEC_create(const char* dataObjectName, ModelNode* parent, uint32_t options);

/**
 * \brief create a new MV (Measured value) CDC instance (data object)
 *
 * Allowed parent types are LogicalNode and DataObject.
 *
 * possible options:
 *   CDC_OPTION_INST_MAG
 *   CDC_OPTION_RANGE
 *   CDC_OPTION_PICS_SUBST
 *   standard (include standard optional elements like extension namespaces and descriptions (d, dU).
 *
 * \param dataObjectName the name of the new object
 * \param parent the parent of the new data object (either a LogicalNode or another DataObject)
 * \param options bit mask to encode required optional elements
 * \param isIntegerNotFloat if true the AnalogueValue instance have integer instead of float
 *
 */
LIB61850_API DataObject*
CDC_MV_create(const char* dataObjectName, ModelNode* parent, uint32_t options, bool isIntegerNotFloat);

/**
 * CDC_OPTION_INST_MAG
 * CDC_OPTION_RANGE
 */
LIB61850_API DataObject*
CDC_CMV_create(const char* dataObjectName, ModelNode* parent, uint32_t options);

/**
 * \brief create a new SAV (Sampled analog value) CDC instance (data object)
 *
 * Allowed parent types are LogicalNode and DataObject.
 *
 * possible options:
 *   CDC_OPTION_UNIT
 *   CDC_OPTION_AC_SCAV
 *   CDC_OPTION_MIN
 *   CDC_OPTION_MAX
 *   standard (include standard optional elements like extension namespaces and descriptions (d, dU).
 *
 * \param dataObjectName the name of the new object
 * \param parent the parent of the new data object (either a LogicalNode or another DataObject)
 * \param options bit mask to encode required optional elements
 * \param isIntegerNotFloat if true the AnalogueValue instance have integer instead of float
 *
 */
LIB61850_API DataObject*
CDC_SAV_create(const char* dataObjectName, ModelNode* parent, uint32_t options, bool isIntegerNotFloat);

/**
 * \brief create a new LPL (Logical node name plate) CDC instance (data object)
 *
 * Allowed parent type is LogicalNode
 *
 * possible options:
 *   CDC_OPTION_AC_LN0_M (includes "configRev")
 *   CDC_OPTION_AC_LN0_EX (includes "ldNs")
 *   CDC_OPTION_AC_DLD_M (includes "lnNs")
 *   standard options:
 *   CDC_OPTION_DESC (includes "d")
 *   CDC_OPTION_DESC_UNICODE (include "du")
 *   CDC_OPTION_AC_DLNDA (include "cdcNs" and "cdcName")
 *   CDC_OPTION_AC_DLN (includes "dataNs")
 *
 * \param dataObjectName the name of the new object
 * \param parent the parent of the new data object (either a LogicalNode or another DataObject)
 * \param options bit mask to encode required optional elements
 *
 * \return new DataObject instance
 */
LIB61850_API DataObject*
CDC_LPL_create(const char* dataObjectName, ModelNode* parent, uint32_t options);

/**
 * \brief create a new DPL (Device name plate) CDC instance (data object)
 *
 * Allowed parent type is LogicalNode
 *
 * possible options:
 *   CDC_OPTION_DPL_HWREV (includes "hwRev")
 *   CDC_OPTION_DPL_SWREV (includes "swRev")
 *   CDC_OPTION_DPL_SERNUM (includes "serNum")
 *   CDC_OPTION_DPL_MODEL (includes "model")
 *   CDC_OPTION_DPL_LOCATION (includes "location")
 *   standard options:
 *   CDC_OPTION_AC_DLNDA (include "cdcNs" and "cdcName")
 *   CDC_OPTION_AC_DLN (includes "dataNs")
 *
 * \param dataObjectName the name of the new object
 * \param parent the parent of the new data object (either a LogicalNode or another DataObject)
 * \param options bit mask to encode required optional elements
 *
 * \return new DataObject instance
 */
LIB61850_API DataObject*
CDC_DPL_create(const char* dataObjectName, ModelNode* parent, uint32_t options);

LIB61850_API DataObject*
CDC_HST_create(const char* dataObjectName, ModelNode* parent, uint32_t options, uint16_t maxPts);

/**
 * \brief Directional protection activation information (ACD)
 *
 * Allowed parent types are LogicalNode and DataObject.
 *
 * possible options:
 *   CDC_OPTION_PHASE_A
 *   CDC_OPTION_PHASE_B
 *   CDC_OPTION_PHASE_C
 *   CDC_OPTION_PHASE_NEUT
 *   CDC_OPTION_PHASES_ABC
 *   CDC_OPTION_PHASES_ALL
 *   standard (include standard optional elements like extension namespaces and descriptions (d, dU).
 *
 * \param dataObjectName the name of the new object
 * \param parent the parent of the new data object (either a LogicalNode or another DataObject)
 * \param options bit mask to encode required optional elements
 */
LIB61850_API DataObject*
CDC_ACD_create(const char* dataObjectName, ModelNode* parent, uint32_t options);

/**
 * \brief Protection activation information (ACT)
 */
LIB61850_API DataObject*
CDC_ACT_create(const char* dataObjectName, ModelNode* parent, uint32_t options);

/**
 * \brief Single point setting (SPG)
 *
 * \param dataObjectName the name of the new object
 * \param parent the parent of the new data object (either a LogicalNode or another DataObject)
 * \param options bit mask to encode required optional elements
 */
LIB61850_API DataObject*
CDC_SPG_create(const char* dataObjectName, ModelNode* parent, uint32_t options);

/**
 * \brief Visible string setting (VSG)
 *
 * \param dataObjectName the name of the new object
 * \param parent the parent of the new data object (either a LogicalNode or another DataObject)
 * \param options bit mask to encode required optional elements
 */
LIB61850_API DataObject*
CDC_VSG_create(const char* dataObjectName, ModelNode* parent, uint32_t options);

/**
 * \brief Enumerated status setting (ENG)
 *
 * \param dataObjectName the name of the new object
 * \param parent the parent of the new data object (either a LogicalNode or another DataObject)
 * \param options bit mask to encode required optional elements
 */
LIB61850_API DataObject*
CDC_ENG_create(const char* dataObjectName, ModelNode* parent, uint32_t options);

/**
 * \brief Integer status setting (ING)
 *
 * possible options:
 *   CDC_OPTION_UNIT
 *   CDC_OPTION_MIN
 *   CDC_OPTION_MAX
 *   CDC_OPTION_STEP_SIZE
 *   standard (include standard optional elements like extension namespaces and descriptions (d, dU).
 *
 */
LIB61850_API DataObject*
CDC_ING_create(const char* dataObjectName, ModelNode* parent, uint32_t options);

/**
 * \brief Analogue Setting (ASG)
 *
 * possible options:
 *   CDC_OPTION_UNIT
 *   CDC_OPTION_MIN
 *   CDC_OPTION_MAX
 *   CDC_OPTION_STEP_SIZE
 *   standard (include standard optional elements like extension namespaces and descriptions (d, dU).
 *
 */
LIB61850_API DataObject*
CDC_ASG_create(const char* dataObjectName, ModelNode* parent, uint32_t options, bool isIntegerNotFloat);

/**
 * \brief Phase to ground/neutral related measured values of a three-phase system (WYE)
 *
 * possible options:
 *   CDC_OPTION_ANGLE_REF
 */
LIB61850_API DataObject*
CDC_WYE_create(const char* dataObjectName, ModelNode* parent, uint32_t options);

/**
 * \brief Phase to phase related measured values of a three-phase system (DEL)
 *
 * possible options:
 *   CDC_OPTION_ANGLE_REF
 */
LIB61850_API DataObject*
CDC_DEL_create(const char* dataObjectName, ModelNode* parent, uint32_t options);

/***************************
 * Controls
 ***************************/

/**
 * \brief Controllable single point (SPC)
 *
 * \param controlOptions specify which control model to set as default and other control related options
 */
LIB61850_API DataObject*
CDC_SPC_create(const char* dataObjectName, ModelNode* parent, uint32_t options, uint32_t controlOptions);

/**
 * \brief Controllable double point (DPC)
 *
 * CDC_OPTION_IS_TIME_ACTICATED
 *
 * substitution options
 * CDC_OPTION_BLK_ENA
 * standard description and namespace options
 *
 * \param dataObjectName the name of the new object
 * \param parent the parent of the new data object (either a LogicalNode or another DataObject)
 * \param options bit mask to encode required optional elements
 * \param defaultControlModel specify which control model to set as default.
 *
 */
LIB61850_API DataObject*
CDC_DPC_create(const char* dataObjectName, ModelNode* parent, uint32_t options, uint32_t controlOptions);

/**
 * \brief Controllable integer status (INC)
 *
 * CDC_OPTION_IS_TIME_ACTICATED
 *
 * CDC_OPTION_MIN
 * CDC_OPTION_MAX
 * CDC_OPTION_STEP_SIZE
 *
 * substitution options
 * CDC_OPTION_BLK_ENA
 * standard description and namespace options
 *
 * \param dataObjectName the name of the new object
 * \param parent the parent of the new data object (either a LogicalNode or another DataObject)
 * \param options bit mask to encode required optional elements
 * \param defaultControlModel specify which control model to set as default.
 *
 */
LIB61850_API DataObject*
CDC_INC_create(const char* dataObjectName, ModelNode* parent, uint32_t options, uint32_t controlOptions);

/**
 * \brief Controllable enumerated status (ENC)
 *
 * CDC_OPTION_IS_TIME_ACTICATED
 *
 * substitution options
 * CDC_OPTION_BLK_ENA
 * standard description and namespace options
 *
 * \param dataObjectName the name of the new object
 * \param parent the parent of the new data object (either a LogicalNode or another DataObject)
 * \param options bit mask to encode required optional elements
 * \param defaultControlModel specify which control model to set as default.
 *
 */
LIB61850_API DataObject*
CDC_ENC_create(const char* dataObjectName, ModelNode* parent, uint32_t options, uint32_t controlOptions);

/**
 * \brief Controllable enumerated status (ENC)
 *
 * CDC_OPTION_IS_TIME_ACTICATED
 *
 * substitution options
 * CDC_OPTION_BLK_ENA
 * standard description and namespace options
 *
 * \param dataObjectName the name of the new object
 * \param parent the parent of the new data object (either a LogicalNode or another DataObject)
 * \param options bit mask to encode required optional elements
 * \param controlOptions specify which control model to set as default and other control specific options
 * \param hasTransientIndicator specifies if the step position information contains the transient indicator
 *
 */
LIB61850_API DataObject*
CDC_BSC_create(const char* dataObjectName, ModelNode* parent, uint32_t options, uint32_t controlOptions, bool hasTransientIndicator);

/**
 * \brief Integer controlled step position information (ISC)
 *
 * CDC_OPTION_IS_TIME_ACTICATED
 *
 * substitution options
 * CDC_OPTION_BLK_ENA
 * standard description and namespace options
 *
 * \param dataObjectName the name of the new object
 * \param parent the parent of the new data object (either a LogicalNode or another DataObject)
 * \param options bit mask to encode required optional elements
 * \param controlOptions specify which control model to set as default and other control specific options
 * \param hasTransientIndicator specifies if the step position information contains the transient indicator
 *
 */
LIB61850_API DataObject*
CDC_ISC_create(const char* dataObjectName, ModelNode* parent, uint32_t options, uint32_t controlOptions, bool hasTransientIndicator);

/**
 * \brief Controllable analogue process value (APC)
 *
 * CDC_OPTION_IS_TIME_ACTICATED
 *
 * CDC_OPTION_MIN
 * CDC_OPTION_MAX
 *
 * substitution options
 * CDC_OPTION_BLK_ENA
 * standard description and namespace options
 *
 * \param dataObjectName the name of the new object
 * \param parent the parent of the new data object (either a LogicalNode or another DataObject)
 * \param options bit mask to encode required optional elements
 * \param controlOptions specify which control model to set as default and other control specific options
 * \param isIntegerNotFloat
 */
LIB61850_API DataObject*
CDC_APC_create(const char* dataObjectName, ModelNode* parent, uint32_t options, uint32_t controlOptions, bool isIntegerNotFloat);

/**
 * \brief Binary controlled ananlogue process value (BAC)
 *
 * CDC_OPTION_IS_TIME_ACTICATED
 *
 * CDC_OPTION_MIN
 * CDC_OPTION_MAX
 * CDC_OPTION_STEP_SIZE
 *
 * substitution options
 * CDC_OPTION_BLK_ENA
 * standard description and namespace options
 *
 * \param dataObjectName the name of the new object
 * \param parent the parent of the new data object (either a LogicalNode or another DataObject)
 * \param options bit mask to encode required optional elements
 * \param controlOptions specify which control model to set as default and other control specific options
 * \param isIntegerNotFloat
 */
LIB61850_API DataObject*
CDC_BAC_create(const char* dataObjectName, ModelNode* parent, uint32_t options, uint32_t controlOptions, bool isIntegerNotFloat);

/** Minimum measured value */
#define CDC_OPTION_61400_MIN_MX_VAL (1 << 10)

/** Maximum measured value */
#define CDC_OPTION_61400_MAX_MX_VAL (1 << 11)

/** Total average value of data */
#define CDC_OPTION_61400_TOT_AV_VAL (1 << 12)

/** Standard deviation of data */
#define CDC_OPTION_61400_SDV_VAL (1 << 13)

/** Rate of increase */
#define CDC_OPTION_61400_INC_RATE (1 << 14)

/** Rate of decrease */
#define CDC_OPTION_61400_DEC_RATE (1 << 15)

/** Setpoint or parameter access level (low/medium/high) */
#define CDC_OPTION_61400_SP_ACS (1 << 16)

/** Time periodical reset (hourly/daily/weekly/monthly) */
#define CDC_OPTION_61400_CHA_PER_RS (1 << 17)

/** Command access level */
#define CDC_OPTION_61400_CM_ACS (1 << 18)

/** Total time duration of a state */
#define CDC_OPTION_61400_TM_TOT (1 << 19)

/** Daily counting data */
#define CDC_OPTION_61400_COUNTING_DAILY (1 << 20)

/** Monthly counting data */
#define CDC_OPTION_61400_COUNTING_MONTHLY (1 << 21)

/** Yearly counting data */
#define CDC_OPTION_61400_COUNTING_YEARLY (1 << 22)

/** Total counting data */
#define CDC_OPTION_61400_COUNTING_TOTAL (1 << 23)

/** All counting data */
#define CDC_OPTION_61400_COUNTING_ALL (CDC_OPTION_61400_COUNTING_DAILY | CDC_OPTION_61400_COUNTING_MONTHLY | CDC_OPTION_61400_COUNTING_YEARLY | CDC_OPTION_61400_COUNTING_TOTAL)

LIB61850_API DataObject*
CDC_SPV_create(const char* dataObjectName, ModelNode* parent,
        uint32_t options,
        uint32_t controlOptions,
        uint32_t wpOptions,
        bool hasChaManRs);

LIB61850_API DataObject*
CDC_STV_create(const char* dataObjectName, ModelNode* parent,
        uint32_t options,
        uint32_t controlOptions,
        uint32_t wpOptions,
        bool hasOldStatus);

LIB61850_API DataObject*
CDC_CMD_create(const char* dataObjectName, ModelNode* parent,
        uint32_t options,
        uint32_t controlOptions,
        uint32_t wpOptions,
        bool hasOldStatus,
        bool hasCmTm,
        bool hasCmCt);

LIB61850_API DataObject*
CDC_ALM_create(const char* dataObjectName, ModelNode* parent,
        uint32_t options,
        uint32_t controlOptions,
        uint32_t wpOptions,
        bool hasOldStatus);

LIB61850_API DataObject*
CDC_CTE_create(const char* dataObjectName, ModelNode* parent,
        uint32_t options,
        uint32_t controlOptions,
        uint32_t wpOptions,
        bool hasHisRs);

LIB61850_API DataObject*
CDC_TMS_create(const char* dataObjectName, ModelNode* parent,
        uint32_t options,
        uint32_t controlOptions,
        uint32_t wpOptions,
        bool hasHisRs);

/**@}*/

/**@}*/

#ifdef __cplusplus
}
#endif

#endif /* CDC_H_ */
