/*
 *  r_session_internal.h
 *
 *  Copyright 2013-2021 Michael Zillgith
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

#ifndef SRC_R_SESSION_R_SESSION_INTERNAL_H_
#define SRC_R_SESSION_R_SESSION_INTERNAL_H_

#include "r_session.h"

LIB61850_INTERNAL Socket
RSession_getSocket(RSession self);

typedef enum {
    RSESSION_SPDU_ID_TUNNELED = 0xa0,
    RSESSION_SPDU_ID_GOOSE = 0xa1,
    RSESSION_SPDU_ID_SV = 0xa2,
    RSESSION_SPDU_ID_MGMT = 0xa3
} RSessionProtocol_SPDU_ID;

/*
 * NOTE: For library internal use!
 *
 * \param spduId SPDU Identifier(SI) - 0xa0(tunneled), 0xa1(GOOSE), 0xa2(Sampled Values)
 */
LIB61850_INTERNAL RSessionError
RSession_sendMessage(RSession self, RSessionProtocol_SPDU_ID spduId, bool simulation, uint16_t appId, uint8_t* payload, int payloadSize);

//RSessionError
//RSession_sendMultiplePayloadMessage(RSession self, RSessionPayloadElement elements);

typedef void (*RSessionPayloadElementHandler) (void* parameter, uint16_t appId, uint8_t* payloadData, int payloadSize);

LIB61850_INTERNAL RSessionError
RSession_receiveMessage(RSession self, RSessionPayloadElementHandler handler, void* parameter);

#endif /* SRC_R_SESSION_R_SESSION_INTERNAL_H_ */
