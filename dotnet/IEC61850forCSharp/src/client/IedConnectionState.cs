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
        /// <summary>
        /// Connection state of an IedConnection instance
        /// </summary>
        public enum IedConnectionState
        {
            /// <summary>
            /// The connection is closed. Requests cannot be sent.
            /// </summary>
            IED_STATE_CLOSED = 0,

            /// <summary>
            /// The connection is connecting. Requests cannot be sent yet.
            /// </summary>
            IED_STATE_CONNECTING = 1,

            /// <summary>
            /// The connection is up. Requests can be sent.
            /// </summary>
            IED_STATE_CONNECTED = 2,

            /// <summary>
            /// The connection is closing. Requests connect be sent.
            /// </summary>
            IED_STATE_CLOSING = 3
        }
    }
}