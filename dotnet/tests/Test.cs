using IEC61850.Client;
using IEC61850.Common;
using IEC61850.Server;
using NUnit.Framework;
using System;
using System.Collections.Generic;
using System.Threading;
using System.Xml;

namespace tests
{
	[TestFixture ()]
	public class Test
	{
		[Test ()]
		public void TestCase ()
		{
			var val = new MmsValue (10.0f);

			Assert.That(val.ToFloat(), Is.EqualTo(10.0f));
		}

		[Test ()]
		public void MmsValueBitString ()
		{
			var val = MmsValue.NewBitString(10);

			Assert.That(val.GetType(), Is.EqualTo(MmsType.MMS_BIT_STRING));
			Assert.That(val.Size(), Is.EqualTo(10));

			val.BitStringFromUInt32(7);

			Assert.That(val.BitStringToUInt32(), Is.EqualTo(7));

			Assert.That(val.GetBit(0), Is.True);
			Assert.That(val.GetBit(1), Is.True);
			Assert.That(val.GetBit(2), Is.True);
			Assert.That(val.GetBit(3), Is.False);

			Assert.That(val.GetBit(9), Is.False);

			Assert.That(val.GetBit(10), Is.False);

			val.SetBit(3, true);
			Assert.That(val.GetBit(3), Is.True);

			Assert.That(val.BitStringToUInt32(), Is.EqualTo(15));

			val.SetBit(3, false);
			Assert.That(val.BitStringToUInt32(), Is.EqualTo(7));
		}

		[Test ()]
		public void MmsValueUtcTime ()
		{
			var val = MmsValue.NewUtcTime (100000);
			val.GetUtcTimeInMs ();

			Assert.That(val.GetUtcTimeInMs(), Is.EqualTo(100000));
		}

		[Test()]
		public void MmsValueOctetString ()
		{
			var val = MmsValue.NewOctetString(20);

			Assert.That(val.Size(), Is.EqualTo(0));
			Assert.That(val.MaxSize(), Is.EqualTo(20));

			byte[] octetString = val.getOctetString();

			Assert.That(octetString.Length, Is.EqualTo(0));

			octetString = new byte[5];
			octetString[0] = 0x11;
			octetString[1] = 0x12;
			octetString[2] = 0x13;
			octetString[3] = 0x14;
			octetString[4] = 0x15;

			val.setOctetString(octetString);

			Assert.That(val.Size(), Is.EqualTo(5));

			byte[] secondOctetString = val.getOctetString();

			Assert.That(secondOctetString, Is.EqualTo(octetString));
		}

		[Test ()]
		public void MmsValueFloat() 
		{
			var val = new MmsValue ((float) 1234.5678);

			Assert.That(val.ToFloat(), Is.EqualTo((float)1234.5678));

			val.SetFloat ((float) 0.1234);

			Assert.That(val.ToFloat(), Is.EqualTo((float)0.1234));

			Assert.That(val.ToDouble(), Is.EqualTo((double)0.1234).Within((double)0.0001));
		}

		[Test ()]
		public void MmsValueDouble() 
		{
			var val = new MmsValue ((double) 1234.5678);

			Assert.That(val.ToDouble(), Is.EqualTo((double)1234.5678));

			val.SetDouble ((double) 0.1234);

			Assert.That(val.ToDouble(), Is.EqualTo((double)0.1234));

			Assert.That(val.ToFloat(), Is.EqualTo((float)0.1234));
		}

