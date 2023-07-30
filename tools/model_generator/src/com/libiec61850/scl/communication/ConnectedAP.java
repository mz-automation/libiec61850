package com.libiec61850.scl.communication;

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

public class ConnectedAP {

    private String iedName;
    private String apName;

    private List<GSE> gses;
    private List<SMV> smvs;
    
    private Address address = null;

    public ConnectedAP(Node node) throws SclParserException {
        iedName = ParserUtils.parseAttribute(node, "iedName");
        apName = ParserUtils.parseAttribute(node, "apName");

        if ((iedName == null) || (apName == null))
            throw new SclParserException(node, "ConnectedAP is missing required attribute");

        gses = new LinkedList<GSE>();

        List<Node> gseNodes = ParserUtils.getChildNodesWithTag(node, "GSE");

        for (Node gseNode : gseNodes)
            gses.add(new GSE(gseNode));
                
        smvs = new LinkedList<SMV>();
        
        List<Node> smvNodes = ParserUtils.getChildNodesWithTag(node, "SMV");
        
        for (Node smvNode : smvNodes)
        	smvs.add(new SMV(smvNode));
        
        Node addressNode = ParserUtils.getChildNodeWithTag(node, "Address");
        
        if (addressNode != null)
            address = new Address(addressNode);
    }

    public String getIedName() {
        return iedName;
    }

    public String getApName() {
        return apName;
    }
    
    public Address getAddress() {
        return address;
    }

    public List<GSE> getGses() {
        return gses;
    }

    public List<SMV> getSmvs() {
    	return smvs;
    }

    public GSE lookupGSE(String logicalDeviceName, String name) {

        for (GSE gse : this.getGses()) {
            if (gse.getLdInst().equals(logicalDeviceName)) {
                if (gse.getCbName().equals(name))
                    return gse;
            }
        }

        return null;
    }

    public SMV lookupSMV(String logicalDeviceName, String name) {

        for (SMV smv : this.getSmvs()) {
            if (smv.getLdInst().equals(logicalDeviceName)) {
                if (smv.getCbName().equals(name))
                    return smv;
            }
        }

        return null;
    }
    
    public PhyComAddress lookupSMVAddress(String logicalDeviceName, String name) {

        for (SMV smv : this.getSmvs()) {
            if (smv.getLdInst().equals(logicalDeviceName)) {
                if (smv.getCbName().equals(name))
                    return smv.getAddress();
            }
        }

        return null;
    }

}
