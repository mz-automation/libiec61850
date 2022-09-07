/*
 * static_model.c
 *
 * automatically generated from complexModel.cid
 */
#include "static_model.h"

static void initializeValues();

extern DataSet iedModelds_Inverter_LLN0_dataset1;


extern DataSetEntry iedModelds_Inverter_LLN0_dataset1_fcda0;
extern DataSetEntry iedModelds_Inverter_LLN0_dataset1_fcda1;
extern DataSetEntry iedModelds_Inverter_LLN0_dataset1_fcda2;
extern DataSetEntry iedModelds_Inverter_LLN0_dataset1_fcda3;
extern DataSetEntry iedModelds_Inverter_LLN0_dataset1_fcda4;

DataSetEntry iedModelds_Inverter_LLN0_dataset1_fcda0 = {
  "Inverter",
  false,
  "LLN0$ST$Mod$q", 
  -1,
  NULL,
  NULL,
  &iedModelds_Inverter_LLN0_dataset1_fcda1
};

DataSetEntry iedModelds_Inverter_LLN0_dataset1_fcda1 = {
  "Battery",
  false,
  "LLN0$ST$Mod$q", 
  -1,
  NULL,
  NULL,
  &iedModelds_Inverter_LLN0_dataset1_fcda2
};

DataSetEntry iedModelds_Inverter_LLN0_dataset1_fcda2 = {
  "Inverter",
  false,
  "MMXU1$ST$Mod$q", 
  -1,
  NULL,
  NULL,
  &iedModelds_Inverter_LLN0_dataset1_fcda3
};

DataSetEntry iedModelds_Inverter_LLN0_dataset1_fcda3 = {
  "Inverter",
  false,
  "MMXU1$CF$Mod$ctlModel", 
  -1,
  NULL,
  NULL,
  &iedModelds_Inverter_LLN0_dataset1_fcda4
};

DataSetEntry iedModelds_Inverter_LLN0_dataset1_fcda4 = {
  "Inverter",
  false,
  "MMXU1$MX$TotW$mag", 
  -1,
  NULL,
  NULL,
  NULL
};

DataSet iedModelds_Inverter_LLN0_dataset1 = {
  "Inverter",
  "LLN0$dataset1",
  5,
  &iedModelds_Inverter_LLN0_dataset1_fcda0,
  NULL
};

LogicalDevice iedModel_Inverter = {
    LogicalDeviceModelType,
    "Inverter",
    (ModelNode*) &iedModel,
    (ModelNode*) &iedModel_Battery,
    (ModelNode*) &iedModel_Inverter_LLN0,
    NULL
};

LogicalNode iedModel_Inverter_LLN0 = {
    LogicalNodeModelType,
    "LLN0",
    (ModelNode*) &iedModel_Inverter,
    (ModelNode*) &iedModel_Inverter_LPHD1,
    (ModelNode*) &iedModel_Inverter_LLN0_Mod,
};

DataObject iedModel_Inverter_LLN0_Mod = {
    DataObjectModelType,
    "Mod",
    (ModelNode*) &iedModel_Inverter_LLN0,
    (ModelNode*) &iedModel_Inverter_LLN0_Beh,
    (ModelNode*) &iedModel_Inverter_LLN0_Mod_q,
    0,
    -1
};

DataAttribute iedModel_Inverter_LLN0_Mod_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Inverter_LLN0_Mod,
    (ModelNode*) &iedModel_Inverter_LLN0_Mod_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_LLN0_Mod_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Inverter_LLN0_Mod,
    (ModelNode*) &iedModel_Inverter_LLN0_Mod_ctlModel,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_Inverter_LLN0_Mod_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_Inverter_LLN0_Mod,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_Inverter_LLN0_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_Inverter_LLN0,
    (ModelNode*) &iedModel_Inverter_LLN0_Health,
    (ModelNode*) &iedModel_Inverter_LLN0_Beh_stVal,
    0,
    -1
};

DataAttribute iedModel_Inverter_LLN0_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_Inverter_LLN0_Beh,
    (ModelNode*) &iedModel_Inverter_LLN0_Beh_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_LLN0_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Inverter_LLN0_Beh,
    (ModelNode*) &iedModel_Inverter_LLN0_Beh_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_LLN0_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Inverter_LLN0_Beh,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_Inverter_LLN0_Health = {
    DataObjectModelType,
    "Health",
    (ModelNode*) &iedModel_Inverter_LLN0,
    (ModelNode*) &iedModel_Inverter_LLN0_NamPlt,
    (ModelNode*) &iedModel_Inverter_LLN0_Health_stVal,
    0,
    -1
};

DataAttribute iedModel_Inverter_LLN0_Health_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_Inverter_LLN0_Health,
    (ModelNode*) &iedModel_Inverter_LLN0_Health_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_LLN0_Health_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Inverter_LLN0_Health,
    (ModelNode*) &iedModel_Inverter_LLN0_Health_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_LLN0_Health_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Inverter_LLN0_Health,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_Inverter_LLN0_NamPlt = {
    DataObjectModelType,
    "NamPlt",
    (ModelNode*) &iedModel_Inverter_LLN0,
    NULL,
    (ModelNode*) &iedModel_Inverter_LLN0_NamPlt_vendor,
    0,
    -1
};

