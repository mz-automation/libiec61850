/*
 * static_model.c
 *
 * automatically generated from simpleIO_control_tests.icd
 */
#include <stdlib.h>
#include "iec61850_model.h"

extern IedModel iedModel;
static void initializeValues();
extern LogicalDevice iedModel_GenericIO;
extern LogicalNode   iedModel_GenericIO_LLN0;
extern DataObject    iedModel_GenericIO_LLN0_Mod;
extern DataAttribute iedModel_GenericIO_LLN0_Mod_q;
extern DataAttribute iedModel_GenericIO_LLN0_Mod_t;
extern DataAttribute iedModel_GenericIO_LLN0_Mod_ctlModel;
extern DataObject    iedModel_GenericIO_LLN0_Beh;
extern DataAttribute iedModel_GenericIO_LLN0_Beh_stVal;
extern DataAttribute iedModel_GenericIO_LLN0_Beh_q;
extern DataAttribute iedModel_GenericIO_LLN0_Beh_t;
extern DataObject    iedModel_GenericIO_LLN0_Health;
extern DataAttribute iedModel_GenericIO_LLN0_Health_stVal;
extern DataAttribute iedModel_GenericIO_LLN0_Health_q;
extern DataAttribute iedModel_GenericIO_LLN0_Health_t;
extern DataObject    iedModel_GenericIO_LLN0_NamPlt;
extern DataAttribute iedModel_GenericIO_LLN0_NamPlt_vendor;
extern DataAttribute iedModel_GenericIO_LLN0_NamPlt_swRev;
extern DataAttribute iedModel_GenericIO_LLN0_NamPlt_d;
extern DataAttribute iedModel_GenericIO_LLN0_NamPlt_configRev;
extern DataAttribute iedModel_GenericIO_LLN0_NamPlt_ldNs;
extern LogicalNode   iedModel_GenericIO_LPHD1;
extern DataObject    iedModel_GenericIO_LPHD1_PhyNam;
extern DataAttribute iedModel_GenericIO_LPHD1_PhyNam_vendor;
extern DataObject    iedModel_GenericIO_LPHD1_PhyHealth;
extern DataAttribute iedModel_GenericIO_LPHD1_PhyHealth_stVal;
extern DataAttribute iedModel_GenericIO_LPHD1_PhyHealth_q;
extern DataAttribute iedModel_GenericIO_LPHD1_PhyHealth_t;
extern DataObject    iedModel_GenericIO_LPHD1_Proxy;
extern DataAttribute iedModel_GenericIO_LPHD1_Proxy_stVal;
extern DataAttribute iedModel_GenericIO_LPHD1_Proxy_q;
extern DataAttribute iedModel_GenericIO_LPHD1_Proxy_t;
extern LogicalNode   iedModel_GenericIO_GGIO1;
extern DataObject    iedModel_GenericIO_GGIO1_Mod;
extern DataAttribute iedModel_GenericIO_GGIO1_Mod_q;
extern DataAttribute iedModel_GenericIO_GGIO1_Mod_t;
extern DataAttribute iedModel_GenericIO_GGIO1_Mod_ctlModel;
extern DataObject    iedModel_GenericIO_GGIO1_Beh;
extern DataAttribute iedModel_GenericIO_GGIO1_Beh_stVal;
extern DataAttribute iedModel_GenericIO_GGIO1_Beh_q;
extern DataAttribute iedModel_GenericIO_GGIO1_Beh_t;
extern DataObject    iedModel_GenericIO_GGIO1_Health;
extern DataAttribute iedModel_GenericIO_GGIO1_Health_stVal;
extern DataAttribute iedModel_GenericIO_GGIO1_Health_q;
extern DataAttribute iedModel_GenericIO_GGIO1_Health_t;
extern DataObject    iedModel_GenericIO_GGIO1_NamPlt;
extern DataAttribute iedModel_GenericIO_GGIO1_NamPlt_vendor;
extern DataAttribute iedModel_GenericIO_GGIO1_NamPlt_swRev;
extern DataAttribute iedModel_GenericIO_GGIO1_NamPlt_d;
extern DataObject    iedModel_GenericIO_GGIO1_AnIn1;
extern DataAttribute iedModel_GenericIO_GGIO1_AnIn1_mag;
extern DataAttribute iedModel_GenericIO_GGIO1_AnIn1_mag_f;
extern DataAttribute iedModel_GenericIO_GGIO1_AnIn1_q;
extern DataAttribute iedModel_GenericIO_GGIO1_AnIn1_t;
extern DataObject    iedModel_GenericIO_GGIO1_AnIn2;
extern DataAttribute iedModel_GenericIO_GGIO1_AnIn2_mag;
extern DataAttribute iedModel_GenericIO_GGIO1_AnIn2_mag_f;
extern DataAttribute iedModel_GenericIO_GGIO1_AnIn2_q;
extern DataAttribute iedModel_GenericIO_GGIO1_AnIn2_t;
extern DataObject    iedModel_GenericIO_GGIO1_AnIn3;
extern DataAttribute iedModel_GenericIO_GGIO1_AnIn3_mag;
extern DataAttribute iedModel_GenericIO_GGIO1_AnIn3_mag_f;
extern DataAttribute iedModel_GenericIO_GGIO1_AnIn3_q;
extern DataAttribute iedModel_GenericIO_GGIO1_AnIn3_t;
extern DataObject    iedModel_GenericIO_GGIO1_AnIn4;
extern DataAttribute iedModel_GenericIO_GGIO1_AnIn4_mag;
extern DataAttribute iedModel_GenericIO_GGIO1_AnIn4_mag_f;
extern DataAttribute iedModel_GenericIO_GGIO1_AnIn4_q;
extern DataAttribute iedModel_GenericIO_GGIO1_AnIn4_t;
extern DataObject    iedModel_GenericIO_GGIO1_SPCSO1;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO1_Oper;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO1_Oper_ctlVal;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO1_Oper_origin;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO1_Oper_origin_orCat;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO1_Oper_origin_orIdent;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO1_Oper_ctlNum;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO1_Oper_T;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO1_Oper_Test;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO1_Oper_Check;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO1_stVal;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO1_q;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO1_t;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO1_ctlModel;
extern DataObject    iedModel_GenericIO_GGIO1_SPCSO2;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_SBO;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_Oper;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_Oper_ctlVal;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_Oper_origin;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_Oper_origin_orCat;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_Oper_origin_orIdent;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_Oper_ctlNum;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_Oper_T;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_Oper_Test;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_Oper_Check;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_Cancel;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_Cancel_ctlVal;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_Cancel_origin;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_Cancel_origin_orCat;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_Cancel_origin_orIdent;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_Cancel_ctlNum;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_Cancel_T;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_Cancel_Test;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_stVal;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_q;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_t;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_ctlModel;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_sboClass;
extern DataObject    iedModel_GenericIO_GGIO1_SPCSO3;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_Oper;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_Oper_ctlVal;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_Oper_origin;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_Oper_origin_orCat;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_Oper_origin_orIdent;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_Oper_ctlNum;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_Oper_T;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_Oper_Test;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_Oper_Check;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_Cancel;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_Cancel_ctlVal;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_Cancel_origin;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_Cancel_origin_orCat;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_Cancel_origin_orIdent;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_Cancel_ctlNum;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_Cancel_T;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_Cancel_Test;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_stVal;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_q;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_t;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_ctlModel;
extern DataObject    iedModel_GenericIO_GGIO1_SPCSO4;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_SBOw;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_SBOw_ctlVal;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_SBOw_origin;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_SBOw_origin_orCat;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_SBOw_origin_orIdent;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_SBOw_ctlNum;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_SBOw_T;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_SBOw_Test;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_SBOw_Check;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_Oper;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_Oper_ctlVal;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_Oper_origin;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_Oper_origin_orCat;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_Oper_origin_orIdent;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_Oper_ctlNum;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_Oper_T;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_Oper_Test;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_Oper_Check;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_Cancel;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_Cancel_ctlVal;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_Cancel_origin;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_Cancel_origin_orCat;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_Cancel_origin_orIdent;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_Cancel_ctlNum;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_Cancel_T;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_Cancel_Test;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_stVal;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_q;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_t;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_ctlModel;
extern DataObject    iedModel_GenericIO_GGIO1_SPCSO5;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO5_Oper;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO5_Oper_ctlVal;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO5_Oper_operTm;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO5_Oper_origin;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO5_Oper_origin_orCat;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO5_Oper_origin_orIdent;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO5_Oper_ctlNum;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO5_Oper_T;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO5_Oper_Test;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO5_Oper_Check;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO5_stVal;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO5_q;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO5_t;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO5_ctlModel;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO5_Cancel;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO5_Cancel_ctlVal;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO5_Cancel_origin;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO5_Cancel_origin_orCat;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO5_Cancel_origin_orIdent;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO5_Cancel_ctlNum;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO5_Cancel_T;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO5_Cancel_Test;
extern DataObject    iedModel_GenericIO_GGIO1_SPCSO6;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO6_SBO;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO6_Oper;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO6_Oper_ctlVal;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO6_Oper_operTm;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO6_Oper_origin;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO6_Oper_origin_orCat;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO6_Oper_origin_orIdent;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO6_Oper_ctlNum;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO6_Oper_T;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO6_Oper_Test;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO6_Oper_Check;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO6_Cancel;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO6_Cancel_ctlVal;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO6_Cancel_operTm;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO6_Cancel_origin;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO6_Cancel_origin_orCat;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO6_Cancel_origin_orIdent;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO6_Cancel_ctlNum;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO6_Cancel_T;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO6_Cancel_Test;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO6_stVal;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO6_q;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO6_t;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO6_ctlModel;
extern DataObject    iedModel_GenericIO_GGIO1_SPCSO7;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO7_Oper;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO7_Oper_ctlVal;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO7_Oper_operTm;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO7_Oper_origin;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO7_Oper_origin_orCat;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO7_Oper_origin_orIdent;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO7_Oper_ctlNum;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO7_Oper_T;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO7_Oper_Test;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO7_Oper_Check;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO7_Cancel;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO7_Cancel_ctlVal;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO7_Cancel_operTm;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO7_Cancel_origin;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO7_Cancel_origin_orCat;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO7_Cancel_origin_orIdent;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO7_Cancel_ctlNum;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO7_Cancel_T;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO7_Cancel_Test;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO7_stVal;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO7_q;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO7_t;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO7_ctlModel;
extern DataObject    iedModel_GenericIO_GGIO1_SPCSO8;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_SBOw;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_SBOw_ctlVal;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_SBOw_operTm;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_SBOw_origin;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_SBOw_origin_orCat;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_SBOw_origin_orIdent;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_SBOw_ctlNum;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_SBOw_T;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_SBOw_Test;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_SBOw_Check;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_Oper;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_Oper_ctlVal;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_Oper_operTm;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_Oper_origin;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_Oper_origin_orCat;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_Oper_origin_orIdent;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_Oper_ctlNum;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_Oper_T;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_Oper_Test;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_Oper_Check;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_Cancel;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_Cancel_ctlVal;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_Cancel_operTm;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_Cancel_origin;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_Cancel_origin_orCat;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_Cancel_origin_orIdent;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_Cancel_ctlNum;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_Cancel_T;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_Cancel_Test;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_origin;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_origin_orCat;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_origin_orIdent;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_ctlNum;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_stVal;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_q;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_t;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_ctlModel;
extern DataObject    iedModel_GenericIO_GGIO1_SPCSO9;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO9_Oper;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO9_Oper_ctlVal;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO9_Oper_origin;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO9_Oper_origin_orCat;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO9_Oper_origin_orIdent;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO9_Oper_ctlNum;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO9_Oper_T;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO9_Oper_Test;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO9_Oper_Check;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO9_Cancel;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO9_Cancel_ctlVal;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO9_Cancel_origin;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO9_Cancel_origin_orCat;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO9_Cancel_origin_orIdent;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO9_Cancel_ctlNum;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO9_Cancel_T;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO9_Cancel_Test;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO9_stVal;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO9_q;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO9_t;
extern DataAttribute iedModel_GenericIO_GGIO1_SPCSO9_ctlModel;
extern DataObject    iedModel_GenericIO_GGIO1_Ind1;
extern DataAttribute iedModel_GenericIO_GGIO1_Ind1_stVal;
extern DataAttribute iedModel_GenericIO_GGIO1_Ind1_q;
extern DataAttribute iedModel_GenericIO_GGIO1_Ind1_t;
extern DataObject    iedModel_GenericIO_GGIO1_Ind2;
extern DataAttribute iedModel_GenericIO_GGIO1_Ind2_stVal;
extern DataAttribute iedModel_GenericIO_GGIO1_Ind2_q;
extern DataAttribute iedModel_GenericIO_GGIO1_Ind2_t;
extern DataObject    iedModel_GenericIO_GGIO1_Ind3;
extern DataAttribute iedModel_GenericIO_GGIO1_Ind3_stVal;
extern DataAttribute iedModel_GenericIO_GGIO1_Ind3_q;
extern DataAttribute iedModel_GenericIO_GGIO1_Ind3_t;
extern DataObject    iedModel_GenericIO_GGIO1_Ind4;
extern DataAttribute iedModel_GenericIO_GGIO1_Ind4_stVal;
extern DataAttribute iedModel_GenericIO_GGIO1_Ind4_q;
extern DataAttribute iedModel_GenericIO_GGIO1_Ind4_t;



