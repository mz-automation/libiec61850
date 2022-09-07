/*
 * static_model.c
 *
 * automatically generated from cid_example_deadband.cid
 */
#include "static_model.h"

static void initializeValues();

extern DataSet iedModelds_LD1_LLN0_AnalogEvents;


extern DataSetEntry iedModelds_LD1_LLN0_AnalogEvents_fcda0;
extern DataSetEntry iedModelds_LD1_LLN0_AnalogEvents_fcda1;
extern DataSetEntry iedModelds_LD1_LLN0_AnalogEvents_fcda2;
extern DataSetEntry iedModelds_LD1_LLN0_AnalogEvents_fcda3;

DataSetEntry iedModelds_LD1_LLN0_AnalogEvents_fcda0 = {
  "LD1",
  false,
  "AnInGGIO1$MX$AnIn1", 
  -1,
  NULL,
  NULL,
  &iedModelds_LD1_LLN0_AnalogEvents_fcda1
};

DataSetEntry iedModelds_LD1_LLN0_AnalogEvents_fcda1 = {
  "LD1",
  false,
  "AnInGGIO1$MX$AnIn2", 
  -1,
  NULL,
  NULL,
  &iedModelds_LD1_LLN0_AnalogEvents_fcda2
};

DataSetEntry iedModelds_LD1_LLN0_AnalogEvents_fcda2 = {
  "LD1",
  false,
  "AnInGGIO1$MX$AnIn3", 
  -1,
  NULL,
  NULL,
  &iedModelds_LD1_LLN0_AnalogEvents_fcda3
};

DataSetEntry iedModelds_LD1_LLN0_AnalogEvents_fcda3 = {
  "LD1",
  false,
  "AnInGGIO1$MX$AnIn4", 
  -1,
  NULL,
  NULL,
  NULL
};

DataSet iedModelds_LD1_LLN0_AnalogEvents = {
  "LD1",
  "LLN0$AnalogEvents",
  4,
  &iedModelds_LD1_LLN0_AnalogEvents_fcda0,
  NULL
};

LogicalDevice iedModel_LD1 = {
    LogicalDeviceModelType,
    "LD1",
    (ModelNode*) &iedModel,
    NULL,
    (ModelNode*) &iedModel_LD1_LLN0,
    NULL
};

LogicalNode iedModel_LD1_LLN0 = {
    LogicalNodeModelType,
    "LLN0",
    (ModelNode*) &iedModel_LD1,
    (ModelNode*) &iedModel_LD1_LPHD1,
    (ModelNode*) &iedModel_LD1_LLN0_Mod,
};

DataObject iedModel_LD1_LLN0_Mod = {
    DataObjectModelType,
    "Mod",
    (ModelNode*) &iedModel_LD1_LLN0,
    (ModelNode*) &iedModel_LD1_LLN0_Beh,
    (ModelNode*) &iedModel_LD1_LLN0_Mod_stVal,
    0,
    -1
};

DataAttribute iedModel_LD1_LLN0_Mod_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD1_LLN0_Mod,
    (ModelNode*) &iedModel_LD1_LLN0_Mod_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD1_LLN0_Mod_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD1_LLN0_Mod,
    (ModelNode*) &iedModel_LD1_LLN0_Mod_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD1_LLN0_Mod_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD1_LLN0_Mod,
    (ModelNode*) &iedModel_LD1_LLN0_Mod_ctlModel,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_LLN0_Mod_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_LD1_LLN0_Mod,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_LD1_LLN0_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_LD1_LLN0,
    (ModelNode*) &iedModel_LD1_LLN0_Health,
    (ModelNode*) &iedModel_LD1_LLN0_Beh_stVal,
    0,
    -1
};

DataAttribute iedModel_LD1_LLN0_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD1_LLN0_Beh,
    (ModelNode*) &iedModel_LD1_LLN0_Beh_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_LLN0_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD1_LLN0_Beh,
    (ModelNode*) &iedModel_LD1_LLN0_Beh_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_LLN0_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD1_LLN0_Beh,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_LD1_LLN0_Health = {
    DataObjectModelType,
    "Health",
    (ModelNode*) &iedModel_LD1_LLN0,
    (ModelNode*) &iedModel_LD1_LLN0_NamPlt,
    (ModelNode*) &iedModel_LD1_LLN0_Health_stVal,
    0,
    -1
};

