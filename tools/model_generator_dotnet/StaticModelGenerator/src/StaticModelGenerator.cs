/*
 *  Copyright 2013-2025 Michael Zillgith, MZ Automation GmbH
 *
 *  This file is part of MZ Automation IEC 61850 SDK
 * 
 *  All rights reserved.
 */

using IEC61850.SCL;
using IEC61850.SCL.DataModel;
using StaticModelGenerator.C_Structures;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Xml.Linq;
using DataSet = IEC61850.SCL.DataModel.DataSet;

namespace StaticModelGenerator
{

    public class StaticModelGenerator
    {
        List<C_DataSetStructure> c_DataSetStructures = new List<C_DataSetStructure>();
        List<C_LogicalDeviceStructure> c_LogicalDeviceStructures = new List<C_LogicalDeviceStructure>();
        List<C_ReportControlBlockStructure> c_ReportContorlBlockStructures = new List<C_ReportControlBlockStructure>();
        List<C_GSEControlBlockStructure> c_GSEControlBlockStructures = new List<C_GSEControlBlockStructure>();
        List<C_SMVControlBlockStructure> c_SMVControlBlockStructures = new List<C_SMVControlBlockStructure>();
        C_SettingGroupStructure c_SettingGroupControlStructure = null;
        List<C_LogControlBlockStructure> c_LogControlBlockStructures = new List<C_LogControlBlockStructure>();
        List<C_LogStructure> c_LogStructures = new List<C_LogStructure>();
        C_IEDModelStructure c_IEDModelStructure = new C_IEDModelStructure();
        List<C_InitializeValues> c_InitializeValues = new List<C_InitializeValues>();
        List<C_DO_DA_Structure> c_DO_DA_Structures = new List<C_DO_DA_Structure>();
        SclDocument sclParser;
        bool initializeOnce = false;
        bool hasOwner = false;
        SclConnectedAP connectedAP = null;
        string modelPrefix;


        public StaticModelGenerator(string fileName, string icdFile, FileStream cOut, FileStream hOut,
            string outputFileName, string iedName, string accessPointName, string modelPrefix,
            bool initializeOnce)
        {
            this.modelPrefix = modelPrefix;
            this.initializeOnce = initializeOnce;

            sclParser = new SclDocument(fileName);

            string hDefineName = outputFileName.ToUpper().Replace('.', '_').Replace('-', '_') + "_H_";

            if (hDefineName.LastIndexOf('/') >= 0)
            {
                hDefineName = hDefineName.Substring(hDefineName.LastIndexOf('/') + 1);
            }

            SclIED ied = null;

            if (iedName == null)
                ied = sclParser.IEDs.First();
            else
                ied = sclParser.IEDs.Find(x => x.Name == iedName);

            if (ied == null)
            {
                throw new Exception("IED model not found in SCL file! Exit.");
            }

            SclServices services = ied.SclServices;

            if (services != null)
            {
                ReportSettings rptSettings = services.ReportSettings;

                if (rptSettings != null)
                {
                    hasOwner = rptSettings.Owner;
                }
            }

            SclAccessPoint accessPoint = null;

            if (accessPointName == null)
                accessPoint = ied.AccessPoints.First();
            else
                accessPoint = ied.AccessPoints.Find(x => x.Name == accessPointName);

            if (accessPoint == null)
            {
                throw new Exception("AccessPoint not found in SCL file! Exit.");
            }

            if (sclParser.Communication != null)
                connectedAP = sclParser.Communication.GetConnectedAP(accessPoint.Name, ied.Name);

            IEDDataModel iedModel = sclParser.GetDataModel(ied.Name, accessPoint.Name);

            using (cOut)
            {
                using (hOut)
                {
                    using (StreamWriter writerC = new StreamWriter(cOut))
                    {
                        using (StreamWriter writerH = new StreamWriter(hOut))
                        {
                            printCFileHeader(icdFile, outputFileName, writerC);

                            printHeaderFileHeader(icdFile, outputFileName, hDefineName, writerH);

                            createDataCStructure(iedModel, modelPrefix, false);

                            printForwardDeclarations(iedModel, writerC, writerH, modelPrefix);

                            printDeviceModelDefinitions(iedModel, writerC, writerH, modelPrefix);

                            printInitializerFunction(writerC);

                            printVariablePointerDefines(writerH, modelPrefix);

                            printHeaderFileFooter(writerH, hDefineName);
                        }
                    }
                }
            }
        }

        private void printVariablePointerDefines(StreamWriter hOut, string modelPrefix)
        {
            hOut.WriteLine("\n\n\n");
            foreach (C_LogicalDeviceStructure c_LogicalDeviceStructure in c_LogicalDeviceStructures)
            {
                string define = c_LogicalDeviceStructure.objRef.Remove(0, modelPrefix.Length);
                hOut.WriteLine("#define " + modelPrefix.ToUpper() + define + " (&" + c_LogicalDeviceStructure.objRef + ")");

                foreach (C_LogicalNodeStructure c_LogicalNodeStructure in c_LogicalDeviceStructure.c_LogicalNodeStructures)
                {
                    define = c_LogicalNodeStructure.objRef.Remove(0, modelPrefix.Length);
                    hOut.WriteLine("#define " + modelPrefix.ToUpper() + define + " (&" + c_LogicalNodeStructure.objRef + ")");

                    foreach (C_DO_DA_Structure c_DataObjectStructure in c_LogicalNodeStructure.c_DataObjectOrDataAttributeStructures)
                        printVariablePointerDefines(hOut, modelPrefix, c_DataObjectStructure);

                }
            }
        }

        private void printVariablePointerDefines(StreamWriter hOut, string modelPrefix, C_DO_DA_Structure c_DataAttributeStructure)
        {
            string define = c_DataAttributeStructure.objRef.Remove(0, modelPrefix.Length);
            hOut.WriteLine("#define " + modelPrefix.ToUpper() + define + " (&" + c_DataAttributeStructure.objRef + ")");

        }

        private void printInitializerFunction(StreamWriter cOut)
        {
            cOut.WriteLine("\nstatic void\ninitializeValues()");
            cOut.WriteLine("{");

            foreach (C_InitializeValues c_InitializeValue in c_InitializeValues)
                cOut.WriteLine(c_InitializeValue.c_text);
            cOut.WriteLine("}");

        }

