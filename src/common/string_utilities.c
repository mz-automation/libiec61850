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

#include "libiec61850_platform_includes.h"

char*
StringUtils_copySubString(char* startPos, char* endPos)
{
	int newStringLength = endPos - startPos;

	char* newString = (char*) GLOBAL_MALLOC(newStringLength + 1);

	if (newString) {
	    memcpy(newString, startPos, newStringLength);

	    newString[newStringLength] = 0;
	}

	return newString;
}

char*
StringUtils_copyString(const char* string)
{
	int newStringLength = strlen(string) + 1;

	char* newString = (char*) GLOBAL_MALLOC(newStringLength);

	if (newString)
	    memcpy(newString, string, newStringLength);

	return newString;
}

char*
StringUtils_copyStringToBuffer(const char* string, char* buffer)
{
    int newStringLength = strlen(string) + 1;

    memcpy(buffer, string, newStringLength);

    return buffer;
}


char*
StringUtils_createStringFromBuffer(const uint8_t* buf, int size)
{
	char* newStr = (char*) GLOBAL_MALLOC(size + 1);

	if (newStr) {
	    memcpy(newStr, buf, size);
	    newStr[size] = 0;
	}

	return newStr;
}

char*
StringUtils_createStringFromBufferInBuffer(char* newString, const uint8_t* buf, int size)
{
    memcpy(newString, buf, size);
    newString[size] = 0;

    return newString;
}


char*
StringUtils_createStringInBuffer(char* newStr, int count, ...)
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
StringUtils_createString(int count, ...)
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

	if (newStr) {
	    currentPos = newStr;

	    va_start(ap, count);
	    for (i = 0; i < count; i++) {
	        char* str = va_arg(ap, char*);
	        strcpy(currentPos, str);
	        currentPos += strlen(str);
	    }
	    va_end(ap);

	    *currentPos = 0;
	}

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
StringUtils_digitsToInt(const char* digits, int count)
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

bool
StringUtils_endsWith(const char* str, const char* suffix)
{
    int stringLength = strlen(str);
    int suffixLength = strlen(suffix);


    if (stringLength >= suffixLength) {

        if (!strcmp(str + (stringLength - suffixLength), suffix))
            return true;

    }

    return false;
}

#define LT_MAX_CHARS 128

static int
getCharWeight(int c)
{
	static bool initialized = false;
	static char lookupTable[LT_MAX_CHARS + 1];
	static const char* charOrder = "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz$_0123456789";

	if (!initialized) {
		int ltIndex;
		int weight = 1;

		for (ltIndex = 1;  ltIndex < LT_MAX_CHARS; ltIndex++) {
			if (strchr(charOrder, ltIndex)) continue;
			lookupTable[ltIndex] = weight;
			weight++;
		}

		int charIndex;

		for (charIndex = 0; charOrder[charIndex]; charIndex++) {
			lookupTable[(int)charOrder[charIndex]] = weight;
			weight++;
		}

		initialized = true;
	}

	if ((c < 1) || (c > LT_MAX_CHARS))
		return c;
	else
		return lookupTable[c];
}

int
StringUtils_compareChars(char a, char b)
{
	return (getCharWeight(a) - getCharWeight(b));
}

int
StringUtils_compareStrings(const char* a, const char* b)
{
	int diff = StringUtils_compareChars(*a, *b);

	while (diff == 0) {
		if ((*a == 0) || (*b == 0)) {
			return b - a;
		}

		diff = StringUtils_compareChars(*++a, *++b);
	}

	return diff;
}

