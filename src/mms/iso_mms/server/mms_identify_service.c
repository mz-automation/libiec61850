/*
 *  mms_identify_service.c
 *
 *  Copyright 2013, 2014 Michael Zillgith
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
#include "mms_server_internal.h"

#if MMS_IDENTIFY_SERVICE == 1

void
mmsServer_handleIdentifyRequest(
        MmsServerConnection connection,
        uint32_t invokeId,
        ByteBuffer* response)
{
    int bufPos = 0;
    uint8_t* buffer = response->buffer;

    uint32_t invokeIdLength = BerEncoder_UInt32determineEncodedSize(invokeId);

    MmsServer mmsServer = connection->server;

    uint32_t vendorNameLength = strlen(MmsServer_getVendorName(mmsServer));
    uint32_t modelNameLength = strlen(MmsServer_getModelName(mmsServer));
    uint32_t revisionLength = strlen(MmsServer_getRevision(mmsServer));

    uint32_t identityLength = 3 +  BerEncoder_determineLengthSize(vendorNameLength)
            + BerEncoder_determineLengthSize(modelNameLength) + BerEncoder_determineLengthSize(revisionLength)
            + vendorNameLength + modelNameLength + revisionLength;

    uint32_t identifyResponseLength = invokeIdLength + 2 + 1 + BerEncoder_determineLengthSize(identityLength)
            + identityLength;

    /* Identify response pdu */
    bufPos = BerEncoder_encodeTL(0xa1, identifyResponseLength, buffer, bufPos);

    /* invokeId */
    bufPos = BerEncoder_encodeTL(0x02, invokeIdLength, buffer, bufPos);
    bufPos = BerEncoder_encodeUInt32(invokeId, buffer, bufPos);

    bufPos = BerEncoder_encodeTL(0xa2, identityLength, buffer, bufPos);
    bufPos = BerEncoder_encodeStringWithTag(0x80, MmsServer_getVendorName(mmsServer), buffer, bufPos);
    bufPos = BerEncoder_encodeStringWithTag(0x81, MmsServer_getModelName(mmsServer), buffer, bufPos);
    bufPos = BerEncoder_encodeStringWithTag(0x82, MmsServer_getRevision(mmsServer), buffer, bufPos);

    response->size = bufPos;
}

void
MmsServer_setServerIdentity(MmsServer self, char* vendorName, char* modelName, char* revision)
{
    self->vendorName = vendorName;
    self->modelName = modelName;
    self->revision = revision;
}

char*
MmsServer_getVendorName(MmsServer self)
{
    if (self->vendorName != NULL)
        return self->vendorName;
    else
        return CONFIG_DEFAULT_MMS_VENDOR_NAME;
}

char*
MmsServer_getModelName(MmsServer self)
{
    if (self->modelName != NULL)
        return self->modelName;
    else
        return CONFIG_DEFAULT_MMS_MODEL_NAME;
}

char*
MmsServer_getRevision(MmsServer self)
{
    if (self->revision != NULL)
        return self->revision;
    else
        return CONFIG_DEFAULT_MMS_REVISION;
}

#endif /* MMS_IDENTIFY_SERVICE == 1 */

