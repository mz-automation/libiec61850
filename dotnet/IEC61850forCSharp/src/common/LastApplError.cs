using IEC61850.Common;
using System;
using System.Runtime.InteropServices;

namespace IEC61850
{
    // IEC 61850 common API parts (used by client and server API)

    namespace Client
    {
        [StructLayout(LayoutKind.Sequential)]
        internal struct LastApplErrorInternal
        {
            public int ctlNum;
            public int error;
            public int addCause;
        }

        public class LastApplError
        {
            public int ctlNum;
            public int error;
            public ControlAddCause addCause;


            internal LastApplError(LastApplErrorInternal lastApplError)
            {
                addCause = (ControlAddCause)lastApplError.addCause;
                error = lastApplError.error;
                ctlNum = lastApplError.ctlNum;
            }
        }
    }
}