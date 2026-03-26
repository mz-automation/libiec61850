using System;
using System.Collections.Generic;
using System.Text;


using IEC61850.Common;
using System;
using System.Runtime.InteropServices;

namespace IEC61850
{
    // IEC 61850 common API parts (used by client and server API)
    namespace Common
    {
        /// <summary>
        /// Originator category
        /// </summary>
        public enum OrCat
        {
            /** Not supported - should not be used */
            NOT_SUPPORTED = 0,
            /** Control operation issued from an operator using a client located at bay level */
            BAY_CONTROL = 1,
            /** Control operation issued from an operator using a client located at station level */
            STATION_CONTROL = 2,
            /** Control operation from a remote operator outside the substation (for example network control center) */
            REMOTE_CONTROL = 3,
            /** Control operation issued from an automatic function at bay level */
            AUTOMATIC_BAY = 4,
            /** Control operation issued from an automatic function at station level */
            AUTOMATIC_STATION = 5,
            /** Control operation issued from a automatic function outside of the substation */
            AUTOMATIC_REMOTE = 6,
            /** Control operation issued from a maintenance/service tool */
            MAINTENANCE = 7,
            /** Status change occurred without control action (for example external trip of a circuit breaker or failure inside the breaker) */
            PROCESS = 8
        }
    }
}