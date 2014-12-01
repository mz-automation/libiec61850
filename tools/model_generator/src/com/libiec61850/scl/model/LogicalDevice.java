package com.libiec61850.scl.model;

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

import com.libiec61850.scl.ParserUtils;
import com.libiec61850.scl.SclParserException;
import com.libiec61850.scl.types.TypeDeclarations;

public class LogicalDevice {
	
	private String inst;
	
	private String ldName;
	
	private List<LogicalNode> logicalNodes;

	public LogicalDevice(Node ldNode, TypeDeclarations typeDeclarations) throws SclParserException {
		this.inst = ParserUtils.parseAttribute(ldNode, "inst");
		
		if (this.inst == null)
			throw new SclParserException(ldNode, "Logical devices misses inst attribute.");
		
		this.ldName = ParserUtils.parseAttribute(ldNode, "ldName");
		
		parseLogicalNodes(ldNode, typeDeclarations);
	}

	private void parseLogicalNodes(Node ldNode, TypeDeclarations typeDeclarations) throws SclParserException {
		Node ln0Node = ParserUtils.getChildNodeWithTag(ldNode, "LN0");
		
		if (ln0Node == null)
			throw new SclParserException(ldNode, "Logical device misses LN0.");
		
		logicalNodes = new LinkedList<LogicalNode>();
		
		logicalNodes.add(new LogicalNode(ln0Node, typeDeclarations, this));
		
		List<Node> lnNodes = ParserUtils.getChildNodesWithTag(ldNode, "LN");
		
		for (Node lnNode : lnNodes) {
			logicalNodes.add(new LogicalNode(lnNode, typeDeclarations, this));
		}
	}

	public String getInst() {
		return inst;
	}

	public String getLdName() {
		return ldName;
	}

	public List<LogicalNode> getLogicalNodes() {
		return logicalNodes;
	}
	
}
