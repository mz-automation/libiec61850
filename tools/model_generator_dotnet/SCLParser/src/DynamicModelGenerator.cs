/*
 *  Copyright 2013-2025 Michael Zillgith, MZ Automation GmbH
 *
 *  This file is part of MZ Automation IEC 61850 SDK
 * 
 *  All rights reserved.
 */

using IEC61850.SCL.DataModel;
using System;
using System.Collections.Generic;
using System.IO;
using System.Runtime.Serialization;

namespace IEC61850.SCL
{
    public class DynamicModelGenerator
    {
        private SclConnectedAP connectedAP = null;
        private SclIED ied = null;
        private bool hasOwner = false;
        private IEDDataModel iED = null;
        private SclDocument sclDocument;

        public DynamicModelGenerator(SclDocument sclDocument, StreamWriter output, IEDDataModel iED, SclAccessPoint accessPoint)
        {
            this.sclDocument = sclDocument;
            this.iED = iED;

            connectedAP = sclDocument.GetConnectedAP(accessPoint.Name, iED.Name);

            output.WriteLine("MODEL(" + iED.Name + "){");
            foreach (LogicalDevice ld in iED.LogicalDevices)
            {
                output.Write("LD(" + ld.Inst + "){\n");

                ExportLogicalNodes(output, ld);

                output.WriteLine("}");
            }
            output.WriteLine("}");
        }

        private void ExportLogicalNodes(StreamWriter output, LogicalDevice logicalDevice)
        {
            foreach (LogicalNode logicalNode in logicalDevice.LogicalNodes)
            {
                output.Write("LN(" + logicalNode.Name + "){\n");

                ExportLogicalNode(output, logicalNode, logicalDevice);

                output.WriteLine("}");
            }
        }

        private void ExportLogicalNode(StreamWriter output, LogicalNode logicalNode, LogicalDevice logicalDevice)
        {
            if (logicalNode.SettingControl != null)
                output.Write("SG(" + logicalNode.SettingControl.ActSG + " " + logicalNode.SettingControl.NumOfSGs + ")\n");


            foreach (DataObject dataObject in logicalNode.DataObjects)
            {
                output.Write("DO(" + dataObject.Name + " " + dataObject.Count + "){\n");

                ExportDataObject(output, dataObject, false);

                output.WriteLine("}");
            }

            foreach (DataSet dataSet in logicalNode.DataSets)
                ExportDataSet(output, dataSet, logicalNode);

            foreach (ReportControl rcb in logicalNode.ReportControlBlocks)
            {
                if (rcb.SclReportControl.Indexed)
                {

                    int maxInstances = 1;

                    if (rcb.SclReportControl.RptEna != null)
                        maxInstances = rcb.SclReportControl.RptEna.Max;

                    for (int i = 0; i < maxInstances; i++)
                    {
                        string index = (i + 1).ToString("00");
                        PrintRCBInstance(output, rcb, index);
                    }
                }
                else
                    PrintRCBInstance(output, rcb, "");
            }

            foreach (LogControl lcb in logicalNode.LogControls)
                PrintLCB(output, lcb, logicalNode, logicalDevice);

            foreach (Log log in logicalNode.Logs)
                output.WriteLine("LOG(" + log.Name + ");");

            foreach (GSEControl gcb in logicalNode.GSEControls)
            {
                PrintGSEControl(output, gcb, logicalDevice);
            }

            foreach (SMVControl smv in logicalNode.SMVControls)
            {
                PrintSMVControl(output, smv, logicalDevice);
            }
        }

