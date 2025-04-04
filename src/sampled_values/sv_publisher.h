/*
 *  sv_publisher.h
 *
 *  Copyright 2016-2025 Michael Zillgith
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

#ifndef LIBIEC61850_SRC_SAMPLED_VALUES_SV_PUBLISHER_H_
#define LIBIEC61850_SRC_SAMPLED_VALUES_SV_PUBLISHER_H_

#include "iec61850_common.h"
#include "r_session.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef GOOSE_SV_COMM_PARAMETERS
#define GOOSE_SV_COMM_PARAMETERS

typedef struct sCommParameters
{
    uint8_t vlanPriority;
    uint16_t vlanId;
    uint16_t appId;
    uint8_t dstAddress[6];
} CommParameters;

#endif

/**
 * \defgroup sv_publisher_api_group IEC 61850 Sampled Values (SV) publisher API
 */
/**@{*/

#define IEC61850_SV_SMPSYNC_NOT_SYNCHRONIZED 0
#define IEC61850_SV_SMPSYNC_SYNCED_UNSPEC_LOCAL_CLOCK 1
#define IEC61850_SV_SMPSYNC_SYNCED_GLOBAL_CLOCK 2

#define IEC61850_SV_SMPMOD_PER_NOMINAL_PERIOD 0
#define IEC61850_SV_SMPMOD_SAMPLES_PER_SECOND 1
#define IEC61850_SV_SMPMOD_SECONDS_PER_SAMPLE 2

/**
 * \brief An opaque type representing an IEC 61850-9-2 Sampled Values publisher.
 */
typedef struct sSVPublisher* SVPublisher;

/**
 * \brief An opaque type representing an IEC 61850-9-2 Sampled Values Application Service Data Unit (ASDU).
 */
typedef struct sSVPublisher_ASDU* SVPublisher_ASDU;

/**
 * \brief Create a new IEC61850-9-2 Sampled Values publisher.
 *
 * NOTE: VLAN tagging is enabled when calling this constructor. To disable VLAN tagging
 * use \ref SVPublisher_createEx instead.
 *
 * \param[in] interfaceId the name of the interface over which the SV publisher should send SV packets.
 * \param[in] parameters optional parameters for setting VLAN options and destination MAC address. Use NULL for default values.
 *
 * \return the new SV publisher instance.
 */
LIB61850_API SVPublisher
SVPublisher_create(CommParameters* parameters, const char* interfaceId);

/**
 * \brief Create a new IEC61850-9-2 Sampled Values publisher.
 *
 * \param[in] interfaceId the name of the interface over which the SV publisher should send SV packets.
 * \param[in] parameters optional parameters for setting VLAN options and destination MAC address. Use NULL for default
 * values.
 * \param[in] useVlanTags enable(true)/disable(false) VLAN tagging
 *
 * \return the new SV publisher instance.
 */
LIB61850_API SVPublisher
SVPublisher_createEx(CommParameters* parameters, const char* interfaceId, bool useVlanTag);

/**
 * \brief Create a new SVPublisher instance for R-SMV
 *
 * \param session R-session protocol instance to use
 * \param appId the appID value to use
 *
 * \return the new SVPublisher instance
 */
LIB61850_API SVPublisher
SVPublisher_createRemote(RSession session, uint16_t appId);

/**
 * \brief Create an Application Service Data Unit (ASDU) and add it to an existing Sampled Values publisher.
 *
 * \param[in] svID
 * \param[in] datset
 * \param[in] confRev Configuration revision number. Should be incremented each time that the configuration of the
 *    logical device changes.
 * \return the new ASDU instance.
 */
LIB61850_API SVPublisher_ASDU
SVPublisher_addASDU(SVPublisher self, const char* svID, const char* datset, uint32_t confRev);

/**
 * \brief Prepare the publisher for publishing.
 *
 * This method must be called before SVPublisher_publish().
 *
 * \param[in] self the Sampled Values publisher instance.
 */
LIB61850_API void
SVPublisher_setupComplete(SVPublisher self);

/**
 * \brief Publish all registered ASDUs
 *
 * \param[in] self the Sampled Values publisher instance.
 */
LIB61850_API void
SVPublisher_publish(SVPublisher self);

/**
 * \brief Destroy an IEC61850-9-2 Sampled Values instance.
 *
 * \param[in] self the Sampled Values publisher instance.
 */
LIB61850_API void
SVPublisher_destroy(SVPublisher self);

/**
 * \addtogroup sv_publisher_asdu_group Values Application Service Data Unit (ASDU)
 *  @{
 */

/**
 * \brief Reset the internal data buffer of an ASDU.
 *
 * All data elements added by SVPublisher_ASDU_add*() functions are removed.
 * SVPublisher_setupComplete() must be called afterwards.
 *
 * \param[in] self the Sampled Values ASDU instance.
 */
