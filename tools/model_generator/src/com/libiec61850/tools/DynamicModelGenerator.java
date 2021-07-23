package com.libiec61850.tools;

/*
 *  DynamicModelGenerator.java
 *
 *  Copyright 2014-2020 Michael Zillgith
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
import java.util.List;

import com.libiec61850.scl.DataAttributeDefinition;
import com.libiec61850.scl.SclParser;
import com.libiec61850.scl.SclParserException;
import com.libiec61850.scl.communication.ConnectedAP;
import com.libiec61850.scl.communication.GSE;
import com.libiec61850.scl.communication.PhyComAddress;
import com.libiec61850.scl.model.AccessPoint;
import com.libiec61850.scl.model.DataAttribute;
import com.libiec61850.scl.model.DataModelValue;
import com.libiec61850.scl.model.DataObject;
import com.libiec61850.scl.model.DataSet;
import com.libiec61850.scl.model.FunctionalConstraintData;
import com.libiec61850.scl.model.GSEControl;
import com.libiec61850.scl.model.IED;
import com.libiec61850.scl.model.Log;
import com.libiec61850.scl.model.LogControl;
import com.libiec61850.scl.model.LogicalDevice;
import com.libiec61850.scl.model.LogicalNode;
import com.libiec61850.scl.model.ReportControlBlock;
import com.libiec61850.scl.model.ReportSettings;
import com.libiec61850.scl.model.Services;
import com.libiec61850.scl.model.SettingControl;

public class DynamicModelGenerator {

    private ConnectedAP connectedAP;
    private IED ied = null;
    private boolean hasOwner = false;
    
    public DynamicModelGenerator(InputStream stream, String icdFile, PrintStream output, String iedName, String accessPointName) 
    		throws SclParserException {

        SclParser sclParser = new SclParser(stream);
        
        if (iedName == null)
        	ied = sclParser.getFirstIed();
        else
        	ied = sclParser.getIedByName(iedName);

        if (ied == null)
            throw new SclParserException("No data model present in SCL file! Exit.");
        
        Services services = ied.getServices();
        
        if (services != null) {
            ReportSettings rptSettings = services.getReportSettings();
            
            if (rptSettings != null) {
                hasOwner = rptSettings.hasOwner();
            }
        }
       
        AccessPoint accessPoint = null;
        
        if (accessPointName != null)
        	accessPoint = ied.getAccessPointByName(accessPointName);
        else
        	accessPoint = ied.getFirstAccessPoint();

        if (accessPoint == null)
        	throw new SclParserException("No valid access point found!");
        
        this.connectedAP = sclParser.getConnectedAP(ied, accessPoint.getName());
        
        List<LogicalDevice> logicalDevices = accessPoint.getServer().getLogicalDevices();

        output.println("MODEL(" + ied.getName() + "){");
        for (LogicalDevice logicalDevice : logicalDevices) {
            output.print("LD(");
            output.print(logicalDevice.getInst() + "){\n");

            exportLogicalNodes(output, logicalDevice);

            output.println("}");
        }
        output.println("}");
    }

    private void exportLogicalNodes(PrintStream output, LogicalDevice logicalDevice) throws SclParserException {
        for (LogicalNode logicalNode : logicalDevice.getLogicalNodes()) {
            output.print("LN(" + logicalNode.getName() + "){\n");

            exportLogicalNode(output, logicalNode, logicalDevice);

            output.println("}");
        }
    }
    
    private static String toMmsString(String iecString) {
        return iecString.replace('.', '$');
    }

    private void exportLogicalNode(PrintStream output, LogicalNode logicalNode, LogicalDevice logicalDevice)  throws SclParserException {
    	
    	for (SettingControl sgcb : logicalNode.getSettingGroupControlBlocks()) {
    		output.print("SG(" + sgcb.getActSG() + " " + sgcb.getNumOfSGs() + ")\n");
    	}
    	
        for (DataObject dataObject : logicalNode.getDataObjects()) {
            output.print("DO(" + dataObject.getName() + " " + dataObject.getCount() + "){\n");

            exportDataObject(output, dataObject, false);

            output.println("}");
        }
        
        for (DataSet dataSet : logicalNode.getDataSets())
            exportDataSet(output, dataSet, logicalNode);
        
        for (ReportControlBlock rcb : logicalNode.getReportControlBlocks()) {
             
            if (rcb.isIndexed()) {
                
                int maxInstances = 1;
                
                if (rcb.getRptEna() != null)
                    maxInstances = rcb.getRptEna().getMaxInstances();

                for (int i = 0; i < maxInstances; i++) {
                    String index = String.format("%02d", (i + 1));                
                    printRCBInstance(output, rcb, index); 
                }
            }
            else 
                printRCBInstance(output, rcb, "");  
        }
        
        for (LogControl lcb : logicalNode.getLogControlBlocks())
            printLCB(output, lcb, logicalNode, logicalDevice);
        
        for (Log log : logicalNode.getLogs())
            output.println("LOG(" + log.getName() + ");");
        
        for (GSEControl gcb : logicalNode.getGSEControlBlocks()) {
            LogicalDevice ld = logicalNode.getParentLogicalDevice();
            
            GSE gse = null;
            PhyComAddress gseAddress = null;
            
            if (connectedAP != null) {
                gse = connectedAP.lookupGSE(ld.getInst(), gcb.getName());
            	
                if (gse != null)
                	gseAddress = gse.getAddress();
            }
            else
            	System.out.println("WARNING: IED \"" + ied.getName() + "\" has no connected access point!");
            
            output.print("GC(");
            output.print(gcb.getName() + " ");
            output.print(gcb.getAppID() + " ");
            output.print(gcb.getDataSet() + " ");
            output.print(gcb.getConfRev() + " ");
            if (gcb.isFixedOffs())
                output.print('1');
            else
                output.print('0');
            output.print(' ');
            if (gse != null) {
	            output.print(gse.getMinTime());
	            output.print(' ');
	            output.print(gse.getMaxTime());
            }
            else {
            	output.print("-1 -1");
            }
            output.print(' ');
                       
            if (gseAddress == null) {
                output.println(");");
            }
            else {
                output.println("){");
                
                output.print("PA(");
                output.print(gseAddress.getVlanPriority() + " ");
                output.print(gseAddress.getVlanId() + " ");
                output.print(gseAddress.getAppId() + " ");
             
                for (int i = 0; i < 6; i++)
                    output.printf("%02x", gseAddress.getMacAddress()[i]);
                
                output.println(");");
                
                output.println("}");
            }
        }
    }
    
    private void printLCB(PrintStream output, LogControl lcb, LogicalNode ln, LogicalDevice logicalDevice) {
        output.print("LC(");
        output.print(lcb.getName() + " ");
        
        if (lcb.getDataSet() != null)
            output.print(lcb.getDataSet() + " ");
        else
            output.print("- ");
        
        if (lcb.getLogName() != null) {
            String logRef = logicalDevice.getInst() + "/" + ln.getName() + "$" + lcb.getLogName();
            output.print(logRef + " ");
        }
        else
            output.print("- ");
        
        output.print(lcb.getTriggerOptions().getIntValue() + " ");
        output.print(lcb.getIntgPd() + " ");
        
        if (lcb.isLogEna())
            output.print("1 ");
        else
            output.print("0 ");
        
        if (lcb.isReasonCode())
            output.println("1);");
        else
            output.println("0);");
    }

    private void printRCBInstance(PrintStream output, ReportControlBlock rcb, String index) {
        output.print("RC(");
        output.print(rcb.getName() +  index + " ");
        
        if (rcb.getRptID() != null)
            output.print(rcb.getRptID() + " ");
        else
            output.print("- ");
        
        if (rcb.isBuffered())
            output.print("1 ");
        else
            output.print("0 ");
        
        if (rcb.getDataSet() != null)
            output.print(rcb.getDataSet() + " ");
        else
            output.print("- ");
        
        output.print(rcb.getConfRef() + " ");
        
        int triggerOptions = rcb.getTriggerOptions().getIntValue();
        
        if (hasOwner)
            triggerOptions += 64;
        
        output.print(triggerOptions + " ");
        
        output.print(rcb.getOptionFields().getIntValue() + " ");

        output.print(rcb.getBufferTime() + " ");
        
        if (rcb.getIntegrityPeriod() != null)
        	output.print(rcb.getIntegrityPeriod());
        else
        	output.print("0");
        
        output.println(");");
    }

    private void exportDataSet(PrintStream output, DataSet dataSet, LogicalNode logicalNode) throws SclParserException {
        output.print("DS(" + dataSet.getName() + "){\n");
        for (FunctionalConstraintData fcda : dataSet.getFcda()) {
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
            
            /* check for array index and component */
            
            int arrayStart = mmsVariableName.indexOf('(');
            
            String variableName = mmsVariableName;
            int arrayIndex = -1;
            String componentName = null;
            
            if (arrayStart != -1) {
                variableName = mmsVariableName.substring(0, arrayStart);
                
                int arrayEnd = mmsVariableName.indexOf(')');
                
                String arrayIndexStr = mmsVariableName.substring(arrayStart + 1, arrayEnd);
                arrayIndex = Integer.parseInt(arrayIndexStr);
                
                if (arrayIndex < 0) {
                    throw new SclParserException("Array index out of range in data set entry definition");
                }
                
                String componentNamePart = mmsVariableName.substring(arrayEnd + 1);
                
                if ((componentNamePart != null) && (componentNamePart.length() > 0)) {
                    if (componentNamePart.charAt(0) == '$') {
                        componentNamePart = componentNamePart.substring(1);
                    }
                    
                    if ((componentNamePart != null) && (componentNamePart.length() > 0))
                        componentName = componentNamePart;
                }
            }
            
            /* check for LD name */
            
            String logicalDeviceName = null;
            
            if (fcda.getLdInstance() != null) {
            	
            	if (!fcda.getLdInstance().equals(logicalNode.getParentLogicalDevice().getInst())) {
            		logicalDeviceName = fcda.getLdInstance();
            	}
            }
            
            if (logicalDeviceName != null)
                variableName = logicalDeviceName + "/" + variableName;
            
            if (variableName != null && arrayIndex != -1 && componentName != null) {
                output.print("DE(" + variableName + " " + arrayIndex + " " + componentName + ");\n");
            }
            else if (variableName != null && arrayIndex != -1) {
                output.print("DE(" + variableName + " " + arrayIndex + ");\n");
            }
            else if (variableName != null) {
                output.print("DE(" + variableName + ");\n");
            }            
        }
        output.println("}");
    }

    private void exportDataObject(PrintStream output, DataObject dataObject, boolean isTransient) {
        
        if (dataObject.isTransient())
            isTransient = true;
        
        for (DataObject subDataObject : dataObject.getSubDataObjects()) {
            output.print("DO(" + subDataObject.getName() + " " + subDataObject.getCount() + "){\n");

            exportDataObject(output, subDataObject, isTransient);

            output.println("}");
        }

        for (DataAttribute dataAttribute : dataObject.getDataAttributes()) {
            exportDataAttribute(output, dataAttribute, isTransient);
        }
    }

    private void exportDataAttribute(PrintStream output, DataAttribute dataAttribute, boolean isTransient) {

        output.print("DA(" + dataAttribute.getName() + " ");
        output.print(dataAttribute.getCount() + " ");
        output.print(dataAttribute.getType().getIntValue() + " ");
        output.print(dataAttribute.getFc().getIntValue() + " ");
        
        int trgOpsVal = dataAttribute.getTriggerOptions().getIntValue();
        
        if (isTransient)
            trgOpsVal += 128;
        
        output.print(trgOpsVal + " ");
        
        Long sAddr = null;
        
        try {
            if (dataAttribute.getShortAddress() != null)
                sAddr = new Long(dataAttribute.getShortAddress());
        } catch (NumberFormatException e) {
            System.out.println("WARNING: short address \"" + dataAttribute.getShortAddress() + "\" is not valid for libIEC61850!\n");
        }
        
        if (sAddr != null)
            output.print(sAddr.longValue());
        else
            output.print("0");
        
        output.print(")"); 
                
        if (dataAttribute.isBasicAttribute()) {
           DataModelValue value = dataAttribute.getValue();
           
           /* if no value is given use default value for type if present */
           if (value == null) { 
        	   value = dataAttribute.getDefinition().getValue();
        	   
        	   if (value != null)
	        	   if (value.getValue() == null)
	        		   value.updateEnumOrdValue(ied.getTypeDeclarations());        	   
           }
           
           if (value != null) {
               
               switch (dataAttribute.getType()) {
               case ENUMERATED:
               case INT8:
               case INT16:
               case INT32:
               case INT64:
                   output.print("=" + value.getIntValue());
                   break;
               case INT8U:
               case INT16U:
               case INT24U:
               case INT32U:
                   output.print("=" + value.getLongValue());
                   break;
               case BOOLEAN:
                   {
                       Boolean boolVal = (Boolean) value.getValue();
                       
                       if (boolVal.booleanValue())
                           output.print("=1");
                   }
                   break;
               case UNICODE_STRING_255:
                   output.print("=\"" + value.getValue()+ "\"");
                   break;
               case CURRENCY:
               case VISIBLE_STRING_32:
               case VISIBLE_STRING_64:
               case VISIBLE_STRING_129:
               case VISIBLE_STRING_255:
               case VISIBLE_STRING_65:
                   output.print("=\"" + value.getValue()+ "\"");
                   break;
               case FLOAT32:
               case FLOAT64:
                   output.print("=" + value.getValue());
                   break;
               default:
                   System.out.println("Unknown default value for " + dataAttribute.getName() + " type: " + dataAttribute.getType());
                   break;
               }
               
           }
            
            output.println(";");
        } 
        else {
            output.println("{");

            for (DataAttribute subDataAttribute : dataAttribute.getSubDataAttributes()) {
                exportDataAttribute(output, subDataAttribute, isTransient);
            }

            output.println("}");
        }

    }

    public static void main(String[] args) throws FileNotFoundException {
        System.out.println("Dynamic model generator");

        if (args.length < 1) {
            System.out.println("Usage: genconfig <ICD file> [-ied  <ied-name>] [-ap <access-point-name>] [<output filename>]");
            System.exit(1);
        }

        String icdFile = args[0];

        PrintStream outputStream = System.out;
        
        String accessPointName = null;
        String iedName = null;
        
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
        		else {
        			 outputStream = new PrintStream(new FileOutputStream(new File(args[i])));
        		}
        	}
        	      
        }

        InputStream stream = new FileInputStream(icdFile);

        try {
			new DynamicModelGenerator(stream, icdFile, outputStream, iedName, accessPointName);
		} catch (SclParserException e) {
			System.err.println("ERROR: " + e.getMessage());
		}
    }

}
