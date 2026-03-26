using IEC61850.Common;
using IEC61850.Model;
using IEC61850.TLS;
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

// IEC 61850 API for the libiec61850 .NET wrapper library
namespace IEC61850
{
    // IEC 61850 server API.
    namespace Server
    {
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

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr IedModel_getSVControlBlock(IntPtr self, IntPtr parentLN, string svcbName);

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

                return GetModelNodeFromNodeRef(nodeRef);
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

                return GetModelNodeFromNodeRef(nodeRef);
            }

            public SVControlBlock GetSVControlBlock(LogicalNode logicalNode, string svcbName)
            {
                IntPtr nodeRef = IedModel_getSVControlBlock(self, logicalNode.self, svcbName);

                if (nodeRef == IntPtr.Zero)
                    return null;

                return new SVControlBlock(nodeRef);
            }

        }
    }
}