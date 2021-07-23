UNAME := $(shell uname)

MIPSEL_TOOLCHAIN_PREFIX=mipsel-openwrt-linux-
#ARM_TOOLCHAIN_PREFIX=arm-linux-
#ARM_TOOLCHAIN_PREFIX=arm-linux-gnueabi-
#ARM_TOOLCHAIN_PREFIX=arm-poky-linux-gnueabi-
ARM_TOOLCHAIN_PREFIX=arm-linux-gnueabihf-
UCLINUX_ARM_TOOLCHAIN_PREFIX=arm-uclinux-elf-
UCLINUX_XPORT_TOOLCHAIN_PREFIX=m68k-uclinux-
MINGW_TOOLCHAIN_PREFIX=i586-mingw32msvc-
#MINGW_TOOLCHAIN_PREFIX=x86_64-w64-mingw32-
MINGW64_TOOLCHAIN_PREFIX=x86_64-w64-mingw32-

#POWERPC_TOOLCHAIN_PREFIX=powerpc-poky-linux-
POWERPC_TOOLCHAIN_PREFIX=powerpc-linux-gnu-

ifndef TARGET
ifeq ($(UNAME), Linux)
TARGET=POSIX
else ifeq ($(findstring MINGW,$(UNAME)), MINGW)
TARGET=WIN32
else ifeq ($(UNAME), Darwin)
TARGET=MACOS
else ifeq ($(UNAME), FreeBSD)
TARGET=BSD
endif
endif

ifeq ($(TARGET), WIN32)
WINDOWS=1
endif

ifeq ($(TARGET), WIN64)
WINDOWS=1
endif

ifdef WINDOWS
ifeq ($(UNAME), Linux)
ifeq ($(TARGET), WIN32)
TOOLCHAIN_PREFIX=$(MINGW_TOOLCHAIN_PREFIX)
endif
ifeq ($(TARGET), WIN64)
TOOLCHAIN_PREFIX=$(MINGW64_TOOLCHAIN_PREFIX)
endif
else
TOOLCHAIN_PREFIX=
endif
endif

ifeq ($(TARGET), LINUX-POWERPC)
TOOLCHAIN_PREFIX=$(POWERPC_TOOLCHAIN_PREFIX)
endif

ifeq ($(TARGET), LINUX-MIPSEL)
TOOLCHAIN_PREFIX=$(MIPSEL_TOOLCHAIN_PREFIX)
endif

ifeq ($(TARGET), LINUX-ARM)
TOOLCHAIN_PREFIX=$(ARM_TOOLCHAIN_PREFIX)
#CFLAGS += -mno-unaligned-access
#CFLAGS += -mcpu=arm926ej-s
endif

ifeq ($(TARGET), UCLINUX-XPORT)
TOOLCHAIN_PREFIX=$(UCLINUX_XPORT_TOOLCHAIN_PREFIX)
CFLAGS += -DPLATFORM_BYTE_ORDER
CFLAGS += -mcpu=5208  
CFLAGS += -fno-builtin -fno-common 
CFLAGS += -fno-dwarf2-cfi-asm -msep-data -DCONFIG_COLDFIRE -D__linux__ -Dunix -D__uClinux__
endif

ifeq ($(TARGET), UCLINUX-WAGO)
TOOLCHAIN_PREFIX=$(UCLINUX_ARM_TOOLCHAIN_PREFIX)
CFLAGS += -msoft-float
CFLAGS += -Wall
CFLAGS += -DEMBED
CFLAGS += -Dlinux -D__linux__ -Dunix
CFLAGS += -D__uClinux__
CFLAGS += -DTARGET=UCLINUX-WAGO
LDFLAGS += -Wl,-move-rodata -Wl,-elf2flt
endif

ifdef WINDOWS
HAL_IMPL = WIN32
LIB_OBJS_DIR = $(LIBIEC_HOME)/build_win32
CFLAGS=-g -DWIN32

ifeq ($(TARGET), WIN32)
CFLAGS+=-m32
endif

ifeq ($(TARGET), WIN64)
CFLAGS+=-m64
endif

LDLIBS=-lws2_32
DYNLIB_LDFLAGS=-Wl,-no-undefined -Wl,--enable-runtime-pseudo-reloc -Wl,--out-implib,libiec61850.a


# on Windows: only compile with ethernet support if winpcap files are in third_party/winpcap!
ifneq (, $(wildcard  $(LIBIEC_HOME)/third_party/winpcap/Include/.))

ifeq ($(TARGET), WIN64)
LDFLAGS += -L$(LIBIEC_HOME)/third_party/winpcap/Lib/x64
else
LDFLAGS += -L$(LIBIEC_HOME)/third_party/winpcap/Lib
endif

LDLIBS+=-liphlpapi -lwpcap
else
$(warning winpcap not found - will build without GOOSE support!)
CFLAGS += -DEXCLUDE_ETHERNET_WINDOWS
EXCLUDE_ETHERNET_WINDOWS = 1
endif


else 
ifeq ($(TARGET), BSD)
HAL_IMPL = BSD
else ifeq ($(TARGET), MACOS)
HAL_IMPL = MACOS
else
HAL_IMPL = POSIX
endif



LDLIBS = -lpthread

ifeq ($(TARGET), LINUX-MIPSEL)
LIB_OBJS_DIR = $(LIBIEC_HOME)/build-mipsel
else ifeq ($(TARGET), LINUX-ARM)
LIB_OBJS_DIR = $(LIBIEC_HOME)/build-arm
else ifeq ($(TARGET), UCLINUX-WAGO)
LIB_OBJS_DIR = $(LIBIEC_HOME)/build-wago
CFLAGS += -DTARGET_SYSTEM_UCLINUX_WAGO
else ifeq ($(TARGET), LINUX-POWERPC)
LIB_OBJS_DIR = $(LIBIEC_HOME)/build-powerpc
else
LIB_OBJS_DIR = $(LIBIEC_HOME)/build
endif

CFLAGS += -g 
#CFLAGS += -Os

DYNLIB_LDFLAGS=-lpthread
endif

ifneq ($(TARGET), CLANG-CHECK)
CC=$(TOOLCHAIN_PREFIX)gcc
CPP=$(TOOLCHAIN_PREFIX)g++
endif

ifeq ($(TARGET), BSD)
CC=cc
CPP=c++
endif

AR=$(TOOLCHAIN_PREFIX)ar
RANLIB=$(TOOLCHAIN_PREFIX)ranlib


ifeq ($(TARGET), WIN32)
PROJECT_BINARY_NAME := $(PROJECT_BINARY_NAME).exe
endif

LIB_NAME = $(LIB_OBJS_DIR)/libiec61850.a

ifeq ($(TARGET), BSD)
CFLAGS += -arch i386
LDFLAGS += -arch i386
endif

ifdef WINDOWS
DYN_LIB_NAME = $(LIB_OBJS_DIR)/iec61850.dll
else 

ifeq ($(TARGET), BSD)
DYN_LIB_NAME = $(LIB_OBJS_DIR)/libiec61850.dylib
else
ifeq ($(TARGET), MACOS)
DYN_LIB_NAME = $(LIB_OBJS_DIR)/libiec61850.dylib
else
DYN_LIB_NAME = $(LIB_OBJS_DIR)/libiec61850.so
endif
endif

endif
