/*
 *  Reporting.cs
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

namespace IEC61850
{
	namespace Client
	{
		public partial class IedConnection
		{
			public ReportControlBlock getReportControlBlock (string rcbObjectReference)
			{
				return new ReportControlBlock (rcbObjectReference, connection);
			}
		}

		public enum ReasonForInclusion
		{
			/** the element is not included in the received report */
			REASON_NOT_INCLUDED = 0,

			/** the element is included due to a change of the data value */
			REASON_DATA_CHANGE = 1,

			/** the element is included due to a change in the quality of data */
			REASON_QUALITY_CHANGE = 2,

			/** the element is included due to an update of the data value */
			REASON_DATA_UPDATE = 3,

			/** the element is included due to a periodic integrity report task */
			REASON_INTEGRITY = 4,

			/** the element is included due to a general interrogation by the client */
			REASON_GI = 5,

			/** the reason for inclusion is unknown */
			REASON_UNKNOWN = 6
		}

		public class Report
		{

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern bool ClientReport_hasTimestamp (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern UInt64 ClientReport_getTimestamp (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr ClientReport_getDataSetValues(IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern int ClientReport_getReasonForInclusion(IntPtr self, int elementIndex);

			private IntPtr self;

			private IntPtr dataSetValues = IntPtr.Zero;

			private MmsValue values = null;

			internal Report (IntPtr self)
			{
				this.self = self;
			}

			public bool HasTimestamp ()
			{
				return ClientReport_hasTimestamp (self);
			}

			public UInt64 GetTimestamp ()
			{
				if (HasTimestamp ())
					return ClientReport_getTimestamp (self);
				else
					return 0;
			}

			public MmsValue GetDataSetValues ()
			{
				if (dataSetValues == IntPtr.Zero) {
					dataSetValues = ClientReport_getDataSetValues(self);

					if (dataSetValues == IntPtr.Zero)
						throw new IedConnectionException("No report values available yet");

					values = new MmsValue(dataSetValues);
				}

				return values;
			}

			public ReasonForInclusion GetReasonForInclusion (int index)
			{
				if (values == null) {
					GetDataSetValues ();

					if (values == null) 
						throw new IedConnectionException("No ReasonForInclusion available yet");
				}

				int dataSetSize = values.Size();

				if (index >= dataSetSize)
					throw new IedConnectionException("data set index out of range (count = " + dataSetSize + ")");

				return (ReasonForInclusion) ClientReport_getReasonForInclusion(self, index);
			}

		}

	}
}

