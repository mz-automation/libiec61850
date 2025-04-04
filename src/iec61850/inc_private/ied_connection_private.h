/*
 *  ied_connection_private.h
 *
 *  Copyright 2013-2024 Michael Zillgith
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

#ifndef IED_CONNECTION_PRIVATE_H_
#define IED_CONNECTION_PRIVATE_H_

#ifndef DEBUG_IED_CLIENT
#define DEBUG_IED_CLIENT 0
#endif

#include "iec61850_common_internal.h"

#include "hal_thread.h"

typedef struct sIedConnectionOutstandingCall* IedConnectionOutstandingCall;

struct sIedConnectionOutstandingCall
{
    bool used;
    uint32_t invokeId;
    void* callback;
    void* callbackParameter;
    void* specificParameter; /* function/service specific parameter */

    union
    {
        void* pointer;
        struct {
            uint32_t originalInvokeId;
        } getFileInfo;
        struct {
            bool cont;
        } getFileDirectory;
    } specificParameter2; /* function/service specific parameter */
};

struct sIedConnection
{
    MmsConnection connection;
    IedConnectionState state;
    LinkedList enabledReports;
    LinkedList logicalDevices;

    Semaphore clientControlsLock;
    LinkedList clientControls;

    LastApplError lastApplError;

    Semaphore stateMutex;
    Semaphore reportHandlerMutex;

    Semaphore outstandingCallsLock;
    IedConnectionOutstandingCall outstandingCalls;
    int maxOutstandingCalled;

    IedConnectionClosedHandler connectionCloseHandler;
    void* connectionClosedParameter;

    IedConnection_StateChangedHandler connectionStateChangedHandler;
    void* connectionStateChangedHandlerParameter;

    uint32_t connectionTimeout;

    uint8_t timeQuality;
};

struct sClientReportControlBlock
{
    char* objectReference;
    bool isBuffered;

    MmsValue* rptId;
    MmsValue* rptEna;
    MmsValue* resv;
    MmsValue* datSet;
    MmsValue* confRev;
    MmsValue* optFlds;
    MmsValue* bufTm;
    MmsValue* sqNum;
    MmsValue* trgOps;
    MmsValue* intgPd;
    MmsValue* gi;
    MmsValue* purgeBuf;
    MmsValue* entryId;
    MmsValue* timeOfEntry;
    MmsValue* resvTms;
    MmsValue* owner;
};

LIB61850_INTERNAL bool
iedConnection_doesControlObjectMatch(const char* objRef, const char* cntrlObj);

LIB61850_INTERNAL void
iedConnection_addControlClient(IedConnection self, ControlObjectClient control);

LIB61850_INTERNAL void
iedConnection_removeControlClient(IedConnection self, ControlObjectClient control);

LIB61850_INTERNAL bool
clientReportControlBlock_updateValues(ClientReportControlBlock self, MmsValue* values);

LIB61850_INTERNAL void
iedConnection_handleReport(IedConnection self, MmsValue* value);

LIB61850_INTERNAL IedClientError
iedConnection_mapMmsErrorToIedError(MmsError mmsError);

LIB61850_INTERNAL IedClientError
iedConnection_mapDataAccessErrorToIedError(MmsDataAccessError mmsError);

LIB61850_INTERNAL IedConnectionOutstandingCall
iedConnection_allocateOutstandingCall(IedConnection self);

LIB61850_INTERNAL void
iedConnection_releaseOutstandingCall(IedConnection self, IedConnectionOutstandingCall call);

LIB61850_INTERNAL IedConnectionOutstandingCall
iedConnection_lookupOutstandingCall(IedConnection self, uint32_t invokeId);

LIB61850_INTERNAL ClientReport
ClientReport_create(void);

LIB61850_INTERNAL void
ClientReport_destroy(ClientReport self);

LIB61850_INTERNAL void
controlObjectClient_invokeCommandTerminationHandler(ControlObjectClient self);

LIB61850_INTERNAL void
ControlObjectClient_setLastApplError(ControlObjectClient self, LastApplError lastAppIError);

#endif /* IED_CONNECTION_PRIVATE_H_ */
