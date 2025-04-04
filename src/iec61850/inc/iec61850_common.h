/*
 *  iec61850_common.h
 *
 *  Copyright 2013-2019 Michael Zillgith
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

#ifndef IEC61850_COMMON_H_
#define IEC61850_COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "libiec61850_common_api.h"
#include "logging_api.h"
#include "linked_list.h"

/**
 * @defgroup iec61850_common_api_group IEC 61850 API common parts
 */
/**@{*/

/** IEC 61850 edition 1 */
#define IEC_61850_EDITION_1   0

/** IEC 61850 edition 2 */
#define IEC_61850_EDITION_2   1

/** IEC 61850 edition 2.1 */
#define IEC_61850_EDITION_2_1 2

/** PhyComAddress type contains Ethernet address and VLAN attributes */
typedef struct {
    uint8_t vlanPriority;
    uint16_t vlanId;
    uint16_t appId;
    uint8_t dstAddress[6];
} PhyComAddress;

/** IEC 61850 ACSI classes */
typedef enum {
    ACSI_CLASS_DATA_OBJECT,
    ACSI_CLASS_DATA_SET,
    ACSI_CLASS_BRCB,
    ACSI_CLASS_URCB,
    ACSI_CLASS_LCB,
    ACSI_CLASS_LOG,
    ACSI_CLASS_SGCB,
    ACSI_CLASS_GoCB,
    ACSI_CLASS_GsCB,
    ACSI_CLASS_MSVCB,
    ACSI_CLASS_USVCB
} ACSIClass;

/**
 * \brief Control model (represented by "ctlModel" attribute)
 */
typedef enum {
    /**
     * No support for control functions. Control object only support status information.
     */
    CONTROL_MODEL_STATUS_ONLY = 0,

    /**
     * Direct control with normal security: Supports Operate, TimeActivatedOperate (optional),
     * and Cancel (optional).
     */
    CONTROL_MODEL_DIRECT_NORMAL = 1,

    /**
     * Select before operate (SBO) with normal security: Supports Select, Operate, TimeActivatedOperate (optional),
     * and Cancel (optional).
     */
    CONTROL_MODEL_SBO_NORMAL = 2,

    /**
     * Direct control with enhanced security (enhanced security includes the CommandTermination service)
     */
    CONTROL_MODEL_DIRECT_ENHANCED = 3,

    /**
     * Select before operate (SBO) with enhanced security (enhanced security includes the CommandTermination service)
     */
    CONTROL_MODEL_SBO_ENHANCED = 4
} ControlModel;

/**
 * @defgroup TRIGGER_OPTIONS Trigger options (bit values combinable)
 *
 * @{
 */

/** Report will be triggered when data changes */
#define TRG_OPT_DATA_CHANGED 1

/** Report will be triggered when quality changes */
#define TRG_OPT_QUALITY_CHANGED 2

/** Report will be triggered when data is updated */
#define TRG_OPT_DATA_UPDATE 4

/** Report will be triggered periodically */
#define TRG_OPT_INTEGRITY 8

/** Report will be triggered by GI (general interrogation) request */
#define TRG_OPT_GI 16

/** Report will be triggered only on rising edge (transient variable */
#define TRG_OPT_TRANSIENT 128
/** @} */



/**
 * @defgroup REPORT_OPTIONS Report options (bit values combinable)
 *
 * @{
 */

/** Report contains sequence number */
#define RPT_OPT_SEQ_NUM 1

/** Report contains a report timestamp */
#define RPT_OPT_TIME_STAMP 2

/** Report contains reason for inclusion value for each included data set member */
#define RPT_OPT_REASON_FOR_INCLUSION 4

/** Report contains data set object reference */
#define RPT_OPT_DATA_SET 8

/** Report contains data reference for each included data set member */
#define RPT_OPT_DATA_REFERENCE 16

/** Report contains buffer overflow flag */
#define RPT_OPT_BUFFER_OVERFLOW 32

/** Report contains entry id */
#define RPT_OPT_ENTRY_ID 64

/** Report contains configuration revision */
#define RPT_OPT_CONF_REV 128
/** @} */

/**
 * @defgroup ORIGINATOR_CATEGORIES Originator categories (orCat)
 *
 * @{
 */

/** Not supported - should not be used */
#define CONTROL_ORCAT_NOT_SUPPORTED 0