        private void printDeviceModelDefinitions(IEDDataModel iedModel, StreamWriter cOut, StreamWriter hOut, string modelPrefix)
        {
            cOut.WriteLine();

            foreach (C_DataSetStructure c_DataSetStructure in c_DataSetStructures)
                cOut.WriteLine(c_DataSetStructure.ExternToString());

            cOut.WriteLine();

            foreach (C_DataSetStructure c_DataSetStructure in c_DataSetStructures)
            {
                foreach (C_DatasetEntry c_DatasetEntry in c_DataSetStructure.externDataSetEntries)
                    cOut.WriteLine(c_DatasetEntry.externDataSetNameToString());

                cOut.WriteLine();

                foreach (C_DatasetEntry c_DatasetEntry in c_DataSetStructure.externDataSetEntries)
                {
                    cOut.WriteLine(c_DatasetEntry.ToString());
                    cOut.WriteLine();
                }

                cOut.WriteLine(c_DataSetStructure.ToString());

            }

            cOut.WriteLine();

            foreach (C_LogicalDeviceStructure c_LogicalDeviceStructure in c_LogicalDeviceStructures)
            {
                cOut.WriteLine(c_LogicalDeviceStructure.ToString());
                cOut.WriteLine();

                foreach (C_LogicalNodeStructure c_LogicalNodeStructure in c_LogicalDeviceStructure.c_LogicalNodeStructures)
                {
                    cOut.WriteLine(c_LogicalNodeStructure.ToString());
                    cOut.WriteLine();

                    foreach (C_DO_DA_Structure c_DO_DA_Structure in c_LogicalNodeStructure.c_DataObjectOrDataAttributeStructures)
                    {
                        if (c_DO_DA_Structure is C_DataObjectStructure objectStructure)
                            cOut.WriteLine(objectStructure.ToString());
                        else
                            cOut.WriteLine((c_DO_DA_Structure as C_DataAttributeStructure).ToString());
                    }
                }

            }

            foreach (C_ReportControlBlockStructure c_ReportContorlBlockStructure in c_ReportContorlBlockStructures)
            {
                cOut.WriteLine(c_ReportContorlBlockStructure.ExternNameToString());
            }

            cOut.WriteLine();

            foreach (C_ReportControlBlockStructure c_ReportContorlBlockStructure in c_ReportContorlBlockStructures)
            {
                cOut.WriteLine(c_ReportContorlBlockStructure.ToString());
            }

            cOut.WriteLine();

            foreach (C_GSEControlBlockStructure c_GSEContorlBlockStructure in c_GSEControlBlockStructures)
            {
                cOut.WriteLine(c_GSEContorlBlockStructure.ExternNameToString());
            }

            cOut.WriteLine();

            foreach (C_GSEControlBlockStructure c_GSEContorlBlockStructure in c_GSEControlBlockStructures)
            {
                cOut.WriteLine(c_GSEContorlBlockStructure.ToString());
            }

            if (c_SettingGroupControlStructure != null)
            {
                cOut.WriteLine(c_SettingGroupControlStructure.ExternNameToString());
                cOut.WriteLine();
                cOut.WriteLine(c_SettingGroupControlStructure.ToString());
            }

            cOut.WriteLine();

            foreach (C_SMVControlBlockStructure c_SMVContorlBlockStructure in c_SMVControlBlockStructures)
            {
                cOut.WriteLine(c_SMVContorlBlockStructure.ExternNameToString());
            }

            cOut.WriteLine();

            foreach (C_SMVControlBlockStructure c_SMVContorlBlockStructure in c_SMVControlBlockStructures)
            {
                cOut.WriteLine(c_SMVContorlBlockStructure.ToString());
            }

            cOut.WriteLine();

            foreach (C_LogControlBlockStructure c_LogControlBlockStructure in c_LogControlBlockStructures)
            {
                cOut.WriteLine(c_LogControlBlockStructure.ExternNameToString());
            }

            cOut.WriteLine();

            foreach (C_LogControlBlockStructure c_LogControlBlockStructure in c_LogControlBlockStructures)
            {
                cOut.WriteLine(c_LogControlBlockStructure.ToString());
            }

            cOut.WriteLine();

            foreach (C_LogStructure log in c_LogStructures)
            {
                cOut.WriteLine(log.ExternNameToString());
            }

            cOut.WriteLine();

            foreach (C_LogStructure log in c_LogStructures)
            {
                cOut.WriteLine(log.ToString());
            }

            cOut.WriteLine();

            cOut.WriteLine(c_IEDModelStructure.ToString());

        }

        private void printForwardDeclarations(IEDDataModel iedModel, StreamWriter cOut, StreamWriter hOut, string modelPrefix)
        {

            cOut.WriteLine("static void initializeValues();");
            hOut.WriteLine("extern IedModel " + modelPrefix + ";");

            foreach (C_LogicalDeviceStructure logicalDevice in c_LogicalDeviceStructures)
            {
                hOut.WriteLine(logicalDevice.ExternNameToString());

                foreach (C_LogicalNodeStructure logicalNode in logicalDevice.c_LogicalNodeStructures)
                {
                    hOut.WriteLine(logicalNode.ExternNameToString());

                    foreach (C_DO_DA_Structure c_DODA in logicalNode.c_DataObjectOrDataAttributeStructures)
                        printDataObjectForwardDeclarations(c_DODA, hOut);
                }
            }
        }

        private static string toMmsString(string iecString)
        {
            if (string.IsNullOrEmpty(iecString)) {
                return string.Empty;
            }
            return iecString.Replace('.', '$');
        }


