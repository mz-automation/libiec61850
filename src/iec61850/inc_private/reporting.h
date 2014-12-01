/*
 *  reporting.h
 *
 *  Copyright 2013, 2014 Michael Zillgith
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
    uint8_t flags; /* bit 0 (1 = isIntegrityReport), bit 1 (1 = isGiReport) */
    int entryLength;
    ReportBufferEntry* next;
};

typedef struct {
    uint8_t* memoryBlock;
    int memoryBlockSize;
    int reportsCount;     /* number of reports currently enqueued */
    ReportBufferEntry* oldestReport;
    ReportBufferEntry* lastEnqueuedReport;
    ReportBufferEntry* nextToTransmit;
} ReportBuffer;

typedef struct {
    char* name;
    MmsDomain* domain;

    LogicalNode* parentLN;

    MmsValue* rcbValues;
    MmsValue* inclusionField;
    MmsValue* confRev;
    DataSet* dataSet;
    bool isDynamicDataSet;
    bool enabled;
    bool reserved;
    bool buffered; /* true if report is a buffered report */

    MmsValue** bufferedDataSetValues; /* used to buffer values during bufTm time */

    MmsValue** valueReferences; /* array to store value references for fast access */

    bool gi;

    uint16_t sqNum;
    uint32_t intgPd;
    uint32_t bufTm;
    uint64_t nextIntgReportTime;
    uint64_t reservationTimeout;
    MmsServerConnection* clientConnection;

    uint64_t lastEntryId;

    int triggerOps;

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore createNotificationsMutex;  /* { covered by mutex } */
#endif

    ReportInclusionFlag* inclusionFlags; /* { covered by mutex } */
    bool triggered;                      /* { covered by mutex } */
    uint64_t reportTime;                 /* { covered by mutex } */

    /* the following members are only required for buffered RCBs */
    bool isBuffering; /* true if buffered RCB is buffering (datSet is set to a valid value) */
    bool isResync; /* true if buffered RCB is in resync state */
    ReportBuffer* reportBuffer;
    MmsValue* timeOfEntry;
} ReportControl;

ReportControl*
ReportControl_create(bool buffered, LogicalNode* parentLN);

void
ReportControl_destroy(ReportControl* self);

void
ReportControl_valueUpdated(ReportControl* self, int dataSetEntryIndex, ReportInclusionFlag flag, MmsValue* value);

MmsValue*
ReportControl_getRCBValue(ReportControl* rc, char* elementName);

MmsVariableSpecification*
Reporting_createMmsBufferedRCBs(MmsMapping* self, MmsDomain* domain,
        LogicalNode* logicalNode, int reportsCount);

MmsVariableSpecification*
Reporting_createMmsUnbufferedRCBs(MmsMapping* self, MmsDomain* domain,
        LogicalNode* logicalNode, int reportsCount);

MmsDataAccessError
Reporting_RCBWriteAccessHandler(MmsMapping* self, ReportControl* rc, char* elementName, MmsValue* value,
        MmsServerConnection* connection);

void
Reporting_activateBufferedReports(MmsMapping* self);

void
Reporting_processReportEvents(MmsMapping* self, uint64_t currentTimeInMs);

void
Reporting_deactivateReportsForConnection(MmsMapping* self, MmsServerConnection* connection);

#endif /* REPORTING_H_ */
