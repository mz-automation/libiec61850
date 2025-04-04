/*
 *  TLS.cs
 *
 *  Copyright 2017-2024 Michael Zillgith
 *
 *  This file is part of libIEC61850.
 *
 *  libIEC61850 is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  libIEC61850 is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with libIEC61850.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  See COPYING file for the complete license text.
 */
using System;
using System.Text;
using System.Runtime.InteropServices;
using System.Collections.Generic;
using System.Collections;
using System.Security.Cryptography.X509Certificates;
using System.Security.Cryptography;

using IEC61850.Common;

/// <summary>
/// IEC 61850 API for the libiec61850 .NET wrapper library
/// </summary>
namespace IEC61850
{
    namespace TLS
    {
        public enum TLSConfigVersion
        {
            NOT_SELECTED = 0,
            SSL_3_0 = 3,
            TLS_1_0 = 4,
            TLS_1_1 = 5,
            TLS_1_2 = 6,
            TLS_1_3 = 7
        }

        public enum TLSEventLevel
        {
            INFO = 0,
            WARNING = 1,
            INCIDENT = 2
        }

        public enum TLSEventCode
        {
            ALM_ALGO_NOT_SUPPORTED = 1,
            ALM_UNSECURE_COMMUNICATION = 2,
            ALM_CERT_UNAVAILABLE = 3,
            ALM_BAD_CERT = 4,
            ALM_CERT_SIZE_EXCEEDED = 5,
            ALM_CERT_VALIDATION_FAILED = 6,
            ALM_CERT_REQUIRED = 7,
            ALM_HANDSHAKE_FAILED_UNKNOWN_REASON = 8,
            WRN_INSECURE_TLS_VERSION = 9,
            INF_SESSION_RENEGOTIATION = 10,
            ALM_CERT_EXPIRED = 11,
            ALM_CERT_REVOKED = 12,
            ALM_CERT_NOT_CONFIGURED = 13,
            ALM_CERT_NOT_TRUSTED = 14,
            ALM_NO_CIPHER = 15,
            INF_SESSION_ESTABLISHED = 16
        }

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
                            return (TLSConfigVersion)TLSConnection_getTLSVersion((IntPtr)self);
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
                            IntPtr peerAddrStr = TLSConnection_getPeerAddress(this.self, peerAddrBuf);

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

        /// <summary>
        /// TLS security event handler
        /// </summary>
        /// <param name="parameter">user provided context paramter to be passed to the handler</param>
        /// <param name="eventLevel">severity level of the event</param>
        /// <param name="eventCode">code to identify the event type</param>
        /// <param name="message">text message describing the event</param>
        /// <param name="connection">TLS connection that caused the event</param>
        public delegate void TLSEventHandler(object parameter, TLSEventLevel eventLevel, TLSEventCode eventCode, string message, TLSConnection connection);

        /// <summary>
        /// A container for TLS configuration and certificates.
        /// </summary>
        public class TLSConfiguration : IDisposable
        {
            private IntPtr self = IntPtr.Zero;

