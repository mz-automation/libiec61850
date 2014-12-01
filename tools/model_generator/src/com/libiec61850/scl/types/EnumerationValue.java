package com.libiec61850.scl.types;

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

import org.w3c.dom.Node;

import com.libiec61850.scl.ParserUtils;
import com.libiec61850.scl.SclParserException;

public class EnumerationValue {
	private String symbolicName;
	private int ord;
	
	public EnumerationValue(String symbolicName, int ord) {
		this.symbolicName = symbolicName;
		this.ord = ord;
	}
	
	public EnumerationValue(Node xmlNode) throws SclParserException {
		String ordString = ParserUtils.parseAttribute(xmlNode, "ord");
		
		if (ordString == null)
			throw new SclParserException(xmlNode, "ord attribute missing");
		
		this.ord = new Integer(ordString);
		
		this.symbolicName = xmlNode.getTextContent();
	}

	public String getSymbolicName() {
		return symbolicName;
	}

	public int getOrd() {
		return ord;
	}
	
	

}
