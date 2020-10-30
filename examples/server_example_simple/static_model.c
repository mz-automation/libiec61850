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
    (ModelNode*) &iedModel_Device1_LLN0
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
    0
};

DataAttribute iedModel_Device1_LLN0_Mod_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Device1_LLN0_Mod,
    (ModelNode*) &iedModel_Device1_LLN0_Mod_t,
    NULL,
    0,
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
    0
};

DataAttribute iedModel_Device1_LLN0_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_Device1_LLN0_Beh,
    (ModelNode*) &iedModel_Device1_LLN0_Beh_q,
    NULL,
    0,
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
    0
};

DataAttribute iedModel_Device1_LLN0_Health_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_Device1_LLN0_Health,
    (ModelNode*) &iedModel_Device1_LLN0_Health_q,
    NULL,
    0,
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
    0
};

DataAttribute iedModel_Device1_LLN0_NamPlt_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_Device1_LLN0_NamPlt,
    (ModelNode*) &iedModel_Device1_LLN0_NamPlt_swRev,
    NULL,
    0,
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
    0
};

DataAttribute iedModel_Device1_LPHD1_PhyNam_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_Device1_LPHD1_PhyNam,
    NULL,
    NULL,
    0,
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
    0
};

DataAttribute iedModel_Device1_LPHD1_PhyHealth_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_Device1_LPHD1_PhyHealth,
    (ModelNode*) &iedModel_Device1_LPHD1_PhyHealth_q,
    NULL,
    0,
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
    0
};

DataAttribute iedModel_Device1_LPHD1_Proxy_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_Device1_LPHD1_Proxy,
    (ModelNode*) &iedModel_Device1_LPHD1_Proxy_q,
    NULL,
    0,
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
    0
};

DataAttribute iedModel_Device1_DGEN1_Mod_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Device1_DGEN1_Mod,
    (ModelNode*) &iedModel_Device1_DGEN1_Mod_t,
    NULL,
    0,
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
    0
};

DataAttribute iedModel_Device1_DGEN1_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_Device1_DGEN1_Beh,
    (ModelNode*) &iedModel_Device1_DGEN1_Beh_q,
    NULL,
    0,
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
    0
};

DataAttribute iedModel_Device1_DGEN1_Health_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_Device1_DGEN1_Health,
    (ModelNode*) &iedModel_Device1_DGEN1_Health_q,
    NULL,
    0,
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
    0
};

DataAttribute iedModel_Device1_DGEN1_NamPlt_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_Device1_DGEN1_NamPlt,
    (ModelNode*) &iedModel_Device1_DGEN1_NamPlt_swRev,
    NULL,
    0,
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
    0
};

DataAttribute iedModel_Device1_DGEN1_OpTmh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_Device1_DGEN1_OpTmh,
    (ModelNode*) &iedModel_Device1_DGEN1_OpTmh_q,
    NULL,
    0,
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
    0
};

DataAttribute iedModel_Device1_DGEN1_GnOpSt_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_Device1_DGEN1_GnOpSt,
    (ModelNode*) &iedModel_Device1_DGEN1_GnOpSt_q,
    NULL,
    0,
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
    0
};

DataAttribute iedModel_Device1_DGEN1_OpTmsRs_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_Device1_DGEN1_OpTmsRs,
    (ModelNode*) &iedModel_Device1_DGEN1_OpTmsRs_q,
    NULL,
    0,
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
    0
};

DataAttribute iedModel_Device1_DGEN1_TotWh_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_Device1_DGEN1_TotWh,
    (ModelNode*) &iedModel_Device1_DGEN1_TotWh_q,
    (ModelNode*) &iedModel_Device1_DGEN1_TotWh_mag_f,
    0,
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
    0
};

DataAttribute iedModel_Device1_DSCH1_Mod_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Device1_DSCH1_Mod,
    (ModelNode*) &iedModel_Device1_DSCH1_Mod_t,
    NULL,
    0,
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
    0
};

