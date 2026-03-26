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
        /// Config file parser.
        /// </summary>
        public class ConfigFileParser
        {
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr ConfigFileParser_createModelFromConfigFileEx(string filename);

            public static IedModel CreateModelFromConfigFile(string filePath)
            {
                IntPtr retVal = ConfigFileParser_createModelFromConfigFileEx(filePath);
                if (retVal == IntPtr.Zero)
                {
                    return null;
                }

                return new IedModel(retVal);
            }
        }
    }
}