package com.libiec61850.scl.model;

import org.w3c.dom.Node;

import com.libiec61850.scl.ParserUtils;

public class RptEnabled {
	
	private int maxInstances = 1;
	private String desc = null;

	public RptEnabled(Node rptEnabledNode) {
		this.desc = ParserUtils.parseAttribute(rptEnabledNode, "desc");
		String maxString = ParserUtils.parseAttribute(rptEnabledNode, "max");
		
		if (maxString != null) {
			maxInstances = new Integer(maxString);
		}
	}

	public int getMaxInstances() {
		return maxInstances;
	}

	public String getDesc() {
		return desc;
	}
}
