/*
 *  IedServerConfig.cs
 *
 *  Copyright 2018 Michael Zillgith
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

namespace IEC61850.Server
{
	/// <summary>
	/// IedServer configuration object
	/// </summary>
	public class IedServerConfig : IDisposable
	{
		[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
		static extern IntPtr IedServerConfig_create();

		[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
		static extern IntPtr IedServerConfig_destroy(IntPtr self);

		[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
		static extern void IedServerConfig_setReportBufferSize(IntPtr self, int reportBufferSize);

		[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
		static extern int IedServerConfig_getReportBufferSize(IntPtr self);

		[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
		static extern void IedServerConfig_setFileServiceBasePath(IntPtr self, string basepath);

		[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
		static extern IntPtr IedServerConfig_getFileServiceBasePath(IntPtr self);

		[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
		static extern void IedServerConfig_setEdition(IntPtr self, byte edition);

		[DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
		static extern byte IedServerConfig_getEdition(IntPtr self);

		internal IntPtr self;

		public IedServerConfig ()
		{
			self = IedServerConfig_create ();
		}

		/// <summary>
		/// Gets or sets the size of the report buffer for buffered report control blocks
		/// </summary>
		/// <value>The size of the report buffer.</value>
		public int ReportBufferSize
		{
			get {
				return IedServerConfig_getReportBufferSize (self);
			}
			set {
				IedServerConfig_setReportBufferSize (self, value);
			}
		}

		/// <summary>
		/// Gets or sets the file service base path.
		/// </summary>
		/// <value>The file service base path.</value>
		public string FileServiceBasePath
		{
			get {
				return Marshal.PtrToStringAnsi (IedServerConfig_getFileServiceBasePath (self));
			}
			set {
				IedServerConfig_setFileServiceBasePath (self, value);
			}
		}

		public Iec61850Edition Edition
		{
			get {
				return (Iec61850Edition)IedServerConfig_getEdition (self);
			}
			set {
				IedServerConfig_setEdition (self, (byte) value);
			}
		}

		/// <summary>
		/// Releases all resource used by the <see cref="IEC61850.Server.IedServerConfig"/> object.
		/// </summary>
		/// <remarks>Call <see cref="Dispose"/> when you are finished using the <see cref="IEC61850.Server.IedServerConfig"/>. The
		/// <see cref="Dispose"/> method leaves the <see cref="IEC61850.Server.IedServerConfig"/> in an unusable state. After
		/// calling <see cref="Dispose"/>, you must release all references to the
		/// <see cref="IEC61850.Server.IedServerConfig"/> so the garbage collector can reclaim the memory that the
		/// <see cref="IEC61850.Server.IedServerConfig"/> was occupying.</remarks>
		public void Dispose()
		{
			lock (this) {
				if (self != IntPtr.Zero) {
					IedServerConfig_destroy (self);
					self = IntPtr.Zero;
				}
			}
		}

		~IedServerConfig()
		{
			Dispose ();
		}
	}
}

