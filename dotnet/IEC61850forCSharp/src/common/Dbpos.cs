using System;
using System.Runtime.InteropServices;

namespace IEC61850
{
    namespace Common
    {
        public enum Dbpos
        {
            DBPOS_INTERMEDIATE_STATE = 0,
            DBPOS_OFF = 1,
            DBPOS_ON = 2,
            DBPOS_BAD_STATE = 3
        }
    }
}