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
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../third_party/winpcap/Include")
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

set (libhal_macos_SRCS
 ${CMAKE_CURRENT_LIST_DIR}/socket/bsd/socket_bsd.c
 ${CMAKE_CURRENT_LIST_DIR}/ethernet/bsd/ethernet_bsd.c
 ${CMAKE_CURRENT_LIST_DIR}/thread/macos/thread_macos.c
 ${CMAKE_CURRENT_LIST_DIR}/filesystem/linux/file_provider_linux.c
 ${CMAKE_CURRENT_LIST_DIR}/time/unix/time.c
 ${CMAKE_CURRENT_LIST_DIR}/memory/lib_memory.c
)

IF(WIN32)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/winpcap/Lib/wpcap.lib")
message("Found winpcap -> can compile with GOOSE support")
set(WITH_WPCAP 1)
endif()

set (libhal_SRCS
    ${libhal_windows_SRCS}
)

IF(MSVC)
set_source_files_properties(${libhal_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF()

ELSEIF(UNIX)
IF(APPLE)
set (libhal_SRCS
    ${libhal_macos_SRCS}
)
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
set (libhal_SRCS
    ${libhal_bsd_SRCS}
)
ELSE()
set (libhal_SRCS
    ${libhal_linux_SRCS}
)
ENDIF(APPLE)
ENDIF(WIN32)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC" )
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC" )

if(WITH_MBEDTLS)
message("Found mbedtls -> can compile HAL with TLS support")
set(WITH_MBEDTLS 1)
endif(WITH_MBEDTLS)

if(WITH_MBEDTLS)
include_directories(
	${CMAKE_CURRENT_LIST_DIR}/tls/mbedtls
    ${MBEDTLS_INCLUDE_DIR}
)

if(CONFIG_USE_EXTERNAL_MBEDTLS_DYNLIB)
link_directories(${CONFIG_EXTERNAL_MBEDTLS_DYNLIB_PATH})
else()
file(GLOB tls_SRCS ${CMAKE_CURRENT_LIST_DIR}/../third_party/mbedtls/mbedtls-2.16/library/*.c)
endif(CONFIG_USE_EXTERNAL_MBEDTLS_DYNLIB)

add_definitions(-DMBEDTLS_CONFIG_FILE="mbedtls_config.h")

set (libhal_SRCS ${libhal_SRCS}
  ${CMAKE_CURRENT_LIST_DIR}/tls/mbedtls/tls_mbedtls.c
)

IF(MSVC)
set_source_files_properties(${libhal_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF()

list (APPEND libhal_SRCS ${tls_SRCS})

endif(WITH_MBEDTLS)

add_library (hal STATIC ${libhal_SRCS})

add_library (hal-shared STATIC ${libhal_SRCS})

target_compile_definitions(hal-shared PRIVATE EXPORT_FUNCTIONS_FOR_DLL)

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

IF(CONFIG_USE_EXTERNAL_MBEDTLS_DYNLIB)
  target_link_libraries(hal mbedcrypto mbedx509 mbedtls)
ENDIF(CONFIG_USE_EXTERNAL_MBEDTLS_DYNLIB)

IF(MINGW)
  target_link_libraries(hal ws2_32 iphlpapi)
ENDIF(MINGW)

iF(WITH_WPCAP)
target_link_libraries(hal
	${CMAKE_CURRENT_SOURCE_DIR}/../third_party/winpcap/Lib/wpcap.lib
	${CMAKE_CURRENT_SOURCE_DIR}/../third_party/winpcap/Lib/packet.lib
)
ENDIF(WITH_WPCAP)

set(BINDIR "bin")
set(LIBDIR "lib")
if(UNIX)
    # GNUInstallDirs is required for Debian multiarch
    include(GNUInstallDirs)
    set(LIBDIR ${CMAKE_INSTALL_LIBDIR})
    set(BINDIR ${CMAKE_INSTALL_BINDIR})
endif()

install (TARGETS hal hal-shared
	RUNTIME DESTINATION ${BINDIR} COMPONENT Applications
	ARCHIVE DESTINATION ${LIBDIR} COMPONENT Libraries
    LIBRARY DESTINATION ${LIBDIR} COMPONENT Libraries
)