LIB61850_API void
SVPublisher_ASDU_resetBuffer(SVPublisher_ASDU self);

/**
 * \brief Reserve memory for a signed 8-bit integer in the ASDU.
 *
 * \param[in] self the Sampled Values ASDU instance.
 *
 * \return the offset in bytes within the ASDU data block.
 */
LIB61850_API int
SVPublisher_ASDU_addINT8(SVPublisher_ASDU self);

/**
 * \brief Set the value of a 8-bit integer in the ASDU.
 *
 * \param[in] self the Sampled Values ASDU instance.
 * \param[in] index The offset within the data block of the ASDU in bytes.
 * \param[in] value The value which should be set.
 */
LIB61850_API void
SVPublisher_ASDU_setINT8(SVPublisher_ASDU self, int index, int8_t value);

/**
 * \brief Reserve memory for a signed 32-bit integer in the ASDU.
 *
 * \param[in] self the Sampled Values ASDU instance.
 *
 * \return the offset in bytes within the ASDU data block.
 */
LIB61850_API int
SVPublisher_ASDU_addINT32(SVPublisher_ASDU self);

/**
 * \brief Set the value of a 32-bit integer in the ASDU.
 *
 * \param[in] self the Sampled Values ASDU instance.
 * \param[in] index The offset within the data block of the ASDU in bytes.
 * \param[in] value The value which should be set.
 */
LIB61850_API void
SVPublisher_ASDU_setINT32(SVPublisher_ASDU self, int index, int32_t value);

/**
 * \brief Reserve memory for a signed 64-bit integer in the ASDU.
 *
 * \param[in] self the Sampled Values ASDU instance.
 *
 * \return the offset in bytes of the new element within the ASDU data block.
 */
LIB61850_API int
SVPublisher_ASDU_addINT64(SVPublisher_ASDU self);

/**
 * \brief Set the value of a 64-bit integer in the ASDU.
 *
 * \param[in] self the Sampled Values ASDU instance.
 * \param[in] index The offset within the data block of the ASDU in bytes.
 * \param[in] value The value which should be set.
 */
LIB61850_API void
SVPublisher_ASDU_setINT64(SVPublisher_ASDU self, int index, int64_t value);

/**
 * \brief Reserve memory for a single precision floating point number in the ASDU.
 *
 * \param[in] self the Sampled Values ASDU instance.
 *
 * \return the offset in bytes of the new element within the ASDU data block.
 */
LIB61850_API int
SVPublisher_ASDU_addFLOAT(SVPublisher_ASDU self);

/**
 * \brief Set the value of a single precision floating point number in the ASDU.
 *
 * \param[in] self the Sampled Values ASDU instance.
 * \param[in] index The offset within the data block of the ASDU in bytes.
 * \param[in] value The value which should be set.
 */
LIB61850_API void
SVPublisher_ASDU_setFLOAT(SVPublisher_ASDU self, int index, float value);

/**
 * \brief Reserve memory for a double precision floating point number in the ASDU.
 *
 * \param[in] self the Sampled Values ASDU instance.
 *
 * \return the offset in bytes of the new element within the ASDU data block.
 */
LIB61850_API int
SVPublisher_ASDU_addFLOAT64(SVPublisher_ASDU self);

/**
 * \brief Set the value of a double precision floating pointer number in the ASDU.
 *
 * \param[in] self the Sampled Values ASDU instance.
 * \param[in] index The offset within the data block of the ASDU in bytes.
 * \param[in] value The value which should be set.
 */
LIB61850_API void
SVPublisher_ASDU_setFLOAT64(SVPublisher_ASDU self, int index, double value);

/**
 * \brief Reserve memory for a 64 bit time stamp in the ASDU
 *
 * \param[in] self the Sampled Values ASDU instance.
 *
 * \return the offset in bytes of the new element within the ASDU data block.
 */
LIB61850_API int
SVPublisher_ASDU_addTimestamp(SVPublisher_ASDU self);

/**
 * \brief Set the value of a 64 bit time stamp in the ASDU.
 *
 * \param[in] self the Sampled Values ASDU instance.
 * \param[in] index The offset within the data block of the ASDU in bytes.
 * \param[in] value The value which should be set.
 */
LIB61850_API void
SVPublisher_ASDU_setTimestamp(SVPublisher_ASDU self, int index, Timestamp value);

/**
 * \brief Reserve memory for a quality value in the ASDU
 *
 * NOTE: Quality is encoded as BITSTRING (4 byte)
 *
 * \param[in] self the Sampled Values ASDU instance.
 *
 * \return the offset in bytes of the new element within the ASDU data block.
 */
