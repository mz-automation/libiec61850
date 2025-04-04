/*
 *  log_storage_sqlite.c
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

#include "sqlite3.h"
#include <stdio.h>

#ifndef DEBUG_LOG_STORAGE_DRIVER
#define DEBUG_LOG_STORAGE_DRIVER 0
#endif

static uint64_t
SqliteLogStorage_addEntry(LogStorage self, uint64_t timestamp);

static bool
SqliteLogStorage_addEntryData(LogStorage self, uint64_t entryID, const char* dataRef, uint8_t* data, int dataSize, uint8_t reasonCode);

static bool
SqliteLogStorage_getEntries(LogStorage self, uint64_t startingTime, uint64_t endingTime,
        LogEntryCallback entryCallback, LogEntryDataCallback entryDataCallback, void* parameter);

static bool
SqliteLogStorage_getEntriesAfter(LogStorage self, uint64_t startingTime, uint64_t entryID,
        LogEntryCallback entryCallback, LogEntryDataCallback entryDataCallback, void* parameter);

static bool
SqliteLogStorage_getOldestAndNewestEntries(LogStorage self, uint64_t* newEntry, uint64_t* newEntryTime,
        uint64_t* oldEntry, uint64_t* oldEntryTime);

static void
SqliteLogStorage_destroy(LogStorage self);


typedef struct sSqliteLogStorage {
    char* filename;
    sqlite3* db;
    sqlite3_stmt* insertEntryStmt;
    sqlite3_stmt* insertEntryDataStmt;
    sqlite3_stmt* getEntriesWithRange;
    sqlite3_stmt* getEntriesAfter;
    sqlite3_stmt* getEntryData;
    sqlite3_stmt* getOldEntry;
    sqlite3_stmt* getNewEntry;
    sqlite3_stmt* getEntriesCount;
    sqlite3_stmt* deleteEntryData;
    sqlite3_stmt* deleteEntry;
} SqliteLogStorage;

static const char* CREATE_TABLE_ENTRYS = "create table if not exists Entries (entryID integer primary key, timeOfEntry integer)";
static const char* CREATE_TABLE_ENTRY_DATA = "create table if not exists EntryData (entryID integer, dataRef text, value blob, reasonCode integer)";
static const char* INSERT_ENTRY = "insert into Entries (timeOfEntry) values (?)";
static const char* INSERT_ENTRY_DATA = "insert into EntryData (entryID, dataRef, value, reasonCode) values (?,?,?,?)";
static const char* GET_ENTRIES_WITH_RANGE = "select entryID, timeOfEntry from Entries where timeOfEntry >= ? and timeOfEntry <= ?";
static const char* GET_ENTRIES_AFTER = "select entryID, timeOfEntry from Entries where entryID > ?";
static const char* GET_ENTRY_DATA = "select dataRef, value, reasonCode from EntryData where entryID = ?";

static const char* GET_OLD_ENTRY = "select * from Entries where entryID = (select min(entryID) from Entries where timeOfEntry = (select min(timeOfEntry) from Entries))";
static const char* GET_NEW_ENTRY = "select * from Entries where entryID = (select max(entryID) from Entries where timeOfEntry = (select max(timeOfEntry) from Entries))";

static const char* GET_ENTRIES_COUNT = "select Count(*) from Entries";

static const char* DELETE_ENTRY_DATA = "delete from EntryData where entryID=?";
static const char* DELETE_ENTRY = "delete from Entries where entryID=?";

static char*
copyStringInternal(const char* string)
{
    int newStringLength = strlen(string) + 1;

    char* newString = (char*) malloc(newStringLength);

    memcpy(newString, string, newStringLength);

    return newString;
}

LogStorage
SqliteLogStorage_createInstance(const char* filename)
{
    sqlite3* db = NULL;
    sqlite3_stmt* insertEntryStmt = NULL;
    sqlite3_stmt* insertEntryDataStmt = NULL;
    sqlite3_stmt* getEntriesWithRange = NULL;
    sqlite3_stmt* getEntriesAfter = NULL;
    sqlite3_stmt* getEntryData = NULL;
    sqlite3_stmt* getOldEntry = NULL;
    sqlite3_stmt* getNewEntry = NULL;
    sqlite3_stmt* getEntriesCount = NULL;
    sqlite3_stmt* deleteEntryData = NULL;
    sqlite3_stmt* deleteEntry = NULL;

    char *zErrMsg = 0;

    int rc = sqlite3_open(filename, &db);

    if (rc != SQLITE_OK)
       goto exit_with_error;

    rc = sqlite3_exec(db, CREATE_TABLE_ENTRYS, NULL, 0, &zErrMsg);
    if (rc != SQLITE_OK)
        goto exit_with_error;

    rc = sqlite3_exec(db, CREATE_TABLE_ENTRY_DATA, NULL, 0, &zErrMsg);
    if (rc != SQLITE_OK)
        goto exit_with_error;

    rc = sqlite3_prepare(db, INSERT_ENTRY, -1, &insertEntryStmt, NULL);
    if (rc != SQLITE_OK)
        goto exit_with_error;

    rc = sqlite3_prepare(db, INSERT_ENTRY_DATA, -1, &insertEntryDataStmt, NULL);
    if (rc != SQLITE_OK)
        goto exit_with_error;

    rc = sqlite3_prepare_v2(db, GET_ENTRIES_WITH_RANGE, -1, &getEntriesWithRange, NULL);
    if (rc != SQLITE_OK)
        goto exit_with_error;

    rc = sqlite3_prepare_v2(db, GET_ENTRIES_AFTER, -1, &getEntriesAfter, NULL);
    if (rc != SQLITE_OK)
        goto exit_with_error;

    rc = sqlite3_prepare_v2(db, GET_ENTRY_DATA, -1, &getEntryData, NULL);
    if (rc != SQLITE_OK)
        goto exit_with_error;

    rc = sqlite3_prepare_v2(db, GET_OLD_ENTRY, -1, &getOldEntry, NULL);
    if (rc != SQLITE_OK)
        goto exit_with_error;

    rc = sqlite3_prepare_v2(db, GET_NEW_ENTRY, -1, &getNewEntry, NULL);
    if (rc != SQLITE_OK)
        goto exit_with_error;

    rc = sqlite3_prepare_v2(db, GET_ENTRIES_COUNT, -1, &getEntriesCount, NULL);
    if (rc != SQLITE_OK)
        goto exit_with_error;

    rc = sqlite3_prepare_v2(db, DELETE_ENTRY_DATA, -1, &deleteEntryData, NULL);
    if (rc != SQLITE_OK)
        goto exit_with_error;

    rc = sqlite3_prepare_v2(db, DELETE_ENTRY, -1, &deleteEntry, NULL);
    if (rc != SQLITE_OK)
        goto exit_with_error;

    LogStorage self = (LogStorage) calloc(1, sizeof(struct sLogStorage));

    SqliteLogStorage* instanceData = (SqliteLogStorage*) calloc(1, sizeof(struct sSqliteLogStorage));

    instanceData->filename = copyStringInternal(filename);
    instanceData->db = db;
    instanceData->insertEntryStmt = insertEntryStmt;
    instanceData->insertEntryDataStmt = insertEntryDataStmt;
    instanceData->getEntriesWithRange = getEntriesWithRange;
    instanceData->getEntriesAfter = getEntriesAfter;
    instanceData->getEntryData = getEntryData;
    instanceData->getOldEntry = getOldEntry;
    instanceData->getNewEntry = getNewEntry;
    instanceData->getEntriesCount = getEntriesCount;
    instanceData->deleteEntryData = deleteEntryData;
    instanceData->deleteEntry = deleteEntry;

    self->instanceData = (void*) instanceData;

    self->addEntry = SqliteLogStorage_addEntry;
    self->addEntryData = SqliteLogStorage_addEntryData;
    self->getEntries = SqliteLogStorage_getEntries;
    self->getEntriesAfter = SqliteLogStorage_getEntriesAfter;
    self->getOldestAndNewestEntries = SqliteLogStorage_getOldestAndNewestEntries;
    self->destroy = SqliteLogStorage_destroy;
    self->maxLogEntries = -1;

    return self;

exit_with_error:
    if (DEBUG_LOG_STORAGE_DRIVER)
        printf("LOG_STORAGE_DRIVER: sqlite - failed to create LogStorage instance!\n");

    return NULL;
}

static void
deleteOldestEntry(SqliteLogStorage* self)
{
    int rc;

    /* Get oldest entry */
    rc = sqlite3_reset(self->getOldEntry);

    if (rc != SQLITE_OK)
        goto exit_with_error;

    rc = sqlite3_step(self->getOldEntry);

    int64_t entryId;

    if (rc == SQLITE_ROW) {
        entryId = sqlite3_column_int64(self->getOldEntry, 0);

        sqlite3_reset(self->deleteEntryData);

        rc = sqlite3_bind_int64(self->deleteEntryData, 1, (sqlite_int64) entryId);

        rc = sqlite3_step(self->deleteEntryData);

        if (rc != SQLITE_DONE)
            goto exit_with_error;

        sqlite3_reset(self->deleteEntry);

        rc = sqlite3_bind_int64(self->deleteEntry, 1, (sqlite_int64) entryId);

        rc = sqlite3_step(self->deleteEntry);

        if (rc != SQLITE_DONE)
            goto exit_with_error;

    }
    else
        goto exit_with_error;

    return;

