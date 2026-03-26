/*
 *  IEC61850CommonAPI.cs
 *
 *  Copyright 2014-2025 Michael Zillgith
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

namespace IEC61850
{
    namespace Common
    {

        /// <summary>
        /// Object reference. Helper function to handle object reference strings.
        /// </summary>
        public static class ObjectReference
        {

            /// <summary>
            /// Get the name part of an object reference with appended FC
            /// </summary>
            /// <returns>
            /// The element name.
            /// </returns>
            /// <param name='objectReferenceWithFc'>
            /// Object reference with appended fc.
            /// </param>
            public static string getElementName(string objectReferenceWithFc)
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
            public static FunctionalConstraint getFC(string objectReferenceWithFc)
            {
                int fcPartStartIndex = objectReferenceWithFc.IndexOf('[');

                if (fcPartStartIndex == -1)
                    return FunctionalConstraint.NONE;

                string fcString = objectReferenceWithFc.Substring(fcPartStartIndex + 1, 2);

                try
                {
                    return (FunctionalConstraint)Enum.Parse(typeof(FunctionalConstraint), fcString);
                }
                catch (ArgumentException)
                {
                    return FunctionalConstraint.NONE;
                }
            }
        }

    }
}

