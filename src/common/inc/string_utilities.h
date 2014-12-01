/*
 *  string_utilities.h
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

#ifndef STRING_UTILITIES_H_
#define STRING_UTILITIES_H_

#include "libiec61850_platform_includes.h"

char*
copyString(const char* string);

char*
copyStringToBuffer(const char* string, char* buffer);

char*
copySubString(char* startPos, char* endPos);

/**
 * \brief Concatenate strings. count indicates the number of strings
 * to concatenate.
 */
char*
createString(int count, ...);

/**
 * \brief Concatenate strings in user provided buffer. count indicates the number of strings
 * to concatenate.
 */
char*
createStringInBuffer(char* buffer, int count, ...);

char*
createStringFromBuffer(uint8_t* buf, int size);

void
StringUtils_replace(char* string, char oldChar, char newChar);

bool
StringUtils_isDigit(char character);

int
StringUtils_digitToInt(char digit);

int
StringUtils_digitsToInt(char* digits, int count);

int
StringUtils_createBufferFromHexString(char* hexString, uint8_t* buffer);

/**
 * \brief test if string starts with prefix
 */
bool
StringUtils_startsWith(char* string, char* prefix);

#endif /* STRING_UTILITIES_H_ */
