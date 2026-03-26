using System;
using System.Runtime.InteropServices;

namespace IEC61850
{
    namespace Common
    {
        public class LibIEC61850
        {
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr LibIEC61850_getVersionString();

            /// <summary>
            /// Get the version string of the native libiec61850 library
            /// </summary>
            /// <returns>The version string in format MAJOR.MINOR.PATCH</returns>
            public static string GetVersionString()
            {
                return Marshal.PtrToStringAnsi(LibIEC61850_getVersionString());
            }

            /// <summary>
            /// Converts millisecond timestamp to a DateTime object
            /// </summary>
            /// <returns>The DateTime object representing the value of the timestamp</returns>
            /// <param name="msTime">The timestamp in milliseconds since epoch (UTC)</param>
            public static DateTime MsTimestampToDateTime(ulong msTime)
            {
                DateTime dateTime = new DateTime(1970, 1, 1, 0, 0, 0, 0, DateTimeKind.Utc);

                ulong seconds = msTime / 1000;
                ulong millies = msTime % 1000;

                dateTime.AddSeconds(seconds);
                dateTime.AddMilliseconds(millies);

                return dateTime;
            }

            /// <summary>
            /// Converts a DateTime object in milliseconds since epoch timestamp (UTC)
            /// </summary>
            /// <returns>The timestamp in ms</returns>
            /// <param name="msTime">The DateTime object to convert</param>
            public static ulong DateTimeToMsTimestamp(DateTime dateTime)
            {
                return (ulong)(dateTime.ToUniversalTime().Subtract(new DateTime(1970, 1, 1, 0, 0, 0, DateTimeKind.Utc)).TotalMilliseconds);
            }
        }
    }
}