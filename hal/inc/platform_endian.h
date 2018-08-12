/*
 *  platform_endian.h
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

#ifndef ENDIAN_H_
#define ENDIAN_H_

#ifndef PLATFORM_IS_BIGENDIAN
#ifdef __GNUC__
#ifdef __BYTE_ORDER__
#if (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#define PLATFORM_IS_BIGENDIAN 1
#endif

#else

/* older versions of GCC have __BYTE_ORDER__ not defined! */
#ifdef __PPC__
#define PLATFORM_IS_BIGENDIAN 1
#endif

#endif /* __BYTE_ORDER__ */
#endif /* __GNUC__ */
#endif

#if (PLATFORM_IS_BIGENDIAN == 1)
#  define ORDER_LITTLE_ENDIAN 0
#else
#  define ORDER_LITTLE_ENDIAN 1
#endif

#endif /* ENDIAN_H_ */
