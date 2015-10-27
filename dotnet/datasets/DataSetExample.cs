using System;
using System.Collections.Generic;

using IEC61850.Client;
using IEC61850.Common;

namespace datasets
{
    class DataSetExample
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

                List<string> serverDirectory = con.GetServerDirectory(false);

                foreach (string entry in serverDirectory)
                {
                    Console.WriteLine("LD: " + entry);
                }

				// create a new data set

				List<string> dataSetElements = new List<string>();

				string dataSetName = "simpleIOGenericIO/UNKNOWN.ds1";

				//string dataSetName = "simpleIOGenericIO/LLN0.ds1";

				dataSetElements.Add("simpleIOGenericIO/GGIO1.AnIn1.mag.f[MX]");
				dataSetElements.Add("simpleIOGenericIO/GGIO1.AnIn2.mag.f[MX]");

				con.CreateDataSet(dataSetName, dataSetElements);

				// get the directory of the data set
				List<string> dataSetDirectory = con.GetDataSetDirectory(dataSetName);

				foreach (string entry in dataSetDirectory)
                {
                    Console.WriteLine("DS element: " + entry);
                }

				// read the values of the newly created data set
				DataSet dataSet = con.ReadDataSetValues(dataSetName, null);

				MmsValue dataSetValues = dataSet.GetValues();

				Console.WriteLine ("Data set contains " + dataSetValues.Size() + " elements");

				foreach (MmsValue value in dataSetValues) {
					Console.WriteLine("  DS value: " + value + " type: " + value.GetType());
				}

				// delete the data set
				con.DeleteDataSet(dataSetName);

                con.Abort();
            }
            catch (IedConnectionException e)
            {
				Console.WriteLine(e.Message + " reason: " + e.GetIedClientError().ToString());
            }

			// release all resources - do NOT use the object after this call!!
			con.Dispose ();

        }
    }
}
