/*
 * Copyright Waterford Institute of Technology 2017,
 * Telecommunications Software and Systems Group (TSSG),
 * Author Miguel Ponce de Leon <miguelpdl@tssg.org>.
 *
*/

import java.lang.System;
import java.lang.Throwable;
import java.lang.Thread;
import java.lang.String;
import java.io.*;

/**
 * This test programme should mirror the functionality laid out in the Python
 * equivalent.
 *
 * @author Miguel Ponce de Leon
 */

public class test_javaiec61850 {

  static {
    try {
      //Using loadLibrary means that LD_LIBRARY_PATH must be set
    	System.load("/opt/libiec61850/javaiec61850/libiec61850.so");
    } catch (UnsatisfiedLinkError e) {
      System.err.println("Native code library libiec61850 failed to load.\n" + e);
      System.exit(1);
    }
  }

  /**
  * main is the main entry point into this tester application. IEC61850Server runs
  * as a server in a waiting Thread and then a IEC61850Client interacts with that
  * servers model.
  */

  public static void main (String[] args) throws Throwable {

        IEC61850Server iec61850Server = new IEC61850Server();

        iec61850Server.start();

        Thread.sleep((long) 200);
        //Run the test IEC Client
        IEC61850Client testIECClient = new IEC61850Client();

      //Stop the iec61850Server
      if (iec61850Server != null) {
            iec61850Server.interrupt();
      }
  }

}
