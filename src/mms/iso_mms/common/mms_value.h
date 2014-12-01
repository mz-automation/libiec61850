/*
 *  mms_value.h
 *
 *  Copyright 2013 Michael Zillgith
 *
 *	This file is part of libIEC61850.
 *
 *	libIEC61850 is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	libIEC61850 is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with libIEC61850.  If not, see <http://www.gnu.org/licenses/>.
 *
 *	See COPYING file for the complete license text.
 */

#ifndef MMS_VALUE_H_
#define MMS_VALUE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "libiec61850_common_api.h"
#include "mms_common.h"
#include "mms_types.h"
#include "ber_integer.h"

/**
 * \defgroup common_api_group libIEC61850 API common parts
 */
/**@{*/

/**
 * \defgroup MMS_VALUE MmsValue data type definition and handling functions
 */
/**@{*/


typedef enum ATTRIBUTE_PACKED {
    DATA_ACCESS_ERROR_NO_RESPONSE = -2, /* for server internal purposes only! */
    DATA_ACCESS_ERROR_SUCCESS = -1,
    DATA_ACCESS_ERROR_OBJECT_INVALIDATED = 0,
    DATA_ACCESS_ERROR_HARDWARE_FAULT = 1,
    DATA_ACCESS_ERROR_TEMPORARILY_UNAVAILABLE = 2,
    DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED = 3,
    DATA_ACCESS_ERROR_OBJECT_UNDEFINED = 4,
    DATA_ACCESS_ERROR_INVALID_ADDRESS = 5,
    DATA_ACCESS_ERROR_TYPE_UNSUPPORTED = 6,
    DATA_ACCESS_ERROR_TYPE_INCONSISTENT = 7,
    DATA_ACCESS_ERROR_OBJECT_ATTRIBUTE_INCONSISTENT = 8,
    DATA_ACCESS_ERROR_OBJECT_ACCESS_UNSUPPORTED = 9,
    DATA_ACCESS_ERROR_OBJECT_NONE_EXISTENT = 10,
    DATA_ACCESS_ERROR_OBJECT_VALUE_INVALID = 11,
    DATA_ACCESS_ERROR_UNKNOWN = 12
} MmsDataAccessError;

/**
 * MmsValue - complex value type for MMS Client API
 */
typedef struct sMmsValue MmsValue;

/*************************************************************************************
 *  Array functions
 *************************************************************************************/

/**
 * \brief Create an Array and initialize elements with default values.
 *
 * \param elementType type description for the elements the new array
 * \param size the size of the new array
 *
 * \return a newly created array instance
 */
MmsValue*
MmsValue_createArray(MmsVariableSpecification* elementType, int size);

/**
 * \brief Get the size of an array.
 *
 * \param self MmsValue instance to operate on. Has to be of type MMS_ARRAY.
 *
 * \return the size of the array
 */
uint32_t
MmsValue_getArraySize(MmsValue* self);

/**
 * \brief Get an element of an array or structure.
 *
 * \param self MmsValue instance to operate on. Has to be of type MMS_ARRAY or MMS_STRUCTURE.
 * \param index ndex of the requested array or structure element
 *
 * \return the element object
 */
MmsValue*
MmsValue_getElement(MmsValue* array, int index);

/**
 * \brief Create an emtpy array.
 *
 * \param size the size of the new array
 *
 * \return a newly created empty array instance
 */
MmsValue*
MmsValue_createEmtpyArray(int size);

void
MmsValue_setElement(MmsValue* complexValue, int index, MmsValue* elementValue);


/*************************************************************************************
 * Basic type functions
 *************************************************************************************/

MmsDataAccessError
MmsValue_getDataAccessError(MmsValue* self);

/**
 * \brief Get the int64_t value of a MmsValue object.
 *
 * \param self MmsValue instance to operate on. Has to be of a type MMS_INTEGER or MMS_UNSIGNED
 *
 * \return signed 64 bit integer
 */
