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
        public enum DataSetOperation
        {
            DATASET_CREATE,
            DATASET_DELETE,
            DATASET_READ,
            DATASET_WRITE,
            DATASET_GET_DIRECTORY
        }
    }
}