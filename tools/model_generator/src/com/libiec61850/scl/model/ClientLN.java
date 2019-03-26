package com.libiec61850.scl.model;

/*
 *  Copyright 2013-2019 Michael Zillgith, MZ Automation GmbH
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

public class ClientLN 
{
	private Node node;
	
    public ClientLN(Node node)
    {
    	this.node = node;
    }
	
    public String getIedName()
    {
        return ParserUtils.parseAttribute(node, "iedName");
    }
    
    public String getApRef()
    {
        return ParserUtils.parseAttribute(node, "apRef");
    }
    
    public String getLdInst()
    {
        return ParserUtils.parseAttribute(node, "ldInst");
    }
    
    public String getPrefix()
    {
        return ParserUtils.parseAttribute(node, "prefix");
    }
    
    public String getLnClass()
    {
        return ParserUtils.parseAttribute(node, "lnClass");
    }
    
    public String getLnInst()
    {
        return ParserUtils.parseAttribute(node, "lnInst");
    }
    
    public String getDesc()
    {
        return ParserUtils.parseAttribute(node, "desc");
    }
    
}
