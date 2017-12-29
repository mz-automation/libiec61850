using System;

using IEC61850.GOOSE.Subscriber;
using System.Threading;
using IEC61850.Common;

namespace goose_subscriber
{
	class MainClass
	{
		private static void gooseListener (GooseSubscriber subscriber, object parameter)
		{
			Console.WriteLine ("Received GOOSE message:\n-------------------------");

			Console.WriteLine ("  stNum: " + subscriber.GetStNum ());

			Console.WriteLine ("  sqNum: " + subscriber.GetSqNum ());


			MmsValue values = subscriber.GetDataSetValues ();
	
			Console.WriteLine ("  values: " +values.Size ().ToString ());

			foreach (MmsValue value in values) {
				Console.WriteLine ("   value: " + value.ToString ());
			}
		}

		public static void Main (string[] args)
		{
			Console.WriteLine ("Starting GOOSE subscriber...");

			GooseReceiver receiver = new GooseReceiver ();

			receiver.SetInterfaceId ("eth0");

			GooseSubscriber subscriber = new GooseSubscriber ("simpleIOGenericIO/LLN0$GO$gcbAnalogValues");

			subscriber.SetAppId(1000);

			subscriber.SetListener (gooseListener, null);

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
				Console.WriteLine ("Failed to start GOOSE receiver. Running as root?");

			receiver.Dispose ();
		}
	}
}
