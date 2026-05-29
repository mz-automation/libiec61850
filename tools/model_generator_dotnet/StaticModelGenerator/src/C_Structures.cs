/*
 *  Copyright 2013-2025 Michael Zillgith, MZ Automation GmbH
 *
 *  This file is part of MZ Automation IEC 61850 SDK
 * 
 *  All rights reserved.
 */

using IEC61850.SCL;
using IEC61850.SCL.DataModel;
using System;
using System.Collections.Generic;

namespace StaticModelGenerator.C_Structures
{
    /** FCs (Functional constraints) according to IEC 61850-7-2 */
    public enum eFunctionalConstraint
    {
        /** Status information */
        IEC61850_FC_ST = 0,
        /** Measurands - analog values */
        IEC61850_FC_MX = 1,
        /** Setpoint */
        IEC61850_FC_SP = 2,
        /** Substitution */
        IEC61850_FC_SV = 3,
        /** Configuration */
        IEC61850_FC_CF = 4,
        /** Description */
        IEC61850_FC_DC = 5,
        /** Setting group */
        IEC61850_FC_SG = 6,
        /** Setting group editable */
        IEC61850_FC_SE = 7,
        /** Service response / Service tracking */
        IEC61850_FC_SR = 8,
        /** Operate received */
        IEC61850_FC_OR = 9,
        /** Blocking */
        IEC61850_FC_BL = 10,
        /** Extended definition */
        IEC61850_FC_EX = 11,
        /** Control */
        IEC61850_FC_CO = 12,
        /** Unicast SV */
        IEC61850_FC_US = 13,
        /** Multicast SV */
        IEC61850_FC_MS = 14,
        /** Unbuffered report */
        IEC61850_FC_RP = 15,
        /** Buffered report */
        IEC61850_FC_BR = 16,
        /** Log control blocks */
        IEC61850_FC_LG = 17,
        /** Goose control blocks */
        IEC61850_FC_GO = 18,

        /** All FCs - wildcard value */
        IEC61850_FC_ALL = 99,
        IEC61850_FC_NONE = -1
    }

    public enum DataAttributeType
    {
        IEC61850_UNKNOWN_TYPE = -1,
        IEC61850_BOOLEAN = 0,/* int */
        IEC61850_INT8 = 1,   /* int8_t */
        IEC61850_INT16 = 2,  /* int16_t */
        IEC61850_INT32 = 3,  /* int32_t */
        IEC61850_INT64 = 4,  /* int64_t */
        IEC61850_INT128 = 5, /* no native mapping! */
        IEC61850_INT8U = 6,  /* uint8_t */
        IEC61850_INT16U = 7, /* uint16_t */
        IEC61850_INT24U = 8, /* uint32_t */
        IEC61850_INT32U = 9, /* uint32_t */
        IEC61850_FLOAT32 = 10, /* float */
        IEC61850_FLOAT64 = 11, /* double */
        IEC61850_ENUMERATED = 12,
        IEC61850_OCTET_STRING_64 = 13,
        IEC61850_OCTET_STRING_6 = 14,
        IEC61850_OCTET_STRING_8 = 15,
        IEC61850_VISIBLE_STRING_32 = 16,
        IEC61850_VISIBLE_STRING_64 = 17,
        IEC61850_VISIBLE_STRING_65 = 18,
        IEC61850_VISIBLE_STRING_129 = 19,
        IEC61850_VISIBLE_STRING_255 = 20,
        IEC61850_UNICODE_STRING_255 = 21,
        IEC61850_TIMESTAMP = 22,
        IEC61850_QUALITY = 23,
        IEC61850_CHECK = 24,
        IEC61850_CODEDENUM = 25,
        IEC61850_GENERIC_BITSTRING = 26,
        IEC61850_CONSTRUCTED = 27,
        IEC61850_ENTRY_TIME = 28,
        IEC61850_PHYCOMADDR = 29,
        IEC61850_CURRENCY = 30,
        IEC61850_OPTFLDS = 31, /* bit-string(10) */
        IEC61850_TRGOPS = 32 /* bit-string(6) */
    }


    public class C_InitializeValues
    {
        public string c_text;
        public C_InitializeValues()
        {
        }

    }

    public class C_DO_DA_Structure
    {
        public string objRef;

        public C_DO_DA_Structure()
        {
        }
    }

