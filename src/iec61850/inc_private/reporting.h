/*
 *  reporting.h
 *
 *  Copyright 2013-2019 Michael Zillgith
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

#ifndef REPORTING_H_
#define REPORTING_H_

typedef struct sReportBufferEntry ReportBufferEntry;

struct sReportBufferEntry {
    uint8_t entryId[8];
    uint64_t timeOfEntry;
    int entryLength:30;
    unsigned int flags:2; /* bit 0 (1 = isIntegrityReport), bit 1 (1 = isGiReport) */
    ReportBufferEntry* next;
};

typedef struct {
    uint8_t* memoryBlock;
    int memoryBlockSize;
    int reportsCount;     /* number of reports currently enqueued */
    ReportBufferEntry* oldestReport;
    ReportBufferEntry* lastEnqueuedReport;
    ReportBufferEntry* nextToTransmit;
    bool isOverflow; /* true if overflow condition is active */

    Semaphore lock; /* protect access to report buffer */
} ReportBuffer;

typedef struct {
    char* name;
    MmsDomain* domain;

    LogicalNode* parentLN;

    MmsValue* rcbValues;

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore rcbValuesLock;
#endif

    MmsValue* inclusionField;

    DataSet* dataSet;
    bool isDynamicDataSet;
    bool enabled;
    bool reserved;
    bool buffered; /* true if report is a buffered report */

    MmsValue** bufferedDataSetValues; /* used to buffer values during bufTm time */

    MmsValue** valueReferences; /* array to store value references for fast access */

    bool gi; /* flag to indicate that a GI report is triggered */

    uint16_t sqNum;
    uint32_t intgPd; /* integrity period in ms */
    uint32_t bufTm;
    uint64_t nextIntgReportTime; /* time when to send next integrity report */
    uint64_t reservationTimeout;
    MmsServerConnection clientConnection;

    uint64_t lastEntryId;

    int triggerOps;

    bool hasOwner; /* indicate that owner field is present */

    /* information for segmented reporting */
    bool segmented; /* indicates that a segmented report is in process */
    int startIndexForNextSegment; /* start data set index for the next report segment */
    MmsValue* subSeqVal; /* sub sequence value for segmented reporting */
    uint64_t segmentedReportTimestamp; /* time stamp used for all report segments */

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore createNotificationsMutex;  /* { covered by mutex } */
#endif

    uint8_t* inclusionFlags; /* { covered by mutex } */
    bool triggered;                      /* { covered by mutex } */
    uint64_t reportTime;                 /* { covered by mutex } */

    /*
     * the following members are only required for buffered RCBs *
     */

    bool isBuffering; /* true if buffered RCB is buffering (datSet is set to a valid value) */
    bool isResync; /* true if buffered RCB is in resync state */
    int resvTms; /* -1 for preconfigured client, 0 - not reserved, > 0 reserved by client */

    ReportBuffer* reportBuffer;
    MmsValue* timeOfEntry;

    ReportControlBlock* rcb;
    ReportControlBlock* sibling; /* backup sibling field of original ReportControlBlock */

    IedServer server;
} ReportControl;

LIB61850_INTERNAL ReportControl*
ReportControl_create(bool buffered, LogicalNode* parentLN, int reportBufferSize, IedServer server);

LIB61850_INTERNAL void
ReportControl_destroy(ReportControl* self);

LIB61850_INTERNAL void
ReportControl_valueUpdated(ReportControl* self, int dataSetEntryIndex, int flag, bool modelLocked);

LIB61850_INTERNAL MmsValue*
ReportControl_getRCBValue(ReportControl* rc, const char* elementName);

LIB61850_INTERNAL MmsVariableSpecification*
Reporting_createMmsBufferedRCBs(MmsMapping* self, MmsDomain* domain,
        LogicalNode* logicalNode, int reportsCount);

LIB61850_INTERNAL MmsVariableSpecification*
Reporting_createMmsUnbufferedRCBs(MmsMapping* self, MmsDomain* domain,
        LogicalNode* logicalNode, int reportsCount);

LIB61850_INTERNAL MmsDataAccessError
Reporting_RCBWriteAccessHandler(MmsMapping* self, ReportControl* rc, const char* elementName, MmsValue* value,
        MmsServerConnection connection);

LIB61850_INTERNAL bool
ReportControl_readAccess(ReportControl* rc, MmsMapping* mmsMapping, MmsServerConnection connection, char* elementName);

LIB61850_INTERNAL void
Reporting_activateBufferedReports(MmsMapping* self);

/* periodic check if reports have to be sent */
LIB61850_INTERNAL void
Reporting_processReportEvents(MmsMapping* self, uint64_t currentTimeInMs);

/* check if report have to be sent after data model update */
LIB61850_INTERNAL void
Reporting_processReportEventsAfterUnlock(MmsMapping* self);

/* send reports in report buffer */
LIB61850_INTERNAL void
Reporting_sendReports(MmsMapping* self, MmsServerConnection connection);

LIB61850_INTERNAL void
Reporting_deactivateAllReports(MmsMapping* self);

LIB61850_INTERNAL void
Reporting_deactivateReportsForConnection(MmsMapping* self, MmsServerConnection connection);

#endif /* REPORTING_H_ */