int64_t
MmsValue_toInt64(MmsValue* self);

/**
 * \brief Get the int32_t value of a MmsValue object.
 *
 * \param self MmsValue instance to operate on. Has to be of a type MMS_INTEGER or MMS_UNSIGNED
 *
 * \return signed 32 bit integer
 */
int32_t
MmsValue_toInt32(MmsValue* value);

/**
 * \brief Get the uint32_t value of a MmsValue object.
 *
 * \param self MmsValue instance to operate on. Has to be of a type MMS_INTEGER or MMS_UNSIGNED
 *
 * \return unsigned 32 bit integer
 */
uint32_t
MmsValue_toUint32(MmsValue* value);

/**
 * \brief Get the double value of a MmsValue object.
 *
 * \param self MmsValue instance to operate on. Has to be of type MMS_FLOAT.
 *
 * \return 64 bit floating point value
 */
double
MmsValue_toDouble(MmsValue* self);

/**
 * \brief Get the float value of a MmsValue object.
 *
 * \param self MmsValue instance to operate on. Has to be of type MMS_FLOAT.
 *
 * \return 32 bit floating point value
 */
float
MmsValue_toFloat(MmsValue* self);

/**
 * \brief Get the unix timestamp of a MmsValue object of type MMS_UTCTIME.
 *
 * \param self MmsValue instance to operate on. Has to be of a type MMS_UTC_TIME.
 *
 * \return unix timestamp of the MMS_UTCTIME variable.
 */
uint32_t
MmsValue_toUnixTimestamp(MmsValue* self);

/**
 * \brief Set the float value of a MmsValue object.
 *
 * \param self MmsValue instance to operate on. Has to be of a type MMS_FLOAT.
 */
void
MmsValue_setFloat(MmsValue* self, float newFloatValue);

/**
 * \brief Set the double value of a MmsValue object.
 *
 * \param self MmsValue instance to operate on. Has to be of a type MMS_FLOAT.
 */
void
MmsValue_setDouble(MmsValue* self, double newFloatValue);

/**
 * \brief Set the Int8 value of a MmsValue object.
 *
 * \param self MmsValue instance to operate on. Has to be of a type MMS_INTEGER.
 * \param integer the new value to set
 */
void
MmsValue_setInt8(MmsValue* value, int8_t integer);

/**
 * \brief Set the Int16 value of a MmsValue object.
 *
 * \param self MmsValue instance to operate on. Has to be of a type MMS_INTEGER.
 * \param integer the new value to set
 */
void
MmsValue_setInt16(MmsValue* value, int16_t integer);

/**
 * \brief Set the Int32 value of a MmsValue object.
 *
 * \param self MmsValue instance to operate on. Has to be of a type MMS_INTEGER.
 * \param integer the new value to set
 */
void
MmsValue_setInt32(MmsValue* self, int32_t integer);

/**
 * \brief Set the Int64 value of a MmsValue object.
 *
 * \param self MmsValue instance to operate on. Has to be of a type MMS_INTEGER.
 * \param integer the new value to set
 */
void
MmsValue_setInt64(MmsValue* value, int64_t integer);

/**
 * \brief Set the UInt8 value of a MmsValue object.
 *
 * \param self MmsValue instance to operate on. Has to be of a type MMS_UNSIGNED.
 * \param integer the new value to set
 */
void
MmsValue_setUint8(MmsValue* value, uint8_t integer);

/**
 * \brief Set the UInt16 value of a MmsValue object.
 *
 * \param self MmsValue instance to operate on. Has to be of a type MMS_UNSIGNED.
 * \param integer the new value to set
 */
void
MmsValue_setUint16(MmsValue* value, uint16_t integer);

/**
 * \brief Set the UInt32 value of a MmsValue object.
 *
 * \param self MmsValue instance to operate on. Has to be of a type MMS_UNSIGNED.
 * \param integer the new value to set
 */
void
MmsValue_setUint32(MmsValue* value, uint32_t integer);