    public class C_DataAttributeStructure : C_DO_DA_Structure
    {
        public string ModelNodeType = "DataAttributeModelType";
        public string name;
        public string parent;
        public string sibling = "NULL";
        public string child = "NULL";
        public int elementCount = 0; /* value > 0 if this is an array */
        public int arrayIndex = -1; /* value > -1 when this is an array element */

        public DataAttribute DataAttribute = null;

        public string triggerOptions; /* TRG_OPT_DATA_CHANGED | TRG_OPT_QUALITY_CHANGED | TRG_OPT_DATA_UPDATE */

        public string mmsValue = "NULL";

        public string sAddr = "0"; /* TODO remove in version 2.0 */

        public string refName = null;

        public bool isTransient = false;

        public SclFC sclFC = SclFC.NONE;

        //public List<C_DataAttributeStructure> c_DataAttributeStructures = new List<C_DataAttributeStructure>();

        public C_DataAttributeStructure() : base()
        {

        }

        public string ExternNameToString()
        {
            return "extern DataAttribute " + objRef + ";";
        }

        private static DataAttributeType ConvertToDataAttributeType(AttributeType type)
        {
            switch (type)
            {
                case AttributeType.BOOLEAN: return DataAttributeType.IEC61850_BOOLEAN;
                case AttributeType.INT8: return DataAttributeType.IEC61850_INT8;
                case AttributeType.INT16: return DataAttributeType.IEC61850_INT16;
                case AttributeType.INT32: return DataAttributeType.IEC61850_INT32;
                case AttributeType.INT64: return DataAttributeType.IEC61850_INT64;
                case AttributeType.INT128: return DataAttributeType.IEC61850_INT128;
                case AttributeType.INT8U: return DataAttributeType.IEC61850_INT8U;
                case AttributeType.INT16U: return DataAttributeType.IEC61850_INT16U;
                case AttributeType.INT24U: return DataAttributeType.IEC61850_INT24U;
                case AttributeType.INT32U: return DataAttributeType.IEC61850_INT32U;
                case AttributeType.FLOAT32: return DataAttributeType.IEC61850_FLOAT32;
                case AttributeType.FLOAT64: return DataAttributeType.IEC61850_FLOAT64;
                case AttributeType.ENUMERATED: return DataAttributeType.IEC61850_ENUMERATED;
                case AttributeType.OCTET_STRING_64: return DataAttributeType.IEC61850_OCTET_STRING_64;
                case AttributeType.OCTET_STRING_6: return DataAttributeType.IEC61850_OCTET_STRING_6;
                case AttributeType.OCTET_STRING_8: return DataAttributeType.IEC61850_OCTET_STRING_8;
                case AttributeType.VISIBLE_STRING_32: return DataAttributeType.IEC61850_VISIBLE_STRING_32;
                case AttributeType.VISIBLE_STRING_64: return DataAttributeType.IEC61850_VISIBLE_STRING_64;
                case AttributeType.VISIBLE_STRING_65: return DataAttributeType.IEC61850_VISIBLE_STRING_65;
                case AttributeType.VISIBLE_STRING_129: return DataAttributeType.IEC61850_VISIBLE_STRING_129;
                case AttributeType.VISIBLE_STRING_255: return DataAttributeType.IEC61850_VISIBLE_STRING_255;
                case AttributeType.UNICODE_STRING_255: return DataAttributeType.IEC61850_UNICODE_STRING_255;
                case AttributeType.TIMESTAMP: return DataAttributeType.IEC61850_TIMESTAMP;
                case AttributeType.QUALITY: return DataAttributeType.IEC61850_QUALITY;
                case AttributeType.CHECK: return DataAttributeType.IEC61850_CHECK;
                case AttributeType.CODEDENUM: return DataAttributeType.IEC61850_CODEDENUM;
                case AttributeType.GENERIC_BITSTRING: return DataAttributeType.IEC61850_GENERIC_BITSTRING;
                case AttributeType.CONSTRUCTED: return DataAttributeType.IEC61850_CONSTRUCTED;
                case AttributeType.ENTRY_TIME: return DataAttributeType.IEC61850_ENTRY_TIME;
                case AttributeType.PHYCOMADDR: return DataAttributeType.IEC61850_PHYCOMADDR;
                case AttributeType.CURRENCY: return DataAttributeType.IEC61850_CURRENCY;
                case AttributeType.OTHER:
                default:
                    return DataAttributeType.IEC61850_UNKNOWN_TYPE;
            }
        }

