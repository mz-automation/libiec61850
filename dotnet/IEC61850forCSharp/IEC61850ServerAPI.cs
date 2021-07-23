/*
 *  IEC61850ServerAPI.cs
 *
 *  Copyright 2016 Michael Zillgith
 *
 *  This file is part of libIEC61850.
 *
 *  libIEC61850 is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  libIEC61850 is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with libIEC61850.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  See COPYING file for the complete license text.
 */
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using IEC61850.Common;
using IEC61850.TLS;

// IEC 61850 API for the libiec61850 .NET wrapper library
namespace IEC61850
{
    // IEC 61850 server API.
    namespace Server
    {
        /// <summary>
        /// Config file parser.
        /// </summary>
        public class ConfigFileParser
        {
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr ConfigFileParser_createModelFromConfigFileEx(string filename);

            public static IedModel CreateModelFromConfigFile(string filePath)
            { 
                IntPtr retVal = ConfigFileParser_createModelFromConfigFileEx (filePath);
                if (retVal == IntPtr.Zero) {
                    return null;
                }

                return new IedModel (retVal);
            }
        }

        /// <summary>
        /// Representation of the IED server data model
        /// </summary>
        public class IedModel : IDisposable
        {
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr IedModel_create(string name);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr IedModel_destroy(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr IedModel_getModelNodeByObjectReference(IntPtr self, string objectReference);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr IedModel_getModelNodeByShortObjectReference(IntPtr self, string objectReference);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern int ModelNode_getType(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedModel_setIedNameForDynamicModel(IntPtr self, string iedName);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr IedModel_getDeviceByInst(IntPtr self, string ldInst);

            internal IntPtr self = IntPtr.Zero;

            internal IedModel(IntPtr self)
            {
                this.self = self;
            }

            /* cache managed ModelNode instances of the IedModel */
            internal Dictionary<IntPtr, ModelNode> modelNodes = new Dictionary<IntPtr, ModelNode>();

            /// <summary>
            /// Initializes a new instance of the <see cref="T:IEC61850.Server.IedModel"/> class.
            /// </summary>
            /// <param name="name">IED name</param>
            public IedModel(string name)
            {
                self = IedModel_create(name);
            }

            ~IedModel()
            {
                Dispose();    
            }

            /// <summary>
            /// Releases all resource used by the <see cref="IEC61850.Server.IedModel"/> object.
            /// </summary>
            public void Destroy()
            {
                Dispose();
            }

            /// <summary>
            /// Releases all resource used by the <see cref="IEC61850.Server.IedModel"/> object.
            /// </summary>
            /// <remarks>Call <see cref="Dispose"/> when you are done using the <see cref="IEC61850.Server.IedModel"/>. The
            /// <see cref="Dispose"/> method leaves the <see cref="IEC61850.Server.IedModel"/> in an unusable state. After
            /// calling <see cref="Dispose"/>, you must release all references to the <see cref="IEC61850.Server.IedModel"/> so
            /// the garbage collector can reclaim the memory that the <see cref="IEC61850.Server.IedModel"/> was occupying.</remarks>
            public void Dispose()
            {
                lock (this)
                {
                    if (self != IntPtr.Zero)
                    {
                        IedModel_destroy(self);
                        self = IntPtr.Zero;
                    }
                }
            }

            /// <summary>
            /// Creates a new <see cref="IEC61850.Server.IedModel"/> instance with the data model of <see langword="async"/> config file.
            /// </summary>
            /// <returns>new <see cref="IEC61850.Server.IedModel"/> instance</returns>
            /// <param name="filename">fila name of the configuration (.cfg) file</param>
            public static IedModel CreateFromFile(string filename)
            {
                return ConfigFileParser.CreateModelFromConfigFile(filename);
            }

            /// <summary>
            /// Get parent node. When not found create the parent node and add to modelNode list
            /// </summary>
            /// <returns>The parent node, or <see langword="null"/> when not found</returns>
            /// <param name="nodeRef">the native reference of the model node</param>
            private ModelNode GetParent(IntPtr nodeRef)
            {
                ModelNode parentNode = null;

                IntPtr parentNodeRef = ModelNode.GetNativeParent(nodeRef);

                if (parentNodeRef != IntPtr.Zero)
                {
                    if (modelNodes.TryGetValue(parentNodeRef, out parentNode) == false)
                    {
                        int nodeType = ModelNode_getType(parentNodeRef);

                        if (nodeType == 0)
                        {
                            parentNode = new LogicalDevice(parentNodeRef, this);
                        }
                        else
                        {
                            ModelNode parentOfParent = GetParent(parentNodeRef);

                            if (parentOfParent != null)
                            {
                                switch (nodeType)
                                {
                                    case 1:
                                        parentNode = new LogicalNode(parentNodeRef, parentOfParent);
                                        break;

                                    case 2:
                                        parentNode = new DataObject(parentNodeRef, parentOfParent);
                                        break;

                                    case 3:
                                        parentNode = new DataAttribute(parentNodeRef, parentOfParent);
                                        break;

                                    default:
                                        parentNode = new ModelNode(parentNodeRef, parentOfParent);
                                        break;
                                }
                            }
                        }

                        if (parentNode != null)
                        {
                            modelNodes.Add(parentNodeRef, parentNode);
                        }
                    }
                }

                return parentNode;
            }

            public LogicalDevice GetDeviceByInst(string ldInst)
            {
                IntPtr ldPtr = IedModel_getDeviceByInst(self, ldInst);

                if (ldPtr != IntPtr.Zero)
                {
                    return new LogicalDevice(ldPtr, this);
                }
                else
                    return null;
            }

            internal ModelNode GetModelNodeFromNodeRef(IntPtr nodeRef)
            {
                ModelNode modelNode = null;

                modelNodes.TryGetValue(nodeRef, out modelNode);

                if (modelNode == null)
                {
                    int nodeType = ModelNode_getType(nodeRef);

                    if (nodeType == 0)
                    {
                        modelNode = new LogicalDevice(nodeRef, this);
                    }
                    else
                    {
                        ModelNode parent = GetParent(nodeRef);

                        if (parent != null)
                        {
                            switch (nodeType)
                            {
                                case 1:
                                    modelNode = new LogicalNode(nodeRef, parent);
                                    break;

                                case 2:
                                    modelNode = new DataObject(nodeRef, parent);
                                    break;

                                case 3:
                                    modelNode = new DataAttribute(nodeRef, parent);
                                    break;

                                default:
                                    modelNode = new ModelNode(nodeRef, parent);
                                    break;
                            }
                        }
                    }

                    if (modelNode != null)
                    {
                        modelNodes.Add(nodeRef, modelNode);
                    }
                }

                return modelNode;
            }

            /// <summary>
            /// Change the IED name of the data model
            /// </summary>
            /// <param name="iedName">the new IED name</param>
            public void SetIedName(string iedName)
            {
                IedModel_setIedNameForDynamicModel(self, iedName);
            }

            /// <summary>
            /// Gets the model node by full object reference.
            /// </summary>
            /// <returns>The model node</returns>
            /// <param name="objectReference">Full object reference including the IED name</param>
            public ModelNode GetModelNodeByObjectReference(string objectReference)
            {
                IntPtr nodeRef = IedModel_getModelNodeByObjectReference(self, objectReference);

                if (nodeRef == IntPtr.Zero)
                    return null;

                return GetModelNodeFromNodeRef (nodeRef);
            }

            /// <summary>
            /// Gets the model node by short object reference (without IED name)
            /// </summary>
            /// <returns>The model node</returns>
            /// <param name="objectReference">Object reference without IED name (e.g. LD0/GGIO1.Ind1.stVal)</param>
            public ModelNode GetModelNodeByShortObjectReference(string objectReference)
            {
                IntPtr nodeRef = IedModel_getModelNodeByShortObjectReference(self, objectReference);

                if (nodeRef == IntPtr.Zero)
                    return null;

                return GetModelNodeFromNodeRef (nodeRef);
            }

        }

        public class LogicalDevice : ModelNode
        {
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr LogicalDevice_create(string name, IntPtr parent);

            private IedModel iedModel = null;

            public IedModel IedModel { get => iedModel; }

            public LogicalDevice (IntPtr self, IedModel iedModel) : base (self)
            {
                this.iedModel = iedModel;
            }

            public LogicalDevice(string name, IedModel parent)
            {
                this.iedModel = parent;

                self = LogicalDevice_create(name, parent.self);
            }
        }

        public class LogicalNode : ModelNode
        {
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr LogicalNode_create(string name, IntPtr parent);

            public LogicalNode (IntPtr self, ModelNode parent) : base(self)
            {
                this.parent = parent;
            }

            public LogicalNode(string name, LogicalDevice parent)
            {
                this.parent = parent;

                base.self = LogicalNode_create(name, parent.self);
            }
        }

        public enum AccessPolicy {
            ACCESS_POLICY_ALLOW = 0,
            ACCESS_POLICY_DENY = 1
        }

        public enum DataAttributeType {
            BOOLEAN = 0,
            INT8 = 1,
            INT16 = 2,
            INT32 = 3,
            INT64 = 4, 
            INT128 = 5, 
            INT8U = 6,
            INT16U = 7,
            INT24U = 8,
            INT32U = 9,
            FLOAT32 = 10,
            FLOAT64 = 11,
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
            CURRENCY = 30
        }

        public enum ModeValues
        {
            ON = 1,
            BLOCKED = 2,
            TEST = 3,
            TEST_BLOCKED = 4,
            OFF = 5
        }

        public enum HealthValues
        {
            OK = 1,
            WARNING = 2,
            ALARM = 3
        }

        /// <summary>
        /// The CDC class contains helper functions to create DataObject instances for the
        /// most common Common Data Classes.
        /// </summary>
        public class CDC 
        {
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_SPS_create(string name, IntPtr parent, uint options);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_DPS_create(string name, IntPtr parent, uint options);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_VSS_create(string name, IntPtr parent, uint options);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_SEC_create(string name, IntPtr parent, uint options);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_CMV_create(string name, IntPtr parent, uint options);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_SAV_create(string name, IntPtr parent, uint options, [MarshalAs(UnmanagedType.I1)] bool isIntegerNotFloat);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_ACD_create(string name, IntPtr parent, uint options);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_ACT_create(string name, IntPtr parent, uint options);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_SPG_create(string name, IntPtr parent, uint options);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_VSG_create(string name, IntPtr parent, uint options);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_ENG_create(string name, IntPtr parent, uint options);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_ING_create(string name, IntPtr parent, uint options);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_ASG_create(string name, IntPtr parent, uint options, [MarshalAs(UnmanagedType.I1)] bool isIntegerNotFloat);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_WYE_create(string name, IntPtr parent, uint options);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_DEL_create(string name, IntPtr parent, uint options);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_HST_create(string name, IntPtr parent, uint options, ushort maxPts);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_INS_create(string name, IntPtr parent, uint options);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_MV_create(string name, IntPtr parent, uint options, [MarshalAs(UnmanagedType.I1)] bool isIntegerNotFloat);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_INC_create(string name, IntPtr parent, uint options, uint controlOptions);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_LPL_create(string name, IntPtr parent, uint options);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_DPL_create(string name, IntPtr parent, uint options);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_ENS_create(string name, IntPtr parent, uint options);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_SPC_create(string name, IntPtr parent, uint options, uint controlOptions);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_DPC_create(string name, IntPtr parent, uint options, uint controlOptions);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_BSC_create(string name, IntPtr parent, uint options, uint controlOptions, [MarshalAs(UnmanagedType.I1)] bool hasTransientIndicator);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_APC_create(string name, IntPtr parent, uint options, uint controlOptions, [MarshalAs(UnmanagedType.I1)] bool isIntegerNotFloat);
            
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_BCR_create(string name, IntPtr parent, uint options);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_ENC_create(string name, IntPtr parent, uint options, uint controlOptions);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_SPV_create(string name, IntPtr parent, uint options, uint controlOptions, uint wpOptions, [MarshalAs(UnmanagedType.I1)] bool hasChaManRs);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_STV_create(string name, IntPtr parent, uint options, uint controlOptions, uint wpOptions, [MarshalAs(UnmanagedType.I1)] bool hasOldStatus);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_CMD_create(string name, IntPtr parent, uint options, uint controlOptions, uint wpOptions, [MarshalAs(UnmanagedType.I1)] bool hasOldStatus, [MarshalAs(UnmanagedType.I1)] bool hasCmTm, [MarshalAs(UnmanagedType.I1)] bool hasCmCt);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_ALM_create(string name, IntPtr parent, uint options, uint controlOptions, uint wpOptions, [MarshalAs(UnmanagedType.I1)] bool hasOldStatus);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_CTE_create(string name, IntPtr parent, uint options, uint controlOptions, uint wpOptions, [MarshalAs(UnmanagedType.I1)] bool hasHisRs);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_TMS_create(string name, IntPtr parent, uint options, uint controlOptions, uint wpOptions, [MarshalAs(UnmanagedType.I1)] bool hasHisRs);

            public const int CDC_OPTION_DESC = (1 << 2);
            public const int CDC_OPTION_DESC_UNICODE = (1 << 3);
            public const int CDC_OPTION_AC_DLNDA = (1 << 4);
            public const int CDC_OPTION_AC_DLN = (1 << 5);

            // options that are only valid for DPL CDC
            public const int CDC_OPTION_DPL_HWREV = (1 << 17);
            public const int CDC_OPTION_DPL_SWREV = (1 << 18);
            public const int CDC_OPTION_DPL_SERNUM = (1 << 19);
            public const int CDC_OPTION_DPL_MODEL = (1 << 20);
            public const int CDC_OPTION_DPL_LOCATION = (1 << 21);

            // mandatory data attributes for LLN0 (e.g. LBL configRef)
            public const int CDC_OPTION_AC_LN0_M = (1 << 24);
            public const int CDC_OPTION_AC_LN0_EX = (1 << 25);
            public const int CDC_OPTION_AC_DLD_M = (1 << 26);


            public static DataObject Create_CDC_SPS(ModelNode parent, string name, uint options)
            {
                IntPtr self = CDC_SPS_create(name, parent.self, options);

                if (self != IntPtr.Zero)
                    return new DataObject (self, parent);
                else
                    return null;
            }
            
            public static DataObject Create_CDC_DPS(ModelNode parent, string name, uint options)
            {
                IntPtr self = CDC_DPS_create(name, parent.self, options);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_VSS(ModelNode parent, string name, uint options)
            {
                IntPtr self = CDC_VSS_create(name, parent.self, options);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_SEC(ModelNode parent, string name, uint options)
            {
                IntPtr self = CDC_SEC_create(name, parent.self, options);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_CMV(ModelNode parent, string name, uint options)
            {
                IntPtr self = CDC_CMV_create(name, parent.self, options);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_SAV(ModelNode parent, string name, uint options, bool isIntegerNotFloat)
            {
                IntPtr self = CDC_SAV_create(name, parent.self, options, isIntegerNotFloat);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_ACD(ModelNode parent, string name, uint options)
            {
                IntPtr self = CDC_ACD_create(name, parent.self, options);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_ACT(ModelNode parent, string name, uint options)
            {
                IntPtr self = CDC_ACT_create(name, parent.self, options);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_SPG(ModelNode parent, string name, uint options)
            {
                IntPtr self = CDC_SPG_create(name, parent.self, options);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_VSG(ModelNode parent, string name, uint options)
            {
                IntPtr self = CDC_VSG_create(name, parent.self, options);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_ENG(ModelNode parent, string name, uint options)
            {
                IntPtr self = CDC_ENG_create(name, parent.self, options);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_ING(ModelNode parent, string name, uint options)
            {
                IntPtr self = CDC_ING_create(name, parent.self, options);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_ASG(ModelNode parent, string name, uint options, bool isIntegerNotFloat)
            {
                IntPtr self = CDC_ASG_create(name, parent.self, options, isIntegerNotFloat);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_WYE(ModelNode parent, string name, uint options)
            {
                IntPtr self = CDC_WYE_create(name, parent.self, options);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_DEL(ModelNode parent, string name, uint options)
            {
                IntPtr self = CDC_DEL_create(name, parent.self, options);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_HST(ModelNode parent, string name, uint options, ushort maxPts)
            {
                IntPtr self = CDC_HST_create(name, parent.self, options, maxPts);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_INS(ModelNode parent, string name, uint options)
            {
                IntPtr self = CDC_INS_create(name, parent.self, options);

                if (self != IntPtr.Zero)
                    return new DataObject (self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_MV(ModelNode parent, string name, uint options, bool isIntegerNotFloat)
            {
                IntPtr self = CDC_MV_create(name, parent.self, options, isIntegerNotFloat);

                if (self != IntPtr.Zero)
                    return new DataObject (self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_INC(ModelNode parent, string name, uint options, uint controlOptions)
            {
                IntPtr self = CDC_INC_create(name, parent.self, options, controlOptions);

                if (self != IntPtr.Zero)
                    return new DataObject (self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_LPL(ModelNode parent, string name, uint options)
            {
                IntPtr self = CDC_LPL_create(name, parent.self, options);

                if (self != IntPtr.Zero)
                    return new DataObject (self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_DPL(ModelNode parent, string name, uint options)
            {
                IntPtr self = CDC_DPL_create(name, parent.self, options);

                if (self != IntPtr.Zero)
                    return new DataObject (self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_ENS(ModelNode parent, string name, uint options)
            {
                IntPtr self = CDC_ENS_create(name, parent.self, options);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_SPC(ModelNode parent, string name, uint options, uint controlOptions)
            {
                IntPtr self = CDC_SPC_create(name, parent.self, options, controlOptions);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_DPC(ModelNode parent, string name, uint options, uint controlOptions)
            {
                IntPtr self = CDC_DPC_create(name, parent.self, options, controlOptions);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_BSC(ModelNode parent, string name, uint options, uint controlOptions, bool hasTransientIndicator)
            {
                IntPtr self = CDC_BSC_create(name, parent.self, options, controlOptions, hasTransientIndicator);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_APC(ModelNode parent, string name, uint options, uint controlOptions, bool isIntegerNotFloat)
            {
                IntPtr self = CDC_APC_create(name, parent.self, options, controlOptions, isIntegerNotFloat);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_BCR(ModelNode parent, string name, uint options)
            {
                IntPtr self = CDC_BCR_create(name, parent.self, options);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_ENC(ModelNode parent, string name, uint options, uint controlOptions)
            {
                IntPtr self = CDC_ENC_create(name, parent.self, options, controlOptions);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_SPV(ModelNode parent, string name, uint options, uint controlOptions, uint wpOptions, bool hasChaManRs)
            {
                IntPtr self = CDC_SPV_create(name, parent.self, options, controlOptions, wpOptions, hasChaManRs);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_STV(ModelNode parent, string name, uint options, uint controlOptions, uint wpOptions, bool hasOldStatus)
            {
                IntPtr self = CDC_STV_create(name, parent.self, options, controlOptions, wpOptions, hasOldStatus);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_CMD(ModelNode parent, string name, uint options, uint controlOptions, uint wpOptions, bool hasOldStatus, bool hasCmTm, bool hasCmCt)
            {
                IntPtr self = CDC_CMD_create(name, parent.self, options, controlOptions, wpOptions, hasOldStatus, hasCmTm, hasCmCt);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_ALM(ModelNode parent, string name, uint options, uint controlOptions, uint wpOptions, bool hasOldStatus)
            {
                IntPtr self = CDC_ALM_create(name, parent.self, options, controlOptions, wpOptions, hasOldStatus);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_CTE(ModelNode parent, string name, uint options, uint controlOptions, uint wpOptions, bool hasHisRs)
            {
                IntPtr self = CDC_CTE_create(name, parent.self, options, controlOptions, wpOptions, hasHisRs);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_TMS(ModelNode parent, string name, uint options, uint controlOptions, uint wpOptions, bool hasHisRs)
            {
                IntPtr self = CDC_TMS_create(name, parent.self, options, controlOptions, wpOptions, hasHisRs);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }
        }

        public class DataObject : ModelNode
        {
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr DataObject_create(string name, IntPtr parent, int arrayElements);

            internal DataObject(IntPtr self, ModelNode parent) : base(self)
            {
                this.parent = parent;
            }

            public DataObject(string name, ModelNode parent) : this(name, parent, 0)
            {
            }

            public DataObject(string name, ModelNode parent, int arrayElements)
            {
                this.parent = parent;

                self = DataObject_create (name, parent.self, arrayElements);
            }

        }

        public class DataAttribute : ModelNode
        {
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr DataAttribute_create(string name, IntPtr parent, int type, int fc,
                byte triggerOptions, int arrayElements, UInt32 sAddr);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void DataAttribute_setValue(IntPtr self, IntPtr mmsValue);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern int DataAttribute_getType(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern byte DataAttribute_getTrgOps(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern int DataAttribute_getFC(IntPtr self);

            internal DataAttribute(IntPtr self, ModelNode parent) : base(self)
            {
                this.parent = parent;
            }

            /// <summary>
            /// Create a new data attribute and add it to a parent model node
            /// </summary>
            /// The parent model node has to be of type DataObject or DataAttribute
            /// <param name="name">the name of the data attribute (e.g. "stVal")</param>
            /// <param name="parent">the parent model node (of type DataObject or DataAttribute)</param>
            /// <param name="type">the type of the data attribute (CONSTRUCTED if the type contains sub data attributes)</param>
            /// <param name="fc">the functional constraint (FC) of the data attribute</param>
            /// <param name="trgOps">the trigger options (dupd, dchg, qchg) that cause an event notification</param>
            /// <param name="arrayElements">the number of array elements if the data attribute is an array or 0</param>
            /// <param name="sAddr">an optional short address (deprecated)</param>
            public DataAttribute (string name, ModelNode parent, DataAttributeType type, FunctionalConstraint fc, TriggerOptions trgOps,
                int arrayElements, UInt32 sAddr)
            {
                this.parent = parent;

                self = DataAttribute_create (name, parent.self, (int)type, (int)fc, (byte)trgOps, arrayElements, sAddr);
            }

            /// <summary>
            /// Create a new data attribute and add it to a parent model node
            /// </summary>
            /// The parent model node has to be of type DataObject or DataAttribute
            /// <param name="name">the name of the data attribute (e.g. "stVal")</param>
            /// <param name="parent">the parent model node (of type DataObject or DataAttribute)</param>
            /// <param name="type">the type of the data attribute (CONSTRUCTED if the type contains sub data attributes)</param>
            /// <param name="fc">the functional constraint (FC) of the data attribute</param>
            /// <param name="trgOps">the trigger options (dupd, dchg, qchg) that cause an event notification</param>
            /// <param name="arrayElements">the number of array elements if the data attribute is an array or 0</param>
            public DataAttribute(string name, ModelNode parent, DataAttributeType type, FunctionalConstraint fc, TriggerOptions trgOps,
                int arrayElements)
            {
                this.parent = parent;

                self = DataAttribute_create(name, parent.self, (int)type, (int)fc, (byte)trgOps, arrayElements, 0);
            }

            /// <summary>
            /// Get IEC 61850 data attribute type of the data attribute
            /// </summary>
            public DataAttributeType Type
            {
                get
                {
                    return (DataAttributeType)DataAttribute_getType(self);
                }
            }

            /// <summary>
            /// The trigger options (dchg, qchg, dupd) of the data attribute
            /// </summary>
            public TriggerOptions TrgOps
            {
                get
                {
                    return (TriggerOptions)DataAttribute_getTrgOps(self);
                }
            }

            /// <summary>
            /// The functional constraint (FC) of the data attribute
            /// </summary>
            public FunctionalConstraint FC
            {
                get
                {
                    return (FunctionalConstraint)DataAttribute_getFC(self);
                }
            }

            /// <summary>
            /// Set the value of the data attribute (can be used to set default values before server is created)
            /// </summary>
            /// <param name="value">New value for the data attribute</param>
            public void SetValue(MmsValue value)
            {
                DataAttribute_setValue(self, value.valueReference);
            }
        }

        public class ModelNode
        {
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr ModelNode_getChild(IntPtr self, string name);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern int ModelNode_getType(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr ModelNode_getObjectReference(IntPtr self, IntPtr objectReference);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr ModelNode_getObjectReferenceEx(IntPtr self, IntPtr objectReference, [MarshalAs(UnmanagedType.I1)] bool withoutIedName);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr ModelNode_getParent(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr ModelNode_getChildren(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr ModelNode_getName(IntPtr self);

            /****************
            * LinkedList
            ***************/
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr LinkedList_getNext(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr LinkedList_getData(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void LinkedList_destroyStatic(IntPtr self);

            public IntPtr self;

            internal ModelNode parent = null;

            static internal IntPtr GetNativeParent(IntPtr self)
            {
                return ModelNode_getParent(self);
            }

            internal ModelNode()
            {
                self = IntPtr.Zero;
            }

            internal ModelNode(IntPtr self, ModelNode parent)
            {
                this.self = self;
                this.parent = parent;
            }

            internal ModelNode(IntPtr self)
            {
                this.self = self;
            }

            /// <summary>
            /// Gets the IedModel for this ModelNode instance
            /// </summary>
            /// <returns>the IedModel instance of this ModelNode.</returns>
            public IedModel GetIedModel()
            {
                if (this is LogicalDevice)
                {
                    return (this as LogicalDevice).IedModel;
                }
                else
                {
                    if (this.parent != null)
                        return parent.GetIedModel();
                    else
                        return null;
                }
            }

            /// <summary>
            /// Gets the name of the model node
            /// </summary>
            /// <returns>name of the model node</returns>
            public string GetName()
            {
                return Marshal.PtrToStringAnsi(ModelNode_getName(self));
            }

            /// <summary>
            /// Gets the parent node of this model node
            /// </summary>
            /// <returns>The parent node</returns>
            public ModelNode GetParent()
            {
                return parent;
            }


            /// <summary>
            /// Get the child node of this model node with the given name
            /// </summary>
            /// <returns>The child node or null when there is no child with the given name</returns>
            /// <param name="name">name of the child node</param>
            public ModelNode GetChild(string name)
            {
                IntPtr childPtr = ModelNode_getChild(self, name);

                if (childPtr == IntPtr.Zero)
                    return null;

                ModelNode child = null;

                IedModel iedModel = GetIedModel();

                if (iedModel != null)
                {
                    iedModel.modelNodes.TryGetValue(childPtr, out child);
                }

                if (child == null)
                {
                    int nodeType = ModelNode_getType(childPtr);

                    switch (nodeType)
                    {
                        case 0:
                            child = new LogicalDevice(childPtr, iedModel);
                            break;

                        case 1:
                            child = new LogicalNode(childPtr, this);
                            break;

                        case 2:
                            child = new DataObject(childPtr, this);
                            break;

                        case 3:
                            child = new DataAttribute(childPtr, this);
                            break;

                        default:
                            child = new ModelNode(childPtr, this);
                            break;
                    }

                    if (child != null && iedModel != null)
                    {
                        iedModel.modelNodes.Add(childPtr, child);
                    }
                }

                return child;
            }

            internal static ModelNode CreateInstance(IntPtr instPtr, ModelNode parent)
            {
                int nodeType = ModelNode_getType(instPtr);

                ModelNode newInstance = null;

                switch (nodeType)
                {
                    case 1:
                        newInstance = new LogicalNode(instPtr, parent);
                        break;

                    case 2:
                        newInstance = new DataObject(instPtr, parent);
                        break;

                    case 3:
                        newInstance = new DataAttribute(instPtr, parent);
                        break;

                    default:
                        newInstance = new ModelNode(instPtr, parent);
                        break;
                }

                return newInstance;
            }

            /// <summary>
            /// Gets the direct child nodes of this ModelNode instance
            /// </summary>
            /// <returns>List of child nodes</returns>
            public LinkedList<ModelNode> GetChildren()
            {
                LinkedList<ModelNode> children = new LinkedList<ModelNode>();

                IntPtr childListPtr = ModelNode_getChildren(self);

                if (childListPtr != IntPtr.Zero)
                {
                    IedModel iedModel = GetIedModel();

                    IntPtr listElem = LinkedList_getNext(childListPtr);

                    while (listElem != IntPtr.Zero)
                    {
                        IntPtr modelNodePtr = LinkedList_getData(listElem);

                        ModelNode childNode = null;

                        if (iedModel != null)
                        {
                            iedModel.modelNodes.TryGetValue(modelNodePtr, out childNode);
                        }

                        if (childNode == null)
                        {
                            childNode =  ModelNode.CreateInstance(modelNodePtr, this);

                            if ((childNode != null) && (iedModel != null))
                            {
                                iedModel.modelNodes.Add(modelNodePtr, childNode);
                            }

                        }

                        if (childNode != null)
                            children.AddLast(childNode);

                        listElem = LinkedList_getNext(listElem);
                    }

                    LinkedList_destroyStatic(childListPtr);
                }

                return children;
            }

            /// <summary>
            /// Gets the object reference of the model node
            /// </summary>
            /// <returns>the object reference</returns>
            /// <param name="withoutIedName">If set to <c>true</c> the object reference is created without IED name.</param>
            public string GetObjectReference(bool withoutIedName = false)
            {
                IntPtr nativeMemory = Marshal.AllocHGlobal(130);

                IntPtr objRefPtr = ModelNode_getObjectReferenceEx(self, nativeMemory, withoutIedName);

                if (objRefPtr != IntPtr.Zero) {
                    string objRef = Marshal.PtrToStringAnsi(objRefPtr);

                    Marshal.FreeHGlobal(nativeMemory);

                    return objRef;
                }
                else {
                    Marshal.FreeHGlobal(nativeMemory);

                    return null;
                }
            }

        }

        public class DataSet
        {
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr DataSet_create(string name, IntPtr parent);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr DataSet_getName(IntPtr self);

            public IntPtr self = IntPtr.Zero;

            internal DataSet(IntPtr dataSetPtr)
            {
                self = dataSetPtr;
            }

            public DataSet(string name, LogicalNode parent)
            {
                self = DataSet_create(name, parent.self);
            }

            public string Name
            {
                get
                {
                    return Marshal.PtrToStringAnsi(DataSet_getName(self));
                }
            }
        }

        public class DataSetEntry
        {
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr DataSetEntry_create(IntPtr dataSet, string variable, int index, string component);

            public IntPtr self = IntPtr.Zero;

            public DataSetEntry(DataSet dataSet, string variable, int index, string component)
            {
                self = DataSetEntry_create(dataSet.self, variable, index, component);
            }
        }

        /// <summary>
        /// Report control block (RCB) instance for server data model
        /// </summary>
        public class ReportControlBlock
        {
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr ReportControlBlock_create(string name, IntPtr parent, string rptId, [MarshalAs(UnmanagedType.I1)] bool isBuffered,
                string dataSetName, uint confRef, byte trgOps, byte options, uint bufTm, uint intgPd);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void ReportControlBlock_setPreconfiguredClient(IntPtr self, byte type, [Out] byte[] buf);

            public IntPtr self = IntPtr.Zero;

            public ReportControlBlock(string name, LogicalNode parent, string rptId, bool isBuffered,
                string dataSetName, uint confRev, byte trgOps, byte options, uint bufTm, uint intgPd)
            {
                self = ReportControlBlock_create(name, parent.self, rptId, isBuffered, dataSetName, confRev, trgOps, options, bufTm, intgPd);
            }

            public void SetPreconfiguredClient(byte[] clientAddress)
            {
                if (clientAddress.Length == 4)
                    ReportControlBlock_setPreconfiguredClient(self, 4, clientAddress);
                else if (clientAddress.Length == 6)
                    ReportControlBlock_setPreconfiguredClient(self, 6, clientAddress);
            }
        }

        /// <summary>
        /// GOOSE/GSE control block instance for server data model
        /// </summary>
        public class GSEControlBlock
        {
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr GSEControlBlock_create(string name, IntPtr parent, string appId, string dataSet, UInt32 confRev,
                [MarshalAs(UnmanagedType.I1)] bool fixedOffs, int minTime, int maxTime);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void GSEControlBlock_addPhyComAddress(IntPtr self, IntPtr phyComAddress);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr PhyComAddress_create(byte vlanPriority, UInt16 vlanId, UInt16 appId, [Out] byte[] buf);

            internal IntPtr self = IntPtr.Zero;

            public GSEControlBlock(string name, LogicalNode parent, string appId, string dataSetName, UInt32 confRev, bool fixedOffs, int minTime, int maxTime)
            {
                self = GSEControlBlock_create(name, parent.self, appId, dataSetName, confRev, fixedOffs, minTime, maxTime);
            }

            public void AddPhyComAddress(PhyComAddress addr)
            {
                IntPtr phyComAddrPtr = PhyComAddress_create(addr.vlanPriority, addr.vlanId, addr.appId, addr.dstAddress);

                if (phyComAddrPtr != IntPtr.Zero)
                {
                    GSEControlBlock_addPhyComAddress(self, phyComAddrPtr);
                }
                else
                {
                    Console.WriteLine("ERROR: Failed to create native PhyComAddress instance!");
                }
            }
        }

        /// <summary>
        /// Log control block (LCB) instance for server data model
        /// </summary>
        public class LogControlBlock
        {
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr LogControlBlock_create(string name, IntPtr parent, string dataSetName, string logRef, byte trgOps, UInt32 intPeriod,
                    [MarshalAs(UnmanagedType.I1)] bool logEna, [MarshalAs(UnmanagedType.I1)] bool reasonCode);

            internal IntPtr self = IntPtr.Zero;

            public LogControlBlock(string name, LogicalNode parent, string dataSet, string logRef, byte trgOps, UInt32 intPerdiod, bool logEna, bool reasonCode)
            {
                self = LogControlBlock_create(name, parent.self, dataSet, logRef, trgOps, intPerdiod, logEna, reasonCode);
            }
        }

        /// <summary>
        /// Setting group control block for server data model
        /// </summary>
        public class SettingGroupControlBlock
        {
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr SettingGroupControlBlock_create(IntPtr parent, byte actSG, byte numOfSGs);

            internal IntPtr self = IntPtr.Zero;

            public SettingGroupControlBlock(LogicalNode parent, UInt32 actSG, UInt32 numOfSGs)
            {
                self = SettingGroupControlBlock_create(parent.self, (byte) actSG, (byte)  numOfSGs);
            }
        }

        public class ClientConnection 
        {
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr ClientConnection_getPeerAddress(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr ClientConnection_getLocalAddress(IntPtr self);

            internal IntPtr self;

            internal ClientConnection (IntPtr self) {
                this.self = self;
            }

            public string GetPeerAddress()
            {
                IntPtr peerAddrPtr = ClientConnection_getPeerAddress (self);

                if (peerAddrPtr != IntPtr.Zero)
                    return Marshal.PtrToStringAnsi (peerAddrPtr);
                else
                    return null;
            }

            public string GetLocalAddress()
            {
                IntPtr localAddrPtr = ClientConnection_getLocalAddress(self);

                if (localAddrPtr != IntPtr.Zero)
                    return Marshal.PtrToStringAnsi(localAddrPtr);
                else
                    return null;
            }
        }

        public class MmsGooseControlBlock
        {
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr MmsGooseControlBlock_getName(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr MmsGooseControlBlock_getLogicalNode(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr MmsGooseControlBlock_getDataSet(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            [return: MarshalAs(UnmanagedType.I1)]
            static extern bool MmsGooseControlBlock_getGoEna(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern int MmsGooseControlBlock_getMinTime(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern int MmsGooseControlBlock_getMaxTime(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            [return: MarshalAs(UnmanagedType.I1)]
            static extern bool MmsGooseControlBlock_getFixedOffs(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            [return: MarshalAs(UnmanagedType.I1)]
            static extern bool MmsGooseControlBlock_getNdsCom(IntPtr self);

            private IntPtr self;
            private LogicalNode ln = null;
            private DataSet dataSet = null;

            internal MmsGooseControlBlock(IntPtr self, IedModel iedModel)
            {
                this.self = self;

                IntPtr lnPtr = MmsGooseControlBlock_getLogicalNode(self);

                ModelNode lnModelNode = iedModel.GetModelNodeFromNodeRef(lnPtr);

                if (lnModelNode != null && lnModelNode is LogicalNode)
                {
                    this.ln = lnModelNode as LogicalNode;
                }
            }

            public string Name
            {
                get
                {
                    return Marshal.PtrToStringAnsi(MmsGooseControlBlock_getName(self));
                }
            }

            public LogicalNode LN
            {
                get
                {
                    return ln;
                }
            }

            public DataSet DataSet
            {
                get
                {
                    if (dataSet == null)
                        dataSet = new DataSet(MmsGooseControlBlock_getDataSet(self));

                    return dataSet;
                }
            }

            public bool GoEna
            {
                get
                {
                    return MmsGooseControlBlock_getGoEna(self);
                }
            }

            public int MinTime
            {
                get
                {
                    return MmsGooseControlBlock_getMinTime(self);
                }
            }

            public int MaxTime
            {
                get
                {
                    return MmsGooseControlBlock_getMaxTime(self);
                }
            }

            public bool FixedOffs
            {
                get
                {
                    return MmsGooseControlBlock_getFixedOffs(self);
                }
            }

            public bool NdsCom
            {
                get
                {
                    return MmsGooseControlBlock_getNdsCom(self);
                }
            }
        }

        /// <summary>
        /// Represents additional context information of the control action that caused the callback invokation
        /// </summary>
        public class ControlAction
        {
            [DllImport ("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void ControlAction_setAddCause (IntPtr self, int addCause);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void ControlAction_setError(IntPtr self, int error);

            [DllImport ("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern int ControlAction_getOrCat (IntPtr self);

            [DllImport ("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr ControlAction_getOrIdent (IntPtr self, ref int size);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr ControlAction_getClientConnection(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern int ControlAction_getCtlNum(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern UInt64 ControlAction_getControlTime(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            [return: MarshalAs(UnmanagedType.I1)]
            static extern bool ControlAction_isSelect(IntPtr self);

            private IntPtr self;
            private IedServer.ControlHandlerInfo info;
            private IedServer iedServer;

            internal ControlAction (IntPtr self, IedServer.ControlHandlerInfo info, IedServer iedServer)
            {
                this.self = self;
                this.info = info;
                this.iedServer = iedServer;
            }

            /// <summary>
            /// Sets the error code for the next command termination or application error message.
            /// </summary>
            /// <param name="error">the errror code to use</param>
            public void SetError(ControlLastApplError error)
            {
                ControlAction_setError(self, (int)error);
            }

            /// <summary>
            /// Sets the add cause for the next command termination or application error message
            /// </summary>
            /// <param name="addCause">the additional cause code</param>
            public void SetAddCause (ControlAddCause addCause)
            {
                ControlAction_setAddCause (self, (int)addCause);
            }

            /// <summary>
            /// Gets the originator category provided by the client
            /// </summary>
            /// <returns>The or cat.</returns>
            public OrCat GetOrCat ()
            {
                return (OrCat)ControlAction_getOrCat (self);
            }

            /// <summary>
            ///  Get the originator identifier provided by the client
            /// </summary>
            /// <returns>The or ident.</returns>
            public byte [] GetOrIdent ()
            {
                int size = 0;

                IntPtr orIdentPtr = ControlAction_getOrIdent (self, ref size);

                if (orIdentPtr == IntPtr.Zero)
                    return null;

                byte [] orIdent = new byte [size];

                Marshal.Copy (orIdentPtr, orIdent, 0, size);

                return orIdent;
            }

            /// <summary>
            /// Gets the ctlNum attribute of the control action
            /// </summary>
            /// <returns>The ctlNum value. Valid values are restricted from 0 to 255, -1 means not present</returns>
            public int GetCtlNum()
            {
                return ControlAction_getCtlNum(self);
            }

            /// <summary>
            /// Gets the control object that is subject to this action
            /// </summary>
            /// <returns>the controllable data object instance</returns>
            public DataObject GetControlObject ()
            {
                return info.controlObject;
            }

            /// <summary>
            ///  Gets the time of control execution, if it's a time activated control
            /// </summary>
            /// <returns>The time of control execution or 0 for immediate execution</returns>
            public UInt64 GetControlTime()
            {
                return ControlAction_getControlTime(self);
            }

            /// <summary>
            /// Gets the tome of control execution as data time offset.
            /// </summary>
            /// <returns>The control execution time as data time offset.</returns>
            public DateTimeOffset GetControlTimeAsDataTimeOffset()
            {
                return MmsValue.MsTimeToDateTimeOffset(GetControlTime());
            }

            /// <summary>
            /// Gets the client object associated with the client that caused the control action
            /// </summary>
            /// <returns>The client connection.</returns>
            public ClientConnection GetClientConnection ()
            {
                ClientConnection con = null;

                IntPtr conPtr = ControlAction_getClientConnection (self);

                if (conPtr != IntPtr.Zero) {
                    iedServer.clientConnections.TryGetValue (conPtr, out con);
                }

                return con;
            }

            /// <summary>
            /// Cehck if the control callback is called by a select or operate command
            /// </summary>
            /// <returns><c>true</c>, if select, <c>false</c> otherwise.</returns>
            public bool IsSelect()
            {
                return ControlAction_isSelect(self);
            }
        }

        public delegate void GoCBEventHandler(MmsGooseControlBlock goCB, int cbEvent, object parameter);

        public delegate MmsDataAccessError WriteAccessHandler (DataAttribute dataAttr, MmsValue value, 
            ClientConnection connection, object parameter);

        /// <summary>
        /// Reason for the select state change
        /// </summary>
        public enum SelectStateChangedReason
        {
            /// <summary>
            /// Control has been selected
            /// </summary>
            SELECT_STATE_REASON_SELECTED = 0,
            /// <summary>
            /// Cancel received for the control
            /// </summary>
            SELECT_STATE_REASON_CANCELED = 1,
            /// <summary>
            /// Unselected due to timeout (sboTimeout)
            /// </summary>
            SELECT_STATE_REASON_TIMEOUT = 2,
            /// <summary>
            /// Unselected due to successful operate
            /// </summary>
            SELECT_STATE_REASON_OPERATED = 3,
            /// <summary>
            /// Unselected due to failed operate
            /// </summary>
            SELECT_STATE_REASON_OPERATE_FAILED = 4,
            /// <summary>
            /// Unselected due to disconnection of selecting client
            /// </summary>
            SELECT_STATE_REASON_DISCONNECTED = 5
        }

        public delegate void ControlSelectStateChangedHandler(ControlAction action, object parameter, bool isSelected, SelectStateChangedReason reason);

        /// <summary>
        /// Return type of ControlHandler and ControlWaitForExecutionHandler
        /// </summary>
        public enum ControlHandlerResult {
            /// <summary>
            /// check or operation failed
            /// </summary>
            FAILED = 0,
            /// <summary>
            /// check or operation was successful
            /// </summary>
            OK = 1,
            /// <summary>
            /// check or operation is in progress
            /// </summary>
            WAITING = 2
        }

        public delegate ControlHandlerResult ControlWaitForExecutionHandler (ControlAction action, object parameter, MmsValue ctlVal, bool test, bool synchroCheck);

        public delegate ControlHandlerResult ControlHandler (ControlAction action, object parameter, MmsValue ctlVal, bool test);

        public enum CheckHandlerResult {
            /// <summary>
            /// check passed
            /// </summary>
            ACCEPTED = -1,
            /// <summary>
            /// check failed due to hardware fault
            /// </summary>
            HARDWARE_FAULT = 1,
            /// <summary>
            /// control is already selected or operated
            /// </summary>
            TEMPORARILY_UNAVAILABLE = 2,
            /// <summary>
            /// check failed due to access control reason - access denied for this client or state
            /// </summary>
            OBJECT_ACCESS_DENIED = 3,
            /// <summary>
            /// object not visible in this security context ??? 
            /// </summary>
            OBJECT_UNDEFINED = 4
        }

        public delegate CheckHandlerResult CheckHandler (ControlAction action, object parameter, MmsValue ctlVal, bool test, bool interlockCheck);

        /// <summary>
        /// This class acts as the entry point for the IEC 61850 client API. It represents a single
        /// (MMS) connection to a server.
        /// </summary>
        public class IedServer : IDisposable
        {
            [DllImport ("iec61850", CallingConvention=CallingConvention.Cdecl)]
            static extern IntPtr IedServer_createWithConfig(IntPtr modelRef, IntPtr tlsConfiguration, IntPtr serverConfiguratio);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_setLocalIpAddress(IntPtr self, string localIpAddress);

            [DllImport ("iec61850", CallingConvention=CallingConvention.Cdecl)]
            static extern void IedServer_start(IntPtr self, int tcpPort);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_stop(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_destroy(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            [return: MarshalAs(UnmanagedType.Bool)]
            static extern bool IedServer_isRunning(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern int IedServer_getNumberOfOpenConnections(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_lockDataModel(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_unlockDataModel(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_updateAttributeValue(IntPtr self, IntPtr DataAttribute, IntPtr MmsValue);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_updateBooleanAttributeValue(IntPtr self, IntPtr dataAttribute, [MarshalAs(UnmanagedType.I1)] bool value);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_updateInt32AttributeValue(IntPtr self, IntPtr dataAttribute, int value);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_updateInt64AttributeValue(IntPtr self, IntPtr dataAttribute, Int64 value);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_updateFloatAttributeValue(IntPtr self, IntPtr dataAttribute, float value);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_updateVisibleStringAttributeValue(IntPtr self, IntPtr dataAttribute, string value);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_updateUTCTimeAttributeValue(IntPtr self, IntPtr dataAttribute, ulong value);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_updateTimestampAttributeValue(IntPtr self, IntPtr dataAttribute, IntPtr timestamp);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_updateQuality(IntPtr self, IntPtr dataAttribute, ushort value);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_setServerIdentity(IntPtr self, string vendor, string model, string revision);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr IedServer_getAttributeValue(IntPtr self, IntPtr dataAttribute);

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            private delegate int InternalControlPerformCheckHandler (IntPtr action, IntPtr parameter, IntPtr ctlVal, [MarshalAs(UnmanagedType.I1)] bool test, [MarshalAs(UnmanagedType.I1)] bool interlockCheck);

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            private delegate int InternalControlWaitForExecutionHandler (IntPtr action, IntPtr parameter, IntPtr ctlVal, [MarshalAs(UnmanagedType.I1)] bool test, [MarshalAs(UnmanagedType.I1)] bool synchoCheck);

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            private delegate int InternalControlHandler (IntPtr action, IntPtr parameter, IntPtr ctlVal, [MarshalAs(UnmanagedType.I1)] bool test);

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            private delegate void InternalSelectStateChangedHandler(IntPtr action, IntPtr parameter, [MarshalAs(UnmanagedType.I1)] bool isSelected, int reason);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_setWaitForExecutionHandler(IntPtr self, IntPtr node, InternalControlWaitForExecutionHandler handler, IntPtr parameter);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_setPerformCheckHandler(IntPtr self, IntPtr node, InternalControlPerformCheckHandler handler, IntPtr parameter);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_setControlHandler (IntPtr self, IntPtr node, InternalControlHandler handler, IntPtr parameter);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_setSelectStateChangedHandler(IntPtr self, IntPtr node, InternalSelectStateChangedHandler handler, IntPtr parameter);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_setWriteAccessPolicy(IntPtr self, FunctionalConstraint fc, AccessPolicy policy);

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            private delegate int InternalWriteAccessHandler (IntPtr dataAttribute, IntPtr value, IntPtr connection, IntPtr parameter);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_handleWriteAccess(IntPtr self, IntPtr dataAttribute,
                InternalWriteAccessHandler handler, IntPtr parameter);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_handleWriteAccessForComplexAttribute(IntPtr self, IntPtr dataAttribute,
                InternalWriteAccessHandler handler, IntPtr parameter);

            public delegate void ConnectionIndicationHandler(IedServer iedServer, ClientConnection clientConnection, bool connected, object parameter);

            private ConnectionIndicationHandler connectionHandler = null;
            private object connectionHandlerParameter = null;

            public void SetConnectionIndicationHandler(ConnectionIndicationHandler handler, object parameter)
            {
                connectionHandler = handler;
                connectionHandlerParameter = parameter;
            }

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            private delegate void InternalConnectionHandler (IntPtr iedServer, IntPtr clientConnection, [MarshalAs(UnmanagedType.I1)] bool connected, IntPtr parameter);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_setConnectionIndicationHandler(IntPtr self, InternalConnectionHandler handler, IntPtr parameter);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_enableGoosePublishing(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_disableGoosePublishing(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_setGooseInterfaceId(IntPtr self, string interfaceId);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_setGooseInterfaceIdEx(IntPtr self, IntPtr ln, string gcbName, string interfaceId);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_useGooseVlanTag(IntPtr self, IntPtr ln, string gcbName, [MarshalAs(UnmanagedType.I1)] bool useVlanTag);

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            private delegate void InternalGoCBEventHandler(IntPtr goCB, int cbEvent, IntPtr parameter);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_setGoCBHandler(IntPtr self, InternalGoCBEventHandler handler, IntPtr parameter);

            private IntPtr self = IntPtr.Zero;

            private InternalControlHandler internalControlHandlerRef = null;
            private InternalControlPerformCheckHandler internalControlPerformCheckHandlerRef = null;
            private InternalControlWaitForExecutionHandler internalControlWaitForExecutionHandlerRef = null;
            private InternalSelectStateChangedHandler internalSelectedStateChangedHandlerRef = null;

            internal class ControlHandlerInfo {
                public DataObject controlObject = null;
                public GCHandle handle;

                public ControlHandler controlHandler = null;
                public object controlHandlerParameter = null;

                public CheckHandler checkHandler = null;
                public object checkHandlerParameter = null;

                public ControlWaitForExecutionHandler waitForExecHandler = null;
                public object waitForExecHandlerParameter = null;

                public ControlSelectStateChangedHandler selectStateChangedHandler = null;
                public object selectStateChangedHandlerParameter = null;

                public ControlHandlerInfo(DataObject controlObject)
                {
                    this.controlObject = controlObject;
                    this.handle = GCHandle.Alloc(this);
                }

                ~ControlHandlerInfo()
                {
                    this.handle.Free();
                }
            }

            private Dictionary<DataObject, ControlHandlerInfo> controlHandlers = new Dictionary<DataObject, ControlHandlerInfo> ();

            int InternalControlHandlerImpl (IntPtr action, IntPtr parameter, IntPtr ctlVal, bool test)
            {
                GCHandle handle = GCHandle.FromIntPtr (parameter);

                ControlHandlerInfo info = (ControlHandlerInfo)handle.Target;

                ControlAction controlAction = new ControlAction (action, info, this);

                if (info != null && info.controlHandler != null)
                    return (int)info.controlHandler (controlAction, info.controlHandlerParameter, new MmsValue (ctlVal), test);
                else
                    return (int)ControlHandlerResult.FAILED;
            }

            int InternalCheckHandlerImpl(IntPtr action, IntPtr parameter, IntPtr ctlVal, bool test, bool interlockCheck)
            {
                GCHandle handle = GCHandle.FromIntPtr (parameter);

                ControlHandlerInfo info = (ControlHandlerInfo)handle.Target;

                if (info != null && info.checkHandler != null) 
                {
                    ControlAction controlAction = new ControlAction (action, info, this);

                    return (int)info.checkHandler (controlAction, info.checkHandlerParameter, new MmsValue (ctlVal), test, interlockCheck); 
                } else
                    return (int)CheckHandlerResult.OBJECT_UNDEFINED;
            }

            int InternalControlWaitForExecutionHandlerImpl (IntPtr action, IntPtr parameter, IntPtr ctlVal, bool test, bool synchoCheck)
            {
                GCHandle handle = GCHandle.FromIntPtr (parameter);

                ControlHandlerInfo info = (ControlHandlerInfo)handle.Target;

                if (info != null && info.waitForExecHandler != null)
                {
                    ControlAction controlAction = new ControlAction (action, info, this);

                    return (int)info.waitForExecHandler (controlAction, info.waitForExecHandlerParameter, new MmsValue (ctlVal), test, synchoCheck);
                } 
                else
                    return (int)ControlHandlerResult.FAILED;
            }

            void InternalSelectStateChangedHandlerImpl(IntPtr action, IntPtr parameter, bool isSelected, int reason)
            {
                GCHandle handle = GCHandle.FromIntPtr(parameter);

                ControlHandlerInfo info = (ControlHandlerInfo)handle.Target;

                if (info != null && info.selectStateChangedHandler != null)
                {
                    ControlAction controlAction = new ControlAction(action, info, this);

                    info.selectStateChangedHandler(controlAction, info.selectStateChangedHandlerParameter, isSelected, (SelectStateChangedReason)reason);
                }
            }

            private struct WriteAccessHandlerInfo {
                public WriteAccessHandler handler;
                public InternalWriteAccessHandler internalHandler;
                public object parameter;
                public DataAttribute dataAttribute;

                public WriteAccessHandlerInfo (WriteAccessHandler h, object p, DataAttribute da, InternalWriteAccessHandler internalHandler) 
                {
                    handler = h;
                    parameter = p;
                    dataAttribute = da;
                    this.internalHandler = internalHandler;
                }
            }

            int WriteAccessHandlerImpl (IntPtr dataAttribute, IntPtr value, IntPtr connection, IntPtr parameter)
            {
                WriteAccessHandlerInfo info;

                if (writeAccessHandlers.TryGetValue(dataAttribute, out info) == true)
                {
                    ClientConnection con = null;

                    clientConnections.TryGetValue(connection, out con);

                    MmsValue mmsValue = new MmsValue(value);

                    return (int)info.handler(info.dataAttribute, mmsValue.Clone(), con, info.parameter);
                }
                else
                {
                    return (int) MmsDataAccessError.OBJECT_ACCESS_DENIED;
                }
            }

            private Dictionary<IntPtr, WriteAccessHandlerInfo> writeAccessHandlers = new Dictionary<IntPtr, WriteAccessHandlerInfo> ();

            private void ConnectionIndicationHandlerImpl (IntPtr iedServer, IntPtr clientConnection, bool connected, IntPtr parameter)
            {
                if (connected == false) {
                    ClientConnection con = null;

                    clientConnections.TryGetValue (clientConnection, out con);

                    if (con != null) {
                        
                        if (connectionHandler != null)
                            connectionHandler (this, con, false, connectionHandlerParameter);

                        clientConnections.Remove (clientConnection);
                    }
                } else {
                    ClientConnection con = new ClientConnection (clientConnection);

                    clientConnections.Add (clientConnection, con);

                    if (connectionHandler != null)
                        connectionHandler (this, con, true, connectionHandlerParameter);
                }
            }

            internal Dictionary<IntPtr, ClientConnection> clientConnections = new Dictionary<IntPtr, ClientConnection> ();

            /* store IedModel instance to prevent garbage collector */
            private IedModel iedModel = null;

            public IedServer(IedModel iedModel, IedServerConfig config = null)
            {
                this.iedModel = iedModel;

                IntPtr nativeConfig = IntPtr.Zero;

                if (config != null)
                    nativeConfig = config.self;

                self = IedServer_createWithConfig (iedModel.self, IntPtr.Zero, nativeConfig);
            }

            public IedServer(IedModel iedModel, TLSConfiguration tlsConfig, IedServerConfig config = null)
            {
                this.iedModel = iedModel;

                IntPtr nativeConfig = IntPtr.Zero;
                IntPtr nativeTLSConfig = IntPtr.Zero;

                if (config != null)
                    nativeConfig = config.self;

                if (tlsConfig != null)
                    nativeTLSConfig = tlsConfig.GetNativeInstance ();

                self = IedServer_createWithConfig (iedModel.self, nativeTLSConfig, nativeConfig);
            }

            private InternalConnectionHandler internalConnectionHandler = null;

            /// <summary>
            /// Sets the local ip address for listening
            /// </summary>
            /// <param name="localIpAddress">Local IP address.</param>
            public void SetLocalIpAddress(string localIpAddress)
            {
                IedServer_setLocalIpAddress (self, localIpAddress);
            }

            /// <summary>
            /// Start MMS server
            /// </summary>
            /// <param name="localIpAddress">Local IP address.</param>
            /// <param name="tcpPort">TCP port to use</param>
            public void Start(string localIpAddress, int tcpPort)
            {
                SetLocalIpAddress (localIpAddress);
                Start (tcpPort);
            }

            /// <summary>Start MMS server</summary>
            /// <param name="tcpPort">TCP port to use</param>
            public void Start(int tcpPort)
            {
                if (internalConnectionHandler == null)
                    internalConnectionHandler = new InternalConnectionHandler (ConnectionIndicationHandlerImpl);                    

                IedServer_setConnectionIndicationHandler (self, internalConnectionHandler, IntPtr.Zero);

                IedServer_start(self, tcpPort);
            }

            /// <summary>Start MMS server</summary>
            public void Start ()
            {
                Start(-1);
            }

            /// <summary>
            /// Stop the MMS server.
            /// </summary>
            /// <description>This function will stop the server. This will close the TCP server socket and all client sockets.</description>
            public void Stop()
            {
                IedServer_stop(self);
                internalConnectionHandler = null;
            }

            /// <summary>
            /// Release all server resources (same as <see cref="Dispose"/>)
            /// </summary>
            /// <description>This function releases all MMS server resources.</description>
            public void Destroy()
            {
                Dispose();
            }

            /// <summary>
            /// Releases all resource used by the <see cref="IEC61850.Server.IedServer"/> object.
            /// </summary>
            /// <remarks>Call <see cref="Dispose"/> when you are done using the <see cref="IEC61850.Server.IedServer"/>. The
            /// <see cref="Dispose"/> method leaves the <see cref="IEC61850.Server.IedServer"/> in an unusable state. After
            /// calling <see cref="Dispose"/>, you must release all references to the <see cref="IEC61850.Server.IedServer"/> so
            /// the garbage collector can reclaim the memory that the <see cref="IEC61850.Server.IedServer"/> was occupying.</remarks>
            public void Dispose()
            {
                lock (this)
                {
                    if (self != IntPtr.Zero)
                    {
                        IedServer_destroy(self);
                        self = IntPtr.Zero;
                        internalConnectionHandler = null;
                        this.iedModel = null;
                    }
                }
            }

            ~IedServer()
            {
                Dispose();
            }

            /// <summary>
            /// Set the identify for the MMS identify service
            /// </summary>
            /// <param name="vendor">the IED vendor name</param>
            /// <param name="model">the IED model name</param>
            /// <param name="revision">the IED revision/version number</param>
            public void SetServerIdentity(string vendor, string model, string revision)
            {
                IedServer_setServerIdentity(self, vendor, model, revision);
            }

            /// <summary>
            /// Check if server is running (accepting client connections)
            /// </summary>
            /// <returns><c>true</c>, if running, <c>false</c> otherwise.</returns>
            public bool IsRunning()
            {
                return IedServer_isRunning(self);
            }

            /// <summary>
            /// Get number of open MMS connections
            /// </summary>
            /// <returns>the number of open and accepted MMS connections</returns>
            public int GetNumberOfOpenConnections()
            {
                return IedServer_getNumberOfOpenConnections(self);
            }

            private ControlHandlerInfo GetControlHandlerInfo(DataObject controlObject)
            {
                ControlHandlerInfo info;

                controlHandlers.TryGetValue (controlObject, out info);

                if (info == null) {
                    info = new ControlHandlerInfo (controlObject);
                    controlHandlers.Add (controlObject, info);
                }

                return info;
            }

            public void SetControlHandler (DataObject controlObject, ControlHandler handler, object parameter)
            {
                ControlHandlerInfo info = GetControlHandlerInfo (controlObject);

                info.controlHandler = handler;
                info.controlHandlerParameter = parameter;

                if (internalControlHandlerRef == null)
                    internalControlHandlerRef = new InternalControlHandler (InternalControlHandlerImpl);

                IedServer_setControlHandler(self, controlObject.self, internalControlHandlerRef, GCHandle.ToIntPtr(info.handle));
            }

            public void SetCheckHandler (DataObject controlObject, CheckHandler handler, object parameter)
            {
                ControlHandlerInfo info = GetControlHandlerInfo (controlObject);

                info.checkHandler = handler;
                info.checkHandlerParameter = parameter;

                if (internalControlPerformCheckHandlerRef == null)
                    internalControlPerformCheckHandlerRef = new InternalControlPerformCheckHandler (InternalCheckHandlerImpl);

                IedServer_setPerformCheckHandler(self, controlObject.self, internalControlPerformCheckHandlerRef, GCHandle.ToIntPtr(info.handle));
            }

            public void SetWaitForExecutionHandler (DataObject controlObject, ControlWaitForExecutionHandler handler, object parameter)
            {
                ControlHandlerInfo info = GetControlHandlerInfo (controlObject);

                info.waitForExecHandler = handler;
                info.waitForExecHandlerParameter = parameter;

                if (internalControlWaitForExecutionHandlerRef == null)
                    internalControlWaitForExecutionHandlerRef = new InternalControlWaitForExecutionHandler (InternalControlWaitForExecutionHandlerImpl);

                IedServer_setWaitForExecutionHandler(self, controlObject.self, internalControlWaitForExecutionHandlerRef, GCHandle.ToIntPtr(info.handle));
            }

            /// <summary>
            /// Set a callback handler for a controllable data object to track select state changes
            /// </summary>
            /// <param name="controlObject">Control object.</param>
            /// <param name="handler">Handler.</param>
            /// <param name="parameter">A user provided parameter that is passed to the callback handler.</param>
            public void SetSelectStateChangedHandler(DataObject controlObject, ControlSelectStateChangedHandler handler, object parameter)
            {
                ControlHandlerInfo info = GetControlHandlerInfo(controlObject);

                info.selectStateChangedHandler = handler;
                info.selectStateChangedHandlerParameter = parameter;

                if (internalSelectedStateChangedHandlerRef == null)
                    internalSelectedStateChangedHandlerRef = new InternalSelectStateChangedHandler(InternalSelectStateChangedHandlerImpl);

                IedServer_setSelectStateChangedHandler(self, controlObject.self, internalSelectedStateChangedHandlerRef, GCHandle.ToIntPtr(info.handle));
            }

            /// <summary>
            /// Install a WriteAccessHandler for a data attribute.
            /// </summary>
            /// This instructs the server to monitor write attempts by MMS clients to specific
            /// data attributes.If a client tries to write to the monitored data attribute the
            /// handler is invoked.The handler can decide if the write access will be allowed
            /// or denied.If a WriteAccessHandler is set for a specific data attribute - the
            /// default write access policy will not be performed for that data attribute.
            /// <remarks>
            /// NOTE: If the data attribute has sub data attributes, the WriteAccessHandler is not
            /// set for the sub data attributes and will not be called when the sub data attribute is
            /// written directly!
            /// </remarks>
            /// <param name="dataAttr">the data attribute to monitor</param>
            /// <param name="handler">the callback function that is invoked if a client tries to write to the monitored data attribute.</param>
            /// <param name="parameter">a user provided parameter that is passed to the WriteAccessHandler when called.</param>
            public void HandleWriteAccess (DataAttribute dataAttr, WriteAccessHandler handler, object parameter)
            {
                InternalWriteAccessHandler internalHandler = new InternalWriteAccessHandler(WriteAccessHandlerImpl);
 
                writeAccessHandlers.Add (dataAttr.self, new WriteAccessHandlerInfo(handler, parameter, dataAttr, internalHandler));

                IedServer_handleWriteAccess (self, dataAttr.self, internalHandler, IntPtr.Zero);
            }

            private void AddHandlerInfoForDataAttributeRecursive(DataAttribute da, WriteAccessHandler handler, object parameter, InternalWriteAccessHandler internalHandler)
            {
                writeAccessHandlers.Add(da.self, new WriteAccessHandlerInfo(handler, parameter, da, internalHandler));

                foreach (ModelNode child in da.GetChildren())
                {
                    if (child is DataAttribute)
                    {
                        AddHandlerInfoForDataAttributeRecursive(child as DataAttribute, handler, parameter, internalHandler);
                    }
                }
            }

            /// <summary>
            /// Install a WriteAccessHandler for a data attribute and for all sub data attributes
            /// </summary>
            /// This instructs the server to monitor write attempts by MMS clients to specific
            /// data attributes.If a client tries to write to the monitored data attribute the
            /// handler is invoked.The handler can decide if the write access will be allowed
            /// or denied.If a WriteAccessHandler is set for a specific data attribute - the
            /// default write access policy will not be performed for that data attribute.
            /// <remarks>
            /// NOTE: When the data attribute is a complex attribute then the handler will also be installed
            /// for all sub data attributes. When the data attribute is a basic data attribute then
            /// this function behaves like  <see cref="HandleWriteAccess"/>.
            /// </remarks>
            /// <param name="dataAttr">the data attribute to monitor</param>
            /// <param name="handler">the callback function that is invoked if a client tries to write to the monitored data attribute.</param>
            /// <param name="parameter">a user provided parameter that is passed to the WriteAccessHandler when called.</param>
            public void HandleWriteAccessForComplexAttribute(DataAttribute dataAttr, WriteAccessHandler handler, object parameter)
            {
                InternalWriteAccessHandler internalHandler = new InternalWriteAccessHandler(WriteAccessHandlerImpl);

                AddHandlerInfoForDataAttributeRecursive(dataAttr, handler, parameter, internalHandler);

                IedServer_handleWriteAccessForComplexAttribute(self, dataAttr.self, internalHandler, IntPtr.Zero);
            }

            /// <summary>
            /// Set the defualt write access policy for a specific FC. The default policy is applied when no handler is installed for a data attribute
            /// </summary>
            /// <param name="fc">The functional constraint (FC)</param>
            /// <param name="policy">The new default access policy</param>
            public void SetWriteAccessPolicy(FunctionalConstraint fc, AccessPolicy policy)
            {
                IedServer_setWriteAccessPolicy (self, fc, policy);
            }

            /// <summary>
            /// Locks the data model for data update.
            /// </summary>
            /// This function should be called before the data model is updated.
            /// After updating the data model the function <see cref="UnlockDataModel"/> should be called.
            /// 
            /// <remarks>
            /// his method should never be called inside of a library callback function. In the context of
            /// a library callback the data model is always already locked! Calling this function inside of a
            /// library callback may lead to a deadlock condition.
            /// </remarks>
            public void LockDataModel()
            {
                IedServer_lockDataModel(self);
            }

            /// <summary>
            /// Unlocks the data model and process pending client requests.
            /// </summary>
            /// 
            /// <remarks>
            /// This method should never be called inside of a library callback function. In the context of
            /// a library callback the data model is always already locked!
            /// </remarks>
            public void UnlockDataModel()
            {
                IedServer_unlockDataModel(self);
            }

            public void UpdateAttributeValue(DataAttribute dataAttr, MmsValue value)
            {
                IedServer_updateAttributeValue (self, dataAttr.self, value.valueReference);
            }

            public void UpdateBooleanAttributeValue(DataAttribute dataAttr, bool value)
            {
                IedServer_updateBooleanAttributeValue(self, dataAttr.self, value);
            }

            public void UpdateFloatAttributeValue(DataAttribute dataAttr, float value)
            {
                IedServer_updateFloatAttributeValue(self, dataAttr.self, value);
            }

            public void UpdateInt32AttributeValue(DataAttribute dataAttr, int value)
            {
                IedServer_updateInt32AttributeValue(self, dataAttr.self, value);
            }

            public void UpdateInt64AttributeValue(DataAttribute dataAttr, Int64 value)
            {
                IedServer_updateInt64AttributeValue (self, dataAttr.self, value);
            }

            public void UpdateVisibleStringAttributeValue(DataAttribute dataAttr, string value)
            {
                IedServer_updateVisibleStringAttributeValue(self, dataAttr.self, value);
            }

            public void UpdateUTCTimeAttributeValue(DataAttribute dataAttr, DateTime timestamp)
            {
                DateTime epoch = new DateTime(1970, 1, 1, 0, 0, 0, DateTimeKind.Utc);
                DateTime timestampUTC = timestamp.ToUniversalTime();

                TimeSpan timeDiff = timestampUTC - epoch;
                ulong timeVal = Convert.ToUInt64(timeDiff.TotalMilliseconds);

                IedServer_updateUTCTimeAttributeValue(self, dataAttr.self, timeVal);
            }

            public void UpdateTimestampAttributeValue(DataAttribute dataAttr, Timestamp timestamp)
            {
                IedServer_updateTimestampAttributeValue (self, dataAttr.self, timestamp.timestampRef);
            }

            public void UpdateQuality(DataAttribute dataAttr, ushort value)
            {
                IedServer_updateQuality(self, dataAttr.self, value);
            }

            public MmsValue GetAttributeValue(DataAttribute dataAttr)
            {
                IntPtr mmsValuePtr = IedServer_getAttributeValue (self, dataAttr.self);

                if (mmsValuePtr != IntPtr.Zero)
                    return new MmsValue (mmsValuePtr);
                else
                    return null;
            }

            /// <summary>
            /// Enable all GOOSE control blocks.
            /// </summary>
            /// This will set the GoEna attribute of all configured GOOSE control blocks
            /// to true. If this method is not called at the startup or reset of the server
            /// then configured GOOSE control blocks keep inactive until a MMS client enables
            /// them by writing to the GOOSE control block.
            public void EnableGoosePublishing()
            {
                IedServer_enableGoosePublishing(self);
            }

            /// <summary>
            /// Disable all GOOSE control blocks.
            /// </summary>
            /// This will set the GoEna attribute of all configured GOOSE control blocks
            /// to false. This will stop GOOSE transmission.
            public void DisableGoosePublishing()
            {
                IedServer_disableGoosePublishing(self);
            }

            /// <summary>
            /// Set the Ethernet interface to be used by GOOSE publishing
            /// </summary>
            /// 
            /// This function can be used to set the GOOSE interface ID. If not used or set to null the
            /// default interface ID from stack_config.h is used.Note the interface ID is operating system
            /// specific!
            /// 
            /// <param name="interfaceId">the ID of the ethernet interface to be used for GOOSE publishing</param>
            public void SetGooseInterfaceId(string interfaceId)
            {
                IedServer_setGooseInterfaceId(self, interfaceId);
            }

            /// <summary>
            /// Set the Ethernet interface to be used by GOOSE publishing
            /// </summary>
            /// 
            /// This function can be used to set the GOOSE interface ID for all GCBs (parameter ln = null) or for
            /// a specific GCB specified by the logical node instance and the GCB name.
            /// 
            /// <param name="ln">ln the logical node that contains the GCB or null to set the ethernet interface ID for all GCBs</param>
            /// <param name="gcbName">the name (not object reference!) of the GCB</param>
            /// <param name="interfaceId">the ID of the ethernet interface to be used for GOOSE publishing</param>
            public void SetGooseInterfaceId(LogicalNode ln, string gcbName, string interfaceId)
            {
                if (ln == null)
                    IedServer_setGooseInterfaceIdEx(self, IntPtr.Zero, gcbName, interfaceId);
                else
                    IedServer_setGooseInterfaceIdEx(self, ln.self, gcbName, interfaceId);
            }

            /// <summary>
            /// Enable/disable the use of VLAN tags in GOOSE messages
            /// </summary>
            /// 
            /// This function can be used to enable/disable VLAN tagging for all GCBs (parameter ln = null) or for
            /// a specific GCB specified by the logical node instance and the GCB name.
            /// 
            /// <param name="ln">the logical node that contains the GCB or null to enable/disable VLAN tagging for all GCBs</param>
            /// <param name="gcbName">the name (not object reference!) of the GCB</param>
            /// <param name="useVlanTag">true to enable VLAN tagging, false otherwise</param>
            public void UseGooseVlanTag(LogicalNode ln, string gcbName, bool useVlanTag)
            {
                if (ln == null)
                    IedServer_useGooseVlanTag(self, IntPtr.Zero, gcbName, useVlanTag);
                else
                    IedServer_useGooseVlanTag(self, ln.self, gcbName, useVlanTag);
            }

            private GoCBEventHandler goCbEventHandler = null;
            private object goCbEventHandlerParameter = null;

            private InternalGoCBEventHandler internalGoCBEventHandler = null;

            private void InternalGoCBEventHandlerImplementation(IntPtr goCB, int cbEvent, IntPtr parameter)
            {
                if (goCbEventHandler != null)
                {
                    MmsGooseControlBlock mmsGoCb = new MmsGooseControlBlock(goCB, iedModel);

                    goCbEventHandler.Invoke(mmsGoCb, cbEvent, goCbEventHandlerParameter);
                }
            }

            /// <summary>
            /// Set a callback handler for GoCB events (enabled/disabled)
            /// </summary>
            /// <param name="handler">the callback handler</param>
            /// <param name="parameter">user provided parameter that is passed to the callback handler</param>
            public void SetGoCBHandler(GoCBEventHandler handler, object parameter)
            {
                goCbEventHandler = handler;
                goCbEventHandlerParameter = parameter;
   
                if (internalGoCBEventHandler == null)
                {
                    internalGoCBEventHandler = new InternalGoCBEventHandler(InternalGoCBEventHandlerImplementation);

                    IedServer_setGoCBHandler(self, internalGoCBEventHandler, IntPtr.Zero);
                }
            }

        }

    }
}
