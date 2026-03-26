using System;
using System.Runtime.InteropServices;

namespace IEC61850
{
    namespace Common
    {
        /// <summary>
        /// SmpMod values
        /// </summary>
        public enum SmpMod
        {
            SAMPLES_PER_PERIOD = 0,
            SAMPLES_PER_SECOND = 1,
            SECONDS_PER_SAMPLE = 2
        }
    }
}