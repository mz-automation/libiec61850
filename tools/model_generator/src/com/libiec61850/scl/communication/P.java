package com.libiec61850.scl.communication;

import org.w3c.dom.Node;

import com.libiec61850.scl.ParserUtils;
import com.libiec61850.scl.SclParserException;

public class P 
{
	private Node node;
	
    private String type;

    public P(Node pNode) throws SclParserException
    {
        node = pNode;
        
        this.type = ParserUtils.parseAttribute(node, "type");

        if (this.type == null)
            throw new SclParserException(node, "type is missing in P element!");
    }
    
    public String getType()
    {
        return type;
    }
        
    public String getText()
    {
        return node.getTextContent();
    }
    
    public void setText(String text)
    {
        node.setTextContent(text);
    }
}
