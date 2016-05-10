/*
 * logging_api.h
 *
 */

#ifndef LIBIEC61850_SRC_LOGGING_LOGGING_API_H_
#define LIBIEC61850_SRC_LOGGING_LOGGING_API_H_

#include <stdint.h>
#include <stdbool.h>
#include "mms_value.h"

typedef struct sLogStorage* LogStorage;

typedef void (*LogEntryCallback) (void* parameter, MmsValue* timestamp, uint64_t entryID, int entrySize,
        uint8_t* entryData);

struct sLogStorage {
    bool (*initializeLog) (const char* logName, int logSize);

    bool (*storeEntry) (const char* logName, uint64_t entryID, MmsValue* timestamp, int entrySize,
            uint8_t* entryData);

    bool (*getEntries) (const char* logName, MmsValue* timestamp,
            LogEntryCallback callback, void* parameter);

    bool (*getEntriesAfter) (const char* logName, MmsValue* timestamp, uint64_t entryID,
            LogEntryCallback callback, void* parameter);
};

#endif /* LIBIEC61850_SRC_LOGGING_LOGGING_API_H_ */
