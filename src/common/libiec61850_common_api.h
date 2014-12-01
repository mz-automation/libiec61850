/*
 * libiec61850_common_api_includes.h
 */

#ifndef LIBIEC61850_COMMON_API_INCLUDES_H_
#define LIBIEC61850_COMMON_API_INCLUDES_H_

#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

#ifdef __GNUC__
#define ATTRIBUTE_PACKED __attribute__ ((__packed__))
#else
#define ATTRIBUTE_PACKED
#endif

#include "time_hal.h"
#include "mms_value.h"

#endif /* LIBIEC61850_COMMON_API_INCLUDES_H_ */
