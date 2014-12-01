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

			if (report.HasTimestamp ())
				Console.WriteLine ("  timestamp: " + MmsValue.MsTimeToDateTimeOffset (report.GetTimestamp ()).ToString ());

			MmsValue values = report.GetDataSetValues ();

			Console.WriteLine ("  report dataset contains " + values.Size () + " elements");

			for (int i = 0; i < values.Size(); i++) {
				if (report.GetReasonForInclusion(i) != ReasonForInclusion.REASON_NOT_INCLUDED) {
					Console.WriteLine("    element " + i + " included for reason " + report.GetReasonForInclusion(i).ToString() + " " + values.GetElement(i));
				}
			}

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

			Console.WriteLine ("Connect to " + hostname);

			try {
				con.Connect (hostname, 102);

				string rcbReference = "simpleIOGenericIO/LLN0.RP.EventsRCB";

				ReportControlBlock rcb = con.getReportControlBlock(rcbReference);

				rcb.GetRCBValues();

				rcb.InstallReportHandler(reportHandler, null);

				if (rcb.IsBuffered())
					Console.WriteLine ("RCB: " + rcbReference + " is buffered");

				Console.WriteLine(rcb.GetDataSetReference());

				rcb.SetTrgOps(TriggerOptions.DATA_CHANGED | TriggerOptions.INTEGRITY);				
				rcb.SetIntgPd(5000);
				rcb.SetRptEna(true);

				rcb.SetRCBValues();

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
