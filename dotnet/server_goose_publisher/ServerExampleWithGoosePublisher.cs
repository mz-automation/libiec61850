using System;
using IEC61850.Server;
using IEC61850.Common;
using System.Threading;

namespace server_goose_publisher
{
    /// <summary>
    /// This example shows how to use the server integrated GOOSE publisher from .NET
    /// </summary>
    /// <remarks>
    /// This example requires that the native library (libiec61850) is compiled with GOOSE support.
    /// </remarks>
    class ServerExampleWithGoosePublisher
    {
        public static void Main(string[] args)
        {
            bool running = true;

            /* run until Ctrl-C is pressed */
            Console.CancelKeyPress += delegate (object sender, ConsoleCancelEventArgs e)
            {
                e.Cancel = true;
                running = false;
            };

            IedModel iedModel = ConfigFileParser.CreateModelFromConfigFile("simpleIO_direct_control_goose.cfg");

            if (iedModel == null)
            {
                Console.WriteLine("No valid data model found!");
                return;
            }

            iedModel.SetIedName("TestIED");

            DataObject spcso1 = (DataObject)iedModel.GetModelNodeByShortObjectReference("GenericIO/GGIO1.SPCSO1");

            IedServerConfig config = new IedServerConfig();
            config.ReportBufferSize = 100000;

            IedServer iedServer = new IedServer(iedModel, config);

            // Set GOOSE interface (e.g. "eth0" for Linux or "0" for Windows (first interface)
            iedServer.SetGooseInterfaceId("0");

            // The GoCBEventHandler can be used to track GoCB events
            iedServer.SetGoCBHandler(delegate (MmsGooseControlBlock goCB, int cbEvent, object parameter)
            {
                if (cbEvent == 1)
                {
                    Console.WriteLine("GCB " + goCB.LN.GetObjectReference() + ":" + goCB.Name + " enabled");
                }
                else
                {
                    Console.WriteLine("GCB " + goCB.LN.GetObjectReference() + ":" + goCB.Name + " disabled");
                }
            }, null);

            iedServer.SetCheckHandler(spcso1, delegate (ControlAction action, object parameter, MmsValue ctlVal, bool test, bool interlockCheck)
            {

                Console.WriteLine("Received binary control command:");
                Console.WriteLine("   ctlNum: " + action.GetCtlNum());
                Console.WriteLine("   execution-time: " + action.GetControlTimeAsDataTimeOffset().ToString());

                return CheckHandlerResult.ACCEPTED;
            }, null);

            iedServer.SetControlHandler(spcso1, delegate (ControlAction action, object parameter, MmsValue ctlVal, bool test)
            {
                bool val = ctlVal.GetBoolean();

                if (val)
                    Console.WriteLine("execute binary control command: on");
                else
                    Console.WriteLine("execute binary control command: off");

                return ControlHandlerResult.OK;
            }, null);

            DataObject spcso2 = (DataObject)iedModel.GetModelNodeByShortObjectReference("GenericIO/GGIO1.SPCSO2");

            iedServer.SetSelectStateChangedHandler(spcso2, delegate (ControlAction action, object parameter, bool isSelected, SelectStateChangedReason reason)
            {
                DataObject cObj = action.GetControlObject();

                Console.WriteLine("Control object " + cObj.GetObjectReference() + (isSelected ? " selected" : " unselected") + " reason: " + reason.ToString());

            }, null);

            iedServer.Start(102);

            // Enable GOOSE publishing for all GOOSE control blocks
            iedServer.EnableGoosePublishing();

            if (iedServer.IsRunning())
            {
                Console.WriteLine("Server started");

                GC.Collect();

                DataObject ggio1AnIn1 = (DataObject)iedModel.GetModelNodeByShortObjectReference("GenericIO/GGIO1.AnIn1");

                DataAttribute ggio1AnIn1magF = (DataAttribute)ggio1AnIn1.GetChild("mag.f");
                DataAttribute ggio1AnIn1T = (DataAttribute)ggio1AnIn1.GetChild("t");

                float floatVal = 1.0f;

                while (running)
                {
                    floatVal += 1f;
                    iedServer.UpdateTimestampAttributeValue(ggio1AnIn1T, new Timestamp(DateTime.Now));
                    iedServer.UpdateFloatAttributeValue(ggio1AnIn1magF, floatVal);
                    Thread.Sleep(100);
                }

                iedServer.Stop();
                Console.WriteLine("Server stopped");
            }
            else
            {
                Console.WriteLine("Failed to start server");
            }

            iedServer.Destroy();
        }
    }
}