/** Control operation issued from an operator using a client located at bay level */
#define CONTROL_ORCAT_BAY_CONTROL 1

/** Control operation issued from an operator using a client located at station level */
#define CONTROL_ORCAT_STATION_CONTROL 2

/** Control operation from a remote operator outside the substation (for example network control center) */
#define CONTROL_ORCAT_REMOTE_CONTROL 3

/** Control operation issued from an automatic function at bay level */
#define CONTROL_ORCAT_AUTOMATIC_BAY 4

/** Control operation issued from an automatic function at station level */
#define CONTROL_ORCAT_AUTOMATIC_STATION 5

/** Control operation issued from a automatic function outside of the substation */
#define CONTROL_ORCAT_AUTOMATIC_REMOTE 6

/** Control operation issued from a maintenance/service tool */
#define CONTROL_ORCAT_MAINTENANCE 7

/** Status change occurred without control action (for example external trip of a circuit breaker or failure inside the breaker) */
#define CONTROL_ORCAT_PROCESS 8

/** @} */

/**
 * @defgroup CONTROL_ADD_CAUSE Definition for addCause type - used in control models
 *
 * @{
 */

/** AddCause - additional cause information for control model errors */
typedef enum {
    ADD_CAUSE_UNKNOWN = 0,
    ADD_CAUSE_NOT_SUPPORTED = 1,
    ADD_CAUSE_BLOCKED_BY_SWITCHING_HIERARCHY = 2,
    ADD_CAUSE_SELECT_FAILED = 3,
    ADD_CAUSE_INVALID_POSITION = 4,
    ADD_CAUSE_POSITION_REACHED = 5,
    ADD_CAUSE_PARAMETER_CHANGE_IN_EXECUTION = 6,
    ADD_CAUSE_STEP_LIMIT = 7,
    ADD_CAUSE_BLOCKED_BY_MODE = 8,
    ADD_CAUSE_BLOCKED_BY_PROCESS = 9,
    ADD_CAUSE_BLOCKED_BY_INTERLOCKING = 10,
    ADD_CAUSE_BLOCKED_BY_SYNCHROCHECK = 11,
    ADD_CAUSE_COMMAND_ALREADY_IN_EXECUTION = 12,
    ADD_CAUSE_BLOCKED_BY_HEALTH = 13,
    ADD_CAUSE_1_OF_N_CONTROL = 14,
    ADD_CAUSE_ABORTION_BY_CANCEL = 15,
    ADD_CAUSE_TIME_LIMIT_OVER = 16,
    ADD_CAUSE_ABORTION_BY_TRIP = 17,
    ADD_CAUSE_OBJECT_NOT_SELECTED = 18,
    ADD_CAUSE_OBJECT_ALREADY_SELECTED = 19,
    ADD_CAUSE_NO_ACCESS_AUTHORITY = 20,
    ADD_CAUSE_ENDED_WITH_OVERSHOOT = 21,
    ADD_CAUSE_ABORTION_DUE_TO_DEVIATION = 22,
    ADD_CAUSE_ABORTION_BY_COMMUNICATION_LOSS = 23,
    ADD_CAUSE_ABORTION_BY_COMMAND = 24,
    ADD_CAUSE_NONE = 25,
    ADD_CAUSE_INCONSISTENT_PARAMETERS = 26,
    ADD_CAUSE_LOCKED_BY_OTHER_CLIENT = 27
} ControlAddCause;

/** @} */

/**
 * @defgroup CONTROL_LAST_APPL_ERROR Definition for LastAppError error type - used in control models
 *
 * @{
 */

typedef enum {
    CONTROL_ERROR_NO_ERROR = 0,
    CONTROL_ERROR_UNKNOWN = 1,
    CONTROL_ERROR_TIMEOUT_TEST = 2,
    CONTROL_ERROR_OPERATOR_TEST = 3
} ControlLastApplError;

/** @} */

/**
 * @defgroup FUNCTIONAL_CONSTRAINTS Definitions and functions related to functional constraints (FCs)
 *
 * @{
 */

