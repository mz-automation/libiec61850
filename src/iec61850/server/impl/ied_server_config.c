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
    }

    return self;
}

void
IedServerConfig_destroy(IedServerConfig self)
{
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
