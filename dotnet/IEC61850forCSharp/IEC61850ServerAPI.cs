/*
 *  IEC61850ServerAPI.cs
 *
 *  Copyright 2016 Michael Zillgith
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

/// <summary>
/// IEC 61850 API for the libiec61850 .NET wrapper library
/// </summary>
namespace IEC61850
{
	/// <summary>
	/// IEC 61850 server API.
	/// </summary>
	namespace Server
	{
		
		public class ConfigFileParser
		{

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr FileSystem_openFile(string filePath, bool readWrite);


			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr ConfigFileParser_createModelFromConfigFile(IntPtr fileHandle);

			public static IedModel CreateModelFromConfigFile(string filePath)
			{
				IntPtr fileHandle = FileSystem_openFile (filePath, false);

				if (fileHandle != IntPtr.Zero) {
					
					IntPtr retVal = ConfigFileParser_createModelFromConfigFile (fileHandle);
					if (retVal == IntPtr.Zero) {
						return null;
					}

					return new IedModel (retVal);

				} else
					return null;
				//TODO else throw exception
			}
		}

		public class IedModel
		{
			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr IedModel_create(string name);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr IedModel_destroy(IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr IedModel_getModelNodeByObjectReference(IntPtr self, string objectReference);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr IedModel_getModelNodeByShortObjectReference(IntPtr self, string objectReference);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern int ModelNode_getType(IntPtr self);

			internal IntPtr self = IntPtr.Zero;

			internal IedModel(IntPtr self)
			{
				this.self = self;
			}

			public IedModel(string name)
			{
				self = IedModel_create(name);
			}

			// causes undefined behavior
			//~IedModel()
			//{
			//    if (self != IntPtr.Zero)
			//    {
			//        IedModel_destroy(self);
			//    }
			//}

			public void Destroy()
			{
				IedModel_destroy(self);
				self = IntPtr.Zero;
			}

			public static IedModel CreateFromFile(string filePath)
			{
				return ConfigFileParser.CreateModelFromConfigFile(filePath);
			}

			private ModelNode getModelNodeFromNodeRef(IntPtr nodeRef)
			{				
				int nodeType = ModelNode_getType (nodeRef);

				switch (nodeType) {
				case 0:
					return new LogicalDevice (nodeRef);

				case 1:
					return new LogicalNode (nodeRef);

				case 2:
					return new DataObject (nodeRef);

				case 3:
					return new DataAttribute (nodeRef);

				default:
					return new ModelNode (nodeRef);
				}
			}

			public ModelNode GetModelNodeByObjectReference(string objectReference)
			{
				IntPtr nodeRef = IedModel_getModelNodeByObjectReference(self, objectReference);

				if (nodeRef == IntPtr.Zero)
					return null;

				return getModelNodeFromNodeRef (nodeRef);
			}

			public ModelNode GetModelNodeByShortObjectReference(string objectReference)
			{
				IntPtr nodeRef = IedModel_getModelNodeByShortObjectReference(self, objectReference);

				if (nodeRef == IntPtr.Zero)
					return null;

				return getModelNodeFromNodeRef (nodeRef);
			}


		}

		public class LogicalDevice : ModelNode
		{
			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr LogicalDevice_create(string name, IntPtr parent);

			public LogicalDevice (IntPtr self) : base (self)
			{
			}

			public LogicalDevice(string name, IedModel parent)
			{
				self = LogicalDevice_create(name, parent.self);
			}
		}

		public class LogicalNode : ModelNode
		{
			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr LogicalNode_create(string name, IntPtr parent);

			public LogicalNode (IntPtr self) : base(self)
			{
			}

			public LogicalNode(string name, LogicalDevice parent)
			{
				base.self = LogicalNode_create(name, parent.self);
			}
		}

		public enum AccessPolicy {
			ACCESS_POLICY_ALLOW = 0,
			ACCESS_POLICY_DENY = 1
		}

		public enum DataAttributeType {
			BOOLEAN = 0,
			INT8 = 1,
			INT16 = 2,
			INT32 = 3,
			INT64 = 4, 
			INT128 = 5, 
			INT8U = 6,
			INT16U = 7,
			INT24U = 8,
			INT32U = 9,
			FLOAT32 = 10,
			FLOAT64 = 11,
			ENUMERATED = 12,
			OCTET_STRING_64 = 13,
			OCTET_STRING_6 = 14,
			OCTET_STRING_8 = 15,
			VISIBLE_STRING_32 = 16,
			VISIBLE_STRING_64 = 17,
			VISIBLE_STRING_65 = 18,
			VISIBLE_STRING_129 = 19,
			VISIBLE_STRING_255 = 20,
			UNICODE_STRING_255 = 21,
			TIMESTAMP = 22,
			QUALITY = 23,
			CHECK = 24,
			CODEDENUM = 25,
			GENERIC_BITSTRING = 26,
			CONSTRUCTED = 27,
			ENTRY_TIME = 28,
			PHYCOMADDR = 29,
			CURRENCY = 30
		}

		public enum ModeValues
		{
			ON = 1,
			BLOCKED = 2,
			TEST = 3,
			TEST_BLOCKED = 4,
			OFF = 5
		}

		public enum HealthValues
		{
			OK = 1,
			WARNING = 2,
			ALARM = 3
		}

		/// <summary>
		/// The CDC class contains helper functions to create DataObject instances for the
		/// most common Common Data Classes.
		/// </summary>
		public class CDC 
		{
			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr CDC_SPS_create(string name, IntPtr parent, uint options);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr CDC_INS_create(string name, IntPtr parent, uint options);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr CDC_MV_create(string name, IntPtr parent, uint options, bool isIntegerNotFloat);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr CDC_INC_create(string name, IntPtr parent, uint options, uint controlOptions);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr CDC_LPL_create(string name, IntPtr parent, uint options);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr CDC_DPL_create(string name, IntPtr parent, uint options);

			public const int CDC_OPTION_DESC = (1 << 2);
			public const int CDC_OPTION_DESC_UNICODE = (1 << 3);
			public const int CDC_OPTION_AC_DLNDA = (1 << 4);
			public const int CDC_OPTION_AC_DLN = (1 << 5);

			// options that are only valid for DPL CDC
			public const int CDC_OPTION_DPL_HWREV = (1 << 17);
			public const int CDC_OPTION_DPL_SWREV = (1 << 18);
			public const int CDC_OPTION_DPL_SERNUM = (1 << 19);
			public const int CDC_OPTION_DPL_MODEL = (1 << 20);
			public const int CDC_OPTION_DPL_LOCATION = (1 << 21);

			// mandatory data attributes for LLN0 (e.g. LBL configRef)
			public const int CDC_OPTION_AC_LN0_M = (1 << 24);
			public const int CDC_OPTION_AC_LN0_EX = (1 << 25);
			public const int CDC_OPTION_AC_DLD_M = (1 << 26);


			public static DataObject Create_CDC_SPS(ModelNode parent, string name, uint options)
			{
				IntPtr self = CDC_SPS_create(name, parent.self, options);

				if (self != IntPtr.Zero)
					return new DataObject (self);
				else
					return null;
			}

			public static DataObject Create_CDC_INS(ModelNode parent, string name, uint options)
			{
				IntPtr self = CDC_INS_create(name, parent.self, options);

				if (self != IntPtr.Zero)
					return new DataObject (self);
				else
					return null;
			}

			public static DataObject Create_CDC_MV(ModelNode parent, string name, uint options, bool isIntegerNotFloat)
			{
				IntPtr self = CDC_MV_create(name, parent.self, options, isIntegerNotFloat);

				if (self != IntPtr.Zero)
					return new DataObject (self);
				else
					return null;
			}

			public static DataObject Create_CDC_INC(ModelNode parent, string name, uint options, uint controlOptions)
			{
				IntPtr self = CDC_INC_create(name, parent.self, options, controlOptions);

				if (self != IntPtr.Zero)
					return new DataObject (self);
				else
					return null;
			}

			public static DataObject Create_CDC_LPL(ModelNode parent, string name, uint options)
			{
				IntPtr self = CDC_LPL_create(name, parent.self, options);

				if (self != IntPtr.Zero)
					return new DataObject (self);
				else
					return null;
			}

			public static DataObject Create_CDC_DPL(ModelNode parent, string name, uint options)
			{
				IntPtr self = CDC_DPL_create(name, parent.self, options);

				if (self != IntPtr.Zero)
					return new DataObject (self);
				else
					return null;
			}
		}

		public class DataObject : ModelNode
		{
			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr DataObject_create(string name, IntPtr parent, int arrayElements);

			internal DataObject(IntPtr self) : base(self)
			{
			}

			public DataObject(string name, ModelNode parent) : this(name, parent, 0)
			{
			}

			public DataObject(string name, ModelNode parent, int arrayElements)
			{
				self = DataObject_create (name, parent.self, arrayElements);
			}

		}

		public class DataAttribute : ModelNode
		{

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr DataAttribute_create(string name, IntPtr parent, int type, int fc,
				byte triggerOptions, int arrayElements, UInt32 sAddr);

			internal DataAttribute(IntPtr self) : base(self)
			{
			}

			public DataAttribute (string name, ModelNode parent, DataAttributeType type, FunctionalConstraint fc, TriggerOptions trgOps,
				int arrayElements, UInt32 sAddr)
			{
				self = DataAttribute_create (name, parent.self, (int)type, (int)fc, (byte)trgOps, arrayElements, sAddr);
			}

		}

		public class ModelNode
		{
			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr ModelNode_getChild(IntPtr self, string name);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern int ModelNode_getType(IntPtr self);

			public IntPtr self;

			internal ModelNode()
			{
			}

			public ModelNode(IntPtr self)
			{
				this.self = self;
			}

			public ModelNode GetChild(string name)
			{
				IntPtr childPtr = ModelNode_getChild(self, name);

				if (childPtr == IntPtr.Zero)
					return null;

				int nodeType = ModelNode_getType (childPtr);

				switch (nodeType) {
				case 0:
					return new LogicalDevice (childPtr);

				case 1:
					return new LogicalNode (childPtr);

				case 2:
					return new DataObject (childPtr);

				case 3:
					return new DataAttribute (childPtr);

				default:
					return new ModelNode (childPtr);
				}

			}
		}



		public class DataSet
		{
			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr DataSet_create(string name, IntPtr parent);

			public IntPtr self = IntPtr.Zero;

			public DataSet(string name, LogicalNode parent)
			{
				self = DataSet_create(name, parent.self);
			}
		}

		public class DataSetEntry
		{
			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr DataSetEntry_create(IntPtr dataSet, string variable, int index, string component);

			public IntPtr self = IntPtr.Zero;

			public DataSetEntry(DataSet dataSet, string variable, int index, string component)
			{
				self = DataSetEntry_create(dataSet.self, variable, index, component);
			}
		}

		public class ReportControlBlock
		{
			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr ReportControlBlock_create(string name, IntPtr parent, string rptId, bool isBuffered,
				string dataSetName, uint confRef, byte trgOps, byte options, uint bufTm, uint intgPd);

			public IntPtr self = IntPtr.Zero;

			public ReportControlBlock(string name, LogicalNode parent, string rptId, bool isBuffered,
				string dataSetName, uint confRef, byte trgOps, byte options, uint bufTm, uint intgPd)
			{
				self = ReportControlBlock_create(name, parent.self, rptId, isBuffered, dataSetName, confRef, trgOps, options, bufTm, intgPd);
			}
		}

		public class ClientConnection 
		{
			

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr ClientConnection_getPeerAddress(IntPtr self);

			internal IntPtr self;

			internal ClientConnection (IntPtr self) {
				this.self = self;
			}

			public string GetPeerAddress()
			{
				IntPtr peerAddrPtr = ClientConnection_getPeerAddress (self);

				if (peerAddrPtr != IntPtr.Zero)
					return Marshal.PtrToStringAnsi (peerAddrPtr);
				else
					return null;
			}
		}

		public delegate MmsDataAccessError WriteAccessHandler (DataAttribute dataAttr, MmsValue value, 
			ClientConnection connection, object parameter);

		public enum ControlHandlerResult {
			/// <summary>
			/// check or operation failed
			/// </summary>
			FAILED = 0,
			/// <summary>
			/// check or operation was successful
			/// </summary>
			OK = 1,
			/// <summary>
			/// check or operation is in progress
			/// </summary>
			WAITING = 2
		}

		public delegate ControlHandlerResult ControlWaitForExecutionHandler (DataObject controlObject, object parameter, MmsValue ctlVal, bool test, bool synchroCheck);

		public delegate ControlHandlerResult ControlHandler (DataObject controlObject, object parameter, MmsValue ctlVal, bool test);

		public enum CheckHandlerResult {
			/// <summary>
			/// check passed
			/// </summary>
			ACCEPTED = -1,
			/// <summary>
			/// check failed due to hardware fault
			/// </summary>
			HARDWARE_FAULT = 1,
			/// <summary>
			/// control is already selected or operated
			/// </summary>
			TEMPORARILY_UNAVAILABLE = 2,
			/// <summary>
			/// check failed due to access control reason - access denied for this client or state
			/// </summary>
			OBJECT_ACCESS_DENIED = 3,
			/// <summary>
			/// object not visible in this security context ??? 
			/// </summary>
			OBJECT_UNDEFINED = 4
		}

		public delegate CheckHandlerResult CheckHandler (DataObject controlObject, object parameter, MmsValue ctlVal, bool test, bool interlockCheck, 
			ClientConnection connection);

		/// <summary>
		/// This class acts as the entry point for the IEC 61850 client API. It represents a single
		/// (MMS) connection to a server.
		/// </summary>
		public class IedServer
		{
			[DllImport ("iec61850", CallingConvention=CallingConvention.Cdecl)]
			static extern IntPtr IedServer_create(IntPtr modelRef);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void IedServer_setLocalIpAddress(IntPtr self, string localIpAddress);

			[DllImport ("iec61850", CallingConvention=CallingConvention.Cdecl)]
			static extern void IedServer_start(IntPtr self, int tcpPort);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void IedServer_stop(IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void IedServer_destroy(IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern bool IedServer_isRunning(IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void IedServer_lockDataModel(IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void IedServer_unlockDataModel(IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void IedServer_updateAttributeValue(IntPtr self, IntPtr DataAttribute, IntPtr MmsValue);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void IedServer_updateBooleanAttributeValue(IntPtr self, IntPtr dataAttribute, bool value);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void IedServer_updateInt32AttributeValue(IntPtr self, IntPtr dataAttribute, int value);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void IedServer_updateInt64AttributeValue(IntPtr self, IntPtr dataAttribute, Int64 value);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void IedServer_updateFloatAttributeValue(IntPtr self, IntPtr dataAttribute, float value);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void IedServer_updateVisibleStringAttributeValue(IntPtr self, IntPtr dataAttribute, string value);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void IedServer_updateUTCTimeAttributeValue(IntPtr self, IntPtr dataAttribute, ulong value);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void IedServer_updateTimestampAttributeValue(IntPtr self, IntPtr dataAttribute, IntPtr timestamp);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void IedServer_updateQuality(IntPtr self, IntPtr dataAttribute, ushort value);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr IedServer_getAttributeValue(IntPtr self, IntPtr dataAttribute);

			[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
			private delegate int InternalControlPerformCheckHandler (IntPtr parameter, IntPtr ctlVal, bool test, bool interlockCheck, IntPtr connection);

			[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
			private delegate int InternalControlWaitForExecutionHandler (IntPtr parameter, IntPtr ctlVal, bool test, bool synchoCheck);

			[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
			private delegate int InternalControlHandler (IntPtr parameter, IntPtr ctlVal, bool test);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void IedServer_setWaitForExecutionHandler(IntPtr self, IntPtr node, InternalControlWaitForExecutionHandler handler, IntPtr parameter);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void IedServer_setPerformCheckHandler(IntPtr self, IntPtr node, InternalControlPerformCheckHandler handler, IntPtr parameter);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void IedServer_setControlHandler (IntPtr self, IntPtr node, InternalControlHandler handler, IntPtr parameter);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void IedServer_setWriteAccessPolicy(IntPtr self, FunctionalConstraint fc, AccessPolicy policy);

			[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
			private delegate int InternalWriteAccessHandler (IntPtr dataAttribute, IntPtr value, IntPtr connection, IntPtr parameter);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void IedServer_handleWriteAccess(IntPtr self, IntPtr dataAttribute,
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
			private delegate void InternalConnectionHandler (IntPtr iedServer, IntPtr clientConnection, bool connected, IntPtr parameter);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void IedServer_setConnectionIndicationHandler(IntPtr self, InternalConnectionHandler handler, IntPtr parameter);

			private IntPtr self = IntPtr.Zero;



			private class ControlHandlerInfo {
				public DataObject controlObject = null;
				public GCHandle handle;

				public ControlHandler controlHandler = null;
				public object controlHandlerParameter = null;

				public CheckHandler checkHandler = null;
				public object checkHandlerParameter = null;

				public ControlWaitForExecutionHandler waitForExecHandler = null;
				public object waitForExecHandlerParameter = null;

				public ControlHandlerInfo(DataObject controlObject)
				{
					this.controlObject = controlObject;
					this.handle = GCHandle.Alloc(this);
				}
				~ControlHandlerInfo() {
					this.handle.Free();
				}
					
			}

			private Dictionary<DataObject, ControlHandlerInfo> controlHandlers = new Dictionary<DataObject, ControlHandlerInfo> ();

			int internalControlHandler (IntPtr parameter, IntPtr ctlVal, bool test)
			{
				GCHandle handle = GCHandle.FromIntPtr (parameter);

				ControlHandlerInfo info = (ControlHandlerInfo)handle.Target;

				if (info != null & info.controlHandler != null)
					return (int)info.controlHandler (info.controlObject, info.controlHandlerParameter, new MmsValue (ctlVal), test);
				else
					return (int)ControlHandlerResult.FAILED;
			}

			int internalCheckHandler(IntPtr parameter, IntPtr ctlVal, bool test, bool interlockCheck, IntPtr connection)
			{
				GCHandle handle = GCHandle.FromIntPtr (parameter);

				ControlHandlerInfo info = (ControlHandlerInfo)handle.Target;

				if (info != null & info.checkHandler != null) {
					ClientConnection con = null;

					clientConnections.TryGetValue (connection, out con);

					return (int)info.checkHandler (info.controlObject, info.checkHandlerParameter, new MmsValue (ctlVal), test, interlockCheck, con); 
				} else
					return (int)CheckHandlerResult.OBJECT_UNDEFINED;
			}

			int internalControlWaitForExecutionHandler (IntPtr parameter, IntPtr ctlVal, bool test, bool synchoCheck)
			{
				GCHandle handle = GCHandle.FromIntPtr (parameter);

				ControlHandlerInfo info = (ControlHandlerInfo)handle.Target;

				if (info != null & info.waitForExecHandler != null) {
					return (int)info.waitForExecHandler (info.controlObject, info.waitForExecHandlerParameter, new MmsValue (ctlVal), test, synchoCheck);
				} 
				else
					return (int)ControlHandlerResult.FAILED;
			}

			private struct WriteAccessHandlerInfo {
				public WriteAccessHandler handler;
				public object parameter;
				public DataAttribute dataAttribute;

				public WriteAccessHandlerInfo (WriteAccessHandler h, object p, DataAttribute da) 
				{
					handler = h;
					parameter = p;
					dataAttribute = da;
				}
			}

			int writeAccessHandler (IntPtr dataAttribute, IntPtr value, IntPtr connection, IntPtr parameter)
			{
				//object info = writeAccessHandlers.Item [dataAttribute];
				WriteAccessHandlerInfo info;

				writeAccessHandlers.TryGetValue (dataAttribute, out info);

				ClientConnection con = null;

				clientConnections.TryGetValue (connection, out con);

				return (int) info.handler (info.dataAttribute, new MmsValue (value), con, info.parameter);
			}

			private Dictionary<IntPtr, WriteAccessHandlerInfo> writeAccessHandlers = new Dictionary<IntPtr, WriteAccessHandlerInfo> ();

			private void connectionIndicationHandler (IntPtr iedServer, IntPtr clientConnection, bool connected, IntPtr parameter)
			{
				if (connected == false) {
					ClientConnection con = null;

					clientConnections.TryGetValue (clientConnection, out con);

					if (con != null) {
						
						if (connectionHandler != null)
							connectionHandler (this, con, false, connectionHandlerParameter);

						clientConnections.Remove (clientConnection);
					}
				} else {
					ClientConnection con = new ClientConnection (clientConnection);

					clientConnections.Add (clientConnection, con);

					if (connectionHandler != null)
						connectionHandler (this, con, true, connectionHandlerParameter);
				}
			}

			private Dictionary<IntPtr, ClientConnection> clientConnections = new Dictionary<IntPtr, ClientConnection> ();

			public IedServer(IedModel iedModel)
			{
				self = IedServer_create(iedModel.self);
			}

			// causes undefined behavior
			//~IedServer()
			//{
			//    if (self != IntPtr.Zero)
			//    {
			//        IedServer_destroy(self);
			//    }
			//}

			private InternalConnectionHandler internalConnectionHandler = null;

			/// <summary>
			/// Sets the local ip address for listening
			/// </summary>
			/// <param name="localIpAddress">Local IP address.</param>
			public void SetLocalIpAddress(string localIpAddress)
			{
				IedServer_setLocalIpAddress (self, localIpAddress);
			}

			/// <summary>
			/// Start MMS server
			/// </summary>
			/// <param name="localIpAddress">Local IP address.</param>
			/// <param name="tcpPort">TCP port to use</param>
			public void Start(string localIpAddress, int tcpPort)
			{
				SetLocalIpAddress (localIpAddress);
				Start (tcpPort);
			}

			/// <summary>Start MMS server</summary>
			/// <param name="tcpPort">TCP port to use</param>
			public void Start(int tcpPort)
			{
				if (internalConnectionHandler == null)
					internalConnectionHandler = new InternalConnectionHandler (connectionIndicationHandler);					

				IedServer_setConnectionIndicationHandler (self, internalConnectionHandler, IntPtr.Zero);

				IedServer_start(self, tcpPort);
			}

			/// <summary>Start MMS server</summary>
			public void Start ()
			{
				Start(102);
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
			/// Release all server resources.
			/// </summary>
			/// <description>This function releases all MMS server resources.</description>
			public void Destroy()
			{
				IedServer_destroy(self);
				self = IntPtr.Zero;
				internalConnectionHandler = null;
			}

			public bool IsRunning()
			{
				return IedServer_isRunning(self);
			}

			private ControlHandlerInfo GetControlHandlerInfo(DataObject controlObject)
			{
				ControlHandlerInfo info;

				controlHandlers.TryGetValue (controlObject, out info);

				if (info == null) {
					info = new ControlHandlerInfo (controlObject);
					controlHandlers.Add (controlObject, info);
				}

				return info;
			}

			public void SetControlHandler (DataObject controlObject, ControlHandler handler, object parameter)
			{
				ControlHandlerInfo info = GetControlHandlerInfo (controlObject);

				info.controlHandler = handler;
				info.controlHandlerParameter = parameter;

				IedServer_setControlHandler(self, controlObject.self, internalControlHandler, GCHandle.ToIntPtr(info.handle));
			}

			public void SetCheckHandler (DataObject controlObject, CheckHandler handler, object parameter)
			{
				ControlHandlerInfo info = GetControlHandlerInfo (controlObject);

				info.checkHandler = handler;
				info.checkHandlerParameter = parameter;

				IedServer_setPerformCheckHandler(self, controlObject.self, internalCheckHandler, GCHandle.ToIntPtr(info.handle));
			}

			public void SetWaitForExecutionHandler (DataObject controlObject, ControlWaitForExecutionHandler handler, object parameter)
			{
				ControlHandlerInfo info = GetControlHandlerInfo (controlObject);

				info.waitForExecHandler = handler;
				info.waitForExecHandlerParameter = parameter;

				IedServer_setWaitForExecutionHandler(self, controlObject.self, internalControlWaitForExecutionHandler, GCHandle.ToIntPtr(info.handle));
			}
				
			public void HandleWriteAccess (DataAttribute dataAttr, WriteAccessHandler handler, object parameter)
			{
				writeAccessHandlers.Add (dataAttr.self, new WriteAccessHandlerInfo(handler, parameter, dataAttr));
				//writeAccessHandlers.Item [dataAttr.self] = handler;

				IedServer_handleWriteAccess (self, dataAttr.self, writeAccessHandler, IntPtr.Zero);
			}

			public void SetWriteAccessPolicy(FunctionalConstraint fc, AccessPolicy policy)
			{
				IedServer_setWriteAccessPolicy (self, fc, policy);
			}
		

			public void LockDataModel()
			{
				IedServer_lockDataModel(self);
			}

			public void UnlockDataModel()
			{
				IedServer_unlockDataModel(self);
			}

			public void UpdateAttributeValue(DataAttribute dataAttr, MmsValue value)
			{
				IedServer_updateAttributeValue (self, dataAttr.self, value.valueReference);
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
				IedServer_updateInt64AttributeValue (self, dataAttr.self, value);
			}

			public void UpdateVisibleStringAttributeValue(DataAttribute dataAttr, string value)
			{
				IedServer_updateVisibleStringAttributeValue(self, dataAttr.self, value);
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
				IedServer_updateTimestampAttributeValue (self, dataAttr.self, timestamp.timestampRef);
			}

			public void UpdateQuality(DataAttribute dataAttr, ushort value)
			{
				IedServer_updateQuality(self, dataAttr.self, value);
			}

			public MmsValue GetAttributeValue(DataAttribute dataAttr)
			{
				IntPtr mmsValuePtr = IedServer_getAttributeValue (self, dataAttr.self);

				if (mmsValuePtr != IntPtr.Zero)
					return new MmsValue (mmsValuePtr);
				else
					return null;
			}
		}

	}
}