exit_with_error:
    if (DEBUG_LOG_STORAGE_DRIVER)
        printf("LOG_STORAGE_DRIVER: sqlite - failed to delete oldest entry (rc=%i)!\n", rc);

    return;
}

static int
getEntriesCount(SqliteLogStorage* self)
{
    int rc;

    rc = sqlite3_reset(self->getEntriesCount);

    if (rc != SQLITE_OK)
        goto exit_with_error;

    rc = sqlite3_step(self->getEntriesCount);

    int entryCount = sqlite3_column_int(self->getEntriesCount, 0);

    return entryCount;

exit_with_error:
    if (DEBUG_LOG_STORAGE_DRIVER)
        printf("LOG_STORAGE_DRIVER: sqlite - failed to get entry count! (rc=%i)\n", rc);

    return -1;
}

static void
trimToMaxEntries(SqliteLogStorage* self, int maxEntries)
{
    int deleteEntries = getEntriesCount(self) - maxEntries;

    int i;
    for (i = 0; i < deleteEntries; i++)
        deleteOldestEntry(self);
}

static uint64_t
SqliteLogStorage_addEntry(LogStorage self, uint64_t timestamp)
{
    if (DEBUG_LOG_STORAGE_DRIVER)
        printf("LOG_STORAGE_DRIVER: sqlite - add entry\n");

    SqliteLogStorage* instanceData = (SqliteLogStorage*) (self->instanceData);

    sqlite3* db = instanceData->db;
    int rc;

    rc = sqlite3_bind_int64(instanceData->insertEntryStmt, 1, (sqlite_int64) timestamp);

    if (rc != SQLITE_OK)
        goto exit_with_error;

    rc = sqlite3_step(instanceData->insertEntryStmt);

    if (rc != SQLITE_DONE)
        goto exit_with_error;

    uint64_t id = sqlite3_last_insert_rowid(db);

    if (DEBUG_LOG_STORAGE_DRIVER)
        printf("LOG_STORAGE_DRIVER: sqlite - new entry with ID = %lu\n", id);

    rc = sqlite3_reset(instanceData->insertEntryStmt);

    if (rc != SQLITE_OK)
        goto exit_with_error;

    if (self->maxLogEntries > 0) {
        if (getEntriesCount(instanceData) > self->maxLogEntries)
            trimToMaxEntries(instanceData, self->maxLogEntries);
    }

    return id;

exit_with_error:
    if (DEBUG_LOG_STORAGE_DRIVER)
        printf("LOG_STORAGE_DRIVER: sqlite - failed to add entry to log!\n");

    return 0;
}