DataAttribute iedModel_LD1_LLN0_Health_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD1_LLN0_Health,
    (ModelNode*) &iedModel_LD1_LLN0_Health_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD1_LLN0_Health_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD1_LLN0_Health,
    (ModelNode*) &iedModel_LD1_LLN0_Health_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD1_LLN0_Health_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD1_LLN0_Health,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_LD1_LLN0_NamPlt = {
    DataObjectModelType,
    "NamPlt",
    (ModelNode*) &iedModel_LD1_LLN0,
    NULL,
    (ModelNode*) &iedModel_LD1_LLN0_NamPlt_vendor,
    0,
    -1
};

DataAttribute iedModel_LD1_LLN0_NamPlt_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_LD1_LLN0_NamPlt,
    (ModelNode*) &iedModel_LD1_LLN0_NamPlt_swRev,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_LLN0_NamPlt_swRev = {
    DataAttributeModelType,
    "swRev",
    (ModelNode*) &iedModel_LD1_LLN0_NamPlt,
    (ModelNode*) &iedModel_LD1_LLN0_NamPlt_d,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_LLN0_NamPlt_d = {
    DataAttributeModelType,
    "d",
    (ModelNode*) &iedModel_LD1_LLN0_NamPlt,
    (ModelNode*) &iedModel_LD1_LLN0_NamPlt_configRev,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_LLN0_NamPlt_configRev = {
    DataAttributeModelType,
    "configRev",
    (ModelNode*) &iedModel_LD1_LLN0_NamPlt,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

LogicalNode iedModel_LD1_LPHD1 = {
    LogicalNodeModelType,
    "LPHD1",
    (ModelNode*) &iedModel_LD1,
    (ModelNode*) &iedModel_LD1_AnInGGIO1,
    (ModelNode*) &iedModel_LD1_LPHD1_PhyNam,
};

DataObject iedModel_LD1_LPHD1_PhyNam = {
    DataObjectModelType,
    "PhyNam",
    (ModelNode*) &iedModel_LD1_LPHD1,
    (ModelNode*) &iedModel_LD1_LPHD1_PhyHealth,
    (ModelNode*) &iedModel_LD1_LPHD1_PhyNam_vendor,
    0,
    -1
};

DataAttribute iedModel_LD1_LPHD1_PhyNam_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_LD1_LPHD1_PhyNam,
    (ModelNode*) &iedModel_LD1_LPHD1_PhyNam_hwRev,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_LPHD1_PhyNam_hwRev = {
    DataAttributeModelType,
    "hwRev",
    (ModelNode*) &iedModel_LD1_LPHD1_PhyNam,
    (ModelNode*) &iedModel_LD1_LPHD1_PhyNam_swRev,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_LPHD1_PhyNam_swRev = {
    DataAttributeModelType,
    "swRev",
    (ModelNode*) &iedModel_LD1_LPHD1_PhyNam,
    (ModelNode*) &iedModel_LD1_LPHD1_PhyNam_serNum,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_LPHD1_PhyNam_serNum = {
    DataAttributeModelType,
    "serNum",
    (ModelNode*) &iedModel_LD1_LPHD1_PhyNam,
    (ModelNode*) &iedModel_LD1_LPHD1_PhyNam_model,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_LPHD1_PhyNam_model = {
    DataAttributeModelType,
    "model",
    (ModelNode*) &iedModel_LD1_LPHD1_PhyNam,
    (ModelNode*) &iedModel_LD1_LPHD1_PhyNam_location,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_LPHD1_PhyNam_location = {
    DataAttributeModelType,
    "location",
    (ModelNode*) &iedModel_LD1_LPHD1_PhyNam,
    (ModelNode*) &iedModel_LD1_LPHD1_PhyNam_name,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_LPHD1_PhyNam_name = {
    DataAttributeModelType,
    "name",
    (ModelNode*) &iedModel_LD1_LPHD1_PhyNam,
    (ModelNode*) &iedModel_LD1_LPHD1_PhyNam_owner,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_LPHD1_PhyNam_owner = {
    DataAttributeModelType,
    "owner",
    (ModelNode*) &iedModel_LD1_LPHD1_PhyNam,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD1_LPHD1_PhyHealth = {
    DataObjectModelType,
    "PhyHealth",
    (ModelNode*) &iedModel_LD1_LPHD1,
    (ModelNode*) &iedModel_LD1_LPHD1_Proxy,
    (ModelNode*) &iedModel_LD1_LPHD1_PhyHealth_stVal,
    0,
    -1
};

DataAttribute iedModel_LD1_LPHD1_PhyHealth_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD1_LPHD1_PhyHealth,
    (ModelNode*) &iedModel_LD1_LPHD1_PhyHealth_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD1_LPHD1_PhyHealth_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD1_LPHD1_PhyHealth,
    (ModelNode*) &iedModel_LD1_LPHD1_PhyHealth_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD1_LPHD1_PhyHealth_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD1_LPHD1_PhyHealth,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_LD1_LPHD1_Proxy = {
    DataObjectModelType,
    "Proxy",
    (ModelNode*) &iedModel_LD1_LPHD1,
    NULL,
    (ModelNode*) &iedModel_LD1_LPHD1_Proxy_stVal,
    0,
    -1
};

DataAttribute iedModel_LD1_LPHD1_Proxy_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD1_LPHD1_Proxy,
    (ModelNode*) &iedModel_LD1_LPHD1_Proxy_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD1_LPHD1_Proxy_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD1_LPHD1_Proxy,
    (ModelNode*) &iedModel_LD1_LPHD1_Proxy_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD1_LPHD1_Proxy_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD1_LPHD1_Proxy,
    (ModelNode*) &iedModel_LD1_LPHD1_Proxy_d,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_LPHD1_Proxy_d = {
    DataAttributeModelType,
    "d",
    (ModelNode*) &iedModel_LD1_LPHD1_Proxy,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

LogicalNode iedModel_LD1_AnInGGIO1 = {
    LogicalNodeModelType,
    "AnInGGIO1",
    (ModelNode*) &iedModel_LD1,
    NULL,
    (ModelNode*) &iedModel_LD1_AnInGGIO1_Beh,
};

DataObject iedModel_LD1_AnInGGIO1_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_LD1_AnInGGIO1,
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn1,
    (ModelNode*) &iedModel_LD1_AnInGGIO1_Beh_stVal,
    0,
    -1
};

DataAttribute iedModel_LD1_AnInGGIO1_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD1_AnInGGIO1_Beh,
    (ModelNode*) &iedModel_LD1_AnInGGIO1_Beh_q,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_AnInGGIO1_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD1_AnInGGIO1_Beh,
    (ModelNode*) &iedModel_LD1_AnInGGIO1_Beh_t,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_AnInGGIO1_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD1_AnInGGIO1_Beh,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_LD1_AnInGGIO1_AnIn1 = {
    DataObjectModelType,
    "AnIn1",
    (ModelNode*) &iedModel_LD1_AnInGGIO1,
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn2,
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn1_instMag,
    0,
    -1
};

DataAttribute iedModel_LD1_AnInGGIO1_AnIn1_instMag = {
    DataAttributeModelType,
    "instMag",
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn1,
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn1_mag,
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn1_instMag_f,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_AnInGGIO1_AnIn1_instMag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn1_instMag,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_AnInGGIO1_AnIn1_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn1,
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn1_q,
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn1_mag_f,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD1_AnInGGIO1_AnIn1_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn1_mag,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD1_AnInGGIO1_AnIn1_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn1,
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn1_t,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD1_AnInGGIO1_AnIn1_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn1,
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn1_db,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_AnInGGIO1_AnIn1_db = {
    DataAttributeModelType,
    "db",
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn1,
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn1_zeroDb,
    NULL,
    0,
    -1,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD1_AnInGGIO1_AnIn1_zeroDb = {
    DataAttributeModelType,
    "zeroDb",
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn1,
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn1_dbRef,
    NULL,
    0,
    -1,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD1_AnInGGIO1_AnIn1_dbRef = {
    DataAttributeModelType,
    "dbRef",
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn1,
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn1_zeroDbRef,
    NULL,
    0,
    -1,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD1_AnInGGIO1_AnIn1_zeroDbRef = {
    DataAttributeModelType,
    "zeroDbRef",
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn1,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_LD1_AnInGGIO1_AnIn2 = {
    DataObjectModelType,
    "AnIn2",
    (ModelNode*) &iedModel_LD1_AnInGGIO1,
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn3,
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn2_instMag,
    0,
    -1
};

DataAttribute iedModel_LD1_AnInGGIO1_AnIn2_instMag = {
    DataAttributeModelType,
    "instMag",
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn2,
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn2_mag,
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn2_instMag_f,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_AnInGGIO1_AnIn2_instMag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn2_instMag,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_AnInGGIO1_AnIn2_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn2,
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn2_q,
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn2_mag_f,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD1_AnInGGIO1_AnIn2_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn2_mag,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD1_AnInGGIO1_AnIn2_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn2,
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn2_t,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD1_AnInGGIO1_AnIn2_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn2,
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn2_db,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_AnInGGIO1_AnIn2_db = {
    DataAttributeModelType,
    "db",
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn2,
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn2_zeroDb,
    NULL,
    0,
    -1,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD1_AnInGGIO1_AnIn2_zeroDb = {
    DataAttributeModelType,
    "zeroDb",
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn2,
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn2_dbRef,
    NULL,
    0,
    -1,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD1_AnInGGIO1_AnIn2_dbRef = {
    DataAttributeModelType,
    "dbRef",
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn2,
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn2_zeroDbRef,
    NULL,
    0,
    -1,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD1_AnInGGIO1_AnIn2_zeroDbRef = {
    DataAttributeModelType,
    "zeroDbRef",
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn2,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_LD1_AnInGGIO1_AnIn3 = {
    DataObjectModelType,
    "AnIn3",
    (ModelNode*) &iedModel_LD1_AnInGGIO1,
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn4,
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn3_instMag,
    0,
    -1
};

DataAttribute iedModel_LD1_AnInGGIO1_AnIn3_instMag = {
    DataAttributeModelType,
    "instMag",
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn3,
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn3_mag,
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn3_instMag_f,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_AnInGGIO1_AnIn3_instMag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn3_instMag,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_AnInGGIO1_AnIn3_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn3,
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn3_q,
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn3_mag_f,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD1_AnInGGIO1_AnIn3_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn3_mag,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD1_AnInGGIO1_AnIn3_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn3,
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn3_t,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD1_AnInGGIO1_AnIn3_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn3,
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn3_db,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_AnInGGIO1_AnIn3_db = {
    DataAttributeModelType,
    "db",
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn3,
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn3_zeroDb,
    NULL,
    0,
    -1,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD1_AnInGGIO1_AnIn3_zeroDb = {
    DataAttributeModelType,
    "zeroDb",
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn3,
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn3_rangeC,
    NULL,
    0,
    -1,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD1_AnInGGIO1_AnIn3_rangeC = {
    DataAttributeModelType,
    "rangeC",
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn3,
    NULL,
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn3_rangeC_min,
    0,
    -1,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD1_AnInGGIO1_AnIn3_rangeC_min = {
    DataAttributeModelType,
    "min",
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn3_rangeC,
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn3_rangeC_max,
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn3_rangeC_min_f,
    0,
    -1,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD1_AnInGGIO1_AnIn3_rangeC_min_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn3_rangeC_min,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD1_AnInGGIO1_AnIn3_rangeC_max = {
    DataAttributeModelType,
    "max",
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn3_rangeC,
    NULL,
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn3_rangeC_max_f,
    0,
    -1,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD1_AnInGGIO1_AnIn3_rangeC_max_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn3_rangeC_max,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_LD1_AnInGGIO1_AnIn4 = {
    DataObjectModelType,
    "AnIn4",
    (ModelNode*) &iedModel_LD1_AnInGGIO1,
    NULL,
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn4_instMag,
    0,
    -1
};

DataAttribute iedModel_LD1_AnInGGIO1_AnIn4_instMag = {
    DataAttributeModelType,
    "instMag",
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn4,
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn4_mag,
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn4_instMag_f,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_AnInGGIO1_AnIn4_instMag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn4_instMag,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_AnInGGIO1_AnIn4_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn4,
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn4_q,
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn4_mag_f,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD1_AnInGGIO1_AnIn4_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn4_mag,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD1_AnInGGIO1_AnIn4_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn4,
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn4_t,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD1_AnInGGIO1_AnIn4_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn4,
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn4_db,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_AnInGGIO1_AnIn4_db = {
    DataAttributeModelType,
    "db",
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn4,
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn4_zeroDb,
    NULL,
    0,
    -1,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD1_AnInGGIO1_AnIn4_zeroDb = {
    DataAttributeModelType,
    "zeroDb",
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn4,
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn4_rangeC,
    NULL,
    0,
    -1,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD1_AnInGGIO1_AnIn4_rangeC = {
    DataAttributeModelType,
    "rangeC",
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn4,
    NULL,
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn4_rangeC_min,
    0,
    -1,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD1_AnInGGIO1_AnIn4_rangeC_min = {
    DataAttributeModelType,
    "min",
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn4_rangeC,
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn4_rangeC_max,
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn4_rangeC_min_f,
    0,
    -1,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD1_AnInGGIO1_AnIn4_rangeC_min_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn4_rangeC_min,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD1_AnInGGIO1_AnIn4_rangeC_max = {
    DataAttributeModelType,
    "max",
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn4_rangeC,
    NULL,
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn4_rangeC_max_f,
    0,
    -1,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD1_AnInGGIO1_AnIn4_rangeC_max_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_LD1_AnInGGIO1_AnIn4_rangeC_max,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

extern ReportControlBlock iedModel_LD1_LLN0_report0;
extern ReportControlBlock iedModel_LD1_LLN0_report1;
extern ReportControlBlock iedModel_LD1_LLN0_report2;
extern ReportControlBlock iedModel_LD1_LLN0_report3;

ReportControlBlock iedModel_LD1_LLN0_report0 = {&iedModel_LD1_LLN0, "BRCB_Events01", NULL, true, "AnalogEvents", 1, 19, 247, 0, 5000, {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, &iedModel_LD1_LLN0_report1};
ReportControlBlock iedModel_LD1_LLN0_report1 = {&iedModel_LD1_LLN0, "BRCB_Events02", NULL, true, "AnalogEvents", 1, 19, 247, 0, 5000, {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, &iedModel_LD1_LLN0_report2};
ReportControlBlock iedModel_LD1_LLN0_report2 = {&iedModel_LD1_LLN0, "URCB_Events01", NULL, false, "AnalogEvents", 1, 19, 183, 0, 5000, {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, &iedModel_LD1_LLN0_report3};
ReportControlBlock iedModel_LD1_LLN0_report3 = {&iedModel_LD1_LLN0, "URCB_Events02", NULL, false, "AnalogEvents", 1, 19, 183, 0, 5000, {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, NULL};







IedModel iedModel = {
    "IED1",
    &iedModel_LD1,
    &iedModelds_LD1_LLN0_AnalogEvents,
    &iedModel_LD1_LLN0_report0,
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

iedModel_LD1_LLN0_Mod_stVal.mmsValue = MmsValue_newIntegerFromInt32(1);

iedModel_LD1_AnInGGIO1_Beh_stVal.mmsValue = MmsValue_newIntegerFromInt32(1);

iedModel_LD1_AnInGGIO1_AnIn1_db.mmsValue = MmsValue_newUnsignedFromUint32(10000);

iedModel_LD1_AnInGGIO1_AnIn1_zeroDb.mmsValue = MmsValue_newUnsignedFromUint32(1000);

iedModel_LD1_AnInGGIO1_AnIn1_dbRef.mmsValue = MmsValue_newFloat(10.0);

iedModel_LD1_AnInGGIO1_AnIn1_zeroDbRef.mmsValue = MmsValue_newFloat(10.0);

iedModel_LD1_AnInGGIO1_AnIn2_db.mmsValue = MmsValue_newUnsignedFromUint32(1000);

iedModel_LD1_AnInGGIO1_AnIn2_zeroDb.mmsValue = MmsValue_newUnsignedFromUint32(1000);

iedModel_LD1_AnInGGIO1_AnIn2_dbRef.mmsValue = MmsValue_newFloat(10.0);

iedModel_LD1_AnInGGIO1_AnIn2_zeroDbRef.mmsValue = MmsValue_newFloat(10.0);

iedModel_LD1_AnInGGIO1_AnIn3_db.mmsValue = MmsValue_newUnsignedFromUint32(10000);

iedModel_LD1_AnInGGIO1_AnIn3_zeroDb.mmsValue = MmsValue_newUnsignedFromUint32(1000);

iedModel_LD1_AnInGGIO1_AnIn3_rangeC_min_f.mmsValue = MmsValue_newFloat(-5.0);

iedModel_LD1_AnInGGIO1_AnIn3_rangeC_max_f.mmsValue = MmsValue_newFloat(5.0);

iedModel_LD1_AnInGGIO1_AnIn4_db.mmsValue = MmsValue_newUnsignedFromUint32(1000);

iedModel_LD1_AnInGGIO1_AnIn4_zeroDb.mmsValue = MmsValue_newUnsignedFromUint32(1000);

iedModel_LD1_AnInGGIO1_AnIn4_rangeC_min_f.mmsValue = MmsValue_newFloat(-100.0);

iedModel_LD1_AnInGGIO1_AnIn4_rangeC_max_f.mmsValue = MmsValue_newFloat(100.0);
}
