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
        /// Setting group control block for server data model
        /// </summary>
        public class SettingGroupControlBlock
        {
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr SettingGroupControlBlock_create(IntPtr parent, byte actSG, byte numOfSGs);

            internal IntPtr self = IntPtr.Zero;

            public SettingGroupControlBlock(LogicalNode parent, UInt32 actSG, UInt32 numOfSGs)
            {
                self = SettingGroupControlBlock_create(parent.self, (byte)actSG, (byte)numOfSGs);
            }

            public SettingGroupControlBlock(IntPtr self)
            {
                this.self = self;
            }
        }
    }
}