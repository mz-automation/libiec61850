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

		[Test ()]
		public void MmsValueBitString ()
		{
			var val = MmsValue.NewBitString(10);

			Assert.AreEqual (MmsType.MMS_BIT_STRING, val.GetType());
			Assert.AreEqual (10, val.Size());

			val.BitStringFromUInt32(7);

			Assert.AreEqual(7, val.BitStringToUInt32());

			Assert.AreEqual(true, val.GetBit(0));
			Assert.AreEqual(true, val.GetBit(1));
			Assert.AreEqual(true, val.GetBit(2));
			Assert.AreEqual(false, val.GetBit(3));

			Assert.AreEqual(false, val.GetBit(9));

			Assert.AreEqual(false, val.GetBit(10));

			val.SetBit(3, true);
			Assert.AreEqual(true, val.GetBit(3));

			Assert.AreEqual(15, val.BitStringToUInt32());

			val.SetBit(3, false);
			Assert.AreEqual(7, val.BitStringToUInt32());
		}

		[Test()]
		public void MmsValueOctetString ()
		{
			var val = MmsValue.NewOctetString(20);

			Assert.AreEqual (0, val.Size());
			Assert.AreEqual (20, val.MaxSize());

			byte[] octetString = val.getOctetString();

			Assert.AreEqual (0, octetString.Length);

			octetString = new byte[5];
			octetString[0] = 0x11;
			octetString[1] = 0x12;
			octetString[2] = 0x13;
			octetString[3] = 0x14;
			octetString[4] = 0x15;

			val.setOctetString(octetString);

			Assert.AreEqual(5, val.Size());

			byte[] secondOctetString = val.getOctetString();

			Assert.AreEqual(octetString, secondOctetString);
		}

		[Test ()]
		public void MmsValueFloat() 
		{
			var val = new MmsValue ((float) 1234.5678);

			Assert.AreEqual (val.ToFloat (), (float)1234.5678);

			val.SetFloat ((float) 0.1234);

			Assert.AreEqual (val.ToFloat (), (float) 0.1234);

			Assert.AreEqual (val.ToDouble (), (double) 0.1234, (double) 0.0001);
		}

		[Test ()]
		public void MmsValueDouble() 
		{
			var val = new MmsValue ((double) 1234.5678);

			Assert.AreEqual (val.ToDouble (), (double)1234.5678);

			val.SetDouble ((double) 0.1234);

			Assert.AreEqual (val.ToDouble (), (double) 0.1234);

			Assert.AreEqual (val.ToFloat (), (float)0.1234);
		}


	}
}

