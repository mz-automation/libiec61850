/*
 * static_model.c
 *
 * automatically generated from sampleModel_with_dataset.cid
 */
#include "static_model.h"

static void initializeValues();

extern DataSet iedModelds_Device1_LLN0_dataset1;


extern DataSetEntry iedModelds_Device1_LLN0_dataset1_fcda0;
extern DataSetEntry iedModelds_Device1_LLN0_dataset1_fcda1;
extern DataSetEntry iedModelds_Device1_LLN0_dataset1_fcda2;

DataSetEntry iedModelds_Device1_LLN0_dataset1_fcda0 = {
  "Device1",
  false,
  "LLN0$ST$Mod$q", 
  -1,
  NULL,
  NULL,
  &iedModelds_Device1_LLN0_dataset1_fcda1
};

DataSetEntry iedModelds_Device1_LLN0_dataset1_fcda1 = {
  "Device1",
  false,
  "MMXU1$ST$Mod$q", 
  -1,
  NULL,
  NULL,
  &iedModelds_Device1_LLN0_dataset1_fcda2
};

DataSetEntry iedModelds_Device1_LLN0_dataset1_fcda2 = {
  "Device1",
  false,
  "MMXU1$CF$Mod$ctlModel", 
  -1,
  NULL,
  NULL,
  NULL
};

DataSet iedModelds_Device1_LLN0_dataset1 = {
  "Device1",
  "LLN0$dataset1",
  3,
  &iedModelds_Device1_LLN0_dataset1_fcda0,
  NULL
};

LogicalDevice iedModel_Device1 = {
    LogicalDeviceModelType,
    "Device1",
    (ModelNode*) &iedModel,
    NULL,
    (ModelNode*) &iedModel_Device1_LLN0,
    NULL
};

LogicalNode iedModel_Device1_LLN0 = {
    LogicalNodeModelType,
    "LLN0",
    (ModelNode*) &iedModel_Device1,
    (ModelNode*) &iedModel_Device1_LPHD1,
    (ModelNode*) &iedModel_Device1_LLN0_Mod,
};

DataObject iedModel_Device1_LLN0_Mod = {
    DataObjectModelType,
    "Mod",
    (ModelNode*) &iedModel_Device1_LLN0,
    (ModelNode*) &iedModel_Device1_LLN0_Beh,
    (ModelNode*) &iedModel_Device1_LLN0_Mod_q,
    0,
    -1
};

DataAttribute iedModel_Device1_LLN0_Mod_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Device1_LLN0_Mod,
    (ModelNode*) &iedModel_Device1_LLN0_Mod_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_LLN0_Mod_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Device1_LLN0_Mod,
    (ModelNode*) &iedModel_Device1_LLN0_Mod_ctlModel,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_Device1_LLN0_Mod_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_Device1_LLN0_Mod,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_Device1_LLN0_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_Device1_LLN0,
    (ModelNode*) &iedModel_Device1_LLN0_Health,
    (ModelNode*) &iedModel_Device1_LLN0_Beh_stVal,
    0,
    -1
};

DataAttribute iedModel_Device1_LLN0_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_Device1_LLN0_Beh,
    (ModelNode*) &iedModel_Device1_LLN0_Beh_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_LLN0_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Device1_LLN0_Beh,
    (ModelNode*) &iedModel_Device1_LLN0_Beh_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_LLN0_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Device1_LLN0_Beh,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_Device1_LLN0_Health = {
    DataObjectModelType,
    "Health",
    (ModelNode*) &iedModel_Device1_LLN0,
    (ModelNode*) &iedModel_Device1_LLN0_NamPlt,
    (ModelNode*) &iedModel_Device1_LLN0_Health_stVal,
    0,
    -1
};

DataAttribute iedModel_Device1_LLN0_Health_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_Device1_LLN0_Health,
    (ModelNode*) &iedModel_Device1_LLN0_Health_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_LLN0_Health_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Device1_LLN0_Health,
    (ModelNode*) &iedModel_Device1_LLN0_Health_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_LLN0_Health_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Device1_LLN0_Health,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_Device1_LLN0_NamPlt = {
    DataObjectModelType,
    "NamPlt",
    (ModelNode*) &iedModel_Device1_LLN0,
    NULL,
    (ModelNode*) &iedModel_Device1_LLN0_NamPlt_vendor,
    0,
    -1
};