/** FCs (Functional constraints) according to IEC 61850-7-2 */
typedef enum eFunctionalConstraint {
    /** Status information */
    IEC61850_FC_ST = 0,
    /** Measurands - analog values */
    IEC61850_FC_MX = 1,
    /** Setpoint */
    IEC61850_FC_SP = 2,
    /** Substitution */
    IEC61850_FC_SV = 3,
    /** Configuration */
    IEC61850_FC_CF = 4,
    /** Description */
    IEC61850_FC_DC = 5,
    /** Setting group */
    IEC61850_FC_SG = 6,
    /** Setting group editable */
    IEC61850_FC_SE = 7,
    /** Service response / Service tracking */
    IEC61850_FC_SR = 8,
    /** Operate received */
    IEC61850_FC_OR = 9,
    /** Blocking */
    IEC61850_FC_BL = 10,
    /** Extended definition */
    IEC61850_FC_EX = 11,
    /** Control */
    IEC61850_FC_CO = 12,
    /** Unicast SV */
    IEC61850_FC_US = 13,
    /** Multicast SV */
    IEC61850_FC_MS = 14,
    /** Unbuffered report */
    IEC61850_FC_RP = 15,
    /** Buffered report */
    IEC61850_FC_BR = 16,
    /** Log control blocks */
    IEC61850_FC_LG = 17,
    /** Goose control blocks */
    IEC61850_FC_GO = 18,

    /** All FCs - wildcard value */
    IEC61850_FC_ALL = 99,
    IEC61850_FC_NONE = -1
} FunctionalConstraint;

/**extern "C" {
 * \brief convert a function constraint to a static string
 */
LIB61850_API char*
FunctionalConstraint_toString(FunctionalConstraint fc);

/**
 * \brief parse a string treated as a functional constraint representation
 */
LIB61850_API FunctionalConstraint
FunctionalConstraint_fromString(const char* fcString);

/** @} */

/**
 * @defgroup QUALITY Definitions and functions related to data attribute quality
 *
 * @{
 */


typedef uint16_t Quality;
typedef uint16_t Validity;

#define QUALITY_VALIDITY_GOOD 0
#define QUALITY_VALIDITY_INVALID 2
#define QUALITY_VALIDITY_RESERVED 1
#define QUALITY_VALIDITY_QUESTIONABLE 3

#define QUALITY_DETAIL_OVERFLOW 4
#define QUALITY_DETAIL_OUT_OF_RANGE 8
#define QUALITY_DETAIL_BAD_REFERENCE 16
#define QUALITY_DETAIL_OSCILLATORY 32
#define QUALITY_DETAIL_FAILURE 64
#define QUALITY_DETAIL_OLD_DATA 128
#define QUALITY_DETAIL_INCONSISTENT 256
#define QUALITY_DETAIL_INACCURATE 512

#define QUALITY_SOURCE_SUBSTITUTED 1024

#define QUALITY_TEST 2048

#define QUALITY_OPERATOR_BLOCKED  4096

#define QUALITY_DERIVED 8192

LIB61850_API Validity
Quality_getValidity(Quality* self);

LIB61850_API void
Quality_setValidity(Quality* self, Validity validity);

LIB61850_API void
Quality_setFlag(Quality* self, int flag);

LIB61850_API void
Quality_unsetFlag(Quality* self, int flag);

LIB61850_API bool
Quality_isFlagSet(Quality* self, int flag);

LIB61850_API Quality
Quality_fromMmsValue(const MmsValue* mmsValue);

LIB61850_API MmsValue*
Quality_toMmsValue(Quality* self, MmsValue* mmsValue);

/** @} */

/**
 * @defgroup DBPOS Definitions and functions related to IEC 61850 Dbpos (a CODED ENUM) data type
 *
 * @{
 */

typedef enum {
    DBPOS_INTERMEDIATE_STATE = 0,
    DBPOS_OFF = 1,
    DBPOS_ON = 2,
    DBPOS_BAD_STATE = 3
} Dbpos;


/**
 * \brief convert MMS bit string to Dbpos enumeration type
 *
 * \param mmsValue the MmsValue instance representing the Dbpos value
 *
 * \return the corresponding Dbpos value
 */
LIB61850_API Dbpos
Dbpos_fromMmsValue(const MmsValue* mmsValue);

/**
 * \brief conver Dbpos to MMS bit string
 *
 * \param mmsValue the MmsValue instance representing a Dbpos value or NULL to create a new MmsValue instance
 * \param a Dbpos value
 *
 * \return the corresponding MmsValue instance
 */
LIB61850_API MmsValue*
Dbpos_toMmsValue(MmsValue* mmsValue, Dbpos dbpos);

/** @} */

