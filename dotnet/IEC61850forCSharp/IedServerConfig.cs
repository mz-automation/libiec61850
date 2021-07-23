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
        static extern void IedServerConfig_setReportBufferSizeForURCBs(IntPtr self, int reportBufferSize);

        [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
        static extern int IedServerConfig_getReportBufferSizeForURCBs(IntPtr self);

        [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
        static extern void IedServerConfig_setFileServiceBasePath(IntPtr self, string basepath);

        [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
        static extern IntPtr IedServerConfig_getFileServiceBasePath(IntPtr self);

        [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
        static extern void IedServerConfig_enableFileService(IntPtr self, [MarshalAs(UnmanagedType.I1)] bool enable);

        [return: MarshalAs(UnmanagedType.I1)]
        [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
        static extern bool IedServerConfig_isFileServiceEnabled(IntPtr self);

        [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
        static extern void IedServerConfig_setEdition(IntPtr self, byte edition);

        [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
        static extern byte IedServerConfig_getEdition(IntPtr self);

        [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
        static extern void IedServerConfig_setMaxMmsConnections(IntPtr self, int maxConnections);

        [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
        static extern int IedServerConfig_getMaxMmsConnections(IntPtr self);

        [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I1)]
        static extern bool IedServerConfig_isDynamicDataSetServiceEnabled(IntPtr self);

        [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
        static extern void IedServerConfig_enableDynamicDataSetService(IntPtr self, [MarshalAs(UnmanagedType.I1)] bool enable);

        [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
        static extern void IedServerConfig_setMaxAssociationSpecificDataSets(IntPtr self, int maxDataSets);

        [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
        static extern int IedServerConfig_getMaxAssociationSpecificDataSets(IntPtr self);

        [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
        static extern void IedServerConfig_setMaxDomainSpecificDataSets(IntPtr self, int maxDataSets);

        [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
        static extern int IedServerConfig_getMaxDomainSpecificDataSets(IntPtr self);

        [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
        static extern void IedServerConfig_setMaxDataSetEntries(IntPtr self, int maxDataSetEntries);

        [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
        static extern int IedServerConfig_getMaxDatasSetEntries(IntPtr self);

        [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
        static extern void IedServerConfig_enableLogService(IntPtr self, [MarshalAs(UnmanagedType.I1)] bool enable);

        [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I1)]
        static extern bool IedServerConfig_isLogServiceEnabled(IntPtr self);

        [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
        static extern void IedServerConfig_enableResvTmsForBRCB(IntPtr self, [MarshalAs(UnmanagedType.I1)] bool enable);

        [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I1)]
        static extern bool IedServerConfig_isResvTmsForBRCBEnabled(IntPtr self);

        [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
        static extern void IedServerConfig_enableOwnerForRCB(IntPtr self, [MarshalAs(UnmanagedType.I1)] bool enable);

        [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I1)]
        static extern bool IedServerConfig_isOwnerForRCBEnabled(IntPtr self);

        [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
        static extern void IedServerConfig_useIntegratedGoosePublisher(IntPtr self, [MarshalAs(UnmanagedType.I1)] bool enable);

        internal IntPtr self;

        public IedServerConfig()
        {
            self = IedServerConfig_create();
        }

        /// <summary>
        /// Gets or sets the size of the report buffer for buffered report control blocks
        /// </summary>
        /// <value>The size of the report buffer.</value>
        public int ReportBufferSize
        {
            get
            {
                return IedServerConfig_getReportBufferSize(self);
            }
            set
            {
                IedServerConfig_setReportBufferSize(self, value);
            }
        }

        /// <summary>
        /// Gets or sets the size of the report buffer for unbuffered report control blocks
        /// </summary>
        /// <value>The size of the report buffer.</value>
        public int ReportBufferSizeForURCBs
        {
            get
            {
                return IedServerConfig_getReportBufferSizeForURCBs(self);
            }
            set
            {
                IedServerConfig_setReportBufferSizeForURCBs(self, value);
            }
        }

        /// <summary>
        /// Gets or sets the file service base path.
        /// </summary>
        /// <value>The file service base path.</value>
        public string FileServiceBasePath
        {
            get
            {
                return Marshal.PtrToStringAnsi(IedServerConfig_getFileServiceBasePath(self));
            }
            set
            {
                IedServerConfig_setFileServiceBasePath(self, value);
            }
        }

        /// <summary>
        /// Enable/Disable file service for MMS
        /// </summary>
        /// <value><c>true</c> if file service is enabled; otherwise, <c>false</c>.</value>
        public bool FileServiceEnabled
        {
            get
            {
                return IedServerConfig_isFileServiceEnabled(self);
            }
            set
            {
                IedServerConfig_enableFileService(self, value);
            }
        }

        /// <summary>
        /// Gets or sets the edition of the IEC 61850 standard to use
        /// </summary>
        /// <value>The IEC 61850 edition to use.</value>
        public Iec61850Edition Edition
        {
            get
            {
                return (Iec61850Edition)IedServerConfig_getEdition(self);
            }
            set
            {
                IedServerConfig_setEdition(self, (byte)value);
            }
        }

        /// <summary>
        /// Gets or sets maximum number of MMS clients
        /// </summary>
        /// <value>The max number of MMS client connections.</value>
        public int MaxMmsConnections
        {
            get
            {
                return IedServerConfig_getMaxMmsConnections(self);
            }
            set
            {
                IedServerConfig_setMaxMmsConnections(self, value);
            }
        }

        /// <summary>
        /// Enable/Disable dynamic data set service for MMS
        /// </summary>
        /// <value><c>true</c> if dynamic data set service enabled; otherwise, <c>false</c>.</value>
        public bool DynamicDataSetServiceEnabled
        {
            get
            {
                return IedServerConfig_isDynamicDataSetServiceEnabled(self);
            }
            set
            {
                IedServerConfig_enableDynamicDataSetService(self, value);
            }
        }

        /// <summary>
        /// Gets or sets the maximum number of data set entries for dynamic data sets
        /// </summary>
        /// <value>The max. number data set entries.</value>
        public int MaxDataSetEntries
        {
            get
            {
                return IedServerConfig_getMaxDatasSetEntries(self);
            }
            set
            {
                IedServerConfig_setMaxDataSetEntries(self, value);
            }
        }

        /// <summary>
        /// Gets or sets the maximum number of association specific (non-permanent) data sets.
        /// </summary>
        /// <value>The max. number of association specific data sets.</value>
        public int MaxAssociationSpecificDataSets
        {
            get
            {
                return IedServerConfig_getMaxAssociationSpecificDataSets(self);
            }
            set
            {
                IedServerConfig_setMaxAssociationSpecificDataSets(self, value);
            }
        }

        /// <summary>
        /// Gets or sets the maximum number of domain specific (permanent) data sets.
        /// </summary>
        /// <value>The max. numebr of domain specific data sets.</value>
        public int MaxDomainSpecificDataSets
        {
            get
            {
                return IedServerConfig_getMaxDomainSpecificDataSets(self);
            }
            set
            {
                IedServerConfig_setMaxDomainSpecificDataSets(self, value);
            }
        }

        /// <summary>
        /// Enable/Disable log service for MMS
        /// </summary>
        /// <value><c>true</c> if log service is enabled; otherwise, <c>false</c>.</value>
        public bool LogServiceEnabled
        {
            get
            {
                return IedServerConfig_isLogServiceEnabled(self);
            }
            set
            {
                IedServerConfig_enableLogService(self, value);
            }
        }

        /// <summary>
        /// Enable/Disable the presence of ResvTms attribute in BRCBs (buffered report control blocks)
        /// </summary>
        /// <value><c>true</c> if BRCB has ResvTms; otherwise, <c>false</c>. Defaults to true</value>
        public bool BRCBHasResvTms
        {
            get
            {
                return IedServerConfig_isResvTmsForBRCBEnabled(self);
            }
            set
            {
                IedServerConfig_enableResvTmsForBRCB(self, value);
            }
        }

        /// <summary>
        /// Enable/Disable the presence of Owner attribute in RCBs (report control blocks)
        /// </summary>
        /// <value><c>true</c> if RCB has Owner; otherwise, <c>false</c>. Defaults to false</value>
        public bool RCBHasOwner
        {
            get
            {
                return IedServerConfig_isOwnerForRCBEnabled(self);
            }
            set
            {
                IedServerConfig_enableOwnerForRCB(self, value);
            }
        }

        /// <summary>
        /// Enable/disable using the integrated GOOSE publisher for configured GoCBs
        /// </summary>
        /// <value><c>true</c> when integrated GOOSE publisher is used; otherwise, <c>false</c>. Defaults to true</value>
        public bool UseIntegratedGoosePublisher
        {
            set
            {
                IedServerConfig_useIntegratedGoosePublisher(self, value);
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
            lock (this)
            {
                if (self != IntPtr.Zero)
                {
                    IedServerConfig_destroy(self);
                    self = IntPtr.Zero;
                }
            }
        }

        ~IedServerConfig()
        {
            Dispose();
        }
    }
}

