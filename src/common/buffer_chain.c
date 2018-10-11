/*
 *  buffer_chain.c
 *
 *  Copyright 2013 Michael Zillgith
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

#include "libiec61850_platform_includes.h"

#include "buffer_chain.h"

void
BufferChain_init(BufferChain self, int length, int partLength, BufferChain nextPart, uint8_t* buffer)
{
    self->length = length;
    self->partLength = partLength;
    self->partMaxLength = partLength;
    self->nextPart = nextPart;
    self->buffer = buffer;
}

void
MemoryArea_initialize(MemoryArea* self, uint8_t* memory, int size)
{
    self->memory = memory;
    self->size = size;
    self->currentPos = 0;
}

uint8_t*
MemoryArea_getNextBlock(MemoryArea* self, int size)
{
    if ((self->size - self->currentPos) >= size) {
        uint8_t* newBlock = self->memory + self->currentPos;

        self->currentPos += size;
        return newBlock;
    }
    else
        return NULL;

}
