/*
 *  file_provider_win32.c
 *
 *  Copyright 2014, 2015 Michael Zillgith
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

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "hal_filesystem.h"

#include "platform_endian.h"
#include "lib_memory.h"

#include <malloc.h>

#include <windows.h>

struct sDirectoryHandle {
    HANDLE handle;
    WIN32_FIND_DATAW findData;
	char utf8Filename[MAX_PATH * 3 + 1];
    bool available;
};


FileHandle
FileSystem_openFile(char* fileName, bool readWrite)
{
    FileHandle newHandle = NULL;

    if (readWrite)
        newHandle = (FileHandle) fopen(fileName, "wb");
    else
        newHandle = (FileHandle) fopen(fileName, "rb");

    return newHandle;
}

int
FileSystem_readFile(FileHandle handle, uint8_t* buffer, int maxSize)
{
    return fread(buffer, maxSize, 1, (FILE*) handle);
}

int
FileSystem_writeFile(FileHandle handle, uint8_t* buffer, int size)
{
    return fwrite(buffer, size, 1, (FILE*) handle);
}

void
FileSystem_closeFile(FileHandle handle)
{
    fclose((FILE*) handle);
}

bool
FileSystem_getFileInfo(char* filename, uint32_t* fileSize, uint64_t* lastModificationTimestamp)
{
    WIN32_FILE_ATTRIBUTE_DATA fad;

    if (GetFileAttributesEx(filename, GetFileExInfoStandard, &fad) == 0)
        return false;

    if (lastModificationTimestamp != NULL) {
        FILETIME lastModTime = fad.ftLastWriteTime;

        uint64_t now;

        static const uint64_t DIFF_TO_UNIXTIME = 11644473600000LL;

        now = (LONGLONG) lastModTime.dwLowDateTime + ((LONGLONG)(lastModTime.dwHighDateTime) << 32LL);

        *lastModificationTimestamp =  (now / 10000LL) - DIFF_TO_UNIXTIME;
    }

    if (fileSize != NULL)
        *fileSize = (uint32_t) fad.nFileSizeLow;

    return true;
}

DirectoryHandle
FileSystem_openDirectory(char* directoryName)
{
    DirectoryHandle dirHandle = (DirectoryHandle) GLOBAL_CALLOC(1, sizeof(struct sDirectoryHandle));

    char fullPath[MAX_PATH + 1];

    strncpy(fullPath, directoryName, MAX_PATH - 3);
    strncat(fullPath, "\\*", MAX_PATH);

	/* convert UTF-8 path name to WCHAR */
	WCHAR unicodeFullPath[MAX_PATH + 1];
	MultiByteToWideChar(CP_UTF8, 0, fullPath, -1, unicodeFullPath, MAX_PATH);

	dirHandle->handle = FindFirstFileW(unicodeFullPath, &(dirHandle->findData));

	if (dirHandle->handle != NULL) {
		dirHandle->available = true;

		/* convert WCHAR to UTF-8 */
		WideCharToMultiByte(CP_UTF8, 0, dirHandle->findData.cFileName, -1, dirHandle->utf8Filename, (MAX_PATH * 3) + 1, NULL, NULL);
	}

    if (dirHandle->handle == INVALID_HANDLE_VALUE) {
        GLOBAL_FREEMEM(dirHandle);
        return NULL;
    }
    else
        return dirHandle;
}

static char*
getNextDirectoryEntry(DirectoryHandle directory, bool* isDirectory)
{
    if (directory->available == true) {
        directory->available = false;

        if (directory->findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            *isDirectory = true;
        else
            *isDirectory = false;

        return directory->utf8Filename;
    }
    else {

        if (FindNextFileW(directory->handle, &(directory->findData)) != 0) {

            if (directory->findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                *isDirectory = true;
            else
                *isDirectory = false;

			/* convert WCHAR to UTF-8 */
			WideCharToMultiByte(CP_UTF8, 0, directory->findData.cFileName, -1, directory->utf8Filename, (MAX_PATH * 3) + 1, NULL, NULL);

            return directory->utf8Filename;
        }
        else
            return NULL;
    }
}


bool
FileSystem_deleteFile(char* filename)
{
    if (remove(filename) == 0)
        return true;
    else
        return false;
}

bool
FileSystem_renameFile(char* oldFilename, char* newFilename)
{
    if (rename(oldFilename, newFilename) == 0)
        return true;
    else
        return false;
}


char*
FileSystem_readDirectory(DirectoryHandle directory, bool* isDirectory)
{
    char* fileName = getNextDirectoryEntry(directory, isDirectory);

    if (fileName != NULL) {
        if (fileName[0] == '.')
            return FileSystem_readDirectory(directory, isDirectory);
        else
            return fileName;
    }

    return NULL;
}

void
FileSystem_closeDirectory(DirectoryHandle directory)
{
    FindClose(directory->handle);
    GLOBAL_FREEMEM(directory);
}

