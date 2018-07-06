/*
 * static_model.c
 *
 * automatically generated from sv.icd
 */
#include "static_model.h"

static void initializeValues();

extern DataSet iedModelds_MUnn_LLN0_PhsMeas1;


extern DataSetEntry iedModelds_MUnn_LLN0_PhsMeas1_fcda0;
extern DataSetEntry iedModelds_MUnn_LLN0_PhsMeas1_fcda1;
extern DataSetEntry iedModelds_MUnn_LLN0_PhsMeas1_fcda2;
extern DataSetEntry iedModelds_MUnn_LLN0_PhsMeas1_fcda3;
extern DataSetEntry iedModelds_MUnn_LLN0_PhsMeas1_fcda4;
extern DataSetEntry iedModelds_MUnn_LLN0_PhsMeas1_fcda5;
extern DataSetEntry iedModelds_MUnn_LLN0_PhsMeas1_fcda6;
extern DataSetEntry iedModelds_MUnn_LLN0_PhsMeas1_fcda7;

DataSetEntry iedModelds_MUnn_LLN0_PhsMeas1_fcda0 = {
  "MUnn",
  false,
  "TCTR1$MX$Amp",
  -1,
  NULL,
  NULL,
  &iedModelds_MUnn_LLN0_PhsMeas1_fcda1
};

DataSetEntry iedModelds_MUnn_LLN0_PhsMeas1_fcda1 = {
  "MUnn",
  false,
  "TCTR2$MX$Amp",
  -1,
  NULL,
  NULL,
  &iedModelds_MUnn_LLN0_PhsMeas1_fcda2
};

DataSetEntry iedModelds_MUnn_LLN0_PhsMeas1_fcda2 = {
  "MUnn",
  false,
  "TCTR3$MX$Amp",
  -1,
  NULL,
  NULL,
  &iedModelds_MUnn_LLN0_PhsMeas1_fcda3
};

DataSetEntry iedModelds_MUnn_LLN0_PhsMeas1_fcda3 = {
  "MUnn",
  false,
  "TCTR4$MX$Amp",
  -1,
  NULL,
  NULL,
  &iedModelds_MUnn_LLN0_PhsMeas1_fcda4
};

DataSetEntry iedModelds_MUnn_LLN0_PhsMeas1_fcda4 = {
  "MUnn",
  false,
  "TVTR1$MX$Vol",
  -1,
  NULL,
  NULL,
  &iedModelds_MUnn_LLN0_PhsMeas1_fcda5
};

DataSetEntry iedModelds_MUnn_LLN0_PhsMeas1_fcda5 = {
  "MUnn",
  false,
  "TVTR2$MX$Vol",
  -1,
  NULL,
  NULL,
  &iedModelds_MUnn_LLN0_PhsMeas1_fcda6
};

DataSetEntry iedModelds_MUnn_LLN0_PhsMeas1_fcda6 = {
  "MUnn",
  false,
  "TVTR3$MX$Vol",
  -1,
  NULL,
  NULL,
  &iedModelds_MUnn_LLN0_PhsMeas1_fcda7
};

DataSetEntry iedModelds_MUnn_LLN0_PhsMeas1_fcda7 = {
  "MUnn",
  false,
  "TVTR4$MX$Vol",
  -1,
  NULL,
  NULL,
  NULL
};

DataSet iedModelds_MUnn_LLN0_PhsMeas1 = {
  "MUnn",
  "LLN0$PhsMeas1",
  8,
  &iedModelds_MUnn_LLN0_PhsMeas1_fcda0,
  NULL
};

LogicalDevice iedModel_MUnn = {
    LogicalDeviceModelType,
    "MUnn",
    (ModelNode*) &iedModel,
    NULL,
    (ModelNode*) &iedModel_MUnn_LLN0
};

LogicalNode iedModel_MUnn_LLN0 = {
    LogicalNodeModelType,
    "LLN0",
    (ModelNode*) &iedModel_MUnn,
    (ModelNode*) &iedModel_MUnn_TCTR1,
    (ModelNode*) &iedModel_MUnn_LLN0_Mod,
};

