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
using System.Xml;


namespace IEC61850
{
    namespace SCL
    {

        public enum AttributeType
        {
            BOOLEAN = 0, /* int */
            INT8 = 1, /* int8_t */
            INT16 = 2, /* int16_t */
            INT32 = 3, /* int32_t */
            INT64 = 4, /* int64_t */
            INT128 = 5,
            INT8U = 6, /* uint8_t */
            INT16U = 7, /* uint16_t */
            INT24U = 8, /* uint32_t */
            INT32U = 9, /* uint32_t */
            FLOAT32 = 10, /* float */
            FLOAT64 = 11, /* double */
            ENUMERATED = 12,
            OCTET_STRING_64 = 13,
            OCTET_STRING_6 = 14,
            OCTET_STRING_8 = 15,
            VISIBLE_STRING_32 = 16,
            VISIBLE_STRING_64 = 17,
            VISIBLE_STRING_65 = 18,
            VISIBLE_STRING_129 = 19,
            VISIBLE_STRING_255 = 20,
            UNICODE_STRING_255 = 21,
            TIMESTAMP = 22,
            QUALITY = 23,
            CHECK = 24,
            CODEDENUM = 25,
            GENERIC_BITSTRING = 26,
            CONSTRUCTED = 27,
            ENTRY_TIME = 28,
            PHYCOMADDR = 29,
            CURRENCY = 30,
            OBJECT_REFERENCE = 31,
            OTHER = -1
        }

        public static class AttributeTypeExtensions
        {
            public static string ToSclString(AttributeType value)
            {
                switch (value)
                {
                    case AttributeType.BOOLEAN:
                        return "BOOLEAN";
                    case AttributeType.INT8:
                        return "INT8";
                    case AttributeType.INT16:
                        return "INT16";
                    case AttributeType.INT32:
                        return "INT32";
                    case AttributeType.INT64:
                        return "INT64";
                    case AttributeType.INT8U:
                        return "INT8U";
                    case AttributeType.INT16U:
                        return "INT16U";
                    case AttributeType.INT24U:
                        return "INT24U";
                    case AttributeType.INT32U:
                        return "INT32U";
                    case AttributeType.FLOAT32:
                        return "FLOAT32";
                    case AttributeType.FLOAT64:
                        return "FLOAT64";
                    case AttributeType.ENUMERATED:
                        return "Enum";
                    case AttributeType.CODEDENUM:
                        return "Dbpos";
                    case AttributeType.CHECK:
                        return "Check";
                    case AttributeType.OCTET_STRING_64:
                        return "Octet64";
                    case AttributeType.QUALITY:
                        return "Quality";
                    case AttributeType.TIMESTAMP:
                        return "Timestamp";
                    case AttributeType.VISIBLE_STRING_32:
                        return "VisString32";
                    case AttributeType.VISIBLE_STRING_64:
                        return "VisString64";
                    case AttributeType.VISIBLE_STRING_65:
                        return "VisString65";
                    case AttributeType.VISIBLE_STRING_129:
                        return "VisString129";
                    case AttributeType.VISIBLE_STRING_255:
                        return "VisString255";
                    case AttributeType.GENERIC_BITSTRING:
                        return "OptFlds";
                    case AttributeType.OBJECT_REFERENCE:
                        return "ObjRef";
                    case AttributeType.ENTRY_TIME:
                        return "EntryTime";
                    case AttributeType.PHYCOMADDR:
                        return "PhyComAddr";
                    case AttributeType.CONSTRUCTED:
                        return "Struct";
                    default:
                        return "null";
                }
            }

