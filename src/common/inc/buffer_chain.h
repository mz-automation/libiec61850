/*
 *  buffer_chain.h
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

#ifndef BUFFER_CHAIN_H_
#define BUFFER_CHAIN_H_

#include "libiec61850_platform_includes.h"

typedef struct sBufferChain* BufferChain;

struct sBufferChain {
    int length;
    int partLength;
    int partMaxLength;
    uint8_t* buffer;
    BufferChain nextPart;
};

LIB61850_INTERNAL void
BufferChain_init(BufferChain self, int length, int partLength, BufferChain nextPart, uint8_t* buffer);

#endif /* BUFFER_CHAIN_H_ */
