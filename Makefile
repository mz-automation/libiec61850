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
LIB_SOURCE_DIRS += src/mms/iso_client/impl
LIB_SOURCE_DIRS += src/mms/iso_common
LIB_SOURCE_DIRS += src/mms/iso_mms/common
LIB_SOURCE_DIRS += src/mms/iso_mms/asn1c
LIB_SOURCE_DIRS += src/mms/iso_server
ifndef EXCLUDE_ETHERNET_WINDOWS
LIB_SOURCE_DIRS += src/goose
endif
LIB_SOURCE_DIRS += src/iedclient/impl
LIB_SOURCE_DIRS += src/iedcommon
LIB_SOURCE_DIRS += src/iedserver
LIB_SOURCE_DIRS += src/iedserver/model
LIB_SOURCE_DIRS += src/iedserver/mms_mapping
LIB_SOURCE_DIRS += src/iedserver/impl
LIB_SOURCE_DIRS += src/hal
ifeq ($(HAL_IMPL), WIN32)
LIB_SOURCE_DIRS += src/hal/socket/win32
LIB_SOURCE_DIRS += src/hal/thread/win32
LIB_SOURCE_DIRS += src/hal/ethernet/win32
LIB_SOURCE_DIRS += src/hal/filesystem/win32
LIB_SOURCE_DIRS += src/hal/time/win32
else ifeq ($(HAL_IMPL), POSIX)
LIB_SOURCE_DIRS += src/hal/socket/linux
LIB_SOURCE_DIRS += src/hal/thread/linux
LIB_SOURCE_DIRS += src/hal/ethernet/linux
LIB_SOURCE_DIRS += src/hal/filesystem/linux
LIB_SOURCE_DIRS += src/hal/time/unix
else ifeq ($(HAL_IMPL), BSD)
LIB_SOURCE_DIRS += src/hal/socket/bsd
LIB_SOURCE_DIRS += src/hal/thread/linux
LIB_SOURCE_DIRS += src/hal/ethernet/bsd
LIB_SOURCE_DIRS += src/hal/filesystem/linux
LIB_SOURCE_DIRS += src/hal/time/unix
endif

LIB_INCLUDE_DIRS +=	src/mms/iso_presentation
LIB_INCLUDE_DIRS +=	src/mms/iso_session
LIB_INCLUDE_DIRS +=	src/mms/iso_cotp
LIB_INCLUDE_DIRS +=	src/mms/iso_acse
LIB_INCLUDE_DIRS +=	config
LIB_INCLUDE_DIRS += src/mms/asn1
LIB_INCLUDE_DIRS += src/mms/iso_client
LIB_INCLUDE_DIRS +=	src/mms/iso_mms/server
LIB_INCLUDE_DIRS +=	src/mms/iso_mms/common
LIB_INCLUDE_DIRS += src/mms/iso_mms/client
LIB_INCLUDE_DIRS +=	src/mms/iso_mms/asn1c
LIB_INCLUDE_DIRS +=	src/common
LIB_INCLUDE_DIRS +=	src/hal/socket
LIB_INCLUDE_DIRS +=	src/hal/thread
LIB_INCLUDE_DIRS +=	src/hal/ethernet
LIB_INCLUDE_DIRS +=	src/hal/filesystem
LIB_INCLUDE_DIRS +=	src/hal/time
LIB_INCLUDE_DIRS += src/hal
LIB_INCLUDE_DIRS +=	src/goose
LIB_INCLUDE_DIRS +=	src/mms/iso_server
LIB_INCLUDE_DIRS +=	src/mms/iso_common
LIB_INCLUDE_DIRS += src/iedclient
LIB_INCLUDE_DIRS += src/iedcommon
LIB_INCLUDE_DIRS += src/iedserver
LIB_INCLUDE_DIRS += src/iedserver/model
LIB_INCLUDE_DIRS += src/iedserver/mms_mapping
LIB_INCLUDE_DIRS += src/iedserver/impl
ifeq ($(HAL_IMPL), WIN32)
LIB_INCLUDE_DIRS += third_party/winpcap/Include
endif

LIB_INCLUDES = $(addprefix -I,$(LIB_INCLUDE_DIRS))

ifndef INSTALL_PREFIX
INSTALL_PREFIX = ./.install
endif

LIB_API_HEADER_FILES = src/hal/time/time_hal.h 
LIB_API_HEADER_FILES += src/hal/ethernet/ethernet.h 
LIB_API_HEADER_FILES += src/hal/thread/thread.h
LIB_API_HEADER_FILES += src/hal/filesystem/filesystem.h 
LIB_API_HEADER_FILES += src/common/libiec61850_common_api.h
LIB_API_HEADER_FILES += src/common/linked_list.h
LIB_API_HEADER_FILES += src/common/byte_buffer.h
LIB_API_HEADER_FILES += src/iedclient/iec61850_client.h
LIB_API_HEADER_FILES += src/iedcommon/iec61850_common.h
LIB_API_HEADER_FILES += src/iedserver/iec61850_server.h
LIB_API_HEADER_FILES += src/iedserver/model/model.h
LIB_API_HEADER_FILES += src/iedserver/model/cdc.h
LIB_API_HEADER_FILES += src/iedserver/model/dynamic_model.h
LIB_API_HEADER_FILES += src/iedserver/model/config_file_parser.h
LIB_API_HEADER_FILES += src/mms/iso_mms/common/mms_value.h
LIB_API_HEADER_FILES += src/mms/iso_mms/common/mms_common.h
LIB_API_HEADER_FILES += src/mms/iso_mms/common/mms_types.h
LIB_API_HEADER_FILES += src/mms/iso_mms/server/mms_device_model.h
LIB_API_HEADER_FILES += src/mms/iso_mms/server/mms_server.h
LIB_API_HEADER_FILES += src/mms/iso_mms/server/mms_named_variable_list.h
LIB_API_HEADER_FILES += src/mms/iso_mms/common/mms_type_spec.h
LIB_API_HEADER_FILES += src/mms/asn1/ber_integer.h
LIB_API_HEADER_FILES += src/mms/asn1/asn1_ber_primitive_value.h
LIB_API_HEADER_FILES += src/mms/iso_server/iso_server.h
LIB_API_HEADER_FILES += src/mms/iso_common/iso_connection_parameters.h
LIB_API_HEADER_FILES += src/goose/goose_subscriber.h
LIB_API_HEADER_FILES += src/mms/iso_mms/client/mms_client_connection.h
LIB_API_HEADER_FILES += src/mms/iso_client/iso_client_connection.h
LIB_API_HEADER_FILES += src/hal/socket/socket.h 

get_sources_from_directory  = $(wildcard $1/*.c)
get_sources = $(foreach dir, $1, $(call get_sources_from_directory,$(dir)))
src_to = $(addprefix $(LIB_OBJS_DIR)/,$(subst .c,$1,$2))

LIB_SOURCES = $(call get_sources,$(LIB_SOURCE_DIRS))

LIB_OBJS = $(call src_to,.o,$(LIB_SOURCES))

CFLAGS += -std=gnu99
#CFLAGS += -Wno-error=format 
CFLAGS += -Wstrict-prototypes 
CFLAGS += -Wuninitialized 
CFLAGS += -Wsign-compare 
CFLAGS += -Wpointer-arith 
CFLAGS += -Wnested-externs 
CFLAGS += -Wmissing-declarations 
CFLAGS += -Wshadow
CFLAGS += -Wall
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