static bool
SqliteLogStorage_addEntryData(LogStorage self, uint64_t entryID, const char* dataRef, uint8_t* data, int dataSize, uint8_t reasonCode)
{
    SqliteLogStorage* instanceData = (SqliteLogStorage*) (self->instanceData);

    int rc;

    rc = sqlite3_bind_int64(instanceData->insertEntryDataStmt, 1, (sqlite_int64) entryID);

    if (rc != SQLITE_OK)
        goto exit_with_error;

    rc = sqlite3_bind_text(instanceData->insertEntryDataStmt, 2, dataRef, -1, SQLITE_STATIC);

    if (rc != SQLITE_OK)
        goto exit_with_error;

    rc = sqlite3_bind_blob(instanceData->insertEntryDataStmt, 3, data, dataSize, SQLITE_STATIC);

    if (rc != SQLITE_OK)
        goto exit_with_error;

    rc = sqlite3_bind_int(instanceData->insertEntryDataStmt, 4, reasonCode);

    if (rc != SQLITE_OK)
        goto exit_with_error;

    rc = sqlite3_step(instanceData->insertEntryDataStmt);

    if (rc != SQLITE_DONE)
        goto exit_with_error;

    rc = sqlite3_reset(instanceData->insertEntryDataStmt);

    if (rc != SQLITE_OK)
        goto exit_with_error;

    return true;

exit_with_error:
    if (DEBUG_LOG_STORAGE_DRIVER)
        printf("LOG_STORAGE_DRIVER: sqlite - failed to add entry data!\n");

    return false;
}

