/*
 *  log_storage_sqlite.c
 *
 */


#include "logging_api.h"


static bool
SqLiteLogStorage_initializeLog(const char* logName, int logSize);

static bool
SqLiteLogStorage_storeEntry(const char* logName, uint64_t entryID, MmsValue* timestamp,
        int entrySize, uint8_t* entryData);

static bool
SqLiteLogStorage_getEntries(const char* logName, MmsValue* timestamp,
        LogEntryCallback callback, void* parameter);

static bool
SqLiteLogStorage_getEntriesAfter(const char* logName, MmsValue* timestamp, uint64_t entryID,
        LogEntryCallback callback, void* parameter);

static struct sLogStorage logStorageInstance = {
        SqLiteLogStorage_initializeLog,
        SqLiteLogStorage_storeEntry,
        SqLiteLogStorage_getEntries,
        SqLiteLogStorage_getEntriesAfter
};

LogStorage
SqLiteStorage_createInstance()
{
    return &logStorageInstance;
}

static bool
SqLiteLogStorage_initializeLog(const char* logName, int logSize)
{
    return true;
}

static bool
SqLiteLogStorage_storeEntry(const char* logName, uint64_t entryID, MmsValue* timestamp,
        int entrySize, uint8_t* entryData)
{
    return true;
}

static bool
SqLiteLogStorage_getEntries(const char* logName, MmsValue* timestamp,
        LogEntryCallback callback, void* parameter)
{
    return true;
}

static bool
SqLiteLogStorage_getEntriesAfter(const char* logName, MmsValue* timestamp, uint64_t entryID,
        LogEntryCallback callback, void* parameter)
{
    return true;
}




