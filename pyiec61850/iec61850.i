/* File : iec61850.i */
%module(directors="1") iec61850
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
DataObject* toDataObject(ModelNode * MN)
{ return (DataObject*)MN;}
%}
%apply int *OUTPUT {IedClientError* error};

%include "cstring.i"
%cstring_bounded_output(char *buffer, 1024);

%include "libiec61850_common_api.h"
%include "iec61850_client.h"
%include "iso_connection_parameters.h"
%include "mms_client_connection.h"
%include "iso_connection_parameters.h"
%include "iec61850_common.h"
%include "mms_value.h"
%include "mms_common.h"
%include "iec61850_model.h"
%include "iec61850_server.h"
%include "iec61850_dynamic_model.h"
%include "iec61850_cdc.h"
%include "linked_list.h"
%include "iec61850_config_file_parser.h"

/* User-defined data types, also used: */
typedef uint64_t msSinceEpoch;
typedef uint64_t nsSinceEpoch;

ModelNode* toModelNode(LogicalNode *);
ModelNode* toModelNode(DataObject *);
DataAttribute* toDataAttribute(DataObject *);
DataAttribute* toDataAttribute(ModelNode *);
DataObject* toDataObject(ModelNode *);
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

/* Event Handler section */
%feature("director") RCBHandler;
%feature("director") GooseHandler;
%feature("director") CommandTermHandler;
%feature("director") CheckHandlerForPython;
%feature("director") WaitForExecutionHandlerForPython;
%feature("director") ControlHandlerForPython;
%{
#include "eventHandlers/eventHandler.hpp"
#include "eventHandlers/reportControlBlockHandler.hpp"
#include "eventHandlers/gooseHandler.hpp"
#include "eventHandlers/commandTermHandler.hpp"
#include "eventHandlers/controlActionHandler.hpp"
std::map< std::string, EventSubscriber*> EventSubscriber::m_subscriber_map = {};
%}

%include "eventHandlers/eventHandler.hpp"
%include "eventHandlers/reportControlBlockHandler.hpp"
%include "eventHandlers/gooseHandler.hpp"
%include "eventHandlers/commandTermHandler.hpp"
%include "eventHandlers/controlActionHandler.hpp"

/* Goose Publisher section */
%{
#include "goose_publisher.h"

void LinkedList_destroyDeep_MmsValueDelete(LinkedList dataSetValues)
{
    LinkedList_destroyDeep(dataSetValues, (LinkedListValueDeleteFunction) MmsValue_delete);
}
void CommParameters_setDstAddress(CommParameters *gooseCommParameters,
                                  uint8_t dst_mac_0,
                                  uint8_t dst_mac_1,
                                  uint8_t dst_mac_2,
                                  uint8_t dst_mac_3,
                                  uint8_t dst_mac_4,
                                  uint8_t dst_mac_5)
{
    gooseCommParameters->dstAddress[0] = dst_mac_0;
    gooseCommParameters->dstAddress[1] = dst_mac_1;
    gooseCommParameters->dstAddress[2] = dst_mac_2;
    gooseCommParameters->dstAddress[3] = dst_mac_3;
    gooseCommParameters->dstAddress[4] = dst_mac_4;
    gooseCommParameters->dstAddress[5] = dst_mac_5;
}
%}
%include "goose_publisher.h"
void LinkedList_destroyDeep_MmsValueDelete(LinkedList dataSetValues);
void CommParameters_setDstAddress(CommParameters *gooseCommParameters,
                                  uint8_t dst_mac_0,
                                  uint8_t dst_mac_1,
                                  uint8_t dst_mac_2,
                                  uint8_t dst_mac_3,
                                  uint8_t dst_mac_4,
                                  uint8_t dst_mac_5);


/* Wrapper for synchronous functions */
%{
#include "servicePythonWrapper.hpp"
%}
%include "servicePythonWrapper.hpp"

/* SV Subscriber section */
%{
struct sSVSubscriber;
typedef struct sSVSubscriber* SVSubscriber;
struct sSVSubscriber_ASDU {

    char* svId;
    char* datSet;

    uint8_t* smpCnt;
    uint8_t* confRev;
    uint8_t* refrTm;
    uint8_t* smpSynch;
    uint8_t* smpMod;
    uint8_t* smpRate;

    int dataBufferLength;
    uint8_t* dataBuffer;
};
typedef struct sSVSubscriber_ASDU* SVSubscriber_ASDU;
#include "sv_subscriber.h"
#include "sv_publisher.h"
%}
%include "sv_publisher.h"
%include "sv_subscriber.h"
struct sSVSubscriber_ASDU {

    char* svId;
    char* datSet;

    uint8_t* smpCnt;
    uint8_t* confRev;
    uint8_t* refrTm;
    uint8_t* smpSynch;
    uint8_t* smpMod;
    uint8_t* smpRate;

    int dataBufferLength;
    uint8_t* dataBuffer;
};
typedef struct sSVSubscriber_ASDU* SVSubscriber_ASDU;