static void
getEntryData(LogStorage self, uint64_t entryID, LogEntryDataCallback entryDataCallback, void* parameter)
{
    SqliteLogStorage* instanceData = (SqliteLogStorage*) (self->instanceData);

    int rc;

    rc = sqlite3_bind_int64(instanceData->getEntryData, 1, entryID);

    if (rc != SQLITE_OK) {
        if (DEBUG_LOG_STORAGE_DRIVER)
            printf("LOG_STORAGE_DRIVER: sqlite - getEntryData 1 rc:%i\n", rc);
    }

    bool sendFinalEvent = true;

    while ((rc = sqlite3_step(instanceData->getEntryData)) == SQLITE_ROW) {

        const char* dataRef = (const char*) sqlite3_column_text(instanceData->getEntryData, 0);
        uint8_t* data = (uint8_t*) sqlite3_column_blob(instanceData->getEntryData, 1);
        int dataSize = sqlite3_column_bytes(instanceData->getEntryData, 1);
        int reasonCode = sqlite3_column_int(instanceData->getEntryData, 2);

        if (entryDataCallback != NULL) {
            if (entryDataCallback(parameter, dataRef, data, dataSize, (uint8_t) reasonCode, true) == false) {
                sendFinalEvent = false;
                break;
            }

        }
    }

    if (sendFinalEvent) {
        if (entryDataCallback != NULL)
            entryDataCallback(parameter, NULL, NULL, 0, (uint8_t) 0, false);
    }

    rc = sqlite3_reset(instanceData->getEntryData);

    if (rc != SQLITE_OK)
        if (DEBUG_LOG_STORAGE_DRIVER)
            printf("LOG_STORAGE_DRIVER: sqlite - getEntryData reset rc:%i\n", rc);
}

static bool
SqliteLogStorage_getEntries(LogStorage self, uint64_t startingTime, uint64_t endingTime,
        LogEntryCallback entryCallback, LogEntryDataCallback entryDataCallback, void* parameter)
{
    SqliteLogStorage* instanceData = (SqliteLogStorage*) (self->instanceData);

    int rc;

    rc = sqlite3_bind_int64(instanceData->getEntriesWithRange, 1, startingTime);

    if (rc != SQLITE_OK)
        if (DEBUG_LOG_STORAGE_DRIVER)
            printf("LOG_STORAGE_DRIVER: sqlite - SqliteLogStorage_getEntries 1 rc:%i\n", rc);

    rc = sqlite3_bind_int64(instanceData->getEntriesWithRange, 2, endingTime);

    if (rc != SQLITE_OK)
        if (DEBUG_LOG_STORAGE_DRIVER)
            printf("LOG_STORAGE_DRIVER: sqlite - SqliteLogStorage_getEntries 2 rc:%i\n", rc);

    bool sendFinalEvent = true;

    while((rc = sqlite3_step(instanceData->getEntriesWithRange)) == SQLITE_ROW) {

        uint64_t entryID = sqlite3_column_int64(instanceData->getEntriesWithRange, 0);
        uint64_t timestamp = sqlite3_column_int64(instanceData->getEntriesWithRange, 1);

        if (entryCallback != NULL) {
            if (entryCallback(parameter, timestamp, entryID, true) == false) {
                sendFinalEvent = false;
                break;
            }

        }

        getEntryData(self, entryID, entryDataCallback, parameter);
    }

    if (sendFinalEvent)
        if (entryCallback != NULL)
            entryCallback(parameter, 0, 0, false);


    rc = sqlite3_reset(instanceData->getEntriesWithRange);

    if (rc != SQLITE_OK)
        if (DEBUG_LOG_STORAGE_DRIVER)
            printf("LOG_STORAGE_DRIVER: sqlite - SqliteLogStorage_getEntries reset rc:%i\n", rc);

    return true;
}

