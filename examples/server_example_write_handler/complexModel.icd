<?xml version="1.0" encoding="UTF-8"?>
<SCL xmlns="http://www.iec.ch/61850/2003/SCL">
  <Header id="" />
  <IED name="ied1">
    <Services>
      <DynAssociation />
      <GetDirectory />
      <GetDataObjectDefinition />
      <GetDataSetValue />
      <DataSetDirectory />
      <ReadWrite />
      <GetCBValues />
      <ConfLNs fixPrefix="true" fixLnInst="true" />
    </Services>
    <AccessPoint name="accessPoint1">
      <Server>
        <Authentication password="true" />
        <LDevice inst="Inverter">
          <LN0 lnClass="LLN0" lnType="LLN02" inst="">
            <DataSet name="dataset1" desc="">
			  <FCDA ldInst="Inverter" lnClass="LLN0" fc="ST" doName="Mod" daName="q" />
			  <FCDA ldInst="Battery" lnClass="LLN0" fc="ST" doName="Mod" daName="q" />
              <FCDA ldInst="Inverter" lnClass="MMXU" fc="ST" lnInst="1" doName="Mod" daName="q" />
              <FCDA ldInst="Inverter" lnClass="MMXU" fc="CF" lnInst="1" doName="Mod" daName="ctlModel" />
              <FCDA ldInst="Inverter" lnClass="MMXU" fc="MX" lnInst="1" doName="TotW" daName="mag" />
            </DataSet>
            
            <ReportControl name="rcb1" confRev="0" datSet="dataset1" rptID="ID" intgPd="0" bufTime="0">
              <TrgOps dchg="true" qchg="true" />
              <OptFields />
            </ReportControl>
            
            <DOI name="Mod">
              <DAI name="ctlModel">
                <Val>status-only</Val>
              </DAI>
            </DOI>
          </LN0>
          <LN lnClass="LPHD" lnType="LPHD2" inst="1" prefix="" />
          <LN lnClass="ZINV" lnType="ZINV3" inst="1" prefix="">
            <DOI name="Mod">
              <DAI name="ctlModel">
                <Val>status-only</Val>
              </DAI>
            </DOI>
          </LN>
          <LN lnClass="MMXU" lnType="MMXU1" inst="1" prefix="">
            <DOI name="Mod">
              <DAI name="ctlModel">
                <Val>status-only</Val>
              </DAI>
            </DOI>
          </LN>
        </LDevice>
        <LDevice inst="Battery">
          <LN0 lnClass="LLN0" lnType="LLN03" inst="">
            <DOI name="Mod">
              <DAI name="ctlModel">
                <Val>status-only</Val>
              </DAI>
            </DOI>
          </LN0>
          <LN lnClass="LPHD" lnType="LPHD3" inst="1" prefix="" />
          <LN lnClass="ZBAT" lnType="ZBAT1" inst="1" prefix="">
            <DOI name="Mod">
              <DAI name="ctlModel">
                <Val>status-only</Val>
              </DAI>
            </DOI>
          </LN>
          <LN lnClass="ZBTC" lnType="ZBTC1" inst="1" prefix="">
            <DOI name="Mod">
              <DAI name="ctlModel">
                <Val>status-only</Val>
              </DAI>
            </DOI>
          </LN>
        </LDevice>
        <LDevice inst="Physical_Measurements">
          <LN0 lnClass="LLN0" lnType="LLN01" inst="">
            <DOI name="Mod">
              <DAI name="ctlModel">
                <Val>status-only</Val>
              </DAI>
            </DOI>
          </LN0>
          <LN lnClass="LPHD" lnType="LPHD1" inst="1" prefix="" />
        </LDevice>
      </Server>
    </AccessPoint>
  </IED>
  <DataTypeTemplates>
    <LNodeType id="MMXU1" lnClass="MMXU" desc="Measurement">
      <DO name="Mod" type="INC_1_Mod" />
      <DO name="Beh" type="INS_1_Beh" />
      <DO name="Health" type="INS_1_Beh" />
      <DO name="NamPlt" type="LPL_2_NamPlt" />
      <DO name="TotW" type="MV_1_Vol" />
      <DO name="TotVAr" type="MV_1_Vol" />
      <DO name="TotVA" type="MV_1_Vol" />
      <DO name="Hz" type="MV_1_Vol" />
      <DO name="PhV" type="WYE_1_PhV" />
      <DO name="A" type="WYE_1_PhV" />
      <DO name="W" type="WYE_1_W" />
    </LNodeType>
    <LNodeType id="ZBTC1" lnClass="ZBTC" desc="Battery charger">
      <DO name="Mod" type="ENC_1_Mod" />
      <DO name="Beh" type="ENS_1_Beh" />
      <DO name="Health" type="ENS_1_Beh" />
      <DO name="NamPlt" type="LPL_2_NamPlt" />
      <DO name="BatChaSt" type="ENG_1_BatChaSt" />
      <DO name="BatChaPwr" type="ASG_1_BatChaPwr" />
      <DO name="BatChaMod" type="ENG_1_BatChaSt" />
      <DO name="ChaV" type="MV_1_Vol" />
      <DO name="ChaA" type="MV_1_Vol" />
    </LNodeType>
    <LNodeType id="ZBAT1" lnClass="ZBAT" desc="Battery">
      <DO name="Mod" type="INC_1_Mod" />
      <DO name="Beh" type="INS_1_Beh" />
      <DO name="Health" type="INS_1_Beh" />
      <DO name="NamPlt" type="LPL_2_NamPlt" />
      <DO name="Vol" type="MV_1_Vol" />
      <DO name="Amp" type="MV_1_Vol" />
    </LNodeType>
    <LNodeType id="LPHD1" lnClass="LPHD" desc="Physical device information">
      <DO name="PhyNam" type="DPL_1_PhyNam" />
      <DO name="PhyHealth" type="INS_1_Beh" />
      <DO name="Proxy" type="SPS_1_Proxy" />
    </LNodeType>
    <LNodeType id="LLN01" lnClass="LLN0" desc="Logical node zero">
      <DO name="Mod" type="INC_1_Mod" />
      <DO name="Beh" type="INS_1_Beh" />
      <DO name="Health" type="INS_1_Beh" />
      <DO name="NamPlt" type="LPL_1_NamPlt" />
    </LNodeType>
    <LNodeType id="LLN02" lnClass="LLN0" desc="Logical node zero">
      <DO name="Mod" type="INC_1_Mod" />
      <DO name="Beh" type="INS_1_Beh" />
      <DO name="Health" type="INS_1_Beh" />
      <DO name="NamPlt" type="LPL_1_NamPlt" />
    </LNodeType>
    <LNodeType id="LPHD2" lnClass="LPHD" desc="Physical device information">
      <DO name="PhyNam" type="DPL_1_PhyNam" />
      <DO name="PhyHealth" type="INS_1_Beh" />
      <DO name="Proxy" type="SPS_1_Proxy" />
    </LNodeType>
    <LNodeType id="LLN03" lnClass="LLN0" desc="Logical node zero">
      <DO name="Mod" type="INC_1_Mod" />
      <DO name="Beh" type="INS_1_Beh" />
      <DO name="Health" type="INS_1_Beh" />
      <DO name="NamPlt" type="LPL_1_NamPlt" />
    </LNodeType>
    <LNodeType id="LPHD3" lnClass="LPHD" desc="Physical device information">
      <DO name="PhyNam" type="DPL_1_PhyNam" />
      <DO name="PhyHealth" type="INS_1_Beh" />
      <DO name="Proxy" type="SPS_1_Proxy" />
    </LNodeType>
    <LNodeType id="ZINV3" lnClass="ZINV" desc="Inverter">
      <DO name="Mod" type="ENC_1_Mod" />
      <DO name="Beh" type="ENS_1_Beh" />
      <DO name="Health" type="ENS_1_Beh" />
      <DO name="NamPlt" type="LPL_2_NamPlt" />
      <DO name="WRtg" type="ASG_1_WRtg" />
      <DO name="VarRtg" type="ASG_1_WRtg" />
      <DO name="ACTyp" type="ENG_1_ACTyp" />
      <DO name="OutWSet" type="ASG_1_WRtg" />
      <DO name="OutVarSet" type="ASG_1_WRtg" />
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
    <DOType id="LPL_2_NamPlt" cdc="LPL">
      <DA name="vendor" bType="VisString255" fc="DC" />
      <DA name="swRev" bType="VisString255" fc="DC" />
      <DA name="d" bType="VisString255" fc="DC" />
    </DOType>
    <DOType id="ENC_1_Mod" cdc="ENC">
      <DA name="q" bType="Quality" fc="ST" qchg="true" />
      <DA name="t" bType="Timestamp" fc="ST" />
      <DA name="ctlModel" type="CtlModels" bType="Enum" fc="CF" />
    </DOType>
    <DOType id="ENS_1_Beh" cdc="ENS">
      <DA name="stVal" bType="INT32" fc="ST" dchg="true" />
      <DA name="q" bType="Quality" fc="ST" qchg="true" />
      <DA name="t" bType="Timestamp" fc="ST" />
    </DOType>
    <DOType id="WYE_1_W" cdc="WYE">
      <SDO name="phsA" type="CMV_1_phsA" />
      <SDO name="phsB" type="CMV_1_phsA" />
      <SDO name="phsC" type="CMV_1_phsA" />
    </DOType>
    <DOType id="CMV_1_phsA" cdc="CMV">
      <DA name="cVal" type="Vector_1" bType="Struct" fc="MX" dchg="true" />
      <DA name="q" bType="Quality" fc="MX" qchg="true" />
      <DA name="t" bType="Timestamp" fc="MX" />
    </DOType>
    <DOType id="ASG_1_BatChaPwr" cdc="ASG" />
    <DOType id="ENG_1_BatChaSt" cdc="ENG" />
    <DOType id="WYE_1_PhV" cdc="WYE">
      <SDO name="phsA" type="CMV_1_phsA" />
      <SDO name="phsB" type="CMV_1_phsA" />
      <SDO name="phsC" type="CMV_1_phsA" />
      <SDO name="neut" type="CMV_1_phsA" />
    </DOType>
    <DOType id="MV_1_Vol" cdc="MV">
      <DA name="mag" type="AnalogueValue_1" bType="Struct" fc="MX" dchg="true" />
      <DA name="q" bType="Quality" fc="MX" qchg="true" />
      <DA name="t" bType="Timestamp" fc="MX" />
    </DOType>
    <DOType id="ASG_1_WRtg" cdc="ASG">
      <DA name="setMag" type="AnalogueValue_1" bType="Struct" fc="SP" />
      <DA name="units" type="Unit_1" bType="Struct" fc="CF" />
    </DOType>
    <DOType id="ENG_1_ACTyp" cdc="ENG">
      <DA name="setVal" bType="INT32" fc="SP" />
    </DOType>
    <DAType id="AnalogueValue_1">
      <BDA name="f" bType="FLOAT32" />
    </DAType>
    <DAType id="Vector_1">
      <BDA name="mag" type="AnalogueValue_1" bType="Struct" />
    </DAType>
    <DAType id="Unit_1">
      <BDA name="SIUnit" type="SIUnit" bType="Enum" />
    </DAType>
    <EnumType id="CtlModels">
      <EnumVal ord="0">status-only</EnumVal>
      <EnumVal ord="1">direct-with-normal-security</EnumVal>
      <EnumVal ord="2">sbo-with-normal-security</EnumVal>
      <EnumVal ord="3">direct-with-enhanced-security</EnumVal>
      <EnumVal ord="4">sbo-with-enhanced-security</EnumVal>
    </EnumType>
    <EnumType id="SIUnit">
      <EnumVal ord="1">
      </EnumVal>
      <EnumVal ord="10">rad</EnumVal>
      <EnumVal ord="11">sr</EnumVal>
      <EnumVal ord="2">m</EnumVal>
      <EnumVal ord="21">Gy</EnumVal>
      <EnumVal ord="22">q</EnumVal>
      <EnumVal ord="23">Ã‚Â°C</EnumVal>
      <EnumVal ord="24">Sv</EnumVal>
      <EnumVal ord="25">F</EnumVal>
      <EnumVal ord="26">C</EnumVal>
      <EnumVal ord="27">S</EnumVal>
      <EnumVal ord="28">H</EnumVal>
      <EnumVal ord="29">V</EnumVal>
      <EnumVal ord="3">kg</EnumVal>
      <EnumVal ord="30">ohm</EnumVal>
      <EnumVal ord="31">J</EnumVal>
      <EnumVal ord="32">N</EnumVal>
      <EnumVal ord="33">Hz</EnumVal>
      <EnumVal ord="34">Ix</EnumVal>
      <EnumVal ord="35">Lm</EnumVal>
      <EnumVal ord="36">Wb</EnumVal>
      <EnumVal ord="37">T</EnumVal>
      <EnumVal ord="38">W</EnumVal>
      <EnumVal ord="39">Pa</EnumVal>
      <EnumVal ord="4">s</EnumVal>
      <EnumVal ord="41">mÃ‚Â²</EnumVal>
      <EnumVal ord="42">mÃ‚Â³</EnumVal>
      <EnumVal ord="43">m/s</EnumVal>
      <EnumVal ord="44">m/sÃ‚Â²</EnumVal>
      <EnumVal ord="45">mÃ‚Â³/s</EnumVal>
      <EnumVal ord="46">m/mÃ‚Â³</EnumVal>
      <EnumVal ord="47">M</EnumVal>
      <EnumVal ord="48">kg/mÃ‚Â³</EnumVal>
      <EnumVal ord="49">mÃ‚Â²/s</EnumVal>
      <EnumVal ord="5">A</EnumVal>
      <EnumVal ord="50">W/m K</EnumVal>
      <EnumVal ord="51">J/K</EnumVal>
      <EnumVal ord="52">ppm</EnumVal>
      <EnumVal ord="53">1/s</EnumVal>
      <EnumVal ord="54">rad/s</EnumVal>
      <EnumVal ord="6">K</EnumVal>
      <EnumVal ord="61">VA</EnumVal>
      <EnumVal ord="62">Watts</EnumVal>
      <EnumVal ord="63">VAr</EnumVal>
      <EnumVal ord="64">theta</EnumVal>
      <EnumVal ord="65">cos(theta)</EnumVal>
      <EnumVal ord="66">Vs</EnumVal>
      <EnumVal ord="67">VÃ‚Â²</EnumVal>
      <EnumVal ord="68">As</EnumVal>
      <EnumVal ord="69">AÃ‚Â²</EnumVal>
      <EnumVal ord="7">mol</EnumVal>
      <EnumVal ord="70">AÃ‚Â²t</EnumVal>
      <EnumVal ord="71">VAh</EnumVal>
      <EnumVal ord="72">Wh</EnumVal>
      <EnumVal ord="73">VArh</EnumVal>
      <EnumVal ord="74">V/Hz</EnumVal>
      <EnumVal ord="8">cd</EnumVal>
      <EnumVal ord="9">deg</EnumVal>
    </EnumType>
    <EnumType id="Multiplier">
      <EnumVal ord="-24">Yocto</EnumVal>
      <EnumVal ord="-21">Zepto</EnumVal>
      <EnumVal ord="-18">Atto</EnumVal>
      <EnumVal ord="-15">Femto</EnumVal>
      <EnumVal ord="-12">Pico</EnumVal>
      <EnumVal ord="-9">Nano</EnumVal>
      <EnumVal ord="-6">Micro</EnumVal>
      <EnumVal ord="-3">Milli</EnumVal>
      <EnumVal ord="-2">Centi</EnumVal>
      <EnumVal ord="-1">Deci</EnumVal>
      <EnumVal ord="0">zeroNoValue</EnumVal>
      <EnumVal ord="1">Deca</EnumVal>
      <EnumVal ord="2">Hecto</EnumVal>
      <EnumVal ord="3">Kilo</EnumVal>
      <EnumVal ord="6">Mega</EnumVal>
      <EnumVal ord="9">Giga</EnumVal>
      <EnumVal ord="12">Tera</EnumVal>
      <EnumVal ord="15">Petra</EnumVal>
      <EnumVal ord="18">Exa</EnumVal>
      <EnumVal ord="21">Zetta</EnumVal>
      <EnumVal ord="24">Yotta</EnumVal>
    </EnumType>
    <EnumType id="Range">
      <EnumVal ord="0">normal</EnumVal>
      <EnumVal ord="1">high</EnumVal>
      <EnumVal ord="2">low</EnumVal>
      <EnumVal ord="3">high-high</EnumVal>
      <EnumVal ord="4">low-low</EnumVal>
    </EnumType>
  </DataTypeTemplates>
</SCL>
