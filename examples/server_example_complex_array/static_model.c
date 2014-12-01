/*
 * static_model.c
 *
 * automatically generated from mhai_array.icd
 */
#include <stdlib.h>
#include "iec61850_model.h"

extern IedModel iedModel;
static void initializeValues();
extern LogicalDevice iedModel_ComplexArray;
extern LogicalNode   iedModel_ComplexArray_LLN0;
extern DataObject    iedModel_ComplexArray_LLN0_Mod;
extern DataAttribute iedModel_ComplexArray_LLN0_Mod_q;
extern DataAttribute iedModel_ComplexArray_LLN0_Mod_t;
extern DataAttribute iedModel_ComplexArray_LLN0_Mod_ctlModel;
extern DataObject    iedModel_ComplexArray_LLN0_Beh;
extern DataAttribute iedModel_ComplexArray_LLN0_Beh_stVal;
extern DataAttribute iedModel_ComplexArray_LLN0_Beh_q;
extern DataAttribute iedModel_ComplexArray_LLN0_Beh_t;
extern DataObject    iedModel_ComplexArray_LLN0_Health;
extern DataAttribute iedModel_ComplexArray_LLN0_Health_stVal;
extern DataAttribute iedModel_ComplexArray_LLN0_Health_q;
extern DataAttribute iedModel_ComplexArray_LLN0_Health_t;
extern DataObject    iedModel_ComplexArray_LLN0_NamPlt;
extern DataAttribute iedModel_ComplexArray_LLN0_NamPlt_vendor;
extern DataAttribute iedModel_ComplexArray_LLN0_NamPlt_swRev;
extern DataAttribute iedModel_ComplexArray_LLN0_NamPlt_d;
extern DataAttribute iedModel_ComplexArray_LLN0_NamPlt_configRev;
extern DataAttribute iedModel_ComplexArray_LLN0_NamPlt_ldNs;
extern LogicalNode   iedModel_ComplexArray_LPHD1;
extern DataObject    iedModel_ComplexArray_LPHD1_PhyNam;
extern DataAttribute iedModel_ComplexArray_LPHD1_PhyNam_vendor;
extern DataObject    iedModel_ComplexArray_LPHD1_PhyHealth;
extern DataAttribute iedModel_ComplexArray_LPHD1_PhyHealth_stVal;
extern DataAttribute iedModel_ComplexArray_LPHD1_PhyHealth_q;
extern DataAttribute iedModel_ComplexArray_LPHD1_PhyHealth_t;
extern DataObject    iedModel_ComplexArray_LPHD1_Proxy;
extern DataAttribute iedModel_ComplexArray_LPHD1_Proxy_stVal;
extern DataAttribute iedModel_ComplexArray_LPHD1_Proxy_q;
extern DataAttribute iedModel_ComplexArray_LPHD1_Proxy_t;
extern LogicalNode   iedModel_ComplexArray_MHAI1;
extern DataObject    iedModel_ComplexArray_MHAI1_HA;
extern DataObject    iedModel_ComplexArray_MHAI1_HA_phsAHar;
extern DataAttribute iedModel_ComplexArray_MHAI1_HA_phsAHar_cVal;
extern DataAttribute iedModel_ComplexArray_MHAI1_HA_phsAHar_cVal_mag;
extern DataAttribute iedModel_ComplexArray_MHAI1_HA_phsAHar_cVal_mag_f;
extern DataAttribute iedModel_ComplexArray_MHAI1_HA_phsAHar_cVal_ang;
extern DataAttribute iedModel_ComplexArray_MHAI1_HA_phsAHar_cVal_ang_f;
extern DataAttribute iedModel_ComplexArray_MHAI1_HA_phsAHar_q;
extern DataAttribute iedModel_ComplexArray_MHAI1_HA_phsAHar_t;
extern DataAttribute iedModel_ComplexArray_MHAI1_HA_numHar;
extern DataAttribute iedModel_ComplexArray_MHAI1_HA_numCyc;
extern DataAttribute iedModel_ComplexArray_MHAI1_HA_evalTm;
extern DataAttribute iedModel_ComplexArray_MHAI1_HA_frequency;



LogicalDevice iedModel_ComplexArray = {
    LogicalDeviceModelType,
    "ComplexArray",
    (ModelNode*) &iedModel,
    NULL,
    (ModelNode*) &iedModel_ComplexArray_LLN0
};

