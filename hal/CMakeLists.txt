cmake_minimum_required(VERSION 2.8)

# automagically detect if we should cross-compile
if(DEFINED ENV{TOOLCHAIN})
    set(CMAKE_C_COMPILER        $ENV{TOOLCHAIN}gcc)
    set(CMAKE_CXX_COMPILER      $ENV{TOOLCHAIN}g++)
    set(CMAKE_AR        "$ENV{TOOLCHAIN}ar" CACHE FILEPATH "CW archiver" FORCE)
endif()

project(hal)

set(LIBHAL_VERSION_MAJOR "2")
set(LIBHAL_VERSION_MINOR "0")
set(LIBHAL_VERSION_PATCH "0")

# feature checks
include(CheckLibraryExists)
check_library_exists(rt clock_gettime "time.h" CONFIG_SYSTEM_HAS_CLOCK_GETTIME)

# check if we are on a little or a big endian
include (TestBigEndian)
test_big_endian(PLATFORM_IS_BIGENDIAN)

if(WIN32)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/winpcap/Lib/wpcap.lib")
message("Found winpcap -> compile ethernet HAL layer (required for GOOSE/SV support)")
set(WITH_WPCAP 1)
else()
message("winpcap not found -> skip ethernet HAL layer (no GOOSE/SV support)")
endif()

endif(WIN32)

include_directories(
    ${CMAKE_CURRENT_LIST_DIR}/inc
)

set (libhal_linux_SRCS
 ${CMAKE_CURRENT_LIST_DIR}/socket/linux/socket_linux.c
 ${CMAKE_CURRENT_LIST_DIR}/ethernet/linux/ethernet_linux.c
 ${CMAKE_CURRENT_LIST_DIR}/thread/linux/thread_linux.c
 ${CMAKE_CURRENT_LIST_DIR}/filesystem/linux/file_provider_linux.c
 ${CMAKE_CURRENT_LIST_DIR}/time/unix/time.c
 ${CMAKE_CURRENT_LIST_DIR}/serial/linux/serial_port_linux.c
 ${CMAKE_CURRENT_LIST_DIR}/memory/lib_memory.c
)

set (libhal_windows_SRCS
 ${CMAKE_CURRENT_LIST_DIR}/socket/win32/socket_win32.c
 ${CMAKE_CURRENT_LIST_DIR}/thread/win32/thread_win32.c
 ${CMAKE_CURRENT_LIST_DIR}/filesystem/win32/file_provider_win32.c
 ${CMAKE_CURRENT_LIST_DIR}/time/win32/time.c
 ${CMAKE_CURRENT_LIST_DIR}/serial/win32/serial_port_win32.c
 ${CMAKE_CURRENT_LIST_DIR}/memory/lib_memory.c
)

if(WITH_WPCAP)
set (libhal_windows_SRCS ${libhal_windows_SRCS}
 ${CMAKE_CURRENT_LIST_DIR}/ethernet/win32/ethernet_win32.c
)
endif(WITH_WPCAP)

set (libhal_bsd_SRCS
 ${CMAKE_CURRENT_LIST_DIR}/socket/bsd/socket_bsd.c
 ${CMAKE_CURRENT_LIST_DIR}/ethernet/bsd/ethernet_bsd.c
 ${CMAKE_CURRENT_LIST_DIR}/thread/bsd/thread_bsd.c
 ${CMAKE_CURRENT_LIST_DIR}/filesystem/linux/file_provider_linux.c
 ${CMAKE_CURRENT_LIST_DIR}/time/unix/time.c
 ${CMAKE_CURRENT_LIST_DIR}/memory/lib_memory.c
)

IF(WIN32)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/winpcap/Lib/wpcap.lib")
message("Found winpcap -> can compile with GOOSE support")
set(WITH_WPCAP 1)
endif()

IF(MSVC)
set_source_files_properties(${libhal_windows_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF()

set (libhal_SRCS
    ${libhal_windows_SRCS}
)

ELSEIF(UNIX)
IF(APPLE)
set (libhal_SRCS
    ${libhal_bsd_SRCS}
)
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
set (libhal_SRCS
    ${libhal_bsd_SRCS}}
)
ELSE()
set (libhal_SRCS
    ${libhal_linux_SRCS}
)
ENDIF(APPLE)
ENDIF(WIN32)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC" )
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC" )

if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/../third_party/mbedtls/mbedtls-2.6.0)
message("Found mbedtls -> can compile with TLS support")
set(WITH_MBEDTLS 1)
endif(EXISTS ${CMAKE_CURRENT_LIST_DIR}/../third_party/mbedtls/mbedtls-2.6.0)

if(WITH_MBEDTLS)
include_directories(
	${CMAKE_CURRENT_LIST_DIR}/tls/mbedtls
    ${CMAKE_CURRENT_LIST_DIR}/../third_party/mbedtls/mbedtls-2.6.0/include
)

file(GLOB tls_SRCS ${CMAKE_CURRENT_LIST_DIR}/../third_party/mbedtls/mbedtls-2.6.0/library/*.c)

add_definitions(-DMBEDTLS_CONFIG_FILE="mbedtls_config.h")

set (libhal_SRCS ${libhal_SRCS}
 ${CMAKE_CURRENT_LIST_DIR}/tls/mbedtls/tls_mbedtls.c
)

list (APPEND libhal_SRCS ${tls_SRCS})

endif(WITH_MBEDTLS)

add_library (hal STATIC ${libhal_SRCS})

add_library (hal-shared STATIC ${libhal_SRCS})

SET_TARGET_PROPERTIES(hal-shared PROPERTIES
  COMPILE_FLAGS "-fPIC"
)

IF(UNIX)
  IF (CONFIG_SYSTEM_HAS_CLOCK_GETTIME)
     target_link_libraries (hal
         -lpthread
         -lrt
     )
  ELSE ()
     target_link_libraries (hal
         -lpthread
     )
  ENDIF (CONFIG_SYSTEM_HAS_CLOCK_GETTIME)
ENDIF(UNIX)
IF(MINGW)
  target_link_libraries(hal ws2_32 iphlpapi)
ENDIF(MINGW)

iF(WITH_WPCAP)
target_link_libraries(hal
   ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/winpcap/lib/wpcap.lib
   ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/winpcap/lib/packet.lib
)
ENDIF(WITH_WPCAP)
