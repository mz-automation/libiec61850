/*
 *  logging_api.h
 *
 *  Copyright 2016 Michael Zillgith
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

#ifndef LIBIEC61850_SRC_LOGGING_LOGGING_API_H_
#define LIBIEC61850_SRC_LOGGING_LOGGING_API_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct sLogStorage* LogStorage;

/**
 *
 * \param moreFollow - more data will follow - if false, the data is not valid
 *
 * \return true ready to receive new data, false abort operation
 */
typedef bool (*LogEntryCallback) (void* parameter, uint64_t timestamp, uint64_t entryID, bool moreFollow);

typedef bool (*LogEntryDataCallback) (void* parameter, const char* dataRef, const uint8_t* data, int dataSize, uint8_t reasonCode, bool moreFollow);

struct sLogStorage {

    void* instanceData;

    uint64_t (*addEntry) (LogStorage self, uint64_t timestamp);

    bool (*addEntryData) (LogStorage self, uint64_t entryID, const char* dataRef, uint8_t* data, int dataSize, uint8_t reasonCode);

    bool (*getEntries) (LogStorage self, uint64_t startingTime, uint64_t endingTime,
            LogEntryCallback entryCallback, LogEntryDataCallback entryDataCallback, void* parameter);

    bool (*getEntriesAfter) (LogStorage self, uint64_t startingTime, uint64_t entryID,
            LogEntryCallback entryCallback, LogEntryDataCallback entryDataCallback, void* parameter);

    bool (*getOldestAndNewestEntries) (LogStorage self, uint64_t* newEntry, uint64_t* newEntryTime,
            uint64_t* oldEntry, uint64_t* oldEntryTime);

    void (*destroy) (LogStorage self);
};



uint64_t
LogStorage_addEntry(LogStorage self, uint64_t timestamp);

bool
LogStorage_addEntryData(LogStorage self, uint64_t entryID, const char* dataRef, uint8_t* data, int dataSize, uint8_t reasonCode);

bool
LogStorage_getEntries(LogStorage self, uint64_t startingTime, uint64_t endingTime,
        LogEntryCallback entryCallback, LogEntryDataCallback entryDataCallback, void* parameter);

bool
LogStorage_getEntriesAfter(LogStorage self, uint64_t startingTime, uint64_t entryID,
        LogEntryCallback entryCallback, LogEntryDataCallback entryDataCallback, void* parameter);

bool
LogStorage_getOldestAndNewestEntries(LogStorage self, uint64_t* newEntry, uint64_t* newEntryTime,
        uint64_t* oldEntry, uint64_t* oldEntryTime);

void
LogStorage_destroy(LogStorage self);

#endif /* LIBIEC61850_SRC_LOGGING_LOGGING_API_H_ */
