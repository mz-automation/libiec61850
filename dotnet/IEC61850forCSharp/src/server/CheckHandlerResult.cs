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
        public enum CheckHandlerResult
        {
            /// <summary>
            /// check passed
            /// </summary>
            ACCEPTED = -1,
            /// <summary>
            /// check failed due to hardware fault
            /// </summary>
            HARDWARE_FAULT = 1,
            /// <summary>
            /// control is already selected or operated
            /// </summary>
            TEMPORARILY_UNAVAILABLE = 2,
            /// <summary>
            /// check failed due to access control reason - access denied for this client or state
            /// </summary>
            OBJECT_ACCESS_DENIED = 3,
            /// <summary>
            /// object not visible in this security context ???
            /// </summary>
            OBJECT_UNDEFINED = 4
        }

    }
}