package com.libiec61850.scl.model;

/*
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

import java.util.LinkedList;
import java.util.List;

import org.w3c.dom.Node;

import com.libiec61850.scl.ParserUtils;
import com.libiec61850.scl.SclParserException;
import com.libiec61850.scl.types.TypeDeclarations;

public class IED {
	private String name;
	private List<AccessPoint> accessPoints;
	private TypeDeclarations typeDeclarations;
	private Node self;

	public IED(Node iedNode, TypeDeclarations typeDeclarations)
			throws SclParserException {
		this.name = ParserUtils.parseAttribute(iedNode, "name");
		
		self = iedNode;
		
		List<Node> accessPointNodes = ParserUtils.getChildNodesWithTag(iedNode, "AccessPoint");

		this.accessPoints = new LinkedList<AccessPoint>();
		
		for (Node accessPointNode : accessPointNodes) {
			this.accessPoints.add(new AccessPoint(accessPointNode, typeDeclarations));
		}
		
		this.typeDeclarations = typeDeclarations;
	}
	
	public TypeDeclarations getTypeDeclarations() {
		return typeDeclarations;
	}

	public String getName() {
		return name;
	}

	public List<AccessPoint> getAccessPoints() {
		return accessPoints;
	}
	
	public AccessPoint getAccessPointByName(String name) {
		
		for (AccessPoint ap : accessPoints) {
			if (ap.getName().equals(name))
				return ap;
		}
		
		return null;
	}
	
	public AccessPoint getFirstAccessPoint() {
		return accessPoints.get(0);
	}

	public Services getServices() {
        Node servicesNode = ParserUtils.getChildNodeWithTag(self, "Services");
        
        if (servicesNode != null) {
            return new Services(servicesNode);
        }
        else
            return null;
    }
}