/**
 * \brief Set the bool value of a MmsValue object.
 *
 * \param self MmsValue instance to operate on. Has to be of a type MMS_BOOLEAN.
 * \param boolValue a bool value
 */
void
MmsValue_setBoolean(MmsValue* value, bool boolValue);

/**
 * \brief Get the bool value of a MmsValue object.
 *
 * \param self MmsValue instance to operate on. Has to be of a type MMS_BOOLEAN.
 * \return  the MmsValue value as bool value
 */
bool
MmsValue_getBoolean(MmsValue* value);

char*
MmsValue_toString(MmsValue* self);

void
MmsValue_setVisibleString(MmsValue* self, char* string);


/**
 * \brief Set a single bit (set to one) of an MmsType object of type MMS_BITSTRING
 *
 * \param self MmsValue instance to operate on. Has to be of a type MMS_BITSTRING.
 * \param bitPos the position of the bit in the bit string. Starting with 0. The bit
 *        with position 0 is the first bit if the MmsValue instance is serialized.
 * \param value the new value of the bit (true = 1 / false = 0)
 */
void
MmsValue_setBitStringBit(MmsValue* self, int bitPos, bool value);

/**
 * \brief Get the value of a single bit (set to one) of an MmsType object of type MMS_BITSTRING
 *
 * \param self MmsValue instance to operate on. Has to be of a type MMS_BITSTRING.
 * \param bitPos the position of the bit in the bit string. Starting with 0. The bit
 *        with position 0 is the first bit if the MmsValue instance is serialized.
 * \return the value of the bit (true = 1 / false = 0)
 */
bool
MmsValue_getBitStringBit(MmsValue* self, int bitPos);

/**
 * \brief Delete all bits (set to zero) of an MmsType object of type MMS_BITSTRING
 *
 * \param self MmsValue instance to operate on. Has to be of a type MMS_BITSTRING.
 */
void
MmsValue_deleteAllBitStringBits(MmsValue* self);


/**
 * \brief Get the size of a bit string in bits.
 *
 * \param self MmsValue instance to operate on. Has to be of a type MMS_BITSTRING.
 */
int
MmsValue_getBitStringSize(MmsValue* self);

/**
 * \brief Get the number of bytes required by this bitString
 *
 * \param self MmsValue instance to operate on. Has to be of a type MMS_BITSTRING.
 */
int
MmsValue_getBitStringByteSize(MmsValue* self);

/**
 * \brief Count the number of set bits in a bit string.
 *
 * \param self MmsValue instance to operate on. Has to be of a type MMS_BITSTRING.
 */
int
MmsValue_getNumberOfSetBits(MmsValue* self);

/**
 * Set all bits (set to one) of an MmsType object of type MMS_BITSTRING
 *
 * \param self MmsValue instance to operate on. Has to be of a type MMS_BITSTRING.
 */
void
MmsValue_setAllBitStringBits(MmsValue* self);

/**
 * \brief Convert a bit string to an unsigned integer
 *
 * \param self MmsValue instance to operate on. Has to be of a type MMS_BITSTRING.
 */
uint32_t
MmsValue_getBitStringAsInteger(MmsValue* self);

/**
 * \brief Convert an unsigned integer to a bit string
 *
 * \param self MmsValue instance to operate on. Has to be of a type MMS_BITSTRING.
 * \param intValue the integer value that is used to set the bit string
 */
void
MmsValue_setBitStringFromInteger(MmsValue* self, uint32_t intValue);

/**
 * \brief Update an MmsValue object of UtcTime type with a timestamp in s
 *
 * \param self MmsValue instance to operate on. Has to be of a type MMS_BOOLEAN.
 * \param timeval the new value in seconds since epoch (1970/01/01 00:00 UTC)
 */
MmsValue*
MmsValue_setUtcTime(MmsValue* self, uint32_t timeval);

