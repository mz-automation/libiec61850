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
        /// Reason for the select state change
        /// </summary>
        public enum SelectStateChangedReason
        {
            /// <summary>
            /// Control has been selected
            /// </summary>
            SELECT_STATE_REASON_SELECTED = 0,
            /// <summary>
            /// Cancel received for the control
            /// </summary>
            SELECT_STATE_REASON_CANCELED = 1,
            /// <summary>
            /// Unselected due to timeout (sboTimeout)
            /// </summary>
            SELECT_STATE_REASON_TIMEOUT = 2,
            /// <summary>
            /// Unselected due to successful operate
            /// </summary>
            SELECT_STATE_REASON_OPERATED = 3,
            /// <summary>
            /// Unselected due to failed operate
            /// </summary>
            SELECT_STATE_REASON_OPERATE_FAILED = 4,
            /// <summary>
            /// Unselected due to disconnection of selecting client
            /// </summary>
            SELECT_STATE_REASON_DISCONNECTED = 5
        }
    }
}