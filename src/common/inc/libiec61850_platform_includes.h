/*
 * libiec61850_platform_includes.h
 */

#ifndef LIBIEC61850_PLATFORM_INCLUDES_H_
#define LIBIEC61850_PLATFORM_INCLUDES_H_

#include "stack_config.h"

#include "libiec61850_common_api.h"

#include "string_utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "platform_endian.h"

#define LIBIEC61850_VERSION "1.5.0"

#ifndef CONFIG_DEFAULT_MMS_VENDOR_NAME
#define CONFIG_DEFAULT_MMS_VENDOR_NAME "libiec61850.com"
#endif

#ifndef CONFIG_DEFAULT_MMS_MODEL_NAME
#define CONFIG_DEFAULT_MMS_MODEL_NAME "LIBIEC61850"
#endif

#ifndef CONFIG_DEFAULT_MMS_REVISION
#define CONFIG_DEFAULT_MMS_REVISION LIBIEC61850_VERSION
#endif

#ifndef CONFIG_IEC61850_SAMPLED_VALUES_SUPPORT
#define CONFIG_IEC61850_SAMPLED_VALUES_SUPPORT 0
#endif

#if (DEBUG != 1)
#define NDEBUG 1
#endif

#include <assert.h>

#include "lib_memory.h"

#endif /* LIBIEC61850_PLATFORM_INCLUDES_H_ */
