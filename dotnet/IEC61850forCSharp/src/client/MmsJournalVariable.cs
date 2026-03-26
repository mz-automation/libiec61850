using IEC61850.Common;
using IEC61850.TLS;
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

// IEC 61850 API for the libiec61850 .NET wrapper library
namespace IEC61850
{
    // IEC 61850 client API.
    namespace Client
    {
        /// <summary>
        /// Represents a variable in a log entry
        /// </summary>
        public class MmsJournalVariable
        {
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr MmsJournalVariable_getTag(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr MmsJournalVariable_getValue(IntPtr self);

            private IntPtr self;

            internal MmsJournalVariable(IntPtr self)
            {
                this.self = self;
            }

            public string GetTag()
            {
                return Marshal.PtrToStringAnsi(MmsJournalVariable_getTag(self));
            }

            public MmsValue GetValue()
            {
                MmsValue mmsValue = new MmsValue(MmsJournalVariable_getValue(self));

                return mmsValue;
            }

        }
    }
}