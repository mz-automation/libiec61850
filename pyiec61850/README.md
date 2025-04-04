# Python wrapper for libIEC61850

## Building
Before building you should install SWIG and Python
(see the '[Setup development environment on Linux](#setup-development-environment-on-linux-ubuntu)' section for help).

To build the Python bindings you have to turn on the BUILD\_PYTHON\_BINDINGS flag in CMake from cmake-gui or in command line:
```sh
$ mkdir build && cd build
$ cmake -DBUILD_PYTHON_BINDINGS=ON ..
```

Then compile the library and install it:
```sh
$ make
$ sudo make install
```
(Eventually, update your ld cache with: `sudo ldconfig`)

CMake and SWIG will automatically detect your Python version and install the Python library in Python library directories.

For running the integrated tests:
```sh
$ make test
```

pyiec61850 library is to be imported calling:
```python
import pyiec61850 as iec61850
```

## Client tutorial

The Python bindings works similarly to the basic C library. However there are some differences:

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

## Appendix

## Setup development environment on Linux Ubuntu

_[Tested on Ubuntu 20.04 LTS, Ubuntu 22.04 LTS, Ubuntu 23.10]_

Here the minimum required packages for compiling libiec61850 and the Python
wrapper (without TLS, SQlite, ...):

```sh
$ sudo apt-get update
$ sudo apt-get install g++ cmake swig git python3 python3-all-dev
```

## Setup development environment on Linux Alpine

_[Tested on Alpine 3.18]_

Here the minimum required packages for compiling libiec61850 and the Python
wrapper (without TLS, SQlite, ...):

```sh
$ apk update
$ apk add git g++ swig make cmake python3 python3-dev linux-headers
```
