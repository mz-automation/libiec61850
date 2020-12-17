/*
 * static_model.c
 *
 * automatically generated from substitution_example.cid
 */
#include "static_model.h"

static void initializeValues();



LogicalDevice iedModel_LD1 = {
    LogicalDeviceModelType,
    "LD1",
    (ModelNode*) &iedModel,
    NULL,
    (ModelNode*) &iedModel_LD1_LLN0
};

LogicalNode iedModel_LD1_LLN0 = {
    LogicalNodeModelType,
    "LLN0",
    (ModelNode*) &iedModel_LD1,
    (ModelNode*) &iedModel_LD1_LPHD1,
    (ModelNode*) &iedModel_LD1_LLN0_Beh,
};

DataObject iedModel_LD1_LLN0_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_LD1_LLN0,
    NULL,
    (ModelNode*) &iedModel_LD1_LLN0_Beh_stVal,
    0
};

DataAttribute iedModel_LD1_LLN0_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD1_LLN0_Beh,
    (ModelNode*) &iedModel_LD1_LLN0_Beh_q,
    NULL,
    0,
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
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

LogicalNode iedModel_LD1_LPHD1 = {
    LogicalNodeModelType,
    "LPHD1",
    (ModelNode*) &iedModel_LD1,
    (ModelNode*) &iedModel_LD1_MMDC1,
    (ModelNode*) &iedModel_LD1_LPHD1_PhyNam,
};

DataObject iedModel_LD1_LPHD1_PhyNam = {
    DataObjectModelType,
    "PhyNam",
    (ModelNode*) &iedModel_LD1_LPHD1,
    (ModelNode*) &iedModel_LD1_LPHD1_PhyHealth,
    (ModelNode*) &iedModel_LD1_LPHD1_PhyNam_vendor,
    0
};

DataAttribute iedModel_LD1_LPHD1_PhyNam_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_LD1_LPHD1_PhyNam,
    NULL,
    NULL,
    0,
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
    0
};

DataAttribute iedModel_LD1_LPHD1_PhyHealth_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD1_LPHD1_PhyHealth,
    (ModelNode*) &iedModel_LD1_LPHD1_PhyHealth_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_LPHD1_PhyHealth_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD1_LPHD1_PhyHealth,
    (ModelNode*) &iedModel_LD1_LPHD1_PhyHealth_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_LPHD1_PhyHealth_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD1_LPHD1_PhyHealth,
    NULL,
    NULL,
    0,
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
    0
};

