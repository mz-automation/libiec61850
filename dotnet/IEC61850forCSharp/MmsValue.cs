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
using System.Text;

namespace IEC61850
{
	namespace Common
	{

        /// <summary>
        /// This class is used to hold MMS data values of different types.
        /// </summary>
		public class MmsValue : IEnumerable
		{
			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr MmsValue_toString (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern float MmsValue_toFloat (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void MmsValue_setFloat (IntPtr self, float value);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern double MmsValue_toDouble (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void MmsValue_setDouble (IntPtr self, double newFloatValue);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            [return: MarshalAs(UnmanagedType.I1)]
			static extern bool MmsValue_getBoolean (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern UInt32 MmsValue_getBitStringAsInteger (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void MmsValue_setBitStringFromInteger(IntPtr self, UInt32 intValue);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern int MmsValue_getBitStringSize(IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void MmsValue_setBitStringBit(IntPtr self, int bitPos, [MarshalAs(UnmanagedType.I1)] bool value);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            [return: MarshalAs(UnmanagedType.I1)]
			static extern bool MmsValue_getBitStringBit(IntPtr self, int bitPos);

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
			static extern void MmsValue_setUtcTimeMs (IntPtr self, ulong timeval);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern ulong MmsValue_getUtcTimeInMsWithUs(IntPtr self, [Out] uint usec);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern byte MmsValue_getUtcTimeQuality (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void MmsValue_setUtcTimeQuality (IntPtr self, byte timeQuality);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern UInt32 MmsValue_toUnixTimestamp (IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr MmsValue_newBoolean ([MarshalAs(UnmanagedType.I1)] bool value);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr MmsValue_newFloat (float value);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr MmsValue_newDouble (double value);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr MmsValue_newIntegerFromInt32 (Int32 integer);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr MmsValue_newUnsignedFromUint32(UInt32 integer);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr MmsValue_newIntegerFromInt64 (Int64 integer);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr MmsValue_newBitString(int bitSize);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr MmsValue_newVisibleString(string value);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr MmsValue_newOctetString(int size, int maxSize);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void MmsValue_setOctetString(IntPtr self, [Out] byte[] buf, int size);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern UInt16 MmsValue_getOctetStringSize(IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern UInt16 MmsValue_getOctetStringMaxSize(IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr MmsValue_getOctetStringBuffer(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            [return: MarshalAs(UnmanagedType.I1)]
            static extern bool MmsValue_equals(IntPtr self, IntPtr otherValue);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr MmsValue_newBinaryTime ([MarshalAs(UnmanagedType.I1)] bool timeOfDay);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void MmsValue_setBinaryTime (IntPtr self, UInt64 timestamp);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern ulong MmsValue_getBinaryTimeAsUtcMs (IntPtr self);

			[DllImport ("iec61850", CallingConvention=CallingConvention.Cdecl)]
			static extern int MmsValue_getDataAccessError(IntPtr self);

			internal IntPtr valueReference; /* reference to native MmsValue instance */

			private bool responsableForDeletion; /* if .NET wrapper is responsable for the deletion of the native MmsValue instance */

			// TODO make internal
			public MmsValue (IntPtr value)
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

            public MmsValue (UInt32 value)
            {
                valueReference = MmsValue_newUnsignedFromUint32(value);
            }

			public MmsValue (long value)
			{
				valueReference = MmsValue_newIntegerFromInt64 (value);
			}

            public MmsValue (string value)
            {
                valueReference = MmsValue_newVisibleString(value);
            }

			~MmsValue ()
			{
				if (responsableForDeletion)
					MmsValue_delete (valueReference);
			}

			/// <summary>
			/// Create a new MmsValue instance of type MMS_BIT_STRING.
			/// </summary>
			/// <returns>
			/// the new MmsValue instance
			/// </returns>
			/// <param name='bitSize'>
			/// the size of the bit string in bits.
			/// </param>
			public static MmsValue NewBitString(int bitSize) 
			{
				IntPtr newValue = MmsValue_newBitString(bitSize);

				return new MmsValue(newValue, true);
			}

			/// <summary>
			/// Create a new MmsValue instance of type MMS_OCTET_STRING.
			/// </summary>
			/// <returns>
			/// the new MmsValue instance
			/// </returns>
			/// <param name='maxSize'>
			/// the maximum size of the octet string in bytes
			/// </param>
			/// <param name='size'>
			/// the current size of the octet string in bytes (defaults to 0)
			/// </param>
			public static MmsValue NewOctetString (int maxSize, int size = 0)
			{
				IntPtr newValue = MmsValue_newOctetString(size, maxSize);

				return new MmsValue(newValue, true);
			}																												

			public MmsValue(byte[] octetString)
			{
				if (octetString.Length > 255)
					throw new MmsValueException ("octet string too long");

				valueReference = MmsValue_newOctetString(octetString.Length, octetString.Length);

				this.setOctetString (octetString);
			}

			/// <summary>
			/// Create a new MmsValue instance of type MMS_BINARY_TIME
			/// </summary>
			/// <returns>the new MmsValue instance.</returns>
			/// <param name="largeFormat">If set to <c>true</c> large 6 byte format.</param>
			public static MmsValue NewBinaryTime(bool largeFormat) 
			{
				IntPtr newValue = MmsValue_newBinaryTime (largeFormat);

				return new MmsValue (newValue, true);
			}

			/// <summary>
			/// Sets the binary time.
			/// </summary>
			/// <param name="timestamp">Timestamp.</param>
			public void SetBinaryTime(UInt64 timestamp) {
				MmsValue_setBinaryTime (this.valueReference, timestamp);
			}

			/// <summary>
			/// Gets the binary time value as UTC time in ms.
			/// </summary>
			/// <description>
			/// Return the value as milliseconds since epoch (1.1.1970 UTC).
			/// The value has to be of type MMS_UTC_TIME.
			/// </description>
			/// <returns>
			/// The UTC time in ms.
			/// </returns>
			/// <exception cref="MmsValueException">This exception is thrown if the value has the wrong type.</exception>
			public ulong GetBinaryTimeAsUtcMs ()
			{
				if (GetType () == MmsType.MMS_BINARY_TIME) {
					return MmsValue_getBinaryTimeAsUtcMs (valueReference);
				} else
					throw new MmsValueException ("Value is not a time type");
			}

            /// <summary>
            /// Gets the type of the value
            /// </summary>
            /// <returns>
            /// The type.
            /// </returns>
			public new MmsType GetType ()
			{
				return (MmsType)MmsValue_getType (valueReference);
			}

            /// <summary>
            /// Gets the size of an array, structure, or bit string
            /// </summary>
            /// <returns>
            /// <description>
            /// Return the size of an array of structure (number of elements)
            /// The value has to be of type MMS_ARRAY, MMS_STRUCTURE, MMS_BIT_STRING ...
            /// </description>
            /// the number of elements (array/structure elements, octets, bits depending on type)
            /// </returns>
            /// <exception cref="MmsValueException">This exception is thrown if the value has the wrong type.</exception>
			public int Size ()
			{
				if ((GetType () == MmsType.MMS_ARRAY) || (GetType () == MmsType.MMS_STRUCTURE)) {
					return MmsValue_getArraySize (valueReference);
				} else if (GetType () == MmsType.MMS_BIT_STRING) {
					return MmsValue_getBitStringSize(valueReference);
				}
				else if (GetType () == MmsType.MMS_OCTET_STRING) {
					return MmsValue_getOctetStringSize(valueReference);
				}
				else
					throw new MmsValueException ("Operation not supported for this type");
			}

			/// <summary>
			/// Gets the maximum size of an octet string
			/// </summary>
			/// <returns>
			/// The maximum size (in bytes) of the octet string
			/// </returns>
			public int MaxSize ()
			{
				if (GetType () == MmsType.MMS_OCTET_STRING) {
					return MmsValue_getOctetStringMaxSize(valueReference);
				}
				else
					throw new MmsValueException ("Operation not supported for this type");
			}

			/// <summary>
			/// Gets the octet string as byte array
			/// </summary>
			/// <description>Instance has to be of type MMS_OCTET_STRING.
			/// </description>
			/// <returns>
			/// Byte array containing the bytes of the octet string.
			/// </returns>
			/// <exception cref="MmsValueException">This exception is thrown if the value has the wrong type.</exception>
			public byte[] getOctetString ()
			{
				if (GetType () == MmsType.MMS_OCTET_STRING) {
					IntPtr buffer = MmsValue_getOctetStringBuffer(valueReference);
					int bufferSize = this.Size();

					byte[] octetString = new byte[bufferSize];

					Marshal.Copy(buffer, octetString, 0, bufferSize);

					return octetString;
				}
				else
					throw new MmsValueException ("Operation not supported for this type");
			}

			public void setOctetString (byte[] octetString)
			{
				if (GetType () == MmsType.MMS_OCTET_STRING) {

					if (this.MaxSize() < octetString.Length)
						throw new MmsValueException("octet string is to large");

					MmsValue_setOctetString(valueReference, octetString, octetString.Length);
				}
				else
					throw new MmsValueException ("Operation not supported for this type");
			}

            /// <summary>
            /// Get an element of an array or structure
            /// </summary>
            /// <returns>
            /// the MmsValue element.
            /// </returns>
            /// <param name='index'>
            /// index of the element starting with 0
            /// </param>
            /// <exception cref="MmsValueException">This exception is thrown if the value has the wrong type.</exception>
            /// <exception cref="MmsValueException">This exception is thrown if the index is out of range.</exception>
			public MmsValue GetElement (int index)
			{
				MmsType type = GetType ();

				if ((type == MmsType.MMS_ARRAY) || (type == MmsType.MMS_STRUCTURE)) {
					if ((index >= 0) && (index < Size ())) {
						IntPtr value = MmsValue_getElement (valueReference, index);

						if (value == IntPtr.Zero)
							return null;
						else 
							return new MmsValue (value);
					} else
						throw new MmsValueException ("Index out of bounds");
				} else
					throw new MmsValueException ("Value is of wrong type");
			}


			public MmsDataAccessError GetDataAccessError ()
			{
				if (GetType () == MmsType.MMS_DATA_ACCESS_ERROR) {
					int errorCode = MmsValue_getDataAccessError (valueReference);

					return (MmsDataAccessError)errorCode;
				}
				else
					throw new MmsValueException ("Value is of wrong type");
			}

            /// <summary>
            /// Gets the timestamp value as UTC time in s (UNIX time stamp).
            /// </summary>
            /// <description>
            /// Return the value as seconds since epoch (1.1.1970 UTC).
            /// The value has to be of type MMS_UTC_TIME.
            /// </description>
            /// <returns>
            /// The UTC time in seconds (UNIX time stamp).
            /// </returns>
            /// <exception cref="MmsValueException">This exception is thrown if the value has the wrong type.</exception>
			public UInt32 ToUnixTimestamp ()
			{
				if (GetType () == MmsType.MMS_UTC_TIME)
					return MmsValue_toUnixTimestamp (valueReference);
				else
					throw new MmsValueException ("Value is not a time type");
			}

			/// <summary>
			/// Sets the timestamp value as UTC time in ms.
			/// </summary>
			/// <description>
            /// Sets the value as milliseconds since epoch (1.1.1970 UTC).
            /// The value has to be of type MMS_UTC_TIME.
			/// </description>
            /// <exception cref="MmsValueException">This exception is thrown if the value has the wrong type.</exception>
            public void SetUtcTimeMs(ulong timeval)
			{
				if (GetType () == MmsType.MMS_UTC_TIME) {
                    MmsValue_setUtcTimeMs(valueReference, timeval);
				} else
					throw new MmsValueException ("Value is not a time type");
			}

            /// <summary>
            /// Gets the timestamp value as UTC time in ms.
            /// </summary>
            /// <description>
            /// Return the value as milliseconds since epoch (1.1.1970 UTC).
            /// The value has to be of type MMS_UTC_TIME.
            /// </description>
            /// <returns>
            /// The UTC time in ms.
            /// </returns>
            /// <exception cref="MmsValueException">This exception is thrown if the value has the wrong type.</exception>
			public ulong GetUtcTimeInMs ()
			{
				if (GetType () == MmsType.MMS_UTC_TIME) {
					return MmsValue_getUtcTimeInMs (valueReference);
				} else
					throw new MmsValueException ("Value is not a time type");
			}

			/// <summary>
			/// Gets the timestamp value as UTC time in ms and the additional us part.
			/// </summary>
			/// <description>
			/// Return the value as milliseconds since epoch (1.1.1970 UTC) and the additional us part.
			/// The value has to be of type MMS_UTC_TIME.
			/// </description>
			/// <param name='usec'>
			/// returns the usec part of the time value
			/// </param>
			/// <returns>
			/// The UTC time in ms.
			/// </returns>
			/// <exception cref="MmsValueException">This exception is thrown if the value has the wrong type.</exception>
			public ulong GetUtcTimeInMsWithUs(out int usec)
			{
				if (GetType() == MmsType.MMS_UTC_TIME)
				{
					uint uusec = 0;
					var msVal = MmsValue_getUtcTimeInMsWithUs(valueReference, uusec);
					usec = (int)uusec;
					return msVal;
				}
				else
					throw new MmsValueException("Value is not a time type");
			}

			/// <summary>
			/// Gets quality flags of an UTC timestamp.
			/// </summary>
			/// <description>
			/// Meaning of the bits in the timeQuality byte:
			/// bit 7 = leapSecondsKnown
	        /// bit 6 = clockFailure
	        /// bit 5 = clockNotSynchronized
		    /// bit 0-4 = subsecond time accuracy (number of significant bits of subsecond time)
			/// </description>
			/// <returns>The UTC time quality.</returns>
			public byte GetUtcTimeQuality()
			{
				if (GetType() == MmsType.MMS_UTC_TIME)
					return MmsValue_getUtcTimeQuality(valueReference);
				else
					throw new MmsValueException("Value is not a time type");
			}

			/// <summary>
			/// Sets the quality flags of an UTC timestamp
			/// </summary>
			/// <description>
			/// Meaning of the bits in the timeQuality byte:
			/// bit 7 = leapSecondsKnown
			/// bit 6 = clockFailure
			/// bit 5 = clockNotSynchronized
			/// bit 0-4 = subsecond time accuracy (number of significant bits of subsecond time)
			/// </description>
			/// <param name="timeQuality">Time quality.</param>
			public void SetUtcTimeQuality(byte timeQuality)
			{
				if (GetType () == MmsType.MMS_UTC_TIME)
					MmsValue_setUtcTimeQuality (valueReference, timeQuality);
				else
					throw new MmsValueException("Value is not a time type");
			}

            /// <summary>
            /// Convert a millisecond time (milliseconds since epoch) to DataTimeOffset
            /// </summary>
            /// <returns>
            /// The time as DataTimeOffset
            /// </returns>
            /// <param name='msTime'>
            /// the millisecond time
            /// </param>
			public static DateTimeOffset MsTimeToDateTimeOffset (UInt64 msTime)
			{
				DateTimeOffset retVal = new DateTimeOffset (1970, 1, 1, 0, 0, 0, TimeSpan.Zero);

				return retVal.AddMilliseconds ((double) msTime);
			}

            /// <summary>
            /// Convert MMS_UTC_TIME to DateTimeOffset instance
            /// </summary>
            /// <description>
            /// Return the value as DateTimeOffset instance.
            /// The value has to be of type MMS_UTC_TIME.
            /// </description>
            /// <returns>
            /// the value as DataTimeOffset instance
            /// </returns>
            /// <exception cref="MmsValueException">This exception is thrown if the value has the wrong type.</exception>
			public DateTimeOffset GetUtcTimeAsDateTimeOffset ()
			{
				if (GetType () == MmsType.MMS_UTC_TIME)
					return MsTimeToDateTimeOffset (GetUtcTimeInMs ());
				else
					throw new MmsValueException ("Value is not a time type");
			}


            /// <summary>
            /// Return the value as 32 bit signed integer.
            /// </summary>
            /// <description>
            /// Return the value as 32 bit signed integer (Int32).
            /// The value has to be of type MMS_INTEGER.
            /// </description>
            /// <returns>
            /// the value if the object as 32 bit signed integer
            /// </returns>
            /// <exception cref="MmsValueException">This exception is thrown if the value has the wrong type.</exception>
			public Int32 ToInt32 ()
			{
				if (GetType () != MmsType.MMS_INTEGER)
					throw new MmsValueException ("Value type is not integer");

				return MmsValue_toInt32 (valueReference);
			}

            /// <summary>
            /// Return the value as 64 bit signed integer.
            /// </summary>
            /// <description>
            /// Return the value as 64 bit signed integer (Int64).
            /// The value has to be of type MMS_INTEGER.
            /// </description>
            /// <returns>
            /// the value if the object as 64 bit signed integer
            /// </returns>
            /// <exception cref="MmsValueException">This exception is thrown if the value has the wrong type.</exception>
			public Int64 ToInt64 ()
			{
				if (GetType () != MmsType.MMS_INTEGER)
					throw new MmsValueException ("Value type is not integer");

				return MmsValue_toInt64 (valueReference);
			}

            /// <summary>
            /// Return the value as 32 bit unsigned integer.
            /// </summary>
            /// <description>
            /// Return the value as 32 bit unsigned integer (Int32).
            /// The value has to be of type MMS_INTEGER.
            /// </description>
            /// <returns>
            /// the value if the object as 32 bit unsigned integer
            /// </returns>
            /// <exception cref="MmsValueException">This exception is thrown if the value has the wrong type.</exception>
			public UInt32 ToUint32 ()
			{
				if (GetType () != MmsType.MMS_UNSIGNED)
					throw new MmsValueException ("Value type is not unsigned");

				return MmsValue_toUint32 (valueReference);
			}

			public UInt32 BitStringToUInt32 ()
			{
				if (GetType () != MmsType.MMS_BIT_STRING)
					throw new MmsValueException("Value type is not bit string");

				return MmsValue_getBitStringAsInteger(valueReference);
			}

			public void BitStringFromUInt32 (UInt32 intValue)
			{
				if (GetType () != MmsType.MMS_BIT_STRING)
					throw new MmsValueException("Value type is not bit string");

				MmsValue_setBitStringFromInteger(valueReference, intValue);
			}

			public void SetBit (int bitPos, bool bitValue)
			{
				if (GetType () != MmsType.MMS_BIT_STRING)
					throw new MmsValueException("Value type is not bit string");

				MmsValue_setBitStringBit(valueReference, bitPos, bitValue);
			}

			public bool GetBit (int bitPos)
			{
				if (GetType () != MmsType.MMS_BIT_STRING)
					throw new MmsValueException("Value type is not bit string");

				return MmsValue_getBitStringBit(valueReference, bitPos);
			}

            private string GetBitStringAsString()
            {
                if (GetType() != MmsType.MMS_BIT_STRING)
                    throw new MmsValueException("Value type is not bit string");

                int size = Size();

                StringBuilder builder = new StringBuilder(size);

                for (int i = 0; i < size; i++)
                {
                    if (MmsValue_getBitStringBit(valueReference, i))
                        builder.Append('1');
                    else
                        builder.Append('0');
                }

                return builder.ToString();
            }

            /// <summary>
            /// Gets the boolean value
            /// </summary>
            /// <returns>
            /// The boolean value
            /// </returns>
            /// <exception cref="MmsValueException">This exception is thrown if the value has the wrong type.</exception>
			public bool GetBoolean ()
			{
				if (GetType () == MmsType.MMS_BOOLEAN)
					return MmsValue_getBoolean (valueReference);
				else
					throw new MmsValueException ("Value type is not boolean");
			}

            /// <summary>
            /// Gets the float value of an MMS_FLOAT instance
            /// </summary>
            /// <returns>
            /// The float value
            /// </returns>
            /// <exception cref="MmsValueException">This exception is thrown if the value has the wrong type.</exception>
			public float ToFloat ()
			{
				if (GetType () == MmsType.MMS_FLOAT)
					return MmsValue_toFloat (valueReference);
				else
					throw new MmsValueException ("Value type is not float");
			}

			/// <summary>
			/// Sets the float value of an MMS_FLOAT instance
			/// </summary>
			/// <param name='value'>
			/// the new value to set
			/// </param>
			/// <exception cref="MmsValueException">This exception is thrown if the value has the wrong type.</exception>
			public void SetFloat (float value)
			{
			    if (GetType () == MmsType.MMS_FLOAT)
			        MmsValue_setFloat (valueReference, value);
			    else
			        throw new MmsValueException ("Value type is not float");
			}

            /// <summary>
            /// Gets the double value of an MMS_FLOAT instance
            /// </summary>
            /// <returns>
            /// The float value
            /// </returns>
            /// <exception cref="MmsValueException">This exception is thrown if the value has the wrong type.</exception>
			public double ToDouble ()
			{
				if (GetType () == MmsType.MMS_FLOAT)
					return MmsValue_toDouble (valueReference);
				else
					throw new MmsValueException ("Value type is not float");
			}
						
			/// <summary>
			/// Sets the float/double value of an MMS_FLOAT instance
			/// </summary>
			/// <param name='value'>
			/// the new value to set
			/// </param>
			/// <exception cref="MmsValueException">This exception is thrown if the value has the wrong type.</exception>
			public void SetDouble (double value)
			{
				if (GetType () == MmsType.MMS_FLOAT)
					MmsValue_setDouble (valueReference, value);
				else
					throw new MmsValueException ("Value type is not float");
			}

            public override bool Equals (object obj)
            {
                MmsValue otherValue = (MmsValue) obj;

                return MmsValue_equals(this.valueReference, otherValue.valueReference);
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
				case MmsType.MMS_BINARY_TIME:
					return (MsTimeToDateTimeOffset (GetBinaryTimeAsUtcMs ()).ToString ());
				case MmsType.MMS_OCTET_STRING:
					return BitConverter.ToString (getOctetString ());
                case MmsType.MMS_BIT_STRING:
                    return GetBitStringAsString();
				case MmsType.MMS_STRUCTURE:
					{
						string retString = "{";

						bool first = true;

						foreach (MmsValue element in this) {
							if (first) {
								retString += element.ToString ();

								first = false;
							} else {
								retString += ", " + element.ToString ();
							}
						}

						retString += "}";

						return retString;
					}

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
            /** array type (multiple elements of the same type) */
			MMS_ARRAY = 0,
            /** structure type (multiple elements of different types) */
			MMS_STRUCTURE = 1,
            /** boolean */
			MMS_BOOLEAN = 2,
            /** bit string */
			MMS_BIT_STRING = 3,
            /** signed integer */
			MMS_INTEGER = 4,
            /** unsigned integer */
			MMS_UNSIGNED = 5,
            /** floating point value (32 or 64 bit) */
			MMS_FLOAT = 6,
            /** octet string */
			MMS_OCTET_STRING = 7,
            /** visible string - ANSI string */
			MMS_VISIBLE_STRING = 8,
            /** Generalized time */
			MMS_GENERALIZED_TIME = 9,
            /** Binary time */
			MMS_BINARY_TIME = 10,
            /** Binary coded decimal (BCD) - not used */
			MMS_BCD = 11,
            /** object ID - not used */
			MMS_OBJ_ID = 12,
            /** Unicode string */
			MMS_STRING = 13,
            /** UTC time */
			MMS_UTC_TIME = 14,
            /** will be returned in case of an error (contains error code) */
			MMS_DATA_ACCESS_ERROR = 15
		}

	}
}

