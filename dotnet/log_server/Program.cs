using IEC61850.Common;
using IEC61850.Server;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace log_server
{
    internal class Program
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

            IedModel iedModel = ConfigFileParser.CreateModelFromConfigFile("model.cfg");

            if (iedModel == null)
            {
                Console.WriteLine("No valid data model found!");
                return;
            }

            IedServerConfig config = new IedServerConfig();
            config.ReportBufferSize = 100000;

            IedServer iedServer = new IedServer(iedModel, config);

            LogStorage statusLog = SqliteLogStorage.CreateLogStorage("log_status.db");

            statusLog.MaxLogEntries = 10;

            iedServer.SetLogStorage("GenericIO/LLN0$EventLog", statusLog);

            iedServer.Start(10002);

            if (iedServer.IsRunning())
            {
                Console.WriteLine("Server started");

                DataObject ggio1AnIn1 = (DataObject)iedModel.GetModelNodeByShortObjectReference("GenericIO/GGIO1.AnIn1");

                DataAttribute ggio1AnIn1magF = (DataAttribute)ggio1AnIn1.GetChild("mag.f");
                DataAttribute ggio1AnIn1T = (DataAttribute)ggio1AnIn1.GetChild("t");

                DataObject ggio1Spcso1 = (DataObject)iedModel.GetModelNodeByShortObjectReference("GenericIO/GGIO1.SPCSO1");

                DataAttribute ggio1Spcso1stVal = (DataAttribute)ggio1Spcso1.GetChild("stVal");
                DataAttribute ggio1Spcso1T = (DataAttribute)ggio1Spcso1.GetChild("t");

                float floatVal = 1.0f;

                bool stVal = true;

                while (running)
                {
                    floatVal += 1f;
                    stVal = !stVal;

                    iedServer.LockDataModel();
                    var ts = new Timestamp(DateTime.Now);
                    iedServer.UpdateTimestampAttributeValue(ggio1AnIn1T, ts);
                    iedServer.UpdateFloatAttributeValue(ggio1AnIn1magF, floatVal);
                    iedServer.UpdateTimestampAttributeValue(ggio1Spcso1T, ts);
                    iedServer.UpdateBooleanAttributeValue(ggio1Spcso1stVal, stVal);
                    iedServer.UnlockDataModel();

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
