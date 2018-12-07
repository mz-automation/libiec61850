using System;
using IEC61850.Client;
using IEC61850.Common;

namespace client_examples_setting_groups
{
    /// <summary>
    /// This class is intended to show how to use setting groups from the client side.
    /// It works with server_example_setting_groups.
    /// </summary>
    class SettingGroupsClientExample
    {
        public static void Main(string[] args)
        {
            Console.WriteLine("Hello World!");

            IedConnection con = new IedConnection ();

            string hostname;

            if (args.Length > 0)
                hostname = args[0];
            else
                hostname = "127.0.0.1";

            int port = 102;

            if (args.Length > 1)
                port = Int32.Parse(args [1]);

            Console.WriteLine("Connect to " + hostname);

            try
            {
                con.Connect(hostname, port);

                /* Get variable specification of the SGCB (optional) */
                MmsVariableSpecification sgcbVarSpec = con.GetVariableSpecification("DEMOPROT/LLN0.SGCB", FunctionalConstraint.SP);

                /* Read SGCB */
                MmsValue sgcbVal = con.ReadValue("DEMOPROT/LLN0.SGCB", FunctionalConstraint.SP);

                Console.WriteLine("NumOfSG: {0}", sgcbVal.GetChildValue("NumOfSG", sgcbVarSpec).ToString());
                Console.WriteLine("ActSG: {0}", sgcbVal.GetChildValue("ActSG", sgcbVarSpec).ToString());
                Console.WriteLine("EditSG: {0}", sgcbVal.GetChildValue("EditSG", sgcbVarSpec).ToString());
                Console.WriteLine("CnfEdit: {0}", sgcbVal.GetChildValue("CnfEdit", sgcbVarSpec).ToString());

                /* Set active setting group */
                con.WriteValue("DEMOPROT/LLN0.SGCB.ActSG", FunctionalConstraint.SP, new MmsValue((uint) 2));

                /* Set edit setting group */
                con.WriteValue("DEMOPROT/LLN0.SGCB.EditSG", FunctionalConstraint.SP, new MmsValue((uint) 1));

                /* Change a setting group value */
                con.WriteValue("DEMOPROT/PTOC1.StrVal.setMag.f", FunctionalConstraint.SE, new MmsValue(1.0f));

                /* Confirm new setting group values */
                con.WriteValue("DEMOPROT/LLN0.SGCB.CnfEdit", FunctionalConstraint.SP, new MmsValue(true));

                /* Read SGCB again */
                sgcbVal = con.ReadValue("DEMOPROT/LLN0.SGCB", FunctionalConstraint.SP);

                Console.WriteLine("ActSG: {0}",sgcbVal.GetChildValue("ActSG", sgcbVarSpec).ToString());


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