DataObject iedModel_MUnn_LLN0_Mod = {
    DataObjectModelType,
    "Mod",
    (ModelNode*) &iedModel_MUnn_LLN0,
    NULL,
    (ModelNode*) &iedModel_MUnn_LLN0_Mod_stVal,
    0
};

DataAttribute iedModel_MUnn_LLN0_Mod_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_MUnn_LLN0_Mod,
    (ModelNode*) &iedModel_MUnn_LLN0_Mod_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MUnn_LLN0_Mod_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_MUnn_LLN0_Mod,
    (ModelNode*) &iedModel_MUnn_LLN0_Mod_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MUnn_LLN0_Mod_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_MUnn_LLN0_Mod,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

LogicalNode iedModel_MUnn_TCTR1 = {
    LogicalNodeModelType,
    "TCTR1",
    (ModelNode*) &iedModel_MUnn,
    (ModelNode*) &iedModel_MUnn_TCTR2,
    (ModelNode*) &iedModel_MUnn_TCTR1_Amp,
};

DataObject iedModel_MUnn_TCTR1_Amp = {
    DataObjectModelType,
    "Amp",
    (ModelNode*) &iedModel_MUnn_TCTR1,
    NULL,
    (ModelNode*) &iedModel_MUnn_TCTR1_Amp_instMag,
    0
};

