/*
 *  SampledValuesControlBlock.cs
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
using System.Runtime.InteropServices;
using System.Diagnostics;

using IEC61850.Common;

namespace IEC61850
{
	namespace Client
	{
		/// <summary>
		/// Sampled values control bloc (SvCB) representation.
		/// </summary>
		/// <description>
		/// This class is used as a client side representation (copy) of a sampled values control block (SvCB).
		/// </description>
		public class SampledValuesControlBlock : IDisposable
		{
			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr ClientSVControlBlock_create (IntPtr iedConnection, string reference);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void ClientSVControlBlock_destroy(IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern int ClientSVControlBlock_getLastComError (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			[return: MarshalAs(UnmanagedType.I1)]
			static extern bool ClientSVControlBlock_isMulticast (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			[return: MarshalAs(UnmanagedType.I1)]
			static extern bool ClientSVControlBlock_setSvEna (IntPtr self, [MarshalAs(UnmanagedType.I1)] bool value);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			[return: MarshalAs(UnmanagedType.I1)]
			static extern bool ClientSVControlBlock_setResv (IntPtr self, [MarshalAs(UnmanagedType.I1)] bool value);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			[return: MarshalAs(UnmanagedType.I1)]
			static extern bool ClientSVControlBlock_getSvEna (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			[return: MarshalAs(UnmanagedType.I1)]
			static extern bool ClientSVControlBlock_getResv (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr ClientSVControlBlock_getMsvID (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr ClientSVControlBlock_getDatSet (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern UInt32 ClientSVControlBlock_getConfRev (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern UInt16 ClientSVControlBlock_getSmpRate (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern int ClientSVControlBlock_getOptFlds (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern byte ClientSVControlBlock_getSmpMod(IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern int ClientSVControlBlock_getNoASDU (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern PhyComAddress ClientSVControlBlock_getDstAddress (IntPtr self);

			private IntPtr self;
			private string objectReference;

			private bool isDisposed = false;


			internal SampledValuesControlBlock(IntPtr iedConnection, string objectReference)
			{
				self = ClientSVControlBlock_create (iedConnection, objectReference);
				this.objectReference = objectReference;
			}

			public string GetObjectReference ()
			{
				return this.objectReference;
			}

			public IedClientError GetLastComError()
			{
				return (IedClientError)ClientSVControlBlock_getLastComError (self);
			}

			public bool IsMulticast()
			{
				return ClientSVControlBlock_isMulticast (self);
			}

			public bool GetResv()
			{
				return ClientSVControlBlock_getResv (self);
			}

			public bool SetResv(bool value)
			{
				return ClientSVControlBlock_setResv (self, value);
			}

			public bool GetSvEna()
			{
				return ClientSVControlBlock_getSvEna (self);
			}

			public bool SetSvEna(bool value)
			{
				return ClientSVControlBlock_setSvEna (self, value);
			}

			public string GetMsvID ()
			{
				IntPtr msvIdPtr = ClientSVControlBlock_getMsvID (self);

				return Marshal.PtrToStringAnsi (msvIdPtr);
			}

			public string GetDatSet ()
			{
				IntPtr datSetPtr = ClientSVControlBlock_getDatSet (self);

				return Marshal.PtrToStringAnsi (datSetPtr);
			}

			public UInt32 GetConfRev ()
			{
				return ClientSVControlBlock_getConfRev (self);
			}

			public UInt16 GetSmpRate ()
			{
				return ClientSVControlBlock_getSmpRate (self);
			}

			public SVOptions GetOptFlds ()
			{
				return (SVOptions)ClientSVControlBlock_getOptFlds (self);
			}

			public SmpMod GetSmpMod ()
			{
				return (SmpMod)ClientSVControlBlock_getSmpMod (self);
			}

			public int GetNoASDU ()
			{
				return ClientSVControlBlock_getNoASDU (self);
			}

			public PhyComAddress GetDstAddress()
			{
				return ClientSVControlBlock_getDstAddress (self);
			}

			public void Dispose()
			{
				if (isDisposed == false) {
					isDisposed = true;
					ClientSVControlBlock_destroy (self);
					self = IntPtr.Zero;
				}
			}

			~SampledValuesControlBlock()
			{
				Dispose ();
			}

		}


	}
}
