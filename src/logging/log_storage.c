/*
 *  log_storage.c
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

#include "logging_api.h"


void
LogStorage_setMaxLogEntries(LogStorage self, int maxEntries)
{
    self->maxLogEntries = maxEntries;
}

int
LogStorage_getMaxLogEntries(LogStorage self)
{
    return self->maxLogEntries;
}

uint64_t
LogStorage_addEntry(LogStorage self, uint64_t timestamp)
{
    return self->addEntry(self, timestamp);
}

bool
LogStorage_addEntryData(LogStorage self, uint64_t entryID, const char* dataRef, uint8_t* data, int dataSize, uint8_t reasonCode)
{
    return self->addEntryData(self, entryID, dataRef, data, dataSize, reasonCode);
}

bool
LogStorage_getEntries(LogStorage self, uint64_t startingTime, uint64_t endingTime,
        LogEntryCallback entryCallback, LogEntryDataCallback entryDataCallback, void* parameter)
{
    return self->getEntries(self, startingTime, endingTime, entryCallback, entryDataCallback, parameter);
}

bool
LogStorage_getEntriesAfter(LogStorage self, uint64_t startingTime, uint64_t entryID,
        LogEntryCallback entryCallback, LogEntryDataCallback entryDataCallback, void* parameter)
{
    return self->getEntriesAfter(self, startingTime, entryID, entryCallback, entryDataCallback, parameter);
}

void
LogStorage_destroy(LogStorage self)
{
    self->destroy(self);
}


bool
LogStorage_getOldestAndNewestEntries(LogStorage self, uint64_t* newEntry, uint64_t* newEntryTime,
        uint64_t* oldEntry, uint64_t* oldEntryTime)
{
    return self->getOldestAndNewestEntries(self, newEntry, newEntryTime, oldEntry, oldEntryTime);
}

