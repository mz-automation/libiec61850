/*
 *  ied_server_private.h
 *
 *  Library private function definitions for IedServer.
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


#ifndef IED_SERVER_PRIVATE_H_
#define IED_SERVER_PRIVATE_H_

#define ALLOW_WRITE_ACCESS_DC 1
#define ALLOW_WRITE_ACCESS_CF 2
#define ALLOW_WRITE_ACCESS_SP 4
#define ALLOW_WRITE_ACCESS_SV 8
#define ALLOW_WRITE_ACCESS_SE 16

struct sIedServer
{
    IedModel* model;
    MmsDevice* mmsDevice;
    MmsServer mmsServer;
    IsoServer isoServer;
    char* localIpAddress;
    MmsMapping* mmsMapping;
    LinkedList clientConnections;
    uint8_t writeAccessPolicies;

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore dataModelLock;
#endif

    bool running;
};


ClientConnection
private_IedServer_getClientConnectionByHandle(IedServer self, void* serverConnectionHandle);

ClientConnection
private_ClientConnection_create(void* serverConnectionHandle);

void
private_ClientConnection_destroy(ClientConnection self);

int
private_ClientConnection_getTasksCount(ClientConnection self);

void
private_ClientConnection_increaseTasksCount(ClientConnection self);

void
private_ClientConnection_decreaseTasksCount(ClientConnection self);

void*
private_ClientConnection_getServerConnectionHandle(ClientConnection self);

void
private_IedServer_addNewClientConnection(IedServer self, ClientConnection newClientConnection);

void
private_IedServer_removeClientConnection(IedServer self, ClientConnection clientConnection);

#endif /* IED_SERVER_PRIVATE_H_ */