void
StringUtils_sortList(LinkedList list)
{
	LinkedList selectedElement = list->next;

	if (selectedElement == NULL) /* list is empty */
		return;

	list->next = selectedElement->next;
	selectedElement->next = NULL;

	struct sLinkedList sortedListData;

	LinkedList sortedList = &sortedListData;

	sortedList->next = selectedElement;

	selectedElement = list->next;

	while (selectedElement != NULL) {

		list->next = selectedElement->next;
		selectedElement->next = NULL;

		char* str1 = (char*) LinkedList_getData(selectedElement);

		LinkedList prevElement = sortedList;

		while (true) {

			if (prevElement->next == NULL) {
				prevElement->next = selectedElement;
				break;
			}

			char* str2 = (char*) LinkedList_getData(prevElement->next);

			if (StringUtils_compareStrings(str1, str2) < 0) {

				/* insert "nextElement" before */
				if (sortedList == prevElement) {
					selectedElement->next = sortedList->next;
					sortedList->next = selectedElement;
				}
				else {
					selectedElement->next = prevElement->next;
					prevElement->next = selectedElement;
				}

				break;
			}

			prevElement = prevElement->next;
		}

		selectedElement = list->next;

		if (selectedElement == NULL)
			break;
	}

	list->next = sortedList->next;
}

static bool
convertHexStrToUint16(char* hexStr, uint16_t* result)
{
    int strSize = strlen(hexStr);

    if (strSize > 4)
        return false;

    int val = 0;
    int i;
    int nibble;

    for (i = 0; i < strSize; i++) {
        char nibbleChar = hexStr[i];

        if ((nibbleChar > 47) && (nibbleChar < 58)) {
            nibble = nibbleChar - 48;
        }
        else if ((nibbleChar > 96) && (nibbleChar < 103)) {
            nibble = nibbleChar - 87;
        }
        else if ((nibbleChar > 64) && (nibbleChar < 71)) {
            nibble = nibbleChar - 55;
        }
        else {
            return false;
        }

        val <<= 4;
        val += nibble;
    }

    *result = val;

    return true;
}

bool
StringUtils_convertIPv6AdddressStringToByteArray(const char* addressString, uint8_t ipV6Addr[])
{
    char tokenBuf[100];

    uint16_t addrBlocks[8];

    int blockCount = 0;
    int emptyBlockIndex = 0;
    bool hasEmptyBlock = false;
    bool end = false;

    char* savePtr = (char*) addressString;
    char* sepPos = strchr(savePtr, ':');

    while (sepPos) {

        memcpy(tokenBuf, savePtr, sepPos - savePtr);
        tokenBuf[sepPos - savePtr] = 0;
        savePtr = sepPos + 1;

        if (strlen(tokenBuf) == 0) {

            if (hasEmptyBlock) {
                return false;
            }

            hasEmptyBlock = true;
            emptyBlockIndex = blockCount;
        }
        else {
            uint16_t blockVal;

            if (convertHexStrToUint16(tokenBuf, &blockVal) == false)
                return false;

            addrBlocks[blockCount] = blockVal;

            blockCount ++;
        }

        if (blockCount == 8)
            break;

        if (end)
            break;

        sepPos = strchr(savePtr, ':');

        if (sepPos == NULL) {
            if (*savePtr != 0)
                sepPos = strchr(savePtr, 0);
            end = true;
        }
    }

    if (hasEmptyBlock) {
        /* shift blocks */
        int shiftBlocks = blockCount - emptyBlockIndex;
        int shift = 8 - blockCount;

        int s;

        /* fill empty blocks with zero */
        for (s = blockCount; s < 8; s++) {
            addrBlocks[s] = 0;
            blockCount++;
        }

        for (s = 0; s < shiftBlocks; s++) {
            addrBlocks[s + emptyBlockIndex + shift] =  addrBlocks[s + emptyBlockIndex];
            addrBlocks[s + emptyBlockIndex] = 0;
        }
    }

    if (blockCount != 8)
        return false;

    int i = 0;

    for (i = 0; i < 8; i++) {
        ipV6Addr[(i * 2)] = addrBlocks[i] / 0x100;
        ipV6Addr[(i * 2) + 1] = addrBlocks[i] & 0xff;
    }

    return true;
}
