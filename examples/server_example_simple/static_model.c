/*
 * static_model.c
 *
 * automatically generated from Teszt_3000_Ed2_v2.cid
 */
#include "static_model.h"

static void initializeValues();

extern DataSet iedModelds_LD0_LLN0_DataSet4Goose;
extern DataSet iedModelds_LD0_LLN0_MaxLengthDatasetBRCB32characters;
extern DataSet iedModelds_LD0_LLN0_Meas;
extern DataSet iedModelds_LD0_LLN0_ServiceTracking;
extern DataSet iedModelds_LD0_LLN0_StatusWithQDA;


extern DataSetEntry iedModelds_LD0_LLN0_DataSet4Goose_fcda0;
extern DataSetEntry iedModelds_LD0_LLN0_DataSet4Goose_fcda1;
extern DataSetEntry iedModelds_LD0_LLN0_DataSet4Goose_fcda2;
extern DataSetEntry iedModelds_LD0_LLN0_DataSet4Goose_fcda3;

DataSetEntry iedModelds_LD0_LLN0_DataSet4Goose_fcda0 = {
  "LD0",
  false,
  "IN8GGIO1$ST$Ind1$stVal", 
  -1,
  NULL,
  NULL,
  &iedModelds_LD0_LLN0_DataSet4Goose_fcda1
};

DataSetEntry iedModelds_LD0_LLN0_DataSet4Goose_fcda1 = {
  "LD0",
  false,
  "IN8GGIO1$ST$Ind1$q", 
  -1,
  NULL,
  NULL,
  &iedModelds_LD0_LLN0_DataSet4Goose_fcda2
};

DataSetEntry iedModelds_LD0_LLN0_DataSet4Goose_fcda2 = {
  "LD0",
  false,
  "IN8GGIO1$ST$Ind2$stVal", 
  -1,
  NULL,
  NULL,
  &iedModelds_LD0_LLN0_DataSet4Goose_fcda3
};

DataSetEntry iedModelds_LD0_LLN0_DataSet4Goose_fcda3 = {
  "LD0",
  false,
  "IN8GGIO1$ST$Ind2$q", 
  -1,
  NULL,
  NULL,
  NULL
};

DataSet iedModelds_LD0_LLN0_DataSet4Goose = {
  "LD0",
  "LLN0$DataSet4Goose",
  4,
  &iedModelds_LD0_LLN0_DataSet4Goose_fcda0,
  &iedModelds_LD0_LLN0_MaxLengthDatasetBRCB32characters
};

extern DataSetEntry iedModelds_LD0_LLN0_MaxLengthDatasetBRCB32characters_fcda0;
extern DataSetEntry iedModelds_LD0_LLN0_MaxLengthDatasetBRCB32characters_fcda1;
extern DataSetEntry iedModelds_LD0_LLN0_MaxLengthDatasetBRCB32characters_fcda2;

DataSetEntry iedModelds_LD0_LLN0_MaxLengthDatasetBRCB32characters_fcda0 = {
  "LD0",
  false,
  "IN8GGIO1$ST$Ind1", 
  -1,
  NULL,
  NULL,
  &iedModelds_LD0_LLN0_MaxLengthDatasetBRCB32characters_fcda1
};

DataSetEntry iedModelds_LD0_LLN0_MaxLengthDatasetBRCB32characters_fcda1 = {
  "LD0",
  false,
  "IN8GGIO1$ST$Ind2", 
  -1,
  NULL,
  NULL,
  &iedModelds_LD0_LLN0_MaxLengthDatasetBRCB32characters_fcda2
};

DataSetEntry iedModelds_LD0_LLN0_MaxLengthDatasetBRCB32characters_fcda2 = {
  "LD0",
  false,
  "CBXCBR1$ST$Pos", 
  -1,
  NULL,
  NULL,
  NULL
};

DataSet iedModelds_LD0_LLN0_MaxLengthDatasetBRCB32characters = {
  "LD0",
  "LLN0$MaxLengthDatasetBRCB32characters",
  3,
  &iedModelds_LD0_LLN0_MaxLengthDatasetBRCB32characters_fcda0,
  &iedModelds_LD0_LLN0_Meas
};

extern DataSetEntry iedModelds_LD0_LLN0_Meas_fcda0;
extern DataSetEntry iedModelds_LD0_LLN0_Meas_fcda1;
extern DataSetEntry iedModelds_LD0_LLN0_Meas_fcda2;
extern DataSetEntry iedModelds_LD0_LLN0_Meas_fcda3;
extern DataSetEntry iedModelds_LD0_LLN0_Meas_fcda4;

DataSetEntry iedModelds_LD0_LLN0_Meas_fcda0 = {
  "LD0",
  false,
  "CMMXU1$MX$A$phsA", 
  -1,
  NULL,
  NULL,
  &iedModelds_LD0_LLN0_Meas_fcda1
};

DataSetEntry iedModelds_LD0_LLN0_Meas_fcda1 = {
  "LD0",
  false,
  "CMMXU1$MX$A$phsB$cVal", 
  -1,
  NULL,
  NULL,
  &iedModelds_LD0_LLN0_Meas_fcda2
};

DataSetEntry iedModelds_LD0_LLN0_Meas_fcda2 = {
  "LD0",
  false,
  "CMMXU1$MX$A$phsC$cVal$mag", 
  -1,
  NULL,
  NULL,
  &iedModelds_LD0_LLN0_Meas_fcda3
};

DataSetEntry iedModelds_LD0_LLN0_Meas_fcda3 = {
  "LD0",
  false,
  "VMMXU1$MX$PhV$phsA$cVal$mag$f", 
  -1,
  NULL,
  NULL,
  &iedModelds_LD0_LLN0_Meas_fcda4
};

DataSetEntry iedModelds_LD0_LLN0_Meas_fcda4 = {
  "LD0",
  false,
  "PQSMMXU1$MX$TotPF", 
  -1,
  NULL,
  NULL,
  NULL
};

DataSet iedModelds_LD0_LLN0_Meas = {
  "LD0",
  "LLN0$Meas",
  5,
  &iedModelds_LD0_LLN0_Meas_fcda0,
  &iedModelds_LD0_LLN0_ServiceTracking
};

extern DataSetEntry iedModelds_LD0_LLN0_ServiceTracking_fcda0;
extern DataSetEntry iedModelds_LD0_LLN0_ServiceTracking_fcda1;
extern DataSetEntry iedModelds_LD0_LLN0_ServiceTracking_fcda2;

DataSetEntry iedModelds_LD0_LLN0_ServiceTracking_fcda0 = {
  "LD0",
  false,
  "TRKLTRK1$SR$DpcTrk", 
  -1,
  NULL,
  NULL,
  &iedModelds_LD0_LLN0_ServiceTracking_fcda1
};

DataSetEntry iedModelds_LD0_LLN0_ServiceTracking_fcda1 = {
  "LD0",
  false,
  "TRKLTRK1$SR$SpcTrk", 
  -1,
  NULL,
  NULL,
  &iedModelds_LD0_LLN0_ServiceTracking_fcda2
};

DataSetEntry iedModelds_LD0_LLN0_ServiceTracking_fcda2 = {
  "LD0",
  false,
  "TRKLTRK1$SR$BrcbTrk", 
  -1,
  NULL,
  NULL,
  NULL
};

DataSet iedModelds_LD0_LLN0_ServiceTracking = {
  "LD0",
  "LLN0$ServiceTracking",
  3,
  &iedModelds_LD0_LLN0_ServiceTracking_fcda0,
  &iedModelds_LD0_LLN0_StatusWithQDA
};

extern DataSetEntry iedModelds_LD0_LLN0_StatusWithQDA_fcda0;
extern DataSetEntry iedModelds_LD0_LLN0_StatusWithQDA_fcda1;
extern DataSetEntry iedModelds_LD0_LLN0_StatusWithQDA_fcda2;
extern DataSetEntry iedModelds_LD0_LLN0_StatusWithQDA_fcda3;

DataSetEntry iedModelds_LD0_LLN0_StatusWithQDA_fcda0 = {
  "LD0",
  false,
  "IN8GGIO1$ST$Ind1$stVal", 
  -1,
  NULL,
  NULL,
  &iedModelds_LD0_LLN0_StatusWithQDA_fcda1
};

DataSetEntry iedModelds_LD0_LLN0_StatusWithQDA_fcda1 = {
  "LD0",
  false,
  "IN8GGIO1$ST$Ind1$q", 
  -1,
  NULL,
  NULL,
  &iedModelds_LD0_LLN0_StatusWithQDA_fcda2
};

DataSetEntry iedModelds_LD0_LLN0_StatusWithQDA_fcda2 = {
  "LD0",
  false,
  "IN8GGIO1$ST$Ind2$stVal", 
  -1,
  NULL,
  NULL,
  &iedModelds_LD0_LLN0_StatusWithQDA_fcda3
};

DataSetEntry iedModelds_LD0_LLN0_StatusWithQDA_fcda3 = {
  "LD0",
  false,
  "IN8GGIO1$ST$Ind2$q", 
  -1,
  NULL,
  NULL,
  NULL
};

DataSet iedModelds_LD0_LLN0_StatusWithQDA = {
  "LD0",
  "LLN0$StatusWithQDA",
  4,
  &iedModelds_LD0_LLN0_StatusWithQDA_fcda0,
  NULL
};

LogicalDevice iedModel_LD0 = {
    LogicalDeviceModelType,
    "LD0",
    (ModelNode*) &iedModel,
    NULL,
    (ModelNode*) &iedModel_LD0_LLN0
};

LogicalNode iedModel_LD0_LLN0 = {
    LogicalNodeModelType,
    "LLN0",
    (ModelNode*) &iedModel_LD0,
    (ModelNode*) &iedModel_LD0_LPHD1,
    (ModelNode*) &iedModel_LD0_LLN0_NamPlt,
};

DataObject iedModel_LD0_LLN0_NamPlt = {
    DataObjectModelType,
    "NamPlt",
    (ModelNode*) &iedModel_LD0_LLN0,
    (ModelNode*) &iedModel_LD0_LLN0_Beh,
    (ModelNode*) &iedModel_LD0_LLN0_NamPlt_vendor,
    0
};

