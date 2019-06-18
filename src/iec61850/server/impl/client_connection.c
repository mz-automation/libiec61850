/*
 *  client_connection.c
 *
 *  Copyright 2013, 2014, 2015 Michael Zillgith
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

#include "iec61850_server.h"
#include "iso_server.h"
#include "mms_mapping.h"
#include "control.h"
#include "stack_config.h"
#include "hal_thread.h"

#include "ied_server_private.h"

#include "libiec61850_platform_includes.h"

struct sClientConnection {

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore tasksCountMutex;
#endif

    int tasksCount;
    void* serverConnectionHandle;
};

ClientConnection
private_ClientConnection_create(void* serverConnectionHandle)
{
    ClientConnection self = (ClientConnection) GLOBAL_MALLOC(sizeof(struct sClientConnection));

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    self->tasksCountMutex = Semaphore_create(1);
#endif

    self->tasksCount = 0;
    self->serverConnectionHandle = serverConnectionHandle;

    return self;
}

void
private_ClientConnection_destroy(ClientConnection self)
{
#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_destroy(self->tasksCountMutex);
#endif

    GLOBAL_FREEMEM(self);
}

int
private_ClientConnection_getTasksCount(ClientConnection self)
{
    int tasksCount;

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_wait(self->tasksCountMutex);
#endif

    tasksCount = self->tasksCount;

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_post(self->tasksCountMutex);
#endif

    return tasksCount;
}

void
private_ClientConnection_increaseTasksCount(ClientConnection self)
{
#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_wait(self->tasksCountMutex);
#endif

    self->tasksCount++;

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_post(self->tasksCountMutex);
#endif
}

void
private_ClientConnection_decreaseTasksCount(ClientConnection self)
{
#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_wait(self->tasksCountMutex);
#endif

    self->tasksCount--;

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore_post(self->tasksCountMutex);
#endif
}

void*
private_ClientConnection_getServerConnectionHandle(ClientConnection self)
{
    return self->serverConnectionHandle;
}


const char*
ClientConnection_getPeerAddress(ClientConnection self)
{
    MmsServerConnection mmsConnection = (MmsServerConnection) self->serverConnectionHandle;

    return MmsServerConnection_getClientAddress(mmsConnection);
}

const char*
ClientConnection_getLocalAddress(ClientConnection self)
{
    MmsServerConnection mmsConnection = (MmsServerConnection) self->serverConnectionHandle;

    return MmsServerConnection_getLocalAddress(mmsConnection);
}


void*
ClientConnection_getSecurityToken(ClientConnection self)
{
    MmsServerConnection mmsConnection = (MmsServerConnection) self->serverConnectionHandle;

    return MmsServerConnection_getSecurityToken(mmsConnection);
}