        private static eFunctionalConstraint ConvertToFunctionalConstraint(SclFC sclFC)
        {
            switch (sclFC)
            {
                case SclFC.ST: return eFunctionalConstraint.IEC61850_FC_ST;
                case SclFC.MX: return eFunctionalConstraint.IEC61850_FC_MX;
                case SclFC.SP: return eFunctionalConstraint.IEC61850_FC_SP;
                case SclFC.SV: return eFunctionalConstraint.IEC61850_FC_SV;
                case SclFC.CF: return eFunctionalConstraint.IEC61850_FC_CF;
                case SclFC.DC: return eFunctionalConstraint.IEC61850_FC_DC;
                case SclFC.SG: return eFunctionalConstraint.IEC61850_FC_SG;
                case SclFC.SE: return eFunctionalConstraint.IEC61850_FC_SE;
                case SclFC.SR: return eFunctionalConstraint.IEC61850_FC_SR;
                case SclFC.OR: return eFunctionalConstraint.IEC61850_FC_OR;
                case SclFC.BL: return eFunctionalConstraint.IEC61850_FC_BL;
                case SclFC.EX: return eFunctionalConstraint.IEC61850_FC_EX;
                case SclFC.CO: return eFunctionalConstraint.IEC61850_FC_CO;
                case SclFC.US: return eFunctionalConstraint.IEC61850_FC_US;
                case SclFC.MS: return eFunctionalConstraint.IEC61850_FC_MS;
                case SclFC.RP: return eFunctionalConstraint.IEC61850_FC_RP;
                case SclFC.BR: return eFunctionalConstraint.IEC61850_FC_BR;
                case SclFC.LG: return eFunctionalConstraint.IEC61850_FC_LG;
                case SclFC.ALL: return eFunctionalConstraint.IEC61850_FC_ALL;
                case SclFC.NONE: return eFunctionalConstraint.IEC61850_FC_NONE;
                default:
                    throw new ArgumentOutOfRangeException(nameof(sclFC), $"Unhandled SclFC value: {sclFC}");
            }
        }

        private static SclTriggerOptions GetEffectiveTriggerOptions(DataAttribute da)
        {
            if (da.Parent is DataAttribute parentDA)
                return GetEffectiveTriggerOptions(parentDA);
            return da.Definition?.triggerOptions;
        }

        private string TrgOrtTotring(SclTriggerOptions trgOps)
        {
            string text = "0";

            if (trgOps != null)
            {
                if (trgOps.Dchg)
                    text += " + TRG_OPT_DATA_CHANGED";

                if (trgOps.Dupd)
                    text += " + TRG_OPT_DATA_UPDATE";

                if (trgOps.Qchg)
                    text += " + TRG_OPT_QUALITY_CHANGED";
            }

            if (isTransient)
                text += " + TRG_OPT_TRANSIENT";

            return text;
        }

        public override string ToString()
        {
            string daName = parent;

            //if (DataAttribute.Fc == SclFC.SE)
            //    daName += "_SE_" + name;
            //else
            daName += "_" + name;


            string cText = "DataAttribute " + objRef + " = {\n";
            cText += "  " + ModelNodeType + ",\n";
            cText += "  \"" + name + "\",\n";
            cText += "  (ModelNode*) &" + parent + ",\n";

            if (sibling == "NULL")
                cText += "  " + sibling + ",\n";
            else
                cText += "  (ModelNode*) &" + sibling + ",\n";



            if (child == "NULL")
                cText += "  " + child + ",\n";
            else
                cText += "  (ModelNode*) &" + child + ",\n";

            cText += "  " + elementCount + ",\n";
            cText += "  " + arrayIndex + ",\n";
            cText += "  " + ConvertToFunctionalConstraint(sclFC).ToString() + ",\n";
            cText += "  " + ConvertToDataAttributeType(DataAttribute.AttributeType).ToString() + ",\n";
            cText += "  " + TrgOrtTotring(GetEffectiveTriggerOptions(DataAttribute)) + ",\n";
            cText += "  " + mmsValue + ",\n";

            //TODO -> Shot address -> ataAttribute.getShortAddress()

            cText += "  " + sAddr.ToString() + "\n";

            cText += "};";

            return cText;
        }

    }

    public class C_DataObjectStructure : C_DO_DA_Structure
    {
        public string ModelNodeType = "DataObjectModelType";
        public string name;
        public string parent;
        public string sibling = "NULL";
        public string child = "NULL";
        public int elementCount = 0; /* value > 0 if this is an array */
        public int arrayIndex = -1; /* value > -1 when this is an array element */
        //public List<C_DO_DA_Structure> c_DO_DA_Structures = new List<C_DO_DA_Structure>();
        public DataObject DataObject;

