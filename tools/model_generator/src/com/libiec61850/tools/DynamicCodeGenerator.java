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

import com.libiec61850.scl.DataObjectDefinition;
import com.libiec61850.scl.SclParser;
import com.libiec61850.scl.SclParserException;
import com.libiec61850.scl.model.LogicalNode;
import com.libiec61850.scl.types.DataObjectType;
import com.libiec61850.scl.types.LogicalNodeType;
import com.libiec61850.scl.types.SclType;
import com.libiec61850.scl.types.TypeDeclarations;

public class DynamicCodeGenerator {

    public static void main(String[] args) throws FileNotFoundException, SclParserException {

        System.out.println("Dynamic code generator");

        if (args.length < 1) {
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

        SclParser sclParser = new SclParser(stream);
        
        createDynamicCode(sclParser);
    }

    private static void createDynamicCode(SclParser sclParser) {
        TypeDeclarations declarations = sclParser.getTypeDeclarations();
        
        PrintStream out = System.out;
        
        Set<DataObjectType> doTypeDefs = new HashSet<DataObjectType>();
        Set<LogicalNodeType> lnTypeDefs = new HashSet<LogicalNodeType>();
        
        List<String> functionPrototypes = new LinkedList<String>();
        
        List<SclType> types =  declarations.getTypeDeclarations();
        
        for (SclType type : types) {
            
            if (type.getClass().equals(LogicalNodeType.class))
                lnTypeDefs.add((LogicalNodeType) type);
            else if (type.getClass().equals(DataObjectType.class))
                doTypeDefs.add((DataObjectType) type);
            
        }
        
        for (LogicalNodeType lnType : lnTypeDefs) {
            String functionPrototype = "LogicalNode*\nLN_" + lnType.getId() 
                    + "_createInstance(char* lnName, LogicalDevice* parent);";
            
            functionPrototypes.add(functionPrototype);
            
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
                out.printf("    %s_createInstance(\"%s\", (ModelNode*) newLn);\n", objDef.getType(), objDef.getName());
            }
            
            out.println("\n    return newLn;");           
            out.println("}\n\n");
        }
        
        for (DataObjectType doType : doTypeDefs) {
            String functionPrototype = "DataObject*\nDO_" + doType.getId()
                    + "_createInstance(char* doName, ModelNode* parent);";
            
            functionPrototypes.add(functionPrototype);
            
            out.println("/**");
            out.printf(" * DO: %s ", doType.getId());
            if (doType.getDesc() != null)
                out.printf("(%s)", doType.getDesc());
            out.println();
            out.println(" */");
            out.println("DataObject*");
            out.printf("DO_%s_createInstance(char* doName, ModelNode* parent)\n", doType.getId());
            out.println("{");
            out.println("    LogicalNode* newDo = DataObject_create(doName, parent);\n");
            
            out.println("\n    return newDo;");
            out.println("}\n\n");
        }
        

    }

}
