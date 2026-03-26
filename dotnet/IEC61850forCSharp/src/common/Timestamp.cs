using System;
using System.Runtime.InteropServices;

namespace IEC61850
{
    namespace Common
    {
        /// <summary>
        /// Timestamp (represents IEC 61850 timestamps e.g. "t" attribute)
        /// </summary>
        public class Timestamp
        {
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr Timestamp_create();

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr Timestamp_createFromByteArray(byte[] byteArry);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void Timestamp_destroy(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void Timestamp_clearFlags(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            [return: MarshalAs(UnmanagedType.I1)]
            static extern bool Timestamp_isLeapSecondKnown(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void Timestamp_setLeapSecondKnown(IntPtr self, [MarshalAs(UnmanagedType.I1)] bool value);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            [return: MarshalAs(UnmanagedType.I1)]
            static extern bool Timestamp_hasClockFailure(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void Timestamp_setClockFailure(IntPtr self, [MarshalAs(UnmanagedType.I1)] bool value);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            [return: MarshalAs(UnmanagedType.I1)]
            static extern bool Timestamp_isClockNotSynchronized(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void Timestamp_setClockNotSynchronized(IntPtr self, [MarshalAs(UnmanagedType.I1)] bool value);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern int Timestamp_getSubsecondPrecision(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void Timestamp_setSubsecondPrecision(IntPtr self, int subsecondPrecision);


            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void Timestamp_setTimeInSeconds(IntPtr self, UInt32 secondsSinceEpoch);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void Timestamp_setTimeInMilliseconds(IntPtr self, UInt64 millisSinceEpoch);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern UInt32 Timestamp_getTimeInSeconds(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern UInt64 Timestamp_getTimeInMs(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void Timestamp_setByMmsUtcTime(IntPtr self, IntPtr mmsValue);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern UInt32 Timestamp_getFractionOfSecondPart(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void Timestamp_setFractionOfSecondPart(IntPtr self, UInt32 fractionOfSecond);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern float Timestamp_getFractionOfSecond(IntPtr self);

            internal IntPtr self = IntPtr.Zero;
            private bool responsibleForDeletion;

            internal Timestamp(IntPtr timestampRef, bool selfAllocated)
            {
                self = timestampRef;
                responsibleForDeletion = selfAllocated;
            }

            public Timestamp(DateTime timestamp) : this()
            {
                SetDateTime(timestamp);
            }

            public Timestamp(MmsValue mmsUtcTime) : this()
            {
                SetByMmsUtcTime(mmsUtcTime);
            }

            /// <summary>
            /// Initializes a new instance of the <see cref="IEC61850CommonAPI.Timestamp"/> class.
            /// </summary>
            public Timestamp()
            {
                self = Timestamp_create();
                LeapSecondKnown = true;
                responsibleForDeletion = true;
            }

            /// <summary>
            /// Initializes a new instance of the <see cref="IEC61850CommonAPI.Timestamp"/> class.
            /// </summary>
            public Timestamp(Timestamp other) : this()
            {
                SetTimeInSeconds(other.GetTimeInSeconds());
                SetSubsecondPrecision(other.GetSubsecondPrecision());
                SetFractionOfSecondPart(other.GetFractionOfSecondPart());
                SetLeapSecondKnow(other.IsLeapSecondKnown());
                SetClockFailure(other.HasClockFailure());
                SetClockNotSynchronized(other.IsClockNotSynchronized());
            }

            public Timestamp(byte[] value)
            {
                self = Timestamp_createFromByteArray(value);
                responsibleForDeletion = true;
            }

            ~Timestamp()
            {
                if (responsibleForDeletion)
                    Timestamp_destroy(self);
            }

            public void ClearFlags()
            {
                Timestamp_clearFlags(self);
            }

            public void SetDateTime(DateTime timestamp)
            {
                DateTime epoch = new DateTime(1970, 1, 1, 0, 0, 0, DateTimeKind.Utc);
                DateTime timestampUTC = timestamp.ToUniversalTime();

                TimeSpan timeDiff = timestampUTC - epoch;
                ulong timeVal = Convert.ToUInt64(timeDiff.TotalMilliseconds);

                SetTimeInMilliseconds(timeVal);
            }

            public bool LeapSecondKnown
            {
                get { return IsLeapSecondKnown(); }
                set { SetLeapSecondKnow(value); }
            }

            public bool IsLeapSecondKnown()
            {
                return Timestamp_isLeapSecondKnown(self);
            }

            public void SetLeapSecondKnow(bool value)
            {
                Timestamp_setLeapSecondKnown(self, value);
            }

            public bool ClockFailure
            {
                get { return HasClockFailure(); }
                set { SetClockFailure(value); }
            }

            public bool HasClockFailure()
            {
                return Timestamp_hasClockFailure(self);
            }

            public void SetClockFailure(bool value)
            {
                Timestamp_setClockFailure(self, value);
            }

            public bool ClockNotSynchronized
            {
                get { return IsClockNotSynchronized(); }
                set { SetClockNotSynchronized(value); }
            }

            public bool IsClockNotSynchronized()
            {
                return Timestamp_isClockNotSynchronized(self);
            }

            public void SetClockNotSynchronized(bool value)
            {
                Timestamp_setClockNotSynchronized(self, value);
            }

            public int SubsecondPrecision
            {
                get { return GetSubsecondPrecision(); }
                set { SetSubsecondPrecision(value); }
            }

            public int GetSubsecondPrecision()
            {
                return Timestamp_getSubsecondPrecision(self);
            }

            public void SetSubsecondPrecision(int subsecondPrecision)
            {
                Timestamp_setSubsecondPrecision(self, subsecondPrecision);
            }

            public UInt32 TimeInSeconds
            {
                get { return GetTimeInSeconds(); }
                set { SetTimeInSeconds(value); }
            }

            public UInt32 GetTimeInSeconds()
            {
                return Timestamp_getTimeInSeconds(self);
            }

            public void SetTimeInSeconds(UInt32 secondsSinceEpoch)
            {
                Timestamp_setTimeInSeconds(self, secondsSinceEpoch);
            }

            public UInt64 TimeInMilliseconds
            {
                get { return GetTimeInMilliseconds(); }
                set { SetTimeInMilliseconds(value); }
            }

            public UInt64 GetTimeInMilliseconds()
            {
                return Timestamp_getTimeInMs(self);
            }

            public void SetTimeInMilliseconds(UInt64 millisSinceEpoch)
            {
                Timestamp_setTimeInMilliseconds(self, millisSinceEpoch);
            }

            public void SetByMmsUtcTime(MmsValue mmsValue)
            {
                Timestamp_setByMmsUtcTime(self, mmsValue.valueReference);
            }

            public DateTime AsDateTime()
            {
                DateTime epoch = new DateTime(1970, 1, 1, 0, 0, 0, DateTimeKind.Utc);

                DateTime retVal = epoch.AddMilliseconds(GetTimeInMilliseconds());

                return retVal;
            }

            public UInt32 GetFractionOfSecondPart()
            {
                return Timestamp_getFractionOfSecondPart(self);
            }

            public void SetFractionOfSecondPart(UInt32 fractionOfSecond)
            {
                Timestamp_setFractionOfSecondPart(self, fractionOfSecond);
            }

            public float GetFractionOfSecond()
            {
                return Timestamp_getFractionOfSecond(self);
            }

        }
    }
}