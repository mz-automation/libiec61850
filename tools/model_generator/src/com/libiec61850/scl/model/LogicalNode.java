package com.libiec61850.scl.model;

/*
 *  Copyright 2013, 2014 Michael Zillgith
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

import org.w3c.dom.Node;

import com.libiec61850.scl.DataObjectDefinition;
import com.libiec61850.scl.ParserUtils;
import com.libiec61850.scl.SclParserException;
import com.libiec61850.scl.types.IllegalValueException;
import com.libiec61850.scl.types.LogicalNodeType;
import com.libiec61850.scl.types.SclType;
import com.libiec61850.scl.types.TypeDeclarations;

public class LogicalNode implements DataModelNode {

    private String lnClass;
    private String lnType;
    private String inst;
    private String desc;
    private String prefix;
    private SclType sclType;

    private List<DataObject> dataObjects;
    private List<DataSet> dataSets;
    private List<ReportControlBlock> reportControlBlocks;
    private List<GSEControl> gseControlBlocks;
    private List<SampledValueControl> smvControlBlocks;
    private List<LogControl> logControlBlocks;
	private List<Log> logs;
	private List<SettingControl> settingGroupControlBlocks;
	
	private LogicalDevice parentLogicalDevice;


    public LogicalNode(Node lnNode, TypeDeclarations typeDeclarations, LogicalDevice parent) throws SclParserException {
        this.lnClass = ParserUtils.parseAttribute(lnNode, "lnClass");
        this.lnType = ParserUtils.parseAttribute(lnNode, "lnType");
        this.inst = ParserUtils.parseAttribute(lnNode, "inst");
        this.desc = ParserUtils.parseAttribute(lnNode, "desc");
        this.prefix = ParserUtils.parseAttribute(lnNode, "prefix");
        this.parentLogicalDevice = parent;
       
        if (this.lnClass == null)
            throw new SclParserException(lnNode, "required attribute \"lnClass\" is missing in logical node.");

        if (this.lnType == null)
            throw new SclParserException(lnNode, "required attribute \"lnType\" is missing in logical node.");

        if (this.inst == null)
            throw new SclParserException(lnNode, "required attribute \"inst\" is missing in logical node.");

        // instantiate DataObjects
        this.sclType = typeDeclarations.lookupType(this.lnType);

        if (sclType == null)
            throw new SclParserException(lnNode, "missing type declaration " + this.lnType);

        if (sclType instanceof LogicalNodeType) {
            dataObjects = new LinkedList<DataObject>();

            LogicalNodeType type = (LogicalNodeType) sclType;
            
            /* mark type as used */
            type.setUsed(true);

            List<DataObjectDefinition> doDefinitions = type.getDataObjectDefinitions();

            for (DataObjectDefinition doDefinition : doDefinitions) {
                dataObjects.add(new DataObject(doDefinition, typeDeclarations, this));
            }

        } else
            throw new SclParserException(lnNode, "wrong type " + this.lnType + " for logical node");

        /* Parse data set definitions */
        dataSets = new LinkedList<DataSet>();

        List<Node> dataSetNodes = ParserUtils.getChildNodesWithTag(lnNode, "DataSet");
        for (Node dataSet : dataSetNodes)
            dataSets.add(new DataSet(dataSet));

        /* Parse report control block definitions */
        reportControlBlocks = new LinkedList<ReportControlBlock>();

        List<Node> reportControlNodes = ParserUtils.getChildNodesWithTag(lnNode, "ReportControl");

        for (Node reportControlNode : reportControlNodes)
            reportControlBlocks.add(new ReportControlBlock(reportControlNode));

        /* Parse GSE control block definitions */
        gseControlBlocks = new LinkedList<GSEControl>();

        List<Node> gseControlNodes = ParserUtils.getChildNodesWithTag(lnNode, "GSEControl");
        for (Node gseControlNode : gseControlNodes)
            gseControlBlocks.add(new GSEControl(gseControlNode));
        
        /* Parse Sampled Values (SV) control block definitions */
        smvControlBlocks = new LinkedList<SampledValueControl>();

        List<Node> svControlNodes = ParserUtils.getChildNodesWithTag(lnNode, "SampledValueControl");
        for (Node svControlNode : svControlNodes)
            smvControlBlocks.add(new SampledValueControl(svControlNode));
        
        /* Parse log control block definitions */
        logControlBlocks = new LinkedList<LogControl>();
        
        List<Node> logControlNodes = ParserUtils.getChildNodesWithTag(lnNode, "LogControl");
        for (Node logControlNode : logControlNodes)
        	logControlBlocks.add(new LogControl(logControlNode));
        
        /* Parse logs */
        logs = new LinkedList<Log>();
        
        List<Node> logNodes = ParserUtils.getChildNodesWithTag(lnNode, "Log");
        for (Node logNode : logNodes)
        	logs.add(new Log(logNode));

        
        /* Parse setting group control block definitions */
        settingGroupControlBlocks = new LinkedList<SettingControl>();
        
        List<Node> sgNodes = ParserUtils.getChildNodesWithTag(lnNode, "SettingControl");
        
        if ((this.lnClass.equals("LLN0") == false) && (sgNodes.size() > 0))
        	throw new SclParserException(lnNode, "LN other than LN0 is not allowed to contain SettingControl");
        
        if (sgNodes.size() > 1)
        	throw new SclParserException(lnNode, "LN contains more then one SettingControl");
        
        for (Node sgNode : sgNodes)
        	settingGroupControlBlocks.add(new SettingControl(sgNode));
           
        /* Parse data object instances */
        List<Node> doiNodes = ParserUtils.getChildNodesWithTag(lnNode, "DOI");

        for (Node doiNode : doiNodes) {
            String doiName = ParserUtils.parseAttribute(doiNode, "name");

            DataObject dataObject = (DataObject) getChildByName(doiName);

            if (dataObject == null)
                throw new SclParserException(doiNode, "Missing data object with name \"" + doiName + "\"");

            parseDataAttributeNodes(doiNode, dataObject);

            parseSubDataInstances(doiNode, dataObject);
        }
    }

	private void parseDataAttributeNodes(Node doiNode, DataModelNode dataObject)
			throws SclParserException {
		List<Node> daiNodes = ParserUtils.getChildNodesWithTag(doiNode, "DAI");

		for (Node daiNode : daiNodes) {
		    String daiName = ParserUtils.parseAttribute(daiNode, "name");

		    DataAttribute dataAttribute = (DataAttribute) dataObject.getChildByName(daiName);

		    if (dataAttribute == null)
		        throw new SclParserException(daiNode, "Missing data attribute with name \"" + daiName + "\"");

		    Node valNode = ParserUtils.getChildNodeWithTag(daiNode, "Val");

		    if (valNode != null) {
		        String value = valNode.getTextContent();

		        try {
		            dataAttribute.setValue(new DataModelValue(dataAttribute.getType(), dataAttribute.getSclType(), value));
		        } catch (IllegalValueException e) {
		            throw new SclParserException(valNode, e.getMessage());
		        }
		    }

		    String shortAddress = ParserUtils.parseAttribute(daiNode, "sAddr");

		    if (shortAddress != null) {
		    	
		    	if (!shortAddress.equals(""))
			        dataAttribute.setShortAddress(shortAddress);
		    }

		}
	}

	private void parseSubDataInstances(Node doiNode, DataModelNode dataObject)
			throws SclParserException {
		List<Node> sdiNodes = ParserUtils.getChildNodesWithTag(doiNode, "SDI");

		for (Node sdiNode : sdiNodes) {
		    String sdiName = ParserUtils.parseAttribute(sdiNode, "name");

		    DataModelNode subDataAttribute = dataObject.getChildByName(sdiName);

		    if (subDataAttribute == null)
		        System.out.println("subelement with name " + sdiName + " not found!");
		    
		    parseDataAttributeNodes(sdiNode, subDataAttribute);
		    
		    parseSubDataInstances(sdiNode,  subDataAttribute);
		}
	}

    public String getLnClass() {
        return lnClass;
    }

    public String getLnType() {
        return lnType;
    }

    public String getInst() {
        return inst;
    }

    public String getDesc() {
        return desc;
    }

    public String getPrefix() {
        return prefix;
    }

    public String getName() {
        String name = "";

        if (prefix != null)
            name += prefix;

        name += lnClass;

        name += inst;

        return name;
    }

    public List<DataObject> getDataObjects() {
        return dataObjects;
    }

    public List<DataSet> getDataSets() {
        return dataSets;
    }

    public List<ReportControlBlock> getReportControlBlocks() {
        return reportControlBlocks;
    }

    public List<GSEControl> getGSEControlBlocks() {
        return gseControlBlocks;
    }
    
    public List<SampledValueControl> getSampledValueControlBlocks() {
        return smvControlBlocks;
    }
    
    public List<SettingControl> getSettingGroupControlBlocks() {
		return settingGroupControlBlocks;
	}

    public List<LogControl> getLogControlBlocks() {
        return logControlBlocks;
    }
    
    public List<Log> getLogs() {
        return logs;
    }
    
    @Override
    public DataModelNode getChildByName(String childName) {
        for (DataObject dataObject : dataObjects) {
            if (dataObject.getName().equals(childName))
                return dataObject;
        }

        return null;
    }

    
    public LogicalDevice getParentLogicalDevice() {
        return parentLogicalDevice;
    }

    @Override
    public SclType getSclType() {
        return sclType;
    }

    @Override
    public DataModelNode getParent() {
        return null;
    }

}
