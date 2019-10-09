/*
 * Copyright Waterford Institute of Technology 2017,
 * Telecommunications Software and Systems Group (TSSG),
 * Author Miguel Ponce de Leon <miguelpdl@tssg.org>.
 *
*/

public class IEC61850Client {

  private int tcpPort = 8102;
  private SWIGTYPE_p_sIedConnection con;

  // List of int values for Client Side errors from iec61850_client.h
  private int[] error = {0, 1, 2,3, 4, 5, 10, 11, 12, 13, 21, 22, 23, 24,25,26, 27, 28, 29, 30, 31, 32, 33, 98, 99};
  private IedConnectionState conState = null;
  private String theVal = null;
  private FunctionalConstraint theValType = null;
  private float temperatureValue;
  private float newValue;
  private LastApplError err = null;
  private int theExpectedError = 21;
  private float temperatureSetpoint;

  IEC61850Client() {

    if (con == null) {
      con = iec61850.IedConnection_create();
    }

    iec61850.IedConnection_connect(con, error, "localhost", tcpPort);

    conState = iec61850.IedConnection_getState(con);

    if ( conState.toString() == "IED_STATE_CONNECTED" || conState.toString() == "IED_STATE_IDLE") {
      System.out.println("IEC61850Client has connected to the IEC61850Server" );

      //Accessing to SAV values
  		theVal = "testmodelSENSORS/TTMP1.TmpSv.instMag.f";
      theValType = FunctionalConstraint.IEC61850_FC_MX;

  		temperatureValue = iec61850.IedConnection_readFloatValue(con, error, theVal, theValType);

  		if (temperatureValue == 0) {
        System.out.println("IEC61850Client has read testmodelSENSORS" );
      }

      newValue = temperatureValue + 10;

  		iec61850.IedConnection_writeFloatValue(con, error, theVal, theValType, newValue);

      err = iec61850.IedConnection_getLastApplError(con);

      if (err.getError() == theExpectedError) {
        System.out.println("IEC61850Client caught the expected client error = " + err.getError() );
      } else {
        System.out.println("IEC61850Client caught a error = " + err.getError() );
      }

      // Accessing to ASG values
  		theVal = "testmodelSENSORS/TTMP1.TmpSp.setMag.f";
  		theValType = FunctionalConstraint.IEC61850_FC_SP;
  		temperatureSetpoint = iec61850.IedConnection_readFloatValue(con, error, theVal, theValType);
  		System.out.println("IEC61850Client read a temperature set point of = " + temperatureSetpoint );

      newValue = temperatureValue + 10;
  		iec61850.IedConnection_writeFloatValue(con, error, theVal, theValType, newValue);

  		temperatureSetpoint = iec61850.IedConnection_readFloatValue(con, error, theVal, theValType);

      if (temperatureSetpoint == newValue) {
          System.out.println("IEC61850Client read a temperature set point of = " + temperatureSetpoint );
      }

      //Must close the connection at the end of the test.
      iec61850.IedConnection_close(con);

    } else {
      System.out.println("IEC61850Client connection error " + conState );

      iec61850.IedConnection_close(con);

    }
    //Destroy the connection at the end of the test.
    iec61850.IedConnection_destroy(con);

  }
}
