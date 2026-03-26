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
                    ln = lnModelNode as LogicalNode;
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

    }
}