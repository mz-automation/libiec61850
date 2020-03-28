/*
 *  mms_mapping_internal.h
 *
 *  Copyright 2013-2018 Michael Zillgith
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
    LinkedList gseControls;
    const char* gooseInterfaceId;
#endif

#if (CONFIG_IEC61850_SAMPLED_VALUES_SUPPORT == 1)
    LinkedList svControls;
    const char* svInterfaceId;
#endif

    LinkedList controlObjects;

    LinkedList attributeAccessHandlers;

#if (CONFIG_IEC61850_SUPPORT_USER_READ_ACCESS_CONTROL == 1)
    ReadAccessHandler readAccessHandler;
    void* readAccessHandlerParameter;
#endif

#if (CONFIG_IEC61850_SETTING_GROUPS == 1)
    LinkedList settingGroups;
#endif

#if (CONFIG_MMS_THREADLESS_STACK != 1)
    bool reportThreadRunning;
    Thread reportWorkerThread;
#endif

    /* flag indicates if data model is locked --> prevents reports to be sent */
    bool isModelLocked;

    IedServer iedServer;

    IedConnectionIndicationHandler connectionIndicationHandler;
    void* connectionIndicationHandlerParameter;
};

#endif /* MMS_MAPPING_INTERNAL_H_ */
