/*
 *  Control.cs
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

using IEC61850.Common;

namespace IEC61850
{

	namespace Common {

		public enum ControlModel {
			CONTROL_MODEL_STATUS_ONLY = 0,
			CONTROL_MODEL_DIRECT_NORMAL = 1,
			CONTROL_MODEL_SBO_NORMAL = 2,
			CONTROL_MODEL_DIRECT_ENHANCED = 3,
			CONTROL_MODEL_SBO_ENHANCED = 4
		}
	}

	namespace Client {

		public class ControlObject
		{
			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern IntPtr ControlObjectClient_create(string objectReference, IntPtr connection);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern void ControlObjectClient_destroy(IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern int ControlObjectClient_getControlModel(IntPtr self);

			[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
			static extern bool ControlObjectClient_operate(IntPtr self, IntPtr ctlVal, UInt64 operTime);

			private IntPtr controlObject;

			internal ControlObject (string objectReference, IntPtr connection)
			{
				this.controlObject = ControlObjectClient_create(objectReference, connection);

				if (this.controlObject == System.IntPtr.Zero)
					throw new IedConnectionException("Control object not found", 0);
			}

			~ControlObject ()
			{
				ControlObjectClient_destroy(controlObject);
			}

			public ControlModel GetControlModel ()
			{
				ControlModel controlModel = (ControlModel) ControlObjectClient_getControlModel(controlObject);

				return controlModel;
			}

			public bool Operate (bool ctlVal)
			{
				return Operate (ctlVal, 0);
			}

			public bool Operate (bool ctlVal, UInt64 operTime)
			{
				MmsValue value = new MmsValue(ctlVal);

				return Operate (value, operTime);
			}

			public bool Operate (float ctlVal)
			{
				return Operate (ctlVal, 0);
			}

			public bool Operate (float ctlVal, UInt64 operTime)
			{
				MmsValue value = new MmsValue(ctlVal);

				return Operate (value, operTime);
			}

			public bool Operate (int ctlVal)
			{
				return Operate (ctlVal, 0);
			}

			public bool Operate (int ctlVal, UInt64 operTime)
			{
				MmsValue value = new MmsValue(ctlVal);

				return Operate (value, operTime);
			}

			public bool Operate (MmsValue ctlVal)
			{
				return Operate (ctlVal, 0);
			}

			public bool Operate (MmsValue ctlVal, UInt64 operTime)
			{
				return ControlObjectClient_operate(controlObject, ctlVal.valueReference, operTime);
			}
		}

	}

}

