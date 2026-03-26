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
    }
}