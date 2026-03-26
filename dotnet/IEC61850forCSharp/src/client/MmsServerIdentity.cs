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

        [StructLayout(LayoutKind.Sequential)]
        public class MmsServerIdentity
        {
            public string vendorName;
            public string modelName;
            public string revision;
        }
    }
}