/*
 *  iso_server_private.h
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

#ifndef ISO_SERVER_PRIVATE_H_
#define ISO_SERVER_PRIVATE_H_

#include "tls_config.h"
#include "hal_socket.h"

IsoConnection
IsoConnection_create(Socket socket, IsoServer isoServer);

void
IsoConnection_destroy(IsoConnection self);

void
IsoConnection_handleTcpConnection(IsoConnection self);

void
IsoConnection_addHandleSet(const IsoConnection self, HandleSet handles);

void
private_IsoServer_increaseConnectionCounter(IsoServer self);

void
private_IsoServer_decreaseConnectionCounter(IsoServer self);

int
private_IsoServer_getConnectionCounter(IsoServer self);

/**
 * \brief User provided lock that will be called when higher layer (MMS) is called
 */
void
IsoServer_setUserLock(IsoServer self, Semaphore userLock);

void
IsoServer_userLock(IsoServer self);

void
IsoServer_userUnlock(IsoServer self);

bool
IsoConnection_isRunning(IsoConnection self);

#endif /* ISO_SERVER_PRIVATE_H_ */
