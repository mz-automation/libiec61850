/*
 *  TLS.cs
 *
 *  Copyright 2017 Michael Zillgith
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
			static extern void TLSConfiguration_setChainValidation (IntPtr self, [MarshalAs(UnmanagedType.I1)] bool value);

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
			static extern bool TLSConfiguration_setOwnKeyFromFile (IntPtr self, string filename, string keyPassword);

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

			public TLSConfiguration() {
				self = TLSConfiguration_create ();
			}

			~TLSConfiguration() 
			{
				Dispose ();
			}

			internal IntPtr GetNativeInstance()
			{
				return self;
			}

			public bool AllowOnlyKnownCertificates 
			{
				set {
					TLSConfiguration_setAllowOnlyKnownCertificates (self, value);
					allowOnlyKnownCerts = value;
				}
				get {
					return allowOnlyKnownCerts;
				}
			}

			public bool ChainValidation
			{
				set {
					TLSConfiguration_setChainValidation (self, value);
					chainValidation = value;
				}
				get {
					return chainValidation;
				}
			}


			public void SetClientMode()
			{
				TLSConfiguration_setClientMode (self);
			}

			public void SetOwnCertificate(string filename)
			{
				if (TLSConfiguration_setOwnCertificateFromFile (self, filename) == false) {
					throw new CryptographicException ("Failed to read certificate from file");
				}
			}
				
			public void SetOwnCertificate(X509Certificate2 cert)
			{
				byte[] certBytes = cert.GetRawCertData ();

				if (TLSConfiguration_setOwnCertificate (self, certBytes, certBytes.Length) == false) {
					throw new CryptographicException ("Failed to set certificate");
				}
			}

			public void AddAllowedCertificate(string filename)
			{
				if (TLSConfiguration_addAllowedCertificateFromFile (self, filename) == false) {
					throw new CryptographicException ("Failed to read allowed certificate from file");
				}
			}

			public void AddAllowedCertificate(X509Certificate2 cert)
			{
				byte[] certBytes = cert.GetRawCertData ();

				if (TLSConfiguration_addAllowedCertificate (self, certBytes, certBytes.Length) == false) {
					throw new CryptographicException ("Failed to add allowed certificate");
				}
			}

			public void AddCACertificate(string filename)
			{
				if (TLSConfiguration_addCACertificateFromFile (self, filename) == false) {
					throw new CryptographicException ("Failed to read CA certificate from file");
				}
			}

			public void AddCACertificate(X509Certificate2 cert)
			{
				byte[] certBytes = cert.GetRawCertData ();

				if (TLSConfiguration_addCACertificate (self, certBytes, certBytes.Length) == false) {
					throw new CryptographicException ("Failed to add CA certificate");
				}
			}

			public void SetOwnKey (string filename, string password)
			{
				if (TLSConfiguration_setOwnKeyFromFile (self, filename, password) == false) {
					throw new CryptographicException ("Failed to read own key from file");
				}
			}

			public void SetOwnKey (X509Certificate2 key, string password)
			{
				byte[] certBytes = key.Export (X509ContentType.Pkcs12);

				if (TLSConfiguration_setOwnKey (self, certBytes, certBytes.Length, password) == false) {
					throw new CryptographicException ("Failed to set own key");
				}
			}

			public void Dispose()
			{
				lock (this) {
					if (self != IntPtr.Zero) {
						TLSConfiguration_destroy (self);
						self = IntPtr.Zero;
					}
				}
			}

		}
	}
}