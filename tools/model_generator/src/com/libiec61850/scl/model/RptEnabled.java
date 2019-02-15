package com.libiec61850.scl.model;

import java.util.LinkedList;
import java.util.List;

import org.w3c.dom.Node;

import com.libiec61850.scl.ParserUtils;

public class RptEnabled {
	
	private int maxInstances = 1;
	private String desc = null;
	
	private List<ClientLN> clientLNs = new LinkedList<ClientLN>();

	public RptEnabled(Node rptEnabledNode) {
		this.desc = ParserUtils.parseAttribute(rptEnabledNode, "desc");
		String maxString = ParserUtils.parseAttribute(rptEnabledNode, "max");
		
		if (maxString != null) {
			maxInstances = new Integer(maxString);
		}
		
		List<Node> clientLNNodes = ParserUtils.getChildNodesWithTag(rptEnabledNode, "ClientLN");
		
		for (Node clientLNNode : clientLNNodes) {
			ClientLN clientLN = new ClientLN(clientLNNode);
		    
		    clientLNs.add(clientLN);
		}
	}

	public int getMaxInstances() {
		return maxInstances;
	}

	public String getDesc() {
		return desc;
	}
	
	public List<ClientLN> getClientLNs()
	{
	    return clientLNs;
	}
}