/**
 * \brief Update an MmsValue object of type MMS_UTCTIME with a millisecond time.
 *
 * \param self MmsValue instance to operate on. Has to be of a type MMS_UTCTIME.
 * \param timeval the new value in milliseconds since epoch (1970/01/01 00:00 UTC)
 */
MmsValue*
MmsValue_setUtcTimeMs(MmsValue* self, uint64_t timeval);

/**
 * \brief Update an MmsValue object of type MMS_UTCTIME with a buffer containing a BER encoded UTCTime.
 *
 * The buffer must have a size of 8 bytes!
 *
 * \param self MmsValue instance to operate on. Has to be of a type MMS_UTCTIME.
 * \param buffer buffer containing the encoded UTCTime.
 */
void
MmsValue_setUtcTimeByBuffer(MmsValue* self, uint8_t* buffer);

/**
 * \brief Get a millisecond time value from an MmsValue object of MMS_UTCTIME type.
 *
 * \param self MmsValue instance to operate on. Has to be of a type MMS_UTCTIME.
 *
 * \return the value in milliseconds since epoch (1970/01/01 00:00 UTC)
 */
uint64_t
MmsValue_getUtcTimeInMs(MmsValue* value);

/**
 * \brief set the TimeQuality byte of the UtcTime
 *
 * Meaning of the bits in the timeQuality byte:
 *
 * bit 7 = leapSecondsKnown
 * bit 6 = clockFailure
 * bit 5 = clockNotSynchronized
 * bit 0-4 = subsecond time accuracy (number of significant bits of subsecond time)
 *
 * \param self MmsValue instance to operate on. Has to be of a type MMS_UTCTIME.
 *
 * \param timeQuality the byte representing the time quality
 */
void
MmsValue_setUtcTimeQuality(MmsValue* self, uint8_t timeQuality);

/**
 * \brief get the TimeQuality byte of the UtcTime
 *
 * Meaning of the bits in the timeQuality byte:
 *
 * bit 7 = leapSecondsKnown
 * bit 6 = clockFailure
 * bit 5 = clockNotSynchronized
 * bit 0-4 = subsecond time accuracy (number of significant bits of subsecond time)
 *
 * \param self MmsValue instance to operate on. Has to be of a type MMS_UTCTIME.
 *
 * \return the byte representing the time quality
 */
uint8_t
MmsValue_getUtcTimeQuality(MmsValue* self);

/**
 * \brief Update an MmsValue object of type MMS_BINARYTIME with a millisecond time.
 *
 * \param self MmsValue instance to operate on. Has to be of a type MMS_UTCTIME.
 * \param timeval the new value in milliseconds since epoch (1970/01/01 00:00 UTC)
 */
void
MmsValue_setBinaryTime(MmsValue* self, uint64_t timestamp);

/**
 * \brief Get a millisecond time value from an MmsValue object of type MMS_BINARYTIME.
 *
 * \param self MmsValue instance to operate on. Has to be of a type MMS_BINARYTIME.
 *
 * \return the value in milliseconds since epoch (1970/01/01 00:00 UTC)
 */
uint64_t
MmsValue_getBinaryTimeAsUtcMs(MmsValue* self);

/**
 * \brief Set the value of an MmsValue object of type MMS_OCTET_STRING.
 *
 * This method will copy the provided buffer to the internal buffer of the
 * MmsValue instance. This will only happen if the internal buffer size is large
 * enough for the new value.
 *
 * \param self MmsValue instance to operate on. Has to be of a type MMS_OCTET_STRING.
 * \param buf the buffer that contains the new value
 * \param size the size of the buffer that contains the new value
 */
void
MmsValue_setOctetString(MmsValue* self, uint8_t* buf, int size);

/**
 * \brief Returns the size in bytes of an MmsValue object of type MMS_OCTET_STRING.
 *
 * \param self MmsValue instance to operate on. Has to be of a type MMS_OCTET_STRING.
 *
 * \return size in bytes
 */
