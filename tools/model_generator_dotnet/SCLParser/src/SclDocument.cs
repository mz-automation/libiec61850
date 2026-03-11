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
using System.Linq;
using System.Reflection;
using System.Text;
using System.Xml;
using System.Xml.Schema;
using DataSet = IEC61850.SCL.DataModel.DataSet;

namespace IEC61850.SCL
{

    public class SclParserException : Exception
    {
        private XmlNode xmlNode;

        public XmlNode XmlNode
        {
            get
            {
                return xmlNode;
            }
        }

        public SclParserException(XmlNode xmlNode, string message)
            : base(message)
        {
            this.xmlNode = xmlNode;
        }
    }

    public class SclFileIssue
    {
        public string Severity { get; set; }

        public int Line { get; set; }

        public string Type { get; set; }

        public string Issue { get; set; }

        public object Object { get; set; }

        public string ObjectIssue { get; set; }

        public int Index { get; set; }

        public override string ToString()
        {
            return "Severity: " + Severity + "| Line: " + Line.ToString() + " | Type: " + Type + " | Issue: " + Issue;
        }

    }

    public class SclDocument
    {
        public const string SCL_XMLNS = "http://www.iec.ch/61850/2003/SCL";
        private string filename = null;
        private List<SclFileIssue> sclFileIssues = new List<SclFileIssue>();
        private List<SclValidatorMessage> messages = new List<SclValidatorMessage>();
        private PositionXmlDocument doc;

        public void InitializePositionDoc()
        {
            using (var reader = new XmlTextReader(filename))
            {
                doc = new PositionXmlDocument();
                doc.Load(reader);
            }
        }

        public PositionXmlElement GetXmlNodePosition(XmlNode xmlNode)
        {
            try
            {
                if (doc == null)
                    InitializePositionDoc();

                string xpath = FindXPath(xmlNode);
                var node = doc.SelectSingleNode(xpath, nsManager);

                return (PositionXmlElement)node;
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
                return null;
            }

        }

        static string FindXPath(XmlNode node)
        {
            StringBuilder builder = new StringBuilder();
            while (node != null)
            {
                switch (node.NodeType)
                {
                    case XmlNodeType.Attribute:
                        builder.Insert(0, "/@" + node.Name);
                        node = ((XmlAttribute)node).OwnerElement;
                        break;
                    case XmlNodeType.Element:

                        int index = FindElementIndex((XmlElement)node);

                        if (node.ParentNode.Name == "SCL")
                        {
                            builder.Insert(0, "//scl:" + node.Name + "[" + index + "]");
                            return builder.ToString();
                        }
                        else
                        {
                            builder.Insert(0, "/scl:" + node.Name + "[" + index + "]");
                            node = node.ParentNode;
                        }

                        break;
                    case XmlNodeType.Document:
                        return builder.ToString();
                    default:
                        throw new ArgumentException("Only elements and attributes are supported");
                }
            }
            throw new ArgumentException("Node was not in a document");
        }

        static int FindElementIndex(XmlElement element)
        {
            XmlNode parentNode = element.ParentNode;
            if (parentNode is XmlDocument)
            {
                return 1;
            }
            XmlElement parent = (XmlElement)parentNode;
            int index = 1;
            foreach (XmlNode candidate in parent.ChildNodes)
            {
                if (candidate is XmlElement && candidate.Name == element.Name)
                {
                    if (candidate == element)
                    {
                        return index;
                    }
                    index++;
                }
            }
            throw new ArgumentException("Couldn't find element within parent");
        }

        public List<SclValidatorMessage> Messages
        {
            get
            {
                return messages;
            }
        }

        private XmlDocument sclDocument;
        private XmlNamespaceManager nsManager;

        public XmlNamespaceManager NsManager
        {
            get { return nsManager; }
        }

        private SclDataTypeTemplates dataTypeTemplates = null;

        private List<SclIED> ieds = null;
        private List<SclSubstation> substations = null;
        private SclHeader header = null;
        private SclCommunication communication = null;
        private string sclVersion = null;
        private string sclRevision = null;
        private string sclRelease = null;
        private static bool changed = false;



        public string SclVersion
        {
            get { return sclVersion; }

            set { XmlHelper.SetAttributeCreateIfNotExists(sclDocument, sclDocument.SelectSingleNode("//scl:SCL", nsManager), "version", value); sclVersion = value; }

        }

        public string SclRevision
        {
            get { return sclRevision; }

            set { XmlHelper.SetAttributeCreateIfNotExists(sclDocument, sclDocument.SelectSingleNode("//scl:SCL", nsManager), "revision", value); sclRevision = value; }

        }

        public string SclRelease
        {
            get { return sclRelease; }
        }

        XmlNodeChangedEventHandler handler = (sender, e) => changed = true;

        private List<string> schemaFiles = new List<string> {"SCL.xsd", "SCL_BaseSimpleTypes.xsd","SCL_BaseTypes.xsd", "SCL_Communication.xsd",
        "SCL_DataTypeTemplates.xsd","SCL_Enums.xsd","SCL_IED.xsd","SCL_Substation.xsd"};

        private string ProgramDirectory()
        {
            return Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
        }

        private void XmlDocValidation()
        {

            if (!changed)
            {
                XmlReaderSettings xmlSettings = new XmlReaderSettings();

                if (sclRevision != null)
                {
                    if (sclRevision == "B")
                    {
                        foreach (string schemaName in schemaFiles)
                        {
                            if (sclRelease == null)
                            {
                                xmlSettings.Schemas.Add("http://www.iec.ch/61850/2003/SCL", ProgramDirectory() + "\\IEC_61850-6.2009.SCL.2007B\\" + schemaName);

                            }
                            else if (sclRelease == "4")
                            {
                                xmlSettings.Schemas.Add("http://www.iec.ch/61850/2003/SCL", ProgramDirectory() + "\\IEC_61850-6.2018.SCL.2007B4\\" + schemaName);
                            }

                        }
                    }
                    else
                    {
                        foreach (string schemaName in schemaFiles)
                        {
                            xmlSettings.Schemas.Add("http://www.iec.ch/61850/2003/SCL", ProgramDirectory() + "\\IEC_61850-6.2018.SCL.2007A\\" + schemaName);
                        }

                    }
                }
                else
                {
                    foreach (string schemaName in schemaFiles)
                    {

                        if (sclRelease == null)
                        {
                            xmlSettings.Schemas.Add("http://www.iec.ch/61850/2003/SCL", ProgramDirectory() + "\\IEC_61850-6.2009.SCL.2007B\\" + schemaName);

                        }
                        else if (sclRelease == "4")
                        {
                            xmlSettings.Schemas.Add("http://www.iec.ch/61850/2003/SCL", ProgramDirectory() + "\\IEC_61850-6.2018.SCL.2007B4\\" + schemaName);

                        }

                    }

                }

                xmlSettings.Schemas.XmlResolver = new XmlUrlResolver();

                xmlSettings.ValidationType = ValidationType.Schema;

                xmlSettings.ValidationEventHandler += new ValidationEventHandler(delegate (object sender, ValidationEventArgs e)
                {

                    if (e.Exception != null)
                        messages.Add(new SclValidatorMessage(e.Severity, e.Message, e.Exception.LineNumber, e.Exception.LinePosition));
                    else
                        messages.Add(new SclValidatorMessage(e.Severity, e.Message));


                });

                try
                {
                    if (filename != null)
                    {
                        XmlReader scl = XmlReader.Create(filename, xmlSettings);

                        while (scl.Read())
                        {
                        }

                        scl.Dispose();
                    }

                }
                catch (XmlSchemaValidationException e)
                {

                    if (e.SourceObject != null)
                        Console.WriteLine(e.SourceObject.ToString());

                    Console.WriteLine(e.Message);

                    if (e.SourceSchemaObject != null)
                        Console.WriteLine(e.SourceSchemaObject.SourceUri);

                    Console.WriteLine(e.LineNumber);
                    Console.WriteLine(e.SourceUri);
                }
            }
            else
            {
                XmlDocument xmlDoc = sclDocument;

                xmlDoc.Schemas = new XmlSchemaSet();

                if (header != null)
                {
                    if (sclRevision == "B")
                    {
                        foreach (string schemaName in schemaFiles)
                        {
                            if (sclRelease == null)
                                xmlDoc.Schemas.Add("http://www.iec.ch/61850/2003/SCL", ProgramDirectory() + "\\IEC_61850-6.2009.SCL.2007B\\" + schemaName);
                            else if (sclRelease == "4")
                                xmlDoc.Schemas.Add("http://www.iec.ch/61850/2003/SCL", ProgramDirectory() + "\\IEC_61850-6.2018.SCL.2007B4\\" + schemaName);
                        }

                    }
                    else
                    {
                        foreach (string schemaName in schemaFiles)
                        {
                            xmlDoc.Schemas.Add("http://www.iec.ch/61850/2003/SCL", ProgramDirectory() + "\\IEC_61850-6.2018.SCL.2007A\\" + schemaName);

                        }

                    }
                }
                else
                {
                    foreach (string schemaName in schemaFiles)
                    {
                        if (sclRelease == null)
                            xmlDoc.Schemas.Add("http://www.iec.ch/61850/2003/SCL", ProgramDirectory() + "\\IEC_61850-6.2009.SCL.2007B\\" + schemaName);
                        else if (sclRelease == "4")
                            xmlDoc.Schemas.Add("http://www.iec.ch/61850/2003/SCL", ProgramDirectory() + "\\IEC_61850-6.2018.SCL.2007B4\\" + schemaName);
                    }
                }

                xmlDoc.Validate(new ValidationEventHandler(delegate (object sender, ValidationEventArgs e)
                {
                    if (e.Exception != null)
                        messages.Add(new SclValidatorMessage(e.Severity, e.Message, e.Exception.LineNumber, e.Exception.LinePosition));
                    else
                        messages.Add(new SclValidatorMessage(e.Severity, e.Message));

                }));
            }


        }

