using System;
using System.Threading;
using System.Security.Cryptography.X509Certificates;

using IEC61850.Server;
using IEC61850.Common;
using IEC61850.TLS;

namespace tls_server_example
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

			IedModel iedModel = ConfigFileParser.CreateModelFromConfigFile ("model.cfg");

			if (iedModel == null) {
				Console.WriteLine ("No valid data model found!");
				return;
			}

			DataObject spcso1 = (DataObject)iedModel.GetModelNodeByShortObjectReference ("GenericIO/GGIO1.SPCSO1");

			TLSConfiguration tlsConfig = new TLSConfiguration ();

			tlsConfig.SetOwnCertificate (new X509Certificate2 ("server.cer"));

			tlsConfig.SetOwnKey ("server-key.pem", null);

			// Add a CA certificate to check the certificate provided by the server - not required when ChainValidation == false
			tlsConfig.AddCACertificate (new X509Certificate2 ("root.cer"));

			// Check if the certificate is signed by a provided CA
			tlsConfig.ChainValidation = true;

			// Check that the shown server certificate is in the list of allowed certificates
			tlsConfig.AllowOnlyKnownCertificates = false;

			IedServer iedServer = new IedServer (iedModel, tlsConfig);

			iedServer.SetControlHandler (spcso1, delegate(ControlAction action, object parameter, MmsValue ctlVal, bool test) {
				bool val = ctlVal.GetBoolean();

				if (val)
					Console.WriteLine("received binary control command: on");
				else
					Console.WriteLine("received binary control command: off");

				return ControlHandlerResult.OK;
			}, null);

			iedServer.Start ();
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