            public static AttributeType CreateFromSclString(string typeString)
            {
                if (typeString.Equals("BOOLEAN"))
                    return AttributeType.BOOLEAN;
                else if (typeString.Equals("INT8"))
                    return AttributeType.INT8;
                else if (typeString.Equals("INT16"))
                    return AttributeType.INT16;
                else if (typeString.Equals("INT32"))
                    return AttributeType.INT32;
                else if (typeString.Equals("INT64"))
                    return AttributeType.INT64;
                else if (typeString.Equals("INT128"))
                    return AttributeType.INT128;
                else if (typeString.Equals("INT8U"))
                    return AttributeType.INT8U;
                else if (typeString.Equals("INT16U"))
                    return AttributeType.INT16U;
                else if (typeString.Equals("INT24U"))
                    return AttributeType.INT24U;
                else if (typeString.Equals("INT32U"))
                    return AttributeType.INT32U;
                else if (typeString.Equals("FLOAT32"))
                    return AttributeType.FLOAT32;
                else if (typeString.Equals("FLOAT64"))
                    return AttributeType.FLOAT64;
                else if (typeString.Equals("Enum"))
                    return AttributeType.ENUMERATED;
                else if (typeString.Equals("Dbpos"))
                    return AttributeType.CODEDENUM;
                else if (typeString.Equals("Check"))
                    return AttributeType.CHECK;
                else if (typeString.Equals("Tcmd"))
                    return AttributeType.CODEDENUM;
                else if (typeString.Equals("Octet64"))
                    return AttributeType.OCTET_STRING_64;
                else if (typeString.Equals("Quality"))
                    return AttributeType.QUALITY;
                else if (typeString.Equals("Timestamp"))
                    return AttributeType.TIMESTAMP;
                else if (typeString.Equals("VisString32"))
                    return AttributeType.VISIBLE_STRING_32;
                else if (typeString.Equals("VisString64"))
                    return AttributeType.VISIBLE_STRING_64;
                else if (typeString.Equals("VisString65"))
                    return AttributeType.VISIBLE_STRING_65;
                else if (typeString.Equals("VisString129"))
                    return AttributeType.VISIBLE_STRING_129;
                else if (typeString.Equals("ObjRef"))
                    return AttributeType.OBJECT_REFERENCE;
                else if (typeString.Equals("VisString255"))
                    return AttributeType.VISIBLE_STRING_255;
                else if (typeString.Equals("Unicode255"))
                    return AttributeType.UNICODE_STRING_255;
                else if (typeString.Equals("OptFlds"))
                    return AttributeType.GENERIC_BITSTRING;
                else if (typeString.Equals("TrgOps"))
                    return AttributeType.GENERIC_BITSTRING;
                else if (typeString.Equals("EntryID"))
                    return AttributeType.OCTET_STRING_8;
                else if (typeString.Equals("EntryTime"))
                    return AttributeType.ENTRY_TIME;
                else if (typeString.Equals("PhyComAddr"))
                    return AttributeType.PHYCOMADDR;
                else if (typeString.Equals("Struct"))
                    return AttributeType.CONSTRUCTED;
                else
                    return AttributeType.OTHER;

            }
        }

        public enum ValKind
        {
            Spec = 0,
            Conf = 1,
            RO = 2,
            Set = 3,
            NONE = 4

        }

        public enum SclFC
        {
            /** Status information */
            ST = 0,
            /** Measurands - analog values */
            MX = 1,
            /** Setpoint */
            SP = 2,
            /** Substitution */
            SV = 3,
            /** Configuration */
            CF = 4,
            /** Description */
            DC = 5,
            /** Setting group */
            SG = 6,
            /** Setting group editable */
            SE = 7,
            /** Service response / Service tracking */
            SR = 8,
            /** Operate received */
            OR = 9,
            /** Blocking */
            BL = 10,
            /** Extended definition */
            EX = 11,
            /** Control */
            CO = 12,
            /** Unicast SV */
            US = 13,
            /** Multicast SV */
            MS = 14,
            /** Unbuffered report */
            RP = 15,
            /** Buffered report */
            BR = 16,
            /** Log control blocks */
            LG = 17,

            /** All FCs - wildcard value */
            ALL = 99,
            NONE = -1
        }

        public class SclType : SclElementWithPrivate
        {
            private bool isUsed = false;
            private bool isNSDCheckedType = false;
            private List<IEDModelNode> usedOn = new List<IEDModelNode>();