LogicalDevice iedModel_GenericIO = {
    LogicalDeviceModelType,
    "GenericIO",
    (ModelNode*) &iedModel,
    NULL,
    (ModelNode*) &iedModel_GenericIO_LLN0
};

LogicalNode iedModel_GenericIO_LLN0 = {
    LogicalNodeModelType,
    "LLN0",
    (ModelNode*) &iedModel_GenericIO,
    (ModelNode*) &iedModel_GenericIO_LPHD1,
    (ModelNode*) &iedModel_GenericIO_LLN0_Mod,
};

DataObject iedModel_GenericIO_LLN0_Mod = {
    DataObjectModelType,
    "Mod",
    (ModelNode*) &iedModel_GenericIO_LLN0,
    (ModelNode*) &iedModel_GenericIO_LLN0_Beh,
    (ModelNode*) &iedModel_GenericIO_LLN0_Mod_q,
    0
};

DataAttribute iedModel_GenericIO_LLN0_Mod_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_LLN0_Mod,
    (ModelNode*) &iedModel_GenericIO_LLN0_Mod_t,
    NULL,
    0,
    IEC61850_FC_ST,
    QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LLN0_Mod_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_LLN0_Mod,
    (ModelNode*) &iedModel_GenericIO_LLN0_Mod_ctlModel,
    NULL,
    0,
    IEC61850_FC_ST,
    TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LLN0_Mod_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_GenericIO_LLN0_Mod,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_LLN0_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_GenericIO_LLN0,
    (ModelNode*) &iedModel_GenericIO_LLN0_Health,
    (ModelNode*) &iedModel_GenericIO_LLN0_Beh_stVal,
    0
};