        private void PrintGSEControl(StreamWriter output, GSEControl gcb, LogicalDevice ld)
        {
            SclGSE gse = null;
            SclAddress gseAddress = null;

            if (connectedAP != null)
            {
                gse = connectedAP.GSEs.Find(x => x.CbName == gcb.Name);
                if (gse == null)
                {
                    bool found = false;
                    foreach (SclConnectedAP ap in sclDocument.GetConnectedAPs())
                    {
                        foreach (SclGSE sclGSE in ap.GSEs)
                        {
                            if (sclGSE.CbName == gcb.Name && sclGSE.LdInst == ld.Inst)
                            {
                                gse = sclGSE;
                                found = true;
                                break;
                            }
                        }
                        if (found)
                            break;
                    }
                }
                if (gse != null)
                    gseAddress = gse.SclAddress;
            }
            else
                Console.WriteLine("WARNING: IED \"" + ied.Name + "\" has no connected access point!");

            output.Write("GC(");
            output.Write(gcb.Name + " ");

            if (gcb.SclGSEControl.AppID != null)
                output.Write(gcb.SclGSEControl.AppID + " ");
            else
                output.Write("null ");


            if (gcb.SclGSEControl.DatSet != null)
                output.Write(gcb.SclGSEControl.DatSet + " ");
            else
                output.Write("null ");

            if (gcb.SclGSEControl.ConfRev >= 0)
                output.Write(gcb.SclGSEControl.ConfRev + " ");
            else
                output.Write("0 ");

            if (gcb.SclGSEControl.FixedOffs)
                output.Write('1');
            else
                output.Write('0');

            output.Write(' ');

            if (gse != null)
            {
                if (gse.Mintime != null)
                    output.Write(gse.Mintime);
                else
                    output.Write("0");

                output.Write(' ');

                if (gse.Maxtime != null)
                    output.Write(gse.Maxtime);
                else
                    output.Write("0");
            }
            else
            {
                output.Write("0 0");
            }

            if (gseAddress == null)
            {
                output.WriteLine(");");
            }
            else
            {
                int appID_decimal = Convert.ToInt32(gseAddress.AppId, 16);
                int vlanID_decimal = Convert.ToInt32(gseAddress.VlanId, 16);

                output.WriteLine("){");

                output.Write("PA(");
                output.Write(gseAddress.VlanPriority + " ");
                output.Write(vlanID_decimal.ToString() + " ");
                output.Write(appID_decimal.ToString() + " ");

                for (int i = 0; i < 6; i++)
                {
                    string hexValue = gseAddress.MacAddress[i].ToString("X2");
                    output.Write(hexValue);
                }

                output.WriteLine(");");

                output.WriteLine("}");
            }
        }

        private void PrintSMVControl(StreamWriter output, SMVControl smv, LogicalDevice ld)
        {
            SclSMV sclsmv = null;
            SclAddress smvAddress = null;

            if (connectedAP != null)
            {
                sclsmv = connectedAP.SMVs.Find(x => x.CbName == smv.Name);
                if(sclsmv == null)
                {
                    bool found = false;
                    foreach (SclConnectedAP ap in sclDocument.GetConnectedAPs())
                    {
                        foreach(SclSMV sclSMV in ap.SMVs)
                        {
                            if(sclSMV.CbName == smv.Name && sclSMV.LdInst == ld.Inst)
                            {
                                sclsmv = sclSMV;
                                found = true;
                                break;
                            }
                        }
                        if (found)
                            break;  

                    }
                }

                if (sclsmv != null)
                    smvAddress = sclsmv.SclAddress;
            }

            output.Write("SMVC(");
            output.Write(smv.Name + " ");
            if (smv.SclSMVControl.SmvID != null)
                output.Write(smv.SclSMVControl.SmvID + " ");
            else
                output.Write("null ");

            if (smv.SclSMVControl.DataSet != null)
                output.Write(smv.SclSMVControl.DataSet + " ");
            else
                output.Write("null ");

            if (smv.SclSMVControl.ConfRev >= 0)
                output.Write(smv.SclSMVControl.ConfRev + " ");
            else
                output.Write("0 ");

            if (smv.SclSMVControl.SmpMod != null)
            {
                int smp_val = -1;
                if (smv.SclSMVControl.SmpMod == "SmpPerPeriod")
                    smp_val = 0;
                else if (smv.SclSMVControl.SmpMod == "SmpPerSec")
                    smp_val = 1;
                else if (smv.SclSMVControl.SmpMod == "SmpPerSamp")
                    smp_val = 2;
                output.Write(smp_val + " ");
            }
            else
                output.Write("0 ");

            if (smv.SclSMVControl.SmpRate != -1)
                output.Write(smv.SclSMVControl.SmpRate + " ");
            else
                output.Write("0 ");

            if (smv.SclSMVControl.SclSmvOpts != null)
                output.Write(smv.SclSMVControl.SclSmvOpts.GetIntValue());
            else
                output.Write("0");

            output.Write(' ');

            if (smv.SclSMVControl.Multicast)
                output.Write('1');
            else
                output.Write("0");
            output.Write(' ');


            if (smvAddress == null)
            {
                output.WriteLine(");");
            }
            else
            {
                int appID_decimal = Convert.ToInt32(smvAddress.AppId, 16);
                int vlanID_decimal = Convert.ToInt32(smvAddress.VlanId, 16);
                output.WriteLine("){");

                output.Write("PA(");
                output.Write(smvAddress.VlanPriority + " ");
                output.Write(vlanID_decimal + " ");
                output.Write(appID_decimal + " ");

                for (int i = 0; i < 6; i++)
                {
                    string hexValue = smvAddress.MacAddress[i].ToString("X2");
                    output.Write(hexValue);
                }

                output.WriteLine(");");

                output.WriteLine("}");
            }
        }

