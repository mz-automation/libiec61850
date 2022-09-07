/*
 * static_model.c
 *
 * automatically generated from simpleIO_ltrk_tests.icd
 */
#include "static_model.h"

static void initializeValues();

extern DataSet iedModelds_GenericIO_LLN0_Events;
extern DataSet iedModelds_GenericIO_LLN0_Events2;
extern DataSet iedModelds_GenericIO_LLN0_Measurements;
extern DataSet iedModelds_GenericIO_LLN0_ServiceTracking;


extern DataSetEntry iedModelds_GenericIO_LLN0_Events_fcda0;
extern DataSetEntry iedModelds_GenericIO_LLN0_Events_fcda1;
extern DataSetEntry iedModelds_GenericIO_LLN0_Events_fcda2;
extern DataSetEntry iedModelds_GenericIO_LLN0_Events_fcda3;

DataSetEntry iedModelds_GenericIO_LLN0_Events_fcda0 = {
  "GenericIO",
  false,
  "GGIO1$ST$SPCSO1$stVal", 
  -1,
  NULL,
  NULL,
  &iedModelds_GenericIO_LLN0_Events_fcda1
};

DataSetEntry iedModelds_GenericIO_LLN0_Events_fcda1 = {
  "GenericIO",
  false,
  "GGIO1$ST$SPCSO2$stVal", 
  -1,
  NULL,
  NULL,
  &iedModelds_GenericIO_LLN0_Events_fcda2
};

DataSetEntry iedModelds_GenericIO_LLN0_Events_fcda2 = {
  "GenericIO",
  false,
  "GGIO1$ST$SPCSO3$stVal", 
  -1,
  NULL,
  NULL,
  &iedModelds_GenericIO_LLN0_Events_fcda3
};

DataSetEntry iedModelds_GenericIO_LLN0_Events_fcda3 = {
  "GenericIO",
  false,
  "GGIO1$ST$SPCSO4$stVal", 
  -1,
  NULL,
  NULL,
  NULL
};

DataSet iedModelds_GenericIO_LLN0_Events = {
  "GenericIO",
  "LLN0$Events",
  4,
  &iedModelds_GenericIO_LLN0_Events_fcda0,
  &iedModelds_GenericIO_LLN0_Events2
};

extern DataSetEntry iedModelds_GenericIO_LLN0_Events2_fcda0;
extern DataSetEntry iedModelds_GenericIO_LLN0_Events2_fcda1;
extern DataSetEntry iedModelds_GenericIO_LLN0_Events2_fcda2;
extern DataSetEntry iedModelds_GenericIO_LLN0_Events2_fcda3;

DataSetEntry iedModelds_GenericIO_LLN0_Events2_fcda0 = {
  "GenericIO",
  false,
  "GGIO1$ST$SPCSO1", 
  -1,
  NULL,
  NULL,
  &iedModelds_GenericIO_LLN0_Events2_fcda1
};

DataSetEntry iedModelds_GenericIO_LLN0_Events2_fcda1 = {
  "GenericIO",
  false,
  "GGIO1$ST$SPCSO2", 
  -1,
  NULL,
  NULL,
  &iedModelds_GenericIO_LLN0_Events2_fcda2
};

DataSetEntry iedModelds_GenericIO_LLN0_Events2_fcda2 = {
  "GenericIO",
  false,
  "GGIO1$ST$SPCSO3", 
  -1,
  NULL,
  NULL,
  &iedModelds_GenericIO_LLN0_Events2_fcda3
};

DataSetEntry iedModelds_GenericIO_LLN0_Events2_fcda3 = {
  "GenericIO",
  false,
  "GGIO1$ST$SPCSO4", 
  -1,
  NULL,
  NULL,
  NULL
};

DataSet iedModelds_GenericIO_LLN0_Events2 = {
  "GenericIO",
  "LLN0$Events2",
  4,
  &iedModelds_GenericIO_LLN0_Events2_fcda0,
  &iedModelds_GenericIO_LLN0_Measurements
};

extern DataSetEntry iedModelds_GenericIO_LLN0_Measurements_fcda0;
extern DataSetEntry iedModelds_GenericIO_LLN0_Measurements_fcda1;
extern DataSetEntry iedModelds_GenericIO_LLN0_Measurements_fcda2;
extern DataSetEntry iedModelds_GenericIO_LLN0_Measurements_fcda3;
extern DataSetEntry iedModelds_GenericIO_LLN0_Measurements_fcda4;
extern DataSetEntry iedModelds_GenericIO_LLN0_Measurements_fcda5;
extern DataSetEntry iedModelds_GenericIO_LLN0_Measurements_fcda6;
extern DataSetEntry iedModelds_GenericIO_LLN0_Measurements_fcda7;

DataSetEntry iedModelds_GenericIO_LLN0_Measurements_fcda0 = {
  "GenericIO",
  false,
  "GGIO1$MX$AnIn1$mag$f", 
  -1,
  NULL,
  NULL,
  &iedModelds_GenericIO_LLN0_Measurements_fcda1
};

DataSetEntry iedModelds_GenericIO_LLN0_Measurements_fcda1 = {
  "GenericIO",
  false,
  "GGIO1$MX$AnIn1$q", 
  -1,
  NULL,
  NULL,
  &iedModelds_GenericIO_LLN0_Measurements_fcda2
};

DataSetEntry iedModelds_GenericIO_LLN0_Measurements_fcda2 = {
  "GenericIO",
  false,
  "GGIO1$MX$AnIn2$mag$f", 
  -1,
  NULL,
  NULL,
  &iedModelds_GenericIO_LLN0_Measurements_fcda3
};

DataSetEntry iedModelds_GenericIO_LLN0_Measurements_fcda3 = {
  "GenericIO",
  false,
  "GGIO1$MX$AnIn2$q", 
  -1,
  NULL,
  NULL,
  &iedModelds_GenericIO_LLN0_Measurements_fcda4
};

DataSetEntry iedModelds_GenericIO_LLN0_Measurements_fcda4 = {
  "GenericIO",
  false,
  "GGIO1$MX$AnIn3$mag$f", 
  -1,
  NULL,
  NULL,
  &iedModelds_GenericIO_LLN0_Measurements_fcda5
};

DataSetEntry iedModelds_GenericIO_LLN0_Measurements_fcda5 = {
  "GenericIO",
  false,
  "GGIO1$MX$AnIn3$q", 
  -1,
  NULL,
  NULL,
  &iedModelds_GenericIO_LLN0_Measurements_fcda6
};

DataSetEntry iedModelds_GenericIO_LLN0_Measurements_fcda6 = {
  "GenericIO",
  false,
  "GGIO1$MX$AnIn4$mag$f", 
  -1,
  NULL,
  NULL,
  &iedModelds_GenericIO_LLN0_Measurements_fcda7
};

DataSetEntry iedModelds_GenericIO_LLN0_Measurements_fcda7 = {
  "GenericIO",
  false,
  "GGIO1$MX$AnIn4$q", 
  -1,
  NULL,
  NULL,
  NULL
};

DataSet iedModelds_GenericIO_LLN0_Measurements = {
  "GenericIO",
  "LLN0$Measurements",
  8,
  &iedModelds_GenericIO_LLN0_Measurements_fcda0,
  &iedModelds_GenericIO_LLN0_ServiceTracking
};

extern DataSetEntry iedModelds_GenericIO_LLN0_ServiceTracking_fcda0;
extern DataSetEntry iedModelds_GenericIO_LLN0_ServiceTracking_fcda1;
extern DataSetEntry iedModelds_GenericIO_LLN0_ServiceTracking_fcda2;
extern DataSetEntry iedModelds_GenericIO_LLN0_ServiceTracking_fcda3;
extern DataSetEntry iedModelds_GenericIO_LLN0_ServiceTracking_fcda4;
extern DataSetEntry iedModelds_GenericIO_LLN0_ServiceTracking_fcda5;
extern DataSetEntry iedModelds_GenericIO_LLN0_ServiceTracking_fcda6;
extern DataSetEntry iedModelds_GenericIO_LLN0_ServiceTracking_fcda7;
extern DataSetEntry iedModelds_GenericIO_LLN0_ServiceTracking_fcda8;

DataSetEntry iedModelds_GenericIO_LLN0_ServiceTracking_fcda0 = {
  "GenericIO",
  false,
  "LTRK1$SR$SpcTrk", 
  -1,
  NULL,
  NULL,
  &iedModelds_GenericIO_LLN0_ServiceTracking_fcda1
};

DataSetEntry iedModelds_GenericIO_LLN0_ServiceTracking_fcda1 = {
  "GenericIO",
  false,
  "LTRK1$SR$DpcTrk", 
  -1,
  NULL,
  NULL,
  &iedModelds_GenericIO_LLN0_ServiceTracking_fcda2
};

DataSetEntry iedModelds_GenericIO_LLN0_ServiceTracking_fcda2 = {
  "GenericIO",
  false,
  "LTRK1$SR$IncTrk", 
  -1,
  NULL,
  NULL,
  &iedModelds_GenericIO_LLN0_ServiceTracking_fcda3
};

DataSetEntry iedModelds_GenericIO_LLN0_ServiceTracking_fcda3 = {
  "GenericIO",
  false,
  "LTRK1$SR$BscTrk", 
  -1,
  NULL,
  NULL,
  &iedModelds_GenericIO_LLN0_ServiceTracking_fcda4
};

DataSetEntry iedModelds_GenericIO_LLN0_ServiceTracking_fcda4 = {
  "GenericIO",
  false,
  "LTRK1$SR$UrcbTrk", 
  -1,
  NULL,
  NULL,
  &iedModelds_GenericIO_LLN0_ServiceTracking_fcda5
};

