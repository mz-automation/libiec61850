package com.libiec61850.scl.model;

/*
 *  AttributeType.java
 *
 *  Copyright 2013, 2014 Michael Zillgith
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

import com.libiec61850.scl.SclParserException;

public enum AttributeType {
    BOOLEAN(0), /* int */
    INT8(1), /* int8_t */
    INT16(2), /* int16_t */
    INT32(3), /* int32_t */
    INT64(4), /* int64_t */
    INT128(5), 
    INT8U(6), /* uint8_t */
    INT16U(7), /* uint16_t */
    INT24U(8), /* uint32_t */
    INT32U(9), /* uint32_t */
    FLOAT32(10), /* float */
    FLOAT64(11), /* double */
    ENUMERATED(12), 
    OCTET_STRING_64(13), 
    OCTET_STRING_6(14), 
    OCTET_STRING_8(15), 
    VISIBLE_STRING_32(16), 
    VISIBLE_STRING_64(17), 
    VISIBLE_STRING_65(18), 
    VISIBLE_STRING_129(19), 
    VISIBLE_STRING_255(20), 
    UNICODE_STRING_255(21), 
    TIMESTAMP(22), 
    QUALITY(23), 
    CHECK(24), 
    CODEDENUM(25), 
    GENERIC_BITSTRING(26), 
    CONSTRUCTED(27), 
    ENTRY_TIME(28), 
    PHYCOMADDR(29),
    CURRENCY(30),
    OPTFLDS(31),
    TRGOPS(32);
    
    private int intValue;
    
    AttributeType(int intValue) {
        this.intValue = intValue;
    }

    public int getIntValue() {
        return this.intValue;
    }
    
    public static AttributeType createFromSclString(String typeString)
            throws SclParserException {
        if (typeString.equals("BOOLEAN"))
            return BOOLEAN;
        else if (typeString.equals("INT8"))
            return INT8;
        else if (typeString.equals("INT16"))
            return INT16;
        else if (typeString.equals("INT32"))
            return INT32;
        else if (typeString.equals("INT64"))
            return INT64;
        else if (typeString.equals("INT128"))
            return INT128;
        else if (typeString.equals("INT8U"))
            return INT8U;
        else if (typeString.equals("INT16U"))
            return INT16U;
        else if (typeString.equals("INT24U"))
            return INT24U;
        else if (typeString.equals("INT32U"))
            return INT32U;
        else if (typeString.equals("FLOAT32"))
            return FLOAT32;
        else if (typeString.equals("FLOAT64"))
            return FLOAT64;
        else if (typeString.equals("Enum"))
            return ENUMERATED;
        else if (typeString.equals("Dbpos"))
            return CODEDENUM;
        else if (typeString.equals("Check"))
            return CHECK;
        else if (typeString.equals("Tcmd"))
            return CODEDENUM;
        else if (typeString.equals("Octet64"))
            return OCTET_STRING_64;
        else if (typeString.equals("Quality"))
            return QUALITY;
        else if (typeString.equals("Timestamp"))
            return TIMESTAMP;
        else if (typeString.equals("Currency"))
            return CURRENCY;
        else if (typeString.equals("VisString32"))
            return VISIBLE_STRING_32;
        else if (typeString.equals("VisString64"))
            return VISIBLE_STRING_64;
        else if (typeString.equals("VisString65"))
            return VISIBLE_STRING_65;
        else if (typeString.equals("VisString129"))
            return VISIBLE_STRING_129;
        else if (typeString.equals("ObjRef"))
            return VISIBLE_STRING_129;
        else if (typeString.equals("VisString255"))
            return VISIBLE_STRING_255;
        else if (typeString.equals("Unicode255"))
            return UNICODE_STRING_255;
        else if (typeString.equals("OptFlds"))
            return OPTFLDS;
        else if (typeString.equals("TrgOps"))
            return TRGOPS;
        else if (typeString.equals("EntryID"))
            return OCTET_STRING_8;
        else if (typeString.equals("EntryTime"))
            return ENTRY_TIME;
        else if (typeString.equals("PhyComAddr"))
            return PHYCOMADDR;
        else if (typeString.equals("Struct"))
            return CONSTRUCTED;
        else
            throw new SclParserException("unsupported attribute type "
                    + typeString);

    }
}
