<?xml version="1.0" encoding="UTF-8"?>
<!--This file is generated using Kalkitech SCL Manager IEC61850 Configuration Tool (www.kalkitech.com)-->
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
    </LNodeType>
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
      <DA name="mag" type="AnalogueValue_1" bType="Struct" fc="MX" dchg="true" />
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
    <DAType id="AnalogueValue_1">
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
