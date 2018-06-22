using System;
using System.Collections.Generic;
using IEC61850.Common;
using IEC61850.Client;
using System.Threading;


namespace control
{
    class ControlExample
    {
		private static void commandTerminationHandler (Object parameter, ControlObject control)
		{
			LastApplError lastApplError = control.GetLastApplError();
			Console.WriteLine("HANDLER CALLED! " + lastApplError.addCause);
		}

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

				/* direct control with normal security or SBO with normal security */
				string objectReference = "simpleIOGenericIO/GGIO1.SPCSO1";

				ControlObject control = con.CreateControlObject(objectReference);

				ControlModel controlModel = control.GetControlModel();

				Console.WriteLine(objectReference + " has control model " + controlModel.ToString());
				Console.WriteLine("  type of ctlVal: " + control.GetCtlValType().ToString());


				switch (controlModel) {

				case ControlModel.STATUS_ONLY:
					Console.WriteLine("Control is status-only!");
					break;

				case ControlModel.DIRECT_NORMAL:
				case ControlModel.DIRECT_ENHANCED:
					if (!control.Operate(true))
						Console.WriteLine("operate failed!");
					else
						Console.WriteLine("operated successfully!");
					break;

				case ControlModel.SBO_NORMAL:
				case ControlModel.SBO_ENHANCED:

					if (control.Select()) {
						if (!control.Operate(true))
							Console.WriteLine("operate failed!");
						else
							Console.WriteLine("operated successfully!");
					}
					else
						Console.WriteLine("Select failed!");

					break;
				}

				/* direct control with enhanced security */
				objectReference = "simpleIOGenericIO/GGIO1.SPCSO3";
				control = con.CreateControlObject(objectReference);

				controlModel = control.GetControlModel();
				Console.WriteLine(objectReference + " has control model " + controlModel.ToString());

				if (controlModel == ControlModel.DIRECT_ENHANCED) {
					control.SetCommandTerminationHandler(commandTerminationHandler, null);

					control.Operate(true);

					Thread.Sleep(1000);
				}

				// has to be called before IedConnection dispose method!
				control.Dispose();

             	con.Abort();
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
