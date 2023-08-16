/*
 *  mms_mapping_internal.h
 *
 *  Copyright 2013-2022 Michael Zillgith
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

#ifndef MMS_MAPPING_INTERNAL_H_
#define MMS_MAPPING_INTERNAL_H_

#include "stack_config.h"

#include "hal_thread.h"
#include "linked_list.h"

#if (CONFIG_IEC61850_SERVICE_TRACKING == 1)

typedef int IEC61850_ServiceType;

#define IEC61850_SERVICE_TYPE_UNKOWN 0
#define IEC61850_SERVICE_TYPE_ASSOCIATE 1
#define IEC61850_SERVICE_TYPE_ABORT 2
#define IEC61850_SERVICE_TYPE_RELEASE 3
#define IEC61850_SERVICE_TYPE_GET_SERVER_DIRECTORY 4
#define IEC61850_SERVICE_TYPE_GET_LOGICAL_DEVICE_DIRECTORY 5
#define IEC61850_SERVICE_TYPE_GET_ALL_DATA_VALUES 6
#define IEC61850_SERVICE_TYPE_GET_DATA_VALUES 7
#define IEC61850_SERVICE_TYPE_SET_DATA_VALUES 8
#define IEC61850_SERVICE_TYPE_GET_DATA_DIRECTORY 9
#define IEC61850_SERVICE_TYPE_GET_DATA_DEFINITION 10
#define IEC61850_SERVICE_TYPE_GET_DATA_SET_VALUES 11
#define IEC61850_SERVICE_TYPE_SET_DATA_SET_VALUES 12
#define IEC61850_SERVICE_TYPE_CREATE_DATA_SET 13
#define IEC61850_SERVICE_TYPE_DELETE_DATA_SET 14
#define IEC61850_SERVICE_TYPE_GET_DATA_SET_DIRECTORY 15
#define IEC61850_SERVICE_TYPE_SELECT_ACTIVE_SG 16
#define IEC61850_SERVICE_TYPE_SELECT_EDIT_SG 17
#define IEC61850_SERVICE_TYPE_SET_EDIT_SG_VALUE 18
#define IEC61850_SERVICE_TYPE_CONFIRM_EDIT_SG_VALUES 19
#define IEC61850_SERVICE_TYPE_GET_EDIT_SG_VALUE 20
#define IEC61850_SERVICE_TYPE_GET_SGCB_VALUES 21
#define IEC61850_SERVICE_TYPE_REPORT 22
#define IEC61850_SERVICE_TYPE_GET_BRCB_VALUES 23
#define IEC61850_SERVICE_TYPE_SET_BRCB_VALUES 24
#define IEC61850_SERVICE_TYPE_GET_URCB_VALUES 25
#define IEC61850_SERVICE_TYPE_SET_URCB_VALUES 26
#define IEC61850_SERVICE_TYPE_GET_LCB_VALUES 27
#define IEC61850_SERVICE_TYPE_SET_LCB_VALUES 28
#define IEC61850_SERVICE_TYPE_QUERY_LOG_BY_TIME 29
#define IEC61850_SERVICE_TYPE_QUERY_LOG_AFTER 30
#define IEC61850_SERVICE_TYPE_GET_LOG_STATUS 31
#define IEC61850_SERVICE_TYPE_SEND_GOOSE_MESSAGE 32
#define IEC61850_SERVICE_TYPE_GET_GOCB_VALUES 33
#define IEC61850_SERVICE_TYPE_SET_GOCB_VALUES 34
#define IEC61850_SERVICE_TYPE_GET_GO_REFERENCE 35
#define IEC61850_SERVICE_TYPE_GET_GOOSE_ELEMENT_NUMBER 36
#define IEC61850_SERVICE_TYPE_SEND_MSV_MESSAGE 37
#define IEC61850_SERVICE_TYPE_GET_MSVCB_VALUES 38
#define IEC61850_SERVICE_TYPE_SET_MSVCB_VALUES 39
#define IEC61850_SERVICE_TYPE_SEND_USV_MESSAGE 40
#define IEC61850_SERVICE_TYPE_GET_USVCB_VALUES 41
#define IEC61850_SERVICE_TYPE_SET_USVCB_VALUES 42
#define IEC61850_SERVICE_TYPE_SELECT 43
#define IEC61850_SERVICE_TYPE_SELECT_WITH_VALUES 44
#define IEC61850_SERVICE_TYPE_CANCEL 45
#define IEC61850_SERVICE_TYPE_OPERATE 46
#define IEC61850_SERVICE_TYPE_COMMAND_TERMINATION 47
#define IEC61850_SERVICE_TYPE_TIME_ACTIVATED_OPERATE 48
#define IEC61850_SERVICE_TYPE_GET_FILE 49
#define IEC61850_SERVICE_TYPE_SET_FILE 50
#define IEC61850_SERVICE_TYPE_DELETE_FILE 51
#define IEC61850_SERVICE_TYPE_GET_FILE_ATTRIBUTE_VALUES 52
#define IEC61850_SERVICE_TYPE_TIME_SYNCHRONISATION 53
#define IEC61850_SERVICE_TYPE_INTERNAL_CHANGE 54

typedef int IEC61850_ServiceError;

#define IEC61850_SERVICE_ERROR_NO_ERROR 0
#define IEC61850_SERVICE_ERROR_INSTANCE_NOT_AVAILABLE 1
#define IEC61850_SERVICE_ERROR_INSTANCE_IN_USE 2
#define IEC61850_SERVICE_ERROR_ACCESS_VIOLATION 3
#define IEC61850_SERVICE_ERROR_ACCESS_NOT_ALLOWED_IN_CURRENT_STATE 4
#define IEC61850_SERVICE_ERROR_PARAMETER_VALUE_INAPPROPRIATE 5
#define IEC61850_SERVICE_ERROR_PARAMETER_VALUE_INCONSISTENT 6
#define IEC61850_SERVICE_ERROR_CLASS_NOT_SUPPORTED 7
#define IEC61850_SERVICE_ERROR_INSTANCE_LOCKED_BY_OTHER_CLIENT 8
#define IEC61850_SERVICE_ERROR_CONTROL_MUST_BE_SELECTED 9
#define IEC61850_SERVICE_ERROR_TYPE_CONFLICT 10
#define IEC61850_SERVICE_ERROR_FAILED_DUE_TO_COMMUNICATION_CONSTRAINT 11
#define IEC61850_SERVICE_ERROR_FAILED_DUE_TO_SERVER_CONSTRAINT 12

typedef struct sServiceTrkInstance* ServiceTrkInstance;

struct sServiceTrkInstance
{
    DataObject* dobj;
    DataAttribute* objRef;
    DataAttribute* serviceType;
    DataAttribute* errorCode;
    DataAttribute* originatorID; /* optional */
    DataAttribute* t;
};

