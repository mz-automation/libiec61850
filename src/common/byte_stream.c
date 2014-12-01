/*
 *  byte_stream.c
 *
 *  Some helper functions to read from sockets
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
#include "stack_config.h"

#include "byte_stream.h"
#include "socket.h"

int
ByteStream_readUint8(Socket self, uint8_t* byte)
{
    int bytesRead;

    uint64_t start = Hal_getTimeInMs();

    do {
        bytesRead = Socket_read(self, byte, 1);

        if (bytesRead == -1) break;

    } while ((bytesRead < 1) && ((Hal_getTimeInMs() - start) < CONFIG_TCP_READ_TIMEOUT_MS));

    if (bytesRead != 1) {
        return -1;
    }

    return 1;
}

int
ByteStream_readUint16(Socket self, uint16_t* value)
{
    uint8_t byte[2];
    int bytes_read = 0;

    uint64_t start = Hal_getTimeInMs();

    do {
        int readVal = Socket_read(self, byte + bytes_read, 2);

        if (readVal == -1) break;

        bytes_read += readVal;
    } while ((bytes_read < 2)
            && ((Hal_getTimeInMs() - start) < CONFIG_TCP_READ_TIMEOUT_MS));

    if (bytes_read != 2)
        return -1;

    *value = (byte[0] * 0x100) + byte[1];
    return 2;
}

int
ByteStream_skipBytes(Socket self, int number)
{
    int c = 0;
    uint8_t byte;

    uint64_t start = Hal_getTimeInMs();

    do {
        int readBytes = Socket_read(self, &byte, 1);

        if (readBytes < 0)
            return -1;
        else
            c = c + readBytes;

    } while ((c < number)
            && ((Hal_getTimeInMs() - start) < CONFIG_TCP_READ_TIMEOUT_MS));

    return c;
}

int
ByteStream_readOctets(Socket self, uint8_t* buffer, int size)
{
    int readBytes = 0;
    int remainingSize = size;

    uint64_t start = Hal_getTimeInMs();

    do {
        int chunkSize = Socket_read(self, buffer + readBytes, remainingSize);
        if (chunkSize < 0)
            return -1;
        else
        {
            readBytes += chunkSize;
            remainingSize = size - readBytes;
        }
    } while ((readBytes < size)
            && ((Hal_getTimeInMs() - start) < CONFIG_TCP_READ_TIMEOUT_MS));

    return readBytes;
}

