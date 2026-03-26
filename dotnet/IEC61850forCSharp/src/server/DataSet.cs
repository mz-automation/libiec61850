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
        public class DataSet
        {
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr DataSet_create(string name, IntPtr parent);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr DataSet_getName(IntPtr self);

            public IntPtr self = IntPtr.Zero;

            internal DataSet(IntPtr dataSetPtr)
            {
                self = dataSetPtr;
            }

            public DataSet(string name, LogicalNode parent)
            {
                self = DataSet_create(name, parent.self);
            }

            public string Name
            {
                get
                {
                    return Marshal.PtrToStringAnsi(DataSet_getName(self));
                }
            }
        }

    }
}