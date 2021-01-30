/*
 * file-tool.c
 *
 * This example demonstrates the usage of the file services
 *
 * - How to browse the file system of the server
 * - How to download a file from the server
 *
 * Note: intended to be used with server_example3 or server_example_files
 *
 */

#include "iec61850_client.h"

#include <stdlib.h>
#include <stdio.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <libgen.h>
#endif

#ifdef _WIN32
static char _dirname[1000];

static char*
dirname(char* path)
{
    char* lastSep = NULL;

    int len = strlen(path);
    int i = 0;

    while (i < len) {
        if (path[i] == '/' || path[i] == ':' || path[i] == '\\')
            lastSep = path + i;

        i++;
    }

    if (lastSep) {
        strcpy(_dirname, path);
        _dirname[lastSep - path] = 0;
    }
    else
        strcpy(_dirname, "");

    return _dirname;
}


static char _basename[1000];

static char*
basename(char* path)
{
    char* lastSep = NULL;

    int len = strlen(path);
    int i = 0;

    while (i < len) {
        if (path[i] == '/' || path[i] == ':' || path[i] == '\\')
            lastSep = path + i;

        i++;
    }

    if (lastSep)
        strcpy(_basename, lastSep + 1);
    else
        strcpy(_basename, path);

    return _basename;
}

#endif

static char* hostname = "localhost";
static int tcpPort = 102;
static char* filename = NULL;
static bool singleRequest = false;

typedef enum {
    FileOperationType_None = 0,
    FileOperationType_Dir,
    FileOperationType_Info,
    FileOperationType_Del,
    FileOperationType_Get,
    FileOperationType_Set
} FileOperationType;

static FileOperationType operation = FileOperationType_None;



static bool
downloadHandler(void* parameter, uint8_t* buffer, uint32_t bytesRead)
{
    FILE* fp = (FILE*) parameter;

    printf("received %i bytes\n", bytesRead);

    if (bytesRead > 0) {
        if (fwrite(buffer, bytesRead, 1, fp) != 1) {
            printf("Failed to write local file!\n");
            return false;
        }
    }

    return true;
}

static void
printHelp()
{
    printf("file-tool [options] <operation> [<parameters>]\n");
    printf("  Options:\n");
    printf("    -h <hostname/IP>\n");
    printf("    -p portnumber\n");
    printf("    -s single request for show (sub) directory (ignore morefollows");
    printf("  Operations\n");
    printf("     dir - show directory\n");
    printf("     subdir <dirname> - show sub directory\n");
    printf("     info <filename> - show file info\n");
    printf("     del <filename> - delete file\n");
    printf("     get <filename> - get file\n");
    printf("     set <filename> - set file\n");
}


static int
parseOptions(int argc, char** argv)
{
    int currentArgc = 1;

    int retVal = 0;

    while (currentArgc < argc) {
        if (strcmp(argv[currentArgc], "-h") == 0) {
            hostname = argv[++currentArgc];
        }
        else if (strcmp(argv[currentArgc], "-p") == 0) {
            tcpPort = atoi(argv[++currentArgc]);
        }
        else if (strcmp(argv[currentArgc], "-s") == 0) {
            singleRequest = true;
        }
        else if (strcmp(argv[currentArgc], "del") == 0) {
            operation = FileOperationType_Del;
            filename = argv[++currentArgc];
        }
        else if (strcmp(argv[currentArgc], "dir") == 0) {
            operation = FileOperationType_Dir;
        }
        else if (strcmp(argv[currentArgc], "subdir") == 0) {
            operation = FileOperationType_Dir;
            filename = argv[++currentArgc];
        }
        else if (strcmp(argv[currentArgc], "info") == 0) {
            operation = FileOperationType_Info;
            filename = argv[++currentArgc];
        }
        else if (strcmp(argv[currentArgc], "get") == 0) {
            operation = FileOperationType_Get;
            filename = argv[++currentArgc];
        }
        else if (strcmp(argv[currentArgc], "set") == 0) {
            operation = FileOperationType_Set;
            filename = argv[++currentArgc];
        }
        else {
            printf("Unknown operation!\n");
            return 1;
        }

        currentArgc++;
    }

    return retVal;
}