DataSetEntry iedModelds_GenericIO_LLN0_ServiceTracking_fcda5 = {
  "GenericIO",
  false,
  "LTRK1$SR$BrcbTrk", 
  -1,
  NULL,
  NULL,
  &iedModelds_GenericIO_LLN0_ServiceTracking_fcda6
};

DataSetEntry iedModelds_GenericIO_LLN0_ServiceTracking_fcda6 = {
  "GenericIO",
  false,
  "LTRK1$SR$GocbTrk", 
  -1,
  NULL,
  NULL,
  &iedModelds_GenericIO_LLN0_ServiceTracking_fcda7
};

DataSetEntry iedModelds_GenericIO_LLN0_ServiceTracking_fcda7 = {
  "GenericIO",
  false,
  "LTRK1$SR$SgcbTrk", 
  -1,
  NULL,
  NULL,
  &iedModelds_GenericIO_LLN0_ServiceTracking_fcda8
};

DataSetEntry iedModelds_GenericIO_LLN0_ServiceTracking_fcda8 = {
  "GenericIO",
  false,
  "LTRK1$SR$LocbTrk", 
  -1,
  NULL,
  NULL,
  NULL
};

DataSet iedModelds_GenericIO_LLN0_ServiceTracking = {
  "GenericIO",
  "LLN0$ServiceTracking",
  9,
  &iedModelds_GenericIO_LLN0_ServiceTracking_fcda0,
  NULL
};

LogicalDevice iedModel_GenericIO = {
    LogicalDeviceModelType,
    "GenericIO",
    (ModelNode*) &iedModel,
    NULL,
    (ModelNode*) &iedModel_GenericIO_LLN0,
    NULL
};

LogicalNode iedModel_GenericIO_LLN0 = {
    LogicalNodeModelType,
    "LLN0",
    (ModelNode*) &iedModel_GenericIO,
    (ModelNode*) &iedModel_GenericIO_LPHD1,
    (ModelNode*) &iedModel_GenericIO_LLN0_Mod,
};

DataObject iedModel_GenericIO_LLN0_Mod = {
    DataObjectModelType,
    "Mod",
    (ModelNode*) &iedModel_GenericIO_LLN0,
    (ModelNode*) &iedModel_GenericIO_LLN0_Beh,
    (ModelNode*) &iedModel_GenericIO_LLN0_Mod_stVal,
    0,
    -1
};

DataAttribute iedModel_GenericIO_LLN0_Mod_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_LLN0_Mod,
    (ModelNode*) &iedModel_GenericIO_LLN0_Mod_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LLN0_Mod_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_LLN0_Mod,
    (ModelNode*) &iedModel_GenericIO_LLN0_Mod_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LLN0_Mod_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_LLN0_Mod,
    (ModelNode*) &iedModel_GenericIO_LLN0_Mod_ctlModel,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LLN0_Mod_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_GenericIO_LLN0_Mod,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_LLN0_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_GenericIO_LLN0,
    (ModelNode*) &iedModel_GenericIO_LLN0_Health,
    (ModelNode*) &iedModel_GenericIO_LLN0_Beh_stVal,
    0,
    -1
};

DataAttribute iedModel_GenericIO_LLN0_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_LLN0_Beh,
    (ModelNode*) &iedModel_GenericIO_LLN0_Beh_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LLN0_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_LLN0_Beh,
    (ModelNode*) &iedModel_GenericIO_LLN0_Beh_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LLN0_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_LLN0_Beh,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_LLN0_Health = {
    DataObjectModelType,
    "Health",
    (ModelNode*) &iedModel_GenericIO_LLN0,
    (ModelNode*) &iedModel_GenericIO_LLN0_NamPlt,
    (ModelNode*) &iedModel_GenericIO_LLN0_Health_stVal,
    0,
    -1
};

DataAttribute iedModel_GenericIO_LLN0_Health_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_LLN0_Health,
    (ModelNode*) &iedModel_GenericIO_LLN0_Health_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LLN0_Health_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_LLN0_Health,
    (ModelNode*) &iedModel_GenericIO_LLN0_Health_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LLN0_Health_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_LLN0_Health,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_LLN0_NamPlt = {
    DataObjectModelType,
    "NamPlt",
    (ModelNode*) &iedModel_GenericIO_LLN0,
    NULL,
    (ModelNode*) &iedModel_GenericIO_LLN0_NamPlt_vendor,
    0,
    -1
};

