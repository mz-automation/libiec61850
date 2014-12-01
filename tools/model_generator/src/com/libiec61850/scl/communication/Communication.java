package com.libiec61850.scl.communication;

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

public class Communication {

    private List<SubNetwork> subNetworks;
    
    public Communication(Node comSection) throws SclParserException {
        List<Node> subnetworks = ParserUtils.getChildNodesWithTag(comSection, "SubNetwork");
        
        if (subnetworks.size() == 0)
            throw new SclParserException(comSection, "no subnetworks defined");
        
        subNetworks = new LinkedList<SubNetwork>();
        
        for (Node node : subnetworks) {
            subNetworks.add(new SubNetwork(node));
        }
    }

    public List<SubNetwork> getSubNetworks() {
        return subNetworks;
    }

}
