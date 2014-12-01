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
import com.libiec61850.scl.DataObjectDefinition;
import com.libiec61850.scl.ParserUtils;
import com.libiec61850.scl.SclParserException;

public class DataObjectType extends SclType {

	private String cdc = null;
	
	private List<DataAttributeDefinition> dataAttributes = null;
	private List<DataObjectDefinition> subDataObjects = null;
	
	private DataAttributeDefinition getDataAttributeByName(String name) {
		for (DataAttributeDefinition dad : dataAttributes) {
			if (dad.getName().equals(name))
				return dad;
		}
		
		return null;
	}
	
	private DataObjectDefinition getDataObjectByName(String name) {
		for (DataObjectDefinition dod : subDataObjects) {
			if (dod.getName().equals(name))
				return dod;
		}
		
		return null;
	}
	
	public DataObjectType(Node xmlNode) throws SclParserException {
		super(xmlNode);
		
		this.cdc = ParserUtils.parseAttribute(xmlNode, "cdc");

		if (this.cdc == null)
			throw new SclParserException(xmlNode, "cdc is missing!");

		NodeList elementNodes = xmlNode.getChildNodes();

		if (elementNodes != null) {
			this.dataAttributes = new LinkedList<DataAttributeDefinition>();
			this.subDataObjects = new LinkedList<DataObjectDefinition>();

			for (int i = 0; i < elementNodes.getLength(); i++) {
				Node elementNode = elementNodes.item(i);
				
				if (elementNode.getNodeName().equals("DA")) {
					DataAttributeDefinition dad = new DataAttributeDefinition(elementNode);
						
					DataAttributeDefinition otherDefinition = getDataAttributeByName(dad.getName());				
					
					if (otherDefinition != null) {
						
						if (otherDefinition.getFc() == dad.getFc())
							throw new SclParserException(xmlNode, 
									"DO type definition contains multiple elements of name \"" + 
											dad.getName() + "\"");
					}
					
					if (getDataObjectByName(dad.getName()) != null) {
						throw new SclParserException(xmlNode, 
								"DO type definition contains multiple elements of name \"" + 
										dad.getName() + "\"");
					}
						
					
					dataAttributes.add(dad);
				}
				else if (elementNode.getNodeName().equals("SDO")) {
					DataObjectDefinition dod = new DataObjectDefinition(elementNode);
					
					if ((getDataAttributeByName(dod.getName()) != null) || 
						(getDataObjectByName(dod.getName()) != null))
						throw new SclParserException(xmlNode, 
								"DO type definition contains multiple elements of name \"" + 
										dod.getName() + "\"");
					
					this.subDataObjects.add(dod);
				}
				
			}
		}
	}

	public String getCdc() {
		return cdc;
	}

	public List<DataAttributeDefinition> getDataAttributes() {
		return dataAttributes;
	}

	public List<DataObjectDefinition> getSubDataObjects() {
		return subDataObjects;
	}

}
