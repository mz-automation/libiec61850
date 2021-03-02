/*
 *  ied_server_private.h
 *
 *  Library private function definitions for IedServer.
 *
 *  Copyright 2013-2018 Michael Zillgith
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
#define ALLOW_WRITE_ACCESS_BL 32

struct sIedServer
{
    IedModel* model;
    MmsDevice* mmsDevice;
    MmsServer mmsServer;
    char* localIpAddress;
    MmsMapping* mmsMapping;
    LinkedList clientConnections;
    uint8_t writeAccessPolicies;

#if (CONFIG_IEC61850_REPORT_SERVICE == 1)
    int reportBufferSizeBRCBs;
    int reportBufferSizeURCBs;
    bool enableBRCBResvTms;
    bool enableOwnerForRCB;
#endif

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore dataModelLock;
    Semaphore clientConnectionsLock;
#endif

#if (CONFIG_MMS_SERVER_CONFIG_SERVICES_AT_RUNTIME == 1)
    bool logServiceEnabled;
#endif

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Thread serverThread;
#endif

#if (CONFIG_IEC61850_SUPPORT_SERVER_IDENTITY == 1)
    char* vendorName;
    char* modelName;
    char* revision;
#endif

#if (CONFIG_IEC61850_SETTING_GROUPS == 1)
    bool enableEditSG;
    bool hasSGCBResvTms;
#endif

    uint8_t edition;

    bool running;
};


LIB61850_INTERNAL IEC61850_ServiceError
private_IedServer_convertMmsDataAccessErrorToServiceError(MmsDataAccessError mmsError);

LIB61850_INTERNAL ClientConnection
private_IedServer_getClientConnectionByHandle(IedServer self, void* serverConnectionHandle);

LIB61850_INTERNAL ClientConnection
private_ClientConnection_create(void* serverConnectionHandle);

LIB61850_INTERNAL void
private_ClientConnection_destroy(ClientConnection self);

LIB61850_INTERNAL int
private_ClientConnection_getTasksCount(ClientConnection self);

LIB61850_INTERNAL void
private_ClientConnection_increaseTasksCount(ClientConnection self);

LIB61850_INTERNAL void
private_ClientConnection_decreaseTasksCount(ClientConnection self);

LIB61850_INTERNAL void*
private_ClientConnection_getServerConnectionHandle(ClientConnection self);

LIB61850_INTERNAL void
private_IedServer_addNewClientConnection(IedServer self, ClientConnection newClientConnection);

LIB61850_INTERNAL void
private_IedServer_removeClientConnection(IedServer self, ClientConnection clientConnection);

#endif /* IED_SERVER_PRIVATE_H_ */
