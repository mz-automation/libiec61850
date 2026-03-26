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
            /// SV receiver.
            /// </summary>
            /// A receiver is responsible for processing all SV message for a single Ethernet interface.
            /// In order to process messages from multiple Ethernet interfaces you have to create multiple
            /// instances.
            public class SVReceiver : IDisposable
            {
                [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
                private static extern IntPtr SVReceiver_create();

                [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
                private static extern void SVReceiver_disableDestAddrCheck(IntPtr self);

                [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
                private static extern void SVReceiver_enableDestAddrCheck(IntPtr self);

                [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
                private static extern void SVReceiver_addSubscriber(IntPtr self, IntPtr subscriber);

                [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
                private static extern void SVReceiver_removeSubscriber(IntPtr self, IntPtr subscriber);

                [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
                private static extern void SVReceiver_start(IntPtr self);

                [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
                private static extern void SVReceiver_stop(IntPtr self);

                [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
                [return: MarshalAs(UnmanagedType.I1)]
                private static extern bool SVReceiver_isRunning(IntPtr self);

                [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
                private static extern void SVReceiver_destroy(IntPtr self);

                [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
                private static extern void SVReceiver_setInterfaceId(IntPtr self, string interfaceId);

                private IntPtr self;

                private bool isDisposed = false;

                /// <summary>
                /// Initializes a new instance of the <see cref="IEC61850.SV.Subscriber.SVReceiver"/> class.
                /// </summary>

                public SVReceiver()
                {
                    self = SVReceiver_create();
                }

                public void SetInterfaceId(string interfaceId)
                {
                    SVReceiver_setInterfaceId(self, interfaceId);
                }

                public void DisableDestAddrCheck()
                {
                    SVReceiver_disableDestAddrCheck(self);
                }

                public void EnableDestAddrCheck()
                {
                    SVReceiver_enableDestAddrCheck(self);
                }

                /// <summary>
                /// Add a subscriber to handle
                /// </summary>
                /// <param name="subscriber">Subscriber.</param>
                public void AddSubscriber(SVSubscriber subscriber)
                {
                    SVReceiver_addSubscriber(self, subscriber.self);
                }


                public void RemoveSubscriber(SVSubscriber subscriber)
                {
                    SVReceiver_removeSubscriber(self, subscriber.self);
                }

                /// <summary>
                /// Start handling SV messages
                /// </summary>
                public void Start()
                {
                    SVReceiver_start(self);
                }

                /// <summary>
                /// Stop handling SV messges
                /// </summary>
                public void Stop()
                {
                    SVReceiver_stop(self);
                }

                public bool IsRunning()
                {
                    return SVReceiver_isRunning(self);
                }

                /// <summary>
                /// Releases all resource used by the <see cref="IEC61850.SV.Subscriber.SVReceiver"/> object.
                /// </summary>
                /// <remarks>Call <see cref="Dispose"/> when you are finished using the <see cref="IEC61850.SV.Subscriber.SVReceiver"/>. The
                /// <see cref="Dispose"/> method leaves the <see cref="IEC61850.SV.Subscriber.SVReceiver"/> in an unusable state.
                /// After calling <see cref="Dispose"/>, you must release all references to the
                /// <see cref="IEC61850.SV.Subscriber.SVReceiver"/> so the garbage collector can reclaim the memory that the
                /// <see cref="IEC61850.SV.Subscriber.SVReceiver"/> was occupying.</remarks>
                public void Dispose()
                {
                    if (isDisposed == false)
                    {
                        isDisposed = true;
                        SVReceiver_destroy(self);
                        self = IntPtr.Zero;
                    }
                }

                ~SVReceiver()
                {
                    Dispose();
                }
            }
        }
    }
}