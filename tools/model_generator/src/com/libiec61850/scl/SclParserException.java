package com.libiec61850.scl;

import org.w3c.dom.Node;

/*
 *  SclParserException.java
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

public class SclParserException extends Exception {

    private Node node = null;

    public SclParserException() {
    }

    public SclParserException(String message) {
        super(message);
    }

    public SclParserException(Node node, String message) {
        super(message);
        this.node = node;
    }

    @Override
    public String getMessage() {
        String message = null;

        if (node != null) {
            message = node.getNodeName() + " starting at line " + node.getUserData("START_LINE_NUMBER_ATTR") + " column "
                    + node.getUserData("START_COLUMN_NUMBER_ATTR") + ": " + super.getMessage();
        } else
            message = super.getMessage();

        return message;
    }

    private static final long serialVersionUID = 6243253854159814835L;

}
