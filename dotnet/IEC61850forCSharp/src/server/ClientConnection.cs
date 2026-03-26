using IEC61850.Common;
using IEC61850.Model;
using IEC61850.TLS;
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

// IEC 61850 API for the libiec61850 .NET wrapper library
namespace IEC61850
{
    // IEC 61850 server API.
    namespace Server
    {
        public class ClientConnection : IDisposable
        {
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr ClientConnection_getSecurityToken(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr ClientConnection_getPeerAddress(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr ClientConnection_getLocalAddress(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            [return: MarshalAs(UnmanagedType.I1)]
            static extern bool ClientConnection_abort(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr ClientConnection_claimOwnership(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void ClientConnection_release(IntPtr self);

            internal IntPtr self;

            internal ClientConnection(IntPtr self)
            {
                this.self = ClientConnection_claimOwnership(self);
            }

            /// <summary>
            /// Get the security token associated with this connection
            /// The security token is an opaque handle that is associated with the connection.It is provided by the
            /// authenticator (if one is present). If no security token is used then this function returns NULL
            /// </summary>
            /// <returns>the security token or NULL</returns>
            public object GetSecurityToken()
            {
                lock (this)
                {
                    if (self != IntPtr.Zero)
                    {
                        IntPtr token = ClientConnection_getSecurityToken(self);

                        if (token != IntPtr.Zero)
                        {
                            GCHandle handle = GCHandle.FromIntPtr(token);
                            return handle;
                        }
                    }
                }

                return null;
            }

            public string GetPeerAddress()
            {
                lock (this)
                {
                    if (self != IntPtr.Zero)
                    {
                        IntPtr peerAddrPtr = ClientConnection_getPeerAddress(self);

                        if (peerAddrPtr != IntPtr.Zero)
                            return Marshal.PtrToStringAnsi(peerAddrPtr);
                    }
                }

                return null;
            }

            public string GetLocalAddress()
            {
                lock (this)
                {
                    if (self != IntPtr.Zero)
                    {
                        IntPtr localAddrPtr = ClientConnection_getLocalAddress(self);

                        if (localAddrPtr != IntPtr.Zero)
                            return Marshal.PtrToStringAnsi(localAddrPtr);
                    }
                }

                return null;
            }

            /// <summary>
            /// Abort/Close the client connection
            /// </summary>
            /// <returns>true, when connection has been closed, false when connection was already close</returns>
            public bool Abort()
            {
                lock (this)
                {
                    if (self != IntPtr.Zero)
                    {
                        return ClientConnection_abort(self);
                    }
                }

                return false;
            }

            public void Dispose()
            {
                lock (this)
                {
                    if (self != IntPtr.Zero)
                    {
                        ClientConnection_release(self);

                        self = IntPtr.Zero;
                    }
                }
            }

            ~ClientConnection()
            {
                Dispose();
            }

        }

    }
}