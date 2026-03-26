using System;
using System.Runtime.InteropServices;

namespace IEC61850
{
    namespace Common
    {
        /// <summary>
        /// MMS data access error for MmsValue type MMS_DATA_ACCESS_ERROR
        /// </summary>

        [Flags]
        public enum TriggerOptions
        {
            NONE = 0,
            /** send report when value of data changed */
            DATA_CHANGED = 1,
            /** send report when quality of data changed */
            QUALITY_CHANGED = 2,
            /** send report when data or quality is updated */
            DATA_UPDATE = 4,
            /** periodic transmission of all data set values */
            INTEGRITY = 8,
            /** general interrogation (on client request) */
            GI = 16,
            /** Report will be triggered only on rising edge (transient variable) */
            TRG_OPT_TRANSIENT = 128
        }
    }
}