            private bool allowOnlyKnownCerts = false;
            private bool chainValidation = true;

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr TLSConfiguration_create();

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void TLSConfiguration_destroy(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void TLSConfiguration_setAllowOnlyKnownCertificates(IntPtr self, [MarshalAs(UnmanagedType.I1)] bool value);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void TLSConfiguration_setChainValidation(IntPtr self, [MarshalAs(UnmanagedType.I1)] bool value);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void TLSConfiguration_setClientMode(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            [return: MarshalAs(UnmanagedType.I1)]
            static extern bool TLSConfiguration_setOwnCertificate(IntPtr self, byte[] certificate, int certLen);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            [return: MarshalAs(UnmanagedType.I1)]
            static extern bool TLSConfiguration_setOwnCertificateFromFile(IntPtr self, string filename);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            [return: MarshalAs(UnmanagedType.I1)]
            static extern bool TLSConfiguration_setOwnKey(IntPtr self, byte[] key, int keyLen, string keyPassword);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            [return: MarshalAs(UnmanagedType.I1)]
            static extern bool TLSConfiguration_setOwnKeyFromFile(IntPtr self, string filename, string keyPassword);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            [return: MarshalAs(UnmanagedType.I1)]
            static extern bool TLSConfiguration_addAllowedCertificate(IntPtr self, byte[] certificate, int certLen);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            [return: MarshalAs(UnmanagedType.I1)]
            static extern bool TLSConfiguration_addAllowedCertificateFromFile(IntPtr self, string filename);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            [return: MarshalAs(UnmanagedType.I1)]
            static extern bool TLSConfiguration_addCACertificate(IntPtr self, byte[] certificate, int certLen);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            [return: MarshalAs(UnmanagedType.I1)]
            static extern bool TLSConfiguration_addCACertificateFromFile(IntPtr self, string filename);

            [DllImport("tase2", CallingConvention = CallingConvention.Cdecl)]
            static extern void TLSConfiguration_setMinTlsVersion(IntPtr self, int version);

            [DllImport("tase2", CallingConvention = CallingConvention.Cdecl)]
            static extern void TLSConfiguration_setMaxTlsVersion(IntPtr self, int version);

            [DllImport("tase2", CallingConvention = CallingConvention.Cdecl)]
            static extern void TLSConfiguration_addCipherSuite(IntPtr self, int ciphersuite);

            [DllImport("tase2", CallingConvention = CallingConvention.Cdecl)]
            static extern void TLSConfiguration_clearCipherSuiteList(IntPtr self);

            private TLSEventHandler eventHandler = null;
            private object eventHandlerParameter = null;

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            private delegate void InternalTLSEventHandler(IntPtr parameter, int eventLevel, int eventCode, IntPtr message, IntPtr tlsCon);

            private InternalTLSEventHandler internalTLSEventHandlerRef = null;

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void TLSConfiguration_setEventHandler(IntPtr self, InternalTLSEventHandler handler, IntPtr parameter);

            void InternalTLSEventHandlerImpl(IntPtr parameter, int eventLevel, int eventCode, IntPtr message, IntPtr tlsCon)
            {
                if (eventHandler != null)
                {
                    TLSConnection connection = new TLSConnection(tlsCon);

                    string msg = Marshal.PtrToStringAnsi(message);

                    eventHandler(eventHandlerParameter, (TLSEventLevel)eventLevel, (TLSEventCode)eventCode, msg, connection);

                    connection.InValidate();
                }
            }

            public void SetEventHandler(TLSEventHandler handler, object parameter)
            {
                this.eventHandler = handler;
                this.eventHandlerParameter = parameter;

                if (internalTLSEventHandlerRef == null)
                {
                    internalTLSEventHandlerRef = new InternalTLSEventHandler(InternalTLSEventHandlerImpl);

                    TLSConfiguration_setEventHandler(self, internalTLSEventHandlerRef, IntPtr.Zero);
                }
            }

            public TLSConfiguration()
            {
                self = TLSConfiguration_create();
            }

            ~TLSConfiguration()
            {
                Dispose();
            }

            internal IntPtr GetNativeInstance()
            {
                return self;
            }

            public bool AllowOnlyKnownCertificates
            {
                set
                {
                    TLSConfiguration_setAllowOnlyKnownCertificates(self, value);
                    allowOnlyKnownCerts = value;
                }
                get
                {
                    return allowOnlyKnownCerts;
                }
            }

            public bool ChainValidation
            {
                set
                {
                    TLSConfiguration_setChainValidation(self, value);
                    chainValidation = value;
                }
                get
                {
                    return chainValidation;
                }
            }

            public void SetClientMode()
            {
                TLSConfiguration_setClientMode(self);
            }

            public void SetOwnCertificate(string filename)
            {
                if (TLSConfiguration_setOwnCertificateFromFile(self, filename) == false)
                {
                    throw new CryptographicException("Failed to read certificate from file");
                }
            }

            public void SetOwnCertificate(X509Certificate2 cert)
            {
                byte[] certBytes = cert.GetRawCertData();

                if (TLSConfiguration_setOwnCertificate(self, certBytes, certBytes.Length) == false)
                {
                    throw new CryptographicException("Failed to set certificate");
                }
            }

            public void AddAllowedCertificate(string filename)
            {
                if (TLSConfiguration_addAllowedCertificateFromFile(self, filename) == false)
                {
                    throw new CryptographicException("Failed to read allowed certificate from file");
                }
            }

            public void AddAllowedCertificate(X509Certificate2 cert)
            {
                byte[] certBytes = cert.GetRawCertData();

                if (TLSConfiguration_addAllowedCertificate(self, certBytes, certBytes.Length) == false)
                {
                    throw new CryptographicException("Failed to add allowed certificate");
                }
            }

            public void AddCACertificate(string filename)
            {
                if (TLSConfiguration_addCACertificateFromFile(self, filename) == false)
                {
                    throw new CryptographicException("Failed to read CA certificate from file");
                }
            }

            public void AddCACertificate(X509Certificate2 cert)
            {
                byte[] certBytes = cert.GetRawCertData();

                if (TLSConfiguration_addCACertificate(self, certBytes, certBytes.Length) == false)
                {
                    throw new CryptographicException("Failed to add CA certificate");
                }
            }

            public void SetOwnKey(string filename, string password)
            {
                if (TLSConfiguration_setOwnKeyFromFile(self, filename, password) == false)
                {
                    throw new CryptographicException("Failed to read own key from file");
                }
            }

            public void SetOwnKey(X509Certificate2 key, string password)
            {
                byte[] certBytes = key.Export(X509ContentType.Pkcs12);

                if (TLSConfiguration_setOwnKey(self, certBytes, certBytes.Length, password) == false)
                {
                    throw new CryptographicException("Failed to set own key");
                }
            }

            /// <summary>
            /// Set minimal allowed TLS version to use
            /// </summary>
            /// <param name="version">lowest allowed TLS version</param>
            public void SetMinTlsVersion(TLSConfigVersion version)
            {
                TLSConfiguration_setMinTlsVersion(self, (int)version);
            }

            /// <summary>
            /// Set highest allowed TLS version to use
            /// </summary>
            /// <param name="version">highest allowed TLS version</param>
            public void SetMaxTlsVersion(TLSConfigVersion version)
            {
                TLSConfiguration_setMaxTlsVersion(self, (int)version);
            }

#if NET
            /// <summary>
            /// Add an allowed ciphersuite to the list of allowed ciphersuites
            /// </summary>
            /// <param name="ciphersuite"></param>
            public void addCipherSuite(TlsCipherSuite ciphersuite)
            {
                TLSConfiguration_addCipherSuite(self,(int) ciphersuite);
            }
#endif
            /// <summary>
            /// Add an allowed ciphersuite to the list of allowed ciphersuites
            /// </summary>
            /// <remarks>Version for .NET framework that does not support TlsCipherSuite enum</remarks>
            /// <param name="ciphersuite"></param>
            public void addCipherSuite(int ciphersuite)
            {
                TLSConfiguration_addCipherSuite(self, ciphersuite);
            }

            /// <summary>
            /// Clears list of allowed ciphersuites
            /// </summary>
            /// <returns></returns>
            public void clearCipherSuiteList()
            {
                TLSConfiguration_clearCipherSuiteList(self);
            }

            public void Dispose()
            {
                lock (this)
                {
                    if (self != IntPtr.Zero)
                    {
                        TLSConfiguration_destroy(self);
                        self = IntPtr.Zero;
                    }
                }
            }

        }
    }
}
