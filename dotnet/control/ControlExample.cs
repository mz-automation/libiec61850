using System;
using System.Collections.Generic;
using IEC61850.Common;
using IEC61850.Client;


namespace control
{
    class ControlExample
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
                con.Connect(hostname, 102);

				string objectReference = "IEDM1CPUBHKW/DRCC1.DERStr";

				ControlObject control = con.CreateControlObject(objectReference);

				ControlModel controlModel = control.GetControlModel();

				Console.WriteLine(objectReference + " has control model " + controlModel.ToString());

				if (!control.Operate(true))
					Console.WriteLine("operate failed!");



                con.Abort();
            }
            catch (IedConnectionException e)
            {
				Console.WriteLine(e.Message);
           }

        }
    }
}
