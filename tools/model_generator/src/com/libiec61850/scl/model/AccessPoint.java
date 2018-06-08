package com.libiec61850.scl.model;

/*
 *  AccessPoint.java
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

import org.w3c.dom.Node;

import com.libiec61850.scl.ParserUtils;
import com.libiec61850.scl.SclParserException;
import com.libiec61850.scl.types.TypeDeclarations;

public class AccessPoint {
	
	private String name;
	private Server server;
	
	public AccessPoint(Node apNode, TypeDeclarations typeDeclarations) throws SclParserException {
		this.name = ParserUtils.parseAttribute(apNode, "name");
		
		if (this.name == null)
			throw new SclParserException(apNode, "AccessPoint as no name defined!");

		Node serverNode = ParserUtils.getChildNodeWithTag(apNode, "Server");
		
		if (serverNode == null)
			this.server = null;
		else
		    this.server = new Server(serverNode, typeDeclarations);
		
	}

	public String getName() {
		return name;
	}

	public Server getServer() {
		return server;
	}
	
}
