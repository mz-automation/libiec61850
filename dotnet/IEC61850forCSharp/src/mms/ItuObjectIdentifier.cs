using System;
using System.Runtime.InteropServices;
using System.Text;

// IEC 61850 API for the libiec61850 .NET wrapper library
namespace IEC61850
{
    public class ItuObjectIdentifier
    {
        private IntPtr self = IntPtr.Zero;

        [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
        static extern int ItuObjectIdentifier_getArcCount(IntPtr self);

        [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
        static extern IntPtr ItuObjectIdentifier_getArc(IntPtr self);

        public ItuObjectIdentifier(IntPtr self)
        {
            this.self = self;
        }

        public int GetArcCount()
        {
            return ItuObjectIdentifier_getArcCount(self);
        }

        public ushort[] GetArcs()
        {
            int count = ItuObjectIdentifier_getArcCount(self);
            if (count <= 0 || count > 10) return Array.Empty<ushort>();

            IntPtr arcPtr = ItuObjectIdentifier_getArc(self);

            ushort[] arcs = new ushort[count];

            short[] temp = new short[count];
            Marshal.Copy(arcPtr, temp, 0, count);

            for (int i = 0; i < count; i++)
                arcs[i] = (ushort)temp[i];

            return arcs;
        }
    }
}
