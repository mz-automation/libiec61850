using System;
using IEC61850.Client;
using IEC61850.Common;
using System.Threading;

namespace client_example_async
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
                hostname = "127.0.0.1";

            int port = 102;

            if (args.Length > 1)
                port = Int32.Parse(args [1]);

            Console.WriteLine("Connect to " + hostname);

            try
            {
                con.Connect(hostname, port);

                /* read FCDO */
                con.ReadValueAsync("simpleIOGenericIO/GGIO1.AnIn1", FunctionalConstraint.MX, delegate(uint invokeId, object parameter, IedClientError err, MmsValue value) {

                    if (err == IedClientError.IED_ERROR_OK) 
                    {
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
                    }
                    else {
                        Console.WriteLine("Read error: " + err.ToString());
                    }


                }, null);

                con.ReadValueAsync("simpleIOGenericIO/GGIO1.AnIn1", FunctionalConstraint.MX, delegate(uint invokeId, object parameter, IedClientError err, MmsValue value) {

                    if (err == IedClientError.IED_ERROR_OK) 
                    {
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
                    }
                    else {
                        Console.WriteLine("Read error: " + err.ToString());
                    }


                }, null);


               // Thread.Sleep(5000);    
              
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

        static void HandleReadObjectHandler (uint invokeId, object parameter, IedClientError err, MmsValue value)
        {
            
        }
    }
}