		[Test()]
		public void MmsValueArray()
		{
			MmsValue val = MmsValue.NewEmptyArray (3);

			val.SetElement (0, new MmsValue (1));
			val.SetElement (1, new MmsValue (2));
			val.SetElement (2, new MmsValue (3));

			Assert.That(val.GetType(), Is.EqualTo(MmsType.MMS_ARRAY));
			Assert.That(val.Size(), Is.EqualTo(3));

			MmsValue elem0 = val.GetElement (0);

			Assert.That(elem0.GetType(), Is.EqualTo(MmsType.MMS_INTEGER));
			Assert.That(elem0.ToInt32(), Is.EqualTo(1));

			MmsValue elem2 = val.GetElement (2);

			Assert.That(elem2.GetType(), Is.EqualTo(MmsType.MMS_INTEGER));
			Assert.That(elem2.ToInt32(), Is.EqualTo(3));

			val.SetElement (0, null);
			val.SetElement (1, null);
			val.SetElement (2, null);
		}

		[Test()]
		public void MmsValueStructure()
		{
			MmsValue val = MmsValue.NewEmptyStructure (2);

			val.SetElement (0, new MmsValue(true));
			val.SetElement (1, MmsValue.NewBitString (10));

			Assert.That(val.GetType(), Is.EqualTo(MmsType.MMS_STRUCTURE));
			Assert.That(val.Size(), Is.EqualTo(2));

			MmsValue elem0 = val.GetElement (0);

			Assert.That(elem0.GetType(), Is.EqualTo(MmsType.MMS_BOOLEAN));
			Assert.That(elem0.GetBoolean(), Is.True);

			MmsValue elem1 = val.GetElement (1);

			Assert.That(elem1.GetType(), Is.EqualTo(MmsType.MMS_BIT_STRING));

			val.SetElement (0, null);
			val.SetElement (1, null);
		}

		[Test ()]
		public void Timestamps()
		{
			Timestamp timestamp = new Timestamp ();

			Assert.That(timestamp.LeapSecondKnown, Is.True);
			Assert.That(timestamp.ClockFailure, Is.False);
			Assert.That(timestamp.ClockNotSynchronized, Is.False);

			timestamp.LeapSecondKnown = false;
			Assert.That(timestamp.LeapSecondKnown, Is.False);

			timestamp.ClockFailure = true;
			Assert.That(timestamp.ClockFailure, Is.True);

			timestamp.ClockNotSynchronized = true;
			Assert.That(timestamp.ClockNotSynchronized, Is.True);

			Assert.That(timestamp.SubsecondPrecision, Is.EqualTo(0));

			timestamp.SubsecondPrecision = 10;
			Assert.That(timestamp.SubsecondPrecision, Is.EqualTo(10));
		}

		[Test ()]
		public void CreateModelFromNonExistingFile()
		{
			IedModel iedModel = ConfigFileParser.CreateModelFromConfigFile ("test.cfg");

			Assert.That(iedModel, Is.Null);
		}

		[Test ()]
		public void CreateModelFromFile()
		{			
			IedModel iedModel = ConfigFileParser.CreateModelFromConfigFile ("../../../model.cfg");

			Assert.That(iedModel, Is.Not.Null);
		}

		[Test ()]
		public void StartStopSimpleServer()
		{	
			IedModel iedModel = ConfigFileParser.CreateModelFromConfigFile ("../../../model.cfg");

			IedServer iedServer = new IedServer (iedModel);

			Assert.That(iedServer, Is.Not.Null);

			iedServer.Start (10002);

			Assert.That(iedServer.IsRunning(), Is.True);

			iedServer.Stop ();

			Assert.That(iedServer.IsRunning(), Is.False);

			iedServer.Destroy ();
		}

		[Test ()]
		public void ConnectToServer()
		{	
			IedModel iedModel = ConfigFileParser.CreateModelFromConfigFile ("../../../model.cfg");

			IedServer iedServer = new IedServer (iedModel);

			iedServer.Start (10002);

			IedConnection connection = new IedConnection ();

			connection.Connect ("localhost", 10002);

			List<string> list = connection.GetServerDirectory ();

			Assert.That(list, Is.Not.Empty);

			Assert.That(list.ToArray()[0], Is.EqualTo("simpleIOGenericIO"));

			Assert.That(iedServer.IsRunning(), Is.True);

			iedServer.Stop ();

			iedServer.Destroy ();
		}

