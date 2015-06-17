package com.libiec61850.scl.types;

/*
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

import java.util.LinkedList;
import java.util.List;

import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import com.libiec61850.scl.SclParserException;

public class EnumerationType extends SclType {

	private List<EnumerationValue> enumValues;
	
	public EnumerationType(Node xmlNode) throws SclParserException {
		super(xmlNode);
		parseEnumerationValueNodes(xmlNode);
	}

	public EnumerationType(String name) {
		super(name, null);
		this.enumValues = new LinkedList<EnumerationValue>();
	}
	
	static List<EnumerationType> getDefaultEnumTypes() {
		LinkedList<EnumerationType> defaultTypes = new LinkedList<EnumerationType>();
		
		return defaultTypes;
	}
	
	private void addEnumValue(EnumerationValue enumValue) {
		this.enumValues.add(enumValue);
	}
	
	private void parseEnumerationValueNodes(Node enumTypeNode) throws SclParserException {
		NodeList elements = enumTypeNode.getChildNodes();

		if (elements != null) {
			this.enumValues = new LinkedList<EnumerationValue>();

			Node enumValueNode;
			
			for (int i = 0; i < elements.getLength(); i++) {
				enumValueNode = elements.item(i);

				if (enumValueNode.getNodeName().equals("EnumVal"))
					this.enumValues.add(new EnumerationValue(enumValueNode));

			}
		}
	}
	
	public int getOrdByEnumString(String enumString) throws IllegalValueException {
	    for (EnumerationValue value : enumValues) {
	        if (value.getSymbolicName().equals(enumString))
	            return value.getOrd();
	    }
	    
	    throw new IllegalValueException("Enum has no value " + enumString);
	}
	
	
	public boolean isValidOrdValue(int ordValue) {
		for (EnumerationValue enumValue : enumValues) {
			if (enumValue.getOrd() == ordValue)
				return true;
		}
		
		return false;
	}
}
