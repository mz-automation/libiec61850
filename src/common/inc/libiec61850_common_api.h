/*
 * libiec61850_common_api.h
 */

#ifndef LIBIEC61850_COMMON_API_INCLUDES_H_
#define LIBIEC61850_COMMON_API_INCLUDES_H_

#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#ifdef __GNUC__
#define ATTRIBUTE_PACKED __attribute__ ((__packed__))
#else
#define ATTRIBUTE_PACKED
#endif

#ifndef DEPRECATED
#if defined(__GNUC__) || defined(__clang__)
  #define DEPRECATED __attribute__((deprecated))
#else
  #define DEPRECATED
#endif
#endif

#if defined _WIN32 || defined __CYGWIN__
    #ifdef EXPORT_FUNCTIONS_FOR_DLL
        #define LIB61850_API __declspec(dllexport)
    #else
        #define LIB61850_API
    #endif

    #define LIB61850_INTERNAL
#else
    #if __GNUC__ >= 4
        #define LIB61850_API __attribute__ ((visibility ("default")))
        #define LIB61850_INTERNAL  __attribute__ ((visibility ("hidden")))
    #else
        #define LIB61850_API
        #define LIB61850_INTERNAL
    #endif
#endif

#include "hal_time.h"
#include "mms_value.h"

#endif /* LIBIEC61850_COMMON_API_INCLUDES_H_ */
