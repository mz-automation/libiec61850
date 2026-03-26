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
        /// <summary>
        /// Error codes for client side functions
        /// </summary>
        public enum IedClientError
        {
            /* general errors */

            /** No error occurred - service request has been successful */
            IED_ERROR_OK = 0,

            /** The service request can not be executed because the client is not yet connected */
            IED_ERROR_NOT_CONNECTED = 1,

            /** Connect service not execute because the client is already connected */
            IED_ERROR_ALREADY_CONNECTED = 2,

            /** The service request can not be executed caused by a loss of connection */
            IED_ERROR_CONNECTION_LOST = 3,

            /** The service or some given parameters are not supported by the client stack or by the server */
            IED_ERROR_SERVICE_NOT_SUPPORTED = 4,

            /** Connection rejected by server */
            IED_ERROR_CONNECTION_REJECTED = 5,

            /* client side errors */

            /** API function has been called with an invalid argument */
            IED_ERROR_USER_PROVIDED_INVALID_ARGUMENT = 10,

            IED_ERROR_ENABLE_REPORT_FAILED_DATASET_MISMATCH = 11,

            /** The object provided object reference is invalid (there is a syntactical error). */
            IED_ERROR_OBJECT_REFERENCE_INVALID = 12,

            /** Received object is of unexpected type */
            IED_ERROR_UNEXPECTED_VALUE_RECEIVED = 13,

            /* service error - error reported by server */

            /** The communication to the server failed with a timeout */
            IED_ERROR_TIMEOUT = 20,

            /** The server rejected the access to the requested object/service due to access control */
            IED_ERROR_ACCESS_DENIED = 21,

            /** The server reported that the requested object does not exist */
            IED_ERROR_OBJECT_DOES_NOT_EXIST = 22,

            /** The server reported that the requested object already exists */
            IED_ERROR_OBJECT_EXISTS = 23,

            /** The server does not support the requested access method */
            IED_ERROR_OBJECT_ACCESS_UNSUPPORTED = 24,

            /** The server expected an object of another type */
            IED_ERROR_TYPE_INCONSISTENT = 25,

            /** The object or service is temporarily unavailable */
            IED_ERROR_TEMPORARILY_UNAVAILABLE = 26,

            /** The specified object is not defined in the server (returned by server) */
            IED_ERROR_OBJECT_UNDEFINED = 27,

            /** The specified address is invalid (returned by server) */
            IED_ERROR_INVALID_ADDRESS = 28,

            /** Service failed due to a hardware fault (returned by server) */
            IED_ERROR_HARDWARE_FAULT = 29,

            /** The requested data type is not supported by the server (returned by server) */
            IED_ERROR_TYPE_UNSUPPORTED = 30,

            /** The provided attributes are inconsistent (returned by server) */
            IED_ERROR_OBJECT_ATTRIBUTE_INCONSISTENT = 31,

            /** The provided object value is invalid (returned by server) */
            IED_ERROR_OBJECT_VALUE_INVALID = 32,

            /** The object is invalidated (returned by server) */
            IED_ERROR_OBJECT_INVALIDATED = 33,

            /** Received an invalid response message from the server */
            IED_ERROR_MALFORMED_MESSAGE = 34,

            /** Service was not executed because required resource is still in use */
            IED_ERROR_OBJECT_CONSTRAINT_CONFLICT = 35,

            /** Service not implemented */
            IED_ERROR_SERVICE_NOT_IMPLEMENTED = 98,

            /* unknown error */
            IED_ERROR_UNKNOWN = 99
        }
    }
}