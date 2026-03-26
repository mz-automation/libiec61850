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
                    if (parent != null)
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
                            childNode = ModelNode.CreateInstance(modelNodePtr, this);

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

                if (objRefPtr != IntPtr.Zero)
                {
                    string objRef = Marshal.PtrToStringAnsi(objRefPtr);

                    Marshal.FreeHGlobal(nativeMemory);

                    return objRef;
                }
                else
                {
                    Marshal.FreeHGlobal(nativeMemory);

                    return null;
                }
            }

        }

    }
}