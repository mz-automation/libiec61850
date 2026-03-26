using System;
using System.Runtime.InteropServices;

namespace IEC61850
{
    namespace Common
    {
        public enum FunctionalConstraint
        {
            /** Status information */
            ST = 0,
            /** Measurands - analog values */
            MX = 1,
            /** Setpoint */
            SP = 2,
            /** Substitution */
            SV = 3,
            /** Configuration */
            CF = 4,
            /** Description */
            DC = 5,
            /** Setting group */
            SG = 6,
            /** Setting group editable */
            SE = 7,
            /** Service response / Service tracking */
            SR = 8,
            /** Operate received */
            OR = 9,
            /** Blocking */
            BL = 10,
            /** Extended definition */
            EX = 11,
            /** Control */
            CO = 12,
            /** Unicast SV */
            US = 13,
            /** Multicast SV */
            MS = 14,
            /** Unbuffered report */
            RP = 15,
            /** Buffered report */
            BR = 16,
            /** Log control blocks */
            LG = 17,

            /** All FCs - wildcard value */
            ALL = 99,
            NONE = -1
        }
    }
}