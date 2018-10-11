/*
 *  simple_allocator.h
 *
 *  Copyright 2013-2018 Michael Zillgith
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

#ifndef SIMPLE_ALLOCATOR_H_
#define SIMPLE_ALLOCATOR_H_

typedef struct {
    char* memoryBlock;
    char* currentPtr;
    int size;
} MemoryAllocator;

LIB61850_INTERNAL void
MemoryAllocator_init(MemoryAllocator* self, char* memoryBlock, int size);

LIB61850_INTERNAL int
MemoryAllocator_getAlignedSize(int size);

LIB61850_INTERNAL char*
MemoryAllocator_allocate(MemoryAllocator* self, int size);

#endif /* SIMPLE_ALLOCATOR_H_ */
