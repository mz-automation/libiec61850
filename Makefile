LIBIEC_HOME=.

include make/target_system.mk

LIB_SOURCE_DIRS = src/mms/iso_acse
LIB_SOURCE_DIRS += src/mms/iso_acse/asn1c
LIB_SOURCE_DIRS += src/mms/iso_presentation/asn1c 
LIB_SOURCE_DIRS += src/mms/iso_presentation
LIB_SOURCE_DIRS += src/mms/iso_session
LIB_SOURCE_DIRS += src/common
LIB_SOURCE_DIRS += src/mms/asn1
LIB_SOURCE_DIRS += src/mms/iso_cotp
LIB_SOURCE_DIRS += src/mms/iso_mms/server
LIB_SOURCE_DIRS += src/mms/iso_mms/client
LIB_SOURCE_DIRS += src/mms/iso_client
LIB_SOURCE_DIRS += src/mms/iso_common
LIB_SOURCE_DIRS += src/mms/iso_mms/common
LIB_SOURCE_DIRS += src/mms/iso_mms/asn1c
LIB_SOURCE_DIRS += src/mms/iso_server

LIB_SOURCE_DIRS += src/logging

ifndef EXCLUDE_ETHERNET_WINDOWS
LIB_SOURCE_DIRS += src/goose
LIB_SOURCE_DIRS += src/sampled_values
endif
LIB_SOURCE_DIRS += src/iec61850/client
LIB_SOURCE_DIRS += src/iec61850/common
LIB_SOURCE_DIRS += src/iec61850/server
LIB_SOURCE_DIRS += src/iec61850/server/model
LIB_SOURCE_DIRS += src/iec61850/server/mms_mapping
LIB_SOURCE_DIRS += src/iec61850/server/impl
ifeq ($(HAL_IMPL), WIN32)
LIB_SOURCE_DIRS += hal/socket/win32
LIB_SOURCE_DIRS += hal/thread/win32
LIB_SOURCE_DIRS += hal/ethernet/win32
LIB_SOURCE_DIRS += hal/filesystem/win32
LIB_SOURCE_DIRS += hal/time/win32
LIB_SOURCE_DIRS += hal/serial/win32
LIB_SOURCE_DIRS += hal/memory
else ifeq ($(HAL_IMPL), POSIX)
LIB_SOURCE_DIRS += hal/socket/linux
LIB_SOURCE_DIRS += hal/thread/linux
LIB_SOURCE_DIRS += hal/ethernet/linux
LIB_SOURCE_DIRS += hal/filesystem/linux
LIB_SOURCE_DIRS += hal/time/unix
LIB_SOURCE_DIRS += hal/serial/linux
LIB_SOURCE_DIRS += hal/memory
else ifeq ($(HAL_IMPL), BSD)
LIB_SOURCE_DIRS += hal/socket/bsd
LIB_SOURCE_DIRS += hal/thread/bsd
LIB_SOURCE_DIRS += hal/ethernet/bsd
LIB_SOURCE_DIRS += hal/filesystem/linux
LIB_SOURCE_DIRS += hal/time/unix
LIB_SOURCE_DIRS += hal/memory
else ifeq ($(HAL_IMPL), MACOS)
LIB_SOURCE_DIRS += hal/socket/bsd
LIB_SOURCE_DIRS += hal/thread/macos
LIB_SOURCE_DIRS += hal/ethernet/bsd
LIB_SOURCE_DIRS += hal/filesystem/linux
LIB_SOURCE_DIRS += hal/time/unix
LIB_SOURCE_DIRS += hal/memory
endif
LIB_INCLUDE_DIRS += config
LIB_INCLUDE_DIRS += hal/inc
LIB_INCLUDE_DIRS += src/common/inc
LIB_INCLUDE_DIRS += src/mms/iso_mms/asn1c
LIB_INCLUDE_DIRS += src/mms/inc
LIB_INCLUDE_DIRS += src/mms/inc_private
LIB_INCLUDE_DIRS += src/goose
LIB_INCLUDE_DIRS += src/sampled_values
LIB_INCLUDE_DIRS += src/iec61850/inc
LIB_INCLUDE_DIRS += src/iec61850/inc_private
LIB_INCLUDE_DIRS += src/logging
LIB_INCLUDE_DIRS += src/tls
ifeq ($(HAL_IMPL), WIN32)
LIB_INCLUDE_DIRS += third_party/winpcap/Include
endif

ifdef WITH_MBEDTLS
LIB_SOURCE_DIRS += third_party/mbedtls/mbedtls-2.16/library
LIB_SOURCE_DIRS += hal/tls/mbedtls
LIB_INCLUDE_DIRS += third_party/mbedtls/mbedtls-2.16/include
LIB_INCLUDE_DIRS += hal/tls/mbedtls
CFLAGS += -D'MBEDTLS_CONFIG_FILE="mbedtls_config.h"'
CFLAGS += -D'CONFIG_MMS_SUPPORT_TLS=1'
endif

