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

import java.util.List;

import org.w3c.dom.DOMException;
import org.w3c.dom.Node;

import com.libiec61850.scl.ParserUtils;
import com.libiec61850.scl.SclParserException;

public class PhyComAddress {

    private Integer vlanId = null;
    private Integer vlanPriority = null;
    private Integer appId = null;
    private int[] macAddress = null;
    
    public PhyComAddress(Node addressNode) throws DOMException, SclParserException {
        
       List<Node> pNodes = ParserUtils.getChildNodesWithTag(addressNode, "P");
       
       for (Node pNode : pNodes) {
           String type = ParserUtils.parseAttribute(pNode, "type");
           
           if (type.equals("VLAN-ID")) {
               vlanId = Integer.parseInt(pNode.getTextContent(), 16);
               
               if (vlanId > 0xfff)
            	   throw new SclParserException(addressNode, "VLAN-ID value out of range");
           }
           else if (type.equals("VLAN-PRIORITY")) {
               vlanPriority = Integer.parseInt(pNode.getTextContent());
           }
           else if (type.equals("APPID")) {
               appId = Integer.parseInt(pNode.getTextContent(), 16);
               
               if (appId > 0xffff)
            	   throw new SclParserException(addressNode, "APPID value out of range");
           }
           else if (type.equals("MAC-Address")) {
               String[] addressElements = pNode.getTextContent().split("-");
               
               if (addressElements.length != 6)
                   throw new SclParserException(addressNode, "malformed address " + pNode.getTextContent());
               
               macAddress = new int[6];
               
               for (int i = 0; i < addressElements.length; i++) {
                   macAddress[i] = Integer.parseInt(addressElements[i], 16);           
               }
           }

       }
       
       if (vlanId == null)
    	   vlanId = new Integer(0);
       
       if (vlanPriority == null)
    	   vlanPriority = new Integer(4);
       
       if (macAddress == null) {
    	   macAddress = new int[6];
    	   /* default MAC address: 01-0C-CD-01-00-00 */

    	   macAddress[0] = 0x01;
    	   macAddress[1] = 0x0c;
    	   macAddress[2] = 0xcd;
    	   macAddress[3] = 0x01;
    	   macAddress[4] = 0x00;
    	   macAddress[5] = 0x00;
       }
    	   
       if (appId == null)
    	   appId = new Integer(0);
    }

    public Integer getVlanId() {
        return vlanId;
    }

    public Integer getVlanPriority() {
        return vlanPriority;
    }

    public Integer getAppId() {
        return appId;
    }

    public int[] getMacAddress() {
        return macAddress;
    }
    
}
