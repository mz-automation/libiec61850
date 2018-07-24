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
)

set (libhal_windows_SRCS
 ${CMAKE_CURRENT_LIST_DIR}/socket/win32/socket_win32.c
 ${CMAKE_CURRENT_LIST_DIR}/ethernet/win32/ethernet_win32.c
 ${CMAKE_CURRENT_LIST_DIR}/thread/win32/thread_win32.c
 ${CMAKE_CURRENT_LIST_DIR}/filesystem/win32/file_provider_win32.c
 ${CMAKE_CURRENT_LIST_DIR}/time/win32/time.c
 ${CMAKE_CURRENT_LIST_DIR}/serial/win32/serial_port_win32.c
)

set (libhal_bsd_SRCS
 ${CMAKE_CURRENT_LIST_DIR}/socket/bsd/socket_bsd.c
 ${CMAKE_CURRENT_LIST_DIR}/ethernet/bsd/ethernet_bsd.c
 ${CMAKE_CURRENT_LIST_DIR}/thread/bsd/thread_bsd.c
 ${CMAKE_CURRENT_LIST_DIR}/filesystem/linux/file_provider_linux.c
 ${CMAKE_CURRENT_LIST_DIR}/time/unix/time.c
)

IF(WIN32)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/winpcap/Lib/wpcap.lib")
message("Found winpcap -> can compile with GOOSE support")
set(WITH_WPCAP 1)
endif()



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