static bool
SqliteLogStorage_getOldestAndNewestEntries(LogStorage self, uint64_t* newEntry, uint64_t* newEntryTime,
        uint64_t* oldEntry, uint64_t* oldEntryTime)
{
    bool validOldEntry = false;
    bool validNewEntry = false;

    SqliteLogStorage* instanceData = (SqliteLogStorage*) (self->instanceData);

    int rc;

    /* Get oldest entry */
    sqlite3_reset(instanceData->getOldEntry);

    rc = sqlite3_step(instanceData->getOldEntry);

    if (rc == SQLITE_ROW) {
        *oldEntry = sqlite3_column_int64(instanceData->getOldEntry, 0);
        *oldEntryTime = sqlite3_column_int64(instanceData->getOldEntry, 1);
        validNewEntry = true;
    }
    else {
        *oldEntry = 0;
        *oldEntryTime = 0;
    }


    /* Get newest entry */
    sqlite3_reset(instanceData->getNewEntry);

    rc = sqlite3_step(instanceData->getNewEntry);

    if (rc == SQLITE_ROW) {
        *newEntry = sqlite3_column_int64(instanceData->getNewEntry, 0);
        *newEntryTime = sqlite3_column_int64(instanceData->getNewEntry, 1);
        validOldEntry = true;
    }
    else {
        *newEntry = 0;
        *newEntryTime = 0;
    }

    return (validOldEntry && validNewEntry);
}

static bool
SqliteLogStorage_getEntriesAfter(LogStorage self, uint64_t startingTime, uint64_t entryID,
        LogEntryCallback entryCallback, LogEntryDataCallback entryDataCallback, void* parameter)
{
    SqliteLogStorage* instanceData = (SqliteLogStorage*) (self->instanceData);

    int rc;

    rc = sqlite3_bind_int64(instanceData->getEntriesAfter, 1, entryID);

    if (rc != SQLITE_OK)
        if (DEBUG_LOG_STORAGE_DRIVER)
            printf("LOG_STORAGE_DRIVER: sqlite - SqliteLogStorage_getEntriesAfter 1 rc:%i\n", rc);

    bool sendFinalEvent = true;

    while ((rc = sqlite3_step(instanceData->getEntriesAfter)) == SQLITE_ROW) {

        uint64_t entryID = sqlite3_column_int64(instanceData->getEntriesAfter, 0);
        uint64_t timestamp = sqlite3_column_int64(instanceData->getEntriesAfter, 1);

        if (entryCallback != NULL) {
            if (entryCallback(parameter, timestamp, entryID, true) == false) {
                sendFinalEvent = false;
                break;
            }
        }

        getEntryData(self, entryID, entryDataCallback, parameter);
    }

    if (sendFinalEvent)
        if (entryCallback != NULL)
            entryCallback(parameter, 0, 0, false);

    rc = sqlite3_reset(instanceData->getEntriesAfter);

    if (rc != SQLITE_OK)
        if (DEBUG_LOG_STORAGE_DRIVER)
            printf("LOG_STORAGE_DRIVER: sqlite - SqliteLogStorage_getEntriesAfter reset rc:%i\n", rc);

    return true;
}

static void
SqliteLogStorage_destroy(LogStorage self)
{
    SqliteLogStorage* instanceData = (SqliteLogStorage*) self->instanceData;

    sqlite3_finalize(instanceData->insertEntryStmt);
    sqlite3_finalize(instanceData->insertEntryDataStmt);
    sqlite3_finalize(instanceData->getEntriesWithRange);
    sqlite3_finalize(instanceData->getEntriesAfter);
    sqlite3_finalize(instanceData->getEntryData);
    sqlite3_finalize(instanceData->getOldEntry);
    sqlite3_finalize(instanceData->getNewEntry);
    sqlite3_finalize(instanceData->getEntriesCount);
    sqlite3_finalize(instanceData->deleteEntryData);
    sqlite3_finalize(instanceData->deleteEntry);

    if (sqlite3_close(instanceData->db) != SQLITE_OK)
        if (DEBUG_LOG_STORAGE_DRIVER)
            printf("LOG_STORAGE_DRIVER: sqlite - SqliteLogStorage: failed to close database %s!\n", instanceData->filename);

    free(instanceData->filename);
    free(instanceData);
    free(self);
}