DataAttribute iedModel_MUnn_TCTR1_Amp_instMag = {
    DataAttributeModelType,
    "instMag",
    (ModelNode*) &iedModel_MUnn_TCTR1_Amp,
    (ModelNode*) &iedModel_MUnn_TCTR1_Amp_q,
    (ModelNode*) &iedModel_MUnn_TCTR1_Amp_instMag_i,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MUnn_TCTR1_Amp_instMag_i = {
    DataAttributeModelType,
    "i",
    (ModelNode*) &iedModel_MUnn_TCTR1_Amp_instMag,
    NULL,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_INT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MUnn_TCTR1_Amp_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_MUnn_TCTR1_Amp,
    (ModelNode*) &iedModel_MUnn_TCTR1_Amp_sVC,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_MUnn_TCTR1_Amp_sVC = {
    DataAttributeModelType,
    "sVC",
    (ModelNode*) &iedModel_MUnn_TCTR1_Amp,
    NULL,
    (ModelNode*) &iedModel_MUnn_TCTR1_Amp_sVC_scaleFactor,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MUnn_TCTR1_Amp_sVC_scaleFactor = {
    DataAttributeModelType,
    "scaleFactor",
    (ModelNode*) &iedModel_MUnn_TCTR1_Amp_sVC,
    (ModelNode*) &iedModel_MUnn_TCTR1_Amp_sVC_offset,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MUnn_TCTR1_Amp_sVC_offset = {
    DataAttributeModelType,
    "offset",
    (ModelNode*) &iedModel_MUnn_TCTR1_Amp_sVC,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

LogicalNode iedModel_MUnn_TCTR2 = {
    LogicalNodeModelType,
    "TCTR2",
    (ModelNode*) &iedModel_MUnn,
    (ModelNode*) &iedModel_MUnn_TCTR3,
    (ModelNode*) &iedModel_MUnn_TCTR2_Amp,
};

DataObject iedModel_MUnn_TCTR2_Amp = {
    DataObjectModelType,
    "Amp",
    (ModelNode*) &iedModel_MUnn_TCTR2,
    NULL,
    (ModelNode*) &iedModel_MUnn_TCTR2_Amp_instMag,
    0
};

DataAttribute iedModel_MUnn_TCTR2_Amp_instMag = {
    DataAttributeModelType,
    "instMag",
    (ModelNode*) &iedModel_MUnn_TCTR2_Amp,
    (ModelNode*) &iedModel_MUnn_TCTR2_Amp_q,
    (ModelNode*) &iedModel_MUnn_TCTR2_Amp_instMag_i,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MUnn_TCTR2_Amp_instMag_i = {
    DataAttributeModelType,
    "i",
    (ModelNode*) &iedModel_MUnn_TCTR2_Amp_instMag,
    NULL,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_INT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MUnn_TCTR2_Amp_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_MUnn_TCTR2_Amp,
    (ModelNode*) &iedModel_MUnn_TCTR2_Amp_sVC,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_MUnn_TCTR2_Amp_sVC = {
    DataAttributeModelType,
    "sVC",
    (ModelNode*) &iedModel_MUnn_TCTR2_Amp,
    NULL,
    (ModelNode*) &iedModel_MUnn_TCTR2_Amp_sVC_scaleFactor,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MUnn_TCTR2_Amp_sVC_scaleFactor = {
    DataAttributeModelType,
    "scaleFactor",
    (ModelNode*) &iedModel_MUnn_TCTR2_Amp_sVC,
    (ModelNode*) &iedModel_MUnn_TCTR2_Amp_sVC_offset,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MUnn_TCTR2_Amp_sVC_offset = {
    DataAttributeModelType,
    "offset",
    (ModelNode*) &iedModel_MUnn_TCTR2_Amp_sVC,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

LogicalNode iedModel_MUnn_TCTR3 = {
    LogicalNodeModelType,
    "TCTR3",
    (ModelNode*) &iedModel_MUnn,
    (ModelNode*) &iedModel_MUnn_TCTR4,
    (ModelNode*) &iedModel_MUnn_TCTR3_Amp,
};

DataObject iedModel_MUnn_TCTR3_Amp = {
    DataObjectModelType,
    "Amp",
    (ModelNode*) &iedModel_MUnn_TCTR3,
    NULL,
    (ModelNode*) &iedModel_MUnn_TCTR3_Amp_instMag,
    0
};

DataAttribute iedModel_MUnn_TCTR3_Amp_instMag = {
    DataAttributeModelType,
    "instMag",
    (ModelNode*) &iedModel_MUnn_TCTR3_Amp,
    (ModelNode*) &iedModel_MUnn_TCTR3_Amp_q,
    (ModelNode*) &iedModel_MUnn_TCTR3_Amp_instMag_i,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MUnn_TCTR3_Amp_instMag_i = {
    DataAttributeModelType,
    "i",
    (ModelNode*) &iedModel_MUnn_TCTR3_Amp_instMag,
    NULL,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_INT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MUnn_TCTR3_Amp_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_MUnn_TCTR3_Amp,
    (ModelNode*) &iedModel_MUnn_TCTR3_Amp_sVC,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_MUnn_TCTR3_Amp_sVC = {
    DataAttributeModelType,
    "sVC",
    (ModelNode*) &iedModel_MUnn_TCTR3_Amp,
    NULL,
    (ModelNode*) &iedModel_MUnn_TCTR3_Amp_sVC_scaleFactor,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MUnn_TCTR3_Amp_sVC_scaleFactor = {
    DataAttributeModelType,
    "scaleFactor",
    (ModelNode*) &iedModel_MUnn_TCTR3_Amp_sVC,
    (ModelNode*) &iedModel_MUnn_TCTR3_Amp_sVC_offset,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MUnn_TCTR3_Amp_sVC_offset = {
    DataAttributeModelType,
    "offset",
    (ModelNode*) &iedModel_MUnn_TCTR3_Amp_sVC,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

LogicalNode iedModel_MUnn_TCTR4 = {
    LogicalNodeModelType,
    "TCTR4",
    (ModelNode*) &iedModel_MUnn,
    (ModelNode*) &iedModel_MUnn_TVTR1,
    (ModelNode*) &iedModel_MUnn_TCTR4_Amp,
};

DataObject iedModel_MUnn_TCTR4_Amp = {
    DataObjectModelType,
    "Amp",
    (ModelNode*) &iedModel_MUnn_TCTR4,
    NULL,
    (ModelNode*) &iedModel_MUnn_TCTR4_Amp_instMag,
    0
};

DataAttribute iedModel_MUnn_TCTR4_Amp_instMag = {
    DataAttributeModelType,
    "instMag",
    (ModelNode*) &iedModel_MUnn_TCTR4_Amp,
    (ModelNode*) &iedModel_MUnn_TCTR4_Amp_q,
    (ModelNode*) &iedModel_MUnn_TCTR4_Amp_instMag_i,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MUnn_TCTR4_Amp_instMag_i = {
    DataAttributeModelType,
    "i",
    (ModelNode*) &iedModel_MUnn_TCTR4_Amp_instMag,
    NULL,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_INT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MUnn_TCTR4_Amp_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_MUnn_TCTR4_Amp,
    (ModelNode*) &iedModel_MUnn_TCTR4_Amp_sVC,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_MUnn_TCTR4_Amp_sVC = {
    DataAttributeModelType,
    "sVC",
    (ModelNode*) &iedModel_MUnn_TCTR4_Amp,
    NULL,
    (ModelNode*) &iedModel_MUnn_TCTR4_Amp_sVC_scaleFactor,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MUnn_TCTR4_Amp_sVC_scaleFactor = {
    DataAttributeModelType,
    "scaleFactor",
    (ModelNode*) &iedModel_MUnn_TCTR4_Amp_sVC,
    (ModelNode*) &iedModel_MUnn_TCTR4_Amp_sVC_offset,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MUnn_TCTR4_Amp_sVC_offset = {
    DataAttributeModelType,
    "offset",
    (ModelNode*) &iedModel_MUnn_TCTR4_Amp_sVC,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

LogicalNode iedModel_MUnn_TVTR1 = {
    LogicalNodeModelType,
    "TVTR1",
    (ModelNode*) &iedModel_MUnn,
    (ModelNode*) &iedModel_MUnn_TVTR2,
    (ModelNode*) &iedModel_MUnn_TVTR1_Vol,
};

DataObject iedModel_MUnn_TVTR1_Vol = {
    DataObjectModelType,
    "Vol",
    (ModelNode*) &iedModel_MUnn_TVTR1,
    NULL,
    (ModelNode*) &iedModel_MUnn_TVTR1_Vol_instMag,
    0
};

DataAttribute iedModel_MUnn_TVTR1_Vol_instMag = {
    DataAttributeModelType,
    "instMag",
    (ModelNode*) &iedModel_MUnn_TVTR1_Vol,
    (ModelNode*) &iedModel_MUnn_TVTR1_Vol_q,
    (ModelNode*) &iedModel_MUnn_TVTR1_Vol_instMag_i,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MUnn_TVTR1_Vol_instMag_i = {
    DataAttributeModelType,
    "i",
    (ModelNode*) &iedModel_MUnn_TVTR1_Vol_instMag,
    NULL,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_INT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MUnn_TVTR1_Vol_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_MUnn_TVTR1_Vol,
    (ModelNode*) &iedModel_MUnn_TVTR1_Vol_sVC,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_MUnn_TVTR1_Vol_sVC = {
    DataAttributeModelType,
    "sVC",
    (ModelNode*) &iedModel_MUnn_TVTR1_Vol,
    NULL,
    (ModelNode*) &iedModel_MUnn_TVTR1_Vol_sVC_scaleFactor,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MUnn_TVTR1_Vol_sVC_scaleFactor = {
    DataAttributeModelType,
    "scaleFactor",
    (ModelNode*) &iedModel_MUnn_TVTR1_Vol_sVC,
    (ModelNode*) &iedModel_MUnn_TVTR1_Vol_sVC_offset,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MUnn_TVTR1_Vol_sVC_offset = {
    DataAttributeModelType,
    "offset",
    (ModelNode*) &iedModel_MUnn_TVTR1_Vol_sVC,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

LogicalNode iedModel_MUnn_TVTR2 = {
    LogicalNodeModelType,
    "TVTR2",
    (ModelNode*) &iedModel_MUnn,
    (ModelNode*) &iedModel_MUnn_TVTR3,
    (ModelNode*) &iedModel_MUnn_TVTR2_Vol,
};

DataObject iedModel_MUnn_TVTR2_Vol = {
    DataObjectModelType,
    "Vol",
    (ModelNode*) &iedModel_MUnn_TVTR2,
    NULL,
    (ModelNode*) &iedModel_MUnn_TVTR2_Vol_instMag,
    0
};

DataAttribute iedModel_MUnn_TVTR2_Vol_instMag = {
    DataAttributeModelType,
    "instMag",
    (ModelNode*) &iedModel_MUnn_TVTR2_Vol,
    (ModelNode*) &iedModel_MUnn_TVTR2_Vol_q,
    (ModelNode*) &iedModel_MUnn_TVTR2_Vol_instMag_i,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MUnn_TVTR2_Vol_instMag_i = {
    DataAttributeModelType,
    "i",
    (ModelNode*) &iedModel_MUnn_TVTR2_Vol_instMag,
    NULL,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_INT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MUnn_TVTR2_Vol_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_MUnn_TVTR2_Vol,
    (ModelNode*) &iedModel_MUnn_TVTR2_Vol_sVC,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_MUnn_TVTR2_Vol_sVC = {
    DataAttributeModelType,
    "sVC",
    (ModelNode*) &iedModel_MUnn_TVTR2_Vol,
    NULL,
    (ModelNode*) &iedModel_MUnn_TVTR2_Vol_sVC_scaleFactor,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MUnn_TVTR2_Vol_sVC_scaleFactor = {
    DataAttributeModelType,
    "scaleFactor",
    (ModelNode*) &iedModel_MUnn_TVTR2_Vol_sVC,
    (ModelNode*) &iedModel_MUnn_TVTR2_Vol_sVC_offset,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MUnn_TVTR2_Vol_sVC_offset = {
    DataAttributeModelType,
    "offset",
    (ModelNode*) &iedModel_MUnn_TVTR2_Vol_sVC,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

LogicalNode iedModel_MUnn_TVTR3 = {
    LogicalNodeModelType,
    "TVTR3",
    (ModelNode*) &iedModel_MUnn,
    (ModelNode*) &iedModel_MUnn_TVTR4,
    (ModelNode*) &iedModel_MUnn_TVTR3_Vol,
};

DataObject iedModel_MUnn_TVTR3_Vol = {
    DataObjectModelType,
    "Vol",
    (ModelNode*) &iedModel_MUnn_TVTR3,
    NULL,
    (ModelNode*) &iedModel_MUnn_TVTR3_Vol_instMag,
    0
};

DataAttribute iedModel_MUnn_TVTR3_Vol_instMag = {
    DataAttributeModelType,
    "instMag",
    (ModelNode*) &iedModel_MUnn_TVTR3_Vol,
    (ModelNode*) &iedModel_MUnn_TVTR3_Vol_q,
    (ModelNode*) &iedModel_MUnn_TVTR3_Vol_instMag_i,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MUnn_TVTR3_Vol_instMag_i = {
    DataAttributeModelType,
    "i",
    (ModelNode*) &iedModel_MUnn_TVTR3_Vol_instMag,
    NULL,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_INT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MUnn_TVTR3_Vol_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_MUnn_TVTR3_Vol,
    (ModelNode*) &iedModel_MUnn_TVTR3_Vol_sVC,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_MUnn_TVTR3_Vol_sVC = {
    DataAttributeModelType,
    "sVC",
    (ModelNode*) &iedModel_MUnn_TVTR3_Vol,
    NULL,
    (ModelNode*) &iedModel_MUnn_TVTR3_Vol_sVC_scaleFactor,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MUnn_TVTR3_Vol_sVC_scaleFactor = {
    DataAttributeModelType,
    "scaleFactor",
    (ModelNode*) &iedModel_MUnn_TVTR3_Vol_sVC,
    (ModelNode*) &iedModel_MUnn_TVTR3_Vol_sVC_offset,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MUnn_TVTR3_Vol_sVC_offset = {
    DataAttributeModelType,
    "offset",
    (ModelNode*) &iedModel_MUnn_TVTR3_Vol_sVC,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

LogicalNode iedModel_MUnn_TVTR4 = {
    LogicalNodeModelType,
    "TVTR4",
    (ModelNode*) &iedModel_MUnn,
    NULL,
    (ModelNode*) &iedModel_MUnn_TVTR4_Vol,
};

DataObject iedModel_MUnn_TVTR4_Vol = {
    DataObjectModelType,
    "Vol",
    (ModelNode*) &iedModel_MUnn_TVTR4,
    NULL,
    (ModelNode*) &iedModel_MUnn_TVTR4_Vol_instMag,
    0
};

DataAttribute iedModel_MUnn_TVTR4_Vol_instMag = {
    DataAttributeModelType,
    "instMag",
    (ModelNode*) &iedModel_MUnn_TVTR4_Vol,
    (ModelNode*) &iedModel_MUnn_TVTR4_Vol_q,
    (ModelNode*) &iedModel_MUnn_TVTR4_Vol_instMag_i,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MUnn_TVTR4_Vol_instMag_i = {
    DataAttributeModelType,
    "i",
    (ModelNode*) &iedModel_MUnn_TVTR4_Vol_instMag,
    NULL,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_INT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MUnn_TVTR4_Vol_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_MUnn_TVTR4_Vol,
    (ModelNode*) &iedModel_MUnn_TVTR4_Vol_sVC,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_MUnn_TVTR4_Vol_sVC = {
    DataAttributeModelType,
    "sVC",
    (ModelNode*) &iedModel_MUnn_TVTR4_Vol,
    NULL,
    (ModelNode*) &iedModel_MUnn_TVTR4_Vol_sVC_scaleFactor,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MUnn_TVTR4_Vol_sVC_scaleFactor = {
    DataAttributeModelType,
    "scaleFactor",
    (ModelNode*) &iedModel_MUnn_TVTR4_Vol_sVC,
    (ModelNode*) &iedModel_MUnn_TVTR4_Vol_sVC_offset,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MUnn_TVTR4_Vol_sVC_offset = {
    DataAttributeModelType,
    "offset",
    (ModelNode*) &iedModel_MUnn_TVTR4_Vol_sVC,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};



extern SVControlBlock iedModel_MUnn_LLN0_smv0;

static PhyComAddress iedModel_MUnn_LLN0_smv0_address = {
  4,
  1,
  4097,
  {0x1, 0xc, 0xcd, 0x4, 0x0, 0x1}
};

SVControlBlock iedModel_MUnn_LLN0_smv0 = {&iedModel_MUnn_LLN0, "MSVCB01", "xxxxMUnn01", "PhsMeas1", 2, 0, 80, 1, &iedModel_MUnn_LLN0_smv0_address, true, 1, NULL};






IedModel iedModel = {
    "TEMPLATE",
    &iedModel_MUnn,
    &iedModelds_MUnn_LLN0_PhsMeas1,
    NULL,
    NULL,
    &iedModel_MUnn_LLN0_smv0,
    NULL,
    NULL,
    NULL,
    initializeValues
};

static void
initializeValues()
{

iedModel_MUnn_TCTR1_Amp_sVC_scaleFactor.mmsValue = MmsValue_newFloat(0.001);

iedModel_MUnn_TCTR1_Amp_sVC_offset.mmsValue = MmsValue_newFloat(0.0);

iedModel_MUnn_TCTR2_Amp_sVC_scaleFactor.mmsValue = MmsValue_newFloat(0.001);

iedModel_MUnn_TCTR2_Amp_sVC_offset.mmsValue = MmsValue_newFloat(0.0);

iedModel_MUnn_TCTR3_Amp_sVC_scaleFactor.mmsValue = MmsValue_newFloat(0.001);

iedModel_MUnn_TCTR3_Amp_sVC_offset.mmsValue = MmsValue_newFloat(0.0);

iedModel_MUnn_TCTR4_Amp_sVC_scaleFactor.mmsValue = MmsValue_newFloat(0.001);

iedModel_MUnn_TCTR4_Amp_sVC_offset.mmsValue = MmsValue_newFloat(0.0);

iedModel_MUnn_TVTR1_Vol_sVC_scaleFactor.mmsValue = MmsValue_newFloat(0.01);

iedModel_MUnn_TVTR1_Vol_sVC_offset.mmsValue = MmsValue_newFloat(0.0);

iedModel_MUnn_TVTR2_Vol_sVC_scaleFactor.mmsValue = MmsValue_newFloat(0.01);

iedModel_MUnn_TVTR2_Vol_sVC_offset.mmsValue = MmsValue_newFloat(0.0);

iedModel_MUnn_TVTR3_Vol_sVC_scaleFactor.mmsValue = MmsValue_newFloat(0.01);

iedModel_MUnn_TVTR3_Vol_sVC_offset.mmsValue = MmsValue_newFloat(0.0);

iedModel_MUnn_TVTR4_Vol_sVC_scaleFactor.mmsValue = MmsValue_newFloat(0.01);

iedModel_MUnn_TVTR4_Vol_sVC_offset.mmsValue = MmsValue_newFloat(0.0);
}
