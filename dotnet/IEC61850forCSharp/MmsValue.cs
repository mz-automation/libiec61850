/*
 *  MmsValue.cs
 *
 *  Copyright 2014 Michael Zillgith
 *
 *  This file is part of libIEC61850.
 *
 *  libIEC61850 is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  libIEC61850 is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with libIEC61850.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  See COPYING file for the complete license text.
 */
using System;

using System.Runtime.InteropServices;
using System.Collections.Generic;

using System.Collections;

namespace IEC61850
{
	namespace Client
	{
		public class MmsValue : IEnumerable
		{
			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr MmsValue_toString (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern float MmsValue_toFloat (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern double MmsValue_toDouble (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern bool MmsValue_getBoolean (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern UInt32 MmsValue_getBitStringAsInteger (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern Int32 MmsValue_toInt32 (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern Int64 MmsValue_toInt64 (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern UInt32 MmsValue_toUint32 (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern int MmsValue_getType (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void MmsValue_delete (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr MmsValue_getElement (IntPtr complexValue, int index);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern int MmsValue_getArraySize (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern ulong MmsValue_getUtcTimeInMs (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern UInt32 MmsValue_toUnixTimestamp (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr MmsValue_newBoolean (bool value);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr MmsValue_newFloat (float value);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr MmsValue_newDouble (double value);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr MmsValue_newIntegerFromInt32 (Int32 integer);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr MmsValue_newIntegerFromInt64 (Int64 integer);

			internal MmsValue (IntPtr value)
			{
				valueReference = value;
				this.responsableForDeletion = false;
			}

			internal MmsValue (IntPtr value, bool responsableForDeletion)
			{
				valueReference = value;
				this.responsableForDeletion = responsableForDeletion;
			}

			public MmsValue (bool value)
			{
				valueReference = MmsValue_newBoolean (value);
			}

			public MmsValue (float value)
			{
				valueReference = MmsValue_newFloat (value);
			}

			public MmsValue (double value)
			{
				valueReference = MmsValue_newDouble (value);
			}

			public MmsValue (int value)
			{
				valueReference = MmsValue_newIntegerFromInt32 (value);
			}

			public MmsValue (long value)
			{
				valueReference = MmsValue_newIntegerFromInt64 (value);
			}

			~MmsValue ()
			{
				if (responsableForDeletion)
					MmsValue_delete (valueReference);
			}

			internal IntPtr valueReference;
			private bool responsableForDeletion;

			public new MmsType GetType ()
			{
				return (MmsType)MmsValue_getType (valueReference);
			}

			public int Size ()
			{
				if ((GetType () == MmsType.MMS_ARRAY) || (GetType () == MmsType.MMS_STRUCTURE)) {
					return MmsValue_getArraySize (valueReference);
				} else
					throw new MmsValueException ("Value is not a complex type");
			}

			public MmsValue GetElement (int index)
			{
				MmsType type = GetType ();

				if ((type == MmsType.MMS_ARRAY) || (type == MmsType.MMS_STRUCTURE)) {
					if ((index >= 0) && (index < Size ())) {
						IntPtr value = MmsValue_getElement (valueReference, index);
						return new MmsValue (value);
					} else
						throw new MmsValueException ("Index out of bounds");
				} else
					throw new MmsValueException ("Value is of wrong type");
			}

			public UInt32 ToUnixTimestamp ()
			{
				if (GetType () == MmsType.MMS_UTC_TIME)
					return MmsValue_toUnixTimestamp (valueReference);
				else
					throw new MmsValueException ("Value is not a time type");
			}

			public ulong GetUtcTimeInMs ()
			{
				if (GetType () == MmsType.MMS_UTC_TIME) {
					return MmsValue_getUtcTimeInMs (valueReference);
				} else
					throw new MmsValueException ("Value is not a time type");
			}

			public static DateTimeOffset MsTimeToDateTimeOffset (UInt64 msTime)
			{
				DateTimeOffset retVal = new DateTimeOffset (1970, 1, 1, 0, 0, 0, TimeSpan.Zero);

				return retVal.AddMilliseconds (msTime);
			}

			public DateTimeOffset GetUtcTimeAsDateTimeOffset ()
			{
				if (GetType () == MmsType.MMS_UTC_TIME)
					return MsTimeToDateTimeOffset (GetUtcTimeInMs ());
				else
					throw new MmsValueException ("Value is not a time type");
			}

			public Int32 ToInt32 ()
			{
				if (GetType () != MmsType.MMS_INTEGER)
					throw new MmsValueException ("Value type is not integer");

				Int32 retVal = MmsValue_toInt32 (valueReference);

				return retVal;
			}

			public Int64 ToInt64 ()
			{
				if (GetType () != MmsType.MMS_INTEGER)
					throw new MmsValueException ("Value type is not integer");

				Int64 retVal = MmsValue_toInt64 (valueReference);

				return retVal;
			}

			public UInt32 ToUint32 ()
			{
				if (GetType () != MmsType.MMS_UNSIGNED)
					throw new MmsValueException ("Value type is not unsigned");

				UInt32 retVal = MmsValue_toUint32 (valueReference);

				return retVal;
			}

			public bool GetBoolean ()
			{
				if (GetType () == MmsType.MMS_BOOLEAN)
					return MmsValue_getBoolean (valueReference);
				else
					throw new MmsValueException ("Value type is not boolean");
			}

			public float ToFloat ()
			{
				if (GetType () == MmsType.MMS_FLOAT)
					return MmsValue_toFloat (valueReference);
				else
					throw new MmsValueException ("Value type is not float");
			}

			public double ToDouble ()
			{
				if (GetType () == MmsType.MMS_FLOAT)
					return MmsValue_toDouble (valueReference);
				else
					throw new MmsValueException ("Value type is not float");
			}

			// override standard ToString() method
			public override string ToString ()
			{
				switch (GetType ()) {
				case MmsType.MMS_VISIBLE_STRING:
				case MmsType.MMS_STRING:
					return Marshal.PtrToStringAnsi (MmsValue_toString (valueReference));
				case MmsType.MMS_BOOLEAN:
					return GetBoolean ().ToString ();
				case MmsType.MMS_INTEGER:
					return ToInt64 ().ToString ();
				case MmsType.MMS_UNSIGNED:
					return ToUint32 ().ToString ();
				case MmsType.MMS_FLOAT:
					return ToDouble ().ToString ();
				case MmsType.MMS_UTC_TIME:
					return GetUtcTimeAsDateTimeOffset ().ToString ();
				default:
					return "unknown";
				}
			}

			IEnumerator IEnumerable.GetEnumerator ()
			{
				return new MmsValueEnumerator (this);
			}

			private class MmsValueEnumerator : IEnumerator
			{
				private MmsValue value;
				private int index = -1;

				public MmsValueEnumerator (MmsValue value)
				{
					this.value = value;
				}

			#region IEnumerator Members
				public void Reset ()
				{
					index = -1;
				}

				public object Current {

					get { return value.GetElement (index);}
				}

				public bool MoveNext ()
				{
					index++;

					if (index >= value.Size ())
						return false;
					else
						return true;
				}

			#endregion
			}

		}

		public class MmsValueException : Exception
		{
			public MmsValueException (string message)
            : base(message)
			{
			}
		}

		public enum MmsType
		{
			MMS_ARRAY = 0,
			MMS_STRUCTURE = 1,
			MMS_BOOLEAN = 2,
			MMS_BIT_STRING = 3,
			MMS_INTEGER = 4,
			MMS_UNSIGNED = 5,
			MMS_FLOAT = 6,
			MMS_OCTET_STRING = 7,
			MMS_VISIBLE_STRING = 8,
			MMS_GENERALIZED_TIME = 9,
			MMS_BINARY_TIME = 10,
			MMS_BCD = 11,
			MMS_OBJ_ID = 12,
			MMS_STRING = 13,
			MMS_UTC_TIME = 14,
			MMS_DATA_ACCESS_ERROR = 15
		}

	}
}