LogicalNode iedModel_ComplexArray_LLN0 = {
    LogicalNodeModelType,
    "LLN0",
    (ModelNode*) &iedModel_ComplexArray,
    (ModelNode*) &iedModel_ComplexArray_LPHD1,
    (ModelNode*) &iedModel_ComplexArray_LLN0_Mod,
};

DataObject iedModel_ComplexArray_LLN0_Mod = {
    DataObjectModelType,
    "Mod",
    (ModelNode*) &iedModel_ComplexArray_LLN0,
    (ModelNode*) &iedModel_ComplexArray_LLN0_Beh,
    (ModelNode*) &iedModel_ComplexArray_LLN0_Mod_q,
    0
};

DataAttribute iedModel_ComplexArray_LLN0_Mod_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_ComplexArray_LLN0_Mod,
    (ModelNode*) &iedModel_ComplexArray_LLN0_Mod_t,
    NULL,
    0,
    ST,
    QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_ComplexArray_LLN0_Mod_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_ComplexArray_LLN0_Mod,
    (ModelNode*) &iedModel_ComplexArray_LLN0_Mod_ctlModel,
    NULL,
    0,
    ST,
    TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_ComplexArray_LLN0_Mod_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_ComplexArray_LLN0_Mod,
    NULL,
    NULL,
    0,
    CF,
    ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_ComplexArray_LLN0_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_ComplexArray_LLN0,
    (ModelNode*) &iedModel_ComplexArray_LLN0_Health,
    (ModelNode*) &iedModel_ComplexArray_LLN0_Beh_stVal,
    0
};

DataAttribute iedModel_ComplexArray_LLN0_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_ComplexArray_LLN0_Beh,
    (ModelNode*) &iedModel_ComplexArray_LLN0_Beh_q,
    NULL,
    0,
    ST,
    INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_ComplexArray_LLN0_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_ComplexArray_LLN0_Beh,
    (ModelNode*) &iedModel_ComplexArray_LLN0_Beh_t,
    NULL,
    0,
    ST,
    QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_ComplexArray_LLN0_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_ComplexArray_LLN0_Beh,
    NULL,
    NULL,
    0,
    ST,
    TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_ComplexArray_LLN0_Health = {
    DataObjectModelType,
    "Health",
    (ModelNode*) &iedModel_ComplexArray_LLN0,
    (ModelNode*) &iedModel_ComplexArray_LLN0_NamPlt,
    (ModelNode*) &iedModel_ComplexArray_LLN0_Health_stVal,
    0
};

DataAttribute iedModel_ComplexArray_LLN0_Health_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_ComplexArray_LLN0_Health,
    (ModelNode*) &iedModel_ComplexArray_LLN0_Health_q,
    NULL,
    0,
    ST,
    INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_ComplexArray_LLN0_Health_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_ComplexArray_LLN0_Health,
    (ModelNode*) &iedModel_ComplexArray_LLN0_Health_t,
    NULL,
    0,
    ST,
    QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_ComplexArray_LLN0_Health_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_ComplexArray_LLN0_Health,
    NULL,
    NULL,
    0,
    ST,
    TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_ComplexArray_LLN0_NamPlt = {
    DataObjectModelType,
    "NamPlt",
    (ModelNode*) &iedModel_ComplexArray_LLN0,
    NULL,
    (ModelNode*) &iedModel_ComplexArray_LLN0_NamPlt_vendor,
    0
};

