using System;
using System.Collections;
using System.Runtime.InteropServices;
using System.Text;

namespace IEC61850
{
    namespace Common
    {
        public class MmsValueException : Exception
        {
            public MmsValueException(string message)
            : base(message)
            {
            }
        }

    }
}