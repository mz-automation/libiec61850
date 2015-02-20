/*
 *  mms_indication.h
 *
 *  Copyright 2015 Michael Zillgith
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
 *
 *
 *  MMS client connection handling code for libiec61850.
 *
 *  Handles a MMS client connection.
 */

#ifndef MMS_INDICATION_H_
#define MMS_INDICATION_H_

#define DEFAULT_MAX_SERV_OUTSTANDING_CALLING 5
#define DEFAULT_MAX_SERV_OUTSTANDING_CALLED 5
#define DEFAULT_DATA_STRUCTURE_NESTING_LEVEL 10

typedef enum
{
    MMS_ERROR, MMS_INITIATE, MMS_CONFIRMED_REQUEST, MMS_OK, MMS_CONCLUDE
} MmsIndication;

#endif /* MMS_INDICATION_H_ */
