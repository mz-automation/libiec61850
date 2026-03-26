using IEC61850.Common;
using System;
using System.Runtime.InteropServices;

namespace IEC61850
{
    namespace SV
    {

        namespace Subscriber
        {
            public class SVSubscriberASDU
            {
                [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
                private static extern UInt16 SVSubscriber_ASDU_getSmpCnt(IntPtr self);

                [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
                private static extern IntPtr SVSubscriber_ASDU_getSvId(IntPtr self);

                [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
                private static extern IntPtr SVSubscriber_ASDU_getDatSet(IntPtr self);

                [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
                private static extern UInt32 SVSubscriber_ASDU_getConfRev(IntPtr self);

                [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
                private static extern byte SVSubscriber_ASDU_getSmpMod(IntPtr self);

                [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
                private static extern UInt16 SVSubscriber_ASDU_getSmpRate(IntPtr self);

                [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
                [return: MarshalAs(UnmanagedType.I1)]
                private static extern bool SVSubscriber_ASDU_hasDatSet(IntPtr self);

                [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
                [return: MarshalAs(UnmanagedType.I1)]
                private static extern bool SVSubscriber_ASDU_hasRefrTm(IntPtr self);

                [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
                [return: MarshalAs(UnmanagedType.I1)]
                private static extern bool SVSubscriber_ASDU_hasSmpMod(IntPtr self);

                [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
                [return: MarshalAs(UnmanagedType.I1)]
                private static extern bool SVSubscriber_ASDU_hasSmpRate(IntPtr self);

                [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
                private static extern UInt64 SVSubscriber_ASDU_getRefrTmAsMs(IntPtr self);

                [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
                private static extern sbyte SVSubscriber_ASDU_getINT8(IntPtr self, int index);

                [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
                private static extern Int16 SVSubscriber_ASDU_getINT16(IntPtr self, int index);

                [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
                private static extern Int32 SVSubscriber_ASDU_getINT32(IntPtr self, int index);

                [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
                private static extern Int64 SVSubscriber_ASDU_getINT64(IntPtr self, int index);

                [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
                private static extern byte SVSubscriber_ASDU_getINT8U(IntPtr self, int index);

                [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
                private static extern UInt16 SVSubscriber_ASDU_getINT16U(IntPtr self, int index);

                [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
                private static extern UInt32 SVSubscriber_ASDU_getINT32U(IntPtr self, int index);

                [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
                private static extern UInt64 SVSubscriber_ASDU_getINT64U(IntPtr self, int index);

                [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
                private static extern float SVSubscriber_ASDU_getFLOAT32(IntPtr self, int index);

                [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
                private static extern double SVSubscriber_ASDU_getFLOAT64(IntPtr self, int index);

                [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
                private static extern UInt16 SVSubscriber_ASDU_getQuality(IntPtr self, int index);

                [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
                private static extern int SVSubscriber_ASDU_getDataSize(IntPtr self);

                private IntPtr self;

                internal SVSubscriberASDU(IntPtr self)
                {
                    this.self = self;
                }

                public UInt16 GetSmpCnt()
                {
                    return SVSubscriber_ASDU_getSmpCnt(self);
                }

                public string GetSvId()
                {
                    return Marshal.PtrToStringAnsi(SVSubscriber_ASDU_getSvId(self));
                }

                public string GetDatSet()
                {
                    return Marshal.PtrToStringAnsi(SVSubscriber_ASDU_getDatSet(self));
                }

                public UInt32 GetConfRev()
                {
                    return SVSubscriber_ASDU_getConfRev(self);
                }

                public SmpMod GetSmpMod()
                {
                    return (SmpMod)SVSubscriber_ASDU_getSmpMod(self);
                }

                public UInt16 GetSmpRate()
                {
                    return SVSubscriber_ASDU_getSmpRate(self);
                }

                public bool HasDatSet()
                {
                    return SVSubscriber_ASDU_hasDatSet(self);
                }

                public bool HasRefrRm()
                {
                    return SVSubscriber_ASDU_hasRefrTm(self);
                }

                public bool HasSmpMod()
                {
                    return SVSubscriber_ASDU_hasSmpMod(self);
                }

                public bool HasSmpRate()
                {
                    return SVSubscriber_ASDU_hasSmpRate(self);
                }

                public UInt64 GetRefrTmAsMs()
                {
                    return SVSubscriber_ASDU_getRefrTmAsMs(self);
                }

                public sbyte GetINT8(int index)
                {
                    return SVSubscriber_ASDU_getINT8(self, index);
                }

                public Int16 GetINT16(int index)
                {
                    return SVSubscriber_ASDU_getINT16(self, index);
                }

                public Int32 GetINT32(int index)
                {
                    return SVSubscriber_ASDU_getINT32(self, index);
                }

                public Int64 GetINT64(int index)
                {
                    return SVSubscriber_ASDU_getINT64(self, index);
                }

                public byte GetINT8U(int index)
                {
                    return SVSubscriber_ASDU_getINT8U(self, index);
                }

                public UInt16 GetINT16U(int index)
                {
                    return SVSubscriber_ASDU_getINT16U(self, index);
                }

                public UInt32 GetINT32U(int index)
                {
                    return SVSubscriber_ASDU_getINT32U(self, index);
                }

                public UInt64 GetINT64U(int index)
                {
                    return SVSubscriber_ASDU_getINT64U(self, index);
                }

                public float GetFLOAT32(int index)
                {
                    return SVSubscriber_ASDU_getFLOAT32(self, index);
                }

                public double GetFLOAT64(int index)
                {
                    return SVSubscriber_ASDU_getFLOAT64(self, index);
                }

                private struct PTimestamp
                {
                    [MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.I1, SizeConst = 8)]
                    public byte[] val;
                }

                [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
                private static extern PTimestamp SVSubscriber_ASDU_getTimestamp(IntPtr self, int index);

                public Timestamp GetTimestamp(int index)
                {
                    PTimestamp retVal = SVSubscriber_ASDU_getTimestamp(self, index);

                    return new Timestamp(retVal.val);
                }

                public Quality GetQuality(int index)
                {
                    UInt16 qValue = SVSubscriber_ASDU_getQuality(self, index);

                    return new Quality(qValue);
                }

                /// <summary>
                /// Gets the size of the payload data in bytes. The payload comprises the data set data.
                /// </summary>
                /// <returns>The payload data size in byte</returns>
                public int GetDataSize()
                {
                    return SVSubscriber_ASDU_getDataSize(self);
                }
            }
        }

    }
}
