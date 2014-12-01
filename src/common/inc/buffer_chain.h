/*
 * buffer_chain.h
 *
 *  Created on: Nov 10, 2013
 *      Author: mzillgit
 */

#ifndef BUFFER_CHAIN_H_
#define BUFFER_CHAIN_H_

typedef struct sBufferChain* BufferChain;

struct sBufferChain {
    int length;
    int partLength;
    int partMaxLength;
    uint8_t* buffer;
    BufferChain nextPart;
};

void
BufferChain_init(BufferChain self, int length, int partLength, BufferChain nextPart, uint8_t* buffer);

void
BufferChain_destroy(BufferChain self);

int /* returns the number of bytes written to the buffer */
BufferChain_dumpToBuffer(BufferChain self, uint8_t* buffer, int bufferMaxSize);


typedef struct {
    uint8_t* memory;
    int currentPos;
    int size;
} MemoryArea;

void
MemoryArea_initialize(MemoryArea* self, uint8_t* memory, int size);

uint8_t*
MemoryArea_getNextBlock(MemoryArea* self, int size);

#if 0

typedef struct sMemoryPool* MemoryPool;

typedef struct sMemoryChunk MemoryChunk;

struct sMemoryPool {
    uint8_t* memory;
    int size;
    MemoryChunk firstChunk;
};

struct sMemoryChunk {
    MemoryChunk previous;
    MemoryChunk next;
    uint8_t* data;
    uint8_t free;
    int size;
};

MemoryPool
MemoryPool_create(uint8_t* memoryAddress, int size);

void
MemoryPool_destroy(MemoryPool self);

MemoryChunk
MemoryPool_allocateChunk(MemoryPool self, int size);
MemoryPool_freeChunk(MemoryPool self, MemoryChunk chunk);

#endif

#endif /* BUFFER_CHAIN_H_ */
