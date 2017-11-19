/*
 *  sv_publisher.h
 *
 *  Copyright 2016 Michael Zillgith
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


#ifndef LIBIEC61850_SRC_SAMPLED_VALUES_SV_PUBLISHER_DEPRECATED_H_
#define LIBIEC61850_SRC_SAMPLED_VALUES_SV_PUBLISHER_DEPRECATED_H_

#include "libiec61850_platform_includes.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__GNUC__) || defined(__clang__)
  #define DEPRECATED __attribute__((deprecated))
#else
  #define DEPRECATED
#endif

/**
 * \addtogroup sv_publisher_deprecated_api_group Deprecated API
 * \ingroup sv_publisher_api_group IEC 61850 Sampled Values (SV) publisher API
 * \deprecated
 * @{
 */

typedef DEPRECATED struct sSVPublisher* SampledValuesPublisher;

typedef DEPRECATED struct sSV_ASDU* SV_ASDU;

static DEPRECATED
SVPublisher
SampledValuesPublisher_create(CommParameters* parameters, const char* interfaceId)
{
    return SVPublisher_create(parameters, interfaceId);
}

static DEPRECATED
SVPublisher_ASDU
SampledValuesPublisher_addASDU(SVPublisher self, char* svID, char* datset, uint32_t confRev)
{
    return SVPublisher_addASDU(self, svID, datset, confRev);
}

static DEPRECATED
void
SampledValuesPublisher_setupComplete(SVPublisher self)
{
    SVPublisher_setupComplete(self);
}

static DEPRECATED
void
SampledValuesPublisher_publish(SVPublisher self)
{
    SVPublisher_publish(self);
}

static DEPRECATED
void
SampledValuesPublisher_destroy(SVPublisher self)
{
    SVPublisher_destroy(self);
}

static DEPRECATED
void
SV_ASDU_resetBuffer(SVPublisher_ASDU self)
{
    SVPublisher_ASDU_resetBuffer(self);
}

static DEPRECATED
int
SV_ASDU_addINT8(SVPublisher_ASDU self)
{
    return SVPublisher_ASDU_addINT8(self);
}

static DEPRECATED
void
SV_ASDU_setINT8(SVPublisher_ASDU self, int index, int8_t value)
{
    SVPublisher_ASDU_setINT8(self, index, value);
}

static DEPRECATED
int
SV_ASDU_addINT32(SVPublisher_ASDU self)
{
    return SVPublisher_ASDU_addINT32(self);
}

static DEPRECATED
void
SV_ASDU_setINT32(SVPublisher_ASDU self, int index, int32_t value)
{
    SVPublisher_ASDU_setINT32(self, index, value);
}

static DEPRECATED
int
SV_ASDU_addINT64(SVPublisher_ASDU self)
{
    return SVPublisher_ASDU_addINT64(self);
}

static DEPRECATED
void
SV_ASDU_setINT64(SVPublisher_ASDU self, int index, int64_t value)
{
    SVPublisher_ASDU_setINT64(self, index, value);
}

static DEPRECATED
int
SV_ASDU_addFLOAT(SVPublisher_ASDU self)
{
    return SVPublisher_ASDU_addFLOAT(self);
}

static DEPRECATED
void
SV_ASDU_setFLOAT(SVPublisher_ASDU self, int index, float value)
{
    SVPublisher_ASDU_setFLOAT(self, index, value);
}

static DEPRECATED
int
SV_ASDU_addFLOAT64(SVPublisher_ASDU self)
{
    return SVPublisher_ASDU_addFLOAT64(self);
}

static DEPRECATED
void
SV_ASDU_setFLOAT64(SVPublisher_ASDU self, int index, double value)
{
    SVPublisher_ASDU_setFLOAT64(self, index, value);
}

static DEPRECATED
void
SV_ASDU_setSmpCnt(SVPublisher_ASDU self, uint16_t value)
{
    SVPublisher_ASDU_setSmpCnt(self, value);
}

static DEPRECATED
uint16_t
SV_ASDU_getSmpCnt(SVPublisher_ASDU self)
{
    return SVPublisher_ASDU_getSmpCnt(self);
}

static DEPRECATED
void
SV_ASDU_increaseSmpCnt(SVPublisher_ASDU self)
{
    SVPublisher_ASDU_increaseSmpCnt(self);
}

static DEPRECATED
void
SV_ASDU_setRefrTm(SVPublisher_ASDU self, uint64_t refrTm)
{
    SVPublisher_ASDU_setRefrTm(self, refrTm);
}

static DEPRECATED
void
SV_ASDU_setSmpMod(SVPublisher_ASDU self, uint8_t smpMod)
{
    SVPublisher_ASDU_setSmpMod(self, smpMod);
}

static DEPRECATED
void
SV_ASDU_setSmpRate(SVPublisher_ASDU self, uint16_t smpRate)
{
    SVPublisher_ASDU_setSmpRate(self, smpRate);
}

/**@}*/

#ifdef __cplusplus
}
#endif

#endif /* LIBIEC61850_SRC_SAMPLED_VALUES_SV_PUBLISHER_DEPRECATED_H_ */
