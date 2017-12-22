/*
 *  ber_encoder.h
 *
 *  Copyright 2013 Michael Zillgith
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

#ifndef BER_ENCODER_H_
#define BER_ENCODER_H_

#include "libiec61850_platform_includes.h"
#include "asn1_ber_primitive_value.h"

/*
 * encoding functions
 *
 * Encoding to buffer starts at bufPos. The return value is the position in the buffer
 * of the next entity to encode.
 */

int
BerEncoder_encodeLength(uint32_t length, uint8_t* buffer, int bufPos);

int
BerEncoder_encodeTL(uint8_t tag, uint32_t length, uint8_t* buffer, int bufPos);

int
BerEncoder_encodeBoolean(uint8_t tag, bool value, uint8_t* buffer, int bufPos);

int
BerEncoder_encodeStringWithTag(uint8_t tag, const char* string, uint8_t* buffer, int bufPos);

int
BerEncoder_encodeOctetString(uint8_t tag, uint8_t* octetString, uint32_t octetStringSize, uint8_t* buffer, int bufPos);

int
BerEncoder_encodeAsn1PrimitiveValue(uint8_t tag, Asn1PrimitiveValue* value, uint8_t* buffer, int bufPos);

int
BerEncoder_encodeUInt32(uint32_t value, uint8_t* buffer, int bufPos);

int
BerEncoder_encodeInt32(int32_t value, uint8_t* buffer, int bufPos);

int
BerEncoder_encodeUInt32WithTL(uint8_t tag, uint32_t value, uint8_t* buffer, int bufPos);

int
BerEncoder_encodeBitString(uint8_t tag, int bitStringSize, uint8_t* bitString, uint8_t* buffer, int bufPos);

int
BerEncoder_determineEncodedBitStringSize(int bitStringSize);

int
BerEncoder_encodeFloat(uint8_t* floatValue, uint8_t formatWidth, uint8_t exponentWidth,
        uint8_t* buffer, int bufPos);

/*
 * functions to determine size of encoded entities.
 */

int
BerEncoder_UInt32determineEncodedSize(uint32_t value);

int
BerEncoder_determineLengthSize(uint32_t length);

int
BerEncoder_determineEncodedStringSize(const char* string);

int
BerEncoder_determineEncodedBitStringSize(int bitStringSize);

/*
 * helper functions
 */

int
BerEncoder_encodeOIDToBuffer(const char* oidString, uint8_t* buffer, int maxBufLen);

void
BerEncoder_revertByteOrder(uint8_t* octets, const int size);

int
BerEncoder_compressInteger(uint8_t* integer, int originalSize);

#endif /* BER_ENCODER_H_ */