DataAttribute iedModel_GenericIO_LLN0_NamPlt_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_GenericIO_LLN0_NamPlt,
    (ModelNode*) &iedModel_GenericIO_LLN0_NamPlt_swRev,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LLN0_NamPlt_swRev = {
    DataAttributeModelType,
    "swRev",
    (ModelNode*) &iedModel_GenericIO_LLN0_NamPlt,
    (ModelNode*) &iedModel_GenericIO_LLN0_NamPlt_d,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LLN0_NamPlt_d = {
    DataAttributeModelType,
    "d",
    (ModelNode*) &iedModel_GenericIO_LLN0_NamPlt,
    (ModelNode*) &iedModel_GenericIO_LLN0_NamPlt_configRev,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LLN0_NamPlt_configRev = {
    DataAttributeModelType,
    "configRev",
    (ModelNode*) &iedModel_GenericIO_LLN0_NamPlt,
    (ModelNode*) &iedModel_GenericIO_LLN0_NamPlt_ldNs,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LLN0_NamPlt_ldNs = {
    DataAttributeModelType,
    "ldNs",
    (ModelNode*) &iedModel_GenericIO_LLN0_NamPlt,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_EX,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

LogicalNode iedModel_GenericIO_LPHD1 = {
    LogicalNodeModelType,
    "LPHD1",
    (ModelNode*) &iedModel_GenericIO,
    (ModelNode*) &iedModel_GenericIO_GGIO1,
    (ModelNode*) &iedModel_GenericIO_LPHD1_PhyNam,
};

DataObject iedModel_GenericIO_LPHD1_PhyNam = {
    DataObjectModelType,
    "PhyNam",
    (ModelNode*) &iedModel_GenericIO_LPHD1,
    (ModelNode*) &iedModel_GenericIO_LPHD1_PhyHealth,
    (ModelNode*) &iedModel_GenericIO_LPHD1_PhyNam_vendor,
    0,
    -1
};

DataAttribute iedModel_GenericIO_LPHD1_PhyNam_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_GenericIO_LPHD1_PhyNam,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_LPHD1_PhyHealth = {
    DataObjectModelType,
    "PhyHealth",
    (ModelNode*) &iedModel_GenericIO_LPHD1,
    (ModelNode*) &iedModel_GenericIO_LPHD1_Proxy,
    (ModelNode*) &iedModel_GenericIO_LPHD1_PhyHealth_stVal,
    0,
    -1
};

DataAttribute iedModel_GenericIO_LPHD1_PhyHealth_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_LPHD1_PhyHealth,
    (ModelNode*) &iedModel_GenericIO_LPHD1_PhyHealth_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LPHD1_PhyHealth_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_LPHD1_PhyHealth,
    (ModelNode*) &iedModel_GenericIO_LPHD1_PhyHealth_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LPHD1_PhyHealth_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_LPHD1_PhyHealth,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_LPHD1_Proxy = {
    DataObjectModelType,
    "Proxy",
    (ModelNode*) &iedModel_GenericIO_LPHD1,
    NULL,
    (ModelNode*) &iedModel_GenericIO_LPHD1_Proxy_stVal,
    0,
    -1
};

DataAttribute iedModel_GenericIO_LPHD1_Proxy_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_LPHD1_Proxy,
    (ModelNode*) &iedModel_GenericIO_LPHD1_Proxy_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LPHD1_Proxy_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_LPHD1_Proxy,
    (ModelNode*) &iedModel_GenericIO_LPHD1_Proxy_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LPHD1_Proxy_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_LPHD1_Proxy,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

LogicalNode iedModel_GenericIO_GGIO1 = {
    LogicalNodeModelType,
    "GGIO1",
    (ModelNode*) &iedModel_GenericIO,
    (ModelNode*) &iedModel_GenericIO_PTOC1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Mod,
};

DataObject iedModel_GenericIO_GGIO1_Mod = {
    DataObjectModelType,
    "Mod",
    (ModelNode*) &iedModel_GenericIO_GGIO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Beh,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Mod_stVal,
    0,
    -1
};

DataAttribute iedModel_GenericIO_GGIO1_Mod_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_Mod,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Mod_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_Mod_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_GGIO1_Mod,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Mod_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_Mod_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_GGIO1_Mod,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Mod_ctlModel,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_Mod_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_GenericIO_GGIO1_Mod,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_GGIO1_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_GenericIO_GGIO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Health,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Beh_stVal,
    0,
    -1
};

DataAttribute iedModel_GenericIO_GGIO1_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_Beh,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Beh_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_GGIO1_Beh,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Beh_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_GGIO1_Beh,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_GGIO1_Health = {
    DataObjectModelType,
    "Health",
    (ModelNode*) &iedModel_GenericIO_GGIO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_NamPlt,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Health_stVal,
    0,
    -1
};

DataAttribute iedModel_GenericIO_GGIO1_Health_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_Health,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Health_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_Health_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_GGIO1_Health,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Health_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_Health_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_GGIO1_Health,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_GGIO1_NamPlt = {
    DataObjectModelType,
    "NamPlt",
    (ModelNode*) &iedModel_GenericIO_GGIO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_NamPlt_vendor,
    0,
    -1
};

DataAttribute iedModel_GenericIO_GGIO1_NamPlt_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_GenericIO_GGIO1_NamPlt,
    (ModelNode*) &iedModel_GenericIO_GGIO1_NamPlt_swRev,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_NamPlt_swRev = {
    DataAttributeModelType,
    "swRev",
    (ModelNode*) &iedModel_GenericIO_GGIO1_NamPlt,
    (ModelNode*) &iedModel_GenericIO_GGIO1_NamPlt_d,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_NamPlt_d = {
    DataAttributeModelType,
    "d",
    (ModelNode*) &iedModel_GenericIO_GGIO1_NamPlt,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_GGIO1_AnIn1 = {
    DataObjectModelType,
    "AnIn1",
    (ModelNode*) &iedModel_GenericIO_GGIO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn2,
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn1_mag,
    0,
    -1
};

DataAttribute iedModel_GenericIO_GGIO1_AnIn1_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn1_q,
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn1_mag_f,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_AnIn1_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn1_mag,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_AnIn1_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn1_t,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_AnIn1_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn1,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_GGIO1_AnIn2 = {
    DataObjectModelType,
    "AnIn2",
    (ModelNode*) &iedModel_GenericIO_GGIO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn3,
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn2_mag,
    0,
    -1
};

DataAttribute iedModel_GenericIO_GGIO1_AnIn2_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn2,
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn2_q,
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn2_mag_f,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_AnIn2_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn2_mag,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_AnIn2_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn2,
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn2_t,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_AnIn2_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn2,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_GGIO1_AnIn3 = {
    DataObjectModelType,
    "AnIn3",
    (ModelNode*) &iedModel_GenericIO_GGIO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn4,
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn3_mag,
    0,
    -1
};

DataAttribute iedModel_GenericIO_GGIO1_AnIn3_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn3,
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn3_q,
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn3_mag_f,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_AnIn3_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn3_mag,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_AnIn3_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn3,
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn3_t,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_AnIn3_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn3,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_GGIO1_AnIn4 = {
    DataObjectModelType,
    "AnIn4",
    (ModelNode*) &iedModel_GenericIO_GGIO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn4_mag,
    0,
    -1
};

DataAttribute iedModel_GenericIO_GGIO1_AnIn4_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn4,
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn4_q,
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn4_mag_f,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_AnIn4_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn4_mag,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_AnIn4_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn4,
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn4_t,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_AnIn4_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn4,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_GGIO1_SPCSO1 = {
    DataObjectModelType,
    "SPCSO1",
    (ModelNode*) &iedModel_GenericIO_GGIO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1_origin,
    0,
    -1
};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO1_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1_ctlNum,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1_origin_orCat,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO1_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1_origin,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1_origin_orIdent,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO1_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1_origin,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO1_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1_stVal,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO1_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO1_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO1_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1_ctlModel,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO1_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1_Oper,
    NULL,
    0,
    -1,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO1_Oper = {
    DataAttributeModelType,
    "Oper",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1,
    NULL,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1_Oper_ctlVal,
    0,
    -1,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO1_Oper_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1_Oper_origin,
    NULL,
    0,
    -1,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO1_Oper_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1_Oper_ctlNum,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1_Oper_origin_orCat,
    0,
    -1,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO1_Oper_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1_Oper_origin,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1_Oper_origin_orIdent,
    NULL,
    0,
    -1,
    IEC61850_FC_CO,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO1_Oper_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1_Oper_origin,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_CO,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO1_Oper_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1_Oper_T,
    NULL,
    0,
    -1,
    IEC61850_FC_CO,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO1_Oper_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1_Oper_Test,
    NULL,
    0,
    -1,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO1_Oper_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1_Oper_Check,
    NULL,
    0,
    -1,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO1_Oper_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1_Oper,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_CO,
    IEC61850_CHECK,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_GGIO1_SPCSO2 = {
    DataObjectModelType,
    "SPCSO2",
    (ModelNode*) &iedModel_GenericIO_GGIO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_stVal,
    0,
    -1
};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Oper,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_Oper = {
    DataAttributeModelType,
    "Oper",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_ctlModel,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Oper_ctlVal,
    0,
    -1,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_Oper_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Oper_origin,
    NULL,
    0,
    -1,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_Oper_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Oper_ctlNum,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Oper_origin_orCat,
    0,
    -1,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_Oper_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Oper_origin,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Oper_origin_orIdent,
    NULL,
    0,
    -1,
    IEC61850_FC_CO,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_Oper_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Oper_origin,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_CO,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_Oper_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Oper_T,
    NULL,
    0,
    -1,
    IEC61850_FC_CO,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_Oper_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Oper_Test,
    NULL,
    0,
    -1,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_Oper_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Oper_Check,
    NULL,
    0,
    -1,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_Oper_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Oper,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_CO,
    IEC61850_CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_t,
    NULL,
    0,
    -1,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_GGIO1_SPCSO3 = {
    DataObjectModelType,
    "SPCSO3",
    (ModelNode*) &iedModel_GenericIO_GGIO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_stVal,
    0,
    -1
};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_Oper,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_Oper = {
    DataAttributeModelType,
    "Oper",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_ctlModel,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_Oper_ctlVal,
    0,
    -1,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_Oper_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_Oper_origin,
    NULL,
    0,
    -1,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_Oper_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_Oper_ctlNum,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_Oper_origin_orCat,
    0,
    -1,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_Oper_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_Oper_origin,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_Oper_origin_orIdent,
    NULL,
    0,
    -1,
    IEC61850_FC_CO,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_Oper_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_Oper_origin,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_CO,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_Oper_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_Oper_T,
    NULL,
    0,
    -1,
    IEC61850_FC_CO,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_Oper_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_Oper_Test,
    NULL,
    0,
    -1,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_Oper_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_Oper_Check,
    NULL,
    0,
    -1,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_Oper_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_Oper,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_CO,
    IEC61850_CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_t,
    NULL,
    0,
    -1,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_GGIO1_SPCSO4 = {
    DataObjectModelType,
    "SPCSO4",
    (ModelNode*) &iedModel_GenericIO_GGIO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_stVal,
    0,
    -1
};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_Oper,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_Oper = {
    DataAttributeModelType,
    "Oper",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_ctlModel,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_Oper_ctlVal,
    0,
    -1,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_Oper_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_Oper_origin,
    NULL,
    0,
    -1,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_Oper_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_Oper_ctlNum,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_Oper_origin_orCat,
    0,
    -1,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_Oper_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_Oper_origin,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_Oper_origin_orIdent,
    NULL,
    0,
    -1,
    IEC61850_FC_CO,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_Oper_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_Oper_origin,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_CO,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_Oper_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_Oper_T,
    NULL,
    0,
    -1,
    IEC61850_FC_CO,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_Oper_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_Oper_Test,
    NULL,
    0,
    -1,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_Oper_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_Oper_Check,
    NULL,
    0,
    -1,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_Oper_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_Oper,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_CO,
    IEC61850_CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_t,
    NULL,
    0,
    -1,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_GGIO1_Ind1 = {
    DataObjectModelType,
    "Ind1",
    (ModelNode*) &iedModel_GenericIO_GGIO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind2,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind1_stVal,
    0,
    -1
};

DataAttribute iedModel_GenericIO_GGIO1_Ind1_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind1_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_Ind1_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind1_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_Ind1_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind1,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_GGIO1_Ind2 = {
    DataObjectModelType,
    "Ind2",
    (ModelNode*) &iedModel_GenericIO_GGIO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind3,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind2_stVal,
    0,
    -1
};

DataAttribute iedModel_GenericIO_GGIO1_Ind2_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind2,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind2_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_Ind2_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind2,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind2_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_Ind2_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind2,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_GGIO1_Ind3 = {
    DataObjectModelType,
    "Ind3",
    (ModelNode*) &iedModel_GenericIO_GGIO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind4,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind3_stVal,
    0,
    -1
};

DataAttribute iedModel_GenericIO_GGIO1_Ind3_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind3,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind3_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_Ind3_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind3,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind3_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_Ind3_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind3,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_GGIO1_Ind4 = {
    DataObjectModelType,
    "Ind4",
    (ModelNode*) &iedModel_GenericIO_GGIO1,
    NULL,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind4_stVal,
    0,
    -1
};

DataAttribute iedModel_GenericIO_GGIO1_Ind4_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind4,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind4_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_Ind4_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind4,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind4_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_Ind4_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind4,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

LogicalNode iedModel_GenericIO_PTOC1 = {
    LogicalNodeModelType,
    "PTOC1",
    (ModelNode*) &iedModel_GenericIO,
    (ModelNode*) &iedModel_GenericIO_LTRK1,
    (ModelNode*) &iedModel_GenericIO_PTOC1_Mod,
};

DataObject iedModel_GenericIO_PTOC1_Mod = {
    DataObjectModelType,
    "Mod",
    (ModelNode*) &iedModel_GenericIO_PTOC1,
    (ModelNode*) &iedModel_GenericIO_PTOC1_Beh,
    (ModelNode*) &iedModel_GenericIO_PTOC1_Mod_stVal,
    0,
    -1
};

DataAttribute iedModel_GenericIO_PTOC1_Mod_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_PTOC1_Mod,
    (ModelNode*) &iedModel_GenericIO_PTOC1_Mod_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_PTOC1_Mod_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_PTOC1_Mod,
    (ModelNode*) &iedModel_GenericIO_PTOC1_Mod_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_PTOC1_Mod_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_PTOC1_Mod,
    (ModelNode*) &iedModel_GenericIO_PTOC1_Mod_ctlModel,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_PTOC1_Mod_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_GenericIO_PTOC1_Mod,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_PTOC1_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_GenericIO_PTOC1,
    (ModelNode*) &iedModel_GenericIO_PTOC1_Str,
    (ModelNode*) &iedModel_GenericIO_PTOC1_Beh_stVal,
    0,
    -1
};

DataAttribute iedModel_GenericIO_PTOC1_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_PTOC1_Beh,
    (ModelNode*) &iedModel_GenericIO_PTOC1_Beh_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_PTOC1_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_PTOC1_Beh,
    (ModelNode*) &iedModel_GenericIO_PTOC1_Beh_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_PTOC1_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_PTOC1_Beh,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_PTOC1_Str = {
    DataObjectModelType,
    "Str",
    (ModelNode*) &iedModel_GenericIO_PTOC1,
    (ModelNode*) &iedModel_GenericIO_PTOC1_Op,
    (ModelNode*) &iedModel_GenericIO_PTOC1_Str_general,
    0,
    -1
};

DataAttribute iedModel_GenericIO_PTOC1_Str_general = {
    DataAttributeModelType,
    "general",
    (ModelNode*) &iedModel_GenericIO_PTOC1_Str,
    (ModelNode*) &iedModel_GenericIO_PTOC1_Str_dirGeneral,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_PTOC1_Str_dirGeneral = {
    DataAttributeModelType,
    "dirGeneral",
    (ModelNode*) &iedModel_GenericIO_PTOC1_Str,
    (ModelNode*) &iedModel_GenericIO_PTOC1_Str_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_PTOC1_Str_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_PTOC1_Str,
    (ModelNode*) &iedModel_GenericIO_PTOC1_Str_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_PTOC1_Str_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_PTOC1_Str,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_PTOC1_Op = {
    DataObjectModelType,
    "Op",
    (ModelNode*) &iedModel_GenericIO_PTOC1,
    (ModelNode*) &iedModel_GenericIO_PTOC1_StrVal,
    (ModelNode*) &iedModel_GenericIO_PTOC1_Op_general,
    0,
    -1
};

DataAttribute iedModel_GenericIO_PTOC1_Op_general = {
    DataAttributeModelType,
    "general",
    (ModelNode*) &iedModel_GenericIO_PTOC1_Op,
    (ModelNode*) &iedModel_GenericIO_PTOC1_Op_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_PTOC1_Op_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_PTOC1_Op,
    (ModelNode*) &iedModel_GenericIO_PTOC1_Op_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_PTOC1_Op_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_PTOC1_Op,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_PTOC1_StrVal = {
    DataObjectModelType,
    "StrVal",
    (ModelNode*) &iedModel_GenericIO_PTOC1,
    (ModelNode*) &iedModel_GenericIO_PTOC1_OpDlTmms,
    (ModelNode*) &iedModel_GenericIO_PTOC1_StrVal_setMag,
    0,
    -1
};

DataAttribute iedModel_GenericIO_PTOC1_StrVal_setMag = {
    DataAttributeModelType,
    "setMag",
    (ModelNode*) &iedModel_GenericIO_PTOC1_StrVal,
    (ModelNode*) &iedModel_SE_GenericIO_PTOC1_StrVal_setMag,
    (ModelNode*) &iedModel_GenericIO_PTOC1_StrVal_setMag_f,
    0,
    -1,
    IEC61850_FC_SG,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_PTOC1_StrVal_setMag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_GenericIO_PTOC1_StrVal_setMag,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_SG,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_SE_GenericIO_PTOC1_StrVal_setMag = {
    DataAttributeModelType,
    "setMag",
    (ModelNode*) &iedModel_GenericIO_PTOC1_StrVal,
    NULL,
    (ModelNode*) &iedModel_SE_GenericIO_PTOC1_StrVal_setMag_f,
    0,
    -1,
    IEC61850_FC_SE,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_SE_GenericIO_PTOC1_StrVal_setMag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_SE_GenericIO_PTOC1_StrVal_setMag,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_SE,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_PTOC1_OpDlTmms = {
    DataObjectModelType,
    "OpDlTmms",
    (ModelNode*) &iedModel_GenericIO_PTOC1,
    (ModelNode*) &iedModel_GenericIO_PTOC1_RsDlTmms,
    (ModelNode*) &iedModel_GenericIO_PTOC1_OpDlTmms_setVal,
    0,
    -1
};

DataAttribute iedModel_GenericIO_PTOC1_OpDlTmms_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_PTOC1_OpDlTmms,
    (ModelNode*) &iedModel_SE_GenericIO_PTOC1_OpDlTmms_setVal,
    NULL,
    0,
    -1,
    IEC61850_FC_SG,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_SE_GenericIO_PTOC1_OpDlTmms_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_PTOC1_OpDlTmms,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_SE,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_PTOC1_RsDlTmms = {
    DataObjectModelType,
    "RsDlTmms",
    (ModelNode*) &iedModel_GenericIO_PTOC1,
    (ModelNode*) &iedModel_GenericIO_PTOC1_RstTms,
    (ModelNode*) &iedModel_GenericIO_PTOC1_RsDlTmms_setVal,
    0,
    -1
};

DataAttribute iedModel_GenericIO_PTOC1_RsDlTmms_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_PTOC1_RsDlTmms,
    (ModelNode*) &iedModel_SE_GenericIO_PTOC1_RsDlTmms_setVal,
    NULL,
    0,
    -1,
    IEC61850_FC_SG,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_SE_GenericIO_PTOC1_RsDlTmms_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_PTOC1_RsDlTmms,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_SE,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_PTOC1_RstTms = {
    DataObjectModelType,
    "RstTms",
    (ModelNode*) &iedModel_GenericIO_PTOC1,
    NULL,
    (ModelNode*) &iedModel_GenericIO_PTOC1_RstTms_setVal,
    0,
    -1
};

DataAttribute iedModel_GenericIO_PTOC1_RstTms_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_PTOC1_RstTms,
    (ModelNode*) &iedModel_SE_GenericIO_PTOC1_RstTms_setVal,
    NULL,
    0,
    -1,
    IEC61850_FC_SG,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_SE_GenericIO_PTOC1_RstTms_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_PTOC1_RstTms,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_SE,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

LogicalNode iedModel_GenericIO_LTRK1 = {
    LogicalNodeModelType,
    "LTRK1",
    (ModelNode*) &iedModel_GenericIO,
    NULL,
    (ModelNode*) &iedModel_GenericIO_LTRK1_Beh,
};

DataObject iedModel_GenericIO_LTRK1_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_GenericIO_LTRK1,
    (ModelNode*) &iedModel_GenericIO_LTRK1_SpcTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_Beh_stVal,
    0,
    -1
};

DataAttribute iedModel_GenericIO_LTRK1_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_LTRK1_Beh,
    (ModelNode*) &iedModel_GenericIO_LTRK1_Beh_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_LTRK1_Beh,
    (ModelNode*) &iedModel_GenericIO_LTRK1_Beh_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_LTRK1_Beh,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_LTRK1_SpcTrk = {
    DataObjectModelType,
    "SpcTrk",
    (ModelNode*) &iedModel_GenericIO_LTRK1,
    (ModelNode*) &iedModel_GenericIO_LTRK1_DpcTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_SpcTrk_objRef,
    0,
    -1
};

DataAttribute iedModel_GenericIO_LTRK1_SpcTrk_objRef = {
    DataAttributeModelType,
    "objRef",
    (ModelNode*) &iedModel_GenericIO_LTRK1_SpcTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_SpcTrk_serviceType,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_VISIBLE_STRING_129,
    0 + TRG_OPT_DATA_UPDATE,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_SpcTrk_serviceType = {
    DataAttributeModelType,
    "serviceType",
    (ModelNode*) &iedModel_GenericIO_LTRK1_SpcTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_SpcTrk_errorCode,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_SpcTrk_errorCode = {
    DataAttributeModelType,
    "errorCode",
    (ModelNode*) &iedModel_GenericIO_LTRK1_SpcTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_SpcTrk_originatorID,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_SpcTrk_originatorID = {
    DataAttributeModelType,
    "originatorID",
    (ModelNode*) &iedModel_GenericIO_LTRK1_SpcTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_SpcTrk_t,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_SpcTrk_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_LTRK1_SpcTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_SpcTrk_d,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_SpcTrk_d = {
    DataAttributeModelType,
    "d",
    (ModelNode*) &iedModel_GenericIO_LTRK1_SpcTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_SpcTrk_dU,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_SpcTrk_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_GenericIO_LTRK1_SpcTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_SpcTrk_cdcNs,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_SpcTrk_cdcNs = {
    DataAttributeModelType,
    "cdcNs",
    (ModelNode*) &iedModel_GenericIO_LTRK1_SpcTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_SpcTrk_cdcName,
    NULL,
    0,
    -1,
    IEC61850_FC_EX,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_SpcTrk_cdcName = {
    DataAttributeModelType,
    "cdcName",
    (ModelNode*) &iedModel_GenericIO_LTRK1_SpcTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_SpcTrk_dataNs,
    NULL,
    0,
    -1,
    IEC61850_FC_EX,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_SpcTrk_dataNs = {
    DataAttributeModelType,
    "dataNs",
    (ModelNode*) &iedModel_GenericIO_LTRK1_SpcTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_SpcTrk_ctlVal,
    NULL,
    0,
    -1,
    IEC61850_FC_EX,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_SpcTrk_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_GenericIO_LTRK1_SpcTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_SpcTrk_origin,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_SpcTrk_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_GenericIO_LTRK1_SpcTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_SpcTrk_ctlNum,
    (ModelNode*) &iedModel_GenericIO_LTRK1_SpcTrk_origin_orCat,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_SpcTrk_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_GenericIO_LTRK1_SpcTrk_origin,
    (ModelNode*) &iedModel_GenericIO_LTRK1_SpcTrk_origin_orIdent,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_SpcTrk_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_GenericIO_LTRK1_SpcTrk_origin,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_SpcTrk_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_GenericIO_LTRK1_SpcTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_SpcTrk_T,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_SpcTrk_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_GenericIO_LTRK1_SpcTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_SpcTrk_Test,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_SpcTrk_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_GenericIO_LTRK1_SpcTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_SpcTrk_Check,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_SpcTrk_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_GenericIO_LTRK1_SpcTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_SpcTrk_respAddCause,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_SpcTrk_respAddCause = {
    DataAttributeModelType,
    "respAddCause",
    (ModelNode*) &iedModel_GenericIO_LTRK1_SpcTrk,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_LTRK1_DpcTrk = {
    DataObjectModelType,
    "DpcTrk",
    (ModelNode*) &iedModel_GenericIO_LTRK1,
    (ModelNode*) &iedModel_GenericIO_LTRK1_IncTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_DpcTrk_objRef,
    0,
    -1
};

DataAttribute iedModel_GenericIO_LTRK1_DpcTrk_objRef = {
    DataAttributeModelType,
    "objRef",
    (ModelNode*) &iedModel_GenericIO_LTRK1_DpcTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_DpcTrk_serviceType,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_VISIBLE_STRING_129,
    0 + TRG_OPT_DATA_UPDATE,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_DpcTrk_serviceType = {
    DataAttributeModelType,
    "serviceType",
    (ModelNode*) &iedModel_GenericIO_LTRK1_DpcTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_DpcTrk_errorCode,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_DpcTrk_errorCode = {
    DataAttributeModelType,
    "errorCode",
    (ModelNode*) &iedModel_GenericIO_LTRK1_DpcTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_DpcTrk_originatorID,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_DpcTrk_originatorID = {
    DataAttributeModelType,
    "originatorID",
    (ModelNode*) &iedModel_GenericIO_LTRK1_DpcTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_DpcTrk_t,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_DpcTrk_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_LTRK1_DpcTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_DpcTrk_d,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_DpcTrk_d = {
    DataAttributeModelType,
    "d",
    (ModelNode*) &iedModel_GenericIO_LTRK1_DpcTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_DpcTrk_dU,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_DpcTrk_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_GenericIO_LTRK1_DpcTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_DpcTrk_cdcNs,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_DpcTrk_cdcNs = {
    DataAttributeModelType,
    "cdcNs",
    (ModelNode*) &iedModel_GenericIO_LTRK1_DpcTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_DpcTrk_cdcName,
    NULL,
    0,
    -1,
    IEC61850_FC_EX,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_DpcTrk_cdcName = {
    DataAttributeModelType,
    "cdcName",
    (ModelNode*) &iedModel_GenericIO_LTRK1_DpcTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_DpcTrk_dataNs,
    NULL,
    0,
    -1,
    IEC61850_FC_EX,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_DpcTrk_dataNs = {
    DataAttributeModelType,
    "dataNs",
    (ModelNode*) &iedModel_GenericIO_LTRK1_DpcTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_DpcTrk_ctlVal,
    NULL,
    0,
    -1,
    IEC61850_FC_EX,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_DpcTrk_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_GenericIO_LTRK1_DpcTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_DpcTrk_origin,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_DpcTrk_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_GenericIO_LTRK1_DpcTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_DpcTrk_ctlNum,
    (ModelNode*) &iedModel_GenericIO_LTRK1_DpcTrk_origin_orCat,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_DpcTrk_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_GenericIO_LTRK1_DpcTrk_origin,
    (ModelNode*) &iedModel_GenericIO_LTRK1_DpcTrk_origin_orIdent,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_DpcTrk_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_GenericIO_LTRK1_DpcTrk_origin,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_DpcTrk_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_GenericIO_LTRK1_DpcTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_DpcTrk_T,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_DpcTrk_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_GenericIO_LTRK1_DpcTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_DpcTrk_Test,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_DpcTrk_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_GenericIO_LTRK1_DpcTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_DpcTrk_Check,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_DpcTrk_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_GenericIO_LTRK1_DpcTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_DpcTrk_respAddCause,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_DpcTrk_respAddCause = {
    DataAttributeModelType,
    "respAddCause",
    (ModelNode*) &iedModel_GenericIO_LTRK1_DpcTrk,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_LTRK1_IncTrk = {
    DataObjectModelType,
    "IncTrk",
    (ModelNode*) &iedModel_GenericIO_LTRK1,
    (ModelNode*) &iedModel_GenericIO_LTRK1_BscTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_IncTrk_objRef,
    0,
    -1
};

DataAttribute iedModel_GenericIO_LTRK1_IncTrk_objRef = {
    DataAttributeModelType,
    "objRef",
    (ModelNode*) &iedModel_GenericIO_LTRK1_IncTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_IncTrk_serviceType,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_VISIBLE_STRING_129,
    0 + TRG_OPT_DATA_UPDATE,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_IncTrk_serviceType = {
    DataAttributeModelType,
    "serviceType",
    (ModelNode*) &iedModel_GenericIO_LTRK1_IncTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_IncTrk_errorCode,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_IncTrk_errorCode = {
    DataAttributeModelType,
    "errorCode",
    (ModelNode*) &iedModel_GenericIO_LTRK1_IncTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_IncTrk_originatorID,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_IncTrk_originatorID = {
    DataAttributeModelType,
    "originatorID",
    (ModelNode*) &iedModel_GenericIO_LTRK1_IncTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_IncTrk_t,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_IncTrk_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_LTRK1_IncTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_IncTrk_d,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_IncTrk_d = {
    DataAttributeModelType,
    "d",
    (ModelNode*) &iedModel_GenericIO_LTRK1_IncTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_IncTrk_dU,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_IncTrk_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_GenericIO_LTRK1_IncTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_IncTrk_cdcNs,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_IncTrk_cdcNs = {
    DataAttributeModelType,
    "cdcNs",
    (ModelNode*) &iedModel_GenericIO_LTRK1_IncTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_IncTrk_cdcName,
    NULL,
    0,
    -1,
    IEC61850_FC_EX,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_IncTrk_cdcName = {
    DataAttributeModelType,
    "cdcName",
    (ModelNode*) &iedModel_GenericIO_LTRK1_IncTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_IncTrk_dataNs,
    NULL,
    0,
    -1,
    IEC61850_FC_EX,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_IncTrk_dataNs = {
    DataAttributeModelType,
    "dataNs",
    (ModelNode*) &iedModel_GenericIO_LTRK1_IncTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_IncTrk_ctlVal,
    NULL,
    0,
    -1,
    IEC61850_FC_EX,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_IncTrk_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_GenericIO_LTRK1_IncTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_IncTrk_origin,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_INT32,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_IncTrk_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_GenericIO_LTRK1_IncTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_IncTrk_ctlNum,
    (ModelNode*) &iedModel_GenericIO_LTRK1_IncTrk_origin_orCat,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_IncTrk_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_GenericIO_LTRK1_IncTrk_origin,
    (ModelNode*) &iedModel_GenericIO_LTRK1_IncTrk_origin_orIdent,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_IncTrk_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_GenericIO_LTRK1_IncTrk_origin,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_IncTrk_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_GenericIO_LTRK1_IncTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_IncTrk_T,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_IncTrk_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_GenericIO_LTRK1_IncTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_IncTrk_Test,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_IncTrk_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_GenericIO_LTRK1_IncTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_IncTrk_Check,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_IncTrk_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_GenericIO_LTRK1_IncTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_IncTrk_respAddCause,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_IncTrk_respAddCause = {
    DataAttributeModelType,
    "respAddCause",
    (ModelNode*) &iedModel_GenericIO_LTRK1_IncTrk,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_LTRK1_BscTrk = {
    DataObjectModelType,
    "BscTrk",
    (ModelNode*) &iedModel_GenericIO_LTRK1,
    (ModelNode*) &iedModel_GenericIO_LTRK1_UrcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_BscTrk_objRef,
    0,
    -1
};

DataAttribute iedModel_GenericIO_LTRK1_BscTrk_objRef = {
    DataAttributeModelType,
    "objRef",
    (ModelNode*) &iedModel_GenericIO_LTRK1_BscTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_BscTrk_serviceType,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_VISIBLE_STRING_129,
    0 + TRG_OPT_DATA_UPDATE,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_BscTrk_serviceType = {
    DataAttributeModelType,
    "serviceType",
    (ModelNode*) &iedModel_GenericIO_LTRK1_BscTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_BscTrk_errorCode,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_BscTrk_errorCode = {
    DataAttributeModelType,
    "errorCode",
    (ModelNode*) &iedModel_GenericIO_LTRK1_BscTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_BscTrk_originatorID,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_BscTrk_originatorID = {
    DataAttributeModelType,
    "originatorID",
    (ModelNode*) &iedModel_GenericIO_LTRK1_BscTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_BscTrk_t,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_BscTrk_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_LTRK1_BscTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_BscTrk_d,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_BscTrk_d = {
    DataAttributeModelType,
    "d",
    (ModelNode*) &iedModel_GenericIO_LTRK1_BscTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_BscTrk_dU,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_BscTrk_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_GenericIO_LTRK1_BscTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_BscTrk_cdcNs,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_BscTrk_cdcNs = {
    DataAttributeModelType,
    "cdcNs",
    (ModelNode*) &iedModel_GenericIO_LTRK1_BscTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_BscTrk_cdcName,
    NULL,
    0,
    -1,
    IEC61850_FC_EX,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_BscTrk_cdcName = {
    DataAttributeModelType,
    "cdcName",
    (ModelNode*) &iedModel_GenericIO_LTRK1_BscTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_BscTrk_dataNs,
    NULL,
    0,
    -1,
    IEC61850_FC_EX,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_BscTrk_dataNs = {
    DataAttributeModelType,
    "dataNs",
    (ModelNode*) &iedModel_GenericIO_LTRK1_BscTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_BscTrk_ctlVal,
    NULL,
    0,
    -1,
    IEC61850_FC_EX,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_BscTrk_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_GenericIO_LTRK1_BscTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_BscTrk_origin,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_CODEDENUM,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_BscTrk_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_GenericIO_LTRK1_BscTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_BscTrk_ctlNum,
    (ModelNode*) &iedModel_GenericIO_LTRK1_BscTrk_origin_orCat,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_BscTrk_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_GenericIO_LTRK1_BscTrk_origin,
    (ModelNode*) &iedModel_GenericIO_LTRK1_BscTrk_origin_orIdent,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_BscTrk_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_GenericIO_LTRK1_BscTrk_origin,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_BscTrk_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_GenericIO_LTRK1_BscTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_BscTrk_T,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_BscTrk_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_GenericIO_LTRK1_BscTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_BscTrk_Test,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_BscTrk_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_GenericIO_LTRK1_BscTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_BscTrk_Check,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_BscTrk_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_GenericIO_LTRK1_BscTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_BscTrk_respAddCause,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_BscTrk_respAddCause = {
    DataAttributeModelType,
    "respAddCause",
    (ModelNode*) &iedModel_GenericIO_LTRK1_BscTrk,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_LTRK1_UrcbTrk = {
    DataObjectModelType,
    "UrcbTrk",
    (ModelNode*) &iedModel_GenericIO_LTRK1,
    (ModelNode*) &iedModel_GenericIO_LTRK1_BrcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_UrcbTrk_objRef,
    0,
    -1
};

DataAttribute iedModel_GenericIO_LTRK1_UrcbTrk_objRef = {
    DataAttributeModelType,
    "objRef",
    (ModelNode*) &iedModel_GenericIO_LTRK1_UrcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_UrcbTrk_serviceType,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_VISIBLE_STRING_129,
    0 + TRG_OPT_DATA_UPDATE,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_UrcbTrk_serviceType = {
    DataAttributeModelType,
    "serviceType",
    (ModelNode*) &iedModel_GenericIO_LTRK1_UrcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_UrcbTrk_errorCode,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_UrcbTrk_errorCode = {
    DataAttributeModelType,
    "errorCode",
    (ModelNode*) &iedModel_GenericIO_LTRK1_UrcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_UrcbTrk_originatorID,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_UrcbTrk_originatorID = {
    DataAttributeModelType,
    "originatorID",
    (ModelNode*) &iedModel_GenericIO_LTRK1_UrcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_UrcbTrk_t,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_UrcbTrk_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_LTRK1_UrcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_UrcbTrk_d,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_UrcbTrk_d = {
    DataAttributeModelType,
    "d",
    (ModelNode*) &iedModel_GenericIO_LTRK1_UrcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_UrcbTrk_dU,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_UrcbTrk_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_GenericIO_LTRK1_UrcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_UrcbTrk_cdcNs,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_UrcbTrk_cdcNs = {
    DataAttributeModelType,
    "cdcNs",
    (ModelNode*) &iedModel_GenericIO_LTRK1_UrcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_UrcbTrk_cdcName,
    NULL,
    0,
    -1,
    IEC61850_FC_EX,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_UrcbTrk_cdcName = {
    DataAttributeModelType,
    "cdcName",
    (ModelNode*) &iedModel_GenericIO_LTRK1_UrcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_UrcbTrk_dataNs,
    NULL,
    0,
    -1,
    IEC61850_FC_EX,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_UrcbTrk_dataNs = {
    DataAttributeModelType,
    "dataNs",
    (ModelNode*) &iedModel_GenericIO_LTRK1_UrcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_UrcbTrk_rptID,
    NULL,
    0,
    -1,
    IEC61850_FC_EX,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_UrcbTrk_rptID = {
    DataAttributeModelType,
    "rptID",
    (ModelNode*) &iedModel_GenericIO_LTRK1_UrcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_UrcbTrk_rptEna,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_VISIBLE_STRING_129,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_UrcbTrk_rptEna = {
    DataAttributeModelType,
    "rptEna",
    (ModelNode*) &iedModel_GenericIO_LTRK1_UrcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_UrcbTrk_resv,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_UrcbTrk_resv = {
    DataAttributeModelType,
    "resv",
    (ModelNode*) &iedModel_GenericIO_LTRK1_UrcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_UrcbTrk_datSet,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_UrcbTrk_datSet = {
    DataAttributeModelType,
    "datSet",
    (ModelNode*) &iedModel_GenericIO_LTRK1_UrcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_UrcbTrk_confRev,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_VISIBLE_STRING_129,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_UrcbTrk_confRev = {
    DataAttributeModelType,
    "confRev",
    (ModelNode*) &iedModel_GenericIO_LTRK1_UrcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_UrcbTrk_optFlds,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_UrcbTrk_optFlds = {
    DataAttributeModelType,
    "optFlds",
    (ModelNode*) &iedModel_GenericIO_LTRK1_UrcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_UrcbTrk_bufTm,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_OPTFLDS,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_UrcbTrk_bufTm = {
    DataAttributeModelType,
    "bufTm",
    (ModelNode*) &iedModel_GenericIO_LTRK1_UrcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_UrcbTrk_sqNum,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_UrcbTrk_sqNum = {
    DataAttributeModelType,
    "sqNum",
    (ModelNode*) &iedModel_GenericIO_LTRK1_UrcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_UrcbTrk_trgOps,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_UrcbTrk_trgOps = {
    DataAttributeModelType,
    "trgOps",
    (ModelNode*) &iedModel_GenericIO_LTRK1_UrcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_UrcbTrk_intgPd,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_TRGOPS,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_UrcbTrk_intgPd = {
    DataAttributeModelType,
    "intgPd",
    (ModelNode*) &iedModel_GenericIO_LTRK1_UrcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_UrcbTrk_gi,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_UrcbTrk_gi = {
    DataAttributeModelType,
    "gi",
    (ModelNode*) &iedModel_GenericIO_LTRK1_UrcbTrk,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_LTRK1_BrcbTrk = {
    DataObjectModelType,
    "BrcbTrk",
    (ModelNode*) &iedModel_GenericIO_LTRK1,
    (ModelNode*) &iedModel_GenericIO_LTRK1_GocbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_BrcbTrk_objRef,
    0,
    -1
};

DataAttribute iedModel_GenericIO_LTRK1_BrcbTrk_objRef = {
    DataAttributeModelType,
    "objRef",
    (ModelNode*) &iedModel_GenericIO_LTRK1_BrcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_BrcbTrk_serviceType,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_VISIBLE_STRING_129,
    0 + TRG_OPT_DATA_UPDATE,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_BrcbTrk_serviceType = {
    DataAttributeModelType,
    "serviceType",
    (ModelNode*) &iedModel_GenericIO_LTRK1_BrcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_BrcbTrk_errorCode,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_BrcbTrk_errorCode = {
    DataAttributeModelType,
    "errorCode",
    (ModelNode*) &iedModel_GenericIO_LTRK1_BrcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_BrcbTrk_originatorID,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_BrcbTrk_originatorID = {
    DataAttributeModelType,
    "originatorID",
    (ModelNode*) &iedModel_GenericIO_LTRK1_BrcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_BrcbTrk_t,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_BrcbTrk_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_LTRK1_BrcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_BrcbTrk_d,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_BrcbTrk_d = {
    DataAttributeModelType,
    "d",
    (ModelNode*) &iedModel_GenericIO_LTRK1_BrcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_BrcbTrk_dU,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_BrcbTrk_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_GenericIO_LTRK1_BrcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_BrcbTrk_cdcNs,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_BrcbTrk_cdcNs = {
    DataAttributeModelType,
    "cdcNs",
    (ModelNode*) &iedModel_GenericIO_LTRK1_BrcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_BrcbTrk_cdcName,
    NULL,
    0,
    -1,
    IEC61850_FC_EX,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_BrcbTrk_cdcName = {
    DataAttributeModelType,
    "cdcName",
    (ModelNode*) &iedModel_GenericIO_LTRK1_BrcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_BrcbTrk_dataNs,
    NULL,
    0,
    -1,
    IEC61850_FC_EX,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_BrcbTrk_dataNs = {
    DataAttributeModelType,
    "dataNs",
    (ModelNode*) &iedModel_GenericIO_LTRK1_BrcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_BrcbTrk_rptID,
    NULL,
    0,
    -1,
    IEC61850_FC_EX,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_BrcbTrk_rptID = {
    DataAttributeModelType,
    "rptID",
    (ModelNode*) &iedModel_GenericIO_LTRK1_BrcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_BrcbTrk_rptEna,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_VISIBLE_STRING_129,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_BrcbTrk_rptEna = {
    DataAttributeModelType,
    "rptEna",
    (ModelNode*) &iedModel_GenericIO_LTRK1_BrcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_BrcbTrk_datSet,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_BrcbTrk_datSet = {
    DataAttributeModelType,
    "datSet",
    (ModelNode*) &iedModel_GenericIO_LTRK1_BrcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_BrcbTrk_confRev,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_VISIBLE_STRING_129,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_BrcbTrk_confRev = {
    DataAttributeModelType,
    "confRev",
    (ModelNode*) &iedModel_GenericIO_LTRK1_BrcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_BrcbTrk_optFlds,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_BrcbTrk_optFlds = {
    DataAttributeModelType,
    "optFlds",
    (ModelNode*) &iedModel_GenericIO_LTRK1_BrcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_BrcbTrk_bufTm,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_OPTFLDS,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_BrcbTrk_bufTm = {
    DataAttributeModelType,
    "bufTm",
    (ModelNode*) &iedModel_GenericIO_LTRK1_BrcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_BrcbTrk_sqNum,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_BrcbTrk_sqNum = {
    DataAttributeModelType,
    "sqNum",
    (ModelNode*) &iedModel_GenericIO_LTRK1_BrcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_BrcbTrk_trgOps,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_INT16U,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_BrcbTrk_trgOps = {
    DataAttributeModelType,
    "trgOps",
    (ModelNode*) &iedModel_GenericIO_LTRK1_BrcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_BrcbTrk_intgPd,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_TRGOPS,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_BrcbTrk_intgPd = {
    DataAttributeModelType,
    "intgPd",
    (ModelNode*) &iedModel_GenericIO_LTRK1_BrcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_BrcbTrk_gi,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_BrcbTrk_gi = {
    DataAttributeModelType,
    "gi",
    (ModelNode*) &iedModel_GenericIO_LTRK1_BrcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_BrcbTrk_purgeBuf,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_BrcbTrk_purgeBuf = {
    DataAttributeModelType,
    "purgeBuf",
    (ModelNode*) &iedModel_GenericIO_LTRK1_BrcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_BrcbTrk_entryID,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_BrcbTrk_entryID = {
    DataAttributeModelType,
    "entryID",
    (ModelNode*) &iedModel_GenericIO_LTRK1_BrcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_BrcbTrk_timeOfEntry,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_OCTET_STRING_8,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_BrcbTrk_timeOfEntry = {
    DataAttributeModelType,
    "timeOfEntry",
    (ModelNode*) &iedModel_GenericIO_LTRK1_BrcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_BrcbTrk_resvTms,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_ENTRY_TIME,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_BrcbTrk_resvTms = {
    DataAttributeModelType,
    "resvTms",
    (ModelNode*) &iedModel_GenericIO_LTRK1_BrcbTrk,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_INT16,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_LTRK1_GocbTrk = {
    DataObjectModelType,
    "GocbTrk",
    (ModelNode*) &iedModel_GenericIO_LTRK1,
    (ModelNode*) &iedModel_GenericIO_LTRK1_SgcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_GocbTrk_objRef,
    0,
    -1
};

DataAttribute iedModel_GenericIO_LTRK1_GocbTrk_objRef = {
    DataAttributeModelType,
    "objRef",
    (ModelNode*) &iedModel_GenericIO_LTRK1_GocbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_GocbTrk_serviceType,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_VISIBLE_STRING_129,
    0 + TRG_OPT_DATA_UPDATE,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_GocbTrk_serviceType = {
    DataAttributeModelType,
    "serviceType",
    (ModelNode*) &iedModel_GenericIO_LTRK1_GocbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_GocbTrk_errorCode,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_GocbTrk_errorCode = {
    DataAttributeModelType,
    "errorCode",
    (ModelNode*) &iedModel_GenericIO_LTRK1_GocbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_GocbTrk_originatorID,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_GocbTrk_originatorID = {
    DataAttributeModelType,
    "originatorID",
    (ModelNode*) &iedModel_GenericIO_LTRK1_GocbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_GocbTrk_t,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_GocbTrk_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_LTRK1_GocbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_GocbTrk_d,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_GocbTrk_d = {
    DataAttributeModelType,
    "d",
    (ModelNode*) &iedModel_GenericIO_LTRK1_GocbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_GocbTrk_dU,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_GocbTrk_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_GenericIO_LTRK1_GocbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_GocbTrk_cdcNs,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_GocbTrk_cdcNs = {
    DataAttributeModelType,
    "cdcNs",
    (ModelNode*) &iedModel_GenericIO_LTRK1_GocbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_GocbTrk_cdcName,
    NULL,
    0,
    -1,
    IEC61850_FC_EX,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_GocbTrk_cdcName = {
    DataAttributeModelType,
    "cdcName",
    (ModelNode*) &iedModel_GenericIO_LTRK1_GocbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_GocbTrk_dataNs,
    NULL,
    0,
    -1,
    IEC61850_FC_EX,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_GocbTrk_dataNs = {
    DataAttributeModelType,
    "dataNs",
    (ModelNode*) &iedModel_GenericIO_LTRK1_GocbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_GocbTrk_goEna,
    NULL,
    0,
    -1,
    IEC61850_FC_EX,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_GocbTrk_goEna = {
    DataAttributeModelType,
    "goEna",
    (ModelNode*) &iedModel_GenericIO_LTRK1_GocbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_GocbTrk_goID,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_GocbTrk_goID = {
    DataAttributeModelType,
    "goID",
    (ModelNode*) &iedModel_GenericIO_LTRK1_GocbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_GocbTrk_datSet,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_VISIBLE_STRING_129,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_GocbTrk_datSet = {
    DataAttributeModelType,
    "datSet",
    (ModelNode*) &iedModel_GenericIO_LTRK1_GocbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_GocbTrk_confRev,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_VISIBLE_STRING_129,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_GocbTrk_confRev = {
    DataAttributeModelType,
    "confRev",
    (ModelNode*) &iedModel_GenericIO_LTRK1_GocbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_GocbTrk_ndsCom,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_GocbTrk_ndsCom = {
    DataAttributeModelType,
    "ndsCom",
    (ModelNode*) &iedModel_GenericIO_LTRK1_GocbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_GocbTrk_dstAddress,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_GocbTrk_dstAddress = {
    DataAttributeModelType,
    "dstAddress",
    (ModelNode*) &iedModel_GenericIO_LTRK1_GocbTrk,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_PHYCOMADDR,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_LTRK1_SgcbTrk = {
    DataObjectModelType,
    "SgcbTrk",
    (ModelNode*) &iedModel_GenericIO_LTRK1,
    (ModelNode*) &iedModel_GenericIO_LTRK1_LocbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_SgcbTrk_objRef,
    0,
    -1
};

DataAttribute iedModel_GenericIO_LTRK1_SgcbTrk_objRef = {
    DataAttributeModelType,
    "objRef",
    (ModelNode*) &iedModel_GenericIO_LTRK1_SgcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_SgcbTrk_serviceType,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_VISIBLE_STRING_129,
    0 + TRG_OPT_DATA_UPDATE,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_SgcbTrk_serviceType = {
    DataAttributeModelType,
    "serviceType",
    (ModelNode*) &iedModel_GenericIO_LTRK1_SgcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_SgcbTrk_errorCode,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_SgcbTrk_errorCode = {
    DataAttributeModelType,
    "errorCode",
    (ModelNode*) &iedModel_GenericIO_LTRK1_SgcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_SgcbTrk_originatorID,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_SgcbTrk_originatorID = {
    DataAttributeModelType,
    "originatorID",
    (ModelNode*) &iedModel_GenericIO_LTRK1_SgcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_SgcbTrk_t,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_SgcbTrk_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_LTRK1_SgcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_SgcbTrk_d,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_SgcbTrk_d = {
    DataAttributeModelType,
    "d",
    (ModelNode*) &iedModel_GenericIO_LTRK1_SgcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_SgcbTrk_dU,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_SgcbTrk_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_GenericIO_LTRK1_SgcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_SgcbTrk_cdcNs,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_SgcbTrk_cdcNs = {
    DataAttributeModelType,
    "cdcNs",
    (ModelNode*) &iedModel_GenericIO_LTRK1_SgcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_SgcbTrk_cdcName,
    NULL,
    0,
    -1,
    IEC61850_FC_EX,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_SgcbTrk_cdcName = {
    DataAttributeModelType,
    "cdcName",
    (ModelNode*) &iedModel_GenericIO_LTRK1_SgcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_SgcbTrk_dataNs,
    NULL,
    0,
    -1,
    IEC61850_FC_EX,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_SgcbTrk_dataNs = {
    DataAttributeModelType,
    "dataNs",
    (ModelNode*) &iedModel_GenericIO_LTRK1_SgcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_SgcbTrk_numOfSG,
    NULL,
    0,
    -1,
    IEC61850_FC_EX,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_SgcbTrk_numOfSG = {
    DataAttributeModelType,
    "numOfSG",
    (ModelNode*) &iedModel_GenericIO_LTRK1_SgcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_SgcbTrk_actSG,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_SgcbTrk_actSG = {
    DataAttributeModelType,
    "actSG",
    (ModelNode*) &iedModel_GenericIO_LTRK1_SgcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_SgcbTrk_editSG,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_SgcbTrk_editSG = {
    DataAttributeModelType,
    "editSG",
    (ModelNode*) &iedModel_GenericIO_LTRK1_SgcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_SgcbTrk_cnfEdit,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_SgcbTrk_cnfEdit = {
    DataAttributeModelType,
    "cnfEdit",
    (ModelNode*) &iedModel_GenericIO_LTRK1_SgcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_SgcbTrk_lActTm,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_SgcbTrk_lActTm = {
    DataAttributeModelType,
    "lActTm",
    (ModelNode*) &iedModel_GenericIO_LTRK1_SgcbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_SgcbTrk_resvTms,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_SgcbTrk_resvTms = {
    DataAttributeModelType,
    "resvTms",
    (ModelNode*) &iedModel_GenericIO_LTRK1_SgcbTrk,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_INT16U,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_LTRK1_LocbTrk = {
    DataObjectModelType,
    "LocbTrk",
    (ModelNode*) &iedModel_GenericIO_LTRK1,
    NULL,
    (ModelNode*) &iedModel_GenericIO_LTRK1_LocbTrk_objRef,
    0,
    -1
};

DataAttribute iedModel_GenericIO_LTRK1_LocbTrk_objRef = {
    DataAttributeModelType,
    "objRef",
    (ModelNode*) &iedModel_GenericIO_LTRK1_LocbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_LocbTrk_serviceType,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_VISIBLE_STRING_129,
    0 + TRG_OPT_DATA_UPDATE,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_LocbTrk_serviceType = {
    DataAttributeModelType,
    "serviceType",
    (ModelNode*) &iedModel_GenericIO_LTRK1_LocbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_LocbTrk_errorCode,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_LocbTrk_errorCode = {
    DataAttributeModelType,
    "errorCode",
    (ModelNode*) &iedModel_GenericIO_LTRK1_LocbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_LocbTrk_originatorID,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_LocbTrk_originatorID = {
    DataAttributeModelType,
    "originatorID",
    (ModelNode*) &iedModel_GenericIO_LTRK1_LocbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_LocbTrk_t,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_LocbTrk_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_LTRK1_LocbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_LocbTrk_d,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_LocbTrk_d = {
    DataAttributeModelType,
    "d",
    (ModelNode*) &iedModel_GenericIO_LTRK1_LocbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_LocbTrk_dU,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_LocbTrk_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_GenericIO_LTRK1_LocbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_LocbTrk_cdcNs,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_LocbTrk_cdcNs = {
    DataAttributeModelType,
    "cdcNs",
    (ModelNode*) &iedModel_GenericIO_LTRK1_LocbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_LocbTrk_cdcName,
    NULL,
    0,
    -1,
    IEC61850_FC_EX,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_LocbTrk_cdcName = {
    DataAttributeModelType,
    "cdcName",
    (ModelNode*) &iedModel_GenericIO_LTRK1_LocbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_LocbTrk_dataNs,
    NULL,
    0,
    -1,
    IEC61850_FC_EX,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_LocbTrk_dataNs = {
    DataAttributeModelType,
    "dataNs",
    (ModelNode*) &iedModel_GenericIO_LTRK1_LocbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_LocbTrk_logEna,
    NULL,
    0,
    -1,
    IEC61850_FC_EX,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_LocbTrk_logEna = {
    DataAttributeModelType,
    "logEna",
    (ModelNode*) &iedModel_GenericIO_LTRK1_LocbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_LocbTrk_datSet,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_LocbTrk_datSet = {
    DataAttributeModelType,
    "datSet",
    (ModelNode*) &iedModel_GenericIO_LTRK1_LocbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_LocbTrk_bufTm,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_VISIBLE_STRING_129,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_LocbTrk_bufTm = {
    DataAttributeModelType,
    "bufTm",
    (ModelNode*) &iedModel_GenericIO_LTRK1_LocbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_LocbTrk_trgOps,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_LocbTrk_trgOps = {
    DataAttributeModelType,
    "trgOps",
    (ModelNode*) &iedModel_GenericIO_LTRK1_LocbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_LocbTrk_intgPd,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_TRGOPS,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_LocbTrk_intgPd = {
    DataAttributeModelType,
    "intgPd",
    (ModelNode*) &iedModel_GenericIO_LTRK1_LocbTrk,
    (ModelNode*) &iedModel_GenericIO_LTRK1_LocbTrk_logRef,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LTRK1_LocbTrk_logRef = {
    DataAttributeModelType,
    "logRef",
    (ModelNode*) &iedModel_GenericIO_LTRK1_LocbTrk,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_SR,
    IEC61850_VISIBLE_STRING_129,
    0,
    NULL,
    0};

extern ReportControlBlock iedModel_GenericIO_LLN0_report0;
extern ReportControlBlock iedModel_GenericIO_LLN0_report1;
extern ReportControlBlock iedModel_GenericIO_LLN0_report2;
extern ReportControlBlock iedModel_GenericIO_LLN0_report3;
extern ReportControlBlock iedModel_GenericIO_LLN0_report4;
extern ReportControlBlock iedModel_GenericIO_LLN0_report5;
extern ReportControlBlock iedModel_GenericIO_LLN0_report6;
extern ReportControlBlock iedModel_GenericIO_LLN0_report7;
extern ReportControlBlock iedModel_GenericIO_LLN0_report8;
extern ReportControlBlock iedModel_GenericIO_LLN0_report9;

ReportControlBlock iedModel_GenericIO_LLN0_report0 = {&iedModel_GenericIO_LLN0, "EventsRCB01", "Events1", false, "Events2", 1, 24, 175, 50, 1000, {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, &iedModel_GenericIO_LLN0_report1};
ReportControlBlock iedModel_GenericIO_LLN0_report1 = {&iedModel_GenericIO_LLN0, "EventsIndexed01", "Events2", false, "Events", 1, 24, 175, 50, 1000, {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, &iedModel_GenericIO_LLN0_report2};
ReportControlBlock iedModel_GenericIO_LLN0_report2 = {&iedModel_GenericIO_LLN0, "EventsIndexed02", "Events2", false, "Events", 1, 24, 175, 50, 1000, {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, &iedModel_GenericIO_LLN0_report3};
ReportControlBlock iedModel_GenericIO_LLN0_report3 = {&iedModel_GenericIO_LLN0, "EventsIndexed03", "Events2", false, "Events", 1, 24, 175, 50, 1000, {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, &iedModel_GenericIO_LLN0_report4};
ReportControlBlock iedModel_GenericIO_LLN0_report4 = {&iedModel_GenericIO_LLN0, "Measurements01", "Measurements", true, "Measurements", 1, 16, 239, 50, 1000, {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, &iedModel_GenericIO_LLN0_report5};
ReportControlBlock iedModel_GenericIO_LLN0_report5 = {&iedModel_GenericIO_LLN0, "Measurements02", "Measurements", true, "Measurements", 1, 16, 239, 50, 1000, {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, &iedModel_GenericIO_LLN0_report6};
ReportControlBlock iedModel_GenericIO_LLN0_report6 = {&iedModel_GenericIO_LLN0, "Measurements03", "Measurements", true, "Measurements", 1, 16, 239, 50, 1000, {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, &iedModel_GenericIO_LLN0_report7};
ReportControlBlock iedModel_GenericIO_LLN0_report7 = {&iedModel_GenericIO_LLN0, "brcbServiceTracking01", "ServiceTracking", true, "ServiceTracking", 1, 19, 228, 0, 0, {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, &iedModel_GenericIO_LLN0_report8};
ReportControlBlock iedModel_GenericIO_LLN0_report8 = {&iedModel_GenericIO_LLN0, "brcbServiceTracking02", "ServiceTracking", true, "ServiceTracking", 1, 19, 228, 0, 0, {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, &iedModel_GenericIO_LLN0_report9};
ReportControlBlock iedModel_GenericIO_LLN0_report9 = {&iedModel_GenericIO_LLN0, "brcbServiceTracking03", "ServiceTracking", true, "ServiceTracking", 1, 19, 228, 0, 0, {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, NULL};


extern GSEControlBlock iedModel_GenericIO_LLN0_gse0;

static PhyComAddress iedModel_GenericIO_LLN0_gse0_address = {
  4,
  1,
  4096,
  {0x1, 0xc, 0xcd, 0x1, 0x0, 0x1}
};

GSEControlBlock iedModel_GenericIO_LLN0_gse0 = {&iedModel_GenericIO_LLN0, "gcbEvents", "events", "Events", 3, false, &iedModel_GenericIO_LLN0_gse0_address, 1000, 3000, NULL};

extern SettingGroupControlBlock iedModel_GenericIO_LLN0_sgcb;

SettingGroupControlBlock iedModel_GenericIO_LLN0_sgcb = {&iedModel_GenericIO_LLN0, 1, 5, 0, false, 0, 0, NULL};

extern LogControlBlock iedModel_GenericIO_LLN0_lcb0;
extern LogControlBlock iedModel_GenericIO_LLN0_lcb1;
LogControlBlock iedModel_GenericIO_LLN0_lcb0 = {&iedModel_GenericIO_LLN0, "EventLog", "Events", "GenericIO/LLN0$EventLog", 3, 0, true, true, &iedModel_GenericIO_LLN0_lcb1};
LogControlBlock iedModel_GenericIO_LLN0_lcb1 = {&iedModel_GenericIO_LLN0, "GeneralLog", NULL, NULL, 3, 0, true, true, NULL};

extern Log iedModel_GenericIO_LLN0_log0;
extern Log iedModel_GenericIO_LLN0_log1;
Log iedModel_GenericIO_LLN0_log0 = {&iedModel_GenericIO_LLN0, "GeneralLog", &iedModel_GenericIO_LLN0_log1};
Log iedModel_GenericIO_LLN0_log1 = {&iedModel_GenericIO_LLN0, "EventLog", NULL};


IedModel iedModel = {
    "simpleIO",
    &iedModel_GenericIO,
    &iedModelds_GenericIO_LLN0_Events,
    &iedModel_GenericIO_LLN0_report0,
    &iedModel_GenericIO_LLN0_gse0,
    NULL,
    &iedModel_GenericIO_LLN0_sgcb,
    &iedModel_GenericIO_LLN0_lcb0,
    &iedModel_GenericIO_LLN0_log0,
    initializeValues
};

static void
initializeValues()
{

iedModel_GenericIO_LLN0_Mod_stVal.mmsValue = MmsValue_newIntegerFromInt32(1);

iedModel_GenericIO_LLN0_Mod_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(0);

iedModel_GenericIO_LLN0_Beh_stVal.mmsValue = MmsValue_newIntegerFromInt32(1);

iedModel_GenericIO_LLN0_Health_stVal.mmsValue = MmsValue_newIntegerFromInt32(1);

iedModel_GenericIO_LLN0_NamPlt_vendor.mmsValue = MmsValue_newVisibleString("MZ Automation");

iedModel_GenericIO_LLN0_NamPlt_swRev.mmsValue = MmsValue_newVisibleString("1.3.0");

iedModel_GenericIO_LLN0_NamPlt_d.mmsValue = MmsValue_newVisibleString("libiec61850 server example");

iedModel_GenericIO_LPHD1_PhyHealth_stVal.mmsValue = MmsValue_newIntegerFromInt32(1);

iedModel_GenericIO_GGIO1_Mod_stVal.mmsValue = MmsValue_newIntegerFromInt32(1);

iedModel_GenericIO_GGIO1_Mod_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(0);

iedModel_GenericIO_GGIO1_Beh_stVal.mmsValue = MmsValue_newIntegerFromInt32(1);

iedModel_GenericIO_GGIO1_Health_stVal.mmsValue = MmsValue_newIntegerFromInt32(1);

iedModel_GenericIO_GGIO1_SPCSO1_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(1);

iedModel_GenericIO_GGIO1_SPCSO2_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(1);

iedModel_GenericIO_GGIO1_SPCSO3_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(1);

iedModel_GenericIO_GGIO1_SPCSO4_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(1);
}
