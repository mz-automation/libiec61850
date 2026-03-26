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
        public enum AccessPolicy
        {
            ACCESS_POLICY_ALLOW = 0,
            ACCESS_POLICY_DENY = 1
        }

    }
}