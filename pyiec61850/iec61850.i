/* File : iec61850.i */
%module iec61850
%ignore ControlObjectClient_setTestMode(ControlObjectClient self);
%ignore CDA_OperBoolean(ModelNode* parent, bool isTImeActivated);
%ignore LogicalNode_hasBufferedReports(LogicalNode* node);
%ignore LogicalNode_hasUnbufferedReports(LogicalNode* node);
%ignore MmsConnection_setIsoConnectionParameters(MmsConnection self, IsoConnectionParameters* params);
%include "stdint.i"
%{
#include <iec61850_client.h>
#include <iec61850_model.h>
#include <iec61850_server.h>
ModelNode* toModelNode(LogicalNode * ln)
{
    return (ModelNode*) ln;
}
ModelNode* toModelNode(DataObject * DO)
{
    return (ModelNode*) DO;
}
char* toCharP(void * v)
{
    return (char *) v;
}
DataAttribute* toDataAttribute(DataObject * DO)
{ return (DataAttribute*)DO;}
DataAttribute* toDataAttribute(ModelNode * MN)
{ return (DataAttribute*)MN;}
%}
%apply int *OUTPUT {IedClientError* error};

%include "libiec61850_common_api.h"
%include "iec61850_client.h"
%include "iso_connection_parameters.h"
%include "mms_client_connection.h"
%include "iso_connection_parameters.h"
%include "iec61850_common.h"
%include "mms_value.h"
%include "iec61850_model.h"
%include "iec61850_server.h"
%include "iec61850_dynamic_model.h"
%include "iec61850_cdc.h"
%include "linked_list.h"

/* User-defined data types, also used: */
typedef uint64_t msSinceEpoch;
typedef uint64_t nsSinceEpoch;

ModelNode* toModelNode(LogicalNode *);
ModelNode* toModelNode(DataObject *);
DataAttribute* toDataAttribute(DataObject *);
DataAttribute* toDataAttribute(ModelNode *);
char* toCharP(void *);

/* Goose Subscriber section */
%{
struct sGooseSubscriber;
typedef struct sGooseSubscriber* GooseSubscriber;
#include "goose_subscriber.h"
#include "goose_receiver.h"

void GooseSubscriber_setDstMac(GooseSubscriber subscriber,
                               uint8_t dst_mac_0,
                               uint8_t dst_mac_1,
                               uint8_t dst_mac_2,
                               uint8_t dst_mac_3,
                               uint8_t dst_mac_4,
                               uint8_t dst_mac_5)
{
    uint8_t dst_mac[6];
    dst_mac[0] = dst_mac_0;
    dst_mac[1] = dst_mac_1;
    dst_mac[2] = dst_mac_2;
    dst_mac[3] = dst_mac_3;
    dst_mac[4] = dst_mac_4;
    dst_mac[5] = dst_mac_5;

    GooseSubscriber_setDstMac(subscriber, dst_mac);
}
%}

%include "goose_subscriber.h"
%include "goose_receiver.h"

void GooseSubscriber_setDstMac(GooseSubscriber subscriber,
                               uint8_t dst_mac_0,
                               uint8_t dst_mac_1,
                               uint8_t dst_mac_2,
                               uint8_t dst_mac_3,
                               uint8_t dst_mac_4,
                               uint8_t dst_mac_5);

