/*
 *  lib_memory.h
 *
 *  Copyright 2014-2021 Michael Zillgith
 *
 *  This file is part of Platform Abstraction Layer (libpal)
 *  for libiec61850, libmms, and lib60870.
 */

#ifndef MEMORY_H_
#define MEMORY_H_

#include "hal_base.h"

#define CALLOC(nmemb, size) Memory_calloc(nmemb, size)
#define MALLOC(size)        Memory_malloc(size)
#define REALLOC(oldptr, size)   Memory_realloc(oldptr, size)
#define FREEMEM(ptr)        Memory_free(ptr)

#define GLOBAL_CALLOC(nmemb, size) Memory_calloc(nmemb, size)
#define GLOBAL_MALLOC(size)        Memory_malloc(size)
#define GLOBAL_REALLOC(oldptr, size)   Memory_realloc(oldptr, size)
#define GLOBAL_FREEMEM(ptr)        Memory_free(ptr)

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

typedef void
(*MemoryExceptionHandler) (void* parameter);

PAL_API void
Memory_installExceptionHandler(MemoryExceptionHandler handler, void* parameter);

PAL_API void*
Memory_malloc(size_t size);

PAL_API void*
Memory_calloc(size_t nmemb, size_t size);

PAL_API void *
Memory_realloc(void *ptr, size_t size);

PAL_API void
Memory_free(void* memb);

#ifdef __cplusplus
}
#endif

#endif /* MEMORY_H_ */
