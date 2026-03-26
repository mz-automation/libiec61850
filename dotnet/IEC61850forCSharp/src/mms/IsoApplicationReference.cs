using System;
using System.Collections.Generic;
using System.Text;

using System;
using System.Runtime.InteropServices;
using System.Text;

// IEC 61850 API for the libiec61850 .NET wrapper library
namespace IEC61850
{
    public class IsoApplicationReference
    {
        private IntPtr self = IntPtr.Zero;

        [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
        static extern int IsoApplicationReference_getAeQualifier(IntPtr self);

        [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
        static extern IntPtr IsoApplicationReference_getApTitle(IntPtr self);

        public IsoApplicationReference(IntPtr self)
        {
            this.self = self;
        }

        public int GetAeQualifier()
        {
            return IsoApplicationReference_getAeQualifier(self);
        }

        public ItuObjectIdentifier GetApTitle()
        {
            IntPtr identfier = IsoApplicationReference_getApTitle(self);

            if (identfier == IntPtr.Zero)
                return null;

            return new ItuObjectIdentifier(identfier);
        }

    }
}

