/*
 *  file_provider_win32.c
 *
 *  Copyright 2014 Michael Zillgith
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

#include <sys/types.h>
#include <sys/stat.h>

#include "hal_filesystem.h"

#include "libiec61850_platform_includes.h"
#include "stack_config.h"

#include <malloc.h>

#include <windows.h>

#ifndef CONFIG_VIRTUAL_FILESTORE_BASEPATH
#define CONFIG_VIRTUAL_FILESTORE_BASEPATH ".\\vmd-filestore\\"
#endif

static char* fileBasePath = CONFIG_VIRTUAL_FILESTORE_BASEPATH;

struct sDirectoryHandle {
    HANDLE handle;
    WIN32_FIND_DATA findData;
    bool available;
};

static void
createFullPathFromFileName(char* fullPath, char* filename)
{
    strcpy(fullPath, fileBasePath);

    if (filename != NULL)
        strcat(fullPath, filename);
}

void
FileSystem_setBasePath(char* basePath)
{
    fileBasePath = basePath;
}

FileHandle
FileSystem_openFile(char* fileName, bool readWrite)
{
    char fullPath[sizeof(CONFIG_VIRTUAL_FILESTORE_BASEPATH) + 255];

    createFullPathFromFileName(fullPath, fileName);

    FileHandle newHandle = NULL;

    if (readWrite)
        newHandle = (FileHandle) fopen(fullPath, "w");
    else
        newHandle = (FileHandle) fopen(fullPath, "r");

    return newHandle;
}

int
FileSystem_readFile(FileHandle handle, uint8_t* buffer, int maxSize)
{
    return fread(buffer, maxSize, 1, (FILE*) handle);
}

void
FileSystem_closeFile(FileHandle handle)
{
    fclose((FILE*) handle);
}

bool
FileSystem_getFileInfo(char* filename, uint32_t* fileSize, uint64_t* lastModificationTimestamp)
{
    char fullPath[sizeof(CONFIG_VIRTUAL_FILESTORE_BASEPATH) + 255];

    createFullPathFromFileName(fullPath, filename);

    WIN32_FILE_ATTRIBUTE_DATA fad;

    if (GetFileAttributesEx(fullPath, GetFileExInfoStandard, &fad) == 0)
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
    char fullPath[sizeof(CONFIG_VIRTUAL_FILESTORE_BASEPATH) + 255];

    createFullPathFromFileName(fullPath, directoryName);

    DirectoryHandle dirHandle = (DirectoryHandle) GLOBAL_CALLOC(1, sizeof(struct sDirectoryHandle));

    strcat(fullPath, "\\*");

    dirHandle->handle = FindFirstFile(fullPath, &(dirHandle->findData));

    if (dirHandle->handle != NULL)
        dirHandle->available = true;

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

        return directory->findData.cFileName;
    }
    else {

        if (FindNextFile(directory->handle, &(directory->findData)) != 0) {

            if (directory->findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                *isDirectory = true;
            else
                *isDirectory = false;

            return directory->findData.cFileName;
        }
        else
            return NULL;
    }
}


bool
FileSystem_deleteFile(char* filename)
{
    char fullPath[sizeof(CONFIG_VIRTUAL_FILESTORE_BASEPATH) + 255];

    createFullPathFromFileName(fullPath, filename);

    if (remove(fullPath) == 0)
        return true;
    else
        return false;
}

bool
FileSystem_renameFile(char* oldFilename, char* newFilename)
{
    char oldFullPath[sizeof(CONFIG_VIRTUAL_FILESTORE_BASEPATH) + 255];
    char newFullPath[sizeof(CONFIG_VIRTUAL_FILESTORE_BASEPATH) + 255];

    createFullPathFromFileName(oldFullPath, oldFilename);
    createFullPathFromFileName(newFullPath, newFilename);

    if (rename(oldFullPath, newFullPath) == 0)
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