/**
 * @defgroup TIMESTAMP Definitions and functions related to IEC 61850 Timestamp (UTC Time) data type
 *
 * @{
 */

typedef union {
    uint8_t val[8];
} Timestamp;

LIB61850_API Timestamp*
Timestamp_create(void);

LIB61850_API Timestamp*
Timestamp_createFromByteArray(const uint8_t* byteArray);

LIB61850_API void
Timestamp_destroy(Timestamp* self);

LIB61850_API void
Timestamp_clearFlags(Timestamp* self);

LIB61850_API uint32_t
Timestamp_getTimeInSeconds(Timestamp* self);

LIB61850_API msSinceEpoch
Timestamp_getTimeInMs(Timestamp* self);

LIB61850_API nsSinceEpoch
Timestamp_getTimeInNs(Timestamp* self);

LIB61850_API bool
Timestamp_isLeapSecondKnown(Timestamp* self);

LIB61850_API void
Timestamp_setLeapSecondKnown(Timestamp* self, bool value);

LIB61850_API bool
Timestamp_hasClockFailure(Timestamp* self);

LIB61850_API void
Timestamp_setClockFailure(Timestamp* self, bool value);

LIB61850_API bool
Timestamp_isClockNotSynchronized(Timestamp* self);

LIB61850_API void
Timestamp_setClockNotSynchronized(Timestamp* self, bool value);

LIB61850_API int
Timestamp_getSubsecondPrecision(Timestamp* self);

LIB61850_API void
Timestamp_setFractionOfSecondPart(Timestamp* self, uint32_t fractionOfSecond);

LIB61850_API uint32_t
Timestamp_getFractionOfSecondPart(Timestamp* self);

LIB61850_API float
Timestamp_getFractionOfSecond(Timestamp* self);

/**
 * \brief Set the subsecond precision value of the time stamp
 *
 * \param subsecondPrecision the number of significant bits of the fractionOfSecond part of the time stamp
 */
LIB61850_API void
Timestamp_setSubsecondPrecision(Timestamp* self, int subsecondPrecision);

/**
 * \brief Set the time in seconds
 *
 * NOTE: the fractionOfSecond part is set to zero
 * NOTE: the subSecondPrecision is not touched
 *
 * \param self the Timestamp instance
 * \param secondsSinceEpoch the seconds since unix epoch (unix timestamp)
 */
LIB61850_API void
Timestamp_setTimeInSeconds(Timestamp* self, uint32_t secondsSinceEpoch);

/**
 * \brief Set the time in milliseconds
 *
 * NOTE: the subSecondPrecision is not touched
 *
 * \param self the Timestamp instance
 * \param msTime the milliseconds since unix epoch
 */
LIB61850_API void
Timestamp_setTimeInMilliseconds(Timestamp* self, msSinceEpoch msTime);

/**
 * \brief Set the time in nanoseconds
 *
 * NOTE: the subSecondPrecision is not touched
 *
 * \param self the Timestamp instance
 * \param msTime the nanoseconds since unix epoch
 */
LIB61850_API void
Timestamp_setTimeInNanoseconds(Timestamp* self, nsSinceEpoch nsTime);

LIB61850_API void
Timestamp_setByMmsUtcTime(Timestamp* self, const MmsValue* mmsValue);

/**
 * \brief Set an MmsValue instance of type UTCTime to the timestamp value
 *
 * \param self the Timestamp instance
 * \param mmsValue the mmsValue instance, if NULL a new instance will be created
 */
LIB61850_API MmsValue*
Timestamp_toMmsValue(Timestamp* self, MmsValue* mmsValue);

/**
 * \brief Get the Timestamp value from an MmsValue instance of type MMS_UTC_TIME
 *
 * \param self the Timestamp instance or NULL to create a new instance
 * \param mmsValue the mmsValue instance of type MMS_UTC_TIME
 *
 * \return the updated Timestamp value or NULL in case of an error
 */
LIB61850_API Timestamp*
Timestamp_fromMmsValue(Timestamp* self, MmsValue* mmsValue);

/**
 * \brief Get the version of the library as string
 *
 * \return the version of the library (e.g. "1.2.2")
 */
LIB61850_API char*
LibIEC61850_getVersionString(void);

/** @} */

/**@}*/

#ifdef __cplusplus
}
#endif

#endif /* IEC61850_COMMON_H_ */
