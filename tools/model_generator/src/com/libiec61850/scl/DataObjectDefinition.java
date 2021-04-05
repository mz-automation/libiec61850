package com.libiec61850.scl;

/*
 *  DataObjectDefinition.java
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

import org.w3c.dom.Node;

public class DataObjectDefinition {
    private String name;
    private String type;
    private boolean trans = false; /* transient attribute value */
    private int count = 0;

    public DataObjectDefinition(Node dataObjectNode) throws SclParserException {
        this.name = ParserUtils.parseAttribute(dataObjectNode, "name");
        this.type = ParserUtils.parseAttribute(dataObjectNode, "type");
        
        
        Boolean isTransient = ParserUtils.parseBooleanAttribute(dataObjectNode, "transient");
        
        if (isTransient != null)
            this.trans = isTransient;

        if ((this.type == null) || (this.name == null))
            throw new SclParserException(dataObjectNode, "DO misses required attribute.");

        String countStr = ParserUtils.parseAttribute(dataObjectNode, "count");
        if (countStr != null)
            count = new Integer(countStr);
    }

    public String getName() {
        return name;
    }

    public String getType() {
        return type;
    }

    public int getCount() {
        return count;
    }
    
    public boolean isTransient() {
        return trans;
    }

}
