/*
 *  client_sv_control.c
 *
 *  Copyright 2015 Michael Zillgith
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

#include "iec61850_client.h"

#include "stack_config.h"

#include "ied_connection_private.h"

#include "libiec61850_platform_includes.h"

struct sClientSVControlBlock {
    IedConnection connection;
    bool isMulticast;
    char* reference;
    IedClientError lastError;
};

ClientSVControlBlock
ClientSVControlBlock_create(IedConnection connection, const char* reference)
{
    bool isMulticast = false;

    /* Check if CB exists and if it is a unicast or mulitcast CB */
    IedClientError error;
    MmsValue* value = IedConnection_readObject(connection, &error, reference, IEC61850_FC_MS);

    if ((error == IED_ERROR_OK) && (MmsValue_getType(value) != MMS_DATA_ACCESS_ERROR)) {
        isMulticast = true;
        MmsValue_delete(value);
    }
    else {
        MmsValue_delete(value);

        value = IedConnection_readObject(connection, &error, reference, IEC61850_FC_US);

        if ((error == IED_ERROR_OK) && (MmsValue_getType(value) != MMS_DATA_ACCESS_ERROR))
            MmsValue_delete(value);
        else {
            MmsValue_delete(value);
            return NULL;
        }
    }

    ClientSVControlBlock self = (ClientSVControlBlock) GLOBAL_CALLOC(1, sizeof(struct sClientSVControlBlock));

    if (self) {
        self->connection = connection;
        self->reference = StringUtils_copyString(reference);
        self->isMulticast = isMulticast;
    }

    return self;
}

void
ClientSVControlBlock_destroy(ClientSVControlBlock self)
{
    if (self) {
        GLOBAL_FREEMEM(self->reference);
        GLOBAL_FREEMEM(self);
    }
}


IedClientError
ClientSVControlBlock_getLastComError(ClientSVControlBlock self)
{
    return self->lastError;
}

bool
ClientSVControlBlock_isMulticast(ClientSVControlBlock self)
{
    return self->isMulticast;
}

static bool
setBooleanVariable(ClientSVControlBlock self, const char* varName, bool value)
{
    char refBuf[130];

    strcpy(refBuf, self->reference);
    strcat(refBuf, ".");
    strcat(refBuf, varName);

    self->lastError = IED_ERROR_OK;

    if (self->isMulticast)
        IedConnection_writeBooleanValue(self->connection, &(self->lastError), refBuf, IEC61850_FC_MS, value);
    else
        IedConnection_writeBooleanValue(self->connection, &(self->lastError), refBuf, IEC61850_FC_US, value);


    if (self->lastError == IED_ERROR_OK)
        return true;
    else
        return false;
}

bool
ClientSVControlBlock_setSvEna(ClientSVControlBlock self, bool value)
{
    return setBooleanVariable(self, "SvEna", value);
}

bool
ClientSVControlBlock_setResv(ClientSVControlBlock self, bool value)
{
    if (self->isMulticast == false)
        return setBooleanVariable(self, "SvEna", value);
    else
        return false;
}

static bool
readBooleanVariable(ClientSVControlBlock self, const char* varName)
{
    char refBuf[130];

    strcpy(refBuf, self->reference);
    strcat(refBuf, ".");
    strcat(refBuf, varName);

    self->lastError = IED_ERROR_OK;

    bool retVal;

    if (self->isMulticast)
        retVal = IedConnection_readBooleanValue(self->connection, &(self->lastError), refBuf, IEC61850_FC_MS);
    else
        retVal = IedConnection_readBooleanValue(self->connection, &(self->lastError), refBuf, IEC61850_FC_US);

    return retVal;
}

bool
ClientSVControlBlock_getSvEna(ClientSVControlBlock self)
{
    return readBooleanVariable(self, "SvEna");
}

bool
ClientSVControlBlock_getResv(ClientSVControlBlock self)
{
    return readBooleanVariable(self, "Resv");
}

static char*
readStringVariable(ClientSVControlBlock self, const char* varName)
{
    char refBuf[130];

    strcpy(refBuf, self->reference);
    strcat(refBuf, ".");
    strcat(refBuf, varName);

    self->lastError = IED_ERROR_OK;

    char* retVal;

    if (self->isMulticast)
        retVal = IedConnection_readStringValue(self->connection, &(self->lastError), refBuf, IEC61850_FC_MS);
    else
        retVal = IedConnection_readStringValue(self->connection, &(self->lastError), refBuf, IEC61850_FC_US);

    return retVal;
}

char*
ClientSVControlBlock_getMsvID(ClientSVControlBlock self)
{
    return readStringVariable(self, "MsvID");
}

char*
ClientSVControlBlock_getDatSet(ClientSVControlBlock self)
{
    return readStringVariable(self, "DatSet");
}

