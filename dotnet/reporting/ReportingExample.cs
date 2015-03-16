using System;
using System.Collections.Generic;
using System.Threading;

using IEC61850.Client;
using IEC61850.Common;

namespace reporting
{
	class ReportingExample
	{

		private static void reportHandler (Report report, object parameter)
		{
			Console.WriteLine ("Received report:\n----------------");

			Console.WriteLine ("  for RCB: " + report.GetRcbReference());

			if (report.HasTimestamp ())
				Console.WriteLine ("  timestamp: " + MmsValue.MsTimeToDateTimeOffset (report.GetTimestamp ()).ToString ());

			MmsValue values = report.GetDataSetValues ();

			Console.WriteLine ("  report dataset contains " + values.Size () + " elements");

			for (int i = 0; i < values.Size(); i++) {
				if (report.GetReasonForInclusion(i) != ReasonForInclusion.REASON_NOT_INCLUDED) {
					Console.WriteLine("    element " + i + " included for reason " + report.GetReasonForInclusion(i).ToString() + " " + values.GetElement(i));
				}
			}

			ReportControlBlock rcb = (ReportControlBlock) parameter;

			Console.WriteLine("  For RCB: " + rcb.GetObjectReference() + " Buffered: " + rcb.IsBuffered());

		}


		private static bool running = true;

		public static void Main (string[] args)
		{
			IedConnection con = new IedConnection ();

			string hostname;

			if (args.Length > 0)
				hostname = args [0];
			else
				hostname = "localhost";

            hostname = "10.0.2.2";

			Console.WriteLine ("Connect to " + hostname);

			try {
				con.Connect (hostname, 102);

				string rcbReference1 = "simpleIOGenericIO/LLN0.RP.EventsRCB01";
				string rcbReference2 = "simpleIOGenericIO/LLN0.RP.EventsIndexed01";

				ReportControlBlock rcb1 = con.GetReportControlBlock(rcbReference1);
				ReportControlBlock rcb2 = con.GetReportControlBlock(rcbReference2);

				rcb1.GetRCBValues();

				// note: the second parameter is not required!
				rcb1.InstallReportHandler(reportHandler, rcb1);

				if (rcb1.IsBuffered())
					Console.WriteLine ("RCB: " + rcbReference1 + " is buffered");

				rcb1.SetTrgOps(TriggerOptions.DATA_CHANGED | TriggerOptions.INTEGRITY);				
				rcb1.SetIntgPd(5000);
				rcb1.SetRptEna(true);

				rcb1.SetRCBValues();

				rcb2.GetRCBValues();

				rcb2.InstallReportHandler(reportHandler, rcb2);

				rcb2.SetTrgOps(TriggerOptions.DATA_CHANGED | TriggerOptions.INTEGRITY);				
				rcb2.SetIntgPd(2000);
				rcb2.SetRptEna(true);

				rcb2.SetRCBValues();


				/* run until Ctrl-C is pressed */
				Console.CancelKeyPress += delegate(object sender, ConsoleCancelEventArgs e) {
					e.Cancel = true;
					ReportingExample.running = false;
				};

				while (running) {
					Thread.Sleep(10);
				}

				con.Abort ();
			} catch (IedConnectionException e) {
				Console.WriteLine (e.Message);
			}

		}
	}
}