uint16_t
MmsValue_getOctetStringSize(MmsValue* self);

/**
 * \brief Returns the maximum size in bytes of an MmsValue object of type MMS_OCTET_STRING.
 *
 * Returns the maximum size if bytes of the MmsValue object of type MMS_OCTET_STRING. This
 * is the size of the internally allocated buffer to hold the octet string data.
 *
 * \param self MmsValue instance to operate on. Has to be of a type MMS_OCTET_STRING.
 *
 * \return maximum size in bytes
 */
uint16_t
MmsValue_getOctetStringMaxSize(MmsValue* self);

/**
 * \brief Returns the reference to the internally hold buffer of an MmsValue object of type MMS_OCTET_STRING.
 *
 * \param self MmsValue instance to operate on. Has to be of a type MMS_OCTET_STRING.
 *
 * \return reference to the buffer
 */
uint8_t*
MmsValue_getOctetStringBuffer(MmsValue* self);

/**
 * \brief Update the value of an MmsValue instance by the value of another MmsValue instance.
 *
 * Both instances should be of same time. E.g. is self is of type MMS_INTEGER then
 * source has also to be of type MMS_INTEGER. Otherwise the call will have no effect.
 *
 * \param self MmsValue instance to operate on.
 * \param source MmsValue used as source for the update. Has to be of same type as self
 *
 * \return indicates if the update has been successful (false if not)
 */
bool
MmsValue_update(MmsValue* self, MmsValue* source);

/**
 * \brief Check if two instances of MmsValue have the same value.
 *
 * Both instances should be of same type. E.g. is self is of type MMS_INTEGER then
 * source has also to be of type MMS_INTEGER. Otherwise the call will return false.
 *
 * \param self MmsValue instance to operate on.
 * \param otherValue MmsValue that is used to test
 *
 * \return true if both instances are of the same type and have the same value
 */
bool
MmsValue_equals(MmsValue* self, MmsValue* otherValue);

/**
 * \brief Check if two (complex) instances of MmsValue have the same type.
 *
 * This function will test if the two values are of the same type. The function
 * will return true only if all child instances in the MmsValue instance tree are
 * also of equal type.
 *
 * \param self MmsValue instance to operate on.
 * \param otherValue MmsValue that is used to test
 *
 * \return true if both instances and all their children are of the same type.
 */
bool
MmsValue_equalTypes(MmsValue* self, MmsValue* otherValue);

/*************************************************************************************
 * Constructors and destructors
 *************************************************************************************/


MmsValue*
MmsValue_newDataAccessError(MmsDataAccessError accessError);

MmsValue*
MmsValue_newIntegerFromBerInteger(Asn1PrimitiveValue* berInteger);

MmsValue*
MmsValue_newUnsignedFromBerInteger(Asn1PrimitiveValue* berInteger);

MmsValue*
MmsValue_newInteger(int size);

MmsValue*
MmsValue_newUnsigned(int size);

MmsValue*
MmsValue_newBoolean(bool boolean);

/**
 * \brief Create a new MmsValue instance of type MMS_BITSTRING.
 *
 * \param bitSize the size of the bit string in bit
 *
 * \return new MmsValue instance of type MMS_BITSTRING
 */
MmsValue*
MmsValue_newBitString(int bitSize);

MmsValue*
MmsValue_newOctetString(int size, int maxSize);

MmsValue*
MmsValue_newStructure(MmsVariableSpecification* typeSpec);

MmsValue*
MmsValue_createEmptyStructure(int size);

MmsValue*
MmsValue_newDefaultValue(MmsVariableSpecification* typeSpec);

MmsValue*
MmsValue_newIntegerFromInt8(int8_t integer);

MmsValue*
MmsValue_newIntegerFromInt16(int16_t integer);

MmsValue*
MmsValue_newIntegerFromInt32(int32_t integer);

MmsValue*
MmsValue_newIntegerFromInt64(int64_t integer);

MmsValue*
MmsValue_newUnsignedFromUint32(uint32_t integer);

