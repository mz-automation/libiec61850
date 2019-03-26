package com.libiec61850.scl.communication;

import java.util.LinkedList;
import java.util.List;

import org.w3c.dom.Node;

import com.libiec61850.scl.ParserUtils;
import com.libiec61850.scl.SclParserException;

public class Address
{
	Node node;
	
    private List<P> addressParameters = new LinkedList<P>();

    public Address(Node addressNode)
            throws SclParserException
    {
        node = addressNode;
        
        List<Node> pNodes = ParserUtils.getChildNodesWithTag(node, "P");

        for (Node pNode : pNodes)
            addressParameters.add(new P(pNode));
    }
	
    public List<P> getAddressParameters()
    {
        return addressParameters;
    }
    
    public P getAddressParameter(String type)
    {
        for (P p : addressParameters) {
            if (p.getType().equals(type))
                return p;
        }
        
        return null;
    }
}
