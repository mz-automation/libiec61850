using System;
using IEC61850.Client;
using IEC61850.Common;
using System.Threading;
using System.Collections.Generic;

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
                con.MaxPduSize = 1000;

                con.StateChanged = delegate(IedConnection connection, IedConnectionState newState) {
                    Console.WriteLine("state change: " + newState.ToString());  
                };

                con.Connect(hostname, port);

                AutoResetEvent waitForCallback = new AutoResetEvent(false);

                List<string> ldList = null;

                con.GetServerDirectoryAsync(ldList, null, delegate(uint invokeId, object parameter, IedClientError err, System.Collections.Generic.List<string> nameList, bool moreFollows) {
                    
                    if (nameList != null) {
                        ldList = nameList;
                    }
                    else
                    {
                        Console.WriteLine("Get server directory error: " + err.ToString());
                    }

                    waitForCallback.Set();
                        
                }, null);

                waitForCallback.WaitOne();

                if (ldList != null) {

                    string firstLdName = null;

                    Console.WriteLine("Server directory:");

                    foreach (string ldName in ldList) {
                        Console.WriteLine("  LD: " + ldName);
                        if (firstLdName == null)
                            firstLdName = ldName;
                    }

                    bool moreVariabesFollows = true;
                    string lastVariableName = null;

                    List<string> variablesList = new List<string>();

                    while (moreVariabesFollows) {

                        waitForCallback.Reset();

                        con.GetLogicalDeviceVariablesAsync(variablesList, firstLdName, lastVariableName, delegate(uint invokeId, object parameter, IedClientError err, List<string> nameList, bool moreFollows) {

                            if (nameList != null) {
  
                                if (moreFollows)
                                    Console.WriteLine("More variables available...");

                                lastVariableName = nameList[nameList.Count - 1];
                            }
                            else
                            {
                                Console.WriteLine("Get logical device variables error: " + err.ToString());
                            }


                            moreVariabesFollows = moreFollows;

                            waitForCallback.Set();
                        }, null);

                        waitForCallback.WaitOne();
                    }

                    Console.WriteLine("Variables in logical device {0}:", firstLdName);

                    foreach (string variableName in variablesList) {
                        Console.WriteLine("  {0}", variableName);
                    }

                }

                Console.WriteLine("Now read variables...");

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

                Thread.Sleep(5000);    
              
                con.Abort();

                Console.WriteLine("Aborted");
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