        public void AddIssue(XmlNode xmlNode, string Severity, string Type, string Issue, object Object, string ObjectIssue)
        {
            SclFileIssue sclFileIssue = new SclFileIssue();
            sclFileIssue.Issue = Issue;
            sclFileIssue.Severity = Severity;
            sclFileIssue.Type = Type;
            sclFileIssue.Object = Object;
            sclFileIssue.ObjectIssue = ObjectIssue;

            if (xmlNode != null)
            {
                PositionXmlElement positionXmlElement = GetXmlNodePosition(xmlNode);
                if (positionXmlElement != null)
                    sclFileIssue.Line = positionXmlElement.LineNumber;

            }

            if (!sclFileIssues.Exists(x => x.Issue == sclFileIssue.Issue && x.Severity == sclFileIssue.Severity && x.Type == sclFileIssue.Type && x.Object == sclFileIssue.Object &&
            x.ObjectIssue == sclFileIssue.ObjectIssue && x.Line == sclFileIssue.Line))
                sclFileIssues.Add(sclFileIssue);
        }

        public List<SclFileIssue> SclFileIssues
        {
            get { return sclFileIssues; }
        }

        public List<SclFileIssue> ValidationMessages()
        {
            List<SclFileIssue> validationMessages = new List<SclFileIssue>();

            messages = new List<SclValidatorMessage>();

            XmlDocValidation();

            foreach (SclValidatorMessage msg in messages)
            {
                validationMessages.Add(new SclFileIssue()
                {
                    Line = msg.LineNo,
                    Severity = msg.Level == System.Xml.Schema.XmlSeverityType.Error ? "ERROR" : "WARNING",
                    Type = "Schema",
                    Issue = msg.Message
                });
            }

            return validationMessages;


        }

        public XmlDocument XmlDocument
        {
            get
            {
                return sclDocument;
            }
            set
            {
                sclDocument = value;
            }
        }

        public List<SclIED> IEDs
        {
            get
            {
                return new List<SclIED>(ieds);
            }
        }

        public void CheckUsedDataTypes()
        {
            if (dataTypeTemplates != null)
                MarkAllDataTypesAsUnused();

            foreach (SclIED ied in ieds)
            {
                foreach (SclAccessPoint ap in ied.AccessPoints)
                {
                    if (ap.Server != null)
                    {
                        IEDDataModel iedModel = GetDataModel(ied.Name, ap.Name);

                        foreach (LogicalDevice ld in iedModel.LogicalDevices)
                        {
                            foreach (LogicalNode ln in ld.LogicalNodes)
                            {
                                if (dataTypeTemplates != null)
                                {
                                    SclLNodeType sclLNodeType = dataTypeTemplates.LNTypes.Find(x => x.Id == ln.SclElement.LnType);

                                    CheckUsedEnumtypes(ln);

                                    if (sclLNodeType != null)
                                    {
                                        sclLNodeType.IsUsed = true;
                                        if (!sclLNodeType.UsedOn.Contains(ln))
                                            sclLNodeType.UsedOn.Add(ln);
                                    }

                                    foreach (DataObject dobj in ln.DataObjects)
                                    {
                                        CheckUsedDataObject(dobj);
                                    }
                                }

                            }
                        }
                    }
                }
            }
        }

        private void CheckUsedEnumtypes(LogicalNode logicalNode)
        {
            foreach (SclDOI sclDOI_ in logicalNode.SclElement.DOIs)
            {
                CheckUsedPredefinedValuesEnumType(sclDOI_, logicalNode, null);
            }
        }

