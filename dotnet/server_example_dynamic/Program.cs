using IEC61850.Common;
using IEC61850.Server;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Data.Common;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using static IEC61850.Server.IedServer;

namespace server_example_dynamic
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

            IedModel model = new IedModel("testmodel");

            LogicalDevice lDevice1 = new LogicalDevice("SENSORS", model);

            LogicalNode lln0 = new LogicalNode("LLN0", lDevice1);

            DataObject lln0_mod = CDC.Create_CDC_ENS(lln0, "Mod", 0);
            DataObject lln0_health = CDC.Create_CDC_ENS(lln0, "Health", 0);

            SettingGroupControlBlock sgcb = new SettingGroupControlBlock(lln0, 1, 1);

            LogicalNode ttmp1 = new LogicalNode("TTMP1", lDevice1);

            DataObject ttmp1_tmpsv = CDC.Create_CDC_SAV(ttmp1, "TmpSv", 0, false);

            DataAttribute temperatureValue = (DataAttribute)ttmp1_tmpsv.GetChild("instMag.f");
            DataAttribute temperatureTimestamp = (DataAttribute)ttmp1_tmpsv.GetChild("t");

            LogicalNode ggio1 = new LogicalNode("GGIO1", lDevice1);
        
            const uint CDC_CTL_MODEL_HAS_CANCEL = 1;
            const uint CDC_CTL_MODEL_SBO_ENHANCED = (uint)ControlModel.SBO_ENHANCED << 1;

            DataObject ggio1_anOut1 = CDC.Create_CDC_APC(
                ggio1,
                "AnOut1",
                0,
                CDC_CTL_MODEL_HAS_CANCEL | CDC_CTL_MODEL_SBO_ENHANCED,
                false // false = float type
            );

            DataSet dataSet = new DataSet("events", lln0);

            DataSetEntry entry = new DataSetEntry(dataSet, "TTMP1$MX$TmpSv$instMag$f", -1, null);

            byte rptOptions = (byte)(
                ReportOptions.SEQ_NUM |
                ReportOptions.TIME_STAMP |
                ReportOptions.REASON_FOR_INCLUSION
            );

            ReportControlBlock rcb1 = new ReportControlBlock(
                "events01",           // RCB name
                lln0,                 // parent logical node
                "events01",           // Report ID
                false,                // isBuffered = false (unbuffered)
                "events",             // DataSet name
                1,                    // confRev
                (byte)TriggerOptions.DATA_CHANGED, // trigger options
                rptOptions,           // report options
                50,                   // bufTm (ms)
                0                     // intgPd (integrity period)
            );

            ReportControlBlock rcb2 = new ReportControlBlock(
                "events02",
                lln0,
                "events02",
                false,
                "events",
                1,
                (byte)TriggerOptions.DATA_CHANGED,
                rptOptions,
                50,
                0
            );

            GSEControlBlock gseControlBlock = new GSEControlBlock(
                "gse01",              // name
                lln0,                 // parent logical node
                "events01",           // appId
                "events",             // DataSet name
                1,                    // confRev
                false,                // fixedOffs
                200,                  // minTime (ms)
                3000                  // maxTime (ms)
            );

            IedServerConfig config = new IedServerConfig();
            config.ReportBufferSize = 100000;

            IedServer iedServer = new IedServer(model, config);

            iedServer.Start(102);

            while (running)
            {
                if (iedServer.IsRunning())
                {
                    Thread.Sleep(100);
                }
            }
       
            iedServer.Stop();
            Console.WriteLine("Server stopped");
            iedServer.Destroy();
        }
    }
}
