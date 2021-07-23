using NUnit.Framework;
using System;
using IEC61850.Common;
using IEC61850.Client;
using IEC61850.Server;
using System.Collections.Generic;
using System.Threading;

namespace tests
{
	[TestFixture ()]
	public class Test
	{
		[Test ()]
		public void TestCase ()
		{
			var val = new MmsValue (10.0f);

			Assert.AreEqual (10.0f, val.ToFloat ());
		}

		[Test ()]
		public void MmsValueBitString ()
		{
			var val = MmsValue.NewBitString(10);

			Assert.AreEqual (MmsType.MMS_BIT_STRING, val.GetType());
			Assert.AreEqual (10, val.Size());

			val.BitStringFromUInt32(7);

			Assert.AreEqual(7, val.BitStringToUInt32());

			Assert.AreEqual(true, val.GetBit(0));
			Assert.AreEqual(true, val.GetBit(1));
			Assert.AreEqual(true, val.GetBit(2));
			Assert.AreEqual(false, val.GetBit(3));

			Assert.AreEqual(false, val.GetBit(9));

			Assert.AreEqual(false, val.GetBit(10));

			val.SetBit(3, true);
			Assert.AreEqual(true, val.GetBit(3));

			Assert.AreEqual(15, val.BitStringToUInt32());

			val.SetBit(3, false);
			Assert.AreEqual(7, val.BitStringToUInt32());
		}

        [Test ()]
        public void MmsValueUtcTime ()
        {
            var val = MmsValue.NewUtcTime (100000);
            val.GetUtcTimeInMs ();

            Assert.AreEqual (val.GetUtcTimeInMs (), 100000);
        }

		[Test()]
		public void MmsValueOctetString ()
		{
			var val = MmsValue.NewOctetString(20);

			Assert.AreEqual (0, val.Size());
			Assert.AreEqual (20, val.MaxSize());

			byte[] octetString = val.getOctetString();

			Assert.AreEqual (0, octetString.Length);

			octetString = new byte[5];
			octetString[0] = 0x11;
			octetString[1] = 0x12;
			octetString[2] = 0x13;
			octetString[3] = 0x14;
			octetString[4] = 0x15;

			val.setOctetString(octetString);

			Assert.AreEqual(5, val.Size());

			byte[] secondOctetString = val.getOctetString();

			Assert.AreEqual(octetString, secondOctetString);
		}

		[Test ()]
		public void MmsValueFloat() 
		{
			var val = new MmsValue ((float) 1234.5678);

			Assert.AreEqual (val.ToFloat (), (float)1234.5678);

			val.SetFloat ((float) 0.1234);

			Assert.AreEqual (val.ToFloat (), (float) 0.1234);

			Assert.AreEqual (val.ToDouble (), (double) 0.1234, (double) 0.0001);
		}

		[Test ()]
		public void MmsValueDouble() 
		{
			var val = new MmsValue ((double) 1234.5678);

			Assert.AreEqual (val.ToDouble (), (double)1234.5678);

			val.SetDouble ((double) 0.1234);

			Assert.AreEqual (val.ToDouble (), (double) 0.1234);

			Assert.AreEqual (val.ToFloat (), (float)0.1234);
		}

		[Test()]
		public void MmsValueArray()
		{
			MmsValue val = MmsValue.NewEmptyArray (3);

			val.SetElement (0, new MmsValue (1));
			val.SetElement (1, new MmsValue (2));
			val.SetElement (2, new MmsValue (3));

			Assert.AreEqual (val.GetType (), MmsType.MMS_ARRAY);
			Assert.AreEqual (val.Size (), 3);

			MmsValue elem0 = val.GetElement (0);

			Assert.AreEqual (elem0.GetType (), MmsType.MMS_INTEGER);
			Assert.AreEqual (elem0.ToInt32 (), 1);

			MmsValue elem2 = val.GetElement (2);

			Assert.AreEqual (elem2.GetType (), MmsType.MMS_INTEGER);
			Assert.AreEqual (elem2.ToInt32 (), 3);

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

			Assert.AreEqual (val.GetType (), MmsType.MMS_STRUCTURE);
			Assert.AreEqual (val.Size (), 2);

			MmsValue elem0 = val.GetElement (0);

			Assert.AreEqual (elem0.GetType (), MmsType.MMS_BOOLEAN);
			Assert.AreEqual (elem0.GetBoolean(), true);

			MmsValue elem1 = val.GetElement (1);

			Assert.AreEqual (elem1.GetType (), MmsType.MMS_BIT_STRING);

            val.SetElement (0, null);
            val.SetElement (1, null);
		}