static uint32_t
readUIntVariable(ClientSVControlBlock self, const char* varName)
{
    char refBuf[130];

    strcpy(refBuf, self->reference);
    strcat(refBuf, ".");
    strcat(refBuf, varName);

    self->lastError = IED_ERROR_OK;

    uint32_t retVal;

    if (self->isMulticast)
        retVal = IedConnection_readUnsigned32Value(self->connection, &(self->lastError), refBuf, IEC61850_FC_MS);
    else
        retVal = IedConnection_readUnsigned32Value(self->connection, &(self->lastError), refBuf, IEC61850_FC_US);

    return retVal;
}

uint32_t
ClientSVControlBlock_getConfRev(ClientSVControlBlock self)
{
    return readUIntVariable(self, "ConfRev");
}

uint16_t
ClientSVControlBlock_getSmpRate(ClientSVControlBlock self)
{
    return readUIntVariable(self, "SmpRate");
}

int
ClientSVControlBlock_getOptFlds(ClientSVControlBlock self)
{
    char refBuf[130];

    strcpy(refBuf, self->reference);
    strcat(refBuf, ".");
    strcat(refBuf, "OptFlds");

    self->lastError = IED_ERROR_OK;

    MmsValue* optFlds;

    if (self->isMulticast)
        optFlds = IedConnection_readObject(self->connection, &(self->lastError), refBuf, IEC61850_FC_MS);
    else
        optFlds = IedConnection_readObject(self->connection, &(self->lastError), refBuf, IEC61850_FC_US);

    if (optFlds == NULL)
        return 0;

    int retVal = 0;

    if (MmsValue_getType(optFlds) == MMS_BIT_STRING)
        retVal = MmsValue_getBitStringAsInteger(optFlds);

    MmsValue_delete(optFlds);

    return retVal;
}

uint8_t
ClientSVControlBlock_getSmpMod(ClientSVControlBlock self)
{
    return readUIntVariable(self, "SmpMod");
}

int
ClientSVControlBlock_getNoASDU(ClientSVControlBlock self)
{
    return readUIntVariable(self, "noASDU");
}

PhyComAddress
ClientSVControlBlock_getDstAddress(ClientSVControlBlock self)
{
    char refBuf[130];

    strcpy(refBuf, self->reference);
    strcat(refBuf, ".");
    strcat(refBuf, "DstAddress");

    self->lastError = IED_ERROR_OK;

    MmsValue* dstAddrValue;

    if (self->isMulticast)
        dstAddrValue = IedConnection_readObject(self->connection, &(self->lastError), refBuf, IEC61850_FC_MS);
    else
        dstAddrValue = IedConnection_readObject(self->connection, &(self->lastError), refBuf, IEC61850_FC_US);

    PhyComAddress retVal;
    memset(&retVal, 0, sizeof(retVal));

    if (dstAddrValue == NULL) goto exit_error;

    if (MmsValue_getType(dstAddrValue) != MMS_STRUCTURE) {
        if (DEBUG_IED_CLIENT) printf("IED_CLIENT: SVCB - addr has wrong type\n");
        goto exit_cleanup;
    }

    if (MmsValue_getArraySize(dstAddrValue) != 4) {
        if (DEBUG_IED_CLIENT) printf("IED_CLIENT: SVCB - addr has wrong type\n");
        goto exit_cleanup;
    }

    MmsValue* addr = MmsValue_getElement(dstAddrValue, 0);

    if (MmsValue_getType(addr) != MMS_OCTET_STRING) {
        if (DEBUG_IED_CLIENT) printf("IED_CLIENT: SVCB - addr has wrong type\n");
        goto exit_cleanup;
    }

    if (MmsValue_getOctetStringSize(addr) != 6) {
        if (DEBUG_IED_CLIENT) printf("IED_CLIENT: SVCB - addr has wrong size\n");
        goto exit_cleanup;
    }

    uint8_t* addrBuf = MmsValue_getOctetStringBuffer(addr);

    memcpy(&(retVal.dstAddress), addrBuf, 6);

    MmsValue* prio = MmsValue_getElement(dstAddrValue, 1);

    if (MmsValue_getType(prio) != MMS_UNSIGNED) {
        if (DEBUG_IED_CLIENT) printf("IED_CLIENT: SVCB - prio has wrong type\n");
        goto exit_cleanup;
    }

    retVal.vlanPriority = MmsValue_toUint32(prio);

    MmsValue* vid = MmsValue_getElement(dstAddrValue, 2);

    if (MmsValue_getType(vid) != MMS_UNSIGNED) {
        if (DEBUG_IED_CLIENT) printf("IED_CLIENT: SVCB - vid has wrong type\n");
        goto exit_cleanup;
    }

    retVal.vlanId = MmsValue_toUint32(vid);

    MmsValue* appID = MmsValue_getElement(dstAddrValue, 3);

    if (MmsValue_getType(appID) != MMS_UNSIGNED) {
        if (DEBUG_IED_CLIENT) printf("IED_CLIENT: SVCB - appID has wrong type\n");
        goto exit_cleanup;
    }

    retVal.appId = MmsValue_toUint32(appID);


exit_cleanup:
    MmsValue_delete(dstAddrValue);

exit_error:
    return retVal;
}


