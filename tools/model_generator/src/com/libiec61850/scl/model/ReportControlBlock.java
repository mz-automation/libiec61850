package com.libiec61850.scl.model;

/*
 *  Copyright 2013, 2014 Michael Zillgith
 *
 *	This file is part of libIEC61850.
 *
 *	libIEC61850 is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	libIEC61850 is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with libIEC61850.  If not, see <http://www.gnu.org/licenses/>.
 *
 *	See COPYING file for the complete license text.
 */

import org.w3c.dom.Node;

import com.libiec61850.scl.ParserUtils;
import com.libiec61850.scl.SclParserException;

public class ReportControlBlock {

	private String name;
	private String desc;
	private String dataSet;
	private Long integrityPeriod = null;
	private String rptID;
	private Long confRef = null;
	private boolean buffered = false;
	private long bufferTime = 0;
	private TriggerOptions triggerOptions = null;
	private OptionFields optionFields = null;
	private boolean indexed = true;
	private RptEnabled rptEna = null;
	
	public ReportControlBlock(Node reportControlNode) throws SclParserException {
	
		this.name = ParserUtils.parseAttribute(reportControlNode, "name");
		this.desc = ParserUtils.parseAttribute(reportControlNode, "desc");
		this.dataSet = ParserUtils.parseAttribute(reportControlNode, "datSet");
		String intgPdString = ParserUtils.parseAttribute(reportControlNode, "intgPd");
		
		if (intgPdString != null)
			this.integrityPeriod = new Long(intgPdString);
		
		this.rptID = ParserUtils.parseAttribute(reportControlNode, "rptID");
		
		if (this.rptID != null) {
			if (this.rptID.equals(""))
				this.rptID = null;
		}
		
		String confRefStr = ParserUtils.parseAttribute(reportControlNode, "confRev");
		
		if (confRefStr != null)
			this.confRef = new Long(confRefStr);
		else
			throw new SclParserException("Missing required attribute \"confRef\"");
		
		Boolean bufferedVal = ParserUtils.parseBooleanAttribute(reportControlNode, "buffered");
		
		if (bufferedVal != null)
				this.buffered = bufferedVal;
		
		String bufferTimeStr = ParserUtils.parseAttribute(reportControlNode, "bufTime");
		
		if (bufferTimeStr != null)
			this.bufferTime = new Long(bufferTimeStr);
				
		Node trgOpsNode = ParserUtils.getChildNodeWithTag(reportControlNode, "TrgOps");
		
		if (trgOpsNode != null)
			this.triggerOptions = new TriggerOptions(trgOpsNode);
		else
			this.triggerOptions = new TriggerOptions(); // use default values if no node present
		
		Node optFieldsNode = ParserUtils.getChildNodeWithTag(reportControlNode, "OptFields");
		
		this.optionFields = new OptionFields(optFieldsNode);
		
		Boolean indexed = ParserUtils.parseBooleanAttribute(reportControlNode, "indexed");
		
		if (indexed != null)
			this.indexed = indexed.booleanValue();
		
		Node rptEnabledNode = ParserUtils.getChildNodeWithTag(reportControlNode, "RptEnabled");
		
		if (rptEnabledNode != null) {
			RptEnabled rptEna = new RptEnabled(rptEnabledNode);
			
			if (this.indexed == false) {
				if (rptEna.getMaxInstances() != 1)
					throw new SclParserException("RptEnabled.max != 1 is not allowed when indexed=\"false\"");
			}
			
			this.rptEna = rptEna;
		}
		
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

	public Long getIntegrityPeriod() {
		return integrityPeriod;
	}

	public String getRptID() {
		return rptID;
	}

	public Long getConfRef() {
		return confRef;
	}

	public boolean isBuffered() {
		return buffered;
	}

	public long getBufferTime() {
		return bufferTime;
	}

	public TriggerOptions getTriggerOptions() {
		return triggerOptions;
	}

	public OptionFields getOptionFields() {
		return optionFields;
	}

	public boolean isIndexed() {
		return indexed;
	}

	public RptEnabled getRptEna() {
		return rptEna;
	}
}
