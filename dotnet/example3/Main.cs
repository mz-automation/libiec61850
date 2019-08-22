using System;
using IEC61850.Client;
using System.Collections.Generic;

namespace example3
{
	class MainClass
    {
        public static void Main (string[] args)
        {
            IedConnection con = new IedConnection ();

            string hostname;

            if (args.Length > 0)
                hostname = args[0];
            else
                hostname = "localhost";

            Console.WriteLine("Connect to " + hostname);


            try
            {
				IsoConnectionParameters parameters = con.GetConnectionParameters();

				parameters.SetRemoteAddresses(new byte[] { 0x00, 0x01 }, new byte[] {0x00, 0x01}, new byte[] {0x00, 0x01, 0x02, 0x03});

				con.ConnectTimeout = 10000;

                con.MaxPduSize = 1200;

                con.Connect(hostname, 102);

				Console.WriteLine("Negotiated PDU size: " + con.GetMmsConnection().GetLocalDetail());

                List<string> serverDirectory = con.GetServerDirectory(false);

                foreach (string entry in serverDirectory)
                {
                    Console.WriteLine("LD: " + entry);
                }

				con.Release();
            }
            catch (IedConnectionException e)
            {
				Console.WriteLine(e.Message);
            }

			// release all resources - do NOT use the object after this call!!
			con.Dispose ();
        }
    }
}
