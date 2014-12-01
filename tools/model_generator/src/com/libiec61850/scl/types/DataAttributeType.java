package com.libiec61850.scl.types;

/*
 *  Copyright 2013, 2014 Michael Zillgith
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

import com.libiec61850.scl.DataAttributeDefinition;
import com.libiec61850.scl.SclParserException;

public class DataAttributeType extends SclType {
	
	private List<DataAttributeDefinition> subDataAttributes = null;

	private DataAttributeDefinition getDataAttributeByName(String name) {
		for (DataAttributeDefinition dad : subDataAttributes) {
			if (dad.getName().equals(name))
				return dad;
		}
		
		return null;
	}
	
	public DataAttributeType(Node xmlNode) throws SclParserException {
		super(xmlNode);
		
		NodeList elementNodes = xmlNode.getChildNodes();
		
		if (elementNodes != null) {
			this.subDataAttributes = new LinkedList<DataAttributeDefinition>();

			for (int i = 0; i < elementNodes.getLength(); i++) {
				Node elementNode = elementNodes.item(i);
				
				if (elementNode.getNodeName().equals("BDA")) {
					DataAttributeDefinition dad = new DataAttributeDefinition(elementNode);
					
					if (getDataAttributeByName(dad.getName()) != null) {
						throw new SclParserException(xmlNode, 
								"DA type definition contains multiple elements of name \"" + 
										dad.getName() + "\"");
					}
						
					
					subDataAttributes.add(dad);
				}
				
			}
		}
	}

	public List<DataAttributeDefinition> getSubDataAttributes() {
		return subDataAttributes;
	}

}