MmsValue*
MmsValue_newFloat(float variable);

MmsValue*
MmsValue_newDouble(double variable);

/**
 * \brief Create a (deep) copy of an MmsValue instance
 *
 * This operation will allocate dynamic memory. It is up to the caller to
 * free this memory by calling MmsValue_delete() later.
 *
 * \param self the MmsValue instance that will be cloned
 *
 * \return an MmsValue instance that is an exact copy of the given instance.
 */
MmsValue*
MmsValue_clone(MmsValue* self);

/**
 * \brief Create a (deep) copy of an MmsValue instance in a user provided buffer
 *
 * This operation copies the give MmsValue instance to a user provided buffer.
 *
 * \param self the MmsValue instance that will be cloned
 * \param destinationAddress the start address of the user provided buffer
 *
 * \return an MmsValue instance that is an exact copy of the given instance.
 */
uint8_t*
MmsValue_cloneToBuffer(MmsValue* self, uint8_t* destinationAddress);

/**
 * \brief Determine the required amount of bytes by a clone.
 *
 * This function is intended to be used to determine the buffer size of a clone operation
 * (MmsValue_cloneToBuffer) in advance.
 *
 * \param self the MmsValue instance
 *
 * \return the number of bytes required by a clone
 */
int
MmsValue_getSizeInMemory(MmsValue* self);

/**
 * \brief Delete an MmsValue instance.
 *
 * This operation frees all dynamically allocated memory of the MmsValue instance.
 * If the instance is of type MMS_STRUCTURE or MMS_ARRAY all child elements will
 * be deleted too.
 *
 * \param self the MmsValue instance to be deleted.
 */
void
MmsValue_delete(MmsValue* self);

/**
 * \brief Delete an MmsValue instance.
 *
 * This operation frees all dynamically allocated memory of the MmsValue instance.
 * If the instance is of type MMS_STRUCTURE or MMS_ARRAY all child elements will
 * be deleted too.
 *
 * NOTE: this functions only frees the instance if deleteValue field = 0!
 *
 *
 * \param self the MmsValue instance to be deleted.
 */
void
MmsValue_deleteConditional(MmsValue* value);

/**
 * \brief Delete an MmsValue instance if the provided pointer is not NULL
 *
 * This operation frees all dynamically allocated memory of the MmsValue instance.
 * If the instance is of type MMS_STRUCTURE or MMS_ARRAY all child elements will
 * be deleted too.
 *
 * \param self the MmsValue instance to be deleted.
 */
void
MmsValue_deleteIfNotNull(MmsValue* value);

/**
 * \brief Create a new MmsValue instance of type MMS_VISIBLE_STRING.
 *
 * This function will allocate as much memory as required to hold the string and sets the maximum size of
 * the string to this size.
 *
 * \param string a text string that should be the value of the new instance of NULL for an empty string.
 *
 * \return new MmsValue instance of type MMS_VISIBLE_STRING
 */
MmsValue*
MmsValue_newVisibleString(char* string);

/**
 * \brief Create a new MmsValue instance of type MMS_VISIBLE_STRING.
 *
 * This function will create a new empty MmsValue string object. The maximum size of the string is set
 * according to the size parameter. The function allocates as much memory as is required to hold a string
 * of the maximum size.
 *
 * \param size the new maximum size of the string.
 *
 * \return new MmsValue instance of type MMS_VISIBLE_STRING
 */
MmsValue*
MmsValue_newVisibleStringWithSize(int size);

/**
 * \brief Create a new MmsValue instance of type MMS_STRING.
 *
 * This function will create a new empty MmsValue string object. The maximum size of the string is set
 * according to the size parameter. The function allocates as much memory as is required to hold a string
 * of the maximum size.
 *
 * \param size the new maximum size of the string.
 *
 * \return new MmsValue instance of type MMS_STRING
 */
MmsValue*
MmsValue_newMmsStringWithSize(int size);

