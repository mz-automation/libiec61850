/*
 * static_model.c
 *
 * automatically generated from beagle_demo.iid
 */
#include "static_model.h"

static void initializeValues();

extern DataSet iedModelds_GenericIO_LLN0_Events;


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
  "GGIO1$ST$DPCSO1$stVal",
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
  NULL
};

LogicalDevice iedModel_GenericIO = {
    LogicalDeviceModelType,
    "GenericIO",
    (ModelNode*) &iedModel,
    NULL,
    (ModelNode*) &iedModel_GenericIO_LLN0
};

LogicalNode iedModel_GenericIO_LLN0 = {
    LogicalNodeModelType,
    "LLN0",
    (ModelNode*) &iedModel_GenericIO,
    (ModelNode*) &iedModel_GenericIO_LPHD1,
    (ModelNode*) &iedModel_GenericIO_LLN0_Beh,
};

DataObject iedModel_GenericIO_LLN0_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_GenericIO_LLN0,
    (ModelNode*) &iedModel_GenericIO_LLN0_Mod,
    (ModelNode*) &iedModel_GenericIO_LLN0_Beh_stVal,
    0
};

DataAttribute iedModel_GenericIO_LLN0_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_LLN0_Beh,
    (ModelNode*) &iedModel_GenericIO_LLN0_Beh_q,
    NULL,
    0,
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
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_LLN0_Mod = {
    DataObjectModelType,
    "Mod",
    (ModelNode*) &iedModel_GenericIO_LLN0,
    (ModelNode*) &iedModel_GenericIO_LLN0_Health,
    (ModelNode*) &iedModel_GenericIO_LLN0_Mod_stVal,
    0
};

DataAttribute iedModel_GenericIO_LLN0_Mod_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_LLN0_Mod,
    (ModelNode*) &iedModel_GenericIO_LLN0_Mod_q,
    NULL,
    0,
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
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_LLN0_Health = {
    DataObjectModelType,
    "Health",
    (ModelNode*) &iedModel_GenericIO_LLN0,
    (ModelNode*) &iedModel_GenericIO_LLN0_NamPlt,
    (ModelNode*) &iedModel_GenericIO_LLN0_Health_stVal,
    0
};

DataAttribute iedModel_GenericIO_LLN0_Health_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_LLN0_Health,
    (ModelNode*) &iedModel_GenericIO_LLN0_Health_q,
    NULL,
    0,
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
    0
};

DataAttribute iedModel_GenericIO_LLN0_NamPlt_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_GenericIO_LLN0_NamPlt,
    (ModelNode*) &iedModel_GenericIO_LLN0_NamPlt_swRev,
    NULL,
    0,
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
    0
};

DataAttribute iedModel_GenericIO_LPHD1_PhyNam_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_GenericIO_LPHD1_PhyNam,
    NULL,
    NULL,
    0,
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
    0
};

DataAttribute iedModel_GenericIO_LPHD1_PhyHealth_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_LPHD1_PhyHealth,
    (ModelNode*) &iedModel_GenericIO_LPHD1_PhyHealth_q,
    NULL,
    0,
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
    0
};

DataAttribute iedModel_GenericIO_LPHD1_Proxy_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_LPHD1_Proxy,
    (ModelNode*) &iedModel_GenericIO_LPHD1_Proxy_q,
    NULL,
    0,
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
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

LogicalNode iedModel_GenericIO_GGIO1 = {
    LogicalNodeModelType,
    "GGIO1",
    (ModelNode*) &iedModel_GenericIO,
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Beh,
};

DataObject iedModel_GenericIO_GGIO1_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_GenericIO_GGIO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Mod,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Beh_stVal,
    0
};

DataAttribute iedModel_GenericIO_GGIO1_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_Beh,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Beh_q,
    NULL,
    0,
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
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_GGIO1_Mod = {
    DataObjectModelType,
    "Mod",
    (ModelNode*) &iedModel_GenericIO_GGIO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Health,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Mod_stVal,
    0
};

DataAttribute iedModel_GenericIO_GGIO1_Mod_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_Mod,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Mod_q,
    NULL,
    0,
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
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_GGIO1_Health = {
    DataObjectModelType,
    "Health",
    (ModelNode*) &iedModel_GenericIO_GGIO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_NamPlt,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Health_stVal,
    0
};

DataAttribute iedModel_GenericIO_GGIO1_Health_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_Health,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Health_q,
    NULL,
    0,
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
    0
};

DataAttribute iedModel_GenericIO_GGIO1_NamPlt_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_GenericIO_GGIO1_NamPlt,
    (ModelNode*) &iedModel_GenericIO_GGIO1_NamPlt_swRev,
    NULL,
    0,
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
    0
};

DataAttribute iedModel_GenericIO_GGIO1_AnIn1_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn1_q,
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn1_mag_f,
    0,
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
    0
};

DataAttribute iedModel_GenericIO_GGIO1_AnIn2_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn2,
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn2_q,
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn2_mag_f,
    0,
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
    0
};

DataAttribute iedModel_GenericIO_GGIO1_AnIn3_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn3,
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn3_q,
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn3_mag_f,
    0,
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
    0
};

