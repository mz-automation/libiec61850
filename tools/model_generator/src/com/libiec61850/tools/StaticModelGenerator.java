package com.libiec61850.tools;

/*
 *  StaticModelGenerator.java
 *
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

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.PrintStream;
import java.util.LinkedList;
import java.util.List;

import com.libiec61850.scl.SclParser;
import com.libiec61850.scl.SclParserException;
import com.libiec61850.scl.communication.Communication;
import com.libiec61850.scl.communication.ConnectedAP;
import com.libiec61850.scl.communication.GSE;
import com.libiec61850.scl.communication.GSEAddress;
import com.libiec61850.scl.communication.SubNetwork;
import com.libiec61850.scl.model.AccessPoint;
import com.libiec61850.scl.model.DataAttribute;
import com.libiec61850.scl.model.DataModelValue;
import com.libiec61850.scl.model.DataObject;
import com.libiec61850.scl.model.DataSet;
import com.libiec61850.scl.model.FunctionalConstraint;
import com.libiec61850.scl.model.FunctionalConstraintData;
import com.libiec61850.scl.model.GSEControl;
import com.libiec61850.scl.model.IED;
import com.libiec61850.scl.model.LogicalDevice;
import com.libiec61850.scl.model.LogicalNode;
import com.libiec61850.scl.model.ReportControlBlock;
import com.libiec61850.scl.model.Server;
import com.libiec61850.scl.model.SettingControl;
import com.libiec61850.scl.model.TriggerOptions;

public class StaticModelGenerator {

    private List<String> variablesList;
    private PrintStream cOut;
    private PrintStream hOut;

    private StringBuffer initializerBuffer;

    private StringBuffer reportControlBlocks;
    private List<String> rcbVariableNames;
    private int currentRcbVariableNumber = 0;

    private StringBuffer gseControlBlocks;
    private List<String> gseVariableNames;
    private int currentGseVariableNumber = 0;
    
    private StringBuffer settingGroupControlBlocks;
    private List<String> sgcbVariableNames;
    private int currentSGCBVariableNumber = 0;

    private List<String> dataSetNames;

    private Communication communication;

    private ConnectedAP connectedAP;
    
    private IED ied;
    private AccessPoint accessPoint;

	private String outputFileName;
	private String hDefineName;
	private String modelPrefix;
	private boolean initializeOnce;

    public StaticModelGenerator(InputStream stream, String icdFile, PrintStream cOut, PrintStream hOut,
    		String outputFileName, String iedName, String accessPointName, String modelPrefix,
			boolean initializeOnce) throws SclParserException 
    {
        this.cOut = cOut;
        this.hOut = hOut;
        this.initializerBuffer = new StringBuffer();
        this.reportControlBlocks = new StringBuffer();
        this.rcbVariableNames = new LinkedList<String>();
        this.gseControlBlocks = new StringBuffer();
        this.gseVariableNames = new LinkedList<String>();
        
        this.settingGroupControlBlocks = new StringBuffer();
        this.sgcbVariableNames = new LinkedList<String>();

        SclParser sclParser = new SclParser(stream);

		this.outputFileName = outputFileName;
		this.hDefineName = outputFileName.toUpperCase().replace( '.', '_' ).replace( '-', '_' ) + "_H_";
		this.modelPrefix = modelPrefix;
		this.initializeOnce = initializeOnce;

		if( hDefineName.lastIndexOf( '/' ) >= 0 )
		{
			hDefineName = hDefineName.substring( hDefineName.lastIndexOf( '/' ) + 1 );
		}

        ied = null; 
        
        if (iedName == null)
        	ied = sclParser.getFirstIed();
        else
        	ied = sclParser.getIedByteName(iedName);

        if (ied == null)
            System.out.println("IED model not found in SCL file! Exit.");
       
        accessPoint = null;

        if (accessPointName == null)
        	accessPoint = ied.getFirstAccessPoint();
        else
        	accessPoint = ied.getAccessPointByName(accessPointName);

        connectedAP = sclParser.getConnectedAP(ied, accessPoint.getName());

        printCFileHeader(icdFile);

        printHeaderFileHeader(icdFile);

        variablesList = new LinkedList<String>();

        Server server = accessPoint.getServer();

        printForwardDeclarations(server);

        printDeviceModelDefinitions();

        printInitializerFunction();

        printVariablePointerDefines();

        printHeaderFileFooter();
    }

    private void printInitializerFunction() {
        cOut.println("\nstatic void\ninitializeValues()");
        cOut.println("{");
        cOut.print(this.initializerBuffer);
        cOut.println("}");
    }

    public static void main(String[] args) throws FileNotFoundException  {
        if (args.length < 1) {
            System.out.println("Usage: genmodel <ICD file>  [-ied  <ied-name>] [-ap <access-point-name>] [-out <output-name>] [-modelprefix <model-prefix>]");
            System.exit(1);
        }

        String icdFile = args[0];

		String outputFileName = "static_model";
        
        String accessPointName = null;
        String iedName = null;
		String modelPrefix = "iedModel";
		boolean initializeOnce = false;
        
        if (args.length > 1) {
        	for (int i = 1; i < args.length; i++) {
        		if (args[i].equals("-ap")) {
        			accessPointName = args[i+1];
        			
        			System.out.println("Select access point " + accessPointName);
        			
        			i++;
        		}
        		else if (args[i].equals("-ied")) {
        			iedName = args[i+1];
        			
        			System.out.println("Select IED " + iedName);
        			
        			i++;
        			
        		}
        		else if (args[i].equals("-out")) {
					outputFileName = args[i+1];

        			System.out.println("Select Output File " + outputFileName);

        			i++;
        			
        		}
        		else if (args[i].equals("-modelprefix")) {
					modelPrefix = args[i+1];

        			System.out.println("Select Model Prefix " + modelPrefix);

        			i++;
        			
        		}
        		else if (args[i].equals("-initializeonce")) {
					initializeOnce = true;

        			System.out.println("Select Initialize Once");

        		}
        		else {
        			 System.out.println("Unknown option: \"" + args[i] + "\"");
        		}
        	}
        	      
        }

        PrintStream cOutStream = new PrintStream(new FileOutputStream(new File(outputFileName + ".c")));
        PrintStream hOutStream = new PrintStream(new FileOutputStream(new File(outputFileName + ".h")));

		System.out.println("Select ICD File " + icdFile);
        InputStream stream = new FileInputStream(icdFile);

        try {
			new StaticModelGenerator(stream, icdFile, cOutStream, hOutStream, outputFileName, iedName, accessPointName, modelPrefix, initializeOnce);
        } catch (SclParserException e) {
			System.err.println("ERROR: " + e.getMessage());
		}
    }

    private void printVariablePointerDefines() {
        hOut.println("\n\n");

        for (String variableName : variablesList) {
			String name = modelPrefix.toUpperCase() + variableName.substring( modelPrefix.length() );
            hOut.println("#define " + name + " (&" + variableName + ")");
        }
    }

//    private String getLogicalDeviceName(LogicalDevice logicalDevice) {
//        String logicalDeviceName = logicalDevice.getLdName();
//
//        if (logicalDeviceName == null)
//            logicalDeviceName = ied.getName() + logicalDevice.getInst();
//
//        return logicalDeviceName;
//    }

    private String getLogicalDeviceInst(LogicalDevice logicalDevice) {
    	return logicalDevice.getInst();
    }
    
    private void printDeviceModelDefinitions() {

        printDataSets();

        List<LogicalDevice> logicalDevices = accessPoint.getServer().getLogicalDevices();
        
        createReportVariableList(logicalDevices);
        
        createGooseVariableList(logicalDevices);
        
        createSettingControlsVariableList(logicalDevices);

        for (int i = 0; i < logicalDevices.size(); i++) {

            LogicalDevice logicalDevice = logicalDevices.get(i);

            String ldName = modelPrefix + "_" + logicalDevice.getInst();

            variablesList.add(ldName);

            String logicalDeviceName = getLogicalDeviceInst(logicalDevice);

            cOut.println("\nLogicalDevice " + ldName + " = {");

            cOut.println("    LogicalDeviceModelType,");

            cOut.println("    \"" + logicalDeviceName + "\",");

            cOut.println("    (ModelNode*) &" + modelPrefix + ",");

            if (i < (logicalDevices.size() - 1))
                cOut.println("    (ModelNode*) &" + modelPrefix + "_" + logicalDevices.get(i + 1).getInst() + ",");
            else
                cOut.println("    NULL,");

            String firstChildName = ldName + "_" + logicalDevice.getLogicalNodes().get(0).getName();

            cOut.println("    (ModelNode*) &" + firstChildName);
            cOut.println("};\n");

            printLogicalNodeDefinitions(ldName, logicalDevice.getLogicalNodes());
        }

        
        for (String rcb : rcbVariableNames)
        	cOut.println("extern ReportControlBlock " + rcb + ";");
        
        cOut.println();
        
        cOut.println(reportControlBlocks);
        
        for (String gcb : gseVariableNames)
        	cOut.println("extern GSEControlBlock " + gcb + ";");

        cOut.println(gseControlBlocks);
        
        for (String sgcb : sgcbVariableNames)
        	cOut.println("extern SettingGroupControlBlock " + sgcb + ";");
        
        cOut.println(settingGroupControlBlocks);

        String firstLogicalDeviceName = logicalDevices.get(0).getInst();
        cOut.println("\nIedModel " + modelPrefix + " = {");
        cOut.println("    \"" + ied.getName() + "\",");
        cOut.println("    &" + modelPrefix + "_" + firstLogicalDeviceName + ",");

        if (dataSetNames.size() > 0)
            cOut.println("    &" + dataSetNames.get(0) + ",");
        else
            cOut.println("    NULL,");

        if (rcbVariableNames.size() > 0)
            cOut.println("    &" + rcbVariableNames.get(0) + ",");
        else
            cOut.println("    NULL,");

        if (gseVariableNames.size() > 0)
            cOut.println("    &" + gseVariableNames.get(0) + ",");
        else
            cOut.println("    NULL,");
        
        if (sgcbVariableNames.size() > 0)
        	cOut.println("    &" + sgcbVariableNames.get(0) + ",");
        else
            cOut.println("    NULL,");

        cOut.println("    initializeValues\n};");
    }

    private void createGooseVariableList(List<LogicalDevice> logicalDevices) {
		for (LogicalDevice ld : logicalDevices) {
			List<LogicalNode> lnodes = ld.getLogicalNodes();
			
			String ldName = ld.getInst();
			
			for (LogicalNode ln : lnodes) {
				List<GSEControl> goCBs = ln.getGSEControlBlocks();
				
				int gseCount = 0;
				
				for (GSEControl gse : goCBs) {
					String gcbVariableName = modelPrefix + "_" + ldName + "_" + ln.getName() + "_gse" + gseCount;					
					gseVariableNames.add(gcbVariableName);
					gseCount++;
				}
			}
		}
	}

	private void createReportVariableList(List<LogicalDevice> logicalDevices) {

		for (LogicalDevice ld : logicalDevices) {
			List<LogicalNode> lnodes = ld.getLogicalNodes();
			
			String ldName = ld.getInst();
			
			for (LogicalNode ln : lnodes) {
				List<ReportControlBlock> rcbs = ln.getReportControlBlocks();
				
				int rcbCount = 0;
				
				for (ReportControlBlock rcb : rcbs) {
					
					int maxInstances = 1;
					
					if (rcb.getRptEna() != null)
						maxInstances = rcb.getRptEna().getMaxInstances();
								
					for (int i = 0; i < maxInstances; i++) {
						String rcbVariableName = modelPrefix + "_" + ldName + "_" + ln.getName() + "_report" + rcbCount;					
						rcbVariableNames.add(rcbVariableName);
						rcbCount++;
					}
					
				}
			}
		}
	}
	
	private void createSettingControlsVariableList(List<LogicalDevice> logicalDevices) {
		for (LogicalDevice ld : logicalDevices) {
			List<LogicalNode> lnodes = ld.getLogicalNodes();
			
			String ldName = ld.getInst();
			
			for (LogicalNode ln : lnodes) {
				List<SettingControl> sgcbs = ln.getSettingGroupControlBlocks();
				
				for (SettingControl sgcb : sgcbs) {
					
					String sgcbVariableName = modelPrefix + "_" + ldName + "_" + ln.getName() + "_sgcb";
					
					sgcbVariableNames.add(sgcbVariableName);
					
				}
			}
		}
	}

	private void printLogicalNodeDefinitions(String ldName, List<LogicalNode> logicalNodes) {
        for (int i = 0; i < logicalNodes.size(); i++) {
            LogicalNode logicalNode = logicalNodes.get(i);

            String lnName = ldName + "_" + logicalNode.getName();

            variablesList.add(lnName);

            cOut.println("LogicalNode " + lnName + " = {");
            cOut.println("    LogicalNodeModelType,");
            cOut.println("    \"" + logicalNode.getName() + "\",");
            cOut.println("    (ModelNode*) &" + ldName + ",");
            if (i < (logicalNodes.size() - 1))
                cOut.println("    (ModelNode*) &" + ldName + "_" + logicalNodes.get(i + 1).getName() + ",");
            else
                cOut.println("    NULL,");

            String firstChildName = lnName + "_" + logicalNode.getDataObjects().get(0).getName();

            cOut.println("    (ModelNode*) &" + firstChildName + ",");

            cOut.println("};\n");

            printDataObjectDefinitions(lnName, logicalNode.getDataObjects(), null);

            printReportControlBlocks(lnName, logicalNode);

            printGSEControlBlocks(ldName, lnName, logicalNode);
            
            printSettingControlBlock(lnName, logicalNode);
        }
    }

    private void printDataObjectDefinitions(String lnName, List<DataObject> dataObjects, String dataAttributeSibling) {
        for (int i = 0; i < dataObjects.size(); i++) {
            DataObject dataObject = dataObjects.get(i);

            String doName = lnName + "_" + dataObject.getName();

            variablesList.add(doName);

            cOut.println("DataObject " + doName + " = {");
            cOut.println("    DataObjectModelType,");
            cOut.println("    \"" + dataObject.getName() + "\",");
            cOut.println("    (ModelNode*) &" + lnName + ",");

            if (i < (dataObjects.size() - 1))
                cOut.println("    (ModelNode*) &" + lnName + "_" + dataObjects.get(i + 1).getName() + ",");
            else if (dataAttributeSibling != null)
                cOut.println("    (ModelNode*) &" + dataAttributeSibling + ",");
            else
                cOut.println("    NULL,");

            String firstSubDataObjectName = null;
            String firstDataAttributeName = null;

            if ((dataObject.getSubDataObjects() != null) && (dataObject.getSubDataObjects().size() > 0))
                firstSubDataObjectName = doName + "_" + dataObject.getSubDataObjects().get(0).getName();

            if ((dataObject.getDataAttributes() != null) && (dataObject.getDataAttributes().size() > 0))
                firstDataAttributeName = doName + "_" + dataObject.getDataAttributes().get(0).getName();

            if (firstSubDataObjectName != null) {
                cOut.println("    (ModelNode*) &" + firstSubDataObjectName + ",");
            } else if (firstDataAttributeName != null) {
                cOut.println("    (ModelNode*) &" + firstDataAttributeName + ",");
            } else {
                cOut.println("    NULL,");
            }

            cOut.println("    " + dataObject.getCount());

            cOut.println("};\n");

            if (dataObject.getSubDataObjects() != null)
                printDataObjectDefinitions(doName, dataObject.getSubDataObjects(), firstDataAttributeName);

            if (dataObject.getDataAttributes() != null)
                printDataAttributeDefinitions(doName, dataObject.getDataAttributes());

        }
    }

    private void printDataAttributeDefinitions(String doName, List<DataAttribute> dataAttributes) {
        for (int i = 0; i < dataAttributes.size(); i++) {
            DataAttribute dataAttribute = dataAttributes.get(i);

            String daName = doName + "_" + dataAttribute.getName();
            
            if (dataAttribute.getFc() == FunctionalConstraint.SE) {
            	
            	if (daName.startsWith(modelPrefix + "_SE_") == false)
            		daName = daName.substring(0, 9) + "SE_" + daName.substring(9);

            }

            variablesList.add(daName);

            cOut.println("DataAttribute " + daName + " = {");
            cOut.println("    DataAttributeModelType,");
            cOut.println("    \"" + dataAttribute.getName() + "\",");
            cOut.println("    (ModelNode*) &" + doName + ",");
            if (i < (dataAttributes.size() - 1)) {
            	DataAttribute sibling = dataAttributes.get(i + 1);
            	
            	String siblingDoName = doName;
            	
            	if (sibling.getFc() == FunctionalConstraint.SE) {
            		if (siblingDoName.startsWith(modelPrefix + "_SE_") == false)
            			siblingDoName = siblingDoName.substring(0, 9) + "SE_" + siblingDoName.substring(9);
            	}
            	
                cOut.println("    (ModelNode*) &" + siblingDoName + "_" + dataAttributes.get(i + 1).getName() + ",");
            }
            else
                cOut.println("    NULL,");
            if ((dataAttribute.getSubDataAttributes() != null) && (dataAttribute.getSubDataAttributes().size() > 0))
                cOut.println("    (ModelNode*) &" + daName + "_" + dataAttribute.getSubDataAttributes().get(0).getName() + ",");
            else
                cOut.println("    NULL,");

            cOut.println("    " + dataAttribute.getCount() + ",");
            cOut.println("    IEC61850_FC_" + dataAttribute.getFc().toString() + ",");
            cOut.println("    IEC61850_" + dataAttribute.getType() + ",");

            /* print trigger options */
            cOut.print("    0");

            TriggerOptions trgOps = dataAttribute.getTriggerOptions();

            if (trgOps.isDchg())
                cOut.print(" + TRG_OPT_DATA_CHANGED");

            if (trgOps.isDupd())
                cOut.print(" + TRG_OPT_DATA_UPDATE");

            if (trgOps.isQchg())
                cOut.print(" + TRG_OPT_QUALITY_CHANGED");

            cOut.println(",");

            cOut.println("    NULL,");
            
            Long sAddr = null;
            
            try {
                if (dataAttribute.getShortAddress() != null)
                    sAddr = new Long(dataAttribute.getShortAddress());
            } catch (NumberFormatException e) {
                System.out.println("WARNING: short address \"" + dataAttribute.getShortAddress() + "\" is not valid for libIEC61850!\n");
            }
            
            if (sAddr != null)
                cOut.print("    " + sAddr.longValue());
            else
                cOut.print("    0");
            
            cOut.println("};\n");

            if (dataAttribute.getSubDataAttributes() != null)
          
            	
            printDataAttributeDefinitions(daName, dataAttribute.getSubDataAttributes());
            
            DataModelValue value = dataAttribute.getValue();
                     
            /* if no value is given use default value for type if present */
            if (value == null) { 
         	   value = dataAttribute.getDefinition().getValue();
         	   
         	   if (value != null)
 	        	   if (value.getValue() == null)
 	        		   value.updateEnumOrdValue(ied.getTypeDeclarations());        	   
            }
            
            if (value != null) {
                printValue(daName, dataAttribute, value);
            }
            
        }

    }

    private void printValue(String daName, DataAttribute dataAttribute, DataModelValue value) {

        StringBuffer buffer = this.initializerBuffer;

        buffer.append("\n");
		if( initializeOnce )
		{
			buffer.append("if (!");
			buffer.append(daName);
			buffer.append(".mmsValue)\n");
		}
        buffer.append(daName);
        buffer.append(".mmsValue = ");

        switch (dataAttribute.getType()) {
        case ENUMERATED:
        case INT8:
        case INT16:
        case INT32:
        case INT64:
            buffer.append("MmsValue_newIntegerFromInt32(" + value.getIntValue() + ");");
            break;
        case INT8U:
        case INT16U:
        case INT24U:
        case INT32U:
            buffer.append("MmsValue_newUnsignedFromUint32(" + value.getLongValue() + ");");
            break;
        case BOOLEAN:
            buffer.append("MmsValue_newBoolean(" + value.getValue() + ");");
            break;
        case UNICODE_STRING_255:
            buffer.append("MmsValue_newMmsString(\"" + value.getValue() + "\");");
            break;
        case VISIBLE_STRING_32:
        case VISIBLE_STRING_64:
        case VISIBLE_STRING_129:
        case VISIBLE_STRING_255:
        case VISIBLE_STRING_65:
            buffer.append("MmsValue_newVisibleString(\"" + value.getValue() + "\");");
            break;
        case FLOAT32:
            buffer.append("MmsValue_newFloat(" + value.getValue() + ");");
            break;
        case FLOAT64:
            buffer.append("MmsValue_newDouble(" + value.getValue() + ");");
            break;
        default:
            System.out.println("Unknown default value for " + daName + " type: " + dataAttribute.getType());
            buffer.append("NULL;");
            break;
        }

        buffer.append("\n");

    }

    private void printForwardDeclarations(Server server) {

        cOut.println("static void initializeValues();");
        hOut.println("extern IedModel " + modelPrefix + ";");

        for (LogicalDevice logicalDevice : server.getLogicalDevices()) {
            String ldName = modelPrefix + "_" + logicalDevice.getInst();

            hOut.println("extern LogicalDevice " + ldName + ";");

            for (LogicalNode logicalNode : logicalDevice.getLogicalNodes()) {
                String lnName = ldName + "_" + logicalNode.getName();

                hOut.println("extern LogicalNode   " + lnName + ";");

                printDataObjectForwardDeclarations(lnName, logicalNode.getDataObjects());
            }

        }
    }

    private void printDataObjectForwardDeclarations(String prefix, List<DataObject> dataObjects) {
        for (DataObject dataObject : dataObjects) {
            String doName = prefix + "_" + dataObject.getName();

            hOut.println("extern DataObject    " + doName + ";");

            if (dataObject.getSubDataObjects() != null) {
                printDataObjectForwardDeclarations(doName, dataObject.getSubDataObjects());
            }

            printDataAttributeForwardDeclarations(doName, dataObject.getDataAttributes());
        }
    }

    private void printDataAttributeForwardDeclarations(String doName, List<DataAttribute> dataAttributes) {
        for (DataAttribute dataAttribute : dataAttributes) {
            String daName = doName + "_" + dataAttribute.getName();
            
            if (dataAttribute.getFc() == FunctionalConstraint.SE) {
      	
            	if (daName.startsWith(modelPrefix + "_SE_") == false)
            		daName = daName.substring(0, 9) + "SE_" + daName.substring(9);
            }
            	
            hOut.println("extern DataAttribute " + daName + ";");

            if (dataAttribute.getSubDataAttributes() != null)
                printDataAttributeForwardDeclarations(daName, dataAttribute.getSubDataAttributes());
        }
    }

    private void printCFileHeader(String filename) {

		String include = outputFileName + ".h\"";
		if( include.lastIndexOf( '/' ) >= 0 ) {
			include = include.substring( include.lastIndexOf( '/' ) + 1 );
		}

        cOut.println("/*");
        cOut.println(" * " + outputFileName + ".c");
        cOut.println(" *");
        cOut.println(" * automatically generated from " + filename);
        cOut.println(" */");
        cOut.println("#include \"" + include);
        cOut.println();
    }

    private void printHeaderFileHeader(String filename) {
        hOut.println("/*");
        hOut.println(" * " + outputFileName + ".h");
        hOut.println(" *");
        hOut.println(" * automatically generated from " + filename);
        hOut.println(" */\n");
        hOut.println("#ifndef " + hDefineName);
        hOut.println("#define " + hDefineName + "\n");
        hOut.println("#include <stdlib.h>");
        hOut.println("#include \"iec61850_model.h\"");
        hOut.println();
    }

    private void printHeaderFileFooter() {
        hOut.println();
        hOut.println("#endif /* " + hDefineName + " */\n");
    }

    private void printGSEControlBlocks(String ldName, String lnPrefix, LogicalNode logicalNode) {
        List<GSEControl> gseControlBlocks = logicalNode.getGSEControlBlocks();

        String[] ldNameComponents = ldName.split("_");

        String logicalDeviceName = ldNameComponents[1];

        int gseControlNumber = 0;

        for (GSEControl gseControlBlock : gseControlBlocks) {

            GSEAddress gseAddress = connectedAP.lookupGSEAddress(logicalDeviceName, gseControlBlock.getName());

            String gseString = "";

            String phyComAddrName = "";

            if (gseAddress != null) {
                phyComAddrName = lnPrefix + "_gse" + gseControlNumber + "_address";

                gseString += "\nstatic PhyComAddress " + phyComAddrName + " = {\n";
                gseString += "  " + gseAddress.getVlanPriority() + ",\n";
                gseString += "  " + gseAddress.getVlanId() + ",\n";
                gseString += "  " + gseAddress.getAppId() + ",\n";
                gseString += "  {";

                for (int i = 0; i < 6; i++) {
                    gseString += "0x" + Integer.toHexString(gseAddress.getMacAddress()[i]);
                    if (i == 5)
                        gseString += "}\n";
                    else
                        gseString += ", ";
                }

                gseString += "};\n\n";
            }

            String gseVariableName = lnPrefix + "_gse" + gseControlNumber;

            gseString += "GSEControlBlock " + gseVariableName + " = {";
            gseString += "&" + lnPrefix + ", ";

            gseString += "\"" + gseControlBlock.getName() + "\", ";

            if (gseControlBlock.getAppID() == null)
                gseString += "NULL, ";
            else
                gseString += "\"" + gseControlBlock.getAppID() + "\", ";

            if (gseControlBlock.getDataSet() != null)
                gseString += "\"" + gseControlBlock.getDataSet() + "\", ";
            else
                gseString += "NULL, ";

            gseString += gseControlBlock.getConfRev() + ", ";

            if (gseControlBlock.isFixedOffs())
                gseString += "true, ";
            else
                gseString += "false, ";

            if (gseAddress != null)
                gseString += "&" + phyComAddrName + ", ";
            else
                gseString += "NULL, ";
            
            gseString += gseControlBlock.getMinTime() + ", ";
            gseString += gseControlBlock.getMaxTime() + ", ";

            currentGseVariableNumber++;
            
            if (currentGseVariableNumber < gseVariableNames.size())
                gseString += "&" + gseVariableNames.get(currentGseVariableNumber);
            else
                gseString += "NULL";

            gseString += "};\n";

            this.gseControlBlocks.append(gseString);

            gseControlNumber++;
        }
    }

    private void printReportControlBlocks(String lnPrefix, LogicalNode logicalNode) {
        List<ReportControlBlock> reportControlBlocks = logicalNode.getReportControlBlocks();

        int reportsCount = reportControlBlocks.size();

        int reportNumber = 0;

        for (ReportControlBlock rcb : reportControlBlocks) {

            if (rcb.isIndexed()) {

                int maxInstances = 1;

                if (rcb.getRptEna() != null)
                    maxInstances = rcb.getRptEna().getMaxInstances();

                for (int i = 0; i < maxInstances; i++) {
                    String index = String.format("%02d", (i + 1));

                    System.out.println("print report instance " + index);

                    printReportControlBlockInstance(lnPrefix, rcb, index, reportNumber, reportsCount);
                    reportNumber++;
                }
            } else {
                printReportControlBlockInstance(lnPrefix, rcb, "", reportNumber, reportsCount);
                reportNumber++;
            }
        }
    }
    
    private void printSettingControlBlock(String lnPrefix, LogicalNode logicalNode)
    {
    	List<SettingControl> settingControls = logicalNode.getSettingGroupControlBlocks();
    	
    	if (settingControls.size() > 0) {
    		System.out.println("print SGCB for " + lnPrefix);
    		
    		SettingControl sgcb = settingControls.get(0);
    		
    		String sgcbVariableName = lnPrefix + "_sgcb";
    		
    		String sgcbString = "\nSettingGroupControlBlock " + sgcbVariableName + " = {";
    		
    		sgcbString += "&" + lnPrefix + ", ";
    		
    		sgcbString += sgcb.getActSG() + ", " + sgcb.getNumOfSGs() + ", 0, false, 0, 0, ";
    		
    		if (currentSGCBVariableNumber < (sgcbVariableNames.size() - 1))
    			sgcbString += "&" + sgcbVariableNames.get(currentSGCBVariableNumber + 1);
    		else
    			sgcbString += "NULL";
    				
    		sgcbString += "};\n";
    		
    		this.settingGroupControlBlocks.append(sgcbString);
    		
    		currentSGCBVariableNumber++;
    	}
    }

    private void printReportControlBlockInstance(String lnPrefix, ReportControlBlock rcb, String index, int reportNumber, int reportsCount) {
        String rcbVariableName = lnPrefix + "_report" + reportNumber;

        String rcbString = "ReportControlBlock " + rcbVariableName + " = {";

        rcbString += "&" + lnPrefix + ", ";

        rcbString += "\"" + rcb.getName() + index + "\", ";

        if (rcb.getRptID() == null)
            rcbString += "NULL, ";
        else
            rcbString += "\"" + rcb.getRptID() + "\", ";

        if (rcb.isBuffered())
            rcbString += "true, ";
        else
            rcbString += "false, ";

        if (rcb.getDataSet() != null)
            rcbString += "\"" + rcb.getDataSet() + "\", ";
        else
            rcbString += "NULL, ";

        if (rcb.getConfRef() != null)
            rcbString += rcb.getConfRef().toString() + ", ";
        else
            rcbString += "0, ";

        int triggerOps = 16;

        if (rcb.getTriggerOptions() != null)
        	triggerOps = rcb.getTriggerOptions().getIntValue();

        rcbString += triggerOps + ", ";

        int options = 0;

        if (rcb.getOptionFields() != null) {
            if (rcb.getOptionFields().isSeqNum())
                options += 1;
            if (rcb.getOptionFields().isTimeStamp())
                options += 2;
            if (rcb.getOptionFields().isReasonCode())
                options += 4;
            if (rcb.getOptionFields().isDataSet())
                options += 8;
            if (rcb.getOptionFields().isDataRef())
                options += 16;
            if (rcb.getOptionFields().isBufOvfl())
                options += 32;
            if (rcb.getOptionFields().isEntryID())
                options += 64;
            if (rcb.getOptionFields().isConfigRef())
                options += 128;
        } else
            options = 32;

        rcbString += options + ", ";

        rcbString += rcb.getBufferTime() + ", ";

        if (rcb.getIntegrityPeriod() != null)
            rcbString += rcb.getIntegrityPeriod().toString() + ", ";
        else
            rcbString += "0, ";

        currentRcbVariableNumber++;
    
        if (currentRcbVariableNumber < rcbVariableNames.size())
        	rcbString += "&" + rcbVariableNames.get(currentRcbVariableNumber);
        else
            rcbString += "NULL";

        rcbString += "};\n";

        this.reportControlBlocks.append(rcbString);
    }

    private static String toMmsString(String iecString) {
        return iecString.replace('.', '$');
    }

    private void printDataSets() {

        List<LogicalDevice> logicalDevices = accessPoint.getServer().getLogicalDevices();

        /* create list of data set names */
        dataSetNames = new LinkedList<String>();

        for (LogicalDevice logicalDevice : logicalDevices) {

            for (LogicalNode logicalNode : logicalDevice.getLogicalNodes()) {

                List<DataSet> dataSets = logicalNode.getDataSets();

                for (DataSet dataSet : dataSets) {

                    String dataSetVariableName = modelPrefix + "ds_" + logicalDevice.getInst() + "_" + logicalNode.getName() + "_" + dataSet.getName();

                    dataSetNames.add(dataSetVariableName);
                }
            }
        }
        
        
        /* print data set forward declarations */
        cOut.println();
        for (String dataSetName : dataSetNames) {
        	cOut.println("extern DataSet " + dataSetName + ";");
        }
        cOut.println();

        /* print data sets */
        int dataSetNameListIndex = 0;
        
        for (LogicalDevice logicalDevice : logicalDevices) {

            for (LogicalNode logicalNode : logicalDevice.getLogicalNodes()) {

                List<DataSet> dataSets = logicalNode.getDataSets();

                for (DataSet dataSet : dataSets) {

                    String dataSetVariableName = dataSetNames.get(dataSetNameListIndex++);

                    int fcdaCount = 0;

                    int numberOfFcdas = dataSet.getFcda().size();

                    String dataSetElements = "";

                    cOut.println();
                    for (FunctionalConstraintData fcda : dataSet.getFcda()) {
                        String dataSetEntryName = dataSetVariableName + "_fcda" + fcdaCount;

                        cOut.println("extern DataSetEntry " + dataSetEntryName + ";");

                        fcdaCount++;
                    }
                    cOut.println();

                    fcdaCount = 0;

                    for (FunctionalConstraintData fcda : dataSet.getFcda()) {
                        String dataSetEntryName = dataSetVariableName + "_fcda" + fcdaCount;

                        cOut.println("DataSetEntry " + dataSetEntryName + " = {");
                        cOut.println("  \"" + fcda.getLdInstance() + "\",");

                        String mmsVariableName = "";

                        if (fcda.getPrefix() != null)
                            mmsVariableName += fcda.getPrefix();

                        mmsVariableName += fcda.getLnClass();

                        if (fcda.getLnInstance() != null)
                            mmsVariableName += fcda.getLnInstance();

                        mmsVariableName += "$" + fcda.getFc().toString();

                        mmsVariableName += "$" + toMmsString(fcda.getDoName());

                        if (fcda.getDaName() != null)
                            mmsVariableName += "$" + toMmsString(fcda.getDaName());

                        // TODO implement index processing!
                        cOut.println("  false,");
                        cOut.println("  \"" + mmsVariableName + "\",");
                        cOut.println("  -1,");
                        cOut.println("  NULL,");
                        cOut.println("  NULL,");

                        if (fcdaCount + 1 < numberOfFcdas)
                            cOut.println("  &" + dataSetVariableName + "_fcda" + (fcdaCount + 1));
                        else
                            cOut.println("  NULL");

                        cOut.println("};\n");

                        fcdaCount++;

                        if (fcdaCount != dataSet.getFcda().size())
                            dataSetElements += "  &" + dataSetEntryName + ",\n";
                        else
                            dataSetElements += "  &" + dataSetEntryName;
                    }

                    cOut.println("DataSet " + dataSetVariableName + " = {");

                    String lnVariableName = modelPrefix + "_" + logicalDevice.getInst() + "_" + logicalNode.getName();

                    cOut.println("  \"" + logicalDevice.getInst() + "\",");
                    cOut.println("  \"" + logicalNode.getName() + "$" + dataSet.getName() + "\",");
                    cOut.println("  " + dataSet.getFcda().size() + ",");
                    cOut.println("  &" + dataSetVariableName + "_fcda0,");
                                        
                    if (dataSetNameListIndex < dataSetNames.size()) {
                    	 String nextDataSetVariableName = dataSetNames.get(dataSetNameListIndex);
                    	cOut.println("  &" + nextDataSetVariableName);
                    }
                    else
                    	cOut.println("  NULL");
                    
                    cOut.println("};");

                }
            }
        }

    }

}