		[Test ()]
		public void ReadNonExistingObject()
		{	
			IedModel iedModel = ConfigFileParser.CreateModelFromConfigFile ("../../../model.cfg");

			IedServer iedServer = new IedServer (iedModel);

			iedServer.Start (10002);

			IedConnection connection = new IedConnection ();

			connection.Connect ("localhost", 10002);

			MmsValue value = connection.ReadValue ("simpleIOGenericIO/GGIO1.SPCSO1.stVal", FunctionalConstraint.MX);

			Assert.That(value, Is.Not.Null);

			Assert.That(value.GetType(), Is.EqualTo(MmsType.MMS_DATA_ACCESS_ERROR));

			iedServer.Stop ();

			iedServer.Destroy ();
		}

		[Test ()]
		public void AccessDataModelServerSide()
		{
			IedModel iedModel = ConfigFileParser.CreateModelFromConfigFile ("../../../model.cfg");

			ModelNode modelNode = iedModel.GetModelNodeByShortObjectReference ("GenericIO/GGIO1.AnIn1");

			Assert.That(modelNode.GetType().Equals(typeof(DataObject)), Is.True);

			modelNode = iedModel.GetModelNodeByShortObjectReference ("GenericIO/GGIO1.AnIn1.mag.f");

			Assert.That(modelNode.GetType().Equals(typeof(IEC61850.Server.DataAttribute)), Is.True);

			Assert.That(modelNode, Is.Not.Null);
		}

		[Test()]
		public void AccessDataModelServerSideNavigateModelNode()
		{
			IedModel iedModel = ConfigFileParser.CreateModelFromConfigFile("../../../model.cfg");

			ModelNode modelNode = iedModel.GetModelNodeByShortObjectReference("GenericIO/GGIO1.AnIn1");

			Assert.That(modelNode, Is.Not.Null);

			Assert.That(modelNode.GetType().Equals(typeof(DataObject)), Is.True);

			var children = modelNode.GetChildren();

			Assert.That(children.Count, Is.EqualTo(3));

			ModelNode mag = children.First.Value;

			Assert.That(mag.GetName(), Is.EqualTo("mag"));

			ModelNode t = children.Last.Value;

			Assert.That(t.GetName(), Is.EqualTo("t"));

			//modelNode = iedModel.GetModelNodeByShortObjectReference("GenericIO/GGIO1.AnIn1.mag.f");

			//Assert.That(modelNode.GetType().Equals(typeof(IEC61850.Server.DataAttribute)), Is.True);
		}

