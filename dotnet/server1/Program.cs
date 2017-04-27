using System;
using IEC61850.Server;
using IEC61850.Common;
using System.Threading;

namespace server1
{
	class MainClass
	{
		public static void Main (string[] args)
		{
			bool running = true;

			/* run until Ctrl-C is pressed */
			Console.CancelKeyPress += delegate(object sender, ConsoleCancelEventArgs e) {
				e.Cancel = true;
				running = false;
			};

			IedModel iedModel = ConfigFileParser.CreateModelFromConfigFile ("../../model.cfg");

			if (iedModel == null) {
				Console.WriteLine ("No valid data model found!");
				return;
			}

			DataObject spcso1 = (DataObject)iedModel.GetModelNodeByShortObjectReference ("GenericIO/GGIO1.SPCSO1");

			IedServer iedServer = new IedServer (iedModel);

			iedServer.SetControlHandler (spcso1, delegate(DataObject controlObject, object parameter, MmsValue ctlVal, bool test) {
                bool val = ctlVal.GetBoolean();

                if (val)
                    Console.WriteLine("received binary control command: on");
                else
                    Console.WriteLine("received binary control command: off");
                
                return ControlHandlerResult.OK;
			}, null);

			iedServer.Start (102);
			Console.WriteLine ("Server started");

			GC.Collect ();

			DataObject ggio1AnIn1 = (DataObject)iedModel.GetModelNodeByShortObjectReference ("GenericIO/GGIO1.AnIn1");

			DataAttribute ggio1AnIn1magF = (DataAttribute)ggio1AnIn1.GetChild ("mag.f");
			DataAttribute ggio1AnIn1T = (DataAttribute)ggio1AnIn1.GetChild ("t");

			float floatVal = 1.0f;

			while (running) {
				floatVal += 1f;
				iedServer.UpdateTimestampAttributeValue (ggio1AnIn1T, new Timestamp (DateTime.Now));
				iedServer.UpdateFloatAttributeValue (ggio1AnIn1magF, floatVal);
				Thread.Sleep (100);
			}

			iedServer.Stop ();
			Console.WriteLine ("Server stopped");

			iedServer.Destroy ();
		}
	}
}
