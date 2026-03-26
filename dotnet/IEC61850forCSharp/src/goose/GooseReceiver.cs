using IEC61850.Common;
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace IEC61850
{
    namespace GOOSE
    {
        namespace Subscriber
        {
            public class GooseReceiver : IDisposable
            {
                [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
                private static extern IntPtr GooseReceiver_create();

                [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
                private static extern void GooseReceiver_addSubscriber(IntPtr self, IntPtr subscriber);

                [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
                private static extern void GooseReceiver_removeSubscriber(IntPtr self, IntPtr subscriber);

                [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
                private static extern void GooseReceiver_start(IntPtr self);

                [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
                private static extern void GooseReceiver_stop(IntPtr self);

                [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
                [return: MarshalAs(UnmanagedType.I1)]
                private static extern bool GooseReceiver_isRunning(IntPtr self);

                [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
                private static extern void GooseReceiver_destroy(IntPtr self);

                [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
                private static extern void GooseReceiver_setInterfaceId(IntPtr self, string interfaceId);

                private IntPtr self;

                private bool isDisposed = false;

                private List<GooseSubscriber> subscribers = new List<GooseSubscriber>();

                public GooseReceiver()
                {
                    self = GooseReceiver_create();
                }

                public void SetInterfaceId(string interfaceId)
                {
                    GooseReceiver_setInterfaceId(self, interfaceId);
                }

                /// <summary>
                /// Add the subscriber to be handled by this receiver instance
                /// </summary>
                /// <remarks>A GooseSubscriber can only be added to one GooseReceiver!</remarks>
                /// <param name="subscriber"></param>
                public void AddSubscriber(GooseSubscriber subscriber)
                {
                    if (subscriber.attachedToReceiver == false)
                    {
                        subscriber.attachedToReceiver = true;
                        GooseReceiver_addSubscriber(self, subscriber.self);
                        subscribers.Add(subscriber);
                    }
                }

                public void RemoveSubscriber(GooseSubscriber subscriber)
                {
                    if (subscriber.attachedToReceiver)
                    {
                        GooseReceiver_removeSubscriber(self, subscriber.self);
                        subscribers.Remove(subscriber);
                        subscriber.attachedToReceiver = false;
                    }
                }

                public void Start()
                {
                    GooseReceiver_start(self);
                }

                public void Stop()
                {
                    GooseReceiver_stop(self);
                }

                public bool IsRunning()
                {
                    return GooseReceiver_isRunning(self);
                }

                public void Dispose()
                {
                    if (isDisposed == false)
                    {
                        isDisposed = true;
                        GooseReceiver_destroy(self);
                        self = IntPtr.Zero;
                    }
                }

                ~GooseReceiver()
                {
                    Dispose();
                }
            }
        }
    }
}