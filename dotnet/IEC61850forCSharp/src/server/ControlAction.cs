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
        /// <summary>
        /// Represents additional context information of the control action that caused the callback invokation
        /// </summary>
        public class ControlAction
        {
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void ControlAction_setAddCause(IntPtr self, int addCause);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void ControlAction_setError(IntPtr self, int error);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern int ControlAction_getOrCat(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr ControlAction_getOrIdent(IntPtr self, ref int size);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr ControlAction_getClientConnection(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern int ControlAction_getCtlNum(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern UInt64 ControlAction_getControlTime(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            [return: MarshalAs(UnmanagedType.I1)]
            static extern bool ControlAction_isSelect(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            [return: MarshalAs(UnmanagedType.I1)]
            static extern bool ControlAction_getSynchroCheck(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            [return: MarshalAs(UnmanagedType.I1)]
            static extern bool ControlAction_getInterlockCheck(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr ControlAction_getT(IntPtr self);

            private IntPtr self;
            private IedServer.ControlHandlerInfo info;
            private IedServer iedServer;

            internal ControlAction(IntPtr self, IedServer.ControlHandlerInfo info, IedServer iedServer)
            {
                this.self = self;
                this.info = info;
                this.iedServer = iedServer;
            }

            /// <summary>
            /// Sets the error code for the next command termination or application error message.
            /// </summary>
            /// <param name="error">the errror code to use</param>
            public void SetError(ControlLastApplError error)
            {
                ControlAction_setError(self, (int)error);
            }

            /// <summary>
            /// Sets the add cause for the next command termination or application error message
            /// </summary>
            /// <param name="addCause">the additional cause code</param>
            public void SetAddCause(ControlAddCause addCause)
            {
                ControlAction_setAddCause(self, (int)addCause);
            }

            /// <summary>
            /// Gets the originator category provided by the client
            /// </summary>
            /// <returns>The or cat.</returns>
            public OrCat GetOrCat()
            {
                return (OrCat)ControlAction_getOrCat(self);
            }

            /// <summary>
            ///  Get the originator identifier provided by the client
            /// </summary>
            /// <returns>The or ident.</returns>
            public byte[] GetOrIdent()
            {
                int size = 0;

                IntPtr orIdentPtr = ControlAction_getOrIdent(self, ref size);

                if (orIdentPtr == IntPtr.Zero)
                    return null;

                byte[] orIdent = new byte[size];

                Marshal.Copy(orIdentPtr, orIdent, 0, size);

                return orIdent;
            }

            /// <summary>
            /// Gets the ctlNum attribute of the control action
            /// </summary>
            /// <returns>The ctlNum value. Valid values are restricted from 0 to 255, -1 means not present</returns>
            public int GetCtlNum()
            {
                return ControlAction_getCtlNum(self);
            }

            /// <summary>
            /// Gets the control object that is subject to this action
            /// </summary>
            /// <returns>the controllable data object instance</returns>
            public DataObject GetControlObject()
            {
                return info.controlObject;
            }

            /// <summary>
            ///  Gets the time of control execution, if it's a time activated control
            /// </summary>
            /// <returns>The time of control execution or 0 for immediate execution</returns>
            public UInt64 GetControlTime()
            {
                return ControlAction_getControlTime(self);
            }

            /// <summary>
            /// Gets the tome of control execution as data time offset.
            /// </summary>
            /// <returns>The control execution time as data time offset.</returns>
            public DateTimeOffset GetControlTimeAsDataTimeOffset()
            {
                return MmsValue.MsTimeToDateTimeOffset(GetControlTime());
            }

            /// <summary>
            /// Gets the client object associated with the client that caused the control action
            /// </summary>
            /// <returns>The client connection.</returns>
            public ClientConnection GetClientConnection()
            {
                ClientConnection con = null;

                IntPtr conPtr = ControlAction_getClientConnection(self);

                if (conPtr != IntPtr.Zero)
                {
                    iedServer.clientConnections.TryGetValue(conPtr, out con);
                }

                return con;
            }

            /// <summary>
            /// Cehck if the control callback is called by a select or operate command
            /// </summary>
            /// <returns><c>true</c>, if select, <c>false</c> otherwise.</returns>
            public bool IsSelect()
            {
                return ControlAction_isSelect(self);
            }

            public bool GetSynchroCheck()
            {
                return ControlAction_getSynchroCheck(self);
            }

            public bool GetInterlockCheck()
            {
                return ControlAction_getInterlockCheck(self);
            }

            /// <summary>
            /// Gets the time (paramter T) of the control action
            /// </summary>
            public Timestamp GetT()
            {
                IntPtr tPtr = ControlAction_getT(self);

                Timestamp t = new Timestamp(tPtr, false);

                return new Timestamp(t);
            }
        }

    }
}