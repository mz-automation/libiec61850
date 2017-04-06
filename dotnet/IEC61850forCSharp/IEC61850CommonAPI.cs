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

		/// <summary>
		/// MMS data access error for MmsValue type MMS_DATA_ACCESS_ERROR
		/// </summary>
		public enum MmsDataAccessError {
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

		/// <summary>
		/// Timestamp (represents IEC 61850 timestamps e.g. "t" attribute)
		/// </summary>
		public class Timestamp
		{
			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr Timestamp_create ();

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void Timestamp_destroy (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void Timestamp_clearFlags (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			[return: MarshalAs(UnmanagedType.I1)]
			static extern bool Timestamp_isLeapSecondKnown (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void Timestamp_setLeapSecondKnown (IntPtr self, bool value);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			[return: MarshalAs(UnmanagedType.I1)]
			static extern bool Timestamp_hasClockFailure (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void Timestamp_setClockFailure (IntPtr self, bool value);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			[return: MarshalAs(UnmanagedType.I1)]
			static extern bool Timestamp_isClockNotSynchronized (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void Timestamp_setClockNotSynchronized (IntPtr self, bool value);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern int Timestamp_getSubsecondPrecision (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void Timestamp_setSubsecondPrecision(IntPtr self, int subsecondPrecision);


			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void Timestamp_setTimeInSeconds (IntPtr self, UInt32 secondsSinceEpoch);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void Timestamp_setTimeInMilliseconds (IntPtr self, UInt64 millisSinceEpoch);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern UInt32 Timestamp_getTimeInSeconds (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern UInt64 Timestamp_getTimeInMs (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void Timestamp_setByMmsUtcTime (IntPtr self, IntPtr mmsValue);

			internal IntPtr timestampRef = IntPtr.Zero;
			private bool responsableForDeletion;

			internal Timestamp(IntPtr timestampRef, bool selfAllocated)
			{
				this.timestampRef = timestampRef;
				this.responsableForDeletion = selfAllocated;
			}

			public Timestamp (DateTime timestamp) : this ()
			{
				SetDateTime (timestamp);
			}

			public Timestamp (MmsValue mmsUtcTime) : this() 
			{
				SetByMmsUtcTime (mmsUtcTime);
			}

			public Timestamp()
			{
				timestampRef = Timestamp_create ();
				LeapSecondKnown = true;
				responsableForDeletion = true;
			}

			~Timestamp ()
			{
				if (responsableForDeletion)
					Timestamp_destroy (timestampRef);
			}

			public void ClearFlags()
			{
				Timestamp_clearFlags (timestampRef);
			}

			public void SetDateTime(DateTime timestamp)
			{
				DateTime epoch = new DateTime(1970, 1, 1, 0, 0, 0, DateTimeKind.Utc);
				DateTime timestampUTC = timestamp.ToUniversalTime();

				TimeSpan timeDiff = timestampUTC - epoch;
				ulong timeVal = Convert.ToUInt64(timeDiff.TotalMilliseconds);

				SetTimeInMilliseconds (timeVal);
			}

			public bool LeapSecondKnown {
				get { return IsLeapSecondKnown ();}
				set { SetLeapSecondKnow (value);}
			}

			public bool IsLeapSecondKnown()
			{
				return Timestamp_isLeapSecondKnown (timestampRef);
			}

			public void SetLeapSecondKnow(bool value)
			{
				Timestamp_setLeapSecondKnown (timestampRef, value);
			}

			public bool ClockFailure {
				get { return HasClockFailure ();}
				set { SetClockFailure (value);}
			}

			public bool HasClockFailure()
			{
				return Timestamp_hasClockFailure (timestampRef);
			}

			public void SetClockFailure(bool value)
			{
				Timestamp_setClockFailure (timestampRef, value);
			}

			public bool ClockNotSynchronized {
				get { return IsClockNotSynchronized (); }
				set { SetClockNotSynchronized (value); }
			}

			public bool IsClockNotSynchronized() {
				return Timestamp_isClockNotSynchronized(timestampRef);
			}

			public void SetClockNotSynchronized(bool value) {
				Timestamp_setClockNotSynchronized (timestampRef, value);
			}

			public int SubsecondPrecision {
				get { return GetSubsecondPrecision (); }
				set { SetSubsecondPrecision (value); }
			}

			public int GetSubsecondPrecision() {
				return Timestamp_getSubsecondPrecision (timestampRef);
			}

			public void SetSubsecondPrecision(int subsecondPrecision) {
				Timestamp_setSubsecondPrecision (timestampRef, subsecondPrecision);
			}

			public UInt32 TimeInSeconds {
				get { return GetTimeInSeconds (); }
				set { SetTimeInSeconds (value); }
			}

			public UInt32 GetTimeInSeconds()
			{
				return Timestamp_getTimeInSeconds (timestampRef);
			}

			public void SetTimeInSeconds(UInt32 secondsSinceEpoch)
			{
				Timestamp_setTimeInSeconds (timestampRef, secondsSinceEpoch);
			}

			public UInt64 TimeInMilliseconds {
				get { return GetTimeInMilliseconds (); }
				set { SetTimeInMilliseconds (value); }
			}

			public UInt64 GetTimeInMilliseconds()
			{
				return Timestamp_getTimeInMs (timestampRef);
			}

			public void SetTimeInMilliseconds(UInt64 millisSinceEpoch) {
				Timestamp_setTimeInMilliseconds (timestampRef, millisSinceEpoch);
			}

			public void SetByMmsUtcTime(MmsValue mmsValue)
			{
				Timestamp_setByMmsUtcTime (timestampRef, mmsValue.valueReference);
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

