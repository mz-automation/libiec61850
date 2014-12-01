package com.libiec61850.scl.communication;

/*
 *  Copyright 2013 Michael Zillgith
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

public class SubNetwork {

    private String name;
    private String type;
    
    private List<ConnectedAP> connectedAPs;
    
    public SubNetwork(Node netNode) throws SclParserException {
        name = ParserUtils.parseAttribute(netNode, "name");
        type = ParserUtils.parseAttribute(netNode, "type");
        
        if (name == null)
            throw new SclParserException(netNode, "SubNetwork is missing attribute name!");
        
        connectedAPs = new LinkedList<ConnectedAP>();
        
        List<Node> nodes = ParserUtils.getChildNodesWithTag(netNode, "ConnectedAP");
        
        for (Node node : nodes) {
            connectedAPs.add(new ConnectedAP(node));
        }
    }

    public String getName() {
        return name;
    }

    public String getType() {
        return type;
    }

    public List<ConnectedAP> getConnectedAPs() {
        return connectedAPs;
    }
    
}
