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
				return Marshal.PtrToStringAnsi (LibIEC61850_getVersionString ());
			}

			/// <summary>
			/// Converts millisecond timestamp to a DateTime object
			/// </summary>
			/// <returns>The DateTime object representing the value of the timestamp</returns>
			/// <param name="msTime">The timestamp in milliseconds since epoch (UTC)</param>
			public static DateTime MsTimestampToDateTime(ulong msTime)
			{
				DateTime dateTime =  new DateTime(1970, 1, 1, 0, 0, 0, 0, DateTimeKind.Utc);

				ulong seconds = msTime / 1000;
				ulong millies = msTime % 1000;

				dateTime.AddSeconds ((double) seconds);
				dateTime.AddMilliseconds((double) millies);

				return dateTime;
			}

			/// <summary>
			/// Converts a DateTime object in milliseconds since epoch timestamp (UTC)
			/// </summary>
			/// <returns>The timestamp in ms</returns>
			/// <param name="msTime">The DateTime object to convert</param>
			public static ulong DateTimeToMsTimestamp(DateTime dateTime)
			{
				return (ulong) (dateTime.ToUniversalTime ().Subtract (new DateTime (1970, 1, 1, 0, 0, 0, DateTimeKind.Utc)).TotalMilliseconds);
			}
		}

		[Flags]
		public enum TriggerOptions {
			NONE = 0,
			/** send report when value of data changed */
			DATA_CHANGED = 1,
			/** send report when quality of data changed */
			QUALITY_CHANGED = 2,
			/** send report when data or quality is updated */
			DATA_UPDATE = 4,
			/** periodic transmission of all data set values */
			INTEGRITY = 8,
			/** general interrogation (on client request) */
			GI = 16
		}

		[Flags]
		public enum ReportOptions {
			NONE = 0,
			SEQ_NUM = 1,
			TIME_STAMP = 2,
			REASON_FOR_INCLUSION = 4,
			DATA_SET = 8,
			DATA_REFERENCE = 16,
			BUFFER_OVERFLOW = 32,
			ENTRY_ID = 64,
			CONF_REV = 128,
			ALL = 255
		}

		public enum Validity
		{
			GOOD = 0,
			RESERVED = 1,
			INVALID = 2,
			QUESTIONABLE = 3
		}

		/// <summary>
		/// The quality of a data object.
		/// </summary>
		public class Quality
		{

			private UInt16 value;

			public Quality (int bitStringValue)
			{
				value = (UInt16)bitStringValue;
			}

			public Quality ()
			{
				value = 0;
			}

			public Validity GetValidity ()
			{
				int qualityVal = value & 0x3;

				return (Validity)qualityVal;
			}

			public void SetValidity (Validity validity)
			{
				value = (UInt16)(value & 0xfffc);

				value += (ushort)validity;
			}
		}

		public enum ACSIClass
		{
			/** data objects */
			ACSI_CLASS_DATA_OBJECT,
			/** data sets (container for multiple data objects) */
			ACSI_CLASS_DATA_SET,
			/** buffered report control blocks */
			ACSI_CLASS_BRCB,
			/** unbuffered report control blocks */
			ACSI_CLASS_URCB,
			/** log control blocks */
			ACSI_CLASS_LCB,
			/** logs (journals) */
			ACSI_CLASS_LOG,
			/** setting group control blocks */
			ACSI_CLASS_SGCB,
			/** GOOSE control blocks */
			ACSI_CLASS_GoCB,
			/** GSE control blocks */
			ACSI_CLASS_GsCB,
			/** multicast sampled values control blocks */
			ACSI_CLASS_MSVCB,
			/** unicast sampled values control blocks */
			ACSI_CLASS_USVCB
		}

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

		public enum ControlAddCause {
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

		/// <summary>
		/// Object reference. Helper function to handle object reference strings.
		/// </summary>
		public static class ObjectReference {

			/// <summary>
			/// Get the name part of an object reference with appended FC
			/// </summary>
			/// <returns>
			/// The element name.
			/// </returns>
			/// <param name='objectReferenceWithFc'>
			/// Object reference with appended fc.
			/// </param>
			public static string getElementName (string objectReferenceWithFc)
			{
				int fcPartStartIndex = objectReferenceWithFc.IndexOf('[');

				if (fcPartStartIndex == -1)
					return objectReferenceWithFc;

				return objectReferenceWithFc.Substring(0, fcPartStartIndex);
			}

			/// <summary>
			/// Get the FC of an object reference with appended FC.
			/// </summary>
			/// <returns>
			/// The FC
			/// </returns>
			/// <param name='objectReferenceWithFc'>
			/// Object reference with FC.
			/// </param>
			public static FunctionalConstraint getFC (string objectReferenceWithFc)
			{
				int fcPartStartIndex = objectReferenceWithFc.IndexOf('[');

				if (fcPartStartIndex == -1)
					return FunctionalConstraint.NONE;

				string fcString = objectReferenceWithFc.Substring(fcPartStartIndex + 1 , 2);

				try
				{
					return (FunctionalConstraint) Enum.Parse(typeof(FunctionalConstraint), fcString);
				}
				catch(ArgumentException)
				{
					return FunctionalConstraint.NONE;
				}
			}
		}

	}
}