		[Test ()]
		public void AccessDataModelClientServer()
		{
			IedModel iedModel = ConfigFileParser.CreateModelFromConfigFile("../../../model.cfg");

			ModelNode ind1 = iedModel.GetModelNodeByShortObjectReference ("GenericIO/GGIO1.Ind1.stVal");

			Assert.That(ind1.GetType().Equals(typeof(IEC61850.Server.DataAttribute)), Is.True);

			IedServer iedServer = new IedServer (iedModel);

			iedServer.Start (10002);

			iedServer.UpdateBooleanAttributeValue((IEC61850.Server.DataAttribute) ind1, true);

			IedConnection connection = new IedConnection ();

			connection.Connect ("localhost", 10002);

			bool stVal = connection.ReadBooleanValue ("simpleIOGenericIO/GGIO1.Ind1.stVal", FunctionalConstraint.ST);

			Assert.That(stVal, Is.True);

			iedServer.UpdateBooleanAttributeValue((IEC61850.Server.DataAttribute) ind1, false);

			stVal = connection.ReadBooleanValue ("simpleIOGenericIO/GGIO1.Ind1.stVal", FunctionalConstraint.ST);

			Assert.That(stVal, Is.False);

			connection.Abort ();

			iedServer.Stop ();

			iedServer.Destroy ();
		}
			
	
		[Test()]
		public void ControlWriteAccessToServer()
		{

			IedModel iedModel = ConfigFileParser.CreateModelFromConfigFile ("../../../model.cfg");

			IEC61850.Server.DataAttribute opDlTmms = (IEC61850.Server.DataAttribute) iedModel.GetModelNodeByShortObjectReference("GenericIO/PDUP1.OpDlTmms.setVal");
			IEC61850.Server.DataAttribute rsDlTmms = (IEC61850.Server.DataAttribute)iedModel.GetModelNodeByShortObjectReference ("GenericIO/PDUP1.RsDlTmms.setVal");

			IedServer iedServer = new IedServer (iedModel);

			int opDlTmmsValue = 0;

			iedServer.HandleWriteAccess (opDlTmms, delegate(IEC61850.Server.DataAttribute dataAttr, MmsValue value, ClientConnection con, object parameter) {
				opDlTmmsValue = value.ToInt32();
				return MmsDataAccessError.SUCCESS;
			}, null);

			iedServer.HandleWriteAccess (rsDlTmms, delegate(IEC61850.Server.DataAttribute dataAttr, MmsValue value, ClientConnection con, object parameter) {
				if (value.ToInt32() > 1000)
					return MmsDataAccessError.OBJECT_VALUE_INVALID;
				else
					return MmsDataAccessError.SUCCESS;
			}, null);

			iedServer.Start (10002);

			IedConnection connection = new IedConnection ();

			connection.Connect ("localhost", 10002);

			connection.WriteValue ("simpleIOGenericIO/PDUP1.OpDlTmms.setVal", FunctionalConstraint.SP, new MmsValue ((int)1234));


			try {
				connection.WriteValue ("simpleIOGenericIO/PDUP1.RsDlTmms.setVal", FunctionalConstraint.SP, new MmsValue ((int)1234));
			}
			catch (IedConnectionException e) {
				Assert.That(e.GetIedClientError(), Is.EqualTo(IedClientError.IED_ERROR_OBJECT_VALUE_INVALID));
			}

			connection.WriteValue ("simpleIOGenericIO/PDUP1.RsDlTmms.setVal", FunctionalConstraint.SP, new MmsValue ((int)999));

			MmsValue rsDlTmmsValue = iedServer.GetAttributeValue (rsDlTmms);

			Assert.That(rsDlTmmsValue.ToInt32(), Is.EqualTo(999));

			connection.Abort ();

			iedServer.Stop ();

			Assert.That(opDlTmmsValue, Is.EqualTo((int)1234));

			iedServer.Destroy ();
		}

        [Test()]
        public void ControlWriteAccessComplexDAToServer()
        {
            IedModel iedModel = ConfigFileParser.CreateModelFromConfigFile("../../../model2.cfg");

            IEC61850.Server.DataAttribute setAnVal_setMag = (IEC61850.Server.DataAttribute)iedModel.GetModelNodeByShortObjectReference("GenericIO/LLN0.SetAnVal.setMag");

            IedServer iedServer = new IedServer(iedModel);

            int handlerCalled = 0;

            MmsValue receivedValue = null;

            iedServer.SetWriteAccessPolicy(FunctionalConstraint.SP, AccessPolicy.ACCESS_POLICY_DENY);

            iedServer.HandleWriteAccessForComplexAttribute(setAnVal_setMag, delegate (IEC61850.Server.DataAttribute dataAttr, MmsValue value, ClientConnection con, object parameter) {
                receivedValue = value;
                handlerCalled++;
                return MmsDataAccessError.SUCCESS;
            }, null);

            iedServer.Start(10002);

            IedConnection connection = new IedConnection();

            connection.Connect("localhost", 10002);

            MmsValue complexValue = MmsValue.NewEmptyStructure(1);
            complexValue.SetElement(0, new MmsValue((float)1.0));

			connection.WriteValue("simpleIOGenericIO/LLN0.SetAnVal.setMag", FunctionalConstraint.SP, complexValue);

			Assert.That(receivedValue, Is.Not.Null);
			Assert.That(receivedValue.GetType(), Is.EqualTo(MmsType.MMS_STRUCTURE));
			Assert.That(receivedValue.GetElement(0).ToFloat(), Is.EqualTo(1.0));

			receivedValue.Dispose();

			receivedValue = null;

			connection.WriteValue("simpleIOGenericIO/LLN0.SetAnVal.setMag.f", FunctionalConstraint.SP, new MmsValue((float)2.0));

			Assert.That(receivedValue, Is.Not.Null);
			Assert.That(receivedValue.GetType(), Is.EqualTo(MmsType.MMS_FLOAT));
			Assert.That(receivedValue.ToFloat(), Is.EqualTo(2.0));

            connection.Abort();

            iedServer.Stop();

            iedServer.Dispose();
        }

