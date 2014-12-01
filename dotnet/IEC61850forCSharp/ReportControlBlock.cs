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

		public delegate void ReportHandler (Report report, object parameter);

		public class ReportControlBlock
		{
			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr ClientReportControlBlock_create (string dataAttributeReference);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr IedConnection_getRCBValues (IntPtr connection, out int error, string rcbReference, IntPtr updateRcb);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void IedConnection_setRCBValues (IntPtr connection, out int error, IntPtr rcb, UInt32 parametersMask, bool singleRequest);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern bool ClientReportControlBlock_isBuffered (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr ClientReportControlBlock_getRptId (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void ClientReportControlBlock_setRptId (IntPtr self, string rptId);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern bool ClientReportControlBlock_getRptEna (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void ClientReportControlBlock_setRptEna (IntPtr self, bool rptEna);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
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
			static extern bool ClientReportControlBlock_getGI (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void ClientReportControlBlock_setGI (IntPtr self, bool gi);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
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
				Console.WriteLine ("internalReportHandler called " + this);

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
		
			public void InstallReportHandler (ReportHandler reportHandler, object parameter)
			{
				this.reportHandler = new ReportHandler(reportHandler);

				Console.WriteLine("Installed report handler " + this.reportHandler);

				this.reportHandlerParameter = parameter;

				if (reportHandlerInstalled == false) {
					IedConnection_installReportHandler (this.connection, objectReference, this.GetRptId (), new InternalReportHandler(internalReportHandler), IntPtr.Zero);
					reportHandlerInstalled = true;
				}
			}

			public void GetRCBValues ()
			{
				int error;

				IedConnection_getRCBValues (connection, out error, objectReference, self);

				if (error != 0)
					throw new IedConnectionException ("getRCBValues service failed", error);
			}

			public void SetRCBValues ()
			{
				SetRCBValues (true);
			}

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
			}

			public bool IsBuffered ()
			{
				return ClientReportControlBlock_isBuffered (self);
			}

			public UInt64 getEntryTime ()
			{
				return ClientReportControlBlock_getEntryTime (self);
			}

			public DateTimeOffset GetEntryTimeAsDateTimeOffset ()
			{
				UInt64 entryTime = getEntryTime ();

				DateTimeOffset retVal = new DateTimeOffset (1970, 1, 1, 0, 0, 0, TimeSpan.Zero);

				return retVal.AddMilliseconds (entryTime);
			}

			public string GetDataSetReference ()
			{
				IntPtr dataSetRefPtr = ClientReportControlBlock_getDataSetReference (self);

				return Marshal.PtrToStringAnsi (dataSetRefPtr);
			}

			public void SetDataSetReference (string dataSetReference) 
			{
				ClientReportControlBlock_setDataSetReference (self, dataSetReference);

				flagDataSetReference = true;
			}

			public string GetRptId ()
			{
				IntPtr rptIdPtr = ClientReportControlBlock_getRptId (self);

				return Marshal.PtrToStringAnsi (rptIdPtr);
			}

			public bool GetRptEna ()
			{
				return ClientReportControlBlock_getRptEna (self);
			}

			public void SetRptEna (bool rptEna)
			{
				ClientReportControlBlock_setRptEna (self, rptEna);
				flagRptEna = true;
			}

			public UInt32 GetBufTm() 
			{
				return ClientReportControlBlock_getBufTm (self);
			}

			public void SetBufTm (UInt32 bufTm)
			{
				ClientReportControlBlock_setBufTm (self, bufTm);

				flagBufTm = true;
			}

			public bool GetGI ()
			{
				return ClientReportControlBlock_getGI (self);
			}

			public void SetGI (bool GI)
			{
				ClientReportControlBlock_setGI (self, GI);
				flagGI = true;
			}

			public bool GetResv ()
			{
				return ClientReportControlBlock_getResv (self);
			}

			public void SetResv (bool resv)
			{
				ClientReportControlBlock_setResv (self, resv);
				flagResv = true;
			}

			public TriggerOptions GetTrgOps()
			{
				return (TriggerOptions) ClientReportControlBlock_getTrgOps (self);
			}

			public void SetTrgOps(TriggerOptions trgOps)
			{
				ClientReportControlBlock_setTrgOps (self, (int) trgOps);

				flagTrgOps = true;
			}

			public UInt32 GetIntgPd ()
			{
				return ClientReportControlBlock_getIntgPd (self);
			}

			public void SetIntgPd (UInt32 intgPd)
			{
				ClientReportControlBlock_setIntgPd (self, intgPd);
				flagIntgPd = true;
			}

			public ReportOptions GetOptFlds()
			{
				return (ReportOptions) ClientReportControlBlock_getOptFlds (self);
			}

			public void SetOptFlds(ReportOptions optFlds)
			{
				ClientReportControlBlock_setOptFlds (self, (int)optFlds);

				flagOptFlds = true;
			}
		}

	}
}
