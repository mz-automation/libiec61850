/*
 *  mms_server_libinternal.h
 *
 *  Copyright 2013-2023 Michael Zillgith
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

#ifndef MMS_SERVER_LIBINTERNAL_H_
#define MMS_SERVER_LIBINTERNAL_H_

#include "mms_server.h"
#include "mms_device_model.h"

typedef MmsValue* (*MmsReadVariableHandler)(void* parameter, MmsDomain* domain,
        char* variableId, MmsServerConnection connection, bool isDirectAccess);

typedef MmsDataAccessError (*MmsReadAccessHandler) (void* parameter, MmsDomain* domain,
        char* variableId, MmsServerConnection connection, bool isDirectAccess);

typedef MmsDataAccessError (*MmsWriteVariableHandler)(void* parameter,
        MmsDomain* domain, const char* variableId, int arrayIdx, const char* componentId, MmsValue* value,
        MmsServerConnection connection);

typedef bool (*MmsListAccessHandler) (void* parameter, MmsGetNameListType listType, MmsDomain* domain,
        char* variableId, MmsServerConnection connection);

typedef void (*MmsConnectionHandler)(void* parameter,
        MmsServerConnection connection, MmsServerEvent event);

LIB61850_INTERNAL MmsServer
MmsServer_create(MmsDevice* device, TLSConfiguration tlsConfiguration);

LIB61850_INTERNAL void
MmsServer_destroy(MmsServer self);

/**
 * \brief Add a new passive access point to the server
 */
LIB61850_INTERNAL bool
MmsServer_addAP(MmsServer self, const char* ipAddr, int tcpPort, TLSConfiguration tlsConfiguration);

LIB61850_INTERNAL void
MmsServer_installReadHandler(MmsServer self, MmsReadVariableHandler,
        void* parameter);

LIB61850_INTERNAL void
MmsServer_installReadAccessHandler(MmsServer self, MmsReadAccessHandler, void* parameter);

LIB61850_INTERNAL void
MmsServer_installWriteHandler(MmsServer self, MmsWriteVariableHandler,
        void* parameter);

LIB61850_INTERNAL void
MmsServer_installListAccessHandler(MmsServer self, MmsListAccessHandler listAccessHandler, void* parameter);

/**
 * A connection handler will be invoked whenever a new client connection is opened or closed
 */
LIB61850_INTERNAL void
MmsServer_installConnectionHandler(MmsServer self, MmsConnectionHandler,
        void* parameter);

LIB61850_INTERNAL void
MmsServer_setClientAuthenticator(MmsServer self, AcseAuthenticator authenticator, void* authenticatorParameter);

LIB61850_INTERNAL MmsDevice*
MmsServer_getDevice(MmsServer self);

LIB61850_INTERNAL MmsValue*
MmsServer_getValueFromCache(MmsServer self, MmsDomain* domain, const char* itemId);

LIB61850_INTERNAL MmsValue*
MmsServer_getValueFromCacheEx(MmsServer self, MmsDomain* domain, const char* itemId, MmsVariableSpecification** typeSpec);

LIB61850_INTERNAL MmsValue*
MmsServer_getValueFromCacheEx2(MmsServer self, MmsDomain* domain, const char* itemId, int idx, const char* componentId);

LIB61850_INTERNAL bool
MmsServer_isLocked(MmsServer self);

/**
 * \brief lock the cached server data model
 *
 * NOTE: This method should never be called inside of a library callback function. In the context of
 * a library callback the data model is always already locked! Calling this function inside of a
 * library callback may lead to a deadlock condition.
 *
 * \param self the MmsServer instance to operate on
 */
LIB61850_INTERNAL void
MmsServer_lockModel(MmsServer self);

/**
 * \brief unlock the cached server data model
 *
 * NOTE: This method should never be called inside of a library callback function. In the context of
 * a library callback the data model is always already locked!
 *
 * \param self the MmsServer instance to operate on
 */
LIB61850_INTERNAL void
MmsServer_unlockModel(MmsServer self);

LIB61850_INTERNAL void
MmsServer_insertIntoCache(MmsServer self, MmsDomain* domain, char* itemId,
        MmsValue* value);

/***************************************************
 * Functions for multi-threaded operation mode
 ***************************************************/

/**
 * \brief Start a new server thread and listen for incoming connections
 *
 * \param self the MmsServer instance to operate on
 * \param tcpPort the TCP port the server is listening on.
 */
LIB61850_INTERNAL void
MmsServer_startListening(MmsServer self, int tcpPort);

/**
 * \brief Stop server thread an all open connection threads
 *
 * \param self the MmsServer instance to operate on
 */
LIB61850_INTERNAL void
MmsServer_stopListening(MmsServer self);

/***************************************************
 * Functions for threadless operation mode
 ***************************************************/

/**
 * \brief Start a new server in non-threaded operation mode
 *
 * \param self the MmsServer instance to operate on
 * \param tcpPort the TCP port the server is listening on.
 */
LIB61850_INTERNAL void
MmsServer_startListeningThreadless(MmsServer self, int tcpPort);

/**
 * \brief Wait for the server to come ready in non-threaded operation mode
 *
 * \param self the MmsServer instance to operate on
 * \param timeoutMs maximum number of milliseconds to wait
 * \return 1 if the server is ready, 0 if not or -1 on error
 */
LIB61850_INTERNAL int
MmsServer_waitReady(MmsServer self, unsigned int timeoutMs);

/**
 * \brief Handle client connections (for non-threaded operation mode)
 *
 * This function is listening for new client connections and handles incoming
 * requests for existing client connections.
 *
 * \param self the MmsServer instance to operate on
 */
LIB61850_INTERNAL void
MmsServer_handleIncomingMessages(MmsServer self);

/**
 * \brief Handle MmsServer background task
 *
 * \param self the MmsServer instance to operate on
 */
LIB61850_INTERNAL void
MmsServer_handleBackgroundTasks(MmsServer self);

/**
 * \brief Get number of open connections
 *
 * \param self the MmsServer instance to operate on
 *
 * \return the number of open MMS connections
 */
LIB61850_INTERNAL int
MmsServer_getConnectionCounter(MmsServer self);

/**
 * \brief Stop the server (for non-threaded operation mode)
 *
 * \param self the MmsServer instance to operate on
 */
LIB61850_INTERNAL void
MmsServer_stopListeningThreadless(MmsServer self);

LIB61850_INTERNAL const char*
MmsServer_getFilesystemBasepath(MmsServer self);

#endif /* MMS_SERVER_LIBINTERNAL_H_ */
