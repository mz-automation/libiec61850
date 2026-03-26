using System;
using System.Runtime.InteropServices;

namespace IEC61850
{
    namespace Common
    {
        [Flags]
        public enum ReportOptions
        {
            NONE = 0,
            SEQ_NUM = 1,
            TIME_STAMP = 2,
            REASON_FOR_INCLUSION = 4,
            DATA_SET = 8,
            DATA_REFERENCE = 16,
            BUFFER_OVERFLOW = 32,
            ENTRY_ID = 64,
            CONF_REV = 128,
            SEGMENTATION = 256,
            ALL = SEQ_NUM | TIME_STAMP | REASON_FOR_INCLUSION | DATA_SET | DATA_REFERENCE |
                BUFFER_OVERFLOW | ENTRY_ID | CONF_REV | SEGMENTATION
        }
    }
}