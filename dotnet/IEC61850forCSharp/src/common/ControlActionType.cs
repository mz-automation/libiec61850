using IEC61850.Common;
using System;
using System.Runtime.InteropServices;

namespace IEC61850
{
    // IEC 61850 common API parts (used by client and server API)

    namespace Client
    {

        public enum ControlActionType
        {
            SELECT = 0,
            OPERATE = 1,
            CANCEL = 2
        }
    }
}