/*
 *  hal_base.h
 *
 *  Copyright 2018 MZ Automation GmbH
 *
 *  This file is part of Platform Abstraction Layer (libpal)
 *  for libiec61850 and lib60870.
 *
 *  libpal is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  libpal is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with libpal.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  See COPYING file for the complete license text.
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
