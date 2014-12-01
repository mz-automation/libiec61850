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

import com.libiec61850.scl.DataObjectDefinition;
import com.libiec61850.scl.ParserUtils;
import com.libiec61850.scl.SclParserException;

public class LogicalNodeType extends SclType {
	
	private String lnClass = null;
	private List<DataObjectDefinition> dataObjects = null;
	
	public LogicalNodeType(Node lnNode) throws SclParserException {
		super(lnNode);

		parseAttributes(lnNode);

		parseDataObjectNodes(lnNode);
	}
	
	private DataObjectDefinition getObjectDefinitionByName(String name) {
		for (DataObjectDefinition dod : dataObjects) {
			if (dod.getName().equals(name))
				return dod;
		}
		
		return null;
	}

	private void parseDataObjectNodes(Node lnNode) throws SclParserException {
		dataObjects = new LinkedList<DataObjectDefinition>();
		
		List<Node> doNodeList = ParserUtils.getChildNodesWithTag(lnNode, "DO");
		
		for (Node doNode : doNodeList) {
			DataObjectDefinition dod = new DataObjectDefinition(doNode);
			
			if (getObjectDefinitionByName(dod.getName()) != null)
				throw new SclParserException(lnNode, "Logical node contains multiple data objects with name \"" + dod.getName() + "\"");
			
			this.dataObjects.add(dod);
		}
	}

	private void parseAttributes(Node lnNode) throws SclParserException {
		this.lnClass = ParserUtils.parseAttribute(lnNode, "lnClass");

		if (this.lnClass == null)
			throw new SclParserException(lnNode, "no lnClass attribute");
	}

	public List<DataObjectDefinition> getDataObjectDefinitions() {
		return dataObjects;
	}
	
	
}
