using IEC61850.Common;
using System;
using System.Runtime.InteropServices;

namespace IEC61850.Server
{

    /// <summary>
    /// RCB properties that are configurable to be writable or read-only
    /// </summary>
    [Flags]
    public enum ReportSettings
    {
        RPT_ID = 1,
        BUF_TIME = 2,
        DATSET = 4,
        TRG_OPS = 8,
        OPT_FIELDS = 16,
        INTG_PD = 32,
    }
}