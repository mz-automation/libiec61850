using System;
using System.Runtime.InteropServices;

namespace IEC61850
{
    namespace Common
    {
        /// <summary>
        /// MMS data access error for MmsValue type MMS_DATA_ACCESS_ERROR
        /// </summary>
        public enum MmsDataAccessError
        {
            NO_RESPONSE = -2, /* for server internal purposes only! */
            SUCCESS = -1,
            OBJECT_INVALIDATED = 0,
            HARDWARE_FAULT = 1,
            TEMPORARILY_UNAVAILABLE = 2,
            OBJECT_ACCESS_DENIED = 3,
            OBJECT_UNDEFINED = 4,
            INVALID_ADDRESS = 5,
            TYPE_UNSUPPORTED = 6,
            TYPE_INCONSISTENT = 7,
            OBJECT_ATTRIBUTE_INCONSISTENT = 8,
            OBJECT_ACCESS_UNSUPPORTED = 9,
            OBJECT_NONE_EXISTENT = 10,
            OBJECT_VALUE_INVALID = 11,
            UNKNOWN = 12,
        }
    }
}