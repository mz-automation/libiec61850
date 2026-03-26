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
        /// Generic asynchonous service handler - used by simple services that have only success or error result
        /// </summary>
        /// <param name="invokeId">The invoke ID of the request triggering this callback</param>
        /// <param name="parameter">user provided callback parameter</param>
        /// <param name="err">Error code of response or timeout error in case of a response timeout</param>
        public delegate void GenericServiceHandler(UInt32 invokeId, object parameter, IedClientError err);
    }
}