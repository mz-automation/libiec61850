/*
 *  time.c
 *
 *  Copyright 2013-2021 Michael Zillgith
 *
 *  This file is part of Platform Abstraction Layer (libpal)
 *  for libiec61850, libmms, and lib60870.
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