DataAttribute iedModel_GenericIO_LLN0_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_LLN0_Beh,
    (ModelNode*) &iedModel_GenericIO_LLN0_Beh_q,
    NULL,
    0,
    IEC61850_FC_ST,
    INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LLN0_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_LLN0_Beh,
    (ModelNode*) &iedModel_GenericIO_LLN0_Beh_t,
    NULL,
    0,
    IEC61850_FC_ST,
    QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LLN0_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_LLN0_Beh,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_LLN0_Health = {
    DataObjectModelType,
    "Health",
    (ModelNode*) &iedModel_GenericIO_LLN0,
    (ModelNode*) &iedModel_GenericIO_LLN0_NamPlt,
    (ModelNode*) &iedModel_GenericIO_LLN0_Health_stVal,
    0
};

DataAttribute iedModel_GenericIO_LLN0_Health_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_LLN0_Health,
    (ModelNode*) &iedModel_GenericIO_LLN0_Health_q,
    NULL,
    0,
    IEC61850_FC_ST,
    INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LLN0_Health_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_LLN0_Health,
    (ModelNode*) &iedModel_GenericIO_LLN0_Health_t,
    NULL,
    0,
    IEC61850_FC_ST,
    QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LLN0_Health_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_LLN0_Health,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_LLN0_NamPlt = {
    DataObjectModelType,
    "NamPlt",
    (ModelNode*) &iedModel_GenericIO_LLN0,
    NULL,
    (ModelNode*) &iedModel_GenericIO_LLN0_NamPlt_vendor,
    0
};