        public C_DataObjectStructure() : base()
        {

        }

        public string ExternNameToString()
        {
            return "extern DataObject    " + objRef + ";";
        }

        public override string ToString()
        {
            string cText = "";
            if (arrayIndex == -1)
                cText = "DataObject " + parent + "_" + name + " = {\n";
            else
                cText = "DataObject " + parent + "_" + name + "_" + arrayIndex + " = {\n";
            //cText = "DataObject " + parent + "_" + name + " = {\n";
            cText += "  " + ModelNodeType + ",\n";
            cText += "  \"" + name + "\",\n";

            if (arrayIndex != -1)
                cText += "  (ModelNode*) &" + parent + "_" + name + ",\n";
            else
                cText += "  (ModelNode*) &" + parent + ",\n";

            if (sibling == "NULL")
                cText += "  " + sibling + ",\n";
            else
                cText += "  (ModelNode*) &" + parent + "_" + sibling + ",\n";

            if (child != "NULL")
                cText += "  (ModelNode*) &" + child + ",\n";
            else
                cText += "  NULL,\n";

            cText += "  " + elementCount + ",\n";

            if (elementCount == 0)
                cText += "  " + arrayIndex + "\n";
            else
                cText += "  0" + arrayIndex + "\n";
            cText += "};";

            return cText;
        }

    }

    public class C_LogicalNodeStructure
    {
        public string ModelNodeType = "LogicalNodeModelType";
        public string name;
        public string parent;
        public string sibling = "NULL";
        public string firstChild = "NULL";
        public string objRef;
        public List<C_DO_DA_Structure> c_DataObjectOrDataAttributeStructures = new List<C_DO_DA_Structure>();

        public C_LogicalNodeStructure()
        {

        }

        public string ExternNameToString()
        {
            return "extern LogicalNode   " + objRef + ";";
        }

        public override string ToString()
        {
            string cText = "LogicalNode " + parent + "_" + name + " = {\n";
            cText += "  " + ModelNodeType + ",\n";
            cText += "  \"" + name + "\",\n";
            cText += "  (ModelNode*) &" + parent + ",\n";
            if (sibling == "NULL")
                cText += "  " + sibling + ",\n";
            else
                cText += "  (ModelNode*) &" + parent + "_" + sibling + ",\n";

            if (firstChild == "NULL")
                cText += "  " + firstChild + ",\n";
            else
                cText += "  (ModelNode*) &" + parent + "_" + name + "_" + firstChild + ",\n";

            cText += "};";

            return cText;
        }

    }

    public class C_LogicalDeviceStructure
    {
        public string ModelNodeType = "LogicalDeviceModelType";
        public string name; /* LD instance */
        public string parent;
        public string sibling = "NULL";
        public string firstChild = "NULL";
        public string ldName = "NULL"; /* ldName (when using functional naming) */
        public List<C_LogicalNodeStructure> c_LogicalNodeStructures = new List<C_LogicalNodeStructure>();
        public string objRef;

        public C_LogicalDeviceStructure()
        {
        }

        public string ExternNameToString()
        {
            return "extern LogicalDevice " + objRef + ";";
        }

        public override string ToString()
        {
            string cText = "LogicalDevice " + objRef + " = {\n";
            cText += "  " + ModelNodeType + ",\n";
            cText += "  \"" + name + "\",\n";
            cText += "  (ModelNode*) &" + parent + ",\n";
            if (sibling == "NULL")
                cText += "  " + sibling + ",\n";
            else
                cText += "  (ModelNode*) &" + sibling + ",\n";

            if (firstChild == "NULL")
                cText += "  " + firstChild + ",\n";
            else
                cText += "  (ModelNode*) &" + parent + "_" + name + "_" + firstChild + ",\n";

            if (ldName == "NULL")
                cText += "  " + ldName + "\n";
            else
                cText += "  \"" + ldName + "\"\n";

            cText += "};";

            return cText;
        }
    }

    public class C_LogStructure
    {
        public Log log;
        public string externName;
        public string sibling = "NULL";
        public string parent;

        public C_LogStructure()
        {
        }

        public string ExternNameToString()
        {
            return "extern Log " + externName + ";";
        }

