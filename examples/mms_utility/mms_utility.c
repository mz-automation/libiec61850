#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "string_utilities.h"
#include "iec61850_common.h"
#include "mms_client_connection.h"
#include "conversions.h"

static void
print_help()
{
    printf("MMS utility (libiec61850 %s) options:\n", LibIEC61850_getVersionString());
    printf("-h <hostname> specify hostname\n");
    printf("-p <port> specify port\n");
    printf("-l <max_pdu_size> specify maximum PDU size\n");
    printf("-d show list of MMS domains\n");
    printf("-i show server identity\n");
    printf("-t <domain_name> show domain directory\n");
    printf("-r <variable_name> read domain variable\n");
    printf("-c <component_name> specify component name for variable read\n");
    printf("-a <domain_name> specify domain for read or write command\n");
    printf("-f show file list\n");
    printf("-g <filename> get file attributes\n");
    printf("-x <filename> delete file\n");
    printf("-j <domainName/journalName> read journal\n");
    printf("-v <variable list_name> read domain variable list\n");
    printf("-y <index> array index for read access\n");
    printf("-m print raw MMS messages\n");
}

static void
mmsFileDirectoryHandler(void* parameter, char* filename, uint32_t size, uint64_t lastModified)
{
    char* lastName = (char*) parameter;

    strcpy(lastName, filename);

    printf("%s\n", filename);
}

static void
mmsGetFileAttributeHandler(void* parameter, char* filename, uint32_t size, uint64_t lastModified)
{
    char gtString[30];
    Conversions_msTimeToGeneralizedTime(lastModified, (uint8_t*) gtString);

    printf("FILENAME: %s\n", filename);
    printf("SIZE: %u\n", size);
    printf("DATE: %s\n", gtString);
}

static void
printJournalEntries(LinkedList journalEntries)
{
    char buf[1024];

    LinkedList journalEntriesElem = LinkedList_getNext(journalEntries);

    while (journalEntriesElem != NULL) {

        MmsJournalEntry journalEntry = (MmsJournalEntry) LinkedList_getData(journalEntriesElem);

        MmsValue_printToBuffer(MmsJournalEntry_getEntryID(journalEntry), buf, 1024);
        printf("EntryID: %s\n", buf);
        MmsValue_printToBuffer(MmsJournalEntry_getOccurenceTime(journalEntry), buf, 1024);
        printf("  occurence time: %s\n", buf);

        LinkedList journalVariableElem = LinkedList_getNext(journalEntry->journalVariables);

        while (journalVariableElem != NULL) {

            MmsJournalVariable journalVariable = (MmsJournalVariable) LinkedList_getData(journalVariableElem);

            printf("   variable-tag: %s\n", MmsJournalVariable_getTag(journalVariable));
            MmsValue_printToBuffer(MmsJournalVariable_getValue(journalVariable), buf, 1024);
            printf("   variable-value: %s\n", buf);

            journalVariableElem = LinkedList_getNext(journalVariableElem);
        }

        journalEntriesElem = LinkedList_getNext(journalEntriesElem);
    }
}

void
printRawMmsMessage(void* parameter, uint8_t* message, int messageLength, bool received)
{
    if (received)
        printf("RECV: ");
    else
        printf("SEND: ");

    int i;
    for (i = 0; i < messageLength; i++) {
        printf("%02x ", message[i]);
    }

    printf("\n");
}

