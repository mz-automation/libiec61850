using System;
using System.Collections.Generic;

using IEC61850.Client;
using IEC61850.Common;

namespace example1
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
                hostname = "10.0.2.2";

            Console.WriteLine("Connect to " + hostname);


            try
            {
                con.Connect(hostname, 102);

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
        }
    }
}