        DataObject findDOParent(DataAttribute dataAttribute)
        {
            DataObject parentObject = null;

            while (!(dataAttribute.Parent is DataObject))
            {
                dataAttribute = dataAttribute.Parent as DataAttribute;
            }
            parentObject = dataAttribute.Parent as DataObject;

            while ((parentObject.Parent is LogicalNode) == false)
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




        private void createDataObjectCStructure(List<C_DO_DA_Structure> c_DO_DA_Structures, string dataAttributeSibling, string lnRef, DataObject dataObject, object parent, bool isTransiente, object sclDOI, int arrayIdx)
        {
            C_DataObjectStructure c_DataObjectStructure = new C_DataObjectStructure();
            c_DO_DA_Structures.Add(c_DataObjectStructure);

            c_DataObjectStructure.DataObject = dataObject;

            c_DataObjectStructure.parent = lnRef;

            c_DataObjectStructure.name = dataObject.Name;
            if (c_DataObjectStructure.arrayIndex != -1)
                c_DataObjectStructure.name = dataObject.Name + "_" + c_DataObjectStructure.arrayIndex;
            else
                c_DataObjectStructure.objRef = lnRef + "_" + dataObject.Name;

            if (dataObject.DataObjectsAndAttributes.Count > 0)
                c_DataObjectStructure.child = c_DataObjectStructure.objRef + "_" + dataObject.DataObjectsAndAttributes.First().Name;

            if (parent is LogicalNode ln)
            {
                if (ln.DataObjects.Last() != dataObject)
                {
                    DataObject sibling = ln.DataObjects[ln.DataObjects.IndexOf(dataObject) + 1];
                    c_DataObjectStructure.sibling = sibling.Name;
                }
                else if (dataAttributeSibling != null)
                    c_DataObjectStructure.sibling = dataAttributeSibling;
            }
            else if (parent is DataObject doObj)
            {
                if (doObj.DataObjectsAndAttributes.Last() != dataObject)
                {
                    DataObjectOrAttribute sibling = doObj.DataObjectsAndAttributes[doObj.DataObjectsAndAttributes.IndexOf(dataObject) + 1];
                    c_DataObjectStructure.sibling = sibling.Name;
                }
                else if (dataAttributeSibling != null)
                    c_DataObjectStructure.sibling = dataAttributeSibling;
            }



            bool isDoTransient = false;

            if (isTransiente)
                isDoTransient = true;
            else
                if (dataObject.IsTransiente)
                isDoTransient = true;

            if (dataObject.Count > 0)
            {
                c_DataObjectStructure.child = c_DataObjectStructure.objRef + "_0";
                c_DataObjectStructure.elementCount = dataObject.Count;
                c_DataObjectStructure.arrayIndex = arrayIdx;

                for (int idx = 0; idx < dataObject.Count; idx++)
                {
                    C_DataObjectStructure c_ArrayDataObjectStructure = new C_DataObjectStructure();
                    c_DO_DA_Structures.Add(c_ArrayDataObjectStructure);

                    c_ArrayDataObjectStructure.DataObject = dataObject;

                    //c_ArrayDataObjectStructure.parent = lnRef;
                    c_ArrayDataObjectStructure.parent = lnRef + "_" + dataObject.Name;

                    c_ArrayDataObjectStructure.name = dataObject.Name;
                    c_ArrayDataObjectStructure.objRef = lnRef + "_" + dataObject.Name + "_" + idx;

                    if (idx != dataObject.Count - 1)
                        c_ArrayDataObjectStructure.sibling = lnRef + "_" + dataObject.Name + "_" + (idx + 1);

                    c_ArrayDataObjectStructure.arrayIndex = idx;

                    if (dataObject.DataObjectsAndAttributes.Count > 0)
                    {
                        string firstDataAttributeName = "NULL";

                        DataObjectOrAttribute dataObjectOrAttribute1 = dataObject.DataObjectsAndAttributes.First();
                        if (dataObjectOrAttribute1 is DataAttribute da)
                            firstDataAttributeName = da.Name;
                        else
                        {
                            firstDataAttributeName = lnRef;
                        }

                        c_ArrayDataObjectStructure.child = c_ArrayDataObjectStructure.objRef + "_" + dataObject.DataObjectsAndAttributes.First().Name;

                        foreach (DataObjectOrAttribute dataObjectOrAttribute in dataObject.DataObjectsAndAttributes)
                        {
                            if (dataObjectOrAttribute is DataObject doObj)
                            {
                                SclSDI sclSDO = getSDI(sclDOI, dataObject.Name);
                            
                                firstDataAttributeName += "_" + dataObject.Name;

                                createDataObjectCStructure(c_DO_DA_Structures, c_DataObjectStructure.objRef, firstDataAttributeName, doObj, dataObject, isDoTransient, sclSDO, -1);

                            }
                            else
                            {

                                createDataAttributeCStructure(c_DO_DA_Structures, c_DataObjectStructure.objRef, dataObjectOrAttribute as DataAttribute, dataObject, isDoTransient, sclDOI, -1);

                            }
                        }
                    }
                }
            }
            else
            {
                c_DataObjectStructure.arrayIndex = arrayIdx;

                string firstDataAttributeName = "NULL";

                DataObjectOrAttribute dataObjectOrAttribute1 = dataObject.DataObjectsAndAttributes.First();
                if (dataObjectOrAttribute1 is DataAttribute da)
                    firstDataAttributeName = da.Name;
                else
                {
                    firstDataAttributeName = lnRef;
                }

                foreach (DataObjectOrAttribute dataObjectOrAttribute in dataObject.DataObjectsAndAttributes)
                {
                    if (dataObjectOrAttribute is DataObject doObj)
                    {
                        firstDataAttributeName += "_" + dataObject.Name;

                        SclSDI sclSDO = getSDI(sclDOI, dataObject.Name);

                        createDataObjectCStructure(c_DO_DA_Structures, c_DataObjectStructure.objRef, firstDataAttributeName, doObj, dataObject, isDoTransient, sclSDO, -1);

                    }
                    else
                    {

                        createDataAttributeCStructure(c_DO_DA_Structures, c_DataObjectStructure.objRef, dataObjectOrAttribute as DataAttribute, dataObject, isDoTransient, sclDOI, -1);

                    }
                }

            }

        }

        private void createDataAttributeCStructure(List<C_DO_DA_Structure> c_DO_DA_Structures, string doName, DataAttribute dataAttribute, DataObjectOrAttribute parent, bool isDoTransient, object daiObj, int arrayIdx)
        {
            C_DataAttributeStructure c_DataAttributeStructure = new C_DataAttributeStructure();
            c_DO_DA_Structures.Add(c_DataAttributeStructure);

            c_DataAttributeStructure.parent = doName;
            c_DataAttributeStructure.isTransient = isDoTransient;
            c_DataAttributeStructure.name = dataAttribute.Name;
            c_DataAttributeStructure.sclFC = dataAttribute.Fc;
            c_DataAttributeStructure.DataAttribute = dataAttribute;

            if (c_DataAttributeStructure.arrayIndex != -1)
                c_DataAttributeStructure.name = dataAttribute.Name + "_" + c_DataAttributeStructure.arrayIndex;
            else
                c_DataAttributeStructure.objRef = doName + "_" + dataAttribute.Name;

            c_DataAttributeStructure.objRef = doName + "_" + dataAttribute.Name;
            c_DataAttributeStructure.elementCount = dataAttribute.Count;


            if (dataAttribute.Definition.SAddr != null)
                c_DataAttributeStructure.sAddr = dataAttribute.Definition.SAddr;

            if (parent is DataObject doObj)
            {
                if (doObj.DataObjectsAndAttributes.Count > 0)
                {
                    int i = doObj.DataObjectsAndAttributes.IndexOf(dataAttribute);
                    if (i < doObj.DataObjectsAndAttributes.Count - 1)
                    {
                        DataObjectOrAttribute sibling = doObj.DataObjectsAndAttributes[i + 1];
                        string siblindDOName = doName + "_" + sibling.Name;

                        if (sibling is DataAttribute da)
                        {
                            if (da.Fc == SclFC.SE)
                                if (!siblindDOName.StartsWith(modelPrefix + "_SE"))
                                    siblindDOName = siblindDOName.Replace(modelPrefix, modelPrefix + "_SE");
                        }

                        c_DataAttributeStructure.sibling = siblindDOName;

                    }

                }
            }
            else if (parent is DataAttribute da)
            {
                if (da.subDataAttributes.Count > 0)
                {
                    int i = da.subDataAttributes.IndexOf(dataAttribute);
                    if (i < da.subDataAttributes.Count - 1)
                    {
                        DataAttribute sibling = da.subDataAttributes[i + 1];
                        string siblindDOName = doName + "_" + sibling.Name;

                        if (sibling.Fc == SclFC.SE)
                            if (!siblindDOName.StartsWith(modelPrefix + "_SE"))
                                siblindDOName = siblindDOName.Replace(modelPrefix, modelPrefix + "_SE");

                        c_DataAttributeStructure.sibling = siblindDOName;

                    }

                }
            }


            if (dataAttribute.Fc == SclFC.SE)
            {
                C_DataAttributeStructure c_SEDataAttributeStructure = new C_DataAttributeStructure();
                c_DO_DA_Structures.Add(c_SEDataAttributeStructure);

                c_SEDataAttributeStructure.parent = doName;
                c_SEDataAttributeStructure.isTransient = isDoTransient;
                c_SEDataAttributeStructure.name = dataAttribute.Name;
                c_SEDataAttributeStructure.sclFC = dataAttribute.Fc;
                c_SEDataAttributeStructure.DataAttribute = dataAttribute;
                c_SEDataAttributeStructure.objRef = doName + "_" + dataAttribute.Name;
                c_SEDataAttributeStructure.elementCount = dataAttribute.Count;

                if (dataAttribute.Definition.SAddr != null)
                    c_SEDataAttributeStructure.sAddr = dataAttribute.Definition.SAddr;

                if (!c_SEDataAttributeStructure.objRef.StartsWith(modelPrefix + "_SE_"))
                    c_SEDataAttributeStructure.objRef = c_SEDataAttributeStructure.objRef.Replace(modelPrefix, modelPrefix + "_SE");

                if (dataAttribute.subDataAttributes.Count > 0)
                    c_SEDataAttributeStructure.child = c_SEDataAttributeStructure.objRef + "_" + dataAttribute.subDataAttributes.First().Name;

                if (dataAttribute.subDataAttributes.Count > 0)
                    c_DataAttributeStructure.sibling = c_SEDataAttributeStructure.objRef;

                if (parent is DataAttribute data_attribute)
                {
                    if (!c_SEDataAttributeStructure.parent.StartsWith(modelPrefix + "_SE"))
                        c_SEDataAttributeStructure.parent = c_SEDataAttributeStructure.parent.Replace(modelPrefix, modelPrefix + "_SE");
                }


                c_DataAttributeStructure.sclFC = SclFC.SG;
            }

            if (dataAttribute.subDataAttributes.Count > 0)
                c_DataAttributeStructure.child = c_DataAttributeStructure.objRef + "_" + dataAttribute.subDataAttributes.First().Name;

            if (dataAttribute.Count > 0)
            {
                for (int idx = 0; idx < dataAttribute.Count; idx++)
                {
                    C_DataAttributeStructure arrayElement = new C_DataAttributeStructure();
                    c_DO_DA_Structures.Add(arrayElement);
                    arrayElement.name = "NULL";
                    arrayElement.parent = c_DataAttributeStructure.objRef;

                    arrayElement.objRef = doName + "_" + dataAttribute.Name + "_" + idx;

                    if (idx != dataAttribute.Count - 1)
                        arrayElement.sibling = doName + "_" + dataAttribute.Name + "_" + (idx + 1);

                    if (dataAttribute.subDataAttributes.Count > 0)
                        arrayElement.child = arrayElement.objRef + "_" + dataAttribute.subDataAttributes.First().Name;

                    arrayElement.elementCount = 0;
                    arrayElement.arrayIndex = idx;

                    arrayElement.DataAttribute = dataAttribute;


                    if (idx == 0)
                        c_DataAttributeStructure.child = arrayElement.objRef;

                    foreach (DataAttribute dataObjectOrAttribute in dataAttribute.subDataAttributes)
                    {
                        SclSDI sclSDI = getSDI(daiObj, dataAttribute.Name);

                        createDataAttributeCStructure(c_DO_DA_Structures, arrayElement.objRef, dataObjectOrAttribute, dataAttribute, isDoTransient, sclSDI, -1);
                    }
                }
            }
            else
            {
                if (dataAttribute.subDataAttributes.Count > 0)
                    c_DataAttributeStructure.child = c_DataAttributeStructure.objRef + "_" + dataAttribute.subDataAttributes.First().Name;

                foreach (DataAttribute dataObjectOrAttribute in dataAttribute.subDataAttributes)
                {
                    SclSDI sclSDI = getSDI(daiObj, dataAttribute.Name);

                    createDataAttributeCStructure(c_DO_DA_Structures, c_DataAttributeStructure.objRef, dataObjectOrAttribute, dataAttribute, isDoTransient, sclSDI, -1);
                }
            }

            DataObject dataObject = findDOParent(dataAttribute);
            LogicalNode logicalNode = findLNParent(dataObject);
            LogicalDevice logicalDevice = logicalNode.Parent as LogicalDevice;

            string value = null;

            SclDOI sclDOI = logicalNode.SclElement.DOIs.Find(x => x.Name == dataObject.Name);

            SclDAI sclDAI1 = null;

            if (sclDOI != null)
            {
                
                SclDAI sclDAI = sclDOI.SclDAIs.Find(x => x.Name == dataAttribute.Name);
                if (sclDAI != null && dataAttribute.ObjRef == logicalDevice.Name + "/" + logicalNode.Name + "." + sclDOI.Name + "." + sclDAI.Name)
                {
                    sclDAI1 = sclDAI;
                }

                else
                {
                    sclDAI1 = getNestedDAI(sclDOI, dataAttribute.ObjRef);
                }

            }

            if (sclDAI1 != null)
                if (sclDAI1.Val != null)
                    printValue(c_DataAttributeStructure, sclDAI1.Val);
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

        SclSDI getSDI(object parent, string name)
        {
            if (parent == null)
                return null;

            if (parent is SclDOI sclDOI)
                return sclDOI.SclSDIs.Find(x => x.Name == name);
            else if (parent is SclSDI sclSDI)
                return sclSDI.SclSDIs.Find(x => x.Name == name);
            else
                return null;
        }

        SclSDI getSDINested(object parent, string name)
        {
            if (parent == null)
                return null;

            if (parent is SclDOI sclDOI)
                return sclDOI.SclSDIs.Find(x => x.Name == name);
            else if (parent is SclSDI sclSDI)
                return sclSDI.SclSDIs.Find(x => x.Name == name);
            else
                return null;
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


        SclDAI getNestedDAI(SclDOI initialDO, string name)
        {
            string strippedObjRef = getStippedObjRef(name);


            string[] parts = strippedObjRef.Split('.');

            object obj = null;
            Object foundObject = initialDO;
            SclDAI sclDAI = null;
            for (int i = 1; i < parts.Length; i++)
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

        private void printValue(C_DataAttributeStructure c_DataAttributeStructure, string value)
        {
            C_InitializeValues c_InitializeValue = new C_InitializeValues();

            c_InitializeValue.c_text = "\n";
            if (initializeOnce)
            {
                c_InitializeValue.c_text += "if (!";
                c_InitializeValue.c_text += c_DataAttributeStructure.objRef;
                c_InitializeValue.c_text += ".mmsValue)\n";
            }
            c_InitializeValue.c_text += c_DataAttributeStructure.objRef;
            c_InitializeValue.c_text += ".mmsValue = ";

            switch (c_DataAttributeStructure.DataAttribute.AttributeType)
            {
                case AttributeType.INT8:
                case AttributeType.INT16:
                case AttributeType.INT32:
                case AttributeType.INT64:
                    c_InitializeValue.c_text += "MmsValue_newIntegerFromInt32(" + value + ");";
                    break;

                case AttributeType.ENUMERATED:

                    string EnumType = c_DataAttributeStructure.DataAttribute.Definition.Type;
                    if (EnumType != null)
                    {
                        SclEnumType sclEnumType = sclParser.DataTypeTemplates.GetEnumType(EnumType);
                        if (sclEnumType != null)
                        {
                            SclEnumVal sclEnumVal = sclEnumType.EnumValues.Find(x => x.SymbolicName == value);
                            if (sclEnumVal != null)
                                c_InitializeValue.c_text += "MmsValue_newIntegerFromInt32(" + sclEnumVal.Ord + ");";
                            else
                            {
                                Console.WriteLine("ERROR", "Model integrity", "Value " + value + " in DAI " + c_DataAttributeStructure.objRef +
                               "  does not exist in enumerated type " + EnumType + ". Reload the file after fixing the problem!", "DAI_DataAttributeMissing");

                            }
                        }
                        else
                        {
                            Console.WriteLine("ERROR", "Model integrity", "Wrong ENUMERATED type " + EnumType + " in DAI " + c_DataAttributeStructure.objRef
                               , "DAI_DataAttributeMissing");
                        }

                    }
                    else
                    {
                        Console.WriteLine("ERROR", "Model integrity", "Wrong ENUMERATED type in DAI " + c_DataAttributeStructure.objRef, "DAI_DataAttributeMissing");
                    }
                    break;

                case AttributeType.INT8U:
                case AttributeType.INT16U:
                case AttributeType.INT24U:
                case AttributeType.INT32U:
                    c_InitializeValue.c_text += "MmsValue_newUnsignedFromUint32(" + value + ");";
                    break;
                case AttributeType.BOOLEAN:
                    c_InitializeValue.c_text += "MmsValue_newBoolean(" + value + ");";
                    break;

                case AttributeType.OCTET_STRING_64:
                    {
                        string daValName = c_DataAttributeStructure.DataAttribute.Name + "__val";

                        c_InitializeValue.c_text += "MmsValue_newOctetString(0, 64);\n";
                        c_InitializeValue.c_text += "uint8_t " + daValName + "[] = ";
                        c_InitializeValue.c_text += value;
                        c_InitializeValue.c_text += ";\n";
                        c_InitializeValue.c_text += "MmsValue_setOctetString(";
                        c_InitializeValue.c_text += c_DataAttributeStructure.DataAttribute.Name;
                        c_InitializeValue.c_text += ".mmsValue, " + daValName + ", " + value.Length + ");\n";
                    }
                    break;

                case AttributeType.CODEDENUM:
                    {
                        c_InitializeValue.c_text += "MmsValue_newBitString(2);\n";
                        c_InitializeValue.c_text += "MmsValue_setBitStringFromIntegerBigEndian(";
                        c_InitializeValue.c_text += c_DataAttributeStructure.DataAttribute.Name;
                        c_InitializeValue.c_text += ".mmsValue, ";
                        c_InitializeValue.c_text += value;
                        c_InitializeValue.c_text += ");\n";
                    }
                    break;

                case AttributeType.UNICODE_STRING_255:
                    c_InitializeValue.c_text += "MmsValue_newMmsString(\"" + value + "\");";
                    break;
                case AttributeType.CURRENCY:
                    c_InitializeValue.c_text += "MmsValue_newVisibleString(\"" + value + "\");";
                    break;
                case AttributeType.FLOAT32:
                    c_InitializeValue.c_text += "MmsValue_newFloat(" + float.Parse(value).ToString(".0").Replace(",", ".") + ");";
                    break;
                case AttributeType.FLOAT64:
                    c_InitializeValue.c_text += "MmsValue_newDouble(" + value + ");";
                    break;

                case AttributeType.TIMESTAMP:
                    c_InitializeValue.c_text += "MmsValue_newUtcTimeByMsTime(" + value + ");";
                    break;

                case AttributeType.VISIBLE_STRING_32:
                case AttributeType.VISIBLE_STRING_64:
                case AttributeType.VISIBLE_STRING_129:
                case AttributeType.VISIBLE_STRING_255:
                case AttributeType.VISIBLE_STRING_65:
                    c_InitializeValue.c_text += "MmsValue_newVisibleString(\"" + value + "\");";
                    break;

                default:
                    Console.WriteLine("Unknown default value for " + c_DataAttributeStructure.objRef + " type: " + c_DataAttributeStructure.DataAttribute.AttributeType.ToString());
                    c_InitializeValue.c_text += "NULL;";
                    break;
            }

            c_InitializeValues.Add(c_InitializeValue);
        }

        private void addLogControlBlockInstance(string lnPrefix, LogControl logcontrol, int lcbNumber, LogicalDevice logicalDevice, string lnClass)
        {
            C_LogControlBlockStructure c_LogContorlBlockStructure = new C_LogControlBlockStructure();
            c_LogContorlBlockStructure.logControl = logcontrol;
            c_LogContorlBlockStructure.externName = lnPrefix + "_lcb" + lcbNumber.ToString();
            c_LogContorlBlockStructure.parent = lnPrefix;
            c_LogContorlBlockStructure.hasOwner = hasOwner;
            c_LogContorlBlockStructure.lnClass = lnClass;
            c_LogContorlBlockStructure.ldInst = logicalDevice.Inst;

            c_LogControlBlockStructures.Add(c_LogContorlBlockStructure);
        }

        private void addLogInstance(string lnPrefix, Log log, int lcbNumber)
        {
            C_LogStructure c_LogStructure = new C_LogStructure();
            c_LogStructure.log = log;
            c_LogStructure.externName = lnPrefix + "_log" + lcbNumber.ToString();
            c_LogStructure.parent = lnPrefix;

            c_LogStructures.Add(c_LogStructure);
        }

        private void addReportControlBlockInstance(string lnPrefix, ReportControl reportControl, string index, int reportNumber, int reportsCount, byte[] clientIpAddr)
        {
            C_ReportControlBlockStructure c_ReportContorlBlockStructure = new C_ReportControlBlockStructure();
            c_ReportContorlBlockStructure.ReportControl = reportControl;
            c_ReportContorlBlockStructure.externName = lnPrefix + "_report" + reportsCount.ToString();
            c_ReportContorlBlockStructure.parent = lnPrefix;
            c_ReportContorlBlockStructure.clientIpAddr = clientIpAddr;
            c_ReportContorlBlockStructure.index = index;
            c_ReportContorlBlockStructure.reportNumber = reportNumber;
            c_ReportContorlBlockStructure.reportsCount = reportsCount;
            c_ReportContorlBlockStructure.hasOwner = hasOwner;

            c_ReportContorlBlockStructures.Add(c_ReportContorlBlockStructure);
        }

        private void addGSEControlBlockInstance(string lnPrefix, GSEControl reportControl, SclGSE sclGSE, int gseNumber)
        {
            C_GSEControlBlockStructure c_GSEContorlBlockStructure = new C_GSEControlBlockStructure();
            c_GSEContorlBlockStructure.GSEControl = reportControl;
            c_GSEContorlBlockStructure.externName = lnPrefix + "_gse" + gseNumber.ToString();
            c_GSEContorlBlockStructure.parent = lnPrefix;
            c_GSEContorlBlockStructure.reportNumber = gseNumber;
            c_GSEContorlBlockStructure.hasOwner = hasOwner;
            c_GSEContorlBlockStructure.lnPrefix = lnPrefix;
            c_GSEContorlBlockStructure.SclGSE = sclGSE;

            c_GSEControlBlockStructures.Add(c_GSEContorlBlockStructure);
        }

        private void addSMVControlBlockInstance(string lnPrefix, SMVControl smvControl, SclSMV sclSMV, int smvNumber)
        {
            C_SMVControlBlockStructure c_SMVContorlBlockStructure = new C_SMVControlBlockStructure();
            c_SMVContorlBlockStructure.SMVControl = smvControl;
            c_SMVContorlBlockStructure.externName = lnPrefix + "_smv" + smvNumber.ToString();
            c_SMVContorlBlockStructure.parent = lnPrefix;
            c_SMVContorlBlockStructure.reportNumber = smvNumber;
            c_SMVContorlBlockStructure.hasOwner = hasOwner;
            c_SMVContorlBlockStructure.lnPrefix = lnPrefix;
            c_SMVContorlBlockStructure.SclSMV = sclSMV;

            c_SMVControlBlockStructures.Add(c_SMVContorlBlockStructure);
        }
        private void addSettingGroulBlockInstance(string lnPrefix, SclSettingControl settingControl)
        {
            c_SettingGroupControlStructure = new C_SettingGroupStructure();
            c_SettingGroupControlStructure.settingControl = settingControl;
            c_SettingGroupControlStructure.parent = lnPrefix;
            c_SettingGroupControlStructure.externName = lnPrefix + "_sgcb";
        }

        private void createDataCStructure(IEDDataModel iedModel, string modelPrefix, bool isTransient)
        {
            c_IEDModelStructure = new C_IEDModelStructure();
            c_IEDModelStructure.name = iedModel.Name;
            c_IEDModelStructure.modelPrefix = modelPrefix;

            foreach (LogicalDevice logicalDevice in iedModel.LogicalDevices)
            {
                C_LogicalDeviceStructure c_LogicalDeviceStructure = new C_LogicalDeviceStructure();
                c_LogicalDeviceStructure.parent = modelPrefix;
                c_LogicalDeviceStructure.name = logicalDevice.Inst;
                c_LogicalDeviceStructure.objRef = modelPrefix + "_" + logicalDevice.Inst;

                c_IEDModelStructure.firstChild = c_LogicalDeviceStructure.objRef;

                if (iedModel.LogicalDevices.Last() != logicalDevice)
                {
                    LogicalDevice sibling = iedModel.LogicalDevices[iedModel.LogicalDevices.IndexOf(logicalDevice) + 1];
                    c_LogicalDeviceStructure.sibling = sibling.Inst;
                }

                c_LogicalDeviceStructure.firstChild = logicalDevice.LogicalNodes.First().Name;

                foreach (LogicalNode logicalNode in logicalDevice.LogicalNodes)
                {
                    C_LogicalNodeStructure c_LogicalNodeStructure = new C_LogicalNodeStructure();
                    c_LogicalNodeStructure.parent = c_LogicalDeviceStructure.objRef;
                    c_LogicalNodeStructure.name = logicalNode.Name;
                    c_LogicalNodeStructure.objRef = c_LogicalDeviceStructure.objRef + "_" + logicalNode.Name;

                    if (logicalDevice.LogicalNodes.Last() != logicalNode)
                    {
                        LogicalNode sibling = logicalDevice.LogicalNodes[logicalDevice.LogicalNodes.IndexOf(logicalNode) + 1];
                        c_LogicalNodeStructure.sibling = sibling.Name;
                    }

                    c_LogicalNodeStructure.firstChild = logicalNode.DataObjects.First().Name;

                    foreach (DataObject dataObject in logicalNode.DataObjects)
                    {
                        SclDOI sclDOI = logicalNode.SclElement.DOIs.Find(x => x.Name == dataObject.Name);
                        createDataObjectCStructure(c_LogicalNodeStructure.c_DataObjectOrDataAttributeStructures, null, c_LogicalNodeStructure.objRef, dataObject, logicalNode, isTransient, sclDOI, -1);
                    }

                    if (connectedAP != null)
                    {
                        int gseNumber = 0;
                        foreach (GSEControl gSEControl in logicalNode.GSEControls)
                        {
                            SclGSE sclGSE = connectedAP.GSEs.Find(x => x.CbName == gSEControl.Name);

                            if (sclGSE != null)
                            {
                                addGSEControlBlockInstance(c_LogicalNodeStructure.objRef, gSEControl, sclGSE, gseNumber);

                                gseNumber++;

                            }
                            else
                            {
                                Console.WriteLine("GSE not found for GoCB " + gSEControl.Name);
                            }
                        }

                        int gseIndex = 0;

                        if (c_GSEControlBlockStructures.Count > 0)
                        {
                            foreach (C_GSEControlBlockStructure c_GSEContorlBlockStructure in c_GSEControlBlockStructures)
                            {
                                if (c_GSEContorlBlockStructure != c_GSEControlBlockStructures.Last())
                                {
                                    c_GSEContorlBlockStructure.sibling = c_GSEControlBlockStructures[gseIndex + 1].externName;
                                }

                                gseIndex++;
                            }

                            c_IEDModelStructure.gseCBs = c_GSEControlBlockStructures.First().externName;
                        }

                        int smvNumber = 0;
                        foreach (SMVControl sMVControl in logicalNode.SMVControls)
                        {
                            SclSMV sclSMV = connectedAP.SMVs.Find(x => x.CbName == sMVControl.Name);

                            if (sclSMV != null)
                            {
                                addSMVControlBlockInstance(c_LogicalNodeStructure.objRef, sMVControl, sclSMV, smvNumber);

                                gseNumber++;

                            }
                            else
                            {
                                Console.WriteLine("GSE not found for GoCB " + sMVControl.Name);
                            }
                        }

                        int smvIndex = 0;

                        if (c_SMVControlBlockStructures.Count > 0)
                        {
                            foreach (C_SMVControlBlockStructure c_SMVContorlBlockStructure in c_SMVControlBlockStructures)
                            {
                                if (c_SMVContorlBlockStructure != c_SMVControlBlockStructures.Last())
                                {
                                    c_SMVContorlBlockStructure.sibling = c_SMVControlBlockStructures[gseIndex + 1].externName;
                                }

                                smvIndex++;
                            }

                            c_IEDModelStructure.svCBs = c_SMVControlBlockStructures.First().externName;
                        }
                    }

                    int lcbNumber = 0;

                    foreach (LogControl logControl in logicalNode.LogControls)
                    {
                        addLogControlBlockInstance(c_LogicalNodeStructure.objRef, logControl, lcbNumber, logicalDevice, logicalNode.LnClass);

                        lcbNumber++;
                    }

                    int logIndex = 0;

                    if (c_LogControlBlockStructures.Count > 0)
                    {
                        foreach (C_LogControlBlockStructure c_LogContorlBlockStructure in c_LogControlBlockStructures)
                        {
                            if (c_LogContorlBlockStructure != c_LogControlBlockStructures.Last())
                            {
                                c_LogContorlBlockStructure.sibling = c_LogControlBlockStructures[logIndex + 1].externName;
                            }

                            logIndex++;
                        }

                        c_IEDModelStructure.lcbs = c_LogControlBlockStructures.First().externName;
                    }

                    int logNumber = 0;

                    foreach (Log log in logicalNode.Logs)
                    {
                        addLogInstance(c_LogicalNodeStructure.objRef, log, logNumber);

                        logNumber++;
                    }

                    int log_Index = 0;

                    if (c_LogStructures.Count > 0)
                    {
                        foreach (C_LogStructure c_logs in c_LogStructures)
                        {
                            if (c_logs != c_LogStructures.Last())
                            {
                                c_logs.sibling = c_LogStructures[log_Index + 1].externName;
                            }

                            log_Index++;
                        }

                        c_IEDModelStructure.logs = c_LogStructures.First().externName;
                    }

                    if (logicalNode.SettingControl != null)
                    {
                        addSettingGroulBlockInstance(c_LogicalNodeStructure.objRef, logicalNode.SettingControl);

                        c_IEDModelStructure.sgcbs = c_SettingGroupControlStructure.externName;
                    }

                    int reportsCount = 0;

                    foreach (ReportControl reportControl in logicalNode.ReportControlBlocks)
                    {
                        if (reportControl.SclReportControl.Indexed)
                        {
                            int maxInstances = 1;

                            List<SclClientLN> clientLNs = null;

                            if (reportControl.SclReportControl.RptEna != null)
                            {
                                maxInstances = reportControl.SclReportControl.RptEna.Max;

                                clientLNs = reportControl.SclReportControl.RptEna.ClientLNs;
                            }

                            try
                            {
                                for (int i = 0; i < maxInstances; i++)
                                {
                                    string index = (i + 1).ToString("00");
                                    Console.WriteLine("print report instance " + index);

                                    byte[] clientAddress = new byte[17];
                                    clientAddress[0] = 0;

                                    if (clientLNs != null)
                                    {
                                        if (i <= (clientLNs.Count - 1))
                                        {
                                            SclClientLN sclClientLN = clientLNs[i];

                                            string iedName = sclClientLN.IedName;
                                            string apRef = sclClientLN.ApRef;

                                            string ipAddress = getIpAddressByIedName(iedName, apRef);

                                            IPAddress iPAddress = IPAddress.Parse(ipAddress);

                                            if (iPAddress.AddressFamily == System.Net.Sockets.AddressFamily.InterNetwork)
                                            {
                                                clientAddress[0] = 4;
                                                for (int j = 0; j < 4; j++)
                                                    clientAddress[j + 1] = iPAddress.GetAddressBytes()[j];
                                            }
                                            else if (iPAddress.AddressFamily == System.Net.Sockets.AddressFamily.InterNetworkV6)
                                            {
                                                clientAddress[0] = 6;
                                                for (int j = 0; j < 16; j++)
                                                    clientAddress[j + 1] = iPAddress.GetAddressBytes()[j];
                                            }
                                        }
                                    }

                                    addReportControlBlockInstance(c_LogicalNodeStructure.objRef, reportControl, index, logicalNode.ReportControlBlocks.Count, reportsCount, clientAddress);
                                    reportsCount++;
                                }
                            }
                            catch (Exception ex)
                            {
                                Console.WriteLine(ex.Message);
                            }
                        }
                        else
                        {
                            byte[] clientAddress = new byte[17];
                            clientAddress[0] = 0;
                            addReportControlBlockInstance(c_LogicalNodeStructure.objRef, reportControl, "", logicalNode.ReportControlBlocks.Count, reportsCount, clientAddress);
                            reportsCount++;
                        }

                        int rptIndex = 0;

                        if (c_ReportContorlBlockStructures.Count > 0)
                        {
                            foreach (C_ReportControlBlockStructure c_ReportContorlBlockStructure in c_ReportContorlBlockStructures)
                            {
                                if (c_ReportContorlBlockStructure != c_ReportContorlBlockStructures.Last())
                                {
                                    c_ReportContorlBlockStructure.sibling = c_ReportContorlBlockStructures[rptIndex + 1].externName;
                                }

                                rptIndex++;
                            }

                            c_IEDModelStructure.rcbs = c_ReportContorlBlockStructures.First().externName;
                        }
                    }

                    int datasetCOunt = 0;
                    foreach (DataSet dataSet in logicalNode.DataSets)
                    {
                        C_DataSetStructure c_DataSetStructure = new C_DataSetStructure();
                        c_DataSetStructure.DataSet = dataSet;
                        c_DataSetStructure.externDataSetName = modelPrefix + "ds_" + logicalDevice.Inst + "_" + logicalNode.Name + "_" + dataSet.Name;
                        c_DataSetStructure.logicalDeviceName = logicalDevice.Inst;
                        c_DataSetStructure.name = logicalNode.Name + "$" + dataSet.Name;
                        c_DataSetStructure.elementCount = dataSet.SclDataSet.Fcdas.Count;

                        if (datasetCOunt + 1 < logicalNode.DataSets.Count)
                            c_DataSetStructure.sibling = modelPrefix + "ds_" + logicalDevice.Inst + "_" + logicalNode.Name + "_" + logicalNode.DataSets[datasetCOunt + 1].Name;

                        int fcdaCount = 0;
                        foreach (SclFCDA sclFCDA in dataSet.SclDataSet.Fcdas)
                        {
                            C_DatasetEntry c_DatasetEntry = new C_DatasetEntry();
                            c_DatasetEntry.externDataSetName = c_DataSetStructure.externDataSetName + "_fcda" + fcdaCount.ToString();

                            if (fcdaCount == 0)
                                c_DataSetStructure.fcdas = c_DatasetEntry.externDataSetName;

                            if (fcdaCount + 1 < dataSet.SclDataSet.Fcdas.Count)
                                c_DatasetEntry.sibling = c_DataSetStructure.externDataSetName + "_fcda" + (fcdaCount + 1).ToString();

                            c_DatasetEntry.logicalDeviceName = logicalDevice.Inst;
                            c_DatasetEntry.isLDNameDynamicallyAllocated = false;

                            if (sclFCDA.Prefix != null)
                                c_DatasetEntry.variableName += sclFCDA.Prefix;

                            c_DatasetEntry.variableName += sclFCDA.LnClass + sclFCDA.LnInst;

                            c_DatasetEntry.variableName += "$" + sclFCDA.Fc.ToString();

                            c_DatasetEntry.variableName += "$" + toMmsString(sclFCDA.DoName);

                            if (sclFCDA.DaName != null)
                                c_DatasetEntry.variableName += "$" + toMmsString(sclFCDA.DaName);

                            /* check for array index and component */
                            int arrayStart = c_DatasetEntry.variableName.IndexOf('(');
                            string componentName = "";
                            if (arrayStart != -1)
                            {
                                string variableName = c_DatasetEntry.variableName.Substring(0, arrayStart);

                                int arrayEnd = c_DatasetEntry.variableName.IndexOf(')');

                                string arrayIndexStr = c_DatasetEntry.variableName.Substring(arrayStart + 1, arrayEnd);
                                int arrayIndex = int.Parse(arrayIndexStr);

                                string componentNamePart = c_DatasetEntry.variableName.Substring(arrayEnd + 1);

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

                            if (componentName != "")
                                c_DatasetEntry.componentName = componentName;

                            c_DataSetStructure.externDataSetEntries.Add(c_DatasetEntry);

                            fcdaCount++;
                        }

                        datasetCOunt++;

                        c_DataSetStructures.Add(c_DataSetStructure);
                    }

                    if (c_DataSetStructures.Count > 0)
                        c_IEDModelStructure.dataSets = c_DataSetStructures.First().externDataSetName;

                    c_LogicalDeviceStructure.c_LogicalNodeStructures.Add(c_LogicalNodeStructure);
                }

                c_LogicalDeviceStructures.Add(c_LogicalDeviceStructure);
            }
        }

        private string getIpAddressByIedName(string iedName, string apName)
        {
            if (sclParser.Communication != null)
            {
                SclSubNetwork sclSubNetwork = sclParser.Communication.GetSubNetwork(apName, iedName);

                if (sclSubNetwork != null)
                {
                    SclConnectedAP ap = sclSubNetwork.GetConnectedAPs().Find(x => x.IedName == iedName && x.ApName == apName);

                    if (ap != null)
                    {
                        SclAddress sclAddress = ap.Address;

                        if (sclAddress != null)
                        {
                            SclP ip = sclAddress.SclPs.Find(x => x.Type == "IP");
                            if (ip != null)
                                return ip.Text;
                        }
                    }
                }
            }

            return null;
        }

        private void printDataObjectForwardDeclarations(C_DO_DA_Structure c_DataObjectStructure, StreamWriter hOut)
        {
            if (c_DataObjectStructure is C_DataObjectStructure dataObject1)
                hOut.WriteLine(dataObject1.ExternNameToString());
            else
                hOut.WriteLine((c_DataObjectStructure as C_DataAttributeStructure).ExternNameToString());
        }

        private void printHeaderFileHeader(string filename, string outputFileName, string hDefineName, StreamWriter hOut)
        {
            hOut.WriteLine("/*");
            hOut.WriteLine(" * " + outputFileName + ".h");
            hOut.WriteLine(" *");
            hOut.WriteLine(" * automatically generated from " + filename);
            hOut.WriteLine(" */\n");
            hOut.WriteLine("#ifndef " + hDefineName);
            hOut.WriteLine("#define " + hDefineName + "\n");
            hOut.WriteLine("#include <stdlib.h>");
            hOut.WriteLine("#include \"iec61850_model.h\"");
            hOut.WriteLine();
        }

        private void printHeaderFileFooter(StreamWriter hOut, string hDefineName)
        {
            hOut.WriteLine();
            hOut.WriteLine("#endif /* " + hDefineName + " */\n");
        }

        private void printCFileHeader(string filename, string outputFileName, StreamWriter cOut)
        {

            string include = outputFileName + ".h\"";
            if (include.LastIndexOf('/') >= 0)
            {
                include = include.Substring(include.LastIndexOf('/') + 1);
            }
            cOut.WriteLine("/*");
            cOut.WriteLine(" * " + outputFileName + ".c");
            cOut.WriteLine(" *");
            cOut.WriteLine(" * automatically generated from " + filename);
            cOut.WriteLine(" */");
            cOut.WriteLine("#include \"" + include);
            cOut.WriteLine();
        }
    }
}
