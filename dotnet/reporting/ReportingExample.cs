using System;
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

			byte[] entryId = report.GetEntryId ();

			if (entryId != null) {
                Console.WriteLine ("  entryID: " + BitConverter.ToString(entryId));
			}

			if (report.HasDataSetName ()) {
				Console.WriteLine ("   report data set: " + report.GetDataSetName ());
			}

			Console.WriteLine ("  report dataset contains " + values.Size () + " elements");

			for (int i = 0; i < values.Size(); i++) {
				if (report.GetReasonForInclusion(i) != ReasonForInclusion.REASON_NOT_INCLUDED) {
					Console.WriteLine("    element " + i + " included for reason " + report.GetReasonForInclusion(i).ToString() + " " + values.GetElement(i));
				}

                if (report.HasDataReference()) {
                    Console.WriteLine("       data-ref: " + report.GetDataReference(i));
                }
                
			}

			ReportControlBlock rcb = (ReportControlBlock) parameter;
            
			Console.WriteLine("  For RCB: " + rcb.GetObjectReference() + " Buffered: " + rcb.IsBuffered() +
			                  " data-set: " + rcb.GetDataSetReference ());
           
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

				string rcbReference1 = "simpleIOGenericIO/LLN0.RP.EventsRCB01";
				string rcbReference2 = "simpleIOGenericIO/LLN0.RP.EventsIndexed01";
				string rcbReference3 = "simpleIOGenericIO/LLN0.BR.Measurements01";

				ReportControlBlock rcb1 = con.GetReportControlBlock(rcbReference1);
				ReportControlBlock rcb2 = con.GetReportControlBlock(rcbReference2);
				ReportControlBlock rcb3 = con.GetReportControlBlock(rcbReference3);

                rcb1.GetRCBValues();

                // note: the second parameter is not required!
                rcb1.InstallReportHandler(reportHandler, rcb1);

                if (rcb1.IsBuffered())
                    Console.WriteLine("RCB: " + rcbReference1 + " is buffered");

                rcb1.SetTrgOps(TriggerOptions.DATA_CHANGED | TriggerOptions.INTEGRITY);
                rcb1.SetIntgPd(5000);
                rcb1.SetRptEna(true);

                rcb2.GetRCBValues();

                if (rcb2.IsBuffered())
                    Console.WriteLine("RCB: " + rcbReference2 + " is buffered");

                rcb2.InstallReportHandler(reportHandler, rcb2);

                rcb2.SetOptFlds(ReportOptions.REASON_FOR_INCLUSION | ReportOptions.SEQ_NUM | ReportOptions.TIME_STAMP |
                    ReportOptions.CONF_REV | ReportOptions.ENTRY_ID | ReportOptions.DATA_REFERENCE | ReportOptions.DATA_SET);
                rcb2.SetTrgOps(TriggerOptions.DATA_CHANGED | TriggerOptions.INTEGRITY);
                rcb2.SetIntgPd(2000);
                rcb2.SetRptEna(true);

                rcb2.SetRCBValues();

                rcb2.GetRCBValues();

                Console.WriteLine("RCB: " + rcbReference2 + " Owner: " + BitConverter.ToString(rcb2.GetOwner()));

				rcb3.GetRCBValues();

				if (rcb3.IsBuffered())
					Console.WriteLine ("RCB: " + rcbReference3 + " is buffered");

				rcb3.InstallReportHandler(reportHandler, rcb3);

				rcb3.SetOptFlds(ReportOptions.REASON_FOR_INCLUSION | ReportOptions.SEQ_NUM | ReportOptions.TIME_STAMP |
				                ReportOptions.CONF_REV | ReportOptions.ENTRY_ID | ReportOptions.DATA_REFERENCE | ReportOptions.DATA_SET);
				rcb3.SetTrgOps(TriggerOptions.DATA_CHANGED | TriggerOptions.INTEGRITY);				
				rcb3.SetIntgPd(2000);
				rcb3.SetRptEna(true);

				rcb3.SetRCBValues();


				/* run until Ctrl-C is pressed */
				Console.CancelKeyPress += delegate(object sender, ConsoleCancelEventArgs e) {
					e.Cancel = true;
					ReportingExample.running = false;
				};

				/* stop main loop when connection is lost */
				con.InstallConnectionClosedHandler(delegate(IedConnection connection) {
					Console.WriteLine("Connection closed");
					ReportingExample.running = false;
				});

				while (running) {
					Thread.Sleep(10);
				}

				/* Dispose the RCBs when you no longer need them */
				rcb1.Dispose();
				rcb2.Dispose();
				rcb3.Dispose();

				con.Abort ();

				con.Dispose();
			} catch (IedConnectionException e) {
				Console.WriteLine ("Error: " + e.Message);

				con.Dispose ();
			}

		}
	}
}