        [Test()]
		public void WriteAccessPolicy()
		{
			IedModel iedModel = ConfigFileParser.CreateModelFromConfigFile ("../../../model.cfg");

			IEC61850.Server.DataAttribute opDlTmms = (IEC61850.Server.DataAttribute) iedModel.GetModelNodeByShortObjectReference("GenericIO/PDUP1.OpDlTmms.setVal");
            IEC61850.Server.DataAttribute rsDlTmms = (IEC61850.Server.DataAttribute)iedModel.GetModelNodeByShortObjectReference("GenericIO/PDUP1.RsDlTmms.setVal");

			IedServer iedServer = new IedServer (iedModel);

            iedServer.HandleWriteAccess (opDlTmms, delegate(IEC61850.Server.DataAttribute dataAttr, MmsValue value, ClientConnection con, object parameter) {
				return MmsDataAccessError.SUCCESS;
			}, null);
				

			iedServer.Start (10002);

			IedConnection connection = new IedConnection ();

			connection.Connect ("localhost", 10002);

            iedServer.SetWriteAccessPolicy(FunctionalConstraint.SP, AccessPolicy.ACCESS_POLICY_ALLOW);

            connection.WriteValue ("simpleIOGenericIO/PDUP1.RsDlTmms.setVal", FunctionalConstraint.SP, new MmsValue ((int)1234));

			iedServer.SetWriteAccessPolicy (FunctionalConstraint.SP, AccessPolicy.ACCESS_POLICY_DENY);

			connection.WriteValue ("simpleIOGenericIO/PDUP1.OpDlTmms.setVal", FunctionalConstraint.SP, new MmsValue ((int)1234));

			try {
				connection.WriteValue ("simpleIOGenericIO/PDUP1.RsDlTmms.setVal", FunctionalConstraint.SP, new MmsValue ((int)999));
			}
			catch (IedConnectionException e) {
				Assert.That(e.GetIedClientError(), Is.EqualTo(IedClientError.IED_ERROR_ACCESS_DENIED));
			}

			MmsValue rsDlTmmsValue = iedServer.GetAttributeValue (rsDlTmms);

			Assert.That(rsDlTmmsValue.ToInt32(), Is.EqualTo(1234));

			connection.Abort ();

			iedServer.Stop ();

			iedServer.Dispose();
		}

