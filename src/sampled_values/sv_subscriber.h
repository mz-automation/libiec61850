/*
 *  sv_subscriber_api.h
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

#ifndef SAMPLED_VALUES_SV_SUBSCRIBER_H_
#define SAMPLED_VALUES_SV_SUBSCRIBER_H_

#include "libiec61850_common_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup sv_subscriber_api_group IEC 61850 sampled values (SV) subscriber API
 *
 * The sampled values (SV) subscriber API consists of three different objects.
 * The \ref SVReceiver object is responsible for handling all SV Ethernet messages
 * for a specific Ethernet interface. If you want to receive SV messages on multiple
 * Ethernet interfaces you have to use several \ref SVReceiver instances.
 * An \ref SVSubscriber object is associated to a SV data stream that is identified by its appID
 * and destination Ethernet address. The \reg SVSubscriber object is used to install a callback
 * handler that is invoked for each ASDU (application service data unit) received for the
 * associated stream. An \ref SVClientASDU is an object that represents a single ASDU. Each ASDU contains
 * some meta information that can be obtained by specific access functions like e.g.
 * \ref SVClientASDU_getSmpCnt to access the "SmpCnt" (sample count) attribute of the ASDU. The actual
 * measurement data contained in the ASDU does not consist of structured ASN.1 data but stored as raw binary
 * data. Without a priori knowledge of the dataset associated with the ASDU data stream it is not
 * possible to interpret the received data correctly. Therefore you have to provide the data access functions
 * with an index value to indicate the data type and the start of the data in the data block of the ASDU.
 * E.g. reading a data set consisting of two FLOAT32 values you can use two subsequent calls of
 * \ref SVClientASDU_getFLOAT32 one with index = 0 and the second one with index = 4.
 *
 *  | IEC 61850 type | required bytes |
 *  | -------------- | -------------- |
 *  | BOOLEAN | 1 byte |
 *  | INT8    | 1 byte |
 *  | INT16   | 2 byte |
 *  | INT32   | 4 byte |
 *  | INT64   | 8 byte |
 *  | INT8U   | 1 byte |
 *  | INT16U  | 2 byte |
 *  | INT24U  | 3 byte |
 *  | INT32U  | 4 byte |
 *  | FLOAT32 | 4 byte |
 *  | FLOAT64 | 8 byte |
 *  | ENUMERATED | 4 byte |
 *  | CODED ENUM | 4 byte |
 *  | OCTET STRING | 20 byte |
 *  | VISIBLE STRING | 35 byte |
 *  | TimeStamp | 8 byte |
 *  | EntryTime | 6 byte |
 *  | BITSTRING | 4 byte |
 *
 * The SV subscriber API can be used independent of the IEC 61850 client API. In order to access the SVCB via MMS you
 * have to use the IEC 61850 client API. Please see \ref ClientSVControlBlock object in section \ref IEC61850_CLIENT_SV.
 *
 */
/**@{*/


/**
 * \brief opaque handle to a SV ASDU (Application service data unit) instance.
 *
 * Sampled Values (SV) ASDUs (application service data units) are the basic units for
 * sampled value data. Each ASDU represents a single sample consisting of multiple measurement
 * values with a single dedicated timestamp.
 *
 * NOTE: SVClientASDU are statically allocated and are only valid inside of the SVUpdateListener
 * function when called by the library. If you need the data contained in the ASDU elsewhere
 * you have to copy and store the data by yourself!
 */
typedef struct sSVClientASDU* SVClientASDU;

/**
 * \brief opaque handle to a SV subscriber instance
 *
 * A subscriber is an instance associated with a single stream of measurement data.  It is identified
 * by the Ethernet destination address, the appID value (both are on SV message level) and the svID value
 * that is part of each ASDU (SVClientASDU object).
 */
typedef struct sSVSubscriber* SVSubscriber;

/**
 * \brief Callback function for received SV messages
 *
 * Will be called for each ASDU contained in a SV message!
 *
 * \param subscriber the subscriber that was associated with the received SV message
 * \param parameter a user provided parameter that is simply passed to the callback
 * \param asdu SV ASDU data structure. This structure is only valid inside of the  callback function
 */
typedef void (*SVUpdateListener)(SVSubscriber subscriber, void* parameter, SVClientASDU asdu);

/**
 * \brief opaque handle to a SV receiver instance
 */
typedef struct sSVReceiver* SVReceiver;

/**
 * \brief Create a new SV receiver instance.
 *
 * A receiver is responsible for processing all SV message for a single Ethernet interface.
 * In order to process messages from multiple Ethernet interfaces you have to create multiple
 * instances.
 *
 * \return the newly created receiver instance
 */
SVReceiver
SVReceiver_create(void);

/**
 * \brief Disable check for destination address of the received SV messages
 *
 * Per default both the appID and the destination address are checked to identify
 * relevant SV messages. Destination address check can be disabled for performance
 * reason when the appIDs are unique in the local system.
 *
 * \param self the receiver instance reference
 */
void
SVReceiver_disableDestAddrCheck(SVReceiver self);

/**
 * \brief Set the Ethernet interface ID for the receiver instance
 *
 * Use this function if you want to use a different interface than
 * the default interface set by CONFIG_ETHERNET_INTERFACE_ID (stack_config.h)
 * NOTE: This function has to be called before calling SVReceiver_start.
 *
 * \param self the receiver instance reference
 * \param interfaceId the Ethernet interface id (platform specific e.g. eth0 for linux).
 */
