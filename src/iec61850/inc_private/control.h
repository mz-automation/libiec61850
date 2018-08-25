/*
 *  control.h
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

#ifndef CONTROL_H_
#define CONTROL_H_

#include "iec61850_model.h"
#include "iec61850_server.h"
#include "mms_server_connection.h"
#include "mms_device_model.h"

#include "mms_mapping_internal.h"
#include "mms_client_connection.h"

#include "libiec61850_platform_includes.h"

typedef struct sControlObject ControlObject;

struct sControlObject
{
    MmsDomain* mmsDomain;
    IedServer iedServer;
    char* lnName;
    char* name;

    int state;

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore stateLock;
#endif

    MmsValue* mmsValue;
    MmsVariableSpecification* typeSpec;

    MmsValue* oper;
    MmsValue* sbo;
    MmsValue* sbow;
    MmsValue* cancel;

    MmsValue* ctlVal;
    MmsValue* ctlNum;
    MmsValue* origin;
    MmsValue* timestamp;

    char ctlObjectName[130];

    /* for LastAppIError */
    MmsValue* error;
    MmsValue* addCause;

    bool selected;
    uint64_t selectTime;
    uint32_t selectTimeout;
    MmsValue* sboClass;
    MmsValue* sboTimeout;

    bool timeActivatedOperate;
    uint64_t operateTime;

    bool operateOnce;
    MmsServerConnection mmsConnection;

    MmsValue* emptyString;

    uint32_t ctlModel;

    bool testMode;
    bool interlockCheck;
    bool synchroCheck;

    uint32_t operateInvokeId;

    ControlHandler operateHandler;
    void* operateHandlerParameter;

    ControlPerformCheckHandler checkHandler;
    void* checkHandlerParameter;

    ControlWaitForExecutionHandler waitForExecutionHandler;
    void* waitForExecutionHandlerParameter;
};

ControlObject*
ControlObject_create(IedServer iedServer, MmsDomain* domain, char* lnName, char* name);

void
ControlObject_initialize(ControlObject* self);

void
ControlObject_destroy(ControlObject* self);

void
ControlObject_setMmsValue(ControlObject* self, MmsValue* value);

MmsValue*
ControlObject_getMmsValue(ControlObject* self);

void
ControlObject_setTypeSpec(ControlObject* self, MmsVariableSpecification* typeSpec);

MmsVariableSpecification*
ControlObject_getTypeSpec(ControlObject* self);

char*
ControlObject_getName(ControlObject* self);

char*
ControlObject_getLNName(ControlObject* self);

MmsDomain*
ControlObject_getDomain(ControlObject* self);

bool
ControlObject_select(ControlObject* self, MmsServerConnection connection);

bool
ControlObject_unselect(ControlObject* self, MmsServerConnection connection);

void
ControlObject_installListener(ControlObject* self, ControlHandler listener, void* parameter);

void
ControlObject_installCheckHandler(ControlObject* self, ControlPerformCheckHandler handler, void* parameter);

void
ControlObject_installWaitForExecutionHandler(ControlObject* self, ControlWaitForExecutionHandler handler, void* parameter);

#endif /* CONTROL_H_ */
