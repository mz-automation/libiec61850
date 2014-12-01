/*
 *  iec61850_common.h
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

#ifndef IEC61850_COMMON_H_
#define IEC61850_COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "libiec61850_common_api.h"

/**
 * @defgroup iec61850_common_api_group IEC 61850 API common parts
 */
/**@{*/

/**
 * @defgroup TRIGGER_OPTIONS Trigger options (bit values combinable)
 *
 * @{
 */

/** Report will be triggerd when data changes */
#define TRG_OPT_DATA_CHANGED 1

/** Report will be triggered when quality changes */
#define TRG_OPT_QUALITY_CHANGED 2

/** Report will be triggered when data is updated */
#define TRG_OPT_DATA_UPDATE 4

/** Report will be triggered periodically */
#define TRG_OPT_INTEGRITY 8

/** Report will be triggered by GI (general interrogation) request */
#define TRG_OPT_GI 16
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
 * @defgroup FUNCTIONAL_CONSTRAINTS Definitions and functions related to functional constraints (FCs)
 *
 * @{
 */

/** FCs (Functional constraints) according to IEC 61850-7-2 */
typedef enum eFunctionalConstraint {
    /** Status information */
    ST = 0,
    /** Measurands - analog values */
    MX = 1,
    /** Setpoint */
    SP = 2,
    /** Substitution */
    SV = 3,
    /** Configuration */
    CF = 4,
    /** Description */
    DC = 5,
    /** Setting group */
    SG = 6,
    /** Setting group editable */
    SE = 7,
    /** Service response / Service tracking */
    SR = 8,
    /** Operate received */
    OR = 9,
    /** Blocking */
    BL = 10,
    /** Extended definition */
    EX = 11,
    /** Control */
    CO = 12,
    ALL = 99,
    NONE = -1
} FunctionalConstraint;

/**extern "C" {
 * \brief convert a function constraint to a static string
 */
char*
FunctionalConstraint_toString(FunctionalConstraint fc);

/**
 * \brief parse a string treated as a functional constraint representation
 */
FunctionalConstraint
FunctionalConstraint_fromString(char* fcString);

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

Validity
Quality_getValidity(Quality* self);

void
Quality_setValidity(Quality* self, Validity validity);

void
Quality_setFlag(Quality* self, int flag);

void
Quality_unsetFlag(Quality* self, int flag);

bool
Quality_isFlagSet(Quality* self, int flag);

Quality
Quality_fromMmsValue(MmsValue* mmsValue);

/** @} */

/**
 * @defgroup TIMESTAMP Definitions and functions related to IEC 61850 Timestamp (UTC Time) data type
 *
 * @{
 */

typedef union {
    uint8_t val[8];
} Timestamp;

uint32_t
Timestamp_getTimeInSeconds(Timestamp* self);

uint64_t
Timestamp_getTimeInMs(Timestamp* self);

bool
Timestamp_isLeapSecondKnown(Timestamp* self);

void
Timestamp_setLeapSecondKnown(Timestamp* self, bool value);

bool
Timestamp_hasClockFailure(Timestamp* self);

void
Timestamp_setClockFailure(Timestamp* self, bool value);

bool
Timestamp_isClockNotSynchronized(Timestamp* self);

void
Timestamp_setClockNotSynchronized(Timestamp* self, bool value);

int
Timestamp_getSubsecondPrecision(Timestamp* self);

/**
 * \brief Set the subsecond precision value of the time stamp
 *
 * \param subsecondPrecision the number of significant bits of the fractionOfSecond part of the time stamp
 */
void
Timestamp_setSubsecondPrecision(Timestamp* self, int subsecondPrecision);

void
Timestamp_setTimeInSeconds(Timestamp* self, uint32_t secondsSinceEpoch);

void
Timestamp_setTimeInMilliseconds(Timestamp* self, uint64_t millisSinceEpoch);


/** @} */

/**@}*/

#ifdef __cplusplus
}
#endif

#endif /* IEC61850_COMMON_H_ */
