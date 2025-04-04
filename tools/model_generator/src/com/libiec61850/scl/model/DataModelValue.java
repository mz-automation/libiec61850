package com.libiec61850.scl.model;

import java.text.SimpleDateFormat;

/*
 *  DataModelValue.java
 *
 *  Copyright 2013, 2015 Michael Zillgith
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

import java.util.Base64;
import java.util.Date;
import java.util.TimeZone;

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
    		
    		try {
    			this.value = (Object) (new Integer(enumType.getOrdByEnumString(value)));
    		} catch (IllegalValueException e) {
    			try {
    				
    				this.value = Integer.parseInt(value);
    				
    				if (enumType.isValidOrdValue(Integer.parseInt(value)) == false) {
    					throw new IllegalValueException(value + 
        						" is not a valid value of type " + sclType.getId());
    				}
    				
    				System.out.println("WARNING: Initialization of ENUM with ordinal value!");
    			}
    			catch (NumberFormatException nfe) {
    				throw new IllegalValueException(value + 
    						" is not a valid value of type " + sclType.getId());
    			}
    		}
       	
            break;
        case INT8:
        case INT16:
        case INT32:
        case INT8U:
        case INT16U:
        case INT32U:
        case INT24U:
        case INT64:
        	
        	String trimmedValue = value.trim();
        	
        	if (trimmedValue != value) {
        		System.out.println("WARNING: value initializer contains leading or trailing whitespace");
        	}
        	
        	if (trimmedValue.isEmpty())
        		this.value = new Long(0);
        	else
        		this.value = Long.parseLong(trimmedValue);
            break;
        case BOOLEAN:
        	
        	trimmedValue = value.trim();
        	
        	if (trimmedValue != value) {
        		System.out.println("WARNING: value initializer contains leading or trailing whitespace");
        	}
        	
            if (trimmedValue.toLowerCase().equals("true"))
                this.value = new Boolean(true);
            else
                this.value = new Boolean(false);
            break;
        case FLOAT32:

            trimmedValue = value.trim();

            if (trimmedValue != value) {
                System.out.println("WARNING: value initializer contains leading or trailing whitespace");
            }

            trimmedValue.replace(',', '.');

            if (trimmedValue.isEmpty())
                this.value = new Float(0);
            else
                this.value = new Float(trimmedValue);
            break;
        case FLOAT64:

            trimmedValue = value.trim();

            if (trimmedValue != value) {
                System.out.println("WARNING: value initializer contains leading or trailing whitespace");
            }

            trimmedValue.replace(',', '.');

            if (trimmedValue.isEmpty())
                this.value = new Double(0);
            else
                this.value = new Double(trimmedValue);
            break;
        case UNICODE_STRING_255:
            this.value = value;
            break;

        case OCTET_STRING_64:
            try {
                this.value = Base64.getDecoder().decode(value);
            }
            catch (IllegalArgumentException e) {
                throw new IllegalValueException("Val element for Octet64 type does not contain a valid base64 encoded string");
            }
            break;
            
        case VISIBLE_STRING_32:
        case VISIBLE_STRING_64:
        case VISIBLE_STRING_65:
        case VISIBLE_STRING_129:
        case VISIBLE_STRING_255:
        case CURRENCY:
            this.value = (Object) value;
            break;
        case CHECK:
            System.out.println("Warning: Initialization of CHECK is unsupported!");
        case CODEDENUM:
            this.value = null;
            
            if (value.equals("intermediate-state"))
            	this.value = new Integer(0);
            else if (value.equals("off"))
            	this.value = new Integer(1);
            else if (value.equals("on"))
            	this.value = new Integer(2);
            else if (value.equals("bad-state"))
            	this.value = new Integer(4);
            else if (value.equals("stop"))
            	this.value = new Integer(0);
            else if (value.equals("lower"))
            	this.value = new Integer(1);
            else if (value.equals("higher"))
            	this.value = new Integer(2);
            else if (value.equals("reserved"))
            	this.value = new Integer(4);
            else 
            	System.out.println("Warning: CODEDENUM is initialized with unsupported value " + value.toString());
            
            break;
        case QUALITY:
        	this.value = null;
        	System.out.println("Warning: Initialization of QUALITY is unsupported!");
        	break;
        	
        case TIMESTAMP:
        case ENTRY_TIME:
            {
                String modValueString = value.replace(',', '.');
                
                try {
                    SimpleDateFormat parser = new SimpleDateFormat("yyyy-MM-d'T'HH:mm:ss.SSS");
                    parser.setTimeZone(TimeZone.getTimeZone("UTC"));

                    Date date = parser.parse(modValueString);
                
                    this.value = new Long(date.toInstant().toEpochMilli());

                    break;
                }
                catch (java.text.ParseException e) {};

                try {
                    SimpleDateFormat parser = new SimpleDateFormat("yyyy-MM-d'T'HH:mm:ss");
                    parser.setTimeZone(TimeZone.getTimeZone("UTC"));

                    Date date = parser.parse(modValueString);
                
                    this.value = new Long(date.toInstant().toEpochMilli());

                    break;
                }
                catch (java.text.ParseException e) {};

                this.value = null;
                System.out.println("Warning: Val element does not contain a valid time stamp: " + value); 
            }
        
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
