package com.libiec61850.tools;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.PrintStream;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.Set;

import com.libiec61850.scl.DataAttributeDefinition;
import com.libiec61850.scl.DataObjectDefinition;
import com.libiec61850.scl.SclParser;
import com.libiec61850.scl.SclParserException;
import com.libiec61850.scl.model.AttributeType;
import com.libiec61850.scl.model.LogicalNode;
import com.libiec61850.scl.types.DataAttributeType;
import com.libiec61850.scl.types.DataObjectType;
import com.libiec61850.scl.types.LogicalNodeType;
import com.libiec61850.scl.types.SclType;
import com.libiec61850.scl.types.TypeDeclarations;

public class DynamicCodeGenerator {

    public static void main(String[] args) throws FileNotFoundException, SclParserException {

        if (args.length < 1) {
            System.out.println("Dynamic code generator");
            
            System.out.println("Usage: gencode <ICD file> [-ied  <ied-name>] [-ap <access-point-name>] [<output filename>]");
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
        
        SclParser.withOutput = false;

        SclParser sclParser = new SclParser(stream, false);
        
        createDynamicCode(sclParser);
    }

    private static void createDynamicCode(SclParser sclParser) {
        TypeDeclarations declarations = sclParser.getTypeDeclarations();
        
        PrintStream out = System.out;
        
        Set<DataObjectType> doTypeDefs = new HashSet<DataObjectType>();
        Set<LogicalNodeType> lnTypeDefs = new HashSet<LogicalNodeType>();
        Set<DataAttributeType> daTypeDefs = new HashSet<DataAttributeType>();
        
        List<String> functionPrototypes = new LinkedList<String>();
         
        
        /* Create type lists */
        
        for (SclType type : declarations.getTypeDeclarations()) {
            
            if (type.getClass().equals(LogicalNodeType.class))
                lnTypeDefs.add((LogicalNodeType) type);
            else if (type.getClass().equals(DataObjectType.class))
                doTypeDefs.add((DataObjectType) type);
            else if (type.getClass().equals(DataAttributeType.class))
                daTypeDefs.add((DataAttributeType) type);    
        }
        
        /* Create function prototypes */
        
        for (LogicalNodeType lnType : lnTypeDefs) {
            String functionPrototype = "LogicalNode*\nLN_" + lnType.getId() 
                    + "_createInstance(char* lnName, LogicalDevice* parent);";
            
            functionPrototypes.add(functionPrototype);
        }
        
        for (DataObjectType doType : doTypeDefs) {
            String functionPrototype = "DataObject*\nDO_" + doType.getId()
                    + "_createInstance(char* doName, ModelNode* parent, int arrayCount);";
            
            functionPrototypes.add(functionPrototype);
        }
        
        for (DataAttributeType daType : daTypeDefs) {
            String functionPrototype = "DataAttribute*\nDA_" + daType.getId()
                    + "_createInstance(char* daName, ModelNode* parent, FunctionalConstraint fc, uint8_t triggerOptions);";
            
            functionPrototypes.add(functionPrototype);
        }
        
        /* Print header */
        
        System.out.println("#include \"iec61850_server.h\"");
        System.out.println();
        
        for (String prototype : functionPrototypes) {
            System.out.println(prototype);
            System.out.println();
        }
        
        
        for (LogicalNodeType lnType : lnTypeDefs) {
            
            out.println("/**");
            out.printf(" * LN: %s ", lnType.getId());
            if (lnType.getDesc() != null)
                out.printf("(%s)", lnType.getDesc());
            out.println();
            out.println(" */");
            out.println("LogicalNode*");
            out.printf("LN_%s_createInstance(char* lnName, LogicalDevice* parent)\n", lnType.getId());
            out.println("{");
            out.println("    LogicalNode* newLn = LogicalNode_create(lnName, parent);\n");
            List<DataObjectDefinition> doDefs = lnType.getDataObjectDefinitions();
            
            for (DataObjectDefinition objDef : doDefs) {             
                out.printf("    DO_%s_createInstance(\"%s\", (ModelNode*) newLn, %d);\n", objDef.getType(), objDef.getName(), objDef.getCount());
            }
            
            out.println("\n    return newLn;");           
            out.println("}\n\n");
        }
        
        for (DataObjectType doType : doTypeDefs) {
            out.println("/**");
            out.printf(" * DO: %s ", doType.getId());
            if (doType.getDesc() != null)
                out.printf("(%s)", doType.getDesc());
            out.println();
            out.println(" */");
            out.println("DataObject*");
            out.printf("DO_%s_createInstance(char* doName, ModelNode* parent, int arrayCount)\n", doType.getId());
            out.println("{");
          
            out.println("    DataObject* newDo = DataObject_create(doName, parent, arrayCount);\n");
            
            for (DataAttributeDefinition dad : doType.getDataAttributes()) {
                
                if (dad.getAttributeType() == AttributeType.CONSTRUCTED) {    
                    out.print("    DA_" + dad.getType() + "_createInstance(\"" + dad.getName() + "\", ");
                    out.print("(ModelNode*) newDo, IEC61850_FC_" + dad.getFc().toString());
                    out.print(", " + dad.getTriggerOptions().getIntValue());
                    out.println(");");
                }
                else {
                    out.print("    DataAttribute_create(\"" + dad.getName() + "\", ");
                    out.print("(ModelNode*) newDo, IEC61850_" + dad.getAttributeType());
                    out.print(", IEC61850_FC_" + dad.getFc().toString());
                    out.print(", " + dad.getTriggerOptions().getIntValue());
                    out.print(", " + dad.getCount());
                    out.print(", 0");
                    out.println(");");
                }
                    
            }
            
            
            for (DataObjectDefinition dod : doType.getSubDataObjects()) {
                out.print("    DO_" + dod.getType() + "_createInstance(\"" + dod.getName() + "\", ");
                out.println("(ModelNode*) newDo, " + dod.getCount() + ");");
            }
            
            out.println("\n    return newDo;");
            out.println("}\n\n");
        }
        
        for (DataAttributeType daType : daTypeDefs) {  
            out.println("/**");
            out.printf(" * DA: %s ", daType.getId());
            if (daType.getDesc() != null)
                out.printf("(%s)", daType.getDesc());
            out.println();
            out.println(" */");
            out.println("DataAttribute*");
            out.printf("DA_%s_createInstance(char* daName, ModelNode* parent, FunctionalConstraint fc, uint8_t triggerOptions)\n", daType.getId());
            out.println("{");   
            out.println("    DataAttribute* newDa = DataAttribute_create(daName, parent, IEC61850_CONSTRUCTED, fc, triggerOptions, 0, 0);\n");
            
            for (DataAttributeDefinition dad : daType.getSubDataAttributes()) {
                if (dad.getAttributeType() == AttributeType.CONSTRUCTED) {    
                    out.print("    DA_" + dad.getType() + "_createInstance(\"" + dad.getName() + "\", ");
                    out.println("(ModelNode*) newDa, fc, triggerOptions);");
                }
                else {
                    out.print("    DataAttribute_create(\"" + dad.getName() + "\", ");
                    out.print("(ModelNode*) newDa, IEC61850_" + dad.getAttributeType());
                    out.print(", fc");
                    out.print(", triggerOptions");
                    out.print(", " + dad.getCount());
                    out.print(", 0");
                    out.println(");");
                }
            }
            
            out.println("\n    return newDa;");
            out.println("}\n\n");
            
        }

    }

}
