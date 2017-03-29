using System;
using System.Collections.Generic;

using IEC61850.Client;
using IEC61850.Common;

namespace authenticate
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

				parameters.UsePasswordAuthentication("top secret");

                con.Connect(hostname, 102);

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