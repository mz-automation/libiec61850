using System;
using System.Runtime.InteropServices;

namespace IEC61850
{
    namespace Common
    {
        /// <summary>
        /// Definition for LastAppError error type for control models
        /// Used in LastApplError and CommandTermination messages.
        /// </summary>
        public enum ControlLastApplError
        {
            NO_ERROR = 0,
            UNKNOWN = 1,
            TIMEOUT_TEST = 2,
            OPERATOR_TEST = 3
        }
    }
}