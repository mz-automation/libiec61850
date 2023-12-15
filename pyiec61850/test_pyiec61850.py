#!/usr/bin/python
import sys
import time
import threading
import traceback
import signal
import sys
sys.path.append('.')
import pyiec61850 as iec61850
def signal_handler(signal, frame):
	global running
	running =0
	print('You pressed Ctrl+C!')
	#sys.exit(0)
signal.signal(signal.SIGINT, signal_handler)
tcpPort = 8102
running = 1
class myIECServer():
	def __init__(self):
		self.__model = iec61850.IedModel_create("testmodel")
		lDevice1 = iec61850.LogicalDevice_create("SENSORS", self.__model);
		lln0 = iec61850.LogicalNode_create("LLN0", lDevice1);
		ttmp1 = iec61850.LogicalNode_create("TTMP1", lDevice1);
		iec61850.CDC_SAV_create("TmpSv",  iec61850.toModelNode(ttmp1), 0, False)
		iec61850.CDC_ASG_create("TmpSp",  iec61850.toModelNode(ttmp1), 0, False)
		self.__iedServer = iec61850.IedServer_create(self.__model)
		iec61850.IedServer_start(self.__iedServer, tcpPort);
		if not(iec61850.IedServer_isRunning(self.__iedServer)) :
			print("Starting server failed! Exit.\n")
			iec61850.IedServer_destroy(self.__iedServer)
			sys.exit(-1)
	def run(self):
		global running
		while running:
			time.sleep(0.1)
		self.stop()
	def stop(self):
		iec61850.IedServer_stop(self.__iedServer)
		iec61850.IedServer_destroy(self.__iedServer)
		iec61850.IedModel_destroy(self.__model)
	
def testClient():
	con = iec61850.IedConnection_create()
	error = iec61850.IedConnection_connect(con, "localhost", tcpPort)
	if (error == iec61850.IED_ERROR_OK):
		# Accessing to SAV values
		theVal = "testmodelSENSORS/TTMP1.TmpSv.instMag.f"
		theValType = iec61850.IEC61850_FC_MX
		temperatureValue = iec61850.IedConnection_readFloatValue(con, theVal, theValType)
		assert(temperatureValue[1]==0)
		newValue= temperatureValue[0]+10
		err = iec61850.IedConnection_writeFloatValue(con, theVal, theValType, newValue)
		assert(err==21)
		# Accessing to ASG values
		theVal = "testmodelSENSORS/TTMP1.TmpSp.setMag.f"
		theValType = iec61850.IEC61850_FC_SP
		temperatureSetpoint = iec61850.IedConnection_readFloatValue(con, theVal, theValType)
		print(temperatureSetpoint)
		assert(temperatureValue[1]==0)
		newValue= temperatureValue[0]+10
		err = iec61850.IedConnection_writeFloatValue(con, theVal, theValType, newValue)
		assert(err==0)
		temperatureSetpoint = iec61850.IedConnection_readFloatValue(con, theVal, theValType)
		print(temperatureSetpoint)
		assert(temperatureSetpoint[0]==newValue)
		iec61850.IedConnection_close(con)
	else:
		print("Connection error")
		sys.exit(-1)
	iec61850.IedConnection_destroy(con)
	print("client ok")
try:
	srv=myIECServer()
	srvThread = threading.Thread(target = srv.run)
	srvThread.start()
	testClient()
	running = 0
	#signal.pause()
except:
	running = 0
	print("Error :")
	traceback.print_exc(file=sys.stdout)
	sys.exit(-1)
