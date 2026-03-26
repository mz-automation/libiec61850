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
        public class DataSetEntry
        {
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr DataSetEntry_create(IntPtr dataSet, string variable, int index, string component);

            public IntPtr self = IntPtr.Zero;

            public DataSetEntry(DataSet dataSet, string variable, int index, string component)
            {
                self = DataSetEntry_create(dataSet.self, variable, index, component);
            }
        }
    }
}