package com.libiec61850.scl.model;

/*
 *  DataSet.java
 *
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

public class DataSet {

	private String name = null;
	private String desc = null;
	
	List<FunctionalConstraintData> fcda = null;

	public DataSet(Node dataSet) throws SclParserException {
		this.name = ParserUtils.parseAttribute(dataSet, "name");
		this.desc = ParserUtils.parseAttribute(dataSet, "desc");
		
		if (this.name == null)
			throw new SclParserException(dataSet, "Dataset misses required attribute \"name\"");
		
		fcda = new LinkedList<FunctionalConstraintData>();
		
		List<Node> fcdaNodes = ParserUtils.getChildNodesWithTag(dataSet, "FCDA");
		for (Node fcdaNode : fcdaNodes) {
			fcda.add(new FunctionalConstraintData(fcdaNode));
		}
		
		
	}

	public String getName() {
		return name;
	}

	public String getDesc() {
		return desc;
	}

	public List<FunctionalConstraintData> getFcda() {
		return fcda;
	}
	
}
