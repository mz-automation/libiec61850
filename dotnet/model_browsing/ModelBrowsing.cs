using System;
using System.Collections.Generic;

using IEC61850.Client;
using IEC61850.Common;

/// <summary>
/// Example for browsing the data model of a server. Shows logical devices, logical nodes,
/// data sets and report control blocks
/// </summary>
namespace model_browsing
{
    class ModelBrowsing
    {
        private static void PrintChildSpec(MmsVariableSpecification spec, int indent)
        {
            string indentStr = "";

            for (int i = 0; i < indent; i++)
                indentStr += " ";

            if (spec.GetType() == MmsType.MMS_STRUCTURE)
            {
                foreach (MmsVariableSpecification elementSpec in spec)
                {
                    Console.WriteLine(indentStr + "DA: " +  elementSpec.GetName() + " : " + elementSpec.GetType());

                    if (elementSpec.GetType() == MmsType.MMS_STRUCTURE || elementSpec.GetType() == MmsType.MMS_ARRAY)
                    {
                        PrintChildSpec(elementSpec, indent + 2);
                    }
                }
            }
        }

        public static void Main (string[] args)
        {
            IedConnection con = new IedConnection ();

            string hostname;

            if (args.Length > 0)
                hostname = args[0];
            else
                hostname = "localhost";

            try
            {
				Console.WriteLine("Connect to " + hostname + " ...");

				con.Connect(hostname, 102);

				Console.WriteLine("Connected.");

				MmsConnection mmsCon = con.GetMmsConnection();

				MmsServerIdentity identity = mmsCon.GetServerIdentity();

				Console.WriteLine("Vendor:   " + identity.vendorName);
				Console.WriteLine("Model:    " + identity.modelName);
				Console.WriteLine("Revision: " + identity.revision);

                List<string> serverDirectory = con.GetServerDirectory(false);

                foreach (string ldName in serverDirectory)
                {
                    Console.WriteLine("LD: " + ldName);

					List<string> lnNames = con.GetLogicalDeviceDirectory(ldName);

					foreach (string lnName in lnNames)
					{
						Console.WriteLine("  LN: " + lnName);

						string logicalNodeReference = ldName + "/" + lnName;

						// discover data objects
						List<string> dataObjects = 
							con.GetLogicalNodeDirectory(logicalNodeReference, ACSIClass.ACSI_CLASS_DATA_OBJECT);

						foreach (string dataObject in dataObjects) {
							Console.WriteLine("    DO: " + dataObject);

							List<string> dataDirectory = con.GetDataDirectoryFC(logicalNodeReference + "." + dataObject);

							foreach (string dataDirectoryElement in dataDirectory) {

								string daReference = logicalNodeReference + "." + dataObject + "." + ObjectReference.getElementName(dataDirectoryElement);

								// get the type specification of a variable
								MmsVariableSpecification specification = con.GetVariableSpecification(daReference,  ObjectReference.getFC(dataDirectoryElement));

								Console.WriteLine ("      DA/SDO: [" + ObjectReference.getFC(dataDirectoryElement) + "] " +
								                   ObjectReference.getElementName(dataDirectoryElement) + " : " + specification.GetType()
								                   + "(" + specification.Size() + ")");

                                if (specification.GetType() == MmsType.MMS_STRUCTURE)
                                {
                                    foreach (MmsVariableSpecification elementSpec in specification)
                                    {
                                        PrintChildSpec(elementSpec, 8);
                                        //Console.WriteLine("           " + elementSpec.GetName() + " : " + elementSpec.GetType());
                                    }
                                }
                                else if (specification.GetType() == MmsType.MMS_ARRAY)
                                {
                                    MmsVariableSpecification arrayElementspec = specification.getArrayElementType();
                                    PrintChildSpec(arrayElementspec, 8);


                                    //Console.WriteLine("           elements: " + arrayElementspec.GetType());
                                }
							}

						}

						// discover data sets
						List<string> dataSets =
							con.GetLogicalNodeDirectory(logicalNodeReference, ACSIClass.ACSI_CLASS_DATA_SET);

						foreach (string dataSet in dataSets) {
							Console.WriteLine("    Dataset: " + dataSet);
						}

						// discover unbuffered report control blocks
						List<string> urcbs =
							con.GetLogicalNodeDirectory(logicalNodeReference, ACSIClass.ACSI_CLASS_URCB);

						foreach (string urcb in urcbs) {
							Console.WriteLine("    URCB: " + urcb);
						}

						// discover buffered report control blocks
						List<string> brcbs =
							con.GetLogicalNodeDirectory(logicalNodeReference, ACSIClass.ACSI_CLASS_BRCB);

						foreach (string brcb in brcbs) {
							Console.WriteLine("    BRCB: " + brcb);
						}


					}

                }

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
