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
        public class DataObject : ModelNode
        {
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr DataObject_create(string name, IntPtr parent, int arrayElements);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr ModelNode_getChildWithFc(IntPtr self, string objectReference, int fc);

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

                self = DataObject_create(name, parent.self, arrayElements);
            }

            /// <summary>
            /// return a child model node with a given functional constraint
            /// Sometimes the name is not enough to identify a model node.This is the case when
            /// editable setting groups are used.In this case the setting group members have two different
            /// model nodes associated that differ in their FC (SG and SE).
            /// </summary>
            /// <param name="objectReference">the name of the child model node</param>
            /// <param name="fc">the functional constraint of the model node</param>
            /// <returns>the model node instance or NULL if model node does not exist.</returns>
            public DataAttribute GetChildWithFc(string objRef, FunctionalConstraint fc)
            {
                DataAttribute dataAttribute = null;
                IntPtr da = ModelNode_getChildWithFc(self, objRef, (int)fc);

                if (da != IntPtr.Zero)
                {
                    dataAttribute = new DataAttribute(da, this);
                }

                return dataAttribute;
            }

        }

    }
}