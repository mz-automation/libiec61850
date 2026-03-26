using System;
using System.Runtime.InteropServices;

namespace IEC61850
{
    namespace Common
    {

        [StructLayout(LayoutKind.Sequential)]
        public class PhyComAddress
        {
            public byte vlanPriority;
            public UInt16 vlanId;
            public UInt16 appId;

            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 6)]
            public byte[] dstAddress = new byte[6];
        }
    }
}