DataAttribute iedModel_Device1_DSCH1_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_Device1_DSCH1_Beh,
    (ModelNode*) &iedModel_Device1_DSCH1_Beh_q,
    NULL,
    0,
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
    0
};

DataAttribute iedModel_Device1_DSCH1_Health_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_Device1_DSCH1_Health,
    (ModelNode*) &iedModel_Device1_DSCH1_Health_q,
    NULL,
    0,
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
    0
};

DataAttribute iedModel_Device1_DSCH1_NamPlt_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_Device1_DSCH1_NamPlt,
    (ModelNode*) &iedModel_Device1_DSCH1_NamPlt_swRev,
    NULL,
    0,
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
    0
};

DataAttribute iedModel_Device1_DSCH1_SchdSt_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_Device1_DSCH1_SchdSt,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdSt_q,
    NULL,
    0,
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
    0
};

DataObject iedModel_Device1_DSCH1_SchdCat = {
    DataObjectModelType,
    "SchdCat",
    (ModelNode*) &iedModel_Device1_DSCH1,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdTyp,
    NULL,
    0
};

DataObject iedModel_Device1_DSCH1_SchdTyp = {
    DataObjectModelType,
    "SchdTyp",
    (ModelNode*) &iedModel_Device1_DSCH1,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm,
    NULL,
    0
};

DataObject iedModel_Device1_DSCH1_SchdAbsTm = {
    DataObjectModelType,
    "SchdAbsTm",
    (ModelNode*) &iedModel_Device1_DSCH1,
    NULL,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_val,
    0
};

DataAttribute iedModel_Device1_DSCH1_SchdAbsTm_val = {
    DataAttributeModelType,
    "val",
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm,
    (ModelNode*) &iedModel_Device1_DSCH1_SchdAbsTm_time,
    NULL,
    255,
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
    NULL,
    255,
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
    0
};

DataAttribute iedModel_Device1_MMXU1_Mod_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Device1_MMXU1_Mod,
    (ModelNode*) &iedModel_Device1_MMXU1_Mod_t,
    NULL,
    0,
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
    0
};

DataAttribute iedModel_Device1_MMXU1_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_Device1_MMXU1_Beh,
    (ModelNode*) &iedModel_Device1_MMXU1_Beh_q,
    NULL,
    0,
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
    0
};

DataAttribute iedModel_Device1_MMXU1_Health_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_Device1_MMXU1_Health,
    (ModelNode*) &iedModel_Device1_MMXU1_Health_q,
    NULL,
    0,
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
    0
};

DataAttribute iedModel_Device1_MMXU1_NamPlt_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_Device1_MMXU1_NamPlt,
    (ModelNode*) &iedModel_Device1_MMXU1_NamPlt_swRev,
    NULL,
    0,
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
    0
};

DataAttribute iedModel_Device1_MMXU2_Mod_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Device1_MMXU2_Mod,
    (ModelNode*) &iedModel_Device1_MMXU2_Mod_t,
    NULL,
    0,
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
    0
};

DataAttribute iedModel_Device1_MMXU2_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_Device1_MMXU2_Beh,
    (ModelNode*) &iedModel_Device1_MMXU2_Beh_q,
    NULL,
    0,
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
    0
};

DataAttribute iedModel_Device1_MMXU2_Health_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_Device1_MMXU2_Health,
    (ModelNode*) &iedModel_Device1_MMXU2_Health_q,
    NULL,
    0,
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
    0
};

DataAttribute iedModel_Device1_MMXU2_NamPlt_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_Device1_MMXU2_NamPlt,
    (ModelNode*) &iedModel_Device1_MMXU2_NamPlt_swRev,
    NULL,
    0,
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
    0
};

DataAttribute iedModel_Device1_MMXU2_TotW_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_Device1_MMXU2_TotW,
    (ModelNode*) &iedModel_Device1_MMXU2_TotW_q,
    (ModelNode*) &iedModel_Device1_MMXU2_TotW_mag_f,
    0,
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
