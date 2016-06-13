package com.libiec61850.scl.model;

import org.w3c.dom.Node;

import com.libiec61850.scl.ParserUtils;
import com.libiec61850.scl.SclParserException;

/*
 *  Copyright 2014-2016 Michael Zillgith
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

public class LogControl {
	
	private String name;
	private String desc;
	private String dataSet;
	private String ldInst = null;
	private String prefix = "";
	private String lnClass = "LLN0";
	private String lnInst = "";
	private String logName;
	private boolean logEna = true;
	private boolean reasonCode = true;
	private int intgPd = 0; // integrity period in ms
	private TriggerOptions triggerOptions;
	
    public LogControl(Node logControlNode) throws SclParserException {
        name = ParserUtils.parseAttribute(logControlNode, "name");
        
        if (name == null)
        	throw new SclParserException(logControlNode, "LogControl is missing required attribute \"name\"");
        
        desc = ParserUtils.parseAttribute(logControlNode, "desc");
        dataSet = ParserUtils.parseAttribute(logControlNode, "datSet");
        
        if (dataSet != null) 
            if (dataSet.equals(""))
                dataSet = null;
        
        ldInst = ParserUtils.parseAttribute(logControlNode, "ldInst");
        prefix = ParserUtils.parseAttribute(logControlNode, "prefix");
        
        String lnClassString = ParserUtils.parseAttribute(logControlNode, "lnClass");
        
        if (lnClassString != null)
        	lnClass = lnClassString;
        
        String lnInstString = ParserUtils.parseAttribute(logControlNode, "lnInst");
        
        if (lnInstString != null)
            lnInst = lnInstString;
        
        logName = ParserUtils.parseAttribute(logControlNode, "logName");
        
        if (logName == null)
        	throw new SclParserException(logControlNode, "LogControl is missing required attribute \"logName\"");
        
        if (logName.equals(""))
            logName = null;
        
        String intgPdString = ParserUtils.parseAttribute(logControlNode, "intgPd");

        if (intgPdString != null)
        	intgPd = Integer.decode(intgPdString);
        
        Boolean logEnaBoolean = ParserUtils.parseBooleanAttribute(logControlNode, "logEna");
        if (logEnaBoolean != null)
        	logEna = logEnaBoolean;
        
        Boolean reasonCodeBoolean = ParserUtils.parseBooleanAttribute(logControlNode, "reasonCode");
        if (reasonCodeBoolean != null)
        	reasonCode = reasonCodeBoolean;

		Node trgOpsNode = ParserUtils.getChildNodeWithTag(logControlNode, "TrgOps");
				
		if (trgOpsNode != null)
			this.triggerOptions = new TriggerOptions(trgOpsNode);
		else
			this.triggerOptions = new TriggerOptions(); // use default values if no node present

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

	public String getLdInst() {
		return ldInst;
	}

	public String getPrefix() {
		return prefix;
	}

	public String getLnClass() {
		return lnClass;
	}

	public String getLogName() {
		return logName;
	}

	public boolean isLogEna() {
		return logEna;
	}

	public boolean isReasonCode() {
		return reasonCode;
	}

	public int getIntgPd() {
		return intgPd;
	}

	public TriggerOptions getTriggerOptions() {
		return triggerOptions;
	}
    
    
}
