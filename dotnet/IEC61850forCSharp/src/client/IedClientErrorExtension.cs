using IEC61850.Common;
using IEC61850.TLS;
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

// IEC 61850 API for the libiec61850 .NET wrapper library
namespace IEC61850
{
    // IEC 61850 client API.
    namespace Client
    {
        public static class IedClientErrorExtension
        {
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr IedClientError_toString(int err);

            public static string ToString(this IedClientError err)
            {
                string stringVal = Marshal.PtrToStringAnsi(IedClientError_toString((int)err));

                return stringVal;
            }
        }
    }
}