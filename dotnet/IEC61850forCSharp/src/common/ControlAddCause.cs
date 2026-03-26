using System;
using System.Runtime.InteropServices;

namespace IEC61850
{
    namespace Common
    {

        /// <summary>
        /// AddCause - additional cause information for control model errors
        /// Used in LastApplError and CommandTermination messages.
        /// </summary>
		public enum ControlAddCause
        {
            ADD_CAUSE_UNKNOWN = 0,
            ADD_CAUSE_NOT_SUPPORTED = 1,
            ADD_CAUSE_BLOCKED_BY_SWITCHING_HIERARCHY = 2,
            ADD_CAUSE_SELECT_FAILED = 3,
            ADD_CAUSE_INVALID_POSITION = 4,
            ADD_CAUSE_POSITION_REACHED = 5,
            ADD_CAUSE_PARAMETER_CHANGE_IN_EXECUTION = 6,
            ADD_CAUSE_STEP_LIMIT = 7,
            ADD_CAUSE_BLOCKED_BY_MODE = 8,
            ADD_CAUSE_BLOCKED_BY_PROCESS = 9,
            ADD_CAUSE_BLOCKED_BY_INTERLOCKING = 10,
            ADD_CAUSE_BLOCKED_BY_SYNCHROCHECK = 11,
            ADD_CAUSE_COMMAND_ALREADY_IN_EXECUTION = 12,
            ADD_CAUSE_BLOCKED_BY_HEALTH = 13,
            ADD_CAUSE_1_OF_N_CONTROL = 14,
            ADD_CAUSE_ABORTION_BY_CANCEL = 15,
            ADD_CAUSE_TIME_LIMIT_OVER = 16,
            ADD_CAUSE_ABORTION_BY_TRIP = 17,
            ADD_CAUSE_OBJECT_NOT_SELECTED = 18,
            ADD_CAUSE_OBJECT_ALREADY_SELECTED = 19,
            ADD_CAUSE_NO_ACCESS_AUTHORITY = 20,
            ADD_CAUSE_ENDED_WITH_OVERSHOOT = 21,
            ADD_CAUSE_ABORTION_DUE_TO_DEVIATION = 22,
            ADD_CAUSE_ABORTION_BY_COMMUNICATION_LOSS = 23,
            ADD_CAUSE_ABORTION_BY_COMMAND = 24,
            ADD_CAUSE_NONE = 25,
            ADD_CAUSE_INCONSISTENT_PARAMETERS = 26,
            ADD_CAUSE_LOCKED_BY_OTHER_CLIENT = 27
        }
    }
}