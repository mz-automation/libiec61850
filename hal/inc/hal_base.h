/*
 *  hal_base.h
 *
 *  Copyright 2013-2021 Michael Zillgith
 *
 *  This file is part of Platform Abstraction Layer (libpal)
 *  for libiec61850, libmms, and lib60870.
 */

#ifndef HAL_INC_HAL_BASE_H_
#define HAL_INC_HAL_BASE_H_

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
        #define PAL_API __declspec(dllexport)
    #else
        #define PAL_API
    #endif

    #define PAL_INTERNAL
#else
    #if __GNUC__ >= 4
        #define PAL_API __attribute__ ((visibility ("default")))
        #define PAL_INTERNAL  __attribute__ ((visibility ("hidden")))
    #else
        #define PAL_API
        #define PAL_INTERNAL
    #endif
#endif

#endif /* HAL_INC_HAL_BASE_H_ */
