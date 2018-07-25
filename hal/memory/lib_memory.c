/*
 *  lib_memory.c
 *
 *  Copyright 2014-2018 Michael Zillgith
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

