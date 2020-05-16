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

import org.w3c.dom.Node;

import com.libiec61850.scl.ParserUtils;
import com.libiec61850.scl.SclParserException;

public class GSE {

    private String ldInst;
    private String cbName;
    
    private int minTime = -1;
    private int maxTime = -1;
    
    private PhyComAddress address;
    
    public GSE(Node gseNode) throws SclParserException {
        ldInst = ParserUtils.parseAttribute(gseNode, "ldInst");
        cbName = ParserUtils.parseAttribute(gseNode, "cbName");
        
        if ((ldInst == null) || (cbName == null))
            throw new SclParserException(gseNode, "GSE is missing required attribute");
        
        Node minTimeNode = ParserUtils.getChildNodeWithTag(gseNode, "MinTime");
        
        if (minTimeNode != null) {
        	minTime = Integer.parseInt(minTimeNode.getTextContent());
        }
        
        Node maxTimeNode = ParserUtils.getChildNodeWithTag(gseNode, "MaxTime");
        
        if (maxTimeNode != null) {
        	maxTime = Integer.parseInt(maxTimeNode.getTextContent());
        }
        
        Node addressNode = ParserUtils.getChildNodeWithTag(gseNode, "Address");
        
        if (addressNode == null)
            throw new SclParserException(gseNode, "GSE is missing address definition!");
        
        address = new PhyComAddress(addressNode);
    }

    public String getLdInst() {
        return ldInst;
    }

    public String getCbName() {
        return cbName;
    }
    
	public int getMinTime() {
		return minTime;
	}

	public int getMaxTime() {
		return maxTime;
	}

    public PhyComAddress getAddress() {
        return address;
    }

}
