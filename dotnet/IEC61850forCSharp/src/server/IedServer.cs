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
        /// This class acts as the entry point for the IEC 61850 client API. It represents a single
        /// (MMS) connection to a server.
        /// </summary>
        public class IedServer : IDisposable
        {
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr IedServer_createWithConfig(IntPtr modelRef, IntPtr tlsConfiguration, IntPtr serverConfiguratio);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_setLocalIpAddress(IntPtr self, string localIpAddress);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_setTLSConfiguration(IntPtr self, IntPtr tlsConfiguration);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_start(IntPtr self, int tcpPort);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_stop(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_destroy(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            [return: MarshalAs(UnmanagedType.Bool)]
            static extern bool IedServer_isRunning(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern int IedServer_getNumberOfOpenConnections(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_lockDataModel(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_unlockDataModel(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_updateAttributeValue(IntPtr self, IntPtr DataAttribute, IntPtr MmsValue);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_updateBooleanAttributeValue(IntPtr self, IntPtr dataAttribute, [MarshalAs(UnmanagedType.I1)] bool value);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_updateInt32AttributeValue(IntPtr self, IntPtr dataAttribute, int value);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_updateInt64AttributeValue(IntPtr self, IntPtr dataAttribute, Int64 value);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_updateFloatAttributeValue(IntPtr self, IntPtr dataAttribute, float value);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_updateVisibleStringAttributeValue(IntPtr self, IntPtr dataAttribute, string value);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_updateBitStringAttributeValue(IntPtr self, IntPtr dataAttribute, UInt32 value);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_updateUnsignedAttributeValue(IntPtr self, IntPtr dataAttribute, UInt32 value);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_updateDbposValue(IntPtr self, IntPtr dataAttribute, Dbpos value);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_updateUTCTimeAttributeValue(IntPtr self, IntPtr dataAttribute, ulong value);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_updateTimestampAttributeValue(IntPtr self, IntPtr dataAttribute, IntPtr timestamp);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_updateQuality(IntPtr self, IntPtr dataAttribute, ushort value);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_setServerIdentity(IntPtr self, string vendor, string model, string revision);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr IedServer_getAttributeValue(IntPtr self, IntPtr dataAttribute);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr IedServer_getFunctionalConstrainedData(IntPtr self, IntPtr dataObject, int fc);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_setListObjectsAccessHandler(IntPtr self, IedServer_ListObjectsAccessHandler handler, IntPtr parameter);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_setRequestTimeout(IntPtr self, Int32 timeoutMs);

            /// <summary>
            /// Set a handler to control read and write access to control blocks and logs
            /// </summary>
            /// <param name="self">IedServer</param>
            /// <param name="handler">handler the callback handler to be used</param>
            /// <param name="parameter">a user provided parameter that is passed to the handler</param>
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_setControlBlockAccessHandler(IntPtr self, IedServer_ControlBlockAccessHandler handler, IntPtr parameter);

            /// <summary>
            /// Install the global read access handler
            /// The read access handler will be called for every read access before the server grants access to the client
            /// </summary>
            /// <param name="self">IedServer</param>
            /// <param name="handler">the callback function that is invoked if a client tries to read a data object</param>
            /// <param name="parameter">a user provided parameter that is passed to the callback function</param>
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_setReadAccessHandler(IntPtr self, IedServer_ReadAccessHandler handler, IntPtr parameter);

            /// <summary>
            /// Set a handler to control access to a dataset (create, delete, read, write, list directory)
            /// </summary>
            /// <param name="self">IedServer</param>
            /// <param name="handler">the callback handler to be used</param>
            /// <param name="parameter">a user provided parameter that is passed to the handler</param>
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_setDataSetAccessHandler(IntPtr self, IedServer_DataSetAccessHandler handler, IntPtr parameter);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_setDirectoryAccessHandler(IntPtr self, IedServer_DirectoryAccessHandler handler, IntPtr parameter);

            /// <summary>
            ///  Set the callback handler for the SetActSG event
            /// </summary>
            /// <param name="self">the instance of IedServer to operate on</param>
            /// <param name="sgcb">the handle of the setting group control block of the setting group</param>
            /// <param name="handler">the user provided callback handler</param>
            /// <param name="parameter">a user provided parameter that is passed to the control handler</param>
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_setActiveSettingGroupChangedHandler(IntPtr self, IntPtr sgcb, IedServer_ActiveSettingGroupChangedHandler handler, IntPtr parameter);

            /// <summary>
            ///  Get the active setting group number
            /// </summary>
            /// <param name="self">the instance of IedServer to operate on</param>
            /// <param name="sgcb">the handle of the setting group control block of the setting group</param>
            /// <returns>the number of the active setting group</returns>
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern uint IedServer_getActiveSettingGroup(IntPtr self, IntPtr sgcb);

            /// <summary>
            /// Set the callback handler for the SetEditSG event
            /// </summary>
            /// <param name="self">the instance of IedServer to operate on</param>
            /// <param name="sgcb">the handle of the setting group control block of the setting group</param>
            /// <param name="handler">the user provided callback handler</param>
            /// <param name="parameter">a user provided parameter that is passed to the control handler</param>
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_setEditSettingGroupChangedHandler(IntPtr self, IntPtr sgcb, IedServer_EditSettingGroupChangedHandler handler, IntPtr parameter);

            /// <summary>
            /// Set the callback handler for the COnfEditSG event
            /// </summary>
            /// <param name="self">the instance of IedServer to operate on</param>
            /// <param name="sgcb">the handle of the setting group control block of the setting group</param>
            /// <param name="handler">the user provided callback handler</param>
            /// <param name="parameter">a user provided parameter that is passed to the control handler</param>
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_setEditSettingGroupConfirmationHandler(IntPtr self, IntPtr sgcb, IedServer_EditSettingGroupConfirmationHandler handler, IntPtr parameter);

            ///// <summary>
            ///// Set a handler for SVCB control block events (enable/disable)
            ///// </summary>
            ///// <param name="self">the instance of IedServer to operate on.</param>
            ///// <param name="svcb">the SVCB control block instance</param>
            ///// <param name="handler">the event handler to be used</param>
            ///// <param name="parameter">user provided parameter that is passed to the handler</param>
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_setSVCBHandler(IntPtr self, IntPtr svcb, IedServer_SVCBEventHandler handler, IntPtr parameter);

            ///// <summary>
            ///// callback handler for SVCB events
            ///// </summary>
            ///// <param name="svcb">the related SVCB instance</param>
            ///// <param name="eventType">event type</param>
            ///// <param name="parameter">user defined parameter</param>
            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            private delegate void IedServer_SVCBEventHandler(IntPtr svcb, int eventType, IntPtr parameter);

            /// <summary>
            /// set the authenticator for this server
            /// This function sets a user specified authenticator that is used to identify and authenticate clients that
            /// wants to connect.The authenticator is called on each connection attempt.Depending on the return value
            /// connections are accepted.
            /// </summary>
            /// <param name="self">the instance of IedServer to operate on.</param>
            /// <param name="authenticator">the user provided authenticator callback</param>
            /// <param name="authenticatorParameter">user provided parameter that is passed to the authenticator</param>
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_setAuthenticator(IntPtr self, IedServer_AcseAuthenticator authenticator, IntPtr authenticatorParameter);

            /// <summary>
            /// Callback function to authenticate a client
            /// </summary>
            /// <param name="parameter">user provided parameter - set when user registers the authenticator</param>
            /// <param name="authParameter">the authentication parameters provided by the client</param>
            /// <param name="securityToken">pointer where to store an application specific security token - can be ignored if not used.</param>
            /// <param name="appReference">ISO application reference (ap-title + ae-qualifier)</param>
            /// <returns>true if client connection is accepted, false otherwise</returns>
            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            private delegate bool IedServer_AcseAuthenticator(IntPtr parameter, IntPtr authParameter, IntPtr securityToken, IntPtr appReference);

            /// <summary>
            /// callback handler to control client read access to data attributes
            ///  User provided callback function to control MMS client read access to IEC 61850
            ///  data objects.The application is to allow read access to data objects for specific clients only.
            ///  It can be used to implement a role based access control (RBAC).
            /// </summary>
            /// <param name="ld">the logical device the client wants to access</param>
            /// <param name="ln">the logical node the client wants to access</param>
            /// <param name="dataObject">the data object the client wants to access</param>
            /// <param name="fc">the functional constraint of the access</param>
            /// <param name="connection">the client connection that causes the access</param>
            /// <param name="parameter">the user provided parameter</param>
            /// <returns>DATA_ACCESS_ERROR_SUCCESS if access is accepted, DATA_ACCESS_ERROR_OBJECT_ACCESS_DENIED if access is denied</returns>
            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            private delegate MmsDataAccessError IedServer_ReadAccessHandler(IntPtr ld, IntPtr ln, IntPtr dataObject, int fc, IntPtr connection, IntPtr parameter);

            /// <summary>
            /// Callback handler that is invoked when the active setting group is about to be changed by an external client.
            /// This function is called BEFORE the active setting group is changed. The user can reject to change the active setting group by returning false.
            /// </summary>
            /// <param name="parameter">user provided parameter</param>
            /// <param name="sgcb">sgcb the setting group control block of the setting group that is about to be changed</param>
            /// <param name="newActSg">newActSg the new active setting group</param>
            /// <param name="connection">connection the client connection that requests the change</param>
            /// <returns>true if the change is accepted, false otherwise</returns>
            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            private delegate bool IedServer_ActiveSettingGroupChangedHandler(IntPtr parameter, IntPtr sgcb, uint newActSg, IntPtr connection);

            /// <summary>
            /// Callback handler that is invoked when the edit setting group is about to be changed by an external client.
            /// In this function the user should update all SE data attributes associated with the given SettingGroupControlBlock.
            /// This function is called BEFORE the active setting group is changed.The user can reject to change the
            /// edit setting group by returning false. This can be used to implement RBAC.
            /// </summary>
            /// <param name="parameter">user provided parameter</param>
            /// <param name="sgcb">the setting group control block of the setting group that is about to be changed</param>
            /// <param name="newEditSg">the new edit setting group</param>
            /// <param name="connection">the client connection that requests the change</param>
            /// <returns>true if the change is accepted, false otherwise</returns>
            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            private delegate bool IedServer_EditSettingGroupChangedHandler(IntPtr parameter, IntPtr sgcb, uint newEditSg, IntPtr connection);

            /// <summary>
            /// Callback handler that is invoked when the edit setting group has been confirmed by an  external client.
            /// </summary>
            /// <param name="parameter">user provided parameter</param>
            /// <param name="sgcb">the setting group control block of the setting group that is about to be changed</param>
            /// <param name="editSg">the edit setting group that has been confirmed</param>
            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            private delegate void IedServer_EditSettingGroupConfirmationHandler(IntPtr parameter, IntPtr sgcb, uint editSg);

            /// <summary>
            /// Callback that is called when the client is calling a dataset operation (create, delete, read, write, list directory)
            /// his callback is called before the IedServer_RCBEventHandler and only in case of operations (RCB_EVENT_GET_PARAMETER, RCB_EVENT_SET_PARAMETER, RCB_EVENT_ENABLE
            /// </summary>
            /// <param name="parameter">user provided parameter</param>
            /// <param name="connection">client connection that is involved</param>
            /// <param name="operation">one of the following operation types: DATASET_CREATE, DATASET_DELETE, DATASET_READ, DATASET_WRITE, DATASET_GET_DIRECTORY</param>
            /// <param name="datasetRef"></param>
            /// <returns>true to allow operation, false to deny operation</returns>
            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            private delegate bool IedServer_DataSetAccessHandler(IntPtr parameter, IntPtr connection, int operation, string datasetRef);

            /// <summary>
            /// Callback that is called when a client is invoking a read or write service to a control block or log
            /// This callback can be used to control the read and write access to control blocks and logs (SGCB, LCBs, URCBs, BRCBs, GoCBs, SVCBs, logs)
            /// </summary>
            /// <param name="parameter">user provided parameter</param>
            /// <param name="connection">client connection that is involved</param>
            /// <param name="acsiClass">the ACSI class of the object</param>
            /// <param name="ld">the logical device of the object</param>
            /// <param name="ln">the logical node of the object</param>
            /// <param name="objectName">the name of the object (e.g. data object name, data set name, log name, RCB name, ...)</param>
            /// <param name="subObjectName">the name of a sub element of an object or NULL</param>
            /// <param name="accessType">access type (read=IEC61850_CB_ACCESS_TYPE_READ or write=IEC61850_CB_ACCESS_TYPE_WRITE)</param>
            /// <returns>true to include the object in the service response, otherwise false</returns>
            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            private delegate bool IedServer_ControlBlockAccessHandler(IntPtr parameter, IntPtr connection, int acsiClass, IntPtr ld, IntPtr ln, string objectName, string subObjectName, int accessType);

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            private delegate bool IedServer_DirectoryAccessHandler(IntPtr parameter, IntPtr connection, int category, IntPtr logicalDevice);

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            public delegate bool IedServer_ListObjectsAccessHandler(IntPtr parameter, ClientConnection connection, ACSIClass acsiClass, LogicalDevice ld, LogicalNode ln, string objectName, string subObjectName, FunctionalConstraint fc);

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            private delegate int InternalControlPerformCheckHandler(IntPtr action, IntPtr parameter, IntPtr ctlVal, [MarshalAs(UnmanagedType.I1)] bool test, [MarshalAs(UnmanagedType.I1)] bool interlockCheck);

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            private delegate int InternalControlWaitForExecutionHandler(IntPtr action, IntPtr parameter, IntPtr ctlVal, [MarshalAs(UnmanagedType.I1)] bool test, [MarshalAs(UnmanagedType.I1)] bool synchoCheck);

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            private delegate int InternalControlHandler(IntPtr action, IntPtr parameter, IntPtr ctlVal, [MarshalAs(UnmanagedType.I1)] bool test);

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            private delegate void InternalSelectStateChangedHandler(IntPtr action, IntPtr parameter, [MarshalAs(UnmanagedType.I1)] bool isSelected, int reason);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_setWaitForExecutionHandler(IntPtr self, IntPtr node, InternalControlWaitForExecutionHandler handler, IntPtr parameter);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_setPerformCheckHandler(IntPtr self, IntPtr node, InternalControlPerformCheckHandler handler, IntPtr parameter);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_setControlHandler(IntPtr self, IntPtr node, InternalControlHandler handler, IntPtr parameter);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_setSelectStateChangedHandler(IntPtr self, IntPtr node, InternalSelectStateChangedHandler handler, IntPtr parameter);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_setWriteAccessPolicy(IntPtr self, FunctionalConstraint fc, AccessPolicy policy);

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            private delegate int InternalWriteAccessHandler(IntPtr dataAttribute, IntPtr value, IntPtr connection, IntPtr parameter);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_handleWriteAccess(IntPtr self, IntPtr dataAttribute,
                InternalWriteAccessHandler handler, IntPtr parameter);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_handleWriteAccessForComplexAttribute(IntPtr self, IntPtr dataAttribute,
                InternalWriteAccessHandler handler, IntPtr parameter);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_handleWriteAccessForDataObject(IntPtr self, IntPtr dataObject, int fc,
                InternalWriteAccessHandler handler, IntPtr parameter);

            public delegate void ConnectionIndicationHandler(IedServer iedServer, ClientConnection clientConnection, bool connected, object parameter);

            private ConnectionIndicationHandler connectionHandler = null;
            private object connectionHandlerParameter = null;

            public void SetConnectionIndicationHandler(ConnectionIndicationHandler handler, object parameter)
            {
                connectionHandler = handler;
                connectionHandlerParameter = parameter;
            }

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            private delegate void InternalConnectionHandler(IntPtr iedServer, IntPtr clientConnection, [MarshalAs(UnmanagedType.I1)] bool connected, IntPtr parameter);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_setConnectionIndicationHandler(IntPtr self, InternalConnectionHandler handler, IntPtr parameter);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_enableGoosePublishing(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_disableGoosePublishing(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_setGooseInterfaceId(IntPtr self, string interfaceId);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_setGooseInterfaceIdEx(IntPtr self, IntPtr ln, string gcbName, string interfaceId);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_useGooseVlanTag(IntPtr self, IntPtr ln, string gcbName, [MarshalAs(UnmanagedType.I1)] bool useVlanTag);

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            private delegate void InternalGoCBEventHandler(IntPtr goCB, int cbEvent, IntPtr parameter);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_setGoCBHandler(IntPtr self, InternalGoCBEventHandler handler, IntPtr parameter);

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            private delegate void InternalRCBEventHandler(IntPtr paramter, IntPtr rcb, IntPtr connection, int eventType, string parameterName, int serviceError);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_setRCBEventHandler(IntPtr self, InternalRCBEventHandler handler, IntPtr parameter);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_setTimeQuality(IntPtr self, [MarshalAs(UnmanagedType.I1)] bool leapSecondKnown, [MarshalAs(UnmanagedType.I1)] bool clockFailure, [MarshalAs(UnmanagedType.I1)] bool clockNotSynchronized, int subsecondPrecision);

            private IntPtr self = IntPtr.Zero;

            private InternalControlHandler internalControlHandlerRef = null;
            private InternalControlPerformCheckHandler internalControlPerformCheckHandlerRef = null;
            private InternalControlWaitForExecutionHandler internalControlWaitForExecutionHandlerRef = null;
            private InternalSelectStateChangedHandler internalSelectedStateChangedHandlerRef = null;

            // hold references to managed LogStorage instances to avoid problems with GC
            private Dictionary<string, LogStorage> logStorages = new Dictionary<string, LogStorage>();

            internal class ControlHandlerInfo
            {
                public DataObject controlObject = null;
                public GCHandle handle;

                public ControlHandler controlHandler = null;
                public object controlHandlerParameter = null;

                public CheckHandler checkHandler = null;
                public object checkHandlerParameter = null;

                public ControlWaitForExecutionHandler waitForExecHandler = null;
                public object waitForExecHandlerParameter = null;

                public ControlSelectStateChangedHandler selectStateChangedHandler = null;
                public object selectStateChangedHandlerParameter = null;

                public ControlHandlerInfo(DataObject controlObject)
                {
                    this.controlObject = controlObject;
                    handle = GCHandle.Alloc(this);
                }

                ~ControlHandlerInfo()
                {
                    handle.Free();
                }
            }

            private Dictionary<DataObject, ControlHandlerInfo> controlHandlers = new Dictionary<DataObject, ControlHandlerInfo>();

            int InternalControlHandlerImpl(IntPtr action, IntPtr parameter, IntPtr ctlVal, bool test)
            {
                GCHandle handle = GCHandle.FromIntPtr(parameter);

                ControlHandlerInfo info = (ControlHandlerInfo)handle.Target;

                ControlAction controlAction = new ControlAction(action, info, this);

                if (info != null && info.controlHandler != null)
                    return (int)info.controlHandler(controlAction, info.controlHandlerParameter, new MmsValue(ctlVal), test);
                else
                    return (int)ControlHandlerResult.FAILED;
            }

            int InternalCheckHandlerImpl(IntPtr action, IntPtr parameter, IntPtr ctlVal, bool test, bool interlockCheck)
            {
                GCHandle handle = GCHandle.FromIntPtr(parameter);

                ControlHandlerInfo info = (ControlHandlerInfo)handle.Target;

                if (info != null && info.checkHandler != null)
                {
                    ControlAction controlAction = new ControlAction(action, info, this);

                    return (int)info.checkHandler(controlAction, info.checkHandlerParameter, new MmsValue(ctlVal), test, interlockCheck);
                }
                else
                    return (int)CheckHandlerResult.OBJECT_UNDEFINED;
            }

            int InternalControlWaitForExecutionHandlerImpl(IntPtr action, IntPtr parameter, IntPtr ctlVal, bool test, bool synchoCheck)
            {
                GCHandle handle = GCHandle.FromIntPtr(parameter);

                ControlHandlerInfo info = (ControlHandlerInfo)handle.Target;

                if (info != null && info.waitForExecHandler != null)
                {
                    ControlAction controlAction = new ControlAction(action, info, this);

                    return (int)info.waitForExecHandler(controlAction, info.waitForExecHandlerParameter, new MmsValue(ctlVal), test, synchoCheck);
                }
                else
                    return (int)ControlHandlerResult.FAILED;
            }

            void InternalSelectStateChangedHandlerImpl(IntPtr action, IntPtr parameter, bool isSelected, int reason)
            {
                GCHandle handle = GCHandle.FromIntPtr(parameter);

                ControlHandlerInfo info = (ControlHandlerInfo)handle.Target;

                if (info != null && info.selectStateChangedHandler != null)
                {
                    ControlAction controlAction = new ControlAction(action, info, this);

                    info.selectStateChangedHandler(controlAction, info.selectStateChangedHandlerParameter, isSelected, (SelectStateChangedReason)reason);
                }
            }

            private struct WriteAccessHandlerInfo
            {
                public WriteAccessHandler handler;
                public InternalWriteAccessHandler internalHandler;
                public object parameter;
                public DataAttribute dataAttribute;

                public WriteAccessHandlerInfo(WriteAccessHandler h, object p, DataAttribute da, InternalWriteAccessHandler internalHandler)
                {
                    handler = h;
                    parameter = p;
                    dataAttribute = da;
                    this.internalHandler = internalHandler;
                }
            }

            int WriteAccessHandlerImpl(IntPtr dataAttribute, IntPtr value, IntPtr connection, IntPtr parameter)
            {
                WriteAccessHandlerInfo info;

                if (writeAccessHandlers.TryGetValue(dataAttribute, out info) == true)
                {
                    ClientConnection con = null;

                    clientConnections.TryGetValue(connection, out con);

                    MmsValue mmsValue = new MmsValue(value);

                    return (int)info.handler(info.dataAttribute, mmsValue.Clone(), con, info.parameter);
                }
                else
                {
                    return (int)MmsDataAccessError.OBJECT_ACCESS_DENIED;
                }
            }

            public void SetListObjectsAccessHandler(IedServer_ListObjectsAccessHandler handler, IntPtr parameter)
            {
                IedServer_setListObjectsAccessHandler(self, handler, parameter);
            }

            public delegate bool ControlBlockAccessHandler(object parameter, ClientConnection connection, ACSIClass acsiClass, LogicalDevice ld, LogicalNode ln, string objectName, string subObjectName, ControlBlockAccessType accessType);

            private ControlBlockAccessHandler rcbControlHandler = null;

            private object rcbControlHandlerParameter = null;

            private IedServer_ControlBlockAccessHandler internalRCBControlHandler = null;

            private bool InternalRCBControlHandlerImplementation(IntPtr parameter, IntPtr connection, int acsiClass, IntPtr ld, IntPtr ln, string objectName, string subObjectName, int accessType)
            {
                if (rcbControlHandler != null && connection != IntPtr.Zero && ld != IntPtr.Zero && ln != IntPtr.Zero)
                {
                    ClientConnection con = null;

                    clientConnections.TryGetValue(connection, out con);

                    ModelNode ldModelNode = iedModel.GetModelNodeFromNodeRef(ld);
                    ModelNode lnModelNode = iedModel.GetModelNodeFromNodeRef(ln);
                    return rcbControlHandler(rcbControlHandlerParameter, con, (ACSIClass)acsiClass, ldModelNode as LogicalDevice, lnModelNode as LogicalNode, objectName, subObjectName, (ControlBlockAccessType)accessType);
                }

                return false;

            }

            /// <summary>
            /// Set a handler to control read and write access to control blocks and logs
            /// </summary>
            /// <param name="handler">the callback handler to be used</param>
            /// <param name="parameter">a user provided parameter that is passed to the handler</param>
            public void SetControlBlockAccessHandler(ControlBlockAccessHandler handler, object parameter)
            {
                rcbControlHandler = handler;
                rcbControlHandlerParameter = parameter;

                if (internalRCBControlHandler == null)
                {
                    internalRCBControlHandler = new IedServer_ControlBlockAccessHandler(InternalRCBControlHandlerImplementation);

                    IedServer_setControlBlockAccessHandler(self, internalRCBControlHandler, IntPtr.Zero);
                }
            }

            public delegate bool DataSetAccessHandler(object parameter, ClientConnection connection, DataSetOperation operation, string datasetRef);

            private DataSetAccessHandler dataSetAccessHandler = null;

            private object dataSetAccessHandlerParameter = null;

            private IedServer_DataSetAccessHandler internalDataSetAccessHandler = null;

            /// <summary>
            /// Callback that is called when the client is calling a dataset operation (create, delete, read, write, list directory)
            /// note This callback is called before the IedServer_RCBEventHandler and only in case of operations(RCB_EVENT_GET_PARAMETER, RCB_EVENT_SET_PARAMETER, RCB_EVENT_ENABLE
            /// </summary>
            /// <param name="handler">the callback handler to be used</param>
            /// <param name="parameter">a user provided parameter that is passed to the handler</param>
            public void SetDataSetAccessHandler(DataSetAccessHandler handler, object parameter)
            {
                dataSetAccessHandler = handler;
                dataSetAccessHandlerParameter = parameter;

                if (internalDataSetAccessHandler == null)
                {
                    internalDataSetAccessHandler = new IedServer_DataSetAccessHandler(InternalDataSetlHandlerImplementation);

                    IedServer_setDataSetAccessHandler(self, internalDataSetAccessHandler, IntPtr.Zero);
                }
            }

            private bool InternalDataSetlHandlerImplementation(IntPtr parameter, IntPtr connection, int operation, string datasetRef)
            {
                if (dataSetAccessHandler != null && connection != IntPtr.Zero)
                {
                    ClientConnection con = null;

                    clientConnections.TryGetValue(connection, out con);

                    return dataSetAccessHandler(dataSetAccessHandlerParameter, con, (DataSetOperation)operation, datasetRef);
                }

                return false;
            }

            public delegate bool ActiveSettingGroupChangedHandler(object parameter, SettingGroupControlBlock sgcb, uint newActSg, ClientConnection connection);

            private ActiveSettingGroupChangedHandler activeSettingGroupChangedHandler = null;

            private object activeSettingGroupChangedHandlerParameter = null;

            private IedServer_ActiveSettingGroupChangedHandler internalActiveSettingGroupChangedHandler = null;

            public void SetActiveSettingGroupChangedHandler(ActiveSettingGroupChangedHandler handler, SettingGroupControlBlock settingGroupControlBlock, object parameter)
            {
                activeSettingGroupChangedHandler = handler;
                activeSettingGroupChangedHandlerParameter = parameter;

                if (internalActiveSettingGroupChangedHandler == null)
                {
                    internalActiveSettingGroupChangedHandler = new IedServer_ActiveSettingGroupChangedHandler(InternalActiveSettingGroupChangedImplementation);

                    IedServer_setActiveSettingGroupChangedHandler(self, settingGroupControlBlock.self, internalActiveSettingGroupChangedHandler, IntPtr.Zero);
                }
            }

            public int GetActiveSettingGroupChangedHandler(SettingGroupControlBlock settingGroupControlBlock)
            {
                return Convert.ToInt32(IedServer_getActiveSettingGroup(self, settingGroupControlBlock.self));
            }

            private bool InternalActiveSettingGroupChangedImplementation(IntPtr parameter, IntPtr sgcb, uint newActSg, IntPtr connection)
            {
                if (sgcb != IntPtr.Zero && connection != IntPtr.Zero)
                {
                    ClientConnection con = null;

                    clientConnections.TryGetValue(connection, out con);

                    return activeSettingGroupChangedHandler(activeSettingGroupChangedHandlerParameter, new SettingGroupControlBlock(sgcb), newActSg, con);
                }

                return false;
            }

            public delegate bool EditSettingGroupChangedHandler(object parameter, SettingGroupControlBlock sgcb, uint newEditSg, ClientConnection connection);

            private EditSettingGroupChangedHandler editSettingGroupChangedHandler = null;

            private object editSettingGroupChangedHandlerParameter = null;

            private IedServer_EditSettingGroupChangedHandler internalEditSettingGroupChangedHandler = null;

            public void SetEditSettingGroupChangedHandler(EditSettingGroupChangedHandler handler, SettingGroupControlBlock settingGroupControlBlock, object parameter)
            {
                editSettingGroupChangedHandler = handler;
                editSettingGroupChangedHandlerParameter = parameter;

                if (internalEditSettingGroupChangedHandler == null)
                {
                    internalEditSettingGroupChangedHandler = new IedServer_EditSettingGroupChangedHandler(InternalEditSettingGroupChangedImplementation);

                    IedServer_setEditSettingGroupChangedHandler(self, settingGroupControlBlock.self, internalEditSettingGroupChangedHandler, IntPtr.Zero);
                }
            }

            private bool InternalEditSettingGroupChangedImplementation(IntPtr parameter, IntPtr sgcb, uint newEditSg, IntPtr connection)
            {
                if (sgcb != IntPtr.Zero && connection != IntPtr.Zero)
                {
                    ClientConnection con = null;

                    clientConnections.TryGetValue(connection, out con);

                    return editSettingGroupChangedHandler(editSettingGroupChangedHandlerParameter, new SettingGroupControlBlock(sgcb), newEditSg, con);
                }

                return false;
            }

            public delegate void EditSettingGroupConfirmationHandler(object parameter, SettingGroupControlBlock sgcb, uint editSg);

            private EditSettingGroupConfirmationHandler editSettingGroupConfirmationHandler = null;

            private object editSettingGroupConfirmationHandlerParameter = null;

            private IedServer_EditSettingGroupConfirmationHandler internalEditSettingGroupConfirmationHandler = null;

            public void SetEditSettingGroupConfirmationHandler(EditSettingGroupConfirmationHandler handler, SettingGroupControlBlock settingGroupControlBlock, object parameter)
            {
                editSettingGroupConfirmationHandler = handler;
                editSettingGroupConfirmationHandlerParameter = parameter;

                if (internalEditSettingGroupConfirmationHandler == null)
                {
                    internalEditSettingGroupConfirmationHandler = new IedServer_EditSettingGroupConfirmationHandler(InternalEditSettingGroupConfirmationImplementation);

                    IedServer_setEditSettingGroupConfirmationHandler(self, settingGroupControlBlock.self, internalEditSettingGroupConfirmationHandler, IntPtr.Zero);
                }
            }

            private void InternalEditSettingGroupConfirmationImplementation(IntPtr parameter, IntPtr sgcb, uint editSg)
            {
                if (sgcb != IntPtr.Zero)
                {
                    editSettingGroupConfirmationHandler(editSettingGroupChangedHandlerParameter, new SettingGroupControlBlock(sgcb), editSg);
                }
            }

            public delegate void SVCBEventHandler(SVControlBlock sampledValuesControlBlock, SMVEvent sMVEvent, object parameter);

            private IedServer_SVCBEventHandler internalSVCBEventHandler = null;

            internal class SVCHandlerInfo
            {
                public SVControlBlock sampledValuesControlBlock = null;
                public GCHandle handle;

                public SVCBEventHandler sVCBEventHandler = null;
                public object svcHandlerParameter = null;

                public SVCHandlerInfo(SVControlBlock sampledValuesControlBlock)
                {
                    this.sampledValuesControlBlock = sampledValuesControlBlock;
                    handle = GCHandle.Alloc(this);
                }

                ~SVCHandlerInfo()
                {
                    handle.Free();
                }
            }

            private Dictionary<SVControlBlock, SVCHandlerInfo> svcHandlers = new Dictionary<SVControlBlock, SVCHandlerInfo>();

            private SVCHandlerInfo GetSVCHandlerInfo(SVControlBlock sampledValuesControlBlock)
            {
                SVCHandlerInfo info;

                svcHandlers.TryGetValue(sampledValuesControlBlock, out info);

                if (info == null)
                {
                    info = new SVCHandlerInfo(sampledValuesControlBlock);
                    svcHandlers.Add(sampledValuesControlBlock, info);
                }

                return info;
            }

            public void SetSVCBHandler(SVCBEventHandler handler, SVControlBlock sampledValuesControlBlock, object parameter)
            {
                SVCHandlerInfo info = GetSVCHandlerInfo(sampledValuesControlBlock);

                info.sVCBEventHandler = handler;
                info.svcHandlerParameter = parameter;

                if (internalSVCBEventHandler == null)
                    internalSVCBEventHandler = new IedServer_SVCBEventHandler(InternalSVCBEventHandlerImplementation);

                IedServer_setSVCBHandler(self, sampledValuesControlBlock.Self, internalSVCBEventHandler, GCHandle.ToIntPtr(info.handle));
            }

            private void InternalSVCBEventHandlerImplementation(IntPtr svcb, int eventType, IntPtr parameter)
            {
                GCHandle handle = GCHandle.FromIntPtr(parameter);

                SVCHandlerInfo info = (SVCHandlerInfo)handle.Target;

                if (info != null && info.sVCBEventHandler != null)
                    info.sVCBEventHandler(info.sampledValuesControlBlock, (SMVEvent)eventType, info.svcHandlerParameter);
            }

            //TODO -> Add appReference
            public delegate bool AcseAuthenticatorHandler(object parameter, AcseAuthenticationParameter authParameter, object securityToken, IsoApplicationReference isoApplicationReference);

            private AcseAuthenticatorHandler acseAuthenticatorHandler = null;
            private object acseAuthenticatorHandlerParameter = null;
            private IedServer_AcseAuthenticator iedServer_AcseAuthenticator = null;

            public void SetAuthenticator(AcseAuthenticatorHandler acseAuthenticatorHandler, object acseAuthenticatorHandlerParameter)
            {
                this.acseAuthenticatorHandler = acseAuthenticatorHandler;
                this.acseAuthenticatorHandlerParameter = acseAuthenticatorHandlerParameter;

                if (iedServer_AcseAuthenticator == null)
                {
                    iedServer_AcseAuthenticator = new IedServer_AcseAuthenticator(InternalAcseAuthenticator);
                    IedServer_setAuthenticator(self, iedServer_AcseAuthenticator, IntPtr.Zero);
                }
            }

            private bool InternalAcseAuthenticator(IntPtr parameter, IntPtr authParameter, IntPtr securityToken, IntPtr appReference)
            {
                if (acseAuthenticatorHandler != null && authParameter != IntPtr.Zero && appReference != IntPtr.Zero)
                {
                    AcseAuthenticationParameter acseAuthenticationParameter = new AcseAuthenticationParameter(authParameter);
                    IsoApplicationReference isoApplicationReference = new IsoApplicationReference(appReference);
                    GCHandle token = GCHandle.FromIntPtr(securityToken);
                    return acseAuthenticatorHandler(acseAuthenticatorHandlerParameter, acseAuthenticationParameter, token, isoApplicationReference);
                }

                return false;
            }

            public delegate MmsDataAccessError ReadAccessHandler(LogicalDevice ld, LogicalNode ln, DataObject dataObject, FunctionalConstraint fc, ClientConnection connection, object parameter);

            private ReadAccessHandler readAccessHandler = null;

            private object readAccessHandlerParameter = null;

            private IedServer_ReadAccessHandler internalReadAccessHandler = null;

            public void SetReadAccessHandler(ReadAccessHandler handler, object parameter)
            {
                readAccessHandler = handler;
                readAccessHandlerParameter = parameter;

                if (internalReadAccessHandler == null)
                {
                    internalReadAccessHandler = new IedServer_ReadAccessHandler(InternalReadHandlerImplementation);

                    IedServer_setReadAccessHandler(self, internalReadAccessHandler, IntPtr.Zero);
                }
            }

            private MmsDataAccessError InternalReadHandlerImplementation(IntPtr ld, IntPtr ln, IntPtr dataObject, int fc, IntPtr connection, IntPtr parameter)
            {
                if (readAccessHandler != null && ld != IntPtr.Zero && ln != IntPtr.Zero && connection != IntPtr.Zero)
                {
                    ClientConnection con = null;

                    clientConnections.TryGetValue(connection, out con);

                    ModelNode ldModelNode = iedModel.GetModelNodeFromNodeRef(ld);
                    ModelNode lnModelNode = iedModel.GetModelNodeFromNodeRef(ln);
                    ModelNode doModelNode = null;

                    if (dataObject != IntPtr.Zero)
                        doModelNode = iedModel.GetModelNodeFromNodeRef(dataObject);

                    return readAccessHandler(ldModelNode as LogicalDevice, lnModelNode as LogicalNode, doModelNode as DataObject, (FunctionalConstraint)fc, con, readAccessHandlerParameter);
                }

                return MmsDataAccessError.UNKNOWN;
            }

            public enum IedServer_DirectoryCategory
            {
                DIRECTORY_CAT_LD_LIST,
                DIRECTORY_CAT_DATA_LIST,
                DIRECTORY_CAT_DATASET_LIST,
                DIRECTORY_CAT_LOG_LIST
            }

            public delegate bool DirectoryAccessHandler(object parameter, ClientConnection connection, IedServer_DirectoryCategory category, LogicalDevice ld);

            private DirectoryAccessHandler directoryAccessHandler = null;

            private object directoryAccessHandlerParameter = null;

            private IedServer_DirectoryAccessHandler internalDirectoryAccessHandler = null;

            public void SetDirectoryAccessHandler(DirectoryAccessHandler handler, object parameter)
            {
                directoryAccessHandler = handler;
                directoryAccessHandlerParameter = parameter;

                if (internalDirectoryAccessHandler == null)
                {
                    internalDirectoryAccessHandler = new IedServer_DirectoryAccessHandler(InternalDirectoryAccessHandler);

                    IedServer_setDirectoryAccessHandler(self, internalDirectoryAccessHandler, IntPtr.Zero);
                }
            }

            private bool InternalDirectoryAccessHandler(IntPtr parameter, IntPtr connection, int category, IntPtr logicalDevice)
            {
                if (directoryAccessHandler != null && connection != IntPtr.Zero)
                {
                    ClientConnection con = null;

                    clientConnections.TryGetValue(connection, out con);

                    ModelNode ldModelNode = null;

                    if (logicalDevice != IntPtr.Zero)
                        ldModelNode = iedModel.GetModelNodeFromNodeRef(logicalDevice);

                    return directoryAccessHandler(directoryAccessHandlerParameter, con, (IedServer_DirectoryCategory)category, ldModelNode as LogicalDevice);
                }

                return false;
            }


            private Dictionary<IntPtr, WriteAccessHandlerInfo> writeAccessHandlers = new Dictionary<IntPtr, WriteAccessHandlerInfo>();

            private void ConnectionIndicationHandlerImpl(IntPtr iedServer, IntPtr clientConnection, bool connected, IntPtr parameter)
            {
                if (connected == false)
                {
                    ClientConnection con = null;

                    clientConnections.TryGetValue(clientConnection, out con);

                    if (con != null)
                    {

                        if (connectionHandler != null)
                            connectionHandler(this, con, false, connectionHandlerParameter);

                        clientConnections.Remove(clientConnection);
                    }
                }
                else
                {
                    ClientConnection con = new ClientConnection(clientConnection);

                    clientConnections.Add(clientConnection, con);

                    if (connectionHandler != null)
                        connectionHandler(this, con, true, connectionHandlerParameter);
                }
            }

            internal Dictionary<IntPtr, ClientConnection> clientConnections = new Dictionary<IntPtr, ClientConnection>();

            /* store IedModel instance to prevent garbage collector */
            private IedModel iedModel = null;

            /* store TLSConfiguration instance to prevent garbage collector */
            private TLSConfiguration tlsConfiguration = null;

            public IedServer(IedModel iedModel, IedServerConfig config = null)
            {
                this.iedModel = iedModel;

                IntPtr nativeConfig = IntPtr.Zero;

                if (config != null)
                    nativeConfig = config.self;

                self = IedServer_createWithConfig(iedModel.self, IntPtr.Zero, nativeConfig);
            }

            public IedServer(IedModel iedModel, TLSConfiguration tlsConfig, IedServerConfig config = null)
            {
                this.iedModel = iedModel;
                tlsConfiguration = tlsConfig;

                IntPtr nativeConfig = IntPtr.Zero;
                IntPtr nativeTLSConfig = IntPtr.Zero;

                if (config != null)
                    nativeConfig = config.self;

                if (tlsConfig != null)
                    nativeTLSConfig = tlsConfig.GetNativeInstance();

                self = IedServer_createWithConfig(iedModel.self, nativeTLSConfig, nativeConfig);
            }

            private InternalConnectionHandler internalConnectionHandler = null;

            /// <summary>
            /// Sets the local ip address for listening
            /// </summary>
            /// <param name="localIpAddress">Local IP address.</param>
            public void SetLocalIpAddress(string localIpAddress)
            {
                IedServer_setLocalIpAddress(self, localIpAddress);
            }

            /// <summary>
            /// Set or update the TLS configuration for the server.
            /// </summary>
            /// <param name="tlsConfig">TLS configuration instance.</param>
            public void SetTLSConfiguration(TLSConfiguration tlsConfig)
            {
                tlsConfiguration = tlsConfig;

                IntPtr nativeTLSConfig = IntPtr.Zero;

                if (tlsConfig != null)
                    nativeTLSConfig = tlsConfig.GetNativeInstance();

                IedServer_setTLSConfiguration(self, nativeTLSConfig);
            }

            /// <summary>
            /// Start MMS server
            /// </summary>
            /// <param name="localIpAddress">Local IP address.</param>
            /// <param name="tcpPort">TCP port to use</param>
            public void Start(string localIpAddress, int tcpPort)
            {
                SetLocalIpAddress(localIpAddress);
                Start(tcpPort);
            }

            /// <summary>Start MMS server</summary>
            /// <param name="tcpPort">TCP port to use</param>
            public void Start(int tcpPort)
            {
                if (internalConnectionHandler == null)
                    internalConnectionHandler = new InternalConnectionHandler(ConnectionIndicationHandlerImpl);

                IedServer_setConnectionIndicationHandler(self, internalConnectionHandler, IntPtr.Zero);

                IedServer_start(self, tcpPort);
            }

            /// <summary>Start MMS server</summary>
            public void Start()
            {
                Start(-1);
            }

            /// <summary>
            /// Stop the MMS server.
            /// </summary>
            /// <description>This function will stop the server. This will close the TCP server socket and all client sockets.</description>
            public void Stop()
            {
                IedServer_stop(self);
                internalConnectionHandler = null;
            }

            /// <summary>
            /// Release all server resources (same as <see cref="Dispose"/>)
            /// </summary>
            /// <description>This function releases all MMS server resources.</description>
            public void Destroy()
            {
                Dispose();
            }

            /// <summary>
            /// Releases all resource used by the <see cref="IEC61850.Server.IedServer"/> object.
            /// </summary>
            /// <remarks>Call <see cref="Dispose"/> when you are done using the <see cref="IEC61850.Server.IedServer"/>. The
            /// <see cref="Dispose"/> method leaves the <see cref="IEC61850.Server.IedServer"/> in an unusable state. After
            /// calling <see cref="Dispose"/>, you must release all references to the <see cref="IEC61850.Server.IedServer"/> so
            /// the garbage collector can reclaim the memory that the <see cref="IEC61850.Server.IedServer"/> was occupying.</remarks>
            public void Dispose()
            {
                lock (this)
                {
                    if (self != IntPtr.Zero)
                    {
                        IedServer_destroy(self);
                        self = IntPtr.Zero;
                        internalConnectionHandler = null;
                        iedModel = null;
                        tlsConfiguration = null;
                    }
                }
            }

            ~IedServer()
            {
                Dispose();
            }

            /// <summary>
            /// Set the identify for the MMS identify service
            /// </summary>
            /// <param name="vendor">the IED vendor name</param>
            /// <param name="model">the IED model name</param>
            /// <param name="revision">the IED revision/version number</param>
            public void SetServerIdentity(string vendor, string model, string revision)
            {
                IedServer_setServerIdentity(self, vendor, model, revision);
            }

            /// <summary>
            /// Check if server is running (accepting client connections)
            /// </summary>
            /// <returns><c>true</c>, if running, <c>false</c> otherwise.</returns>
            public bool IsRunning()
            {
                return IedServer_isRunning(self);
            }

            /// <summary>
            /// Get number of open MMS connections
            /// </summary>
            /// <returns>the number of open and accepted MMS connections</returns>
            public int GetNumberOfOpenConnections()
            {
                return IedServer_getNumberOfOpenConnections(self);
            }

            /// <summary>
            /// Set the request timeout for the server (used for obtain file service)
            /// </summary>
            /// <param name="timeoutMs">request timeout in milliseconds</param>
            public void SetRequestTimeout(int timeoutMs)
            {
                IedServer_setRequestTimeout(self, timeoutMs);
            }

            private ControlHandlerInfo GetControlHandlerInfo(DataObject controlObject)
            {
                ControlHandlerInfo info;

                controlHandlers.TryGetValue(controlObject, out info);

                if (info == null)
                {
                    info = new ControlHandlerInfo(controlObject);
                    controlHandlers.Add(controlObject, info);
                }

                return info;
            }

            public void SetControlHandler(DataObject controlObject, ControlHandler handler, object parameter)
            {
                ControlHandlerInfo info = GetControlHandlerInfo(controlObject);

                info.controlHandler = handler;
                info.controlHandlerParameter = parameter;

                if (internalControlHandlerRef == null)
                    internalControlHandlerRef = new InternalControlHandler(InternalControlHandlerImpl);

                IedServer_setControlHandler(self, controlObject.self, internalControlHandlerRef, GCHandle.ToIntPtr(info.handle));
            }

            public void SetCheckHandler(DataObject controlObject, CheckHandler handler, object parameter)
            {
                ControlHandlerInfo info = GetControlHandlerInfo(controlObject);

                info.checkHandler = handler;
                info.checkHandlerParameter = parameter;

                if (internalControlPerformCheckHandlerRef == null)
                    internalControlPerformCheckHandlerRef = new InternalControlPerformCheckHandler(InternalCheckHandlerImpl);

                IedServer_setPerformCheckHandler(self, controlObject.self, internalControlPerformCheckHandlerRef, GCHandle.ToIntPtr(info.handle));
            }

            public void SetWaitForExecutionHandler(DataObject controlObject, ControlWaitForExecutionHandler handler, object parameter)
            {
                ControlHandlerInfo info = GetControlHandlerInfo(controlObject);

                info.waitForExecHandler = handler;
                info.waitForExecHandlerParameter = parameter;

                if (internalControlWaitForExecutionHandlerRef == null)
                    internalControlWaitForExecutionHandlerRef = new InternalControlWaitForExecutionHandler(InternalControlWaitForExecutionHandlerImpl);

                IedServer_setWaitForExecutionHandler(self, controlObject.self, internalControlWaitForExecutionHandlerRef, GCHandle.ToIntPtr(info.handle));
            }

            /// <summary>
            /// Set a callback handler for a controllable data object to track select state changes
            /// </summary>
            /// <param name="controlObject">Control object.</param>
            /// <param name="handler">Handler.</param>
            /// <param name="parameter">A user provided parameter that is passed to the callback handler.</param>
            public void SetSelectStateChangedHandler(DataObject controlObject, ControlSelectStateChangedHandler handler, object parameter)
            {
                ControlHandlerInfo info = GetControlHandlerInfo(controlObject);

                info.selectStateChangedHandler = handler;
                info.selectStateChangedHandlerParameter = parameter;

                if (internalSelectedStateChangedHandlerRef == null)
                    internalSelectedStateChangedHandlerRef = new InternalSelectStateChangedHandler(InternalSelectStateChangedHandlerImpl);

                IedServer_setSelectStateChangedHandler(self, controlObject.self, internalSelectedStateChangedHandlerRef, GCHandle.ToIntPtr(info.handle));
            }

            /// <summary>
            /// Install a WriteAccessHandler for a data attribute.
            /// </summary>
            /// This instructs the server to monitor write attempts by MMS clients to specific
            /// data attributes.If a client tries to write to the monitored data attribute the
            /// handler is invoked.The handler can decide if the write access will be allowed
            /// or denied.If a WriteAccessHandler is set for a specific data attribute - the
            /// default write access policy will not be performed for that data attribute.
            /// <remarks>
            /// NOTE: If the data attribute has sub data attributes, the WriteAccessHandler is not
            /// set for the sub data attributes and will not be called when the sub data attribute is
            /// written directly!
            /// </remarks>
            /// <param name="dataAttr">the data attribute to monitor</param>
            /// <param name="handler">the callback function that is invoked if a client tries to write to the monitored data attribute.</param>
            /// <param name="parameter">a user provided parameter that is passed to the WriteAccessHandler when called.</param>
            public void HandleWriteAccess(DataAttribute dataAttr, WriteAccessHandler handler, object parameter)
            {
                InternalWriteAccessHandler internalHandler = new InternalWriteAccessHandler(WriteAccessHandlerImpl);

                writeAccessHandlers.Add(dataAttr.self, new WriteAccessHandlerInfo(handler, parameter, dataAttr, internalHandler));

                IedServer_handleWriteAccess(self, dataAttr.self, internalHandler, IntPtr.Zero);
            }

            private void AddHandlerInfoForDataAttributeRecursive(DataAttribute da, WriteAccessHandler handler, object parameter, InternalWriteAccessHandler internalHandler)
            {
                writeAccessHandlers.Add(da.self, new WriteAccessHandlerInfo(handler, parameter, da, internalHandler));

                foreach (ModelNode child in da.GetChildren())
                {
                    if (child is DataAttribute)
                    {
                        AddHandlerInfoForDataAttributeRecursive(child as DataAttribute, handler, parameter, internalHandler);
                    }
                }
            }

            private void AddHandlerInfoForDataObjectRecursive(DataObject dataObject, FunctionalConstraint fc, WriteAccessHandler handler, object parameter, InternalWriteAccessHandler internalHandler)
            {
                foreach (ModelNode child in dataObject.GetChildren())
                {
                    if (child is DataAttribute && (child as DataAttribute).FC == fc)
                    {
                        AddHandlerInfoForDataAttributeRecursive(child as DataAttribute, handler, parameter, internalHandler);
                    }
                    else if (child is DataObject)
                    {
                        AddHandlerInfoForDataObjectRecursive(child as DataObject, fc, handler, parameter, internalHandler);
                    }
                }
            }

            /// <summary>
            /// Install a WriteAccessHandler for a data attribute and for all sub data attributes
            /// </summary>
            /// This instructs the server to monitor write attempts by MMS clients to specific
            /// data attributes. If a client tries to write to the monitored data attribute the
            /// handler is invoked. The handler can decide if the write access will be allowed
            /// or denied.If a WriteAccessHandler is set for a specific data attribute - the
            /// default write access policy will not be performed for that data attribute.
            /// <remarks>
            /// NOTE: When the data attribute is a complex attribute then the handler will also be installed
            /// for all sub data attributes. When the data attribute is a basic data attribute then
            /// this function behaves like  <see cref="HandleWriteAccess"/>.
            /// </remarks>
            /// <param name="dataAttr">the data attribute to monitor</param>
            /// <param name="handler">the callback function that is invoked if a client tries to write to the monitored data attribute.</param>
            /// <param name="parameter">a user provided parameter that is passed to the WriteAccessHandler when called.</param>
            public void HandleWriteAccessForComplexAttribute(DataAttribute dataAttr, WriteAccessHandler handler, object parameter)
            {
                InternalWriteAccessHandler internalHandler = new InternalWriteAccessHandler(WriteAccessHandlerImpl);

                AddHandlerInfoForDataAttributeRecursive(dataAttr, handler, parameter, internalHandler);

                IedServer_handleWriteAccessForComplexAttribute(self, dataAttr.self, internalHandler, IntPtr.Zero);
            }

            /// <summary>
            /// Install a WriteAccessHandler for a data object and for all sub data objects and sub data attributes that have the same functional constraint
            /// </summary>
            /// This instructs the server to monitor write attempts by MMS clients to specific
            /// data attributes. If a client tries to write to the monitored data attribute the
            /// handler is invoked. The handler can decide if the write access will be allowed
            /// or denied. If a WriteAccessHandler is set the
            /// default write access policy will not be performed for the matching data attributes.
            /// <param name="dataObject">the data object to monitor</param>
            /// <param name="fc">the functional constraint (FC) to monitor</param>
            /// <param name="handler">the callback function that is invoked if a client tries to write to a monitored data attribute that is a child of the data object.</param>
            /// <param name="parameter">a user provided parameter that is passed to the WriteAccessHandler when called.</param>
            public void HandleWriteAccessForDataObject(DataObject dataObj, FunctionalConstraint fc, WriteAccessHandler handler, object parameter)
            {
                InternalWriteAccessHandler internalHandler = new InternalWriteAccessHandler(WriteAccessHandlerImpl);

                AddHandlerInfoForDataObjectRecursive(dataObj, fc, handler, parameter, internalHandler);

                IedServer_handleWriteAccessForDataObject(self, dataObj.self, (int)fc, internalHandler, IntPtr.Zero);
            }

            /// <summary>
            /// Set the defualt write access policy for a specific FC. The default policy is applied when no handler is installed for a data attribute
            /// </summary>
            /// <param name="fc">The functional constraint (FC)</param>
            /// <param name="policy">The new default access policy</param>
            public void SetWriteAccessPolicy(FunctionalConstraint fc, AccessPolicy policy)
            {
                IedServer_setWriteAccessPolicy(self, fc, policy);
            }

            /// <summary>
            /// Locks the data model for data update.
            /// </summary>
            /// This function should be called before the data model is updated.
            /// After updating the data model the function <see cref="UnlockDataModel"/> should be called.
            ///
            /// <remarks>
            /// his method should never be called inside of a library callback function. In the context of
            /// a library callback the data model is always already locked! Calling this function inside of a
            /// library callback may lead to a deadlock condition.
            /// </remarks>
            public void LockDataModel()
            {
                IedServer_lockDataModel(self);
            }

            /// <summary>
            /// Unlocks the data model and process pending client requests.
            /// </summary>
            ///
            /// <remarks>
            /// This method should never be called inside of a library callback function. In the context of
            /// a library callback the data model is always already locked!
            /// </remarks>
            public void UnlockDataModel()
            {
                IedServer_unlockDataModel(self);
            }

            public void UpdateAttributeValue(DataAttribute dataAttr, MmsValue value)
            {
                IedServer_updateAttributeValue(self, dataAttr.self, value.valueReference);
            }

            public void UpdateBooleanAttributeValue(DataAttribute dataAttr, bool value)
            {
                IedServer_updateBooleanAttributeValue(self, dataAttr.self, value);
            }

            public void UpdateFloatAttributeValue(DataAttribute dataAttr, float value)
            {
                IedServer_updateFloatAttributeValue(self, dataAttr.self, value);
            }

            public void UpdateInt32AttributeValue(DataAttribute dataAttr, int value)
            {
                IedServer_updateInt32AttributeValue(self, dataAttr.self, value);
            }

            public void UpdateInt64AttributeValue(DataAttribute dataAttr, Int64 value)
            {
                IedServer_updateInt64AttributeValue(self, dataAttr.self, value);
            }

            public void UpdateVisibleStringAttributeValue(DataAttribute dataAttr, string value)
            {
                IedServer_updateVisibleStringAttributeValue(self, dataAttr.self, value);
            }

            public void UpdateBitStringAttributeValue(DataAttribute dataAttr, UInt32 value)
            {
                IedServer_updateBitStringAttributeValue(self, dataAttr.self, value);
            }

            public void UpdateUnsignedAttributeValue(DataAttribute dataAttr, UInt32 value)
            {
                IedServer_updateUnsignedAttributeValue(self, dataAttr.self, value);
            }

            public void UpdateDbposAttributeValue(DataAttribute dataAttr, Dbpos value)
            {
                IedServer_updateDbposValue(self, dataAttr.self, value);
            }
            public void UpdateUTCTimeAttributeValue(DataAttribute dataAttr, DateTime timestamp)
            {
                DateTime epoch = new DateTime(1970, 1, 1, 0, 0, 0, DateTimeKind.Utc);
                DateTime timestampUTC = timestamp.ToUniversalTime();

                TimeSpan timeDiff = timestampUTC - epoch;
                ulong timeVal = Convert.ToUInt64(timeDiff.TotalMilliseconds);

                IedServer_updateUTCTimeAttributeValue(self, dataAttr.self, timeVal);
            }

            public void UpdateTimestampAttributeValue(DataAttribute dataAttr, Timestamp timestamp)
            {
                IedServer_updateTimestampAttributeValue(self, dataAttr.self, timestamp.self);
            }

            public void UpdateQuality(DataAttribute dataAttr, ushort value)
            {
                IedServer_updateQuality(self, dataAttr.self, value);
            }

            public MmsValue GetAttributeValue(DataAttribute dataAttr)
            {
                IntPtr mmsValuePtr = IedServer_getAttributeValue(self, dataAttr.self);

                if (mmsValuePtr != IntPtr.Zero)
                    return new MmsValue(mmsValuePtr);
                else
                    return null;
            }

            /// <summary>
            /// Get the MmsValue object related to a functional constrained data object (FCD)
            /// </summary>
            /// <param name="dataObject">the data object to specify the FCD</param>
            /// <param name="fc">the FC to specify the FCD</param>
            /// <returns>FCDO corresponding MmsValue object cached by the server</returns>
            public MmsValue GetFunctionalConstrainedData(DataObject dataObject, FunctionalConstraint fc)
            {
                IntPtr mmsValuePtr = IedServer_getFunctionalConstrainedData(self, dataObject.self, (int)fc);

                if (mmsValuePtr != IntPtr.Zero)
                    return new MmsValue(mmsValuePtr);
                else
                    return null;
            }

            /// <summary>
            /// Enable all GOOSE control blocks.
            /// </summary>
            /// This will set the GoEna attribute of all configured GOOSE control blocks
            /// to true. If this method is not called at the startup or reset of the server
            /// then configured GOOSE control blocks keep inactive until a MMS client enables
            /// them by writing to the GOOSE control block.
            public void EnableGoosePublishing()
            {
                IedServer_enableGoosePublishing(self);
            }

            /// <summary>
            /// Disable all GOOSE control blocks.
            /// </summary>
            /// This will set the GoEna attribute of all configured GOOSE control blocks
            /// to false. This will stop GOOSE transmission.
            public void DisableGoosePublishing()
            {
                IedServer_disableGoosePublishing(self);
            }

            /// <summary>
            /// Set the Ethernet interface to be used by GOOSE publishing
            /// </summary>
            ///
            /// This function can be used to set the GOOSE interface ID. If not used or set to null the
            /// default interface ID from stack_config.h is used.Note the interface ID is operating system
            /// specific!
            ///
            /// <param name="interfaceId">the ID of the ethernet interface to be used for GOOSE publishing</param>
            public void SetGooseInterfaceId(string interfaceId)
            {
                IedServer_setGooseInterfaceId(self, interfaceId);
            }

            /// <summary>
            /// Set the Ethernet interface to be used by GOOSE publishing
            /// </summary>
            ///
            /// This function can be used to set the GOOSE interface ID for all GCBs (parameter ln = null) or for
            /// a specific GCB specified by the logical node instance and the GCB name.
            ///
            /// <param name="ln">ln the logical node that contains the GCB or null to set the ethernet interface ID for all GCBs</param>
            /// <param name="gcbName">the name (not object reference!) of the GCB</param>
            /// <param name="interfaceId">the ID of the ethernet interface to be used for GOOSE publishing</param>
            public void SetGooseInterfaceId(LogicalNode ln, string gcbName, string interfaceId)
            {
                if (ln == null)
                    IedServer_setGooseInterfaceIdEx(self, IntPtr.Zero, gcbName, interfaceId);
                else
                    IedServer_setGooseInterfaceIdEx(self, ln.self, gcbName, interfaceId);
            }

            /// <summary>
            /// Enable/disable the use of VLAN tags in GOOSE messages
            /// </summary>
            ///
            /// This function can be used to enable/disable VLAN tagging for all GCBs (parameter ln = null) or for
            /// a specific GCB specified by the logical node instance and the GCB name.
            ///
            /// <param name="ln">the logical node that contains the GCB or null to enable/disable VLAN tagging for all GCBs</param>
            /// <param name="gcbName">the name (not object reference!) of the GCB</param>
            /// <param name="useVlanTag">true to enable VLAN tagging, false otherwise</param>
            public void UseGooseVlanTag(LogicalNode ln, string gcbName, bool useVlanTag)
            {
                if (ln == null)
                    IedServer_useGooseVlanTag(self, IntPtr.Zero, gcbName, useVlanTag);
                else
                    IedServer_useGooseVlanTag(self, ln.self, gcbName, useVlanTag);
            }

            private GoCBEventHandler goCbEventHandler = null;
            private object goCbEventHandlerParameter = null;

            private InternalGoCBEventHandler internalGoCBEventHandler = null;

            private void InternalGoCBEventHandlerImplementation(IntPtr goCB, int cbEvent, IntPtr parameter)
            {
                if (goCbEventHandler != null)
                {
                    MmsGooseControlBlock mmsGoCb = new MmsGooseControlBlock(goCB, iedModel);

                    goCbEventHandler.Invoke(mmsGoCb, cbEvent, goCbEventHandlerParameter);
                }
            }

            /// <summary>
            /// Set a callback handler for GoCB events (enabled/disabled)
            /// </summary>
            /// <param name="handler">the callback handler</param>
            /// <param name="parameter">user provided parameter that is passed to the callback handler</param>
            public void SetGoCBHandler(GoCBEventHandler handler, object parameter)
            {
                goCbEventHandler = handler;
                goCbEventHandlerParameter = parameter;

                if (internalGoCBEventHandler == null)
                {
                    internalGoCBEventHandler = new InternalGoCBEventHandler(InternalGoCBEventHandlerImplementation);

                    IedServer_setGoCBHandler(self, internalGoCBEventHandler, IntPtr.Zero);
                }
            }

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr ReportControlBlock_getParent(IntPtr self);

            private RCBEventHandler rcbEventHandler = null;
            private object rcbEventHandlerParameter = null;

            private InternalRCBEventHandler internalRCBEventHandler = null;

            private void InternalRCBEventHandlerImplementation(IntPtr parameter, IntPtr rcb, IntPtr connection, int eventType, string parameterName, int serviceError)
            {
                if (rcbEventHandler != null)
                {
                    ClientConnection con = null;

                    if (connection != IntPtr.Zero)
                    {
                        clientConnections.TryGetValue(connection, out con);
                    }

                    ReportControlBlock reportControlBlock = null;

                    if (rcb != IntPtr.Zero)
                    {
                        IntPtr lnPtr = ReportControlBlock_getParent(rcb);

                        if (lnPtr != IntPtr.Zero)
                        {
                            ModelNode lnModelNode = iedModel.GetModelNodeFromNodeRef(lnPtr);

                            if (lnModelNode != null)
                            {
                                LogicalNode ln = lnModelNode as LogicalNode;

                                if (ln.rcbs.TryGetValue(rcb, out reportControlBlock) == false)
                                {
                                    reportControlBlock = new ReportControlBlock(rcb, ln);
                                }
                            }
                        }
                    }

                    rcbEventHandler.Invoke(rcbEventHandlerParameter, reportControlBlock, con, (RCBEventType)eventType, parameterName, (MmsDataAccessError)serviceError);
                }
            }

            /// <summary>
            /// Set a callback handler for RCB events
            /// </summary>
            /// <param name="handler">the callback handler</param>
            /// <param name="parameter">user provided parameter that is passed to the callback handler</param>
            public void SetRCBEventHandler(RCBEventHandler handler, object parameter)
            {
                rcbEventHandler = handler;
                rcbEventHandlerParameter = parameter;

                if (internalRCBEventHandler == null)
                {
                    internalRCBEventHandler = new InternalRCBEventHandler(InternalRCBEventHandlerImplementation);

                    IedServer_setRCBEventHandler(self, internalRCBEventHandler, IntPtr.Zero);
                }
            }

            /// <summary>
            /// Set the time quality for all timestamps internally generated by this IedServer instance
            /// </summary>
            /// <param name="leapSecondKnown">set/unset leap seconds known flag</param>
            /// <param name="clockFailure">set/unset clock failure flag</param>
            /// <param name="clockNotSynchronized">set/unset clock not synchronized flag</param>
            /// <param name="subsecondPrecision">set the subsecond precision (number of significant bits of the fractionOfSecond part of the time stamp)</param>
            public void SetTimeQuality(bool leapSecondKnown, bool clockFailure, bool clockNotSynchronized, int subsecondPrecision)
            {
                IedServer_setTimeQuality(self, leapSecondKnown, clockFailure, clockNotSynchronized, subsecondPrecision);
            }

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void IedServer_setLogStorage(IntPtr self, string logRef, IntPtr logStorage);

            public void SetLogStorage(string logRef, LogStorage logStorage)
            {
                if (logStorage != null)
                {
                    logStorages.Add(logRef, logStorage);
                    IedServer_setLogStorage(self, logRef, logStorage.GetNativeInstance());
                }
            }
        }
    }
}