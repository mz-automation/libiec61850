/*
 * static_model.c
 *
 * automatically generated from sg_demo.icd
 */
#include <stdlib.h>
#include "iec61850_model.h"

extern IedModel iedModel;
static void initializeValues();
extern LogicalDevice iedModel_PROT;
extern LogicalNode   iedModel_PROT_LLN0;
extern DataObject    iedModel_PROT_LLN0_Mod;
extern DataAttribute iedModel_PROT_LLN0_Mod_Oper;
extern DataAttribute iedModel_PROT_LLN0_Mod_Oper_ctlVal;
extern DataAttribute iedModel_PROT_LLN0_Mod_Oper_operTm;
extern DataAttribute iedModel_PROT_LLN0_Mod_Oper_origin;
extern DataAttribute iedModel_PROT_LLN0_Mod_Oper_origin_orCat;
extern DataAttribute iedModel_PROT_LLN0_Mod_Oper_origin_orIdent;
extern DataAttribute iedModel_PROT_LLN0_Mod_Oper_ctlNum;
extern DataAttribute iedModel_PROT_LLN0_Mod_Oper_T;
extern DataAttribute iedModel_PROT_LLN0_Mod_Oper_Test;
extern DataAttribute iedModel_PROT_LLN0_Mod_Oper_Check;
extern DataAttribute iedModel_PROT_LLN0_Mod_stVal;
extern DataAttribute iedModel_PROT_LLN0_Mod_q;
extern DataAttribute iedModel_PROT_LLN0_Mod_t;
extern DataAttribute iedModel_PROT_LLN0_Mod_ctlModel;
extern DataObject    iedModel_PROT_LLN0_Beh;
extern DataAttribute iedModel_PROT_LLN0_Beh_stVal;
extern DataAttribute iedModel_PROT_LLN0_Beh_q;
extern DataAttribute iedModel_PROT_LLN0_Beh_t;
extern DataObject    iedModel_PROT_LLN0_Health;
extern DataAttribute iedModel_PROT_LLN0_Health_stVal;
extern DataAttribute iedModel_PROT_LLN0_Health_q;
extern DataAttribute iedModel_PROT_LLN0_Health_t;
extern DataObject    iedModel_PROT_LLN0_NamPlt;
extern DataAttribute iedModel_PROT_LLN0_NamPlt_vendor;
extern DataAttribute iedModel_PROT_LLN0_NamPlt_swRev;
extern DataAttribute iedModel_PROT_LLN0_NamPlt_d;
extern DataAttribute iedModel_PROT_LLN0_NamPlt_configRev;
extern DataAttribute iedModel_PROT_LLN0_NamPlt_ldNs;
extern LogicalNode   iedModel_PROT_LPHD1;
extern DataObject    iedModel_PROT_LPHD1_PhyNam;
extern DataAttribute iedModel_PROT_LPHD1_PhyNam_vendor;
extern DataAttribute iedModel_PROT_LPHD1_PhyNam_hwRev;
extern DataAttribute iedModel_PROT_LPHD1_PhyNam_swRev;
extern DataAttribute iedModel_PROT_LPHD1_PhyNam_serNum;
extern DataAttribute iedModel_PROT_LPHD1_PhyNam_model;
extern DataAttribute iedModel_PROT_LPHD1_PhyNam_location;
extern DataObject    iedModel_PROT_LPHD1_PhyHealth;
extern DataAttribute iedModel_PROT_LPHD1_PhyHealth_stVal;
extern DataAttribute iedModel_PROT_LPHD1_PhyHealth_q;
extern DataAttribute iedModel_PROT_LPHD1_PhyHealth_t;
extern DataObject    iedModel_PROT_LPHD1_Proxy;
extern DataAttribute iedModel_PROT_LPHD1_Proxy_stVal;
extern DataAttribute iedModel_PROT_LPHD1_Proxy_q;
extern DataAttribute iedModel_PROT_LPHD1_Proxy_t;
extern LogicalNode   iedModel_PROT_PTOC1;
extern DataObject    iedModel_PROT_PTOC1_Beh;
extern DataAttribute iedModel_PROT_PTOC1_Beh_stVal;
extern DataAttribute iedModel_PROT_PTOC1_Beh_q;
extern DataAttribute iedModel_PROT_PTOC1_Beh_t;
extern DataObject    iedModel_PROT_PTOC1_Mod;
extern DataAttribute iedModel_PROT_PTOC1_Mod_Oper;
extern DataAttribute iedModel_PROT_PTOC1_Mod_Oper_ctlVal;
extern DataAttribute iedModel_PROT_PTOC1_Mod_Oper_operTm;
extern DataAttribute iedModel_PROT_PTOC1_Mod_Oper_origin;
extern DataAttribute iedModel_PROT_PTOC1_Mod_Oper_origin_orCat;
extern DataAttribute iedModel_PROT_PTOC1_Mod_Oper_origin_orIdent;
extern DataAttribute iedModel_PROT_PTOC1_Mod_Oper_ctlNum;
extern DataAttribute iedModel_PROT_PTOC1_Mod_Oper_T;
extern DataAttribute iedModel_PROT_PTOC1_Mod_Oper_Test;
extern DataAttribute iedModel_PROT_PTOC1_Mod_Oper_Check;
extern DataAttribute iedModel_PROT_PTOC1_Mod_stVal;
extern DataAttribute iedModel_PROT_PTOC1_Mod_q;
extern DataAttribute iedModel_PROT_PTOC1_Mod_t;
extern DataAttribute iedModel_PROT_PTOC1_Mod_ctlModel;
extern DataObject    iedModel_PROT_PTOC1_Str;
extern DataAttribute iedModel_PROT_PTOC1_Str_general;
extern DataAttribute iedModel_PROT_PTOC1_Str_dirGeneral;
extern DataAttribute iedModel_PROT_PTOC1_Str_q;
extern DataAttribute iedModel_PROT_PTOC1_Str_t;
extern DataObject    iedModel_PROT_PTOC1_Op;
extern DataAttribute iedModel_PROT_PTOC1_Op_general;
extern DataAttribute iedModel_PROT_PTOC1_Op_q;
extern DataAttribute iedModel_PROT_PTOC1_Op_t;
extern DataObject    iedModel_PROT_PTOC1_StrVal;
extern DataAttribute iedModel_PROT_PTOC1_StrVal_setMag;
extern DataAttribute iedModel_PROT_PTOC1_StrVal_setMag_f;
extern DataAttribute iedModel_SE_PROT_PTOC1_StrVal_setMag;
extern DataAttribute iedModel_SE_PROT_PTOC1_StrVal_setMag_f;
extern DataObject    iedModel_PROT_PTOC1_OpDlTmms;
extern DataAttribute iedModel_PROT_PTOC1_OpDlTmms_setVal;
extern DataAttribute iedModel_SE_PROT_PTOC1_OpDlTmms_setVal;
extern DataObject    iedModel_PROT_PTOC1_RsDlTmms;
extern DataAttribute iedModel_PROT_PTOC1_RsDlTmms_setVal;
extern DataAttribute iedModel_SE_PROT_PTOC1_RsDlTmms_setVal;
extern DataObject    iedModel_PROT_PTOC1_RstTms;
extern DataAttribute iedModel_PROT_PTOC1_RstTms_setVal;
extern DataAttribute iedModel_SE_PROT_PTOC1_RstTms_setVal;



