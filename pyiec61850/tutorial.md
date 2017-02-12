# Building
Before building you should install swig and python.
To build python bindings you have to turn on the BUILD\_PYTHON\_BINDINGS flag in CMake from cmake-gui or in command line:
```sh
$ cmake -DBUILD_PYTHON_BINDINGS=ON .
```
Then compile the library and install it. CMake and swig will automatically detect your python version and install the python library in python library directories.

pyiec61850 library is to be imported calling 
```python
import iec61850
```
# Client tutorial

The python bindings works similarly to the basic C library. However there are some differences:

* a specific function is to be called to cast variables from one type to another
* arguments passed by pointer are to be removed from arguments and append to the return list


For example to create a connection, call:
```python
	con = iec61850.IedConnection_create()
	error = iec61850.IedConnection_connect(con, "localhost", 102)
	if (error == iec61850.IED_ERROR_OK):
		# Do some work
		iec61850.IedConnection_close(con)
	iec61850.IedConnection_destroy(con)
```

To iterate over a list of logical devices, the code becomes:
```python
[deviceList, error] = iec61850.IedConnection_getLogicalDeviceList(con)
device = iec61850.LinkedList_getNext(deviceList)
 
while device:
	print("LD: %s" % iec61850.toCharP(device.data))
	[logicalNodes, error] = iec61850.IedConnection_getLogicalDeviceDirectory(
				con, iec61850.toCharP(device.data))
	device = iec61850.LinkedList_getNext(device)
iec61850.LinkedList_destroy(deviceList)
```

Reading and writing operations can be performed using this syntax:
```python
[floatValue, error] = iec61850.IedConnection_readFloatValue(con,
	"simpleIOGenericIO/GGIO1.AnIn1.mag.f", iec61850.IEC61850_FC_MX)
err = iec61850.IedConnection_writeFloatValue(con, 
	"simpleIOGenericIO/GGIO1.AnIn1.mag.f", iec61850.IEC61850_FC_MX, 10.0)
```
