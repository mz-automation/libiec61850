/*
 *  Reporting.cs
 *
 *  Copyright 2014-2018 Michael Zillgith
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
using System.Collections.Generic;

namespace IEC61850
{
    namespace Client
    {
        public partial class IedConnection
        {

            private List<ReportControlBlock> activeRCBs = null;

            public ReportControlBlock GetReportControlBlock(string rcbObjectReference)
            {
                var newRCB = new ReportControlBlock(rcbObjectReference, this, connection);

                if (activeRCBs == null)
                    activeRCBs = new List<ReportControlBlock>();

                activeRCBs.Add(newRCB);

                return newRCB;
            }

            internal void RemoveRCB(ReportControlBlock rcb)
            {
                if (activeRCBs != null)
                {
                    activeRCBs.Remove(rcb);
                }
            }

        }

        [Flags]
        public enum ReasonForInclusion
        {
            /** the element is not included in the received report */
            REASON_NOT_INCLUDED = 0,

            /** the element is included due to a change of the data value */
            REASON_DATA_CHANGE = 1,

            /** the element is included due to a change in the quality of data */
            REASON_QUALITY_CHANGE = 2,

            /** the element is included due to an update of the data value */
            REASON_DATA_UPDATE = 4,

            /** the element is included due to a periodic integrity report task */
            REASON_INTEGRITY = 8,

            /** the element is included due to a general interrogation by the client */
            REASON_GI = 16,

            /** the reason for inclusion is unknown */
            REASON_UNKNOWN = 32
        }

        /// <summary>
        /// A class to hold the contents of a received report
        /// </summary>
		public class Report
        {

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            [return: MarshalAs(UnmanagedType.I1)]
            static extern bool ClientReport_hasTimestamp(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern UInt64 ClientReport_getTimestamp(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr ClientReport_getDataSetValues(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern int ClientReport_getReasonForInclusion(IntPtr self, int elementIndex);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            [return: MarshalAs(UnmanagedType.I1)]
            static extern bool ClientReport_hasSeqNum(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern UInt16 ClientReport_getSeqNum(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            [return: MarshalAs(UnmanagedType.I1)]
            static extern bool ClientReport_hasDataSetName(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            [return: MarshalAs(UnmanagedType.I1)]
            static extern bool ClientReport_hasReasonForInclusion(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            [return: MarshalAs(UnmanagedType.I1)]
            static extern bool ClientReport_hasConfRev(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern UInt32 ClientReport_getConfRev(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            [return: MarshalAs(UnmanagedType.I1)]
            static extern bool ClientReport_hasBufOvfl(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            [return: MarshalAs(UnmanagedType.I1)]
            static extern bool ClientReport_getBufOvfl(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            [return: MarshalAs(UnmanagedType.I1)]
            static extern bool ClientReport_hasDataReference(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr ClientReport_getRcbReference(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr ClientReport_getDataSetName(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr ClientReport_getRptId(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr ClientReport_getEntryId(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr ClientReport_getDataReference(IntPtr self, int elementIndex);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern bool ClientReport_hasSubSeqNum(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern UInt16 ClientReport_getSubSeqNum(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern bool ClientReport_getMoreSeqmentsFollow(IntPtr self);

            private IntPtr self;

            private IntPtr dataSetValues = IntPtr.Zero;

            private MmsValue values = null;

            internal Report(IntPtr self)
            {
                this.self = self;
            }

            /// <summary>
            /// Determines whether the report has a timestamp.
            /// </summary>
            /// <returns>
            /// <c>true</c> if this report has a timestamp; otherwise, <c>false</c>.
            /// </returns>
			public bool HasTimestamp()
            {
                return ClientReport_hasTimestamp(self);
            }

            /// <summary>
            /// Gets the timestamp.
            /// </summary>
            /// <returns>
            /// The timestamp as milliseconds since 1.1.1970 UTC 00:00 or 0 if no timestamp is present.
            /// </returns>
			public UInt64 GetTimestamp()
            {
                if (HasTimestamp())
                    return ClientReport_getTimestamp(self);
                else
                    return 0;
            }

            public bool HasDataSetName()
            {
                return ClientReport_hasDataSetName(self);
            }

            public bool HasDataReference()
            {
                return ClientReport_hasDataReference(self);
            }

            public bool HasConfRev()
            {
                return ClientReport_hasConfRev(self);
            }

            public UInt32 GetConfRev()
            {
                return ClientReport_getConfRev(self);
            }

            public bool HasBufOvfl()
            {
                return ClientReport_hasBufOvfl(self);
            }

            public bool GetBufOvfl()
            {
                return ClientReport_getBufOvfl(self);
            }

            /// <summary>
            /// Indicates if the report contains a sequence number (SeqNum) field
            /// </summary>
            /// <returns><c>true</c> if this instance has SeqNum; otherwise, <c>false</c>.</returns>
            public bool HasSeqNum()
            {
                return ClientReport_hasSeqNum(self);
            }

            /// <summary>
            /// Gets the value of the SeqNum field
            /// </summary>
            /// <returns>The report sequence number</returns>
            public UInt16 GetSeqNum()
            {
                return ClientReport_getSeqNum(self);
            }

            /// <summary>
            /// Indicates if the report contains a sub sequence number (SubSeqNum) and more segments follow (MoreSegmentsFollow) field
            /// </summary>
            /// <returns><c>true</c> if this instance has SubSeqNum and MoreSegmentsFollow; otherwise, <c>false</c>.</returns>
            public bool HasSubSeqNum()
            {
                return ClientReport_hasSubSeqNum(self);
            }

            /// <summary>
            /// Gets the sub sequence number (SubSeqNum) value of a segmented report
            /// </summary>
            /// <returns>The sub sequence number.</returns>
            public UInt16 GetSubSeqNum()
            {
                return ClientReport_getSubSeqNum(self);
            }

            /// <summary>
            /// Gets the more segments follow (MoreSegmentsFollow) flag
            /// </summary>
            /// <returns><c>true</c>, if more report segments follow, <c>false</c> otherwise.</returns>
            public bool GetMoreSegmentsFollow()
            {
                return ClientReport_getMoreSeqmentsFollow(self);
            }

            /// <summary>
            /// Determines whether this report contains reason for inclusion information
            /// </summary>
            /// <returns><c>true</c> if this report contains reason for inclusion information; otherwise, <c>false</c>.</returns>
            public bool HasReasonForInclusion()
            {
                return ClientReport_hasReasonForInclusion(self);
            }

            /// <summary>
            /// Gets the data set values as MMS_ARRAY instance.
            /// </summary>
            /// <returns>
            /// The data set values.
            /// </returns>
			public MmsValue GetDataSetValues()
            {
                if (dataSetValues == IntPtr.Zero)
                {
                    dataSetValues = ClientReport_getDataSetValues(self);

                    if (dataSetValues == IntPtr.Zero)
                        throw new IedConnectionException("No report values available yet");

                    values = new MmsValue(dataSetValues);
                }

                return values.Clone();
            }

            /// <summary>
            /// Gets the reason for inclusion of data set member with the given index
            /// </summary>
            /// <returns>
            /// The reason for inclusion.
            /// </returns>
            /// <param name='index'>
            /// index of the data set member in the data set
            /// </param>
			public ReasonForInclusion GetReasonForInclusion(int index)
            {
                if (values == null)
                {
                    GetDataSetValues();

                    if (values == null)
                        throw new IedConnectionException("No ReasonForInclusion available yet");
                }

                int dataSetSize = values.Size();

                if (index >= dataSetSize)
                    throw new IedConnectionException("data set index out of range (count = " + dataSetSize + ")");

                return (ReasonForInclusion)ClientReport_getReasonForInclusion(self, index);
            }

            public string GetRcbReference()
            {
                IntPtr rcbRef = ClientReport_getRcbReference(self);

                return Marshal.PtrToStringAnsi(rcbRef);
            }

            public string GetDataSetName()
            {
                IntPtr dataSetName = ClientReport_getDataSetName(self);

                return Marshal.PtrToStringAnsi(dataSetName);
            }

            /// <summary>
            /// Gets the data reference for the specified data set element
            /// </summary>
            /// <returns>
            /// The data reference or null if the data reference is not available
            /// </returns>
            /// <param name='index'>
            /// index of the data set element starting with 0
            /// </param>
            public string GetDataReference(int index)
            {
                IntPtr dataRef = ClientReport_getDataReference(self, index);

                if (dataRef != IntPtr.Zero)
                    return Marshal.PtrToStringAnsi(dataRef);
                else
                    return null;
            }

            public string GetRptId()
            {
                IntPtr rptId = ClientReport_getRptId(self);

                if (rptId == IntPtr.Zero)
                    return GetRcbReference();
                else
                    return Marshal.PtrToStringAnsi(rptId);
            }

            /// <summary>
            /// Gets the EntryID of this report.
            /// </summary>
            /// <returns>The entryID as a byte array representing an MMS octet string.</returns>
            public byte[] GetEntryId()
            {
                IntPtr entryIdRef = ClientReport_getEntryId(self);

                if (entryIdRef == IntPtr.Zero)
                    return null;
                else
                {
                    MmsValue entryId = new MmsValue(entryIdRef);

                    return entryId.getOctetString();
                }
            }

        }

    }
}

