/*
 *  GooseControlBlock.cs
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

		public class GooseControlBlock : IDisposable {
		
			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr ClientGooseControlBlock_create (string dataAttributeReference);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void ClientGooseControlBlock_destroy(IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr IedConnection_getGoCBValues (IntPtr connection, out int error, string rcbReference, IntPtr updateRcb);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void IedConnection_setGoCBValues (IntPtr connection, out int error, IntPtr rcb, UInt32 parametersMask, [MarshalAs(UnmanagedType.I1)] bool singleRequest);
		
			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			[return: MarshalAs(UnmanagedType.I1)]
			static extern bool ClientGooseControlBlock_getGoEna (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void ClientGooseControlBlock_setGoEna(IntPtr self, [MarshalAs(UnmanagedType.I1)] bool rptEna);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr ClientGooseControlBlock_getGoID (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void ClientGooseControlBlock_setGoID (IntPtr self, string goId);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr ClientGooseControlBlock_getDatSet (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void ClientGooseControlBlock_setDatSet (IntPtr self, string datSet);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern UInt32 ClientGooseControlBlock_getConfRev (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			[return: MarshalAs(UnmanagedType.I1)]
			static extern bool ClientGooseControlBlock_getNdsComm (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern UInt32 ClientGooseControlBlock_getMinTime (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern UInt32 ClientGooseControlBlock_getMaxTime (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			[return: MarshalAs(UnmanagedType.I1)]
			static extern bool ClientGooseControlBlock_getFixedOffs (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern PhyComAddress ClientGooseControlBlock_getDstAddress (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr ClientGooseControlBlock_getDstAddress_addr(IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern byte ClientGooseControlBlock_getDstAddress_priority(IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern UInt16 ClientGooseControlBlock_getDstAddress_vid(IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern UInt16 ClientGooseControlBlock_getDstAddress_appid(IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void ClientGooseControlBlock_setDstAddress (IntPtr self, PhyComAddress value);
			

			private IntPtr self;
			private IntPtr connection;
			private string objectReference;

			private bool isDisposed = false;

			private bool flagGoEna = false;
			private bool flagGoID = false;
			private bool flagDatSet = false;
			private bool flagDstAddress = false;

			internal GooseControlBlock(string objectReference, IntPtr connection)
			{
				self = ClientGooseControlBlock_create (objectReference);
				this.connection = connection;
				this.objectReference = objectReference;
			}

			public string GetObjectReference ()
			{
				return this.objectReference;
			}

			/// <summary>
			/// Read all GoCB values from the server
			/// </summary>
			/// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
			public void GetCBValues ()
			{
				int error;

				IedConnection_getGoCBValues (connection, out error, objectReference, self);

				if (error != 0)
					throw new IedConnectionException ("getGoCBValues service failed", error);
			}

			private void
			resetSendFlags()
			{
				flagGoEna = false;
				flagGoID = false;
				flagDatSet = false;
				flagDstAddress = false;
			}

			public void SetCBValues (bool singleRequest)
			{
				UInt32 parametersMask = 0;

				if (flagGoEna)
					parametersMask += 1;

				if (flagGoID)
					parametersMask += 2;

				if (flagDatSet)
					parametersMask += 4;

				if (flagDstAddress)
					parametersMask += 32;

				int error;

				IedConnection_setGoCBValues (connection, out error, self, parametersMask, singleRequest);

				resetSendFlags ();

				if (error != 0)
					throw new IedConnectionException ("setGoCBValues service failed", error);
			}

			public void SetCBValues ()
			{
				SetCBValues (true);
			}

			public bool GetGoEna()
			{
				return ClientGooseControlBlock_getGoEna (self);
			}

			public void SetGoEna(bool value) 
			{
				ClientGooseControlBlock_setGoEna (self, value);

				flagGoEna = true;
			}

			public string GetGoID()
			{
				IntPtr goIdRef = ClientGooseControlBlock_getGoID (self);

				return Marshal.PtrToStringAnsi (goIdRef);
			}

			public void SetGoID (string goID)
			{
				ClientGooseControlBlock_setGoID (self, goID);

				flagGoID = true;
			}

			public string GetDatSet() 
			{
				IntPtr datSetRef = ClientGooseControlBlock_getDatSet (self);

				return Marshal.PtrToStringAnsi (datSetRef);
			}

			public void SetDataSet(string datSet)
			{
				ClientGooseControlBlock_setDatSet (self, datSet);

				flagDatSet = true;
			}

			public UInt32 GetConfRev()
			{
				return ClientGooseControlBlock_getConfRev (self);
			}

			public bool GetNdsComm()
			{
				return ClientGooseControlBlock_getNdsComm (self);
			}

			public UInt32 GetMinTime()
			{
				return ClientGooseControlBlock_getMinTime (self);
			}

			public UInt32 GetMaxTime()
			{
				return ClientGooseControlBlock_getMaxTime (self);
			}

			public bool GetFixedOffs()
			{
				return ClientGooseControlBlock_getFixedOffs (self);
			}

			public PhyComAddress GetDstAddress()
			{
				PhyComAddress addr = new PhyComAddress();

				IntPtr value = ClientGooseControlBlock_getDstAddress_addr(self);

				MmsValue mmsValue = new MmsValue(value);

				byte[] dstMacAddr = mmsValue.getOctetString();

				dstMacAddr.CopyTo(addr.dstAddress, 0);

				addr.dstAddress = dstMacAddr;

				addr.appId = ClientGooseControlBlock_getDstAddress_appid(self);
				addr.vlanId = ClientGooseControlBlock_getDstAddress_vid(self);
				addr.vlanPriority = ClientGooseControlBlock_getDstAddress_priority(self);

				return addr;
			}

			public void SetDstAddress(PhyComAddress value)
			{
				ClientGooseControlBlock_setDstAddress (self, value);

				flagDstAddress = true;
			}

			public void Dispose()
			{
				if (isDisposed == false) {
					isDisposed = true;
					ClientGooseControlBlock_destroy (self);
					self = IntPtr.Zero;
				}
			}

			~GooseControlBlock()
			{
				Dispose ();
			}

		}
	}
}