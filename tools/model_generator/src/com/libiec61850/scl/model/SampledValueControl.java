package com.libiec61850.scl.model;

import org.w3c.dom.Node;

import com.libiec61850.scl.ParserUtils;
import com.libiec61850.scl.SclParserException;

public class SampledValueControl {
    
    private String name;
    private String desc = null;
    private String datSet;
    private int confRev = 1;
    private String smvID;
    private int smpRate;
    private int nofASDU;
    private boolean multicast = false;
    private SmvOpts smvOpts;
    private SmpMod smpMod = SmpMod.SMP_PER_PERIOD;

    public SampledValueControl(Node smvControlNode) throws SclParserException {
        this.name = ParserUtils.parseAttribute(smvControlNode, "name");
        this.desc = ParserUtils.parseAttribute(smvControlNode, "desc");
        this.datSet = ParserUtils.parseAttribute(smvControlNode, "datSet");
        
        String confRevString = ParserUtils.parseAttribute(smvControlNode, "confRev");
        
        if (confRevString != null)
            this.confRev = new Integer(confRevString);
        
        this.smvID = ParserUtils.parseAttribute(smvControlNode, "smvID");
        
        Boolean multicast = ParserUtils.parseBooleanAttribute(smvControlNode, "multicast");
        
        if (multicast != null)
            this.multicast = multicast;
        
        String smpRateString = ParserUtils.parseAttribute(smvControlNode, "smpRate");
        
        if (smpRateString != null)
            this.smpRate = new Integer(smpRateString);
        
        String nofASDUString = ParserUtils.parseAttribute(smvControlNode, "nofASDU");
        
        if (nofASDUString != null)
            this.nofASDU = new Integer(nofASDUString);
        
        Node smvOptsNode = ParserUtils.getChildNodeWithTag(smvControlNode, "SmvOpts");
        
        this.smvOpts = new SmvOpts(smvOptsNode);

        String smpModString = ParserUtils.parseAttribute(smvControlNode, "smpMod");

        if (smpModString != null) {
            if (smpModString.equals("SmpPerPeriod")) {
                smpMod = SmpMod.SMP_PER_PERIOD;
            }
            else if (smpModString.equals("SmpPerSec")) {
                smpMod = SmpMod.SMP_PER_SECOND;
            }
            else if (smpModString.equals("SecPerSmp")) {
                smpMod = SmpMod.SEC_PER_SMP;
            }
            else {
                throw new SclParserException(smvControlNode, "Invalid smpMod value " + smpModString);
            }
        }
    }

    public String getName() {
        return name;
    }
    
    public String getDesc() {
        return desc;
    }
    
    public String getDatSet() {
        return datSet;
    }
    
    public int getConfRev() {
        return confRev;
    }
    
    public String getSmvID() {
        return smvID;
    }
    
    public int getSmpRate() {
        return smpRate;
    }
    
    public int getNofASDI() {
        return nofASDU;
    }

    public boolean isMulticast() {
        return multicast;
    }
    
    public SmvOpts getSmvOpts() {
        return smvOpts;
    }

    public SmpMod getSmpMod() {
        return smpMod;
    }
}