		[Test ()]
		public void Timestamps()
		{
			Timestamp timestamp = new Timestamp ();

			Assert.IsTrue (timestamp.LeapSecondKnown);
			Assert.IsFalse (timestamp.ClockFailure);
			Assert.IsFalse (timestamp.ClockNotSynchronized);

			timestamp.LeapSecondKnown = false;
			Assert.IsFalse (timestamp.LeapSecondKnown);

			timestamp.ClockFailure = true;
			Assert.IsTrue (timestamp.ClockFailure);

			timestamp.ClockNotSynchronized = true;
			Assert.IsTrue (timestamp.ClockNotSynchronized);

			Assert.AreEqual (0, timestamp.SubsecondPrecision);

			timestamp.SubsecondPrecision = 10;
			Assert.AreEqual (10, timestamp.SubsecondPrecision);
		}

		[Test ()]
		public void CreateModelFromNonExistingFile()
		{
			IedModel iedModel = ConfigFileParser.CreateModelFromConfigFile ("test.cfg");

			Assert.IsNull (iedModel);
		}

		[Test ()]
		public void CreateModelFromFile()
		{			
			IedModel iedModel = ConfigFileParser.CreateModelFromConfigFile ("../../model.cfg");

			Assert.NotNull (iedModel);
		}

		[Test ()]
		public void StartStopSimpleServer()
		{	
			IedModel iedModel = ConfigFileParser.CreateModelFromConfigFile ("../../model.cfg");

			IedServer iedServer = new IedServer (iedModel);

			Assert.NotNull (iedServer);

			iedServer.Start (10002);

			Assert.IsTrue (iedServer.IsRunning ());

			iedServer.Stop ();

			Assert.IsFalse (iedServer.IsRunning ());

			iedServer.Destroy ();
		}

		[Test ()]
		public void ConnectToServer()
		{	
			IedModel iedModel = ConfigFileParser.CreateModelFromConfigFile ("../../model.cfg");

			IedServer iedServer = new IedServer (iedModel);

			iedServer.Start (10002);

			IedConnection connection = new IedConnection ();

			connection.Connect ("localhost", 10002);

			List<string> list = connection.GetServerDirectory ();

			Assert.IsNotEmpty (list);

			Assert.AreEqual (list.ToArray () [0], "simpleIOGenericIO");

            Assert.IsTrue(iedServer.IsRunning());

            iedServer.Stop ();

			iedServer.Destroy ();
		}

		[Test ()]
		public void ReadNonExistingObject()
		{	
			IedModel iedModel = ConfigFileParser.CreateModelFromConfigFile ("../../model.cfg");

			IedServer iedServer = new IedServer (iedModel);

			iedServer.Start (10002);

			IedConnection connection = new IedConnection ();

			connection.Connect ("localhost", 10002);

			MmsValue value = connection.ReadValue ("simpleIOGenericIO/GGIO1.SPCSO1.stVal", FunctionalConstraint.MX);

			Assert.IsNotNull (value);

			Assert.AreEqual (MmsType.MMS_DATA_ACCESS_ERROR, value.GetType ());

			iedServer.Stop ();

			iedServer.Destroy ();
		}

