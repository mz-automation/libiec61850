/*
 *  server_example_access_control.cs
 *
 *  - How to use access control mechanisms
 *  - How to implement RBAC features based on access control mechanisms
 */

using System;
using IEC61850.Server;
using IEC61850.Common;
using IEC61850;
using System.Threading;
using System.Net;
using static IEC61850.Server.IedServer;
using System.Collections.Generic;
using System.Reflection.Metadata;
using IEC61850.Client;
using ReportControlBlock = IEC61850.Server.ReportControlBlock;
using IEC61850.Model;
using System.Data.Common;
using System.Security.Cryptography;

namespace server_access_control
{
    class MainClass
    {
        class PTOC1Settings
        {
            public float strVal;
            public int opDlTmms;
            public int rsDlTmms;
            public int rstTms;
        }

        public static void Main(string[] args)
        {
            bool running = true;

            /* run until Ctrl-C is pressed */
            Console.CancelKeyPress += delegate (object sender, ConsoleCancelEventArgs e)
            {
                e.Cancel = true;
                running = false;
            };

            /* Create new server configuration object */
            IedServerConfig config = new IedServerConfig();

            /* Set buffer size for buffered report control blocks to 200000 bytes */
            config.ReportBufferSize = 200000;

            /* Set stack compliance to a specific edition of the standard (WARNING: data model has also to be checked for compliance) */
            config.Edition = Iec61850Edition.EDITION_2;

            /* Set the base path for the MMS file services */
            config.FileServiceBasePath = "./vmd-filestore/";

            /* disable MMS file service */
            config.FileServiceEnabled =  false;

            /* enable dynamic data set service */
            config.DynamicDataSetServiceEnabled = true;

            /* disable log service */
            config.LogServiceEnabled = false;

            /* set maximum number of clients */
            config.MaxMmsConnections = 2;

            IedModel iedModel = ConfigFileParser.CreateModelFromConfigFile("model.cfg");

            IedServer iedServer = new IedServer(iedModel, config);

            iedServer.SetServerIdentity("libiec61850.com", "access control example", "1.0.0");

            DataObject spcso1 = (DataObject)iedModel.GetModelNodeByShortObjectReference("GenericIO/GGIO1.SPCSO1");

            iedServer.SetControlHandler(spcso1, delegate (ControlAction action, object parameter, MmsValue value, bool test)
            {
                if (test)
                    return ControlHandlerResult.FAILED;

                if (value.GetType() == MmsType.MMS_BOOLEAN)
                {
                    Console.WriteLine("received binary control command: ");

                    if (value.GetBoolean())
                        Console.WriteLine("on\n");
                    else
                        Console.WriteLine("off\n");
                }
                else
                    return ControlHandlerResult.FAILED;

                return ControlHandlerResult.OK;
            }, spcso1);

            void ConnectionCallBack(IedServer server, ClientConnection clientConnection, bool connected, object parameter)
            {
                if (connected)
                    Console.WriteLine("Connection opened\n");
                else
                    Console.WriteLine("Connection closed\n");
            }

            var connectionCallBack = new ConnectionIndicationHandler(ConnectionCallBack);
            iedServer.SetConnectionIndicationHandler(connectionCallBack, "127.0.0.1");


            /* Install handler to log RCB events */

            iedServer.SetRCBEventHandler(delegate (object parameter, ReportControlBlock rcb, ClientConnection con, RCBEventType eventType, string parameterName, MmsDataAccessError serviceError)
            {
                Console.WriteLine("RCB: " + rcb.Parent.GetObjectReference() + "." + rcb.Name + " event: " + eventType.ToString());

                if (con != null)
                {
                    Console.WriteLine("  caused by client " + con.GetPeerAddress());
                }
                else
                {
                    Console.WriteLine("  client = null");
                }

                if ((eventType == RCBEventType.SET_PARAMETER) || (eventType == RCBEventType.GET_PARAMETER))
                {
                    Console.WriteLine("RCB: "+rcb.Name + " event: "+ eventType .ToString()+ "\n");
                    Console.WriteLine("  param:  "+ parameterName + "\n");
                    Console.WriteLine("  result: "+ serviceError.ToString() + "\n");
                }

                if (eventType == RCBEventType.ENABLED)
                {
                    Console.WriteLine("RCB: "+ rcb.Name + " event: " + eventType.ToString() + "\n");
                    string rptId = rcb.RptID;
                    Console.WriteLine("   rptID:  "+ rptId+"\n");
                    string dataSet =rcb.DataSet;
                    Console.WriteLine("   datSet:"+ dataSet+"\n");
                }


             }, null);

            /* Install handler to control access to control blocks (RCBs, LCBs, GoCBs, SVCBs, SGCBs)*/
            bool ControlBlockAccessCallBack(object parameter, ClientConnection connection, ACSIClass acsiClass, LogicalDevice ld, LogicalNode ln, string objectName, string subObjectName, ControlBlockAccessType accessType)
            {
                string password = parameter as string;
                object securityToken = connection.GetSecurityToken();

                if(securityToken != null)
                {
                    if ((securityToken as string == password))
                        Console.WriteLine("Correct securityToken");
                    else
                        Console.WriteLine("Incorrect securityToken");
                }

                Console.WriteLine(acsiClass.ToString() + " "+ accessType.ToString() + " access " +  ld.GetName() + ln.GetName() +"/"+ objectName + "." + subObjectName + "\n");

                return true;
            }

            iedServer.SetControlBlockAccessHandler(ControlBlockAccessCallBack, "securityToken_password");

            /* By default access to variables with FC=DC and FC=CF is not allowed.
            * This allow to write to simpleIOGenericIO/GGIO1.NamPlt.vendor variable used
            * by iec61850_client_example1.
            */
            iedServer.SetWriteAccessPolicy(FunctionalConstraint.DC, AccessPolicy.ACCESS_POLICY_ALLOW);

            /* Install handler to perform access control on datasets */
            bool dataSetAccessHandler(object parameter, ClientConnection connection, DataSetOperation operation, string datasetRef)
            {
                Console.WriteLine("Data set access: "+ datasetRef+" operation: "+ operation.ToString()  + "\n");

                return true;
            }

            iedServer.SetDataSetAccessHandler(dataSetAccessHandler, iedServer);

            /* Install handler to perform read access control on data model elements
            * NOTE: when read access to a data model element is blocked this will also prevent the client
            * to read the data model element  in a data set or enable a RCB instance that uses a dataset
            * containing the restricted data model element.
            */
            MmsDataAccessError readAccessHandler(LogicalDevice ld, LogicalNode ln, DataObject dataObject, FunctionalConstraint fc, ClientConnection connection, object parameter)
            {
                if( dataObject!= null)
                    Console.WriteLine("Read access to "+ld.GetName() + "/"+ln.GetName() + "."+dataObject.GetName() + "\n");
                else
                    Console.WriteLine("Read access to "+ld.GetName() + "/"+ln.GetName() + "\n");

                if (dataObject == null)
                {
                    if (ln.GetName() == "GGIO1")
                    {
                        return MmsDataAccessError.OBJECT_ACCESS_DENIED;
                    }
                }
                else
                {
                    if (ln.GetName() == "GGIO1" && dataObject.GetName() ==  "AnIn1")
                    {
                        return MmsDataAccessError.OBJECT_ACCESS_DENIED;
                    }
                }

                return MmsDataAccessError.SUCCESS;
            }

            iedServer.SetReadAccessHandler(readAccessHandler, null);

            bool directoryAccessHandler(object parameter, ClientConnection connection, IedServer_DirectoryCategory category, LogicalDevice ld)
            {
                switch (category)
                {
                    case IedServer_DirectoryCategory.DIRECTORY_CAT_LD_LIST:

                        Console.WriteLine("Get list of logical devices from "+ connection.GetPeerAddress()+"\n");
                        break;
                    case IedServer_DirectoryCategory.DIRECTORY_CAT_DATASET_LIST:
                        Console.WriteLine("Get list of datasets for LD "+ ld.GetName() + " from "+ connection.GetPeerAddress()+"\n");
                        break;
                    case IedServer_DirectoryCategory.DIRECTORY_CAT_DATA_LIST:
                        Console.WriteLine("Get list of data for LD " + ld.GetName() + " from " + connection.GetPeerAddress() + "\n");
                        break;
                    case IedServer_DirectoryCategory.DIRECTORY_CAT_LOG_LIST:
                        Console.WriteLine("Get list of logs for LD" + ld.GetName() + " from " + connection.GetPeerAddress() + "\n");
                        return false;
                }

                return true;
            }

            iedServer.SetDirectoryAccessHandler(directoryAccessHandler, null);

            /*SettingGroups*/

            LogicalDevice logicalDevice = (LogicalDevice)iedModel.GetModelNodeByShortObjectReference("GenericIO"); ;
            SettingGroupControlBlock settingGroupControlBlock = logicalDevice.GetSettingGroupControlBlock();

            List<PTOC1Settings> ptoc1Settings = new List<PTOC1Settings>();
            ptoc1Settings.Add(new PTOC1Settings { strVal = 1.0f, opDlTmms = 500, rsDlTmms = 500, rstTms = 500 });
            ptoc1Settings.Add(new PTOC1Settings { strVal = 2.0f, opDlTmms = 1500, rsDlTmms = 2500, rstTms = 750 });
            ptoc1Settings.Add(new PTOC1Settings { strVal = 3.0f, opDlTmms = 500, rsDlTmms = 1500, rstTms = 750 });
            ptoc1Settings.Add(new PTOC1Settings { strVal = 3.5f, opDlTmms = 1250, rsDlTmms = 1750, rstTms = 500 });
            ptoc1Settings.Add(new PTOC1Settings { strVal = 3.75f, opDlTmms = 1250, rsDlTmms = 1750, rstTms = 750 });

            void LoadActiveSgValues(int actSG)
            {
                
                DataAttribute dataAttribute = (DataAttribute)iedModel.GetModelNodeByShortObjectReference("GenericIO/PTOC1.StrVal.setMag.f");
                iedServer.UpdateFloatAttributeValue(dataAttribute, ptoc1Settings[actSG - 1].strVal);
                dataAttribute = (DataAttribute)iedModel.GetModelNodeByShortObjectReference("GenericIO/PTOC1.OpDlTmms.setVal");
                iedServer.UpdateInt32AttributeValue(dataAttribute, ptoc1Settings[actSG - 1].opDlTmms);
                dataAttribute = (DataAttribute)iedModel.GetModelNodeByShortObjectReference("GenericIO/PTOC1.RsDlTmms.setVal");
                iedServer.UpdateInt32AttributeValue(dataAttribute, ptoc1Settings[actSG - 1].rsDlTmms);
                dataAttribute = (DataAttribute)iedModel.GetModelNodeByShortObjectReference("GenericIO/PTOC1.RstTms.setVal");
                iedServer.UpdateInt32AttributeValue(dataAttribute, ptoc1Settings[actSG - 1].rstTms);
            }

            void LoadEditSgValues(int actSG)
            {
                DataObject strVal = (DataObject)iedModel.GetModelNodeByShortObjectReference("GenericIO/PTOC1.StrVal");
                DataAttribute setMagF = strVal.GetChildWithFc("setMag.f", FunctionalConstraint.SE);
                iedServer.UpdateFloatAttributeValue(setMagF, ptoc1Settings[actSG - 1].strVal);

                DataObject opDlTmms = (DataObject)iedModel.GetModelNodeByShortObjectReference("GenericIO/PTOC1.OpDlTmms");
                DataAttribute setVal = opDlTmms.GetChildWithFc("setVal", FunctionalConstraint.SE);
                iedServer.UpdateInt32AttributeValue(setVal, ptoc1Settings[actSG - 1].opDlTmms);

                DataObject rsDlTmms = (DataObject)iedModel.GetModelNodeByShortObjectReference("GenericIO/PTOC1.RsDlTmms");
                DataAttribute rsDlTmms_setVal = rsDlTmms.GetChildWithFc("setVal", FunctionalConstraint.SE);
                iedServer.UpdateInt32AttributeValue(rsDlTmms_setVal, ptoc1Settings[actSG - 1].rsDlTmms);

                DataObject rstTms = (DataObject)iedModel.GetModelNodeByShortObjectReference("GenericIO/PTOC1.RstTms");
                DataAttribute rstTms_setVal = rstTms.GetChildWithFc("setVal", FunctionalConstraint.SE);
                iedServer.UpdateInt32AttributeValue(rstTms_setVal, ptoc1Settings[actSG - 1].rstTms);
            }

            bool activeSGChangedHandler(object parameter, SettingGroupControlBlock sgcb, uint newActSg, ClientConnection connection)
            {
                Console.WriteLine("Switch to setting group "+ newActSg +"\n");

                LoadActiveSgValues(Convert.ToInt32(newActSg));

                return true;
            }

            bool editSGChangedHandler(object parameter, SettingGroupControlBlock sgcb, uint newEditSg, ClientConnection connection)
            {
                Console.WriteLine("Set edit setting group to " + newEditSg + "\n");

                LoadEditSgValues(Convert.ToInt32(newEditSg));

                return true;
            }

            void editSGConfirmationHandler(object parameter, SettingGroupControlBlock sgcb, uint editSg)
            {
                Console.WriteLine("Received edit sg confirm for sg " + editSg + "\n");

                int edit = Convert.ToInt32(editSg);

                DataObject strVal = (DataObject)iedModel.GetModelNodeByShortObjectReference("GenericIO/PTOC1.StrVal");
                DataAttribute setMagF = strVal.GetChildWithFc("setMag.f", FunctionalConstraint.SE);
                MmsValue setMagFValue = iedServer.GetAttributeValue(setMagF);

                DataObject opDlTmms = (DataObject)iedModel.GetModelNodeByShortObjectReference("GenericIO/PTOC1.OpDlTmms");
                DataAttribute setVal = opDlTmms.GetChildWithFc("setVal", FunctionalConstraint.SE);
                MmsValue setValValue = iedServer.GetAttributeValue(setVal);

                DataObject rsDlTmms = (DataObject)iedModel.GetModelNodeByShortObjectReference("GenericIO/PTOC1.RsDlTmms");
                DataAttribute rsDlTmmsSetVal = rsDlTmms.GetChildWithFc("setVal", FunctionalConstraint.SE);
                MmsValue rsDlTmmsSetValValue = iedServer.GetAttributeValue(rsDlTmmsSetVal);

                DataObject rstTms = (DataObject)iedModel.GetModelNodeByShortObjectReference("GenericIO/PTOC1.RstTms");
                DataAttribute rstTmsSetVal = rstTms.GetChildWithFc("setVal", FunctionalConstraint.SE);
                MmsValue rstTmsSetValVaue = iedServer.GetAttributeValue(rstTmsSetVal);

                ptoc1Settings[edit - 1].strVal = setMagFValue.ToFloat();
                ptoc1Settings[edit - 1].opDlTmms = setValValue.ToInt32();
                ptoc1Settings[edit - 1].rsDlTmms = rsDlTmmsSetValValue.ToInt32();
                ptoc1Settings[edit - 1].rstTms = rsDlTmmsSetValValue.ToInt32();

                if (iedServer.GetActiveSettingGroupChangedHandler(sgcb) == edit)
                {
                    LoadActiveSgValues(edit);
                }
            }

            iedServer.SetActiveSettingGroupChangedHandler(activeSGChangedHandler, settingGroupControlBlock, null);
            iedServer.SetEditSettingGroupChangedHandler(editSGChangedHandler, settingGroupControlBlock, null);
            iedServer.SetEditSettingGroupConfirmationHandler(editSGConfirmationHandler, settingGroupControlBlock, null);

            LogicalNode logicalNode = (LogicalNode)iedModel.GetModelNodeByShortObjectReference("GenericIO/LLN0");
            SVControlBlock sampledValuesControlBlock_1 = iedModel.GetSVControlBlock(logicalNode, "SMV1");
            SVControlBlock sampledValuesControlBlock_2 = iedModel.GetSVControlBlock(logicalNode, "SMV2");

            void sVCBEventHandler(SVControlBlock sampledValuesControlBlock, SMVEvent sMVEvent, object parameter)
            {
                Console.WriteLine("control called " + sampledValuesControlBlock.Name + " Event: " + sMVEvent.ToString() + "\n");
            }

            iedServer.SetSVCBHandler(sVCBEventHandler, sampledValuesControlBlock_1, null);
            iedServer.SetSVCBHandler(sVCBEventHandler, sampledValuesControlBlock_2, null);

            
            bool clientAuthenticator (object parameter, AcseAuthenticationParameter authParameter, object securityToken, IsoApplicationReference isoApplicationReference)
            {
                List<string> passwords = parameter as List<string>;

                int aeQualifier = isoApplicationReference.GetAeQualifier();
                ItuObjectIdentifier ituObjectIdentifier = isoApplicationReference.GetApTitle();
                int arcCount = ituObjectIdentifier.GetArcCount();
                ushort[] arc = ituObjectIdentifier.GetArcs();

                Console.WriteLine("ACSE Authenticator:\n");

                string appTitle = "";
                for (int i = 0; i < arcCount; i++)
                {
                    appTitle += arc[i];

                    if (i != (arcCount - 1))
                        appTitle += ".";
                }

                Console.WriteLine("  client ap-title: " + appTitle);

                Console.WriteLine("\n  client ae-qualifier: "+ aeQualifier + " \n");

                IEC61850.AcseAuthenticationMechanism acseAuthenticationMechanism = authParameter.GetAuthMechanism();

                if (acseAuthenticationMechanism == IEC61850.AcseAuthenticationMechanism.ACSE_AUTH_PASSWORD)
                {
                    byte[] passArray = authParameter.GetPasswordByteArray();
                    int passwordLenght = passArray.Length;

                    string password = authParameter.GetPasswordString();

                    if (passwordLenght == passwords.First().Length)
                    {
                        if (password == passwords.First())
                        {
                            securityToken = passwords.First();
                            return true;
                        }
                    }
                    else if (passwordLenght == passwords[1].Length)
                    {
                        if (password == passwords[1])
                        {
                            securityToken = passwords[1];
                            return true;
                        }
                    }
                }

                return false;
            }

            List<string> passwords = new List<string>();
            passwords.Add("user1@testpw");
            passwords.Add("user2@testpw");

            iedServer.SetAuthenticator(clientAuthenticator, passwords);

            iedServer.Start(102);

            if (iedServer.IsRunning())
            {
                Console.WriteLine("Server started");

                GC.Collect();

                DataObject ggio1AnIn1 = (DataObject)iedModel.GetModelNodeByShortObjectReference("GenericIO/GGIO1.AnIn1");

                DataAttribute ggio1AnIn1magF = (DataAttribute)ggio1AnIn1.GetChild("mag.f");
                DataAttribute ggio1AnIn1T = (DataAttribute)ggio1AnIn1.GetChild("t");

                float floatVal = 1.0f;

                while (running)
                {
                    floatVal += 1f;
                    iedServer.UpdateTimestampAttributeValue(ggio1AnIn1T, new Timestamp(DateTime.Now));
                    iedServer.UpdateFloatAttributeValue(ggio1AnIn1magF, floatVal);
                    Thread.Sleep(100);
                }

                iedServer.Stop();
                Console.WriteLine("Server stopped");
            }
            else
            {
                Console.WriteLine("Failed to start server");
            }

            iedServer.Destroy();
        }
    }
}