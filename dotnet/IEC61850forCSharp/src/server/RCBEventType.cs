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
        /// Report control block event types
        /// </summary>
        public enum RCBEventType
        {
            /// <summary>
            /// parameter read by client (not implemented).
            /// </summary>
            GET_PARAMETER = 0,
            /// <summary>
            /// parameter set by client.
            /// </summary>
            SET_PARAMETER = 1,
            /// <summary>
            /// reservation canceled.
            /// </summary>
            UNRESERVED = 2,
            /// <summary>
            /// reservation
            /// </summary>
            RESERVED = 3,
            /// <summary>
            /// RCB enabled
            /// </summary>
            ENABLED = 4,
            /// <summary>
            /// RCB disabled
            /// </summary>
            DISABLED = 5,
            /// <summary>
            /// GI report triggered
            /// </summary>
            GI = 6,
            /// <summary>
            /// Purge buffer procedure executed
            /// </summary>
            PURGEBUF = 7,
            /// <summary>
            /// Report buffer overflow
            /// </summary>
            OVERFLOW = 8,
            /// <summary>
            /// A new report was created and inserted into the buffer
            /// </summary>
            REPORT_CREATED = 9
        }

    }
}