            public string Id
            {
                get
                {
                    return XmlHelper.GetAttributeValue(xmlNode, "id");
                }
                set
                {
                    XmlHelper.SetAttributeCreateIfNotExists(sclDocument.XmlDocument, xmlNode, "id", value);
                }
            }

            public string Description
            {
                get
                {
                    return XmlHelper.GetAttributeValue(xmlNode, "desc");
                }
                set
                {
                    XmlHelper.SetAttributeCreateIfNotExists(sclDocument.XmlDocument, xmlNode, "desc", value);
                }
            }

            public bool IsUsed
            {
                get
                {
                    return isUsed;
                }
                set
                {
                    isUsed = value;
                }
            }

            public bool IsNSDCheckedType
            {
                get
                {
                    return isNSDCheckedType;
                }
                set
                {
                    isNSDCheckedType = value;
                }
            }

            public List<IEDModelNode> UsedOn { get => usedOn; set => usedOn = value; }

            public SclType(SclDocument sclDocument, XmlNode xmlNode)
                : base(sclDocument, xmlNode)
            {
                XmlAttribute idAttr = xmlNode.Attributes["id"];

                //if (idAttr == null)
                //                sclDocument.AddIssue(xmlNode, "ERROR", "SclType", "No id attribute", this, "MissingId");

                //throw new SclParserException (xmlNode, "no id attribute");


            }

            public SclType Clone()
            {
                XmlNode newNode = xmlNode.CloneNode(true);

                SclType clone = new SclType(sclDocument, newNode);

                return clone;
            }
        }

        public class SclDataObjectDefinition
        {
            private string name;
            private string type;
            private int count = 0;
            private bool trans = false;

            public XmlNode XmlNode;
            private SclDocument sclDocument;

            public void SetType(String value)
            {
                type = value;
                var attribute = XmlNode.Attributes["type"];
                attribute.Value = value;
            }

            public string Name
            {
                get
                {
                    return XmlHelper.GetAttributeValue(XmlNode, "name");
                }
                set
                {
                    XmlHelper.SetAttributeCreateIfNotExists(sclDocument.XmlDocument, XmlNode, "name", value.ToString());
                }
            }

            public string Type
            {
                get
                {
                    return type;
                }

                set
                {
                    XmlHelper.SetAttributeCreateIfNotExists(sclDocument.XmlDocument, XmlNode, "type", value);
                }
            }

            public int Count
            {
                get
                {
                    return count;
                }
                set
                {
                    count = value;
                    XmlHelper.SetAttributeCreateIfNotExists(sclDocument.XmlDocument, XmlNode, "count", value.ToString());
                }
            }

            public bool IsTransient
            {
                get
                {
                    return XmlHelper.ParseBooleanAttribute(XmlNode, "transient", false);
                }
                set
                {
                    XmlHelper.SetBooleanAttributeCreateIfNotExists(sclDocument.XmlDocument, XmlNode, "transient", value);
                }
            }

            public string Desc
            {
                get
                {
                    XmlAttribute descAttr = XmlNode.Attributes["desc"];

                    if (descAttr == null)
                        return null;
                    else
                        return descAttr.Value;
                }
                set
                {
                    XmlHelper.SetAttributeCreateIfNotExists(sclDocument.XmlDocument, XmlNode, "desc", value);
                }
            }

            public SclDataObjectDefinition(XmlNode xmlNode, SclDocument sclDocument)
            {
                XmlNode = xmlNode;
                this.sclDocument = sclDocument;

                XmlAttribute nameAttr = xmlNode.Attributes["name"];

                if (nameAttr == null)
                    throw new SclParserException(xmlNode, "DO is missing name attribute");

                name = nameAttr.Value;


                XmlAttribute typeAttr = xmlNode.Attributes["type"];

                if (typeAttr == null)
                    throw new SclParserException(xmlNode, "DO is missing type attribute");

                type = typeAttr.Value;

                XmlAttribute countAttr = xmlNode.Attributes["count"];

                if (countAttr != null)
                    count = Convert.ToInt32(countAttr.Value);
            }
        }

