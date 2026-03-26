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
        /// Logical node. Representation of a logical node (LN) in a data model.
        /// </summary>
        public class LogicalNode : ModelNode
        {
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr LogicalNode_create(string name, IntPtr parent);

            internal Dictionary<IntPtr, ReportControlBlock> rcbs = new Dictionary<IntPtr, ReportControlBlock>();

            public LogicalNode(IntPtr self, ModelNode parent) : base(self)
            {
                this.parent = parent;
            }

            /// <summary>
            /// Initializes a new instance of the <see cref="T:IEC61850.Server.LogicalNode"/> class.
            /// </summary>
            /// <param name="name">LN name</param>
            /// <param name="parent">Logical device containing this logical node.</param>
            public LogicalNode(string name, LogicalDevice parent)
            {
                this.parent = parent;

                base.self = LogicalNode_create(name, parent.self);
            }

            internal void AddRcb(ReportControlBlock rcb)
            {
                rcbs.Add(rcb.self, rcb);
            }
        }

    }
}