typedef struct sBrcbTrkInstance* BrcbTrkInstance;

struct sBrcbTrkInstance
{
    /* inherited from ServiceTrkInstance */
    DataObject* dobj;
    DataAttribute* objRef;
    DataAttribute* serviceType;
    DataAttribute* errorCode;
    DataAttribute* originatorID; /* optional */
    DataAttribute* t;

    /* BrcbTrk specific attributes */
    DataAttribute* rptID;
    DataAttribute* rptEna;
    DataAttribute* datSet;
    DataAttribute* confRev;
    DataAttribute* optFlds;
    DataAttribute* bufTm;
    DataAttribute* sqNum;
    DataAttribute* trgOps;
    DataAttribute* intgPd;
    DataAttribute* gi;
    DataAttribute* purgeBuf;
    DataAttribute* entryID;
    DataAttribute* timeOfEntry;
    DataAttribute* resvTms;  /* optional */
};

typedef struct sUrcbTrkInstance* UrcbTrkInstance;

struct sUrcbTrkInstance
{
    /* inherited from ServiceTrkInstance */
    DataObject* dobj;
    DataAttribute* objRef;
    DataAttribute* serviceType;
    DataAttribute* errorCode;
    DataAttribute* originatorID; /* optional */
    DataAttribute* t;

    /* UrcbTrk specific attributes */
    DataAttribute* rptID;
    DataAttribute* rptEna;
    DataAttribute* resv;
    DataAttribute* datSet;
    DataAttribute* confRev;
    DataAttribute* optFlds;
    DataAttribute* bufTm;
    DataAttribute* sqNum;
    DataAttribute* trgOps;
    DataAttribute* intgPd;
    DataAttribute* gi;
};

typedef struct sGocbTrkInstance* GocbTrkInstance;

struct sGocbTrkInstance
{
    /* inherited from ServiceTrkInstance */
    DataObject* dobj;
    DataAttribute* objRef;
    DataAttribute* serviceType;
    DataAttribute* errorCode;
    DataAttribute* originatorID; /* optional */
    DataAttribute* t;

    /* GocbTrk specific attributes */
    DataAttribute* goEna;
    DataAttribute* goID;
    DataAttribute* datSet;
    DataAttribute* confRev;
    DataAttribute* ndsCom;
    DataAttribute* dstAddress;
};

typedef struct sControlTrkInstance* ControlTrkInstance;

struct sControlTrkInstance
{
    /* inherited from ServiceTrkInstance */
    DataObject* dobj;
    DataAttribute* objRef;
    DataAttribute* serviceType;
    DataAttribute* errorCode;
    DataAttribute* originatorID; /* optional */
    DataAttribute* t;

