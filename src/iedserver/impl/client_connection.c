/*
 *  client_connection.c
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

#include "iec61850_server.h"
#include "iso_server.h"
#include "mms_mapping.h"
#include "control.h"
#include "stack_config.h"
#include "thread.h"

#include "ied_server_private.h"

struct sClientConnection {
    Semaphore tasksCountMutex;
    int tasksCount;
    void* serverConnectionHandle;
};

ClientConnection
private_ClientConnection_create(void* serverConnectionHandle)
{
    ClientConnection self = (ClientConnection) malloc(sizeof(struct sClientConnection));

    self->tasksCountMutex = Semaphore_create(1);
    self->tasksCount = 0;
    self->serverConnectionHandle = serverConnectionHandle;

    return self;
}

void
private_ClientConnection_destroy(ClientConnection self)
{
    Semaphore_destroy(self->tasksCountMutex);
    free(self);
}

int
private_ClientConnection_getTasksCount(ClientConnection self)
{
    int tasksCount;

    Semaphore_wait(self->tasksCountMutex);
    tasksCount = self->tasksCount;
    Semaphore_post(self->tasksCountMutex);

    return tasksCount;
}

void
private_ClientConnection_increaseTasksCount(ClientConnection self)
{
    Semaphore_wait(self->tasksCountMutex);
    self->tasksCount++;
    Semaphore_post(self->tasksCountMutex);
}

void
private_ClientConnection_decreaseTasksCount(ClientConnection self)
{
    Semaphore_wait(self->tasksCountMutex);
    self->tasksCount--;
    Semaphore_post(self->tasksCountMutex);
}

void*
private_ClientConnection_getServerConnectionHandle(ClientConnection self)
{
    return self->serverConnectionHandle;
}


char*
ClientConnection_getPeerAddress(ClientConnection self)
{
    MmsServerConnection* mmsConnection = (MmsServerConnection*) self->serverConnectionHandle;

    return IsoConnection_getPeerAddress(mmsConnection->isoConnection);
}


void*
ClientConnection_getSecurityToken(ClientConnection self)
{
    MmsServerConnection* mmsConnection = (MmsServerConnection*) self->serverConnectionHandle;

    return IsoConnection_getSecurityToken(mmsConnection->isoConnection);
}