void
showDirectory(IedConnection con)
{
    IedClientError error;

    bool moreFollows = false;

    /* Get the root directory */
    LinkedList rootDirectory;

    if (singleRequest)
        rootDirectory = IedConnection_getFileDirectoryEx(con, &error, filename, NULL, &moreFollows);
    else
        rootDirectory = IedConnection_getFileDirectory(con, &error, filename);

    if (error != IED_ERROR_OK) {
        printf("Error retrieving file directory\n");
    }
    else {
        LinkedList directoryEntry = LinkedList_getNext(rootDirectory);

        while (directoryEntry != NULL) {

            FileDirectoryEntry entry = (FileDirectoryEntry) directoryEntry->data;

            printf("%s %i\n", FileDirectoryEntry_getFileName(entry), FileDirectoryEntry_getFileSize(entry));

            directoryEntry = LinkedList_getNext(directoryEntry);
        }

        LinkedList_destroyDeep(rootDirectory, (LinkedListValueDeleteFunction) FileDirectoryEntry_destroy);
    }

    if (moreFollows)
        printf("\n- MORE FILES AVAILABLE -\n");
}

void
getFile(IedConnection con)
{
    IedClientError error;

    char* bname = strdup(filename);

    char* localFilename = basename(bname);

    FILE* fp = fopen(localFilename, "wb");

    if (fp != NULL) {

        /* Download a file from the server */
        IedConnection_getFile(con, &error, filename, downloadHandler, (void*) fp);

        if (error != IED_ERROR_OK)
            printf("Failed to get file!\n");

        fclose(fp);
    }
    else
        printf("Failed to open file %s\n", localFilename);

    free(bname);
}

void
setFile(IedConnection con)
{
    IedClientError error;

    char* dirc = strdup(filename);
    char* basec = strdup(filename);

    char* localDirName = dirname(dirc);
    char* localFileName = basename(basec);

    printf("local dir: %s\n", localDirName);
    printf("local file: %s\n", localFileName);

    /* IedConnection_setFilestoreBasepath requires the file separator at the end! */
    strcpy(dirc, localDirName);
    strcat(dirc, "/");

    printf("filestore basepath: %s\n", dirc);

    IedConnection_setFilestoreBasepath(con, dirc);

    IedConnection_setFile(con, &error, localFileName, localFileName);

    if (error != IED_ERROR_OK)
        printf("Failed to set file! (code=%i)\n", error);

    free(dirc);
    free(basec);
}

void
deleteFile(IedConnection con)
{
    IedClientError error;

    /* Delete file at server */
    IedConnection_deleteFile(con, &error, filename);

    if (error != IED_ERROR_OK)
        printf("Failed to delete file! (code=%i)\n", error);
}

int
main(int argc, char** argv)
{
    if (argc < 2) {
        printHelp();
        return 0;
    }

    parseOptions(argc, argv);

    if (operation == FileOperationType_None) {
        printHelp();
        return 0;
    }

    IedClientError error;

    IedConnection con = IedConnection_create();

    IedConnection_connect(con, &error, hostname, tcpPort);

    if (error == IED_ERROR_OK) {


        switch (operation) {
        case FileOperationType_Dir:
            showDirectory(con);
            break;
        case FileOperationType_Get:
            getFile(con);
            break;
        case FileOperationType_Del:
            deleteFile(con);
            break;
        case FileOperationType_Info:
            break;
        case FileOperationType_Set:
            setFile(con);
            break;
        case FileOperationType_None:
            break;
        }


        IedConnection_abort(con, &error);
    }
    else {
        printf("Failed to connect to %s:%i\n", hostname, tcpPort);
    }

    IedConnection_destroy(con);
    return 0;
}


