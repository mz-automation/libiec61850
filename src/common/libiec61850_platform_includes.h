/*
 * libiec61850_platform_includes.h
 */

#ifndef LIBIEC61850_PLATFORM_INCLUDES_H_
#define LIBIEC61850_PLATFORM_INCLUDES_H_

#include "libiec61850_common_api.h"

#include "string_utilities.h"
#include <stdio.h>
#include <stdlib.h>

#include "platform_endian.h"

#if (DEBUG != 1)
#define NDEBUG 1
#endif

#include <assert.h>

#endif /* LIBIEC61850_PLATFORM_INCLUDES_H_ */
