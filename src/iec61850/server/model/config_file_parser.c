/*
 *  config_file_parser.c
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

#include "iec61850_server.h"
#include "iec61850_dynamic_model.h"
#include "iec61850_config_file_parser.h"

#include "libiec61850_platform_includes.h"
#include "stack_config.h"

#define READ_BUFFER_MAX_SIZE 1024

static uint8_t lineBuffer[READ_BUFFER_MAX_SIZE];


static int
readLine(FileHandle fileHandle, uint8_t* buffer, int maxSize)
{
    int bytesRead = 0;
    int bufPos = 0;

    int fileReadResult = 1;

    /* eat up leading cr or lf */
    while (fileReadResult > 0) {

        if (bytesRead == maxSize)
            break;

        fileReadResult = FileSystem_readFile(fileHandle, buffer + bufPos, 1);

        if (fileReadResult == 1) {

            if (!((buffer[bufPos] == '\n') || (buffer[bufPos] == '\r'))) {
                bufPos++;
                bytesRead++;
                break;
            }
        }
    }

    if (fileReadResult > 0) {
        while (fileReadResult > 0) {

            if (bytesRead == maxSize)
                break;

            fileReadResult = FileSystem_readFile(fileHandle, buffer + bufPos, 1);

            if (fileReadResult == 1) {

                if ((buffer[bufPos] == '\n') || (buffer[bufPos] == '\r'))
                    break;
                else {
                    bufPos++;
                    bytesRead++;
                }
            }
        }
    }


    return bytesRead;
}

static void
terminateString(char* string, char ch)
{
    int index = 0;

    while (string[index] != 0) {
        if (string[index] == ch) {
            string[index] = 0;
            break;
        }

        index++;
    }
}

IedModel*
ConfigFileParser_createModelFromConfigFileEx(const char* filename)
{
    FileHandle configFile = FileSystem_openFile((char*)filename, false);

    if (configFile == NULL) {
        if (DEBUG_IED_SERVER)
            printf("IED_SERVER: Error opening config file!\n");
        return NULL;
    }

    IedModel* model = ConfigFileParser_createModelFromConfigFile(configFile);

    FileSystem_closeFile(configFile);

    return model;
}