        private void CheckUsedPredefinedValuesEnumType(Object Object, LogicalNode logicalNode, SclDOI sclDOI_)
        {
            if (Object is SclDOI sclDOI)
            {
                if (sclDOI.SclDAIs.Count > 0)
                {
                    foreach (SclDAI sclDAI in sclDOI.SclDAIs)
                    {
                        foreach (SclVal sclVal in sclDAI.GetValues())
                        {
                            DataAttribute dataAttribute = GetDataAttribute(logicalNode, sclDOI, sclDAI);
                            if (dataAttribute != null)
                            {
                                if (sclVal.Value != null)
                                {
                                    if (dataAttribute.Definition.AttributeType == AttributeType.ENUMERATED)
                                    {

                                        string EnumType = dataAttribute.Definition.Type;
                                        if (EnumType != null)
                                        {
                                            SclEnumType sclEnumType = DataTypeTemplates.GetEnumType(EnumType);
                                            if (sclEnumType != null)
                                            {
                                                sclEnumType.IsUsed = true;
                                                if (!sclEnumType.UsedOn.Contains(dataAttribute))
                                                    sclEnumType.UsedOn.Add(dataAttribute);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }

                }

                if (sclDOI.SclSDIs.Count > 0)
                {
                    foreach (SclSDI sclSDI in sclDOI.SclSDIs)
                        CheckUsedPredefinedValuesEnumType(sclSDI, logicalNode, sclDOI);
                }
            }

            else if (Object is SclSDI)
            {
                SclSDI sclSDI = Object as SclSDI;

                if (sclSDI.SclSDIs.Count > 0)
                {
                    foreach (SclSDI sclSDI_ in sclSDI.SclSDIs)
                        CheckUsedPredefinedValuesEnumType(sclSDI_, logicalNode, sclDOI_);
                }
            }
        }

        private void CheckUsedDataObject(DataObject dataObject)
        {
            SclDOType sclDOType = dataTypeTemplates.DOTypes.Find(x => x == dataObject.DOType);

            if (sclDOType != null)
            {
                sclDOType.IsUsed = true;
                if (!sclDOType.UsedOn.Contains(dataObject))
                    sclDOType.UsedOn.Add(dataObject);

                CheckUsedSubDataObjectsAndDataAttributes(sclDOType);
            }
        }

        private void CheckUsedSubDataObjectsAndDataAttributes(SclDOType type)
        {
            if (type.SubDataObjects != null)
            {
                foreach (SclDataObjectDefinition doDef in type.SubDataObjects)
                {
                    SclDOType doType = DataTypeTemplates?.GetDOType(doDef.Type);

                    if (doType != null)
                    {
                        doType.IsUsed = true;

                        CheckUsedSubDataObjectsAndDataAttributes(doType);

                    }

                }
            }

            if (type.DataAttributes != null)
            {

                foreach (SclDataAttributeDefinition daDef in type.DataAttributes)
                {
                    if (daDef.AttributeType == AttributeType.CONSTRUCTED)
                    {
                        SclDAType daType = DataTypeTemplates?.GetDAType(daDef.Type);

                        if (daType != null)
                        {
                            daType.IsUsed = true;

                            CheckUsedSubDataAttributes(daDef.Fc, daType);
                        }


                    }
                    else if (daDef.AttributeType == AttributeType.ENUMERATED)
                    {
                        SclEnumType enumType = DataTypeTemplates?.GetEnumType(daDef.Type);

                        if (enumType != null)
                            enumType.IsUsed = true;
                    }
                }

            }

        }

        private void CheckUsedSubDataAttributes(SclFC fc, SclDAType daType)
        {
            foreach (SclDataAttributeDefinition daDef in daType.SubDataAttributes)
            {

                SclFC newFc;

                if (daDef.Fc != SclFC.NONE)
                    newFc = daDef.Fc;
                else
                    newFc = fc;


                if (daDef.AttributeType == AttributeType.CONSTRUCTED)
                {
                    SclDAType subDaType = DataTypeTemplates?.GetDAType(daDef.Type);

                    if (subDaType != null)
                    {
                        subDaType.IsUsed = true;

                        //CheckUsedSubDataAttributes(newFc, subDaType);// check this statement
                    }

                }
                else
                {
                    if (daDef.AttributeType == AttributeType.ENUMERATED)
                    {
                        SclEnumType enumType = DataTypeTemplates?.GetEnumType(daDef.Type);

                        if (enumType == null)
                            enumType.IsUsed = true;
                    }
                }
            }
        }

        private void MarkAllDataTypesAsUnused()
        {
            foreach (SclType type in dataTypeTemplates.AllTypes)
            {
                type.IsUsed = false;
                type.UsedOn.Clear();
            }
        }

        public SclDataTypeTemplates DataTypeTemplates
        {
            get
            {
                return dataTypeTemplates;
            }
            set
            {
                dataTypeTemplates = value;
            }
        }

        public void Remove(SclIED ied)
        {
            XmlNode parent = ied.xmlNode.ParentNode;

            if (parent != null)
            {
                parent.RemoveChild(ied.xmlNode);
            }

            ieds.Remove(ied);
        }

        public void RemoveSubstation(SclSubstation sclSubstation)
        {
            XmlNode parent = sclSubstation.xmlNode.ParentNode;

            if (parent != null)
            {
                parent.RemoveChild(sclSubstation.xmlNode);
            }

            substations.Remove(sclSubstation);
        }


        public List<SclSubstation> Substations
        {
            get
            {
                return new List<SclSubstation>(substations);
            }
        }

        public void Remove(SclSubstation substation)
        {
            XmlNode parent = substation.xmlNode.ParentNode;

            if (parent != null)
            {
                parent.RemoveChild(substation.xmlNode);
            }

            substations.Remove(substation);
        }

        public bool HasDataTypeTemplates() => (dataTypeTemplates != null);

        public SclHeader Header
        {
            get
            {
                return header;
            }
        }

        public SclCommunication Communication
        {
            get
            {
                return communication;
            }
        }

        public string Filename
        {
            get
            {
                return filename;
            }
            set
            {
                filename = value;
            }

        }

        public void Save(string filename)
        {
            if (filename != null)
            {
                sclDocument.Save(filename);

                changed = false;
            }

        }

        private void ParseDataTypeTemplatesSection()
        {
            XmlNode dttNode = sclDocument.SelectSingleNode("//scl:DataTypeTemplates", nsManager);

            if (dttNode != null)
            {
                dataTypeTemplates = new SclDataTypeTemplates(this, dttNode);
            }
        }

        public SclHeader AddHeader()
        {
            if (header == null)
            {
                SclHeader newheader = new SclHeader(this, NsManager);

                XmlNode newNode = newheader.XmlNode;

                if (newNode.OwnerDocument != sclDocument)
                {
                    newNode = sclDocument.ImportNode(newheader.XmlNode.CloneNode(true), true);
                }

                if (substations.Count > 0)
                {
                    XmlNode parent = substations[0].xmlNode.ParentNode;

                    parent.InsertBefore(newNode, substations[0].xmlNode);
                }

                else if (communication != null)
                {
                    XmlNode parent = communication.xmlNode.ParentNode;

                    parent.InsertAfter(newNode, communication.xmlNode);
                }

                else if (ieds.Count > 0)
                {
                    XmlNode parent = ieds[0].xmlNode.ParentNode;

                    parent.InsertBefore(newNode, ieds[0].xmlNode);
                }

                else if (dataTypeTemplates != null)
                {
                    XmlNode parent = dataTypeTemplates.xmlNode.ParentNode;

                    parent.InsertBefore(newNode, dataTypeTemplates.xmlNode);
                }

                else
                {
                    XmlNode parent = sclDocument.SelectSingleNode("//scl:SCL", nsManager);
                    parent.AppendChild(newNode);

                }

                try
                {
                    header = new SclHeader(this, newNode, NsManager);
                    return header;

                }
                catch (SclParserException e)
                {
                    Console.WriteLine("Failed to add Header");
                    Console.WriteLine(e.ToString());

                    return null;
                }
            }
            else
                return null;
        }

        public bool DeleteHeader()
        {
            if (header != null)
            {
                XmlNode parent = header.XmlNode.ParentNode;

                if (parent != null)
                {
                    parent.RemoveChild(header.XmlNode);
                }

                header = null;

                return true;
            }

            return false;
        }

        public bool DeleteDataTypeTemplates()
        {
            if (dataTypeTemplates != null)
            {
                XmlNode parent = dataTypeTemplates.XmlNode.ParentNode;

                if (parent != null)
                {
                    parent.RemoveChild(dataTypeTemplates.XmlNode);
                }

                dataTypeTemplates = null;

                return true;
            }

            return false;
        }

        private void ParseIEDSections()
        {
            XmlNodeList iedNodes = sclDocument.SelectNodes("//scl:SCL/scl:IED", nsManager);

            if (iedNodes.Count < 1)
                AddIssue(null, "ERROR", "Model integrity", "SCL contains no IED element", this, "IED");

            foreach (XmlNode ied in iedNodes)
            {
                ieds.Add(new SclIED(this, ied, nsManager));
            }

            if (Communication != null)
            {
                List<SclSubNetwork> subNetworks = communication.GetSubNetworks();

                if (subNetworks != null)
                {
                    foreach (SclSubNetwork subnetwork in subNetworks)
                    {
                        List<SclConnectedAP> connectedAPs = subnetwork.GetConnectedAPs();

                        if (connectedAPs != null)
                        {
                            foreach (SclConnectedAP connectedAP in connectedAPs)
                            {
                                SclIED connectedAP_IED = ieds.Find(x => x.Name == connectedAP.IedName);

                                if (connectedAP_IED == null)
                                {
                                    AddIssue(connectedAP.xmlNode, "ERROR", "Model integrity", "IED " + connectedAP.IedName + " in ConnectedAP doesn't exist", this, "connectedAP");
                                    AddIssue(connectedAP.xmlNode, "ERROR", "Model integrity", "Access Point " + connectedAP.ApName + " in ConnectedAP doesn't exist", this, "connectedAP");
                                }
                                else
                                {
                                    if (connectedAP_IED.AccessPoints.Find(x => x.Name == connectedAP.ApName) == null)
                                    {
                                        AddIssue(connectedAP.xmlNode, "ERROR", "Model integrity", "Access Point " + connectedAP.ApName + " in ConnectedAP doesn't exist", this, "connectedAP");
                                    }
                                }
                            }
                        }
                    }
                }
            }

        }

        private void ParseSubstationSections()
        {
            XmlNodeList substationNodes = sclDocument.SelectNodes("//scl:SCL/scl:Substation", nsManager);

            foreach (XmlNode substationNode in substationNodes)
            {
                substations.Add(new SclSubstation(sclDocument, this, substationNode, nsManager));
            }
        }

        private void ParseScl()
        {
            XmlNode sclVersion = sclDocument.SelectSingleNode("//scl:SCL", nsManager);
            if (sclVersion != null)
            {
                this.sclVersion = XmlHelper.GetAttributeValue(sclVersion, "version");
                sclRevision = XmlHelper.GetAttributeValue(sclVersion, "revision");
                sclRelease = XmlHelper.GetAttributeValue(sclVersion, "release");

            }
        }

        private void ParseHeaderSection()
        {
            XmlNode headerNode = sclDocument.SelectSingleNode("//scl:Header", nsManager);

            if (headerNode == null)
                AddIssue(null, "ERROR", "Model integrity", "Header is missing", this, "Header");
            else
                header = new SclHeader(this, headerNode, nsManager);
        }

        private void ParseCommunicationSection()
        {
            XmlNode communicationNode = sclDocument.SelectSingleNode("//scl:Communication", nsManager);

            if (communicationNode != null)
                communication = new SclCommunication(communicationNode, this, nsManager);
        }

        private void ParseDocument()
        {
            ieds = new List<SclIED>();
            substations = new List<SclSubstation>();
            header = null;
            communication = null;

            ParseScl();

            ParseHeaderSection();

            ParseSubstationSections();

            ParseCommunicationSection();

            ParseDataTypeTemplatesSection();

            ParseIEDSections();
        }

        /// <summary>
        /// Rebuild the internal document structure by the DOM model
        /// </summary>
        public void Rebuild()
        {
            ParseDocument();
        }

        /// <summary>
        /// Removes the "Private" elements from the SCL document.
        /// </summary>
        public void RemovePrivateElements()
        {
            XmlNodeList privateNodes = sclDocument.SelectNodes("//scl:Private", nsManager);

            foreach (XmlNode privateNode in privateNodes)
            {
                XmlNode parent = privateNode.ParentNode;

                if (parent != null)
                {
                    parent.RemoveChild(privateNode);
                }
            }
        }

        private void InitializeDocument()
        {
            nsManager = new XmlNamespaceManager(sclDocument.NameTable);
            nsManager.AddNamespace("scl", SCL_XMLNS);

            ParseDocument();
        }

        public SclDocument(XmlDocument xmlDocument)
        {
            sclDocument = xmlDocument;

            InitializeDocument();
        }

        public SclDocument(string filePath)
        {
            Filename = filePath;
            sclDocument = new XmlDocument();

            sclDocument.Load(filePath);

            InitializeDocument();

            sclDocument.NodeChanged += handler;
            sclDocument.NodeInserted += handler;
            sclDocument.NodeRemoved += handler;
        }

        public List<string> GetIedNames()
        {
            List<string> iedNames = new List<string>();

            foreach (SclIED ied in ieds)
                iedNames.Add(ied.Name);

            return iedNames;
        }

        private SclIED GetIedByName(string iedName)
        {
            foreach (SclIED ied in ieds)
            {
                if (ied.Name.Equals(iedName))
                    return ied;
            }

            return null;
        }

        private void AddSubDataAttributesToDataAttribute(DataAttribute da, SclFC fc, SclDAType daType)
        {
            foreach (SclDataAttributeDefinition daDef in daType.SubDataAttributes)
            {

                SclFC newFc;

                if (daDef.Fc != SclFC.NONE)
                    newFc = daDef.Fc;
                else
                    newFc = fc;

                DataAttribute subDataAttribute = new DataAttribute(daDef.Name, da, newFc, daDef.AttributeType, daDef.Count, daDef);

                if (daDef.AttributeType == AttributeType.CONSTRUCTED)
                {
                    SclDAType subDaType = DataTypeTemplates?.GetDAType(daDef.Type);

                    if (subDaType == null)
                        AddIssue(daDef.XmlNode, "ERROR", "Model integrity", "DAType \"" + daDef.Type + "\" of DA \"" + daDef.Name + "\" is not defined", this, "DAType");
                    else
                    {
                        subDaType.IsUsed = true;
                        AddSubDataAttributesToDataAttribute(subDataAttribute, newFc, subDaType);
                    }

                }
                else
                {
                    if (daDef.AttributeType == AttributeType.ENUMERATED)
                    {
                        SclEnumType enumType = DataTypeTemplates?.GetEnumType(daDef.Type);

                        if (enumType == null)
                            AddIssue(daDef.XmlNode, "ERROR", "Model integrity", "EnumType \"" + daDef.Type + "\" of DA \"" + daDef.Name + "\" is not defined", this, "EnumType");
                        else
                            enumType.IsUsed = true;
                    }
                }

                da.SubDataAttributes.Add(subDataAttribute);
            }
        }

        private void AddSubDataObjectsAndDataAttributesToDataObject(DataObject dataObject, SclDOType type)
        {

            if (type.SubDataObjects != null)
            {
                foreach (SclDataObjectDefinition doDef in type.SubDataObjects)
                {

                    SclDOType doType = DataTypeTemplates?.GetDOType(doDef.Type);

                    if (doType == null)
                        AddIssue(doDef.XmlNode, "ERROR", "Model integrity", "DOType \"" + doDef.Type + "\" of DO \"" + doDef.Name + "\" is not defined", this, "DOType");
                    else
                    {
                        doType.IsUsed = true;

                        DataObject subDataObject = new DataObject(doDef, doType, dataObject);
                        AddSubDataObjectsAndDataAttributesToDataObject(subDataObject, doType);// EXCEPTION WHEN ADDING NEW LN

                        dataObject.DataObjectsAndAttributes.Add(subDataObject);
                    }

                }
            }

            if (type.DataAttributes != null)
            {

                foreach (SclDataAttributeDefinition daDef in type.DataAttributes)
                {

                    DataAttribute da = new DataAttribute(daDef.Name, dataObject, daDef.Fc, daDef.AttributeType, daDef.Count, daDef);

                    if (daDef.AttributeType == AttributeType.CONSTRUCTED)
                    {
                        SclDAType daType = DataTypeTemplates?.GetDAType(daDef.Type);

                        if (daType == null)
                            AddIssue(daDef.XmlNode, "ERROR", "Model integrity", "DAType \"" + daDef.Type + "\" of DA \"" + daDef.Name + "\" is not defined", this, "DAType");
                        else
                        {
                            daType.IsUsed = true;
                            if (!da.ObjRef.EndsWith(daDef.Name + "." + daDef.Name))
                                AddSubDataAttributesToDataAttribute(da, daDef.Fc, daType);
                        }


                    }
                    else if (daDef.AttributeType == AttributeType.ENUMERATED)
                    {
                        SclEnumType enumType = DataTypeTemplates?.GetEnumType(daDef.Type);

                        if (enumType == null)
                            AddIssue(daDef.XmlNode, "ERROR", "Model integrity", "EnumType \"" + daDef.Type + "\" of DA \"" + daDef.Name + "\" is not defined", this, "EnumType");
                        else
                            enumType.IsUsed = true;
                    }

                    dataObject.DataObjectsAndAttributes.Add(da);
                }

            }

        }

        public void AddDataObjectsToLogicalNode(LogicalNode logicalNode, SclLN ln)
        {
            SclLNodeType lnType = DataTypeTemplates?.GetLNodeType(ln.LnType);

            if (lnType == null)
                AddIssue(ln.xmlNode, "ERROR", "Model integrity", "LNType \"" + ln.LnType + "\" of LN \"" + logicalNode.ObjRef + "\" is not defined", this, "LNType");
            else
            {
                lnType.IsUsed = true;

                if (logicalNode.DataObjects.Count > 0)
                    logicalNode.DataObjects.Clear();

                foreach (SclDataObjectDefinition doDef in lnType.DataObjects)
                {

                    SclDOType doType = DataTypeTemplates?.GetDOType(doDef.Type);

                    if (doType == null)
                        AddIssue(doDef.XmlNode, "ERROR", "Model integrity", "DOType \"" + doDef.Type + "\" of DO \"" + doDef.Name + "\" is not defined", this, "DOType");
                    else
                    {
                        doType.IsUsed = true;

                        DataObject dobject = new DataObject(doDef, doType, logicalNode);
                        AddSubDataObjectsAndDataAttributesToDataObject(dobject, doType);

                        logicalNode.DataObjects.Add(dobject);
                    }


                }
            }


        }

        private void AddLogicalNodesToLogicalDevice(LogicalDevice logicalDevice, SclLDevice lDevice)
        {
            if (lDevice.LogicalNodes != null)
            {
                foreach (SclLN ln in lDevice.LogicalNodes)
                {

                    LogicalNode logicalNode = new LogicalNode(ln, logicalDevice);

                    AddDataObjectsToLogicalNode(logicalNode, ln);

                    logicalDevice.LogicalNodes.Add(logicalNode);

                    CheckIfControlExistForDOIandDAI(logicalNode);

                    //CheckIfFcdaExistsOnDataTemplates(logicalNode);

                    if (logicalNode.LnClass == "LLN0")
                        CheckIf_LLN0NamPltconfigRevExist(logicalNode);

                    CheckDANameSpaces(logicalNode);
                }
            }
        }

        private List<string> NSDsNameSpace = new List<string>() { "IEC_61850-7-4", "IEC_61850-7-4_2003", "IEC_61850-7-4_2007","IEC_61850-7-4_2007A",
            "IEC_61850-7-4_2007B","IEC_61850-7-2_2007A3", "IEC_61850-7-2_2007B3", "IEC_61850-7-3_2007A3", "IEC_61850-7-3_2007B3",
        "IEC_61850-7-4_2007A3", "IEC_61850-7-4_2007B3", "IEC_61850-7-420_2019A4", "IEC_61850-7-420_2019A", "IEC_61850-8-1_2003A2"};

        private void CheckDANameSpaces(LogicalNode logicalNode)
        {
            string ldNsValue = null;
            bool ldNsFound = false;
            string lnn0NsValue = null;
            bool lnn0Found = false;
            string lnNsValue = null;
            bool lnNsFound = false;
            LogicalDevice logicalDevice = logicalNode.Parent as LogicalDevice;

            if (logicalNode.LnClass == "LLN0")
            {
                SclDOI sclDOI = logicalNode.SclElement.DOIs.Find(x => x.Name == "NamPlt");

                if (sclDOI != null)
                {
                    SclDAI sclDAIldNs = sclDOI.SclDAIs.Find(x => x.Name == "ldNs");

                    if (sclDAIldNs != null)
                    {
                        ldNsFound = true;
                        string value = sclDAIldNs.Val;

                        if (value != null)
                        {
                            ldNsValue = value;
                        }
                    }

                    SclDAI sclDAIlnNs = sclDOI.SclDAIs.Find(x => x.Name == "lnNs");

                    if (sclDAIlnNs != null)
                    {
                        lnn0Found = true;
                        string value = sclDAIlnNs.Val;

                        if (value != null)
                        {
                            lnn0NsValue = value;
                        }
                    }
                }

                if (ldNsValue == null)
                {
                    DataObject dataObject = logicalNode.DataObjects.Find(x => x.Name == "NamPlt" && x.DOType.Cdc == "LPL");

                    if (dataObject != null)
                    {
                        SclDataAttributeDefinition daLdNs = dataObject.DOType.DataAttributes.Find(x => x.Name == "ldNs" && x.Fc == SclFC.EX);

                        if (daLdNs != null)
                        {
                            ldNsFound = true;
                            SclVal sclVal = daLdNs.GetVal();
                            if (sclVal != null)
                            {
                                ldNsValue = sclVal.Value;
                            }

                        }
                    }
                }

                if (ldNsValue != null)
                {
                    try
                    {
                        logicalDevice.NameSpace = new Namespace(ldNsValue);

                        if (!NSDsNameSpace.Contains(logicalDevice.NameSpace.NsdPath))
                        {
                            logicalDevice.NameSpace.NsdPathFound = false;
                            AddIssue(logicalNode.SclElement.XmlNode, "WARNING", "Model integrity", "Namespace on " + logicalDevice.Name + "/" + logicalNode.Name + ".NamPlt.ldNs not found on NSD files. Value: " + ldNsValue, this, "LLN0.NamPlt.ldNs");
                        }
                    }
                    catch (Exception)
                    {
                        AddIssue(logicalNode.SclElement.XmlNode, "WARNING", "Model integrity", "Namespace on " + logicalDevice.Name + "/" + logicalNode.Name + ".NamPlt.ldNs not found on NSD files. Value: " + ldNsValue, this, "LLN0.NamPlt.ldNs");
                    }
                }

                if (ldNsValue == null && ldNsFound)
                {
                    AddIssue(logicalNode.SclElement.XmlNode, "ERROR", "Model integrity", "Namespace on " + logicalDevice.Name + "/" + logicalNode.Name + ".NamPlt.ldNs has no value", this, "LLN0.NamPlt.ldNs");
                }

                if (lnn0NsValue == null)
                {
                    DataObject dataObject = logicalNode.DataObjects.Find(x => x.Name == "NamPlt" && x.DOType.Cdc == "LPL");

                    if (dataObject != null)
                    {
                        SclDataAttributeDefinition daLnNs = dataObject.DOType.DataAttributes.Find(x => x.Name == "lnNs" && x.Fc == SclFC.EX);

                        if (daLnNs != null)
                        {
                            lnn0Found = true;
                            SclVal sclVal = daLnNs.GetVal();

                            if (sclVal != null)
                            {
                                lnn0NsValue = sclVal.Value;
                            }

                        }
                    }


                }

                if (lnn0NsValue != null)
                {
                    try
                    {

                        logicalNode.NameSpace = new Namespace(lnn0NsValue);

                        if (!NSDsNameSpace.Contains(logicalNode.NameSpace.NsdPath))
                        {
                            logicalNode.NameSpace.NsdPathFound = false;

                            AddIssue(logicalNode.SclElement.XmlNode, "WARNING", "Model integrity", "Namespace on " + logicalDevice.Name + "/" + logicalNode.Name + ".NamPlt.lnNs not found on NSD files. Value: " + lnn0NsValue, this, "LLN0.NamPlt.lnNs");
                        }
                    }
                    catch (Exception)
                    {
                        AddIssue(logicalNode.SclElement.XmlNode, "WARNING", "Model integrity", "Namespace on " + logicalDevice.Name + "/" + logicalNode.Name + ".NamPlt.lnNs not found on NSD files. Value: " + lnn0NsValue, this, "LLN0.NamPlt.lnNs");
                    }
                }

                if (lnn0NsValue == null && lnn0Found)
                {
                    AddIssue(logicalNode.SclElement.XmlNode, "ERROR", "Model integrity", "Namespace on " + logicalDevice.Name + "/" + logicalNode.Name + ".NamPlt.lnNs has no value", this, "LLN0.NamPlt.lnNs");
                }


            }
            else
            {
                SclDOI sclDOI = logicalNode.SclElement.DOIs.Find(x => x.Name == "NamPlt");

                if (sclDOI != null)
                {
                    SclDAI sclDAIlnNs = sclDOI.SclDAIs.Find(x => x.Name == "lnNs");

                    if (sclDAIlnNs != null)
                    {
                        lnNsFound = true;
                        string value = sclDAIlnNs.Val;

                        if (value != null)
                        {
                            lnNsValue = value;
                        }
                    }
                }

                if (lnNsValue == null)
                {
                    DataObject dataObject = logicalNode.DataObjects.Find(x => x.Name == "NamPlt" && x.DOType.Cdc == "LPL");

                    if (dataObject != null)
                    {
                        //Attribute for logical device basic namespace (LNName.NamPlt.lnNs)
                        SclDataAttributeDefinition daLnNs = dataObject.DOType.DataAttributes.Find(x => x.Name == "lnNs" && x.Fc == SclFC.EX);

                        if (daLnNs != null)
                        {
                            lnNsFound = true;
                            SclVal sclVal = daLnNs.GetVal();

                            if (sclVal != null)
                            {
                                lnNsValue = sclVal.Value;

                            }
                        }
                    }
                }

                if (lnNsValue != null)
                {
                    try
                    {
                        logicalNode.NameSpace = new Namespace(lnNsValue);

                        if (!NSDsNameSpace.Contains(logicalNode.NameSpace.NsdPath))
                        {
                            logicalNode.NameSpace.NsdPathFound = false;

                            AddIssue(logicalNode.SclElement.XmlNode, "WARNING", "Model integrity", "Namespace on " + logicalDevice.Name + "/" + logicalNode.Name + ".NamPlt.lnNs not found on NSD files. Value: " + lnNsValue, this, "LLN0.NamPlt.lnNs");
                        }
                    }
                    catch (Exception)
                    {
                        AddIssue(logicalNode.SclElement.XmlNode, "WARNING", "Model integrity", "Namespace on " + logicalDevice.Name + "/" + logicalNode.Name + ".NamPlt.lnNs not found on NSD files. Value: " + lnNsValue, this, "LLN0.NamPlt.lnNs");
                    }
                }

                if (lnNsValue == null && lnNsFound)
                {
                    AddIssue(logicalNode.SclElement.XmlNode, "ERROR", "Model integrity", "Namespace on " + logicalDevice.Name + "/" + logicalNode.Name + ".NamPlt.lnNs has no value", this, "LLN0.NamPlt.ldNs");
                }
            }
        }

        private void CheckIf_LLN0NamPltconfigRevExist(LogicalNode LLNO)
        {
            bool LLN0NamPltconfigRev = false;
            DataObject dataObject = LLNO.DataObjects.Find(x => x.Name == "NamPlt");
            if (dataObject != null)
            {
                SclDataAttributeDefinition da = dataObject.DOType.DataAttributes.Find(x => x.Name == "configRev");
                if (da != null)
                {
                    LLN0NamPltconfigRev = true;

                    if (da.GetVal() != null)
                    {
                        LogicalDevice logicalDevice = LLNO.Parent as LogicalDevice;
                        AddIssue(LLNO.SclElement.XmlNode, "ERROR", "Model integrity", "DataAttribute LLN0.NamPlt.configRev found on LD " + logicalDevice.Inst + " but has not value", this, "LLN0.NamPlt.configRev");
                    }
                    else
                    {
                        LLN0NamPltconfigRev = true;
                    }
                }
            }

            if (!LLN0NamPltconfigRev)
            {
                SclDOI sclDOI = LLNO.SclElement.DOIs.Find(x => x.Name == "NamPlt");
                if (sclDOI != null)
                {
                    SclDAI sclDAI = sclDOI.SclDAIs.Find(x => x.Name == "configRev");
                    if (sclDAI != null)
                    {
                        LLN0NamPltconfigRev = true;

                        if (sclDAI.Val == null)
                        {
                            LogicalDevice logicalDevice = LLNO.Parent as LogicalDevice;
                            AddIssue(LLNO.SclElement.XmlNode, "ERROR", "Model integrity", "DAI LLN0.NamPlt.configRev found on LD " + logicalDevice.Inst + " but has not value", this, "LLN0.NamPlt.configRev");

                        }


                    }
                }
            }

            if (!LLN0NamPltconfigRev)
            {
                LogicalDevice logicalDevice = LLNO.Parent as LogicalDevice;
                AddIssue(LLNO.SclElement.XmlNode, "ERROR", "Model integrity", "DA or DAI LLN0.NamPlt.configRev not found on LD " + logicalDevice.Inst, this, "LLN0.NamPlt.configRev");

            }
        }

        private void CheckIfFcdaExistsOnDataTemplates(LogicalNode logicalNode)
        {
            try
            {
                foreach (DataSet dataSet in logicalNode.DataSets)
                {
                    foreach (SclFCDA sclFCDA in dataSet.SclDataSet.Fcdas)
                    {
                        LogicalDevice logicalDevice = logicalNode.Parent as LogicalDevice;
                        IEDDataModel iEDDataModel = logicalDevice.Parent as IEDDataModel;
                        LogicalDevice fcdaLD = iEDDataModel.LogicalDevices.Find(x => x.Inst == sclFCDA.LdInst);

                        if (fcdaLD == null)
                        {
                            AddIssue(sclFCDA.xmlNode, "ERROR", "Model integrity", "FCDA " + sclFCDA.GetObjectReference() + " on DataSet " + dataSet.ObjRef + " not found on data type templates", this, "FCDA");
                            break;
                        }

                        string lnName = "";
                        if (sclFCDA.Prefix != null)
                            lnName += sclFCDA.Prefix;
                        if (sclFCDA.LnClass != null)
                            lnName += sclFCDA.LnClass;
                        if (sclFCDA.LnInst != null)
                            lnName += sclFCDA.LnInst;

                        LogicalNode fcdaLN = fcdaLD.LogicalNodes.Find(x => x.Name == lnName);
                        if (fcdaLN == null)
                        {
                            AddIssue(sclFCDA.xmlNode, "ERROR", "Model integrity", "FCDA " + sclFCDA.GetObjectReference() + " on DataSet " + dataSet.ObjRef + " not found on data type templates", this, "FCDA");
                            break;
                        }

                        SclLNodeType sclLNodeType = dataTypeTemplates.GetLNodeType(fcdaLN.SclElement.LnType);

                        if (sclLNodeType != null)
                        {

                            SclDataObjectDefinition dataObject = sclLNodeType.DataObjects.Find(x => x.Name == sclFCDA.DoName);

                            if (dataObject == null)
                            {
                                AddIssue(sclFCDA.xmlNode, "ERROR", "Model integrity", "FCDA " + sclFCDA.GetObjectReference() + " on DataSet " + dataSet.ObjRef + " not found on data type templates", this, "FCDA");
                            }
                            else
                            {
                                SclDOType sclDoType = dataTypeTemplates.GetDOType(dataObject.Type);

                                if (sclDoType == null)
                                {
                                    AddIssue(sclFCDA.xmlNode, "ERROR", "Model integrity", "FCDA " + sclFCDA.GetObjectReference() + " on DataSet " + dataSet.ObjRef + " not found on data type templates", this, "FCDA");
                                }

                                else
                                {
                                    if (sclFCDA.DaName != null)
                                    {

                                        SclDataAttributeDefinition sclDataAttributeDefinition = GetSclDataAttributeDefinition(sclDoType, sclFCDA.DaName);
                                        //SclDataAttributeDefinition sclDataAttributeDefinition = sclDoType.DataAttributes.Find(x => x.Name == sclFCDA.DaName);

                                        if (sclDataAttributeDefinition == null)
                                        {
                                            AddIssue(sclFCDA.xmlNode, "ERROR", "Model integrity", "FCDA " + sclFCDA.GetObjectReference() + " on DataSet " + dataSet.ObjRef + " not found on data type templates", this, "FCDA");

                                        }

                                    }

                                }

                            }
                        }
                        else
                        {
                            AddIssue(sclFCDA.xmlNode, "ERROR", "Model integrity", "FCDA " + sclFCDA.GetObjectReference() + " on DataSet " + dataSet.ObjRef + " not found on data type templates", this, "FCDA");

                        }

                    }
                }


            }
            catch (Exception)
            {

            }

        }

        private SclDataAttributeDefinition GetSclDataAttributeDefinition(object parent, string name)
        {

            SclDataAttributeDefinition sclDataAttributeDefinition = null;
            int index = name.IndexOf(".");

            if (index > 0)
            {
                string ParentName = name.Substring(0, index);

                SclDataAttributeDefinition parentDA;
                if (parent is SclDOType doType)
                {
                    parentDA = doType.DataAttributes.Find(x => x.Name == ParentName);
                }
                else
                {
                    parentDA = (parent as SclDAType).SubDataAttributes.Find(x => x.Name == ParentName);
                }

                SclDAType daType = dataTypeTemplates.GetDAType(parentDA.Type);

                string daName = name.Substring(index + 1);

                return GetSclDataAttributeDefinition(daType, daName);
            }
            else
            {
                if (parent is SclDOType doType)
                {
                    sclDataAttributeDefinition = doType.DataAttributes.Find(x => x.Name == name);
                }
                else
                {
                    sclDataAttributeDefinition = (parent as SclDAType).SubDataAttributes.Find(x => x.Name == name);
                }


            }

            return sclDataAttributeDefinition;
        }

        private void CheckIfControlExistForDOIandDAI(LogicalNode logicalNode)
        {
            foreach (SclDOI sclDOI_ in logicalNode.SclElement.DOIs)
                CheckPredefinedValuesExiste(sclDOI_, logicalNode, null, null, null);
        }

        private DataAttribute GetDataAttribute(LogicalNode logicalNode, SclDOI sclDOI, SclDAI sclDAI)
        {
            DataAttribute dataAttribute = null;
            foreach (DataObject dataObject in logicalNode.DataObjects)
            {
                if (dataObject.Name == sclDOI.Name)
                {
                    foreach (DataAttribute dataAttribute_ in GetDataAttribute(dataObject))
                    {
                        if (dataAttribute_.Name == sclDAI.Name)
                        {
                            dataAttribute = dataAttribute_;
                            break;
                        }

                    }
                    break;
                }
            }

            return dataAttribute;
        }

        private List<DataAttribute> GetDataAttribute(object Object)
        {
            List<DataAttribute> dataAttributes = new List<DataAttribute>();

            if (Object is DataObject)
            {
                DataObject dataObject = Object as DataObject;

                foreach (DataObjectOrAttribute DataObjectOrAttribute in dataObject.DataObjectsAndAttributes)
                {
                    foreach (DataAttribute attribute in GetDataAttribute(DataObjectOrAttribute))
                        dataAttributes.Add(attribute);
                }

            }
            else
            {
                DataAttribute dataAttribute_ = Object as DataAttribute;

                if (dataAttribute_.SubDataAttributes != null)
                {
                    foreach (DataAttribute subDataAttribute in dataAttribute_.SubDataAttributes)
                    {
                        foreach (DataAttribute attribute in GetDataAttribute(subDataAttribute))
                            dataAttributes.Add(attribute);
                    }
                }
                else
                    dataAttributes.Add(dataAttribute_);
            }

            return dataAttributes;
        }

        private readonly List<AttributeType> attributeTypeVisibleString = new List<AttributeType>
        {
            AttributeType.VISIBLE_STRING_32, AttributeType.VISIBLE_STRING_64,
            AttributeType.VISIBLE_STRING_65,AttributeType.VISIBLE_STRING_129,AttributeType.VISIBLE_STRING_255
        };

        private readonly List<AttributeType> attributeTypeInteger = new List<AttributeType>
        {
            AttributeType.INT8, AttributeType.INT16,
            AttributeType.INT32, AttributeType.INT64, AttributeType.INT8U, AttributeType.INT16U, AttributeType.INT24U, AttributeType.INT32U
        };

        private readonly List<AttributeType> attributeTypeFloat = new List<AttributeType>
        {
            AttributeType.FLOAT32,
            AttributeType.FLOAT64
        };

        public bool IsBase64String(string value)
        {
            if (value == null || value.Length == 0 || value.Length % 4 != 0
                || value.Contains(' ') || value.Contains('\t') || value.Contains('\r') || value.Contains('\n'))
                return false;

            var index = value.Length - 1;

            if (value[index] == '=')
                index--;
            if (value[index] == '=')
                index--;

            for (var i = 0; i <= index; i++)
                if (IsInvalid(value[i]))
                    return false;
            return true;
        }

        private bool IsInvalid(char value)
        {
            var intValue = (Int32)value;
            if (intValue >= 48 && intValue <= 57)
                return false;
            if (intValue >= 65 && intValue <= 90)
                return false;
            if (intValue >= 97 && intValue <= 122)
                return false;
            return intValue != 43 && intValue != 47;
        }

        private bool CheckValidDaiValue(string value, AttributeType attributeType)
        {
            if (attributeTypeInteger.Contains(attributeType) || attributeTypeFloat.Contains(attributeType))
            {
                if (attributeType == AttributeType.INT8)
                {
                    if (int.TryParse(value, out int i))
                    {
                        if (i >= -128 && i <= 127)
                            return true;
                        else
                            return false;
                    }
                    else
                        return false;
                }
                else if (attributeType == AttributeType.INT16)
                {
                    return Int16.TryParse(value, out Int16 i);
                }
                else if (attributeType == AttributeType.INT32)
                {
                    return Int32.TryParse(value, out Int32 i);
                }
                else if (attributeType == AttributeType.INT64)
                {
                    return Int64.TryParse(value, out Int64 i);
                }
                else if (attributeType == AttributeType.INT8U)
                {
                    if (int.TryParse(value, out int i))
                    {
                        if (i >= 0 && i <= 255)
                            return true;
                        else
                            return false;
                    }
                    else
                        return false;

                }
                else if (attributeType == AttributeType.INT16U)
                {
                    return UInt16.TryParse(value, out UInt16 i);
                }
                else if (attributeType == AttributeType.INT32U)
                {
                    return UInt32.TryParse(value, out UInt32 i);
                }
                else if (attributeType == AttributeType.FLOAT32 || attributeType == AttributeType.FLOAT64)
                {
                    return float.TryParse(value, out float i);
                }
                else
                    return false;

            }

            else
            {
                if (attributeType == AttributeType.OCTET_STRING_64)
                {
                    return IsBase64String(value);
                }
                else
                    return true;

            }
        }


        private void CheckPredefinedValuesExiste(Object Object, LogicalNode logicalNode, SclDOI sclDOI_, string sclSdiNames, DataObject dataObject)
        {
            if (Object is SclDOI sclDOI)
            {
                if (sclDOI.SclDAIs.Count > 0)
                {
                    foreach (SclDAI sclDAI in sclDOI.SclDAIs)
                    {
                        foreach (SclVal sclVal in sclDAI.GetValues())
                        {
                            DataAttribute dataAttribute = GetDataAttribute(logicalNode, sclDOI, sclDAI);
                            if (dataAttribute != null)
                            {
                                if (sclVal.Value != null)
                                {
                                    if (dataAttribute.Definition.AttributeType == AttributeType.ENUMERATED)
                                    {

                                        string EnumType = dataAttribute.Definition.Type;
                                        if (EnumType != null)
                                        {
                                            SclEnumType sclEnumType = DataTypeTemplates.GetEnumType(EnumType);
                                            if (sclEnumType != null)
                                            {
                                                if (!sclEnumType.EnumValues.Exists(x => x.SymbolicName == sclVal.Value))
                                                {
                                                    AddIssue(sclDAI.Node, "ERROR", "Model integrity", "Value " + sclVal.Value + " in DAI " + dataAttribute.ObjRef +
                                                    "  does not exist in enumerated type " + EnumType + ". Reload the file after fixing the problem!", this, "DAI_DataAttributeMissing");

                                                }
                                            }
                                            else
                                            {
                                                AddIssue(sclDAI.Node, "ERROR", "Model integrity", "Wrong ENUMERATED type " + EnumType + " in DAI " + dataAttribute.ObjRef
                                                   , this, "DAI_DataAttributeMissing");
                                            }

                                        }
                                        else
                                        {
                                            AddIssue(sclDAI.Node, "ERROR", "Model integrity", "Wrong ENUMERATED type in DAI " + dataAttribute.ObjRef, this, "DAI_DataAttributeMissing");
                                        }

                                    }
                                    else if (attributeTypeInteger.Contains(dataAttribute.Definition.AttributeType) ||
                                        attributeTypeFloat.Contains(dataAttribute.Definition.AttributeType) || dataAttribute.Definition.AttributeType == AttributeType.OCTET_STRING_64)
                                    {
                                        if (!CheckValidDaiValue(sclVal.Value, dataAttribute.Definition.AttributeType))
                                        {
                                            AddIssue(sclDAI.Node, "ERROR", "Model integrity", "Value " + sclVal.Value + " in DAI " + dataAttribute.ObjRef +
                                                    " wrong for attribute type " + dataAttribute.Definition.AttributeType.ToString() + ". Reload the file after fixing the problem!", this, "DAI_DataAttributeMissing");

                                        }

                                    }

                                }



                            }
                            else
                            {
                                AddIssue(sclDAI.Node, "ERROR", "Model integrity", "There is no DataAttribute definition for DAI " + sclDAI.Name + " on LogicalNode " + logicalNode.ObjRef, this, "DAI_DataAttributeMissing");
                            }
                        }
                    }

                }

                if (sclDOI.SclSDIs.Count > 0)
                {
                    foreach (SclSDI sclSDI in sclDOI.SclSDIs)
                        CheckPredefinedValuesExiste(sclSDI, logicalNode, sclDOI, null, dataObject);
                }
            }

            else if (Object is SclSDI)
            {
                SclSDI sclSDI = Object as SclSDI;
                string name = null;

                if (sclSdiNames != null)
                {
                    name = sclSdiNames + "." + sclSDI.Name;

                    if (sclSDI.Ix != null)
                        name += "[" + sclSDI.Ix + "]";
                }

                else
                {
                    name = sclSDI.Name;

                    if (sclSDI.Ix != null)
                        name += "[" + sclSDI.Ix + "]";
                }

                if (sclSDI.SclDAIs.Count > 0)
                {
                    foreach (SclDAI sclDAI in sclSDI.SclDAIs)
                    {
                        foreach (SclVal sclVal in sclDAI.GetValues())
                        {
                            DataAttribute dataAttribute = GetDataAttribute(logicalNode, sclDOI_, sclDAI);
                            if (dataAttribute != null)
                            {
                                //TODO check if values is correct
                            }
                            else
                            {
                                AddIssue(sclDAI.Node, "ERROR", "Model integrity", "There is no SubDataAttribute for DAI " + name + " on LogicalNode " + logicalNode.ObjRef, this, "DAI_DataAttributeMissing");
                            }
                        }


                    }

                }

                if (sclSDI.SclSDIs.Count > 0)
                {
                    foreach (SclSDI sclSDI_ in sclSDI.SclSDIs)
                        CheckPredefinedValuesExiste(sclSDI_, logicalNode, sclDOI_, name, dataObject);
                }
            }
        }



        private IEDDataModel CreateDataModel(string iedName, SclAccessPoint ap)
        {
            IEDDataModel dataModel = null;

            SclServer server = ap.Server;

            dataModel = new IEDDataModel(iedName);

            if (server != null)
            {
                List<SclLDevice> lDevices = server.LogicalDevices;

                if (lDevices != null)
                {
                    foreach (SclLDevice lDevice in lDevices)
                    {
                        string ldName = lDevice.LdName;

                        if (ldName == null)
                            ldName = iedName + lDevice.Inst;

                        LogicalDevice logicalDevice = new LogicalDevice(this, lDevice, ldName, lDevice.Inst, dataModel);

                        AddLogicalNodesToLogicalDevice(logicalDevice, lDevice);

                        dataModel.LogicalDevices.Add(logicalDevice);
                    }
                }

            }

            return dataModel;
        }

        public IEDDataModel GetDataModel(string iedName, string accessPointName)
        {
            IEDDataModel dataModel = null;

            if (iedName != null)
            {
                SclIED ied = GetIedByName(iedName);

                if (ied != null)
                {
                    foreach (SclAccessPoint ap in ied.AccessPoints)
                    {

                        if ((accessPointName == null) || ap.Name.Equals(accessPointName))
                        {
                            dataModel = CreateDataModel(iedName, ap);

                            if (dataModel != null)
                            {
                                foreach (LogicalDevice logicalDevice in dataModel.LogicalDevices)
                                {
                                    foreach (LogicalNode logicalNode in logicalDevice.LogicalNodes)
                                        CheckIfFcdaExistsOnDataTemplates(logicalNode);
                                }
                            }

                            break;
                        }
                    }
                }
            }


            return dataModel;
        }

        public SclConnectedAP GetConnectedAP(string apName, string iedName)
        {
            if (Communication != null)
                return communication.GetConnectedAP(apName, iedName);
            else
                return null;
        }
        public List<SclConnectedAP> GetConnectedAPs()
        {
            List<SclConnectedAP>  ca_list = new List<SclConnectedAP>();
            if (Communication != null)
            {
                foreach (SclSubNetwork sclSub in Communication.GetSubNetworks())
                {
                    ca_list.AddRange(sclSub.GetConnectedAPs());
                }
                return ca_list;
            }
            else
                return null;
        }
    }

    public class PositionXmlDocument : XmlDocument
    {
        IXmlLineInfo lineInfo; // a reference to the XmlReader, only set during load time

        /// <summary>
        /// Creates a PositionXmlElement.
        /// </summary>
        public override XmlElement CreateElement(string prefix, string localName, string namespaceURI)
        {
            return new PositionXmlElement(prefix, localName, namespaceURI, this, lineInfo);
        }

        /// <summary>
        /// Loads the XML document from the specified <see cref="XmlReader"/>.
        /// </summary>
        public override void Load(XmlReader reader)
        {
            lineInfo = reader as IXmlLineInfo;
            try
            {
                base.Load(reader);
            }
            finally
            {
                lineInfo = null;
            }
        }
    }

    public class PositionXmlElement : XmlElement, IXmlLineInfo
    {
        internal PositionXmlElement(string prefix, string localName, string namespaceURI, XmlDocument doc, IXmlLineInfo lineInfo)
            : base(prefix, localName, namespaceURI, doc)
        {
            if (lineInfo != null)
            {
                lineNumber = lineInfo.LineNumber;
                linePosition = lineInfo.LinePosition;
                hasLineInfo = true;
            }
        }

        int lineNumber;
        int linePosition;
        bool hasLineInfo;

        /// <summary>
        /// Gets whether the element has line information.
        /// </summary>
        public bool HasLineInfo()
        {
            return hasLineInfo;
        }

        /// <summary>
        /// Gets the line number.
        /// </summary>
        public int LineNumber
        {
            get { return lineNumber; }
        }

        /// <summary>
        /// Gets the line position (column).
        /// </summary>
        public int LinePosition
        {
            get { return linePosition; }
        }
    }

    public class Namespace
    {
        private string namespaceIdentifier = null;
        private string version = null;
        private string revision = null;
        private string release = null;
        private string nsdPath = null;
        private bool nsdPathFound = true;

        public Namespace(string fullNamespaceName)
        {
            try
            {
                string namespaceIdentifier_ = null;
                string version_ = null;
                string revision_ = null;
                string release_ = null;
                string nsdPath_ = null;

                if (fullNamespaceName != null)
                {
                    int index = fullNamespaceName.IndexOf(":");

                    if (index != -1)
                    {
                        namespaceIdentifier_ = fullNamespaceName.Substring(0, index);

                        version_ = fullNamespaceName.Substring(index + 1, 4);

                        if ((index + 5) < fullNamespaceName.Length)
                            revision_ = fullNamespaceName.Substring(index + 5, 1);

                        if (revision_ == null)
                            revision_ = "A";

                        if ((index + 6) < fullNamespaceName.Length)
                            release_ = fullNamespaceName.Substring(index + 6, fullNamespaceName.Length - index - 6);

                        nsdPath_ = fullNamespaceName.Replace(" ", "_");
                        nsdPath_ = nsdPath_.Replace(":", "_");
                    }

                    if (namespaceIdentifier_ != null)
                        namespaceIdentifier = namespaceIdentifier_;

                    if (version_ != null)
                        version = version_;

                    if (revision_ != null)
                        revision = revision_;

                    if (release_ != null)
                        release = release_;

                    if (nsdPath_ != null)
                        nsdPath = nsdPath_;

                }
            }
            catch (Exception)
            {

            }


        }

        public Namespace()
        {

        }

        public string NamespaceIdentifier { get => namespaceIdentifier; set => namespaceIdentifier = value; }
        public string Version { get => version; set => version = value; }
        public string Revision { get => revision; set => revision = value; }
        public string Release { get => release; set => release = value; }
        public string NsdPath { get => nsdPath; set => nsdPath = value; }
        public bool NsdPathFound { get => nsdPathFound; set => nsdPathFound = value; }
    }

}