        private void PrintLCB(StreamWriter output, LogControl lcb, LogicalNode ln, LogicalDevice logicalDevice)
        {
            output.Write("LC(");
            output.Write(lcb.Name + " ");

            if (lcb.SclLogControl.DatSet != null && lcb.SclLogControl.DatSet != "")
                output.Write(lcb.SclLogControl.DatSet + " ");
            else
                output.Write("- ");

            if (lcb.SclLogControl.LogName != null && lcb.SclLogControl.LogName != "")
            {
                String logRef = logicalDevice.Inst + "/" + ln.Name + "$" + lcb.SclLogControl.LogName;
                output.Write(logRef + " ");
            }
            else
                output.Write("- ");

            if (lcb.SclLogControl.TrgOps != null)
            {
                output.Write(lcb.SclLogControl.TrgOps.GetIntValue() + " ");
            }


            if (lcb.SclLogControl.IntgPd is null)
                output.Write("0 ");
            else
                output.Write(lcb.SclLogControl.IntgPd + " ");


            if (lcb.SclLogControl.LogEna)
                output.Write("1 ");
            else
                output.Write("0 ");

            if (lcb.SclLogControl.ReasonCode)
                output.WriteLine("1);");
            else
                output.WriteLine("0);");
        }

        private void PrintRCBInstance(StreamWriter output, ReportControl rcb, string index)
        {
            output.Write("RC(");
            output.Write(rcb.Name + index + " ");

            if (rcb.SclReportControl.RptID != null)
                output.Write(rcb.SclReportControl.RptID + " ");
            else
                output.Write("- ");

            if (rcb.SclReportControl.Buffered)
                output.Write("1 ");
            else
                output.Write("0 ");

            if (rcb.SclReportControl.DatSet != null)
                output.Write(rcb.SclReportControl.DatSet + " ");
            else
                output.Write("- ");

            if (rcb.SclReportControl.IntConfRev >= 0)
                output.Write(rcb.SclReportControl.IntConfRev + " ");
            else
                output.Write("0 ");

            int triggerOptions = 0;

            if (rcb.SclReportControl.TrgOps != null)
            {
                triggerOptions = rcb.SclReportControl.TrgOps.GetIntValue();
            }

            if (hasOwner)
                triggerOptions += 64;

            output.Write(triggerOptions + " ");

            int OptFields = 0;
            if (rcb.SclReportControl.OptFields != null)
            {
                OptFields = rcb.SclReportControl.OptFields.GetIntValue();
            }
            output.Write(OptFields + " ");


            if (rcb.SclReportControl.BufTime != null)
                output.Write(rcb.SclReportControl.BufTime + " ");
            else
                output.Write("0 ");


            if (rcb.SclReportControl.IntgPd != null)
                output.Write(rcb.SclReportControl.IntgPd);
            else
                output.Write("0");


            output.WriteLine(");");
        }

        private void exportDataObjectChild(StreamWriter output, DataObject dataObject, bool isTransient)
        {

            foreach (DataObjectOrAttribute child in dataObject.DataObjectsAndAttributes)
            {
                if (child is DataObject)
                {
                    DataObject subDataObject = child as DataObject;
                    output.Write("DO(" + subDataObject.Name + " " + subDataObject.Count + "){\n");

                    ExportDataObject(output, subDataObject, isTransient);

                    output.WriteLine("}");
                }
                else
                {
                    DataAttribute dataAttribute = child as DataAttribute;
                    ExportDataAttribute(output, dataAttribute, isTransient);
                }
            }


        }

        private void ExportDataObject(StreamWriter output, DataObject dataObject, bool isTransient)
        {

            if (dataObject.IsTransiente)
                isTransient = true;

            if (dataObject.Count > 0)
            {
                /* data object is an array */
                for (int i = 0; i < dataObject.Count; i++)
                {
                    output.WriteLine("[" + i + "]{\n");

                    exportDataObjectChild(output, dataObject, isTransient);

                    output.Write("}\n");
                }
            }
            else
            {
                exportDataObjectChild(output, dataObject, isTransient);

            }

        }

        SclDAI getDAI(object parent, string name)
        {
            if (parent == null)
                return null;

            if (parent is SclDOI sclDOI)
                return sclDOI.SclDAIs.Find(x => x.Name == name);
            else if (parent is SclSDI sclSDI)
                return sclSDI.SclDAIs.Find(x => x.Name == name);
            else
                return null;
        }

