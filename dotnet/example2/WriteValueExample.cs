using System;
using System.Collections.Generic;
using IEC61850.Client;
using IEC61850.Common;

namespace example2
{
    class WriteValueExample
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

				float setMagF = con.ReadFloatValue("ied1Inverter/ZINV1.OutWSet.setMag.f", FunctionalConstraint.SP);

				Console.WriteLine("ied1Inverter/ZINV1.OutWSet.setMag.f: " + setMagF);

				setMagF += 1.0f;

				con.WriteValue("ied1Inverter/ZINV1.OutWSet.setMag.f", FunctionalConstraint.SP, new MmsValue(setMagF));

                con.Abort();
            }
            catch (IedConnectionException e)
            {
                Console.WriteLine("IED connection excepion: " + e.Message);
            }

			// release all resources - do NOT use the object after this call!!
			con.Dispose ();
        }
    }
}