DataAttribute iedModel_LD1_LPHD1_Proxy_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD1_LPHD1_Proxy,
    (ModelNode*) &iedModel_LD1_LPHD1_Proxy_q,
    NULL,
    0,
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
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD1_LPHD1_Proxy_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD1_LPHD1_Proxy,
    (ModelNode*) &iedModel_LD1_LPHD1_Proxy_subEna,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_LPHD1_Proxy_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_LD1_LPHD1_Proxy,
    (ModelNode*) &iedModel_LD1_LPHD1_Proxy_subVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_LPHD1_Proxy_subVal = {
    DataAttributeModelType,
    "subVal",
    (ModelNode*) &iedModel_LD1_LPHD1_Proxy,
    (ModelNode*) &iedModel_LD1_LPHD1_Proxy_subQ,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_LPHD1_Proxy_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_LD1_LPHD1_Proxy,
    (ModelNode*) &iedModel_LD1_LPHD1_Proxy_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_LPHD1_Proxy_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_LD1_LPHD1_Proxy,
    (ModelNode*) &iedModel_LD1_LPHD1_Proxy_blkEna,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_LPHD1_Proxy_blkEna = {
    DataAttributeModelType,
    "blkEna",
    (ModelNode*) &iedModel_LD1_LPHD1_Proxy,
    NULL,
    NULL,
    0,
    IEC61850_FC_BL,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

LogicalNode iedModel_LD1_MMDC1 = {
    LogicalNodeModelType,
    "MMDC1",
    (ModelNode*) &iedModel_LD1,
    (ModelNode*) &iedModel_LD1_GGIO1,
    (ModelNode*) &iedModel_LD1_MMDC1_Beh,
};

DataObject iedModel_LD1_MMDC1_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_LD1_MMDC1,
    (ModelNode*) &iedModel_LD1_MMDC1_Watt,
    (ModelNode*) &iedModel_LD1_MMDC1_Beh_stVal,
    0
};

DataAttribute iedModel_LD1_MMDC1_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD1_MMDC1_Beh,
    (ModelNode*) &iedModel_LD1_MMDC1_Beh_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_MMDC1_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD1_MMDC1_Beh,
    (ModelNode*) &iedModel_LD1_MMDC1_Beh_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_MMDC1_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD1_MMDC1_Beh,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_LD1_MMDC1_Watt = {
    DataObjectModelType,
    "Watt",
    (ModelNode*) &iedModel_LD1_MMDC1,
    (ModelNode*) &iedModel_LD1_MMDC1_Amp,
    (ModelNode*) &iedModel_LD1_MMDC1_Watt_mag,
    0
};

DataAttribute iedModel_LD1_MMDC1_Watt_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_LD1_MMDC1_Watt,
    (ModelNode*) &iedModel_LD1_MMDC1_Watt_q,
    (ModelNode*) &iedModel_LD1_MMDC1_Watt_mag_f,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD1_MMDC1_Watt_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_LD1_MMDC1_Watt_mag,
    NULL,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD1_MMDC1_Watt_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD1_MMDC1_Watt,
    (ModelNode*) &iedModel_LD1_MMDC1_Watt_t,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD1_MMDC1_Watt_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD1_MMDC1_Watt,
    (ModelNode*) &iedModel_LD1_MMDC1_Watt_subEna,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_MMDC1_Watt_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_LD1_MMDC1_Watt,
    (ModelNode*) &iedModel_LD1_MMDC1_Watt_subMag,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_MMDC1_Watt_subMag = {
    DataAttributeModelType,
    "subMag",
    (ModelNode*) &iedModel_LD1_MMDC1_Watt,
    (ModelNode*) &iedModel_LD1_MMDC1_Watt_subQ,
    (ModelNode*) &iedModel_LD1_MMDC1_Watt_subMag_f,
    0,
    IEC61850_FC_SV,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_MMDC1_Watt_subMag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_LD1_MMDC1_Watt_subMag,
    NULL,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_MMDC1_Watt_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_LD1_MMDC1_Watt,
    (ModelNode*) &iedModel_LD1_MMDC1_Watt_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_MMDC1_Watt_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_LD1_MMDC1_Watt,
    (ModelNode*) &iedModel_LD1_MMDC1_Watt_blkEna,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_MMDC1_Watt_blkEna = {
    DataAttributeModelType,
    "blkEna",
    (ModelNode*) &iedModel_LD1_MMDC1_Watt,
    NULL,
    NULL,
    0,
    IEC61850_FC_BL,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataObject iedModel_LD1_MMDC1_Amp = {
    DataObjectModelType,
    "Amp",
    (ModelNode*) &iedModel_LD1_MMDC1,
    (ModelNode*) &iedModel_LD1_MMDC1_Vol,
    (ModelNode*) &iedModel_LD1_MMDC1_Amp_mag,
    0
};

DataAttribute iedModel_LD1_MMDC1_Amp_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_LD1_MMDC1_Amp,
    (ModelNode*) &iedModel_LD1_MMDC1_Amp_q,
    (ModelNode*) &iedModel_LD1_MMDC1_Amp_mag_f,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD1_MMDC1_Amp_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_LD1_MMDC1_Amp_mag,
    NULL,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD1_MMDC1_Amp_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD1_MMDC1_Amp,
    (ModelNode*) &iedModel_LD1_MMDC1_Amp_t,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD1_MMDC1_Amp_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD1_MMDC1_Amp,
    (ModelNode*) &iedModel_LD1_MMDC1_Amp_subEna,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_MMDC1_Amp_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_LD1_MMDC1_Amp,
    (ModelNode*) &iedModel_LD1_MMDC1_Amp_subMag,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_MMDC1_Amp_subMag = {
    DataAttributeModelType,
    "subMag",
    (ModelNode*) &iedModel_LD1_MMDC1_Amp,
    (ModelNode*) &iedModel_LD1_MMDC1_Amp_subQ,
    (ModelNode*) &iedModel_LD1_MMDC1_Amp_subMag_f,
    0,
    IEC61850_FC_SV,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_MMDC1_Amp_subMag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_LD1_MMDC1_Amp_subMag,
    NULL,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_MMDC1_Amp_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_LD1_MMDC1_Amp,
    (ModelNode*) &iedModel_LD1_MMDC1_Amp_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_MMDC1_Amp_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_LD1_MMDC1_Amp,
    (ModelNode*) &iedModel_LD1_MMDC1_Amp_blkEna,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_MMDC1_Amp_blkEna = {
    DataAttributeModelType,
    "blkEna",
    (ModelNode*) &iedModel_LD1_MMDC1_Amp,
    NULL,
    NULL,
    0,
    IEC61850_FC_BL,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataObject iedModel_LD1_MMDC1_Vol = {
    DataObjectModelType,
    "Vol",
    (ModelNode*) &iedModel_LD1_MMDC1,
    NULL,
    (ModelNode*) &iedModel_LD1_MMDC1_Vol_mag,
    0
};

DataAttribute iedModel_LD1_MMDC1_Vol_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_LD1_MMDC1_Vol,
    (ModelNode*) &iedModel_LD1_MMDC1_Vol_q,
    (ModelNode*) &iedModel_LD1_MMDC1_Vol_mag_f,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD1_MMDC1_Vol_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_LD1_MMDC1_Vol_mag,
    NULL,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD1_MMDC1_Vol_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD1_MMDC1_Vol,
    (ModelNode*) &iedModel_LD1_MMDC1_Vol_t,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD1_MMDC1_Vol_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD1_MMDC1_Vol,
    (ModelNode*) &iedModel_LD1_MMDC1_Vol_subEna,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_MMDC1_Vol_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_LD1_MMDC1_Vol,
    (ModelNode*) &iedModel_LD1_MMDC1_Vol_subMag,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_MMDC1_Vol_subMag = {
    DataAttributeModelType,
    "subMag",
    (ModelNode*) &iedModel_LD1_MMDC1_Vol,
    (ModelNode*) &iedModel_LD1_MMDC1_Vol_subQ,
    (ModelNode*) &iedModel_LD1_MMDC1_Vol_subMag_f,
    0,
    IEC61850_FC_SV,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_MMDC1_Vol_subMag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_LD1_MMDC1_Vol_subMag,
    NULL,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_MMDC1_Vol_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_LD1_MMDC1_Vol,
    (ModelNode*) &iedModel_LD1_MMDC1_Vol_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_MMDC1_Vol_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_LD1_MMDC1_Vol,
    (ModelNode*) &iedModel_LD1_MMDC1_Vol_blkEna,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_MMDC1_Vol_blkEna = {
    DataAttributeModelType,
    "blkEna",
    (ModelNode*) &iedModel_LD1_MMDC1_Vol,
    NULL,
    NULL,
    0,
    IEC61850_FC_BL,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

LogicalNode iedModel_LD1_GGIO1 = {
    LogicalNodeModelType,
    "GGIO1",
    (ModelNode*) &iedModel_LD1,
    NULL,
    (ModelNode*) &iedModel_LD1_GGIO1_Beh,
};

DataObject iedModel_LD1_GGIO1_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_LD1_GGIO1,
    (ModelNode*) &iedModel_LD1_GGIO1_Ind1,
    (ModelNode*) &iedModel_LD1_GGIO1_Beh_stVal,
    0
};

DataAttribute iedModel_LD1_GGIO1_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD1_GGIO1_Beh,
    (ModelNode*) &iedModel_LD1_GGIO1_Beh_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_GGIO1_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD1_GGIO1_Beh,
    (ModelNode*) &iedModel_LD1_GGIO1_Beh_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_GGIO1_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD1_GGIO1_Beh,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_LD1_GGIO1_Ind1 = {
    DataObjectModelType,
    "Ind1",
    (ModelNode*) &iedModel_LD1_GGIO1,
    (ModelNode*) &iedModel_LD1_GGIO1_AnIn1,
    (ModelNode*) &iedModel_LD1_GGIO1_Ind1_stVal,
    0
};

DataAttribute iedModel_LD1_GGIO1_Ind1_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD1_GGIO1_Ind1,
    (ModelNode*) &iedModel_LD1_GGIO1_Ind1_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD1_GGIO1_Ind1_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD1_GGIO1_Ind1,
    (ModelNode*) &iedModel_LD1_GGIO1_Ind1_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD1_GGIO1_Ind1_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD1_GGIO1_Ind1,
    (ModelNode*) &iedModel_LD1_GGIO1_Ind1_subEna,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_GGIO1_Ind1_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_LD1_GGIO1_Ind1,
    (ModelNode*) &iedModel_LD1_GGIO1_Ind1_subVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_GGIO1_Ind1_subVal = {
    DataAttributeModelType,
    "subVal",
    (ModelNode*) &iedModel_LD1_GGIO1_Ind1,
    (ModelNode*) &iedModel_LD1_GGIO1_Ind1_subQ,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_GGIO1_Ind1_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_LD1_GGIO1_Ind1,
    (ModelNode*) &iedModel_LD1_GGIO1_Ind1_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_GGIO1_Ind1_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_LD1_GGIO1_Ind1,
    (ModelNode*) &iedModel_LD1_GGIO1_Ind1_blkEna,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_GGIO1_Ind1_blkEna = {
    DataAttributeModelType,
    "blkEna",
    (ModelNode*) &iedModel_LD1_GGIO1_Ind1,
    NULL,
    NULL,
    0,
    IEC61850_FC_BL,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataObject iedModel_LD1_GGIO1_AnIn1 = {
    DataObjectModelType,
    "AnIn1",
    (ModelNode*) &iedModel_LD1_GGIO1,
    NULL,
    (ModelNode*) &iedModel_LD1_GGIO1_AnIn1_mag,
    0
};

DataAttribute iedModel_LD1_GGIO1_AnIn1_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_LD1_GGIO1_AnIn1,
    (ModelNode*) &iedModel_LD1_GGIO1_AnIn1_q,
    (ModelNode*) &iedModel_LD1_GGIO1_AnIn1_mag_f,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD1_GGIO1_AnIn1_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_LD1_GGIO1_AnIn1_mag,
    NULL,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD1_GGIO1_AnIn1_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD1_GGIO1_AnIn1,
    (ModelNode*) &iedModel_LD1_GGIO1_AnIn1_t,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD1_GGIO1_AnIn1_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD1_GGIO1_AnIn1,
    (ModelNode*) &iedModel_LD1_GGIO1_AnIn1_subEna,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_GGIO1_AnIn1_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_LD1_GGIO1_AnIn1,
    (ModelNode*) &iedModel_LD1_GGIO1_AnIn1_subMag,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_GGIO1_AnIn1_subMag = {
    DataAttributeModelType,
    "subMag",
    (ModelNode*) &iedModel_LD1_GGIO1_AnIn1,
    (ModelNode*) &iedModel_LD1_GGIO1_AnIn1_subQ,
    (ModelNode*) &iedModel_LD1_GGIO1_AnIn1_subMag_f,
    0,
    IEC61850_FC_SV,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_GGIO1_AnIn1_subMag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_LD1_GGIO1_AnIn1_subMag,
    NULL,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_GGIO1_AnIn1_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_LD1_GGIO1_AnIn1,
    (ModelNode*) &iedModel_LD1_GGIO1_AnIn1_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_GGIO1_AnIn1_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_LD1_GGIO1_AnIn1,
    (ModelNode*) &iedModel_LD1_GGIO1_AnIn1_blkEna,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_LD1_GGIO1_AnIn1_blkEna = {
    DataAttributeModelType,
    "blkEna",
    (ModelNode*) &iedModel_LD1_GGIO1_AnIn1,
    NULL,
    NULL,
    0,
    IEC61850_FC_BL,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

extern ReportControlBlock iedModel_LD1_LLN0_report0;
extern ReportControlBlock iedModel_LD1_LLN0_report1;

ReportControlBlock iedModel_LD1_LLN0_report0 = {&iedModel_LD1_LLN0, "urcb01", "13e08c78", false, "", 1, 23, 247, 3000, 5000, {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, &iedModel_LD1_LLN0_report1};
ReportControlBlock iedModel_LD1_LLN0_report1 = {&iedModel_LD1_LLN0, "urcb02", "13e08c78", false, "", 1, 23, 247, 3000, 5000, {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, NULL};







IedModel iedModel = {
    "IED1",
    &iedModel_LD1,
    NULL,
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
}
