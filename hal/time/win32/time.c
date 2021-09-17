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
#include <windows.h>

uint64_t
Hal_getTimeInMs()
{
   FILETIME ft;
   uint64_t now;

   static const uint64_t DIFF_TO_UNIXTIME = 11644473600000ULL;

   GetSystemTimeAsFileTime(&ft);

   now = (LONGLONG)ft.dwLowDateTime + ((LONGLONG)(ft.dwHighDateTime) << 32LL);

   return (now / 10000LL) - DIFF_TO_UNIXTIME;
}

nsSinceEpoch
Hal_getTimeInNs()
{
   FILETIME ft;

   static const uint64_t DIFF_TO_UNIXTIME = 11644473600000000000ULL;

   GetSystemTimeAsFileTime(&ft);

   nsSinceEpoch nsTime = (LONGLONG)ft.dwLowDateTime + ((LONGLONG)(ft.dwHighDateTime) << 32LL);

   nsTime = nsTime * 100LL - DIFF_TO_UNIXTIME;

   return nsTime;
}

bool
Hal_setTimeInNs(nsSinceEpoch nsTime)
{
    uint64_t t = (nsTime / 100ULL) + 116444736000000000ULL;                               

    FILETIME ft;

    ft.dwLowDateTime = (uint32_t)(t & 0xffffffff);
    ft.dwHighDateTime = (uint32_t)(t >> 32);

    SYSTEMTIME st;

    FileTimeToSystemTime(&ft, &st);

    return SetSystemTime(&st);
}