        public override string ToString()
        {
            string cText = "Log " + externName + " = {\n";
            cText += "  &" + parent + ",\n";

            if (log.Name != null)
                cText += "  \"" + log.Name + "\",\n";
            else
                cText += "  NULL,\n";

            if (sibling == "NULL")
                cText += "  " + sibling + "\n";
            else
                cText += "  &" + sibling + "\n";

            cText += "};";

            return cText;
        }
    }

    public class C_LogControlBlockStructure
    {
        public LogControl logControl;
        public string externName;
        public string sibling = "NULL";
        public string parent;
        public string ldInst;
        public bool hasOwner = false;
        public string lnClass;


        public C_LogControlBlockStructure()
        {
        }

        public string ExternNameToString()
        {
            return "extern LogControlBlock " + externName + ";";
        }

        string logRef()
        {
            string logRef;
            string lcbString = "  ";

            if (logControl.SclLogControl.LogName != null)
            {
                if (logControl.SclLogControl.LdInst == null)
                    logRef = ldInst + "/";
                else
                    logRef = logControl.SclLogControl.LdInst + "/";

                if (logControl.SclLogControl.LnClass != null)
                    logRef += logControl.SclLogControl.LnClass + "$";
                else
                    logRef += lnClass + "$";

                lcbString += "\"" + logRef + logControl.SclLogControl.LogName + "\",\n";
            }

            else
                lcbString += "  NULL,\n";

            return lcbString;
        }

        private int TrgOpsConvert()
        {
            int triggerOps = 0;

            if (logControl.SclLogControl.TrgOps != null)
                triggerOps = logControl.SclLogControl.TrgOps.GetIntValue();

            if (triggerOps >= 16)
                triggerOps = triggerOps - 16;

            return triggerOps;
        }

        public override string ToString()
        {
            string cText = "LogControlBlock " + externName + " = {\n";
            cText += "  &" + parent + ",\n";
            cText += "  \"" + logControl.Name + "\",\n";

            if (logControl.SclLogControl.DatSet != null)
                cText += "  \"" + logControl.SclLogControl.DatSet + "\",\n";
            else
                cText += "  NULL,\n";


            cText += logRef();

            cText += "  " + TrgOpsConvert() + ",\n";

            if (logControl.SclLogControl.IntgPd != null)
                cText += "  " + logControl.SclLogControl.IntgPd + ",\n";
            else
                cText += "  0,\n";


            cText += "  " + logControl.SclLogControl.LogEna.ToString().ToLower() + ",\n";
            cText += "  " + logControl.SclLogControl.ReasonCode.ToString().ToLower() + ",\n";


            if (sibling == "NULL")
                cText += "  " + sibling + "\n";
            else
                cText += "  &" + sibling + "\n";

            cText += "};";

            return cText;
        }
    }

    public class C_SettingGroupStructure
    {
        public SclSettingControl settingControl;
        public string parent;
        public string externName;

        public C_SettingGroupStructure()
        {

        }

        public string ExternNameToString()
        {
            return "extern SettingGroupControlBlock " + externName + ";";
        }

        public override string ToString()
        {
            string cText = "SettingGroupControlBlock " + externName + " = {\n";
            cText += "  &" + parent + ",\n";
            cText += "  " + settingControl.ActSG + ",\n";
            cText += "  " + settingControl.NumOfSGs + ",\n";
            cText += "  0,\n";
            cText += "  false,\n";
            cText += "  0,\n";
            cText += "  0,\n";
            cText += "  NULL,\n";
            cText += "};";

            return cText;
        }
    }

    public class C_GSEControlBlockStructure
    {
        public GSEControl GSEControl;
        public SclGSE SclGSE;
        public string lnPrefix;
        public string parent;
        public string rptId;
        public string externName;
        public int reportNumber = -1;
        public string index;
        public bool hasOwner = false;
        public string sibling = "NULL";
        public string phyComAddrName = "NULL";
        private string gseString = "";
        private string min = "-1";
        private string max = "-1";

        public C_GSEControlBlockStructure()
        {
        }

        public string ExternNameToString()
        {
            return "extern GSEControlBlock " + externName + ";";
        }

