using System;

using IEC61850.SV.Subscriber;
using IEC61850.Common;
using System.Threading;

namespace sv_subscriber
{
	class MainClass
	{
		private static void svUpdateListener(SVSubscriber subscriber, object parameter, SVSubscriberASDU asdu)
		{
			Console.WriteLine ("RECV ASDU:");

			string svID = asdu.GetSvId ();

			if (svID != null)
				Console.WriteLine ("  svID=" + svID);

			Console.WriteLine ("  smpCnt: " + asdu.GetSmpCnt ());
			Console.WriteLine ("  confRev: " + asdu.GetConfRev ());

			if (asdu.GetDataSize () >= 8) {
				Console.WriteLine ("  DATA[0]: " + asdu.GetFLOAT32(0));
				Console.WriteLine ("  DATA[1]: " + asdu.GetFLOAT32 (4));
			}

			if (asdu.GetDataSize () >= 16) {
				Console.WriteLine ("  DATA[2]: " + asdu.GetTimestamp (8).AsDateTime().ToString());
			}

		}

		public static void Main (string[] args)
		{
			Console.WriteLine ("Starting SV subscriber");

			SVReceiver receiver = new SVReceiver ();

			if (args.Length > 0) {
				receiver.SetInterfaceId (args [0]);
			}

			SVSubscriber subscriber = new SVSubscriber (null, 0x4000);

			subscriber.SetListener (svUpdateListener, null);

			receiver.AddSubscriber (subscriber);

			receiver.Start ();

			if (receiver.IsRunning ()) {

				bool running = true;

				/* run until Ctrl-C is pressed */
				Console.CancelKeyPress += delegate(object sender, ConsoleCancelEventArgs e) {
					e.Cancel = true;
					running = false;
				};

				while (running) {
					Thread.Sleep (100);
				}

				receiver.Stop ();
			} else
				Console.WriteLine ("Failed to start SV receiver. Running as root?");

			receiver.Dispose ();

		}
	}
}
