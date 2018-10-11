/*
 *  mem_alloc_linked_list.h
 *
 *  Implementation of linked list (LinkedList) that uses the provided memory buffer as
 *  memory pool for storage allocation.
 *
 *  Copyright 2014 Michael Zillgith
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

#ifndef MEM_ALLOC_LINKED_LIST_H_
#define MEM_ALLOC_LINKED_LIST_H_

#include "libiec61850_common_api.h"
#include "linked_list.h"
#include "simple_allocator.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct sMemAllocLinkedList* MemAllocLinkedList;

LIB61850_INTERNAL MemAllocLinkedList
MemAllocLinkedList_create(MemoryAllocator* ma);

LIB61850_INTERNAL LinkedList
MemAllocLinkedList_add(MemAllocLinkedList list, void* data);

#endif /* MEM_ALLOC_LINKED_LIST_H_ */

#ifdef __cplusplus
}
#endif