/**
 * \brief Create a new MmsValue instance of type MMS_BINARYTIME.
 *
 * If the timeOfDay parameter is set to true then the resulting
 * MMS_BINARYTIME object is only 4 octets long and includes only
 * the seconds since midnight. Otherwise the MMS_BINARYTIME
 *
 * \param timeOfDay if true only the TimeOfDay value is included.
 *
 * \return new MmsValue instance of type MMS_BINARYTIME
 */
MmsValue*
MmsValue_newBinaryTime(bool timeOfDay);

/**
 * \brief Create a new MmsValue instance of type MMS_VISIBLE_STRING from the specified byte array
 *
 * \param byteArray the byte array containing the string data
 * \param size the size of the byte array
 *
 * \return new MmsValue instance of type MMS_VISIBLE_STRING
 */
MmsValue*
MmsValue_newVisibleStringFromByteArray(uint8_t* byteArray, int size);

/**
 * \brief Create a new MmsValue instance of type MMS_STRING from the specified byte array
 *
 * \param byteArray the byte array containing the string data
 * \param size the size of the byte array
 *
 * \return new MmsValue instance of type MMS_STRING
 */
MmsValue*
MmsValue_newMmsStringFromByteArray(uint8_t* byteArray, int size);

/**
 * \brief Create a new MmsValue instance of type MMS_STRING.
 *
 * \param string a text string that should be the value of the new instance of NULL for an empty string.
 *
 * \return new MmsValue instance of type MMS_STRING
 */
MmsValue*
MmsValue_newMmsString(char* string);

void
MmsValue_setMmsString(MmsValue* value, char* string);

/**
 * \brief Create a new MmsValue instance of type MMS_UTCTIME.
 *
 * \param timeval time value as UNIX timestamp (seconds since epoch)
 *
 * \return new MmsValue instance of type MMS_UTCTIME
 */
MmsValue*
MmsValue_newUtcTime(uint32_t timeval);

/**
 * \brief Create a new MmsValue instance of type MMS_UTCTIME.
 *
 * \param timeval time value as millisecond timestamp (milliseconds since epoch)
 *
 * \return new MmsValue instance of type MMS_UTCTIME
 */
MmsValue*
MmsValue_newUtcTimeByMsTime(uint64_t timeval);


void
MmsValue_setDeletable(MmsValue* self);

void
MmsValue_setDeletableRecursive(MmsValue* value);

/**
 * \brief Check if the MmsValue instance has the deletable flag set.
 *
 * The deletable flag indicates if an libiec61850 API client should call the
 * MmsValue_delete() method or not if the MmsValue instance was passed to the
 * client by an API call or callback method.
 *
 * \param self the MmsValue instance
 *
 * \return 1 if deletable flag is set, otherwise 0
 */
int
MmsValue_isDeletable(MmsValue* self);

/**
 * \brief Get the MmsType of an MmsValue instance
 *
 * \param self the MmsValue instance
 */
MmsType
MmsValue_getType(MmsValue* self);

/**
 * \brief Get a sub-element of a MMS_STRUCTURE value specified by a path name.
 *
 * \param self the MmsValue instance
 * \param varSpec - type specification if the MMS_STRUCTURE value
 * \param mmsPath - path (in MMS variable name syntax) to specify the sub element.
 *
 * \return the sub elements MmsValue instance or NULL if the element does not exist
 */
MmsValue*
MmsValue_getSubElement(MmsValue* self, MmsVariableSpecification* varSpec, char* mmsPath);

/**
 * \brief return the value type as a human readable string
 *
 * \param self the MmsValue instance
 *
 * \return the value type as a human readable string
 */
char*
MmsValue_getTypeString(MmsValue* self);

char*
MmsValue_printToBuffer(MmsValue* self, char* buffer, int bufferSize);

/**@}*/

/**@}*/

#ifdef __cplusplus
}
#endif

#endif /* MMS_VALUE_H_ */