        private void LoadPhyComAddrName()
        {
            if (SclGSE != null)
            {
                if (SclGSE.Mintime != null)
                    if (SclGSE.Mintime != "")
                        min = SclGSE.Mintime;
                if (SclGSE.Maxtime != null)
                    if (SclGSE.Maxtime != "")
                        max = SclGSE.Maxtime;

                if (SclGSE.SclAddress != null)
                {
                    phyComAddrName = lnPrefix + "_gse" + reportNumber + "_address";

                    gseString += "\nstatic PhyComAddress " + phyComAddrName + " = {\n";
                    gseString += "  " + SclGSE.SclAddress.VlanPriority + ",\n";
                    gseString += "  " + Convert.ToInt32(SclGSE.SclAddress.VlanId, 16) + ",\n";
                    gseString += "  " + Convert.ToInt32(SclGSE.SclAddress.AppId, 16) + ",\n";
                    gseString += "  {";

                    for (int i = 0; i < 6; i++)
                    {
                        gseString += "0x" + (SclGSE.SclAddress.MacAddress[i]).ToString("x1");
                        if (i == 5)
                            gseString += "}\n";
                        else
                            gseString += ", ";
                    }

                    gseString += "};\n\n";
                }
            }
        }

        public override string ToString()
        {
            LoadPhyComAddrName();

            string cText = gseString;
            cText += "GSEControlBlock " + externName + " = {";
            cText += "&" + parent + ", ";
            cText += "\"" + GSEControl.Name + index + "\", ";

            if (GSEControl.SclGSEControl.AppID != null)
                cText += "\"" + GSEControl.SclGSEControl.AppID + "\", ";
            else
                cText += "NULL, ";

            if (GSEControl.SclGSEControl.DatSet != null)
                cText += "\"" + GSEControl.SclGSEControl.DatSet + "\", ";
            else
                cText += "NULL, ";

            cText += GSEControl.SclGSEControl.ConfRev + ", ";
            cText += GSEControl.SclGSEControl.FixedOffs.ToString().ToLower() + ", ";

            if (phyComAddrName == "NULL")
                cText += phyComAddrName + ", ";
            else
                cText += "&" + phyComAddrName + ", ";

            cText += min + ", ";
            cText += max + ", ";

            if (sibling == "NULL")
                cText += sibling;
            else
                cText += "&" + sibling;

            cText += "};";

            return cText;
        }
    }

    public class C_SMVControlBlockStructure
    {
        public SMVControl SMVControl;
        public SclSMV SclSMV;
        public string lnPrefix;
        public string parent;
        public string externName;
        public int reportNumber = -1;
        public string index;
        public bool hasOwner = false;
        public string sibling = "NULL";
        public string phyComAddrName = "NULL";
        private string smvString = "";


        public C_SMVControlBlockStructure()
        {
        }

        public string ExternNameToString()
        {
            return "extern SVControlBlock " + externName + ";";
        }

        private void LoadPhyComAddrName()
        {
            if (SclSMV != null)
            {
                if (SclSMV.SclAddress != null)
                {
                    phyComAddrName = lnPrefix + "_smv" + reportNumber + "_address";

                    smvString += "\nstatic PhyComAddress " + phyComAddrName + " = {\n";
                    smvString += "  " + SclSMV.SclAddress.VlanPriority + ",\n";
                    smvString += "  " + Convert.ToInt32(SclSMV.SclAddress.VlanId, 16) + ",\n";
                    smvString += "  " + Convert.ToInt32(SclSMV.SclAddress.AppId, 16) + ",\n";
                    smvString += "  {";

                    for (int i = 0; i < 6; i++)
                    {
                        smvString += "0x" + (SclSMV.SclAddress.MacAddress[i]).ToString("x1");
                        if (i == 5)
                            smvString += "}\n";
                        else
                            smvString += ", ";
                    }

                    smvString += "};\n\n";
                }
            }
        }