DataAttribute iedModel_Device1_LLN0_NamPlt_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_Device1_LLN0_NamPlt,
    (ModelNode*) &iedModel_Device1_LLN0_NamPlt_swRev,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_Device1_LLN0_NamPlt_swRev = {
    DataAttributeModelType,
    "swRev",
    (ModelNode*) &iedModel_Device1_LLN0_NamPlt,
    (ModelNode*) &iedModel_Device1_LLN0_NamPlt_d,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_Device1_LLN0_NamPlt_d = {
    DataAttributeModelType,
    "d",
    (ModelNode*) &iedModel_Device1_LLN0_NamPlt,
    (ModelNode*) &iedModel_Device1_LLN0_NamPlt_configRev,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_Device1_LLN0_NamPlt_configRev = {
    DataAttributeModelType,
    "configRev",
    (ModelNode*) &iedModel_Device1_LLN0_NamPlt,
    (ModelNode*) &iedModel_Device1_LLN0_NamPlt_ldNs,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_Device1_LLN0_NamPlt_ldNs = {
    DataAttributeModelType,
    "ldNs",
    (ModelNode*) &iedModel_Device1_LLN0_NamPlt,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_EX,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

LogicalNode iedModel_Device1_LPHD1 = {
    LogicalNodeModelType,
    "LPHD1",
    (ModelNode*) &iedModel_Device1,
    (ModelNode*) &iedModel_Device1_DGEN1,
    (ModelNode*) &iedModel_Device1_LPHD1_PhyNam,
};

DataObject iedModel_Device1_LPHD1_PhyNam = {
    DataObjectModelType,
    "PhyNam",
    (ModelNode*) &iedModel_Device1_LPHD1,
    (ModelNode*) &iedModel_Device1_LPHD1_PhyHealth,
    (ModelNode*) &iedModel_Device1_LPHD1_PhyNam_vendor,
    0,
    -1
};

DataAttribute iedModel_Device1_LPHD1_PhyNam_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_Device1_LPHD1_PhyNam,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_Device1_LPHD1_PhyHealth = {
    DataObjectModelType,
    "PhyHealth",
    (ModelNode*) &iedModel_Device1_LPHD1,
    (ModelNode*) &iedModel_Device1_LPHD1_Proxy,
    (ModelNode*) &iedModel_Device1_LPHD1_PhyHealth_stVal,
    0,
    -1
};

DataAttribute iedModel_Device1_LPHD1_PhyHealth_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_Device1_LPHD1_PhyHealth,
    (ModelNode*) &iedModel_Device1_LPHD1_PhyHealth_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_LPHD1_PhyHealth_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Device1_LPHD1_PhyHealth,
    (ModelNode*) &iedModel_Device1_LPHD1_PhyHealth_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_LPHD1_PhyHealth_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Device1_LPHD1_PhyHealth,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_Device1_LPHD1_Proxy = {
    DataObjectModelType,
    "Proxy",
    (ModelNode*) &iedModel_Device1_LPHD1,
    NULL,
    (ModelNode*) &iedModel_Device1_LPHD1_Proxy_stVal,
    0,
    -1
};

DataAttribute iedModel_Device1_LPHD1_Proxy_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_Device1_LPHD1_Proxy,
    (ModelNode*) &iedModel_Device1_LPHD1_Proxy_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_LPHD1_Proxy_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Device1_LPHD1_Proxy,
    (ModelNode*) &iedModel_Device1_LPHD1_Proxy_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_LPHD1_Proxy_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Device1_LPHD1_Proxy,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

LogicalNode iedModel_Device1_DGEN1 = {
    LogicalNodeModelType,
    "DGEN1",
    (ModelNode*) &iedModel_Device1,
    (ModelNode*) &iedModel_Device1_DSCH1,
    (ModelNode*) &iedModel_Device1_DGEN1_Mod,
};

DataObject iedModel_Device1_DGEN1_Mod = {
    DataObjectModelType,
    "Mod",
    (ModelNode*) &iedModel_Device1_DGEN1,
    (ModelNode*) &iedModel_Device1_DGEN1_Beh,
    (ModelNode*) &iedModel_Device1_DGEN1_Mod_q,
    0,
    -1
};

DataAttribute iedModel_Device1_DGEN1_Mod_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Device1_DGEN1_Mod,
    (ModelNode*) &iedModel_Device1_DGEN1_Mod_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DGEN1_Mod_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Device1_DGEN1_Mod,
    (ModelNode*) &iedModel_Device1_DGEN1_Mod_ctlModel,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_Device1_DGEN1_Mod_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_Device1_DGEN1_Mod,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_Device1_DGEN1_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_Device1_DGEN1,
    (ModelNode*) &iedModel_Device1_DGEN1_Health,
    (ModelNode*) &iedModel_Device1_DGEN1_Beh_stVal,
    0,
    -1
};

DataAttribute iedModel_Device1_DGEN1_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_Device1_DGEN1_Beh,
    (ModelNode*) &iedModel_Device1_DGEN1_Beh_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DGEN1_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Device1_DGEN1_Beh,
    (ModelNode*) &iedModel_Device1_DGEN1_Beh_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DGEN1_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Device1_DGEN1_Beh,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_Device1_DGEN1_Health = {
    DataObjectModelType,
    "Health",
    (ModelNode*) &iedModel_Device1_DGEN1,
    (ModelNode*) &iedModel_Device1_DGEN1_NamPlt,
    (ModelNode*) &iedModel_Device1_DGEN1_Health_stVal,
    0,
    -1
};

DataAttribute iedModel_Device1_DGEN1_Health_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_Device1_DGEN1_Health,
    (ModelNode*) &iedModel_Device1_DGEN1_Health_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DGEN1_Health_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Device1_DGEN1_Health,
    (ModelNode*) &iedModel_Device1_DGEN1_Health_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DGEN1_Health_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Device1_DGEN1_Health,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_Device1_DGEN1_NamPlt = {
    DataObjectModelType,
    "NamPlt",
    (ModelNode*) &iedModel_Device1_DGEN1,
    (ModelNode*) &iedModel_Device1_DGEN1_OpTmh,
    (ModelNode*) &iedModel_Device1_DGEN1_NamPlt_vendor,
    0,
    -1
};

DataAttribute iedModel_Device1_DGEN1_NamPlt_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_Device1_DGEN1_NamPlt,
    (ModelNode*) &iedModel_Device1_DGEN1_NamPlt_swRev,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_Device1_DGEN1_NamPlt_swRev = {
    DataAttributeModelType,
    "swRev",
    (ModelNode*) &iedModel_Device1_DGEN1_NamPlt,
    (ModelNode*) &iedModel_Device1_DGEN1_NamPlt_d,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_Device1_DGEN1_NamPlt_d = {
    DataAttributeModelType,
    "d",
    (ModelNode*) &iedModel_Device1_DGEN1_NamPlt,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_Device1_DGEN1_OpTmh = {
    DataObjectModelType,
    "OpTmh",
    (ModelNode*) &iedModel_Device1_DGEN1,
    (ModelNode*) &iedModel_Device1_DGEN1_GnOpSt,
    (ModelNode*) &iedModel_Device1_DGEN1_OpTmh_stVal,
    0,
    -1
};

DataAttribute iedModel_Device1_DGEN1_OpTmh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_Device1_DGEN1_OpTmh,
    (ModelNode*) &iedModel_Device1_DGEN1_OpTmh_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DGEN1_OpTmh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Device1_DGEN1_OpTmh,
    (ModelNode*) &iedModel_Device1_DGEN1_OpTmh_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DGEN1_OpTmh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Device1_DGEN1_OpTmh,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_Device1_DGEN1_GnOpSt = {
    DataObjectModelType,
    "GnOpSt",
    (ModelNode*) &iedModel_Device1_DGEN1,
    (ModelNode*) &iedModel_Device1_DGEN1_OpTmsRs,
    (ModelNode*) &iedModel_Device1_DGEN1_GnOpSt_stVal,
    0,
    -1
};

DataAttribute iedModel_Device1_DGEN1_GnOpSt_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_Device1_DGEN1_GnOpSt,
    (ModelNode*) &iedModel_Device1_DGEN1_GnOpSt_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DGEN1_GnOpSt_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Device1_DGEN1_GnOpSt,
    (ModelNode*) &iedModel_Device1_DGEN1_GnOpSt_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DGEN1_GnOpSt_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Device1_DGEN1_GnOpSt,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_Device1_DGEN1_OpTmsRs = {
    DataObjectModelType,
    "OpTmsRs",
    (ModelNode*) &iedModel_Device1_DGEN1,
    (ModelNode*) &iedModel_Device1_DGEN1_TotWh,
    (ModelNode*) &iedModel_Device1_DGEN1_OpTmsRs_stVal,
    0,
    -1
};

DataAttribute iedModel_Device1_DGEN1_OpTmsRs_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_Device1_DGEN1_OpTmsRs,
    (ModelNode*) &iedModel_Device1_DGEN1_OpTmsRs_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DGEN1_OpTmsRs_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Device1_DGEN1_OpTmsRs,
    (ModelNode*) &iedModel_Device1_DGEN1_OpTmsRs_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DGEN1_OpTmsRs_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Device1_DGEN1_OpTmsRs,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_Device1_DGEN1_TotWh = {
    DataObjectModelType,
    "TotWh",
    (ModelNode*) &iedModel_Device1_DGEN1,
    NULL,
    (ModelNode*) &iedModel_Device1_DGEN1_TotWh_mag,
    0,
    -1
};

DataAttribute iedModel_Device1_DGEN1_TotWh_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_Device1_DGEN1_TotWh,
    (ModelNode*) &iedModel_Device1_DGEN1_TotWh_q,
    (ModelNode*) &iedModel_Device1_DGEN1_TotWh_mag_f,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DGEN1_TotWh_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_Device1_DGEN1_TotWh_mag,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DGEN1_TotWh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Device1_DGEN1_TotWh,
    (ModelNode*) &iedModel_Device1_DGEN1_TotWh_t,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DGEN1_TotWh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Device1_DGEN1_TotWh,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

LogicalNode iedModel_Device1_DSCH1 = {
    LogicalNodeModelType,
    "DSCH1",
    (ModelNode*) &iedModel_Device1,
    (ModelNode*) &iedModel_Device1_MMXU1,
    (ModelNode*) &iedModel_Device1_DSCH1_Mod,
};

DataObject iedModel_Device1_DSCH1_Mod = {
    DataObjectModelType,
    "Mod",
    (ModelNode*) &iedModel_Device1_DSCH1,
    (ModelNode*) &iedModel_Device1_DSCH1_Beh,
    (ModelNode*) &iedModel_Device1_DSCH1_Mod_q,
    0,
    -1
};

DataAttribute iedModel_Device1_DSCH1_Mod_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Device1_DSCH1_Mod,
    (ModelNode*) &iedModel_Device1_DSCH1_Mod_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_Mod_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Device1_DSCH1_Mod,
    (ModelNode*) &iedModel_Device1_DSCH1_Mod_ctlModel,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_Mod_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_Device1_DSCH1_Mod,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_Device1_DSCH1_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_Device1_DSCH1,
    (ModelNode*) &iedModel_Device1_DSCH1_Health,
    (ModelNode*) &iedModel_Device1_DSCH1_Beh_stVal,
    0,
    -1
};

DataAttribute iedModel_Device1_DSCH1_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_Device1_DSCH1_Beh,
    (ModelNode*) &iedModel_Device1_DSCH1_Beh_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Device1_DSCH1_Beh,
    (ModelNode*) &iedModel_Device1_DSCH1_Beh_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Device1_DSCH1_Beh,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_Device1_DSCH1_Health = {
    DataObjectModelType,
    "Health",
    (ModelNode*) &iedModel_Device1_DSCH1,
    (ModelNode*) &iedModel_Device1_DSCH1_NamPlt,
    (ModelNode*) &iedModel_Device1_DSCH1_Health_stVal,
    0,
    -1
};

DataAttribute iedModel_Device1_DSCH1_Health_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_Device1_DSCH1_Health,
    (ModelNode*) &iedModel_Device1_DSCH1_Health_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_Health_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Device1_DSCH1_Health,
    (ModelNode*) &iedModel_Device1_DSCH1_Health_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_Health_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Device1_DSCH1_Health,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_Device1_DSCH1_NamPlt = {
    DataObjectModelType,
    "NamPlt",
    (ModelNode*) &iedModel_Device1_DSCH1,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdSt,
    (ModelNode*) &iedModel_Device1_DSCH1_NamPlt_vendor,
    0,
    -1
};

DataAttribute iedModel_Device1_DSCH1_NamPlt_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_Device1_DSCH1_NamPlt,
    (ModelNode*) &iedModel_Device1_DSCH1_NamPlt_swRev,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_NamPlt_swRev = {
    DataAttributeModelType,
    "swRev",
    (ModelNode*) &iedModel_Device1_DSCH1_NamPlt,
    (ModelNode*) &iedModel_Device1_DSCH1_NamPlt_d,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_NamPlt_d = {
    DataAttributeModelType,
    "d",
    (ModelNode*) &iedModel_Device1_DSCH1_NamPlt,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_Device1_DSCH1_SchdSt = {
    DataObjectModelType,
    "SchdSt",
    (ModelNode*) &iedModel_Device1_DSCH1,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdId,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdSt_stVal,
    0,
    -1
};

DataAttribute iedModel_Device1_DSCH1_SchdSt_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_Device1_DSCH1_SchdSt,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdSt_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdSt_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Device1_DSCH1_SchdSt,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdSt_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdSt_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Device1_DSCH1_SchdSt,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_Device1_DSCH1_SchdId = {
    DataObjectModelType,
    "SchdId",
    (ModelNode*) &iedModel_Device1_DSCH1,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdCat,
    NULL,
    0,
    -1
};

DataObject iedModel_Device1_DSCH1_SchdCat = {
    DataObjectModelType,
    "SchdCat",
    (ModelNode*) &iedModel_Device1_DSCH1,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdTyp,
    NULL,
    0,
    -1
};

DataObject iedModel_Device1_DSCH1_SchdTyp = {
    DataObjectModelType,
    "SchdTyp",
    (ModelNode*) &iedModel_Device1_DSCH1,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm,
    NULL,
    0,
    -1
};

DataObject iedModel_Device1_DSCH1_SchdAbsTm = {
    DataObjectModelType,
    "SchdAbsTm",
    (ModelNode*) &iedModel_Device1_DSCH1,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    0,
    -1
};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val = {
    DataAttributeModelType,
    "val",
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_0,
    255,
    -1,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_0 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_1,
    NULL,
    0,
    0,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_1 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_2,
    NULL,
    0,
    1,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_2 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_3,
    NULL,
    0,
    2,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_3 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_4,
    NULL,
    0,
    3,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_4 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_5,
    NULL,
    0,
    4,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_5 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_6,
    NULL,
    0,
    5,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_6 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_7,
    NULL,
    0,
    6,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_7 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_8,
    NULL,
    0,
    7,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_8 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_9,
    NULL,
    0,
    8,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_9 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_10,
    NULL,
    0,
    9,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_10 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_11,
    NULL,
    0,
    10,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_11 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_12,
    NULL,
    0,
    11,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_12 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_13,
    NULL,
    0,
    12,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_13 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_14,
    NULL,
    0,
    13,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_14 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_15,
    NULL,
    0,
    14,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_15 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_16,
    NULL,
    0,
    15,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_16 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_17,
    NULL,
    0,
    16,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_17 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_18,
    NULL,
    0,
    17,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_18 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_19,
    NULL,
    0,
    18,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_19 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_20,
    NULL,
    0,
    19,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_20 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_21,
    NULL,
    0,
    20,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_21 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_22,
    NULL,
    0,
    21,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_22 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_23,
    NULL,
    0,
    22,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_23 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_24,
    NULL,
    0,
    23,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_24 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_25,
    NULL,
    0,
    24,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_25 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_26,
    NULL,
    0,
    25,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_26 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_27,
    NULL,
    0,
    26,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_27 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_28,
    NULL,
    0,
    27,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_28 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_29,
    NULL,
    0,
    28,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_29 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_30,
    NULL,
    0,
    29,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_30 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_31,
    NULL,
    0,
    30,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_31 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_32,
    NULL,
    0,
    31,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_32 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_33,
    NULL,
    0,
    32,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_33 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_34,
    NULL,
    0,
    33,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_34 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_35,
    NULL,
    0,
    34,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_35 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_36,
    NULL,
    0,
    35,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_36 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_37,
    NULL,
    0,
    36,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_37 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_38,
    NULL,
    0,
    37,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_38 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_39,
    NULL,
    0,
    38,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_39 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_40,
    NULL,
    0,
    39,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_40 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_41,
    NULL,
    0,
    40,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_41 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_42,
    NULL,
    0,
    41,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_42 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_43,
    NULL,
    0,
    42,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_43 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_44,
    NULL,
    0,
    43,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_44 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_45,
    NULL,
    0,
    44,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_45 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_46,
    NULL,
    0,
    45,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_46 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_47,
    NULL,
    0,
    46,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_47 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_48,
    NULL,
    0,
    47,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_48 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_49,
    NULL,
    0,
    48,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_49 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_50,
    NULL,
    0,
    49,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_50 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_51,
    NULL,
    0,
    50,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_51 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_52,
    NULL,
    0,
    51,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_52 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_53,
    NULL,
    0,
    52,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_53 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_54,
    NULL,
    0,
    53,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_54 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_55,
    NULL,
    0,
    54,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_55 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_56,
    NULL,
    0,
    55,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_56 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_57,
    NULL,
    0,
    56,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_57 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_58,
    NULL,
    0,
    57,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_58 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_59,
    NULL,
    0,
    58,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_59 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_60,
    NULL,
    0,
    59,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_60 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_61,
    NULL,
    0,
    60,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_61 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_62,
    NULL,
    0,
    61,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_62 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_63,
    NULL,
    0,
    62,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_63 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_64,
    NULL,
    0,
    63,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_64 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_65,
    NULL,
    0,
    64,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_65 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_66,
    NULL,
    0,
    65,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_66 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_67,
    NULL,
    0,
    66,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_67 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_68,
    NULL,
    0,
    67,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_68 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_69,
    NULL,
    0,
    68,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_69 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_70,
    NULL,
    0,
    69,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_70 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_71,
    NULL,
    0,
    70,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_71 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_72,
    NULL,
    0,
    71,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_72 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_73,
    NULL,
    0,
    72,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_73 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_74,
    NULL,
    0,
    73,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_74 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_75,
    NULL,
    0,
    74,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_75 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_76,
    NULL,
    0,
    75,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_76 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_77,
    NULL,
    0,
    76,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_77 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_78,
    NULL,
    0,
    77,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_78 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_79,
    NULL,
    0,
    78,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_79 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_80,
    NULL,
    0,
    79,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_80 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_81,
    NULL,
    0,
    80,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_81 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_82,
    NULL,
    0,
    81,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_82 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_83,
    NULL,
    0,
    82,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_83 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_84,
    NULL,
    0,
    83,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_84 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_85,
    NULL,
    0,
    84,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_85 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_86,
    NULL,
    0,
    85,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_86 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_87,
    NULL,
    0,
    86,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_87 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_88,
    NULL,
    0,
    87,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_88 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_89,
    NULL,
    0,
    88,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_89 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_90,
    NULL,
    0,
    89,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_90 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_91,
    NULL,
    0,
    90,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_91 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_92,
    NULL,
    0,
    91,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_92 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_93,
    NULL,
    0,
    92,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_93 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_94,
    NULL,
    0,
    93,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_94 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_95,
    NULL,
    0,
    94,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_95 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_96,
    NULL,
    0,
    95,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_96 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_97,
    NULL,
    0,
    96,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_97 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_98,
    NULL,
    0,
    97,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_98 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_99,
    NULL,
    0,
    98,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_99 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_100,
    NULL,
    0,
    99,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_100 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_101,
    NULL,
    0,
    100,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_101 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_102,
    NULL,
    0,
    101,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_102 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_103,
    NULL,
    0,
    102,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_103 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_104,
    NULL,
    0,
    103,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_104 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_105,
    NULL,
    0,
    104,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_105 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_106,
    NULL,
    0,
    105,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_106 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_107,
    NULL,
    0,
    106,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_107 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_108,
    NULL,
    0,
    107,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_108 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_109,
    NULL,
    0,
    108,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_109 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_110,
    NULL,
    0,
    109,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_110 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_111,
    NULL,
    0,
    110,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_111 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_112,
    NULL,
    0,
    111,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_112 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_113,
    NULL,
    0,
    112,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_113 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_114,
    NULL,
    0,
    113,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_114 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_115,
    NULL,
    0,
    114,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_115 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_116,
    NULL,
    0,
    115,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_116 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_117,
    NULL,
    0,
    116,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_117 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_118,
    NULL,
    0,
    117,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_118 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_119,
    NULL,
    0,
    118,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_119 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_120,
    NULL,
    0,
    119,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_120 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_121,
    NULL,
    0,
    120,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_121 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_122,
    NULL,
    0,
    121,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_122 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_123,
    NULL,
    0,
    122,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_123 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_124,
    NULL,
    0,
    123,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_124 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_125,
    NULL,
    0,
    124,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_125 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_126,
    NULL,
    0,
    125,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_126 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_127,
    NULL,
    0,
    126,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_127 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_128,
    NULL,
    0,
    127,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_128 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_129,
    NULL,
    0,
    128,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_129 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_130,
    NULL,
    0,
    129,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_130 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_131,
    NULL,
    0,
    130,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_131 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_132,
    NULL,
    0,
    131,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_132 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_133,
    NULL,
    0,
    132,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_133 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_134,
    NULL,
    0,
    133,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_134 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_135,
    NULL,
    0,
    134,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_135 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_136,
    NULL,
    0,
    135,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_136 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_137,
    NULL,
    0,
    136,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_137 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_138,
    NULL,
    0,
    137,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_138 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_139,
    NULL,
    0,
    138,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_139 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_140,
    NULL,
    0,
    139,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_140 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_141,
    NULL,
    0,
    140,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_141 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_142,
    NULL,
    0,
    141,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_142 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_143,
    NULL,
    0,
    142,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_143 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_144,
    NULL,
    0,
    143,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_144 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_145,
    NULL,
    0,
    144,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_145 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_146,
    NULL,
    0,
    145,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_146 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_147,
    NULL,
    0,
    146,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_147 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_148,
    NULL,
    0,
    147,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_148 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_149,
    NULL,
    0,
    148,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_149 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_150,
    NULL,
    0,
    149,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_150 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_151,
    NULL,
    0,
    150,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_151 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_152,
    NULL,
    0,
    151,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_152 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_153,
    NULL,
    0,
    152,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_153 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_154,
    NULL,
    0,
    153,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_154 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_155,
    NULL,
    0,
    154,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_155 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_156,
    NULL,
    0,
    155,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_156 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_157,
    NULL,
    0,
    156,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_157 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_158,
    NULL,
    0,
    157,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_158 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_159,
    NULL,
    0,
    158,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_159 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_160,
    NULL,
    0,
    159,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_160 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_161,
    NULL,
    0,
    160,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_161 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_162,
    NULL,
    0,
    161,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_162 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_163,
    NULL,
    0,
    162,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_163 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_164,
    NULL,
    0,
    163,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_164 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_165,
    NULL,
    0,
    164,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_165 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_166,
    NULL,
    0,
    165,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_166 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_167,
    NULL,
    0,
    166,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_167 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_168,
    NULL,
    0,
    167,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_168 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_169,
    NULL,
    0,
    168,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_169 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_170,
    NULL,
    0,
    169,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_170 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_171,
    NULL,
    0,
    170,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_171 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_172,
    NULL,
    0,
    171,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_172 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_173,
    NULL,
    0,
    172,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_173 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_174,
    NULL,
    0,
    173,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_174 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_175,
    NULL,
    0,
    174,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_175 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_176,
    NULL,
    0,
    175,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_176 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_177,
    NULL,
    0,
    176,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_177 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_178,
    NULL,
    0,
    177,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_178 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_179,
    NULL,
    0,
    178,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_179 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_180,
    NULL,
    0,
    179,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_180 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_181,
    NULL,
    0,
    180,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_181 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_182,
    NULL,
    0,
    181,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_182 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_183,
    NULL,
    0,
    182,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_183 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_184,
    NULL,
    0,
    183,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_184 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_185,
    NULL,
    0,
    184,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_185 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_186,
    NULL,
    0,
    185,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_186 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_187,
    NULL,
    0,
    186,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_187 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_188,
    NULL,
    0,
    187,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_188 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_189,
    NULL,
    0,
    188,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_189 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_190,
    NULL,
    0,
    189,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_190 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_191,
    NULL,
    0,
    190,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_191 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_192,
    NULL,
    0,
    191,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_192 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_193,
    NULL,
    0,
    192,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_193 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_194,
    NULL,
    0,
    193,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_194 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_195,
    NULL,
    0,
    194,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_195 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_196,
    NULL,
    0,
    195,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_196 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_197,
    NULL,
    0,
    196,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_197 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_198,
    NULL,
    0,
    197,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_198 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_199,
    NULL,
    0,
    198,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_199 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_200,
    NULL,
    0,
    199,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_200 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_201,
    NULL,
    0,
    200,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_201 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_202,
    NULL,
    0,
    201,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_202 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_203,
    NULL,
    0,
    202,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_203 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_204,
    NULL,
    0,
    203,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_204 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_205,
    NULL,
    0,
    204,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_205 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_206,
    NULL,
    0,
    205,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_206 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_207,
    NULL,
    0,
    206,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_207 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_208,
    NULL,
    0,
    207,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_208 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_209,
    NULL,
    0,
    208,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_209 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_210,
    NULL,
    0,
    209,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_210 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_211,
    NULL,
    0,
    210,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_211 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_212,
    NULL,
    0,
    211,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_212 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_213,
    NULL,
    0,
    212,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_213 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_214,
    NULL,
    0,
    213,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_214 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_215,
    NULL,
    0,
    214,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_215 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_216,
    NULL,
    0,
    215,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_216 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_217,
    NULL,
    0,
    216,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_217 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_218,
    NULL,
    0,
    217,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_218 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_219,
    NULL,
    0,
    218,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_219 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_220,
    NULL,
    0,
    219,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_220 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_221,
    NULL,
    0,
    220,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_221 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_222,
    NULL,
    0,
    221,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_222 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_223,
    NULL,
    0,
    222,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_223 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_224,
    NULL,
    0,
    223,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_224 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_225,
    NULL,
    0,
    224,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_225 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_226,
    NULL,
    0,
    225,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_226 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_227,
    NULL,
    0,
    226,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_227 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_228,
    NULL,
    0,
    227,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_228 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_229,
    NULL,
    0,
    228,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_229 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_230,
    NULL,
    0,
    229,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_230 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_231,
    NULL,
    0,
    230,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_231 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_232,
    NULL,
    0,
    231,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_232 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_233,
    NULL,
    0,
    232,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_233 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_234,
    NULL,
    0,
    233,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_234 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_235,
    NULL,
    0,
    234,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_235 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_236,
    NULL,
    0,
    235,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_236 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_237,
    NULL,
    0,
    236,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_237 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_238,
    NULL,
    0,
    237,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_238 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_239,
    NULL,
    0,
    238,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_239 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_240,
    NULL,
    0,
    239,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_240 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_241,
    NULL,
    0,
    240,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_241 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_242,
    NULL,
    0,
    241,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_242 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_243,
    NULL,
    0,
    242,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_243 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_244,
    NULL,
    0,
    243,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_244 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_245,
    NULL,
    0,
    244,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_245 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_246,
    NULL,
    0,
    245,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_246 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_247,
    NULL,
    0,
    246,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_247 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_248,
    NULL,
    0,
    247,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_248 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_249,
    NULL,
    0,
    248,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_249 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_250,
    NULL,
    0,
    249,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_250 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_251,
    NULL,
    0,
    250,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_251 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_252,
    NULL,
    0,
    251,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_252 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_253,
    NULL,
    0,
    252,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_253 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val_254,
    NULL,
    0,
    253,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val_254 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    NULL,
    NULL,
    0,
    254,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time = {
    DataAttributeModelType,
    "time",
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_0,
    255,
    -1,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_0 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_1,
    NULL,
    0,
    0,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_1 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_2,
    NULL,
    0,
    1,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_2 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_3,
    NULL,
    0,
    2,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_3 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_4,
    NULL,
    0,
    3,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_4 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_5,
    NULL,
    0,
    4,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_5 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_6,
    NULL,
    0,
    5,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_6 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_7,
    NULL,
    0,
    6,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_7 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_8,
    NULL,
    0,
    7,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_8 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_9,
    NULL,
    0,
    8,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_9 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_10,
    NULL,
    0,
    9,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_10 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_11,
    NULL,
    0,
    10,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_11 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_12,
    NULL,
    0,
    11,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_12 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_13,
    NULL,
    0,
    12,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_13 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_14,
    NULL,
    0,
    13,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_14 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_15,
    NULL,
    0,
    14,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_15 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_16,
    NULL,
    0,
    15,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_16 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_17,
    NULL,
    0,
    16,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_17 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_18,
    NULL,
    0,
    17,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_18 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_19,
    NULL,
    0,
    18,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_19 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_20,
    NULL,
    0,
    19,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_20 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_21,
    NULL,
    0,
    20,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_21 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_22,
    NULL,
    0,
    21,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_22 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_23,
    NULL,
    0,
    22,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_23 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_24,
    NULL,
    0,
    23,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_24 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_25,
    NULL,
    0,
    24,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_25 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_26,
    NULL,
    0,
    25,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_26 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_27,
    NULL,
    0,
    26,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_27 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_28,
    NULL,
    0,
    27,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_28 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_29,
    NULL,
    0,
    28,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_29 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_30,
    NULL,
    0,
    29,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_30 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_31,
    NULL,
    0,
    30,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_31 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_32,
    NULL,
    0,
    31,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_32 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_33,
    NULL,
    0,
    32,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_33 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_34,
    NULL,
    0,
    33,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_34 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_35,
    NULL,
    0,
    34,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_35 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_36,
    NULL,
    0,
    35,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_36 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_37,
    NULL,
    0,
    36,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_37 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_38,
    NULL,
    0,
    37,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_38 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_39,
    NULL,
    0,
    38,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_39 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_40,
    NULL,
    0,
    39,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_40 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_41,
    NULL,
    0,
    40,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_41 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_42,
    NULL,
    0,
    41,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_42 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_43,
    NULL,
    0,
    42,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_43 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_44,
    NULL,
    0,
    43,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_44 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_45,
    NULL,
    0,
    44,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_45 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_46,
    NULL,
    0,
    45,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_46 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_47,
    NULL,
    0,
    46,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_47 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_48,
    NULL,
    0,
    47,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_48 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_49,
    NULL,
    0,
    48,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_49 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_50,
    NULL,
    0,
    49,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_50 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_51,
    NULL,
    0,
    50,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_51 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_52,
    NULL,
    0,
    51,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_52 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_53,
    NULL,
    0,
    52,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_53 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_54,
    NULL,
    0,
    53,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_54 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_55,
    NULL,
    0,
    54,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_55 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_56,
    NULL,
    0,
    55,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_56 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_57,
    NULL,
    0,
    56,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_57 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_58,
    NULL,
    0,
    57,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_58 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_59,
    NULL,
    0,
    58,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_59 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_60,
    NULL,
    0,
    59,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_60 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_61,
    NULL,
    0,
    60,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_61 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_62,
    NULL,
    0,
    61,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_62 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_63,
    NULL,
    0,
    62,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_63 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_64,
    NULL,
    0,
    63,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_64 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_65,
    NULL,
    0,
    64,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_65 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_66,
    NULL,
    0,
    65,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_66 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_67,
    NULL,
    0,
    66,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_67 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_68,
    NULL,
    0,
    67,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_68 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_69,
    NULL,
    0,
    68,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_69 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_70,
    NULL,
    0,
    69,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_70 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_71,
    NULL,
    0,
    70,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_71 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_72,
    NULL,
    0,
    71,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_72 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_73,
    NULL,
    0,
    72,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_73 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_74,
    NULL,
    0,
    73,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_74 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_75,
    NULL,
    0,
    74,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_75 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_76,
    NULL,
    0,
    75,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_76 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_77,
    NULL,
    0,
    76,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_77 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_78,
    NULL,
    0,
    77,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_78 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_79,
    NULL,
    0,
    78,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_79 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_80,
    NULL,
    0,
    79,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_80 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_81,
    NULL,
    0,
    80,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_81 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_82,
    NULL,
    0,
    81,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_82 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_83,
    NULL,
    0,
    82,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_83 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_84,
    NULL,
    0,
    83,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_84 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_85,
    NULL,
    0,
    84,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_85 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_86,
    NULL,
    0,
    85,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_86 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_87,
    NULL,
    0,
    86,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_87 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_88,
    NULL,
    0,
    87,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_88 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_89,
    NULL,
    0,
    88,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_89 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_90,
    NULL,
    0,
    89,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_90 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_91,
    NULL,
    0,
    90,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_91 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_92,
    NULL,
    0,
    91,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_92 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_93,
    NULL,
    0,
    92,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_93 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_94,
    NULL,
    0,
    93,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_94 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_95,
    NULL,
    0,
    94,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_95 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_96,
    NULL,
    0,
    95,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_96 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_97,
    NULL,
    0,
    96,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_97 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_98,
    NULL,
    0,
    97,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_98 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_99,
    NULL,
    0,
    98,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_99 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_100,
    NULL,
    0,
    99,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_100 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_101,
    NULL,
    0,
    100,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_101 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_102,
    NULL,
    0,
    101,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_102 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_103,
    NULL,
    0,
    102,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_103 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_104,
    NULL,
    0,
    103,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_104 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_105,
    NULL,
    0,
    104,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_105 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_106,
    NULL,
    0,
    105,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_106 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_107,
    NULL,
    0,
    106,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_107 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_108,
    NULL,
    0,
    107,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_108 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_109,
    NULL,
    0,
    108,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_109 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_110,
    NULL,
    0,
    109,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_110 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_111,
    NULL,
    0,
    110,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_111 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_112,
    NULL,
    0,
    111,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_112 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_113,
    NULL,
    0,
    112,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_113 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_114,
    NULL,
    0,
    113,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_114 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_115,
    NULL,
    0,
    114,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_115 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_116,
    NULL,
    0,
    115,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_116 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_117,
    NULL,
    0,
    116,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_117 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_118,
    NULL,
    0,
    117,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_118 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_119,
    NULL,
    0,
    118,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_119 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_120,
    NULL,
    0,
    119,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_120 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_121,
    NULL,
    0,
    120,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_121 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_122,
    NULL,
    0,
    121,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_122 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_123,
    NULL,
    0,
    122,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_123 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_124,
    NULL,
    0,
    123,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_124 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_125,
    NULL,
    0,
    124,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_125 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_126,
    NULL,
    0,
    125,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_126 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_127,
    NULL,
    0,
    126,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_127 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_128,
    NULL,
    0,
    127,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_128 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_129,
    NULL,
    0,
    128,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_129 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_130,
    NULL,
    0,
    129,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_130 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_131,
    NULL,
    0,
    130,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_131 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_132,
    NULL,
    0,
    131,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_132 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_133,
    NULL,
    0,
    132,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_133 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_134,
    NULL,
    0,
    133,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_134 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_135,
    NULL,
    0,
    134,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_135 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_136,
    NULL,
    0,
    135,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_136 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_137,
    NULL,
    0,
    136,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_137 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_138,
    NULL,
    0,
    137,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_138 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_139,
    NULL,
    0,
    138,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_139 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_140,
    NULL,
    0,
    139,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_140 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_141,
    NULL,
    0,
    140,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_141 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_142,
    NULL,
    0,
    141,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_142 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_143,
    NULL,
    0,
    142,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_143 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_144,
    NULL,
    0,
    143,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_144 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_145,
    NULL,
    0,
    144,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_145 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_146,
    NULL,
    0,
    145,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_146 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_147,
    NULL,
    0,
    146,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_147 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_148,
    NULL,
    0,
    147,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_148 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_149,
    NULL,
    0,
    148,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_149 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_150,
    NULL,
    0,
    149,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_150 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_151,
    NULL,
    0,
    150,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_151 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_152,
    NULL,
    0,
    151,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_152 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_153,
    NULL,
    0,
    152,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_153 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_154,
    NULL,
    0,
    153,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_154 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_155,
    NULL,
    0,
    154,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_155 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_156,
    NULL,
    0,
    155,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_156 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_157,
    NULL,
    0,
    156,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_157 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_158,
    NULL,
    0,
    157,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_158 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_159,
    NULL,
    0,
    158,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_159 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_160,
    NULL,
    0,
    159,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_160 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_161,
    NULL,
    0,
    160,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_161 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_162,
    NULL,
    0,
    161,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_162 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_163,
    NULL,
    0,
    162,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_163 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_164,
    NULL,
    0,
    163,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_164 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_165,
    NULL,
    0,
    164,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_165 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_166,
    NULL,
    0,
    165,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_166 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_167,
    NULL,
    0,
    166,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_167 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_168,
    NULL,
    0,
    167,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_168 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_169,
    NULL,
    0,
    168,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_169 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_170,
    NULL,
    0,
    169,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_170 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_171,
    NULL,
    0,
    170,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_171 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_172,
    NULL,
    0,
    171,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_172 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_173,
    NULL,
    0,
    172,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_173 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_174,
    NULL,
    0,
    173,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_174 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_175,
    NULL,
    0,
    174,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_175 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_176,
    NULL,
    0,
    175,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_176 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_177,
    NULL,
    0,
    176,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_177 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_178,
    NULL,
    0,
    177,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_178 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_179,
    NULL,
    0,
    178,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_179 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_180,
    NULL,
    0,
    179,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_180 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_181,
    NULL,
    0,
    180,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_181 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_182,
    NULL,
    0,
    181,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_182 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_183,
    NULL,
    0,
    182,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_183 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_184,
    NULL,
    0,
    183,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_184 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_185,
    NULL,
    0,
    184,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_185 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_186,
    NULL,
    0,
    185,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_186 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_187,
    NULL,
    0,
    186,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_187 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_188,
    NULL,
    0,
    187,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_188 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_189,
    NULL,
    0,
    188,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_189 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_190,
    NULL,
    0,
    189,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_190 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_191,
    NULL,
    0,
    190,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_191 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_192,
    NULL,
    0,
    191,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_192 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_193,
    NULL,
    0,
    192,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_193 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_194,
    NULL,
    0,
    193,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_194 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_195,
    NULL,
    0,
    194,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_195 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_196,
    NULL,
    0,
    195,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_196 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_197,
    NULL,
    0,
    196,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_197 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_198,
    NULL,
    0,
    197,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_198 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_199,
    NULL,
    0,
    198,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_199 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_200,
    NULL,
    0,
    199,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_200 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_201,
    NULL,
    0,
    200,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_201 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_202,
    NULL,
    0,
    201,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_202 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_203,
    NULL,
    0,
    202,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_203 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_204,
    NULL,
    0,
    203,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_204 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_205,
    NULL,
    0,
    204,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_205 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_206,
    NULL,
    0,
    205,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_206 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_207,
    NULL,
    0,
    206,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_207 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_208,
    NULL,
    0,
    207,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_208 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_209,
    NULL,
    0,
    208,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_209 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_210,
    NULL,
    0,
    209,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_210 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_211,
    NULL,
    0,
    210,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_211 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_212,
    NULL,
    0,
    211,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_212 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_213,
    NULL,
    0,
    212,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_213 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_214,
    NULL,
    0,
    213,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_214 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_215,
    NULL,
    0,
    214,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_215 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_216,
    NULL,
    0,
    215,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_216 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_217,
    NULL,
    0,
    216,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_217 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_218,
    NULL,
    0,
    217,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_218 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_219,
    NULL,
    0,
    218,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_219 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_220,
    NULL,
    0,
    219,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_220 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_221,
    NULL,
    0,
    220,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_221 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_222,
    NULL,
    0,
    221,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_222 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_223,
    NULL,
    0,
    222,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_223 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_224,
    NULL,
    0,
    223,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_224 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_225,
    NULL,
    0,
    224,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_225 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_226,
    NULL,
    0,
    225,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_226 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_227,
    NULL,
    0,
    226,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_227 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_228,
    NULL,
    0,
    227,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_228 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_229,
    NULL,
    0,
    228,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_229 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_230,
    NULL,
    0,
    229,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_230 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_231,
    NULL,
    0,
    230,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_231 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_232,
    NULL,
    0,
    231,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_232 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_233,
    NULL,
    0,
    232,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_233 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_234,
    NULL,
    0,
    233,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_234 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_235,
    NULL,
    0,
    234,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_235 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_236,
    NULL,
    0,
    235,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_236 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_237,
    NULL,
    0,
    236,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_237 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_238,
    NULL,
    0,
    237,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_238 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_239,
    NULL,
    0,
    238,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_239 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_240,
    NULL,
    0,
    239,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_240 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_241,
    NULL,
    0,
    240,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_241 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_242,
    NULL,
    0,
    241,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_242 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_243,
    NULL,
    0,
    242,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_243 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_244,
    NULL,
    0,
    243,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_244 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_245,
    NULL,
    0,
    244,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_245 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_246,
    NULL,
    0,
    245,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_246 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_247,
    NULL,
    0,
    246,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_247 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_248,
    NULL,
    0,
    247,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_248 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_249,
    NULL,
    0,
    248,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_249 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_250,
    NULL,
    0,
    249,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_250 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_251,
    NULL,
    0,
    250,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_251 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_252,
    NULL,
    0,
    251,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_252 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_253,
    NULL,
    0,
    252,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_253 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time_254,
    NULL,
    0,
    253,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_time_254 = {
    DataAttributeModelType,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    NULL,
    NULL,
    0,
    254,
    IEC61850_FC_SP,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

LogicalNode iedModel_Device1_MMXU1 = {
    LogicalNodeModelType,
    "MMXU1",
    (ModelNode*) &iedModel_Device1,
    (ModelNode*) &iedModel_Device1_MMXU2,
    (ModelNode*) &iedModel_Device1_MMXU1_Mod,
};

DataObject iedModel_Device1_MMXU1_Mod = {
    DataObjectModelType,
    "Mod",
    (ModelNode*) &iedModel_Device1_MMXU1,
    (ModelNode*) &iedModel_Device1_MMXU1_Beh,
    (ModelNode*) &iedModel_Device1_MMXU1_Mod_q,
    0,
    -1
};

DataAttribute iedModel_Device1_MMXU1_Mod_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Device1_MMXU1_Mod,
    (ModelNode*) &iedModel_Device1_MMXU1_Mod_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_MMXU1_Mod_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Device1_MMXU1_Mod,
    (ModelNode*) &iedModel_Device1_MMXU1_Mod_ctlModel,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_Device1_MMXU1_Mod_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_Device1_MMXU1_Mod,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_Device1_MMXU1_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_Device1_MMXU1,
    (ModelNode*) &iedModel_Device1_MMXU1_Health,
    (ModelNode*) &iedModel_Device1_MMXU1_Beh_stVal,
    0,
    -1
};

DataAttribute iedModel_Device1_MMXU1_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_Device1_MMXU1_Beh,
    (ModelNode*) &iedModel_Device1_MMXU1_Beh_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_MMXU1_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Device1_MMXU1_Beh,
    (ModelNode*) &iedModel_Device1_MMXU1_Beh_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_MMXU1_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Device1_MMXU1_Beh,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_Device1_MMXU1_Health = {
    DataObjectModelType,
    "Health",
    (ModelNode*) &iedModel_Device1_MMXU1,
    (ModelNode*) &iedModel_Device1_MMXU1_NamPlt,
    (ModelNode*) &iedModel_Device1_MMXU1_Health_stVal,
    0,
    -1
};

DataAttribute iedModel_Device1_MMXU1_Health_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_Device1_MMXU1_Health,
    (ModelNode*) &iedModel_Device1_MMXU1_Health_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_MMXU1_Health_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Device1_MMXU1_Health,
    (ModelNode*) &iedModel_Device1_MMXU1_Health_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_MMXU1_Health_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Device1_MMXU1_Health,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_Device1_MMXU1_NamPlt = {
    DataObjectModelType,
    "NamPlt",
    (ModelNode*) &iedModel_Device1_MMXU1,
    NULL,
    (ModelNode*) &iedModel_Device1_MMXU1_NamPlt_vendor,
    0,
    -1
};

DataAttribute iedModel_Device1_MMXU1_NamPlt_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_Device1_MMXU1_NamPlt,
    (ModelNode*) &iedModel_Device1_MMXU1_NamPlt_swRev,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_Device1_MMXU1_NamPlt_swRev = {
    DataAttributeModelType,
    "swRev",
    (ModelNode*) &iedModel_Device1_MMXU1_NamPlt,
    (ModelNode*) &iedModel_Device1_MMXU1_NamPlt_d,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_Device1_MMXU1_NamPlt_d = {
    DataAttributeModelType,
    "d",
    (ModelNode*) &iedModel_Device1_MMXU1_NamPlt,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

LogicalNode iedModel_Device1_MMXU2 = {
    LogicalNodeModelType,
    "MMXU2",
    (ModelNode*) &iedModel_Device1,
    NULL,
    (ModelNode*) &iedModel_Device1_MMXU2_Mod,
};

DataObject iedModel_Device1_MMXU2_Mod = {
    DataObjectModelType,
    "Mod",
    (ModelNode*) &iedModel_Device1_MMXU2,
    (ModelNode*) &iedModel_Device1_MMXU2_Beh,
    (ModelNode*) &iedModel_Device1_MMXU2_Mod_q,
    0,
    -1
};

DataAttribute iedModel_Device1_MMXU2_Mod_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Device1_MMXU2_Mod,
    (ModelNode*) &iedModel_Device1_MMXU2_Mod_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_MMXU2_Mod_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Device1_MMXU2_Mod,
    (ModelNode*) &iedModel_Device1_MMXU2_Mod_ctlModel,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_Device1_MMXU2_Mod_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_Device1_MMXU2_Mod,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_Device1_MMXU2_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_Device1_MMXU2,
    (ModelNode*) &iedModel_Device1_MMXU2_Health,
    (ModelNode*) &iedModel_Device1_MMXU2_Beh_stVal,
    0,
    -1
};

DataAttribute iedModel_Device1_MMXU2_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_Device1_MMXU2_Beh,
    (ModelNode*) &iedModel_Device1_MMXU2_Beh_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_MMXU2_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Device1_MMXU2_Beh,
    (ModelNode*) &iedModel_Device1_MMXU2_Beh_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_MMXU2_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Device1_MMXU2_Beh,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_Device1_MMXU2_Health = {
    DataObjectModelType,
    "Health",
    (ModelNode*) &iedModel_Device1_MMXU2,
    (ModelNode*) &iedModel_Device1_MMXU2_NamPlt,
    (ModelNode*) &iedModel_Device1_MMXU2_Health_stVal,
    0,
    -1
};

DataAttribute iedModel_Device1_MMXU2_Health_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_Device1_MMXU2_Health,
    (ModelNode*) &iedModel_Device1_MMXU2_Health_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_MMXU2_Health_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Device1_MMXU2_Health,
    (ModelNode*) &iedModel_Device1_MMXU2_Health_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_MMXU2_Health_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Device1_MMXU2_Health,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_Device1_MMXU2_NamPlt = {
    DataObjectModelType,
    "NamPlt",
    (ModelNode*) &iedModel_Device1_MMXU2,
    (ModelNode*) &iedModel_Device1_MMXU2_TotW,
    (ModelNode*) &iedModel_Device1_MMXU2_NamPlt_vendor,
    0,
    -1
};

DataAttribute iedModel_Device1_MMXU2_NamPlt_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_Device1_MMXU2_NamPlt,
    (ModelNode*) &iedModel_Device1_MMXU2_NamPlt_swRev,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_Device1_MMXU2_NamPlt_swRev = {
    DataAttributeModelType,
    "swRev",
    (ModelNode*) &iedModel_Device1_MMXU2_NamPlt,
    (ModelNode*) &iedModel_Device1_MMXU2_NamPlt_d,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_Device1_MMXU2_NamPlt_d = {
    DataAttributeModelType,
    "d",
    (ModelNode*) &iedModel_Device1_MMXU2_NamPlt,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_Device1_MMXU2_TotW = {
    DataObjectModelType,
    "TotW",
    (ModelNode*) &iedModel_Device1_MMXU2,
    NULL,
    (ModelNode*) &iedModel_Device1_MMXU2_TotW_mag,
    0,
    -1
};

DataAttribute iedModel_Device1_MMXU2_TotW_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_Device1_MMXU2_TotW,
    (ModelNode*) &iedModel_Device1_MMXU2_TotW_q,
    (ModelNode*) &iedModel_Device1_MMXU2_TotW_mag_f,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_MMXU2_TotW_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_Device1_MMXU2_TotW_mag,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_MMXU2_TotW_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Device1_MMXU2_TotW,
    (ModelNode*) &iedModel_Device1_MMXU2_TotW_t,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Device1_MMXU2_TotW_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Device1_MMXU2_TotW,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

extern ReportControlBlock iedModel_Device1_LLN0_report0;

ReportControlBlock iedModel_Device1_LLN0_report0 = {&iedModel_Device1_LLN0, "LLN0_Events_BuffRep01", "LLN0$RP$brcbEV1", true, "dataset1", 1, 25, 239, 50, 900000, {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, NULL};







IedModel iedModel = {
    "SampleIED",
    &iedModel_Device1,
    &iedModelds_Device1_LLN0_dataset1,
    &iedModel_Device1_LLN0_report0,
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

iedModel_Device1_LLN0_Mod_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(0);

iedModel_Device1_DGEN1_Mod_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(0);

iedModel_Device1_DSCH1_Mod_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(0);

iedModel_Device1_MMXU1_Mod_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(0);

iedModel_Device1_MMXU2_Mod_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(0);
}