        public class SclDO : SclDataObjectDefinition
        {
            public SclDO(XmlNode xmlNode, SclDocument sclDocument) : base(xmlNode, sclDocument)
            {
            }
        }

        public class SclSDO : SclDataObjectDefinition
        {
            public SclSDO(XmlNode xmlNode, SclDocument sclDocument) : base(xmlNode, sclDocument)
            {
            }
        }

        /// <summary>
        /// Helper class to represent trigger conditions for SclDA and SclBDA classes
        /// </summary>
        public class SclTriggerOptions
        {
            private readonly bool dchg = false; /* 1 */
            private readonly bool qchg = false; /* 2 */
            private readonly bool dupd = false; /* 4 */
            private readonly bool period = false; /* 8 */
            private readonly bool gi = true; /* 16 */

            public bool Dchg
            {
                get
                {
                    return dchg;
                }
            }

            public bool Qchg
            {
                get
                {
                    return qchg;
                }
            }

            public bool Dupd
            {
                get
                {
                    return dupd;
                }
            }

            public bool Period
            {
                get
                {
                    return period;
                }
            }

            public bool Gi
            {
                get
                {
                    return gi;
                }
            }

            private bool ParseBooleanAttribute(XmlNode xmlNode, string attrName, bool defaultValue)
            {
                XmlAttribute attr = xmlNode.Attributes[attrName];

                if (attr == null)
                    return defaultValue;

                string attrValue = attr.Value.ToUpper();

                if (attrValue.Equals("TRUE"))
                    return true;
                else if (attrValue.Equals("FALSE"))
                    return false;
                else
                    throw new SclParserException(xmlNode, "Illegal value for boolean attribute \"" + attrName + "\"");
            }

            public SclTriggerOptions(XmlNode xmlNode, bool defaultGi = true)
            {
                dchg = ParseBooleanAttribute(xmlNode, "dchg", false);
                qchg = ParseBooleanAttribute(xmlNode, "qchg", false);
                dupd = ParseBooleanAttribute(xmlNode, "dupd", false);
                period = ParseBooleanAttribute(xmlNode, "period", false);
                gi = ParseBooleanAttribute(xmlNode, "gi", defaultGi);
            }

            public int GetIntValue()
            {
                int intValue = 0;

                if (dchg) intValue += 1;
                if (qchg) intValue += 2;
                if (dupd) intValue += 4;
                if (period) intValue += 8;
                if (gi) intValue += 16;

                return intValue;
            }
        }

        public class SclVal
        {
            private readonly XmlNode xmlNode;
            private readonly XmlDocument xmlDoc;
            private readonly XmlNamespaceManager nsManager;
            private string value;
            private int group;

            public XmlNode XmlNode
            {
                get
                {
                    return xmlNode;
                }
            }

            public string Value
            {
                get
                {
                    value = xmlNode.InnerText;

                    if (value != null)
                        value.Trim();

                    return value;
                }
                set
                {
                    xmlNode.InnerText = value;
                }
            }

            public int Group
            {
                get
                {
                    XmlAttribute groupAttr = xmlNode.Attributes["sGroup"];

                    if (groupAttr != null)
                        group = Convert.ToInt32(groupAttr.Value);
                    else
                        group = 0;

                    return group;
                }
                set
                {
                    XmlHelper.SetAttributeCreateIfNotExists(xmlDoc, xmlNode, "sGroup", value.ToString());

                }
            }

            public SclVal(XmlNode xmlNode)
            {

                this.xmlNode = xmlNode;

                value = xmlNode.InnerText;

                if (value != null)
                    value.Trim();

                XmlAttribute groupAttr = xmlNode.Attributes["sGroup"];

                if (groupAttr != null)
                    group = Convert.ToInt32(groupAttr.Value);
                else
                    group = 0;
            }

