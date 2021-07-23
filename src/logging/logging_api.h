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

#ifdef __cplusplus
extern "C" {
#endif

#include "libiec61850_common_api.h"


/** \addtogroup server_api_group
 *  @{
 */

/**
 * @defgroup LOGGING_SPI Service provider interface (SPI) for log storage implementations
 *
 * This interface has to be implemented by the log storage provider. The Log storage provider
 * has to provide a specific constructor that creates an instance of LogStorage by allocating
 * the required memory for the struct sLogStorage data structure and populate the function
 * pointers with provider specific implementation functions.
 *
 * @{
 */

/** The LogStorage object handle */
typedef struct sLogStorage* LogStorage;

/**
 * \brief Will be called for each new LogEntry by the getEntries and getEntriesAfter functions
 *
 * \param parameter - a user provided parameter that is passed to the callback handler
 * \param timestamp - the entry timestamp of the LogEntry
 * \param entryID - the entryID of the LogEntry
 * \param moreFollow - more data will follow - if false, the data is not valid and no more data will follow
 *
 * \return true ready to receive new data, false abort operation
 */
typedef bool (*LogEntryCallback) (void* parameter, uint64_t timestamp, uint64_t entryID, bool moreFollow);

/**
 * \brief Will be called for each new LogEntryData by the getEntries and getEntriesAfter functions
 *
 * \param parameter - a user provided parameter that is passed to the callback handler
 * \param dataRef - the data reference of the LogEntryData
 * \param data - the data content as an unstructured binary data block
 * \param dataSize - the size of the binary data block
 * \param reasonCode - the reasonCode of the LogEntryData
 * \param moreFollow - more data will follow - if false, the data is not valid and no more data will follow
 *
 * \return true ready to receive new data, false abort operation
 */
typedef bool (*LogEntryDataCallback) (void* parameter, const char* dataRef, uint8_t* data, int dataSize, uint8_t reasonCode, bool moreFollow);

struct sLogStorage {

    void* instanceData;

    int maxLogEntries;

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


/**
 * \brief Set the maximum number of log entries for this log
 *
 * \param self the pointer of the LogStorage instance
 * \param maxEntries the maximum number of log entries
 */
LIB61850_API void
LogStorage_setMaxLogEntries(LogStorage self, int maxEntries);

/**
 * \brief Get the maximum allowed number of log entries for this log
 *
 * \param self the pointer of the LogStorage instance
 *
 * \return the maximum number of log entries
 */
LIB61850_API int
LogStorage_getMaxLogEntries(LogStorage self);

/**
 * \brief Add an entry to the log
 *
 * \param self the pointer of the LogStorage instance
 * \param timestamp the entry time of the new entry
 *
 * \return the entryID of the new entry
 */
LIB61850_API uint64_t
LogStorage_addEntry(LogStorage self, uint64_t timestamp);

/**
 * \brief Add new entry data to an existing log entry
 *
 * \param self the pointer of the LogStorage instance
 * \param entryID the ID of the log entry where the data will be added
 * \param dataRef the data reference of the log entry data
 * \param data the data content as an unstructured binary data block
 * \param dataSize - the size of the binary data block
 * \param reasonCode - the reasonCode of the LogEntryData
 *
 * \return true if the entry data was successfully added, false otherwise
 */
LIB61850_API bool
LogStorage_addEntryData(LogStorage self, uint64_t entryID, const char* dataRef, uint8_t* data, int dataSize, uint8_t reasonCode);

/**
 * \brief Get log entries specified by a time range
 *
 * \param self the pointer of the LogStorage instance
 * \param startingTime start time of the time range
 * \param endingTime end time of the time range
 * \param entryCallback callback function to be called for each new log entry
 * \param entryDataCallback callback function to be called for each new log entry data
 * \param parameter - a user provided parameter that is passed to the callback handler
 *
 * \return true if the request has been successful, false otherwise
 */
LIB61850_API bool
LogStorage_getEntries(LogStorage self, uint64_t startingTime, uint64_t endingTime,
        LogEntryCallback entryCallback, LogEntryDataCallback entryDataCallback, void* parameter);

/**
 * \brief Get log entries specified by a start log entry
 *
 * The request will return all log entries that where entered into the log after the
 * log entry specified by startingTime and entryID.
 *
 * \param self the pointer of the LogStorage instance
 * \param startingTime timestamp of the start log entry
 * \param entryID entryID of the start log entry
 * \param entryCallback callback function to be called for each new log entry
 * \param entryDataCallback callback function to be called for each new log entry data
 * \param parameter - a user provided parameter that is passed to the callback handler
 *
 * \return true if the request has been successful, false otherwise
 */
LIB61850_API bool
LogStorage_getEntriesAfter(LogStorage self, uint64_t startingTime, uint64_t entryID,
        LogEntryCallback entryCallback, LogEntryDataCallback entryDataCallback, void* parameter);

/**
 * \brief Get the entry time and entryID of the oldest and the newest log entries
 *
 * This function is used to update the log status information in the LCB
 *
 * \param self the pointer of the LogStorage instance
 * \param newEntry pointer to store the entryID of the newest entry
 * \param newEntryTime pointer to store the entry time of the newest entry
 * \param oldEntry pointer to store the entryID of the oldest entry
 * \param oldEntryTime pointer to store the entry time of the oldest entry
 *
 */
LIB61850_API bool
LogStorage_getOldestAndNewestEntries(LogStorage self, uint64_t* newEntry, uint64_t* newEntryTime,
        uint64_t* oldEntry, uint64_t* oldEntryTime);

/**
 * \brief Destroy the LogStorage instance and free all related resources
 *
 * \param self the pointer of the LogStorage instance
 */
LIB61850_API void
LogStorage_destroy(LogStorage self);

/**@}*/

/**@}*/

#ifdef __cplusplus
}
#endif

#endif /* LIBIEC61850_SRC_LOGGING_LOGGING_API_H_ */
