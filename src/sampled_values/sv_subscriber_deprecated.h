/*
 *  sv_subscriber_deprecated.h
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
 */

#ifndef SAMPLED_VALUES_SV_SUBSCRIBER_DEPRECATED_H_
#define SAMPLED_VALUES_SV_SUBSCRIBER_DEPRECATED_H_

#include "libiec61850_common_api.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__GNUC__) || defined(__clang__)
  #define DEPRECATED __attribute__((deprecated))
#else
  #define DEPRECATED
#endif

/**
 * \addtogroup sv_subscriber_deprecated_api_group Deprecated API
 * \ingroup sv_subscriber_api_group IEC 61850 Sampled Values (SV) publisher API
 * \deprecated
 * @{
 */

typedef struct sSVSubscriberASDU* SVClientASDU;

static DEPRECATED
uint16_t
SVClientASDU_getSmpCnt(SVSubscriber_ASDU self)
{
	return SVSubscriber_ASDU_getSmpCnt(self);
}

static DEPRECATED
const char*
SVClientASDU_getSvId(SVSubscriber_ASDU self)
{
	return SVSubscriber_ASDU_getSvId(self);
}

static DEPRECATED
uint32_t
SVClientASDU_getConfRev(SVSubscriber_ASDU self)
{
	return SVSubscriber_ASDU_getConfRev(self);
}

static DEPRECATED
bool
SVClientASDU_hasRefrTm(SVSubscriber_ASDU self)
{
	return SVSubscriber_ASDU_hasRefrTm(self);
}

static DEPRECATED
uint64_t
SVClientASDU_getRefrTmAsMs(SVSubscriber_ASDU self)
{
	return SVSubscriber_ASDU_getRefrTmAsMs(self);
}

static DEPRECATED
int8_t
SVClientASDU_getINT8(SVSubscriber_ASDU self, int index)
{
	return SVSubscriber_ASDU_getINT8(self, index);
}

static DEPRECATED
int16_t
SVClientASDU_getINT16(SVSubscriber_ASDU self, int index)
{
	return SVSubscriber_ASDU_getINT16(self, index);
}

static DEPRECATED
int32_t
SVClientASDU_getINT32(SVSubscriber_ASDU self, int index)
{
	return SVSubscriber_ASDU_getINT32(self, index);
}

static DEPRECATED
int64_t
SVClientASDU_getINT64(SVSubscriber_ASDU self, int index)
{
	return SVSubscriber_ASDU_getINT64(self, index);
}

static DEPRECATED
uint8_t
SVClientASDU_getINT8U(SVSubscriber_ASDU self, int index)
{
	return SVSubscriber_ASDU_getINT8U(self, index);
}

static DEPRECATED
uint16_t
SVClientASDU_getINT16U(SVSubscriber_ASDU self, int index)
{
	return SVSubscriber_ASDU_getINT16U(self, index);
}

static DEPRECATED
uint32_t
SVClientASDU_getINT32U(SVSubscriber_ASDU self, int index)
{
	return SVSubscriber_ASDU_getINT32U(self, index);
}

static DEPRECATED
uint64_t
SVClientASDU_getINT64U(SVSubscriber_ASDU self, int index)
{
	return SVSubscriber_ASDU_getINT64U(self, index);
}

static DEPRECATED
float
SVClientASDU_getFLOAT32(SVSubscriber_ASDU self, int index)
{
	return SVSubscriber_ASDU_getFLOAT32(self, index);
}

static DEPRECATED
double
SVClientASDU_getFLOAT64(SVSubscriber_ASDU self, int index)
{
	return SVSubscriber_ASDU_getFLOAT64(self, index);
}

static DEPRECATED
int
SVClientASDU_getDataSize(SVSubscriber_ASDU self)
{
	return SVSubscriber_ASDU_getDataSize(self);
}

/**@}*/

#ifdef __cplusplus
}
#endif


#endif /* SAMPLED_VALUES_SV_SUBSCRIBER_DEPRECATED_H_ */
