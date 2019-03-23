package com.libiec61850.tools;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.PrintStream;
import java.util.List;

import com.libiec61850.scl.SclParser;
import com.libiec61850.scl.SclParserException;
import com.libiec61850.scl.model.AccessPoint;
import com.libiec61850.scl.model.DataAttribute;
import com.libiec61850.scl.model.DataObject;
import com.libiec61850.scl.model.IED;
import com.libiec61850.scl.model.LogicalDevice;
import com.libiec61850.scl.model.LogicalNode;
import com.libiec61850.scl.model.Server;
import com.libiec61850.scl.types.DataAttributeType;
import com.libiec61850.scl.types.DataObjectType;
import com.libiec61850.scl.types.EnumerationType;
import com.libiec61850.scl.types.LogicalNodeType;
import com.libiec61850.scl.types.SclType;
import com.libiec61850.scl.types.TypeDeclarations;

public class ModelViewer {
    
    private static void showTypes(InputStream stream, String icdFile, PrintStream output, String iedName, String accessPointName, boolean unusedOnly) 
            throws SclParserException {

        SclParser sclParser = new SclParser(stream, false);
        
        TypeDeclarations typeDecl = sclParser.getTypeDeclarations();

        for (SclType type : typeDecl.getTypeDeclarations()) {
            
            if (unusedOnly) {
                if (type.isUsed() == true)
                    continue;
            }
            
            output.print(type.getId());
            
            if (type.getClass() == LogicalNodeType.class)
                output.print(" : LogicalNode");
            else if (type.getClass() == DataObjectType.class)
                output.print(" : DataObject");
            else if (type.getClass() ==  DataAttributeType.class)
                output.print(" : DataAttribute");
            else if (type.getClass() == EnumerationType.class)
                output.print(" : Enumeration");
            
            if (!unusedOnly)
                if (type.isUsed() == false)
                    System.out.print("  UNUSED TYPE!");
            
            output.println();
        }
    }
    
    private static void printSubAttributes(DataAttribute attribute, PrintStream output, String indent, String add) {
        
        if (attribute.getSubDataAttributes() != null) {
        
            for (DataAttribute attr : attribute.getSubDataAttributes()) {
                output.println(indent + add + attr.getName());
                
                printSubAttributes(attr, output, indent + add, add);
            }
        }
    }
    
    private static void printDataObjects(List<DataObject> dataObjects, PrintStream output, String indent, String add)
    {
        for (DataObject dObject : dataObjects) {
            output.println(indent + dObject.getName());
            
            printDataObjects(dObject.getSubDataObjects(), output, indent + add, add);
            
            for (DataAttribute dAttribute : dObject.getDataAttributes()) {
                output.println(indent + add + dAttribute.getName() + " [" + dAttribute.getFc().toString() + "]");
                
                printSubAttributes(dAttribute, output, indent + add,  add);
            }
            
        }
    }
    
    private static void printModelStructure(InputStream stream, String icdFile, PrintStream output, String iedName, String accessPointName) 
            throws SclParserException {

        SclParser sclParser = new SclParser(stream, false);
        
        IED ied = null;
        
        if (iedName == null)
            ied = sclParser.getFirstIed();
        else
            ied = sclParser.getIedByName(iedName);
        
        AccessPoint ap = ied.getFirstAccessPoint();
        
        Server server = ap.getServer();
        
        List<LogicalDevice> devices = server.getLogicalDevices();
        
        for (LogicalDevice device : devices) {
            output.println(device.getInst());
            
            List<LogicalNode> lNodes = device.getLogicalNodes();
            
            for (LogicalNode lNode : lNodes) {
                output.println("  " + lNode.getName());
                
                printDataObjects(lNode.getDataObjects(), output, "    ", "  ");
            }
        }
        
    }
    
    
    private static void printSubAttributeList(DataAttribute attribute, PrintStream output, String prefix) {
        
        if (attribute.getSubDataAttributes() != null) {
        
            for (DataAttribute attr : attribute.getSubDataAttributes()) {
                
                String nextPrefix = prefix + "." + attr.getName();
                
                output.println(nextPrefix + " [" + attr.getFc() + "]");
                
                printSubAttributeList(attr, output, nextPrefix);
            }
        }
    }
    
