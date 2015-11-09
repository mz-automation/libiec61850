package com.libiec61850.scl.model;

import org.w3c.dom.Node;

import com.libiec61850.scl.ParserUtils;
import com.libiec61850.scl.SclParserException;

public class SmvOpts {
    
    
    private boolean refreshTime = false;
    private boolean sampleRate = false;
    private boolean dataSet = false;
    private boolean security = false;
    private boolean sampleSynchronized = false;

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
    
}