        SclDAI getNestedDAI(SclDOI initialDO, string name)
        {
            string[] parts = name.Split('.');

            object obj = null;
            Object foundObject = initialDO;
            SclDAI sclDAI = null;
            for( int i = 1; i < parts.Length; i++)
            {
                if (i == 1)
                    foundObject = initialDO.SclSDIs.Find(x => x.Name == parts[1]);
                else if (i == parts.Length - 1)
                { 
                    SclSDI sclSDI = foundObject as SclSDI;  
                     sclDAI = sclSDI?.SclDAIs.Find(x => x.Name == parts[i]);

                }
                else
                {
                    if (foundObject is SclSDI sclSDI)
                        foundObject = sclSDI.SclSDIs.Find(x => x.Name == parts[i]);
                }

            }
            return sclDAI;  
        }

        DataObject findDOParent(DataAttribute dataAttribute)
        {
            DataObject parentObject = null;

            while (!(dataAttribute.Parent is DataObject))
            {
                dataAttribute = dataAttribute.Parent as DataAttribute;
            }
            parentObject = dataAttribute.Parent as DataObject;

           while((parentObject.Parent is LogicalNode) == false)
            {
                parentObject = parentObject.Parent as DataObject;
            }

            return parentObject;


        }

        LogicalNode findLNParent(DataObject dataObject)
        {
            LogicalNode parentObject = null;

            while (!(dataObject.Parent is LogicalNode))
            {
                dataObject = dataObject.Parent as DataObject;
            }
            parentObject = dataObject.Parent as LogicalNode;


            return parentObject;


        }


        string getStippedObjRef(string objRef)
        {
            string result = "";

            int index = objRef.IndexOf('.');

            if (index >= 0 && index < objRef.Length - 1)
            {
                result = objRef.Substring(index + 1);
            }

            return result;
        
        }


        void printDataAttributes(StreamWriter output, DataAttribute dataAttribute, bool isTransient)
        {
            if (dataAttribute.AttributeType != AttributeType.CONSTRUCTED)
            {

                DataObject dataObject = findDOParent(dataAttribute);
                LogicalNode logicalNode = findLNParent(dataObject);
                LogicalDevice logicalDevice = logicalNode.Parent as LogicalDevice;

                string value = null;

                SclDataAttributeDefinition definition = dataAttribute.Definition;
                List<SclVal> predefined_vals = definition.GetValues();
                if (predefined_vals != null)
                {
                    value = predefined_vals[0].Value;
                }



                SclDOI sclDOI = logicalNode.SclElement.DOIs.Find(x => x.Name == dataObject.Name);

                if (sclDOI !=null)
                {
                    SclDAI sclDAI = sclDOI.SclDAIs.Find(x => x.Name == dataAttribute.Name);
                    if (sclDAI != null && dataAttribute.ObjRef == logicalDevice.Name + "/" + logicalNode.Name + "." + sclDOI.Name + "." + sclDAI.Name)
                    {
                        value = sclDAI.Val;
                    }

                    else
                    {
                        string strippedObjRef = getStippedObjRef(dataAttribute.ObjRef);
                        sclDAI = getNestedDAI(sclDOI, strippedObjRef);
                        value = sclDAI?.Val;
                    }
                }
                
                if (value != null)
                {
                    switch (dataAttribute.AttributeType)
                    {
                        case AttributeType.ENUMERATED:
                            SclEnumType sclEnumType = sclDocument.DataTypeTemplates.GetEnumType(dataAttribute.Definition.Type);

                            if (sclEnumType != null)
                            {
                                if (sclEnumType.EnumValues.Count > 0)
                                {
                                    SclEnumVal sclEnumVal = sclEnumType.EnumValues.Find(x => x.SymbolicName == value);
                                    int value1 = sclEnumVal.Ord;
                                    output.Write("=" + sclEnumVal.Ord);

                                }

                            }

                            break;

                        case AttributeType.INT8:
                        case AttributeType.INT16:
                        case AttributeType.INT32:
                        case AttributeType.INT64:
                        case AttributeType.INT8U:
                        case AttributeType.INT16U:
                        case AttributeType.INT24U:
                        case AttributeType.INT32U:
                            output.Write("=" + value);
                            break;

                        case AttributeType.BOOLEAN:
                            if (value == "true")
                                output.Write("=1");
                            else
                                output.Write("=0");
                            break;

                        case AttributeType.UNICODE_STRING_255:
                            output.Write("=\"" + value + "\"");
                            break;

                        case AttributeType.CURRENCY:
                        case AttributeType.VISIBLE_STRING_32:
                        case AttributeType.VISIBLE_STRING_64:
                        case AttributeType.VISIBLE_STRING_129:
                        case AttributeType.VISIBLE_STRING_255:
                        case AttributeType.VISIBLE_STRING_65:
                            output.Write("=\"" + value + "\"");
                            break;

                        case AttributeType.OCTET_STRING_64:
                            output.Write("=\"" + value + "\"");
                            break;

                        case AttributeType.FLOAT32:
                        case AttributeType.FLOAT64:
                            output.Write("=" + value);
                            break;

                        default:
                            Console.WriteLine("Unknown default value for " + dataAttribute.Name + " type: " + dataAttribute.AttributeType);
                            break;

                    }

                }

                output.WriteLine(";");

            }

            else
            {
                output.WriteLine("{");

                foreach (DataAttribute subDataAttribute in dataAttribute.SubDataAttributes)
                {
                    ExportDataAttribute(output, subDataAttribute, isTransient);
                }

                output.WriteLine("}");
            }

        }