            public SclVal(XmlDocument xmlDocument, XmlNode xmlNode)
            {
                xmlDoc = xmlDocument;
                this.xmlNode = xmlNode;

                value = xmlNode.InnerText;

                if (value != null)
                    value.Trim();

                XmlAttribute groupAttr = xmlNode.Attributes["sGroup"];

                if (groupAttr != null)
                    group = Convert.ToInt32(groupAttr.Value);
                else
                    group = 0;
            }


        }

        public class SclDataAttributeDefinition
        {
            private readonly string name = null;
            private string type = null;
            private string desc = null;
            private bool dchg = false;
            private bool qchg = false;
            private bool dupd = false;
            private bool valImport = false;
            private readonly ValKind valKind = ValKind.Set;
            private int count = 0;
            private readonly SclFC fc = SclFC.NONE;
            private readonly AttributeType attributeType = AttributeType.OTHER;
            public SclTriggerOptions triggerOptions = null;
            private List<SclVal> values = null;
            public XmlNode XmlNode;
            private SclDocument sclDocument;


            public string Desc
            {
                get
                {
                    return desc;
                }
            }
            public bool Dchg
            {
                get
                {
                    return dchg;
                }
            }
            public bool Qchg
            {
                get
                {
                    return qchg;
                }
            }
            public bool Dupd
            {
                get
                {
                    return dupd;
                }
            }
            public bool ValImport
            {
                get
                {
                    return valImport;
                }
            }
            public ValKind ValKind
            {
                get
                {
                    return valKind;
                }
            }


            public List<SclVal> GetValues()
            {
                return values;
            }

            public SclVal GetVal()
            {
                if (values != null)
                {
                    foreach (SclVal val in values)
                        return val;
                }

                return null;
            }

            public string Name
            {
                get
                {
                    return XmlHelper.GetAttributeValue(XmlNode, "name");
                }
                set
                {
                    XmlHelper.SetAttributeCreateIfNotExists(sclDocument.XmlDocument, XmlNode, "name", value.ToString());
                }
            }

            public string Type
            {
                get
                {
                    return type;
                }
            }

            public int Count
            {
                get
                {
                    return count;
                }
                set
                {
                    count = value;
                    XmlHelper.SetAttributeCreateIfNotExists(sclDocument.XmlDocument, XmlNode, "count", value.ToString());
                }
            }

            public SclFC Fc
            {
                get
                {
                    return fc;
                }
            }

            public AttributeType AttributeType
            {
                get
                {
                    return attributeType;
                }

            }

            public SclTriggerOptions TriggerOptions
            {
                get
                {
                    return triggerOptions;
                }
            }

            public string SAddr
            {
                get
                {
                    return XmlHelper.GetAttributeValue(XmlNode, "sAddr");
                }

            }

            public void SetType(String value)
            {
                type = value;
                var attribute = XmlNode.Attributes["type"];
                attribute.Value = value;
            }

            private void ParseValNodes(XmlNode xmlNode, XmlNamespaceManager nsManager)
            {
                // There can be multiple "Val" nodes for setting groups FC=SG
                XmlNodeList valNodes = xmlNode.SelectNodes("scl:Val", nsManager);

                foreach (XmlNode valNode in valNodes)
                {
                    if (values == null)
                        values = new List<SclVal>();

                    SclVal val = new SclVal(valNode);

                    values.Add(val);
                }
            }

