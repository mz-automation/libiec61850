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
        /// Control model
        /// </summary>
        public enum ControlModel
        {
            /** status only */
            STATUS_ONLY = 0,
            /** direct with normal security */
            DIRECT_NORMAL = 1,
            /** select before operate (SBO) with normal security */
            SBO_NORMAL = 2,
            /** direct with enhanced security */
            DIRECT_ENHANCED = 3,
            /** select before operate (SBO) with enhanced security */
            SBO_ENHANCED = 4
        }
    }
}