DataAttribute iedModel_Inverter_LLN0_NamPlt_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_Inverter_LLN0_NamPlt,
    (ModelNode*) &iedModel_Inverter_LLN0_NamPlt_swRev,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_Inverter_LLN0_NamPlt_swRev = {
    DataAttributeModelType,
    "swRev",
    (ModelNode*) &iedModel_Inverter_LLN0_NamPlt,
    (ModelNode*) &iedModel_Inverter_LLN0_NamPlt_d,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_Inverter_LLN0_NamPlt_d = {
    DataAttributeModelType,
    "d",
    (ModelNode*) &iedModel_Inverter_LLN0_NamPlt,
    (ModelNode*) &iedModel_Inverter_LLN0_NamPlt_configRev,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_Inverter_LLN0_NamPlt_configRev = {
    DataAttributeModelType,
    "configRev",
    (ModelNode*) &iedModel_Inverter_LLN0_NamPlt,
    (ModelNode*) &iedModel_Inverter_LLN0_NamPlt_ldNs,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_Inverter_LLN0_NamPlt_ldNs = {
    DataAttributeModelType,
    "ldNs",
    (ModelNode*) &iedModel_Inverter_LLN0_NamPlt,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_EX,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

LogicalNode iedModel_Inverter_LPHD1 = {
    LogicalNodeModelType,
    "LPHD1",
    (ModelNode*) &iedModel_Inverter,
    (ModelNode*) &iedModel_Inverter_ZINV1,
    (ModelNode*) &iedModel_Inverter_LPHD1_PhyNam,
};

DataObject iedModel_Inverter_LPHD1_PhyNam = {
    DataObjectModelType,
    "PhyNam",
    (ModelNode*) &iedModel_Inverter_LPHD1,
    (ModelNode*) &iedModel_Inverter_LPHD1_PhyHealth,
    (ModelNode*) &iedModel_Inverter_LPHD1_PhyNam_vendor,
    0,
    -1
};

DataAttribute iedModel_Inverter_LPHD1_PhyNam_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_Inverter_LPHD1_PhyNam,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_Inverter_LPHD1_PhyHealth = {
    DataObjectModelType,
    "PhyHealth",
    (ModelNode*) &iedModel_Inverter_LPHD1,
    (ModelNode*) &iedModel_Inverter_LPHD1_Proxy,
    (ModelNode*) &iedModel_Inverter_LPHD1_PhyHealth_stVal,
    0,
    -1
};

DataAttribute iedModel_Inverter_LPHD1_PhyHealth_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_Inverter_LPHD1_PhyHealth,
    (ModelNode*) &iedModel_Inverter_LPHD1_PhyHealth_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_LPHD1_PhyHealth_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Inverter_LPHD1_PhyHealth,
    (ModelNode*) &iedModel_Inverter_LPHD1_PhyHealth_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_LPHD1_PhyHealth_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Inverter_LPHD1_PhyHealth,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_Inverter_LPHD1_Proxy = {
    DataObjectModelType,
    "Proxy",
    (ModelNode*) &iedModel_Inverter_LPHD1,
    NULL,
    (ModelNode*) &iedModel_Inverter_LPHD1_Proxy_stVal,
    0,
    -1
};

DataAttribute iedModel_Inverter_LPHD1_Proxy_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_Inverter_LPHD1_Proxy,
    (ModelNode*) &iedModel_Inverter_LPHD1_Proxy_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_LPHD1_Proxy_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Inverter_LPHD1_Proxy,
    (ModelNode*) &iedModel_Inverter_LPHD1_Proxy_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_LPHD1_Proxy_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Inverter_LPHD1_Proxy,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

LogicalNode iedModel_Inverter_ZINV1 = {
    LogicalNodeModelType,
    "ZINV1",
    (ModelNode*) &iedModel_Inverter,
    (ModelNode*) &iedModel_Inverter_MMXU1,
    (ModelNode*) &iedModel_Inverter_ZINV1_Mod,
};

DataObject iedModel_Inverter_ZINV1_Mod = {
    DataObjectModelType,
    "Mod",
    (ModelNode*) &iedModel_Inverter_ZINV1,
    (ModelNode*) &iedModel_Inverter_ZINV1_Beh,
    (ModelNode*) &iedModel_Inverter_ZINV1_Mod_q,
    0,
    -1
};

DataAttribute iedModel_Inverter_ZINV1_Mod_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Inverter_ZINV1_Mod,
    (ModelNode*) &iedModel_Inverter_ZINV1_Mod_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_ZINV1_Mod_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Inverter_ZINV1_Mod,
    (ModelNode*) &iedModel_Inverter_ZINV1_Mod_ctlModel,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_Inverter_ZINV1_Mod_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_Inverter_ZINV1_Mod,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_Inverter_ZINV1_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_Inverter_ZINV1,
    (ModelNode*) &iedModel_Inverter_ZINV1_Health,
    (ModelNode*) &iedModel_Inverter_ZINV1_Beh_stVal,
    0,
    -1
};

DataAttribute iedModel_Inverter_ZINV1_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_Inverter_ZINV1_Beh,
    (ModelNode*) &iedModel_Inverter_ZINV1_Beh_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_ZINV1_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Inverter_ZINV1_Beh,
    (ModelNode*) &iedModel_Inverter_ZINV1_Beh_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_ZINV1_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Inverter_ZINV1_Beh,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_Inverter_ZINV1_Health = {
    DataObjectModelType,
    "Health",
    (ModelNode*) &iedModel_Inverter_ZINV1,
    (ModelNode*) &iedModel_Inverter_ZINV1_NamPlt,
    (ModelNode*) &iedModel_Inverter_ZINV1_Health_stVal,
    0,
    -1
};

DataAttribute iedModel_Inverter_ZINV1_Health_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_Inverter_ZINV1_Health,
    (ModelNode*) &iedModel_Inverter_ZINV1_Health_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_ZINV1_Health_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Inverter_ZINV1_Health,
    (ModelNode*) &iedModel_Inverter_ZINV1_Health_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_ZINV1_Health_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Inverter_ZINV1_Health,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_Inverter_ZINV1_NamPlt = {
    DataObjectModelType,
    "NamPlt",
    (ModelNode*) &iedModel_Inverter_ZINV1,
    (ModelNode*) &iedModel_Inverter_ZINV1_WRtg,
    (ModelNode*) &iedModel_Inverter_ZINV1_NamPlt_vendor,
    0,
    -1
};

DataAttribute iedModel_Inverter_ZINV1_NamPlt_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_Inverter_ZINV1_NamPlt,
    (ModelNode*) &iedModel_Inverter_ZINV1_NamPlt_swRev,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_Inverter_ZINV1_NamPlt_swRev = {
    DataAttributeModelType,
    "swRev",
    (ModelNode*) &iedModel_Inverter_ZINV1_NamPlt,
    (ModelNode*) &iedModel_Inverter_ZINV1_NamPlt_d,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_Inverter_ZINV1_NamPlt_d = {
    DataAttributeModelType,
    "d",
    (ModelNode*) &iedModel_Inverter_ZINV1_NamPlt,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_Inverter_ZINV1_WRtg = {
    DataObjectModelType,
    "WRtg",
    (ModelNode*) &iedModel_Inverter_ZINV1,
    (ModelNode*) &iedModel_Inverter_ZINV1_VarRtg,
    (ModelNode*) &iedModel_Inverter_ZINV1_WRtg_setMag,
    0,
    -1
};

DataAttribute iedModel_Inverter_ZINV1_WRtg_setMag = {
    DataAttributeModelType,
    "setMag",
    (ModelNode*) &iedModel_Inverter_ZINV1_WRtg,
    (ModelNode*) &iedModel_Inverter_ZINV1_WRtg_units,
    (ModelNode*) &iedModel_Inverter_ZINV1_WRtg_setMag_f,
    0,
    -1,
    IEC61850_FC_SP,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_Inverter_ZINV1_WRtg_setMag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_Inverter_ZINV1_WRtg_setMag,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_Inverter_ZINV1_WRtg_units = {
    DataAttributeModelType,
    "units",
    (ModelNode*) &iedModel_Inverter_ZINV1_WRtg,
    NULL,
    (ModelNode*) &iedModel_Inverter_ZINV1_WRtg_units_SIUnit,
    0,
    -1,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_Inverter_ZINV1_WRtg_units_SIUnit = {
    DataAttributeModelType,
    "SIUnit",
    (ModelNode*) &iedModel_Inverter_ZINV1_WRtg_units,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_Inverter_ZINV1_VarRtg = {
    DataObjectModelType,
    "VarRtg",
    (ModelNode*) &iedModel_Inverter_ZINV1,
    (ModelNode*) &iedModel_Inverter_ZINV1_ACTyp,
    (ModelNode*) &iedModel_Inverter_ZINV1_VarRtg_setMag,
    0,
    -1
};

DataAttribute iedModel_Inverter_ZINV1_VarRtg_setMag = {
    DataAttributeModelType,
    "setMag",
    (ModelNode*) &iedModel_Inverter_ZINV1_VarRtg,
    (ModelNode*) &iedModel_Inverter_ZINV1_VarRtg_units,
    (ModelNode*) &iedModel_Inverter_ZINV1_VarRtg_setMag_f,
    0,
    -1,
    IEC61850_FC_SP,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_Inverter_ZINV1_VarRtg_setMag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_Inverter_ZINV1_VarRtg_setMag,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_Inverter_ZINV1_VarRtg_units = {
    DataAttributeModelType,
    "units",
    (ModelNode*) &iedModel_Inverter_ZINV1_VarRtg,
    NULL,
    (ModelNode*) &iedModel_Inverter_ZINV1_VarRtg_units_SIUnit,
    0,
    -1,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_Inverter_ZINV1_VarRtg_units_SIUnit = {
    DataAttributeModelType,
    "SIUnit",
    (ModelNode*) &iedModel_Inverter_ZINV1_VarRtg_units,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_Inverter_ZINV1_ACTyp = {
    DataObjectModelType,
    "ACTyp",
    (ModelNode*) &iedModel_Inverter_ZINV1,
    (ModelNode*) &iedModel_Inverter_ZINV1_OutWSet,
    (ModelNode*) &iedModel_Inverter_ZINV1_ACTyp_setVal,
    0,
    -1
};

DataAttribute iedModel_Inverter_ZINV1_ACTyp_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_Inverter_ZINV1_ACTyp,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_SP,
    IEC61850_INT32,
    0,
    NULL,
    0};

DataObject iedModel_Inverter_ZINV1_OutWSet = {
    DataObjectModelType,
    "OutWSet",
    (ModelNode*) &iedModel_Inverter_ZINV1,
    (ModelNode*) &iedModel_Inverter_ZINV1_OutVarSet,
    (ModelNode*) &iedModel_Inverter_ZINV1_OutWSet_setMag,
    0,
    -1
};

DataAttribute iedModel_Inverter_ZINV1_OutWSet_setMag = {
    DataAttributeModelType,
    "setMag",
    (ModelNode*) &iedModel_Inverter_ZINV1_OutWSet,
    (ModelNode*) &iedModel_Inverter_ZINV1_OutWSet_units,
    (ModelNode*) &iedModel_Inverter_ZINV1_OutWSet_setMag_f,
    0,
    -1,
    IEC61850_FC_SP,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_Inverter_ZINV1_OutWSet_setMag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_Inverter_ZINV1_OutWSet_setMag,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_Inverter_ZINV1_OutWSet_units = {
    DataAttributeModelType,
    "units",
    (ModelNode*) &iedModel_Inverter_ZINV1_OutWSet,
    NULL,
    (ModelNode*) &iedModel_Inverter_ZINV1_OutWSet_units_SIUnit,
    0,
    -1,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_Inverter_ZINV1_OutWSet_units_SIUnit = {
    DataAttributeModelType,
    "SIUnit",
    (ModelNode*) &iedModel_Inverter_ZINV1_OutWSet_units,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_Inverter_ZINV1_OutVarSet = {
    DataObjectModelType,
    "OutVarSet",
    (ModelNode*) &iedModel_Inverter_ZINV1,
    NULL,
    (ModelNode*) &iedModel_Inverter_ZINV1_OutVarSet_setMag,
    0,
    -1
};

DataAttribute iedModel_Inverter_ZINV1_OutVarSet_setMag = {
    DataAttributeModelType,
    "setMag",
    (ModelNode*) &iedModel_Inverter_ZINV1_OutVarSet,
    (ModelNode*) &iedModel_Inverter_ZINV1_OutVarSet_units,
    (ModelNode*) &iedModel_Inverter_ZINV1_OutVarSet_setMag_f,
    0,
    -1,
    IEC61850_FC_SP,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_Inverter_ZINV1_OutVarSet_setMag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_Inverter_ZINV1_OutVarSet_setMag,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_Inverter_ZINV1_OutVarSet_units = {
    DataAttributeModelType,
    "units",
    (ModelNode*) &iedModel_Inverter_ZINV1_OutVarSet,
    NULL,
    (ModelNode*) &iedModel_Inverter_ZINV1_OutVarSet_units_SIUnit,
    0,
    -1,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_Inverter_ZINV1_OutVarSet_units_SIUnit = {
    DataAttributeModelType,
    "SIUnit",
    (ModelNode*) &iedModel_Inverter_ZINV1_OutVarSet_units,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

LogicalNode iedModel_Inverter_MMXU1 = {
    LogicalNodeModelType,
    "MMXU1",
    (ModelNode*) &iedModel_Inverter,
    NULL,
    (ModelNode*) &iedModel_Inverter_MMXU1_Mod,
};

DataObject iedModel_Inverter_MMXU1_Mod = {
    DataObjectModelType,
    "Mod",
    (ModelNode*) &iedModel_Inverter_MMXU1,
    (ModelNode*) &iedModel_Inverter_MMXU1_Beh,
    (ModelNode*) &iedModel_Inverter_MMXU1_Mod_q,
    0,
    -1
};

DataAttribute iedModel_Inverter_MMXU1_Mod_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Inverter_MMXU1_Mod,
    (ModelNode*) &iedModel_Inverter_MMXU1_Mod_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_Mod_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Inverter_MMXU1_Mod,
    (ModelNode*) &iedModel_Inverter_MMXU1_Mod_ctlModel,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_Mod_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_Inverter_MMXU1_Mod,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_Inverter_MMXU1_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_Inverter_MMXU1,
    (ModelNode*) &iedModel_Inverter_MMXU1_Health,
    (ModelNode*) &iedModel_Inverter_MMXU1_Beh_stVal,
    0,
    -1
};

DataAttribute iedModel_Inverter_MMXU1_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_Inverter_MMXU1_Beh,
    (ModelNode*) &iedModel_Inverter_MMXU1_Beh_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Inverter_MMXU1_Beh,
    (ModelNode*) &iedModel_Inverter_MMXU1_Beh_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Inverter_MMXU1_Beh,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_Inverter_MMXU1_Health = {
    DataObjectModelType,
    "Health",
    (ModelNode*) &iedModel_Inverter_MMXU1,
    (ModelNode*) &iedModel_Inverter_MMXU1_NamPlt,
    (ModelNode*) &iedModel_Inverter_MMXU1_Health_stVal,
    0,
    -1
};

DataAttribute iedModel_Inverter_MMXU1_Health_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_Inverter_MMXU1_Health,
    (ModelNode*) &iedModel_Inverter_MMXU1_Health_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_Health_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Inverter_MMXU1_Health,
    (ModelNode*) &iedModel_Inverter_MMXU1_Health_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_Health_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Inverter_MMXU1_Health,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_Inverter_MMXU1_NamPlt = {
    DataObjectModelType,
    "NamPlt",
    (ModelNode*) &iedModel_Inverter_MMXU1,
    (ModelNode*) &iedModel_Inverter_MMXU1_TotW,
    (ModelNode*) &iedModel_Inverter_MMXU1_NamPlt_vendor,
    0,
    -1
};

DataAttribute iedModel_Inverter_MMXU1_NamPlt_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_Inverter_MMXU1_NamPlt,
    (ModelNode*) &iedModel_Inverter_MMXU1_NamPlt_swRev,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_NamPlt_swRev = {
    DataAttributeModelType,
    "swRev",
    (ModelNode*) &iedModel_Inverter_MMXU1_NamPlt,
    (ModelNode*) &iedModel_Inverter_MMXU1_NamPlt_d,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_NamPlt_d = {
    DataAttributeModelType,
    "d",
    (ModelNode*) &iedModel_Inverter_MMXU1_NamPlt,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_Inverter_MMXU1_TotW = {
    DataObjectModelType,
    "TotW",
    (ModelNode*) &iedModel_Inverter_MMXU1,
    (ModelNode*) &iedModel_Inverter_MMXU1_TotVAr,
    (ModelNode*) &iedModel_Inverter_MMXU1_TotW_mag,
    0,
    -1
};

DataAttribute iedModel_Inverter_MMXU1_TotW_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_Inverter_MMXU1_TotW,
    (ModelNode*) &iedModel_Inverter_MMXU1_TotW_q,
    (ModelNode*) &iedModel_Inverter_MMXU1_TotW_mag_f,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_TotW_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_Inverter_MMXU1_TotW_mag,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_TotW_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Inverter_MMXU1_TotW,
    (ModelNode*) &iedModel_Inverter_MMXU1_TotW_t,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_TotW_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Inverter_MMXU1_TotW,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_Inverter_MMXU1_TotVAr = {
    DataObjectModelType,
    "TotVAr",
    (ModelNode*) &iedModel_Inverter_MMXU1,
    (ModelNode*) &iedModel_Inverter_MMXU1_TotVA,
    (ModelNode*) &iedModel_Inverter_MMXU1_TotVAr_mag,
    0,
    -1
};

DataAttribute iedModel_Inverter_MMXU1_TotVAr_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_Inverter_MMXU1_TotVAr,
    (ModelNode*) &iedModel_Inverter_MMXU1_TotVAr_q,
    (ModelNode*) &iedModel_Inverter_MMXU1_TotVAr_mag_f,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_TotVAr_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_Inverter_MMXU1_TotVAr_mag,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_TotVAr_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Inverter_MMXU1_TotVAr,
    (ModelNode*) &iedModel_Inverter_MMXU1_TotVAr_t,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_TotVAr_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Inverter_MMXU1_TotVAr,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_Inverter_MMXU1_TotVA = {
    DataObjectModelType,
    "TotVA",
    (ModelNode*) &iedModel_Inverter_MMXU1,
    (ModelNode*) &iedModel_Inverter_MMXU1_Hz,
    (ModelNode*) &iedModel_Inverter_MMXU1_TotVA_mag,
    0,
    -1
};

DataAttribute iedModel_Inverter_MMXU1_TotVA_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_Inverter_MMXU1_TotVA,
    (ModelNode*) &iedModel_Inverter_MMXU1_TotVA_q,
    (ModelNode*) &iedModel_Inverter_MMXU1_TotVA_mag_f,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_TotVA_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_Inverter_MMXU1_TotVA_mag,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_TotVA_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Inverter_MMXU1_TotVA,
    (ModelNode*) &iedModel_Inverter_MMXU1_TotVA_t,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_TotVA_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Inverter_MMXU1_TotVA,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_Inverter_MMXU1_Hz = {
    DataObjectModelType,
    "Hz",
    (ModelNode*) &iedModel_Inverter_MMXU1,
    (ModelNode*) &iedModel_Inverter_MMXU1_PhV,
    (ModelNode*) &iedModel_Inverter_MMXU1_Hz_mag,
    0,
    -1
};

DataAttribute iedModel_Inverter_MMXU1_Hz_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_Inverter_MMXU1_Hz,
    (ModelNode*) &iedModel_Inverter_MMXU1_Hz_q,
    (ModelNode*) &iedModel_Inverter_MMXU1_Hz_mag_f,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_Hz_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_Inverter_MMXU1_Hz_mag,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_Hz_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Inverter_MMXU1_Hz,
    (ModelNode*) &iedModel_Inverter_MMXU1_Hz_t,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_Hz_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Inverter_MMXU1_Hz,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_Inverter_MMXU1_PhV = {
    DataObjectModelType,
    "PhV",
    (ModelNode*) &iedModel_Inverter_MMXU1,
    (ModelNode*) &iedModel_Inverter_MMXU1_A,
    (ModelNode*) &iedModel_Inverter_MMXU1_PhV_phsA,
    0,
    -1
};

DataObject iedModel_Inverter_MMXU1_PhV_phsA = {
    DataObjectModelType,
    "phsA",
    (ModelNode*) &iedModel_Inverter_MMXU1_PhV,
    (ModelNode*) &iedModel_Inverter_MMXU1_PhV_phsB,
    (ModelNode*) &iedModel_Inverter_MMXU1_PhV_phsA_cVal,
    0,
    -1
};

DataAttribute iedModel_Inverter_MMXU1_PhV_phsA_cVal = {
    DataAttributeModelType,
    "cVal",
    (ModelNode*) &iedModel_Inverter_MMXU1_PhV_phsA,
    (ModelNode*) &iedModel_Inverter_MMXU1_PhV_phsA_q,
    (ModelNode*) &iedModel_Inverter_MMXU1_PhV_phsA_cVal_mag,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_PhV_phsA_cVal_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_Inverter_MMXU1_PhV_phsA_cVal,
    NULL,
    (ModelNode*) &iedModel_Inverter_MMXU1_PhV_phsA_cVal_mag_f,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_PhV_phsA_cVal_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_Inverter_MMXU1_PhV_phsA_cVal_mag,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_PhV_phsA_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Inverter_MMXU1_PhV_phsA,
    (ModelNode*) &iedModel_Inverter_MMXU1_PhV_phsA_t,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_PhV_phsA_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Inverter_MMXU1_PhV_phsA,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_Inverter_MMXU1_PhV_phsB = {
    DataObjectModelType,
    "phsB",
    (ModelNode*) &iedModel_Inverter_MMXU1_PhV,
    (ModelNode*) &iedModel_Inverter_MMXU1_PhV_phsC,
    (ModelNode*) &iedModel_Inverter_MMXU1_PhV_phsB_cVal,
    0,
    -1
};

DataAttribute iedModel_Inverter_MMXU1_PhV_phsB_cVal = {
    DataAttributeModelType,
    "cVal",
    (ModelNode*) &iedModel_Inverter_MMXU1_PhV_phsB,
    (ModelNode*) &iedModel_Inverter_MMXU1_PhV_phsB_q,
    (ModelNode*) &iedModel_Inverter_MMXU1_PhV_phsB_cVal_mag,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_PhV_phsB_cVal_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_Inverter_MMXU1_PhV_phsB_cVal,
    NULL,
    (ModelNode*) &iedModel_Inverter_MMXU1_PhV_phsB_cVal_mag_f,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_PhV_phsB_cVal_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_Inverter_MMXU1_PhV_phsB_cVal_mag,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_PhV_phsB_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Inverter_MMXU1_PhV_phsB,
    (ModelNode*) &iedModel_Inverter_MMXU1_PhV_phsB_t,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_PhV_phsB_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Inverter_MMXU1_PhV_phsB,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_Inverter_MMXU1_PhV_phsC = {
    DataObjectModelType,
    "phsC",
    (ModelNode*) &iedModel_Inverter_MMXU1_PhV,
    (ModelNode*) &iedModel_Inverter_MMXU1_PhV_neut,
    (ModelNode*) &iedModel_Inverter_MMXU1_PhV_phsC_cVal,
    0,
    -1
};

DataAttribute iedModel_Inverter_MMXU1_PhV_phsC_cVal = {
    DataAttributeModelType,
    "cVal",
    (ModelNode*) &iedModel_Inverter_MMXU1_PhV_phsC,
    (ModelNode*) &iedModel_Inverter_MMXU1_PhV_phsC_q,
    (ModelNode*) &iedModel_Inverter_MMXU1_PhV_phsC_cVal_mag,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_PhV_phsC_cVal_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_Inverter_MMXU1_PhV_phsC_cVal,
    NULL,
    (ModelNode*) &iedModel_Inverter_MMXU1_PhV_phsC_cVal_mag_f,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_PhV_phsC_cVal_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_Inverter_MMXU1_PhV_phsC_cVal_mag,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_PhV_phsC_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Inverter_MMXU1_PhV_phsC,
    (ModelNode*) &iedModel_Inverter_MMXU1_PhV_phsC_t,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_PhV_phsC_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Inverter_MMXU1_PhV_phsC,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_Inverter_MMXU1_PhV_neut = {
    DataObjectModelType,
    "neut",
    (ModelNode*) &iedModel_Inverter_MMXU1_PhV,
    NULL,
    (ModelNode*) &iedModel_Inverter_MMXU1_PhV_neut_cVal,
    0,
    -1
};

DataAttribute iedModel_Inverter_MMXU1_PhV_neut_cVal = {
    DataAttributeModelType,
    "cVal",
    (ModelNode*) &iedModel_Inverter_MMXU1_PhV_neut,
    (ModelNode*) &iedModel_Inverter_MMXU1_PhV_neut_q,
    (ModelNode*) &iedModel_Inverter_MMXU1_PhV_neut_cVal_mag,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_PhV_neut_cVal_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_Inverter_MMXU1_PhV_neut_cVal,
    NULL,
    (ModelNode*) &iedModel_Inverter_MMXU1_PhV_neut_cVal_mag_f,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_PhV_neut_cVal_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_Inverter_MMXU1_PhV_neut_cVal_mag,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_PhV_neut_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Inverter_MMXU1_PhV_neut,
    (ModelNode*) &iedModel_Inverter_MMXU1_PhV_neut_t,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_PhV_neut_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Inverter_MMXU1_PhV_neut,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_Inverter_MMXU1_A = {
    DataObjectModelType,
    "A",
    (ModelNode*) &iedModel_Inverter_MMXU1,
    (ModelNode*) &iedModel_Inverter_MMXU1_W,
    (ModelNode*) &iedModel_Inverter_MMXU1_A_phsA,
    0,
    -1
};

DataObject iedModel_Inverter_MMXU1_A_phsA = {
    DataObjectModelType,
    "phsA",
    (ModelNode*) &iedModel_Inverter_MMXU1_A,
    (ModelNode*) &iedModel_Inverter_MMXU1_A_phsB,
    (ModelNode*) &iedModel_Inverter_MMXU1_A_phsA_cVal,
    0,
    -1
};

DataAttribute iedModel_Inverter_MMXU1_A_phsA_cVal = {
    DataAttributeModelType,
    "cVal",
    (ModelNode*) &iedModel_Inverter_MMXU1_A_phsA,
    (ModelNode*) &iedModel_Inverter_MMXU1_A_phsA_q,
    (ModelNode*) &iedModel_Inverter_MMXU1_A_phsA_cVal_mag,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_A_phsA_cVal_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_Inverter_MMXU1_A_phsA_cVal,
    NULL,
    (ModelNode*) &iedModel_Inverter_MMXU1_A_phsA_cVal_mag_f,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_A_phsA_cVal_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_Inverter_MMXU1_A_phsA_cVal_mag,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_A_phsA_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Inverter_MMXU1_A_phsA,
    (ModelNode*) &iedModel_Inverter_MMXU1_A_phsA_t,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_A_phsA_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Inverter_MMXU1_A_phsA,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_Inverter_MMXU1_A_phsB = {
    DataObjectModelType,
    "phsB",
    (ModelNode*) &iedModel_Inverter_MMXU1_A,
    (ModelNode*) &iedModel_Inverter_MMXU1_A_phsC,
    (ModelNode*) &iedModel_Inverter_MMXU1_A_phsB_cVal,
    0,
    -1
};

DataAttribute iedModel_Inverter_MMXU1_A_phsB_cVal = {
    DataAttributeModelType,
    "cVal",
    (ModelNode*) &iedModel_Inverter_MMXU1_A_phsB,
    (ModelNode*) &iedModel_Inverter_MMXU1_A_phsB_q,
    (ModelNode*) &iedModel_Inverter_MMXU1_A_phsB_cVal_mag,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_A_phsB_cVal_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_Inverter_MMXU1_A_phsB_cVal,
    NULL,
    (ModelNode*) &iedModel_Inverter_MMXU1_A_phsB_cVal_mag_f,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_A_phsB_cVal_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_Inverter_MMXU1_A_phsB_cVal_mag,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_A_phsB_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Inverter_MMXU1_A_phsB,
    (ModelNode*) &iedModel_Inverter_MMXU1_A_phsB_t,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_A_phsB_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Inverter_MMXU1_A_phsB,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_Inverter_MMXU1_A_phsC = {
    DataObjectModelType,
    "phsC",
    (ModelNode*) &iedModel_Inverter_MMXU1_A,
    (ModelNode*) &iedModel_Inverter_MMXU1_A_neut,
    (ModelNode*) &iedModel_Inverter_MMXU1_A_phsC_cVal,
    0,
    -1
};

DataAttribute iedModel_Inverter_MMXU1_A_phsC_cVal = {
    DataAttributeModelType,
    "cVal",
    (ModelNode*) &iedModel_Inverter_MMXU1_A_phsC,
    (ModelNode*) &iedModel_Inverter_MMXU1_A_phsC_q,
    (ModelNode*) &iedModel_Inverter_MMXU1_A_phsC_cVal_mag,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_A_phsC_cVal_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_Inverter_MMXU1_A_phsC_cVal,
    NULL,
    (ModelNode*) &iedModel_Inverter_MMXU1_A_phsC_cVal_mag_f,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_A_phsC_cVal_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_Inverter_MMXU1_A_phsC_cVal_mag,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_A_phsC_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Inverter_MMXU1_A_phsC,
    (ModelNode*) &iedModel_Inverter_MMXU1_A_phsC_t,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_A_phsC_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Inverter_MMXU1_A_phsC,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_Inverter_MMXU1_A_neut = {
    DataObjectModelType,
    "neut",
    (ModelNode*) &iedModel_Inverter_MMXU1_A,
    NULL,
    (ModelNode*) &iedModel_Inverter_MMXU1_A_neut_cVal,
    0,
    -1
};

DataAttribute iedModel_Inverter_MMXU1_A_neut_cVal = {
    DataAttributeModelType,
    "cVal",
    (ModelNode*) &iedModel_Inverter_MMXU1_A_neut,
    (ModelNode*) &iedModel_Inverter_MMXU1_A_neut_q,
    (ModelNode*) &iedModel_Inverter_MMXU1_A_neut_cVal_mag,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_A_neut_cVal_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_Inverter_MMXU1_A_neut_cVal,
    NULL,
    (ModelNode*) &iedModel_Inverter_MMXU1_A_neut_cVal_mag_f,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_A_neut_cVal_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_Inverter_MMXU1_A_neut_cVal_mag,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_A_neut_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Inverter_MMXU1_A_neut,
    (ModelNode*) &iedModel_Inverter_MMXU1_A_neut_t,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_A_neut_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Inverter_MMXU1_A_neut,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_Inverter_MMXU1_W = {
    DataObjectModelType,
    "W",
    (ModelNode*) &iedModel_Inverter_MMXU1,
    NULL,
    (ModelNode*) &iedModel_Inverter_MMXU1_W_phsA,
    0,
    -1
};

DataObject iedModel_Inverter_MMXU1_W_phsA = {
    DataObjectModelType,
    "phsA",
    (ModelNode*) &iedModel_Inverter_MMXU1_W,
    (ModelNode*) &iedModel_Inverter_MMXU1_W_phsB,
    (ModelNode*) &iedModel_Inverter_MMXU1_W_phsA_cVal,
    0,
    -1
};

DataAttribute iedModel_Inverter_MMXU1_W_phsA_cVal = {
    DataAttributeModelType,
    "cVal",
    (ModelNode*) &iedModel_Inverter_MMXU1_W_phsA,
    (ModelNode*) &iedModel_Inverter_MMXU1_W_phsA_q,
    (ModelNode*) &iedModel_Inverter_MMXU1_W_phsA_cVal_mag,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_W_phsA_cVal_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_Inverter_MMXU1_W_phsA_cVal,
    NULL,
    (ModelNode*) &iedModel_Inverter_MMXU1_W_phsA_cVal_mag_f,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_W_phsA_cVal_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_Inverter_MMXU1_W_phsA_cVal_mag,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_W_phsA_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Inverter_MMXU1_W_phsA,
    (ModelNode*) &iedModel_Inverter_MMXU1_W_phsA_t,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_W_phsA_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Inverter_MMXU1_W_phsA,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_Inverter_MMXU1_W_phsB = {
    DataObjectModelType,
    "phsB",
    (ModelNode*) &iedModel_Inverter_MMXU1_W,
    (ModelNode*) &iedModel_Inverter_MMXU1_W_phsC,
    (ModelNode*) &iedModel_Inverter_MMXU1_W_phsB_cVal,
    0,
    -1
};

DataAttribute iedModel_Inverter_MMXU1_W_phsB_cVal = {
    DataAttributeModelType,
    "cVal",
    (ModelNode*) &iedModel_Inverter_MMXU1_W_phsB,
    (ModelNode*) &iedModel_Inverter_MMXU1_W_phsB_q,
    (ModelNode*) &iedModel_Inverter_MMXU1_W_phsB_cVal_mag,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_W_phsB_cVal_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_Inverter_MMXU1_W_phsB_cVal,
    NULL,
    (ModelNode*) &iedModel_Inverter_MMXU1_W_phsB_cVal_mag_f,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_W_phsB_cVal_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_Inverter_MMXU1_W_phsB_cVal_mag,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_W_phsB_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Inverter_MMXU1_W_phsB,
    (ModelNode*) &iedModel_Inverter_MMXU1_W_phsB_t,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_W_phsB_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Inverter_MMXU1_W_phsB,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_Inverter_MMXU1_W_phsC = {
    DataObjectModelType,
    "phsC",
    (ModelNode*) &iedModel_Inverter_MMXU1_W,
    NULL,
    (ModelNode*) &iedModel_Inverter_MMXU1_W_phsC_cVal,
    0,
    -1
};

DataAttribute iedModel_Inverter_MMXU1_W_phsC_cVal = {
    DataAttributeModelType,
    "cVal",
    (ModelNode*) &iedModel_Inverter_MMXU1_W_phsC,
    (ModelNode*) &iedModel_Inverter_MMXU1_W_phsC_q,
    (ModelNode*) &iedModel_Inverter_MMXU1_W_phsC_cVal_mag,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_W_phsC_cVal_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_Inverter_MMXU1_W_phsC_cVal,
    NULL,
    (ModelNode*) &iedModel_Inverter_MMXU1_W_phsC_cVal_mag_f,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_W_phsC_cVal_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_Inverter_MMXU1_W_phsC_cVal_mag,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_W_phsC_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Inverter_MMXU1_W_phsC,
    (ModelNode*) &iedModel_Inverter_MMXU1_W_phsC_t,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Inverter_MMXU1_W_phsC_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Inverter_MMXU1_W_phsC,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};


LogicalDevice iedModel_Battery = {
    LogicalDeviceModelType,
    "Battery",
    (ModelNode*) &iedModel,
    (ModelNode*) &iedModel_Physical_Measurements,
    (ModelNode*) &iedModel_Battery_LLN0,
    NULL
};

LogicalNode iedModel_Battery_LLN0 = {
    LogicalNodeModelType,
    "LLN0",
    (ModelNode*) &iedModel_Battery,
    (ModelNode*) &iedModel_Battery_LPHD1,
    (ModelNode*) &iedModel_Battery_LLN0_Mod,
};

DataObject iedModel_Battery_LLN0_Mod = {
    DataObjectModelType,
    "Mod",
    (ModelNode*) &iedModel_Battery_LLN0,
    (ModelNode*) &iedModel_Battery_LLN0_Beh,
    (ModelNode*) &iedModel_Battery_LLN0_Mod_q,
    0,
    -1
};

DataAttribute iedModel_Battery_LLN0_Mod_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Battery_LLN0_Mod,
    (ModelNode*) &iedModel_Battery_LLN0_Mod_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Battery_LLN0_Mod_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Battery_LLN0_Mod,
    (ModelNode*) &iedModel_Battery_LLN0_Mod_ctlModel,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_Battery_LLN0_Mod_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_Battery_LLN0_Mod,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_Battery_LLN0_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_Battery_LLN0,
    (ModelNode*) &iedModel_Battery_LLN0_Health,
    (ModelNode*) &iedModel_Battery_LLN0_Beh_stVal,
    0,
    -1
};

DataAttribute iedModel_Battery_LLN0_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_Battery_LLN0_Beh,
    (ModelNode*) &iedModel_Battery_LLN0_Beh_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Battery_LLN0_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Battery_LLN0_Beh,
    (ModelNode*) &iedModel_Battery_LLN0_Beh_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Battery_LLN0_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Battery_LLN0_Beh,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_Battery_LLN0_Health = {
    DataObjectModelType,
    "Health",
    (ModelNode*) &iedModel_Battery_LLN0,
    (ModelNode*) &iedModel_Battery_LLN0_NamPlt,
    (ModelNode*) &iedModel_Battery_LLN0_Health_stVal,
    0,
    -1
};

DataAttribute iedModel_Battery_LLN0_Health_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_Battery_LLN0_Health,
    (ModelNode*) &iedModel_Battery_LLN0_Health_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Battery_LLN0_Health_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Battery_LLN0_Health,
    (ModelNode*) &iedModel_Battery_LLN0_Health_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Battery_LLN0_Health_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Battery_LLN0_Health,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_Battery_LLN0_NamPlt = {
    DataObjectModelType,
    "NamPlt",
    (ModelNode*) &iedModel_Battery_LLN0,
    NULL,
    (ModelNode*) &iedModel_Battery_LLN0_NamPlt_vendor,
    0,
    -1
};

DataAttribute iedModel_Battery_LLN0_NamPlt_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_Battery_LLN0_NamPlt,
    (ModelNode*) &iedModel_Battery_LLN0_NamPlt_swRev,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_Battery_LLN0_NamPlt_swRev = {
    DataAttributeModelType,
    "swRev",
    (ModelNode*) &iedModel_Battery_LLN0_NamPlt,
    (ModelNode*) &iedModel_Battery_LLN0_NamPlt_d,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_Battery_LLN0_NamPlt_d = {
    DataAttributeModelType,
    "d",
    (ModelNode*) &iedModel_Battery_LLN0_NamPlt,
    (ModelNode*) &iedModel_Battery_LLN0_NamPlt_configRev,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_Battery_LLN0_NamPlt_configRev = {
    DataAttributeModelType,
    "configRev",
    (ModelNode*) &iedModel_Battery_LLN0_NamPlt,
    (ModelNode*) &iedModel_Battery_LLN0_NamPlt_ldNs,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_Battery_LLN0_NamPlt_ldNs = {
    DataAttributeModelType,
    "ldNs",
    (ModelNode*) &iedModel_Battery_LLN0_NamPlt,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_EX,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

LogicalNode iedModel_Battery_LPHD1 = {
    LogicalNodeModelType,
    "LPHD1",
    (ModelNode*) &iedModel_Battery,
    (ModelNode*) &iedModel_Battery_ZBAT1,
    (ModelNode*) &iedModel_Battery_LPHD1_PhyNam,
};

DataObject iedModel_Battery_LPHD1_PhyNam = {
    DataObjectModelType,
    "PhyNam",
    (ModelNode*) &iedModel_Battery_LPHD1,
    (ModelNode*) &iedModel_Battery_LPHD1_PhyHealth,
    (ModelNode*) &iedModel_Battery_LPHD1_PhyNam_vendor,
    0,
    -1
};

DataAttribute iedModel_Battery_LPHD1_PhyNam_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_Battery_LPHD1_PhyNam,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_Battery_LPHD1_PhyHealth = {
    DataObjectModelType,
    "PhyHealth",
    (ModelNode*) &iedModel_Battery_LPHD1,
    (ModelNode*) &iedModel_Battery_LPHD1_Proxy,
    (ModelNode*) &iedModel_Battery_LPHD1_PhyHealth_stVal,
    0,
    -1
};

DataAttribute iedModel_Battery_LPHD1_PhyHealth_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_Battery_LPHD1_PhyHealth,
    (ModelNode*) &iedModel_Battery_LPHD1_PhyHealth_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Battery_LPHD1_PhyHealth_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Battery_LPHD1_PhyHealth,
    (ModelNode*) &iedModel_Battery_LPHD1_PhyHealth_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Battery_LPHD1_PhyHealth_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Battery_LPHD1_PhyHealth,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_Battery_LPHD1_Proxy = {
    DataObjectModelType,
    "Proxy",
    (ModelNode*) &iedModel_Battery_LPHD1,
    NULL,
    (ModelNode*) &iedModel_Battery_LPHD1_Proxy_stVal,
    0,
    -1
};

DataAttribute iedModel_Battery_LPHD1_Proxy_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_Battery_LPHD1_Proxy,
    (ModelNode*) &iedModel_Battery_LPHD1_Proxy_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Battery_LPHD1_Proxy_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Battery_LPHD1_Proxy,
    (ModelNode*) &iedModel_Battery_LPHD1_Proxy_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Battery_LPHD1_Proxy_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Battery_LPHD1_Proxy,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

LogicalNode iedModel_Battery_ZBAT1 = {
    LogicalNodeModelType,
    "ZBAT1",
    (ModelNode*) &iedModel_Battery,
    (ModelNode*) &iedModel_Battery_ZBTC1,
    (ModelNode*) &iedModel_Battery_ZBAT1_Mod,
};

DataObject iedModel_Battery_ZBAT1_Mod = {
    DataObjectModelType,
    "Mod",
    (ModelNode*) &iedModel_Battery_ZBAT1,
    (ModelNode*) &iedModel_Battery_ZBAT1_Beh,
    (ModelNode*) &iedModel_Battery_ZBAT1_Mod_q,
    0,
    -1
};

DataAttribute iedModel_Battery_ZBAT1_Mod_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Battery_ZBAT1_Mod,
    (ModelNode*) &iedModel_Battery_ZBAT1_Mod_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Battery_ZBAT1_Mod_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Battery_ZBAT1_Mod,
    (ModelNode*) &iedModel_Battery_ZBAT1_Mod_ctlModel,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_Battery_ZBAT1_Mod_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_Battery_ZBAT1_Mod,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_Battery_ZBAT1_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_Battery_ZBAT1,
    (ModelNode*) &iedModel_Battery_ZBAT1_Health,
    (ModelNode*) &iedModel_Battery_ZBAT1_Beh_stVal,
    0,
    -1
};

DataAttribute iedModel_Battery_ZBAT1_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_Battery_ZBAT1_Beh,
    (ModelNode*) &iedModel_Battery_ZBAT1_Beh_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Battery_ZBAT1_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Battery_ZBAT1_Beh,
    (ModelNode*) &iedModel_Battery_ZBAT1_Beh_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Battery_ZBAT1_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Battery_ZBAT1_Beh,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_Battery_ZBAT1_Health = {
    DataObjectModelType,
    "Health",
    (ModelNode*) &iedModel_Battery_ZBAT1,
    (ModelNode*) &iedModel_Battery_ZBAT1_NamPlt,
    (ModelNode*) &iedModel_Battery_ZBAT1_Health_stVal,
    0,
    -1
};

DataAttribute iedModel_Battery_ZBAT1_Health_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_Battery_ZBAT1_Health,
    (ModelNode*) &iedModel_Battery_ZBAT1_Health_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Battery_ZBAT1_Health_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Battery_ZBAT1_Health,
    (ModelNode*) &iedModel_Battery_ZBAT1_Health_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Battery_ZBAT1_Health_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Battery_ZBAT1_Health,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_Battery_ZBAT1_NamPlt = {
    DataObjectModelType,
    "NamPlt",
    (ModelNode*) &iedModel_Battery_ZBAT1,
    (ModelNode*) &iedModel_Battery_ZBAT1_Vol,
    (ModelNode*) &iedModel_Battery_ZBAT1_NamPlt_vendor,
    0,
    -1
};

DataAttribute iedModel_Battery_ZBAT1_NamPlt_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_Battery_ZBAT1_NamPlt,
    (ModelNode*) &iedModel_Battery_ZBAT1_NamPlt_swRev,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_Battery_ZBAT1_NamPlt_swRev = {
    DataAttributeModelType,
    "swRev",
    (ModelNode*) &iedModel_Battery_ZBAT1_NamPlt,
    (ModelNode*) &iedModel_Battery_ZBAT1_NamPlt_d,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_Battery_ZBAT1_NamPlt_d = {
    DataAttributeModelType,
    "d",
    (ModelNode*) &iedModel_Battery_ZBAT1_NamPlt,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_Battery_ZBAT1_Vol = {
    DataObjectModelType,
    "Vol",
    (ModelNode*) &iedModel_Battery_ZBAT1,
    (ModelNode*) &iedModel_Battery_ZBAT1_Amp,
    (ModelNode*) &iedModel_Battery_ZBAT1_Vol_mag,
    0,
    -1
};

DataAttribute iedModel_Battery_ZBAT1_Vol_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_Battery_ZBAT1_Vol,
    (ModelNode*) &iedModel_Battery_ZBAT1_Vol_q,
    (ModelNode*) &iedModel_Battery_ZBAT1_Vol_mag_f,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Battery_ZBAT1_Vol_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_Battery_ZBAT1_Vol_mag,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Battery_ZBAT1_Vol_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Battery_ZBAT1_Vol,
    (ModelNode*) &iedModel_Battery_ZBAT1_Vol_t,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Battery_ZBAT1_Vol_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Battery_ZBAT1_Vol,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_Battery_ZBAT1_Amp = {
    DataObjectModelType,
    "Amp",
    (ModelNode*) &iedModel_Battery_ZBAT1,
    NULL,
    (ModelNode*) &iedModel_Battery_ZBAT1_Amp_mag,
    0,
    -1
};

DataAttribute iedModel_Battery_ZBAT1_Amp_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_Battery_ZBAT1_Amp,
    (ModelNode*) &iedModel_Battery_ZBAT1_Amp_q,
    (ModelNode*) &iedModel_Battery_ZBAT1_Amp_mag_f,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Battery_ZBAT1_Amp_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_Battery_ZBAT1_Amp_mag,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Battery_ZBAT1_Amp_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Battery_ZBAT1_Amp,
    (ModelNode*) &iedModel_Battery_ZBAT1_Amp_t,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Battery_ZBAT1_Amp_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Battery_ZBAT1_Amp,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

LogicalNode iedModel_Battery_ZBTC1 = {
    LogicalNodeModelType,
    "ZBTC1",
    (ModelNode*) &iedModel_Battery,
    NULL,
    (ModelNode*) &iedModel_Battery_ZBTC1_Mod,
};

DataObject iedModel_Battery_ZBTC1_Mod = {
    DataObjectModelType,
    "Mod",
    (ModelNode*) &iedModel_Battery_ZBTC1,
    (ModelNode*) &iedModel_Battery_ZBTC1_Beh,
    (ModelNode*) &iedModel_Battery_ZBTC1_Mod_q,
    0,
    -1
};

DataAttribute iedModel_Battery_ZBTC1_Mod_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Battery_ZBTC1_Mod,
    (ModelNode*) &iedModel_Battery_ZBTC1_Mod_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Battery_ZBTC1_Mod_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Battery_ZBTC1_Mod,
    (ModelNode*) &iedModel_Battery_ZBTC1_Mod_ctlModel,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_Battery_ZBTC1_Mod_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_Battery_ZBTC1_Mod,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_Battery_ZBTC1_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_Battery_ZBTC1,
    (ModelNode*) &iedModel_Battery_ZBTC1_Health,
    (ModelNode*) &iedModel_Battery_ZBTC1_Beh_stVal,
    0,
    -1
};

DataAttribute iedModel_Battery_ZBTC1_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_Battery_ZBTC1_Beh,
    (ModelNode*) &iedModel_Battery_ZBTC1_Beh_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Battery_ZBTC1_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Battery_ZBTC1_Beh,
    (ModelNode*) &iedModel_Battery_ZBTC1_Beh_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Battery_ZBTC1_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Battery_ZBTC1_Beh,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_Battery_ZBTC1_Health = {
    DataObjectModelType,
    "Health",
    (ModelNode*) &iedModel_Battery_ZBTC1,
    (ModelNode*) &iedModel_Battery_ZBTC1_NamPlt,
    (ModelNode*) &iedModel_Battery_ZBTC1_Health_stVal,
    0,
    -1
};

DataAttribute iedModel_Battery_ZBTC1_Health_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_Battery_ZBTC1_Health,
    (ModelNode*) &iedModel_Battery_ZBTC1_Health_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Battery_ZBTC1_Health_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Battery_ZBTC1_Health,
    (ModelNode*) &iedModel_Battery_ZBTC1_Health_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Battery_ZBTC1_Health_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Battery_ZBTC1_Health,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_Battery_ZBTC1_NamPlt = {
    DataObjectModelType,
    "NamPlt",
    (ModelNode*) &iedModel_Battery_ZBTC1,
    (ModelNode*) &iedModel_Battery_ZBTC1_BatChaSt,
    (ModelNode*) &iedModel_Battery_ZBTC1_NamPlt_vendor,
    0,
    -1
};

DataAttribute iedModel_Battery_ZBTC1_NamPlt_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_Battery_ZBTC1_NamPlt,
    (ModelNode*) &iedModel_Battery_ZBTC1_NamPlt_swRev,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_Battery_ZBTC1_NamPlt_swRev = {
    DataAttributeModelType,
    "swRev",
    (ModelNode*) &iedModel_Battery_ZBTC1_NamPlt,
    (ModelNode*) &iedModel_Battery_ZBTC1_NamPlt_d,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_Battery_ZBTC1_NamPlt_d = {
    DataAttributeModelType,
    "d",
    (ModelNode*) &iedModel_Battery_ZBTC1_NamPlt,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_Battery_ZBTC1_BatChaSt = {
    DataObjectModelType,
    "BatChaSt",
    (ModelNode*) &iedModel_Battery_ZBTC1,
    (ModelNode*) &iedModel_Battery_ZBTC1_BatChaPwr,
    NULL,
    0,
    -1
};

DataObject iedModel_Battery_ZBTC1_BatChaPwr = {
    DataObjectModelType,
    "BatChaPwr",
    (ModelNode*) &iedModel_Battery_ZBTC1,
    (ModelNode*) &iedModel_Battery_ZBTC1_BatChaMod,
    NULL,
    0,
    -1
};

DataObject iedModel_Battery_ZBTC1_BatChaMod = {
    DataObjectModelType,
    "BatChaMod",
    (ModelNode*) &iedModel_Battery_ZBTC1,
    (ModelNode*) &iedModel_Battery_ZBTC1_ChaV,
    NULL,
    0,
    -1
};

DataObject iedModel_Battery_ZBTC1_ChaV = {
    DataObjectModelType,
    "ChaV",
    (ModelNode*) &iedModel_Battery_ZBTC1,
    (ModelNode*) &iedModel_Battery_ZBTC1_ChaA,
    (ModelNode*) &iedModel_Battery_ZBTC1_ChaV_mag,
    0,
    -1
};

DataAttribute iedModel_Battery_ZBTC1_ChaV_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_Battery_ZBTC1_ChaV,
    (ModelNode*) &iedModel_Battery_ZBTC1_ChaV_q,
    (ModelNode*) &iedModel_Battery_ZBTC1_ChaV_mag_f,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Battery_ZBTC1_ChaV_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_Battery_ZBTC1_ChaV_mag,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Battery_ZBTC1_ChaV_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Battery_ZBTC1_ChaV,
    (ModelNode*) &iedModel_Battery_ZBTC1_ChaV_t,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Battery_ZBTC1_ChaV_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Battery_ZBTC1_ChaV,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_Battery_ZBTC1_ChaA = {
    DataObjectModelType,
    "ChaA",
    (ModelNode*) &iedModel_Battery_ZBTC1,
    NULL,
    (ModelNode*) &iedModel_Battery_ZBTC1_ChaA_mag,
    0,
    -1
};

DataAttribute iedModel_Battery_ZBTC1_ChaA_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_Battery_ZBTC1_ChaA,
    (ModelNode*) &iedModel_Battery_ZBTC1_ChaA_q,
    (ModelNode*) &iedModel_Battery_ZBTC1_ChaA_mag_f,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Battery_ZBTC1_ChaA_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_Battery_ZBTC1_ChaA_mag,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Battery_ZBTC1_ChaA_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Battery_ZBTC1_ChaA,
    (ModelNode*) &iedModel_Battery_ZBTC1_ChaA_t,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Battery_ZBTC1_ChaA_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Battery_ZBTC1_ChaA,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};


LogicalDevice iedModel_Physical_Measurements = {
    LogicalDeviceModelType,
    "Physical_Measurements",
    (ModelNode*) &iedModel,
    NULL,
    (ModelNode*) &iedModel_Physical_Measurements_LLN0,
    NULL
};

LogicalNode iedModel_Physical_Measurements_LLN0 = {
    LogicalNodeModelType,
    "LLN0",
    (ModelNode*) &iedModel_Physical_Measurements,
    (ModelNode*) &iedModel_Physical_Measurements_LPHD1,
    (ModelNode*) &iedModel_Physical_Measurements_LLN0_Mod,
};

DataObject iedModel_Physical_Measurements_LLN0_Mod = {
    DataObjectModelType,
    "Mod",
    (ModelNode*) &iedModel_Physical_Measurements_LLN0,
    (ModelNode*) &iedModel_Physical_Measurements_LLN0_Beh,
    (ModelNode*) &iedModel_Physical_Measurements_LLN0_Mod_q,
    0,
    -1
};

DataAttribute iedModel_Physical_Measurements_LLN0_Mod_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Physical_Measurements_LLN0_Mod,
    (ModelNode*) &iedModel_Physical_Measurements_LLN0_Mod_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Physical_Measurements_LLN0_Mod_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Physical_Measurements_LLN0_Mod,
    (ModelNode*) &iedModel_Physical_Measurements_LLN0_Mod_ctlModel,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_Physical_Measurements_LLN0_Mod_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_Physical_Measurements_LLN0_Mod,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_Physical_Measurements_LLN0_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_Physical_Measurements_LLN0,
    (ModelNode*) &iedModel_Physical_Measurements_LLN0_Health,
    (ModelNode*) &iedModel_Physical_Measurements_LLN0_Beh_stVal,
    0,
    -1
};

DataAttribute iedModel_Physical_Measurements_LLN0_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_Physical_Measurements_LLN0_Beh,
    (ModelNode*) &iedModel_Physical_Measurements_LLN0_Beh_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Physical_Measurements_LLN0_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Physical_Measurements_LLN0_Beh,
    (ModelNode*) &iedModel_Physical_Measurements_LLN0_Beh_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Physical_Measurements_LLN0_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Physical_Measurements_LLN0_Beh,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_Physical_Measurements_LLN0_Health = {
    DataObjectModelType,
    "Health",
    (ModelNode*) &iedModel_Physical_Measurements_LLN0,
    (ModelNode*) &iedModel_Physical_Measurements_LLN0_NamPlt,
    (ModelNode*) &iedModel_Physical_Measurements_LLN0_Health_stVal,
    0,
    -1
};

DataAttribute iedModel_Physical_Measurements_LLN0_Health_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_Physical_Measurements_LLN0_Health,
    (ModelNode*) &iedModel_Physical_Measurements_LLN0_Health_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Physical_Measurements_LLN0_Health_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Physical_Measurements_LLN0_Health,
    (ModelNode*) &iedModel_Physical_Measurements_LLN0_Health_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Physical_Measurements_LLN0_Health_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Physical_Measurements_LLN0_Health,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_Physical_Measurements_LLN0_NamPlt = {
    DataObjectModelType,
    "NamPlt",
    (ModelNode*) &iedModel_Physical_Measurements_LLN0,
    NULL,
    (ModelNode*) &iedModel_Physical_Measurements_LLN0_NamPlt_vendor,
    0,
    -1
};

DataAttribute iedModel_Physical_Measurements_LLN0_NamPlt_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_Physical_Measurements_LLN0_NamPlt,
    (ModelNode*) &iedModel_Physical_Measurements_LLN0_NamPlt_swRev,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_Physical_Measurements_LLN0_NamPlt_swRev = {
    DataAttributeModelType,
    "swRev",
    (ModelNode*) &iedModel_Physical_Measurements_LLN0_NamPlt,
    (ModelNode*) &iedModel_Physical_Measurements_LLN0_NamPlt_d,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_Physical_Measurements_LLN0_NamPlt_d = {
    DataAttributeModelType,
    "d",
    (ModelNode*) &iedModel_Physical_Measurements_LLN0_NamPlt,
    (ModelNode*) &iedModel_Physical_Measurements_LLN0_NamPlt_configRev,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_Physical_Measurements_LLN0_NamPlt_configRev = {
    DataAttributeModelType,
    "configRev",
    (ModelNode*) &iedModel_Physical_Measurements_LLN0_NamPlt,
    (ModelNode*) &iedModel_Physical_Measurements_LLN0_NamPlt_ldNs,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_Physical_Measurements_LLN0_NamPlt_ldNs = {
    DataAttributeModelType,
    "ldNs",
    (ModelNode*) &iedModel_Physical_Measurements_LLN0_NamPlt,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_EX,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

LogicalNode iedModel_Physical_Measurements_LPHD1 = {
    LogicalNodeModelType,
    "LPHD1",
    (ModelNode*) &iedModel_Physical_Measurements,
    NULL,
    (ModelNode*) &iedModel_Physical_Measurements_LPHD1_PhyNam,
};

DataObject iedModel_Physical_Measurements_LPHD1_PhyNam = {
    DataObjectModelType,
    "PhyNam",
    (ModelNode*) &iedModel_Physical_Measurements_LPHD1,
    (ModelNode*) &iedModel_Physical_Measurements_LPHD1_PhyHealth,
    (ModelNode*) &iedModel_Physical_Measurements_LPHD1_PhyNam_vendor,
    0,
    -1
};

DataAttribute iedModel_Physical_Measurements_LPHD1_PhyNam_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_Physical_Measurements_LPHD1_PhyNam,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_Physical_Measurements_LPHD1_PhyHealth = {
    DataObjectModelType,
    "PhyHealth",
    (ModelNode*) &iedModel_Physical_Measurements_LPHD1,
    (ModelNode*) &iedModel_Physical_Measurements_LPHD1_Proxy,
    (ModelNode*) &iedModel_Physical_Measurements_LPHD1_PhyHealth_stVal,
    0,
    -1
};

DataAttribute iedModel_Physical_Measurements_LPHD1_PhyHealth_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_Physical_Measurements_LPHD1_PhyHealth,
    (ModelNode*) &iedModel_Physical_Measurements_LPHD1_PhyHealth_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Physical_Measurements_LPHD1_PhyHealth_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Physical_Measurements_LPHD1_PhyHealth,
    (ModelNode*) &iedModel_Physical_Measurements_LPHD1_PhyHealth_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Physical_Measurements_LPHD1_PhyHealth_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Physical_Measurements_LPHD1_PhyHealth,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_Physical_Measurements_LPHD1_Proxy = {
    DataObjectModelType,
    "Proxy",
    (ModelNode*) &iedModel_Physical_Measurements_LPHD1,
    NULL,
    (ModelNode*) &iedModel_Physical_Measurements_LPHD1_Proxy_stVal,
    0,
    -1
};

DataAttribute iedModel_Physical_Measurements_LPHD1_Proxy_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_Physical_Measurements_LPHD1_Proxy,
    (ModelNode*) &iedModel_Physical_Measurements_LPHD1_Proxy_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Physical_Measurements_LPHD1_Proxy_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_Physical_Measurements_LPHD1_Proxy,
    (ModelNode*) &iedModel_Physical_Measurements_LPHD1_Proxy_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_Physical_Measurements_LPHD1_Proxy_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_Physical_Measurements_LPHD1_Proxy,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

extern ReportControlBlock iedModel_Inverter_LLN0_report0;

ReportControlBlock iedModel_Inverter_LLN0_report0 = {&iedModel_Inverter_LLN0, "rcb101", "ID", false, "dataset1", 0, 19, 32, 0, 0, {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, NULL};







IedModel iedModel = {
    "ied1",
    &iedModel_Inverter,
    &iedModelds_Inverter_LLN0_dataset1,
    &iedModel_Inverter_LLN0_report0,
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

iedModel_Inverter_LLN0_Mod_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(0);

iedModel_Inverter_ZINV1_Mod_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(0);

iedModel_Inverter_MMXU1_Mod_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(0);

iedModel_Battery_LLN0_Mod_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(0);

iedModel_Battery_ZBAT1_Mod_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(0);

iedModel_Battery_ZBTC1_Mod_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(0);

iedModel_Physical_Measurements_LLN0_Mod_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(0);
}