		[Test()]
		public void ControlHandler()
		{
			IedModel iedModel = ConfigFileParser.CreateModelFromConfigFile ("../../../model.cfg");

			DataObject spcso1 = (DataObject)iedModel.GetModelNodeByShortObjectReference ("GenericIO/GGIO1.SPCSO1");

			Assert.That(spcso1, Is.Not.Null);

			int handlerCalled = 0;

			IedServer iedServer = new IedServer (iedModel);

			iedServer.SetControlHandler (spcso1, delegate(ControlAction action, object parameter, MmsValue ctlVal, bool test) {

				byte [] orIdent = action.GetOrIdent ();

				string orIdentStr = System.Text.Encoding.UTF8.GetString (orIdent, 0, orIdent.Length);

				Assert.That(orIdentStr, Is.EqualTo("TEST1234"));
				Assert.That(action.GetOrCat(), Is.EqualTo(OrCat.MAINTENANCE));

				Assert.That(action.GetControlObject(), Is.SameAs(spcso1));

				handlerCalled++;
				return ControlHandlerResult.OK;
			}, null);

			iedServer.Start (10002);

			IedConnection connection = new IedConnection ();

			connection.Connect ("localhost", 10002);

			ControlObject controlClient = connection.CreateControlObject ("simpleIOGenericIO/GGIO1.SPCSO1");
			controlClient.SetOrigin ("TEST1234", OrCat.MAINTENANCE);

			Assert.That(controlClient, Is.Not.Null);

			controlClient.Operate (true);

			connection.Abort ();

			Assert.That(handlerCalled, Is.EqualTo(1));

			iedServer.Stop ();

			iedServer.Dispose();
		}


		[Test()]
		public void ConnectionHandler()
		{
			IedModel iedModel = ConfigFileParser.CreateModelFromConfigFile ("../../../model.cfg");

			int handlerCalled = 0;
			int connectionCount = 0;

			IedServer iedServer = new IedServer (iedModel);

			string ipAddress = null;

			iedServer.SetConnectionIndicationHandler(delegate(IedServer server, ClientConnection clientConnection, bool connected, object parameter) {
				handlerCalled++;
				if (connected)
					connectionCount++;
				else
					connectionCount--;

				ipAddress = clientConnection.GetPeerAddress();
			}, null);

			iedServer.Start (10002);

			IedConnection con1 = new IedConnection ();
			con1.Connect ("localhost", 10002);

			Assert.That(handlerCalled, Is.EqualTo(1));
			Assert.That(connectionCount, Is.EqualTo(1));

			IedConnection con2 = new IedConnection ();
			con2.Connect ("localhost", 10002);

			Assert.That(handlerCalled, Is.EqualTo(2));
			Assert.That(connectionCount, Is.EqualTo(2));

			con1.Abort ();
			con2.Abort ();

			Assert.That(handlerCalled, Is.EqualTo(4));
			Assert.That(connectionCount, Is.EqualTo(0));

			Assert.That(ipAddress.Substring(0, 10), Is.EqualTo("127.0.0.1:"));

			iedServer.Stop ();

            iedServer.Dispose();
		}

		[Test()]
		public void Quality()
		{
			Quality q = new Quality ();

			Assert.That(q.Overflow, Is.False);

			q.Overflow = true;

			Assert.That(q.Overflow, Is.True);

			q.Overflow = false;

			Assert.That(q.Overflow, Is.False);

			Assert.That(q.Validity, Is.EqualTo(Validity.GOOD));

			q.Substituted = true;

			Assert.That(q.Substituted, Is.True);
			Assert.That(q.Overflow, Is.False);

			q.Validity = Validity.QUESTIONABLE;

			Assert.That(q.Validity, Is.EqualTo(Validity.QUESTIONABLE));
		}

        [Test()]
        public void MmsValueCreateStructureAndAddElement()
        {
            MmsValue structure1 = MmsValue.NewEmptyStructure(1);
            MmsValue structure2 = MmsValue.NewEmptyStructure(1);
            MmsValue element = MmsValue.NewEmptyStructure(1);

            structure1.SetElement(0, element);

            /* Clone is required when adding the value to another structure or element */
            MmsValue elementClone = element.Clone();
            structure2.SetElement(0, elementClone);

            element.Dispose();

			structure1.Dispose();
			structure2.Dispose();

			Assert.That(true, Is.True);
		}

