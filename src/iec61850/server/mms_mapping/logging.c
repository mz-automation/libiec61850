/*
 *  logging.c
 *
 *  Copyright 2016 Michael Zillgith
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

#include "libiec61850_platform_includes.h"
#include "stack_config.h"
#include "mms_mapping.h"
#include "logging.h"


LogControl*
LogControl_create(bool buffered, LogicalNode* parentLN)
{
    LogControl* self = (LogControl*) GLOBAL_MALLOC(sizeof(LogControl));

    self->enabled = false;
    self->dataSet = NULL;
    self->triggerOps = 0;

    return self;
}

void
LogControl_destroy(LogControl* self)
{
    if (self != NULL) {
        GLOBAL_FREEMEM(self);
    }
}
