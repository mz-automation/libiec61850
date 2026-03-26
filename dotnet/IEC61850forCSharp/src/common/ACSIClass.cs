using System;
using System.Runtime.InteropServices;

namespace IEC61850
{
    namespace Common
    {
        public enum ACSIClass
        {
            /** data objects */
            ACSI_CLASS_DATA_OBJECT,
            /** data sets (container for multiple data objects) */
            ACSI_CLASS_DATA_SET,
            /** buffered report control blocks */
            ACSI_CLASS_BRCB,
            /** unbuffered report control blocks */
            ACSI_CLASS_URCB,
            /** log control blocks */
            ACSI_CLASS_LCB,
            /** logs (journals) */
            ACSI_CLASS_LOG,
            /** setting group control blocks */
            ACSI_CLASS_SGCB,
            /** GOOSE control blocks */
            ACSI_CLASS_GoCB,
            /** GSE control blocks */
            ACSI_CLASS_GsCB,
            /** multicast sampled values control blocks */
            ACSI_CLASS_MSVCB,
            /** unicast sampled values control blocks */
            ACSI_CLASS_USVCB
        }
    }
}