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
        public class LogStorage : IDisposable
        {
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void LogStorage_setMaxLogEntries(IntPtr self, int maxEntries);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern int LogStorage_getMaxLogEntries(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern int LogStorage_addEntry(IntPtr self, long time);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern bool LogStorage_addEntryData(IntPtr self, int entryID, string dataRef, byte[] data, int dataSize, int reasonCode);

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            public delegate bool LogEntryCallback(IntPtr self, long timeStamp, long entryID, bool moreFollows);

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            public delegate bool LogEntryDataCallback(IntPtr self, string dataRef, byte[] data, int dataSize, int reasonCode, bool moreFollows);


            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern bool LogStorage_getEntries(IntPtr self, long startingTime, long endingTime, LogEntryCallback entryCallback, LogEntryDataCallback entryDataCallback, object parameter);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void LogStorage_destroy(IntPtr self);

            private IntPtr self;

            internal IntPtr GetNativeInstance()
            {
                return self;
            }

            internal LogStorage(IntPtr self)
            {
                this.self = self;
            }

            /// <summary>
            /// Set the maximum number of log entries for this log
            /// </summary>
            /// <param name="maxEntries">the maximum number of log entries</param>
            private void SetMaxLogEntries(int maxEntries)
            {
                LogStorage_setMaxLogEntries(self, maxEntries);
            }

            /// <summary>
            /// Get the maximum allowed number of log entries for this log
            /// </summary>
            /// <returns>the maximum number of log entries</returns>
            private int GetMaxLogEntries()
            {
                return LogStorage_getMaxLogEntries(self);
            }

            /// <summary>
            /// Manually add an entry to the log
            /// </summary>
            /// <remarks>Usually this has not to be done by the user
            /// but is done automatically by the library</remarks>
            /// <param name="time">the entry time of the new entry in ms</param>
            /// <returns>the entryID of the new log entry</returns>
            public int AddEntry(long time)
            {
                return LogStorage_addEntry(self, time);
            }

            /// <summary>
            /// Add new entry data to an existing log entry
            /// </summary>
            /// <param name="entryID">the ID of the log entry where the data will be added</param>
            /// <param name="dataRef">the data reference of the log entry data</param>
            /// <param name="data">the data content as an unstructured binary data block</param>
            /// <param name="dataSize">the size of the binary data block</param>
            /// <param name="reasonCode">the reasonCode of the LogEntryData</param>
            /// <returns>true if the entry data was successfully added, false otherwise</returns>
            public bool AddEntryData(int entryID, string dataRef, byte[] data, int dataSize, int reasonCode)
            {
                return LogStorage_addEntryData(self, entryID, dataRef, data, dataSize, reasonCode);
            }

            /// <summary>
            /// Get log entries specified by a time range
            /// </summary>
            /// <param name="startingTime">start time of the time range</param>
            /// <param name="endingTime">end time of the time range</param>
            /// <param name="entryCallback"></param>
            /// <param name="entryDataCallback"></param>
            /// <param name="parameter"></param>
            /// <returns></returns>
            public bool GetEntries(long startingTime, long endingTime, LogEntryCallback entryCallback, LogEntryDataCallback entryDataCallback, object parameter)
            {
                return LogStorage_getEntries(self, startingTime, endingTime, entryCallback, entryDataCallback, parameter);
            }

            /// <summary>
            /// The maximum allowed number of log entries in the log storage
            /// </summary>
            public int MaxLogEntries
            {
                get
                {
                    return GetMaxLogEntries();
                }

                set
                {
                    SetMaxLogEntries(value);
                }
            }

            public void Dispose()
            {
                lock (this)
                {
                    if (self != IntPtr.Zero)
                    {
                        LogStorage_destroy(self);
                        self = IntPtr.Zero;
                    }
                }
            }

            ~LogStorage()
            {
                Dispose();
            }
        }

    }
}