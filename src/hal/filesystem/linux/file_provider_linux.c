/*
 *  file_provider_linux.c
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

#include <string.h>
#include <dirent.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "libiec61850_platform_includes.h"

#include "hal_filesystem.h"

#include "stack_config.h"

#ifndef CONFIG_VIRTUAL_FILESTORE_BASEPATH
#define CONFIG_VIRTUAL_FILESTORE_BASEPATH "./vmd-filestore/"
#endif

static char* fileBasePath = CONFIG_VIRTUAL_FILESTORE_BASEPATH;

struct sDirectoryHandle {
    DIR* handle;
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


bool
FileSystem_getFileInfo(char* filename, uint32_t* fileSize, uint64_t* lastModificationTimestamp)
{
    struct stat fileStats;

    char fullPath[sizeof(CONFIG_VIRTUAL_FILESTORE_BASEPATH) + 256];

    createFullPathFromFileName(fullPath, filename);

    if (stat(fullPath, &fileStats) == -1)
        return false;

    if (lastModificationTimestamp != NULL)
        *lastModificationTimestamp = (uint64_t) (fileStats.st_mtime) * 1000LL;
        // does not work on older systems --> *lastModificationTimestamp = (uint64_t) (fileStats.st_ctim.tv_sec) * 1000LL;

    if (fileSize != NULL)
        *fileSize = fileStats.st_size;

    return true;
}

DirectoryHandle
FileSystem_openDirectory(char* directoryName)
{
    char fullPath[sizeof(CONFIG_VIRTUAL_FILESTORE_BASEPATH) + 255];

    createFullPathFromFileName(fullPath, directoryName);

    DIR* dirHandle = opendir(fullPath);

    DirectoryHandle handle = NULL;

    if (dirHandle != NULL) {
        handle = GLOBAL_MALLOC(sizeof(struct sDirectoryHandle));
        handle->handle = dirHandle;
    }

    return handle;
}

char*
FileSystem_readDirectory(DirectoryHandle directory, bool* isDirectory)
{
    struct dirent *dir;

    dir = readdir(directory->handle);

    if (dir != NULL) {
        if (dir->d_name[0] == '.')
            return FileSystem_readDirectory(directory, isDirectory);
        else {
            if (isDirectory != NULL) {
                if (dir->d_type == DT_DIR)
                    *isDirectory = true;
                else
                    *isDirectory = false;
            }

            return dir->d_name;
        }
    }
    else
        return NULL;
}

void
FileSystem_closeDirectory(DirectoryHandle directory)
{
    closedir(directory->handle);
    GLOBAL_FREEMEM(directory);
}

#if 0
int
main(int argc, char** argv)
{
    DirectoryHandle directory = FileSystem_openDirectory("/");

    if (directory != NULL) {
        char* fileName = FileSystem_readDirectory(directory);


        while (fileName != NULL) {
            printf("FILE: (%s)\n", fileName);
            fileName = FileSystem_readDirectory(directory);
        }

        FileSystem_closeDirectory(directory);
    }
    else
        printf("Error opening directory!\n");
}
#endif