DataAttribute iedModel_GenericIO_GGIO1_AnIn4_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn4,
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn4_q,
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn4_mag_f,
    0,
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
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1_stVal,
    0
};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO1_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO1_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1_Oper,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO1_Oper = {
    DataAttributeModelType,
    "Oper",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1_ctlModel,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1_Oper_ctlVal,
    0,
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
    IEC61850_FC_CO,
    IEC61850_CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO1_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1_t,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO1_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_GGIO1_SPCSO2 = {
    DataObjectModelType,
    "SPCSO2",
    (ModelNode*) &iedModel_GenericIO_GGIO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_stVal,
    0
};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_SBO,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_SBO = {
    DataAttributeModelType,
    "SBO",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Oper,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_VISIBLE_STRING_129,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_Oper = {
    DataAttributeModelType,
    "Oper",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Cancel,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Oper_ctlVal,
    0,
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
    IEC61850_FC_CO,
    IEC61850_CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_Cancel = {
    DataAttributeModelType,
    "Cancel",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_ctlModel,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Cancel_ctlVal,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_Cancel_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Cancel,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Cancel_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_Cancel_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Cancel,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Cancel_ctlNum,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Cancel_origin_orCat,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_Cancel_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Cancel_origin,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Cancel_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_Cancel_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Cancel_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_Cancel_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Cancel,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Cancel_T,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_Cancel_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Cancel,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Cancel_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_Cancel_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Cancel,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
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
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_GGIO1_SPCSO3 = {
    DataObjectModelType,
    "SPCSO3",
    (ModelNode*) &iedModel_GenericIO_GGIO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_DPCSO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_stVal,
    0
};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_q,
    NULL,
    0,
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
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_GGIO1_DPCSO1 = {
    DataObjectModelType,
    "DPCSO1",
    (ModelNode*) &iedModel_GenericIO_GGIO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_DPCSO1_stVal,
    0
};

DataAttribute iedModel_GenericIO_GGIO1_DPCSO1_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_DPCSO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_DPCSO1_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_CODEDENUM,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_DPCSO1_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_GGIO1_DPCSO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_DPCSO1_Oper,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_DPCSO1_Oper = {
    DataAttributeModelType,
    "Oper",
    (ModelNode*) &iedModel_GenericIO_GGIO1_DPCSO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_DPCSO1_ctlModel,
    (ModelNode*) &iedModel_GenericIO_GGIO1_DPCSO1_Oper_ctlVal,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_DPCSO1_Oper_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_DPCSO1_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_DPCSO1_Oper_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_DPCSO1_Oper_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_GenericIO_GGIO1_DPCSO1_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_DPCSO1_Oper_ctlNum,
    (ModelNode*) &iedModel_GenericIO_GGIO1_DPCSO1_Oper_origin_orCat,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_DPCSO1_Oper_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_GenericIO_GGIO1_DPCSO1_Oper_origin,
    (ModelNode*) &iedModel_GenericIO_GGIO1_DPCSO1_Oper_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_DPCSO1_Oper_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_GenericIO_GGIO1_DPCSO1_Oper_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_DPCSO1_Oper_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_GenericIO_GGIO1_DPCSO1_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_DPCSO1_Oper_T,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_DPCSO1_Oper_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_GenericIO_GGIO1_DPCSO1_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_DPCSO1_Oper_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_DPCSO1_Oper_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_GenericIO_GGIO1_DPCSO1_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_DPCSO1_Oper_Check,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_DPCSO1_Oper_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_GenericIO_GGIO1_DPCSO1_Oper,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_DPCSO1_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_GenericIO_GGIO1_DPCSO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_DPCSO1_t,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_DPCSO1_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_GGIO1_DPCSO1,
    NULL,
    NULL,
    0,
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
    0
};

DataAttribute iedModel_GenericIO_GGIO1_Ind1_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind1_q,
    NULL,
    0,
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
    0
};

DataAttribute iedModel_GenericIO_GGIO1_Ind2_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind2,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind2_q,
    NULL,
    0,
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
    0
};

DataAttribute iedModel_GenericIO_GGIO1_Ind3_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind3,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind3_q,
    NULL,
    0,
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
    0
};

DataAttribute iedModel_GenericIO_GGIO1_Ind4_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind4,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind4_q,
    NULL,
    0,
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
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

LogicalNode iedModel_GenericIO_TIM_GAPC1 = {
    LogicalNodeModelType,
    "TIM_GAPC1",
    (ModelNode*) &iedModel_GenericIO,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_Beh,
};

DataObject iedModel_GenericIO_TIM_GAPC1_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1,
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_Mod,
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_Beh_stVal,
    0
};

DataAttribute iedModel_GenericIO_TIM_GAPC1_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_Beh,
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_Beh_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_TIM_GAPC1_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_Beh,
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_Beh_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_TIM_GAPC1_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_Beh,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_TIM_GAPC1_Mod = {
    DataObjectModelType,
    "Mod",
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1,
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_Str,
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_Mod_stVal,
    0
};

DataAttribute iedModel_GenericIO_TIM_GAPC1_Mod_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_Mod,
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_Mod_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_TIM_GAPC1_Mod_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_Mod,
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_Mod_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_TIM_GAPC1_Mod_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_Mod,
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_Mod_ctlModel,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_TIM_GAPC1_Mod_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_Mod,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_TIM_GAPC1_Str = {
    DataObjectModelType,
    "Str",
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1,
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_Op,
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_Str_general,
    0
};

