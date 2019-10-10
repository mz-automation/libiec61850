/*
 * Copyright Waterford Institute of Technology 2017,
 * Telecommunications Software and Systems Group (TSSG),
 * Author Miguel Ponce de Leon <miguelpdl@tssg.org>.
 *
*/

public class IEC61850Server extends Thread {

    private sIedModel model = null;
    private SWIGTYPE_p_sIedServer iedServer = null;
    private int tcpPort = 8102;

    @Override
    public void interrupt() {
        try {
          iec61850.IedServer_stop(iedServer);
          iec61850.IedServer_destroy(iedServer);
          iec61850.IedModel_destroy(model);
          System.out.println("IEC61850Server has been Interrupted and STOPPED" );
        } catch (Exception ignored) {
        } finally {
            super.interrupt();
        }
    }

   @Override
   public void run() {

     System.out.println("MyThread for IEC61850Server has STARTED" );
     //Create a iec61850.IedModel
     model = iec61850.IedModel_create("testmodel");

     //Create a logical device
     sLogicalDevice lDevice1 = iec61850.LogicalDevice_create("SENSORS", model);

     //Create nodes off this logical device
     sLogicalNode lln0 = iec61850.LogicalNode_create("LLN0", lDevice1);
     sLogicalNode ttmp1 = iec61850.LogicalNode_create("TTMP1", lDevice1);

     sDataObject cdc_sav = iec61850.CDC_SAV_create("TmpSv", iec61850.toModelNode(ttmp1), 0, false);
     sDataObject cdc_asg = iec61850.CDC_ASG_create("TmpSp",  iec61850.toModelNode(ttmp1), 0, false);

     // Create a iec61850.IedServer based on a test IedModel node
     iedServer = iec61850.IedServer_create(model);
     iec61850.IedServer_start(iedServer, tcpPort);

     if (!iec61850.IedServer_isRunning(iedServer)) {
 			System.out.println("Starting server failed! Exit.\n");
 			iec61850.IedServer_destroy(iedServer);
      iec61850.IedModel_destroy(model);
      System.exit(0);
     }

     System.out.println("IEC61850Server has STARTED" );

     while (true) {
            try {
                Thread.sleep((long) 100);
            } catch (InterruptedException e) {
                break;
            }
        }



    }
  }
