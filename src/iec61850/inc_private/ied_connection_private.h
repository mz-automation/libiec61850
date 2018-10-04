/*
 *  ied_connection_private.h
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

#ifndef IED_CONNECTION_PRIVATE_H_
#define IED_CONNECTION_PRIVATE_H_

#ifndef DEBUG_IED_CLIENT
#define DEBUG_IED_CLIENT 0
#endif

#include "hal_thread.h"

typedef struct sIedConnectionOutstandingCall* IedConnectionOutstandingCall;

struct sIedConnectionOutstandingCall {
    bool used;
    uint32_t invokeId;
    void* callback;
    void* callbackParameter;
};

struct sIedConnection
{
    MmsConnection connection;
    IedConnectionState state;
    LinkedList enabledReports;
    LinkedList logicalDevices;
    LinkedList clientControls;
    LastApplError lastApplError;

    Semaphore stateMutex;
    Semaphore reportHandlerMutex;

    Semaphore outstandingCallsLock;
    IedConnectionOutstandingCall outstandingCalls;

    IedConnectionClosedHandler connectionCloseHandler;
    void* connectionClosedParameter;
    uint32_t connectionTimeout;
};

struct sClientReportControlBlock {
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

IedClientError
private_IedConnection_mapMmsErrorToIedError(MmsError mmsError);

bool
private_IedConnection_doesControlObjectMatch(const char* objRef, const char* cntrlObj);

void
private_IedConnection_addControlClient(IedConnection self, ControlObjectClient control);

void
private_IedConnection_removeControlClient(IedConnection self, ControlObjectClient control);

bool
private_ClientReportControlBlock_updateValues(ClientReportControlBlock self, MmsValue* values);

void
private_IedConnection_handleReport(IedConnection self, MmsValue* value);

IedClientError
iedConnection_mapMmsErrorToIedError(MmsError mmsError);

IedClientError
iedConnection_mapDataAccessErrorToIedError(MmsDataAccessError mmsError);

ClientReport
ClientReport_create(void);

void
ClientReport_destroy(ClientReport self);

void
private_ControlObjectClient_invokeCommandTerminationHandler(ControlObjectClient self);

/* some declarations that are shared with server side ! */

char*
MmsMapping_getMmsDomainFromObjectReference(const char* objectReference, char* buffer);

char*
MmsMapping_createMmsVariableNameFromObjectReference(const char* objectReference, FunctionalConstraint fc, char* buffer);


char*
MmsMapping_varAccessSpecToObjectReference(MmsVariableAccessSpecification* varAccessSpec);

MmsVariableAccessSpecification*
MmsMapping_ObjectReferenceToVariableAccessSpec(char* objectReference);

#endif /* IED_CONNECTION_PRIVATE_H_ */
