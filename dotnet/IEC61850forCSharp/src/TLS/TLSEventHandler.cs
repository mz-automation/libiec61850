/*
 *  TLS.cs
 *
 *  Copyright 2017-2024 Michael Zillgith
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
using System.Security.Cryptography;
using System.Security.Cryptography.X509Certificates;

/// <summary>
/// IEC 61850 API for the libiec61850 .NET wrapper library
/// </summary>
namespace IEC61850
{
    namespace TLS
    {
        /// <summary>
        /// TLS security event handler
        /// </summary>
        /// <param name="parameter">user provided context paramter to be passed to the handler</param>
        /// <param name="eventLevel">severity level of the event</param>
        /// <param name="eventCode">code to identify the event type</param>
        /// <param name="message">text message describing the event</param>
        /// <param name="connection">TLS connection that caused the event</param>
        public delegate void TLSEventHandler(object parameter, TLSEventLevel eventLevel, TLSEventCode eventCode, string message, TLSConnection connection);

    }
}
