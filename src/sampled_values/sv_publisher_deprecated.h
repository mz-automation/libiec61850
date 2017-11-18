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

typedef struct sSVPublisher* SampledValuesPublisher DEPRECATED;

/**
 * \brief This function is deprecated and will be removed in the next major release. Use SVPublisher_create() instead.
 * \deprecated
 */
static DEPRECATED
SampledValuesPublisher
SampledValuesPublisher_create(CommParameters* parameters, const char* interfaceId)
{
    return SVPublisher_create(parameters, interfaceId);
}

/**
 * \brief This function is deprecated and will be removed in the next major release. Use SVPublisher_addASDU() instead.
 * \deprecated
 */
static DEPRECATED
SV_ASDU
SampledValuesPublisher_addASDU(SampledValuesPublisher self, char* svID, char* datset, uint32_t confRev)
{
    return SVPublisher_addASDU(self, svID, datset, confRev);
}

/**
 * \brief This function is deprecated and will be removed in the next major release. Use SVPublisher_setupComplete() instead.
 * \deprecated
 */
static DEPRECATED
void
SampledValuesPublisher_setupComplete(SampledValuesPublisher self)
{
    SVPublisher_setupComplete(self);
}

/**
 * \brief This function is deprecated and will be removed in the next major release. Use SVPublisher_publish() instead.
 * \deprecated
 */
static DEPRECATED
void
SampledValuesPublisher_publish(SampledValuesPublisher self)
{
    SVPublisher_publish(self);
}

/**
 * \brief This function is deprecated and will be removed in the next major release. Use SVPublisher_destroy() instead.
 * \deprecated
 */
static DEPRECATED
void
SampledValuesPublisher_destroy(SampledValuesPublisher self)
{
    SVPublisher_destroy(self);
}

#ifdef __cplusplus
}
#endif

#endif /* LIBIEC61850_SRC_SAMPLED_VALUES_SV_PUBLISHER_DEPRECATED_H_ */
