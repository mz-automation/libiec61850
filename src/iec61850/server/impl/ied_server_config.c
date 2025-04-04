/*
 *  ied_server_config.c
 *
 *  Copyright 2018-2024 Michael Zillgith
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
#include "libiec61850_platform_includes.h"

#ifndef CONFIG_MMS_MAX_NUMBER_OF_DATA_SET_MEMBERS
#define CONFIG_MMS_MAX_NUMBER_OF_DATA_SET_MEMBERS 100
#endif

#ifndef CONFIG_MMS_MAX_NUMBER_OF_ASSOCIATION_SPECIFIC_DATA_SETS
#define CONFIG_MMS_MAX_NUMBER_OF_ASSOCIATION_SPECIFIC_DATA_SETS 10
#endif

#ifndef CONFIG_MMS_MAX_NUMBER_OF_DOMAIN_SPECIFIC_DATA_SETS
#define CONFIG_MMS_MAX_NUMBER_OF_DOMAIN_SPECIFIC_DATA_SETS 10
#endif

IedServerConfig
IedServerConfig_create()
{
    IedServerConfig self = (IedServerConfig)GLOBAL_MALLOC(sizeof(struct sIedServerConfig));

    if (self)
    {
        self->reportBufferSize = CONFIG_REPORTING_DEFAULT_REPORT_BUFFER_SIZE;
        self->reportBufferSizeURCBs = CONFIG_REPORTING_DEFAULT_REPORT_BUFFER_SIZE;
        self->fileServiceBasepath = StringUtils_copyString(CONFIG_VIRTUAL_FILESTORE_BASEPATH);
        self->enableFileService = true;
        self->enableDynamicDataSetService = true;
        self->maxAssociationSpecificDataSets = CONFIG_MMS_MAX_NUMBER_OF_ASSOCIATION_SPECIFIC_DATA_SETS;
        self->maxDomainSpecificDataSets = CONFIG_MMS_MAX_NUMBER_OF_DOMAIN_SPECIFIC_DATA_SETS;
        self->maxDataSetEntries = CONFIG_MMS_MAX_NUMBER_OF_DATA_SET_MEMBERS;
        self->enableLogService = true;
        self->useIntegratedGoosePublisher = true;
        self->edition = IEC_61850_EDITION_2;
        self->maxMmsConnections = 5;
        self->enableEditSG = true;
        self->enableResvTmsForSGCB = true;
        self->enableResvTmsForBRCB = true;
        self->enableOwnerForRCB = false;
        self->syncIntegrityReportTimes = false;
        self->reportSettingsWritable = IEC61850_REPORTSETTINGS_RPT_ID + IEC61850_REPORTSETTINGS_BUF_TIME +
                                       IEC61850_REPORTSETTINGS_DATSET + IEC61850_REPORTSETTINGS_TRG_OPS +
                                       IEC61850_REPORTSETTINGS_OPT_FIELDS + IEC61850_REPORTSETTINGS_INTG_PD;
    }

    return self;
}

void
IedServerConfig_destroy(IedServerConfig self)
{
    if (self)
    {
        GLOBAL_FREEMEM(self->fileServiceBasepath);
        GLOBAL_FREEMEM(self);
    }
}

void
IedServerConfig_setEdition(IedServerConfig self, uint8_t edition)
{
    self->edition = edition;
}

uint8_t
IedServerConfig_getEdition(IedServerConfig self)
{
    return self->edition;
}

void
IedServerConfig_setReportBufferSize(IedServerConfig self, int reportBufferSize)
{
    self->reportBufferSize = reportBufferSize;
}

int
IedServerConfig_getReportBufferSize(IedServerConfig self)
{
    return self->reportBufferSize;
}

void
IedServerConfig_setReportBufferSizeForURCBs(IedServerConfig self, int reportBufferSize)
{
    self->reportBufferSizeURCBs = reportBufferSize;
}

int
IedServerConfig_getReportBufferSizeForURCBs(IedServerConfig self)
{
    return self->reportBufferSizeURCBs;
}

void
IedServerConfig_setFileServiceBasePath(IedServerConfig self, const char* basepath)
{
#if (CONFIG_SET_FILESTORE_BASEPATH_AT_RUNTIME == 1)
    GLOBAL_FREEMEM(self->fileServiceBasepath);
    self->fileServiceBasepath = StringUtils_copyString(basepath);
#else
    if (DEBUG_IED_SERVER)
        printf(
            "IED_SERVER_CONFIG: Cannot set file service basepath (enable CONFIG_SET_FILESTORE_BASEPATH_AT_RUNTIME)!\n");
#endif
}

const char*
IedServerConfig_getFileServiceBasePath(IedServerConfig self)
{
    return self->fileServiceBasepath;
}

void
IedServerConfig_enableFileService(IedServerConfig self, bool enable)
{
    self->enableFileService = enable;
}

bool
IedServerConfig_isFileServiceEnabled(IedServerConfig self)
{
    return self->enableFileService;
}

void
IedServerConfig_enableDynamicDataSetService(IedServerConfig self, bool enable)
{
    self->enableDynamicDataSetService = enable;
}

bool
IedServerConfig_isDynamicDataSetServiceEnabled(IedServerConfig self)
{
    return self->enableDynamicDataSetService;
}

void
IedServerConfig_setMaxAssociationSpecificDataSets(IedServerConfig self, int maxDataSets)
{
    self->maxAssociationSpecificDataSets = maxDataSets;
}

int
IedServerConfig_getMaxAssociationSpecificDataSets(IedServerConfig self)
{
    return self->maxAssociationSpecificDataSets;
}

void
IedServerConfig_setMaxDomainSpecificDataSets(IedServerConfig self, int maxDataSets)
{
    self->maxDomainSpecificDataSets = maxDataSets;
}

int
IedServerConfig_getMaxDomainSpecificDataSets(IedServerConfig self)
{
    return self->maxDomainSpecificDataSets;
}

void
IedServerConfig_setMaxDataSetEntries(IedServerConfig self, int maxDataSetEntries)
{
    self->maxDataSetEntries = maxDataSetEntries;
}

int
IedServerConfig_getMaxDatasSetEntries(IedServerConfig self)
{
    return self->maxDataSetEntries;
}

void
IedServerConfig_enableLogService(IedServerConfig self, bool enable)
{
    self->enableLogService = enable;
}

void
IedServerConfig_enableEditSG(IedServerConfig self, bool enable)
{
    self->enableEditSG = enable;
}

void
IedServerConfig_enableResvTmsForSGCB(IedServerConfig self, bool enable)
{
    self->enableResvTmsForSGCB = enable;
}

void
IedServerConfig_enableResvTmsForBRCB(IedServerConfig self, bool enable)
{
    self->enableResvTmsForBRCB = enable;
}

bool
IedServerConfig_isResvTmsForBRCBEnabled(IedServerConfig self)
{
    return self->enableResvTmsForBRCB;
}

void
IedServerConfig_enableOwnerForRCB(IedServerConfig self, bool enable)
{
    self->enableOwnerForRCB = enable;
}

bool
IedServerConfig_isOwnerForRCBEnabled(IedServerConfig self)
{
    return self->enableOwnerForRCB;
}

void
IedServerConfig_useIntegratedGoosePublisher(IedServerConfig self, bool enable)
{
    self->useIntegratedGoosePublisher = enable;
}

bool
IedServerConfig_isLogServiceEnabled(IedServerConfig self)
{
    return self->enableLogService;
}

void
IedServerConfig_setMaxMmsConnections(IedServerConfig self, int maxConnections)
{
    self->maxMmsConnections = maxConnections;
}

int
IedServerConfig_getMaxMmsConnections(IedServerConfig self)
{
    return self->maxMmsConnections;
}

void
IedServerConfig_setSyncIntegrityReportTimes(IedServerConfig self, bool enable)
{
    self->syncIntegrityReportTimes = enable;
}

bool
IedServerConfig_getSyncIntegrityReportTimes(IedServerConfig self)
{
    return self->syncIntegrityReportTimes;
}

static void
configureSetting(IedServerConfig self, uint8_t flags, uint8_t setting, bool value)
{
    if (flags & setting)
    {
        if (value)
        {
            self->reportSettingsWritable |= setting;
        }
        else
        {
            self->reportSettingsWritable &= ~setting;
        }
    }
}

void
IedServerConfig_setReportSetting(IedServerConfig self, uint8_t setting, bool isDyn)
{
    configureSetting(self, setting, IEC61850_REPORTSETTINGS_RPT_ID, isDyn);
    configureSetting(self, setting, IEC61850_REPORTSETTINGS_BUF_TIME, isDyn);
    configureSetting(self, setting, IEC61850_REPORTSETTINGS_DATSET, isDyn);
    configureSetting(self, setting, IEC61850_REPORTSETTINGS_TRG_OPS, isDyn);
    configureSetting(self, setting, IEC61850_REPORTSETTINGS_OPT_FIELDS, isDyn);
    configureSetting(self, setting, IEC61850_REPORTSETTINGS_INTG_PD, isDyn);
}

bool
IedServerConfig_getReportSetting(IedServerConfig self, uint8_t setting)
{
    return (self->reportSettingsWritable & setting);
}
