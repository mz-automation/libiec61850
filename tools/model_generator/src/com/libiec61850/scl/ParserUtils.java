package com.libiec61850.scl;

/*
 *  ParserUtils.java
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
import java.util.List;

import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

public class ParserUtils {
    public static String parseAttribute(Node lnNode, String attributeName) {
        NamedNodeMap attributes = lnNode.getAttributes();
        String attributeValue = null;

        if (attributes != null) {

            Node attributeNode;

            for (int i = 0; i < attributes.getLength(); i++) {
                attributeNode = attributes.item(i);

                if (attributeNode.getNodeName().equals(attributeName))
                    attributeValue = attributeNode.getNodeValue();
            }
        }

        return attributeValue;
    }

    public static Node getChildNodeWithTag(Node node, String nodeTag) {
        NodeList elements = node.getChildNodes();

        if (elements != null) {
            Node childNode;

            for (int i = 0; i < elements.getLength(); i++) {
                childNode = elements.item(i);

                if (childNode.getNodeName().equals(nodeTag))
                    return childNode;
            }
        }

        return null;
    }

    public static List<Node> getChildNodesWithTag(Node node, String nodeTag) {
        List<Node> nodeList = new LinkedList<Node>();
        NodeList elements = node.getChildNodes();

        if (elements != null) {
            Node childNode;

            for (int i = 0; i < elements.getLength(); i++) {
                childNode = elements.item(i);

                if (childNode.getNodeName().equals(nodeTag))
                    nodeList.add(childNode);

            }
        }

        return nodeList;
    }

    public static Boolean parseBooleanAttribute(Node xmlNode, String attributeName) throws SclParserException {
        String valueStr = parseAttribute(xmlNode, attributeName);

        boolean value;

        if (valueStr == null)
            return null;

        if (valueStr.toUpperCase().equals("TRUE"))
            value = true;
        else if (valueStr.toUpperCase().equals("FALSE"))
            value = false;
        else
            throw new SclParserException(xmlNode, "Illegal value for boolean attribute \"" + attributeName + "\"");

        return new Boolean(value);
    }

}