DataAttribute iedModel_LD0_LLN0_NamPlt_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_LD0_LLN0_NamPlt,
    (ModelNode*) &iedModel_LD0_LLN0_NamPlt_swRev,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN0_NamPlt_swRev = {
    DataAttributeModelType,
    "swRev",
    (ModelNode*) &iedModel_LD0_LLN0_NamPlt,
    (ModelNode*) &iedModel_LD0_LLN0_NamPlt_d,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN0_NamPlt_d = {
    DataAttributeModelType,
    "d",
    (ModelNode*) &iedModel_LD0_LLN0_NamPlt,
    (ModelNode*) &iedModel_LD0_LLN0_NamPlt_dU,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN0_NamPlt_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_LLN0_NamPlt,
    (ModelNode*) &iedModel_LD0_LLN0_NamPlt_configRev,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN0_NamPlt_configRev = {
    DataAttributeModelType,
    "configRev",
    (ModelNode*) &iedModel_LD0_LLN0_NamPlt,
    (ModelNode*) &iedModel_LD0_LLN0_NamPlt_ldNs,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN0_NamPlt_ldNs = {
    DataAttributeModelType,
    "ldNs",
    (ModelNode*) &iedModel_LD0_LLN0_NamPlt,
    NULL,
    NULL,
    0,
    IEC61850_FC_EX,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_LLN0_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_LD0_LLN0,
    (ModelNode*) &iedModel_LD0_LLN0_Health,
    (ModelNode*) &iedModel_LD0_LLN0_Beh_stVal,
    0
};

DataAttribute iedModel_LD0_LLN0_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_LLN0_Beh,
    (ModelNode*) &iedModel_LD0_LLN0_Beh_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN0_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_LLN0_Beh,
    (ModelNode*) &iedModel_LD0_LLN0_Beh_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN0_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_LLN0_Beh,
    (ModelNode*) &iedModel_LD0_LLN0_Beh_dU,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN0_Beh_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_LLN0_Beh,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_LLN0_Health = {
    DataObjectModelType,
    "Health",
    (ModelNode*) &iedModel_LD0_LLN0,
    (ModelNode*) &iedModel_LD0_LLN0_Mod,
    (ModelNode*) &iedModel_LD0_LLN0_Health_stVal,
    0
};

DataAttribute iedModel_LD0_LLN0_Health_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_LLN0_Health,
    (ModelNode*) &iedModel_LD0_LLN0_Health_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN0_Health_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_LLN0_Health,
    (ModelNode*) &iedModel_LD0_LLN0_Health_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN0_Health_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_LLN0_Health,
    (ModelNode*) &iedModel_LD0_LLN0_Health_dU,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN0_Health_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_LLN0_Health,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_LLN0_Mod = {
    DataObjectModelType,
    "Mod",
    (ModelNode*) &iedModel_LD0_LLN0,
    (ModelNode*) &iedModel_LD0_LLN0_LEDRs,
    (ModelNode*) &iedModel_LD0_LLN0_Mod_Oper,
    0
};

DataAttribute iedModel_LD0_LLN0_Mod_Oper = {
    DataAttributeModelType,
    "Oper",
    (ModelNode*) &iedModel_LD0_LLN0_Mod,
    (ModelNode*) &iedModel_LD0_LLN0_Mod_stVal,
    (ModelNode*) &iedModel_LD0_LLN0_Mod_Oper_ctlVal,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN0_Mod_Oper_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_LD0_LLN0_Mod_Oper,
    (ModelNode*) &iedModel_LD0_LLN0_Mod_Oper_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN0_Mod_Oper_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_LD0_LLN0_Mod_Oper,
    (ModelNode*) &iedModel_LD0_LLN0_Mod_Oper_ctlNum,
    (ModelNode*) &iedModel_LD0_LLN0_Mod_Oper_origin_orCat,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN0_Mod_Oper_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_LD0_LLN0_Mod_Oper_origin,
    (ModelNode*) &iedModel_LD0_LLN0_Mod_Oper_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN0_Mod_Oper_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_LD0_LLN0_Mod_Oper_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN0_Mod_Oper_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_LD0_LLN0_Mod_Oper,
    (ModelNode*) &iedModel_LD0_LLN0_Mod_Oper_T,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN0_Mod_Oper_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_LD0_LLN0_Mod_Oper,
    (ModelNode*) &iedModel_LD0_LLN0_Mod_Oper_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN0_Mod_Oper_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_LD0_LLN0_Mod_Oper,
    (ModelNode*) &iedModel_LD0_LLN0_Mod_Oper_Check,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN0_Mod_Oper_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_LD0_LLN0_Mod_Oper,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN0_Mod_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_LLN0_Mod,
    (ModelNode*) &iedModel_LD0_LLN0_Mod_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN0_Mod_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_LLN0_Mod,
    (ModelNode*) &iedModel_LD0_LLN0_Mod_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN0_Mod_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_LLN0_Mod,
    (ModelNode*) &iedModel_LD0_LLN0_Mod_ctlModel,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN0_Mod_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_LD0_LLN0_Mod,
    (ModelNode*) &iedModel_LD0_LLN0_Mod_dU,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN0_Mod_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_LLN0_Mod,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_LLN0_LEDRs = {
    DataObjectModelType,
    "LEDRs",
    (ModelNode*) &iedModel_LD0_LLN0,
    NULL,
    (ModelNode*) &iedModel_LD0_LLN0_LEDRs_Oper,
    0
};

DataAttribute iedModel_LD0_LLN0_LEDRs_Oper = {
    DataAttributeModelType,
    "Oper",
    (ModelNode*) &iedModel_LD0_LLN0_LEDRs,
    (ModelNode*) &iedModel_LD0_LLN0_LEDRs_stVal,
    (ModelNode*) &iedModel_LD0_LLN0_LEDRs_Oper_ctlVal,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN0_LEDRs_Oper_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_LD0_LLN0_LEDRs_Oper,
    (ModelNode*) &iedModel_LD0_LLN0_LEDRs_Oper_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN0_LEDRs_Oper_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_LD0_LLN0_LEDRs_Oper,
    (ModelNode*) &iedModel_LD0_LLN0_LEDRs_Oper_ctlNum,
    (ModelNode*) &iedModel_LD0_LLN0_LEDRs_Oper_origin_orCat,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN0_LEDRs_Oper_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_LD0_LLN0_LEDRs_Oper_origin,
    (ModelNode*) &iedModel_LD0_LLN0_LEDRs_Oper_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN0_LEDRs_Oper_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_LD0_LLN0_LEDRs_Oper_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN0_LEDRs_Oper_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_LD0_LLN0_LEDRs_Oper,
    (ModelNode*) &iedModel_LD0_LLN0_LEDRs_Oper_T,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN0_LEDRs_Oper_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_LD0_LLN0_LEDRs_Oper,
    (ModelNode*) &iedModel_LD0_LLN0_LEDRs_Oper_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN0_LEDRs_Oper_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_LD0_LLN0_LEDRs_Oper,
    (ModelNode*) &iedModel_LD0_LLN0_LEDRs_Oper_Check,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN0_LEDRs_Oper_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_LD0_LLN0_LEDRs_Oper,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN0_LEDRs_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_LLN0_LEDRs,
    (ModelNode*) &iedModel_LD0_LLN0_LEDRs_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN0_LEDRs_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_LLN0_LEDRs,
    (ModelNode*) &iedModel_LD0_LLN0_LEDRs_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN0_LEDRs_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_LLN0_LEDRs,
    (ModelNode*) &iedModel_LD0_LLN0_LEDRs_ctlModel,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN0_LEDRs_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_LD0_LLN0_LEDRs,
    (ModelNode*) &iedModel_LD0_LLN0_LEDRs_dU,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN0_LEDRs_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_LLN0_LEDRs,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

LogicalNode iedModel_LD0_LPHD1 = {
    LogicalNodeModelType,
    "LPHD1",
    (ModelNode*) &iedModel_LD0,
    (ModelNode*) &iedModel_LD0_DRRDRE1,
    (ModelNode*) &iedModel_LD0_LPHD1_PhyNam,
};

DataObject iedModel_LD0_LPHD1_PhyNam = {
    DataObjectModelType,
    "PhyNam",
    (ModelNode*) &iedModel_LD0_LPHD1,
    (ModelNode*) &iedModel_LD0_LPHD1_PhyHealth,
    (ModelNode*) &iedModel_LD0_LPHD1_PhyNam_vendor,
    0
};

DataAttribute iedModel_LD0_LPHD1_PhyNam_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_LD0_LPHD1_PhyNam,
    (ModelNode*) &iedModel_LD0_LPHD1_PhyNam_hwRev,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LPHD1_PhyNam_hwRev = {
    DataAttributeModelType,
    "hwRev",
    (ModelNode*) &iedModel_LD0_LPHD1_PhyNam,
    (ModelNode*) &iedModel_LD0_LPHD1_PhyNam_swRev,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LPHD1_PhyNam_swRev = {
    DataAttributeModelType,
    "swRev",
    (ModelNode*) &iedModel_LD0_LPHD1_PhyNam,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_LPHD1_PhyHealth = {
    DataObjectModelType,
    "PhyHealth",
    (ModelNode*) &iedModel_LD0_LPHD1,
    (ModelNode*) &iedModel_LD0_LPHD1_Proxy,
    (ModelNode*) &iedModel_LD0_LPHD1_PhyHealth_stVal,
    0
};

DataAttribute iedModel_LD0_LPHD1_PhyHealth_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_LPHD1_PhyHealth,
    (ModelNode*) &iedModel_LD0_LPHD1_PhyHealth_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_LPHD1_PhyHealth_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_LPHD1_PhyHealth,
    (ModelNode*) &iedModel_LD0_LPHD1_PhyHealth_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_LPHD1_PhyHealth_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_LPHD1_PhyHealth,
    (ModelNode*) &iedModel_LD0_LPHD1_PhyHealth_dU,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LPHD1_PhyHealth_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_LPHD1_PhyHealth,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_LPHD1_Proxy = {
    DataObjectModelType,
    "Proxy",
    (ModelNode*) &iedModel_LD0_LPHD1,
    (ModelNode*) &iedModel_LD0_LPHD1_Sim,
    (ModelNode*) &iedModel_LD0_LPHD1_Proxy_stVal,
    0
};

DataAttribute iedModel_LD0_LPHD1_Proxy_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_LPHD1_Proxy,
    (ModelNode*) &iedModel_LD0_LPHD1_Proxy_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_LPHD1_Proxy_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_LPHD1_Proxy,
    (ModelNode*) &iedModel_LD0_LPHD1_Proxy_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_LPHD1_Proxy_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_LPHD1_Proxy,
    (ModelNode*) &iedModel_LD0_LPHD1_Proxy_dU,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LPHD1_Proxy_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_LPHD1_Proxy,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_LPHD1_Sim = {
    DataObjectModelType,
    "Sim",
    (ModelNode*) &iedModel_LD0_LPHD1,
    NULL,
    (ModelNode*) &iedModel_LD0_LPHD1_Sim_Oper,
    0
};

DataAttribute iedModel_LD0_LPHD1_Sim_Oper = {
    DataAttributeModelType,
    "Oper",
    (ModelNode*) &iedModel_LD0_LPHD1_Sim,
    (ModelNode*) &iedModel_LD0_LPHD1_Sim_stVal,
    (ModelNode*) &iedModel_LD0_LPHD1_Sim_Oper_ctlVal,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LPHD1_Sim_Oper_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_LD0_LPHD1_Sim_Oper,
    (ModelNode*) &iedModel_LD0_LPHD1_Sim_Oper_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LPHD1_Sim_Oper_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_LD0_LPHD1_Sim_Oper,
    (ModelNode*) &iedModel_LD0_LPHD1_Sim_Oper_ctlNum,
    (ModelNode*) &iedModel_LD0_LPHD1_Sim_Oper_origin_orCat,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LPHD1_Sim_Oper_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_LD0_LPHD1_Sim_Oper_origin,
    (ModelNode*) &iedModel_LD0_LPHD1_Sim_Oper_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LPHD1_Sim_Oper_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_LD0_LPHD1_Sim_Oper_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LPHD1_Sim_Oper_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_LD0_LPHD1_Sim_Oper,
    (ModelNode*) &iedModel_LD0_LPHD1_Sim_Oper_T,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LPHD1_Sim_Oper_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_LD0_LPHD1_Sim_Oper,
    (ModelNode*) &iedModel_LD0_LPHD1_Sim_Oper_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LPHD1_Sim_Oper_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_LD0_LPHD1_Sim_Oper,
    (ModelNode*) &iedModel_LD0_LPHD1_Sim_Oper_Check,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LPHD1_Sim_Oper_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_LD0_LPHD1_Sim_Oper,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LPHD1_Sim_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_LPHD1_Sim,
    (ModelNode*) &iedModel_LD0_LPHD1_Sim_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_LPHD1_Sim_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_LPHD1_Sim,
    (ModelNode*) &iedModel_LD0_LPHD1_Sim_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_LPHD1_Sim_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_LPHD1_Sim,
    (ModelNode*) &iedModel_LD0_LPHD1_Sim_ctlModel,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LPHD1_Sim_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_LD0_LPHD1_Sim,
    (ModelNode*) &iedModel_LD0_LPHD1_Sim_dU,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LPHD1_Sim_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_LPHD1_Sim,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

LogicalNode iedModel_LD0_DRRDRE1 = {
    LogicalNodeModelType,
    "DRRDRE1",
    (ModelNode*) &iedModel_LD0,
    (ModelNode*) &iedModel_LD0_CMMXU1,
    (ModelNode*) &iedModel_LD0_DRRDRE1_Beh,
};

DataObject iedModel_LD0_DRRDRE1_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_LD0_DRRDRE1,
    (ModelNode*) &iedModel_LD0_DRRDRE1_RcdMade,
    (ModelNode*) &iedModel_LD0_DRRDRE1_Beh_stVal,
    0
};

DataAttribute iedModel_LD0_DRRDRE1_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_DRRDRE1_Beh,
    (ModelNode*) &iedModel_LD0_DRRDRE1_Beh_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_DRRDRE1_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_DRRDRE1_Beh,
    (ModelNode*) &iedModel_LD0_DRRDRE1_Beh_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_DRRDRE1_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_DRRDRE1_Beh,
    (ModelNode*) &iedModel_LD0_DRRDRE1_Beh_dU,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_DRRDRE1_Beh_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_DRRDRE1_Beh,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_DRRDRE1_RcdMade = {
    DataObjectModelType,
    "RcdMade",
    (ModelNode*) &iedModel_LD0_DRRDRE1,
    (ModelNode*) &iedModel_LD0_DRRDRE1_FltNum,
    (ModelNode*) &iedModel_LD0_DRRDRE1_RcdMade_stVal,
    0
};

DataAttribute iedModel_LD0_DRRDRE1_RcdMade_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_DRRDRE1_RcdMade,
    (ModelNode*) &iedModel_LD0_DRRDRE1_RcdMade_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_DRRDRE1_RcdMade_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_DRRDRE1_RcdMade,
    (ModelNode*) &iedModel_LD0_DRRDRE1_RcdMade_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_DRRDRE1_RcdMade_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_DRRDRE1_RcdMade,
    (ModelNode*) &iedModel_LD0_DRRDRE1_RcdMade_dU,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_DRRDRE1_RcdMade_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_DRRDRE1_RcdMade,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_DRRDRE1_FltNum = {
    DataObjectModelType,
    "FltNum",
    (ModelNode*) &iedModel_LD0_DRRDRE1,
    NULL,
    (ModelNode*) &iedModel_LD0_DRRDRE1_FltNum_stVal,
    0
};

DataAttribute iedModel_LD0_DRRDRE1_FltNum_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_DRRDRE1_FltNum,
    (ModelNode*) &iedModel_LD0_DRRDRE1_FltNum_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_DRRDRE1_FltNum_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_DRRDRE1_FltNum,
    (ModelNode*) &iedModel_LD0_DRRDRE1_FltNum_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_DRRDRE1_FltNum_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_DRRDRE1_FltNum,
    (ModelNode*) &iedModel_LD0_DRRDRE1_FltNum_dU,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_DRRDRE1_FltNum_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_DRRDRE1_FltNum,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

LogicalNode iedModel_LD0_CMMXU1 = {
    LogicalNodeModelType,
    "CMMXU1",
    (ModelNode*) &iedModel_LD0,
    (ModelNode*) &iedModel_LD0_VMMXU1,
    (ModelNode*) &iedModel_LD0_CMMXU1_Beh,
};

DataObject iedModel_LD0_CMMXU1_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_LD0_CMMXU1,
    (ModelNode*) &iedModel_LD0_CMMXU1_A,
    (ModelNode*) &iedModel_LD0_CMMXU1_Beh_stVal,
    0
};

DataAttribute iedModel_LD0_CMMXU1_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_CMMXU1_Beh,
    (ModelNode*) &iedModel_LD0_CMMXU1_Beh_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_CMMXU1_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_CMMXU1_Beh,
    (ModelNode*) &iedModel_LD0_CMMXU1_Beh_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_CMMXU1_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_CMMXU1_Beh,
    (ModelNode*) &iedModel_LD0_CMMXU1_Beh_dU,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_CMMXU1_Beh_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_CMMXU1_Beh,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_CMMXU1_A = {
    DataObjectModelType,
    "A",
    (ModelNode*) &iedModel_LD0_CMMXU1,
    NULL,
    (ModelNode*) &iedModel_LD0_CMMXU1_A_phsA,
    0
};

DataObject iedModel_LD0_CMMXU1_A_phsA = {
    DataObjectModelType,
    "phsA",
    (ModelNode*) &iedModel_LD0_CMMXU1_A,
    (ModelNode*) &iedModel_LD0_CMMXU1_A_phsB,
    (ModelNode*) &iedModel_LD0_CMMXU1_A_phsA_cVal,
    0
};

DataAttribute iedModel_LD0_CMMXU1_A_phsA_cVal = {
    DataAttributeModelType,
    "cVal",
    (ModelNode*) &iedModel_LD0_CMMXU1_A_phsA,
    (ModelNode*) &iedModel_LD0_CMMXU1_A_phsA_q,
    (ModelNode*) &iedModel_LD0_CMMXU1_A_phsA_cVal_mag,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_CMMXU1_A_phsA_cVal_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_LD0_CMMXU1_A_phsA_cVal,
    NULL,
    (ModelNode*) &iedModel_LD0_CMMXU1_A_phsA_cVal_mag_f,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_CMMXU1_A_phsA_cVal_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_LD0_CMMXU1_A_phsA_cVal_mag,
    NULL,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_CMMXU1_A_phsA_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_CMMXU1_A_phsA,
    (ModelNode*) &iedModel_LD0_CMMXU1_A_phsA_t,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_CMMXU1_A_phsA_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_CMMXU1_A_phsA,
    (ModelNode*) &iedModel_LD0_CMMXU1_A_phsA_dU,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_CMMXU1_A_phsA_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_CMMXU1_A_phsA,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_CMMXU1_A_phsB = {
    DataObjectModelType,
    "phsB",
    (ModelNode*) &iedModel_LD0_CMMXU1_A,
    (ModelNode*) &iedModel_LD0_CMMXU1_A_phsC,
    (ModelNode*) &iedModel_LD0_CMMXU1_A_phsB_cVal,
    0
};

DataAttribute iedModel_LD0_CMMXU1_A_phsB_cVal = {
    DataAttributeModelType,
    "cVal",
    (ModelNode*) &iedModel_LD0_CMMXU1_A_phsB,
    (ModelNode*) &iedModel_LD0_CMMXU1_A_phsB_q,
    (ModelNode*) &iedModel_LD0_CMMXU1_A_phsB_cVal_mag,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_CMMXU1_A_phsB_cVal_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_LD0_CMMXU1_A_phsB_cVal,
    NULL,
    (ModelNode*) &iedModel_LD0_CMMXU1_A_phsB_cVal_mag_f,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_CMMXU1_A_phsB_cVal_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_LD0_CMMXU1_A_phsB_cVal_mag,
    NULL,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_CMMXU1_A_phsB_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_CMMXU1_A_phsB,
    (ModelNode*) &iedModel_LD0_CMMXU1_A_phsB_t,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_CMMXU1_A_phsB_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_CMMXU1_A_phsB,
    (ModelNode*) &iedModel_LD0_CMMXU1_A_phsB_dU,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_CMMXU1_A_phsB_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_CMMXU1_A_phsB,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_CMMXU1_A_phsC = {
    DataObjectModelType,
    "phsC",
    (ModelNode*) &iedModel_LD0_CMMXU1_A,
    NULL,
    (ModelNode*) &iedModel_LD0_CMMXU1_A_phsC_cVal,
    0
};

DataAttribute iedModel_LD0_CMMXU1_A_phsC_cVal = {
    DataAttributeModelType,
    "cVal",
    (ModelNode*) &iedModel_LD0_CMMXU1_A_phsC,
    (ModelNode*) &iedModel_LD0_CMMXU1_A_phsC_q,
    (ModelNode*) &iedModel_LD0_CMMXU1_A_phsC_cVal_mag,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_CMMXU1_A_phsC_cVal_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_LD0_CMMXU1_A_phsC_cVal,
    NULL,
    (ModelNode*) &iedModel_LD0_CMMXU1_A_phsC_cVal_mag_f,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_CMMXU1_A_phsC_cVal_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_LD0_CMMXU1_A_phsC_cVal_mag,
    NULL,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_CMMXU1_A_phsC_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_CMMXU1_A_phsC,
    (ModelNode*) &iedModel_LD0_CMMXU1_A_phsC_t,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_CMMXU1_A_phsC_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_CMMXU1_A_phsC,
    (ModelNode*) &iedModel_LD0_CMMXU1_A_phsC_dU,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_CMMXU1_A_phsC_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_CMMXU1_A_phsC,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

LogicalNode iedModel_LD0_VMMXU1 = {
    LogicalNodeModelType,
    "VMMXU1",
    (ModelNode*) &iedModel_LD0,
    (ModelNode*) &iedModel_LD0_PQSMMXU1,
    (ModelNode*) &iedModel_LD0_VMMXU1_Beh,
};

DataObject iedModel_LD0_VMMXU1_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_LD0_VMMXU1,
    (ModelNode*) &iedModel_LD0_VMMXU1_PPV,
    (ModelNode*) &iedModel_LD0_VMMXU1_Beh_stVal,
    0
};

DataAttribute iedModel_LD0_VMMXU1_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_VMMXU1_Beh,
    (ModelNode*) &iedModel_LD0_VMMXU1_Beh_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_VMMXU1_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_VMMXU1_Beh,
    (ModelNode*) &iedModel_LD0_VMMXU1_Beh_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_VMMXU1_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_VMMXU1_Beh,
    (ModelNode*) &iedModel_LD0_VMMXU1_Beh_dU,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_VMMXU1_Beh_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_VMMXU1_Beh,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_VMMXU1_PPV = {
    DataObjectModelType,
    "PPV",
    (ModelNode*) &iedModel_LD0_VMMXU1,
    (ModelNode*) &iedModel_LD0_VMMXU1_PhV,
    (ModelNode*) &iedModel_LD0_VMMXU1_PPV_phsAB,
    0
};

DataObject iedModel_LD0_VMMXU1_PPV_phsAB = {
    DataObjectModelType,
    "phsAB",
    (ModelNode*) &iedModel_LD0_VMMXU1_PPV,
    (ModelNode*) &iedModel_LD0_VMMXU1_PPV_phsBC,
    (ModelNode*) &iedModel_LD0_VMMXU1_PPV_phsAB_cVal,
    0
};

DataAttribute iedModel_LD0_VMMXU1_PPV_phsAB_cVal = {
    DataAttributeModelType,
    "cVal",
    (ModelNode*) &iedModel_LD0_VMMXU1_PPV_phsAB,
    (ModelNode*) &iedModel_LD0_VMMXU1_PPV_phsAB_q,
    (ModelNode*) &iedModel_LD0_VMMXU1_PPV_phsAB_cVal_mag,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_VMMXU1_PPV_phsAB_cVal_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_LD0_VMMXU1_PPV_phsAB_cVal,
    NULL,
    (ModelNode*) &iedModel_LD0_VMMXU1_PPV_phsAB_cVal_mag_f,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_VMMXU1_PPV_phsAB_cVal_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_LD0_VMMXU1_PPV_phsAB_cVal_mag,
    NULL,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_VMMXU1_PPV_phsAB_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_VMMXU1_PPV_phsAB,
    (ModelNode*) &iedModel_LD0_VMMXU1_PPV_phsAB_t,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_VMMXU1_PPV_phsAB_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_VMMXU1_PPV_phsAB,
    (ModelNode*) &iedModel_LD0_VMMXU1_PPV_phsAB_dU,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_VMMXU1_PPV_phsAB_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_VMMXU1_PPV_phsAB,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_VMMXU1_PPV_phsBC = {
    DataObjectModelType,
    "phsBC",
    (ModelNode*) &iedModel_LD0_VMMXU1_PPV,
    (ModelNode*) &iedModel_LD0_VMMXU1_PPV_phsCA,
    (ModelNode*) &iedModel_LD0_VMMXU1_PPV_phsBC_cVal,
    0
};

DataAttribute iedModel_LD0_VMMXU1_PPV_phsBC_cVal = {
    DataAttributeModelType,
    "cVal",
    (ModelNode*) &iedModel_LD0_VMMXU1_PPV_phsBC,
    (ModelNode*) &iedModel_LD0_VMMXU1_PPV_phsBC_q,
    (ModelNode*) &iedModel_LD0_VMMXU1_PPV_phsBC_cVal_mag,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_VMMXU1_PPV_phsBC_cVal_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_LD0_VMMXU1_PPV_phsBC_cVal,
    NULL,
    (ModelNode*) &iedModel_LD0_VMMXU1_PPV_phsBC_cVal_mag_f,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_VMMXU1_PPV_phsBC_cVal_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_LD0_VMMXU1_PPV_phsBC_cVal_mag,
    NULL,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_VMMXU1_PPV_phsBC_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_VMMXU1_PPV_phsBC,
    (ModelNode*) &iedModel_LD0_VMMXU1_PPV_phsBC_t,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_VMMXU1_PPV_phsBC_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_VMMXU1_PPV_phsBC,
    (ModelNode*) &iedModel_LD0_VMMXU1_PPV_phsBC_dU,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_VMMXU1_PPV_phsBC_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_VMMXU1_PPV_phsBC,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_VMMXU1_PPV_phsCA = {
    DataObjectModelType,
    "phsCA",
    (ModelNode*) &iedModel_LD0_VMMXU1_PPV,
    NULL,
    (ModelNode*) &iedModel_LD0_VMMXU1_PPV_phsCA_cVal,
    0
};

DataAttribute iedModel_LD0_VMMXU1_PPV_phsCA_cVal = {
    DataAttributeModelType,
    "cVal",
    (ModelNode*) &iedModel_LD0_VMMXU1_PPV_phsCA,
    (ModelNode*) &iedModel_LD0_VMMXU1_PPV_phsCA_q,
    (ModelNode*) &iedModel_LD0_VMMXU1_PPV_phsCA_cVal_mag,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_VMMXU1_PPV_phsCA_cVal_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_LD0_VMMXU1_PPV_phsCA_cVal,
    NULL,
    (ModelNode*) &iedModel_LD0_VMMXU1_PPV_phsCA_cVal_mag_f,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_VMMXU1_PPV_phsCA_cVal_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_LD0_VMMXU1_PPV_phsCA_cVal_mag,
    NULL,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_VMMXU1_PPV_phsCA_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_VMMXU1_PPV_phsCA,
    (ModelNode*) &iedModel_LD0_VMMXU1_PPV_phsCA_t,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_VMMXU1_PPV_phsCA_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_VMMXU1_PPV_phsCA,
    (ModelNode*) &iedModel_LD0_VMMXU1_PPV_phsCA_dU,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_VMMXU1_PPV_phsCA_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_VMMXU1_PPV_phsCA,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_VMMXU1_PhV = {
    DataObjectModelType,
    "PhV",
    (ModelNode*) &iedModel_LD0_VMMXU1,
    NULL,
    (ModelNode*) &iedModel_LD0_VMMXU1_PhV_phsA,
    0
};

DataObject iedModel_LD0_VMMXU1_PhV_phsA = {
    DataObjectModelType,
    "phsA",
    (ModelNode*) &iedModel_LD0_VMMXU1_PhV,
    (ModelNode*) &iedModel_LD0_VMMXU1_PhV_phsB,
    (ModelNode*) &iedModel_LD0_VMMXU1_PhV_phsA_cVal,
    0
};

DataAttribute iedModel_LD0_VMMXU1_PhV_phsA_cVal = {
    DataAttributeModelType,
    "cVal",
    (ModelNode*) &iedModel_LD0_VMMXU1_PhV_phsA,
    (ModelNode*) &iedModel_LD0_VMMXU1_PhV_phsA_q,
    (ModelNode*) &iedModel_LD0_VMMXU1_PhV_phsA_cVal_mag,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_VMMXU1_PhV_phsA_cVal_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_LD0_VMMXU1_PhV_phsA_cVal,
    NULL,
    (ModelNode*) &iedModel_LD0_VMMXU1_PhV_phsA_cVal_mag_f,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_VMMXU1_PhV_phsA_cVal_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_LD0_VMMXU1_PhV_phsA_cVal_mag,
    NULL,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_VMMXU1_PhV_phsA_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_VMMXU1_PhV_phsA,
    (ModelNode*) &iedModel_LD0_VMMXU1_PhV_phsA_t,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_VMMXU1_PhV_phsA_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_VMMXU1_PhV_phsA,
    (ModelNode*) &iedModel_LD0_VMMXU1_PhV_phsA_dU,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_VMMXU1_PhV_phsA_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_VMMXU1_PhV_phsA,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_VMMXU1_PhV_phsB = {
    DataObjectModelType,
    "phsB",
    (ModelNode*) &iedModel_LD0_VMMXU1_PhV,
    (ModelNode*) &iedModel_LD0_VMMXU1_PhV_phsC,
    (ModelNode*) &iedModel_LD0_VMMXU1_PhV_phsB_cVal,
    0
};

DataAttribute iedModel_LD0_VMMXU1_PhV_phsB_cVal = {
    DataAttributeModelType,
    "cVal",
    (ModelNode*) &iedModel_LD0_VMMXU1_PhV_phsB,
    (ModelNode*) &iedModel_LD0_VMMXU1_PhV_phsB_q,
    (ModelNode*) &iedModel_LD0_VMMXU1_PhV_phsB_cVal_mag,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_VMMXU1_PhV_phsB_cVal_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_LD0_VMMXU1_PhV_phsB_cVal,
    NULL,
    (ModelNode*) &iedModel_LD0_VMMXU1_PhV_phsB_cVal_mag_f,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_VMMXU1_PhV_phsB_cVal_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_LD0_VMMXU1_PhV_phsB_cVal_mag,
    NULL,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_VMMXU1_PhV_phsB_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_VMMXU1_PhV_phsB,
    (ModelNode*) &iedModel_LD0_VMMXU1_PhV_phsB_t,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_VMMXU1_PhV_phsB_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_VMMXU1_PhV_phsB,
    (ModelNode*) &iedModel_LD0_VMMXU1_PhV_phsB_dU,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_VMMXU1_PhV_phsB_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_VMMXU1_PhV_phsB,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_VMMXU1_PhV_phsC = {
    DataObjectModelType,
    "phsC",
    (ModelNode*) &iedModel_LD0_VMMXU1_PhV,
    NULL,
    (ModelNode*) &iedModel_LD0_VMMXU1_PhV_phsC_cVal,
    0
};

DataAttribute iedModel_LD0_VMMXU1_PhV_phsC_cVal = {
    DataAttributeModelType,
    "cVal",
    (ModelNode*) &iedModel_LD0_VMMXU1_PhV_phsC,
    (ModelNode*) &iedModel_LD0_VMMXU1_PhV_phsC_q,
    (ModelNode*) &iedModel_LD0_VMMXU1_PhV_phsC_cVal_mag,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_VMMXU1_PhV_phsC_cVal_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_LD0_VMMXU1_PhV_phsC_cVal,
    NULL,
    (ModelNode*) &iedModel_LD0_VMMXU1_PhV_phsC_cVal_mag_f,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_VMMXU1_PhV_phsC_cVal_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_LD0_VMMXU1_PhV_phsC_cVal_mag,
    NULL,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_VMMXU1_PhV_phsC_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_VMMXU1_PhV_phsC,
    (ModelNode*) &iedModel_LD0_VMMXU1_PhV_phsC_t,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_VMMXU1_PhV_phsC_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_VMMXU1_PhV_phsC,
    (ModelNode*) &iedModel_LD0_VMMXU1_PhV_phsC_dU,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_VMMXU1_PhV_phsC_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_VMMXU1_PhV_phsC,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

LogicalNode iedModel_LD0_PQSMMXU1 = {
    LogicalNodeModelType,
    "PQSMMXU1",
    (ModelNode*) &iedModel_LD0,
    (ModelNode*) &iedModel_LD0_UoIoMMXN1,
    (ModelNode*) &iedModel_LD0_PQSMMXU1_Beh,
};

DataObject iedModel_LD0_PQSMMXU1_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_LD0_PQSMMXU1,
    (ModelNode*) &iedModel_LD0_PQSMMXU1_TotW,
    (ModelNode*) &iedModel_LD0_PQSMMXU1_Beh_stVal,
    0
};

DataAttribute iedModel_LD0_PQSMMXU1_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_PQSMMXU1_Beh,
    (ModelNode*) &iedModel_LD0_PQSMMXU1_Beh_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_PQSMMXU1_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_PQSMMXU1_Beh,
    (ModelNode*) &iedModel_LD0_PQSMMXU1_Beh_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_PQSMMXU1_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_PQSMMXU1_Beh,
    (ModelNode*) &iedModel_LD0_PQSMMXU1_Beh_dU,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_PQSMMXU1_Beh_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_PQSMMXU1_Beh,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_PQSMMXU1_TotW = {
    DataObjectModelType,
    "TotW",
    (ModelNode*) &iedModel_LD0_PQSMMXU1,
    (ModelNode*) &iedModel_LD0_PQSMMXU1_TotVAr,
    (ModelNode*) &iedModel_LD0_PQSMMXU1_TotW_mag,
    0
};

DataAttribute iedModel_LD0_PQSMMXU1_TotW_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_LD0_PQSMMXU1_TotW,
    (ModelNode*) &iedModel_LD0_PQSMMXU1_TotW_q,
    (ModelNode*) &iedModel_LD0_PQSMMXU1_TotW_mag_f,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_PQSMMXU1_TotW_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_LD0_PQSMMXU1_TotW_mag,
    NULL,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_PQSMMXU1_TotW_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_PQSMMXU1_TotW,
    (ModelNode*) &iedModel_LD0_PQSMMXU1_TotW_t,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_PQSMMXU1_TotW_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_PQSMMXU1_TotW,
    (ModelNode*) &iedModel_LD0_PQSMMXU1_TotW_dU,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_PQSMMXU1_TotW_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_PQSMMXU1_TotW,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_PQSMMXU1_TotVAr = {
    DataObjectModelType,
    "TotVAr",
    (ModelNode*) &iedModel_LD0_PQSMMXU1,
    (ModelNode*) &iedModel_LD0_PQSMMXU1_TotVA,
    (ModelNode*) &iedModel_LD0_PQSMMXU1_TotVAr_mag,
    0
};

DataAttribute iedModel_LD0_PQSMMXU1_TotVAr_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_LD0_PQSMMXU1_TotVAr,
    (ModelNode*) &iedModel_LD0_PQSMMXU1_TotVAr_q,
    (ModelNode*) &iedModel_LD0_PQSMMXU1_TotVAr_mag_f,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_PQSMMXU1_TotVAr_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_LD0_PQSMMXU1_TotVAr_mag,
    NULL,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_PQSMMXU1_TotVAr_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_PQSMMXU1_TotVAr,
    (ModelNode*) &iedModel_LD0_PQSMMXU1_TotVAr_t,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_PQSMMXU1_TotVAr_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_PQSMMXU1_TotVAr,
    (ModelNode*) &iedModel_LD0_PQSMMXU1_TotVAr_dU,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_PQSMMXU1_TotVAr_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_PQSMMXU1_TotVAr,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_PQSMMXU1_TotVA = {
    DataObjectModelType,
    "TotVA",
    (ModelNode*) &iedModel_LD0_PQSMMXU1,
    (ModelNode*) &iedModel_LD0_PQSMMXU1_TotPF,
    (ModelNode*) &iedModel_LD0_PQSMMXU1_TotVA_mag,
    0
};

DataAttribute iedModel_LD0_PQSMMXU1_TotVA_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_LD0_PQSMMXU1_TotVA,
    (ModelNode*) &iedModel_LD0_PQSMMXU1_TotVA_q,
    (ModelNode*) &iedModel_LD0_PQSMMXU1_TotVA_mag_f,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_PQSMMXU1_TotVA_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_LD0_PQSMMXU1_TotVA_mag,
    NULL,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_PQSMMXU1_TotVA_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_PQSMMXU1_TotVA,
    (ModelNode*) &iedModel_LD0_PQSMMXU1_TotVA_t,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_PQSMMXU1_TotVA_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_PQSMMXU1_TotVA,
    (ModelNode*) &iedModel_LD0_PQSMMXU1_TotVA_dU,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_PQSMMXU1_TotVA_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_PQSMMXU1_TotVA,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_PQSMMXU1_TotPF = {
    DataObjectModelType,
    "TotPF",
    (ModelNode*) &iedModel_LD0_PQSMMXU1,
    NULL,
    (ModelNode*) &iedModel_LD0_PQSMMXU1_TotPF_mag,
    0
};

DataAttribute iedModel_LD0_PQSMMXU1_TotPF_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_LD0_PQSMMXU1_TotPF,
    (ModelNode*) &iedModel_LD0_PQSMMXU1_TotPF_q,
    (ModelNode*) &iedModel_LD0_PQSMMXU1_TotPF_mag_f,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_PQSMMXU1_TotPF_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_LD0_PQSMMXU1_TotPF_mag,
    NULL,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_PQSMMXU1_TotPF_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_PQSMMXU1_TotPF,
    (ModelNode*) &iedModel_LD0_PQSMMXU1_TotPF_t,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_PQSMMXU1_TotPF_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_PQSMMXU1_TotPF,
    (ModelNode*) &iedModel_LD0_PQSMMXU1_TotPF_dU,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_PQSMMXU1_TotPF_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_PQSMMXU1_TotPF,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

LogicalNode iedModel_LD0_UoIoMMXN1 = {
    LogicalNodeModelType,
    "UoIoMMXN1",
    (ModelNode*) &iedModel_LD0,
    (ModelNode*) &iedModel_LD0_ResetGGIO1,
    (ModelNode*) &iedModel_LD0_UoIoMMXN1_Beh,
};

DataObject iedModel_LD0_UoIoMMXN1_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_LD0_UoIoMMXN1,
    (ModelNode*) &iedModel_LD0_UoIoMMXN1_Amp,
    (ModelNode*) &iedModel_LD0_UoIoMMXN1_Beh_stVal,
    0
};

DataAttribute iedModel_LD0_UoIoMMXN1_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_UoIoMMXN1_Beh,
    (ModelNode*) &iedModel_LD0_UoIoMMXN1_Beh_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_UoIoMMXN1_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_UoIoMMXN1_Beh,
    (ModelNode*) &iedModel_LD0_UoIoMMXN1_Beh_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_UoIoMMXN1_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_UoIoMMXN1_Beh,
    (ModelNode*) &iedModel_LD0_UoIoMMXN1_Beh_dU,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_UoIoMMXN1_Beh_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_UoIoMMXN1_Beh,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_UoIoMMXN1_Amp = {
    DataObjectModelType,
    "Amp",
    (ModelNode*) &iedModel_LD0_UoIoMMXN1,
    (ModelNode*) &iedModel_LD0_UoIoMMXN1_Vol,
    (ModelNode*) &iedModel_LD0_UoIoMMXN1_Amp_mag,
    0
};

DataAttribute iedModel_LD0_UoIoMMXN1_Amp_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_LD0_UoIoMMXN1_Amp,
    (ModelNode*) &iedModel_LD0_UoIoMMXN1_Amp_q,
    (ModelNode*) &iedModel_LD0_UoIoMMXN1_Amp_mag_f,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_UoIoMMXN1_Amp_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_LD0_UoIoMMXN1_Amp_mag,
    NULL,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_UoIoMMXN1_Amp_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_UoIoMMXN1_Amp,
    (ModelNode*) &iedModel_LD0_UoIoMMXN1_Amp_t,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_UoIoMMXN1_Amp_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_UoIoMMXN1_Amp,
    (ModelNode*) &iedModel_LD0_UoIoMMXN1_Amp_dU,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_UoIoMMXN1_Amp_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_UoIoMMXN1_Amp,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_UoIoMMXN1_Vol = {
    DataObjectModelType,
    "Vol",
    (ModelNode*) &iedModel_LD0_UoIoMMXN1,
    NULL,
    (ModelNode*) &iedModel_LD0_UoIoMMXN1_Vol_mag,
    0
};

DataAttribute iedModel_LD0_UoIoMMXN1_Vol_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_LD0_UoIoMMXN1_Vol,
    (ModelNode*) &iedModel_LD0_UoIoMMXN1_Vol_q,
    (ModelNode*) &iedModel_LD0_UoIoMMXN1_Vol_mag_f,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_UoIoMMXN1_Vol_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_LD0_UoIoMMXN1_Vol_mag,
    NULL,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_UoIoMMXN1_Vol_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_UoIoMMXN1_Vol,
    (ModelNode*) &iedModel_LD0_UoIoMMXN1_Vol_t,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_UoIoMMXN1_Vol_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_UoIoMMXN1_Vol,
    (ModelNode*) &iedModel_LD0_UoIoMMXN1_Vol_dU,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_UoIoMMXN1_Vol_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_UoIoMMXN1_Vol,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

LogicalNode iedModel_LD0_ResetGGIO1 = {
    LogicalNodeModelType,
    "ResetGGIO1",
    (ModelNode*) &iedModel_LD0,
    (ModelNode*) &iedModel_LD0_TRPDIF1,
    (ModelNode*) &iedModel_LD0_ResetGGIO1_Beh,
};

DataObject iedModel_LD0_ResetGGIO1_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_LD0_ResetGGIO1,
    (ModelNode*) &iedModel_LD0_ResetGGIO1_SPCSO1,
    (ModelNode*) &iedModel_LD0_ResetGGIO1_Beh_stVal,
    0
};

DataAttribute iedModel_LD0_ResetGGIO1_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_ResetGGIO1_Beh,
    (ModelNode*) &iedModel_LD0_ResetGGIO1_Beh_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_ResetGGIO1_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_ResetGGIO1_Beh,
    (ModelNode*) &iedModel_LD0_ResetGGIO1_Beh_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_ResetGGIO1_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_ResetGGIO1_Beh,
    (ModelNode*) &iedModel_LD0_ResetGGIO1_Beh_dU,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_ResetGGIO1_Beh_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_ResetGGIO1_Beh,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_ResetGGIO1_SPCSO1 = {
    DataObjectModelType,
    "SPCSO1",
    (ModelNode*) &iedModel_LD0_ResetGGIO1,
    NULL,
    (ModelNode*) &iedModel_LD0_ResetGGIO1_SPCSO1_Oper,
    0
};

DataAttribute iedModel_LD0_ResetGGIO1_SPCSO1_Oper = {
    DataAttributeModelType,
    "Oper",
    (ModelNode*) &iedModel_LD0_ResetGGIO1_SPCSO1,
    (ModelNode*) &iedModel_LD0_ResetGGIO1_SPCSO1_stVal,
    (ModelNode*) &iedModel_LD0_ResetGGIO1_SPCSO1_Oper_ctlVal,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_ResetGGIO1_SPCSO1_Oper_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_LD0_ResetGGIO1_SPCSO1_Oper,
    (ModelNode*) &iedModel_LD0_ResetGGIO1_SPCSO1_Oper_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_ResetGGIO1_SPCSO1_Oper_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_LD0_ResetGGIO1_SPCSO1_Oper,
    (ModelNode*) &iedModel_LD0_ResetGGIO1_SPCSO1_Oper_ctlNum,
    (ModelNode*) &iedModel_LD0_ResetGGIO1_SPCSO1_Oper_origin_orCat,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_ResetGGIO1_SPCSO1_Oper_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_LD0_ResetGGIO1_SPCSO1_Oper_origin,
    (ModelNode*) &iedModel_LD0_ResetGGIO1_SPCSO1_Oper_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_ResetGGIO1_SPCSO1_Oper_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_LD0_ResetGGIO1_SPCSO1_Oper_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_ResetGGIO1_SPCSO1_Oper_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_LD0_ResetGGIO1_SPCSO1_Oper,
    (ModelNode*) &iedModel_LD0_ResetGGIO1_SPCSO1_Oper_T,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_ResetGGIO1_SPCSO1_Oper_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_LD0_ResetGGIO1_SPCSO1_Oper,
    (ModelNode*) &iedModel_LD0_ResetGGIO1_SPCSO1_Oper_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_ResetGGIO1_SPCSO1_Oper_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_LD0_ResetGGIO1_SPCSO1_Oper,
    (ModelNode*) &iedModel_LD0_ResetGGIO1_SPCSO1_Oper_Check,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_ResetGGIO1_SPCSO1_Oper_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_LD0_ResetGGIO1_SPCSO1_Oper,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_ResetGGIO1_SPCSO1_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_ResetGGIO1_SPCSO1,
    (ModelNode*) &iedModel_LD0_ResetGGIO1_SPCSO1_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_ResetGGIO1_SPCSO1_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_ResetGGIO1_SPCSO1,
    (ModelNode*) &iedModel_LD0_ResetGGIO1_SPCSO1_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_ResetGGIO1_SPCSO1_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_ResetGGIO1_SPCSO1,
    (ModelNode*) &iedModel_LD0_ResetGGIO1_SPCSO1_ctlModel,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_ResetGGIO1_SPCSO1_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_LD0_ResetGGIO1_SPCSO1,
    (ModelNode*) &iedModel_LD0_ResetGGIO1_SPCSO1_dU,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_ResetGGIO1_SPCSO1_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_ResetGGIO1_SPCSO1,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

LogicalNode iedModel_LD0_TRPDIF1 = {
    LogicalNodeModelType,
    "TRPDIF1",
    (ModelNode*) &iedModel_LD0,
    (ModelNode*) &iedModel_LD0_IN8GGIO1,
    (ModelNode*) &iedModel_LD0_TRPDIF1_Beh,
};

DataObject iedModel_LD0_TRPDIF1_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_LD0_TRPDIF1,
    (ModelNode*) &iedModel_LD0_TRPDIF1_Str,
    (ModelNode*) &iedModel_LD0_TRPDIF1_Beh_stVal,
    0
};

DataAttribute iedModel_LD0_TRPDIF1_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_TRPDIF1_Beh,
    (ModelNode*) &iedModel_LD0_TRPDIF1_Beh_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_TRPDIF1_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_TRPDIF1_Beh,
    (ModelNode*) &iedModel_LD0_TRPDIF1_Beh_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_TRPDIF1_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_TRPDIF1_Beh,
    (ModelNode*) &iedModel_LD0_TRPDIF1_Beh_dU,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRPDIF1_Beh_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_TRPDIF1_Beh,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_TRPDIF1_Str = {
    DataObjectModelType,
    "Str",
    (ModelNode*) &iedModel_LD0_TRPDIF1,
    (ModelNode*) &iedModel_LD0_TRPDIF1_Op,
    (ModelNode*) &iedModel_LD0_TRPDIF1_Str_general,
    0
};

DataAttribute iedModel_LD0_TRPDIF1_Str_general = {
    DataAttributeModelType,
    "general",
    (ModelNode*) &iedModel_LD0_TRPDIF1_Str,
    (ModelNode*) &iedModel_LD0_TRPDIF1_Str_dirGeneral,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_TRPDIF1_Str_dirGeneral = {
    DataAttributeModelType,
    "dirGeneral",
    (ModelNode*) &iedModel_LD0_TRPDIF1_Str,
    (ModelNode*) &iedModel_LD0_TRPDIF1_Str_phsA,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_TRPDIF1_Str_phsA = {
    DataAttributeModelType,
    "phsA",
    (ModelNode*) &iedModel_LD0_TRPDIF1_Str,
    (ModelNode*) &iedModel_LD0_TRPDIF1_Str_dirPhsA,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_TRPDIF1_Str_dirPhsA = {
    DataAttributeModelType,
    "dirPhsA",
    (ModelNode*) &iedModel_LD0_TRPDIF1_Str,
    (ModelNode*) &iedModel_LD0_TRPDIF1_Str_phsB,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_TRPDIF1_Str_phsB = {
    DataAttributeModelType,
    "phsB",
    (ModelNode*) &iedModel_LD0_TRPDIF1_Str,
    (ModelNode*) &iedModel_LD0_TRPDIF1_Str_dirPhsB,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_TRPDIF1_Str_dirPhsB = {
    DataAttributeModelType,
    "dirPhsB",
    (ModelNode*) &iedModel_LD0_TRPDIF1_Str,
    (ModelNode*) &iedModel_LD0_TRPDIF1_Str_phsC,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_TRPDIF1_Str_phsC = {
    DataAttributeModelType,
    "phsC",
    (ModelNode*) &iedModel_LD0_TRPDIF1_Str,
    (ModelNode*) &iedModel_LD0_TRPDIF1_Str_dirPhsC,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_TRPDIF1_Str_dirPhsC = {
    DataAttributeModelType,
    "dirPhsC",
    (ModelNode*) &iedModel_LD0_TRPDIF1_Str,
    (ModelNode*) &iedModel_LD0_TRPDIF1_Str_neut,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_TRPDIF1_Str_neut = {
    DataAttributeModelType,
    "neut",
    (ModelNode*) &iedModel_LD0_TRPDIF1_Str,
    (ModelNode*) &iedModel_LD0_TRPDIF1_Str_dirNeut,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_TRPDIF1_Str_dirNeut = {
    DataAttributeModelType,
    "dirNeut",
    (ModelNode*) &iedModel_LD0_TRPDIF1_Str,
    (ModelNode*) &iedModel_LD0_TRPDIF1_Str_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_TRPDIF1_Str_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_TRPDIF1_Str,
    (ModelNode*) &iedModel_LD0_TRPDIF1_Str_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_TRPDIF1_Str_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_TRPDIF1_Str,
    (ModelNode*) &iedModel_LD0_TRPDIF1_Str_dU,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRPDIF1_Str_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_TRPDIF1_Str,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_TRPDIF1_Op = {
    DataObjectModelType,
    "Op",
    (ModelNode*) &iedModel_LD0_TRPDIF1,
    (ModelNode*) &iedModel_LD0_TRPDIF1_OpUnr,
    (ModelNode*) &iedModel_LD0_TRPDIF1_Op_general,
    0
};

DataAttribute iedModel_LD0_TRPDIF1_Op_general = {
    DataAttributeModelType,
    "general",
    (ModelNode*) &iedModel_LD0_TRPDIF1_Op,
    (ModelNode*) &iedModel_LD0_TRPDIF1_Op_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_TRPDIF1_Op_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_TRPDIF1_Op,
    (ModelNode*) &iedModel_LD0_TRPDIF1_Op_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_TRPDIF1_Op_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_TRPDIF1_Op,
    (ModelNode*) &iedModel_LD0_TRPDIF1_Op_dU,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRPDIF1_Op_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_TRPDIF1_Op,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_TRPDIF1_OpUnr = {
    DataObjectModelType,
    "OpUnr",
    (ModelNode*) &iedModel_LD0_TRPDIF1,
    NULL,
    (ModelNode*) &iedModel_LD0_TRPDIF1_OpUnr_general,
    0
};

DataAttribute iedModel_LD0_TRPDIF1_OpUnr_general = {
    DataAttributeModelType,
    "general",
    (ModelNode*) &iedModel_LD0_TRPDIF1_OpUnr,
    (ModelNode*) &iedModel_LD0_TRPDIF1_OpUnr_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_TRPDIF1_OpUnr_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_TRPDIF1_OpUnr,
    (ModelNode*) &iedModel_LD0_TRPDIF1_OpUnr_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_TRPDIF1_OpUnr_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_TRPDIF1_OpUnr,
    (ModelNode*) &iedModel_LD0_TRPDIF1_OpUnr_dU,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRPDIF1_OpUnr_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_TRPDIF1_OpUnr,
    (ModelNode*) &iedModel_LD0_TRPDIF1_OpUnr_dataNs,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRPDIF1_OpUnr_dataNs = {
    DataAttributeModelType,
    "dataNs",
    (ModelNode*) &iedModel_LD0_TRPDIF1_OpUnr,
    NULL,
    NULL,
    0,
    IEC61850_FC_EX,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

LogicalNode iedModel_LD0_IN8GGIO1 = {
    LogicalNodeModelType,
    "IN8GGIO1",
    (ModelNode*) &iedModel_LD0,
    (ModelNode*) &iedModel_LD0_CBCSWI1,
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Beh,
};

DataObject iedModel_LD0_IN8GGIO1_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_LD0_IN8GGIO1,
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind1,
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Beh_stVal,
    0
};

DataAttribute iedModel_LD0_IN8GGIO1_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Beh,
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Beh_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_IN8GGIO1_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Beh,
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Beh_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_IN8GGIO1_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Beh,
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Beh_dU,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_IN8GGIO1_Beh_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Beh,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_IN8GGIO1_Ind1 = {
    DataObjectModelType,
    "Ind1",
    (ModelNode*) &iedModel_LD0_IN8GGIO1,
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind2,
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind1_stVal,
    0
};

DataAttribute iedModel_LD0_IN8GGIO1_Ind1_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind1,
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind1_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_IN8GGIO1_Ind1_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind1,
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind1_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_IN8GGIO1_Ind1_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind1,
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind1_dU,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_IN8GGIO1_Ind1_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind1,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_IN8GGIO1_Ind2 = {
    DataObjectModelType,
    "Ind2",
    (ModelNode*) &iedModel_LD0_IN8GGIO1,
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind3,
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind2_stVal,
    0
};

DataAttribute iedModel_LD0_IN8GGIO1_Ind2_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind2,
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind2_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_IN8GGIO1_Ind2_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind2,
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind2_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_IN8GGIO1_Ind2_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind2,
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind2_dU,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_IN8GGIO1_Ind2_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind2,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_IN8GGIO1_Ind3 = {
    DataObjectModelType,
    "Ind3",
    (ModelNode*) &iedModel_LD0_IN8GGIO1,
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind4,
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind3_stVal,
    0
};

DataAttribute iedModel_LD0_IN8GGIO1_Ind3_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind3,
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind3_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_IN8GGIO1_Ind3_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind3,
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind3_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_IN8GGIO1_Ind3_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind3,
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind3_dU,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_IN8GGIO1_Ind3_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind3,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_IN8GGIO1_Ind4 = {
    DataObjectModelType,
    "Ind4",
    (ModelNode*) &iedModel_LD0_IN8GGIO1,
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind5,
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind4_stVal,
    0
};

DataAttribute iedModel_LD0_IN8GGIO1_Ind4_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind4,
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind4_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_IN8GGIO1_Ind4_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind4,
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind4_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_IN8GGIO1_Ind4_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind4,
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind4_dU,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_IN8GGIO1_Ind4_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind4,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_IN8GGIO1_Ind5 = {
    DataObjectModelType,
    "Ind5",
    (ModelNode*) &iedModel_LD0_IN8GGIO1,
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind6,
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind5_stVal,
    0
};

DataAttribute iedModel_LD0_IN8GGIO1_Ind5_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind5,
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind5_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_IN8GGIO1_Ind5_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind5,
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind5_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_IN8GGIO1_Ind5_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind5,
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind5_dU,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_IN8GGIO1_Ind5_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind5,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_IN8GGIO1_Ind6 = {
    DataObjectModelType,
    "Ind6",
    (ModelNode*) &iedModel_LD0_IN8GGIO1,
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind7,
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind6_stVal,
    0
};

DataAttribute iedModel_LD0_IN8GGIO1_Ind6_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind6,
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind6_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_IN8GGIO1_Ind6_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind6,
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind6_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_IN8GGIO1_Ind6_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind6,
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind6_dU,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_IN8GGIO1_Ind6_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind6,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_IN8GGIO1_Ind7 = {
    DataObjectModelType,
    "Ind7",
    (ModelNode*) &iedModel_LD0_IN8GGIO1,
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind8,
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind7_stVal,
    0
};

DataAttribute iedModel_LD0_IN8GGIO1_Ind7_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind7,
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind7_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_IN8GGIO1_Ind7_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind7,
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind7_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_IN8GGIO1_Ind7_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind7,
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind7_dU,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_IN8GGIO1_Ind7_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind7,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_IN8GGIO1_Ind8 = {
    DataObjectModelType,
    "Ind8",
    (ModelNode*) &iedModel_LD0_IN8GGIO1,
    NULL,
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind8_stVal,
    0
};

DataAttribute iedModel_LD0_IN8GGIO1_Ind8_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind8,
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind8_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_IN8GGIO1_Ind8_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind8,
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind8_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_IN8GGIO1_Ind8_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind8,
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind8_dU,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_IN8GGIO1_Ind8_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_IN8GGIO1_Ind8,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

LogicalNode iedModel_LD0_CBCSWI1 = {
    LogicalNodeModelType,
    "CBCSWI1",
    (ModelNode*) &iedModel_LD0,
    (ModelNode*) &iedModel_LD0_CBXCBR1,
    (ModelNode*) &iedModel_LD0_CBCSWI1_Beh,
};

DataObject iedModel_LD0_CBCSWI1_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_LD0_CBCSWI1,
    (ModelNode*) &iedModel_LD0_CBCSWI1_Loc,
    (ModelNode*) &iedModel_LD0_CBCSWI1_Beh_stVal,
    0
};

DataAttribute iedModel_LD0_CBCSWI1_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_CBCSWI1_Beh,
    (ModelNode*) &iedModel_LD0_CBCSWI1_Beh_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_CBCSWI1_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_CBCSWI1_Beh,
    (ModelNode*) &iedModel_LD0_CBCSWI1_Beh_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_CBCSWI1_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_CBCSWI1_Beh,
    (ModelNode*) &iedModel_LD0_CBCSWI1_Beh_dU,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_CBCSWI1_Beh_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_CBCSWI1_Beh,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_CBCSWI1_Loc = {
    DataObjectModelType,
    "Loc",
    (ModelNode*) &iedModel_LD0_CBCSWI1,
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos,
    (ModelNode*) &iedModel_LD0_CBCSWI1_Loc_stVal,
    0
};

DataAttribute iedModel_LD0_CBCSWI1_Loc_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_CBCSWI1_Loc,
    (ModelNode*) &iedModel_LD0_CBCSWI1_Loc_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_CBCSWI1_Loc_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_CBCSWI1_Loc,
    (ModelNode*) &iedModel_LD0_CBCSWI1_Loc_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_CBCSWI1_Loc_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_CBCSWI1_Loc,
    (ModelNode*) &iedModel_LD0_CBCSWI1_Loc_dU,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_CBCSWI1_Loc_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_CBCSWI1_Loc,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_CBCSWI1_Pos = {
    DataObjectModelType,
    "Pos",
    (ModelNode*) &iedModel_LD0_CBCSWI1,
    NULL,
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_SBOw,
    0
};

DataAttribute iedModel_LD0_CBCSWI1_Pos_SBOw = {
    DataAttributeModelType,
    "SBOw",
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos,
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_Oper,
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_SBOw_ctlVal,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_CBCSWI1_Pos_SBOw_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_SBOw,
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_SBOw_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_CBCSWI1_Pos_SBOw_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_SBOw,
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_SBOw_ctlNum,
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_SBOw_origin_orCat,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_CBCSWI1_Pos_SBOw_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_SBOw_origin,
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_SBOw_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_CBCSWI1_Pos_SBOw_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_SBOw_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_CBCSWI1_Pos_SBOw_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_SBOw,
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_SBOw_T,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_CBCSWI1_Pos_SBOw_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_SBOw,
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_SBOw_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_CBCSWI1_Pos_SBOw_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_SBOw,
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_SBOw_Check,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_CBCSWI1_Pos_SBOw_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_SBOw,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_CBCSWI1_Pos_Oper = {
    DataAttributeModelType,
    "Oper",
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos,
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_Cancel,
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_Oper_ctlVal,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_CBCSWI1_Pos_Oper_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_Oper,
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_Oper_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_CBCSWI1_Pos_Oper_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_Oper,
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_Oper_ctlNum,
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_Oper_origin_orCat,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_CBCSWI1_Pos_Oper_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_Oper_origin,
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_Oper_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_CBCSWI1_Pos_Oper_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_Oper_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_CBCSWI1_Pos_Oper_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_Oper,
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_Oper_T,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_CBCSWI1_Pos_Oper_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_Oper,
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_Oper_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_CBCSWI1_Pos_Oper_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_Oper,
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_Oper_Check,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_CBCSWI1_Pos_Oper_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_Oper,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_CBCSWI1_Pos_Cancel = {
    DataAttributeModelType,
    "Cancel",
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos,
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_stVal,
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_Cancel_ctlVal,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_CBCSWI1_Pos_Cancel_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_Cancel,
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_Cancel_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_CBCSWI1_Pos_Cancel_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_Cancel,
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_Cancel_ctlNum,
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_Cancel_origin_orCat,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_CBCSWI1_Pos_Cancel_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_Cancel_origin,
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_Cancel_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_CBCSWI1_Pos_Cancel_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_Cancel_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_CBCSWI1_Pos_Cancel_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_Cancel,
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_Cancel_T,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_CBCSWI1_Pos_Cancel_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_Cancel,
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_Cancel_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_CBCSWI1_Pos_Cancel_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_Cancel,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_CBCSWI1_Pos_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos,
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_CODEDENUM,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_CBCSWI1_Pos_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos,
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_CBCSWI1_Pos_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos,
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_ctlModel,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_CBCSWI1_Pos_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos,
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_sboTimeout,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_CBCSWI1_Pos_sboTimeout = {
    DataAttributeModelType,
    "sboTimeout",
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos,
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_sboClass,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_CBCSWI1_Pos_sboClass = {
    DataAttributeModelType,
    "sboClass",
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos,
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos_dU,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_CBCSWI1_Pos_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_CBCSWI1_Pos,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

LogicalNode iedModel_LD0_CBXCBR1 = {
    LogicalNodeModelType,
    "CBXCBR1",
    (ModelNode*) &iedModel_LD0,
    (ModelNode*) &iedModel_LD0_CBCILO1,
    (ModelNode*) &iedModel_LD0_CBXCBR1_Beh,
};

DataObject iedModel_LD0_CBXCBR1_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_LD0_CBXCBR1,
    (ModelNode*) &iedModel_LD0_CBXCBR1_Loc,
    (ModelNode*) &iedModel_LD0_CBXCBR1_Beh_stVal,
    0
};

DataAttribute iedModel_LD0_CBXCBR1_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_CBXCBR1_Beh,
    (ModelNode*) &iedModel_LD0_CBXCBR1_Beh_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_CBXCBR1_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_CBXCBR1_Beh,
    (ModelNode*) &iedModel_LD0_CBXCBR1_Beh_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_CBXCBR1_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_CBXCBR1_Beh,
    (ModelNode*) &iedModel_LD0_CBXCBR1_Beh_dU,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_CBXCBR1_Beh_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_CBXCBR1_Beh,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_CBXCBR1_Loc = {
    DataObjectModelType,
    "Loc",
    (ModelNode*) &iedModel_LD0_CBXCBR1,
    (ModelNode*) &iedModel_LD0_CBXCBR1_OpCnt,
    (ModelNode*) &iedModel_LD0_CBXCBR1_Loc_stVal,
    0
};

DataAttribute iedModel_LD0_CBXCBR1_Loc_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_CBXCBR1_Loc,
    (ModelNode*) &iedModel_LD0_CBXCBR1_Loc_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_CBXCBR1_Loc_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_CBXCBR1_Loc,
    (ModelNode*) &iedModel_LD0_CBXCBR1_Loc_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_CBXCBR1_Loc_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_CBXCBR1_Loc,
    (ModelNode*) &iedModel_LD0_CBXCBR1_Loc_dU,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_CBXCBR1_Loc_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_CBXCBR1_Loc,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_CBXCBR1_OpCnt = {
    DataObjectModelType,
    "OpCnt",
    (ModelNode*) &iedModel_LD0_CBXCBR1,
    (ModelNode*) &iedModel_LD0_CBXCBR1_CBOpCap,
    (ModelNode*) &iedModel_LD0_CBXCBR1_OpCnt_stVal,
    0
};

DataAttribute iedModel_LD0_CBXCBR1_OpCnt_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_CBXCBR1_OpCnt,
    (ModelNode*) &iedModel_LD0_CBXCBR1_OpCnt_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_CBXCBR1_OpCnt_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_CBXCBR1_OpCnt,
    (ModelNode*) &iedModel_LD0_CBXCBR1_OpCnt_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_CBXCBR1_OpCnt_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_CBXCBR1_OpCnt,
    (ModelNode*) &iedModel_LD0_CBXCBR1_OpCnt_dU,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_CBXCBR1_OpCnt_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_CBXCBR1_OpCnt,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_CBXCBR1_CBOpCap = {
    DataObjectModelType,
    "CBOpCap",
    (ModelNode*) &iedModel_LD0_CBXCBR1,
    (ModelNode*) &iedModel_LD0_CBXCBR1_Pos,
    (ModelNode*) &iedModel_LD0_CBXCBR1_CBOpCap_stVal,
    0
};

DataAttribute iedModel_LD0_CBXCBR1_CBOpCap_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_CBXCBR1_CBOpCap,
    (ModelNode*) &iedModel_LD0_CBXCBR1_CBOpCap_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_CBXCBR1_CBOpCap_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_CBXCBR1_CBOpCap,
    (ModelNode*) &iedModel_LD0_CBXCBR1_CBOpCap_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_CBXCBR1_CBOpCap_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_CBXCBR1_CBOpCap,
    (ModelNode*) &iedModel_LD0_CBXCBR1_CBOpCap_dU,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_CBXCBR1_CBOpCap_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_CBXCBR1_CBOpCap,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_CBXCBR1_Pos = {
    DataObjectModelType,
    "Pos",
    (ModelNode*) &iedModel_LD0_CBXCBR1,
    (ModelNode*) &iedModel_LD0_CBXCBR1_BlkOpn,
    (ModelNode*) &iedModel_LD0_CBXCBR1_Pos_stVal,
    0
};

DataAttribute iedModel_LD0_CBXCBR1_Pos_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_CBXCBR1_Pos,
    (ModelNode*) &iedModel_LD0_CBXCBR1_Pos_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_CODEDENUM,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_CBXCBR1_Pos_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_CBXCBR1_Pos,
    (ModelNode*) &iedModel_LD0_CBXCBR1_Pos_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_CBXCBR1_Pos_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_CBXCBR1_Pos,
    (ModelNode*) &iedModel_LD0_CBXCBR1_Pos_ctlModel,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_CBXCBR1_Pos_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_LD0_CBXCBR1_Pos,
    (ModelNode*) &iedModel_LD0_CBXCBR1_Pos_dU,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_CBXCBR1_Pos_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_CBXCBR1_Pos,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_CBXCBR1_BlkOpn = {
    DataObjectModelType,
    "BlkOpn",
    (ModelNode*) &iedModel_LD0_CBXCBR1,
    (ModelNode*) &iedModel_LD0_CBXCBR1_BlkCls,
    (ModelNode*) &iedModel_LD0_CBXCBR1_BlkOpn_stVal,
    0
};

DataAttribute iedModel_LD0_CBXCBR1_BlkOpn_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_CBXCBR1_BlkOpn,
    (ModelNode*) &iedModel_LD0_CBXCBR1_BlkOpn_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_CBXCBR1_BlkOpn_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_CBXCBR1_BlkOpn,
    (ModelNode*) &iedModel_LD0_CBXCBR1_BlkOpn_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_CBXCBR1_BlkOpn_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_CBXCBR1_BlkOpn,
    (ModelNode*) &iedModel_LD0_CBXCBR1_BlkOpn_ctlModel,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_CBXCBR1_BlkOpn_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_LD0_CBXCBR1_BlkOpn,
    (ModelNode*) &iedModel_LD0_CBXCBR1_BlkOpn_dU,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_CBXCBR1_BlkOpn_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_CBXCBR1_BlkOpn,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_CBXCBR1_BlkCls = {
    DataObjectModelType,
    "BlkCls",
    (ModelNode*) &iedModel_LD0_CBXCBR1,
    NULL,
    (ModelNode*) &iedModel_LD0_CBXCBR1_BlkCls_stVal,
    0
};

DataAttribute iedModel_LD0_CBXCBR1_BlkCls_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_CBXCBR1_BlkCls,
    (ModelNode*) &iedModel_LD0_CBXCBR1_BlkCls_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_CBXCBR1_BlkCls_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_CBXCBR1_BlkCls,
    (ModelNode*) &iedModel_LD0_CBXCBR1_BlkCls_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_CBXCBR1_BlkCls_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_CBXCBR1_BlkCls,
    (ModelNode*) &iedModel_LD0_CBXCBR1_BlkCls_ctlModel,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_CBXCBR1_BlkCls_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_LD0_CBXCBR1_BlkCls,
    (ModelNode*) &iedModel_LD0_CBXCBR1_BlkCls_dU,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_CBXCBR1_BlkCls_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_CBXCBR1_BlkCls,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

LogicalNode iedModel_LD0_CBCILO1 = {
    LogicalNodeModelType,
    "CBCILO1",
    (ModelNode*) &iedModel_LD0,
    (ModelNode*) &iedModel_LD0_IN8GGIO2,
    (ModelNode*) &iedModel_LD0_CBCILO1_Beh,
};

DataObject iedModel_LD0_CBCILO1_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_LD0_CBCILO1,
    (ModelNode*) &iedModel_LD0_CBCILO1_EnaOpn,
    (ModelNode*) &iedModel_LD0_CBCILO1_Beh_stVal,
    0
};

DataAttribute iedModel_LD0_CBCILO1_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_CBCILO1_Beh,
    (ModelNode*) &iedModel_LD0_CBCILO1_Beh_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_CBCILO1_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_CBCILO1_Beh,
    (ModelNode*) &iedModel_LD0_CBCILO1_Beh_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_CBCILO1_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_CBCILO1_Beh,
    (ModelNode*) &iedModel_LD0_CBCILO1_Beh_dU,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_CBCILO1_Beh_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_CBCILO1_Beh,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_CBCILO1_EnaOpn = {
    DataObjectModelType,
    "EnaOpn",
    (ModelNode*) &iedModel_LD0_CBCILO1,
    (ModelNode*) &iedModel_LD0_CBCILO1_EnaCls,
    (ModelNode*) &iedModel_LD0_CBCILO1_EnaOpn_stVal,
    0
};

DataAttribute iedModel_LD0_CBCILO1_EnaOpn_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_CBCILO1_EnaOpn,
    (ModelNode*) &iedModel_LD0_CBCILO1_EnaOpn_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_CBCILO1_EnaOpn_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_CBCILO1_EnaOpn,
    (ModelNode*) &iedModel_LD0_CBCILO1_EnaOpn_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_CBCILO1_EnaOpn_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_CBCILO1_EnaOpn,
    (ModelNode*) &iedModel_LD0_CBCILO1_EnaOpn_dU,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_CBCILO1_EnaOpn_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_CBCILO1_EnaOpn,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_CBCILO1_EnaCls = {
    DataObjectModelType,
    "EnaCls",
    (ModelNode*) &iedModel_LD0_CBCILO1,
    NULL,
    (ModelNode*) &iedModel_LD0_CBCILO1_EnaCls_stVal,
    0
};

DataAttribute iedModel_LD0_CBCILO1_EnaCls_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_CBCILO1_EnaCls,
    (ModelNode*) &iedModel_LD0_CBCILO1_EnaCls_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_CBCILO1_EnaCls_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_CBCILO1_EnaCls,
    (ModelNode*) &iedModel_LD0_CBCILO1_EnaCls_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_CBCILO1_EnaCls_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_CBCILO1_EnaCls,
    (ModelNode*) &iedModel_LD0_CBCILO1_EnaCls_dU,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_CBCILO1_EnaCls_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_CBCILO1_EnaCls,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

LogicalNode iedModel_LD0_IN8GGIO2 = {
    LogicalNodeModelType,
    "IN8GGIO2",
    (ModelNode*) &iedModel_LD0,
    (ModelNode*) &iedModel_LD0_TRKLTRK1,
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Beh,
};

DataObject iedModel_LD0_IN8GGIO2_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_LD0_IN8GGIO2,
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind1,
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Beh_stVal,
    0
};

DataAttribute iedModel_LD0_IN8GGIO2_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Beh,
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Beh_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_IN8GGIO2_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Beh,
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Beh_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_IN8GGIO2_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Beh,
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Beh_dU,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_IN8GGIO2_Beh_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Beh,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_IN8GGIO2_Ind1 = {
    DataObjectModelType,
    "Ind1",
    (ModelNode*) &iedModel_LD0_IN8GGIO2,
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind2,
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind1_stVal,
    0
};

DataAttribute iedModel_LD0_IN8GGIO2_Ind1_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind1,
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind1_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_IN8GGIO2_Ind1_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind1,
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind1_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_IN8GGIO2_Ind1_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind1,
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind1_dU,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_IN8GGIO2_Ind1_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind1,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_IN8GGIO2_Ind2 = {
    DataObjectModelType,
    "Ind2",
    (ModelNode*) &iedModel_LD0_IN8GGIO2,
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind3,
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind2_stVal,
    0
};

DataAttribute iedModel_LD0_IN8GGIO2_Ind2_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind2,
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind2_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_IN8GGIO2_Ind2_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind2,
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind2_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_IN8GGIO2_Ind2_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind2,
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind2_dU,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_IN8GGIO2_Ind2_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind2,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_IN8GGIO2_Ind3 = {
    DataObjectModelType,
    "Ind3",
    (ModelNode*) &iedModel_LD0_IN8GGIO2,
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind4,
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind3_stVal,
    0
};

DataAttribute iedModel_LD0_IN8GGIO2_Ind3_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind3,
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind3_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_IN8GGIO2_Ind3_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind3,
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind3_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_IN8GGIO2_Ind3_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind3,
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind3_dU,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_IN8GGIO2_Ind3_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind3,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_IN8GGIO2_Ind4 = {
    DataObjectModelType,
    "Ind4",
    (ModelNode*) &iedModel_LD0_IN8GGIO2,
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind5,
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind4_stVal,
    0
};

DataAttribute iedModel_LD0_IN8GGIO2_Ind4_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind4,
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind4_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_IN8GGIO2_Ind4_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind4,
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind4_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_IN8GGIO2_Ind4_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind4,
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind4_dU,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_IN8GGIO2_Ind4_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind4,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_IN8GGIO2_Ind5 = {
    DataObjectModelType,
    "Ind5",
    (ModelNode*) &iedModel_LD0_IN8GGIO2,
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind6,
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind5_stVal,
    0
};

DataAttribute iedModel_LD0_IN8GGIO2_Ind5_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind5,
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind5_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_IN8GGIO2_Ind5_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind5,
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind5_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_IN8GGIO2_Ind5_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind5,
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind5_dU,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_IN8GGIO2_Ind5_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind5,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_IN8GGIO2_Ind6 = {
    DataObjectModelType,
    "Ind6",
    (ModelNode*) &iedModel_LD0_IN8GGIO2,
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind7,
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind6_stVal,
    0
};

DataAttribute iedModel_LD0_IN8GGIO2_Ind6_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind6,
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind6_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_IN8GGIO2_Ind6_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind6,
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind6_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_IN8GGIO2_Ind6_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind6,
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind6_dU,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_IN8GGIO2_Ind6_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind6,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_IN8GGIO2_Ind7 = {
    DataObjectModelType,
    "Ind7",
    (ModelNode*) &iedModel_LD0_IN8GGIO2,
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind8,
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind7_stVal,
    0
};

DataAttribute iedModel_LD0_IN8GGIO2_Ind7_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind7,
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind7_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_IN8GGIO2_Ind7_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind7,
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind7_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_IN8GGIO2_Ind7_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind7,
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind7_dU,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_IN8GGIO2_Ind7_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind7,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_IN8GGIO2_Ind8 = {
    DataObjectModelType,
    "Ind8",
    (ModelNode*) &iedModel_LD0_IN8GGIO2,
    NULL,
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind8_stVal,
    0
};

DataAttribute iedModel_LD0_IN8GGIO2_Ind8_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind8,
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind8_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_IN8GGIO2_Ind8_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind8,
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind8_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_IN8GGIO2_Ind8_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind8,
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind8_dU,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_IN8GGIO2_Ind8_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_IN8GGIO2_Ind8,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

LogicalNode iedModel_LD0_TRKLTRK1 = {
    LogicalNodeModelType,
    "TRKLTRK1",
    (ModelNode*) &iedModel_LD0,
    NULL,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_Beh,
};

DataObject iedModel_LD0_TRKLTRK1_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_LD0_TRKLTRK1,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_SpcTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_Beh_stVal,
    0
};

DataAttribute iedModel_LD0_TRKLTRK1_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_Beh,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_Beh_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_Beh,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_Beh_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_Beh,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_Beh_dU,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_Beh_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_Beh,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_TRKLTRK1_SpcTrk = {
    DataObjectModelType,
    "SpcTrk",
    (ModelNode*) &iedModel_LD0_TRKLTRK1,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_DpcTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_SpcTrk_objRef,
    0
};

DataAttribute iedModel_LD0_TRKLTRK1_SpcTrk_objRef = {
    DataAttributeModelType,
    "objRef",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_SpcTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_SpcTrk_serviceType,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_VISIBLE_STRING_129,
    0 + TRG_OPT_DATA_UPDATE,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_SpcTrk_serviceType = {
    DataAttributeModelType,
    "serviceType",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_SpcTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_SpcTrk_errorCode,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_SpcTrk_errorCode = {
    DataAttributeModelType,
    "errorCode",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_SpcTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_SpcTrk_originatorID,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_SpcTrk_originatorID = {
    DataAttributeModelType,
    "originatorID",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_SpcTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_SpcTrk_t,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_SpcTrk_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_SpcTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_SpcTrk_ctlVal,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_SpcTrk_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_SpcTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_SpcTrk_origin,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_SpcTrk_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_SpcTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_SpcTrk_ctlNum,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_SpcTrk_origin_orCat,
    0,
    IEC61850_FC_SR,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_SpcTrk_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_SpcTrk_origin,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_SpcTrk_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_SpcTrk_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_SpcTrk_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_SpcTrk_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_SpcTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_SpcTrk_T,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_SpcTrk_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_SpcTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_SpcTrk_Test,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_SpcTrk_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_SpcTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_SpcTrk_Check,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_SpcTrk_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_SpcTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_SpcTrk_respAddCause,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_SpcTrk_respAddCause = {
    DataAttributeModelType,
    "respAddCause",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_SpcTrk,
    NULL,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_LD0_TRKLTRK1_DpcTrk = {
    DataObjectModelType,
    "DpcTrk",
    (ModelNode*) &iedModel_LD0_TRKLTRK1,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IncTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_DpcTrk_objRef,
    0
};

DataAttribute iedModel_LD0_TRKLTRK1_DpcTrk_objRef = {
    DataAttributeModelType,
    "objRef",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_DpcTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_DpcTrk_serviceType,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_VISIBLE_STRING_129,
    0 + TRG_OPT_DATA_UPDATE,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_DpcTrk_serviceType = {
    DataAttributeModelType,
    "serviceType",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_DpcTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_DpcTrk_errorCode,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_DpcTrk_errorCode = {
    DataAttributeModelType,
    "errorCode",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_DpcTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_DpcTrk_originatorID,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_DpcTrk_originatorID = {
    DataAttributeModelType,
    "originatorID",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_DpcTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_DpcTrk_t,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_DpcTrk_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_DpcTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_DpcTrk_ctlVal,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_DpcTrk_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_DpcTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_DpcTrk_origin,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_DpcTrk_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_DpcTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_DpcTrk_ctlNum,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_DpcTrk_origin_orCat,
    0,
    IEC61850_FC_SR,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_DpcTrk_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_DpcTrk_origin,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_DpcTrk_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_DpcTrk_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_DpcTrk_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_DpcTrk_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_DpcTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_DpcTrk_T,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_DpcTrk_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_DpcTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_DpcTrk_Test,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_DpcTrk_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_DpcTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_DpcTrk_Check,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_DpcTrk_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_DpcTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_DpcTrk_respAddCause,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_DpcTrk_respAddCause = {
    DataAttributeModelType,
    "respAddCause",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_DpcTrk,
    NULL,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_LD0_TRKLTRK1_IncTrk = {
    DataObjectModelType,
    "IncTrk",
    (ModelNode*) &iedModel_LD0_TRKLTRK1,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_EncTrk1,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IncTrk_objRef,
    0
};

DataAttribute iedModel_LD0_TRKLTRK1_IncTrk_objRef = {
    DataAttributeModelType,
    "objRef",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IncTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IncTrk_serviceType,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_VISIBLE_STRING_129,
    0 + TRG_OPT_DATA_UPDATE,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_IncTrk_serviceType = {
    DataAttributeModelType,
    "serviceType",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IncTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IncTrk_errorCode,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_IncTrk_errorCode = {
    DataAttributeModelType,
    "errorCode",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IncTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IncTrk_originatorID,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_IncTrk_originatorID = {
    DataAttributeModelType,
    "originatorID",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IncTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IncTrk_t,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_IncTrk_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IncTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IncTrk_ctlVal,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_IncTrk_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IncTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IncTrk_origin,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_INT32,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_IncTrk_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IncTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IncTrk_ctlNum,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IncTrk_origin_orCat,
    0,
    IEC61850_FC_SR,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_IncTrk_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IncTrk_origin,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IncTrk_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_IncTrk_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IncTrk_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_IncTrk_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IncTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IncTrk_T,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_IncTrk_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IncTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IncTrk_Test,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_IncTrk_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IncTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IncTrk_Check,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_IncTrk_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IncTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IncTrk_respAddCause,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_IncTrk_respAddCause = {
    DataAttributeModelType,
    "respAddCause",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IncTrk,
    NULL,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_LD0_TRKLTRK1_EncTrk1 = {
    DataObjectModelType,
    "EncTrk1",
    (ModelNode*) &iedModel_LD0_TRKLTRK1,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BscTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_EncTrk1_objRef,
    0
};

DataAttribute iedModel_LD0_TRKLTRK1_EncTrk1_objRef = {
    DataAttributeModelType,
    "objRef",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_EncTrk1,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_EncTrk1_serviceType,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_VISIBLE_STRING_129,
    0 + TRG_OPT_DATA_UPDATE,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_EncTrk1_serviceType = {
    DataAttributeModelType,
    "serviceType",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_EncTrk1,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_EncTrk1_errorCode,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_EncTrk1_errorCode = {
    DataAttributeModelType,
    "errorCode",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_EncTrk1,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_EncTrk1_originatorID,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_EncTrk1_originatorID = {
    DataAttributeModelType,
    "originatorID",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_EncTrk1,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_EncTrk1_t,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_EncTrk1_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_EncTrk1,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_EncTrk1_ctlVal,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_EncTrk1_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_EncTrk1,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_EncTrk1_origin,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_EncTrk1_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_EncTrk1,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_EncTrk1_ctlNum,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_EncTrk1_origin_orCat,
    0,
    IEC61850_FC_SR,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_EncTrk1_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_EncTrk1_origin,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_EncTrk1_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_EncTrk1_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_EncTrk1_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_EncTrk1_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_EncTrk1,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_EncTrk1_T,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_EncTrk1_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_EncTrk1,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_EncTrk1_Test,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_EncTrk1_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_EncTrk1,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_EncTrk1_Check,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_EncTrk1_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_EncTrk1,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_EncTrk1_respAddCause,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_EncTrk1_respAddCause = {
    DataAttributeModelType,
    "respAddCause",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_EncTrk1,
    NULL,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_LD0_TRKLTRK1_BscTrk = {
    DataObjectModelType,
    "BscTrk",
    (ModelNode*) &iedModel_LD0_TRKLTRK1,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IscTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BscTrk_objRef,
    0
};

DataAttribute iedModel_LD0_TRKLTRK1_BscTrk_objRef = {
    DataAttributeModelType,
    "objRef",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BscTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BscTrk_serviceType,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_VISIBLE_STRING_129,
    0 + TRG_OPT_DATA_UPDATE,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_BscTrk_serviceType = {
    DataAttributeModelType,
    "serviceType",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BscTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BscTrk_errorCode,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_BscTrk_errorCode = {
    DataAttributeModelType,
    "errorCode",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BscTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BscTrk_originatorID,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_BscTrk_originatorID = {
    DataAttributeModelType,
    "originatorID",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BscTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BscTrk_t,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_BscTrk_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BscTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BscTrk_ctlVal,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_BscTrk_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BscTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BscTrk_origin,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_CODEDENUM,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_BscTrk_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BscTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BscTrk_ctlNum,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BscTrk_origin_orCat,
    0,
    IEC61850_FC_SR,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_BscTrk_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BscTrk_origin,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BscTrk_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_BscTrk_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BscTrk_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_BscTrk_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BscTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BscTrk_T,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_BscTrk_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BscTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BscTrk_Test,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_BscTrk_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BscTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BscTrk_Check,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_BscTrk_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BscTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BscTrk_respAddCause,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_BscTrk_respAddCause = {
    DataAttributeModelType,
    "respAddCause",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BscTrk,
    NULL,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_LD0_TRKLTRK1_IscTrk = {
    DataObjectModelType,
    "IscTrk",
    (ModelNode*) &iedModel_LD0_TRKLTRK1,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_UrcbTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IscTrk_objRef,
    0
};

DataAttribute iedModel_LD0_TRKLTRK1_IscTrk_objRef = {
    DataAttributeModelType,
    "objRef",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IscTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IscTrk_serviceType,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_VISIBLE_STRING_129,
    0 + TRG_OPT_DATA_UPDATE,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_IscTrk_serviceType = {
    DataAttributeModelType,
    "serviceType",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IscTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IscTrk_errorCode,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_IscTrk_errorCode = {
    DataAttributeModelType,
    "errorCode",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IscTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IscTrk_originatorID,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_IscTrk_originatorID = {
    DataAttributeModelType,
    "originatorID",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IscTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IscTrk_t,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_IscTrk_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IscTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IscTrk_ctlVal,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_IscTrk_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IscTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IscTrk_origin,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_INT8,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_IscTrk_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IscTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IscTrk_ctlNum,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IscTrk_origin_orCat,
    0,
    IEC61850_FC_SR,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_IscTrk_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IscTrk_origin,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IscTrk_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_IscTrk_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IscTrk_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_IscTrk_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IscTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IscTrk_T,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_IscTrk_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IscTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IscTrk_Test,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_IscTrk_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IscTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IscTrk_Check,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_IscTrk_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IscTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IscTrk_respAddCause,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_IscTrk_respAddCause = {
    DataAttributeModelType,
    "respAddCause",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_IscTrk,
    NULL,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_LD0_TRKLTRK1_UrcbTrk = {
    DataObjectModelType,
    "UrcbTrk",
    (ModelNode*) &iedModel_LD0_TRKLTRK1,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BrcbTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_UrcbTrk_objRef,
    0
};

DataAttribute iedModel_LD0_TRKLTRK1_UrcbTrk_objRef = {
    DataAttributeModelType,
    "objRef",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_UrcbTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_UrcbTrk_serviceType,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_VISIBLE_STRING_129,
    0 + TRG_OPT_DATA_UPDATE,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_UrcbTrk_serviceType = {
    DataAttributeModelType,
    "serviceType",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_UrcbTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_UrcbTrk_errorCode,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_UrcbTrk_errorCode = {
    DataAttributeModelType,
    "errorCode",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_UrcbTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_UrcbTrk_originatorID,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_UrcbTrk_originatorID = {
    DataAttributeModelType,
    "originatorID",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_UrcbTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_UrcbTrk_t,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_UrcbTrk_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_UrcbTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_UrcbTrk_rptID,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_UrcbTrk_rptID = {
    DataAttributeModelType,
    "rptID",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_UrcbTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_UrcbTrk_rptEna,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_VISIBLE_STRING_129,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_UrcbTrk_rptEna = {
    DataAttributeModelType,
    "rptEna",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_UrcbTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_UrcbTrk_resv,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_UrcbTrk_resv = {
    DataAttributeModelType,
    "resv",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_UrcbTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_UrcbTrk_datSet,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_UrcbTrk_datSet = {
    DataAttributeModelType,
    "datSet",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_UrcbTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_UrcbTrk_confRev,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_VISIBLE_STRING_129,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_UrcbTrk_confRev = {
    DataAttributeModelType,
    "confRev",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_UrcbTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_UrcbTrk_optFlds,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_UrcbTrk_optFlds = {
    DataAttributeModelType,
    "optFlds",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_UrcbTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_UrcbTrk_bufTm,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_OPTFLDS,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_UrcbTrk_bufTm = {
    DataAttributeModelType,
    "bufTm",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_UrcbTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_UrcbTrk_sqNum,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_UrcbTrk_sqNum = {
    DataAttributeModelType,
    "sqNum",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_UrcbTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_UrcbTrk_trgOps,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_UrcbTrk_trgOps = {
    DataAttributeModelType,
    "trgOps",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_UrcbTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_UrcbTrk_intgPd,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_TRGOPS,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_UrcbTrk_intgPd = {
    DataAttributeModelType,
    "intgPd",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_UrcbTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_UrcbTrk_gi,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_UrcbTrk_gi = {
    DataAttributeModelType,
    "gi",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_UrcbTrk,
    NULL,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataObject iedModel_LD0_TRKLTRK1_BrcbTrk = {
    DataObjectModelType,
    "BrcbTrk",
    (ModelNode*) &iedModel_LD0_TRKLTRK1,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_GocbTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BrcbTrk_objRef,
    0
};

DataAttribute iedModel_LD0_TRKLTRK1_BrcbTrk_objRef = {
    DataAttributeModelType,
    "objRef",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BrcbTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BrcbTrk_serviceType,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_VISIBLE_STRING_129,
    0 + TRG_OPT_DATA_UPDATE,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_BrcbTrk_serviceType = {
    DataAttributeModelType,
    "serviceType",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BrcbTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BrcbTrk_errorCode,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_BrcbTrk_errorCode = {
    DataAttributeModelType,
    "errorCode",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BrcbTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BrcbTrk_originatorID,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_BrcbTrk_originatorID = {
    DataAttributeModelType,
    "originatorID",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BrcbTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BrcbTrk_t,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_BrcbTrk_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BrcbTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BrcbTrk_rptID,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_BrcbTrk_rptID = {
    DataAttributeModelType,
    "rptID",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BrcbTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BrcbTrk_rptEna,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_VISIBLE_STRING_129,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_BrcbTrk_rptEna = {
    DataAttributeModelType,
    "rptEna",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BrcbTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BrcbTrk_datSet,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_BrcbTrk_datSet = {
    DataAttributeModelType,
    "datSet",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BrcbTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BrcbTrk_confRev,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_VISIBLE_STRING_129,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_BrcbTrk_confRev = {
    DataAttributeModelType,
    "confRev",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BrcbTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BrcbTrk_optFlds,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_BrcbTrk_optFlds = {
    DataAttributeModelType,
    "optFlds",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BrcbTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BrcbTrk_bufTm,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_OPTFLDS,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_BrcbTrk_bufTm = {
    DataAttributeModelType,
    "bufTm",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BrcbTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BrcbTrk_sqNum,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_BrcbTrk_sqNum = {
    DataAttributeModelType,
    "sqNum",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BrcbTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BrcbTrk_trgOps,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_BrcbTrk_trgOps = {
    DataAttributeModelType,
    "trgOps",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BrcbTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BrcbTrk_intgPd,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_TRGOPS,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_BrcbTrk_intgPd = {
    DataAttributeModelType,
    "intgPd",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BrcbTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BrcbTrk_gi,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_BrcbTrk_gi = {
    DataAttributeModelType,
    "gi",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BrcbTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BrcbTrk_purgeBuf,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_BrcbTrk_purgeBuf = {
    DataAttributeModelType,
    "purgeBuf",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BrcbTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BrcbTrk_entryID,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_BrcbTrk_entryID = {
    DataAttributeModelType,
    "entryID",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BrcbTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BrcbTrk_timeOfEntry,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_OCTET_STRING_8,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_BrcbTrk_timeOfEntry = {
    DataAttributeModelType,
    "timeOfEntry",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_BrcbTrk,
    NULL,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_ENTRY_TIME,
    0,
    NULL,
    0};

DataObject iedModel_LD0_TRKLTRK1_GocbTrk = {
    DataObjectModelType,
    "GocbTrk",
    (ModelNode*) &iedModel_LD0_TRKLTRK1,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_SgcbTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_GocbTrk_objRef,
    0
};

DataAttribute iedModel_LD0_TRKLTRK1_GocbTrk_objRef = {
    DataAttributeModelType,
    "objRef",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_GocbTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_GocbTrk_serviceType,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_VISIBLE_STRING_129,
    0 + TRG_OPT_DATA_UPDATE,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_GocbTrk_serviceType = {
    DataAttributeModelType,
    "serviceType",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_GocbTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_GocbTrk_errorCode,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_GocbTrk_errorCode = {
    DataAttributeModelType,
    "errorCode",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_GocbTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_GocbTrk_originatorID,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_GocbTrk_originatorID = {
    DataAttributeModelType,
    "originatorID",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_GocbTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_GocbTrk_t,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_GocbTrk_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_GocbTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_GocbTrk_goEna,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_GocbTrk_goEna = {
    DataAttributeModelType,
    "goEna",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_GocbTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_GocbTrk_goID,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_GocbTrk_goID = {
    DataAttributeModelType,
    "goID",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_GocbTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_GocbTrk_datSet,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_VISIBLE_STRING_129,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_GocbTrk_datSet = {
    DataAttributeModelType,
    "datSet",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_GocbTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_GocbTrk_confRev,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_VISIBLE_STRING_129,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_GocbTrk_confRev = {
    DataAttributeModelType,
    "confRev",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_GocbTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_GocbTrk_ndsCom,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_GocbTrk_ndsCom = {
    DataAttributeModelType,
    "ndsCom",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_GocbTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_GocbTrk_dstAddress,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_GocbTrk_dstAddress = {
    DataAttributeModelType,
    "dstAddress",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_GocbTrk,
    NULL,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_PHYCOMADDR,
    0,
    NULL,
    0};

DataObject iedModel_LD0_TRKLTRK1_SgcbTrk = {
    DataObjectModelType,
    "SgcbTrk",
    (ModelNode*) &iedModel_LD0_TRKLTRK1,
    NULL,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_SgcbTrk_objRef,
    0
};

DataAttribute iedModel_LD0_TRKLTRK1_SgcbTrk_objRef = {
    DataAttributeModelType,
    "objRef",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_SgcbTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_SgcbTrk_serviceType,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_VISIBLE_STRING_129,
    0 + TRG_OPT_DATA_UPDATE,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_SgcbTrk_serviceType = {
    DataAttributeModelType,
    "serviceType",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_SgcbTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_SgcbTrk_errorCode,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_SgcbTrk_errorCode = {
    DataAttributeModelType,
    "errorCode",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_SgcbTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_SgcbTrk_originatorID,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_SgcbTrk_originatorID = {
    DataAttributeModelType,
    "originatorID",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_SgcbTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_SgcbTrk_t,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_SgcbTrk_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_SgcbTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_SgcbTrk_numOfSG,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_SgcbTrk_numOfSG = {
    DataAttributeModelType,
    "numOfSG",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_SgcbTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_SgcbTrk_actSG,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_SgcbTrk_actSG = {
    DataAttributeModelType,
    "actSG",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_SgcbTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_SgcbTrk_editSG,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_SgcbTrk_editSG = {
    DataAttributeModelType,
    "editSG",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_SgcbTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_SgcbTrk_cnfEdit,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_SgcbTrk_cnfEdit = {
    DataAttributeModelType,
    "cnfEdit",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_SgcbTrk,
    (ModelNode*) &iedModel_LD0_TRKLTRK1_SgcbTrk_lActTm,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_TRKLTRK1_SgcbTrk_lActTm = {
    DataAttributeModelType,
    "lActTm",
    (ModelNode*) &iedModel_LD0_TRKLTRK1_SgcbTrk,
    NULL,
    NULL,
    0,
    IEC61850_FC_SR,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

extern ReportControlBlock iedModel_LD0_LLN0_report0;
extern ReportControlBlock iedModel_LD0_LLN0_report1;
extern ReportControlBlock iedModel_LD0_LLN0_report2;
extern ReportControlBlock iedModel_LD0_LLN0_report3;
extern ReportControlBlock iedModel_LD0_LLN0_report4;
extern ReportControlBlock iedModel_LD0_LLN0_report5;
extern ReportControlBlock iedModel_LD0_LLN0_report6;
extern ReportControlBlock iedModel_LD0_LLN0_report7;
extern ReportControlBlock iedModel_LD0_LLN0_report8;
extern ReportControlBlock iedModel_LD0_LLN0_report9;
extern ReportControlBlock iedModel_LD0_LLN0_report10;
extern ReportControlBlock iedModel_LD0_LLN0_report11;
extern ReportControlBlock iedModel_LD0_LLN0_report12;
extern ReportControlBlock iedModel_LD0_LLN0_report13;
extern ReportControlBlock iedModel_LD0_LLN0_report14;
extern ReportControlBlock iedModel_LD0_LLN0_report15;

ReportControlBlock iedModel_LD0_LLN0_report0 = {&iedModel_LD0_LLN0, "rcb_Meas01", "rcb_Meas", false, "Meas", 3, 19, 32, 500, 0, {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, &iedModel_LD0_LLN0_report1};
ReportControlBlock iedModel_LD0_LLN0_report1 = {&iedModel_LD0_LLN0, "rcb_Meas02", "rcb_Meas", false, "Meas", 3, 19, 32, 500, 0, {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, &iedModel_LD0_LLN0_report2};
ReportControlBlock iedModel_LD0_LLN0_report2 = {&iedModel_LD0_LLN0, "rcb_Meas03", "rcb_Meas", false, "Meas", 3, 19, 32, 500, 0, {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, &iedModel_LD0_LLN0_report3};
ReportControlBlock iedModel_LD0_LLN0_report3 = {&iedModel_LD0_LLN0, "rcb_Meas04", "rcb_Meas", false, "Meas", 3, 19, 32, 500, 0, {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, &iedModel_LD0_LLN0_report4};
ReportControlBlock iedModel_LD0_LLN0_report4 = {&iedModel_LD0_LLN0, "rcb_StatusWithQDA01", "rcb_StatusWithQDA", true, "StatusWithQDA", 3, 19, 32, 500, 0, {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, &iedModel_LD0_LLN0_report5};
ReportControlBlock iedModel_LD0_LLN0_report5 = {&iedModel_LD0_LLN0, "rcb_StatusWithQDA02", "rcb_StatusWithQDA", true, "StatusWithQDA", 3, 19, 32, 500, 0, {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, &iedModel_LD0_LLN0_report6};
ReportControlBlock iedModel_LD0_LLN0_report6 = {&iedModel_LD0_LLN0, "rcb_StatusWithQDA03", "rcb_StatusWithQDA", true, "StatusWithQDA", 3, 19, 32, 500, 0, {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, &iedModel_LD0_LLN0_report7};
ReportControlBlock iedModel_LD0_LLN0_report7 = {&iedModel_LD0_LLN0, "rcb_StatusWithQDA04", "rcb_StatusWithQDA", true, "StatusWithQDA", 3, 19, 32, 500, 0, {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, &iedModel_LD0_LLN0_report8};
ReportControlBlock iedModel_LD0_LLN0_report8 = {&iedModel_LD0_LLN0, "rcb_StatusWithQDO01", "BRCBRptIDwithmaximumLengthHunderd29charactersthatisalongIDnotefficientbuthtatiswhatisspecifiedintheEditionTwostandardAndSomeMore8", true, "MaxLengthDatasetBRCB32characters", 3, 19, 32, 500, 0, {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, &iedModel_LD0_LLN0_report9};
ReportControlBlock iedModel_LD0_LLN0_report9 = {&iedModel_LD0_LLN0, "rcb_StatusWithQDO02", "BRCBRptIDwithmaximumLengthHunderd29charactersthatisalongIDnotefficientbuthtatiswhatisspecifiedintheEditionTwostandardAndSomeMore8", true, "MaxLengthDatasetBRCB32characters", 3, 19, 32, 500, 0, {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, &iedModel_LD0_LLN0_report10};
ReportControlBlock iedModel_LD0_LLN0_report10 = {&iedModel_LD0_LLN0, "rcb_StatusWithQDO03", "BRCBRptIDwithmaximumLengthHunderd29charactersthatisalongIDnotefficientbuthtatiswhatisspecifiedintheEditionTwostandardAndSomeMore8", true, "MaxLengthDatasetBRCB32characters", 3, 19, 32, 500, 0, {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, &iedModel_LD0_LLN0_report11};
ReportControlBlock iedModel_LD0_LLN0_report11 = {&iedModel_LD0_LLN0, "rcb_StatusWithQDO04", "BRCBRptIDwithmaximumLengthHunderd29charactersthatisalongIDnotefficientbuthtatiswhatisspecifiedintheEditionTwostandardAndSomeMore8", true, "MaxLengthDatasetBRCB32characters", 3, 19, 32, 500, 0, {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, &iedModel_LD0_LLN0_report12};
ReportControlBlock iedModel_LD0_LLN0_report12 = {&iedModel_LD0_LLN0, "rcbServiceTracking01", "rcbServiceTracking", true, "ServiceTracking", 3, 20, 32, 500, 0, {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, &iedModel_LD0_LLN0_report13};
ReportControlBlock iedModel_LD0_LLN0_report13 = {&iedModel_LD0_LLN0, "rcbServiceTracking02", "rcbServiceTracking", true, "ServiceTracking", 3, 20, 32, 500, 0, {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, &iedModel_LD0_LLN0_report14};
ReportControlBlock iedModel_LD0_LLN0_report14 = {&iedModel_LD0_LLN0, "rcbServiceTracking03", "rcbServiceTracking", true, "ServiceTracking", 3, 20, 32, 500, 0, {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, &iedModel_LD0_LLN0_report15};
ReportControlBlock iedModel_LD0_LLN0_report15 = {&iedModel_LD0_LLN0, "rcbServiceTracking04", "rcbServiceTracking", true, "ServiceTracking", 3, 20, 32, 500, 0, {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, NULL};


extern GSEControlBlock iedModel_LD0_LLN0_gse0;

static PhyComAddress iedModel_LD0_LLN0_gse0_address = {
  4,
  1,
  1,
  {0x1, 0xc, 0xcd, 0x1, 0x0, 0x1}
};

GSEControlBlock iedModel_LD0_LLN0_gse0 = {&iedModel_LD0_LLN0, "gcbDataSet4Goose", "GOCBGoIDwithmaximumLengthHunderd29charactersthatisalongIDnotefficientbuthtatiswhatisspecifiedintheEditionTwostandardAndSomeMore89", "DataSet4Goose", 1, false, &iedModel_LD0_LLN0_gse0_address, -1, -1, NULL};

extern SettingGroupControlBlock iedModel_LD0_LLN0_sgcb;

SettingGroupControlBlock iedModel_LD0_LLN0_sgcb = {&iedModel_LD0_LLN0, 1, 2, 0, false, 0, 0, NULL};




IedModel iedModel = {
    "TESTIED1",
    &iedModel_LD0,
    &iedModelds_LD0_LLN0_DataSet4Goose,
    &iedModel_LD0_LLN0_report0,
    &iedModel_LD0_LLN0_gse0,
    NULL,
    &iedModel_LD0_LLN0_sgcb,
    NULL,
    NULL,
    initializeValues
};

static void
initializeValues()
{

iedModel_LD0_LLN0_NamPlt_configRev.mmsValue = MmsValue_newVisibleString("2020. 10. 29. 10:26:40");

iedModel_LD0_LLN0_NamPlt_ldNs.mmsValue = MmsValue_newVisibleString("IEC 61850-7-4:2007");

iedModel_LD0_LLN0_Mod_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(1);

iedModel_LD0_LLN0_LEDRs_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(1);

iedModel_LD0_LPHD1_Sim_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(1);

iedModel_LD0_ResetGGIO1_SPCSO1_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(1);

iedModel_LD0_TRPDIF1_OpUnr_dataNs.mmsValue = MmsValue_newVisibleString("www.protecta.hu/prot4win");

iedModel_LD0_CBCSWI1_Pos_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(4);

iedModel_LD0_CBCSWI1_Pos_sboClass.mmsValue = MmsValue_newIntegerFromInt32(0);

iedModel_LD0_CBXCBR1_Pos_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(0);

iedModel_LD0_CBXCBR1_BlkOpn_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(0);

iedModel_LD0_CBXCBR1_BlkCls_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(0);
}
