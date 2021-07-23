/*
 *  GooseSubscriber.cs
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
using System.Collections.Generic;
using System.Runtime.InteropServices;
using IEC61850.Common;

namespace IEC61850
{
	namespace GOOSE
	{

		namespace Subscriber 
		{

			/// <summary>
			/// GOOSE listener.
			/// </summary>
			public delegate void GooseListener (GooseSubscriber report, object parameter);

			public class GooseReceiver : IDisposable
			{
				[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
				private static extern IntPtr GooseReceiver_create ();

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
				private static extern bool GooseReceiver_isRunning (IntPtr self);

				[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
				private static extern void GooseReceiver_destroy(IntPtr self);

				[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
				private static extern void GooseReceiver_setInterfaceId(IntPtr self, string interfaceId);

				private IntPtr self;

				private bool isDisposed = false;

				private List<GooseSubscriber> subscribers = new List<GooseSubscriber>();

				public GooseReceiver()
				{
					self = GooseReceiver_create ();
				}

				public void SetInterfaceId(string interfaceId)
				{
					GooseReceiver_setInterfaceId (self, interfaceId);
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
					GooseReceiver_start (self);
				}

				public void Stop()
				{
					GooseReceiver_stop (self);
				}

				public bool IsRunning()
				{
					return GooseReceiver_isRunning (self);
				}

				public void Dispose()
				{
					if (isDisposed == false) {
						isDisposed = true;
						GooseReceiver_destroy (self);
						self = IntPtr.Zero;
					}
				}

				~GooseReceiver()
				{
					Dispose ();
				}
			}


			/// <summary>
			/// Representing a GOOSE subscriber
			/// </summary>
			/// <description>
			/// NOTE: After SetListener is called, do not call any function outside of
			/// the callback handler!
			/// </description>
			public class GooseSubscriber : IDisposable
			{

				[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
				private delegate void InternalGooseListener (IntPtr subscriber, IntPtr parameter);

				[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
				private static extern IntPtr GooseSubscriber_create (string goCbRef, IntPtr dataSetValue);

				[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
				private static extern void GooseSubscriber_setAppId(IntPtr self, UInt16 appId);

				[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
				[return: MarshalAs(UnmanagedType.I1)]
				private static extern bool GooseSubscriber_isValid (IntPtr self);

				[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
				private static extern UInt32 GooseSubscriber_getStNum (IntPtr self);

				[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
				private static extern UInt32 GooseSubscriber_getSqNum (IntPtr self);

				[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
				[return: MarshalAs(UnmanagedType.I1)]
				private static extern bool GooseSubscriber_isTest (IntPtr self);

				[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
				private static extern UInt32 GooseSubscriber_getConfRev (IntPtr self);

				[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
				[return: MarshalAs(UnmanagedType.I1)]
				private static extern bool GooseSubscriber_needsCommission (IntPtr self);

				[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
				private static extern UInt32 GooseSubscriber_getTimeAllowedToLive (IntPtr self);

				[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
				private static extern UInt64 GooseSubscriber_getTimestamp (IntPtr self);

				[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
				private static extern IntPtr GooseSubscriber_getDataSetValues(IntPtr self);

				[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
				private static extern void GooseSubscriber_destroy(IntPtr self);

				[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
				private static extern void GooseSubscriber_setListener (IntPtr self, InternalGooseListener listener, IntPtr parameter);

				[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
				private static extern IntPtr GooseSubscriber_getGoId(IntPtr self);

				[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
				private static extern IntPtr GooseSubscriber_getGoCbRef(IntPtr self);

				[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
				private static extern IntPtr GooseSubscriber_getDataSet(IntPtr self);

				internal IntPtr self;

				private bool isDisposed = false;

				// don't call native destructor when attached to a receiver
				internal bool attachedToReceiver = false;

				private GooseListener listener = null;
				private object listenerParameter = null;

				private event InternalGooseListener internalListener = null;

				private void internalGooseListener (IntPtr subscriber, IntPtr parameter)
				{
					try {

						if (listener != null) {
							listener(this, listenerParameter);
						}

					} catch (Exception e) 
					{
						// older versions of mono 2.10 (for linux?) cause this exception
						Console.WriteLine(e.Message);
					}
				}

				public GooseSubscriber(string goCbRef)
				{
					self = GooseSubscriber_create (goCbRef, IntPtr.Zero);
				}

				public void SetAppId(UInt16 appId)
				{
					GooseSubscriber_setAppId (self, appId);
				}

				public bool IsValid ()
				{
					return GooseSubscriber_isValid (self);
				}

				public void SetListener(GooseListener listener, object parameter)
				{
					this.listener = listener;
					this.listenerParameter = parameter;

					if (internalListener == null) {
						internalListener = new InternalGooseListener (internalGooseListener);

						GooseSubscriber_setListener (self, internalListener, IntPtr.Zero);
					}
				}

				public string GetGoId()
				{
					return Marshal.PtrToStringAnsi(GooseSubscriber_getGoId(self));
				}

				public string GetGoCbRef()
				{
					return Marshal.PtrToStringAnsi(GooseSubscriber_getGoCbRef(self));
				}

				public string GetDataSet()
				{
					return Marshal.PtrToStringAnsi(GooseSubscriber_getDataSet(self));
				}

				public UInt32 GetStNum()
				{
					return GooseSubscriber_getStNum (self);
				}

				public UInt32 GetSqNum()
				{
					return GooseSubscriber_getSqNum (self);
				}

				public bool IsTest()
				{
					return GooseSubscriber_isTest (self);
				}

				public UInt32 GetConfRev()
				{
					return GooseSubscriber_getConfRev (self);
				}

				public bool NeedsCommission()
				{
					return GooseSubscriber_needsCommission (self);
				}

				public UInt32 GetTimeAllowedToLive()
				{
					return GooseSubscriber_getTimeAllowedToLive (self);
				}

				public UInt64 GetTimestamp ()
				{
					return GooseSubscriber_getTimestamp (self);
				}
					
				public DateTimeOffset GetTimestampsDateTimeOffset ()
				{
					UInt64 entryTime = GetTimestamp ();

					DateTimeOffset retVal = new DateTimeOffset (1970, 1, 1, 0, 0, 0, TimeSpan.Zero);

					return retVal.AddMilliseconds (entryTime);
				}

				/// <summary>
				/// Get the values of the GOOSE data set from the last received GOOSE message
				/// </summary>
				/// <remarks>
				/// The MmsValue instance is only valid in the context of the GooseLister callback.
				/// Do not store for outside use!
				/// </remarks>
				/// <returns>The data set values.</returns>
				public MmsValue GetDataSetValues()
				{
					IntPtr mmsValueRef = GooseSubscriber_getDataSetValues (self);

					return (new MmsValue (mmsValueRef));
				}

				/// <summary>
				/// Releases all resource used by the <see cref="IEC61850.GOOSE.Subscriber.GooseSubscriber"/> object.
				/// </summary>
				/// <remarks>>
				/// This function has only to be called when the <see cref="IEC61850.GOOSE.Subscriber.GooseSubscriber"/>
				/// has not been added to the GooseReceiver or has been removed from the GooseReceiver.
				/// When the GooseReceiver holds a reference it will take care for releasing the resources.
				/// In this case Dispose MUST not be called! Otherwise the natice resources will be
				/// released twice.
				/// </remarks>
				public void Dispose()
				{
					if (isDisposed == false) {
						isDisposed = true;

						if (attachedToReceiver == false)
							GooseSubscriber_destroy (self);

						self = IntPtr.Zero;
					}
				}

				~GooseSubscriber()
				{
					Dispose();
				}

            }

		}
			
	}
}