int main(int argc, char** argv)
{

    char* hostname = StringUtils_copyString("localhost");
    int tcpPort = 102;
    int maxPduSize = 65000;
    int arrayIndex = -1;

    char* domainName = NULL;
    char* variableName = NULL;
    char* componentName = NULL;
    char* filename = NULL;
    char* journalName = NULL;

    int readDeviceList = 0;
    int getDeviceDirectory = 0;
    int identifyDevice = 0;
    int readWriteHasDomain = 0;
    int readVariable = 0;
    int showFileList = 0;
    int getFileAttributes = 0;
    int readJournal = 0;
    int printRawMmsMessages = 0;
    int deleteFile = 0;
    int readVariableList = 0;

    int c;

    while ((c = getopt(argc, argv, "mifdh:p:l:t:a:r:g:j:x:v:c:y:")) != -1)
        switch (c) {
        case 'm':
            printRawMmsMessages = 1;
            break;

        case 'h':
            free(hostname);
            hostname = StringUtils_copyString(optarg);
            break;
        case 'p':
            tcpPort = atoi(optarg);
            break;
        case 'l':
            maxPduSize = atoi(optarg);
            break;
        case 'd':
            readDeviceList = 1;
            break;
        case 'i':
            identifyDevice = 1;
            break;
        case 't':
            getDeviceDirectory = 1;
            domainName = StringUtils_copyString(optarg);
            break;
        case 'a':
            readWriteHasDomain = 1;
            domainName = StringUtils_copyString(optarg);
            break;
        case 'r':
            readVariable = 1;
            variableName = StringUtils_copyString(optarg);
            break;
        case 'c':
            componentName = StringUtils_copyString(optarg);
            break;
        case 'v':
            readVariableList = 1;
            variableName = StringUtils_copyString(optarg);
            break;
        case 'f':
            showFileList = 1;
            break;
        case 'g':
            getFileAttributes = 1;
            filename = StringUtils_copyString(optarg);
            break;
        case 'x':
            deleteFile = 1;
            filename = StringUtils_copyString(optarg);
            break;

        case 'j':
            readJournal = 1;
            journalName = StringUtils_copyString(optarg);
            break;

        case 'y':
            arrayIndex = atoi(optarg);
            break;

        default:
            print_help();
            return 0;
        }

    MmsConnection con = MmsConnection_create();

    MmsError error;

    /* Set maximum MMS PDU size (local detail) to 2000 byte */
    MmsConnection_setLocalDetail(con, maxPduSize);

    if (printRawMmsMessages)
        MmsConnection_setRawMessageHandler(con, (MmsRawMessageHandler) printRawMmsMessage, NULL);

    if (!MmsConnection_connect(con, &error, hostname, tcpPort)) {
        printf("MMS connect failed!\n");
        goto exit;
    }
    else
        printf("MMS connected.\n");

    if (identifyDevice) {
        MmsServerIdentity* identity =
                MmsConnection_identify(con, &error);

        if (identity != NULL) {
            printf("\nServer identity:\n----------------\n");
            printf("  vendor:\t%s\n", identity->vendorName);
            printf("  model:\t%s\n", identity->modelName);
            printf("  revision:\t%s\n", identity->revision);
        }
        else
            printf("Reading server identity failed!\n");
    }

    if (readDeviceList) {
        printf("\nDomains present on server:\n--------------------------\n");
        LinkedList nameList = MmsConnection_getDomainNames(con, &error);
        LinkedList_printStringList(nameList);
        LinkedList_destroy(nameList);
    }

    if (getDeviceDirectory) {
        LinkedList variableList = MmsConnection_getDomainVariableNames(con, &error,
                domainName);

        if (variableList) {
            LinkedList element = LinkedList_getNext(variableList);

            printf("\nMMS domain variables for domain %s\n", domainName);

            while (element != NULL) {
                char* name = (char*) element->data;

                printf("  %s\n", name);

                element = LinkedList_getNext(element);
            }

            LinkedList_destroy(variableList);
        }
        else {
            printf("\nFailed to read domain directory (error=%d)\n", error);
        }

        variableList = MmsConnection_getDomainJournals(con, &error, domainName);

        if (variableList) {

            LinkedList element = variableList;

            printf("\nMMS journals for domain %s\n", domainName);

            while ((element = LinkedList_getNext(element)) != NULL) {
                char* name = (char*) element->data;

                printf("  %s\n", name);
            }

            LinkedList_destroy(variableList);
        }
        else {
            printf("\nFailed to read domain journals (error=%d)\n", error);
        }

    }

    if (readJournal) {

        printf("  read journal %s...\n", journalName);

        char* logDomain = journalName;
        char* logName = strchr(journalName, '/');

        if (logName != NULL) {

            logName[0] = 0;
            logName++;

            uint64_t timestamp = Hal_getTimeInMs();

            MmsValue* startTime = MmsValue_newBinaryTime(false);
            MmsValue_setBinaryTime(startTime, timestamp - 6000000000);

            MmsValue* endTime = MmsValue_newBinaryTime(false);
            MmsValue_setBinaryTime(endTime, timestamp);

            bool moreFollows;

            LinkedList journalEntries = MmsConnection_readJournalTimeRange(con, &error, logDomain, logName, startTime, endTime,
                    &moreFollows);

            MmsValue_delete(startTime);
            MmsValue_delete(endTime);

            if (journalEntries != NULL) {

                bool readNext;

                do {
                    readNext = false;

                    LinkedList lastEntry = LinkedList_getLastElement(journalEntries);
                    MmsJournalEntry lastJournalEntry = (MmsJournalEntry) LinkedList_getData(lastEntry);

                    MmsValue* nextEntryId = MmsValue_clone(MmsJournalEntry_getEntryID(lastJournalEntry));
                    MmsValue* nextTimestamp = MmsValue_clone(MmsJournalEntry_getOccurenceTime(lastJournalEntry));

                    printJournalEntries(journalEntries);

                    LinkedList_destroyDeep(journalEntries, (LinkedListValueDeleteFunction)
                            MmsJournalEntry_destroy);

                    if (moreFollows) {
                        char buf[100];
                        MmsValue_printToBuffer(nextEntryId, buf, 100);

                        printf("READ NEXT AFTER entryID: %s ...\n", buf);

                        journalEntries = MmsConnection_readJournalStartAfter(con, &error, logDomain, logName, nextTimestamp, nextEntryId, &moreFollows);

                        MmsValue_delete(nextEntryId);
                        MmsValue_delete(nextTimestamp);

                        readNext = true;
                    }
                }
                while ((moreFollows == true) || (readNext == true));
            }
        }
        else
            printf("  Invalid log name!\n");
    }

    if (readVariable) {
        if (readWriteHasDomain) {

            MmsValue* result;

            if (componentName == NULL) {
                if (arrayIndex == -1) {
                    result = MmsConnection_readVariable(con, &error, domainName, variableName);
                }
                else {
                    result = MmsConnection_readSingleArrayElementWithComponent(con, &error, domainName, variableName, arrayIndex, NULL);
                }
            }
            else {
                if (arrayIndex == -1) {
                    result = MmsConnection_readVariableComponent(con, &error, domainName, variableName, componentName);
                }
                else {
                    result = MmsConnection_readSingleArrayElementWithComponent(con, &error, domainName, variableName, arrayIndex, componentName);
                }
            }

            if (error != MMS_ERROR_NONE) {
                printf("Reading variable failed: (ERROR %i)\n", error);
            }
            else {
                printf("Read SUCCESS\n");

                if (result != NULL) {
                    char outbuf[1024];

                    MmsValue_printToBuffer(result, outbuf, 1024);

                    printf("%s\n", outbuf);

                    MmsValue_delete(result);
                }
                else
                    printf("result: NULL\n");
            }

        }
        else
            printf("Reading VMD scope variable not yet supported!\n");
    }

    if (readVariableList) {
        if (readWriteHasDomain) {
            MmsValue* variables = MmsConnection_readNamedVariableListValues(con, &error, domainName, variableName, true);

            if (error != MMS_ERROR_NONE) {
                printf("Reading variable failed: (ERROR %i)\n", error);
            }
            else {
                printf("Read SUCCESS\n");
            }
        }
        else
            printf("Reading VMD scope variable list not yet supported!\n");
    }

    if (showFileList) {
        char lastName[300];
        lastName[0] = 0;

        char* continueAfter = NULL;

        while (MmsConnection_getFileDirectory(con, &error, "", continueAfter, mmsFileDirectoryHandler, lastName)) {
            continueAfter = lastName;
        }
    }

    if (getFileAttributes) {
        MmsConnection_getFileDirectory(con, &error, filename, NULL, mmsGetFileAttributeHandler, NULL);
    }

    if (deleteFile) {
        MmsConnection_fileDelete(con, &error, filename);

        if (error != MMS_ERROR_NONE) {
            printf("Delete file failed: (ERROR %i)\n", error);
        }
        else {
            printf("File deleted\n");
        }
    }

    exit:
    free(hostname);
    free(domainName);
    free(variableName);
    free(journalName);
    free(componentName);

    MmsConnection_destroy(con);

    return 0;
}

