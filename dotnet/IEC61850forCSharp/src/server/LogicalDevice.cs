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
        /// Logical device. Representation of a logical device (LD) in a data model.
        /// </summary>
        public class LogicalDevice : ModelNode
        {
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr LogicalDevice_create(string name, IntPtr parent);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr LogicalDevice_createEx(string name, IntPtr parent, string ldName);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr LogicalDevice_getSettingGroupControlBlock(IntPtr self);

            public IedModel IedModel { get; }

            public LogicalDevice(IntPtr self, IedModel iedModel) : base(self)
            {
                IedModel = iedModel;
            }

            /// <summary>
            /// Create a new logical device in a data model
            /// </summary>
            /// <param name="inst">LD instance</param>
            /// <param name="parent">Model containing this logical device</param>
            public LogicalDevice(string inst, IedModel parent)
            {
                IedModel = parent;

                self = LogicalDevice_create(inst, parent.self);
            }

            /// <summary>
            /// Create a new logical device in a data model (support for functional naming)
            /// </summary>
            /// <param name="inst">LD instance</param>
            /// <param name="parent">Model containing this logical device</param>
            /// <param name="ldName">LD name (for functional naming). When set, the exernally visible domain name for this LD</param>
            public LogicalDevice(string inst, IedModel parent, string ldName)
            {
                IedModel = parent;

                self = LogicalDevice_createEx(inst, parent.self, ldName);
            }

            /// <summary>
            /// Get the setting group control block (SGCB) of the logical device
            /// </summary>
            /// <returns>the SGCB instance or NULL if no SGCB is available</returns>
            public SettingGroupControlBlock GetSettingGroupControlBlock()
            {
                IntPtr sgcb = LogicalDevice_getSettingGroupControlBlock(self);

                if (sgcb == IntPtr.Zero)
                    return null;

                return new SettingGroupControlBlock(sgcb);
            }
        }
    }
}