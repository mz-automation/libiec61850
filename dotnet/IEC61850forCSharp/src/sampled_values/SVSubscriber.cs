using IEC61850.Common;
using System;
using System.Runtime.InteropServices;

namespace IEC61850
{
    namespace SV
    {

        namespace Subscriber
        {
            /// <summary>
            /// Sampled Values (SV) Subscriber
            /// 
            /// A subscriber is an instance associated with a single stream of measurement data. It is identified
            /// by the Ethernet destination address, the appID value (both are on SV message level) and the svID value
            /// that is part of each ASDU.
            /// </summary>
            public class SVSubscriber : IDisposable
            {
                [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
                private delegate void InternalSVUpdateListener(IntPtr subscriber, IntPtr parameter, IntPtr asdu);

                [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
                private static extern IntPtr SVSubscriber_create([Out] byte[] ethAddr, UInt16 appID);

                [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
                private static extern IntPtr SVSubscriber_create(IntPtr ethAddr, UInt16 appID);

                [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
                private static extern void SVSubscriber_setListener(IntPtr self, InternalSVUpdateListener listener, IntPtr parameter);

                [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
                private static extern void SVSubscriber_destroy(IntPtr self);

                internal IntPtr self;

                private bool isDisposed = false;

                private SVUpdateListener listener;
                private object listenerParameter = null;

                private event InternalSVUpdateListener internalListener = null;

                private void internalSVUpdateListener(IntPtr subscriber, IntPtr parameter, IntPtr asdu)
                {
                    try
                    {

                        if (listener != null)
                        {
                            listener(this, listenerParameter, new SVSubscriberASDU(asdu));
                        }

                    }
                    catch (Exception e)
                    {
                        // older versions of mono 2.10 (for linux?) cause this exception
                        Console.WriteLine(e.Message);
                    }
                }

                public SVSubscriber(byte[] ethAddr, UInt16 appID)
                {
                    if (ethAddr == null)
                    {
                        self = SVSubscriber_create(IntPtr.Zero, appID);
                    }
                    else
                    {

                        if (ethAddr.Length != 6)
                            throw new ArgumentException("ethAddr argument has to be of 6 byte size");

                        self = SVSubscriber_create(ethAddr, appID);
                    }
                }

                public void SetListener(SVUpdateListener listener, object parameter)
                {
                    this.listener = listener;
                    listenerParameter = parameter;

                    if (internalListener == null)
                    {
                        internalListener = new InternalSVUpdateListener(internalSVUpdateListener);

                        SVSubscriber_setListener(self, internalListener, IntPtr.Zero);
                    }
                }

                public void Dispose()
                {
                    if (isDisposed == false)
                    {
                        isDisposed = true;
                        SVSubscriber_destroy(self);
                        self = IntPtr.Zero;
                    }
                }
            }


        }
    }
}