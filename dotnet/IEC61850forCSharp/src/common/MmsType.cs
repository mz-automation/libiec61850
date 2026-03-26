using System.Collections;
using System.Runtime.InteropServices;
using System.Text;

namespace IEC61850
{
    namespace Common
    {
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