            public SclDataAttributeDefinition(SclDocument sclDocument, XmlNode xmlNode)
            {
                XmlNode = xmlNode;
                this.sclDocument = sclDocument;

                XmlAttribute nameAttr = xmlNode.Attributes["name"];

                if (nameAttr == null)
                    throw new SclParserException(xmlNode, "DA is missing name attribute");

                name = nameAttr.Value;


                XmlAttribute bTypeAttr = xmlNode.Attributes["bType"];

                if (bTypeAttr != null)
                {
                    attributeType = AttributeTypeExtensions.CreateFromSclString(bTypeAttr.Value);
                }
                else
                    throw new SclParserException(xmlNode, "DA is missing bType attribute");

                XmlAttribute typeAttr = xmlNode.Attributes["type"];

                if (typeAttr != null)
                    type = typeAttr.Value;

                triggerOptions = new SclTriggerOptions(xmlNode, false);

                XmlAttribute countAttr = xmlNode.Attributes["count"];

                if (countAttr != null)
                    count = Convert.ToInt32(countAttr.Value);

                XmlAttribute fcAttr = xmlNode.Attributes["fc"];

                if (fcAttr != null)
                    fc = (SclFC)Enum.Parse(typeof(SclFC), fcAttr.Value);
                else
                    fc = SclFC.NONE;

                XmlAttribute descAttr = xmlNode.Attributes["desc"];
                if (descAttr != null)
                    desc = descAttr.Value;

                XmlAttribute dchgAttr = xmlNode.Attributes["dchg"];
                if (dchgAttr != null)
                    if (dchgAttr.Value == "true")
                        dchg = true;

                XmlAttribute qchgAttr = xmlNode.Attributes["qchg"];
                if (qchgAttr != null)
                    if (qchgAttr.Value == "true")
                        qchg = true;

                XmlAttribute dupdAttr = xmlNode.Attributes["dupd"];
                if (dupdAttr != null)
                    if (dupdAttr.Value == "true")
                        dupd = true;

                XmlAttribute valImportAttr = xmlNode.Attributes["valImport"];
                if (valImportAttr != null)
                    if (valImportAttr.Value == "true")
                        valImport = true;

                XmlAttribute valKindAttr = xmlNode.Attributes["valKind"];
                if (valKindAttr != null)
                    valKind = (ValKind)Enum.Parse(typeof(ValKind), valKindAttr.Value);
                else
                    valKind = ValKind.Set;

                //Valkind default is "set"

                ParseValNodes(xmlNode, sclDocument.NsManager);
            }
        }

        public class SclDA : SclDataAttributeDefinition
        {
            public SclDA(XmlNode xmlNode, SclDocument sclDocument) : base(sclDocument, xmlNode)
            {
            }
        }

        public class SclBDA : SclDataAttributeDefinition
        {
            public SclBDA(XmlNode xmlNode, SclDocument sclDocument) :
            base(sclDocument, xmlNode)
            {
            }
        }

        public class SclLNodeType : SclType
        {
            private readonly string lnClass = null;
            private readonly List<SclDataObjectDefinition> dataObjects = new List<SclDataObjectDefinition>();
            private readonly XmlDocument xmlDocument;
            private readonly XmlNamespaceManager nsManager;

            public void RemoveType(SclDataObjectDefinition sclDataObjectDefinition)
            {
                dataObjects.Remove(sclDataObjectDefinition);

                xmlNode.RemoveChild(sclDataObjectDefinition.XmlNode);
            }

            public XmlDocument XmlDocument
            {
                get
                {
                    return xmlDocument;
                }
            }

            public XmlNamespaceManager XmlNamespaceManager
            {
                get
                {
                    return nsManager;
                }
            }

            public string LnClass
            {
                get
                {
                    return XmlHelper.GetAttributeValue(xmlNode, "lnClass");

                }
                set
                {
                    XmlHelper.SetAttributeCreateIfNotExists(xmlDocument, xmlNode, "lnClass", value);
                }
            }

            public List<SclDataObjectDefinition> DataObjects
            {
                get
                {
                    return dataObjects;
                }
            }

            public string IedType
            {
                get { return XmlHelper.GetAttributeValue(xmlNode, "iedType"); }
                set { XmlHelper.SetAttributeCreateIfNotExists(xmlDocument, xmlNode, "iedType", value); }
            }

            private void ParseDataObjectNodes()
            {
                XmlNodeList doNodes = xmlNode.SelectNodes("scl:DO", nsManager);

                foreach (XmlNode doNode in doNodes)
                    dataObjects.Add(new SclDO(doNode, sclDocument));
            }

