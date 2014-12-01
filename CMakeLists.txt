cmake_minimum_required(VERSION 2.8)

# automagically detect if we should cross-compile
if(DEFINED ENV{TOOLCHAIN})
	set(CMAKE_C_COMPILER	$ENV{TOOLCHAIN}gcc)
	set(CMAKE_CXX_COMPILER	$ENV{TOOLCHAIN}g++)
	set(CMAKE_AR	"$ENV{TOOLCHAIN}ar" CACHE FILEPATH "CW archiver" FORCE)
endif()

project(libiec61850)

set(LIB_VERSION_MAJOR "0")
set(LIB_VERSION_MINOR "7")
set(LIB_VERSION_PATCH "8")

# feature checks
include(CheckLibraryExists)
check_library_exists(rt clock_gettime "time.h" CONFIG_SYSTEM_HAS_CLOCK_GETTIME)

# check if we are on a little or a big endian
include (TestBigEndian)
test_big_endian(PLATFORM_IS_BIGENDIAN)

set(CONFIG_MMS_MAXIMUM_PDU_SIZE "65000" CACHE STRING "Configure the maximum size of an MMS PDU (default 65000)"   )
set(CONFIG_MAXIMUM_TCP_CLIENT_CONNECTIONS 5 CACHE STRING "Configure the maximum number of clients allowed to connect to the server")

option(BUILD_EXAMPLES "Build the examples" ON)

# choose the library features which shall be included
option(CONFIG_INCLUDE_GOOSE_SUPPORT "Build with GOOSE support" ON)

option(CONFIG_IEC61850_CONTROL_SERVICE "Build with support for IEC 61850 control features" ON)

option(CONFIG_IEC61850_REPORT_SERVICE "Build with support for IEC 61850 reporting services" ON)

option(CONFIG_ACTIVATE_TCP_KEEPALIVE "Activate TCP keepalive" ON)

set(CONFIG_REPORTING_DEFAULT_REPORT_BUFFER_SIZE "8000" CACHE STRING "Default buffer size for buffered reports in byte" )

# advanced options
option(DEBUG "Enable debugging mode (include assertions)" OFF)
option(DEBUG_COTP "Enable COTP printf debugging" OFF)
option(DEBUG_ISO_SERVER "Enable ISO SERVER printf debugging" OFF)
option(DEBUG_ISO_CLIENT "Enable ISO CLIENT printf debugging" OFF)
option(DEBUG_IED_SERVER "Enable IED SERVER printf debugging" OFF)
option(DEBUG_IED_CLIENT "Enable IED CLIENT printf debugging" OFF)
option(DEBUG_MMS_SERVER "Enable MMS SERVER printf debugging" OFF)
option(DEBUG_MMS_CLIENT "Enable MMS CLIENT printf debugging" OFF)
#mark_as_advanced(DEBUG DEBUG_COTP DEBUG_ISO_SERVER DEBUG_ISO_CLIENT DEBUG_IED_SERVER
#				 DEBUG_IED_CLIENT DEBUG_MMS_SERVER DEBUG_MMS_CLIENT)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}/config
    src/common
    src/goose
    src/hal
    src/hal/ethernet
    src/hal/socket
    src/hal/thread
    src/hal/filesystem
    src/hal/time
    src/iedclient
    src/iedcommon
    src/iedserver
    src/iedserver/impl
    src/iedserver/mms_mapping
    src/iedserver/model
    src/mms/asn1
    src/mms/iso_acse
    src/mms/iso_client
    src/mms/iso_cotp
    src/mms/iso_mms/asn1c
    src/mms/iso_mms/client
    src/mms/iso_mms/common
    src/mms/iso_mms/server
    src/mms/iso_presentation
    src/mms/iso_server
    src/mms/iso_common
    src/mms/iso_session
)

set(API_HEADERS 
	src/hal/time/time_hal.h 
	src/hal/ethernet/ethernet.h 
	src/hal/thread/thread.h 
	src/hal/filesystem/filesystem.h 
	src/common/libiec61850_common_api.h
	src/common/linked_list.h
	src/common/byte_buffer.h
	src/iedclient/iec61850_client.h
	src/iedcommon/iec61850_common.h
	src/iedserver/iec61850_server.h
	src/iedserver/model/model.h
	src/iedserver/model/cdc.h
	src/iedserver/model/dynamic_model.h
	src/iedserver/model/config_file_parser.h
	src/mms/iso_mms/common/mms_value.h
	src/mms/iso_mms/common/mms_common.h
	src/mms/iso_mms/common/mms_types.h
	src/mms/iso_mms/server/mms_device_model.h
	src/mms/iso_mms/server/mms_server.h
	src/mms/iso_mms/server/mms_named_variable_list.h
	src/mms/iso_mms/common/mms_type_spec.h
	src/mms/asn1/ber_integer.h
	src/mms/asn1/asn1_ber_primitive_value.h
	src/mms/iso_server/iso_server.h
	src/mms/iso_common/iso_connection_parameters.h
	src/goose/goose_subscriber.h
    src/mms/iso_mms/client/mms_client_connection.h
    src/mms/iso_client/iso_client_connection.h
    src/hal/socket/socket.h 
)

IF(WIN32)
include_directories(
    src/vs
)
ENDIF(WIN32)

# write the detected stuff to this file
configure_file(config/stack_config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config/stack_config.h)

if(BUILD_EXAMPLES)
	add_subdirectory(examples)
endif(BUILD_EXAMPLES)

add_subdirectory(src)

INSTALL(FILES ${API_HEADERS} DESTINATION include/libiec61850)


