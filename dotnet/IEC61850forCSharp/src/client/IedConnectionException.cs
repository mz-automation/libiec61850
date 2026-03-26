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
        public class IedConnectionException : Exception
        {

            private int errorCode;

            public IedConnectionException(string message, int errorCode)
                : base(message)
            {
                this.errorCode = errorCode;
            }

            public IedConnectionException(string message)
                : base(message)
            {
                errorCode = 0;
            }

            public int GetErrorCode()
            {
                return errorCode;
            }

            public IedClientError GetIedClientError()
            {
                return (IedClientError)errorCode;
            }
        }
    }
}