DataAttribute iedModel_ComplexArray_LLN0_NamPlt_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_ComplexArray_LLN0_NamPlt,
    (ModelNode*) &iedModel_ComplexArray_LLN0_NamPlt_swRev,
    NULL,
    0,
    DC,
    VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_ComplexArray_LLN0_NamPlt_swRev = {
    DataAttributeModelType,
    "swRev",
    (ModelNode*) &iedModel_ComplexArray_LLN0_NamPlt,
    (ModelNode*) &iedModel_ComplexArray_LLN0_NamPlt_d,
    NULL,
    0,
    DC,
    VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_ComplexArray_LLN0_NamPlt_d = {
    DataAttributeModelType,
    "d",
    (ModelNode*) &iedModel_ComplexArray_LLN0_NamPlt,
    (ModelNode*) &iedModel_ComplexArray_LLN0_NamPlt_configRev,
    NULL,
    0,
    DC,
    VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_ComplexArray_LLN0_NamPlt_configRev = {
    DataAttributeModelType,
    "configRev",
    (ModelNode*) &iedModel_ComplexArray_LLN0_NamPlt,
    (ModelNode*) &iedModel_ComplexArray_LLN0_NamPlt_ldNs,
    NULL,
    0,
    DC,
    VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_ComplexArray_LLN0_NamPlt_ldNs = {
    DataAttributeModelType,
    "ldNs",
    (ModelNode*) &iedModel_ComplexArray_LLN0_NamPlt,
    NULL,
    NULL,
    0,
    EX,
    VISIBLE_STRING_255,
    0,
    NULL,
    0};

LogicalNode iedModel_ComplexArray_LPHD1 = {
    LogicalNodeModelType,
    "LPHD1",
    (ModelNode*) &iedModel_ComplexArray,
    (ModelNode*) &iedModel_ComplexArray_MHAI1,
    (ModelNode*) &iedModel_ComplexArray_LPHD1_PhyNam,
};

DataObject iedModel_ComplexArray_LPHD1_PhyNam = {
    DataObjectModelType,
    "PhyNam",
    (ModelNode*) &iedModel_ComplexArray_LPHD1,
    (ModelNode*) &iedModel_ComplexArray_LPHD1_PhyHealth,
    (ModelNode*) &iedModel_ComplexArray_LPHD1_PhyNam_vendor,
    0
};

DataAttribute iedModel_ComplexArray_LPHD1_PhyNam_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_ComplexArray_LPHD1_PhyNam,
    NULL,
    NULL,
    0,
    DC,
    VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_ComplexArray_LPHD1_PhyHealth = {
    DataObjectModelType,
    "PhyHealth",
    (ModelNode*) &iedModel_ComplexArray_LPHD1,
    (ModelNode*) &iedModel_ComplexArray_LPHD1_Proxy,
    (ModelNode*) &iedModel_ComplexArray_LPHD1_PhyHealth_stVal,
    0
};

DataAttribute iedModel_ComplexArray_LPHD1_PhyHealth_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_ComplexArray_LPHD1_PhyHealth,
    (ModelNode*) &iedModel_ComplexArray_LPHD1_PhyHealth_q,
    NULL,
    0,
    ST,
    INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_ComplexArray_LPHD1_PhyHealth_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_ComplexArray_LPHD1_PhyHealth,
    (ModelNode*) &iedModel_ComplexArray_LPHD1_PhyHealth_t,
    NULL,
    0,
    ST,
    QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_ComplexArray_LPHD1_PhyHealth_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_ComplexArray_LPHD1_PhyHealth,
    NULL,
    NULL,
    0,
    ST,
    TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_ComplexArray_LPHD1_Proxy = {
    DataObjectModelType,
    "Proxy",
    (ModelNode*) &iedModel_ComplexArray_LPHD1,
    NULL,
    (ModelNode*) &iedModel_ComplexArray_LPHD1_Proxy_stVal,
    0
};

DataAttribute iedModel_ComplexArray_LPHD1_Proxy_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_ComplexArray_LPHD1_Proxy,
    (ModelNode*) &iedModel_ComplexArray_LPHD1_Proxy_q,
    NULL,
    0,
    ST,
    BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_ComplexArray_LPHD1_Proxy_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_ComplexArray_LPHD1_Proxy,
    (ModelNode*) &iedModel_ComplexArray_LPHD1_Proxy_t,
    NULL,
    0,
    ST,
    QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_ComplexArray_LPHD1_Proxy_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_ComplexArray_LPHD1_Proxy,
    NULL,
    NULL,
    0,
    ST,
    TIMESTAMP,
    0,
    NULL,
    0};

LogicalNode iedModel_ComplexArray_MHAI1 = {
    LogicalNodeModelType,
    "MHAI1",
    (ModelNode*) &iedModel_ComplexArray,
    NULL,
    (ModelNode*) &iedModel_ComplexArray_MHAI1_HA,
};

DataObject iedModel_ComplexArray_MHAI1_HA = {
    DataObjectModelType,
    "HA",
    (ModelNode*) &iedModel_ComplexArray_MHAI1,
    NULL,
    (ModelNode*) &iedModel_ComplexArray_MHAI1_HA_phsAHar,
    0
};

DataObject iedModel_ComplexArray_MHAI1_HA_phsAHar = {
    DataObjectModelType,
    "phsAHar",
    (ModelNode*) &iedModel_ComplexArray_MHAI1_HA,
    (ModelNode*) &iedModel_ComplexArray_MHAI1_HA_numHar,
    (ModelNode*) &iedModel_ComplexArray_MHAI1_HA_phsAHar_cVal,
    16
};

DataAttribute iedModel_ComplexArray_MHAI1_HA_phsAHar_cVal = {
    DataAttributeModelType,
    "cVal",
    (ModelNode*) &iedModel_ComplexArray_MHAI1_HA_phsAHar,
    (ModelNode*) &iedModel_ComplexArray_MHAI1_HA_phsAHar_q,
    (ModelNode*) &iedModel_ComplexArray_MHAI1_HA_phsAHar_cVal_mag,
    0,
    MX,
    CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED + TRG_OPT_DATA_UPDATE,
    NULL,
    0};

DataAttribute iedModel_ComplexArray_MHAI1_HA_phsAHar_cVal_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_ComplexArray_MHAI1_HA_phsAHar_cVal,
    (ModelNode*) &iedModel_ComplexArray_MHAI1_HA_phsAHar_cVal_ang,
    (ModelNode*) &iedModel_ComplexArray_MHAI1_HA_phsAHar_cVal_mag_f,
    0,
    MX,
    CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED + TRG_OPT_DATA_UPDATE,
    NULL,
    0};

DataAttribute iedModel_ComplexArray_MHAI1_HA_phsAHar_cVal_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_ComplexArray_MHAI1_HA_phsAHar_cVal_mag,
    NULL,
    NULL,
    0,
    MX,
    FLOAT32,
    0 + TRG_OPT_DATA_CHANGED + TRG_OPT_DATA_UPDATE,
    NULL,
    0};

