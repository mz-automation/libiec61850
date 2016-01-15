package com.libiec61850.scl.communication;

import org.w3c.dom.Node;

import com.libiec61850.scl.ParserUtils;
import com.libiec61850.scl.SclParserException;

/*
 *  Copyright 2015 Michael Zillgith
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

public class SMV {
    private String ldInst;
    private String cbName;
    
    private PhyComAddress address;
    
    public SMV(Node gseNode) throws SclParserException {
        ldInst = ParserUtils.parseAttribute(gseNode, "ldInst");
        cbName = ParserUtils.parseAttribute(gseNode, "cbName");
        
        if ((ldInst == null) || (cbName == null))
            throw new SclParserException(gseNode, "SMV is missing required attribute");
        
        Node addressNode = ParserUtils.getChildNodeWithTag(gseNode, "Address");
        
        if (addressNode == null)
            throw new SclParserException(gseNode, "SMV is missing address definition!");
        
        address = new PhyComAddress(addressNode);
    }

    public String getLdInst() {
        return ldInst;
    }

    public String getCbName() {
        return cbName;
    }

    public PhyComAddress getAddress() {
        return address;
    }
}
