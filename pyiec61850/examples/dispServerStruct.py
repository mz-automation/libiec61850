#!/usr/bin/python
import os,sys
import iec61850
if __name__=="__main__":
	hostname = "localhost";
	tcpPort = 102
	if len(sys.argv)>1:
		hostname = sys.argv[1]
	if len(sys.argv)>2:
		port = sys.argv[2]
	con = iec61850.IedConnection_create()
	error = iec61850.IedConnection_connect(con, hostname, tcpPort)
	if (error == iec61850.IED_ERROR_OK):
		[deviceList, error] = iec61850.IedConnection_getLogicalDeviceList(con)
		device = iec61850.LinkedList_getNext(deviceList)
		while device:
			LD_name=iec61850.toCharP(device.data)
			print("LD: %s" % LD_name)
			[logicalNodes, error] = iec61850.IedConnection_getLogicalDeviceDirectory(con, LD_name)
			logicalNode = iec61850.LinkedList_getNext(logicalNodes)
			while logicalNode:
				LN_name=iec61850.toCharP(logicalNode.data)
				print(" LN: %s" % LN_name)
				[LNobjects, error] = iec61850.IedConnection_getLogicalNodeVariables(con, LD_name+"/"+LN_name)
				LNobject = iec61850.LinkedList_getNext(LNobjects)
				while LNobject:
					print("  DO: %s" % iec61850.toCharP(LNobject.data))
					LNobject = iec61850.LinkedList_getNext(LNobject)
				iec61850.LinkedList_destroy(LNobjects)
				logicalNode = iec61850.LinkedList_getNext(logicalNode)
			iec61850.LinkedList_destroy(logicalNodes)
			device = iec61850.LinkedList_getNext(device)
		iec61850.LinkedList_destroy(deviceList)
		iec61850.IedConnection_close(con)
	else:
		print("Failed to connect to %s:%i\n"%(hostname, tcpPort))
	iec61850.IedConnection_destroy(con)