DataAttribute iedModel_ComplexArray_MHAI1_HA_phsAHar_cVal_ang = {
    DataAttributeModelType,
    "ang",
    (ModelNode*) &iedModel_ComplexArray_MHAI1_HA_phsAHar_cVal,
    NULL,
    (ModelNode*) &iedModel_ComplexArray_MHAI1_HA_phsAHar_cVal_ang_f,
    0,
    MX,
    CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED + TRG_OPT_DATA_UPDATE,
    NULL,
    0};

DataAttribute iedModel_ComplexArray_MHAI1_HA_phsAHar_cVal_ang_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_ComplexArray_MHAI1_HA_phsAHar_cVal_ang,
    NULL,
    NULL,
    0,
    MX,
    FLOAT32,
    0 + TRG_OPT_DATA_CHANGED + TRG_OPT_DATA_UPDATE,
    NULL,
    0};

DataAttribute iedModel_ComplexArray_MHAI1_HA_phsAHar_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_ComplexArray_MHAI1_HA_phsAHar,
    (ModelNode*) &iedModel_ComplexArray_MHAI1_HA_phsAHar_t,
    NULL,
    0,
    MX,
    QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_ComplexArray_MHAI1_HA_phsAHar_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_ComplexArray_MHAI1_HA_phsAHar,
    NULL,
    NULL,
    0,
    MX,
    TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_ComplexArray_MHAI1_HA_numHar = {
    DataAttributeModelType,
    "numHar",
    (ModelNode*) &iedModel_ComplexArray_MHAI1_HA,
    (ModelNode*) &iedModel_ComplexArray_MHAI1_HA_numCyc,
    NULL,
    0,
    CF,
    INT16U,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_ComplexArray_MHAI1_HA_numCyc = {
    DataAttributeModelType,
    "numCyc",
    (ModelNode*) &iedModel_ComplexArray_MHAI1_HA,
    (ModelNode*) &iedModel_ComplexArray_MHAI1_HA_evalTm,
    NULL,
    0,
    CF,
    INT16U,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_ComplexArray_MHAI1_HA_evalTm = {
    DataAttributeModelType,
    "evalTm",
    (ModelNode*) &iedModel_ComplexArray_MHAI1_HA,
    (ModelNode*) &iedModel_ComplexArray_MHAI1_HA_frequency,
    NULL,
    0,
    CF,
    INT16U,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_ComplexArray_MHAI1_HA_frequency = {
    DataAttributeModelType,
    "frequency",
    (ModelNode*) &iedModel_ComplexArray_MHAI1_HA,
    NULL,
    NULL,
    0,
    CF,
    FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};






IedModel iedModel = {
    "test",
    &iedModel_ComplexArray,
    NULL,
    NULL,
    NULL,
    NULL,
    initializeValues
};

static void
initializeValues()
{

iedModel_ComplexArray_LLN0_Mod_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(0);

iedModel_ComplexArray_MHAI1_HA_numHar.mmsValue = MmsValue_newUnsignedFromUint32(16);
}
