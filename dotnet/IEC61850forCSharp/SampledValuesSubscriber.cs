/*
 *  SampledValuedSubscriber.cs
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
using IEC61850.Common;

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
				private static extern IntPtr SVReceiver_create ();

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
				private static extern bool SVReceiver_isRunning (IntPtr self);

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
					self = SVReceiver_create ();
				}

				public void SetInterfaceId(string interfaceId)
				{
					SVReceiver_setInterfaceId (self, interfaceId);
				}

				public void DisableDestAddrCheck()
				{
					SVReceiver_disableDestAddrCheck (self);
				}

				public void EnableDestAddrCheck()
				{
					SVReceiver_enableDestAddrCheck (self);
				}
					
				/// <summary>
				/// Add a subscriber to handle
				/// </summary>
				/// <param name="subscriber">Subscriber.</param>
				public void AddSubscriber(SVSubscriber subscriber)
				{
					SVReceiver_addSubscriber (self, subscriber.self);
				}


				public void RemoveSubscriber(SVSubscriber subscriber)
				{
					SVReceiver_removeSubscriber (self, subscriber.self);
				}

				/// <summary>
				/// Start handling SV messages
				/// </summary>
				public void Start()
				{
					SVReceiver_start (self);
				}

				/// <summary>
				/// Stop handling SV messges
				/// </summary>
				public void Stop()
				{
					SVReceiver_stop (self);
				}

				public bool IsRunning()
				{
					return SVReceiver_isRunning (self);
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
					if (isDisposed == false) {
						isDisposed = true;
						SVReceiver_destroy (self);
						self = IntPtr.Zero;
					}
				}

				~SVReceiver()
				{
					Dispose ();
				}
			}


			/// <summary>
			/// SV listener.
			/// </summary>
			public delegate void SVUpdateListener (SVSubscriber report, object parameter, SVSubscriberASDU asdu);

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
				private delegate void InternalSVUpdateListener (IntPtr subscriber, IntPtr parameter, IntPtr asdu);

				[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
				private static extern IntPtr SVSubscriber_create([Out] byte[] ethAddr, UInt16 appID);

				[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
				private static extern IntPtr SVSubscriber_create(IntPtr ethAddr, UInt16 appID);

				[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
				private static extern void SVSubscriber_setListener(IntPtr self,  InternalSVUpdateListener listener, IntPtr parameter);

				[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
				private static extern void SVSubscriber_destroy(IntPtr self);

				internal IntPtr self;

				private bool isDisposed = false;

				private SVUpdateListener listener;
				private object listenerParameter = null;

				private event InternalSVUpdateListener internalListener = null;

				private void internalSVUpdateListener (IntPtr subscriber, IntPtr parameter, IntPtr asdu)
				{
					try {
					
						if (listener != null) {
							listener(this, listenerParameter, new SVSubscriberASDU(asdu));
						}
					
					}
					catch (Exception e) {
						// older versions of mono 2.10 (for linux?) cause this exception
						Console.WriteLine(e.Message);
					}
				}

				public SVSubscriber(byte[] ethAddr, UInt16 appID)
				{
					if (ethAddr == null) {
						self = SVSubscriber_create (IntPtr.Zero, appID);
					} else {

						if (ethAddr.Length != 6)
							throw new ArgumentException ("ethAddr argument has to be of 6 byte size");

						self = SVSubscriber_create (ethAddr, appID);
					}
				}

				public void SetListener(SVUpdateListener listener, object parameter)
				{
					this.listener = listener;
					this.listenerParameter = parameter;

					if (internalListener == null) {
						internalListener = new InternalSVUpdateListener (internalSVUpdateListener);

						SVSubscriber_setListener (self, internalListener, IntPtr.Zero);
					}
				}

				public void Dispose()
				{
					if (isDisposed == false) {
						isDisposed = true;
						SVSubscriber_destroy (self);
						self = IntPtr.Zero;
					}
				}
			}
		

			public class SVSubscriberASDU
			{
				[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
				private static extern UInt16 SVSubscriber_ASDU_getSmpCnt(IntPtr self);

				[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
				private static extern IntPtr SVSubscriber_ASDU_getSvId(IntPtr self);

				[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
				private static extern IntPtr SVSubscriber_ASDU_getDatSet(IntPtr self);

				[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
				private static extern UInt32 SVSubscriber_ASDU_getConfRev(IntPtr self);

				[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
				private static extern byte SVSubscriber_ASDU_getSmpMod(IntPtr self);

				[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
				private static extern UInt16 SVSubscriber_ASDU_getSmpRate(IntPtr self);

				[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
				[return: MarshalAs(UnmanagedType.I1)]
				private static extern bool SVSubscriber_ASDU_hasDatSet(IntPtr self);

				[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
				[return: MarshalAs(UnmanagedType.I1)]
				private static extern bool SVSubscriber_ASDU_hasRefrTm(IntPtr self);

				[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
				[return: MarshalAs(UnmanagedType.I1)]
				private static extern bool SVSubscriber_ASDU_hasSmpMod(IntPtr self);

				[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
				[return: MarshalAs(UnmanagedType.I1)]
				private static extern bool SVSubscriber_ASDU_hasSmpRate(IntPtr self);

				[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
				private static extern UInt64 SVSubscriber_ASDU_getRefrTmAsMs(IntPtr self);

				[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
				private static extern sbyte SVSubscriber_ASDU_getINT8(IntPtr self, int index);

				[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
				private static extern Int16	SVSubscriber_ASDU_getINT16(IntPtr self, int index);

				[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
				private static extern Int32	SVSubscriber_ASDU_getINT32(IntPtr self, int index);

				[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
				private static extern Int64 SVSubscriber_ASDU_getINT64(IntPtr self, int index);

				[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
				private static extern byte SVSubscriber_ASDU_getINT8U(IntPtr self, int index);

				[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
				private static extern UInt16 SVSubscriber_ASDU_getINT16U(IntPtr self, int index);

				[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
				private static extern UInt32 SVSubscriber_ASDU_getINT32U(IntPtr self, int index);

				[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
				private static extern UInt64 SVSubscriber_ASDU_getINT64U(IntPtr self, int index);

				[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
				private static extern float	SVSubscriber_ASDU_getFLOAT32(IntPtr self, int index);

				[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
				private static extern double SVSubscriber_ASDU_getFLOAT64(IntPtr self, int index);

				[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
				private static extern UInt16 SVSubscriber_ASDU_getQuality(IntPtr self, int index);

				[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
				private static extern int SVSubscriber_ASDU_getDataSize(IntPtr self);

				private IntPtr self;

				internal SVSubscriberASDU (IntPtr self)
				{
					this.self = self;
				}

				public UInt16 GetSmpCnt()
				{
					return SVSubscriber_ASDU_getSmpCnt (self);
				}

				public string GetSvId()
				{
					return Marshal.PtrToStringAnsi (SVSubscriber_ASDU_getSvId(self));
				}

				public string GetDatSet()
				{
					return Marshal.PtrToStringAnsi (SVSubscriber_ASDU_getDatSet(self));
				}

				public UInt32 GetConfRev()
				{
					return SVSubscriber_ASDU_getConfRev (self);
				}

				public SmpMod GetSmpMod()
				{
					return (SmpMod) SVSubscriber_ASDU_getSmpMod (self);
				}

				public UInt16 GetSmpRate()
				{
					return (UInt16)SVSubscriber_ASDU_getSmpRate (self);
				}

				public bool HasDatSet()
				{
					return SVSubscriber_ASDU_hasDatSet (self);
				}

				public bool HasRefrRm()
				{
					return SVSubscriber_ASDU_hasRefrTm (self);
				}

				public bool HasSmpMod()
				{
					return SVSubscriber_ASDU_hasSmpMod (self);
				}

				public bool HasSmpRate()
				{
					return SVSubscriber_ASDU_hasSmpRate (self);
				}

				public UInt64 GetRefrTmAsMs()
				{
					return SVSubscriber_ASDU_getRefrTmAsMs (self);
				}

				public sbyte GetINT8(int index)
				{
					return SVSubscriber_ASDU_getINT8 (self, index);
				}

				public Int16 GetINT16(int index)
				{
					return SVSubscriber_ASDU_getINT16 (self, index);
				}

				public Int32 GetINT32(int index)
				{
					return SVSubscriber_ASDU_getINT32 (self, index);
				}

				public Int64 GetINT64(int index)
				{
					return SVSubscriber_ASDU_getINT64 (self, index);
				}

				public byte GetINT8U(int index)
				{
					return SVSubscriber_ASDU_getINT8U (self, index);
				}

				public UInt16 GetINT16U(int index)
				{
					return SVSubscriber_ASDU_getINT16U (self, index);
				}

				public UInt32 GetINT32U(int index)
				{
					return SVSubscriber_ASDU_getINT32U (self, index);
				}

				public UInt64 GetINT64U(int index)
				{
					return SVSubscriber_ASDU_getINT64U (self, index);
				}

				public float GetFLOAT32(int index)
				{
					return SVSubscriber_ASDU_getFLOAT32 (self, index);
				}

				public double GetFLOAT64(int index)
				{
					return SVSubscriber_ASDU_getFLOAT64 (self, index);
				}

				private struct PTimestamp
				{
					[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.I1, SizeConst = 8)]
					public byte[] val;
				}

				[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
				private static extern PTimestamp SVSubscriber_ASDU_getTimestamp(IntPtr self, int index);

				public Timestamp GetTimestamp(int index)
				{
					PTimestamp retVal = SVSubscriber_ASDU_getTimestamp (self, index);

					return new Timestamp (retVal.val);
				}

				public Quality GetQuality(int index)
				{
					UInt16 qValue = SVSubscriber_ASDU_getQuality (self, index);

					return new Quality (qValue);
				}

				/// <summary>
				/// Gets the size of the payload data in bytes. The payload comprises the data set data.
				/// </summary>
				/// <returns>The payload data size in byte</returns>
				public int GetDataSize()
				{
					return SVSubscriber_ASDU_getDataSize (self);
				}
			}
		}

	}
}

