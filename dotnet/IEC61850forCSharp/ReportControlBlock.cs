/*
 *  ReportControlBlock.cs
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
using System.Diagnostics;

using IEC61850.Common;

namespace IEC61850
{
	namespace Client
	{

        /// <summary>
        /// Report handler.
        /// </summary>
		public delegate void ReportHandler (Report report, object parameter);

        /// <summary>
        /// Report control block (RCB) representation.
        /// </summary>
        /// <description>
        /// This class is used as a client side representation (copy) of a report control block (RCB).
        /// Values from the server will only be read when the GetRCBValues method is called.
        /// Values at the server are only affected when the SetRCBValues method is called.
        /// </description>
		public class ReportControlBlock
		{
			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr ClientReportControlBlock_create (string dataAttributeReference);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr IedConnection_getRCBValues (IntPtr connection, out int error, string rcbReference, IntPtr updateRcb);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void IedConnection_setRCBValues (IntPtr connection, out int error, IntPtr rcb, UInt32 parametersMask, bool singleRequest);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            [return: MarshalAs(UnmanagedType.I1)]
			static extern bool ClientReportControlBlock_isBuffered (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr ClientReportControlBlock_getRptId (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void ClientReportControlBlock_setRptId (IntPtr self, string rptId);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            [return: MarshalAs(UnmanagedType.I1)]
			static extern bool ClientReportControlBlock_getRptEna (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void ClientReportControlBlock_setRptEna(IntPtr self, bool rptEna);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            [return: MarshalAs(UnmanagedType.I1)]
			static extern bool ClientReportControlBlock_getResv (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void ClientReportControlBlock_setResv (IntPtr self, bool resv);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr ClientReportControlBlock_getDataSetReference (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void ClientReportControlBlock_setDataSetReference (IntPtr self, string dataSetReference);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern UInt32 ClientReportControlBlock_getConfRev (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern int ClientReportControlBlock_getOptFlds (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void ClientReportControlBlock_setOptFlds (IntPtr self, int optFlds);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern UInt32 ClientReportControlBlock_getBufTm (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void ClientReportControlBlock_setBufTm (IntPtr self, UInt32 bufTm);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern UInt16 ClientReportControlBlock_getSqNum (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern int ClientReportControlBlock_getTrgOps (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void ClientReportControlBlock_setTrgOps (IntPtr self, int trgOps);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern UInt32 ClientReportControlBlock_getIntgPd (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void ClientReportControlBlock_setIntgPd (IntPtr self, UInt32 intgPd);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            [return: MarshalAs(UnmanagedType.I1)]
			static extern bool ClientReportControlBlock_getGI (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void ClientReportControlBlock_setGI (IntPtr self, bool gi);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            [return: MarshalAs(UnmanagedType.I1)]
			static extern bool ClientReportControlBlock_getPurgeBuf (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void ClientReportControlBlock_setPurgeBuf (IntPtr self, bool purgeBuf);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern Int32 ClientReportControlBlock_getResvTms (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void ClientReportControlBlock_setResvTms (IntPtr self, Int16 resvTms);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr ClientReportControlBlock_getEntryId (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void ClientReportControlBlock_setEntryId (IntPtr self, IntPtr entryId);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern UInt64 ClientReportControlBlock_getEntryTime (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr ClientReportControlBlock_getOwner (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void IedConnection_installReportHandler (IntPtr connection, string rcbReference, string rptId, InternalReportHandler handler,
        		IntPtr handlerParameter);

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
			private delegate void InternalReportHandler (IntPtr parameter, IntPtr report);

			private IntPtr self;
			private IntPtr connection;
			private string objectReference;
			private bool flagRptId = false;
			private bool flagRptEna = false;
			private bool flagResv = false;
			private bool flagDataSetReference = false;
			private bool flagConfRev = false;
			private bool flagOptFlds = false;
			private bool flagBufTm = false;
			private bool flagSqNum = false;
			private bool flagTrgOps = false;
			private bool flagIntgPd = false;
			private bool flagGI = false;
			private bool flagPurgeBuf = false;
			private bool flagResvTms = false;
			private bool flagEntryId = false;

			private event ReportHandler reportHandler = null;
			private object reportHandlerParameter;
			private bool reportHandlerInstalled = false;
            private event InternalReportHandler internalHandler = null;

			private void resetSendFlags ()
			{
				flagRptId = false;
				flagRptEna = false;
				flagResv = false;
				flagDataSetReference = false;
				flagConfRev = false;
				flagOptFlds = false;
				flagBufTm = false;
				flagSqNum = false;
				flagTrgOps = false;
				flagIntgPd = false;
				flagGI = false;
				flagPurgeBuf = false;
				flagResvTms = false;
				flagEntryId = false;
			}

			private Report report = null;

			private void internalReportHandler (IntPtr parameter, IntPtr report)
			{
				try {

					if (this.report == null)
							this.report = new Report (report);

					if (reportHandler != null)
						reportHandler(this.report, reportHandlerParameter);

				} catch (Exception e) 
				{
					// older versions of mono 2.10 (for linux?) cause this exception
					Console.WriteLine(e.Message);
				}
			}

			internal ReportControlBlock (string objectReference, IntPtr connection)
			{
				self = ClientReportControlBlock_create (objectReference);
				this.connection = connection;
				this.objectReference = objectReference;
			}
		
            /// <summary>
            /// Installs the report handler.
            /// </summary>
            /// <description>
            /// This will install a callback handler (delegate) that is invoked whenever a report
            /// related this RCB is received. Any call of this method will replace an previously registered
            /// handler!
            /// </description>
            /// <param name='reportHandler'>
            /// report handler
            /// </param>
            /// <param name='parameter'>
            /// parameter is passed to the handler when the handler is invoked.
            /// </param>
			public void InstallReportHandler (ReportHandler reportHandler, object parameter)
			{
				this.reportHandler = new ReportHandler(reportHandler);

				this.reportHandlerParameter = parameter;

				if (reportHandlerInstalled == false) {

                    string reportId = this.GetRptId ();

                    if (internalHandler == null)
                    {
                        internalHandler =  new InternalReportHandler(internalReportHandler);
                    }

                    IedConnection_installReportHandler(this.connection, objectReference, reportId, internalHandler, IntPtr.Zero);
					reportHandlerInstalled = true;
				}
			}

            /// <summary>
            /// Read all RCB values from the server
            /// </summary>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
			public void GetRCBValues ()
			{
				int error;

				IedConnection_getRCBValues (connection, out error, objectReference, self);

				if (error != 0)
					throw new IedConnectionException ("getRCBValues service failed", error);
			}

            /// <summary>
            /// Write changed RCB values to the server.
            /// </summary>
            /// <description>
            /// This function will only write the RCB values that were set by one of the setter methods.
            /// The RCB values are sent by a single MMS write request.
            /// </description>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
			public void SetRCBValues ()
			{
				SetRCBValues (true);
			}

            /// <summary>
            /// Write changed RCB values to the server.
            /// </summary>
            /// <description>
            /// This function will only write the RCB values that were set by one of the setter methods.
            /// </description>
            /// <exception cref="IedConnectionException">This exception is thrown if there is a connection or service error</exception>
            /// <param name='singleRequest'>
            /// If true the values are sent by single MMS write request. Otherwise the values are all sent by their own MMS write requests.
            /// </param>
			public void SetRCBValues (bool singleRequest)
            {
                UInt32 parametersMask = 0;

                if (flagRptId)
                    parametersMask += 1;

                if (flagRptEna)
                    parametersMask += 2;

                if (flagResv)
                    parametersMask += 4;

                if (flagDataSetReference)
                    parametersMask += 8;

                if (flagConfRev)
                    parametersMask += 16;

                if (flagOptFlds)
                    parametersMask += 32;

                if (flagBufTm)
                    parametersMask += 64;

                if (flagSqNum)
                    parametersMask += 128;

                if (flagTrgOps)
                    parametersMask += 256;

                if (flagIntgPd)
                    parametersMask += 512;

                if (flagGI)
                    parametersMask += 1024;

                if (flagPurgeBuf)
                    parametersMask += 2048;

                if (flagEntryId)
                    parametersMask += 4096;

                if (flagResvTms)
                    parametersMask += 16384;

                int error;

                IedConnection_setRCBValues (connection, out error, self, parametersMask, singleRequest);

                if (error != 0)
                    throw new IedConnectionException ("setRCBValues service failed", error);

                if (flagRptId) {

                    if (reportHandlerInstalled) {
                        reportHandlerInstalled = false;
                        InstallReportHandler(this.reportHandler, this.reportHandlerParameter);
                    }
                }

                resetSendFlags();
			}

            /// <summary>
            /// Determines whether this instance is a buffered or unbuffered RCB.
            /// </summary>
            /// <returns>
            /// <c>true</c> if this instance is a buffered RCB; otherwise, <c>false</c>.
            /// </returns>
			public bool IsBuffered ()
			{
				return ClientReportControlBlock_isBuffered (self);
			}

            /// <summary>
            /// Gets the entry time of the RCB as ms time
            /// </summary>
            /// <description>
            /// The entry time is the timestamp of the last report sent.
            /// </description>
            /// <returns>
            /// The entry time as ms timestamp
            /// </returns>
			public UInt64 GetEntryTime ()
			{
				return ClientReportControlBlock_getEntryTime (self);
			}

            /// <summary>
            /// Gets the entry time of the RCB as DateTimeOffset
            /// </summary>
            /// <description>
            /// The entry time is the timestamp of the last report sent.
            /// </description>
            /// <returns>
            /// The entry time as DataTimeOffset
            /// </returns>
			public DateTimeOffset GetEntryTimeAsDateTimeOffset ()
			{
				UInt64 entryTime = GetEntryTime ();

				DateTimeOffset retVal = new DateTimeOffset (1970, 1, 1, 0, 0, 0, TimeSpan.Zero);

				return retVal.AddMilliseconds (entryTime);
			}

            /// <summary>
            /// Gets the data set reference of the associated data set
            /// </summary>
            /// <returns>
            /// The data set reference.
            /// </returns>
			public string GetDataSetReference ()
			{
				IntPtr dataSetRefPtr = ClientReportControlBlock_getDataSetReference (self);

				return Marshal.PtrToStringAnsi (dataSetRefPtr);
			}

            /// <summary>
            /// Sets the data set reference. Use this method to select the associated data set for the RCB
            /// </summary>
            /// <returns>
            /// The data set reference.
            /// </returns>
			public void SetDataSetReference (string dataSetReference) 
			{
				ClientReportControlBlock_setDataSetReference (self, dataSetReference);

				flagDataSetReference = true;
			}

            /// <summary>
            /// Gets the report identifier.
            /// </summary>
            /// <returns>
            /// The report identifier.
            /// </returns>
			public string GetRptId ()
			{
				IntPtr rptIdPtr = ClientReportControlBlock_getRptId (self);

				return Marshal.PtrToStringAnsi (rptIdPtr);
			}

            /// <summary>
            /// Sets the RptId (report ID) of the RCB
            /// </summary>
            /// <param name='rptId'>
            /// The new RptId
            /// </param>
            public void SetRptId (string rptId)
            {
                ClientReportControlBlock_setRptId(self, rptId);
                flagRptId = true;
            }

            /// <summary>
            /// Check if reporting is currently enabled
            /// </summary>
            /// <returns>
            /// true, if reporting is enabled, false otherwise
            /// </returns>
			public bool GetRptEna ()
			{
				return ClientReportControlBlock_getRptEna (self);
			}

            /// <summary>
            /// Sets report enable flag. Use this to enable reporting
            /// </summary>
            /// <param name='rptEna'>
            /// true to enable reporting, false to disable
            /// </param>
			public void SetRptEna (bool rptEna)
			{
				ClientReportControlBlock_setRptEna (self, rptEna);
				flagRptEna = true;
			}

            /// <summary>
            /// Get the purgeBuf flag of the report control block
            /// </summary>
            /// <returns>the prugeBuf value</returns>
            public bool GetPurgeBuf ()
            {
                return ClientReportControlBlock_getPurgeBuf(self);
            }

            /// <summary>
            /// Set the purgeBuf flag of the report control block
            /// </summary>
            /// <description>This is only for buffered RCBs. If set to true the report buffer of a buffered RCB will be cleaned.</description>
            /// <param name="purgeBuf">set to true to flush report buffer</param>
            public void SetPurgeBuf (bool purgeBuf)
            {
                ClientReportControlBlock_setPurgeBuf(self, purgeBuf);
                flagPurgeBuf = true;
            }

            /// <summary>
            /// Gets the buffer time.
            /// </summary>
            /// <returns>
            /// The buffer time in ms.
            /// </returns>
			public UInt32 GetBufTm() 
			{
				return ClientReportControlBlock_getBufTm (self);
			}

            /// <summary>
            /// Sets the buffer time.
            /// </summary>
            /// <param name='bufTm'>
            /// Buffer time is ms.
            /// </param>
			public void SetBufTm (UInt32 bufTm)
			{
				ClientReportControlBlock_setBufTm (self, bufTm);

				flagBufTm = true;
			}

            /// <summary>
            /// Gets the GI flag
            /// </summary>
            /// <returns>
            /// true, if GI flag is set
            /// </returns>
			public bool GetGI ()
			{
				return ClientReportControlBlock_getGI (self);
			}

            /// <summary>
            /// Sets the GI flag. Use this to trigger a GI (general interrogation) command.
            /// </summary>
            /// <param name='GI'>
            /// request general interrogation of true
            /// </param>
			public void SetGI (bool GI)
			{
				ClientReportControlBlock_setGI (self, GI);
				flagGI = true;
			}

            /// <summary>
            /// Check if RCB is reserved by a client
            /// </summary>
            /// <returns>
            /// true, the RCB is reserver by a client
            /// </returns>
			public bool GetResv ()
			{
				return ClientReportControlBlock_getResv (self);
			}

            /// <summary>
            /// Gets the configuration revision of the RCB
            /// </summary>
            /// <returns>
            /// The conf rev.
            /// </returns>
            public UInt32 GetConfRev ()
            {
                return ClientReportControlBlock_getConfRev (self);
            }

            /// <summary>
            /// Sets RESV flag. Use this to reserve (allocate) this RCB.
            /// </summary>
            /// <param name='resv'>
            /// true: reserver this RCB for exclusive use
            /// </param>
			public void SetResv (bool resv)
			{
				ClientReportControlBlock_setResv (self, resv);
				flagResv = true;
			}

            /// <summary>
            /// Gets the trigger options of the RCB
            /// </summary>
            /// <returns>
            /// trigger options
            /// </returns>
			public TriggerOptions GetTrgOps()
			{
				return (TriggerOptions) ClientReportControlBlock_getTrgOps (self);
			}

            /// <summary>
            /// Sets the trigger options of the RCB.
            /// </summary>
            /// <param name='trgOps'>
            /// trigger options
            /// </param>
			public void SetTrgOps(TriggerOptions trgOps)
			{
				ClientReportControlBlock_setTrgOps (self, (int) trgOps);

				flagTrgOps = true;
			}

            /// <summary>
            /// Gets the integrity period
            /// </summary>
            /// <returns>
            /// integrity period in ms
            /// </returns>
			public UInt32 GetIntgPd ()
			{
				return ClientReportControlBlock_getIntgPd (self);
			}

            /// <summary>
            /// Sets the integrity period
            /// </summary>
            /// <param name='intgPd'>
            /// integrity period in ms
            /// </param>
			public void SetIntgPd (UInt32 intgPd)
			{
				ClientReportControlBlock_setIntgPd (self, intgPd);
				flagIntgPd = true;
			}

            /// <summary>
            /// Gets the option fields. 
            /// </summary>
            /// <returns>
            /// The option fields
            /// </returns>
			public ReportOptions GetOptFlds()
			{
				return (ReportOptions) ClientReportControlBlock_getOptFlds (self);
			}

            /// <summary>
            /// Sets the option field. Used to enable or disable optional report elements
            /// </summary>
            /// <param name='optFlds'>
            /// Option field.
            /// </param>
			public void SetOptFlds(ReportOptions optFlds)
			{
				ClientReportControlBlock_setOptFlds (self, (int)optFlds);

				flagOptFlds = true;
			}
		}

	}
}