LIB_INCLUDES = $(addprefix -I,$(LIB_INCLUDE_DIRS))

ifndef INSTALL_PREFIX
INSTALL_PREFIX = ./.install
endif

LIB_API_HEADER_FILES = hal/inc/hal_time.h 
LIB_API_HEADER_FILES += hal/inc/hal_thread.h
LIB_API_HEADER_FILES += hal/inc/hal_filesystem.h
LIB_API_HEADER_FILES += hal/inc/tls_config.h
LIB_API_HEADER_FILES += hal/inc/lib_memory.h
LIB_API_HEADER_FILES += hal/inc/hal_base.h
LIB_API_HEADER_FILES += src/common/inc/libiec61850_common_api.h
LIB_API_HEADER_FILES += src/common/inc/linked_list.h
LIB_API_HEADER_FILES += src/iec61850/inc/iec61850_client.h
LIB_API_HEADER_FILES += src/iec61850/inc/iec61850_common.h
LIB_API_HEADER_FILES += src/iec61850/inc/iec61850_server.h
LIB_API_HEADER_FILES += src/iec61850/inc/iec61850_model.h
LIB_API_HEADER_FILES += src/iec61850/inc/iec61850_cdc.h
LIB_API_HEADER_FILES += src/iec61850/inc/iec61850_dynamic_model.h
LIB_API_HEADER_FILES += src/iec61850/inc/iec61850_config_file_parser.h
LIB_API_HEADER_FILES += src/mms/inc/mms_value.h
LIB_API_HEADER_FILES += src/mms/inc/mms_common.h
LIB_API_HEADER_FILES += src/mms/inc/mms_types.h
LIB_API_HEADER_FILES += src/mms/inc/mms_type_spec.h
LIB_API_HEADER_FILES += src/mms/inc/mms_client_connection.h
LIB_API_HEADER_FILES += src/mms/inc/mms_server.h
LIB_API_HEADER_FILES += src/mms/inc/iso_connection_parameters.h
LIB_API_HEADER_FILES += src/goose/goose_subscriber.h
LIB_API_HEADER_FILES += src/goose/goose_receiver.h
LIB_API_HEADER_FILES += src/goose/goose_publisher.h
LIB_API_HEADER_FILES += src/sampled_values/sv_subscriber.h
LIB_API_HEADER_FILES += src/sampled_values/sv_publisher.h
LIB_API_HEADER_FILES += src/logging/logging_api.h

get_sources_from_directory  = $(wildcard $1/*.c)
get_sources = $(foreach dir, $1, $(call get_sources_from_directory,$(dir)))
src_to = $(addprefix $(LIB_OBJS_DIR)/,$(subst .c,$1,$2))

LIB_SOURCES = $(call get_sources,$(LIB_SOURCE_DIRS))

LIB_OBJS = $(call src_to,.o,$(LIB_SOURCES))

CFLAGS += -std=gnu99
CFLAGS += -Wno-error=format 
CFLAGS += -Wstrict-prototypes

ifneq ($(HAL_IMPL), WIN32)
CFLAGS += -Wuninitialized 
endif

CFLAGS += -Wsign-compare 
CFLAGS += -Wpointer-arith 
CFLAGS += -Wnested-externs 
CFLAGS += -Wmissing-declarations 
CFLAGS += -Wshadow
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Wno-format
#CFLAGS += -Wconditional-uninitialized
#CFLAGS += -Werror  

all:	lib

static_checks:	lib
	splint -preproc +posixlib +skip-sys-headers +gnuextensions $(LIB_INCLUDES) $(LIB_SOURCES)

cppcheck:	lib
	cppcheck --force --std=c99 --enable=all $(LIB_INCLUDES) $(LIB_SOURCES) 2> cppcheck-output.xml

lib:	$(LIB_NAME)

dynlib: CFLAGS += -fPIC

dynlib:	$(DYN_LIB_NAME)

.PHONY:	examples

examples:
	cd examples; $(MAKE)

$(LIB_NAME):	$(LIB_OBJS)
	$(AR) r $(LIB_NAME) $(LIB_OBJS)
	$(RANLIB) $(LIB_NAME)
	
$(DYN_LIB_NAME):	$(LIB_OBJS)
	$(CC) $(LDFLAGS) $(DYNLIB_LDFLAGS) -shared -o $(DYN_LIB_NAME) $(LIB_OBJS) $(LDLIBS)

$(LIB_OBJS_DIR)/%.o: %.c config
	@echo compiling $(notdir $<)
	$(SILENCE)mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $(LIB_INCLUDES) $(OUTPUT_OPTION) $<
	
install:	$(LIB_NAME)
	mkdir -p $(INSTALL_PREFIX)/include
	mkdir -p $(INSTALL_PREFIX)/lib
	cp $(LIB_API_HEADER_FILES) $(INSTALL_PREFIX)/include
	cp $(LIB_NAME) $(INSTALL_PREFIX)/lib

clean:
	rm -f $(EXAMPLES)
	rm -rf $(LIB_OBJS_DIR)

