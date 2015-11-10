package com.libiec61850.scl.model;

import org.w3c.dom.Node;

import com.libiec61850.scl.ParserUtils;
import com.libiec61850.scl.SclParserException;

public class SmvOpts {
    
    
    private boolean refreshTime = false; /* 1 */
    private boolean sampleSynchronized = false; /* 2 */
    private boolean sampleRate = false; /* 4 */
    private boolean dataSet = false; /* 8 */
    private boolean security = false; /* 16 */
   

    public SmvOpts(Node smvOptsNode) throws SclParserException {

        Boolean refreshTime = ParserUtils.parseBooleanAttribute(smvOptsNode, "refreshTime");
        if (refreshTime != null)
            this.refreshTime = refreshTime;

        Boolean sampleRate = ParserUtils.parseBooleanAttribute(smvOptsNode, "sampleRate");
        if (sampleRate != null)
            this.sampleRate = sampleRate;

        Boolean dataSet = ParserUtils.parseBooleanAttribute(smvOptsNode, "dataSet");
        if (dataSet != null)
            this.dataSet = dataSet;

        Boolean security = ParserUtils.parseBooleanAttribute(smvOptsNode, "security");
        if (security != null)
            this.security = security;
        
        Boolean sampleSynchronized = ParserUtils.parseBooleanAttribute(smvOptsNode, "sampleSynchronized");
        if (sampleSynchronized != null)
            this.sampleSynchronized = sampleSynchronized;

    }
    
    
    public int getIntValue() {
        int intValue = 0;
        
        if (refreshTime) intValue += 1;
        if (sampleSynchronized) intValue += 2;
        if (sampleRate) intValue += 4;
        if (dataSet) intValue += 8;
        if (security) intValue += 16;
        
        return intValue;
    }
}