DataAttribute iedModel_GenericIO_LLN0_NamPlt_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_GenericIO_LLN0_NamPlt,
    (ModelNode*) &iedModel_GenericIO_LLN0_NamPlt_swRev,
    NULL,
    0,
    IEC61850_FC_DC,
    VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LLN0_NamPlt_swRev = {
    DataAttributeModelType,
    "swRev",
    (ModelNode*) &iedModel_GenericIO_LLN0_NamPlt,
    (ModelNode*) &iedModel_GenericIO_LLN0_NamPlt_d,
    NULL,
    0,
    IEC61850_FC_DC,
    VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LLN0_NamPlt_d = {
    DataAttributeModelType,
    "d",
    (ModelNode*) &iedModel_GenericIO_LLN0_NamPlt,
    (ModelNode*) &iedModel_GenericIO_LLN0_NamPlt_configRev,
    NULL,
    0,
    IEC61850_FC_DC,
    VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LLN0_NamPlt_configRev = {
    DataAttributeModelType,
    "configRev",
    (ModelNode*) &iedModel_GenericIO_LLN0_NamPlt,
    (ModelNode*) &iedModel_GenericIO_LLN0_NamPlt_ldNs,
    NULL,
    0,
    IEC61850_FC_DC,
    VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LLN0_NamPlt_ldNs = {
    DataAttributeModelType,
    "ldNs",
    (ModelNode*) &iedModel_GenericIO_LLN0_NamPlt,
    NULL,
    NULL,
    0,
    IEC61850_FC_EX,
    VISIBLE_STRING_255,
    0,
    NULL,
    0};

LogicalNode iedModel_GenericIO_LPHD1 = {
    LogicalNodeModelType,
    "LPHD1",
    (ModelNode*) &iedModel_GenericIO,
    (ModelNode*) &iedModel_GenericIO_GGIO1,
    (ModelNode*) &iedModel_GenericIO_LPHD1_PhyNam,
};

DataObject iedModel_GenericIO_LPHD1_PhyNam = {
    DataObjectModelType,
    "PhyNam",
    (ModelNode*) &iedModel_GenericIO_LPHD1,
    (ModelNode*) &iedModel_GenericIO_LPHD1_PhyHealth,
    (ModelNode*) &iedModel_GenericIO_LPHD1_PhyNam_vendor,
    0
};

DataAttribute iedModel_GenericIO_LPHD1_PhyNam_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_GenericIO_LPHD1_PhyNam,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_LPHD1_PhyHealth = {
    DataObjectModelType,
    "PhyHealth",
    (ModelNode*) &iedModel_GenericIO_LPHD1,
    (ModelNode*) &iedModel_GenericIO_LPHD1_Proxy,
    (ModelNode*) &iedModel_GenericIO_LPHD1_PhyHealth_stVal,
    0
};

DataAttribute iedModel_GenericIO_LPHD1_PhyHealth_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_LPHD1_PhyHealth,
    (ModelNode*) &iedModel_GenericIO_LPHD1_PhyHealth_q,
    NULL,
    0,
    IEC61850_FC_ST,
    INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LPHD1_PhyHealth_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_LPHD1_PhyHealth,
    (ModelNode*) &iedModel_GenericIO_LPHD1_PhyHealth_t,
    NULL,
    0,
    IEC61850_FC_ST,
    QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LPHD1_PhyHealth_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_LPHD1_PhyHealth,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_LPHD1_Proxy = {
    DataObjectModelType,
    "Proxy",
    (ModelNode*) &iedModel_GenericIO_LPHD1,
    NULL,
    (ModelNode*) &iedModel_GenericIO_LPHD1_Proxy_stVal,
    0
};

DataAttribute iedModel_GenericIO_LPHD1_Proxy_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_LPHD1_Proxy,
    (ModelNode*) &iedModel_GenericIO_LPHD1_Proxy_q,
    NULL,
    0,
    IEC61850_FC_ST,
    BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LPHD1_Proxy_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_LPHD1_Proxy,
    (ModelNode*) &iedModel_GenericIO_LPHD1_Proxy_t,
    NULL,
    0,
    IEC61850_FC_ST,
    QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_LPHD1_Proxy_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_LPHD1_Proxy,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    TIMESTAMP,
    0,
    NULL,
    0};

LogicalNode iedModel_GenericIO_GGIO1 = {
    LogicalNodeModelType,
    "GGIO1",
    (ModelNode*) &iedModel_GenericIO,
    NULL,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Mod,
};

DataObject iedModel_GenericIO_GGIO1_Mod = {
    DataObjectModelType,
    "Mod",
    (ModelNode*) &iedModel_GenericIO_GGIO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Beh,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Mod_q,
    0
};

DataAttribute iedModel_GenericIO_GGIO1_Mod_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_GGIO1_Mod,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Mod_t,
    NULL,
    0,
    IEC61850_FC_ST,
    QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_Mod_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_GGIO1_Mod,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Mod_ctlModel,
    NULL,
    0,
    IEC61850_FC_ST,
    TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_Mod_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_GenericIO_GGIO1_Mod,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_GGIO1_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_GenericIO_GGIO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Health,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Beh_stVal,
    0
};

DataAttribute iedModel_GenericIO_GGIO1_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_Beh,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Beh_q,
    NULL,
    0,
    IEC61850_FC_ST,
    INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_GGIO1_Beh,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Beh_t,
    NULL,
    0,
    IEC61850_FC_ST,
    QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_GGIO1_Beh,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_GGIO1_Health = {
    DataObjectModelType,
    "Health",
    (ModelNode*) &iedModel_GenericIO_GGIO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_NamPlt,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Health_stVal,
    0
};

DataAttribute iedModel_GenericIO_GGIO1_Health_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_Health,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Health_q,
    NULL,
    0,
    IEC61850_FC_ST,
    INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_Health_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_GGIO1_Health,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Health_t,
    NULL,
    0,
    IEC61850_FC_ST,
    QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_Health_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_GGIO1_Health,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_GGIO1_NamPlt = {
    DataObjectModelType,
    "NamPlt",
    (ModelNode*) &iedModel_GenericIO_GGIO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_NamPlt_vendor,
    0
};

DataAttribute iedModel_GenericIO_GGIO1_NamPlt_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_GenericIO_GGIO1_NamPlt,
    (ModelNode*) &iedModel_GenericIO_GGIO1_NamPlt_swRev,
    NULL,
    0,
    IEC61850_FC_DC,
    VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_NamPlt_swRev = {
    DataAttributeModelType,
    "swRev",
    (ModelNode*) &iedModel_GenericIO_GGIO1_NamPlt,
    (ModelNode*) &iedModel_GenericIO_GGIO1_NamPlt_d,
    NULL,
    0,
    IEC61850_FC_DC,
    VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_NamPlt_d = {
    DataAttributeModelType,
    "d",
    (ModelNode*) &iedModel_GenericIO_GGIO1_NamPlt,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_GGIO1_AnIn1 = {
    DataObjectModelType,
    "AnIn1",
    (ModelNode*) &iedModel_GenericIO_GGIO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn2,
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn1_mag,
    0
};

DataAttribute iedModel_GenericIO_GGIO1_AnIn1_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn1_q,
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn1_mag_f,
    0,
    IEC61850_FC_MX,
    CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_AnIn1_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn1_mag,
    NULL,
    NULL,
    0,
    IEC61850_FC_MX,
    FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_AnIn1_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn1_t,
    NULL,
    0,
    IEC61850_FC_MX,
    QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_AnIn1_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn1,
    NULL,
    NULL,
    0,
    IEC61850_FC_MX,
    TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_GGIO1_AnIn2 = {
    DataObjectModelType,
    "AnIn2",
    (ModelNode*) &iedModel_GenericIO_GGIO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn3,
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn2_mag,
    0
};

DataAttribute iedModel_GenericIO_GGIO1_AnIn2_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn2,
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn2_q,
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn2_mag_f,
    0,
    IEC61850_FC_MX,
    CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_AnIn2_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn2_mag,
    NULL,
    NULL,
    0,
    IEC61850_FC_MX,
    FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_AnIn2_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn2,
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn2_t,
    NULL,
    0,
    IEC61850_FC_MX,
    QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_AnIn2_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn2,
    NULL,
    NULL,
    0,
    IEC61850_FC_MX,
    TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_GGIO1_AnIn3 = {
    DataObjectModelType,
    "AnIn3",
    (ModelNode*) &iedModel_GenericIO_GGIO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn4,
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn3_mag,
    0
};

DataAttribute iedModel_GenericIO_GGIO1_AnIn3_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn3,
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn3_q,
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn3_mag_f,
    0,
    IEC61850_FC_MX,
    CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_AnIn3_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn3_mag,
    NULL,
    NULL,
    0,
    IEC61850_FC_MX,
    FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_AnIn3_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn3,
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn3_t,
    NULL,
    0,
    IEC61850_FC_MX,
    QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_AnIn3_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn3,
    NULL,
    NULL,
    0,
    IEC61850_FC_MX,
    TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_GGIO1_AnIn4 = {
    DataObjectModelType,
    "AnIn4",
    (ModelNode*) &iedModel_GenericIO_GGIO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn4_mag,
    0
};

DataAttribute iedModel_GenericIO_GGIO1_AnIn4_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn4,
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn4_q,
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn4_mag_f,
    0,
    IEC61850_FC_MX,
    CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_AnIn4_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn4_mag,
    NULL,
    NULL,
    0,
    IEC61850_FC_MX,
    FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_AnIn4_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn4,
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn4_t,
    NULL,
    0,
    IEC61850_FC_MX,
    QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_AnIn4_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_GGIO1_AnIn4,
    NULL,
    NULL,
    0,
    IEC61850_FC_MX,
    TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_GGIO1_SPCSO1 = {
    DataObjectModelType,
    "SPCSO1",
    (ModelNode*) &iedModel_GenericIO_GGIO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1_Oper,
    0
};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO1_Oper = {
    DataAttributeModelType,
    "Oper",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1_stVal,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1_Oper_ctlVal,
    0,
    IEC61850_FC_CO,
    CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO1_Oper_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1_Oper_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO1_Oper_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1_Oper_ctlNum,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1_Oper_origin_orCat,
    0,
    IEC61850_FC_CO,
    CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO1_Oper_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1_Oper_origin,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1_Oper_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO1_Oper_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1_Oper_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO1_Oper_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1_Oper_T,
    NULL,
    0,
    IEC61850_FC_CO,
    INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO1_Oper_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1_Oper_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO1_Oper_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1_Oper_Check,
    NULL,
    0,
    IEC61850_FC_CO,
    BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO1_Oper_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1_Oper,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO1_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1_q,
    NULL,
    0,
    IEC61850_FC_ST,
    BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO1_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1_t,
    NULL,
    0,
    IEC61850_FC_ST,
    QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO1_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1_ctlModel,
    NULL,
    0,
    IEC61850_FC_ST,
    TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO1_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO1,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_GGIO1_SPCSO2 = {
    DataObjectModelType,
    "SPCSO2",
    (ModelNode*) &iedModel_GenericIO_GGIO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_SBO,
    0
};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_SBO = {
    DataAttributeModelType,
    "SBO",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Oper,
    NULL,
    0,
    IEC61850_FC_CO,
    VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_Oper = {
    DataAttributeModelType,
    "Oper",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Cancel,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Oper_ctlVal,
    0,
    IEC61850_FC_CO,
    CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_Oper_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Oper_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_Oper_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Oper_ctlNum,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Oper_origin_orCat,
    0,
    IEC61850_FC_CO,
    CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_Oper_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Oper_origin,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Oper_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_Oper_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Oper_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_Oper_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Oper_T,
    NULL,
    0,
    IEC61850_FC_CO,
    INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_Oper_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Oper_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_Oper_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Oper_Check,
    NULL,
    0,
    IEC61850_FC_CO,
    BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_Oper_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Oper,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_Cancel = {
    DataAttributeModelType,
    "Cancel",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_stVal,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Cancel_ctlVal,
    0,
    IEC61850_FC_CO,
    CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_Cancel_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Cancel,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Cancel_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_Cancel_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Cancel,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Cancel_ctlNum,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Cancel_origin_orCat,
    0,
    IEC61850_FC_CO,
    CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_Cancel_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Cancel_origin,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Cancel_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_Cancel_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Cancel_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_Cancel_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Cancel,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Cancel_T,
    NULL,
    0,
    IEC61850_FC_CO,
    INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_Cancel_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Cancel,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Cancel_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_Cancel_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_Cancel,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_q,
    NULL,
    0,
    IEC61850_FC_ST,
    BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_t,
    NULL,
    0,
    IEC61850_FC_ST,
    QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_ctlModel,
    NULL,
    0,
    IEC61850_FC_ST,
    TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2_sboClass,
    NULL,
    0,
    IEC61850_FC_CF,
    ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO2_sboClass = {
    DataAttributeModelType,
    "sboClass",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO2,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_GGIO1_SPCSO3 = {
    DataObjectModelType,
    "SPCSO3",
    (ModelNode*) &iedModel_GenericIO_GGIO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_Oper,
    0
};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_Oper = {
    DataAttributeModelType,
    "Oper",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_Cancel,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_Oper_ctlVal,
    0,
    IEC61850_FC_CO,
    CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_Oper_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_Oper_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_Oper_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_Oper_ctlNum,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_Oper_origin_orCat,
    0,
    IEC61850_FC_CO,
    CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_Oper_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_Oper_origin,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_Oper_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_Oper_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_Oper_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_Oper_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_Oper_T,
    NULL,
    0,
    IEC61850_FC_CO,
    INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_Oper_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_Oper_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_Oper_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_Oper_Check,
    NULL,
    0,
    IEC61850_FC_CO,
    BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_Oper_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_Oper,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_Cancel = {
    DataAttributeModelType,
    "Cancel",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_stVal,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_Cancel_ctlVal,
    0,
    IEC61850_FC_CO,
    CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_Cancel_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_Cancel,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_Cancel_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_Cancel_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_Cancel,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_Cancel_ctlNum,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_Cancel_origin_orCat,
    0,
    IEC61850_FC_CO,
    CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_Cancel_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_Cancel_origin,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_Cancel_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_Cancel_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_Cancel_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_Cancel_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_Cancel,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_Cancel_T,
    NULL,
    0,
    IEC61850_FC_CO,
    INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_Cancel_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_Cancel,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_Cancel_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_Cancel_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_Cancel,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_q,
    NULL,
    0,
    IEC61850_FC_ST,
    BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_t,
    NULL,
    0,
    IEC61850_FC_ST,
    QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3_ctlModel,
    NULL,
    0,
    IEC61850_FC_ST,
    TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO3_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO3,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_GGIO1_SPCSO4 = {
    DataObjectModelType,
    "SPCSO4",
    (ModelNode*) &iedModel_GenericIO_GGIO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO5,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_SBOw,
    0
};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_SBOw = {
    DataAttributeModelType,
    "SBOw",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_SBOw_ctlVal,
    0,
    IEC61850_FC_CO,
    CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_SBOw_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_SBOw,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_SBOw_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_SBOw_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_SBOw,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_SBOw_ctlNum,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_SBOw_origin_orCat,
    0,
    IEC61850_FC_CO,
    CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_SBOw_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_SBOw_origin,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_SBOw_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_SBOw_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_SBOw_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_SBOw_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_SBOw,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_SBOw_T,
    NULL,
    0,
    IEC61850_FC_CO,
    INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_SBOw_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_SBOw,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_SBOw_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_SBOw_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_SBOw,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_SBOw_Check,
    NULL,
    0,
    IEC61850_FC_CO,
    BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_SBOw_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_SBOw,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_Oper = {
    DataAttributeModelType,
    "Oper",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_Cancel,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_Oper_ctlVal,
    0,
    IEC61850_FC_CO,
    CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_Oper_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_Oper_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_Oper_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_Oper_ctlNum,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_Oper_origin_orCat,
    0,
    IEC61850_FC_CO,
    CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_Oper_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_Oper_origin,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_Oper_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_Oper_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_Oper_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_Oper_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_Oper_T,
    NULL,
    0,
    IEC61850_FC_CO,
    INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_Oper_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_Oper_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_Oper_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_Oper_Check,
    NULL,
    0,
    IEC61850_FC_CO,
    BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_Oper_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_Oper,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_Cancel = {
    DataAttributeModelType,
    "Cancel",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_stVal,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_Cancel_ctlVal,
    0,
    IEC61850_FC_CO,
    CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_Cancel_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_Cancel,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_Cancel_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_Cancel_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_Cancel,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_Cancel_ctlNum,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_Cancel_origin_orCat,
    0,
    IEC61850_FC_CO,
    CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_Cancel_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_Cancel_origin,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_Cancel_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_Cancel_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_Cancel_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_Cancel_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_Cancel,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_Cancel_T,
    NULL,
    0,
    IEC61850_FC_CO,
    INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_Cancel_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_Cancel,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_Cancel_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_Cancel_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_Cancel,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_q,
    NULL,
    0,
    IEC61850_FC_ST,
    BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_t,
    NULL,
    0,
    IEC61850_FC_ST,
    QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4_ctlModel,
    NULL,
    0,
    IEC61850_FC_ST,
    TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO4_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO4,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_GGIO1_SPCSO5 = {
    DataObjectModelType,
    "SPCSO5",
    (ModelNode*) &iedModel_GenericIO_GGIO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO6,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO5_Oper,
    0
};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO5_Oper = {
    DataAttributeModelType,
    "Oper",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO5,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO5_stVal,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO5_Oper_ctlVal,
    0,
    IEC61850_FC_CO,
    CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO5_Oper_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO5_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO5_Oper_operTm,
    NULL,
    0,
    IEC61850_FC_CO,
    BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO5_Oper_operTm = {
    DataAttributeModelType,
    "operTm",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO5_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO5_Oper_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO5_Oper_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO5_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO5_Oper_ctlNum,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO5_Oper_origin_orCat,
    0,
    IEC61850_FC_CO,
    CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO5_Oper_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO5_Oper_origin,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO5_Oper_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO5_Oper_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO5_Oper_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO5_Oper_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO5_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO5_Oper_T,
    NULL,
    0,
    IEC61850_FC_CO,
    INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO5_Oper_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO5_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO5_Oper_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO5_Oper_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO5_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO5_Oper_Check,
    NULL,
    0,
    IEC61850_FC_CO,
    BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO5_Oper_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO5_Oper,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO5_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO5,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO5_q,
    NULL,
    0,
    IEC61850_FC_ST,
    BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO5_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO5,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO5_t,
    NULL,
    0,
    IEC61850_FC_ST,
    QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO5_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO5,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO5_ctlModel,
    NULL,
    0,
    IEC61850_FC_ST,
    TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO5_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO5,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO5_Cancel,
    NULL,
    0,
    IEC61850_FC_CF,
    ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO5_Cancel = {
    DataAttributeModelType,
    "Cancel",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO5,
    NULL,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO5_Cancel_ctlVal,
    0,
    IEC61850_FC_CO,
    CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO5_Cancel_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO5_Cancel,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO5_Cancel_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO5_Cancel_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO5_Cancel,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO5_Cancel_ctlNum,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO5_Cancel_origin_orCat,
    0,
    IEC61850_FC_CO,
    CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO5_Cancel_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO5_Cancel_origin,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO5_Cancel_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO5_Cancel_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO5_Cancel_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO5_Cancel_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO5_Cancel,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO5_Cancel_T,
    NULL,
    0,
    IEC61850_FC_CO,
    INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO5_Cancel_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO5_Cancel,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO5_Cancel_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO5_Cancel_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO5_Cancel,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    BOOLEAN,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_GGIO1_SPCSO6 = {
    DataObjectModelType,
    "SPCSO6",
    (ModelNode*) &iedModel_GenericIO_GGIO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO7,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO6_SBO,
    0
};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO6_SBO = {
    DataAttributeModelType,
    "SBO",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO6,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO6_Oper,
    NULL,
    0,
    IEC61850_FC_CO,
    VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO6_Oper = {
    DataAttributeModelType,
    "Oper",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO6,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO6_Cancel,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO6_Oper_ctlVal,
    0,
    IEC61850_FC_CO,
    CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO6_Oper_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO6_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO6_Oper_operTm,
    NULL,
    0,
    IEC61850_FC_CO,
    BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO6_Oper_operTm = {
    DataAttributeModelType,
    "operTm",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO6_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO6_Oper_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO6_Oper_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO6_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO6_Oper_ctlNum,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO6_Oper_origin_orCat,
    0,
    IEC61850_FC_CO,
    CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO6_Oper_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO6_Oper_origin,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO6_Oper_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO6_Oper_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO6_Oper_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO6_Oper_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO6_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO6_Oper_T,
    NULL,
    0,
    IEC61850_FC_CO,
    INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO6_Oper_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO6_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO6_Oper_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO6_Oper_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO6_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO6_Oper_Check,
    NULL,
    0,
    IEC61850_FC_CO,
    BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO6_Oper_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO6_Oper,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO6_Cancel = {
    DataAttributeModelType,
    "Cancel",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO6,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO6_stVal,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO6_Cancel_ctlVal,
    0,
    IEC61850_FC_CO,
    CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO6_Cancel_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO6_Cancel,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO6_Cancel_operTm,
    NULL,
    0,
    IEC61850_FC_CO,
    BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO6_Cancel_operTm = {
    DataAttributeModelType,
    "operTm",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO6_Cancel,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO6_Cancel_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO6_Cancel_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO6_Cancel,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO6_Cancel_ctlNum,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO6_Cancel_origin_orCat,
    0,
    IEC61850_FC_CO,
    CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO6_Cancel_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO6_Cancel_origin,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO6_Cancel_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO6_Cancel_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO6_Cancel_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO6_Cancel_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO6_Cancel,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO6_Cancel_T,
    NULL,
    0,
    IEC61850_FC_CO,
    INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO6_Cancel_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO6_Cancel,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO6_Cancel_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO6_Cancel_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO6_Cancel,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO6_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO6,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO6_q,
    NULL,
    0,
    IEC61850_FC_ST,
    BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO6_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO6,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO6_t,
    NULL,
    0,
    IEC61850_FC_ST,
    QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO6_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO6,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO6_ctlModel,
    NULL,
    0,
    IEC61850_FC_ST,
    TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO6_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO6,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_GGIO1_SPCSO7 = {
    DataObjectModelType,
    "SPCSO7",
    (ModelNode*) &iedModel_GenericIO_GGIO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO7_Oper,
    0
};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO7_Oper = {
    DataAttributeModelType,
    "Oper",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO7,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO7_Cancel,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO7_Oper_ctlVal,
    0,
    IEC61850_FC_CO,
    CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO7_Oper_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO7_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO7_Oper_operTm,
    NULL,
    0,
    IEC61850_FC_CO,
    BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO7_Oper_operTm = {
    DataAttributeModelType,
    "operTm",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO7_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO7_Oper_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO7_Oper_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO7_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO7_Oper_ctlNum,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO7_Oper_origin_orCat,
    0,
    IEC61850_FC_CO,
    CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO7_Oper_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO7_Oper_origin,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO7_Oper_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO7_Oper_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO7_Oper_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO7_Oper_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO7_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO7_Oper_T,
    NULL,
    0,
    IEC61850_FC_CO,
    INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO7_Oper_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO7_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO7_Oper_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO7_Oper_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO7_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO7_Oper_Check,
    NULL,
    0,
    IEC61850_FC_CO,
    BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO7_Oper_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO7_Oper,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO7_Cancel = {
    DataAttributeModelType,
    "Cancel",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO7,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO7_stVal,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO7_Cancel_ctlVal,
    0,
    IEC61850_FC_CO,
    CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO7_Cancel_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO7_Cancel,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO7_Cancel_operTm,
    NULL,
    0,
    IEC61850_FC_CO,
    BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO7_Cancel_operTm = {
    DataAttributeModelType,
    "operTm",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO7_Cancel,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO7_Cancel_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO7_Cancel_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO7_Cancel,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO7_Cancel_ctlNum,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO7_Cancel_origin_orCat,
    0,
    IEC61850_FC_CO,
    CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO7_Cancel_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO7_Cancel_origin,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO7_Cancel_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO7_Cancel_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO7_Cancel_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO7_Cancel_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO7_Cancel,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO7_Cancel_T,
    NULL,
    0,
    IEC61850_FC_CO,
    INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO7_Cancel_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO7_Cancel,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO7_Cancel_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO7_Cancel_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO7_Cancel,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO7_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO7,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO7_q,
    NULL,
    0,
    IEC61850_FC_ST,
    BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO7_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO7,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO7_t,
    NULL,
    0,
    IEC61850_FC_ST,
    QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO7_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO7,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO7_ctlModel,
    NULL,
    0,
    IEC61850_FC_ST,
    TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO7_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO7,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_GGIO1_SPCSO8 = {
    DataObjectModelType,
    "SPCSO8",
    (ModelNode*) &iedModel_GenericIO_GGIO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO9,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_SBOw,
    0
};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_SBOw = {
    DataAttributeModelType,
    "SBOw",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_SBOw_ctlVal,
    0,
    IEC61850_FC_CO,
    CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_SBOw_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_SBOw,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_SBOw_operTm,
    NULL,
    0,
    IEC61850_FC_CO,
    BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_SBOw_operTm = {
    DataAttributeModelType,
    "operTm",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_SBOw,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_SBOw_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_SBOw_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_SBOw,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_SBOw_ctlNum,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_SBOw_origin_orCat,
    0,
    IEC61850_FC_CO,
    CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_SBOw_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_SBOw_origin,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_SBOw_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_SBOw_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_SBOw_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_SBOw_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_SBOw,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_SBOw_T,
    NULL,
    0,
    IEC61850_FC_CO,
    INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_SBOw_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_SBOw,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_SBOw_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_SBOw_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_SBOw,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_SBOw_Check,
    NULL,
    0,
    IEC61850_FC_CO,
    BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_SBOw_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_SBOw,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_Oper = {
    DataAttributeModelType,
    "Oper",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_Cancel,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_Oper_ctlVal,
    0,
    IEC61850_FC_CO,
    CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_Oper_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_Oper_operTm,
    NULL,
    0,
    IEC61850_FC_CO,
    BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_Oper_operTm = {
    DataAttributeModelType,
    "operTm",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_Oper_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_Oper_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_Oper_ctlNum,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_Oper_origin_orCat,
    0,
    IEC61850_FC_CO,
    CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_Oper_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_Oper_origin,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_Oper_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_Oper_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_Oper_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_Oper_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_Oper_T,
    NULL,
    0,
    IEC61850_FC_CO,
    INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_Oper_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_Oper_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_Oper_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_Oper_Check,
    NULL,
    0,
    IEC61850_FC_CO,
    BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_Oper_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_Oper,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_Cancel = {
    DataAttributeModelType,
    "Cancel",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_origin,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_Cancel_ctlVal,
    0,
    IEC61850_FC_CO,
    CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_Cancel_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_Cancel,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_Cancel_operTm,
    NULL,
    0,
    IEC61850_FC_CO,
    BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_Cancel_operTm = {
    DataAttributeModelType,
    "operTm",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_Cancel,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_Cancel_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_Cancel_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_Cancel,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_Cancel_ctlNum,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_Cancel_origin_orCat,
    0,
    IEC61850_FC_CO,
    CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_Cancel_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_Cancel_origin,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_Cancel_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_Cancel_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_Cancel_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_Cancel_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_Cancel,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_Cancel_T,
    NULL,
    0,
    IEC61850_FC_CO,
    INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_Cancel_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_Cancel,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_Cancel_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_Cancel_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_Cancel,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_ctlNum,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_origin_orCat,
    0,
    IEC61850_FC_ST,
    CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_origin,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_ST,
    ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_stVal,
    NULL,
    0,
    IEC61850_FC_ST,
    INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_q,
    NULL,
    0,
    IEC61850_FC_ST,
    BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_t,
    NULL,
    0,
    IEC61850_FC_ST,
    QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8_ctlModel,
    NULL,
    0,
    IEC61850_FC_ST,
    TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO8_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO8,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_GGIO1_SPCSO9 = {
    DataObjectModelType,
    "SPCSO9",
    (ModelNode*) &iedModel_GenericIO_GGIO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO9_Oper,
    0
};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO9_Oper = {
    DataAttributeModelType,
    "Oper",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO9,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO9_Cancel,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO9_Oper_ctlVal,
    0,
    IEC61850_FC_CO,
    CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO9_Oper_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO9_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO9_Oper_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO9_Oper_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO9_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO9_Oper_ctlNum,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO9_Oper_origin_orCat,
    0,
    IEC61850_FC_CO,
    CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO9_Oper_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO9_Oper_origin,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO9_Oper_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO9_Oper_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO9_Oper_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO9_Oper_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO9_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO9_Oper_T,
    NULL,
    0,
    IEC61850_FC_CO,
    INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO9_Oper_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO9_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO9_Oper_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO9_Oper_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO9_Oper,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO9_Oper_Check,
    NULL,
    0,
    IEC61850_FC_CO,
    BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO9_Oper_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO9_Oper,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO9_Cancel = {
    DataAttributeModelType,
    "Cancel",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO9,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO9_stVal,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO9_Cancel_ctlVal,
    0,
    IEC61850_FC_CO,
    CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO9_Cancel_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO9_Cancel,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO9_Cancel_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO9_Cancel_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO9_Cancel,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO9_Cancel_ctlNum,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO9_Cancel_origin_orCat,
    0,
    IEC61850_FC_CO,
    CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO9_Cancel_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO9_Cancel_origin,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO9_Cancel_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO9_Cancel_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO9_Cancel_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO9_Cancel_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO9_Cancel,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO9_Cancel_T,
    NULL,
    0,
    IEC61850_FC_CO,
    INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO9_Cancel_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO9_Cancel,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO9_Cancel_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO9_Cancel_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO9_Cancel,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO9_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO9,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO9_q,
    NULL,
    0,
    IEC61850_FC_ST,
    BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO9_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO9,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO9_t,
    NULL,
    0,
    IEC61850_FC_ST,
    QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO9_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO9,
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO9_ctlModel,
    NULL,
    0,
    IEC61850_FC_ST,
    TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_SPCSO9_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_GenericIO_GGIO1_SPCSO9,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_GGIO1_Ind1 = {
    DataObjectModelType,
    "Ind1",
    (ModelNode*) &iedModel_GenericIO_GGIO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind2,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind1_stVal,
    0
};

DataAttribute iedModel_GenericIO_GGIO1_Ind1_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind1_q,
    NULL,
    0,
    IEC61850_FC_ST,
    BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_Ind1_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind1_t,
    NULL,
    0,
    IEC61850_FC_ST,
    QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_Ind1_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind1,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_GGIO1_Ind2 = {
    DataObjectModelType,
    "Ind2",
    (ModelNode*) &iedModel_GenericIO_GGIO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind3,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind2_stVal,
    0
};

DataAttribute iedModel_GenericIO_GGIO1_Ind2_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind2,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind2_q,
    NULL,
    0,
    IEC61850_FC_ST,
    BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_Ind2_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind2,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind2_t,
    NULL,
    0,
    IEC61850_FC_ST,
    QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_Ind2_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind2,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_GGIO1_Ind3 = {
    DataObjectModelType,
    "Ind3",
    (ModelNode*) &iedModel_GenericIO_GGIO1,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind4,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind3_stVal,
    0
};

DataAttribute iedModel_GenericIO_GGIO1_Ind3_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind3,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind3_q,
    NULL,
    0,
    IEC61850_FC_ST,
    BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_Ind3_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind3,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind3_t,
    NULL,
    0,
    IEC61850_FC_ST,
    QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_Ind3_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind3,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_GenericIO_GGIO1_Ind4 = {
    DataObjectModelType,
    "Ind4",
    (ModelNode*) &iedModel_GenericIO_GGIO1,
    NULL,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind4_stVal,
    0
};

DataAttribute iedModel_GenericIO_GGIO1_Ind4_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind4,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind4_q,
    NULL,
    0,
    IEC61850_FC_ST,
    BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_Ind4_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind4,
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind4_t,
    NULL,
    0,
    IEC61850_FC_ST,
    QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_GenericIO_GGIO1_Ind4_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_GenericIO_GGIO1_Ind4,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    TIMESTAMP,
    0,
    NULL,
    0};






IedModel iedModel = {
    "simpleIO",
    &iedModel_GenericIO,
    NULL,
    NULL,
    NULL,
    NULL,
    initializeValues
};

static void
initializeValues()
{

iedModel_GenericIO_LLN0_Mod_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(0);

iedModel_GenericIO_GGIO1_Mod_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(0);

iedModel_GenericIO_GGIO1_SPCSO1_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(1);

iedModel_GenericIO_GGIO1_SPCSO2_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(2);

iedModel_GenericIO_GGIO1_SPCSO3_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(3);

iedModel_GenericIO_GGIO1_SPCSO4_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(4);

iedModel_GenericIO_GGIO1_SPCSO5_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(1);

iedModel_GenericIO_GGIO1_SPCSO6_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(2);

iedModel_GenericIO_GGIO1_SPCSO7_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(3);

iedModel_GenericIO_GGIO1_SPCSO8_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(4);

iedModel_GenericIO_GGIO1_SPCSO9_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(3);
}
