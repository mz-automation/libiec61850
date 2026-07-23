# README libIEC61850

This file is part of the documentation of **libIEC61850** (version 1.6.2). More documentation can be found online at http://libiec61850.com.

The API documentation can be found here:
* C API: https://support.mz-automation.de/doc/libiec61850/c/latest/
* .NET API: https://support.mz-automation.de/doc/libiec61850/net/latest/

Also consider to review the examples to understand how to use the library.

Content:

* [Overview](#overview)
* [Features](#features)
* [Examples](#examples)
* [Building and running the examples](#building-and-running-the-examples-with-the-provided-makefiles)
* [Building the library with TLS support](#building-the-library-with-tls-support)
* [Installing the library and the API headers](#installing-the-library-and-the-api-headers)
* [Building on Windows with GOOSE support](#building-on-windows-with-goose-support)
* [Building with the cmake build script](#building-with-the-cmake-build-script)
* [Using the log service with sqlite](#using-the-log-service-with-sqlite)
* [C# API](#c-api)
* [Experimental Python bindings](#experimental-python-bindings)
* [Licensing](#commercial-licenses-and-support)
* [Contributing](#contributing)

## Overview

libiec61850 is an open-source (GPLv3) implementation of an IEC 61850 client and server library implementing the protocols MMS, GOOSE, SV, and R-Session. It is implemented in C (according to the C99 standard) to provide maximum portability. It can be used to implement IEC 61850 compliant client and server applications on embedded systems and PCs running Linux, Windows, and MacOS. Included is a set of simple example applications that can be used as a starting point to implement your own IEC 61850 compliant devices or to communicate with IEC 61850 devices. The library has been successfully used in many commercial software products and devices.

For commercial projects, licenses and support are provided by MZ Automation GmbH. Please contact info@mz-automation.de for more details on licensing options.


## Features

The library supports the following IEC 61850 protocol features:

* MMS client/server, GOOSE (IEC 61850-8-1)
* Sampled Values (SV - IEC 61850-9-2)
* Support for buffered and unbuffered reports
* Online report control block configuration
* Data access service (get data, set data)
* Online data model discovery and browsing
* All data set services (get values, set values, browse)
* Dynamic data set services (create and delete)
* Log service
  * Flexible API to connect custom databases
  * Comes with SQLite implementation
* MMS file services (browse, get file, set file, delete/rename file)
  * Required to download COMTRADE files
* Setting group handling
* Support for service tracking
* GOOSE and SV control block handling
* TLS support (IEC 62351-3/4)
* R-Session protocol support (R-GOOSE/R-SMV - IEC 61850-8-1)
* Simple SNTP client (IEC 61850-5) for testing purposes
* C, C#/.NET, and experimental Python APIs


## Examples

The examples are built automatically when CMake is used to build the library.

**NOTE:** Most examples are intended to show a specific function of the library. They are designed to show this function as simple as possible and may lack error handling that must be present in real applications!

## Building and running the examples with the provided makefiles

In the project root directory type:

```bash
make examples
```

If the build succeeds you can find a few binary files in the project's root directory. You can also find a binary version of the library ("libiec61850.a") in the "build" directory.

Run the sample applications in the example folders. For example:

```bash
cd examples/server_example_basic_io
sudo ./server_example_basic_io
```

on the Linux command line.

You can test the server examples by using a generic client or the provided client example applications.

## Building the library with TLS support

The library has an implementation-agnostic interface for TLS configuration.

Currently it comes with two different implementations of this interface:

- mbedtls 2.28 LTS version, which supports TLS versions up to TLS 1.2
- mbedtls 3.6.0 version, which supports TLS 1.2 and 1.3 only

### mbedtls 2.28

1. Download, unpack, and copy mbedtls-2.28 into the `third_party/mbedtls` folder.

**NOTE:** The current version supports mbedtls version 2.28. When you download the source archive from https://tls.mbed.org/ you have to rename the extracted folder to "mbedtls-2.28".

2. In the main libiec61850 folder run:

```bash
make WITH_MBEDTLS=1
```

When using CMake, the library is built automatically with TLS support when the folder `third_party/mbedtls/mbedtls-2.28` is present.

### mbedtls 3.6

Alternatively, you can use mbedtls 3.6.

1. Download, unpack, and copy mbedtls-3.6.0 into the `third_party/mbedtls` folder

2. In the main libiec61850 folder run:

```bash
make WITH_MBEDTLS3=1
```

When using CMake, the library is built automatically with TLS support when the folder `third_party/mbedtls/mbedtls-3.6.0` is present.


## Installing the library and the API headers

The make and cmake build scripts provide an install target. This target copies the API header files and the static library to a single directory for the headers (`INSTALL_PREFIX/include`) and the static library (`INSTALL_PREFIX/lib`). This feature makes it easier to integrate libiec61850 into an external application since you only have to add a simple include directory to the build tool of your choice.

This can be invoked with:

```bash
make install
```

The default install directory for the make build script is ".install".

You can modify this by setting the INSTALL_PREFIX environment variable, for example:

```bash
make INSTALL_PREFIX=/usr/local install
```

For the cmake build script you have to provide the CMAKE_INSTALL_PREFIX variable.


## Building on Windows with GOOSE support

To build the library and run libiec61850 applications with GOOSE support on Windows (10/11), a third-party library (WinPcap) is required. This is necessary because current versions of Windows have no working support for raw sockets. You can download WinPcap here: http://www.winpcap.org.

1. Download and install WinPcap. Make sure that the WinPcap driver is loaded at boot time (you can choose this option at the last screen of the WinPcap installer).
2. Reboot the system (you can also do this later, but you need to reboot or load the WinPcap driver before running any libiec61850 applications that use GOOSE).
3. Download the WinPcap developers pack from here: http://www.winpcap.org/install/bin/WpdPack_4_1_2.zip
4. Unpack the zip file. Copy the folders `Lib` and `Include` from the WpdPack directory to the `third_party/winpcap` directory of libiec61850.


## Building with the cmake build script

### Build on Linux

You have to install the build tools and cmake (e.g., the packages "build-essential" and "cmake" on Ubuntu 20.04).

Execute the following commands:

```bash
mkdir build
cd build
cmake ..
make
```

Optionally execute the following to install the library and header files in system directories:

```bash
sudo make install
```

### Optional test-only instrumentation

For deterministic testing (e.g., of MMS file ObtainFile request timeout behavior), the library provides a test-only instrumentation API that can be used to modify the standard behavior. This is disabled by default and excluded from production builds.

Enable it via CMake:

```bash
cmake -DLIB61850_ENABLE_TEST_API=ON ..
```

When enabled, some test-specific API functions are available (exported with the `LIB61850_TEST_API` attribute).

Usage example:

```c
#ifdef LIB61850_ENABLE_TEST_API
  /* Introduce 250 ms delay before each FileRead response */
  MmsConnection_setFileReadArtificialDelay(conn, 250);
#endif
```

If the option is OFF, the symbols and related code are omitted entirely and have zero impact on runtime behavior or memory footprint.


### Build on Windows with Visual Studio

With the help of the CMake build script it is possible to create platform-independent project descriptions and let CMake generate specific project or build files for other tools like Make or Visual Studio.

If you have CMake installed, open a command line (cmd.exe) and create a new subdirectory in the libiec61850 folder. Change to this subdirectory. Then you can invoke CMake. As a command line argument you have to supply a "generator" that is used by CMake to create the project file for the actual build tool.

To create a "solution" for Visual Studio 2015 (32-bit):

```bash
cmake -G "Visual Studio 14 2015" ..
```

To build 64-bit libraries, the "Win64" generator option has to be added:

```bash
cmake -G "Visual Studio 14 2015 Win64" ..
```

**Note:** The ".." at the end of the command line tells CMake where to find the main build script file (called CMakeLists.txt). This should point to the folder libiec61850, which in our case is the parent directory (..).

Depending on the system, you don't have to provide a generator to the CMake command.

To select some configuration options you can use ccmake or cmake-gui.

For newer versions of Visual Studio you can use one of the following commands (for 64-bit builds):

**Visual Studio 2017:**

```bash
cmake -G "Visual Studio 15 2017 Win64" ..
```

**Visual Studio 2019:**

```bash
cmake -G "Visual Studio 16 2019" .. -A x64
```

**Visual Studio 2022:**

```bash
cmake -G "Visual Studio 17 2022" .. -A x64
```


## Using the log service with sqlite

The library provides support for the IEC 61850 log service. It provides an abstract interface for a logging database. Included is a driver for using SQLite for logging. This driver can be seen as an example of how to use the abstract logging interface.

You can use the driver by including the `src/logging/drivers/sqlite/log_storage_sqlite.c` file into your application build.

On Ubuntu Linux (and similar Linux distributions), it is enough to install the SQLite dev packages from the standard repository. For other OSes (e.g., Windows) and cross-compiling, it is recommended to download the amalgamation source code (from https://www.sqlite.org/download.html) of SQLite and copy it to the `third_party/sqlite` folder.

On Windows, the CMake script will detect the SQLite source code and also create the example project for logging.


## C# API

A C#/.NET wrapper, examples, and Visual Studio/MonoDevelop project files can be found in the `dotnet` folder. The examples and the C# wrapper API can be built and run on .NET or Mono.


## Experimental Python bindings

The experimental Python binding can be created using SWIG with CMake.

To enable the bindings you have to select the Python configuration option with ccmake or cmake-gui.

**Note:** We do not provide any official support for the Python bindings!


## Commercial licenses and support

Support and commercial license options are provided by MZ Automation GmbH. Please contact info@mz-automation.de for more details.

## Contributing

If you want to contribute to the improvement and development of the library, please send comments, feature requests, bug reports, or patches.

More than trivial contributions require you to sign a Contributor License Agreement. Please contact info@libiec61850.com before you plan such a contribution.

**Please note:** Pull requests to the GitHub repository (https://github.com/mz-automation/libiec61850) are not accepted. The GitHub repository serves as a read-only archive.
