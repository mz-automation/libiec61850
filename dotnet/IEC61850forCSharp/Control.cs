/*
 *  Control.cs
 *
 *  Copyright 2014 Michael Zillgith
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
    // IEC 61850 common API parts (used by client and server API)
	namespace Common {

        /// <summary>
        /// Control model
        /// </summary>
        public enum ControlModel
        {
            /** status only */
            STATUS_ONLY = 0,
            /** direct with normal security */
            DIRECT_NORMAL= 1,
            /** select before operate (SBO) with normal security */
            SBO_NORMAL = 2,
            /** direct with enhanced security */
            DIRECT_ENHANCED = 3,
            /** select before operate (SBO) with enhanced security */
            SBO_ENHANCED = 4
        }

        /// <summary>
        /// Originator category
        /// </summary>
        public enum OrCat {
            /** Not supported - should not be used */
            NOT_SUPPORTED = 0,
            /** Control operation issued from an operator using a client located at bay level */
            BAY_CONTROL = 1,
            /** Control operation issued from an operator using a client located at station level */
            STATION_CONTROL = 2,
            /** Control operation from a remote operator outside the substation (for example network control center) */
            REMOTE_CONTROL = 3,
            /** Control operation issued from an automatic function at bay level */
            AUTOMATIC_BAY = 4,
            /** Control operation issued from an automatic function at station level */
            AUTOMATIC_STATION = 5,
            /** Control operation issued from a automatic function outside of the substation */
            AUTOMATIC_REMOTE = 6,
            /** Control operation issued from a maintenance/service tool */
            MAINTENANCE = 7,
            /** Status change occurred without control action (for example external trip of a circuit breaker or failure inside the breaker) */
            PROCESS = 8
        }
	}

	namespace Client
    {
		[StructLayout(LayoutKind.Sequential)]
		internal struct LastApplErrorInternal
		{
		    public int ctlNum;
		    public int error;
		    public int addCause;
		}

		public class LastApplError
		{
			public int ctlNum;
			public int error;
			public ControlAddCause addCause;


			internal LastApplError (LastApplErrorInternal lastApplError)
			{
				this.addCause = (ControlAddCause) lastApplError.addCause;
				this.error = lastApplError.error;
				this.ctlNum = lastApplError.ctlNum;
			}
		}

        public enum ControlActionType
        {
            SELECT = 0,
            OPERATE = 1,
            CANCEL = 2
        }

        /// <summary>
        /// Control object.
        /// </summary>
		public class ControlObject : IDisposable
		{
			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			private static extern LastApplErrorInternal ControlObjectClient_getLastApplError(IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			private static extern IntPtr ControlObjectClient_create(string objectReference, IntPtr connection);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ControlObjectClient_destroy(IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			private static extern int ControlObjectClient_getControlModel(IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			private static extern int ControlObjectClient_getCtlValType(IntPtr self);

            [DllImport ("iec61850", CallingConvention = CallingConvention.Cdecl)]
            private static extern int ControlObjectClient_getLastError (IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            [return: MarshalAs(UnmanagedType.I1)]
			private static extern bool ControlObjectClient_operate(IntPtr self, IntPtr ctlVal, UInt64 operTime);

            /// <summary>
            /// Handler for asynchronous control actions (select, operate, cancel)
            /// </summary>
            public delegate void ControlActionHandler (UInt32 invokeId, Object parameter, IedClientError error, ControlActionType type, bool success);

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            private delegate void ControlObjectClient_ControlActionHandler (UInt32 invokeId, IntPtr parameter, int err, int type, [MarshalAs(UnmanagedType.I1)] bool success);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            private static extern UInt32 ControlObjectClient_operateAsync(IntPtr self, out int err, IntPtr ctlVal, UInt64 operTime,
                ControlObjectClient_ControlActionHandler handler, IntPtr parameter);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            private static extern UInt32 ControlObjectClient_selectAsync(IntPtr self, out int err,
                ControlObjectClient_ControlActionHandler handler, IntPtr parameter);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            private static extern UInt32 ControlObjectClient_selectWithValueAsync(IntPtr self, out int err, IntPtr ctlVal,
                ControlObjectClient_ControlActionHandler handler, IntPtr parameter);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            private static extern UInt32 ControlObjectClient_cancelAsync(IntPtr self, out int err,
                ControlObjectClient_ControlActionHandler handler, IntPtr parameter);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            [return: MarshalAs(UnmanagedType.I1)]
            private static extern bool ControlObjectClient_select(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            [return: MarshalAs(UnmanagedType.I1)]
            private static extern bool ControlObjectClient_selectWithValue(IntPtr self, IntPtr ctlVal);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            [return: MarshalAs(UnmanagedType.I1)]
            private static extern bool ControlObjectClient_cancel(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            private static extern void ControlObjectClient_setOrigin(IntPtr self, string orIdent, int orCat);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ControlObjectClient_setInterlockCheck(IntPtr self, [MarshalAs(UnmanagedType.I1)] bool value);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ControlObjectClient_setSynchroCheck(IntPtr self, [MarshalAs(UnmanagedType.I1)] bool value);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ControlObjectClient_setTestMode(IntPtr self, [MarshalAs(UnmanagedType.I1)] bool value);

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
			private delegate void InternalCommandTerminationHandler(IntPtr parameter,IntPtr controlClient);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ControlObjectClient_setCommandTerminationHandler(IntPtr self, 
			           InternalCommandTerminationHandler handler, IntPtr handlerParameter);

			public delegate void CommandTerminationHandler (Object parameter, ControlObject controlObject);

            private IedConnection iedConnection;
			private IntPtr self;

			private CommandTerminationHandler commandTerminationHandler = null;
			private Object commandTerminationHandlerParameter = null;

			private void MyCommandTerminationHandler (IntPtr paramter, IntPtr controlClient) 
			{
				if (commandTerminationHandler != null)
					commandTerminationHandler(commandTerminationHandlerParameter, this);
			}

			private InternalCommandTerminationHandler intCommandTerminationHandler;

			internal ControlObject (string objectReference, IntPtr connection, IedConnection iedConnection)
			{
                this.iedConnection = iedConnection;

				this.self = ControlObjectClient_create(objectReference, connection);

				if (this.self == System.IntPtr.Zero)
					throw new IedConnectionException("Control object not found", 0);

				intCommandTerminationHandler = new InternalCommandTerminationHandler (MyCommandTerminationHandler);

				ControlObjectClient_setCommandTerminationHandler(self, intCommandTerminationHandler, self);
			}

            /// <summary>
            /// Gets the control model.
            /// </summary>
            /// <returns>
            /// The control model.
            /// </returns>
			public ControlModel GetControlModel ()
			{
				ControlModel controlModel = (ControlModel) ControlObjectClient_getControlModel(self);

				return controlModel;
			}

			/// <summary>
			/// Get the type of ctlVal.
			/// </summary>
			/// <returns>MmsType required for the ctlVal value.</returns>
			public MmsType GetCtlValType ()
			{
				MmsType ctlValType = (MmsType) ControlObjectClient_getCtlValType (self);

				return ctlValType;
			}

            /// <summary>
            /// Sets the origin parameter used by control commands.
            /// </summary>
            /// <param name='originator'>
            /// Originator. An arbitrary string identifying the controlling client.
            /// </param>
            /// <param name='originatorCategory'>
            /// Originator category.
            /// </param>
            public void SetOrigin (string originator, OrCat originatorCategory)
            {
                ControlObjectClient_setOrigin(self, originator, (int) originatorCategory);
            }

            /// <summary>
            /// Gets the error code of the last synchronous control action (operate, select, select-with-value, cancel)
            /// </summary>
            /// <value>error code.</value>
            public IedClientError LastError {
                get {
                    return (IedClientError)ControlObjectClient_getLastError (self);
                }
            }

            /// <summary>
            /// Operate the control with the specified control value.
            /// </summary>
            /// <param name='ctlVal'>the new value of the control</param>
            /// <returns>true when the operation has been successful, false otherwise</returns>
			public bool Operate (bool ctlVal)
			{
				return Operate (ctlVal, 0);
			}

            /// <summary>
            /// Operate the control with the specified control value (time activated control).
            /// </summary>
            /// <param name='ctlVal'>the new value of the control</param>
            /// <param name='operTime'>the time when the operation will be executed</param>
            /// <returns>true when the operation has been successful, false otherwise</returns>
			public bool Operate (bool ctlVal, UInt64 operTime)
			{
				MmsValue value = new MmsValue(ctlVal);

				return Operate (value, operTime);
			}

            /// <summary>
            /// Operate the control with the specified control value.
            /// </summary>
            /// <param name='ctlVal'>the new value of the control</param>
            /// <returns>true when the operation has been successful, false otherwise</returns>
			public bool Operate (float ctlVal)
			{
				return Operate (ctlVal, 0);
			}

            /// <summary>
            /// Operate the control with the specified control value (time activated control).
            /// </summary>
            /// <param name='ctlVal'>the new value of the control</param>
            /// <param name='operTime'>the time when the operation will be executed</param>
            /// <returns>true when the operation has been successful, false otherwise</returns>
			public bool Operate (float ctlVal, UInt64 operTime)
			{
				MmsValue value = new MmsValue(ctlVal);

				return Operate (value, operTime);
			}

            /// <summary>
            /// Operate the control with the specified control value.
            /// </summary>
            /// <param name='ctlVal'>the new value of the control</param>
            /// <returns>true when the operation has been successful, false otherwise</returns>
			public bool Operate (int ctlVal)
			{
				return Operate (ctlVal, 0);
			}

            /// <summary>
            /// Operate the control with the specified control value (time activated control).
            /// </summary>
            /// <param name='ctlVal'>the new value of the control</param>
            /// <param name='operTime'>the time when the operation will be executed</param>
            /// <returns>true when the operation has been successful, false otherwise</returns>
			public bool Operate (int ctlVal, UInt64 operTime)
			{
				MmsValue value = new MmsValue(ctlVal);

				return Operate (value, operTime);
			}

            /// <summary>
            /// Operate the control with the specified control value.
            /// </summary>
            /// <param name='ctlVal'>the new value of the control</param>
            /// <returns>true when the operation has been successful, false otherwise</returns>
			public bool Operate (MmsValue ctlVal)
			{
				return Operate (ctlVal, 0);
			}

            /// <summary>
            /// Operate the control with the specified control value (time activated control).
            /// </summary>
            /// <param name='ctlVal'>the new value of the control</param>
            /// <param name='operTime'>the time when the operation will be executed</param>
            /// <returns>true when the operation has been successful, false otherwise</returns>
			public bool Operate (MmsValue ctlVal, UInt64 operTime)
			{
				return ControlObjectClient_operate(self, ctlVal.valueReference, operTime);
			}

            private void nativeOperateHandler (UInt32 invokeId, IntPtr parameter, int err, int type, bool success)
            {
                GCHandle handle = GCHandle.FromIntPtr(parameter);

                Tuple<ControlActionHandler, object>  callbackInfo = handle.Target as Tuple<ControlActionHandler, object>;

                ControlActionHandler handler = callbackInfo.Item1;
                object handlerParameter = callbackInfo.Item2;

                handle.Free();

                IedClientError clientError = (IedClientError)err;

                handler(invokeId, handlerParameter, clientError, (ControlActionType) type, success);
            }


            /// <summary>
            /// Operate the control with the specified control value.
            /// </summary>
            /// <param name='ctlVal'>the new value of the control</param>
            /// <param name="handler">Callback function to handle the received response or service timeout</param>
            /// <param name="parameter">User provided callback parameter. Will be passed to the callback function</param>
            /// <returns>the invoke ID of the sent request</returns>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public UInt32 OperateAsync (bool ctlVal, ControlActionHandler handler, object parameter)
            {
                return OperateAsync (ctlVal, 0, handler, parameter);
            }
                
            /// <summary>
            /// Operate the control with the specified control value (time activated control).
            /// </summary>
            /// <param name='ctlVal'>the new value of the control</param>
            /// <param name='operTime'>the time when the operation will be executed</param>
            /// <param name="handler">Callback function to handle the received response or service timeout</param>
            /// <param name="parameter">User provided callback parameter. Will be passed to the callback function</param>
            /// <returns>the invoke ID of the sent request</returns>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public UInt32 OperateAsync (bool ctlVal, UInt64 operTime, ControlActionHandler handler, object parameter)
            {
                MmsValue value = new MmsValue(ctlVal);

                return OperateAsync (value, operTime, handler, parameter);
            }
                
            /// <summary>
            /// Operate the control with the specified control value.
            /// </summary>
            /// <param name='ctlVal'>the new value of the control</param>
            /// <param name="handler">Callback function to handle the received response or service timeout</param>
            /// <param name="parameter">User provided callback parameter. Will be passed to the callback function</param>
            /// <returns>the invoke ID of the sent request</returns>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public UInt32 OperateAsync (float ctlVal, ControlActionHandler handler, object parameter)
            {
                return OperateAsync (ctlVal, 0, handler, parameter);
            }
                
            /// <summary>
            /// Operate the control with the specified control value (time activated control).
            /// </summary>
            /// <param name='ctlVal'>the new value of the control</param>
            /// <param name='operTime'>the time when the operation will be executed</param>
            /// <param name="handler">Callback function to handle the received response or service timeout</param>
            /// <param name="parameter">User provided callback parameter. Will be passed to the callback function</param>
            /// <returns>the invoke ID of the sent request</returns>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public UInt32 OperateAsync (float ctlVal, UInt64 operTime, ControlActionHandler handler, object parameter)
            {
                MmsValue value = new MmsValue(ctlVal);

                return OperateAsync (value, operTime, handler, parameter);
            }
                
            /// <summary>
            /// Operate the control with the specified control value.
            /// </summary>
            /// <param name='ctlVal'>the new value of the control</param>
            /// <param name="handler">Callback function to handle the received response or service timeout</param>
            /// <param name="parameter">User provided callback parameter. Will be passed to the callback function</param>
            /// <returns>the invoke ID of the sent request</returns>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public UInt32 OperateAsync (int ctlVal, ControlActionHandler handler, object parameter)
            {
                return OperateAsync (ctlVal, 0, handler, parameter);
            }

            /// <summary>
            /// Operate the control with the specified control value (time activated control).
            /// </summary>
            /// <param name='ctlVal'>the new value of the control</param>
            /// <param name='operTime'>the time when the operation will be executed</param>
            /// <param name="handler">Callback function to handle the received response or service timeout</param>
            /// <param name="parameter">User provided callback parameter. Will be passed to the callback function</param>
            /// <returns>the invoke ID of the sent request</returns>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public UInt32 OperateAsync (int ctlVal, UInt64 operTime, ControlActionHandler handler, object parameter)
            {
                return OperateAsync (ctlVal, operTime, handler, parameter);
            }

            /// <summary>
            /// Operate the control with the specified control value.
            /// </summary>
            /// <param name='ctlVal'>the new value of the control</param>
            /// <param name="handler">Callback function to handle the received response or service timeout</param>
            /// <param name="parameter">User provided callback parameter. Will be passed to the callback function</param>
            /// <returns>the invoke ID of the sent request</returns>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public UInt32 OperateAsync (MmsValue ctlVal, ControlActionHandler handler, object parameter)
            {
                return OperateAsync (ctlVal, 0, handler, parameter);
            }

            /// <summary>
            /// Operate the control with the specified control value (time activated control).
            /// </summary>
            /// <param name='ctlVal'>the new value of the control</param>
            /// <param name='operTime'>the time when the operation will be executed</param>
            /// <param name="handler">Callback function to handle the received response or service timeout</param>
            /// <param name="parameter">User provided callback parameter. Will be passed to the callback function</param>
            /// <returns>the invoke ID of the sent request</returns>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public UInt32 OperateAsync (MmsValue ctlVal, UInt64 operTime, ControlActionHandler handler, object parameter)
            {
                int error;

                Tuple<ControlActionHandler, object> callbackInfo = Tuple.Create(handler, parameter);

                GCHandle handle = GCHandle.Alloc(callbackInfo);

                UInt32 invokeId = ControlObjectClient_operateAsync(self, out error, ctlVal.valueReference, operTime, nativeOperateHandler, GCHandle.ToIntPtr(handle));

                if (error != 0)
                {
                    handle.Free();
                    throw new IedConnectionException("Operate failed", error);
                }

                return invokeId;
            }

            /// <summary>
            /// Select the control object.
            /// </summary>
            /// <returns>true when the selection has been successful, false otherwise</returns>
            public bool Select ()
            {
                return ControlObjectClient_select(self);
            }

            /// <summary>
            /// Select the control object.
            /// </summary>
            /// <param name="handler">Callback function to handle the received response or service timeout</param>
            /// <param name="parameter">User provided callback parameter. Will be passed to the callback function</param>
            /// <returns>the invoke ID of the sent request</returns>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public UInt32 SelectAsync(ControlActionHandler handler, object parameter)
            {
                int error;

                Tuple<ControlActionHandler, object> callbackInfo = Tuple.Create(handler, parameter);

                GCHandle handle = GCHandle.Alloc(callbackInfo);

                UInt32 invokeId = ControlObjectClient_selectAsync(self, out error, nativeOperateHandler, GCHandle.ToIntPtr(handle));

                if (error != 0)
                {
                    handle.Free();
                    throw new IedConnectionException("Select failed", error);
                }

                return invokeId;
            }

            /// <summary>
            /// Send a select with value command for generic MmsValue instances
            /// </summary>
            /// <param name='ctlVal'>the value to be checked.</param>
            /// <returns>true when the selection has been successful, false otherwise</returns>
            public bool SelectWithValue (MmsValue ctlVal)
            {
                return ControlObjectClient_selectWithValue(self, ctlVal.valueReference);
            }

            /// <summary>
            /// Send a select with value command for boolean controls
            /// </summary>
            /// <param name='ctlVal'>
            /// the value to be checked.
            /// </param>
            /// <returns>true when the selection has been successful, false otherwise</returns>
            public bool SelectWithValue (bool ctlVal)
            {
                return SelectWithValue(new MmsValue(ctlVal));
            }

            /// <summary>
            /// Send a select with value command for integer controls
            /// </summary>
            /// <param name='ctlVal'>
            /// the value to be checked.
            /// </param>
            /// <returns>true when the selection has been successful, false otherwise</returns>
            public bool SelectWithValue (int ctlVal)
            {
                return SelectWithValue(new MmsValue(ctlVal));
            }

            /// <summary>
            /// Send a select with value command for float controls
            /// </summary>
            /// <param name='ctlVal'>
            /// the value to be checked.
            /// </param>
            /// <returns>true when the selection has been successful, false otherwise</returns>
            public bool SelectWithValue (float ctlVal)
            {
                return SelectWithValue(new MmsValue(ctlVal));
            }

            /// <summary>
            /// Send a select with value command for boolean controls - asynchronous version
            /// </summary>
            /// <param name='ctlVal'>the value to be checked.</param>
            /// <param name="handler">Callback function to handle the received response or service timeout</param>
            /// <param name="parameter">User provided callback parameter. Will be passed to the callback function</param>
            /// <returns>the invoke ID of the sent request</returns>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public UInt32 SelectWithValueAsync (bool ctlVal, ControlActionHandler handler, object parameter)
            {
                return SelectWithValueAsync(new MmsValue(ctlVal), handler, parameter);
            }

            /// <summary>
            /// Send a select with value command for integer controls - asynchronous version
            /// </summary>
            /// <param name='ctlVal'>the value to be checked.</param>
            /// <param name="handler">Callback function to handle the received response or service timeout</param>
            /// <param name="parameter">User provided callback parameter. Will be passed to the callback function</param>
            /// <returns>the invoke ID of the sent request</returns>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public UInt32 SelectWithValueAsync (int ctlVal, ControlActionHandler handler, object parameter)
            {
                return SelectWithValueAsync(new MmsValue(ctlVal), handler, parameter);
            }

            /// <summary>
            /// Send a select with value command for float controls - asynchronous version
            /// </summary>
            /// <param name='ctlVal'>the value to be checked.</param>
            /// <param name="handler">Callback function to handle the received response or service timeout</param>
            /// <param name="parameter">User provided callback parameter. Will be passed to the callback function</param>
            /// <returns>the invoke ID of the sent request</returns>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public UInt32 SelectWithValueAsync (float ctlVal, ControlActionHandler handler, object parameter)
            {
                return SelectWithValueAsync(new MmsValue(ctlVal), handler, parameter);
            }
                
            /// <summary>
            /// Send a select with value command for generic MmsValue instances - asynchronous version
            /// </summary>
            /// <param name='ctlVal'>the value to be checked.</param>
            /// <param name="handler">Callback function to handle the received response or service timeout</param>
            /// <param name="parameter">User provided callback parameter. Will be passed to the callback function</param>
            /// <returns>the invoke ID of the sent request</returns>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public UInt32 SelectWithValueAsync (MmsValue ctlVal, ControlActionHandler handler, object parameter)
            {
                int error;

                Tuple<ControlActionHandler, object> callbackInfo = Tuple.Create(handler, parameter);

                GCHandle handle = GCHandle.Alloc(callbackInfo);

                UInt32 invokeId = ControlObjectClient_selectWithValueAsync(self, out error, ctlVal.valueReference, nativeOperateHandler, GCHandle.ToIntPtr(handle));

                if (error != 0)
                {
                    handle.Free();
                    throw new IedConnectionException("Select with value failed", error);
                }

                return invokeId;
            }

            /// <summary>
            /// Cancel a selection or time activated operation
            /// </summary>
            /// <returns>true when the cancelation has been successful, false otherwise</returns>
            /// <param name="handler">Callback function to handle the received response or service timeout</param>
            /// <param name="parameter">User provided callback parameter. Will be passed to the callback function</param>
            /// <returns>the invoke ID of the sent request</returns>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public bool Cancel () 
            {
                return ControlObjectClient_cancel(self);
            }

            /// <summary>
            /// Cancel a selection or time activated operation
            /// </summary>
            public UInt32 CancelAsync(ControlActionHandler handler, object parameter)
            {
                int error;

                Tuple<ControlActionHandler, object> callbackInfo = Tuple.Create(handler, parameter);

                GCHandle handle = GCHandle.Alloc(callbackInfo);

                UInt32 invokeId = ControlObjectClient_cancelAsync(self, out error, nativeOperateHandler, GCHandle.ToIntPtr(handle));

                if (error != 0)
                {
                    handle.Free();
                    throw new IedConnectionException("Cancel failed", error);
                }

                return invokeId;
            }

            /// <summary>
            /// Enables the synchro check for operate commands
            /// </summary>
			[Obsolete("use SetSynchroCheck instead")]
            public void EnableSynchroCheck ()
            {
				ControlObjectClient_setSynchroCheck (self, true);
            }

            /// <summary>
            /// Enables the interlock check for operate and select commands
            /// </summary>
			[Obsolete("use SetInterlockCheck instead")]
			public void EnableInterlockCheck ()
            {
				ControlObjectClient_setInterlockCheck (self, true);
            }

			/// <summary>
			/// Sets the value of the interlock check flag for operate and select commands
			/// </summary>
			public void SetInterlockCheck (bool value)
			{
				ControlObjectClient_setInterlockCheck (self, value);
			}

			/// <summary>
			/// Sets the value of the synchro check flag for operate command
			/// </summary>
			public void SetSynchroCheck (bool value)
			{
				ControlObjectClient_setSynchroCheck (self, value);
			}

			/// <summary>
			/// Sets the value of the test flag for the operate command
			/// </summary>
			public void SetTestMode (bool value)
			{
				ControlObjectClient_setTestMode (self, value);
			}

			/// <summary>
			/// Gets the last received LastApplError (Additional Cause Diagnostics) value.
			/// </summary>
			/// <returns>
			/// The last appl error.
			/// </returns>
			public LastApplError GetLastApplError ()
			{
				LastApplErrorInternal lastApplError = ControlObjectClient_getLastApplError(self);

				return new LastApplError(lastApplError);
			}

			/// <summary>
			/// Sets the command termination handler.
			/// </summary>
			/// <param name='handler'>
			/// the handler (delegate) that is invoked when a CommandTerminationMessage is received.
			/// </param>
			/// <param name='parameter'>
			/// Parameter.
			/// </param>
			public void SetCommandTerminationHandler (CommandTerminationHandler handler, Object parameter)
			{
				this.commandTerminationHandler = handler;
				this.commandTerminationHandlerParameter = parameter;
			}

			protected virtual void Dispose(bool disposing) {
				if (this.self != System.IntPtr.Zero) {
					ControlObjectClient_destroy (self);
					this.self = System.IntPtr.Zero;
				}
			}

			public void Dispose() {
				Dispose (true);
			}
				

		}

	}

}