void
SVReceiver_setInterfaceId(SVReceiver self, const char* interfaceId);

/**
 * \brief Add a subscriber instance to the receiver
 *
 * The given subscriber will be connected to the receiver instance.
 *
 * \param self the receiver instance reference
 * \param subscriber the subscriber instance to connect
 */
void
SVReceiver_addSubscriber(SVReceiver self, SVSubscriber subscriber);

/**
 * \brief Disconnect subscriber and receiver
 *
 * \param self the receiver instance reference
 * \param subscriber the subscriber instance to disconnect
 */
void
SVReceiver_removeSubscriber(SVReceiver self, SVSubscriber subscriber);

/**
 * \brief Receiver starts listening for SV messages.
 *
 * NOTE: This call will start a new background thread.
 *
 * \param self the receiver instance reference
 */
void
SVReceiver_start(SVReceiver self);

/**
 * \brief Receiver stops listening for SV messages
 *
 * \param self the receiver instance reference
 */
void
SVReceiver_stop(SVReceiver self);

/**
 * \brief Destroy receiver instance (cleanup resources)
 *
 * \param self the receiver instance reference
 */
void
SVReceiver_destroy(SVReceiver self);

/***************************************
 * Functions for non-threaded operation
 ***************************************/

void
SVReceiver_startThreadless(SVReceiver self);

void
SVReceiver_stopThreadless(SVReceiver self);

bool
SVReceiver_tick(SVReceiver self);

/*
 * Subscriber
 */

SVSubscriber
SVSubscriber_create(const uint8_t* ethAddr, uint16_t appID);

/**
 * \brief Set a callback handler to process received SV messages
 *
 * If the received SV message contains multiple ASDUs (application service data units) the callback
 * function will be called for each ASDU separately. If a callback function has already been installed
 * for this SVSubscriber object the old callback will be replaced.
 *
 * \param self The subscriber object
 * \param listener the callback function to install
 * \param a user provided parameter that is provided to the callback function
 *
 */
void
SVSubscriber_setListener(SVSubscriber self,  SVUpdateListener listener, void* parameter);

void
SVSubscriber_destroy(SVSubscriber self);

/*************************************************************************
 * SVClientASDU object methods
 **************************************************************************/

/**
 * \brief return the SmpCnt value included in the SV ASDU
 *
 * The SmpCnt (sample counter) is increased for each ASDU to
 * identify the sample.
 *
 * \param self ASDU object instance
 */
uint16_t
SVClientASDU_getSmpCnt(SVClientASDU self);

/**
 * \brief return the SvID value included in the SV ASDU
 *
 * \param self ASDU object instance
 */
const char*
SVClientASDU_getSvId(SVClientASDU self);

/**
 * \brief return the ConfRev value included in the SV ASDU
 *
 * \param self ASDU object instance
 */
uint32_t
SVClientASDU_getConfRev(SVClientASDU self);

/**
 * \brief Get an INT8 data value in the data part of the ASDU
 *
 * \param self ASDU object instance
 * \param index the index (byte position of the start) of the data in the data part
 *
 * \return SV data
 */
int8_t
SVClientASDU_getINT8(SVClientASDU self, int index);

/**
 * \brief Get an INT16 data value in the data part of the ASDU
 *
 * \param self ASDU object instance
 * \param index the index (byte position of the start) of the data in the data part
 *
 * \return SV data
 */
int16_t
SVClientASDU_getINT16(SVClientASDU self, int index);

/**
 * \brief Get an INT32 data value in the data part of the ASDU
 *
 * \param self ASDU object instance
 * \param index the index (byte position of the start) of the data in the data part
 *
 * \return SV data
 */
int32_t
SVClientASDU_getINT32(SVClientASDU self, int index);

/**
 * \brief Get an INT8U data value in the data part of the ASDU
 *
 * \param self ASDU object instance
 * \param index the index (byte position of the start) of the data in the data part
 *
 * \return SV data
 */
uint8_t
SVClientASDU_getINT8U(SVClientASDU self, int index);

/**
 * \brief Get an INT16U data value in the data part of the ASDU
 *
 * \param self ASDU object instance
 * \param index the index (byte position of the start) of the data in the data part
 *
 * \return SV data
 */
uint16_t
SVClientASDU_getINT16U(SVClientASDU self, int index);

/**
 * \brief Get an INT32U data value in the data part of the ASDU
 *
 * \param self ASDU object instance
 * \param index the index (byte position of the start) of the data in the data part
 *
 * \return SV data
 */
uint32_t
SVClientASDU_getINT32U(SVClientASDU self, int index);

/**
 * \brief Get an FLOAT32 data value in the data part of the ASDU
 *
 * \param self ASDU object instance
 * \param index the index (byte position of the start) of the data in the data part
 *
 * \return SV data
 */
float
SVClientASDU_getFLOAT32(SVClientASDU self, int index);

/**
 * \brief Get an FLOAT64 data value in the data part of the ASDU
 *
 * \param self ASDU object instance
 * \param index the index (byte position of the start) of the data in the data part
 *
 * \return SV data
 */
double
SVClientASDU_getFLOAT64(SVClientASDU self, int index);

/**
 * \brief Returns the size of the data part of the ASDU
 *
 * \param self ASDU object instance
 *
 * \return size of the ASDU data part in bytes.
 */
int
SVClientASDU_getDataSize(SVClientASDU self);

/**@}*/

#ifdef __cplusplus
}
#endif


#endif /* SAMPLED_VALUES_SV_SUBSCRIBER_ */