LIB61850_API int
SVPublisher_ASDU_addQuality(SVPublisher_ASDU self);

/**
 * \brief Set the value of a quality attribute in the ASDU.
 *
 * \param[in] self the Sampled Values ASDU instance.
 * \param[in] index The offset within the data block of the ASDU in bytes.
 * \param[in] value The value which should be set.
 */
LIB61850_API void
SVPublisher_ASDU_setQuality(SVPublisher_ASDU self, int index, Quality value);

/**
 * \brief Set the sample count attribute of the ASDU.
 *
 * \param[in] self the Sampled Values ASDU instance.
 * \param[in] value the new value of the attribute.
 */
LIB61850_API void
SVPublisher_ASDU_setSmpCnt(SVPublisher_ASDU self, uint16_t value);

/**
 * \brief Get the sample count attribute of the ASDU.
 *
 * \param[in] self the Sampled Values ASDU instance.
 */
LIB61850_API uint16_t
SVPublisher_ASDU_getSmpCnt(SVPublisher_ASDU self);

/**
 * \brief Increment the sample count attribute of the ASDU.
 *
 * The parameter SmpCnt shall contain the values of a counter, which is incremented each time a new sample of the
 * analogue value is taken. The sample values shall be kept in the right order. If the counter is used to indicate time
 * consistency of various sampled values, the counter shall be reset by an external synchronization event.
 *
 * \param[in] self the Sampled Values ASDU instance.
 */
LIB61850_API void
SVPublisher_ASDU_increaseSmpCnt(SVPublisher_ASDU self);

/**
 * \brief Set the roll-over (wrap) limit for the sample counter. When reaching the limit the
 *        sample counter will be reset to 0 (default is no limit)
 *
 * \param[in] self the Sampled Values ASDU instance.
 * \param[in] value the new sample counter limit
 */
LIB61850_API void
SVPublisher_ASDU_setSmpCntWrap(SVPublisher_ASDU self, uint16_t value);

/**
 * \brief Enables the transmission of refresh time attribute of the ASDU
 *
 * The refresh time is the time when the data in put into the sampled values buffer
 *
 * \param[in] self the Sampled Values ASDU instance.
 */
LIB61850_API void
SVPublisher_ASDU_enableRefrTm(SVPublisher_ASDU self);

/**
 * \brief Set the refresh time attribute of the ASDU with nanosecond resolution
 *
 * \param[in] self the Sampled Values ASDU instance.
 * \param[in] refrTmNs the refresh time value with nanoseconds resolution.
 */
LIB61850_API void
SVPublisher_ASDU_setRefrTmNs(SVPublisher_ASDU self, nsSinceEpoch refrTmNs);

/**
 * \brief Set the refresh time attribute of the ASDU with millisecond resolution
 *
 * \param[in] self the Sampled Values ASDU instance.
 * \param[in] refrTmNs the refresh time value with with milliseconds resolution.
 */
LIB61850_API void
SVPublisher_ASDU_setRefrTm(SVPublisher_ASDU self, msSinceEpoch refrTm);

/**
 * \brief Set the refresh time attribute of the ASDU
 *
 * NOTE: Using this function you can control the time quality flags and the
 * sub second precision (number of valid bits) of the RefrTm value.
 *
 * \param[in] self the Sampled Values ASDU instance.
 * \param[in] refrTm the refresh time value
 */
LIB61850_API void
SVPublisher_ASDU_setRefrTmByTimestamp(SVPublisher_ASDU self, Timestamp* refrTm);

/**
 * \brief Set the sample mode attribute of the ASDU.
 *
 * The attribute SmpMod shall specify if the sample rate is defined in units of samples per nominal period, samples per
 * second or seconds per sample. If it is missing, the default value is samples per period.
 *
 * NOTE: Function has to be called before calling \ref SVPublisher_setupComplete
 *
 * \param[in] self the Sampled Values ASDU instance.
 * \param[in] smpMod one of IEC61850_SV_SMPMOD_PER_NOMINAL_PERIOD, IEC61850_SV_SMPMOD_SAMPLES_PER_SECOND or
 * IEC61850_SV_SMPMOD_SECONDS_PER_SAMPLE
 */
LIB61850_API void
SVPublisher_ASDU_setSmpMod(SVPublisher_ASDU self, uint8_t smpMod);

/**
 * \brief Set the sample rate attribute of the ASDU.
 *
 * The attribute SmpRate shall specify the sample rate.
 * The value shall be interpreted depending on the value of the SmpMod attribute.
 *
 * NOTE: Function has to be called before calling \ref SVPublisher_setupComplete
 *
 * \param[in] self the Sampled Values ASDU instance.
 * \param[in] smpRate Amount of samples (default per nominal period, see SmpMod).
 */