            public SclLNodeType(SclDocument sclDocument, XmlNode xmlNode)
                : base(sclDocument, xmlNode)
            {
                nsManager = sclDocument.NsManager;

                this.xmlNode = xmlNode;

                xmlDocument = sclDocument.XmlDocument;

                XmlAttribute lnClassAttr = xmlNode.Attributes["lnClass"];

                if (lnClassAttr == null)
                    throw new SclParserException(xmlNode, "no lnClass attribute");

                LnClass = lnClassAttr.Value;

                ParseDataObjectNodes();
            }

        }

        public class SclDOType : SclType
        {
            private string cdc = null;
            private readonly string iedType = null;
            private readonly List<SclDataObjectDefinition> subDataObjects = new List<SclDataObjectDefinition>();
            private readonly List<SclDataAttributeDefinition> dataAttributes = new List<SclDataAttributeDefinition>();

            public void RemoveType(SclDataObjectDefinition sclDataObjectDefinition)
            {
                subDataObjects.Remove(sclDataObjectDefinition);
                xmlNode.RemoveChild(sclDataObjectDefinition.XmlNode);
            }

            public void RemoveType(SclDataAttributeDefinition sclDataAttributeDefinition)
            {
                dataAttributes.Remove(sclDataAttributeDefinition);

                xmlNode.RemoveChild(sclDataAttributeDefinition.XmlNode);
            }

            public string Cdc
            {
                get
                {
                    return cdc;
                }
                set
                {
                    XmlHelper.SetAttributeCreateIfNotExists(sclDocument.XmlDocument, xmlNode, "cdc", value);
                    cdc = value;

                }
            }

            public string IedType
            {
                get
                {
                    return iedType;
                }
            }

            public List<SclDataObjectDefinition> SubDataObjects
            {
                get
                {
                    return subDataObjects;
                }
            }

            public SclDataObjectDefinition GetSDO(string name)
            {
                foreach (SclDataObjectDefinition sclSdo in subDataObjects)
                {
                    if (sclSdo.Name != null && sclSdo.Name.Equals(name))
                        return sclSdo;
                }

                return null;
            }

            public List<SclDataAttributeDefinition> DataAttributes
            {
                get
                {
                    return dataAttributes;
                }
            }

            private void ParseSubDataObjectNodes()
            {
                XmlNodeList doNodes = xmlNode.SelectNodes("scl:SDO", sclDocument.NsManager);

                foreach (XmlNode doNode in doNodes)
                    subDataObjects.Add(new SclSDO(doNode, sclDocument));
            }


            private void ParseDataAttributeNodes()
            {
                XmlNodeList daNodes = xmlNode.SelectNodes("scl:DA", sclDocument.NsManager);

                foreach (XmlNode daNode in daNodes)
                    dataAttributes.Add(new SclDA(daNode, sclDocument));

            }

            public SclDOType(SclDocument sclDocument, XmlNode xmlNode)
                : base(sclDocument, xmlNode)
            {
                XmlAttribute cdcAttr = xmlNode.Attributes["cdc"];

                if (cdcAttr == null)
                    throw new SclParserException(xmlNode, "no cdc attribute");


                cdc = cdcAttr.Value;

                XmlAttribute iedType = xmlNode.Attributes["iedType"];
                if (iedType != null)
                    this.iedType = iedType.Value;

                ParseSubDataObjectNodes();

                ParseDataAttributeNodes();
            }

        }

        public class SclDAType : SclType
        {
            private readonly List<SclDataAttributeDefinition> subDataAttributes = new List<SclDataAttributeDefinition>();

            public void RemoveType(SclDataAttributeDefinition sclDataAttributeDefinition)
            {
                subDataAttributes.Remove(sclDataAttributeDefinition);
                xmlNode.RemoveChild(sclDataAttributeDefinition.XmlNode);
            }

