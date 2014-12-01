using NUnit.Framework;
using System;
using IEC61850.Common;
using IEC61850.Client;

namespace tests
{
	[TestFixture ()]
	public class Test
	{
		[Test ()]
		public void TestCase ()
		{
			var val = new MmsValue (10.0f);

			Assert.AreEqual (10.0f, val.ToFloat ());
		}
	}
}

