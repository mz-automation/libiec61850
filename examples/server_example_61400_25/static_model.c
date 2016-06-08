/*
 * static_model.c
 *
 * automatically generated from wtur.icd
 */
#include "static_model.h"

static void initializeValues();



LogicalDevice iedModel_WTG = {
    LogicalDeviceModelType,
    "WTG",
    (ModelNode*) &iedModel,
    NULL,
    (ModelNode*) &iedModel_WTG_LLN0
};

LogicalNode iedModel_WTG_LLN0 = {
    LogicalNodeModelType,
    "LLN0",
    (ModelNode*) &iedModel_WTG,
    (ModelNode*) &iedModel_WTG_LPHD1,
    (ModelNode*) &iedModel_WTG_LLN0_Mod,
};

DataObject iedModel_WTG_LLN0_Mod = {
    DataObjectModelType,
    "Mod",
    (ModelNode*) &iedModel_WTG_LLN0,
    (ModelNode*) &iedModel_WTG_LLN0_Beh,
    (ModelNode*) &iedModel_WTG_LLN0_Mod_Oper,
    0
};

DataAttribute iedModel_WTG_LLN0_Mod_Oper = {
    DataAttributeModelType,
    "Oper",
    (ModelNode*) &iedModel_WTG_LLN0_Mod,
    (ModelNode*) &iedModel_WTG_LLN0_Mod_stVal,
    (ModelNode*) &iedModel_WTG_LLN0_Mod_Oper_ctlVal,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_LLN0_Mod_Oper_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_WTG_LLN0_Mod_Oper,
    (ModelNode*) &iedModel_WTG_LLN0_Mod_Oper_operTm,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_INT32,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_LLN0_Mod_Oper_operTm = {
    DataAttributeModelType,
    "operTm",
    (ModelNode*) &iedModel_WTG_LLN0_Mod_Oper,
    (ModelNode*) &iedModel_WTG_LLN0_Mod_Oper_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_LLN0_Mod_Oper_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_WTG_LLN0_Mod_Oper,
    (ModelNode*) &iedModel_WTG_LLN0_Mod_Oper_ctlNum,
    (ModelNode*) &iedModel_WTG_LLN0_Mod_Oper_origin_orCat,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_LLN0_Mod_Oper_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_WTG_LLN0_Mod_Oper_origin,
    (ModelNode*) &iedModel_WTG_LLN0_Mod_Oper_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_LLN0_Mod_Oper_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_WTG_LLN0_Mod_Oper_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_LLN0_Mod_Oper_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_WTG_LLN0_Mod_Oper,
    (ModelNode*) &iedModel_WTG_LLN0_Mod_Oper_T,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_LLN0_Mod_Oper_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_WTG_LLN0_Mod_Oper,
    (ModelNode*) &iedModel_WTG_LLN0_Mod_Oper_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_LLN0_Mod_Oper_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_WTG_LLN0_Mod_Oper,
    (ModelNode*) &iedModel_WTG_LLN0_Mod_Oper_Check,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_LLN0_Mod_Oper_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_WTG_LLN0_Mod_Oper,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_LLN0_Mod_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_WTG_LLN0_Mod,
    (ModelNode*) &iedModel_WTG_LLN0_Mod_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_LLN0_Mod_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_WTG_LLN0_Mod,
    (ModelNode*) &iedModel_WTG_LLN0_Mod_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_LLN0_Mod_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_WTG_LLN0_Mod,
    (ModelNode*) &iedModel_WTG_LLN0_Mod_ctlModel,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_LLN0_Mod_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_WTG_LLN0_Mod,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_WTG_LLN0_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_WTG_LLN0,
    (ModelNode*) &iedModel_WTG_LLN0_Health,
    (ModelNode*) &iedModel_WTG_LLN0_Beh_stVal,
    0
};

DataAttribute iedModel_WTG_LLN0_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_WTG_LLN0_Beh,
    (ModelNode*) &iedModel_WTG_LLN0_Beh_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_LLN0_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_WTG_LLN0_Beh,
    (ModelNode*) &iedModel_WTG_LLN0_Beh_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_LLN0_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_WTG_LLN0_Beh,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_WTG_LLN0_Health = {
    DataObjectModelType,
    "Health",
    (ModelNode*) &iedModel_WTG_LLN0,
    (ModelNode*) &iedModel_WTG_LLN0_NamPlt,
    (ModelNode*) &iedModel_WTG_LLN0_Health_stVal,
    0
};

DataAttribute iedModel_WTG_LLN0_Health_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_WTG_LLN0_Health,
    (ModelNode*) &iedModel_WTG_LLN0_Health_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_LLN0_Health_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_WTG_LLN0_Health,
    (ModelNode*) &iedModel_WTG_LLN0_Health_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_LLN0_Health_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_WTG_LLN0_Health,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_WTG_LLN0_NamPlt = {
    DataObjectModelType,
    "NamPlt",
    (ModelNode*) &iedModel_WTG_LLN0,
    NULL,
    (ModelNode*) &iedModel_WTG_LLN0_NamPlt_vendor,
    0
};

DataAttribute iedModel_WTG_LLN0_NamPlt_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_WTG_LLN0_NamPlt,
    (ModelNode*) &iedModel_WTG_LLN0_NamPlt_swRev,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_LLN0_NamPlt_swRev = {
    DataAttributeModelType,
    "swRev",
    (ModelNode*) &iedModel_WTG_LLN0_NamPlt,
    (ModelNode*) &iedModel_WTG_LLN0_NamPlt_configRev,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_LLN0_NamPlt_configRev = {
    DataAttributeModelType,
    "configRev",
    (ModelNode*) &iedModel_WTG_LLN0_NamPlt,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

LogicalNode iedModel_WTG_LPHD1 = {
    LogicalNodeModelType,
    "LPHD1",
    (ModelNode*) &iedModel_WTG,
    (ModelNode*) &iedModel_WTG_WTUR1,
    (ModelNode*) &iedModel_WTG_LPHD1_NamPlt,
};

DataObject iedModel_WTG_LPHD1_NamPlt = {
    DataObjectModelType,
    "NamPlt",
    (ModelNode*) &iedModel_WTG_LPHD1,
    (ModelNode*) &iedModel_WTG_LPHD1_PhyNam,
    (ModelNode*) &iedModel_WTG_LPHD1_NamPlt_vendor,
    0
};

DataAttribute iedModel_WTG_LPHD1_NamPlt_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_WTG_LPHD1_NamPlt,
    (ModelNode*) &iedModel_WTG_LPHD1_NamPlt_swRev,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_LPHD1_NamPlt_swRev = {
    DataAttributeModelType,
    "swRev",
    (ModelNode*) &iedModel_WTG_LPHD1_NamPlt,
    (ModelNode*) &iedModel_WTG_LPHD1_NamPlt_configRev,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_LPHD1_NamPlt_configRev = {
    DataAttributeModelType,
    "configRev",
    (ModelNode*) &iedModel_WTG_LPHD1_NamPlt,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_WTG_LPHD1_PhyNam = {
    DataObjectModelType,
    "PhyNam",
    (ModelNode*) &iedModel_WTG_LPHD1,
    (ModelNode*) &iedModel_WTG_LPHD1_PhyHealth,
    (ModelNode*) &iedModel_WTG_LPHD1_PhyNam_vendor,
    0
};

DataAttribute iedModel_WTG_LPHD1_PhyNam_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_WTG_LPHD1_PhyNam,
    (ModelNode*) &iedModel_WTG_LPHD1_PhyNam_hwRev,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_LPHD1_PhyNam_hwRev = {
    DataAttributeModelType,
    "hwRev",
    (ModelNode*) &iedModel_WTG_LPHD1_PhyNam,
    (ModelNode*) &iedModel_WTG_LPHD1_PhyNam_swRev,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_LPHD1_PhyNam_swRev = {
    DataAttributeModelType,
    "swRev",
    (ModelNode*) &iedModel_WTG_LPHD1_PhyNam,
    (ModelNode*) &iedModel_WTG_LPHD1_PhyNam_serNum,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_LPHD1_PhyNam_serNum = {
    DataAttributeModelType,
    "serNum",
    (ModelNode*) &iedModel_WTG_LPHD1_PhyNam,
    (ModelNode*) &iedModel_WTG_LPHD1_PhyNam_model,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_LPHD1_PhyNam_model = {
    DataAttributeModelType,
    "model",
    (ModelNode*) &iedModel_WTG_LPHD1_PhyNam,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_WTG_LPHD1_PhyHealth = {
    DataObjectModelType,
    "PhyHealth",
    (ModelNode*) &iedModel_WTG_LPHD1,
    (ModelNode*) &iedModel_WTG_LPHD1_Proxy,
    (ModelNode*) &iedModel_WTG_LPHD1_PhyHealth_stVal,
    0
};

DataAttribute iedModel_WTG_LPHD1_PhyHealth_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_WTG_LPHD1_PhyHealth,
    (ModelNode*) &iedModel_WTG_LPHD1_PhyHealth_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_LPHD1_PhyHealth_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_WTG_LPHD1_PhyHealth,
    (ModelNode*) &iedModel_WTG_LPHD1_PhyHealth_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_LPHD1_PhyHealth_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_WTG_LPHD1_PhyHealth,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_WTG_LPHD1_Proxy = {
    DataObjectModelType,
    "Proxy",
    (ModelNode*) &iedModel_WTG_LPHD1,
    NULL,
    (ModelNode*) &iedModel_WTG_LPHD1_Proxy_stVal,
    0
};

DataAttribute iedModel_WTG_LPHD1_Proxy_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_WTG_LPHD1_Proxy,
    (ModelNode*) &iedModel_WTG_LPHD1_Proxy_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_LPHD1_Proxy_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_WTG_LPHD1_Proxy,
    (ModelNode*) &iedModel_WTG_LPHD1_Proxy_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_LPHD1_Proxy_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_WTG_LPHD1_Proxy,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

LogicalNode iedModel_WTG_WTUR1 = {
    LogicalNodeModelType,
    "WTUR1",
    (ModelNode*) &iedModel_WTG,
    NULL,
    (ModelNode*) &iedModel_WTG_WTUR1_NamPlt,
};

DataObject iedModel_WTG_WTUR1_NamPlt = {
    DataObjectModelType,
    "NamPlt",
    (ModelNode*) &iedModel_WTG_WTUR1,
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh,
    (ModelNode*) &iedModel_WTG_WTUR1_NamPlt_vendor,
    0
};

DataAttribute iedModel_WTG_WTUR1_NamPlt_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_WTG_WTUR1_NamPlt,
    (ModelNode*) &iedModel_WTG_WTUR1_NamPlt_swRev,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_NamPlt_swRev = {
    DataAttributeModelType,
    "swRev",
    (ModelNode*) &iedModel_WTG_WTUR1_NamPlt,
    (ModelNode*) &iedModel_WTG_WTUR1_NamPlt_configRev,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_NamPlt_configRev = {
    DataAttributeModelType,
    "configRev",
    (ModelNode*) &iedModel_WTG_WTUR1_NamPlt,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_WTG_WTUR1_TotWh = {
    DataObjectModelType,
    "TotWh",
    (ModelNode*) &iedModel_WTG_WTUR1,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt,
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_manRs,
    0
};

DataObject iedModel_WTG_WTUR1_TotWh_manRs = {
    DataObjectModelType,
    "manRs",
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh,
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_hisRs,
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_manRs_Oper,
    0
};

DataAttribute iedModel_WTG_WTUR1_TotWh_manRs_Oper = {
    DataAttributeModelType,
    "Oper",
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_manRs,
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_manRs_stVal,
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_manRs_Oper_ctlVal,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TotWh_manRs_Oper_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_manRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_manRs_Oper_operTm,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TotWh_manRs_Oper_operTm = {
    DataAttributeModelType,
    "operTm",
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_manRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_manRs_Oper_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TotWh_manRs_Oper_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_manRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_manRs_Oper_ctlNum,
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_manRs_Oper_origin_orCat,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TotWh_manRs_Oper_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_manRs_Oper_origin,
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_manRs_Oper_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TotWh_manRs_Oper_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_manRs_Oper_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TotWh_manRs_Oper_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_manRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_manRs_Oper_T,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TotWh_manRs_Oper_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_manRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_manRs_Oper_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TotWh_manRs_Oper_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_manRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_manRs_Oper_Check,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TotWh_manRs_Oper_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_manRs_Oper,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TotWh_manRs_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_manRs,
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_manRs_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TotWh_manRs_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_manRs,
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_manRs_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TotWh_manRs_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_manRs,
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_manRs_ctlModel,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TotWh_manRs_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_manRs,
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_manRs_sboTimeout,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TotWh_manRs_sboTimeout = {
    DataAttributeModelType,
    "sboTimeout",
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_manRs,
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_manRs_sboClass,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TotWh_manRs_sboClass = {
    DataAttributeModelType,
    "sboClass",
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_manRs,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_WTG_WTUR1_TotWh_hisRs = {
    DataObjectModelType,
    "hisRs",
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh,
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_actCtVal,
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_hisRs_Oper,
    0
};

DataAttribute iedModel_WTG_WTUR1_TotWh_hisRs_Oper = {
    DataAttributeModelType,
    "Oper",
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_hisRs,
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_hisRs_stVal,
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_hisRs_Oper_ctlVal,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TotWh_hisRs_Oper_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_hisRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_hisRs_Oper_operTm,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_INT32,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TotWh_hisRs_Oper_operTm = {
    DataAttributeModelType,
    "operTm",
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_hisRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_hisRs_Oper_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TotWh_hisRs_Oper_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_hisRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_hisRs_Oper_ctlNum,
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_hisRs_Oper_origin_orCat,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TotWh_hisRs_Oper_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_hisRs_Oper_origin,
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_hisRs_Oper_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TotWh_hisRs_Oper_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_hisRs_Oper_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TotWh_hisRs_Oper_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_hisRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_hisRs_Oper_T,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TotWh_hisRs_Oper_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_hisRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_hisRs_Oper_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TotWh_hisRs_Oper_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_hisRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_hisRs_Oper_Check,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TotWh_hisRs_Oper_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_hisRs_Oper,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TotWh_hisRs_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_hisRs,
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_hisRs_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TotWh_hisRs_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_hisRs,
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_hisRs_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TotWh_hisRs_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_hisRs,
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_hisRs_ctlModel,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TotWh_hisRs_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_hisRs,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_WTG_WTUR1_TotWh_actCtVal = {
    DataObjectModelType,
    "actCtVal",
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh,
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_oldCtVal,
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_actCtVal_stVal,
    0
};

DataAttribute iedModel_WTG_WTUR1_TotWh_actCtVal_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_actCtVal,
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_actCtVal_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TotWh_actCtVal_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_actCtVal,
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_actCtVal_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TotWh_actCtVal_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_actCtVal,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_WTG_WTUR1_TotWh_oldCtVal = {
    DataObjectModelType,
    "oldCtVal",
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh,
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_ctTot,
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_oldCtVal_stVal,
    0
};

DataAttribute iedModel_WTG_WTUR1_TotWh_oldCtVal_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_oldCtVal,
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_oldCtVal_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TotWh_oldCtVal_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_oldCtVal,
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_oldCtVal_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TotWh_oldCtVal_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_oldCtVal,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TotWh_ctTot = {
    DataAttributeModelType,
    "ctTot",
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh,
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_dly,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TotWh_dly = {
    DataAttributeModelType,
    "dly",
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh,
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_mly,
    NULL,
    32,
    IEC61850_FC_ST,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TotWh_mly = {
    DataAttributeModelType,
    "mly",
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh,
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_yly,
    NULL,
    13,
    IEC61850_FC_ST,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TotWh_yly = {
    DataAttributeModelType,
    "yly",
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh,
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_tot,
    NULL,
    21,
    IEC61850_FC_ST,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TotWh_tot = {
    DataAttributeModelType,
    "tot",
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh,
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh_rsPer,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TotWh_rsPer = {
    DataAttributeModelType,
    "rsPer",
    (ModelNode*) &iedModel_WTG_WTUR1_TotWh,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_WTG_WTUR1_TurSt = {
    DataObjectModelType,
    "TurSt",
    (ModelNode*) &iedModel_WTG_WTUR1,
    (ModelNode*) &iedModel_WTG_WTUR1_W,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_actSt,
    0
};

DataObject iedModel_WTG_WTUR1_TurSt_actSt = {
    DataObjectModelType,
    "actSt",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_oldSt,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_actSt_stVal,
    0
};

DataAttribute iedModel_WTG_WTUR1_TurSt_actSt_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_actSt,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_actSt_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_actSt_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_actSt,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_actSt_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_actSt_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_actSt,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_WTG_WTUR1_TurSt_oldSt = {
    DataObjectModelType,
    "oldSt",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_oldSt_stVal,
    0
};

DataAttribute iedModel_WTG_WTUR1_TurSt_oldSt_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_oldSt,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_oldSt_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_oldSt_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_oldSt,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_oldSt_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_oldSt_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_oldSt,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_WTG_WTUR1_TurSt_stTm = {
    DataObjectModelType,
    "stTm",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_manRs,
    0
};

DataObject iedModel_WTG_WTUR1_TurSt_stTm_manRs = {
    DataObjectModelType,
    "manRs",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_hisRs,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_manRs_Oper,
    0
};

DataAttribute iedModel_WTG_WTUR1_TurSt_stTm_manRs_Oper = {
    DataAttributeModelType,
    "Oper",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_manRs,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_manRs_stVal,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_manRs_Oper_ctlVal,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stTm_manRs_Oper_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_manRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_manRs_Oper_operTm,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stTm_manRs_Oper_operTm = {
    DataAttributeModelType,
    "operTm",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_manRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_manRs_Oper_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stTm_manRs_Oper_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_manRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_manRs_Oper_ctlNum,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_manRs_Oper_origin_orCat,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stTm_manRs_Oper_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_manRs_Oper_origin,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_manRs_Oper_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stTm_manRs_Oper_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_manRs_Oper_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stTm_manRs_Oper_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_manRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_manRs_Oper_T,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stTm_manRs_Oper_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_manRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_manRs_Oper_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stTm_manRs_Oper_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_manRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_manRs_Oper_Check,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stTm_manRs_Oper_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_manRs_Oper,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stTm_manRs_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_manRs,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_manRs_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stTm_manRs_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_manRs,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_manRs_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stTm_manRs_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_manRs,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_manRs_ctlModel,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stTm_manRs_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_manRs,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_manRs_sboTimeout,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stTm_manRs_sboTimeout = {
    DataAttributeModelType,
    "sboTimeout",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_manRs,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_manRs_sboClass,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stTm_manRs_sboClass = {
    DataAttributeModelType,
    "sboClass",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_manRs,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_WTG_WTUR1_TurSt_stTm_hisRs = {
    DataObjectModelType,
    "hisRs",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_actTmVal,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_hisRs_Oper,
    0
};

DataAttribute iedModel_WTG_WTUR1_TurSt_stTm_hisRs_Oper = {
    DataAttributeModelType,
    "Oper",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_hisRs,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_hisRs_stVal,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_hisRs_Oper_ctlVal,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stTm_hisRs_Oper_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_hisRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_hisRs_Oper_operTm,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_INT32,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stTm_hisRs_Oper_operTm = {
    DataAttributeModelType,
    "operTm",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_hisRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_hisRs_Oper_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stTm_hisRs_Oper_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_hisRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_hisRs_Oper_ctlNum,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_hisRs_Oper_origin_orCat,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stTm_hisRs_Oper_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_hisRs_Oper_origin,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_hisRs_Oper_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stTm_hisRs_Oper_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_hisRs_Oper_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stTm_hisRs_Oper_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_hisRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_hisRs_Oper_T,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stTm_hisRs_Oper_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_hisRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_hisRs_Oper_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stTm_hisRs_Oper_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_hisRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_hisRs_Oper_Check,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stTm_hisRs_Oper_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_hisRs_Oper,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stTm_hisRs_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_hisRs,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_hisRs_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stTm_hisRs_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_hisRs,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_hisRs_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stTm_hisRs_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_hisRs,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_hisRs_ctlModel,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stTm_hisRs_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_hisRs,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_WTG_WTUR1_TurSt_stTm_actTmVal = {
    DataObjectModelType,
    "actTmVal",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_oldTmVal,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_actTmVal_stVal,
    0
};

DataAttribute iedModel_WTG_WTUR1_TurSt_stTm_actTmVal_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_actTmVal,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_actTmVal_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stTm_actTmVal_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_actTmVal,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_actTmVal_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stTm_actTmVal_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_actTmVal,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_WTG_WTUR1_TurSt_stTm_oldTmVal = {
    DataObjectModelType,
    "oldTmVal",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_tmTot,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_oldTmVal_stVal,
    0
};

DataAttribute iedModel_WTG_WTUR1_TurSt_stTm_oldTmVal_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_oldTmVal,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_oldTmVal_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stTm_oldTmVal_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_oldTmVal,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_oldTmVal_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stTm_oldTmVal_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_oldTmVal,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stTm_tmTot = {
    DataAttributeModelType,
    "tmTot",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_dly,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stTm_dly = {
    DataAttributeModelType,
    "dly",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_mly,
    NULL,
    32,
    IEC61850_FC_ST,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stTm_mly = {
    DataAttributeModelType,
    "mly",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_yly,
    NULL,
    13,
    IEC61850_FC_ST,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stTm_yly = {
    DataAttributeModelType,
    "yly",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_tot,
    NULL,
    21,
    IEC61850_FC_ST,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stTm_tot = {
    DataAttributeModelType,
    "tot",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm_rsPer,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stTm_rsPer = {
    DataAttributeModelType,
    "rsPer",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stTm,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_WTG_WTUR1_TurSt_stCt = {
    DataObjectModelType,
    "stCt",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_preTmms,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_manRs,
    0
};

DataObject iedModel_WTG_WTUR1_TurSt_stCt_manRs = {
    DataObjectModelType,
    "manRs",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_hisRs,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_manRs_Oper,
    0
};

DataAttribute iedModel_WTG_WTUR1_TurSt_stCt_manRs_Oper = {
    DataAttributeModelType,
    "Oper",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_manRs,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_manRs_stVal,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_manRs_Oper_ctlVal,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stCt_manRs_Oper_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_manRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_manRs_Oper_operTm,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stCt_manRs_Oper_operTm = {
    DataAttributeModelType,
    "operTm",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_manRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_manRs_Oper_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stCt_manRs_Oper_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_manRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_manRs_Oper_ctlNum,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_manRs_Oper_origin_orCat,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stCt_manRs_Oper_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_manRs_Oper_origin,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_manRs_Oper_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stCt_manRs_Oper_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_manRs_Oper_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stCt_manRs_Oper_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_manRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_manRs_Oper_T,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stCt_manRs_Oper_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_manRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_manRs_Oper_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stCt_manRs_Oper_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_manRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_manRs_Oper_Check,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stCt_manRs_Oper_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_manRs_Oper,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stCt_manRs_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_manRs,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_manRs_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stCt_manRs_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_manRs,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_manRs_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stCt_manRs_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_manRs,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_manRs_ctlModel,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stCt_manRs_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_manRs,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_manRs_sboTimeout,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stCt_manRs_sboTimeout = {
    DataAttributeModelType,
    "sboTimeout",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_manRs,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_manRs_sboClass,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stCt_manRs_sboClass = {
    DataAttributeModelType,
    "sboClass",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_manRs,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_WTG_WTUR1_TurSt_stCt_hisRs = {
    DataObjectModelType,
    "hisRs",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_actCtVal,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_hisRs_Oper,
    0
};

DataAttribute iedModel_WTG_WTUR1_TurSt_stCt_hisRs_Oper = {
    DataAttributeModelType,
    "Oper",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_hisRs,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_hisRs_stVal,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_hisRs_Oper_ctlVal,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stCt_hisRs_Oper_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_hisRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_hisRs_Oper_operTm,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_INT32,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stCt_hisRs_Oper_operTm = {
    DataAttributeModelType,
    "operTm",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_hisRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_hisRs_Oper_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stCt_hisRs_Oper_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_hisRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_hisRs_Oper_ctlNum,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_hisRs_Oper_origin_orCat,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stCt_hisRs_Oper_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_hisRs_Oper_origin,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_hisRs_Oper_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stCt_hisRs_Oper_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_hisRs_Oper_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stCt_hisRs_Oper_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_hisRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_hisRs_Oper_T,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stCt_hisRs_Oper_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_hisRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_hisRs_Oper_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stCt_hisRs_Oper_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_hisRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_hisRs_Oper_Check,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stCt_hisRs_Oper_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_hisRs_Oper,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stCt_hisRs_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_hisRs,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_hisRs_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stCt_hisRs_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_hisRs,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_hisRs_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stCt_hisRs_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_hisRs,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_hisRs_ctlModel,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stCt_hisRs_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_hisRs,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_WTG_WTUR1_TurSt_stCt_actCtVal = {
    DataObjectModelType,
    "actCtVal",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_oldCtVal,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_actCtVal_stVal,
    0
};

DataAttribute iedModel_WTG_WTUR1_TurSt_stCt_actCtVal_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_actCtVal,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_actCtVal_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stCt_actCtVal_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_actCtVal,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_actCtVal_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stCt_actCtVal_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_actCtVal,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_WTG_WTUR1_TurSt_stCt_oldCtVal = {
    DataObjectModelType,
    "oldCtVal",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_ctTot,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_oldCtVal_stVal,
    0
};

DataAttribute iedModel_WTG_WTUR1_TurSt_stCt_oldCtVal_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_oldCtVal,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_oldCtVal_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stCt_oldCtVal_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_oldCtVal,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_oldCtVal_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stCt_oldCtVal_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_oldCtVal,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stCt_ctTot = {
    DataAttributeModelType,
    "ctTot",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_dly,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stCt_dly = {
    DataAttributeModelType,
    "dly",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_mly,
    NULL,
    32,
    IEC61850_FC_ST,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stCt_mly = {
    DataAttributeModelType,
    "mly",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_yly,
    NULL,
    13,
    IEC61850_FC_ST,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stCt_yly = {
    DataAttributeModelType,
    "yly",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_tot,
    NULL,
    21,
    IEC61850_FC_ST,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stCt_tot = {
    DataAttributeModelType,
    "tot",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt_rsPer,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_stCt_rsPer = {
    DataAttributeModelType,
    "rsPer",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_stCt,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_preTmms = {
    DataAttributeModelType,
    "preTmms",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_pstTmms,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_pstTmms = {
    DataAttributeModelType,
    "pstTmms",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_smpTmms,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_smpTmms = {
    DataAttributeModelType,
    "smpTmms",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt,
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt_datSetMx,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT16U,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_TurSt_datSetMx = {
    DataAttributeModelType,
    "datSetMx",
    (ModelNode*) &iedModel_WTG_WTUR1_TurSt,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_VISIBLE_STRING_129,
    0,
    NULL,
    0};

DataObject iedModel_WTG_WTUR1_W = {
    DataObjectModelType,
    "W",
    (ModelNode*) &iedModel_WTG_WTUR1,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp,
    (ModelNode*) &iedModel_WTG_WTUR1_W_instMag,
    0
};

DataAttribute iedModel_WTG_WTUR1_W_instMag = {
    DataAttributeModelType,
    "instMag",
    (ModelNode*) &iedModel_WTG_WTUR1_W,
    (ModelNode*) &iedModel_WTG_WTUR1_W_mag,
    (ModelNode*) &iedModel_WTG_WTUR1_W_instMag_i,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_W_instMag_i = {
    DataAttributeModelType,
    "i",
    (ModelNode*) &iedModel_WTG_WTUR1_W_instMag,
    (ModelNode*) &iedModel_WTG_WTUR1_W_instMag_f,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_INT32,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_W_instMag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_WTG_WTUR1_W_instMag,
    NULL,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_W_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_WTG_WTUR1_W,
    (ModelNode*) &iedModel_WTG_WTUR1_W_q,
    (ModelNode*) &iedModel_WTG_WTUR1_W_mag_i,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_W_mag_i = {
    DataAttributeModelType,
    "i",
    (ModelNode*) &iedModel_WTG_WTUR1_W_mag,
    (ModelNode*) &iedModel_WTG_WTUR1_W_mag_f,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_W_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_WTG_WTUR1_W_mag,
    NULL,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_W_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_WTG_WTUR1_W,
    (ModelNode*) &iedModel_WTG_WTUR1_W_t,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_W_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_WTG_WTUR1_W,
    NULL,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_WTG_WTUR1_SetTurOp = {
    DataObjectModelType,
    "SetTurOp",
    (ModelNode*) &iedModel_WTG_WTUR1,
    NULL,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_actSt,
    0
};

DataObject iedModel_WTG_WTUR1_SetTurOp_actSt = {
    DataObjectModelType,
    "actSt",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_oldSt,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_actSt_Oper,
    0
};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_actSt_Oper = {
    DataAttributeModelType,
    "Oper",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_actSt,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_actSt_stVal,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_actSt_Oper_ctlVal,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_actSt_Oper_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_actSt_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_actSt_Oper_operTm,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_INT32,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_actSt_Oper_operTm = {
    DataAttributeModelType,
    "operTm",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_actSt_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_actSt_Oper_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_actSt_Oper_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_actSt_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_actSt_Oper_ctlNum,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_actSt_Oper_origin_orCat,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_actSt_Oper_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_actSt_Oper_origin,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_actSt_Oper_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_actSt_Oper_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_actSt_Oper_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_actSt_Oper_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_actSt_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_actSt_Oper_T,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_actSt_Oper_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_actSt_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_actSt_Oper_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_actSt_Oper_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_actSt_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_actSt_Oper_Check,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_actSt_Oper_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_actSt_Oper,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_actSt_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_actSt,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_actSt_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_actSt_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_actSt,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_actSt_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_actSt_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_actSt,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_actSt_ctlModel,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_actSt_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_actSt,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_WTG_WTUR1_SetTurOp_oldSt = {
    DataObjectModelType,
    "oldSt",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_oldSt_stVal,
    0
};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_oldSt_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_oldSt,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_oldSt_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_oldSt_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_oldSt,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_oldSt_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_oldSt_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_oldSt,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_WTG_WTUR1_SetTurOp_cmTm = {
    DataObjectModelType,
    "cmTm",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_manRs,
    0
};

DataObject iedModel_WTG_WTUR1_SetTurOp_cmTm_manRs = {
    DataObjectModelType,
    "manRs",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_hisRs,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_manRs_Oper,
    0
};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmTm_manRs_Oper = {
    DataAttributeModelType,
    "Oper",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_manRs,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_manRs_stVal,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_manRs_Oper_ctlVal,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmTm_manRs_Oper_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_manRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_manRs_Oper_operTm,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmTm_manRs_Oper_operTm = {
    DataAttributeModelType,
    "operTm",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_manRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_manRs_Oper_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmTm_manRs_Oper_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_manRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_manRs_Oper_ctlNum,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_manRs_Oper_origin_orCat,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmTm_manRs_Oper_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_manRs_Oper_origin,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_manRs_Oper_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmTm_manRs_Oper_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_manRs_Oper_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmTm_manRs_Oper_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_manRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_manRs_Oper_T,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmTm_manRs_Oper_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_manRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_manRs_Oper_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmTm_manRs_Oper_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_manRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_manRs_Oper_Check,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmTm_manRs_Oper_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_manRs_Oper,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmTm_manRs_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_manRs,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_manRs_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmTm_manRs_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_manRs,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_manRs_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmTm_manRs_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_manRs,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_manRs_ctlModel,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmTm_manRs_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_manRs,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_manRs_sboTimeout,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmTm_manRs_sboTimeout = {
    DataAttributeModelType,
    "sboTimeout",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_manRs,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_manRs_sboClass,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmTm_manRs_sboClass = {
    DataAttributeModelType,
    "sboClass",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_manRs,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_WTG_WTUR1_SetTurOp_cmTm_hisRs = {
    DataObjectModelType,
    "hisRs",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_actTmVal,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_hisRs_Oper,
    0
};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmTm_hisRs_Oper = {
    DataAttributeModelType,
    "Oper",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_hisRs,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_hisRs_stVal,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_hisRs_Oper_ctlVal,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmTm_hisRs_Oper_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_hisRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_hisRs_Oper_operTm,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_INT32,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmTm_hisRs_Oper_operTm = {
    DataAttributeModelType,
    "operTm",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_hisRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_hisRs_Oper_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmTm_hisRs_Oper_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_hisRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_hisRs_Oper_ctlNum,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_hisRs_Oper_origin_orCat,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmTm_hisRs_Oper_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_hisRs_Oper_origin,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_hisRs_Oper_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmTm_hisRs_Oper_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_hisRs_Oper_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmTm_hisRs_Oper_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_hisRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_hisRs_Oper_T,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmTm_hisRs_Oper_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_hisRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_hisRs_Oper_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmTm_hisRs_Oper_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_hisRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_hisRs_Oper_Check,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmTm_hisRs_Oper_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_hisRs_Oper,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmTm_hisRs_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_hisRs,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_hisRs_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmTm_hisRs_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_hisRs,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_hisRs_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmTm_hisRs_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_hisRs,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_hisRs_ctlModel,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmTm_hisRs_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_hisRs,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_WTG_WTUR1_SetTurOp_cmTm_actTmVal = {
    DataObjectModelType,
    "actTmVal",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_oldTmVal,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_actTmVal_stVal,
    0
};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmTm_actTmVal_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_actTmVal,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_actTmVal_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmTm_actTmVal_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_actTmVal,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_actTmVal_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmTm_actTmVal_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_actTmVal,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_WTG_WTUR1_SetTurOp_cmTm_oldTmVal = {
    DataObjectModelType,
    "oldTmVal",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_tmTot,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_oldTmVal_stVal,
    0
};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmTm_oldTmVal_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_oldTmVal,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_oldTmVal_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmTm_oldTmVal_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_oldTmVal,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_oldTmVal_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmTm_oldTmVal_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_oldTmVal,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmTm_tmTot = {
    DataAttributeModelType,
    "tmTot",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_dly,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmTm_dly = {
    DataAttributeModelType,
    "dly",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_mly,
    NULL,
    32,
    IEC61850_FC_ST,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmTm_mly = {
    DataAttributeModelType,
    "mly",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_yly,
    NULL,
    13,
    IEC61850_FC_ST,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmTm_yly = {
    DataAttributeModelType,
    "yly",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_tot,
    NULL,
    21,
    IEC61850_FC_ST,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmTm_tot = {
    DataAttributeModelType,
    "tot",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm_rsPer,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmTm_rsPer = {
    DataAttributeModelType,
    "rsPer",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmTm,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_WTG_WTUR1_SetTurOp_cmCt = {
    DataObjectModelType,
    "cmCt",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmAcs,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_manRs,
    0
};

DataObject iedModel_WTG_WTUR1_SetTurOp_cmCt_manRs = {
    DataObjectModelType,
    "manRs",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_hisRs,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_manRs_Oper,
    0
};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmCt_manRs_Oper = {
    DataAttributeModelType,
    "Oper",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_manRs,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_manRs_stVal,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_manRs_Oper_ctlVal,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmCt_manRs_Oper_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_manRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_manRs_Oper_operTm,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmCt_manRs_Oper_operTm = {
    DataAttributeModelType,
    "operTm",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_manRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_manRs_Oper_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmCt_manRs_Oper_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_manRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_manRs_Oper_ctlNum,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_manRs_Oper_origin_orCat,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmCt_manRs_Oper_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_manRs_Oper_origin,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_manRs_Oper_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmCt_manRs_Oper_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_manRs_Oper_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmCt_manRs_Oper_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_manRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_manRs_Oper_T,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmCt_manRs_Oper_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_manRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_manRs_Oper_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmCt_manRs_Oper_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_manRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_manRs_Oper_Check,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmCt_manRs_Oper_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_manRs_Oper,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmCt_manRs_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_manRs,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_manRs_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmCt_manRs_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_manRs,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_manRs_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmCt_manRs_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_manRs,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_manRs_ctlModel,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmCt_manRs_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_manRs,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_manRs_sboTimeout,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmCt_manRs_sboTimeout = {
    DataAttributeModelType,
    "sboTimeout",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_manRs,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_manRs_sboClass,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmCt_manRs_sboClass = {
    DataAttributeModelType,
    "sboClass",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_manRs,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_WTG_WTUR1_SetTurOp_cmCt_hisRs = {
    DataObjectModelType,
    "hisRs",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_actCtVal,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_hisRs_Oper,
    0
};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmCt_hisRs_Oper = {
    DataAttributeModelType,
    "Oper",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_hisRs,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_hisRs_stVal,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_hisRs_Oper_ctlVal,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmCt_hisRs_Oper_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_hisRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_hisRs_Oper_operTm,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_INT32,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmCt_hisRs_Oper_operTm = {
    DataAttributeModelType,
    "operTm",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_hisRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_hisRs_Oper_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmCt_hisRs_Oper_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_hisRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_hisRs_Oper_ctlNum,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_hisRs_Oper_origin_orCat,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmCt_hisRs_Oper_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_hisRs_Oper_origin,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_hisRs_Oper_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmCt_hisRs_Oper_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_hisRs_Oper_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmCt_hisRs_Oper_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_hisRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_hisRs_Oper_T,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmCt_hisRs_Oper_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_hisRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_hisRs_Oper_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmCt_hisRs_Oper_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_hisRs_Oper,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_hisRs_Oper_Check,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmCt_hisRs_Oper_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_hisRs_Oper,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmCt_hisRs_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_hisRs,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_hisRs_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmCt_hisRs_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_hisRs,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_hisRs_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmCt_hisRs_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_hisRs,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_hisRs_ctlModel,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmCt_hisRs_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_hisRs,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_WTG_WTUR1_SetTurOp_cmCt_actCtVal = {
    DataObjectModelType,
    "actCtVal",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_oldCtVal,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_actCtVal_stVal,
    0
};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmCt_actCtVal_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_actCtVal,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_actCtVal_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmCt_actCtVal_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_actCtVal,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_actCtVal_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmCt_actCtVal_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_actCtVal,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_WTG_WTUR1_SetTurOp_cmCt_oldCtVal = {
    DataObjectModelType,
    "oldCtVal",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_ctTot,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_oldCtVal_stVal,
    0
};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmCt_oldCtVal_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_oldCtVal,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_oldCtVal_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmCt_oldCtVal_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_oldCtVal,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_oldCtVal_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmCt_oldCtVal_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_oldCtVal,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmCt_ctTot = {
    DataAttributeModelType,
    "ctTot",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_dly,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmCt_dly = {
    DataAttributeModelType,
    "dly",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_mly,
    NULL,
    32,
    IEC61850_FC_ST,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmCt_mly = {
    DataAttributeModelType,
    "mly",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_yly,
    NULL,
    13,
    IEC61850_FC_ST,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmCt_yly = {
    DataAttributeModelType,
    "yly",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_tot,
    NULL,
    21,
    IEC61850_FC_ST,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmCt_tot = {
    DataAttributeModelType,
    "tot",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt,
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt_rsPer,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmCt_rsPer = {
    DataAttributeModelType,
    "rsPer",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp_cmCt,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_WTG_WTUR1_SetTurOp_cmAcs = {
    DataAttributeModelType,
    "cmAcs",
    (ModelNode*) &iedModel_WTG_WTUR1_SetTurOp,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT8U,
    0,
    NULL,
    0};









IedModel iedModel = {
    "WIND",
    &iedModel_WTG,
    NULL,
    NULL,
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
