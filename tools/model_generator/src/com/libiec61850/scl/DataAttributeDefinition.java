package com.libiec61850.scl;

/*
 *  DataAttributeDefinition.java
 *
 *  Copyright 2013 Michael Zillgith
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

import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import com.libiec61850.scl.model.AttributeType;
import com.libiec61850.scl.model.DataModelValue;
import com.libiec61850.scl.model.FunctionalConstraint;
import com.libiec61850.scl.model.TriggerOptions;
import com.libiec61850.scl.types.IllegalValueException;
import com.libiec61850.scl.types.SclType;

public class DataAttributeDefinition {

    private String name = null;
    private String bType = null;
    private String type = null;
    private int count = 0;
    private FunctionalConstraint fc = null;
    private AttributeType attributeType = null;
    private TriggerOptions triggerOptions = null;
    private DataModelValue value = null;

	public DataAttributeDefinition(Node node) throws SclParserException {
        this.name = ParserUtils.parseAttribute(node, "name");
        this.bType = ParserUtils.parseAttribute(node, "bType");
        this.type = ParserUtils.parseAttribute(node, "type");
        String fcString = ParserUtils.parseAttribute(node, "fc");

        if (this.name == null)
            throw new SclParserException(node, "attribute name is missing");

        if (fcString != null)
            this.fc = FunctionalConstraint.createFromString(fcString);

        if (this.bType == null)
            throw new SclParserException(node, "attribute bType is missing");
        else {
            if (this.bType.equals("Tcmd"))
                this.type = "Tcmd";
            else if (this.bType.equals("Dbpos"))
                this.type = "Dbpos";
            else if (this.bType.equals("Check"))
                this.type = "Check";

            attributeType = AttributeType.createFromSclString(this.bType);
        }

        boolean dchgTrigger = false;
        boolean qchgTrigger = false;
        boolean dupdTrigger = false;

        String dchg = ParserUtils.parseAttribute(node, "dchg");

        if (dchg != null)
            dchgTrigger = new Boolean(dchg);

        String dupd = ParserUtils.parseAttribute(node, "dupd");

        if (dupd != null)
            dupdTrigger = new Boolean(dupd);

        String qchg = ParserUtils.parseAttribute(node, "qchg");

        if (qchg != null)
            qchgTrigger = new Boolean(qchg);

        this.triggerOptions = new TriggerOptions(dchgTrigger, qchgTrigger, dupdTrigger, false, false);

        String countStr = ParserUtils.parseAttribute(node, "count");
        if (countStr != null)
            count = new Integer(countStr);

        
        if (this.bType != null) {
        
        NodeList elementNodes = node.getChildNodes();
		
			if (elementNodes != null) {
	
				for (int i = 0; i < elementNodes.getLength(); i++) {
					Node elementNode = elementNodes.item(i);
					if (elementNode.getNodeName().equals("Val")) {
						
						String value = elementNode.getTextContent();
						
						if (attributeType == AttributeType.ENUMERATED) {
							this.value = new DataModelValue(attributeType, this.type, value);
						} 
						else {
							try {
								this.value = new DataModelValue(attributeType, (SclType) null, value);
							} catch (IllegalValueException e) {
								// TODO Auto-generated catch block
								e.printStackTrace();
							}
						}
							
					}	
				}
			}
        }   
    }

    public String getName() {
        return name;
    }

    public String getbType() {
        return bType;
    }

    public String getType() {
        return type;
    }

    public FunctionalConstraint getFc() {
        return fc;
    }

    public AttributeType getAttributeType() {
        return attributeType;
    }

    public int getCount() {
        return count;
    }

    public DataModelValue getValue() {
    	return value;
    }
    
    public TriggerOptions getTriggerOptions() {
        return this.triggerOptions;
    }

}