IedModel*
ConfigFileParser_createModelFromConfigFile(FileHandle fileHandle)
{
    int bytesRead = 1;

    bool stateInModel = false;
    int indendation = 0;

    IedModel* model = NULL;
    LogicalDevice* currentLD = NULL;
    LogicalNode* currentLN = NULL;
    ModelNode* currentModelNode = NULL;
    DataSet* currentDataSet = NULL;
    GSEControlBlock* currentGoCB = NULL;
    SVControlBlock* currentSMVCB = NULL;

    char nameString[130];
    char nameString2[130];
    char nameString3[130];

    int currentLine = 0;

    while (bytesRead > 0) {
        bytesRead = readLine(fileHandle, lineBuffer, READ_BUFFER_MAX_SIZE);

        currentLine++;

        if (bytesRead > 0) {
            lineBuffer[bytesRead] = 0;

            if (stateInModel) {

                if (StringUtils_startsWith((char*) lineBuffer, "}")) {
                    if (indendation == 1) {
                        stateInModel = false;
                        indendation = 0;
                    }
                    else if (indendation == 2) {
                        indendation = 1;
                    }
                    else if (indendation == 3) {
                        indendation = 2;
                    }
                    else if (indendation == 4) {
                        indendation = 3;
                    }
                    else if (indendation > 4) {
                        currentModelNode = currentModelNode->parent;
                        indendation--;
                    }
                }

                else if (indendation == 1) {
                    if (StringUtils_startsWith((char*) lineBuffer, "LD")) {
                        indendation = 2;

                        if (sscanf((char*) lineBuffer, "LD(%s)", nameString) < 1)
                            goto exit_error;

                        terminateString(nameString, ')');

                        currentLD = LogicalDevice_create(nameString, model);
                    }
                    else
                        goto exit_error;
                }
                else if (indendation == 2) {
                    if (StringUtils_startsWith((char*) lineBuffer, "LN")) {
                        indendation = 3;

                        if (sscanf((char*) lineBuffer, "LN(%s)", nameString) < 1)
                            goto exit_error;

                        terminateString(nameString, ')');

                        currentLN = LogicalNode_create(nameString, currentLD);
                    }
                    else
                        goto exit_error;
                }
                else if (indendation == 3) {
                    if (StringUtils_startsWith((char*) lineBuffer, "DO")) {
                        indendation = 4;

                        int arrayElements = 0;

                        sscanf((char*) lineBuffer, "DO(%s %i)", nameString, &arrayElements);

                        currentModelNode = (ModelNode*)
                                DataObject_create(nameString, (ModelNode*) currentLN, arrayElements);
                    }
                    else if (StringUtils_startsWith((char*) lineBuffer, "DS")) {
                        indendation = 4;

                        sscanf((char*) lineBuffer, "DS(%s)", nameString);
                        terminateString(nameString, ')');

                        currentDataSet = DataSet_create(nameString, currentLN);
                    }
                    else if (StringUtils_startsWith((char*) lineBuffer, "RC")) {
                        int isBuffered;
                        uint32_t confRef;
                        int trgOps;
                        int options;
                        uint32_t bufTm;
                        uint32_t intgPd;

                        int matchedItems = sscanf((char*) lineBuffer, "RC(%s %s %i %s %u %i %i %u %u)",
                                nameString, nameString2, &isBuffered, nameString3, &confRef,
                                &trgOps, &options, &bufTm, &intgPd);

                        if (matchedItems < 9) goto exit_error;

                        char* rptId = NULL;

                        if (strcmp(nameString2, "-") != 0)
                            rptId = nameString2;

                        char* dataSetName = NULL;

                        if (strcmp(nameString3, "-") != 0)
                            dataSetName = nameString3;

                        ReportControlBlock_create(nameString, currentLN, rptId,
                                (bool) isBuffered, dataSetName, confRef, trgOps, options, bufTm, intgPd);
                    }
                    else if (StringUtils_startsWith((char*) lineBuffer, "LC")) {
                        uint32_t trgOps;
                        uint32_t intgPd;
                        int logEna;
                        int withReasonCode;

                        int matchedItems = sscanf((char*) lineBuffer, "LC(%s %s %s %u %u %i %i)",
                                nameString, nameString2, nameString3, &trgOps, &intgPd, &logEna, &withReasonCode);

                        if (matchedItems < 7) goto exit_error;

                        char* dataSet = NULL;
                        if (strcmp(nameString2, "-") != 0)
                            dataSet = nameString2;

                        char* logRef = NULL;
                        if (strcmp(nameString3, "-") != 0)
                            logRef = nameString3;

                        LogControlBlock_create(nameString, currentLN, dataSet, logRef, trgOps, intgPd, logEna, withReasonCode);
                    }
                    else if (StringUtils_startsWith((char*) lineBuffer, "LOG")) {
                        int matchedItems = sscanf((char*) lineBuffer, "LOG(%s)", nameString);

                        if (matchedItems < 1) goto exit_error;

                        /* remove trailing ')' character */
                        strtok(nameString, ")");

                        Log_create(nameString, currentLN);
                    }
                    else if (StringUtils_startsWith((char*) lineBuffer, "GC")) {
                        uint32_t confRef;
                        int fixedOffs;
                        int minTime = -1;
                        int maxTime = -1;

                        int matchedItems = sscanf((char*) lineBuffer, "GC(%s %s %s %u %i %i %i)",
                                nameString, nameString2, nameString3, &confRef, &fixedOffs, &minTime, &maxTime);

                        if (matchedItems < 5) goto exit_error;

                        currentGoCB = GSEControlBlock_create(nameString, currentLN, nameString2,
                                nameString3, confRef, fixedOffs, minTime, maxTime);

                        indendation = 4;

                    }
                    else if (StringUtils_startsWith((char*) lineBuffer, "SMVC")) {
                        uint32_t confRev;
                        int smpMod;
                        int smpRate;
                        int optFlds;
                        int isUnicast;

                        int matchedItems = sscanf((char*) lineBuffer, "SMVC(%s %s %s %u %i %i %i %i)",
                                nameString, nameString2, nameString3, &confRev, &smpMod, &smpRate, &optFlds, &isUnicast);

                        if (matchedItems < 5) goto exit_error;

                        currentSMVCB = SVControlBlock_create(nameString, currentLN, nameString2, nameString3, confRev, smpMod, smpRate, optFlds, (bool) isUnicast);

                        indendation = 4;

                    }
#if (CONFIG_IEC61850_SETTING_GROUPS == 1)
                    else if (StringUtils_startsWith((char*) lineBuffer, "SG")) {

                        if (strcmp(currentLN->name, "LLN0") != 0) {
                            if (DEBUG_IED_SERVER)
                                printf("IED_SERVER: Setting group control is not defined in LLN0\n");

                            goto exit_error;
                        }

                        int actSG;
                        int numOfSGs;

                        int matchedItems = sscanf((char*) lineBuffer, "SG(%i %i)", &actSG, &numOfSGs);

                        if (matchedItems < 2)
                            goto exit_error;

                        SettingGroupControlBlock_create(currentLN, actSG, numOfSGs);
                    }
#endif /* (CONFIG_IEC61850_SETTING_GROUPS == 1) */

                    else {
                        if (DEBUG_IED_SERVER)
                            printf("IED_SERVER: Unknown identifier (%s)\n", lineBuffer);

                        goto exit_error;
                    }

                }
                else if (indendation > 3) {
                    if (StringUtils_startsWith((char*) lineBuffer, "DO")) {
                        indendation++;

                        int arrayElements = 0;

                        int matchedItems = sscanf((char*) lineBuffer, "DO(%s %i)", nameString, &arrayElements);

                        if (matchedItems != 2) goto exit_error;

                        currentModelNode = (ModelNode*) DataObject_create(nameString, currentModelNode, arrayElements);
                    }
                    else if (StringUtils_startsWith((char*) lineBuffer, "DA")) {

                        int arrayElements = 0;

                        int attributeType = 0;
                        int functionalConstraint = 0;
                        int triggerOptions = 0;
                        uint32_t sAddr = 0;

                        sscanf((char*) lineBuffer, "DA(%s %i %i %i %i %u)", nameString, &arrayElements,  &attributeType, &functionalConstraint, &triggerOptions, &sAddr);

                        DataAttribute* dataAttribute = DataAttribute_create(nameString, currentModelNode,
                                (DataAttributeType) attributeType, (FunctionalConstraint) functionalConstraint, triggerOptions, arrayElements, sAddr);

                        char* valueIndicator = strchr((char*) lineBuffer, '=');

                        if (valueIndicator != NULL) {
                            switch (dataAttribute->type) {
                            case IEC61850_UNICODE_STRING_255:
                                {
                                    char* stringStart = valueIndicator + 2;
                                    terminateString(stringStart, '"');
                                    dataAttribute->mmsValue = MmsValue_newMmsString(stringStart);
                                }
                                break;

                            case IEC61850_VISIBLE_STRING_255:
                            case IEC61850_VISIBLE_STRING_129:
                            case IEC61850_VISIBLE_STRING_65:
                            case IEC61850_VISIBLE_STRING_64:
                            case IEC61850_VISIBLE_STRING_32:
                            case IEC61850_CURRENCY:
                                {
                                    char* stringStart = valueIndicator + 2;
                                    terminateString(stringStart, '"');
                                    dataAttribute->mmsValue = MmsValue_newVisibleString(stringStart);
                                }
                                break;

                            case IEC61850_INT8:
                            case IEC61850_INT16:
                            case IEC61850_INT32:
                            case IEC61850_INT64:
                            case IEC61850_INT128:
                            case IEC61850_ENUMERATED:
                                {
                                    int32_t intValue;
                                    if (sscanf(valueIndicator + 1, "%i", &intValue) != 1) goto exit_error;
                                    dataAttribute->mmsValue = MmsValue_newIntegerFromInt32(intValue);
                                }
                                break;

                            case IEC61850_INT8U:
                            case IEC61850_INT16U:
                            case IEC61850_INT24U:
                            case IEC61850_INT32U:
                                {
                                    uint32_t uintValue;
                                    if (sscanf(valueIndicator + 1, "%u", &uintValue) != 1) goto exit_error;
                                    dataAttribute->mmsValue = MmsValue_newUnsignedFromUint32(uintValue);
                                }
                                break;

                            case IEC61850_FLOAT32:
                                {
                                    float floatValue;
                                    if (sscanf(valueIndicator + 1, "%f", &floatValue) != 1) goto exit_error;
                                    dataAttribute->mmsValue = MmsValue_newFloat(floatValue);
                                }
                                break;

                            case IEC61850_FLOAT64:
                                {
                                    double doubleValue;
                                    if (sscanf(valueIndicator + 1, "%lf", &doubleValue) != 1) goto exit_error;
                                    dataAttribute->mmsValue = MmsValue_newDouble(doubleValue);
                                }
                                break;

                            case IEC61850_BOOLEAN:
                                {
                                    int boolean;
                                    if (sscanf(valueIndicator + 1, "%i", &boolean) != 1) goto exit_error;
                                    dataAttribute->mmsValue = MmsValue_newBoolean((bool) boolean);
                                }
                                break;

                            case IEC61850_OPTFLDS:
                                {
                                    int value;
                                    if (sscanf(valueIndicator + 1, "%i", &value) != 1) goto exit_error;
                                    dataAttribute->mmsValue = MmsValue_newBitString(-10);
                                    MmsValue_setBitStringFromIntegerBigEndian(dataAttribute->mmsValue, value);
                                }
                                break;

                            case IEC61850_TRGOPS:
                                {
                                    int value;
                                    if (sscanf(valueIndicator + 1, "%i", &value) != 1) goto exit_error;
                                    dataAttribute->mmsValue = MmsValue_newBitString(-6);
                                    MmsValue_setBitStringFromIntegerBigEndian(dataAttribute->mmsValue, value);
                                }
                                break;

                            default:
                                break;

                            }
                        }

                        int lineLength = (int) strlen((char*) lineBuffer);

                        if (lineBuffer[lineLength - 1] == '{') {
                            indendation++;
                            currentModelNode = (ModelNode*) dataAttribute;
                        }
                    }
                    else if (StringUtils_startsWith((char*) lineBuffer, "DE")) {
                        char* start = strchr((char*) lineBuffer, '(');

                        if (start) {
                            start++;
                            strncpy(nameString, start, 129);
                            terminateString(nameString, ')');

                            int indexVal = -1;
                            char* componentVal = NULL;

                            /* check for index */
                            char* sep = strchr(nameString, ' ');

                            if (sep) {
                                char* indexStr = sep + 1;
                                *sep = 0;

                                /* check for component */
                                sep = strchr(indexStr, ' ');

                                if (sep) {
                                    componentVal = sep + 1;
                                    *sep = 0;
                                }

                                indexVal = atoi(indexStr);
                            }

                            DataSetEntry_create(currentDataSet, nameString, indexVal, componentVal);
                        }
                    }
                    else if (StringUtils_startsWith((char*) lineBuffer, "PA")) {
                        uint32_t vlanPrio;
                        uint32_t vlanId;
                        uint32_t appId;

                        int matchedItems = sscanf((char*) lineBuffer, "PA(%u %u %u %s)", &vlanPrio, &vlanId, &appId, nameString);

                        if ((matchedItems != 4) || ((currentGoCB == NULL) && (currentSMVCB == NULL))) goto exit_error;

                        terminateString(nameString, ')');

                        if (strlen(nameString) != 12) goto exit_error;

                        if (StringUtils_createBufferFromHexString(nameString, (uint8_t*) nameString2) != 6)
                            goto exit_error;


                        PhyComAddress* dstAddress =
                                PhyComAddress_create((uint8_t) vlanPrio, (uint16_t) vlanId, (uint16_t) appId,
                                        (uint8_t*) nameString2);

                        if (currentGoCB) {
                            GSEControlBlock_addPhyComAddress(currentGoCB, dstAddress);
                        }

                        if (currentSMVCB) {
                            SVControlBlock_addPhyComAddress(currentSMVCB, dstAddress);
                        }
                    }
                    else
                        goto exit_error;
                }


            }
            else {
                if (StringUtils_startsWith((char*) lineBuffer, "MODEL{")) {

                    model = IedModel_create("");
                    stateInModel = true;
                    indendation = 1;
                }
                else if (StringUtils_startsWith((char*) lineBuffer, "MODEL(")) {
                    sscanf((char*) lineBuffer, "MODEL(%s)", nameString);
                    terminateString(nameString, ')');
                    model = IedModel_create(nameString);
                    stateInModel = true;
                    indendation = 1;
                }
                else
                    goto exit_error;
            }
        }
    }

    return model;

exit_error:
    if (DEBUG_IED_SERVER)
        printf("IED_SERVER: error parsing line %i (indentation level = %i)\n", currentLine, indendation);

    IedModel_destroy(model);
    return NULL;
}