        public override string ToString()
        {
            LoadPhyComAddrName();

            string cText = smvString;
            cText += "SVControlBlock " + externName + " = {\n";
            cText += "  &" + parent + ",\n";

            if (SMVControl.SclSMVControl.Name != null)
                cText += "  \"" + SMVControl.Name + index + "\",\n";
            else
                cText += "  NULL,\n";

            //if (SMVControl.SclSMVControl.Desc!= null)
            //    cText += "  \"" + SclSMV.SclAddress.AppId + "\",\n";9
            //else
            //    cText += "  NULL,\n";

            if (SMVControl.SclSMVControl.SmvID != null)
                cText += "  \"" + SMVControl.SclSMVControl.SmvID + "\",\n";
            else
                cText += "  NULL,\n";

            if (SMVControl.SclSMVControl.DataSet != null)
                cText += "  \"" + SMVControl.SclSMVControl.DataSet + "\",\n";
            else
                cText += "  NULL,\n";

            if (SMVControl.SclSMVControl.SclSmvOpts != null)
            {
                cText += "  " + SMVControl.SclSMVControl.SclSmvOpts.GetIntValue() + ",\n";
            }
            else
            {
                cText += "  0,\n";
            }

            if (SMVControl.SclSMVControl.SmpMod != null)
                cText += "  " + SMVControl.SclSMVControl.SmpMod + ",\n";
            else
                cText += "  NULL,\n";

            cText += "  " + SMVControl.SclSMVControl.SmpRate + ",\n";

            cText += "  " + SMVControl.SclSMVControl.ConfRev + ",\n";

            if (phyComAddrName == "NULL")
                cText += "  " + phyComAddrName + ",\n";
            else
                cText += "  &" + phyComAddrName + ",\n";


            cText += "  " + SMVControl.SclSMVControl.Multicast.ToString().ToLower() + ",\n";

            cText += "  " + SMVControl.SclSMVControl.NofASDU + ",\n";

            //if(SMVControl.SclSMVControl.SecurityEnabled != null)
            //    cText += "  " + SMVControl.SclSMVControl.SecurityEnabled + ",\n";

            if (sibling == "NULL")
                cText += "  " + sibling + "\n";
            else
                cText += "  &" + sibling + "\n";

            cText += "};";

            return cText;
        }
    }
    public class C_ReportControlBlockStructure
    {
        public ReportControl ReportControl;
        public string parent;
        public string rptId;
        public string externName;
        public byte[] clientIpAddr;
        public int reportNumber = -1;
        public int reportsCount = -1;
        public string index;
        public bool hasOwner = false;


        /* type (first byte) and address of the pre-configured client
           type can be one of (0 - no reservation, 4 - IPv4 client, 6 - IPv6 client) */
        //public int clientReservation[17];

        public string sibling = "NULL";

        public C_ReportControlBlockStructure()
        {
        }

        public string ExternNameToString()
        {
            return "extern ReportControlBlock " + externName + ";";
        }

        private int TrgOpsConvert(SclTrgOps sclTriggerOptions)
        {
            int triggerOps = 16;

            if (sclTriggerOptions != null)
                triggerOps = sclTriggerOptions.GetIntValue();

            if (hasOwner)
                triggerOps += 64;

            return triggerOps;
        }

        private int OptFieldsConvert(SclOptFields sclOptFields)
        {
            int options = 0;

            if (sclOptFields != null)
            {
                if (sclOptFields.SeqNum)
                    options += 1;
                if (sclOptFields.TimeStamp)
                    options += 2;
                if (sclOptFields.ReasonCode)
                    options += 4;
                if (sclOptFields.DataSet)
                    options += 8;
                if (sclOptFields.DataRef)
                    options += 16;
                if (sclOptFields.BufOvfl)
                    options += 32;
                if (sclOptFields.EntryID)
                    options += 64;
                if (sclOptFields.ConfigRef)
                    options += 128;
            }
            else
                options = 32;

            return options;

        }

        private string clientIpAddrConvert()
        {
            string value = "{";
            for (int i = 0; i < 17; i++)
            {
                value += "0x" + (clientIpAddr[i] & 0xff).ToString("x1");
                if (i == 16)
                    value += "}, ";
                else
                    value += ", ";
            }

            return value;
        }

        public override string ToString()
        {
            string cText = "ReportControlBlock " + externName + " = {";
            cText += "&" + parent + ", ";
            cText += "\"" + ReportControl.Name + index + "\", ";

            if (ReportControl.SclReportControl.RptID != null)
                cText += "\"" + ReportControl.SclReportControl.RptID + "\", ";
            else
                cText += "NULL, ";

            cText += ReportControl.SclReportControl.Buffered.ToString().ToLower() + ", ";

            if (ReportControl.SclReportControl.DatSet != null)
                cText += "\"" + ReportControl.SclReportControl.DatSet + "\", ";
            else
                cText += "NULL, ";

            if (ReportControl.SclReportControl.ConfRev != null)
                cText += ReportControl.SclReportControl.ConfRev + ", ";
            else
                cText += "0, ";

            cText += TrgOpsConvert(ReportControl.SclReportControl.TrgOps) + ", ";
            cText += OptFieldsConvert(ReportControl.SclReportControl.OptFields) + ", ";

            if (ReportControl.SclReportControl.BufTime != null)
                cText += ReportControl.SclReportControl.BufTime + ", ";
            else
                cText += "NULL, ";

            if (ReportControl.SclReportControl.IntgPd != null)
                cText += ReportControl.SclReportControl.IntgPd + ", ";
            else
                cText += "0, ";

            cText += clientIpAddrConvert();

            if (sibling == "NULL")
                cText += sibling;
            else
                cText += "&" + sibling;

            cText += "};";

            return cText;
        }
    }