    /* CTS specific attributes */
    DataAttribute* ctlVal;
    DataAttribute* operTm; /* conditional */
    DataAttribute* origin;
    DataAttribute* ctlNum;
    DataAttribute* T;
    DataAttribute* Test;
    DataAttribute* Check;
    DataAttribute* respAddCause;
};

typedef struct sSgcbTrkInstance* SgcbTrkInstance;

struct sSgcbTrkInstance
{
    /* inherited from ServiceTrkInstance */
    DataObject* dobj;
    DataAttribute* objRef;
    DataAttribute* serviceType;
    DataAttribute* errorCode;
    DataAttribute* originatorID; /* optional */
    DataAttribute* t;

    /* SgcbTrk specific attributes */
    DataAttribute* numOfSG;
    DataAttribute* actSG;
    DataAttribute* editSG;
    DataAttribute* cnfEdit;
    DataAttribute* lActTm;
};

typedef struct sLocbTrkInstance* LocbTrkInstance;

struct sLocbTrkInstance
{
    /* inherited from ServiceTrkInstance */
    DataObject* dobj;
    DataAttribute* objRef;
    DataAttribute* serviceType;
    DataAttribute* errorCode;
    DataAttribute* originatorID; /* optional */
    DataAttribute* t;

    /* LocbTrk specific attributes */
    DataAttribute* logEna;
    DataAttribute* datSet;
    DataAttribute* trgOps;
    DataAttribute* intgPd;
    DataAttribute* logRef;
};

#endif /* (CONFIG_IEC61850_SERVICE_TRACKING == 1) */

struct sMmsMapping {
    IedModel* model;
    MmsDevice* mmsDevice;
    MmsServer mmsServer;
    LinkedList reportControls;

#if (CONFIG_IEC61850_LOG_SERVICE == 1)
    LinkedList logControls;
    LinkedList logInstances;
#endif

#if (CONFIG_INCLUDE_GOOSE_SUPPORT == 1)
    bool useIntegratedPublisher;

    LinkedList gseControls;
    char* gooseInterfaceId;

    GoCBEventHandler goCbHandler;
    void* goCbHandlerParameter;
#endif

#if (CONFIG_IEC61850_SAMPLED_VALUES_SUPPORT == 1)
    LinkedList svControls;
    const char* svInterfaceId;
#endif

    LinkedList controlObjects;
    uint64_t nextControlTimeout; /* next timeout in one of the control state machines */

    LinkedList attributeAccessHandlers;

#if (CONFIG_IEC61850_SUPPORT_USER_READ_ACCESS_CONTROL == 1)
    ReadAccessHandler readAccessHandler;
    void* readAccessHandlerParameter;
#endif

#if (CONFIG_IEC61850_SETTING_GROUPS == 1)
    bool allowEditSg;
    LinkedList settingGroups;
#endif

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    bool reportThreadRunning;
    Thread reportWorkerThread;
#endif

#if (CONFIG_IEC61850_SERVICE_TRACKING == 1)
    BrcbTrkInstance brcbTrk;
    UrcbTrkInstance urcbTrk;
    GocbTrkInstance gocbTrk;
    ControlTrkInstance spcTrk;
    ControlTrkInstance dpcTrk;
    ControlTrkInstance incTrk;
    ControlTrkInstance encTrk1;
    ControlTrkInstance apcFTrk;
    ControlTrkInstance apcIntTrk;
    ControlTrkInstance bscTrk;
    ControlTrkInstance iscTrk;
    ControlTrkInstance bacTrk;
    SgcbTrkInstance sgcbTrk;
    ServiceTrkInstance genTrk;
    LocbTrkInstance locbTrk;
#endif /* (CONFIG_IEC61850_SERVICE_TRACKING == 1) */

    /* flag indicates if data model is locked --> prevents reports to be sent */

    bool isModelLocked;

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    Semaphore isModelLockedMutex;
#endif /* (CONFIG_MMS_THREADLESS_STACK != 1) */

    IedServer iedServer;

    IedConnectionIndicationHandler connectionIndicationHandler;
    void* connectionIndicationHandlerParameter;

    IedServer_RCBEventHandler rcbEventHandler;
    void* rcbEventHandlerParameter;

    IedServer_DataSetAccessHandler dataSetAccessHandler;
    void* dataSetAccessHandlerParameter;

    IedServer_DirectoryAccessHandler directoryAccessHandler;
    void* directoryAccessHandlerParameter;

    IedServer_ListObjectsAccessHandler listObjectsAccessHandler;
    void* listObjectsAccessHandlerParameter;

    IedServer_ControlBlockAccessHandler controlBlockAccessHandler;
    void* controlBlockAccessHandlerParameter;
};

#endif /* MMS_MAPPING_INTERNAL_H_ */
