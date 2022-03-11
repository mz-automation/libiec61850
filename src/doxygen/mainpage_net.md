# API Documentation

Copyright 2022 Michael Zillgith

This is the documentation for IEC61850.NET a .NET wrapper for the libiec61850 IEC 61850 protocol library.

##Building the library and the .NET wrapper

### Build the library (native DLL or .so file)

*CMake is required*

Example on linux:

1) create a build folder

mkdir build

2) configure and create the makefiles

```
cd build
cmake ..
```

3) Build the library

```
make
```

Example for Visual Studio 2015 (see cmake --help for other "generator" options)

3a) For 32 bit build

```
cd build
cmake -G "Visual Studio 14 2015" ..
```

3b) For 64 bit build

```
cd build
cmake -G "Visual Studio 14 2015 Win64" ..
```

4) Installing the native DLL/shared library

Put the library in a folder where the OS can find it:

e.g /usr/lib for Linux or C:/Windows/System32 for Windows.

### Build the .NET wrapper:


Open the dotnet.sln solution file in the dotnet folder with Visual Studio or MonoDevelop.

You can build all the targets there or only the IEC61850.NET project.
