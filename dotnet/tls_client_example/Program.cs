using System;
using IEC61850.Client;
using IEC61850.Common;
using IEC61850.TLS;
using System.Threading;
using System.Collections.Generic;
using System.Security.Cryptography.X509Certificates;

namespace tls_client_example
{
	class MainClass
	{
		public static void Main (string[] args)
		{
			TLSConfiguration tlsConfig = new TLSConfiguration ();

			tlsConfig.SetOwnCertificate (new X509Certificate2 ("client1.cer"));

			tlsConfig.SetOwnKey ("client1-key.pem", null);

			// Add a CA certificate to check the certificate provided by the server - not required when ChainValidation == false
			tlsConfig.AddCACertificate (new X509Certificate2 ("root.cer"));

			// Check if the certificate is signed by a provided CA
			tlsConfig.ChainValidation = true;

			// Check that the shown server certificate is in the list of allowed certificates
			tlsConfig.AllowOnlyKnownCertificates = false;

			IedConnection con = new IedConnection (tlsConfig);

			string hostname;

			if (args.Length > 0)
				hostname = args[0];
			else
				hostname = "127.0.0.1";

			int port = -1;

			if (args.Length > 1)
				port = Int32.Parse(args [1]);

			Console.WriteLine("Connect to " + hostname);

			try
			{
				con.Connect(hostname, port);

				List<string> serverDirectory = con.GetServerDirectory(false);

				foreach (string entry in serverDirectory)
				{
					Console.WriteLine("LD: " + entry);
				}

				List<string> lnDirectory = con.GetLogicalNodeDirectory("simpleIOGenericIO/LLN0", ACSIClass.ACSI_CLASS_DATA_SET);

				foreach (string entry in lnDirectory)
				{
					Console.WriteLine("Dataset: " + entry);
				}

				string vendor = con.ReadStringValue ("simpleIOGenericIO/LLN0.NamPlt.vendor", FunctionalConstraint.DC);
				Console.WriteLine ("Vendor: " + vendor);

				/* read FCDO */
				MmsValue value = con.ReadValue("simpleIOGenericIO/GGIO1.AnIn1", FunctionalConstraint.MX);

				if (value.GetType() == MmsType.MMS_STRUCTURE)
				{
					Console.WriteLine("Value is of complex type");

					for (int i = 0; i < value.Size(); i++)
					{
						Console.WriteLine("  element: " + value.GetElement(i).GetType());
						if (value.GetElement(i).GetType() == MmsType.MMS_UTC_TIME)
						{
							Console.WriteLine("   -> " + value.GetElement(i).GetUtcTimeAsDateTimeOffset());
						}
					}
				}

				DataSet dataSet = con.ReadDataSetValues("simpleIOGenericIO/LLN0.Events", null);

				Console.WriteLine("Read data set " + dataSet.GetReference());

				con.Abort();
			}
			catch (IedConnectionException e)
			{
				Console.WriteLine(e.Message);
			}

			System.Threading.Thread.Sleep(2000);

			// release all resources - do NOT use the object after this call!!
			con.Dispose ();
		}
	}
}
