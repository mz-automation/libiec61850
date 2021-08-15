# Building
Before building you should install swig and java.

From the root directory of the libiec61850 project you have to build some of the dependant libraries

> cd ..
>
> make clean
>
> make INSTALL_PREFIX=/usr/local install
>
> make clean
>
> make INSTALL_PREFIX=/usr/local dynlib

Now change into the Java project subdirectory

> cd javaiec61850
>
> swig -java -I/usr/local/include iec61850.i
>
> gcc -fpic -I/usr/lib/jvm/default-java/include -I/usr/lib/jvm/default-java/include/linux -c iec61850_wrap.c
>
> ld -shared ../build/libiec61850.so iec61850_wrap.o -o libiec61850.so
>
> javac test_javaiec61850.java
>
> java test_javaiec61850

This test programme should show an out put of

```sh
MyThread for IEC61850Server has STARTED
IEC61850Server has STARTED
IEC61850Client has connected to the IEC61850Server
IEC61850Client has read testmodelSENSORS
IEC61850Client caught a error = 0
IEC61850Client read a temperature set point of = 0.0
IEC61850Client read a temperature set point of = 10.0
IEC61850Server has been Interrupted and STOPPED

```

The CMakeLists file does not exist yet, however it would be a nice to have a build of the java bindings by turning on a BUILD\_JAVA\_BINDINGS flag in CMake from cmake-gui or in command line:
```sh
$ cmake -DBUILD_JAVA_BINDINGS=ON .
```
Then compile the library and install it.

javaiec61850 library is to be imported calling
```java
import iec61850
```

# Client tutorial

The java bindings works similarly to the python and basic C library ones.

Have a look at the Client, Server and test programme.

For example to create a connection, call:

```java
  if (con == null) {
    con = iec61850.IedConnection_create();
  }

  iec61850.IedConnection_connect(con, error, "localhost", tcpPort);

  conState = iec61850.IedConnection_getState(con);

  if ( conState.toString() == "IED_STATE_CONNECTED" || conState.toString() == "IED_STATE_IDLE") {
  System.out.println("IEC61850Client has connected to the IEC61850Server" );
  }

```

Copyright Waterford Institute of Technology 2017~2019, Telecommunications Software and Systems Group (TSSG), Author Miguel Ponce de Leon <miguelpdl@tssg.org>
This work is supported by European Union’s Horizon 2020 research and innovation programme under grant agreement No 727481, project RE-SERVE (Re- newables in a Stable Electric Grid)
