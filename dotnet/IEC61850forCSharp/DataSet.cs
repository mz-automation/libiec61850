/*
 *  DataSet.cs
 *
 *  Copyright 2014-2018 Michael Zillgith
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

using IEC61850.Common;

namespace IEC61850
{
    namespace Client
    {
        /// <summary>
        /// This class is used to represent a data set. It is used to store the values
        /// of a data set.
        /// </summary>
        /// <remarks>
        /// This class manages native resource. Take care that the finalizer/Dispose is not
        /// called while running a method or the object is still in use by another object.
        /// If in doubt please use the "using" statement.
        /// </remarks>
        public class DataSet : IDisposable
        {
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void ClientDataSet_destroy(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr ClientDataSet_getValues(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr ClientDataSet_getReference(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern int ClientDataSet_getDataSetSize(IntPtr self);

            private IntPtr nativeObject;
            private MmsValue values = null;
            private string reference = null;

            internal DataSet(IntPtr nativeObject)
            {
                this.nativeObject = nativeObject;
            }

            /// <summary>
            /// Gets the object reference of the data set
            /// </summary>
            /// <returns>
            /// object reference.
            /// </returns>
			public string GetReference()
            {
                if (reference == null)
                {
                    IntPtr nativeString = ClientDataSet_getReference(nativeObject);

                    reference = Marshal.PtrToStringAnsi(nativeString);
                }

                return reference;
            }

            /// <summary>
            /// Gets the values associated with the data set object
            /// </summary>
            /// <description>This function will return the locally stored values associated with the data set.
            /// These are the values received by the last request to the server. A call to this method doesn't
            /// invoke a request to the server! </description>
            /// <returns>
            /// The locally stored values of the data set (as MmsValue instance of type MMS_ARRAY)
            /// </returns>
			public MmsValue GetValues()
            {
                if (values == null)
                {
                    IntPtr nativeValues = ClientDataSet_getValues(nativeObject);

                    values = new MmsValue(nativeValues, false);
                }

                return values.Clone();
            }


            /// <summary>
            /// Gets the number of elements of the data set
            /// </summary>
            /// <returns>
            /// the number of elementes (data set members)
            /// </returns>
			public int GetSize()
            {
                return ClientDataSet_getDataSetSize(nativeObject);
            }

            private bool disposed = false;

            protected virtual void Dispose(bool disposing)
            {
                lock (this)
                {
                    if (!disposed)
                    {
                        if (nativeObject != IntPtr.Zero)
                        {
                            ClientDataSet_destroy(nativeObject);
                            nativeObject = IntPtr.Zero;
                        }

                        disposed = true;
                    }
                }
            }

            public void Dispose()
            {
                Dispose(true);
                GC.SuppressFinalize(this);
            }

            ~DataSet()
            {
                Dispose(false);
            }

            internal IntPtr getNativeInstance()
            {
                return nativeObject;
            }
        }

    }

}
