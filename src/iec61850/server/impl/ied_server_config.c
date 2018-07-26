/*
 *  ied_server_config.c
 *
 *  Copyright 2018 Michael Zillgith
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

IedServerConfig
IedServerConfig_create()
{
    IedServerConfig self = (IedServerConfig) GLOBAL_MALLOC(sizeof(struct sIedServerConfig));

    if (self) {
        self->reportBufferSize = CONFIG_REPORTING_DEFAULT_REPORT_BUFFER_SIZE;
        self->fileServiceBasepath = StringUtils_copyString(CONFIG_VIRTUAL_FILESTORE_BASEPATH);
        self->enableFileService = true;
        self->enableDynamicDataSetService = true;
    }

    return self;
}

void
IedServerConfig_destroy(IedServerConfig self)
{
    GLOBAL_FREEMEM(self->fileServiceBasepath);
    GLOBAL_FREEMEM(self);
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
IedServerConfig_setFileServiceBasePath(IedServerConfig self, const char* basepath)
{
#if (CONFIG_SET_FILESTORE_BASEPATH_AT_RUNTIME == 1)
    GLOBAL_FREEMEM(self->fileServiceBasepath);
    self->fileServiceBasepath = StringUtils_copyString(basepath);
#else
    if (DEBUG_IED_SERVER)
        printf("IED_SERVER_CONFIG: Cannot set file service basepath (enable CONFIG_SET_FILESTORE_BASEPATH_AT_RUNTIME)!\n");
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
