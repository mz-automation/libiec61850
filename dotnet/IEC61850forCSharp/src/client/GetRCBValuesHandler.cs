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
        /// Asynchonous service handler for the get RCB values service
        /// </summary>
        /// <param name="invokeId">The invoke ID of the request triggering this callback</param>
        /// <param name="parameter">user provided callback parameter</param>
        /// <param name="err">Error code of response or timeout error in case of a response timeout</param>
        /// <param name="rcb">the report control block instance</param>
        public delegate void GetRCBValuesHandler(UInt32 invokeId, object parameter, IedClientError err, ReportControlBlock rcb);
    }
}