            public List<SclDataAttributeDefinition> SubDataAttributes
            {
                get
                {
                    return subDataAttributes;
                }
            }

            public SclDataAttributeDefinition GetBDA(string name)
            {
                foreach (SclDataAttributeDefinition bda in subDataAttributes)
                {
                    if (bda.Name.Equals(name))
                        return bda;
                }

                return null;
            }

            public SclDAType(SclDocument sclDocument, XmlNode xmlNode)
                : base(sclDocument, xmlNode)
            {
                XmlNodeList bdaNodes = xmlNode.SelectNodes("scl:BDA", sclDocument.NsManager);

                foreach (XmlNode bdaNode in bdaNodes)
                    subDataAttributes.Add(new SclBDA(bdaNode, sclDocument));
            }


        }

        public class SclEnumVal
        {
            private readonly int ord;
            private /*readonly*/ string symbolicName = null;
            public XmlNode XmlNode;
            public SclDocument sclDocument;

            public int Ord
            {
                get
                {
                    return ord;
                }
            }

            public string SymbolicName
            {
                get
                {
                    if (XmlHelper.GetAttributeValue(XmlNode, "symbolicName") != null)
                        return XmlHelper.GetAttributeValue(XmlNode, "symbolicName");
                    else
                        return symbolicName;
                }
                set
                {

                    //this.symbolicName = value;
                    XmlHelper.SetAttributeCreateIfNotExists(sclDocument.XmlDocument, XmlNode, "symbolicName", value);
                }
                //get
                //{
                //		return this.symbolicName;
                //}
                //set
                //{

                //	XmlHelper.SetAttributeCreateIfNotExists(sclDocument.XmlDocument, XmlNode, "id", value);
                //}
            }

            public SclEnumVal(XmlNode xmlNode)
            {
                XmlAttribute xmlAttr = xmlNode.Attributes["ord"];

                if (xmlAttr == null)
                    throw new SclParserException(xmlNode, "EnumVal has no \"ord\" attribute");

                ord = Convert.ToInt32(xmlAttr.Value);

                symbolicName = xmlNode.InnerXml;

                XmlNode = xmlNode;
            }




            public bool Equals(SclEnumVal other)
            {
                if (Ord == other.Ord)
                {
                    if (SymbolicName.Equals(other.SymbolicName))
                        return true;
                    else
                        return false;
                }
                else
                    return false;
            }
        }

        public class SclEnumType : SclType
        {
            private readonly List<SclEnumVal> enumValues = new List<SclEnumVal>();

            public void RemoveType(SclEnumVal sclEnumVal)
            {
                enumValues.Remove(sclEnumVal);

                xmlNode.RemoveChild(sclEnumVal.XmlNode);
            }

            public List<SclEnumVal> EnumValues { get { return enumValues; } }

            public SclEnumType(SclDocument sclDocument, XmlNode xmlNode)
                : base(sclDocument, xmlNode)
            {
                XmlNodeList enumValNodes = xmlNode.SelectNodes("scl:EnumVal", sclDocument.NsManager);

                foreach (XmlNode enumValNode in enumValNodes)
                    enumValues.Add(new SclEnumVal(enumValNode));
            }


            public int GetOrdinalValue(string symbolicValue)
            {
                foreach (SclEnumVal enumValue in enumValues)
                {
                    if (enumValue.SymbolicName == symbolicValue)
                        return enumValue.Ord;
                }

                return -1;
            }

            /// <summary>
            /// Check if two EnumTypes are equal (have the same values)
            /// </summary>
            /// <param name="other"></param>
            /// <returns></returns>
            public bool Equals(SclEnumType other)
            {
                if (Id.Equals(other.Id))
                {
                    if (enumValues.Count == other.enumValues.Count)
                    {

                        for (int i = 0; i < enumValues.Count; i++)
                        {
                            if (enumValues[i].Equals(other.enumValues[i]) == false)
                                return false;
                        }

                        return true;
                    }
                    else
                        return false;
                }
                else
                    return false;
            }
        }

    }
}

