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
        public static class SqliteLogStorage
        {
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr SqliteLogStorage_createInstance(string filename);

            /// <summary>
            /// Create a new LogStorage instance using the embedded sqlite database
            /// </summary>
            /// <param name="filename">name of the sqlite database file to be used</param>
            /// <returns></returns>
            public static LogStorage CreateLogStorage(string filename)
            {
                try
                {
                    IntPtr nativeInstance = SqliteLogStorage_createInstance(filename);

                    if (nativeInstance != IntPtr.Zero)
                        return new LogStorage(nativeInstance);
                    else
                        return null;
                }
                catch (EntryPointNotFoundException ex)
                {
                    Console.WriteLine(ex.Message + " Make sure that the libiec61850.dll was built with sqLite!");
                    return null;
                }
            }
        }

    }
}