        private void ExportDataAttribute(StreamWriter output, DataAttribute dataAttribute, bool isTransient)
        {
            output.Write("DA(" + dataAttribute.Name + " ");
            output.Write(dataAttribute.Count + " ");
            output.Write((int)dataAttribute.AttributeType + " ");
            output.Write((int)dataAttribute.Fc + " ");

            if (dataAttribute.Definition.TriggerOptions != null)
            {
                int trgOpsVal = dataAttribute.Definition.TriggerOptions.GetIntValue();

                if (isTransient)
                    trgOpsVal += 128;

                output.Write(trgOpsVal + " ");
            }


            if (dataAttribute.Definition.SAddr != null)
                output.Write(dataAttribute.Definition.SAddr);
            else
                output.Write("0");

            output.Write(")");

            if (dataAttribute.Count > 0)
            {
                output.WriteLine("{");

                for (int i = 0; i < dataAttribute.Count; i++)
                {
                    output.Write("[" + i + "]");

                    printDataAttributes(output, dataAttribute, isTransient);
                }

                output.WriteLine("}");
            }

            else
            {

                printDataAttributes(output, dataAttribute, isTransient);
            }
        }

        private static String toMmsString(String iecString)
        {
            return iecString.Replace('.', '$');
        }

        private void ExportDataSet(StreamWriter output, DataSet dataSet, LogicalNode logicalNode)
        {
            output.Write("DS(" + dataSet.Name + "){\n");

            foreach (SclFCDA fcda in dataSet.SclDataSet.Fcdas)
            {
                String mmsVariableName = "";

                if (fcda.Prefix != null)
                    mmsVariableName += fcda.Prefix;

                mmsVariableName += fcda.LnClass;

                if (fcda.LnInst != null)
                    mmsVariableName += fcda.LnInst;

                mmsVariableName += "$" + fcda.Fc;

                mmsVariableName += "$" + fcda.DoName;

                if (fcda.DaName != null)
                    mmsVariableName += "$" + toMmsString(fcda.DaName);

                int arrayStart = mmsVariableName.IndexOf('(');

                String variableName = mmsVariableName;
                int arrayIndex = -1;
                string componentName = null;


                if (arrayStart != -1)
                {
                    variableName = mmsVariableName.Substring(0, arrayStart);
                    int arrayEnd = mmsVariableName.IndexOf(')');

                    string arrayIndexStr = mmsVariableName.Substring(arrayStart + 1, arrayEnd);
                    arrayIndex = int.Parse(arrayIndexStr);

                    string componentNamePart = mmsVariableName.Substring(arrayEnd + 1);

                    if ((componentNamePart != null) && (componentNamePart.Length > 0))
                    {
                        if (componentNamePart[0] == '$')
                        {
                            componentNamePart = componentNamePart.Substring(1);
                        }

                        if ((componentNamePart != null) && (componentNamePart.Length > 0))
                            componentName = componentNamePart;
                    }
                }

                /* check for LD name */

                String logicalDeviceName = null;

                if (fcda.LdInst != null)
                {

                    if (fcda.LdInst != (logicalNode.Parent as LogicalDevice).Inst)
                    {
                        logicalDeviceName = fcda.LdInst;
                    }
                }

                if (logicalDeviceName != null)
                    variableName = logicalDeviceName + "/" + variableName;

                if (variableName != null && arrayIndex != -1 && componentName != null)
                {
                    output.Write("DE(" + variableName + " " + arrayIndex + " " + componentName + ");\n");
                }
                else if (variableName != null && arrayIndex != -1)
                {
                    output.Write("DE(" + variableName + " " + arrayIndex + ");\n");
                }
                else if (variableName != null)
                {
                    output.Write("DE(" + variableName + ");\n");
                }

            }

            output.WriteLine("}");

        }
    }
}
