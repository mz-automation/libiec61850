/*
 *  string_utilities.c
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

#include "string_utilities.h"

char*
copySubString(char* startPos, char* endPos)
{
	int newStringLength = endPos - startPos;

	char* newString = (char*) GLOBAL_MALLOC(newStringLength) + 1;

	memcpy(newString, startPos, newStringLength);

	newString[newStringLength] = 0;

	return newString;
}

char*
copyString(const char* string)
{
	int newStringLength = strlen(string) + 1;

	char* newString = (char*) GLOBAL_MALLOC(newStringLength);

	memcpy(newString, string, newStringLength);

	return newString;
}

char*
copyStringToBuffer(const char* string, char* buffer)
{
    int newStringLength = strlen(string) + 1;

    memcpy(buffer, string, newStringLength);

    return buffer;
}


char*
createStringFromBuffer(uint8_t* buf, int size)
{
	char* newStr = (char*) GLOBAL_MALLOC(size + 1);

	memcpy(newStr, buf, size);
	newStr[size] = 0;

	return newStr;
}


char*
createStringInBuffer(char* newStr, int count, ...)
{
    va_list ap;
    char* currentPos = newStr;
    int i;

    va_start(ap, count);
    for (i = 0; i < count; i++) {
        char* str = va_arg(ap, char*);
        strcpy(currentPos, str);
        currentPos += strlen(str);
    }
    va_end(ap);

    *currentPos = 0;

    return newStr;
}

char*
createString(int count, ...)
{
	va_list ap;
	char* newStr;
	char* currentPos;
	int newStringLength = 0;
	int i;

	/* Calculate new string length */
	va_start(ap, count);
	for (i = 0; i < count; i++) {
		char* str = va_arg(ap, char*);

		newStringLength += strlen(str);
	}
	va_end(ap);

	newStr = (char*) GLOBAL_MALLOC(newStringLength + 1);
	currentPos = newStr;


	va_start(ap, count);
	for (i = 0; i < count; i++) {
		char* str = va_arg(ap, char*);
		strcpy(currentPos, str);
		currentPos += strlen(str);
	}
	va_end(ap);

	*currentPos = 0;

	return newStr;
}

void
StringUtils_replace(char* string, char oldChar, char newChar)
{
    int len = strlen(string);
    int i;

    for (i = 0; i < len; i++){
        if (string[i] == oldChar)
            string[i] = newChar;
    }
}

bool
StringUtils_isDigit(char character)
{
	if ((character > 47) && (character < 58))
		return true;
	else
		return false;
}

int
StringUtils_digitToInt(char digit)
{
	if (StringUtils_isDigit(digit)) {
		return (digit - 48);
	}
	else
		return -1;
}

int
StringUtils_digitsToInt(char* digits, int count)
{
	int i = 0;
	int value = 0;

	while (i < count) {
		value = value * 10;

		int digitValue = StringUtils_digitToInt(*(digits + i));

		if (digitValue == -1)
			return -1;

		value += digitValue;

		i++;
	}

	return value;
}

static int
toInt(char c)
{
  if (c >= '0' && c <= '9') return      c - '0';
  if (c >= 'A' && c <= 'F') return 10 + c - 'A';
  if (c >= 'a' && c <= 'f') return 10 + c - 'a';
  return -1;
}

int
StringUtils_createBufferFromHexString(char* hexString, uint8_t* buffer)
{
    int hexStringLen = strlen(hexString);
    int i;
    int bytesCount = 0;

    if (hexStringLen % 2 != 0)
        return -1;

    for (i = 0; i < (hexStringLen/2); i++) {
        int high = toInt(hexString[i * 2]);
        if (high == -1) return -1;

        int low = toInt(hexString[(i * 2) + 1]);
        if (low == -1) return -1;

        buffer[i] = (uint8_t) (high * 16 + low);
        bytesCount += 1;
    }

    return bytesCount;
}

bool
StringUtils_startsWith(char* string, char* prefix)
{
    int index = 0;

    while ((string[index] != 0) && (prefix[index] != 0)) {
        if (string[index] != prefix[index])
            return false;

        index++;
    }

    if (prefix[index] == 0)
        return true;

    return false;
}


