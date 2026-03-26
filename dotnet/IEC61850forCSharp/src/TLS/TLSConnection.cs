using System;
using System.Runtime.InteropServices;
using System.Security.Cryptography;
using System.Security.Cryptography.X509Certificates;

/// <summary>
/// IEC 61850 API for the libiec61850 .NET wrapper library
/// </summary>
namespace IEC61850
{
    namespace TLS
    {
        public class TLSConnection
        {
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern int TLSConnection_getTLSVersion(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr TLSConnection_getPeerAddress(IntPtr self, IntPtr peerAddrBuf);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr TLSConnection_getPeerCertificate(IntPtr self, out int certSize);

            private IntPtr self;
            private bool isValid;

            internal TLSConnection(IntPtr self)
            {
                this.self = self;
                isValid = true;
            }

            // To be called by event callback caller after callback execution
            internal void InValidate()
            {
                lock (this)
                {
                    isValid = false;
                }
            }

            /// <summary>
            /// TLS version used by the connection
            /// </summary>
            public TLSConfigVersion TLSVersion
            {
                get
                {
                    lock (this)
                    {
                        if (isValid)
                        {
                            return (TLSConfigVersion)TLSConnection_getTLSVersion(self);
                        }
                        else
                        {
                            throw new InvalidOperationException("Object cannot be used outside of TLS event callback");
                        }
                    }
                }
            }

            /// <summary>
            /// Peer IP address and TCP port of the TLS connection
            /// </summary>
            public string PeerAddress
            {
                get
                {
                    lock (this)
                    {
                        if (isValid)
                        {
                            IntPtr peerAddrBuf = Marshal.AllocHGlobal(130);
                            IntPtr peerAddrStr = TLSConnection_getPeerAddress(self, peerAddrBuf);

                            string peerAddr = null;

                            if (peerAddrStr != IntPtr.Zero)
                            {
                                peerAddr = Marshal.PtrToStringAnsi(peerAddrStr);
                            }

                            Marshal.FreeHGlobal(peerAddrBuf);

                            return peerAddr;
                        }
                        else
                        {
                            throw new InvalidOperationException("Object cannot be used outside of TLS event callback");
                        }
                    }
                }
            }

            /// <summary>
            /// TLS certificate used by the peer
            /// </summary>
            public byte[] PeerCertificate
            {
                get
                {
                    lock (this)
                    {
                        if (isValid)
                        {
                            int certSize;

                            IntPtr certBuffer = TLSConnection_getPeerCertificate(self, out certSize);

                            if (certBuffer != IntPtr.Zero)
                            {
                                if (certSize > 0)
                                {
                                    byte[] cert = new byte[certSize];

                                    Marshal.Copy(certBuffer, cert, 0, certSize);

                                    return cert;
                                }
                            }

                            return null;
                        }
                        else
                        {
                            throw new InvalidOperationException("Object cannot be used outside of TLS event callback");
                        }
                    }
                }
            }

        }
    }
}