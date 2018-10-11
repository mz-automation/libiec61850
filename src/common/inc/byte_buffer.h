/*
 *  byte_buffer.h
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

#ifndef BYTE_BUFFER_H_
#define BYTE_BUFFER_H_

#include "libiec61850_common_api.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint8_t* buffer;
    int maxSize;
    int size;
} ByteBuffer;

LIB61850_INTERNAL ByteBuffer*
ByteBuffer_create(ByteBuffer* self, int maxSize);

LIB61850_INTERNAL void
ByteBuffer_destroy(ByteBuffer* self);

LIB61850_INTERNAL void
ByteBuffer_wrap(ByteBuffer* self, uint8_t* buf, int size, int maxSize);

LIB61850_INTERNAL int
ByteBuffer_append(ByteBuffer* self, uint8_t* data, int dataSize);

LIB61850_INTERNAL int
ByteBuffer_appendByte(ByteBuffer* self, uint8_t byte);

LIB61850_INTERNAL uint8_t*
ByteBuffer_getBuffer(ByteBuffer* self);

LIB61850_INTERNAL int
ByteBuffer_getSize(ByteBuffer* self);

LIB61850_INTERNAL int
ByteBuffer_getMaxSize(ByteBuffer* self);

LIB61850_INTERNAL int
ByteBuffer_setSize(ByteBuffer* self, int size);

LIB61850_INTERNAL void
ByteBuffer_print(ByteBuffer* self, char* message);

#ifdef __cplusplus
}
#endif


#endif /* BYTE_BUFFER_H_ */
