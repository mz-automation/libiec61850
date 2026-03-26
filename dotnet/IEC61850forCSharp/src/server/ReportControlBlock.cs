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
        /// Report control block (RCB) instance for server data model
        /// </summary>
        public class ReportControlBlock
        {
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr ReportControlBlock_create(string name, IntPtr parent, string rptId, [MarshalAs(UnmanagedType.I1)] bool isBuffered,
                string dataSetName, uint confRef, byte trgOps, byte options, uint bufTm, uint intgPd);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void ReportControlBlock_setPreconfiguredClient(IntPtr self, byte type, [Out] byte[] buf);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr ReportControlBlock_getName(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            [return: MarshalAs(UnmanagedType.I1)]
            static extern bool ReportControlBlock_getRptEna(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr ReportControlBlock_getRptID(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr ReportControlBlock_getDataSet(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern UInt32 ReportControlBlock_getConfRev(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern UInt32 ReportControlBlock_getOptFlds(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern UInt32 ReportControlBlock_getBufTm(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern UInt16 ReportControlBlock_getSqNum(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern UInt32 ReportControlBlock_getTrgOps(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern UInt32 ReportControlBlock_getIntgPd(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            [return: MarshalAs(UnmanagedType.I1)]
            static extern bool ReportControlBlock_getGI(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            [return: MarshalAs(UnmanagedType.I1)]
            static extern bool ReportControlBlock_getPurgeBuf(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr ReportControlBlock_getEntryId(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern UInt64 ReportControlBlock_getTimeofEntry(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern UInt16 ReportControlBlock_getResvTms(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            [return: MarshalAs(UnmanagedType.I1)]
            static extern bool ReportControlBlock_getResv(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr ReportControlBlock_getOwner(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void Memory_free(IntPtr self);

            public IntPtr self = IntPtr.Zero;

            private string name = null;
            private LogicalNode parent = null;

            public ReportControlBlock(string name, LogicalNode parent, string rptId, bool isBuffered,
                string dataSetName, uint confRev, byte trgOps, byte options, uint bufTm, uint intgPd)
            {
                self = ReportControlBlock_create(name, parent.self, rptId, isBuffered, dataSetName, confRev, trgOps, options, bufTm, intgPd);
                parent.AddRcb(this);
                this.parent = parent;
            }

            internal ReportControlBlock(IntPtr self, LogicalNode parent)
            {
                this.parent = parent;
                this.self = self;
                parent.AddRcb(this);
            }

            public void SetPreconfiguredClient(byte[] clientAddress)
            {
                if (clientAddress.Length == 4)
                    ReportControlBlock_setPreconfiguredClient(self, 4, clientAddress);
                else if (clientAddress.Length == 6)
                    ReportControlBlock_setPreconfiguredClient(self, 6, clientAddress);
            }

            public string Name
            {
                get
                {
                    if (name == null)
                    {
                        name = Marshal.PtrToStringAnsi(ReportControlBlock_getName(self));
                    }

                    return name;
                }
            }

            public LogicalNode Parent
            {
                get
                {
                    return parent;
                }
            }

            public bool RptEna
            {
                get
                {
                    return ReportControlBlock_getRptEna(self);
                }
            }

            public string RptID
            {
                get
                {
                    IntPtr rptIdPtr = ReportControlBlock_getRptID(self);

                    string rptId = Marshal.PtrToStringAnsi(rptIdPtr);

                    Memory_free(rptIdPtr);

                    return rptId;
                }
            }

            public string DataSet
            {
                get
                {
                    IntPtr dataSetPtr = ReportControlBlock_getDataSet(self);

                    string dataSet = Marshal.PtrToStringAnsi(dataSetPtr);

                    Memory_free(dataSetPtr);

                    return dataSet;
                }
            }

            public UInt32 ConfRev
            {
                get
                {
                    return ReportControlBlock_getConfRev(self);
                }
            }

            public ReportOptions OptFlds
            {
                get
                {
                    return (ReportOptions)ReportControlBlock_getOptFlds(self);
                }
            }

            public UInt32 BufTm
            {
                get
                {
                    return ReportControlBlock_getBufTm(self);
                }
            }

            public UInt16 SqNum
            {
                get
                {
                    return ReportControlBlock_getSqNum(self);
                }
            }

            public TriggerOptions TrgOps
            {
                get
                {
                    return (TriggerOptions)ReportControlBlock_getTrgOps(self);
                }
            }

            public UInt32 IntgPd
            {
                get
                {
                    return ReportControlBlock_getIntgPd(self);
                }
            }

            public bool GI
            {
                get
                {
                    return ReportControlBlock_getGI(self);
                }
            }

            public bool PurgeBuf
            {
                get
                {
                    return ReportControlBlock_getPurgeBuf(self);
                }
            }

            public byte[] EntryID
            {
                get
                {
                    IntPtr entryIdPtr = ReportControlBlock_getEntryId(self);

                    if (entryIdPtr != IntPtr.Zero)
                    {
                        byte[] entryId = null;

                        MmsValue octetStringVal = new MmsValue(entryIdPtr, true);

                        entryId = octetStringVal.getOctetString();

                        octetStringVal.Dispose();

                        return entryId;
                    }
                    else
                        return null;
                }
            }

            public UInt64 TimeofEntry
            {
                get
                {
                    return ReportControlBlock_getTimeofEntry(self);
                }
            }

            public UInt16 ResvTms
            {
                get
                {
                    return ReportControlBlock_getResvTms(self);
                }
            }

            public bool Resv
            {
                get
                {
                    return ReportControlBlock_getResv(self);
                }
            }

            public byte[] Owner
            {
                get
                {
                    IntPtr mmsValuePtr = ReportControlBlock_getOwner(self);

                    if (mmsValuePtr != IntPtr.Zero)
                    {
                        byte[] owner = null;

                        MmsValue octetStringVal = new MmsValue(mmsValuePtr, true);

                        owner = octetStringVal.getOctetString();

                        return owner;
                    }
                    else
                        return null;
                }

            }

        }

    }
}