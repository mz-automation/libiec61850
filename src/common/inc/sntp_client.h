/*
 *  Copyright 2013-2022 Michael Zillgith
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

#ifndef LIBIEC61850_SRC_COMMON_INC_SNTP_CLIENT_H_
#define LIBIEC61850_SRC_COMMON_INC_SNTP_CLIENT_H_

#include "libiec61850_common_api.h"
#include "hal_socket.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct sSNTPClient* SNTPClient;

typedef void (*SNTPClient_UserCallback)(void* parameter, bool isSynced);

LIB61850_API SNTPClient
SNTPClient_create();

LIB61850_API void
SNTPClient_setLocalAddress(SNTPClient self, const char* localAddress);

LIB61850_API void
SNTPClient_setLocalPort(SNTPClient self, int udpPort);

LIB61850_API HandleSet
SNTPClient_getHandleSet(SNTPClient self);

LIB61850_API void
SNTPClient_addServer(SNTPClient self, const char* serverAddr, int serverPort);

LIB61850_API void
SNTPClient_setPollInterval(SNTPClient self, uint32_t intervalInSeconds);

LIB61850_API void
SNTPClient_setUserCallback(SNTPClient self, SNTPClient_UserCallback callback, void* parameter);

LIB61850_API bool
SNTPClient_isSynchronized(SNTPClient self);

LIB61850_API void
SNTPClient_start(SNTPClient self);

LIB61850_API void
SNTPClient_stop(SNTPClient self);

LIB61850_API void
SNTPClient_destroy(SNTPClient self);

#ifdef __cplusplus
}
#endif

#endif /* LIBIEC61850_SRC_COMMON_INC_SNTP_CLIENT_H_ */