		[Test ()]
		public void AccessDataModelServerSide()
		{
			IedModel iedModel = ConfigFileParser.CreateModelFromConfigFile ("../../model.cfg");

			ModelNode modelNode = iedModel.GetModelNodeByShortObjectReference ("GenericIO/GGIO1.AnIn1");

			Assert.IsTrue (modelNode.GetType ().Equals (typeof(DataObject)));

			modelNode = iedModel.GetModelNodeByShortObjectReference ("GenericIO/GGIO1.AnIn1.mag.f");

			Assert.IsTrue (modelNode.GetType ().Equals (typeof(IEC61850.Server.DataAttribute)));

			Assert.IsNotNull (modelNode);
		}

        [Test()]
        public void AccessDataModelServerSideNavigateModelNode()
        {
            IedModel iedModel = ConfigFileParser.CreateModelFromConfigFile("../../model.cfg");

            ModelNode modelNode = iedModel.GetModelNodeByShortObjectReference("GenericIO/GGIO1.AnIn1");

            Assert.IsNotNull(modelNode);

            Assert.IsTrue(modelNode.GetType().Equals(typeof(DataObject)));

            var children = modelNode.GetChildren();

            Assert.AreEqual(3, children.Count);

            ModelNode mag = children.First.Value;

            Assert.AreEqual("mag", mag.GetName());

            ModelNode t = children.Last.Value;

            Assert.AreEqual("t", t.GetName());

            //modelNode = iedModel.GetModelNodeByShortObjectReference("GenericIO/GGIO1.AnIn1.mag.f");

            //Assert.IsTrue(modelNode.GetType().Equals(typeof(IEC61850.Server.DataAttribute)));
        }

        [Test ()]
		public void AccessDataModelClientServer()
		{
            IedModel iedModel = ConfigFileParser.CreateModelFromConfigFile("../../model.cfg");

			ModelNode ind1 = iedModel.GetModelNodeByShortObjectReference ("GenericIO/GGIO1.Ind1.stVal");

			Assert.IsTrue (ind1.GetType ().Equals (typeof(IEC61850.Server.DataAttribute)));

			IedServer iedServer = new IedServer (iedModel);

			iedServer.Start (10002);

			iedServer.UpdateBooleanAttributeValue((IEC61850.Server.DataAttribute) ind1, true);

			IedConnection connection = new IedConnection ();

			connection.Connect ("localhost", 10002);

			bool stVal = connection.ReadBooleanValue ("simpleIOGenericIO/GGIO1.Ind1.stVal", FunctionalConstraint.ST);

			Assert.IsTrue (stVal);

			iedServer.UpdateBooleanAttributeValue((IEC61850.Server.DataAttribute) ind1, false);

			stVal = connection.ReadBooleanValue ("simpleIOGenericIO/GGIO1.Ind1.stVal", FunctionalConstraint.ST);

			Assert.IsFalse (stVal);

			connection.Abort ();

			iedServer.Stop ();

			iedServer.Destroy ();
		}
			
	
		[Test()]
		public void ControlWriteAccessToServer()
		{

			IedModel iedModel = ConfigFileParser.CreateModelFromConfigFile ("../../model.cfg");

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
				Assert.AreEqual (IedClientError.IED_ERROR_OBJECT_VALUE_INVALID, e.GetIedClientError());
			}

			connection.WriteValue ("simpleIOGenericIO/PDUP1.RsDlTmms.setVal", FunctionalConstraint.SP, new MmsValue ((int)999));

			MmsValue rsDlTmmsValue = iedServer.GetAttributeValue (rsDlTmms);

			Assert.AreEqual (999, rsDlTmmsValue.ToInt32 ());

			connection.Abort ();

			iedServer.Stop ();

			Assert.AreEqual ((int) 1234, opDlTmmsValue);

			iedServer.Destroy ();
		}

        [Test()]
        public void ControlWriteAccessComplexDAToServer()
        {
            IedModel iedModel = ConfigFileParser.CreateModelFromConfigFile("../../model2.cfg");

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

            Assert.NotNull(receivedValue);
            Assert.AreEqual(MmsType.MMS_STRUCTURE, receivedValue.GetType());
            Assert.AreEqual(1.0, receivedValue.GetElement(0).ToFloat());

            receivedValue.Dispose();

            receivedValue = null;

            connection.WriteValue("simpleIOGenericIO/LLN0.SetAnVal.setMag.f", FunctionalConstraint.SP, new MmsValue((float)2.0));

            Assert.NotNull(receivedValue);
            Assert.AreEqual(MmsType.MMS_FLOAT, receivedValue.GetType());
            Assert.AreEqual(2.0, receivedValue.ToFloat());

            connection.Abort();

            iedServer.Stop();

            iedServer.Dispose();
        }

