using IEC61850.Common;
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace IEC61850
{
    namespace Client
    {
        [Flags]
        public enum ReasonForInclusion
        {
            /** the element is not included in the received report */
            REASON_NOT_INCLUDED = 0,

            /** the element is included due to a change of the data value */
            REASON_DATA_CHANGE = 1,

            /** the element is included due to a change in the quality of data */
            REASON_QUALITY_CHANGE = 2,

            /** the element is included due to an update of the data value */
            REASON_DATA_UPDATE = 4,

            /** the element is included due to a periodic integrity report task */
            REASON_INTEGRITY = 8,

            /** the element is included due to a general interrogation by the client */
            REASON_GI = 16,

            /** the reason for inclusion is unknown */
            REASON_UNKNOWN = 32
        }

    }
}