        [Test()]
        public void MmsValueClone()
        {
            MmsValue boolValue = new MmsValue(true);

            MmsValue boolClone = boolValue.Clone();

            boolValue.Dispose();
            boolClone.Dispose();

            MmsValue structure = MmsValue.NewEmptyStructure(1);
            MmsValue structureClone = structure.Clone();

            structure.Dispose();
            structureClone.Dispose();
        }

        [Test]
        public void MmsValue_NoMemoryLeak_WithWeakReference()
        {
            WeakReference wr_stringVal = CreateStringMmsValue();
            WeakReference wr_structure = CreateStructureMmsValue();

            GC.Collect();
            GC.WaitForPendingFinalizers();
            GC.Collect();

            Assert.That(wr_stringVal.IsAlive, Is.False, "String MmsValue was not collected");
            Assert.That(wr_structure.IsAlive, Is.False, "Structure MmsValue was not collected");
        }

        private WeakReference CreateStringMmsValue()
        {
            var value = MmsValue.NewVisibleString(10);

            var wr = new WeakReference(value);

            value.Dispose();

            return wr;
        }

        private WeakReference CreateStructureMmsValue()
        {
            var structure = MmsValue.NewEmptyStructure(1);

            var wr = new WeakReference(structure);

            structure.Dispose();

            return wr;
        }

        [Test()]
		public void UpdateValuesOnServer()
		{
            IedModel iedModel = ConfigFileParser.CreateModelFromConfigFile("../../../new_types_test.cfg");
            IedServer iedServer = new IedServer(iedModel);
            iedServer.Start(10002);

            //Testing Dbpos attribute update
            DataAttribute dbposAttr = (DataAttribute)iedModel.GetModelNodeByShortObjectReference("GenericIO/LPHD1.PhyNam.NewAttributeType");

            MmsValue initial_val = iedServer.GetAttributeValue(dbposAttr);
            Assert.That(initial_val.BitStringToUInt32(), Is.EqualTo((uint)Dbpos.DBPOS_INTERMEDIATE_STATE));

            Dbpos dbpos = Dbpos.DBPOS_BAD_STATE;
            iedServer.UpdateDbposAttributeValue(dbposAttr, dbpos);
			MmsValue final_val = iedServer.GetAttributeValue(dbposAttr);
            Assert.That(final_val.BitStringToUInt32(), Is.EqualTo((uint)Dbpos.DBPOS_BAD_STATE));

            //Testing unsigned attribute update
            DataAttribute unsignedAttr = (DataAttribute)iedModel.GetModelNodeByShortObjectReference("GenericIO/LPHD1.PhyNam.NewAttributeType2");

			MmsValue unsignedInitialVal = iedServer.GetAttributeValue(unsignedAttr);
			Assert.That(unsignedInitialVal.ToUint32(), Is.EqualTo(0));

			iedServer.UpdateUnsignedAttributeValue(unsignedAttr, 1234);
			MmsValue unsignedFinalVal = iedServer.GetAttributeValue(unsignedAttr);
			Assert.That(unsignedFinalVal.ToUint32(), Is.EqualTo(1234));

            //Testing bitstring attribute update
            DataAttribute bitstringAttr = (DataAttribute)iedModel.GetModelNodeByShortObjectReference("GenericIO/LPHD1.Proxy.q");
            MmsValue newbs = MmsValue.NewBitString(13);
            newbs.BitStringFromUInt32(362);

            MmsValue bitstringInitialValue = iedServer.GetAttributeValue(bitstringAttr);
			Assert.That(bitstringInitialValue.BitStringToUInt32(), Is.EqualTo(0));

            iedServer.UpdateBitStringAttributeValue(bitstringAttr, newbs.BitStringToUInt32());
			MmsValue bitstringFinalValue = iedServer.GetAttributeValue(bitstringAttr);
			Assert.That(bitstringFinalValue.BitStringToUInt32(), Is.EqualTo(362));

            iedServer.Stop();
			iedServer.Dispose();
        }
    }
}

