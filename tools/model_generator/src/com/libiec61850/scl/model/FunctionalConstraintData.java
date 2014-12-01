package com.libiec61850.scl.model;

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

public class FunctionalConstraintData {

	private String ldInstance = null;
	private String prefix = null;
	private String lnClass = null;
	private String lnInstance = null;
	private String doName = null;
	private String daName = null;
	private FunctionalConstraint fc = null;
	private Integer ix = null; /* array index */
	
	public FunctionalConstraintData(Node fcdaNode) throws SclParserException {
		this.ldInstance = ParserUtils.parseAttribute(fcdaNode, "ldInst");
		
		prefix = ParserUtils.parseAttribute(fcdaNode, "prefix");
		
		this.lnClass = ParserUtils.parseAttribute(fcdaNode, "lnClass");
		
		this.lnInstance= ParserUtils.parseAttribute(fcdaNode, "lnInst");
		
		this.doName = ParserUtils.parseAttribute(fcdaNode, "doName");
		this.daName = ParserUtils.parseAttribute(fcdaNode, "daName");
		
		String fc = ParserUtils.parseAttribute(fcdaNode, "fc");
		
		if (fc != null)
			this.fc = FunctionalConstraint.createFromString(fc);
		
		String index =  ParserUtils.parseAttribute(fcdaNode, "ix");
		
		if (index != null)
			this.ix = new Integer(index);
		
	}

	public String getLdInstance() {
		return ldInstance;
	}

	public String getLnClass() {
		return lnClass;
	}

	public String getLnInstance() {
		return lnInstance;
	}

	public String getDoName() {
		return doName;
	}

	public String getDaName() {
		return daName;
	}

	public FunctionalConstraint getFc() {
		return fc;
	}

	public Integer getIx() {
		return ix;
	}
	
	public String getPrefix() {
        return prefix;
    }

    @Override
	public String toString() {
		String string = "";
		
		if (ldInstance != null)
			string = ldInstance + "/";
		
		if (lnClass != null) {
		    
		    if (prefix != null)
		        string += prefix;
		    
			string += lnClass;
			if (lnInstance == null)
				string += ".";
		}
		
		if (lnInstance != null)
			string += lnInstance + ".";
		
		if (doName != null)
			string += doName;
		
		if (daName != null)
			string += "." + daName;
		
		return string;
	}
	
	
	
}
