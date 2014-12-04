package com.libiec61850.scl.model;

/*
 *  DataModelValue.java
 *
 *  Copyright 2013 Michael Zillgith
 *
 *	This file is part of libIEC61850.
 *
 *	libIEC61850 is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	libIEC61850 is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with libIEC61850.  If not, see <http://www.gnu.org/licenses/>.
 *
 *	See COPYING file for the complete license text.
 */

import com.libiec61850.scl.types.EnumerationType;
import com.libiec61850.scl.types.IllegalValueException;
import com.libiec61850.scl.types.SclType;
import com.libiec61850.scl.types.TypeDeclarations;

public class DataModelValue {

    private Object value = null;
    private String unknownEnumValue = null;
    private String enumType = null;
    
    public DataModelValue(AttributeType type, String enumType, String value) {
    	this.unknownEnumValue = value;
    	this.enumType = enumType;
    }
    
    public DataModelValue(AttributeType type, SclType sclType, String value) throws IllegalValueException {

        switch (type) {
        case ENUMERATED:
    		EnumerationType enumType = (EnumerationType) sclType;
            this.value = (Object) (new Integer(enumType.getOrdByEnumString(value)));
       	
            break;
        case INT8:
        case INT16:
        case INT32:
        case INT8U:
        case INT16U:
        case INT32U:
        case INT24U:
        case INT64:
        	if (value.isEmpty())
        		this.value = new Long(0);
        	else
        		this.value = new Long(value);
            break;
        case BOOLEAN:
            if (value.toLowerCase().equals("true"))
                this.value = new Boolean(true);
            else
                this.value = new Boolean(false);
            break;
        case FLOAT32:
        	if (value.isEmpty())
        		this.value = new Float(0);
        	else
        		this.value = new Float(value);
        	break;
        case FLOAT64:
        	if (value.isEmpty())
        		this.value = new Double(0);
        	else
        		this.value = new Double(value);
        	break;
        case UNICODE_STRING_255:
        	this.value = value;
        	break;
            
        case VISIBLE_STRING_32:
        case VISIBLE_STRING_64:
        case VISIBLE_STRING_65:
        case VISIBLE_STRING_129:
        case VISIBLE_STRING_255:
            this.value = (Object) value;
            break;
        case CHECK:
            System.out.println("Warning: Initialization of CHECK is unsupported!\n");
        case CODEDENUM:
            this.value = null;
            System.out.println("Warning: Initialization of CODEDENUM is unsupported!\n");
            break;
        default:
            throw new IllegalValueException("Unsupported type " + type.toString() + " value: " + value);
        }
    }
    
    public Object getValue() {
        return value;
    }
    
    public String getUnknownEnumValue() {
    	return unknownEnumValue;
    }
    
    public void updateEnumOrdValue(TypeDeclarations typeDecls)
    {
    	if (enumType != null) {
    	
    		System.out.println("Lookup enum type " + enumType);
    		
	    	SclType sclType = typeDecls.lookupType(enumType);
	    	
	    	if (sclType != null) {
	        	
		    	EnumerationType enumType = (EnumerationType) sclType;
		        try {
					this.value = (Object) (new Integer(enumType.getOrdByEnumString(unknownEnumValue)));
				} catch (IllegalValueException e) {
					e.printStackTrace();
				}
	    	}
	    	else
	    		System.out.println("  failed!");
    	}
    }
    
    public long getLongValue() {
        return (Long) value;
    }
    
    public int getIntValue() {
    	if (value instanceof Long) {
    		Long longValue = (Long) value;
    		return ((Long) value).intValue();
    	}
    	if (value instanceof Integer)
    		return ((Integer) value).intValue();
    
        if (value instanceof Boolean) {
        	if (((Boolean) value) == true)
        		return 1;
        	else
        		return 0;
        }
        
        return 0;
    }
    
}
