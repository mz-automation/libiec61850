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
        /// Return type of ControlHandler and ControlWaitForExecutionHandler
        /// </summary>
        public enum ControlHandlerResult
        {
            /// <summary>
            /// check or operation failed
            /// </summary>
            FAILED = 0,
            /// <summary>
            /// check or operation was successful
            /// </summary>
            OK = 1,
            /// <summary>
            /// check or operation is in progress
            /// </summary>
            WAITING = 2
        }
    }
}