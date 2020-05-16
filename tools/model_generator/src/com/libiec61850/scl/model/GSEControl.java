package com.libiec61850.scl.model;

/*
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

import com.libiec61850.scl.ParserUtils;
import com.libiec61850.scl.SclParserException;

public class GSEControl {
   
    private String name;
    private String desc = null;
    private String dataSet;
    private int confRev = 1;
    private String appID;
    private boolean fixedOffs = false;

    public GSEControl(Node gseControlNode) throws SclParserException {

        this.name = ParserUtils.parseAttribute(gseControlNode, "name");
        this.desc = ParserUtils.parseAttribute(gseControlNode, "desc");
        this.dataSet = ParserUtils.parseAttribute(gseControlNode, "datSet");

        String confRevString = ParserUtils.parseAttribute(gseControlNode,
                "confRev");

        if (confRevString != null)
            this.confRev = new Integer(confRevString);

        this.appID = ParserUtils.parseAttribute(gseControlNode, "appID");

        Boolean fixedOffs = ParserUtils.parseBooleanAttribute(gseControlNode,
                "fixedOffs");

        if (fixedOffs != null)
            this.fixedOffs = fixedOffs;

        String typeString = ParserUtils.parseAttribute(gseControlNode, "type");

        if (typeString != null)
            if (!typeString.equals("GOOSE"))
                throw new SclParserException(gseControlNode,
                        "GSEControl of type " + typeString + " not supported!");

    }

    public String getName() {
        return name;
    }

    public String getDesc() {
        return desc;
    }

    public String getDataSet() {
        return dataSet;
    }

    public int getConfRev() {
        return confRev;
    }

    public String getAppID() {
        return appID;
    }

    public boolean isFixedOffs() {
        return fixedOffs;
    }

}
