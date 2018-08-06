/*
 *  MmsVariableSpecification.cs
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
	namespace Common
	{
        /// <summary>
        /// MMS variable specification. This class is used to represent an MMS variable type definition.
        /// </summary>
		public class MmsVariableSpecification : IEnumerable 
		{
			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void MmsVariableSpecification_destroy(IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr MmsVariableSpecification_getChildValue(IntPtr self, IntPtr value, string childId);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr MmsVariableSpecification_getNamedVariableRecursive(IntPtr variable, string nameId);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern int MmsVariableSpecification_getType(IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr MmsVariableSpecification_getName(IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern int MmsVariableSpecification_getSize(IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr MmsVariableSpecification_getChildSpecificationByIndex(IntPtr self, int index);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr MmsVariableSpecification_getArrayElementSpecification(IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern int MmsVariableSpecification_getExponentWidth(IntPtr self);

			internal IntPtr self;
			private bool responsableForDeletion;

			internal MmsVariableSpecification (IntPtr self)
			{
				this.self = self;
				this.responsableForDeletion = false;
			}

			internal MmsVariableSpecification (IntPtr self, bool responsableForDeletion)
			{
				this.self = self;
				this.responsableForDeletion = responsableForDeletion;
			}

			~MmsVariableSpecification ()
			{
				if (responsableForDeletion)
					MmsVariableSpecification_destroy(self);
			}

            /// <summary>
            /// Gets the MmsValue type of the variable
            /// </summary>
            /// <returns>
            /// The MmsType of the variable
            /// </returns>
			public new MmsType GetType ()
			{
				return (MmsType) MmsVariableSpecification_getType(self);
			}

            /// <summary>
            /// Gets the type of the array elements.
            /// </summary>
            /// <returns>
            /// The array element type.
            /// </returns>
            /// <exception cref="MmsValueException">This exception is thrown if the value is not of type MMS_ARRAY</exception>
			public MmsVariableSpecification getArrayElementType ()
			{
				if (GetType() == MmsType.MMS_ARRAY) {
					IntPtr varSpecPtr = MmsVariableSpecification.MmsVariableSpecification_getArrayElementSpecification(self);
					return new MmsVariableSpecification(varSpecPtr);
				}
				else
					throw new  MmsValueException ("specification is of wrong type"); 
			}

            /// <summary>
            /// Gets the element specification of a structure element
            /// </summary>
            /// <returns>
            /// The element of the structure at given index
            /// </returns>
            /// <param name='index'>
            /// Index.
            /// </param>
			public MmsVariableSpecification GetElement (int index)
			{
				if (GetType () == MmsType.MMS_STRUCTURE) {

					if ((index >= 0) && (index < Size ())) {
						IntPtr varSpecPtr = MmsVariableSpecification_getChildSpecificationByIndex(self, index);
						return new MmsVariableSpecification(varSpecPtr);
					} 
					else
						throw new MmsValueException ("Index out of bounds");
				} 
				else
					throw new  MmsValueException ("specification is of wrong type"); 
			}

            /// <summary>
            /// Gets the name of the variable
            /// </summary>
            /// <returns>
            /// The name.
            /// </returns>
			public string GetName ()
			{
				IntPtr namePtr = MmsVariableSpecification_getName(self);

				return Marshal.PtrToStringAnsi (namePtr);
			}

            /// <summary>
            /// Get the "size" of the variable (array size, number of structure elements ...)
            /// </summary>
			public int Size ()
			{
				return MmsVariableSpecification_getSize(self);
			}

			IEnumerator IEnumerable.GetEnumerator ()
			{
				return new MmsVariableSpecificationEnumerator (this);
			}

			private class MmsVariableSpecificationEnumerator : IEnumerator
			{
				private MmsVariableSpecification value;
				private int index = -1;

				public MmsVariableSpecificationEnumerator (MmsVariableSpecification value)
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
	}
}
