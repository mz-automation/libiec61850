/*
 *  IEC61850ClientAPI.cs
 *
 *  Copyright 2014-2019 Michael Zillgith
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

using IEC61850.Common;
using IEC61850.TLS;

// IEC 61850 API for the libiec61850 .NET wrapper library
namespace IEC61850
{
    // IEC 61850 client API.
    namespace Client
    {

        [StructLayout(LayoutKind.Sequential)]
        public class MmsServerIdentity
        {
            public string vendorName;
            public string modelName;
            public string revision;
        }

        /// <summary>
        /// Represents an MmsConnection object (a single connection to an MMS server)
        /// </summary>
        public class MmsConnection
        {

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            private static extern IntPtr MmsConnection_create();

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            private static extern void MmsConnection_destroy(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            private static extern IntPtr MmsConnection_identify(IntPtr self, out int mmsError);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            private static extern void MmsServerIdentity_destroy(IntPtr self);

            [DllImport ("iec61850", CallingConvention=CallingConvention.Cdecl)]
            private static extern void MmsConnection_setLocalDetail (IntPtr self, Int32 localDetail);

            [DllImport ("iec61850", CallingConvention=CallingConvention.Cdecl)]
            private static extern Int32 MmsConnection_getLocalDetail (IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            private static extern Int32 MmsConnection_setRequestTimeout(IntPtr self, UInt32 timeoutInMs);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            private static extern IntPtr MmsConnection_readMultipleVariables(IntPtr self, out int mmsError,
                                                                             string domainId, IntPtr items);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            private static extern void MmsValue_delete(IntPtr self);

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            private delegate void LinkedListValueDeleteFunction(IntPtr pointer);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            private static extern void LinkedList_destroyDeep(IntPtr list, LinkedListValueDeleteFunction valueDeleteFunction);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            private static extern IntPtr LinkedList_create();

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            private static extern void LinkedList_add(IntPtr self, IntPtr data);


            private IntPtr self = IntPtr.Zero;
            private bool selfDestroy = false;

            public MmsConnection()
            {
                selfDestroy = true;

                self = MmsConnection_create();
            }

            internal MmsConnection(IntPtr mmsConnection)
            {
                self = mmsConnection;
            }

            ~MmsConnection ()
            {
                if (selfDestroy)
                if (self != IntPtr.Zero)
                    MmsConnection_destroy(self);
            }

            private void FreeHGlobaleDeleteFunction(IntPtr pointer)
            {
                Marshal.FreeHGlobal(pointer);
            }

            /// <summary>
            /// Requests the server identity information
            /// </summary>
            /// <returns>The server identity.</returns>
            public MmsServerIdentity GetServerIdentity()
            {
                int mmsError;

                if (self == IntPtr.Zero)
                {
                    throw new IedConnectionException("Pointer is Zero!");
                }

                IntPtr identity = MmsConnection_identify(self, out mmsError);

                if (mmsError != 0)
                    throw new IedConnectionException("Failed to read server identity", mmsError);

                if (identity == IntPtr.Zero)
                    throw new IedConnectionException("Failed to read server identity");

                MmsServerIdentity serverIdentity = (MmsServerIdentity)
					Marshal.PtrToStructure(identity, typeof(MmsServerIdentity));

                MmsServerIdentity_destroy(identity);

                return serverIdentity;
            }

            /// <summary>
            /// Sets the local detail (maximum MMS PDU size)
            /// </summary>
            /// <param name="localDetail">maximum accepted MMS PDU size in bytes</param>
            public void SetLocalDetail(int localDetail)
            {
                MmsConnection_setLocalDetail(self, localDetail);
            }

            /// <summary>
            /// Gets the local detail (maximum MMS PDU size)
            /// </summary>
            /// <returns>maximum accepted MMS PDU size in bytes</returns>
            public int GetLocalDetail()
            {
                return MmsConnection_getLocalDetail(self);
            }

            /// <summary>
            /// Sets the request timeout
            /// </summary>
            /// <param name="timeoutMs">request timeout in milliseconds</param>
            public void SetRequestTimeout(uint timeoutMs)
            {
                MmsConnection_setRequestTimeout(self, timeoutMs);
            }

            /// <summary>
            /// Reads multipe MMS variables from the same domain
            /// </summary>
            /// <returns>MmsValue of type MMS_ARRAY containing the access results for the requested variables.</returns>
            /// <param name="domainName">the domain name (logical device)</param>
            /// <param name="variables">list of variable names (in MMS notation e.g. "GGIO$ST$Ind1")</param>
            public MmsValue ReadMultipleVariables(string domainName, List<string> variables)
            {
                IntPtr linkedList = LinkedList_create();

                foreach (string variableName in variables)
                {
                    IntPtr handle = System.Runtime.InteropServices.Marshal.StringToHGlobalAnsi(variableName);

                    LinkedList_add(linkedList, handle);
                }

                int error;

                IntPtr mmsValue = MmsConnection_readMultipleVariables(self, out error, domainName, linkedList);
               
                LinkedList_destroyDeep(linkedList, new LinkedListValueDeleteFunction(FreeHGlobaleDeleteFunction));

                if (error != 0)
                {
                    if (mmsValue != IntPtr.Zero)
                        MmsValue_delete(mmsValue);

                    throw new IedConnectionException("ReadMultipleVariables failed", error);
                }

                return new MmsValue(mmsValue, true);
            }
        }

        /// <summary>
        /// Represents a variable in a log entry
        /// </summary>
        public class MmsJournalVariable
        {
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr MmsJournalVariable_getTag(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr MmsJournalVariable_getValue(IntPtr self);

            private IntPtr self;

            internal MmsJournalVariable(IntPtr self)
            {
                this.self = self;
            }

            public string GetTag()
            {
                return Marshal.PtrToStringAnsi(MmsJournalVariable_getTag(self));
            }

            public MmsValue GetValue()
            {
                MmsValue mmsValue = new MmsValue(MmsJournalVariable_getValue(self));

                return mmsValue;
            }

        }

        /// <summary>
        /// Represents an entry of a log
        /// </summary>
        public class MmsJournalEntry
        {
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void MmsJournalEntry_destroy(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr MmsJournalEntry_getEntryID(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr MmsJournalEntry_getOccurenceTime(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr MmsJournalEntry_getJournalVariables(IntPtr self);

            /****************
        	* LinkedList
         	***************/
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr LinkedList_getNext(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr LinkedList_getData(IntPtr self);

            private IntPtr self;
            private List<MmsJournalVariable> variables = null;

            internal MmsJournalEntry(IntPtr self)
            {
                this.self = self;
            }

            /// <summary>
            /// Gets the journal variables.
            /// </summary>
            /// <returns>The journal variables.</returns>
            public List<MmsJournalVariable> GetJournalVariables()
            {
                if (variables == null)
                {

                    IntPtr linkedList = MmsJournalEntry_getJournalVariables(self);

                    IntPtr element = LinkedList_getNext(linkedList);

                    variables = new List<MmsJournalVariable>();

                    while (element != IntPtr.Zero)
                    {
                        MmsJournalVariable journalVariable = new MmsJournalVariable(LinkedList_getData(element));

                        variables.Add(journalVariable);

                        element = LinkedList_getNext(element);
                    }
                }

                return variables;
            }

            /// <summary>
            /// Gets the entry identifier of the log entry
            /// </summary>
            /// <returns>The entry identifier.</returns>
            public byte[] GetEntryID()
            {
                IntPtr mmsValuePtr = MmsJournalEntry_getEntryID(self);

                MmsValue mmsValue = new MmsValue(mmsValuePtr);

                byte[] octetString = mmsValue.getOctetString();

                return octetString;
            }

            /// <summary>
            /// Gets the occurence time of the log entry
            /// </summary>
            /// <returns>The occurence time.</returns>
            public ulong GetOccurenceTime()
            {
                IntPtr mmsValuePtr = MmsJournalEntry_getOccurenceTime(self);

                MmsValue mmsValue = new MmsValue(mmsValuePtr);

                return mmsValue.GetBinaryTimeAsUtcMs();
            }

            /// <summary>
            /// Releases all resource used by the <see cref="T:IEC61850.Client.MmsJournalEntry"/> object.
            /// </summary>
            public void Dispose()
            {
                if (self != IntPtr.Zero)
                {
                    MmsJournalEntry_destroy(self);
                    self = IntPtr.Zero;
                }
            }

            ~MmsJournalEntry ()
            {
                Dispose();
            }
        }

        /// <summary>
        /// Asynchonous service handler for the get RCB values service
        /// </summary>
        /// <param name="invokeId">The invoke ID of the request triggering this callback</param>
        /// <param name="parameter">user provided callback parameter</param>
        /// <param name="err">Error code of response or timeout error in case of a response timeout</param>
        /// <param name="rcb">the report control block instance</param>
        public delegate void GetRCBValuesHandler(UInt32 invokeId,object parameter,IedClientError err,ReportControlBlock rcb);

        /// <summary>
        /// Asynchonous service handler for the set RCB values service
        /// </summary>
        /// <param name="invokeId">The invoke ID of the request triggering this callback</param>
        /// <param name="parameter">user provided callback parameter</param>
        /// <param name="err">Error code of response or timeout error in case of a response timeout</param>
        /// <param name="rcb">the report control block instance</param>
        public delegate void SetRCBValuesHandler(UInt32 invokeId,object parameter,IedClientError err,ReportControlBlock rcb);

        /// <summary>
        /// Generic asynchonous service handler - used by simple services that have only success or error result
        /// </summary>
        /// <param name="invokeId">The invoke ID of the request triggering this callback</param>
        /// <param name="parameter">user provided callback parameter</param>
        /// <param name="err">Error code of response or timeout error in case of a response timeout</param>
        public delegate void GenericServiceHandler(UInt32 invokeId,object parameter,IedClientError err);

        /// <summary>
        /// This class acts as the entry point for the IEC 61850 client API. It represents a single
        /// (MMS) connection to a server.
        /// </summary>
        public partial class IedConnection : IDisposable
        {
            /*************
            * MmsValue
            *************/
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr MmsValue_toString(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern float MmsValue_toFloat(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            [return: MarshalAs(UnmanagedType.I1)]
            static extern bool MmsValue_getBoolean(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern UInt32 MmsValue_getBitStringAsInteger(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern int MmsValue_getType(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void MmsValue_delete(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern int MmsValue_getDataAccessError(IntPtr self);

            /****************
            * IedConnection
            ***************/

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr IedConnection_create();

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr IedConnection_createWithTlsSupport(IntPtr tlsConfig);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedConnection_destroy(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern int Connection_getState(IedConnection self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedConnection_setConnectTimeout(IntPtr self, UInt32 timeoutInMs);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedConnection_setRequestTimeout(IntPtr self, UInt32 timeoutInMs);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern UInt32 IedConnection_getRequestTimeout(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedConnection_connect(IntPtr self, out int error, string hostname, int tcpPort);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedConnection_abort(IntPtr self, out int error);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            private static extern void IedConnection_release(IntPtr self, out int error);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            private static extern void IedConnection_close(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr IedConnection_readObject(IntPtr self, out int error, string objectReference, int fc);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedConnection_writeObject(IntPtr self, out int error, string dataAttributeReference, int fc, IntPtr value);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr IedConnection_getDataDirectory(IntPtr self, out int error, string dataReference);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr IedConnection_getDataDirectoryByFC(IntPtr self, out int error, string dataReference, FunctionalConstraint fc);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr IedConnection_getDataDirectoryFC(IntPtr self, out int error, string dataReference);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr IedConnection_getLogicalNodeDirectory(IntPtr self, out int error, string logicalNodeReference, int acsiClass);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr IedConnection_getServerDirectory(IntPtr self, out int error, [MarshalAs(UnmanagedType.I1)] bool getFileNames);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedConnection_getDeviceModelFromServer(IntPtr self, out int error);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr IedConnection_getLogicalDeviceDirectory(IntPtr self, out int error, string logicalDeviceName);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr IedConnection_getVariableSpecification(IntPtr self, out int error, string objectReference, int fc);

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
			private delegate void InternalConnectionClosedHandler(IntPtr parameter,IntPtr Iedconnection);

            /// <summary>
            /// Called when the connection is closed
            /// </summary>
			public delegate void ConnectionClosedHandler(IedConnection connection);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedConnection_installConnectionClosedHandler(IntPtr self, InternalConnectionClosedHandler handler, IntPtr parameter);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr IedConnection_readDataSetValues(IntPtr self, out int error, [MarshalAs(UnmanagedType.LPStr)] string dataSetReference, IntPtr dataSet);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedConnection_writeDataSetValues(IntPtr self, out int error, [MarshalAs(UnmanagedType.LPStr)] string dataSetReference, IntPtr values, out IntPtr accessResults);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr IedConnection_createDataSet(IntPtr self, out int error, [MarshalAs(UnmanagedType.LPStr)] string dataSetReference, IntPtr dataSet);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            [return: MarshalAs(UnmanagedType.Bool)]
            static extern bool IedConnection_deleteDataSet(IntPtr self, out int error, string dataSetReference);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr IedConnection_getDataSetDirectory(IntPtr self, out int error, string dataSetReference, [MarshalAs(UnmanagedType.I1)] out bool isDeletable);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr IedConnection_getMmsConnection(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr MmsConnection_getIsoConnectionParameters(IntPtr mmsConnection);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr IedConnection_getFileDirectory(IntPtr self, out int error, string directoryName);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr
            IedConnection_getFileDirectoryEx(IntPtr self, out int error, string directoryName, string continueAfter
                , [MarshalAs(UnmanagedType.I1)] out bool moreFollows);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedConnection_deleteFile(IntPtr self, out int error, string fileName);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr IedConnection_queryLogAfter(IntPtr self, out int error, string logReference,
                                                    IntPtr entryID, ulong timeStamp, [MarshalAs(UnmanagedType.I1)] out bool moreFollows);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr IedConnection_queryLogByTime(IntPtr self, out int error, string logReference,
                                                      ulong startTime, ulong endTime, [MarshalAs(UnmanagedType.I1)] out bool moreFollows);


            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr IedConnection_getRCBValues(IntPtr connection, out int error, string rcbReference, IntPtr updateRcb);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedConnection_setRCBValues(IntPtr connection, out int error, IntPtr rcb, UInt32 parametersMask, [MarshalAs(UnmanagedType.I1)] bool singleRequest);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedConnection_installReportHandler(IntPtr connection, string rcbReference, string rptId, InternalReportHandler handler,
                                                          IntPtr handlerParameter);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedConnection_uninstallReportHandler(IntPtr connection, string rcbReference);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern int IedConnection_getState(IntPtr connection);

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            private delegate void InternalStateChangedHandler(IntPtr parameter,IntPtr iedConnection,int newState);

            /// <summary>
            /// Called when there is a change in the connection state
            /// </summary>
            public delegate void StateChangedHandler(IedConnection connection, IedConnectionState newState);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedConnection_installStateChangedHandler(IntPtr connection, InternalStateChangedHandler handler, IntPtr parameter);

            /*********************
             * Async functions
             *********************/

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedConnection_connectAsync(IntPtr self, out int error, string hostname, int tcpPort);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedConnection_abortAsync(IntPtr self, out int error);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            private static extern void IedConnection_releaseAsync(IntPtr self, out int error);

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            private delegate void IedConnection_ReadObjectHandler(UInt32 invokeId,IntPtr parameter,int err,IntPtr value);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)] 
            static extern UInt32
            IedConnection_readObjectAsync(IntPtr self, out int error, string objRef, int fc,
                                          IedConnection_ReadObjectHandler handler, IntPtr parameter);

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            private delegate void IedConnection_WriteObjectHandler(UInt32 invokeId,IntPtr parameter,int err);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)] 
            static extern UInt32
            IedConnection_writeObjectAsync(IntPtr self, out int error, string objRef, int fc,
                                           IntPtr value, IedConnection_WriteObjectHandler handler, IntPtr parameter);

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            private delegate void IedConnection_GetNameListHandler(UInt32 invokeId,IntPtr parameter,int err,IntPtr nameList,[MarshalAs(UnmanagedType.I1)] bool  moreFollows);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)] 
            static extern UInt32
            IedConnection_getServerDirectoryAsync(IntPtr self, out int error, string continueAfter, IntPtr result,
                                                  IedConnection_GetNameListHandler handler, IntPtr parameter);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)] 
            static extern UInt32
            IedConnection_getLogicalDeviceVariablesAsync(IntPtr self, out int error, string ldName, string continueAfter, IntPtr result,
                                                         IedConnection_GetNameListHandler handler, IntPtr parameter);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)] 
            static extern UInt32
            IedConnection_getLogicalDeviceDataSetsAsync(IntPtr self, out int error, string ldName, string continueAfter, IntPtr result,
                                                        IedConnection_GetNameListHandler handler, IntPtr parameter);

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            private delegate void IedConnection_QueryLogHandler(UInt32 invokeId,IntPtr parameter,int err,IntPtr journalEntries,[MarshalAs(UnmanagedType.I1)] bool moreFollows);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)] 
            static extern UInt32
            IedConnection_queryLogByTimeAsync(IntPtr self, out int error, string logReference,
                                              UInt64 startTime, UInt64 endTime, IedConnection_QueryLogHandler handler, IntPtr parameter);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)] 
            static extern UInt32
            IedConnection_queryLogAfterAsync(IntPtr self, out int error, string logReference,
                                             IntPtr entryID, UInt64 timeStamp, IedConnection_QueryLogHandler handler, IntPtr parameter);

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            private delegate void IedConnection_GetVariableSpecificationHandler(UInt32 invokeId,IntPtr parameter,int err,IntPtr spec);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)] 
            static extern UInt32
            IedConnection_getVariableSpecificationAsync(IntPtr self, out int error, string dataAttributeReference,
                                                        int fc, IedConnection_GetVariableSpecificationHandler handler, IntPtr parameter);

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            private delegate void IedConnection_ReadDataSetHandler(UInt32 invokeId,IntPtr parameter,int err,IntPtr dataSet);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)] 
            static extern UInt32
            IedConnection_readDataSetValuesAsync(IntPtr self, out int error, string dataSetReference, IntPtr dataSet,
                                                 IedConnection_ReadDataSetHandler handler, IntPtr parameter);

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            private delegate void IedConnection_GetRCBValuesHandler(UInt32 invokeId,IntPtr parameter,int err,IntPtr rcb);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)] 
            static extern UInt32
            IedConnection_getRCBValuesAsync(IntPtr self, out int error, string rcbReference, IntPtr updateRcb,
                                            IedConnection_GetRCBValuesHandler handler, IntPtr parameter);

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            private delegate void IedConnection_GenericServiceHandler(UInt32 invokeId,IntPtr parameter,int err);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)] 
            static extern UInt32
            IedConnection_setRCBValuesAsync(IntPtr self, out int error, IntPtr rcb,
                                            UInt32 parametersMask, [MarshalAs(UnmanagedType.I1)] bool singleRequest, IedConnection_GenericServiceHandler handler, IntPtr parameter);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)] 
            static extern UInt32
            IedConnection_deleteFileAsync(IntPtr self, out int  error, string fileName,
                                          IedConnection_GenericServiceHandler handler, IntPtr parameter);
            

            /********************
            * FileDirectoryEntry
            *********************/
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void FileDirectoryEntry_destroy(IntPtr self);

            /****************
        	* LinkedList
         	***************/
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr LinkedList_getNext(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr LinkedList_getData(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void LinkedList_destroy(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void LinkedList_destroyStatic(IntPtr self);

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
			private delegate void LinkedListValueDeleteFunction(IntPtr pointer);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void LinkedList_destroyDeep(IntPtr list, LinkedListValueDeleteFunction valueDeleteFunction);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr LinkedList_create();

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void LinkedList_add(IntPtr self, IntPtr data);

            private IntPtr connection = IntPtr.Zero;
            private InternalConnectionClosedHandler connectionClosedHandler = null;
            private ConnectionClosedHandler userProvidedConnectionClosedHandler = null;

            private InternalStateChangedHandler internalStateChangedHandler = null;
            private StateChangedHandler stateChangedHandler = null;

            /// <summary>
            /// Initializes a new instance of the <see cref="IEC61850.Client.IedConnection"/> class.
            /// </summary>
            public IedConnection()
            {
                connection = IedConnection_create();
            }

            /// <summary>
            /// Initializes a new instance of the <see cref="IEC61850.Client.IedConnection"/> class.
            /// </summary>
            /// <param name="tlsConfig">TLS configuration to use</param>
            public IedConnection(TLSConfiguration tlsConfig)
            {
                connection = IedConnection_createWithTlsSupport(tlsConfig.GetNativeInstance());
            }

            /// <summary>
            /// Releases all resource used by the <see cref="IEC61850.Client.IedConnection"/> object.
            /// </summary>
            /// <remarks>Call <see cref="Dispose"/> when you are finished using the <see cref="IEC61850.Client.IedConnection"/>. The
            /// <see cref="Dispose"/> method leaves the <see cref="IEC61850.Client.IedConnection"/> in an unusable state. After
            /// calling <see cref="Dispose"/>, you must release all references to the <see cref="IEC61850.Client.IedConnection"/>
            /// so the garbage collector can reclaim the memory that the <see cref="IEC61850.Client.IedConnection"/> was occupying.</remarks>
            public void Dispose()
            {
                if (connection != IntPtr.Zero)
                {

                    IedConnection_destroy(connection);

                    connection = IntPtr.Zero;
                }
            }

            ~IedConnection ()
            {
                Dispose();
            }

            private IsoConnectionParameters isoConnectionParameters = null;

            /// <summary>
            /// Gets the connection parameters
            /// </summary>
            /// <returns>The connection parameters</returns>
            public IsoConnectionParameters GetConnectionParameters()
            {
                if (isoConnectionParameters == null)
                {
                    IntPtr mmsConnection = IedConnection_getMmsConnection(connection);

                    IntPtr parameters = MmsConnection_getIsoConnectionParameters(mmsConnection);

                    isoConnectionParameters = new IsoConnectionParameters(parameters);
                }

                return isoConnectionParameters;
            }

            private void FreeHGlobaleDeleteFunction(IntPtr pointer)
            {
                Marshal.FreeHGlobal(pointer);
            }


            private UInt32 connectTimeout = 10000;

            /// <summary>
            /// Gets or sets the timeout used for connection attempts.
            /// </summary>
            /// <value>The connect timeout in milliseconds</value>
            public UInt32 ConnectTimeout
            {
                get
                {
                    return connectTimeout;
                }
                set
                {
                    connectTimeout = value;
                }
            }

            /// <summary>
            /// Gets or sets the request timeout for this connection
            /// </summary>
            /// <value>The request timeout in milliseconds</value>
            public UInt32 RequestTimeout
            {
                get
                {
                    return IedConnection_getRequestTimeout(connection);
                }
                set
                {
                    IedConnection_setRequestTimeout(connection, value);
                }
            }

            /// <summary>
            /// Gets or sets the maximum size if a PDU (has to be set before calling connect!).
            /// </summary>
            /// <value>The maximum allowed size of an MMS PDU.</value>
            public int MaxPduSize
            {
                get
                {
                    return GetMmsConnection().GetLocalDetail();
                }
                set
                {
                    GetMmsConnection().SetLocalDetail(value);
                }
            }

            /// <summary>
            /// Gets the underlying MmsConnection instance.
            /// </summary>
            /// <returns>The mms connection.</returns>
            public MmsConnection GetMmsConnection()
            {
                IntPtr mmsConnectionPtr = IedConnection_getMmsConnection(connection);

                return new MmsConnection(mmsConnectionPtr);
            }

            /// <summary>Establish an MMS connection to a server</summary>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public void Connect(string hostname, int tcpPort)
            {
                int error;

                IedConnection_setConnectTimeout(connection, connectTimeout);

                IedConnection_connect(connection, out error, hostname, tcpPort);

                if (error != 0)
                    throw new IedConnectionException("Connect to " + hostname + ":" + tcpPort + " failed", error);
            }

            public void ConnectAsync(string hostname, int tcpPort)
            {
                int error;

                IedConnection_setConnectTimeout(connection, connectTimeout);

                IedConnection_connectAsync(connection, out error, hostname, tcpPort);

                if (error != 0)
                    throw new IedConnectionException("Connect to " + hostname + ":" + tcpPort + " failed", error);
                
            }

            /// <summary>
            /// Gets the current state of the connection
            /// </summary>
            /// <returns>The current connection state</returns>
            public IedConnectionState GetState()
            {
                return (IedConnectionState)IedConnection_getState(connection);
            }

            /// <summary>Establish an MMS connection to a server.</summary>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error.</exception>
            public void Connect(string hostname)
            {
                Connect(hostname, -1);
            }

            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public ControlObject CreateControlObject(string objectReference)
            {
                ControlObject controlObject = new ControlObject(objectReference, connection, this);

                return controlObject;
            }

            /// <summary>
            /// Creates a new SampledValuesControlBlock instance.
            /// </summary>
            /// <description>>
            /// This function will also read the SVCB values from the server.
            /// </description>
            /// <returns>The new SVCB instance</returns>
            /// <param name="svcbObjectReference">The object reference of the SVCB</param>
            public SampledValuesControlBlock GetSvControlBlock(string svcbObjectReference)
            {
                return new SampledValuesControlBlock(connection, svcbObjectReference);
            }

            /// <summary>
            /// Creates a new SampledValuesControlBlock instance.
            /// </summary>
            /// <returns>The new GoCB instance</returns>
            /// <param name="gocbObjectReference">The object reference of the GoCB (e.g. "simpleIOGenericIO/LLN0.gcbAnalogValues")</param>
            public GooseControlBlock GetGooseControlBlock(string gocbObjectReference)
            {
                return new GooseControlBlock(gocbObjectReference, connection);
            }

            /// <summary>
            /// Updates the device model by quering the server.
            /// </summary>
            public void UpdateDeviceModel()
            {
                int error;

                IedConnection_getDeviceModelFromServer(connection, out error);

                if (error != 0)
                    throw new IedConnectionException("UpdateDeviceModel failed", error);
            }


            /// <summary>
            /// Gets the server directory (Logical devices or file objects)
            /// </summary>
            /// <returns>List of logical devices or files</returns>
            /// <param name="fileDirectory">If set to <c>true</c> the file directory is returned, otherwise the LD names</param>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public List<string> GetServerDirectory(bool fileDirectory = false)
            {
                int error;

                IntPtr linkedList = IedConnection_getServerDirectory(connection, out error, fileDirectory);

                if (error != 0)
                    throw new IedConnectionException("GetDeviceDirectory failed", error);

                List<string> newList = new List<string>();

                if (fileDirectory == false)
                {

                    IntPtr element = LinkedList_getNext(linkedList);

                    while (element != IntPtr.Zero)
                    {
                        string ld = Marshal.PtrToStringAnsi(LinkedList_getData(element));

                        newList.Add(ld);

                        element = LinkedList_getNext(element);
                    }

                    LinkedList_destroy(linkedList);
                }
                else
                {

                    IntPtr element = LinkedList_getNext(linkedList);

                    while (element != IntPtr.Zero)
                    {
                        IntPtr elementData = LinkedList_getData(element);

                        FileDirectoryEntry entry = new FileDirectoryEntry(elementData);

                        newList.Add(entry.GetFileName());

                        FileDirectoryEntry_destroy(elementData);

                        element = LinkedList_getNext(element);
                    }

                    LinkedList_destroyStatic(linkedList);
                }

                return newList;
            }

            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public List<string> GetLogicalDeviceDirectory(string logicalDeviceName)
            {
                int error;

                IntPtr linkedList = IedConnection_getLogicalDeviceDirectory(connection, out error, logicalDeviceName);

                if (error != 0)
                    throw new IedConnectionException("GetLogicalDeviceDirectory failed", error);

                IntPtr element = LinkedList_getNext(linkedList);

                List<string> newList = new List<string>();

                while (element != IntPtr.Zero)
                {
                    string ln = Marshal.PtrToStringAnsi(LinkedList_getData(element));

                    newList.Add(ln);

                    element = LinkedList_getNext(element);
                }

                LinkedList_destroy(linkedList);

                return newList;
            }

            /// <summary>Get the directory of a logical node (LN)</summary>
            /// <description>This function returns the directory contents of a LN. Depending on the provided ACSI class
            /// The function returns either data object references, or references of other objects like data sets,
            /// report control blocks, ...</description>
            /// <param name="logicalNodeName">The object reference of a DO, SDO, or DA.</param>
            /// <param name="acsiClass">the ACSI class of the requested directory elements.</param>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public List<string> GetLogicalNodeDirectory(string logicalNodeName, ACSIClass acsiClass)
            {
                int error;

                IntPtr linkedList = IedConnection_getLogicalNodeDirectory(connection, out error, logicalNodeName, (int)acsiClass);

                if (error != 0)
                    throw new IedConnectionException("GetLogicalNodeDirectory failed", error);

                IntPtr element = LinkedList_getNext(linkedList);

                List<string> newList = new List<string>();

                while (element != IntPtr.Zero)
                {
                    string dataObject = Marshal.PtrToStringAnsi(LinkedList_getData(element));

                    newList.Add(dataObject);

                    element = LinkedList_getNext(element);
                }

                LinkedList_destroy(linkedList);

                return newList;
            }

            /// <summary>Get a list of attributes (with functional constraints) of a DO, SDO, or DA</summary>
            /// <param name="dataReference">The object reference of a DO, SDO, or DA.</param>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public List<string> GetDataDirectory(string dataReference)
            {
                int error;

                IntPtr linkedList = IedConnection_getDataDirectory(connection, out error, dataReference);

                if (error != 0)
                    throw new IedConnectionException("GetDataDirectory failed", error);

                IntPtr element = LinkedList_getNext(linkedList);

                List<string> newList = new List<string>();

                while (element != IntPtr.Zero)
                {
                    string dataObject = Marshal.PtrToStringAnsi(LinkedList_getData(element));

                    newList.Add(dataObject);

                    element = LinkedList_getNext(element);
                }

                LinkedList_destroy(linkedList);

                return newList;
            }

            /// <summary>Get the list of attributes with the specified FC of a DO, SDO, or DA</summary>
            /// <param name="dataReference">The object reference of a DO, SDO, or DA.</param>
            /// <param name="fc">Functional constraint</param>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public List<string> GetDataDirectory(string dataReference, FunctionalConstraint fc)
            {
                int error;

                IntPtr linkedList = IedConnection_getDataDirectoryByFC(connection, out error, dataReference, fc);

                if (error != 0)
                    throw new IedConnectionException("GetDataDirectory failed", error);

                IntPtr element = LinkedList_getNext(linkedList);

                List<string> newList = new List<string>();

                while (element != IntPtr.Zero)
                {
                    string dataObject = Marshal.PtrToStringAnsi(LinkedList_getData(element));

                    newList.Add(dataObject);

                    element = LinkedList_getNext(element);
                }

                LinkedList_destroy(linkedList);

                return newList;
            }

            /// <summary>Get a list of attributes (with functional constraints) of a DO, SDO, or DA</summary>
            /// <description>This function is similar to the GetDataDirectory except that the returned element names
            /// have the functional contraint (FC) appended.</description>
            /// <param name="dataReference">The object reference of a DO, SDO, or DA.</param>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public List<string> GetDataDirectoryFC(string dataReference)
            {
                int error;

                IntPtr linkedList = IedConnection_getDataDirectoryFC(connection, out error, dataReference);

                if (error != 0)
                    throw new IedConnectionException("GetDataDirectoryFC failed", error);

                IntPtr element = LinkedList_getNext(linkedList);

                List<string> newList = new List<string>();

                while (element != IntPtr.Zero)
                {
                    string dataObject = Marshal.PtrToStringAnsi(LinkedList_getData(element));

                    newList.Add(dataObject);

                    element = LinkedList_getNext(element);
                }

                LinkedList_destroy(linkedList);

                return newList;
            }

            private static List<MmsJournalEntry> WrapNativeLogQueryResult(IntPtr linkedList)
            {
                List<MmsJournalEntry> journalEntries = new List<MmsJournalEntry>();

                IntPtr element = LinkedList_getNext(linkedList);

                while (element != IntPtr.Zero)
                {

                    MmsJournalEntry journalEntry = new MmsJournalEntry(LinkedList_getData(element));

                    journalEntries.Add(journalEntry);

                    element = LinkedList_getNext(element);
                }

                LinkedList_destroyStatic(linkedList);

                return journalEntries;
            }

            /// <summary>
            /// Queries all log entries after the entry with the given entryID and timestamp
            /// </summary>
            /// <returns>The list of log entries contained in the response</returns>
            /// <param name="logRef">The object reference of the log (e.g. "simpleIOGenericIO/LLN0$EventLog")</param>
            /// <param name="entryID">EntryID of the last received MmsJournalEntry</param>
            /// <param name="timestamp">Timestamp of the last received MmsJournalEntry</param>
            /// <param name="moreFollows">Indicates that more log entries are available</param>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public List<MmsJournalEntry> QueryLogAfter(string logRef, byte[] entryID, ulong timestamp, out bool moreFollows)
            {
                int error;
                bool moreFollowsVal;

                MmsValue entryIdValue = new MmsValue(entryID);

                IntPtr linkedList = IedConnection_queryLogAfter(connection, out error, logRef, entryIdValue.valueReference, timestamp, out moreFollowsVal);

                if (error != 0)
                    throw new IedConnectionException("QueryLogAfter failed", error);

                moreFollows = moreFollowsVal;

                return WrapNativeLogQueryResult(linkedList);
            }

            /// <summary>
            /// Queries all log entries of the given time range
            /// </summary>
            /// <returns>The list of log entries contained in the response</returns>
            /// <param name="logRef">The object reference of the log (e.g. "simpleIOGenericIO/LLN0$EventLog")</param>
            /// <param name="startTime">Start time of the time range</param>
            /// <param name="stopTime">End time of the time range</param>
            /// <param name="moreFollows">Indicates that more log entries are available</param>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public List<MmsJournalEntry> QueryLogByTime(string logRef, ulong startTime, ulong stopTime, out bool moreFollows)
            {
                int error;
                bool moreFollowsVal;

                IntPtr linkedList = IedConnection_queryLogByTime(connection, out error, logRef, startTime, stopTime, out moreFollowsVal);

                if (error != 0)
                    throw new IedConnectionException("QueryLogByTime failed", error);

                moreFollows = moreFollowsVal;

                return WrapNativeLogQueryResult(linkedList);

            }

            /// <summary>
            /// Queries all log entries of the given time range
            /// </summary>
            /// <returns>The list of log entries contained in the response</returns>
            /// <param name="logRef">The object reference of the log (e.g. "simpleIOGenericIO/LLN0$EventLog")</param>
            /// <param name="startTime">Start time of the time range</param>
            /// <param name="stopTime">End time of the time range</param>
            /// <param name="moreFollows">Indicates that more log entries are available</param>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public List<MmsJournalEntry> QueryLogByTime(string logRef, DateTime startTime, DateTime stopTime, out bool moreFollows)
            {
                ulong startTimeMs = LibIEC61850.DateTimeToMsTimestamp(startTime);
                ulong stopTimeMs = LibIEC61850.DateTimeToMsTimestamp(stopTime);

                return QueryLogByTime(logRef, startTimeMs, stopTimeMs, out moreFollows);
            }

            /// <summary>Read the variable specification (type description of a DA or FCDO</summary>
            /// <param name="objectReference">The object reference of a DA or FCDO.</param>
            /// <param name="fc">The functional constraint (FC) of the object</param>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public MmsVariableSpecification GetVariableSpecification(string objectReference, FunctionalConstraint fc)
            {
                int error;

                IntPtr varSpecPtr = IedConnection_getVariableSpecification(connection, out error, objectReference, (int)fc);

                if (error != 0)
                    throw new IedConnectionException("GetVariableSpecification failed", error);

                return new MmsVariableSpecification(varSpecPtr, true);
            }

            private IntPtr readObjectInternal(string objectReference, FunctionalConstraint fc)
            {
                int error;

                IntPtr mmsValue = IedConnection_readObject(connection, out error, objectReference, (int)fc); 

                if (error != 0)
                    throw new IedConnectionException("Reading value failed", error);

                if (mmsValue == IntPtr.Zero)
                    throw new IedConnectionException("Variable not found on server", error);

                return mmsValue;
            }

            /// <summary>Read the value of a data attribute (DA) or functional constraint data object (FCDO).</summary>
            /// <param name="objectReference">The object reference of a DA or FCDO.</param>
            /// <param name="fc">The functional constraint (FC) of the object</param>
            /// <returns>the received value as an MmsValue instance</returns>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public MmsValue ReadValue(String objectReference, FunctionalConstraint fc)
            {
                var value = readObjectInternal(objectReference, fc);

                return new MmsValue(value, true);
            }

            private IntPtr readObjectInternalAndCheckDataAccessError(string objectReference, FunctionalConstraint fc)
            {
                IntPtr mmsValue = readObjectInternal(objectReference, fc);

                if (MmsValue_getType(mmsValue) == (int)MmsType.MMS_DATA_ACCESS_ERROR)
                {

                    int dataAccessError = MmsValue_getDataAccessError(mmsValue);

                    MmsValue_delete(mmsValue);

                    throw new IedConnectionException("Data access error", dataAccessError);
                }

                return mmsValue;
            }

            /// <summary>Read the value of a basic data attribute (BDA) of type boolean.</summary>
            /// <param name="objectReference">The object reference of a BDA.</param>
            /// <param name="fc">The functional constraint (FC) of the object</param>
            /// <returns>the received boolean value</returns>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public bool ReadBooleanValue(string objectReference, FunctionalConstraint fc)
            {
                IntPtr mmsValue = readObjectInternalAndCheckDataAccessError(objectReference, fc);

                if (MmsValue_getType(mmsValue) != (int)MmsType.MMS_BOOLEAN)
                {
                    MmsValue_delete(mmsValue);
                    throw new IedConnectionException("Result is not of type boolean (MMS_BOOLEAN)", 0);
                }

                bool value = MmsValue_getBoolean(mmsValue);

                MmsValue_delete(mmsValue);

                return value;
            }

            /// <summary>Read the value of a basic data attribute (BDA) of type float.</summary>
            /// <param name="objectReference">The object reference of a BDA.</param>
            /// <param name="fc">The functional constraint (FC) of the object</param>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public float ReadFloatValue(string objectReference, FunctionalConstraint fc)
            {
                IntPtr mmsValue = readObjectInternalAndCheckDataAccessError(objectReference, fc);

                if (MmsValue_getType(mmsValue) != (int)MmsType.MMS_FLOAT)
                {
                    MmsValue_delete(mmsValue);
                    throw new IedConnectionException("Result is not of type float (MMS_FLOAT)", 0);
                }

                float value = MmsValue_toFloat(mmsValue);

                MmsValue_delete(mmsValue);

                return value;
            }

            /// <summary>Read the value of a basic data attribute (BDA) of type string (VisibleString or MmsString).</summary>
            /// <param name="objectReference">The object reference of a BDA.</param>
            /// <param name="fc">The functional constraint (FC) of the object</param>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public string ReadStringValue(string objectReference, FunctionalConstraint fc)
            {
                IntPtr mmsValue = readObjectInternalAndCheckDataAccessError(objectReference, fc);

                if (!((MmsValue_getType(mmsValue) == (int)MmsType.MMS_VISIBLE_STRING) || (MmsValue_getType(mmsValue) == (int)MmsType.MMS_STRING)))
                {
                    MmsValue_delete(mmsValue);
                    throw new IedConnectionException("Result is not of type string", 0);
                }

                IntPtr ptr = MmsValue_toString(mmsValue);

                string returnString = Marshal.PtrToStringAnsi(ptr);

                MmsValue_delete(mmsValue);

                return returnString;
            }

            /// <summary>Read the value of a basic data attribute (BDA) of type quality.</summary>
            /// <param name="objectReference">The object reference of a BDA.</param>
            /// <param name="fc">The functional constraint (FC) of the object</param>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public Quality ReadQualityValue(string objectReference, FunctionalConstraint fc)
            {
                IntPtr mmsValue = readObjectInternalAndCheckDataAccessError(objectReference, fc);

                if (MmsValue_getType(mmsValue) == (int)MmsType.MMS_BIT_STRING)
                {
                    int bitStringValue = (int)MmsValue_getBitStringAsInteger(mmsValue);

                    MmsValue_delete(mmsValue);
                    return new Quality(bitStringValue);
                }
                else
                {
                    MmsValue_delete(mmsValue);
                    throw new IedConnectionException("Result is not of type bit string(Quality)", 0);
                }
            }

            /// <summary>Read the value of a basic data attribute (BDA) of type bit string.</summary>
            /// <param name="objectReference">The object reference of a BDA.</param>
            /// <param name="fc">The functional constraint (FC) of the object</param>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public int ReadBitStringValue(string objectReference, FunctionalConstraint fc)
            {
                IntPtr mmsValue = readObjectInternalAndCheckDataAccessError(objectReference, fc);

                if (MmsValue_getType(mmsValue) == (int)MmsType.MMS_BIT_STRING)
                {
                    int bitStringValue = (int)MmsValue_getBitStringAsInteger(mmsValue);

                    MmsValue_delete(mmsValue);
                    return bitStringValue;
                }
                else
                {
                    MmsValue_delete(mmsValue);
                    throw new IedConnectionException("Result is not of type bit string", 0);
                }
            }

            /// <summary>Read the value of a basic data attribute (BDA) of type timestamp (MMS_UTC_TIME).</summary>
            /// <param name="objectReference">The object reference of a BDA.</param>
            /// <param name="fc">The functional constraint (FC) of the object</param>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public Timestamp ReadTimestampValue(string objectReference, FunctionalConstraint fc)
            {
                var mmsValuePtr = readObjectInternalAndCheckDataAccessError(objectReference, fc);

                var mmsValue = new MmsValue(mmsValuePtr, true);

                if (mmsValue.GetType() == MmsType.MMS_UTC_TIME)
                    return new Timestamp(mmsValue);
                else
                    throw new IedConnectionException("Result is not of type timestamp (MMS_UTC_TIME)", 0);
            }

            /// <summary>Read the value of a basic data attribute (BDA) of type integer (MMS_INTEGER).</summary>
            /// <description>This function should also be used if enumerations are beeing read. Because
            /// enumerations are mapped to integer types for the MMS mapping</description>
            /// <param name="objectReference">The object reference of a BDA.</param>
            /// <param name="fc">The functional constraint (FC) of the object</param>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public Int64 ReadIntegerValue(string objectReference, FunctionalConstraint fc)
            {
                var mmsValuePtr = readObjectInternalAndCheckDataAccessError(objectReference, fc);

                var mmsValue = new MmsValue(mmsValuePtr, true);

                if (mmsValue.GetType() == MmsType.MMS_INTEGER)
                    return mmsValue.ToInt64();
                else
                    throw new IedConnectionException("Result is not of type integer (MMS_INTEGER)", 0);
            }

            /// <summary>Write the value of a data attribute (DA) or functional constraint data object (FCDO).</summary>
            /// <description>This function can be used to write simple or complex variables (setpoints, parameters, descriptive values...)
            /// of the server.</description>
            /// <param name="objectReference">The object reference of a BDA.</param>
            /// <param name="fc">The functional constraint (FC) of the object</param>
            /// <param name="value">MmsValue object representing asimple or complex variable data</param>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public void WriteValue(string objectReference, FunctionalConstraint fc, MmsValue value)
            {
                int error;

                IedConnection_writeObject(connection, out error, objectReference, (int)fc, value.valueReference);

                if (error != 0)
                    throw new IedConnectionException("Write value failed", error);
            }

            /// <summary>Delete file</summary>
            /// <param name="fileName">The name of the file.</param>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public void DeleteFile(string fileName)
            {
                int error;
                IedConnection_deleteFile(connection, out error, fileName);

                if (error != 0)
                    throw new IedConnectionException("Deleting file " + fileName + " failed", error);
            }

            private void nativeGenericServiceHandler(UInt32 invokeId, IntPtr parameter, int err)
            {
                GCHandle handle = GCHandle.FromIntPtr(parameter);

                Tuple<GenericServiceHandler, object> callbackInfo = handle.Target as Tuple<GenericServiceHandler, object>;

                GenericServiceHandler handler = callbackInfo.Item1;
                object handlerParameter = callbackInfo.Item2;

                handle.Free();

                IedClientError clientError = (IedClientError)err;
                         
                handler(invokeId, handlerParameter, clientError);
            }

            /// <summary>Delete file- asynchronous version</summary>
            /// <param name="fileName">The name of the file.</param>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public UInt32 DeleteFileAsync(string filename, GenericServiceHandler handler, object parameter)
            {
                int error;

                Tuple<GenericServiceHandler, object> callbackInfo = Tuple.Create(handler, parameter);

                GCHandle handle = GCHandle.Alloc(callbackInfo);

                UInt32 invokeId = IedConnection_deleteFileAsync(connection, out error, filename, nativeGenericServiceHandler, GCHandle.ToIntPtr(handle));

                if (error != 0)
                {
                    handle.Free();
                    throw new IedConnectionException("Get file failed", error);
                }

                return invokeId;
            }

            /// <summary>Read the content of a file directory.</summary>
            /// <param name="directoryName">The name of the directory.</param>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public List<FileDirectoryEntry> GetFileDirectory(string directoryName)
            {
                int error;

                IntPtr fileEntryList = IedConnection_getFileDirectory(connection, out error, directoryName);

                if (error != 0)
                    throw new IedConnectionException("Reading file directory failed", error);

                List<FileDirectoryEntry> fileDirectory = new List<FileDirectoryEntry>();

                IntPtr element = LinkedList_getNext(fileEntryList);

                while (element != IntPtr.Zero)
                {
                    IntPtr elementData = LinkedList_getData(element);

                    FileDirectoryEntry entry = new FileDirectoryEntry(elementData);

                    fileDirectory.Add(entry);

                    FileDirectoryEntry_destroy(elementData);

                    element = LinkedList_getNext(element);
                }

                LinkedList_destroyStatic(fileEntryList);

                return fileDirectory;
            }

            /// <summary>Read the content of a file directory. - single request version</summary>
            /// <param name="directoryName">The name of the directory.</param>
            /// <param name="continueAfter">the filename that defines the continuation point, or null for the first request</param>
            /// <param name="moreFollows">true, when more files are available, false otherwise</param>
            /// <returns>list of file directory entries</returns>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public List<FileDirectoryEntry> GetFileDirectoryEx(string directoryName, string continueAfter, out bool moreFollows)
            {
                int error;

                IntPtr fileEntryList = IedConnection_getFileDirectoryEx(connection, out error, directoryName, continueAfter, out moreFollows);

                if (error != 0)
                    throw new IedConnectionException("Reading file directory failed", error);

                List<FileDirectoryEntry> fileDirectory = new List<FileDirectoryEntry>();

                IntPtr element = LinkedList_getNext(fileEntryList);

                while (element != IntPtr.Zero)
                {
                    IntPtr elementData = LinkedList_getData(element);

                    FileDirectoryEntry entry = new FileDirectoryEntry(elementData);

                    fileDirectory.Add(entry);

                    FileDirectoryEntry_destroy(elementData);

                    element = LinkedList_getNext(element);
                }

                LinkedList_destroyStatic(fileEntryList);

                return fileDirectory;
            }

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            [return: MarshalAs(UnmanagedType.I1)]
            private delegate bool InternalIedClientGetFileHandler(IntPtr parameter,IntPtr buffer,UInt32 bytesRead);

            private bool iedClientGetFileHandler(IntPtr parameter, IntPtr buffer, UInt32 bytesRead)
            {
                GCHandle handle = GCHandle.FromIntPtr(parameter);

                GetFileCallback getFileCallback = (GetFileCallback)handle.Target;

                byte[] bytes = new byte[bytesRead];

                Marshal.Copy(buffer, bytes, 0, (int)bytesRead);

                return getFileCallback.handler(getFileCallback.parameter, bytes);
            }

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern UInt32 IedConnection_getFile(IntPtr self, out int error, string fileName, InternalIedClientGetFileHandler handler,
                                                       IntPtr handlerParameter);


            public delegate bool GetFileHandler(object parameter,byte[] data);

            private class GetFileCallback
            {
                public GetFileCallback(GetFileHandler handler, object parameter)
                {
                    this.handler = handler;
                    this.parameter = parameter;
                }

                public GetFileHandler handler;
                public object parameter;
            }

            /// <summary>
            /// Download a file from the server.
            /// </summary>
            /// <param name='fileName'>
            /// File name of the file (full path)
            /// </param>
            /// <param name='handler'>
            /// Callback handler that is invoked for each chunk of the file received
            /// </param>
            /// <param name='parameter'>
            /// User provided parameter that is passed to the callback handler
            /// </param>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public void GetFile(string fileName, GetFileHandler handler, object parameter)
            {
                int error;

                GetFileCallback getFileCallback = new GetFileCallback(handler, parameter);

                GCHandle handle = GCHandle.Alloc(getFileCallback);

                IedConnection_getFile(connection, out error, fileName, new InternalIedClientGetFileHandler(iedClientGetFileHandler), 
                    GCHandle.ToIntPtr(handle));

                if (error != 0)
                    throw new IedConnectionException("Error reading file", error);

                handle.Free();
            }

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            [return: MarshalAs(UnmanagedType.I1)]
            private delegate bool IedConnection_GetFileAsyncHandler(UInt32 invokeId,IntPtr parameter,int err,UInt32 originalInvokeId,
                IntPtr buffer,UInt32 bytesRead,bool moreFollows);

            /// <summary>
            /// Callback handler for the asynchronous get file service. Will be invoked for each chunk of received data
            /// </summary>
            /// <param name="invokeId">The invoke ID of the reqeust triggering this callback</param>
            /// <param name="parameter">user provided callback parameter</param>
            /// <param name="err">Error code of response or timeout error in case of a response timeout</param>
            /// <param name="originalInvokeId">the invokeId of the first (file open) request</param>
            /// <param name="buffer">the file data received with the last response, or null if no file data available</param>
            /// <param name="moreFollows">indicates that more file data follows</param>
            public delegate bool GetFileAsyncHandler(UInt32 invokeId,object parameter,IedClientError err,UInt32 originalInvokeId,byte[] buffer,bool moreFollows);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)] 
            static extern UInt32
            IedConnection_getFileAsync(IntPtr self, out int error, string fileName, IedConnection_GetFileAsyncHandler handler,
                                       IntPtr parameter);

            private bool nativeGetFileAsyncHandler(UInt32 invokeId, IntPtr parameter, int err, UInt32 originalInvokeId,
                                                   IntPtr buffer, UInt32 bytesRead, bool moreFollows)
            {

                GCHandle handle = GCHandle.FromIntPtr(parameter);

                Tuple<GetFileAsyncHandler, object> callbackInfo = handle.Target as Tuple<GetFileAsyncHandler, object>;

                GetFileAsyncHandler handler = callbackInfo.Item1;
                object handlerParameter = callbackInfo.Item2;

                handle.Free();

                IedClientError clientError = (IedClientError)err;

                byte[] bytes = null;
               
                if (clientError == IedClientError.IED_ERROR_OK)
                {
                    bytes = new byte[bytesRead];

                    Marshal.Copy(buffer, bytes, 0, (int)bytesRead);
                }               
                    
                return handler(invokeId, handlerParameter, clientError, originalInvokeId, bytes, moreFollows);
            }

            /// <summary>
            /// Download a file from the server.
            /// </summary>
            /// <param name='fileName'>
            /// File name of the file (full path)
            /// </param>
            /// <param name='handler'>
            /// Callback handler that is invoked for each chunk of the file received
            /// </param>
            /// <param name='parameter'>
            /// User provided parameter that is passed to the callback handler
            /// </param>
            /// <returns>invoke ID of the request</returns>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public UInt32 GetFileAsync(string fileName, GetFileAsyncHandler handler, object parameter)
            {
                int error;

                Tuple<GetFileAsyncHandler, object> callbackInfo = Tuple.Create(handler, parameter);

                GCHandle handle = GCHandle.Alloc(callbackInfo);

                UInt32 invokeId = IedConnection_getFileAsync(connection, out error, fileName, nativeGetFileAsyncHandler, GCHandle.ToIntPtr(handle));

                if (error != 0)
                {
                    handle.Free();
                    throw new IedConnectionException("Get file failed", error);
                }

                return invokeId;
            }




            /// <summary>
            /// Abort (close) the connection.
            /// </summary>
            /// <description>This function will send an abort request to the server. This will immediately interrupt the
            /// connection.</description>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public void Abort()
            {
                int error;

                IedConnection_abort(connection, out error);

                if (error != 0)
                    throw new IedConnectionException("Abort failed", error);
            }

            /// <summary>
            /// Abort (close) the connection - asynchronous version
            /// </summary>
            /// <description>This function will send an abort request to the server. This will immediately interrupt the
            /// connection.</description>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public void AbortAsync()
            {
                int error;

                IedConnection_abortAsync(connection, out error);

                if (error != 0)
                    throw new IedConnectionException("Abort failed", error);
            }

            /// <summary>
            /// Release (close) the connection.
            /// </summary>
            /// <description>This function will send an release request to the server. The function will block until the
            /// connection is released or an error occured.</description>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public void Release()
            {
                int error;

                IedConnection_release(connection, out error);

                if (error != 0)
                    throw new IedConnectionException("Release failed", error);
            }

            /// <summary>
            /// Release (close) the connection - asynchronous version
            /// </summary>
            /// <description>This function will send an release request to the server. The function will block until the
            /// connection is released or an error occured.</description>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public void ReleaseAsync()
            {
                int error;

                IedConnection_releaseAsync(connection, out error);

                if (error != 0)
                    throw new IedConnectionException("Release failed", error);
            }

            /// <summary>
            /// Immediately close the connection.
            /// </summary>
            /// <description>This function will close the connnection to the server by closing the TCP connection.
            /// The client will not send an abort or release request as required by the specification!</description>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public void Close()
            {
                IedConnection_close(connection);
            }

            private void MyConnectionClosedHandler(IntPtr parameter, IntPtr self)
            {
                if (userProvidedConnectionClosedHandler != null)
                    userProvidedConnectionClosedHandler(this);
            }

            /// <summary>
            /// Install a callback handler that will be invoked if the connection is closed.
            /// </summary>
            /// <description>The handler is called when the connection is closed no matter if the connection was closed
            /// by the client or by the server. Any new call to this function will replace the callback handler installed
            /// by a prior function call.</description>
            /// <param name="handler">The user provided callback handler</param>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            [Obsolete("ConnectionClosedHandler is deprecated, please use StateChangedHandler instead")]
            public void InstallConnectionClosedHandler(ConnectionClosedHandler handler)
            {
                if (connectionClosedHandler == null)
                {
                    connectionClosedHandler = new InternalConnectionClosedHandler(MyConnectionClosedHandler);
                    IedConnection_installConnectionClosedHandler(connection, connectionClosedHandler, connection);
                }

                userProvidedConnectionClosedHandler = handler;	
            }

            private void MyStateChangedHandler(IntPtr parameter, IntPtr IedConnection, int newState)
            {
                if (stateChangedHandler != null)
                    stateChangedHandler(this, (IedConnectionState)newState);
            }

            /// <summary>
            /// Sets the handler for StateChanged events
            /// </summary>
            /// <param name="handler">The state changed event handler</param>
            public void InstallStateChangedHandler(StateChangedHandler handler)
            {
                stateChangedHandler = handler;

                if (internalStateChangedHandler == null)
                {
                    internalStateChangedHandler = new InternalStateChangedHandler(MyStateChangedHandler);
                    IedConnection_installStateChangedHandler(connection, internalStateChangedHandler, IntPtr.Zero);
                }
            }

            /// <summary>
            /// Sets the handler for StateChanged events
            /// </summary>
            /// <value>The state changed event handler</value>
            public StateChangedHandler StateChanged
            {
                set
                {
                    InstallStateChangedHandler(value);
                }
            }

            /// <summary>
            /// Read the values of a data set (GetDataSetValues service).
            /// </summary>
            /// <description>This function will invoke a readDataSetValues service and return a new DataSet value containing the
            /// received values.</description>
            /// <param name="dataSetReference">The object reference of the data set</param>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public DataSet GetDataSetValues(string dataSetReference)
            {
                return ReadDataSetValues(dataSetReference, null);
            }

            /// <summary>
            /// Read the values of a data set (GetDataSetValues service).
            /// </summary>
            /// <description>This function will invoke a readDataSetValues service and return a new DataSet value containing the
            /// received values. If an existing instance of DataSet is provided to the function the existing instance will be
            /// updated by the new values.</description>
            /// <param name="dataSetReference">The object reference of the data set</param>
            /// <param name="dataSet">The object reference of an existing data set instance or null</param>
            /// <returns>a DataSet instance containing the received values</returns>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public DataSet ReadDataSetValues(string dataSetReference, DataSet dataSet)
            {
                IntPtr nativeClientDataSet = IntPtr.Zero;

                if (dataSet != null)
                    nativeClientDataSet = dataSet.getNativeInstance();
     
                int error;

                nativeClientDataSet = IedConnection_readDataSetValues(connection, out error, dataSetReference, nativeClientDataSet);
                    
                if (error != 0)
                    throw new IedConnectionException("Reading data set failed", error);

                if (dataSet == null)
                    dataSet = new DataSet(nativeClientDataSet);

                return dataSet;
            }

            /// <summary>
            /// Writes the values of a data set (SetDataSetValues service).
            /// </summary>
            /// <returns>The list of access results</returns>
            /// <param name="dataSetReference">The object reference of the data set</param>
            /// <param name="values">The new values for the data set members. The values have to be of the same number and type as the data set members</param>
            public List<MmsDataAccessError> WriteDataSetValues(string dataSetReference, List<MmsValue> values)
            {
                int error;
                IntPtr accessResults = IntPtr.Zero;

                IntPtr valueList = LinkedList_create();

                foreach (MmsValue mmsValue in values)
                {
                    LinkedList_add(valueList, mmsValue.valueReference);
                }

                IedConnection_writeDataSetValues(connection, out error, dataSetReference, valueList, out accessResults);

                LinkedList_destroyStatic(valueList);

                /* handle access results */

                List<MmsDataAccessError> accessResultList = null;

                if (accessResults != IntPtr.Zero)
                {

                    IntPtr element = LinkedList_getNext(accessResults);

                    while (element != IntPtr.Zero)
                    {
                        IntPtr elementData = LinkedList_getData(element);

                        MmsValue accessResultValue = new MmsValue(elementData, true);

                        MmsDataAccessError dataAccessError = accessResultValue.GetDataAccessError();

                        accessResultList.Add(dataAccessError);

                        element = LinkedList_getNext(element);
                    }

                    LinkedList_destroyStatic(accessResults);
                }

                if (error != 0)
                    throw new IedConnectionException("Writing data set failed", error);

                return accessResultList;
            }

            /// <summary>
            /// Create a new data set.
            /// </summary>
            /// <description>This function creates a new data set at the server. The data set consists of the members defined
            /// by the list of object references provided.</description>
            /// <param name="dataSetReference">The object reference of the data set</param>
            /// <param name="dataSetElements">A list of object references of the data set elements</param>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public void CreateDataSet(string dataSetReference, List<string> dataSetElements)
            {
                IntPtr linkedList = LinkedList_create();

                foreach (string dataSetElement in dataSetElements)
                {
                    IntPtr handle = System.Runtime.InteropServices.Marshal.StringToHGlobalAnsi(dataSetElement);

                    LinkedList_add(linkedList, handle);
                }

                int error;

                IedConnection_createDataSet(connection, out error, dataSetReference, linkedList);

                LinkedList_destroyDeep(linkedList, new LinkedListValueDeleteFunction(FreeHGlobaleDeleteFunction));

                if (error != 0)
                    throw new IedConnectionException("Failed to create data set", error);

            }

            /// <summary>
            /// Delete a data set.
            /// </summary>
            /// <description>This function will delete a data set at the server. This function may fail if the data set is not
            /// deletable.</description>
            /// <param name="dataSetReference">The object reference of the data set</param>
            /// <returns>true if data set has been deleted, false otherwise</returns>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public bool DeleteDataSet(string dataSetReference)
            {
                int error;

                bool isDeleted = IedConnection_deleteDataSet(connection, out error, dataSetReference);

                if (error != 0)
                    throw new IedConnectionException("Failed to delete data set", error);

                return isDeleted;
            }

            /// <summary>
            /// Get the directory of the data set.
            /// </summary>
            /// <description>This function returns a list of object references with appended functional constraints (FC) of the data set elemenents.</description>
            /// <param name="dataSetReference">The object reference of the data set</param>
            /// <returns>the list of object references</returns>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public List<string> GetDataSetDirectory(string dataSetReference)
            {
                bool isDeletable;

                return GetDataSetDirectory(dataSetReference, out isDeletable);
            }

            /// <summary>
            /// Get the directory of the data set.
            /// </summary>
            /// <description>This function returns a list of object references with appended functional constraints (FC) of the data set elemenents.</description>
            /// <param name="dataSetReference">The object reference of the data set</param>
            /// <param name="isDeletable">Indication if this data set is permanent or deletable.</param>
            /// <returns>the list of object references</returns>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public List<string> GetDataSetDirectory(string dataSetReference, out bool isDeletable)
            {
                int error;

                IntPtr linkedList = IedConnection_getDataSetDirectory(connection, out error, dataSetReference, out isDeletable);

                if (error != 0)
                    throw new IedConnectionException("getDataSetDirectory failed", error);

                IntPtr element = LinkedList_getNext(linkedList);

                List<string> newList = new List<string>();

                while (element != IntPtr.Zero)
                {
                    string dataObject = Marshal.PtrToStringAnsi(LinkedList_getData(element));

                    newList.Add(dataObject);

                    element = LinkedList_getNext(element);
                }

                LinkedList_destroy(linkedList);

                return newList;
            }


            /// <summary>
            /// Read object handler.
            /// </summary>
            /// <param name="invokeId">The invoke ID of the reqeust triggering this callback</param>
            /// <param name="parameter">user provided callback parameter</param>
            /// <param name="err">Error code of response or timeout error in case of a response timeout</param>
            /// <param name="value">The read result value or null in case of an error</param>
            public delegate void ReadValueHandler(UInt32 invokeId,object parameter,IedClientError err,MmsValue value);

            private void nativeReadObjectHandler(UInt32 invokeId, IntPtr parameter, int err, IntPtr value)
            {
                GCHandle handle = GCHandle.FromIntPtr(parameter);

                Tuple<ReadValueHandler, object> callbackInfo = handle.Target as Tuple<ReadValueHandler, object>;

                ReadValueHandler handler = callbackInfo.Item1;
                object handlerParameter = callbackInfo.Item2;

                handle.Free();

                IedClientError clientError = (IedClientError)err;

                MmsValue mmsValue = null;

                if (value != IntPtr.Zero)
                {
                    mmsValue = new MmsValue(value, true);
                }

                handler(invokeId, handlerParameter, clientError, mmsValue);
            }

            /// <summary>Asynchronously read the value of a data attribute (DA) or functional constraint data object (FCDO) - GetData service</summary>
            /// <param name="objectReference">The object reference of a DA or FCDO.</param>
            /// <param name="fc">The functional constraint (FC) of the object</param>
            /// <param name="handler">Callback function to handle the received response or service timeout</param>
            /// <param name="parameter">User provided callback parameter. Will be passed to the callback function</param>
            /// <returns>the invoke ID of the sent request</returns>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public UInt32 ReadValueAsync(string objectReference, FunctionalConstraint fc, ReadValueHandler handler, object parameter)
            {
                int error;

                Tuple<ReadValueHandler, object> callbackInfo = Tuple.Create(handler, parameter);

                GCHandle handle = GCHandle.Alloc(callbackInfo);

                UInt32 invokeId = IedConnection_readObjectAsync(connection, out error, objectReference, (int)fc, nativeReadObjectHandler, GCHandle.ToIntPtr(handle));

                if (error != 0)
                {
                    handle.Free();
                    throw new IedConnectionException("Reading value failed", error);
                }

                return invokeId;
            }

            private void nativeGetVariableSpecifcationHandler(UInt32 invokeId, IntPtr parameter, int err, IntPtr spec)
            {
                GCHandle handle = GCHandle.FromIntPtr(parameter);

                Tuple<GetVariableSpecifcationHandler, object> callbackInfo = handle.Target as Tuple<GetVariableSpecifcationHandler, object>;

                GetVariableSpecifcationHandler handler = callbackInfo.Item1;
                object handlerParameter = callbackInfo.Item2;

                handle.Free();

                IedClientError clientError = (IedClientError)err;

                MmsVariableSpecification varSpec = null;

                if (spec != IntPtr.Zero)
                    varSpec = new MmsVariableSpecification(spec, true);

                handler(invokeId, handlerParameter, clientError, varSpec);
            }

            public delegate void GetVariableSpecifcationHandler(UInt32 invokeId,object parameter,IedClientError err,MmsVariableSpecification spec);

            /// <summary>Read the variable specification (type description of a DA or FCDO</summary>
            /// <param name="objectReference">The object reference of a DA or FCDO.</param>
            /// <param name="fc">The functional constraint (FC) of the object</param>
            /// <param name="handler">Callback function to handle the received response or service timeout</param>
            /// <param name="parameter">User provided callback parameter. Will be passed to the callback function</param>
            /// <returns>the invoke ID of the sent request</returns>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public UInt32 GetVariableSpecificationAsync(string objectReference, FunctionalConstraint fc, GetVariableSpecifcationHandler handler, object parameter)
            {
                int error;

                Tuple<GetVariableSpecifcationHandler, object> callbackInfo = Tuple.Create(handler, parameter);

                GCHandle handle = GCHandle.Alloc(callbackInfo);

                UInt32 invokeId = IedConnection_getVariableSpecificationAsync(connection, out error, objectReference, (int)fc, nativeGetVariableSpecifcationHandler, GCHandle.ToIntPtr(handle)); 

                if (error != 0)
                {
                    handle.Free();
                    throw new IedConnectionException("Reading variable specification failed", error);
                }

                return invokeId;
            }

            private void nativeReadDataSetHandler(UInt32 invokeId, IntPtr parameter, int err, IntPtr nativeDataSet)
            {
                GCHandle handle = GCHandle.FromIntPtr(parameter);

                Tuple<ReadDataSetHandler, object, DataSet> callbackInfo = handle.Target as Tuple<ReadDataSetHandler, object, DataSet>;

                ReadDataSetHandler handler = callbackInfo.Item1;
                object handlerParameter = callbackInfo.Item2;
                DataSet dataSet = callbackInfo.Item3;

                handle.Free();

                IedClientError clientError = (IedClientError)err;

                if (nativeDataSet != IntPtr.Zero)
                {
                    if (dataSet == null)
                        dataSet = new DataSet(nativeDataSet);
                }
                

                handler(invokeId, handlerParameter, clientError, dataSet);
            }


            public delegate void ReadDataSetHandler(UInt32 invokeId,object parameter,IedClientError err,DataSet dataSet);

            /// <summary>
            /// Read the values of a data set (GetDataSetValues service) - asynchronous version
            /// </summary>
            /// <description>This function will invoke a readDataSetValues service and in case of success returns a new DataSet value 
            /// containing the received values by the callback function. If an existing instance of DataSet is provided to the 
            ///  function the existing instance will be updated by the new values.
            /// </description>
            /// <param name="dataSetReference">The object reference of the data set</param>
            /// <param name="dataSet">The object reference of an existing data set instance or null</param>
            /// <param name="handler">Callback function to handle the received response or service timeout</param>
            /// <param name="parameter">User provided callback parameter. Will be passed to the callback function</param>
            /// <returns>the invoke ID of the sent request</returns>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public UInt32 ReadDataSetValuesAsync(string dataSetReference, DataSet dataSet, ReadDataSetHandler handler, object parameter)
            {
                int error;

                Tuple<ReadDataSetHandler, object, DataSet> callbackInfo = Tuple.Create(handler, parameter, dataSet);

                GCHandle handle = GCHandle.Alloc(callbackInfo);

                IntPtr dataSetPtr = IntPtr.Zero;

                if (dataSet != null)
                    dataSetPtr = dataSet.getNativeInstance();

                UInt32 invokeId = IedConnection_readDataSetValuesAsync(connection, out error, dataSetReference, dataSetPtr, nativeReadDataSetHandler, GCHandle.ToIntPtr(handle)); 

                if (error != 0)
                {
                    handle.Free();
                    throw new IedConnectionException("Reading data set failed", error);
                }

                return invokeId;
            }

            /// <summary>
            /// Write value handler.
            /// </summary>
            /// <param name="invokeId">The invoke ID of the reqeust triggering this callback</param>
            /// <param name="parameter">user provided callback parameter</param>
            /// <param name="err">Error code of response or timeout error in case of a response timeout</param>
            public delegate void WriteValueHandler(UInt32 invokeId,object parameter,IedClientError err);

            private void nativeWriteObjectHandler(UInt32 invokeId, IntPtr parameter, int err)
            {
                GCHandle handle = GCHandle.FromIntPtr(parameter);

                Tuple<WriteValueHandler, object> callbackInfo = handle.Target as Tuple<WriteValueHandler, object>;

                WriteValueHandler handler = callbackInfo.Item1;
                object handlerParameter = callbackInfo.Item2;

                handle.Free();

                IedClientError clientError = (IedClientError)err;

                handler(invokeId, handlerParameter, clientError);
            }


            public UInt32 WriteValueAsync(string objectReference, FunctionalConstraint fc, MmsValue value, WriteValueHandler handler, object parameter)
            {
                int error;

                Tuple<WriteValueHandler, object> callbackInfo = Tuple.Create(handler, parameter);

                GCHandle handle = GCHandle.Alloc(callbackInfo);

                UInt32 invokeId = IedConnection_writeObjectAsync(connection, out error, objectReference, (int)fc, value.valueReference, nativeWriteObjectHandler, GCHandle.ToIntPtr(handle));

                if (error != 0)
                {
                    handle.Free();
                    throw new IedConnectionException("Writing value failed", error);
                }

                return invokeId;
            }

            public delegate void GetNameListHandler(UInt32 invokeId,object parameter,IedClientError err,List<string> nameList,bool moreFollows);

            private void nativeGetNameListHandler(UInt32 invokeId, IntPtr parameter, int err, IntPtr nameList, [MarshalAs(UnmanagedType.I1)] bool  moreFollows)
            {
                GCHandle handle = GCHandle.FromIntPtr(parameter);

                Tuple<GetNameListHandler, object, List<string>> callbackInfo = handle.Target as Tuple<GetNameListHandler, object, List<string>>;

                GetNameListHandler handler = callbackInfo.Item1;
                object handlerParameter = callbackInfo.Item2;
                List<string> resultList = callbackInfo.Item3;

                handle.Free();

                IedClientError clientError = (IedClientError)err;

                if (nameList != IntPtr.Zero)
                {
                    IntPtr element = LinkedList_getNext(nameList);

                    if (resultList == null)
                        resultList = new List<string>();

                    while (element != IntPtr.Zero)
                    {
                        string ld = Marshal.PtrToStringAnsi(LinkedList_getData(element));

                        resultList.Add(ld);

                        element = LinkedList_getNext(element);
                    }

                    LinkedList_destroy(nameList);

                    handler(invokeId, handlerParameter, clientError, resultList, moreFollows);
                }
                else
                {
                    handler(invokeId, handlerParameter, clientError, null, moreFollows);
                }

            }

            /// <summary>
            /// Gets the server directory (Logical devices or file objects)
            /// </summary>
            /// <param name="result">list where to store the result or null to create a new list for the result</param>
            /// <param name="continueAfter">continuation value (last received name)</param>
            /// <param name="handler">user provided callback function</param>
            /// <param name="parameter">user provided callback parameter</param>
            /// <returns>the invoke ID of the sent request</returns>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public UInt32 GetServerDirectoryAsync(List<string> result, string continueAfter, GetNameListHandler handler, object parameter)
            {
                int error;

                Tuple<GetNameListHandler, object, List<string>> callbackInfo = Tuple.Create(handler, parameter, result);

                GCHandle handle = GCHandle.Alloc(callbackInfo);

                UInt32 invokeId = IedConnection_getServerDirectoryAsync(connection, out error, continueAfter, IntPtr.Zero, nativeGetNameListHandler, GCHandle.ToIntPtr(handle));

                if (error != 0)
                {
                    handle.Free();
                    throw new IedConnectionException("Get server directory failed", error);
                }

                return invokeId;
            }

            public UInt32 GetLogicalDeviceVariablesAsync(string ldName, string continueAfter, GetNameListHandler handler, object parameter)
            {
                return GetLogicalDeviceVariablesAsync(null, ldName, continueAfter, handler, parameter);
            }

            public UInt32 GetLogicalDeviceVariablesAsync(List<string> result, string ldName, string continueAfter, GetNameListHandler handler, object parameter)
            {
                int error;

                Tuple<GetNameListHandler, object, List<string>> callbackInfo = Tuple.Create(handler, parameter, result);

                GCHandle handle = GCHandle.Alloc(callbackInfo);

                UInt32 invokeId = IedConnection_getLogicalDeviceVariablesAsync(connection, out error, ldName, continueAfter, IntPtr.Zero, nativeGetNameListHandler, GCHandle.ToIntPtr(handle));

                if (error != 0)
                {
                    handle.Free();
                    throw new IedConnectionException("Get logical device variables failed", error);
                }

                return invokeId;
            }

            public UInt32 GetLogicalDeviceDataSetsAsync(string ldName, string continueAfter, GetNameListHandler handler, object parameter)
            {
                return GetLogicalDeviceDataSetsAsync(null, ldName, continueAfter, handler, parameter);
            }


            public UInt32 GetLogicalDeviceDataSetsAsync(List<string> result, string ldName, string continueAfter, GetNameListHandler handler, object parameter)
            {
                int error;

                Tuple<GetNameListHandler, object, List<string>> callbackInfo = Tuple.Create(handler, parameter, result);

                GCHandle handle = GCHandle.Alloc(callbackInfo);

                UInt32 invokeId = IedConnection_getLogicalDeviceDataSetsAsync(connection, out error, ldName, continueAfter, IntPtr.Zero, nativeGetNameListHandler, GCHandle.ToIntPtr(handle));

                if (error != 0)
                {
                    handle.Free();
                    throw new IedConnectionException("Get logical device data sets failed", error);
                }

                return invokeId;
            }

            public delegate void QueryLogHandler(UInt32 invokeId,object parameter,IedClientError err,List<MmsJournalEntry> journalEntries,bool moreFollows);

            private void nativeQueryLogHandler(UInt32 invokeId, IntPtr parameter, int err, IntPtr journalEntries, 
                                                [MarshalAs(UnmanagedType.I1)] bool moreFollows)
            {
                GCHandle handle = GCHandle.FromIntPtr(parameter);

                Tuple<QueryLogHandler, object> callbackInfo = handle.Target as Tuple<QueryLogHandler, object>;

                QueryLogHandler handler = callbackInfo.Item1;
                object handlerParameter = callbackInfo.Item2;

                handle.Free();

                IedClientError clientError = (IedClientError)err;

                var logResult = WrapNativeLogQueryResult(journalEntries);

                handler(invokeId, handlerParameter, clientError, logResult, moreFollows);
            }

            /// <summary>
            /// Queries all log entries of the given time range (asynchronous version)
            /// </summary>
            /// <returns>The list of log entries contained in the response</returns>
            /// <param name="logRef">The object reference of the log (e.g. "simpleIOGenericIO/LLN0$EventLog")</param>
            /// <param name="startTime">Start time of the time range</param>
            /// <param name="stopTime">End time of the time range</param>
            /// <param name="handler">user provided callback function</param>
            /// <param name="parameter">user provided callback parameter</param>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public UInt32 QueryLogByTimeAsync(string logRef, ulong startTime, ulong stopTime, QueryLogHandler handler, object parameter)
            {
                int error;

                Tuple<QueryLogHandler, object> callbackInfo = Tuple.Create(handler, parameter);

                GCHandle handle = GCHandle.Alloc(callbackInfo);

                UInt32 invokeId = IedConnection_queryLogByTimeAsync(connection, out error, logRef, startTime, stopTime, nativeQueryLogHandler, GCHandle.ToIntPtr(handle));

                if (error != 0)
                {
                    handle.Free();
                    throw new IedConnectionException("QueryLogByTime failed", error);
                }

                return invokeId;
            }

            /// <summary>
            /// Queries all log entries of the given time range (asynchronous version)
            /// </summary>
            /// <returns>The list of log entries contained in the response</returns>
            /// <param name="logRef">The object reference of the log (e.g. "simpleIOGenericIO/LLN0$EventLog")</param>
            /// <param name="startTime">Start time of the time range</param>
            /// <param name="stopTime">End time of the time range</param>
            /// <param name="handler">user provided callback function</param>
            /// <param name="parameter">user provided callback parameter</param>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public UInt32 QueryLogByTimeAsync(string logRef, DateTime startTime, DateTime stopTime, QueryLogHandler handler, object parameter)
            {
                ulong startTimeMs = LibIEC61850.DateTimeToMsTimestamp(startTime);
                ulong stopTimeMs = LibIEC61850.DateTimeToMsTimestamp(stopTime);

                return QueryLogByTimeAsync(logRef, startTimeMs, stopTimeMs, handler, parameter);
            }

            /// <summary>
            /// Queries all log entries after the entry with the given entryID and timestamp (asynchronous version)
            /// </summary>
            /// <returns>The list of log entries contained in the response</returns>
            /// <param name="logRef">The object reference of the log (e.g. "simpleIOGenericIO/LLN0$EventLog")</param>
            /// <param name="entryID">EntryID of the last received MmsJournalEntry</param>
            /// <param name="handler">user provided callback function</param>
            /// <param name="parameter">user provided callback parameter</param>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            public UInt32 QueryLogAfterAsync(string logRef, byte[] entryID, ulong timestamp, QueryLogHandler handler, object parameter)
            {
                int error;

                Tuple<QueryLogHandler, object> callbackInfo = Tuple.Create(handler, parameter);

                GCHandle handle = GCHandle.Alloc(callbackInfo);

                MmsValue entryIdValue = new MmsValue(entryID);

                UInt32 invokeId = IedConnection_queryLogAfterAsync(connection, out error, logRef, entryIdValue.valueReference, timestamp, nativeQueryLogHandler, GCHandle.ToIntPtr(handle));

                if (error != 0)
                {
                    handle.Free();
                    throw new IedConnectionException("QueryLogAfter failed", error);
                }

                return invokeId;
            }

            internal void UninstallReportHandler(string objectReference)
            {
                if (connection != IntPtr.Zero)
                {
                    IedConnection_uninstallReportHandler(connection, objectReference);
                }
            }

            internal void InstallReportHandler(string objectReference, string reportId, InternalReportHandler internalHandler)
            {
                if (connection != IntPtr.Zero)
                {
                    IedConnection_installReportHandler(connection, objectReference, reportId, internalHandler, IntPtr.Zero);
                }
            }

            internal void GetRCBValues(out int error, string objectReference, IntPtr updateRcb)
            {
                if (connection != IntPtr.Zero)
                {
                    IedConnection_getRCBValues(connection, out error, objectReference, updateRcb);
                }
                else
                {
                    error = 1; /* not connected */
                }
            }

            internal void SetRCBValues(out int error, IntPtr rcb, UInt32 parametersMask, bool singleRequest)
            {
                if (connection != IntPtr.Zero)
                {
                    IedConnection_setRCBValues(connection, out error, rcb, parametersMask, singleRequest);
                }
                else
                {
                    error = 1; /* not connected */
                }
            }


            private void nativeGetRCBValuesHandler(UInt32 invokeId, IntPtr parameter, int err, IntPtr rcbPtr)
            {
                GCHandle handle = GCHandle.FromIntPtr(parameter);

                Tuple<GetRCBValuesHandler, object, ReportControlBlock> callbackInfo = handle.Target as Tuple<GetRCBValuesHandler, object, ReportControlBlock>;

                GetRCBValuesHandler handler = callbackInfo.Item1;
                object handlerParameter = callbackInfo.Item2;
                ReportControlBlock rcb = callbackInfo.Item3;

                handle.Free();

                IedClientError clientError = (IedClientError)err;

                handler(invokeId, handlerParameter, clientError, rcb);
            }

            internal UInt32 GetRCBValuesAsync(string objectReference, ReportControlBlock updateRcb, GetRCBValuesHandler handler, object parameter)
            {
                int error;

                Tuple<GetRCBValuesHandler, object, ReportControlBlock> callbackInfo = Tuple.Create(handler, parameter, updateRcb);

                GCHandle handle = GCHandle.Alloc(callbackInfo);

                UInt32 invokeId = IedConnection_getRCBValuesAsync(connection, out error, objectReference, updateRcb.self, nativeGetRCBValuesHandler, GCHandle.ToIntPtr(handle));

                if (error != 0)
                {
                    handle.Free();
                    throw new IedConnectionException("GetRCBValues failed", error);
                }

                return invokeId;
            }

            private void nativeSetRcbValuesHandler(UInt32 invokeId, IntPtr parameter, int err)
            {
                GCHandle handle = GCHandle.FromIntPtr(parameter);

                Tuple<SetRCBValuesHandler, object, ReportControlBlock> callbackInfo = handle.Target as Tuple<SetRCBValuesHandler, object, ReportControlBlock>;

                SetRCBValuesHandler handler = callbackInfo.Item1;
                object handlerParameter = callbackInfo.Item2;
                ReportControlBlock rcb = callbackInfo.Item3;

                handle.Free();

                IedClientError clientError = (IedClientError)err;

                handler(invokeId, handlerParameter, clientError, rcb);
            }

            internal UInt32 SetRCBValuesAsync(ReportControlBlock rcb, UInt32 parametersMask, bool singleRequest, SetRCBValuesHandler handler, object parameter)
            {
                int error;

                Tuple<SetRCBValuesHandler, object, ReportControlBlock> callbackInfo = Tuple.Create(handler, parameter, rcb);

                GCHandle handle = GCHandle.Alloc(callbackInfo);

                UInt32 invokeId = IedConnection_setRCBValuesAsync(connection, out error, rcb.self, parametersMask, singleRequest, nativeSetRcbValuesHandler, GCHandle.ToIntPtr(handle));

                if (error != 0)
                {
                    handle.Free();
                    throw new IedConnectionException("SetRCBValues failed", error);
                }

                return invokeId;
            }

        }

        public class IedConnectionException : Exception
        {

            private int errorCode;

            public IedConnectionException(string message, int errorCode)
                : base(message)
            {
                this.errorCode = errorCode;
            }

            public IedConnectionException(string message)
                : base(message)
            {
                this.errorCode = 0;
            }

            public int GetErrorCode()
            {
                return this.errorCode;
            }

            public IedClientError GetIedClientError()
            {
                return (IedClientError)this.errorCode;
            }
        }

        public class FileDirectoryEntry
        {
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            private static extern IntPtr FileDirectoryEntry_getFileName(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            private static extern UInt32 FileDirectoryEntry_getFileSize(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            private static extern UInt64 FileDirectoryEntry_getLastModified(IntPtr self);

            private string fileName;
            private UInt32 fileSize;
            private UInt64 lastModified;

            internal FileDirectoryEntry(IntPtr nativeFileDirectoryEntry)
            {
                fileName = Marshal.PtrToStringAnsi(FileDirectoryEntry_getFileName(nativeFileDirectoryEntry));
                fileSize = FileDirectoryEntry_getFileSize(nativeFileDirectoryEntry);
                lastModified = FileDirectoryEntry_getLastModified(nativeFileDirectoryEntry);
            }

            public string GetFileName()
            {
                return fileName;
            }

            public UInt32 GetFileSize()
            {
                return fileSize;
            }

            public UInt64 GetLastModified()
            {
                return lastModified;
            }
        }

        /// <summary>
        /// Connection state of an IedConnection instance
        /// </summary>
        public enum IedConnectionState
        {
            /// <summary>
            /// The connection is closed. Requests cannot be sent.
            /// </summary>
            IED_STATE_CLOSED = 0,

            /// <summary>
            /// The connection is connecting. Requests cannot be sent yet.
            /// </summary>
            IED_STATE_CONNECTING = 1,

            /// <summary>
            /// The connection is up. Requests can be sent.
            /// </summary>
            IED_STATE_CONNECTED = 2,

            /// <summary>
            /// The connection is closing. Requests connect be sent.
            /// </summary>
            IED_STATE_CLOSING = 3
        }

        /// <summary>
        /// Error codes for client side functions
        /// </summary>
        public enum IedClientError
        {
            /* general errors */

            /** No error occurred - service request has been successful */
            IED_ERROR_OK = 0,

            /** The service request can not be executed because the client is not yet connected */
            IED_ERROR_NOT_CONNECTED = 1,

            /** Connect service not execute because the client is already connected */
            IED_ERROR_ALREADY_CONNECTED = 2,

            /** The service request can not be executed caused by a loss of connection */
            IED_ERROR_CONNECTION_LOST = 3,

            /** The service or some given parameters are not supported by the client stack or by the server */
            IED_ERROR_SERVICE_NOT_SUPPORTED = 4,

            /** Connection rejected by server */
            IED_ERROR_CONNECTION_REJECTED = 5,

            /* client side errors */

            /** API function has been called with an invalid argument */
            IED_ERROR_USER_PROVIDED_INVALID_ARGUMENT = 10,

            IED_ERROR_ENABLE_REPORT_FAILED_DATASET_MISMATCH = 11,

            /** The object provided object reference is invalid (there is a syntactical error). */
            IED_ERROR_OBJECT_REFERENCE_INVALID = 12,

            /** Received object is of unexpected type */
            IED_ERROR_UNEXPECTED_VALUE_RECEIVED = 13,

            /* service error - error reported by server */

            /** The communication to the server failed with a timeout */
            IED_ERROR_TIMEOUT = 20,

            /** The server rejected the access to the requested object/service due to access control */
            IED_ERROR_ACCESS_DENIED = 21,

            /** The server reported that the requested object does not exist */
            IED_ERROR_OBJECT_DOES_NOT_EXIST = 22,

            /** The server reported that the requested object already exists */
            IED_ERROR_OBJECT_EXISTS = 23,

            /** The server does not support the requested access method */
            IED_ERROR_OBJECT_ACCESS_UNSUPPORTED = 24,

            /** The server expected an object of another type */
            IED_ERROR_TYPE_INCONSISTENT = 25,

            /** The object or service is temporarily unavailable */
            IED_ERROR_TEMPORARILY_UNAVAILABLE = 26,

            /** The specified object is not defined in the server (returned by server) */
            IED_ERROR_OBJECT_UNDEFINED = 27,

            /** The specified address is invalid (returned by server) */
            IED_ERROR_INVALID_ADDRESS = 28,

            /** Service failed due to a hardware fault (returned by server) */
            IED_ERROR_HARDWARE_FAULT = 29,

            /** The requested data type is not supported by the server (returned by server) */
            IED_ERROR_TYPE_UNSUPPORTED = 30,

            /** The provided attributes are inconsistent (returned by server) */
            IED_ERROR_OBJECT_ATTRIBUTE_INCONSISTENT = 31,

            /** The provided object value is invalid (returned by server) */
            IED_ERROR_OBJECT_VALUE_INVALID = 32,

            /** The object is invalidated (returned by server) */
            IED_ERROR_OBJECT_INVALIDATED = 33,

            /** Received an invalid response message from the server */
            IED_ERROR_MALFORMED_MESSAGE = 34,

            /** Service not implemented */
            IED_ERROR_SERVICE_NOT_IMPLEMENTED = 98,

            /* unknown error */
            IED_ERROR_UNKNOWN = 99
        }
    }
}
