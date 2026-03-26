using IEC61850.Common;
using IEC61850.TLS;
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

// IEC 61850 API for the libiec61850 .NET wrapper library
namespace IEC61850
{
    // IEC 61850 client API.
    namespace Client
    {
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

    }
}