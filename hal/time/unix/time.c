/*
 *  time.c
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

#include "hal_time.h"

#include <time.h>

#ifdef CONFIG_SYSTEM_HAS_CLOCK_GETTIME
uint64_t
Hal_getTimeInMs()
{
	struct timespec tp;

	clock_gettime(CLOCK_REALTIME, &tp);

	return ((uint64_t) tp.tv_sec) * 1000LL + (tp.tv_nsec / 1000000);
}
#else

#include <sys/time.h>

msSinceEpoch
Hal_getTimeInMs()
{
    struct timeval now;

    gettimeofday(&now, NULL);

    return ((uint64_t) now.tv_sec * 1000LL) + (now.tv_usec / 1000);
}

nsSinceEpoch
Hal_getTimeInNs()
{
    struct timespec now;

    clock_gettime(CLOCK_REALTIME, &now);

    nsSinceEpoch nsTime = now.tv_sec * 1000000000UL;
    nsTime += now.tv_nsec;

    return nsTime;
}

bool
Hal_setTimeInNs(nsSinceEpoch nsTime)
{
    struct timespec tv;

    tv.tv_sec = nsTime / 1000000000UL;
    tv.tv_nsec = nsTime % 1000000000UL;

    if (clock_settime(CLOCK_REALTIME, &tv) < 0) {
        return false;
    }

    return true;
}


#endif

