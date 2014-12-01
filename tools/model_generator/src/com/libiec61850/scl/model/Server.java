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

public class Server {
	
	private Authentication authentication = null;
	private List<LogicalDevice> logicalDevices;
	
	public Server(Node serverNode, TypeDeclarations typeDeclarations) throws SclParserException {
		Node authenticationNode = ParserUtils.getChildNodeWithTag(serverNode, "Authentication");
		
		if (authenticationNode != null)
			this.authentication = new Authentication(authenticationNode);
			
		List<Node> ldNodes = ParserUtils.getChildNodesWithTag(serverNode, "LDevice");
		
		if (ldNodes.size() == 0)
			throw new SclParserException(serverNode, "No logical devices defined for AccessPoint");
		
		logicalDevices = new LinkedList<LogicalDevice>();
		
		for (Node ldNode : ldNodes) {
			logicalDevices.add(new LogicalDevice(ldNode, typeDeclarations));
		}
		
	}

	public Authentication getAuthentication() {
		return authentication;
	}

	public List<LogicalDevice> getLogicalDevices() {
		return logicalDevices;
	}
	
}