    private static void printObjectList(List<DataObject> dataObjects, PrintStream output, String prefix)
    {
        for (DataObject dataObject : dataObjects) {
            String dOPrefix = prefix + "." + dataObject.getName();
            
            printObjectList(dataObject.getSubDataObjects(), output, dOPrefix);
            
            for (DataAttribute dAttribute : dataObject.getDataAttributes()) {
                
                String daPrefix = dOPrefix + "." + dAttribute.getName();
                
                output.println(daPrefix  + " [" + dAttribute.getFc().toString() + "]");
                
                printSubAttributeList(dAttribute, output, daPrefix);
            }
        }
    }
    
    
    private static void printAttributeList(InputStream stream, String icdFile, PrintStream output, String iedName, String accessPointName) 
            throws SclParserException {

        SclParser sclParser = new SclParser(stream, false);
        
        IED ied = null;
        
        if (iedName == null)
            ied = sclParser.getFirstIed();
        else
            ied = sclParser.getIedByName(iedName);
        
        AccessPoint ap = ied.getFirstAccessPoint();
        
        Server server = ap.getServer();
        
        List<LogicalDevice> devices = server.getLogicalDevices();
        
        
        
        for (LogicalDevice device : devices) {
            
            String devPrefix = ied.getName() + device.getInst() + "/";
            
            List<LogicalNode> lNodes = device.getLogicalNodes();
            
            for (LogicalNode lNode : lNodes) {
                String lNodePrefix = devPrefix + lNode.getName();
                
                printObjectList(lNode.getDataObjects(), output, lNodePrefix);
                
//                for (DataObject dObject : lNode.getDataObjects()) {
//                    
//                    String dOPrefix = lNodePrefix + "." + dObject.getName();
//                    
//                    for (DataAttribute dAttribute : dObject.getDataAttributes()) {
//                        
//                        String daPrefix = dOPrefix + "." + dAttribute.getName();
//                        
//                        output.println(daPrefix  + " [" + dAttribute.getFc().toString() + "]");
//                        
//                        printSubAttributeList(dAttribute, output, daPrefix);
//                    }
//                    
//                }
            }
        }
        
    }

    public static void main(String[] args) throws FileNotFoundException {

        if (args.length < 1) {
            System.out.println("SCL model viewer");
            System.out.println("Usage: scltool <SCL file> [-ied <ied-name>] [-ap <access-point-name>] [-t] [-s] [-a] [<output filename>]");
            System.out.println("  -ied select IED");
            System.out.println("  -ap select AP");
            System.out.println("  -t print type list");
            System.out.println("  -u print list of unused types");
            System.out.println("  -s print IED device model structure");
            System.out.println("  -a print list of data attributes (object references)");
            System.exit(1);
        }

        String icdFile = args[0];

        PrintStream outputStream = System.out;
        
        String accessPointName = null;
        String iedName = null;
        
        boolean printTypeList = false;
        boolean printUnusedTypes = false;
        boolean printModelStructure = false;
        boolean printDataAttribtues = false;
        
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
                else if (args[i].equals("-t")) {
                    printTypeList = true;
                    
                    i++;
                    
                }
                else if (args[i].equals("-s")) {
                    printModelStructure = true;
                    
                    i++;
                    
                }
                else if (args[i].equals("-a")) {
                    printDataAttribtues = true;
                    
                    i++;
                    
                }
                else if (args[i].equals("-u")) {
                    printUnusedTypes = true;
                    
                    i++;
                    
                }
                else {
                     outputStream = new PrintStream(new FileOutputStream(new File(args[i])));
                }
            }
                  
        }

        InputStream stream = new FileInputStream(icdFile);

        try {
            if (printTypeList)
                showTypes(stream, icdFile, outputStream, iedName, accessPointName, false);
            
            if (printUnusedTypes)
                showTypes(stream, icdFile, outputStream, iedName, accessPointName, true);
            
            if (printModelStructure)
                printModelStructure(stream, icdFile, outputStream, iedName, accessPointName);
            
            if (printDataAttribtues)
                printAttributeList(stream, icdFile, outputStream, iedName, accessPointName);
            
            
        } catch (SclParserException e) {
            System.err.println("ERROR: " + e.getMessage());
        }
    }

}