    public class C_DataSetStructure
    {
        public DataSet DataSet { get; set; }
        public string externDataSetName { get; set; }
        public List<C_DatasetEntry> externDataSetEntries { get; set; }
        public string logicalDeviceName; /* logical device instance name */
        public string name; /* eg. MMXU1$dataset1 */
        public int elementCount = 0;
        public string fcdas = "NULL";
        public string sibling = "NULL";

        public C_DataSetStructure()
        {
            externDataSetEntries = new List<C_DatasetEntry>();
        }

        public string ExternToString()
        {
            return "extern DataSet " + externDataSetName + ";";
        }

        public override string ToString()
        {
            string cText = "DataSet " + externDataSetName + " = {\n";
            cText += "  \"" + logicalDeviceName + "\",\n";
            cText += "  \"" + name + "\",\n";
            cText += "  " + elementCount.ToString() + ",\n";

            if (fcdas == "NULL")
                cText += "  " + fcdas + ",\n";
            else
                cText += "  &" + fcdas + ",\n";

            if (sibling == "NULL")
                cText += "  " + sibling + "\n";
            else
                cText += "  &" + sibling + "\n";

            cText += "};";

            return cText;

        }

    }

    public class C_IEDModelStructure
    {
        public string name;
        public string modelPrefix;
        public string firstChild = "NULL";
        public string dataSets = "NULL";
        public string rcbs = "NULL";
        public string gseCBs = "NULL";
        public string svCBs = "NULL";
        public string sgcbs = "NULL";
        public string lcbs = "NULL";
        public string logs = "NULL";

        public C_IEDModelStructure()
        {
        }

        public override string ToString()
        {
            string cText = "IedModel " + modelPrefix + " = {\n";

            cText += "  \"" + name + "\",\n";

            if (firstChild == "NULL")
                cText += "  " + firstChild + ",\n";
            else
                cText += "  &" + firstChild + ",\n";

            if (dataSets == "NULL")
                cText += "  " + dataSets + ",\n";
            else
                cText += "  &" + dataSets + ",\n";

            if (rcbs == "NULL")
                cText += "  " + rcbs + ",\n";
            else
                cText += "  &" + rcbs + ",\n";

            if (gseCBs == "NULL")
                cText += "  " + gseCBs + ",\n";
            else
                cText += "  &" + gseCBs + ",\n";

            if (svCBs == "NULL")
                cText += "  " + svCBs + ",\n";
            else
                cText += "  &" + svCBs + ",\n";

            if (sgcbs == "NULL")
                cText += "  " + sgcbs + ",\n";
            else
                cText += "  &" + sgcbs + ",\n";

            if (lcbs == "NULL")
                cText += "  " + lcbs + ",\n";
            else
                cText += "  &" + lcbs + ",\n";

            if (logs == "NULL")
                cText += "  " + logs + ",\n";
            else
                cText += "  &" + logs + ",\n";

            cText += "  initializeValues\n";

            cText += "};";


            return cText;

        }
    }

    public class C_DatasetEntry
    {
        public string externDataSetName = "";
        public string logicalDeviceName = "";
        public bool isLDNameDynamicallyAllocated;
        public string variableName = "";
        public int index = -1;
        public string componentName = "NULL";
        public string value = "NULL";
        public string sibling = "NULL";

        public C_DatasetEntry()
        { }

        public string externDataSetNameToString()
        {
            return "extern DataSetEntry " + externDataSetName + ";";
        }

        public override string ToString()
        {
            string cText = "DataSetEntry " + externDataSetName + " = {\n";
            cText += "  \"" + logicalDeviceName + "\",\n";
            cText += "  " + isLDNameDynamicallyAllocated.ToString().ToLower() + ",\n";
            cText += "  \"" + variableName + "\",\n";
            cText += "  " + index.ToString() + ",\n";
            if (componentName != "NULL")
                cText += "  " + componentName + ",\n";
            else
                cText += "  " + componentName + ",\n";

            cText += "  " + value + ",\n";
            if (sibling == "NULL")
                cText += "  " + sibling + "\n";
            else
                cText += "  &" + sibling + "\n";

            cText += "};";

            return cText;
        }
    }

}