        [Test()]
		public void WriteAccessPolicy()
		{
			IedModel iedModel = ConfigFileParser.CreateModelFromConfigFile ("../../model.cfg");

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
				Assert.AreEqual (IedClientError.IED_ERROR_ACCESS_DENIED, e.GetIedClientError());
			}

			MmsValue rsDlTmmsValue = iedServer.GetAttributeValue (rsDlTmms);

			Assert.AreEqual (1234, rsDlTmmsValue.ToInt32 ());

			connection.Abort ();

			iedServer.Stop ();

			iedServer.Dispose();
		}

		[Test()]
		public void ControlHandler()
		{
			IedModel iedModel = ConfigFileParser.CreateModelFromConfigFile ("../../model.cfg");

			DataObject spcso1 = (DataObject)iedModel.GetModelNodeByShortObjectReference ("GenericIO/GGIO1.SPCSO1");

			Assert.IsNotNull (spcso1);

			int handlerCalled = 0;

			IedServer iedServer = new IedServer (iedModel);

			iedServer.SetControlHandler (spcso1, delegate(ControlAction action, object parameter, MmsValue ctlVal, bool test) {

                byte [] orIdent = action.GetOrIdent ();

                string orIdentStr = System.Text.Encoding.UTF8.GetString (orIdent, 0, orIdent.Length);

                Assert.AreEqual ("TEST1234", orIdentStr);
                Assert.AreEqual (OrCat.MAINTENANCE, action.GetOrCat ());

                Assert.AreSame (spcso1, action.GetControlObject ());

				handlerCalled++;
				return ControlHandlerResult.OK;
			}, null);

			iedServer.Start (10002);

			IedConnection connection = new IedConnection ();

			connection.Connect ("localhost", 10002);

			ControlObject controlClient = connection.CreateControlObject ("simpleIOGenericIO/GGIO1.SPCSO1");
            controlClient.SetOrigin ("TEST1234", OrCat.MAINTENANCE);

			Assert.IsNotNull (controlClient);

			controlClient.Operate (true);
		
			connection.Abort ();

			Assert.AreEqual (1, handlerCalled);

			iedServer.Stop ();

			iedServer.Dispose();
		}


		[Test()]
		public void ConnectionHandler()
		{
			IedModel iedModel = ConfigFileParser.CreateModelFromConfigFile ("../../model.cfg");

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

			Assert.AreEqual (1, handlerCalled);
			Assert.AreEqual (1, connectionCount);

			IedConnection con2 = new IedConnection ();
			con2.Connect ("localhost", 10002);

			Assert.AreEqual (2, handlerCalled);
			Assert.AreEqual (2, connectionCount);

			con1.Abort ();
			con2.Abort ();

			Assert.AreEqual (4, handlerCalled);
			Assert.AreEqual (0, connectionCount);

			Assert.AreEqual ("127.0.0.1:", ipAddress.Substring (0, 10));

			iedServer.Stop ();

            iedServer.Dispose();
		}

		[Test()]
		public void Quality()
		{
			Quality q = new Quality ();

			Assert.AreEqual (false, q.Overflow);

			q.Overflow = true;

			Assert.AreEqual (true, q.Overflow);

			q.Overflow = false;

			Assert.AreEqual (false, q.Overflow);

			Assert.AreEqual (Validity.GOOD, q.Validity);

			q.Substituted = true;

			Assert.AreEqual (true, q.Substituted);
			Assert.AreEqual (false, q.Overflow);

			q.Validity = Validity.QUESTIONABLE;

			Assert.AreEqual (Validity.QUESTIONABLE, q.Validity);
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

            Assert.AreEqual(true, true);
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

    }
}

