/*
 *  lib_memory.c
 *
 *  Copyright 2014-2021 Michael Zillgith
 *
 *  This file is part of Platform Abstraction Layer (libpal)
 *  for libiec61850, libmms, and lib60870.
 */

#include <stdlib.h>
#include "lib_memory.h"

static MemoryExceptionHandler exceptionHandler = NULL;
static void* exceptionHandlerParameter = NULL;

static void
noMemoryAvailableHandler(void)
{
    if (exceptionHandler != NULL)
        exceptionHandler(exceptionHandlerParameter);
}

void
Memory_installExceptionHandler(MemoryExceptionHandler handler, void* parameter)
{
    exceptionHandler = handler;
    exceptionHandlerParameter = parameter;
}

void*
Memory_malloc(size_t size)
{
    void* memory = malloc(size);

    if (memory == NULL)
        noMemoryAvailableHandler();

    return memory;
}

void*
Memory_calloc(size_t nmemb, size_t size)
{
    void* memory = calloc(nmemb, size);

    if (memory == NULL)
        noMemoryAvailableHandler();

    return memory;
}

void *
Memory_realloc(void *ptr, size_t size)
{
    void* memory = realloc(ptr, size);

    if (memory == NULL)
        noMemoryAvailableHandler();

    return memory;
}

void
Memory_free(void* memb)
{
    free(memb);
}