LogicalDevice iedModel_PROT = {
    LogicalDeviceModelType,
    "PROT",
    (ModelNode*) &iedModel,
    NULL,
    (ModelNode*) &iedModel_PROT_LLN0
};

LogicalNode iedModel_PROT_LLN0 = {
    LogicalNodeModelType,
    "LLN0",
    (ModelNode*) &iedModel_PROT,
    (ModelNode*) &iedModel_PROT_LPHD1,
    (ModelNode*) &iedModel_PROT_LLN0_Mod,
};

DataObject iedModel_PROT_LLN0_Mod = {
    DataObjectModelType,
    "Mod",
    (ModelNode*) &iedModel_PROT_LLN0,
    (ModelNode*) &iedModel_PROT_LLN0_Beh,
    (ModelNode*) &iedModel_PROT_LLN0_Mod_Oper,
    0
};

DataAttribute iedModel_PROT_LLN0_Mod_Oper = {
    DataAttributeModelType,
    "Oper",
    (ModelNode*) &iedModel_PROT_LLN0_Mod,
    (ModelNode*) &iedModel_PROT_LLN0_Mod_stVal,
    (ModelNode*) &iedModel_PROT_LLN0_Mod_Oper_ctlVal,
    0,
    IEC61850_FC_CO,
    CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_LLN0_Mod_Oper_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_PROT_LLN0_Mod_Oper,
    (ModelNode*) &iedModel_PROT_LLN0_Mod_Oper_operTm,
    NULL,
    0,
    IEC61850_FC_CO,
    ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_LLN0_Mod_Oper_operTm = {
    DataAttributeModelType,
    "operTm",
    (ModelNode*) &iedModel_PROT_LLN0_Mod_Oper,
    (ModelNode*) &iedModel_PROT_LLN0_Mod_Oper_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_LLN0_Mod_Oper_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_PROT_LLN0_Mod_Oper,
    (ModelNode*) &iedModel_PROT_LLN0_Mod_Oper_ctlNum,
    (ModelNode*) &iedModel_PROT_LLN0_Mod_Oper_origin_orCat,
    0,
    IEC61850_FC_CO,
    CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_LLN0_Mod_Oper_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_PROT_LLN0_Mod_Oper_origin,
    (ModelNode*) &iedModel_PROT_LLN0_Mod_Oper_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_LLN0_Mod_Oper_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_PROT_LLN0_Mod_Oper_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_LLN0_Mod_Oper_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_PROT_LLN0_Mod_Oper,
    (ModelNode*) &iedModel_PROT_LLN0_Mod_Oper_T,
    NULL,
    0,
    IEC61850_FC_CO,
    INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_LLN0_Mod_Oper_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_PROT_LLN0_Mod_Oper,
    (ModelNode*) &iedModel_PROT_LLN0_Mod_Oper_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_LLN0_Mod_Oper_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_PROT_LLN0_Mod_Oper,
    (ModelNode*) &iedModel_PROT_LLN0_Mod_Oper_Check,
    NULL,
    0,
    IEC61850_FC_CO,
    BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_LLN0_Mod_Oper_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_PROT_LLN0_Mod_Oper,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_LLN0_Mod_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_LLN0_Mod,
    (ModelNode*) &iedModel_PROT_LLN0_Mod_q,
    NULL,
    0,
    IEC61850_FC_ST,
    ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_LLN0_Mod_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_LLN0_Mod,
    (ModelNode*) &iedModel_PROT_LLN0_Mod_t,
    NULL,
    0,
    IEC61850_FC_ST,
    QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_LLN0_Mod_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_LLN0_Mod,
    (ModelNode*) &iedModel_PROT_LLN0_Mod_ctlModel,
    NULL,
    0,
    IEC61850_FC_ST,
    TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_LLN0_Mod_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_PROT_LLN0_Mod,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_PROT_LLN0_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_PROT_LLN0,
    (ModelNode*) &iedModel_PROT_LLN0_Health,
    (ModelNode*) &iedModel_PROT_LLN0_Beh_stVal,
    0
};

DataAttribute iedModel_PROT_LLN0_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_LLN0_Beh,
    (ModelNode*) &iedModel_PROT_LLN0_Beh_q,
    NULL,
    0,
    IEC61850_FC_ST,
    ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_LLN0_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_LLN0_Beh,
    (ModelNode*) &iedModel_PROT_LLN0_Beh_t,
    NULL,
    0,
    IEC61850_FC_ST,
    QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_LLN0_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_LLN0_Beh,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_PROT_LLN0_Health = {
    DataObjectModelType,
    "Health",
    (ModelNode*) &iedModel_PROT_LLN0,
    (ModelNode*) &iedModel_PROT_LLN0_NamPlt,
    (ModelNode*) &iedModel_PROT_LLN0_Health_stVal,
    0
};

DataAttribute iedModel_PROT_LLN0_Health_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_LLN0_Health,
    (ModelNode*) &iedModel_PROT_LLN0_Health_q,
    NULL,
    0,
    IEC61850_FC_ST,
    ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_LLN0_Health_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_LLN0_Health,
    (ModelNode*) &iedModel_PROT_LLN0_Health_t,
    NULL,
    0,
    IEC61850_FC_ST,
    QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_LLN0_Health_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_LLN0_Health,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_PROT_LLN0_NamPlt = {
    DataObjectModelType,
    "NamPlt",
    (ModelNode*) &iedModel_PROT_LLN0,
    NULL,
    (ModelNode*) &iedModel_PROT_LLN0_NamPlt_vendor,
    0
};

DataAttribute iedModel_PROT_LLN0_NamPlt_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_PROT_LLN0_NamPlt,
    (ModelNode*) &iedModel_PROT_LLN0_NamPlt_swRev,
    NULL,
    0,
    IEC61850_FC_DC,
    VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_LLN0_NamPlt_swRev = {
    DataAttributeModelType,
    "swRev",
    (ModelNode*) &iedModel_PROT_LLN0_NamPlt,
    (ModelNode*) &iedModel_PROT_LLN0_NamPlt_d,
    NULL,
    0,
    IEC61850_FC_DC,
    VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_LLN0_NamPlt_d = {
    DataAttributeModelType,
    "d",
    (ModelNode*) &iedModel_PROT_LLN0_NamPlt,
    (ModelNode*) &iedModel_PROT_LLN0_NamPlt_configRev,
    NULL,
    0,
    IEC61850_FC_DC,
    VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_LLN0_NamPlt_configRev = {
    DataAttributeModelType,
    "configRev",
    (ModelNode*) &iedModel_PROT_LLN0_NamPlt,
    (ModelNode*) &iedModel_PROT_LLN0_NamPlt_ldNs,
    NULL,
    0,
    IEC61850_FC_DC,
    VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_LLN0_NamPlt_ldNs = {
    DataAttributeModelType,
    "ldNs",
    (ModelNode*) &iedModel_PROT_LLN0_NamPlt,
    NULL,
    NULL,
    0,
    IEC61850_FC_EX,
    VISIBLE_STRING_255,
    0,
    NULL,
    0};

LogicalNode iedModel_PROT_LPHD1 = {
    LogicalNodeModelType,
    "LPHD1",
    (ModelNode*) &iedModel_PROT,
    (ModelNode*) &iedModel_PROT_PTOC1,
    (ModelNode*) &iedModel_PROT_LPHD1_PhyNam,
};

DataObject iedModel_PROT_LPHD1_PhyNam = {
    DataObjectModelType,
    "PhyNam",
    (ModelNode*) &iedModel_PROT_LPHD1,
    (ModelNode*) &iedModel_PROT_LPHD1_PhyHealth,
    (ModelNode*) &iedModel_PROT_LPHD1_PhyNam_vendor,
    0
};

DataAttribute iedModel_PROT_LPHD1_PhyNam_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_PROT_LPHD1_PhyNam,
    (ModelNode*) &iedModel_PROT_LPHD1_PhyNam_hwRev,
    NULL,
    0,
    IEC61850_FC_DC,
    VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_LPHD1_PhyNam_hwRev = {
    DataAttributeModelType,
    "hwRev",
    (ModelNode*) &iedModel_PROT_LPHD1_PhyNam,
    (ModelNode*) &iedModel_PROT_LPHD1_PhyNam_swRev,
    NULL,
    0,
    IEC61850_FC_DC,
    VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_LPHD1_PhyNam_swRev = {
    DataAttributeModelType,
    "swRev",
    (ModelNode*) &iedModel_PROT_LPHD1_PhyNam,
    (ModelNode*) &iedModel_PROT_LPHD1_PhyNam_serNum,
    NULL,
    0,
    IEC61850_FC_DC,
    VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_LPHD1_PhyNam_serNum = {
    DataAttributeModelType,
    "serNum",
    (ModelNode*) &iedModel_PROT_LPHD1_PhyNam,
    (ModelNode*) &iedModel_PROT_LPHD1_PhyNam_model,
    NULL,
    0,
    IEC61850_FC_DC,
    VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_LPHD1_PhyNam_model = {
    DataAttributeModelType,
    "model",
    (ModelNode*) &iedModel_PROT_LPHD1_PhyNam,
    (ModelNode*) &iedModel_PROT_LPHD1_PhyNam_location,
    NULL,
    0,
    IEC61850_FC_DC,
    VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_LPHD1_PhyNam_location = {
    DataAttributeModelType,
    "location",
    (ModelNode*) &iedModel_PROT_LPHD1_PhyNam,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_PROT_LPHD1_PhyHealth = {
    DataObjectModelType,
    "PhyHealth",
    (ModelNode*) &iedModel_PROT_LPHD1,
    (ModelNode*) &iedModel_PROT_LPHD1_Proxy,
    (ModelNode*) &iedModel_PROT_LPHD1_PhyHealth_stVal,
    0
};

DataAttribute iedModel_PROT_LPHD1_PhyHealth_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_LPHD1_PhyHealth,
    (ModelNode*) &iedModel_PROT_LPHD1_PhyHealth_q,
    NULL,
    0,
    IEC61850_FC_ST,
    ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_LPHD1_PhyHealth_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_LPHD1_PhyHealth,
    (ModelNode*) &iedModel_PROT_LPHD1_PhyHealth_t,
    NULL,
    0,
    IEC61850_FC_ST,
    QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_LPHD1_PhyHealth_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_LPHD1_PhyHealth,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_PROT_LPHD1_Proxy = {
    DataObjectModelType,
    "Proxy",
    (ModelNode*) &iedModel_PROT_LPHD1,
    NULL,
    (ModelNode*) &iedModel_PROT_LPHD1_Proxy_stVal,
    0
};

DataAttribute iedModel_PROT_LPHD1_Proxy_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_LPHD1_Proxy,
    (ModelNode*) &iedModel_PROT_LPHD1_Proxy_q,
    NULL,
    0,
    IEC61850_FC_ST,
    BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_LPHD1_Proxy_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_LPHD1_Proxy,
    (ModelNode*) &iedModel_PROT_LPHD1_Proxy_t,
    NULL,
    0,
    IEC61850_FC_ST,
    QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_LPHD1_Proxy_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_LPHD1_Proxy,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    TIMESTAMP,
    0,
    NULL,
    0};

LogicalNode iedModel_PROT_PTOC1 = {
    LogicalNodeModelType,
    "PTOC1",
    (ModelNode*) &iedModel_PROT,
    NULL,
    (ModelNode*) &iedModel_PROT_PTOC1_Beh,
};

DataObject iedModel_PROT_PTOC1_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_PROT_PTOC1,
    (ModelNode*) &iedModel_PROT_PTOC1_Mod,
    (ModelNode*) &iedModel_PROT_PTOC1_Beh_stVal,
    0
};

DataAttribute iedModel_PROT_PTOC1_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_PTOC1_Beh,
    (ModelNode*) &iedModel_PROT_PTOC1_Beh_q,
    NULL,
    0,
    IEC61850_FC_ST,
    ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_PTOC1_Beh,
    (ModelNode*) &iedModel_PROT_PTOC1_Beh_t,
    NULL,
    0,
    IEC61850_FC_ST,
    QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_PTOC1_Beh,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_PROT_PTOC1_Mod = {
    DataObjectModelType,
    "Mod",
    (ModelNode*) &iedModel_PROT_PTOC1,
    (ModelNode*) &iedModel_PROT_PTOC1_Str,
    (ModelNode*) &iedModel_PROT_PTOC1_Mod_Oper,
    0
};

DataAttribute iedModel_PROT_PTOC1_Mod_Oper = {
    DataAttributeModelType,
    "Oper",
    (ModelNode*) &iedModel_PROT_PTOC1_Mod,
    (ModelNode*) &iedModel_PROT_PTOC1_Mod_stVal,
    (ModelNode*) &iedModel_PROT_PTOC1_Mod_Oper_ctlVal,
    0,
    IEC61850_FC_CO,
    CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_Mod_Oper_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_PROT_PTOC1_Mod_Oper,
    (ModelNode*) &iedModel_PROT_PTOC1_Mod_Oper_operTm,
    NULL,
    0,
    IEC61850_FC_CO,
    ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_Mod_Oper_operTm = {
    DataAttributeModelType,
    "operTm",
    (ModelNode*) &iedModel_PROT_PTOC1_Mod_Oper,
    (ModelNode*) &iedModel_PROT_PTOC1_Mod_Oper_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_Mod_Oper_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_PROT_PTOC1_Mod_Oper,
    (ModelNode*) &iedModel_PROT_PTOC1_Mod_Oper_ctlNum,
    (ModelNode*) &iedModel_PROT_PTOC1_Mod_Oper_origin_orCat,
    0,
    IEC61850_FC_CO,
    CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_Mod_Oper_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_PROT_PTOC1_Mod_Oper_origin,
    (ModelNode*) &iedModel_PROT_PTOC1_Mod_Oper_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_Mod_Oper_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_PROT_PTOC1_Mod_Oper_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_Mod_Oper_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_PROT_PTOC1_Mod_Oper,
    (ModelNode*) &iedModel_PROT_PTOC1_Mod_Oper_T,
    NULL,
    0,
    IEC61850_FC_CO,
    INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_Mod_Oper_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_PROT_PTOC1_Mod_Oper,
    (ModelNode*) &iedModel_PROT_PTOC1_Mod_Oper_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_Mod_Oper_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_PROT_PTOC1_Mod_Oper,
    (ModelNode*) &iedModel_PROT_PTOC1_Mod_Oper_Check,
    NULL,
    0,
    IEC61850_FC_CO,
    BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_Mod_Oper_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_PROT_PTOC1_Mod_Oper,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_Mod_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_PTOC1_Mod,
    (ModelNode*) &iedModel_PROT_PTOC1_Mod_q,
    NULL,
    0,
    IEC61850_FC_ST,
    ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_Mod_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_PTOC1_Mod,
    (ModelNode*) &iedModel_PROT_PTOC1_Mod_t,
    NULL,
    0,
    IEC61850_FC_ST,
    QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_Mod_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_PTOC1_Mod,
    (ModelNode*) &iedModel_PROT_PTOC1_Mod_ctlModel,
    NULL,
    0,
    IEC61850_FC_ST,
    TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_Mod_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_PROT_PTOC1_Mod,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_PROT_PTOC1_Str = {
    DataObjectModelType,
    "Str",
    (ModelNode*) &iedModel_PROT_PTOC1,
    (ModelNode*) &iedModel_PROT_PTOC1_Op,
    (ModelNode*) &iedModel_PROT_PTOC1_Str_general,
    0
};

DataAttribute iedModel_PROT_PTOC1_Str_general = {
    DataAttributeModelType,
    "general",
    (ModelNode*) &iedModel_PROT_PTOC1_Str,
    (ModelNode*) &iedModel_PROT_PTOC1_Str_dirGeneral,
    NULL,
    0,
    IEC61850_FC_ST,
    BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_Str_dirGeneral = {
    DataAttributeModelType,
    "dirGeneral",
    (ModelNode*) &iedModel_PROT_PTOC1_Str,
    (ModelNode*) &iedModel_PROT_PTOC1_Str_q,
    NULL,
    0,
    IEC61850_FC_ST,
    ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_Str_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_PTOC1_Str,
    (ModelNode*) &iedModel_PROT_PTOC1_Str_t,
    NULL,
    0,
    IEC61850_FC_ST,
    QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_Str_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_PTOC1_Str,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_PROT_PTOC1_Op = {
    DataObjectModelType,
    "Op",
    (ModelNode*) &iedModel_PROT_PTOC1,
    (ModelNode*) &iedModel_PROT_PTOC1_StrVal,
    (ModelNode*) &iedModel_PROT_PTOC1_Op_general,
    0
};

DataAttribute iedModel_PROT_PTOC1_Op_general = {
    DataAttributeModelType,
    "general",
    (ModelNode*) &iedModel_PROT_PTOC1_Op,
    (ModelNode*) &iedModel_PROT_PTOC1_Op_q,
    NULL,
    0,
    IEC61850_FC_ST,
    BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_Op_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_PTOC1_Op,
    (ModelNode*) &iedModel_PROT_PTOC1_Op_t,
    NULL,
    0,
    IEC61850_FC_ST,
    QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_Op_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_PTOC1_Op,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_PROT_PTOC1_StrVal = {
    DataObjectModelType,
    "StrVal",
    (ModelNode*) &iedModel_PROT_PTOC1,
    (ModelNode*) &iedModel_PROT_PTOC1_OpDlTmms,
    (ModelNode*) &iedModel_PROT_PTOC1_StrVal_setMag,
    0
};

DataAttribute iedModel_PROT_PTOC1_StrVal_setMag = {
    DataAttributeModelType,
    "setMag",
    (ModelNode*) &iedModel_PROT_PTOC1_StrVal,
    (ModelNode*) &iedModel_SE_PROT_PTOC1_StrVal_setMag,
    (ModelNode*) &iedModel_PROT_PTOC1_StrVal_setMag_f,
    0,
    IEC61850_FC_SG,
    CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_StrVal_setMag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_PROT_PTOC1_StrVal_setMag,
    NULL,
    NULL,
    0,
    IEC61850_FC_SG,
    FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_SE_PROT_PTOC1_StrVal_setMag = {
    DataAttributeModelType,
    "setMag",
    (ModelNode*) &iedModel_PROT_PTOC1_StrVal,
    NULL,
    (ModelNode*) &iedModel_SE_PROT_PTOC1_StrVal_setMag_f,
    0,
    IEC61850_FC_SE,
    CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_SE_PROT_PTOC1_StrVal_setMag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_SE_PROT_PTOC1_StrVal_setMag,
    NULL,
    NULL,
    0,
    IEC61850_FC_SE,
    FLOAT32,
    0,
    NULL,
    0};

DataObject iedModel_PROT_PTOC1_OpDlTmms = {
    DataObjectModelType,
    "OpDlTmms",
    (ModelNode*) &iedModel_PROT_PTOC1,
    (ModelNode*) &iedModel_PROT_PTOC1_RsDlTmms,
    (ModelNode*) &iedModel_PROT_PTOC1_OpDlTmms_setVal,
    0
};

DataAttribute iedModel_PROT_PTOC1_OpDlTmms_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_PROT_PTOC1_OpDlTmms,
    (ModelNode*) &iedModel_SE_PROT_PTOC1_OpDlTmms_setVal,
    NULL,
    0,
    IEC61850_FC_SG,
    INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_SE_PROT_PTOC1_OpDlTmms_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_PROT_PTOC1_OpDlTmms,
    NULL,
    NULL,
    0,
    IEC61850_FC_SE,
    INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_PROT_PTOC1_RsDlTmms = {
    DataObjectModelType,
    "RsDlTmms",
    (ModelNode*) &iedModel_PROT_PTOC1,
    (ModelNode*) &iedModel_PROT_PTOC1_RstTms,
    (ModelNode*) &iedModel_PROT_PTOC1_RsDlTmms_setVal,
    0
};

DataAttribute iedModel_PROT_PTOC1_RsDlTmms_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_PROT_PTOC1_RsDlTmms,
    (ModelNode*) &iedModel_SE_PROT_PTOC1_RsDlTmms_setVal,
    NULL,
    0,
    IEC61850_FC_SG,
    INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_SE_PROT_PTOC1_RsDlTmms_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_PROT_PTOC1_RsDlTmms,
    NULL,
    NULL,
    0,
    IEC61850_FC_SE,
    INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataObject iedModel_PROT_PTOC1_RstTms = {
    DataObjectModelType,
    "RstTms",
    (ModelNode*) &iedModel_PROT_PTOC1,
    NULL,
    (ModelNode*) &iedModel_PROT_PTOC1_RstTms_setVal,
    0
};

DataAttribute iedModel_PROT_PTOC1_RstTms_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_PROT_PTOC1_RstTms,
    (ModelNode*) &iedModel_SE_PROT_PTOC1_RstTms_setVal,
    NULL,
    0,
    IEC61850_FC_SG,
    INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_SE_PROT_PTOC1_RstTms_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_PROT_PTOC1_RstTms,
    NULL,
    NULL,
    0,
    IEC61850_FC_SE,
    INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};




extern SettingGroupControlBlock iedModel_PROT_LLN0_sgcb;

SettingGroupControlBlock iedModel_PROT_LLN0_sgcb = {&iedModel_PROT_LLN0, 1, 5, 0, false, 0, 0, NULL};


IedModel iedModel = {
    "DEMO",
    &iedModel_PROT,
    NULL,
    NULL,
    NULL,
    &iedModel_PROT_LLN0_sgcb,
    initializeValues
};

static void
initializeValues()
{

iedModel_PROT_LLN0_Mod_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(0);

iedModel_PROT_LLN0_NamPlt_vendor.mmsValue = MmsValue_newVisibleString("MZ Automation");

iedModel_PROT_LLN0_NamPlt_swRev.mmsValue = MmsValue_newVisibleString("1.0");

iedModel_PROT_LLN0_NamPlt_d.mmsValue = MmsValue_newVisibleString("Setting group demo");

iedModel_PROT_LLN0_NamPlt_configRev.mmsValue = MmsValue_newVisibleString("1");

iedModel_PROT_LLN0_NamPlt_ldNs.mmsValue = MmsValue_newVisibleString("IEC 61850-7-4:2007");

iedModel_PROT_LPHD1_PhyNam_vendor.mmsValue = MmsValue_newVisibleString("MZ Automation");
}