DataAttribute iedModel_GenericIO_TIM_GAPC1_Str_general = {
    DataAttributeModelType,
    "general",
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_Str,
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_Str_dirGeneral,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_TIM_GAPC1_Str_dirGeneral = {
    DataAttributeModelType,
    "dirGeneral",
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_Str,
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_Str_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_TIM_GAPC1_Str_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_Str,
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_Str_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_TIM_GAPC1_Str_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_Str,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_TIM_GAPC1_Op = {
    DataObjectModelType,
    "Op",
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1,
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_OpDlTmms,
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_Op_general,
    0
};

DataAttribute iedModel_GenericIO_TIM_GAPC1_Op_general = {
    DataAttributeModelType,
    "general",
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_Op,
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_Op_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_TIM_GAPC1_Op_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_Op,
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_Op_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_TIM_GAPC1_Op_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_Op,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_TIM_GAPC1_OpDlTmms = {
    DataObjectModelType,
    "OpDlTmms",
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1,
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_RsDlTmms,
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_OpDlTmms_setVal,
    0
};

DataAttribute iedModel_GenericIO_TIM_GAPC1_OpDlTmms_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_OpDlTmms,
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_OpDlTmms_dataNs,
    NULL,
    0,
    IEC61850_FC_SP,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_TIM_GAPC1_OpDlTmms_dataNs = {
    DataAttributeModelType,
    "dataNs",
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_OpDlTmms,
    NULL,
    NULL,
    0,
    IEC61850_FC_EX,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_TIM_GAPC1_RsDlTmms = {
    DataObjectModelType,
    "RsDlTmms",
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1,
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_OpCntRs,
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_RsDlTmms_setVal,
    0
};

DataAttribute iedModel_GenericIO_TIM_GAPC1_RsDlTmms_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_RsDlTmms,
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_RsDlTmms_dataNs,
    NULL,
    0,
    IEC61850_FC_SP,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_TIM_GAPC1_RsDlTmms_dataNs = {
    DataAttributeModelType,
    "dataNs",
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_RsDlTmms,
    NULL,
    NULL,
    0,
    IEC61850_FC_EX,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_TIM_GAPC1_OpCntRs = {
    DataObjectModelType,
    "OpCntRs",
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1,
    NULL,
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_OpCntRs_stVal,
    0
};

DataAttribute iedModel_GenericIO_TIM_GAPC1_OpCntRs_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_OpCntRs,
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_OpCntRs_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_TIM_GAPC1_OpCntRs_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_OpCntRs,
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_OpCntRs_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_TIM_GAPC1_OpCntRs_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_OpCntRs,
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_OpCntRs_Oper,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_TIM_GAPC1_OpCntRs_Oper = {
    DataAttributeModelType,
    "Oper",
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_OpCntRs,
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_OpCntRs_ctlModel,
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_OpCntRs_Oper_ctlVal,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_TIM_GAPC1_OpCntRs_Oper_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_OpCntRs_Oper,
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_OpCntRs_Oper_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_INT32,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_TIM_GAPC1_OpCntRs_Oper_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_OpCntRs_Oper,
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_OpCntRs_Oper_ctlNum,
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_OpCntRs_Oper_origin_orCat,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_TIM_GAPC1_OpCntRs_Oper_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_OpCntRs_Oper_origin,
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_OpCntRs_Oper_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_TIM_GAPC1_OpCntRs_Oper_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_OpCntRs_Oper_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_TIM_GAPC1_OpCntRs_Oper_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_OpCntRs_Oper,
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_OpCntRs_Oper_T,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_TIM_GAPC1_OpCntRs_Oper_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_OpCntRs_Oper,
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_OpCntRs_Oper_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_TIM_GAPC1_OpCntRs_Oper_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_OpCntRs_Oper,
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_OpCntRs_Oper_Check,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_TIM_GAPC1_OpCntRs_Oper_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_OpCntRs_Oper,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_TIM_GAPC1_OpCntRs_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_GenericIO_TIM_GAPC1_OpCntRs,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

LogicalNode iedModel_GenericIO_T60S_FSCH1 = {
    LogicalNodeModelType,
    "T60S_FSCH1",
    (ModelNode*) &iedModel_GenericIO,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_Beh,
};

DataObject iedModel_GenericIO_T60S_FSCH1_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_SchdSt,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_Beh_stVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_Beh,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_Beh_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_Beh,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_Beh_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_Beh,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_SchdSt = {
    DataObjectModelType,
    "SchdSt",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_SchdEntr,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_SchdSt_stVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_SchdSt_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_SchdSt,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_SchdSt_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_SchdSt_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_SchdSt,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_SchdSt_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_SchdSt_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_SchdSt,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_SchdEntr = {
    DataObjectModelType,
    "SchdEntr",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPS,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_SchdEntr_stVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_SchdEntr_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_SchdEntr,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_SchdEntr_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_SchdEntr_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_SchdEntr,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_SchdEntr_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_SchdEntr_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_SchdEntr,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPS = {
    DataObjectModelType,
    "ValSPS",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_VldReq,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPS_stVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPS_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPS,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPS_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPS_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPS,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPS_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPS_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPS,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_VldReq = {
    DataObjectModelType,
    "VldReq",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_EnaReq,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_VldReq_stVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_VldReq_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_VldReq,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_VldReq_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_VldReq_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_VldReq,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_VldReq_Oper,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_VldReq_Oper = {
    DataAttributeModelType,
    "Oper",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_VldReq,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_VldReq_ctlModel,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_VldReq_Oper_ctlVal,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_VldReq_Oper_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_VldReq_Oper,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_VldReq_Oper_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_VldReq_Oper_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_VldReq_Oper,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_VldReq_Oper_ctlNum,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_VldReq_Oper_origin_orCat,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_VldReq_Oper_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_VldReq_Oper_origin,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_VldReq_Oper_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_VldReq_Oper_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_VldReq_Oper_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_VldReq_Oper_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_VldReq_Oper,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_VldReq_Oper_T,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_VldReq_Oper_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_VldReq_Oper,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_VldReq_Oper_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_VldReq_Oper_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_VldReq_Oper,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_VldReq_Oper_Check,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_VldReq_Oper_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_VldReq_Oper,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_VldReq_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_VldReq,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_VldReq_t,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_VldReq_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_VldReq,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_EnaReq = {
    DataObjectModelType,
    "EnaReq",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_EdtReq,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_EnaReq_stVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_EnaReq_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_EnaReq,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_EnaReq_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_EnaReq_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_EnaReq,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_EnaReq_Oper,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_EnaReq_Oper = {
    DataAttributeModelType,
    "Oper",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_EnaReq,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_EnaReq_ctlModel,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_EnaReq_Oper_ctlVal,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_EnaReq_Oper_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_EnaReq_Oper,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_EnaReq_Oper_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_EnaReq_Oper_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_EnaReq_Oper,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_EnaReq_Oper_ctlNum,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_EnaReq_Oper_origin_orCat,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_EnaReq_Oper_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_EnaReq_Oper_origin,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_EnaReq_Oper_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_EnaReq_Oper_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_EnaReq_Oper_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_EnaReq_Oper_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_EnaReq_Oper,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_EnaReq_Oper_T,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_EnaReq_Oper_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_EnaReq_Oper,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_EnaReq_Oper_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_EnaReq_Oper_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_EnaReq_Oper,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_EnaReq_Oper_Check,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_EnaReq_Oper_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_EnaReq_Oper,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_EnaReq_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_EnaReq,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_EnaReq_t,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_EnaReq_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_EnaReq,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_EdtReq = {
    DataObjectModelType,
    "EdtReq",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_DsaReq,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_EdtReq_stVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_EdtReq_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_EdtReq,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_EdtReq_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_EdtReq_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_EdtReq,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_EdtReq_Oper,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_EdtReq_Oper = {
    DataAttributeModelType,
    "Oper",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_EdtReq,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_EdtReq_ctlModel,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_EdtReq_Oper_ctlVal,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_EdtReq_Oper_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_EdtReq_Oper,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_EdtReq_Oper_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_EdtReq_Oper_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_EdtReq_Oper,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_EdtReq_Oper_ctlNum,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_EdtReq_Oper_origin_orCat,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_EdtReq_Oper_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_EdtReq_Oper_origin,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_EdtReq_Oper_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_EdtReq_Oper_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_EdtReq_Oper_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_EdtReq_Oper_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_EdtReq_Oper,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_EdtReq_Oper_T,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_EdtReq_Oper_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_EdtReq_Oper,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_EdtReq_Oper_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_EdtReq_Oper_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_EdtReq_Oper,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_EdtReq_Oper_Check,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_EdtReq_Oper_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_EdtReq_Oper,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_EdtReq_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_EdtReq,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_EdtReq_t,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_EdtReq_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_EdtReq,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_DsaReq = {
    DataObjectModelType,
    "DsaReq",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_SchdPrio,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_DsaReq_stVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_DsaReq_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_DsaReq,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_DsaReq_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_DsaReq_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_DsaReq,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_DsaReq_Oper,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_DsaReq_Oper = {
    DataAttributeModelType,
    "Oper",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_DsaReq,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_DsaReq_ctlModel,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_DsaReq_Oper_ctlVal,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_DsaReq_Oper_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_DsaReq_Oper,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_DsaReq_Oper_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_DsaReq_Oper_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_DsaReq_Oper,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_DsaReq_Oper_ctlNum,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_DsaReq_Oper_origin_orCat,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_DsaReq_Oper_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_DsaReq_Oper_origin,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_DsaReq_Oper_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_DsaReq_Oper_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_DsaReq_Oper_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_DsaReq_Oper_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_DsaReq_Oper,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_DsaReq_Oper_T,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_DsaReq_Oper_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_DsaReq_Oper,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_DsaReq_Oper_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_DsaReq_Oper_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_DsaReq_Oper,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_DsaReq_Oper_Check,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_DsaReq_Oper_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_DsaReq_Oper,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_DsaReq_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_DsaReq,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_DsaReq_t,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_DsaReq_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_DsaReq,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_SchdPrio = {
    DataObjectModelType,
    "SchdPrio",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_NumEntr,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_SchdPrio_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_SchdPrio_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_SchdPrio,
    NULL,
    NULL,
    0,
    IEC61850_FC_SP,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_NumEntr = {
    DataObjectModelType,
    "NumEntr",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_SchdIntv,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_NumEntr_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_NumEntr_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_NumEntr,
    NULL,
    NULL,
    0,
    IEC61850_FC_SP,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_SchdIntv = {
    DataObjectModelType,
    "SchdIntv",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_SchdIntv_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_SchdIntv_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_SchdIntv,
    NULL,
    NULL,
    0,
    IEC61850_FC_SP,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG1 = {
    DataObjectModelType,
    "ValSPG1",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG1_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG1_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG1,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG2 = {
    DataObjectModelType,
    "ValSPG2",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG3,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG2_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG2_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG2,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG3 = {
    DataObjectModelType,
    "ValSPG3",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG4,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG3_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG3_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG3,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG4 = {
    DataObjectModelType,
    "ValSPG4",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG5,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG4_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG4_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG4,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG5 = {
    DataObjectModelType,
    "ValSPG5",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG6,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG5_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG5_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG5,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG6 = {
    DataObjectModelType,
    "ValSPG6",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG7,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG6_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG6_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG6,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG7 = {
    DataObjectModelType,
    "ValSPG7",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG8,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG7_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG7_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG7,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG8 = {
    DataObjectModelType,
    "ValSPG8",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG9,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG8_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG8_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG8,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG9 = {
    DataObjectModelType,
    "ValSPG9",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG10,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG9_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG9_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG9,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG10 = {
    DataObjectModelType,
    "ValSPG10",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG11,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG10_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG10_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG10,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG11 = {
    DataObjectModelType,
    "ValSPG11",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG12,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG11_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG11_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG11,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG12 = {
    DataObjectModelType,
    "ValSPG12",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG13,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG12_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG12_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG12,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG13 = {
    DataObjectModelType,
    "ValSPG13",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG14,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG13_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG13_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG13,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG14 = {
    DataObjectModelType,
    "ValSPG14",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG15,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG14_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG14_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG14,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG15 = {
    DataObjectModelType,
    "ValSPG15",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG16,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG15_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG15_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG15,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG16 = {
    DataObjectModelType,
    "ValSPG16",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG17,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG16_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG16_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG16,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG17 = {
    DataObjectModelType,
    "ValSPG17",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG18,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG17_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG17_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG17,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG18 = {
    DataObjectModelType,
    "ValSPG18",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG19,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG18_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG18_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG18,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG19 = {
    DataObjectModelType,
    "ValSPG19",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG20,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG19_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG19_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG19,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG20 = {
    DataObjectModelType,
    "ValSPG20",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG21,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG20_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG20_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG20,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG21 = {
    DataObjectModelType,
    "ValSPG21",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG22,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG21_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG21_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG21,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG22 = {
    DataObjectModelType,
    "ValSPG22",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG23,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG22_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG22_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG22,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG23 = {
    DataObjectModelType,
    "ValSPG23",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG24,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG23_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG23_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG23,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG24 = {
    DataObjectModelType,
    "ValSPG24",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG25,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG24_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG24_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG24,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG25 = {
    DataObjectModelType,
    "ValSPG25",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG26,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG25_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG25_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG25,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG26 = {
    DataObjectModelType,
    "ValSPG26",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG27,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG26_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG26_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG26,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG27 = {
    DataObjectModelType,
    "ValSPG27",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG28,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG27_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG27_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG27,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG28 = {
    DataObjectModelType,
    "ValSPG28",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG29,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG28_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG28_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG28,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG29 = {
    DataObjectModelType,
    "ValSPG29",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG30,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG29_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG29_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG29,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG30 = {
    DataObjectModelType,
    "ValSPG30",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG31,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG30_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG30_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG30,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG31 = {
    DataObjectModelType,
    "ValSPG31",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG32,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG31_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG31_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG31,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG32 = {
    DataObjectModelType,
    "ValSPG32",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG33,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG32_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG32_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG32,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG33 = {
    DataObjectModelType,
    "ValSPG33",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG34,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG33_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG33_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG33,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG34 = {
    DataObjectModelType,
    "ValSPG34",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG35,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG34_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG34_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG34,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG35 = {
    DataObjectModelType,
    "ValSPG35",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG36,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG35_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG35_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG35,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG36 = {
    DataObjectModelType,
    "ValSPG36",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG37,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG36_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG36_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG36,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG37 = {
    DataObjectModelType,
    "ValSPG37",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG38,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG37_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG37_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG37,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG38 = {
    DataObjectModelType,
    "ValSPG38",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG39,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG38_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG38_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG38,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG39 = {
    DataObjectModelType,
    "ValSPG39",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG40,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG39_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG39_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG39,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG40 = {
    DataObjectModelType,
    "ValSPG40",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG41,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG40_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG40_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG40,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG41 = {
    DataObjectModelType,
    "ValSPG41",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG42,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG41_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG41_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG41,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG42 = {
    DataObjectModelType,
    "ValSPG42",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG43,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG42_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG42_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG42,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG43 = {
    DataObjectModelType,
    "ValSPG43",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG44,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG43_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG43_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG43,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG44 = {
    DataObjectModelType,
    "ValSPG44",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG45,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG44_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG44_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG44,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG45 = {
    DataObjectModelType,
    "ValSPG45",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG46,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG45_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG45_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG45,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG46 = {
    DataObjectModelType,
    "ValSPG46",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG47,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG46_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG46_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG46,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG47 = {
    DataObjectModelType,
    "ValSPG47",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG48,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG47_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG47_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG47,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG48 = {
    DataObjectModelType,
    "ValSPG48",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG49,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG48_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG48_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG48,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG49 = {
    DataObjectModelType,
    "ValSPG49",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG50,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG49_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG49_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG49,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG50 = {
    DataObjectModelType,
    "ValSPG50",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG51,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG50_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG50_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG50,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG51 = {
    DataObjectModelType,
    "ValSPG51",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG52,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG51_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG51_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG51,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG52 = {
    DataObjectModelType,
    "ValSPG52",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG53,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG52_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG52_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG52,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG53 = {
    DataObjectModelType,
    "ValSPG53",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG54,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG53_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG53_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG53,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG54 = {
    DataObjectModelType,
    "ValSPG54",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG55,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG54_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG54_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG54,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG55 = {
    DataObjectModelType,
    "ValSPG55",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG56,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG55_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG55_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG55,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG56 = {
    DataObjectModelType,
    "ValSPG56",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG57,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG56_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG56_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG56,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG57 = {
    DataObjectModelType,
    "ValSPG57",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG58,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG57_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG57_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG57,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG58 = {
    DataObjectModelType,
    "ValSPG58",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG59,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG58_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG58_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG58,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG59 = {
    DataObjectModelType,
    "ValSPG59",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG60,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG59_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG59_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG59,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_ValSPG60 = {
    DataObjectModelType,
    "ValSPG60",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_StrTm,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG60_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_ValSPG60_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_ValSPG60,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_StrTm = {
    DataObjectModelType,
    "StrTm",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_IntvPer,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_StrTm_setTm,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_StrTm_setTm = {
    DataAttributeModelType,
    "setTm",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_StrTm,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_StrTm_setCal,
    NULL,
    0,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_StrTm_setCal = {
    DataAttributeModelType,
    "setCal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_StrTm,
    NULL,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_StrTm_setCal_occ,
    0,
    IEC61850_FC_SP,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_StrTm_setCal_occ = {
    DataAttributeModelType,
    "occ",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_StrTm_setCal,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_StrTm_setCal_occType,
    NULL,
    0,
    IEC61850_FC_SP,
    IEC61850_INT16U,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_StrTm_setCal_occType = {
    DataAttributeModelType,
    "occType",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_StrTm_setCal,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_StrTm_setCal_occPer,
    NULL,
    0,
    IEC61850_FC_SP,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_StrTm_setCal_occPer = {
    DataAttributeModelType,
    "occPer",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_StrTm_setCal,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_StrTm_setCal_weekDay,
    NULL,
    0,
    IEC61850_FC_SP,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_StrTm_setCal_weekDay = {
    DataAttributeModelType,
    "weekDay",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_StrTm_setCal,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_StrTm_setCal_month,
    NULL,
    0,
    IEC61850_FC_SP,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_StrTm_setCal_month = {
    DataAttributeModelType,
    "month",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_StrTm_setCal,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_StrTm_setCal_day,
    NULL,
    0,
    IEC61850_FC_SP,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_StrTm_setCal_day = {
    DataAttributeModelType,
    "day",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_StrTm_setCal,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_StrTm_setCal_hr,
    NULL,
    0,
    IEC61850_FC_SP,
    IEC61850_INT8U,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_StrTm_setCal_hr = {
    DataAttributeModelType,
    "hr",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_StrTm_setCal,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_StrTm_setCal_mn,
    NULL,
    0,
    IEC61850_FC_SP,
    IEC61850_INT8U,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH1_StrTm_setCal_mn = {
    DataAttributeModelType,
    "mn",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_StrTm_setCal,
    NULL,
    NULL,
    0,
    IEC61850_FC_SP,
    IEC61850_INT8U,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_IntvPer = {
    DataObjectModelType,
    "IntvPer",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_IntvTyp,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_IntvPer_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_IntvPer_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_IntvPer,
    NULL,
    NULL,
    0,
    IEC61850_FC_SP,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_IntvTyp = {
    DataObjectModelType,
    "IntvTyp",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_SchdReuse,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_IntvTyp_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_IntvTyp_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_IntvTyp,
    NULL,
    NULL,
    0,
    IEC61850_FC_SP,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH1_SchdReuse = {
    DataObjectModelType,
    "SchdReuse",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1,
    NULL,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_SchdReuse_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH1_SchdReuse_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH1_SchdReuse,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

LogicalNode iedModel_GenericIO_T60S_FSCH2 = {
    LogicalNodeModelType,
    "T60S_FSCH2",
    (ModelNode*) &iedModel_GenericIO,
    (ModelNode*) &iedModel_GenericIO_GGIO_FSCC1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_Beh,
};

DataObject iedModel_GenericIO_T60S_FSCH2_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_SchdSt,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_Beh_stVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_Beh,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_Beh_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_Beh,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_Beh_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_Beh,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_SchdSt = {
    DataObjectModelType,
    "SchdSt",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_SchdEntr,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_SchdSt_stVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_SchdSt_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_SchdSt,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_SchdSt_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_SchdSt_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_SchdSt,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_SchdSt_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_SchdSt_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_SchdSt,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_SchdEntr = {
    DataObjectModelType,
    "SchdEntr",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPS,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_SchdEntr_stVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_SchdEntr_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_SchdEntr,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_SchdEntr_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_SchdEntr_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_SchdEntr,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_SchdEntr_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_SchdEntr_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_SchdEntr,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPS = {
    DataObjectModelType,
    "ValSPS",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_VldReq,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPS_stVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPS_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPS,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPS_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPS_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPS,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPS_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPS_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPS,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_VldReq = {
    DataObjectModelType,
    "VldReq",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_EnaReq,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_VldReq_stVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_VldReq_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_VldReq,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_VldReq_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_VldReq_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_VldReq,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_VldReq_Oper,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_VldReq_Oper = {
    DataAttributeModelType,
    "Oper",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_VldReq,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_VldReq_ctlModel,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_VldReq_Oper_ctlVal,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_VldReq_Oper_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_VldReq_Oper,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_VldReq_Oper_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_VldReq_Oper_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_VldReq_Oper,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_VldReq_Oper_ctlNum,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_VldReq_Oper_origin_orCat,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_VldReq_Oper_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_VldReq_Oper_origin,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_VldReq_Oper_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_VldReq_Oper_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_VldReq_Oper_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_VldReq_Oper_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_VldReq_Oper,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_VldReq_Oper_T,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_VldReq_Oper_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_VldReq_Oper,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_VldReq_Oper_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_VldReq_Oper_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_VldReq_Oper,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_VldReq_Oper_Check,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_VldReq_Oper_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_VldReq_Oper,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_VldReq_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_VldReq,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_VldReq_t,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_VldReq_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_VldReq,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_EnaReq = {
    DataObjectModelType,
    "EnaReq",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_EdtReq,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_EnaReq_stVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_EnaReq_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_EnaReq,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_EnaReq_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_EnaReq_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_EnaReq,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_EnaReq_Oper,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_EnaReq_Oper = {
    DataAttributeModelType,
    "Oper",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_EnaReq,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_EnaReq_ctlModel,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_EnaReq_Oper_ctlVal,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_EnaReq_Oper_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_EnaReq_Oper,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_EnaReq_Oper_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_EnaReq_Oper_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_EnaReq_Oper,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_EnaReq_Oper_ctlNum,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_EnaReq_Oper_origin_orCat,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_EnaReq_Oper_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_EnaReq_Oper_origin,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_EnaReq_Oper_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_EnaReq_Oper_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_EnaReq_Oper_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_EnaReq_Oper_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_EnaReq_Oper,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_EnaReq_Oper_T,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_EnaReq_Oper_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_EnaReq_Oper,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_EnaReq_Oper_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_EnaReq_Oper_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_EnaReq_Oper,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_EnaReq_Oper_Check,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_EnaReq_Oper_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_EnaReq_Oper,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_EnaReq_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_EnaReq,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_EnaReq_t,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_EnaReq_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_EnaReq,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_EdtReq = {
    DataObjectModelType,
    "EdtReq",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_DsaReq,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_EdtReq_stVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_EdtReq_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_EdtReq,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_EdtReq_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_EdtReq_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_EdtReq,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_EdtReq_Oper,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_EdtReq_Oper = {
    DataAttributeModelType,
    "Oper",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_EdtReq,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_EdtReq_ctlModel,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_EdtReq_Oper_ctlVal,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_EdtReq_Oper_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_EdtReq_Oper,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_EdtReq_Oper_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_EdtReq_Oper_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_EdtReq_Oper,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_EdtReq_Oper_ctlNum,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_EdtReq_Oper_origin_orCat,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_EdtReq_Oper_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_EdtReq_Oper_origin,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_EdtReq_Oper_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_EdtReq_Oper_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_EdtReq_Oper_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_EdtReq_Oper_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_EdtReq_Oper,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_EdtReq_Oper_T,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_EdtReq_Oper_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_EdtReq_Oper,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_EdtReq_Oper_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_EdtReq_Oper_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_EdtReq_Oper,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_EdtReq_Oper_Check,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_EdtReq_Oper_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_EdtReq_Oper,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_EdtReq_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_EdtReq,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_EdtReq_t,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_EdtReq_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_EdtReq,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_DsaReq = {
    DataObjectModelType,
    "DsaReq",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_SchdPrio,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_DsaReq_stVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_DsaReq_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_DsaReq,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_DsaReq_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_DsaReq_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_DsaReq,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_DsaReq_Oper,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_DsaReq_Oper = {
    DataAttributeModelType,
    "Oper",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_DsaReq,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_DsaReq_ctlModel,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_DsaReq_Oper_ctlVal,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_DsaReq_Oper_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_DsaReq_Oper,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_DsaReq_Oper_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_DsaReq_Oper_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_DsaReq_Oper,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_DsaReq_Oper_ctlNum,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_DsaReq_Oper_origin_orCat,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_DsaReq_Oper_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_DsaReq_Oper_origin,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_DsaReq_Oper_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_DsaReq_Oper_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_DsaReq_Oper_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_DsaReq_Oper_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_DsaReq_Oper,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_DsaReq_Oper_T,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_DsaReq_Oper_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_DsaReq_Oper,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_DsaReq_Oper_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_DsaReq_Oper_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_DsaReq_Oper,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_DsaReq_Oper_Check,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_DsaReq_Oper_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_DsaReq_Oper,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_DsaReq_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_DsaReq,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_DsaReq_t,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_DsaReq_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_DsaReq,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_SchdPrio = {
    DataObjectModelType,
    "SchdPrio",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_NumEntr,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_SchdPrio_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_SchdPrio_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_SchdPrio,
    NULL,
    NULL,
    0,
    IEC61850_FC_SP,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_NumEntr = {
    DataObjectModelType,
    "NumEntr",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_SchdIntv,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_NumEntr_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_NumEntr_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_NumEntr,
    NULL,
    NULL,
    0,
    IEC61850_FC_SP,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_SchdIntv = {
    DataObjectModelType,
    "SchdIntv",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG1,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_SchdIntv_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_SchdIntv_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_SchdIntv,
    NULL,
    NULL,
    0,
    IEC61850_FC_SP,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG1 = {
    DataObjectModelType,
    "ValSPG1",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG1_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG1_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG1,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG2 = {
    DataObjectModelType,
    "ValSPG2",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG3,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG2_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG2_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG2,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG3 = {
    DataObjectModelType,
    "ValSPG3",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG4,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG3_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG3_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG3,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG4 = {
    DataObjectModelType,
    "ValSPG4",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG5,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG4_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG4_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG4,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG5 = {
    DataObjectModelType,
    "ValSPG5",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG6,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG5_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG5_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG5,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG6 = {
    DataObjectModelType,
    "ValSPG6",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG7,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG6_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG6_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG6,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG7 = {
    DataObjectModelType,
    "ValSPG7",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG8,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG7_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG7_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG7,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG8 = {
    DataObjectModelType,
    "ValSPG8",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG9,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG8_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG8_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG8,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG9 = {
    DataObjectModelType,
    "ValSPG9",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG10,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG9_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG9_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG9,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG10 = {
    DataObjectModelType,
    "ValSPG10",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG11,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG10_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG10_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG10,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG11 = {
    DataObjectModelType,
    "ValSPG11",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG12,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG11_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG11_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG11,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG12 = {
    DataObjectModelType,
    "ValSPG12",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG13,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG12_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG12_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG12,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG13 = {
    DataObjectModelType,
    "ValSPG13",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG14,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG13_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG13_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG13,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG14 = {
    DataObjectModelType,
    "ValSPG14",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG15,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG14_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG14_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG14,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG15 = {
    DataObjectModelType,
    "ValSPG15",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG16,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG15_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG15_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG15,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG16 = {
    DataObjectModelType,
    "ValSPG16",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG17,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG16_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG16_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG16,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG17 = {
    DataObjectModelType,
    "ValSPG17",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG18,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG17_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG17_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG17,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG18 = {
    DataObjectModelType,
    "ValSPG18",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG19,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG18_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG18_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG18,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG19 = {
    DataObjectModelType,
    "ValSPG19",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG20,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG19_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG19_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG19,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG20 = {
    DataObjectModelType,
    "ValSPG20",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG21,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG20_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG20_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG20,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG21 = {
    DataObjectModelType,
    "ValSPG21",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG22,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG21_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG21_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG21,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG22 = {
    DataObjectModelType,
    "ValSPG22",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG23,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG22_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG22_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG22,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG23 = {
    DataObjectModelType,
    "ValSPG23",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG24,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG23_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG23_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG23,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG24 = {
    DataObjectModelType,
    "ValSPG24",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG25,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG24_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG24_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG24,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG25 = {
    DataObjectModelType,
    "ValSPG25",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG26,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG25_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG25_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG25,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG26 = {
    DataObjectModelType,
    "ValSPG26",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG27,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG26_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG26_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG26,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG27 = {
    DataObjectModelType,
    "ValSPG27",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG28,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG27_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG27_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG27,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG28 = {
    DataObjectModelType,
    "ValSPG28",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG29,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG28_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG28_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG28,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG29 = {
    DataObjectModelType,
    "ValSPG29",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG30,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG29_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG29_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG29,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG30 = {
    DataObjectModelType,
    "ValSPG30",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG31,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG30_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG30_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG30,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG31 = {
    DataObjectModelType,
    "ValSPG31",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG32,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG31_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG31_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG31,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG32 = {
    DataObjectModelType,
    "ValSPG32",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG33,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG32_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG32_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG32,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG33 = {
    DataObjectModelType,
    "ValSPG33",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG34,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG33_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG33_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG33,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG34 = {
    DataObjectModelType,
    "ValSPG34",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG35,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG34_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG34_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG34,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG35 = {
    DataObjectModelType,
    "ValSPG35",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG36,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG35_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG35_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG35,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG36 = {
    DataObjectModelType,
    "ValSPG36",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG37,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG36_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG36_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG36,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG37 = {
    DataObjectModelType,
    "ValSPG37",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG38,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG37_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG37_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG37,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG38 = {
    DataObjectModelType,
    "ValSPG38",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG39,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG38_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG38_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG38,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG39 = {
    DataObjectModelType,
    "ValSPG39",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG40,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG39_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG39_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG39,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG40 = {
    DataObjectModelType,
    "ValSPG40",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG41,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG40_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG40_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG40,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG41 = {
    DataObjectModelType,
    "ValSPG41",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG42,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG41_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG41_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG41,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG42 = {
    DataObjectModelType,
    "ValSPG42",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG43,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG42_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG42_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG42,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG43 = {
    DataObjectModelType,
    "ValSPG43",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG44,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG43_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG43_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG43,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG44 = {
    DataObjectModelType,
    "ValSPG44",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG45,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG44_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG44_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG44,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG45 = {
    DataObjectModelType,
    "ValSPG45",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG46,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG45_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG45_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG45,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG46 = {
    DataObjectModelType,
    "ValSPG46",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG47,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG46_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG46_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG46,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG47 = {
    DataObjectModelType,
    "ValSPG47",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG48,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG47_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG47_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG47,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG48 = {
    DataObjectModelType,
    "ValSPG48",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG49,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG48_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG48_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG48,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG49 = {
    DataObjectModelType,
    "ValSPG49",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG50,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG49_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG49_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG49,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG50 = {
    DataObjectModelType,
    "ValSPG50",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG51,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG50_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG50_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG50,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG51 = {
    DataObjectModelType,
    "ValSPG51",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG52,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG51_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG51_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG51,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG52 = {
    DataObjectModelType,
    "ValSPG52",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG53,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG52_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG52_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG52,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG53 = {
    DataObjectModelType,
    "ValSPG53",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG54,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG53_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG53_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG53,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG54 = {
    DataObjectModelType,
    "ValSPG54",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG55,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG54_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG54_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG54,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG55 = {
    DataObjectModelType,
    "ValSPG55",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG56,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG55_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG55_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG55,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG56 = {
    DataObjectModelType,
    "ValSPG56",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG57,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG56_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG56_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG56,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG57 = {
    DataObjectModelType,
    "ValSPG57",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG58,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG57_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG57_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG57,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG58 = {
    DataObjectModelType,
    "ValSPG58",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG59,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG58_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG58_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG58,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG59 = {
    DataObjectModelType,
    "ValSPG59",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG60,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG59_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG59_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG59,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_ValSPG60 = {
    DataObjectModelType,
    "ValSPG60",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_StrTm,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG60_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_ValSPG60_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_ValSPG60,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_StrTm = {
    DataObjectModelType,
    "StrTm",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_IntvPer,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_StrTm_setTm,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_StrTm_setTm = {
    DataAttributeModelType,
    "setTm",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_StrTm,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_StrTm_setCal,
    NULL,
    0,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_StrTm_setCal = {
    DataAttributeModelType,
    "setCal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_StrTm,
    NULL,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_StrTm_setCal_occ,
    0,
    IEC61850_FC_SP,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_StrTm_setCal_occ = {
    DataAttributeModelType,
    "occ",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_StrTm_setCal,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_StrTm_setCal_occType,
    NULL,
    0,
    IEC61850_FC_SP,
    IEC61850_INT16U,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_StrTm_setCal_occType = {
    DataAttributeModelType,
    "occType",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_StrTm_setCal,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_StrTm_setCal_occPer,
    NULL,
    0,
    IEC61850_FC_SP,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_StrTm_setCal_occPer = {
    DataAttributeModelType,
    "occPer",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_StrTm_setCal,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_StrTm_setCal_weekDay,
    NULL,
    0,
    IEC61850_FC_SP,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_StrTm_setCal_weekDay = {
    DataAttributeModelType,
    "weekDay",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_StrTm_setCal,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_StrTm_setCal_month,
    NULL,
    0,
    IEC61850_FC_SP,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_StrTm_setCal_month = {
    DataAttributeModelType,
    "month",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_StrTm_setCal,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_StrTm_setCal_day,
    NULL,
    0,
    IEC61850_FC_SP,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_StrTm_setCal_day = {
    DataAttributeModelType,
    "day",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_StrTm_setCal,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_StrTm_setCal_hr,
    NULL,
    0,
    IEC61850_FC_SP,
    IEC61850_INT8U,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_StrTm_setCal_hr = {
    DataAttributeModelType,
    "hr",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_StrTm_setCal,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_StrTm_setCal_mn,
    NULL,
    0,
    IEC61850_FC_SP,
    IEC61850_INT8U,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_T60S_FSCH2_StrTm_setCal_mn = {
    DataAttributeModelType,
    "mn",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_StrTm_setCal,
    NULL,
    NULL,
    0,
    IEC61850_FC_SP,
    IEC61850_INT8U,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_IntvPer = {
    DataObjectModelType,
    "IntvPer",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_IntvTyp,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_IntvPer_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_IntvPer_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_IntvPer,
    NULL,
    NULL,
    0,
    IEC61850_FC_SP,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_IntvTyp = {
    DataObjectModelType,
    "IntvTyp",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_SchdReuse,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_IntvTyp_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_IntvTyp_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_IntvTyp,
    NULL,
    NULL,
    0,
    IEC61850_FC_SP,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_T60S_FSCH2_SchdReuse = {
    DataObjectModelType,
    "SchdReuse",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2,
    NULL,
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_SchdReuse_setVal,
    0
};

DataAttribute iedModel_GenericIO_T60S_FSCH2_SchdReuse_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_GenericIO_T60S_FSCH2_SchdReuse,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

LogicalNode iedModel_GenericIO_GGIO_FSCC1 = {
    LogicalNodeModelType,
    "GGIO_FSCC1",
    (ModelNode*) &iedModel_GenericIO,
    NULL,
    (ModelNode*) &iedModel_GenericIO_GGIO_FSCC1_Beh,
};

DataObject iedModel_GenericIO_GGIO_FSCC1_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_GenericIO_GGIO_FSCC1,
    (ModelNode*) &iedModel_GenericIO_GGIO_FSCC1_ActSchdRef,
    (ModelNode*) &iedModel_GenericIO_GGIO_FSCC1_Beh_stVal,
    0
};

DataAttribute iedModel_GenericIO_GGIO_FSCC1_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_GGIO_FSCC1_Beh,
    (ModelNode*) &iedModel_GenericIO_GGIO_FSCC1_Beh_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO_FSCC1_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_GGIO_FSCC1_Beh,
    (ModelNode*) &iedModel_GenericIO_GGIO_FSCC1_Beh_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO_FSCC1_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_GGIO_FSCC1_Beh,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_GGIO_FSCC1_ActSchdRef = {
    DataObjectModelType,
    "ActSchdRef",
    (ModelNode*) &iedModel_GenericIO_GGIO_FSCC1,
    (ModelNode*) &iedModel_GenericIO_GGIO_FSCC1_CtlEnt,
    (ModelNode*) &iedModel_GenericIO_GGIO_FSCC1_ActSchdRef_stVal,
    0
};

DataAttribute iedModel_GenericIO_GGIO_FSCC1_ActSchdRef_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_GGIO_FSCC1_ActSchdRef,
    (ModelNode*) &iedModel_GenericIO_GGIO_FSCC1_ActSchdRef_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_VISIBLE_STRING_129,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO_FSCC1_ActSchdRef_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_GGIO_FSCC1_ActSchdRef,
    (ModelNode*) &iedModel_GenericIO_GGIO_FSCC1_ActSchdRef_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO_FSCC1_ActSchdRef_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_GGIO_FSCC1_ActSchdRef,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_GGIO_FSCC1_CtlEnt = {
    DataObjectModelType,
    "CtlEnt",
    (ModelNode*) &iedModel_GenericIO_GGIO_FSCC1,
    (ModelNode*) &iedModel_GenericIO_GGIO_FSCC1_Schd1,
    (ModelNode*) &iedModel_GenericIO_GGIO_FSCC1_CtlEnt_setSrcRef,
    0
};

DataAttribute iedModel_GenericIO_GGIO_FSCC1_CtlEnt_setSrcRef = {
    DataAttributeModelType,
    "setSrcRef",
    (ModelNode*) &iedModel_GenericIO_GGIO_FSCC1_CtlEnt,
    NULL,
    NULL,
    0,
    IEC61850_FC_SP,
    IEC61850_VISIBLE_STRING_129,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_GGIO_FSCC1_Schd1 = {
    DataObjectModelType,
    "Schd1",
    (ModelNode*) &iedModel_GenericIO_GGIO_FSCC1,
    (ModelNode*) &iedModel_GenericIO_GGIO_FSCC1_Schd2,
    (ModelNode*) &iedModel_GenericIO_GGIO_FSCC1_Schd1_setSrcRef,
    0
};

DataAttribute iedModel_GenericIO_GGIO_FSCC1_Schd1_setSrcRef = {
    DataAttributeModelType,
    "setSrcRef",
    (ModelNode*) &iedModel_GenericIO_GGIO_FSCC1_Schd1,
    NULL,
    NULL,
    0,
    IEC61850_FC_SP,
    IEC61850_VISIBLE_STRING_129,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_GenericIO_GGIO_FSCC1_Schd2 = {
    DataObjectModelType,
    "Schd2",
    (ModelNode*) &iedModel_GenericIO_GGIO_FSCC1,
    NULL,
    (ModelNode*) &iedModel_GenericIO_GGIO_FSCC1_Schd2_setSrcRef,
    0
};

DataAttribute iedModel_GenericIO_GGIO_FSCC1_Schd2_setSrcRef = {
    DataAttributeModelType,
    "setSrcRef",
    (ModelNode*) &iedModel_GenericIO_GGIO_FSCC1_Schd2,
    NULL,
    NULL,
    0,
    IEC61850_FC_SP,
    IEC61850_VISIBLE_STRING_129,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

extern ReportControlBlock iedModel_GenericIO_LLN0_report0;
extern ReportControlBlock iedModel_GenericIO_LLN0_report1;
extern ReportControlBlock iedModel_GenericIO_LLN0_report2;
extern ReportControlBlock iedModel_GenericIO_LLN0_report3;
extern ReportControlBlock iedModel_GenericIO_LLN0_report4;

ReportControlBlock iedModel_GenericIO_LLN0_report0 = {&iedModel_GenericIO_LLN0, "EventsRCB01", "Events1", false, "Events", 1, 24, 111, 50, 1000, &iedModel_GenericIO_LLN0_report1};
ReportControlBlock iedModel_GenericIO_LLN0_report1 = {&iedModel_GenericIO_LLN0, "EventsRCB02", "Events1", false, "Events", 1, 24, 111, 50, 1000, &iedModel_GenericIO_LLN0_report2};
ReportControlBlock iedModel_GenericIO_LLN0_report2 = {&iedModel_GenericIO_LLN0, "EventsRCB03", "Events1", false, "Events", 1, 24, 111, 50, 1000, &iedModel_GenericIO_LLN0_report3};
ReportControlBlock iedModel_GenericIO_LLN0_report3 = {&iedModel_GenericIO_LLN0, "EventsRCB04", "Events1", false, "Events", 1, 24, 111, 50, 1000, &iedModel_GenericIO_LLN0_report4};
ReportControlBlock iedModel_GenericIO_LLN0_report4 = {&iedModel_GenericIO_LLN0, "EventsRCB05", "Events1", false, "Events", 1, 24, 111, 50, 1000, NULL};







IedModel iedModel = {
    "beagle",
    &iedModel_GenericIO,
    &iedModelds_GenericIO_LLN0_Events,
    &iedModel_GenericIO_LLN0_report0,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    initializeValues
};

static void
initializeValues()
{

iedModel_GenericIO_LLN0_Mod_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(0);

iedModel_GenericIO_GGIO1_Mod_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(0);

iedModel_GenericIO_GGIO1_SPCSO1_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(1);

iedModel_GenericIO_GGIO1_SPCSO2_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(1);

iedModel_GenericIO_GGIO1_SPCSO3_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(1);

iedModel_GenericIO_GGIO1_DPCSO1_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(1);

iedModel_GenericIO_TIM_GAPC1_Mod_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(0);

iedModel_GenericIO_TIM_GAPC1_OpDlTmms_dataNs.mmsValue = MmsValue_newVisibleString("EXT:2015");

iedModel_GenericIO_TIM_GAPC1_RsDlTmms_dataNs.mmsValue = MmsValue_newVisibleString("EXT:2015");

iedModel_GenericIO_TIM_GAPC1_OpCntRs_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(1);
}
