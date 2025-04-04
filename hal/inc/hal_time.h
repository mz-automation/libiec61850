/*
 *  time.c
 *
 *  Copyright 2013-2024 Michael Zillgith
 *
 *  This file is part of Platform Abstraction Layer (libpal)
 *  for libiec61850, libmms, and lib60870.
 */

#ifndef HAL_C_
#define HAL_C_

#include "hal_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \file hal_time.h
 * \brief Abstraction layer for system time access
 */

/*! \addtogroup hal
   *
   *  @{
   */

/**
 * @defgroup HAL_TIME Time related functions
 *
 * @{
 */

typedef uint64_t nsSinceEpoch;
typedef uint64_t msSinceEpoch;

/**
 * Get the system time in milliseconds.
 *
 * The time value returned as 64-bit unsigned integer should represent the milliseconds
 * since the UNIX epoch (1970/01/01 00:00 UTC).
 *
 * \return the system time with millisecond resolution.
 */
PAL_API msSinceEpoch
Hal_getTimeInMs(void);

/**
 * Get the system time in nanoseconds.
 *
 * The time value returned as 64-bit unsigned integer should represent the nanoseconds
 * since the UNIX epoch (1970/01/01 00:00 UTC).
 *
 * \return the system time with nanosecond resolution.
 */
PAL_API nsSinceEpoch
Hal_getTimeInNs(void);

/**
* Set the system time from ns time
*
* The time value returned as 64-bit unsigned integer should represent the nanoseconds
* since the UNIX epoch (1970/01/01 00:00 UTC).
*
* \return true on success, otherwise false
*/
PAL_API bool
Hal_setTimeInNs(nsSinceEpoch nsTime);

/**
 * Get the monotonic time or system tick time in ms
 *
 * \return the system time with millisecond resolution.
 */
PAL_API msSinceEpoch
Hal_getMonotonicTimeInMs(void);

/**
 * Get the monotonic time or system tick in nanoseconds.
 *
 * \return the system time with nanosecond resolution.
 */
PAL_API nsSinceEpoch
Hal_getMonotonicTimeInNs(void);

/*! @} */

/*! @} */

#ifdef __cplusplus
}
#endif


#endif /* HAL_C_ */
