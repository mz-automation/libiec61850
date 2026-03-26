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
    }
}