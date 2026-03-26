using System;
using System.Runtime.InteropServices;

namespace IEC61850
{
    namespace Common
    {
        public enum Validity
        {
            GOOD = 0,
            RESERVED = 1,
            INVALID = 2,
            QUESTIONABLE = 3
        }
    }
}