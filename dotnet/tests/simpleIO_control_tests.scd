<?xml version="1.0" encoding="UTF-8"?>
<SCL xmlns="http://www.iec.ch/61850/2003/SCL">
  <Header id="" version="4.0.2" revision="" toolID="" nameStructure="IEDName">
  </Header>
  <Communication>
    <SubNetwork name="subnetwork1" type="8-MMS">
      <Text>Station bus</Text>
      <BitRate unit="b/s">10</BitRate>
      <ConnectedAP iedName="simpleIO" apName="accessPoint1">
        <Address>
          <P type="IP">10.0.0.2</P>
          <P type="IP-SUBNET">255.255.255.0</P>
          <P type="IP-GATEWAY">10.0.0.1</P>
          <P type="OSI-TSEL">0001</P>
          <P type="OSI-PSEL">00000001</P>
          <P type="OSI-SSEL">0001</P>
        </Address>
        <GSE ldInst="GenericIO" cbName="gcbEvents">
           <Address>
                <P type="VLAN-ID">1</P>
                <P type="VLAN-PRIORITY">4</P>
                <P type="MAC-Address">01-0c-cd-01-00-01</P>
                <P type="APPID">1000</P>
           </Address>
           <MinTime>1000</MinTime>
           <MaxTime>3000</MaxTime>
	    </GSE>
      </ConnectedAP>
    </SubNetwork>
  </Communication>
  <IED name="simpleIO">
    <Services>
      <DynAssociation />
      <GetDirectory />
      <GetDataObjectDefinition />
      <GetDataSetValue />
      <DataSetDirectory />
      <ReadWrite />
      <GetCBValues />
      <ConfLNs fixPrefix="true" fixLnInst="true" />
      <GOOSE max="5" />
      <GSSE max="5" />
      <FileHandling />
      <GSEDir />
      <TimerActivatedControl />
    </Services>
    <AccessPoint name="accessPoint1">
      <Server>
        <Authentication />
        <LDevice inst="GenericIO">
          <LN0 lnClass="LLN0" lnType="LLN01" inst="">
          	
            <DataSet name="Events" desc="Events">
              <FCDA ldInst="GenericIO" lnClass="GGIO" fc="ST" lnInst="1" doName="SPCSO1" daName="stVal" />
              <FCDA ldInst="GenericIO" lnClass="GGIO" fc="ST" lnInst="1" doName="SPCSO2" daName="stVal" />
              <FCDA ldInst="GenericIO" lnClass="GGIO" fc="ST" lnInst="1" doName="SPCSO3" daName="stVal" />
              <FCDA ldInst="GenericIO" lnClass="GGIO" fc="ST" lnInst="1" doName="SPCSO4" daName="stVal" />
            </DataSet>
            
             <DataSet name="EventsFCDO" desc="EventsFCDO">
              <FCDA ldInst="GenericIO" lnClass="GGIO" fc="ST" lnInst="1" doName="SPCSO1"/>
              <FCDA ldInst="GenericIO" lnClass="GGIO" fc="ST" lnInst="1" doName="SPCSO2"/>
              <FCDA ldInst="GenericIO" lnClass="GGIO" fc="ST" lnInst="1" doName="SPCSO3"/>
              <FCDA ldInst="GenericIO" lnClass="GGIO" fc="ST" lnInst="1" doName="SPCSO4"/>
            </DataSet>
            
            <DataSet name="Booleans" desc="Booleans">
              <FCDA ldInst="GenericIO" lnClass="LLN0" fc="SP" doName="SetBool1" daName="setVal" />
              <FCDA ldInst="GenericIO" lnClass="LLN0" fc="SP" doName="SetBool2" daName="setVal" />
              <FCDA ldInst="GenericIO" lnClass="LLN0" fc="SP" doName="SetBool3" daName="setVal" />
              <FCDA ldInst="GenericIO" lnClass="LLN0" fc="SP" doName="SetBool4" daName="setVal" />
            </DataSet>
            
            <DataSet name="Integers" desc="Integers">
              <FCDA ldInst="GenericIO" lnClass="LLN0" fc="SP" lnInst="1" doName="SetInt1" daName="setVal" />
              <FCDA ldInst="GenericIO" lnClass="LLN0" fc="SP" lnInst="1" doName="SetInt2" daName="setVal" />
              <FCDA ldInst="GenericIO" lnClass="LLN0" fc="SP" lnInst="1" doName="SetInt3" daName="setVal" />
              <FCDA ldInst="GenericIO" lnClass="LLN0" fc="SP" lnInst="1" doName="SetInt4" daName="setVal" />
            </DataSet>

            <ReportControl name="EventsRCB" confRev="1" datSet="Events" rptID="Events1" buffered="false" intgPd="1000" bufTime="50" indexed="true">
              <TrgOps period="true" />
              <OptFields seqNum="true" timeStamp="true" dataSet="true" reasonCode="true" entryID="true" configRef="true" />
              <RptEnabled max="2" />
            </ReportControl>
         
	        <ReportControl name="BufferedRCB" confRev="1" datSet="Events" rptID="Events1" buffered="true" intgPd="1000" bufTime="50" indexed="true">
              <TrgOps period="true" />
              <OptFields seqNum="true" timeStamp="true" dataSet="true" reasonCode="true" entryID="true" configRef="true" />
              <RptEnabled max="2" />
            </ReportControl>

 
            <GSEControl appID="events" name="gcbEvents" type="GOOSE" datSet="Events" confRev="1"/> 
          
            <DOI name="Mod">
              <DAI name="ctlModel">
                <Val>status-only</Val>
              </DAI>
            </DOI>
          </LN0>
          <LN lnClass="LPHD" lnType="LPHD1" inst="1" prefix="" />
          <LN lnClass="GGIO" lnType="GGIO1" inst="1" prefix="">
            <DOI name="Mod">
              <DAI name="ctlModel">
                <Val>status-only</Val>
              </DAI>
            </DOI>
            <DOI name="SPCSO1">
              <DAI name="ctlModel">
                <Val>direct-with-normal-security</Val>
              </DAI>
            </DOI>
            <DOI name="SPCSO2">
              <DAI name="ctlModel">
                <Val>sbo-with-normal-security</Val>
              </DAI>
            </DOI>
            <DOI name="SPCSO3">
              <DAI name="ctlModel">
                <Val>direct-with-enhanced-security</Val>
              </DAI>
            </DOI>
            <DOI name="SPCSO4">
              <DAI name="ctlModel">
                <Val>sbo-with-enhanced-security</Val>
              </DAI>
            </DOI>
            <DOI name="SPCSO5">
              <DAI name="ctlModel">
                <Val>direct-with-normal-security</Val>
              </DAI>
            </DOI>
            <DOI name="SPCSO6">
              <DAI name="ctlModel">
                <Val>sbo-with-normal-security</Val>
              </DAI>
            </DOI>
            <DOI name="SPCSO7">
              <DAI name="ctlModel">
                <Val>direct-with-enhanced-security</Val>
              </DAI>
            </DOI>
            <DOI name="SPCSO8">
              <DAI name="ctlModel">
                <Val>sbo-with-enhanced-security</Val>
              </DAI>
            </DOI>
          </LN>
          <LN lnClass="MHAI" lnType="MHAI1" inst="1" prefix=""> 
            <DOI name="HA">
               <DAI name="numHar">
                 <Val>16</Val>
               </DAI>
            </DOI>
          </LN>
        </LDevice>
      </Server>
    </AccessPoint>
  </IED>
  <DataTypeTemplates>
    <LNodeType id="LLN01" lnClass="LLN0">
      <DO name="Mod" type="INC_1_Mod" />
      <DO name="Beh" type="INS_1_Beh" />
      <DO name="Health" type="INS_1_Beh" />
      <DO name="NamPlt" type="LPL_1_NamPlt" />
      <DO name="SetInt1" type="ISP" />
      <DO name="SetInt2" type="ISP" />
      <DO name="SetInt3" type="ISP" />
      <DO name="SetInt4" type="ISP" />
      <DO name="SetBool1" type="SPG_1" />
      <DO name="SetBool2" type="SPG_1" />
      <DO name="SetBool3" type="SPG_1" />
      <DO name="SetBool4" type="SPG_1" />
      <DO name="Int64" type="INS_64" />
      <DO name="SetAnVal" type="ASG" />
    </LNodeType>
    <LNodeType id="LPHD1" lnClass="LPHD">
      <DO name="PhyNam" type="DPL_1_PhyNam" />
      <DO name="PhyHealth" type="INS_1_Beh" />
      <DO name="Proxy" type="SPS_1_Proxy" />
    </LNodeType>
    <LNodeType id="GGIO1" lnClass="GGIO">
      <DO name="Mod" type="INC_1_Mod" />
      <DO name="Beh" type="INS_1_Beh" />
      <DO name="Health" type="INS_1_Beh" />
      <DO name="NamPlt" type="LPL_2_NamPlt" />
      <DO name="AnIn1" type="MV_1_AnIn1" />
      <DO name="AnIn2" type="MV_1_AnIn1" />
      <DO name="AnIn3" type="MV_1_AnIn1" />
      <DO name="AnIn4" type="MV_1_AnIn1" />
      <DO name="SPCSO1" type="SPC_1_SPCSO1" />
      <DO name="SPCSO2" type="SPC_1_SPCSO2" />
      <DO name="SPCSO3" type="SPC_1_SPCSO3" />
      <DO name="SPCSO4" type="SPC_1_SPCSO4" />
      <DO name="SPCSO5" type="SPC_1_SPCSO5" />
      <DO name="SPCSO6" type="SPC_1_SPCSO6" />
      <DO name="SPCSO7" type="SPC_1_SPCSO7" />
      <DO name="SPCSO8" type="SPC_1_SPCSO8" />
      <DO name="Ind1" type="SPS_1_Proxy" />
      <DO name="Ind2" type="SPS_1_Proxy" />
      <DO name="Ind3" type="SPS_1_Proxy" />
      <DO name="Ind4" type="SPS_1_Proxy" />
      <DO name="SchdAbsTm" type="SCA_1_SchdAbsTm" />
    </LNodeType>
    
    <LNodeType id="MHAI1" lnClass="MHAI">
      <DO name="HA" type="HWYE_1_HA" />
    </LNodeType>
    
    <DOType id="SPG_1" cdc="SPG">

    	<DA name="setVal" bType="BOOLEAN" fc="SP" dchg="true" />
    </DOType>
    
    <DOType id="ASG" cdc="ASG">
    	<DA name="setMag" bType="Struct" type="AnalogueValue" fc="SP" dchg="true" />
    </DOType>
      
    <DOType id="ISP" cdc="INS">
      <DA name="setVal" bType="INT32" fc="SP" dchg="true" />
    </DOType>
    
    <DOType id="HWYE_1_HA" cdc="HWYE">
    	<SDO name="phsAHar" type="CMV_1_phsAHar" count="16" />
    	<DA name="numHar" bType="INT16U" fc="CF" dchg="true" />
    	<DA name="numCyc" bType="INT16U" fc="CF" dchg="true" />
    	<DA name="evalTm" bType="INT16U" fc="CF" dchg="true" />
    	<DA name="frequency" bType="FLOAT32" fc="CF" dchg="true" />
    </DOType>
    
	<DOType id="SCA_1_SchdAbsTm" cdc="SCA">
      <DA name="val" bType="FLOAT32" count="24" fc="SP" dchg="true" />
    </DOType>
    
    
    <DOType id="CMV_1_phsAHar" cdc="CMV">
    	<DA name="cVal" bType="Struct" type="Vector" fc="MX" dchg="true" dupd="true" />
        <DA name="q" bType="Quality" fc="MX" qchg="true" />
        <DA name="t" bType="Timestamp" fc="MX" />
    </DOType>
    
    <DAType id="Vector">
      <BDA name="mag" bType="Struct" type="AnalogueValue" /> <!-- M -->
      <BDA name="ang" bType="Struct" type="AnalogueValue" /> <!-- O -->
    </DAType>
    
    <DOType id="INC_1_Mod" cdc="INC">
      <DA name="q" bType="Quality" fc="ST" qchg="true" />
      <DA name="t" bType="Timestamp" fc="ST" />
      <DA name="ctlModel" type="CtlModels" bType="Enum" fc="CF" />
    </DOType>
    <DOType id="INS_1_Beh" cdc="INS">
      <DA name="stVal" bType="INT32" fc="ST" dchg="true" />
      <DA name="q" bType="Quality" fc="ST" qchg="true" />
      <DA name="t" bType="Timestamp" fc="ST" />
    </DOType>
    
     <DOType id="INS_64" cdc="INS">
      <DA name="stVal" bType="INT64" fc="ST" dchg="true" />
      <DA name="q" bType="Quality" fc="ST" qchg="true" />
      <DA name="t" bType="Timestamp" fc="ST" />
    </DOType>
    
    <DOType id="LPL_1_NamPlt" cdc="LPL">
      <DA name="vendor" bType="VisString255" fc="DC" />
      <DA name="swRev" bType="VisString255" fc="DC" />
      <DA name="d" bType="VisString255" fc="DC" />
      <DA name="configRev" bType="VisString255" fc="DC" />
      <DA name="ldNs" bType="VisString255" fc="EX" />
    </DOType>
    <DOType id="DPL_1_PhyNam" cdc="DPL">
      <DA name="vendor" bType="VisString255" fc="DC" />
    </DOType>
    <DOType id="SPS_1_Proxy" cdc="SPS">
      <DA name="stVal" bType="BOOLEAN" fc="ST" dchg="true" />
      <DA name="q" bType="Quality" fc="ST" qchg="true" />
      <DA name="t" bType="Timestamp" fc="ST" />
    </DOType>
    <DOType id="SPC_1_SPCSO8" cdc="SPC">
      <DA name="SBOw" type="SPCOperate_5" bType="Struct" fc="CO" />
      <DA name="Oper" type="SPCOperate_5" bType="Struct" fc="CO" />
      <DA name="Cancel" type="SPCCancel_5" bType="Struct" fc="CO" />
      <DA name="origin" type="Originator_1" bType="Struct" fc="ST" />
      <DA name="ctlNum" bType="INT8U" fc="ST" />
      <DA name="stVal" bType="BOOLEAN" fc="ST" dchg="true" />
      <DA name="q" bType="Quality" fc="ST" qchg="true" />
      <DA name="t" bType="Timestamp" fc="ST" />
      <DA name="ctlModel" type="CtlModels" bType="Enum" fc="CF" />
    </DOType>
    <DOType id="SPC_1_SPCSO7" cdc="SPC">
      <DA name="Oper" type="SPCOperate_5" bType="Struct" fc="CO" />
      <DA name="Cancel" type="SPCCancel_5" bType="Struct" fc="CO" />
      <DA name="stVal" bType="BOOLEAN" fc="ST" dchg="true" />
      <DA name="q" bType="Quality" fc="ST" qchg="true" />
      <DA name="t" bType="Timestamp" fc="ST" />
      <DA name="ctlModel" type="CtlModels" bType="Enum" fc="CF" />
    </DOType>
    <DOType id="SPC_1_SPCSO3" cdc="SPC">
      <DA name="Oper" type="SPCOperate_1" bType="Struct" fc="CO" />
      <DA name="Cancel" type="SPCCancel_1" bType="Struct" fc="CO" />
      <DA name="stVal" bType="BOOLEAN" fc="ST" dchg="true" />
      <DA name="q" bType="Quality" fc="ST" qchg="true" />
      <DA name="t" bType="Timestamp" fc="ST" />
      <DA name="ctlModel" type="CtlModels" bType="Enum" fc="CF" />
    </DOType>
    <DOType id="MV_1_AnIn1" cdc="MV">
      <DA name="mag" type="" bType="Struct" fc="MX" dchg="true" />
      <DA name="q" bType="Quality" fc="MX" qchg="true" />
      <DA name="t" bType="Timestamp" fc="MX" />
    </DOType>
    <DOType id="SPC_1_SPCSO6" cdc="SPC">
      <DA name="SBO" bType="VisString64" fc="CO" />
      <DA name="Oper" type="SPCOperate_5" bType="Struct" fc="CO" />
      <DA name="Cancel" type="SPCCancel_5" bType="Struct" fc="CO" />
      <DA name="stVal" bType="BOOLEAN" fc="ST" dchg="true" />
      <DA name="q" bType="Quality" fc="ST" qchg="true" />
      <DA name="t" bType="Timestamp" fc="ST" />
      <DA name="ctlModel" type="CtlModels" bType="Enum" fc="CF" />
    </DOType>
    <DOType id="SPC_1_SPCSO5" cdc="SPC">
      <DA name="Oper" type="SPCOperate_5" bType="Struct" fc="CO" />
      <DA name="stVal" bType="BOOLEAN" fc="ST" dchg="true" />
      <DA name="q" bType="Quality" fc="ST" qchg="true" />
      <DA name="t" bType="Timestamp" fc="ST" />
      <DA name="ctlModel" type="CtlModels" bType="Enum" fc="CF" />
      <DA name="Cancel" type="SPCCancel_1" bType="Struct" fc="CO" />
    </DOType>
    <DOType id="SPC_1_SPCSO4" cdc="SPC">
      <DA name="SBOw" type="SPCOperate_1" bType="Struct" fc="CO" />
      <DA name="Oper" type="SPCOperate_1" bType="Struct" fc="CO" />
      <DA name="Cancel" type="SPCCancel_1" bType="Struct" fc="CO" />
      <DA name="stVal" bType="BOOLEAN" fc="ST" dchg="true" />
      <DA name="q" bType="Quality" fc="ST" qchg="true" />
      <DA name="t" bType="Timestamp" fc="ST" />
      <DA name="ctlModel" type="CtlModels" bType="Enum" fc="CF" />
    </DOType>
    <DOType id="LPL_2_NamPlt" cdc="LPL">
      <DA name="vendor" bType="VisString255" fc="DC" />
      <DA name="swRev" bType="VisString255" fc="DC" />
      <DA name="d" bType="VisString255" fc="DC" />
      <DA name="dU" fc="DC" bType="Unicode255"/>
    </DOType>
    <DOType id="SPC_1_SPCSO2" cdc="SPC">
      <DA name="SBO" bType="VisString64" fc="CO" />
      <DA name="Oper" type="SPCOperate_1" bType="Struct" fc="CO" />
      <DA name="Cancel" type="SPCCancel_1" bType="Struct" fc="CO" />
      <DA name="stVal" bType="BOOLEAN" fc="ST" dchg="true" />
      <DA name="q" bType="Quality" fc="ST" qchg="true" />
      <DA name="t" bType="Timestamp" fc="ST" />
      <DA name="ctlModel" type="CtlModels" bType="Enum" fc="CF" />
      <DA name="sboClass" type="SboClasses" bType="Enum" fc="CF" />
    </DOType>
    <DOType id="SPC_1_SPCSO1" cdc="SPC">
      <DA name="Oper" type="SPCOperate_1" bType="Struct" fc="CO" />
      <DA name="stVal" bType="BOOLEAN" fc="ST" dchg="true" />
      <DA name="q" bType="Quality" fc="ST" qchg="true" />
      <DA name="t" bType="Timestamp" fc="ST" />
      <DA name="ctlModel" type="CtlModels" bType="Enum" fc="CF" />
    </DOType>
    <DAType id="SPCOperate_1">
      <BDA name="ctlVal" bType="BOOLEAN" />
      <BDA name="origin" type="Originator_1" bType="Struct" />
      <BDA name="ctlNum" bType="INT8U" />
      <BDA name="T" bType="Timestamp" />
      <BDA name="Test" bType="BOOLEAN" />
      <BDA name="Check" bType="Check" />
    </DAType>
    <DAType id="Originator_1">
      <BDA name="orCat" type="OrCat" bType="Enum" />
      <BDA name="orIdent" bType="Octet64" />
    </DAType>
    <DAType id="SPCOperate_5">
      <BDA name="ctlVal" bType="BOOLEAN" />
      <BDA name="operTm" bType="Timestamp" />
      <BDA name="origin" type="Originator_1" bType="Struct" />
      <BDA name="ctlNum" bType="INT8U" />
      <BDA name="T" bType="Timestamp" />
      <BDA name="Test" bType="BOOLEAN" />
      <BDA name="Check" bType="Check" />
    </DAType>
    <DAType id="">
      <BDA name="f" bType="FLOAT32" />
    </DAType>
    <DAType id="SPCCancel_1">
      <BDA name="ctlVal" bType="BOOLEAN" />
      <BDA name="origin" type="Originator_1" bType="Struct" />
      <BDA name="ctlNum" bType="INT8U" />
      <BDA name="T" bType="Timestamp" />
      <BDA name="Test" bType="BOOLEAN" />
    </DAType>
    <DAType id="SPCCancel_5">
      <BDA name="ctlVal" bType="BOOLEAN" />
      <BDA name="operTm" bType="Timestamp" />
      <BDA name="origin" type="Originator_1" bType="Struct" />
      <BDA name="ctlNum" bType="INT8U" />
      <BDA name="T" bType="Timestamp" />
      <BDA name="Test" bType="BOOLEAN" />
    </DAType>
    
    <DAType id="AnalogueValue"><!-- sec 6.3 -->
      <!--<BDA name="i" bType="INT32" /> --><!-- GC_1 -->
      <BDA name="f" bType="FLOAT32" /><!-- GC_1 -->
    </DAType>
    
    
    <EnumType id="CtlModels">
      <EnumVal ord="0">status-only</EnumVal>
      <EnumVal ord="1">direct-with-normal-security</EnumVal>
      <EnumVal ord="2">sbo-with-normal-security</EnumVal>
      <EnumVal ord="3">direct-with-enhanced-security</EnumVal>
      <EnumVal ord="4">sbo-with-enhanced-security</EnumVal>
    </EnumType>
    <EnumType id="SboClasses">
      <EnumVal ord="0">operate-once</EnumVal>
      <EnumVal ord="1">operate-many</EnumVal>
    </EnumType>
    <EnumType id="OrCat">
      <EnumVal ord="0">not-supported</EnumVal>
      <EnumVal ord="1">bay-control</EnumVal>
      <EnumVal ord="2">station-control</EnumVal>
      <EnumVal ord="3">remote-control</EnumVal>
      <EnumVal ord="4">automatic-bay</EnumVal>
      <EnumVal ord="5">automatic-station</EnumVal>
      <EnumVal ord="6">automatic-remote</EnumVal>
      <EnumVal ord="7">maintenance</EnumVal>
      <EnumVal ord="8">process</EnumVal>
    </EnumType>
  </DataTypeTemplates>
</SCL>
