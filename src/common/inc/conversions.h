/*
 *  conversions.h
 *
 *  Some helper functions to convert data.
 *
 *  Copyright 2014 Michael Zillgith
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

#ifndef CONVERSIONS_H_
#define CONVERSIONS_H_

#include "libiec61850_platform_includes.h"

void
Conversions_intToStringBuffer(int intValue, int numberOfDigits, uint8_t* buffer);

void
Conversions_msTimeToGeneralizedTime(uint64_t msTime, uint8_t* buffer);

uint64_t
Conversions_generalizedTimeToMsTime(const char* gtString);

void
memcpyReverseByteOrder(uint8_t* dst, const uint8_t* src, int size);

#endif /* CONVERSIONS_H_ */