LIB61850_API void
SVPublisher_ASDU_setSmpRate(SVPublisher_ASDU self, uint16_t smpRate);

/**
 * \brief Set the clock synchronization information
 *
 * Default value is not synchronized (0).
 * Possible values are:
 * 0 = SV are not synchronized by an external clock signal.
 * 1 = SV are synchronized by a clock signal from an unspecified local area clock.
 * 2 = SV are synchronized by a global area clock signal (time traceable).
 * 5 to 254 = SV are synchronized by a clock signal from a local area clock identified by this value.
 * 3;4;255 = Reserved values – Do not use.
 *
 * \param[in] self the Sampled Values ASDU instance.
 * \param[in] smpSynch the clock synchronization state
 */
LIB61850_API void
SVPublisher_ASDU_setSmpSynch(SVPublisher_ASDU self, uint16_t smpSynch);

/**
 * \brief Set the gmIdentity value
 *
 * \param[in] self the Sampled Values ASDU instance.
 * \param[in] gmIdentity the gmIdentity value (octet string/byte array of size 8 bytes)
 */
LIB61850_API void
SVPublisher_ASDU_setGmIdentity(SVPublisher_ASDU self, uint8_t* gmIdentity);

/**@} @}*/

#ifndef DEPRECATED
#if defined(__GNUC__) || defined(__clang__)
#define DEPRECATED __attribute__((deprecated))
#else
#define DEPRECATED
#endif
#endif

/**
 * \addtogroup sv_publisher_deprecated_api_group Deprecated API
 * \ingroup sv_publisher_api_group IEC 61850 Sampled Values (SV) publisher API
 * \deprecated
 * @{
 */

typedef struct sSVPublisher* SampledValuesPublisher;

typedef struct sSV_ASDU* SV_ASDU;

LIB61850_API DEPRECATED SVPublisher
SampledValuesPublisher_create(CommParameters* parameters, const char* interfaceId);

LIB61850_API DEPRECATED SVPublisher_ASDU
SampledValuesPublisher_addASDU(SVPublisher self, char* svID, char* datset, uint32_t confRev);

LIB61850_API DEPRECATED void
SampledValuesPublisher_setupComplete(SVPublisher self);

LIB61850_API DEPRECATED void
SampledValuesPublisher_publish(SVPublisher self);

LIB61850_API DEPRECATED void
SampledValuesPublisher_destroy(SVPublisher self);

LIB61850_API DEPRECATED void
SV_ASDU_resetBuffer(SVPublisher_ASDU self);

LIB61850_API DEPRECATED int
SV_ASDU_addINT8(SVPublisher_ASDU self);

LIB61850_API DEPRECATED void
SV_ASDU_setINT8(SVPublisher_ASDU self, int index, int8_t value);

LIB61850_API DEPRECATED int
SV_ASDU_addINT32(SVPublisher_ASDU self);

LIB61850_API DEPRECATED void
SV_ASDU_setINT32(SVPublisher_ASDU self, int index, int32_t value);

LIB61850_API DEPRECATED int
SV_ASDU_addINT64(SVPublisher_ASDU self);

LIB61850_API DEPRECATED void
SV_ASDU_setINT64(SVPublisher_ASDU self, int index, int64_t value);

LIB61850_API DEPRECATED int
SV_ASDU_addFLOAT(SVPublisher_ASDU self);

LIB61850_API DEPRECATED void
SV_ASDU_setFLOAT(SVPublisher_ASDU self, int index, float value);

LIB61850_API DEPRECATED int
SV_ASDU_addFLOAT64(SVPublisher_ASDU self);

LIB61850_API DEPRECATED void
SV_ASDU_setFLOAT64(SVPublisher_ASDU self, int index, double value);

LIB61850_API DEPRECATED void
SV_ASDU_setSmpCnt(SVPublisher_ASDU self, uint16_t value);

LIB61850_API DEPRECATED uint16_t
SV_ASDU_getSmpCnt(SVPublisher_ASDU self);

LIB61850_API DEPRECATED void
SV_ASDU_increaseSmpCnt(SVPublisher_ASDU self);

LIB61850_API DEPRECATED void
SV_ASDU_setRefrTm(SVPublisher_ASDU self, uint64_t refrTm);

LIB61850_API DEPRECATED void
SV_ASDU_setSmpMod(SVPublisher_ASDU self, uint8_t smpMod);

LIB61850_API DEPRECATED void
SV_ASDU_setSmpRate(SVPublisher_ASDU self, uint16_t smpRate);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif /* LIBIEC61850_SRC_SAMPLED_VALUES_SV_PUBLISHER_H_ */
