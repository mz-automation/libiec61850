using System;
using IEC61850.Client;
using System.Collections.Generic;
using IEC61850.Common;

namespace log_client
{
	class MainClass
	{
		private static void PrintJournalEntries(List<MmsJournalEntry> journalEntries) {
			foreach (MmsJournalEntry entry in journalEntries) {
				Console.WriteLine("EntryID: " + BitConverter.ToString(entry.GetEntryID()));
				Console.WriteLine("  occurence time: " + MmsValue.MsTimeToDateTimeOffset(entry.GetOccurenceTime()).ToString());
				foreach (MmsJournalVariable variable in entry.GetJournalVariables()) {
					Console.WriteLine("    variable: " + variable.GetTag());
					Console.WriteLine("       value: " + variable.GetValue().ToString());
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

			Console.WriteLine("Connect to " + hostname);


			try
			{

				con.Connect(hostname, 102);

				Console.WriteLine("Negotiated PDU size: " + con.GetMmsConnection().GetLocalDetail());

				List<string> serverDirectory = con.GetServerDirectory(false);

				foreach (string deviceName in serverDirectory)
				{
					Console.WriteLine("LD: " + deviceName);
					List<string> deviceDirectory = con.GetLogicalDeviceDirectory(deviceName);

					foreach (string lnName in deviceDirectory) {
						Console.WriteLine("  LN: " + lnName);

						List<string> lcbs = con.GetLogicalNodeDirectory(deviceName + "/" + lnName, IEC61850.Common.ACSIClass.ACSI_CLASS_LCB);

						foreach(string lcbName in lcbs) {
							Console.WriteLine("    LCB: " + lcbName);

							MmsValue lcbValues = con.ReadValue(deviceName + "/" + lnName + "." + lcbName, FunctionalConstraint.LG);

							Console.WriteLine("      values: " + lcbValues.ToString());
						}

						List<string> logs = con.GetLogicalNodeDirectory(deviceName + "/" + lnName, IEC61850.Common.ACSIClass.ACSI_CLASS_LOG);

						foreach(string logName in logs) {
							Console.WriteLine("    LOG: " + logName);
						}
					}
				}

				bool moreFollows;

				Console.WriteLine("\nQueryLogAfter:");

				List<MmsJournalEntry> journalEntries = con.QueryLogAfter("simpleIOGenericIO/LLN0$EventLog", 
					new byte[]{0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0, out moreFollows);

				PrintJournalEntries(journalEntries);

				Console.WriteLine("\nQueryLogByTime:");

				journalEntries = con.QueryLogByTime("simpleIOGenericIO/LLN0$EventLog", 
					new DateTime(1970, 1, 1, 0, 0, 0, DateTimeKind.Utc),
					DateTime.UtcNow, 
					out moreFollows);
				
				PrintJournalEntries(journalEntries);

				con.Release();
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
