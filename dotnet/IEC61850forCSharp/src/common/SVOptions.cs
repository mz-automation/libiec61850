
using System;
using System.Runtime.InteropServices;

namespace IEC61850
{
    namespace Common
    {
        /// <summary>
        /// Values for Sampled Values (SV) OptFlds
        /// </summary>
        [Flags]
        public enum SVOptions
        {
            NONE = 0,
            REFRESH_TIME = 1,
            SAMPLE_SYNC = 2,
            SAMPLE_RATE = 4,
            DATA_SET = 8,
            SECURITY = 16,
            ALL = 31
        }
    }
}