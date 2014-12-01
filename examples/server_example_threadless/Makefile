LIBIEC_HOME=../..

PROJECT_BINARY_NAME = server_example_threadless
PROJECT_SOURCES = server_example_threadless.c
PROJECT_SOURCES += static_model.c

include $(LIBIEC_HOME)/make/target_system.mk
include $(LIBIEC_HOME)/make/stack_includes.mk

all:	$(PROJECT_BINARY_NAME)

include $(LIBIEC_HOME)/make/common_targets.mk

LDLIBS += -lm

CP = cp

$(PROJECT_BINARY_NAME):	$(PROJECT_SOURCES) $(LIB_NAME)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(PROJECT_BINARY_NAME) $(PROJECT_SOURCES) $(INCLUDES) $(LIB_NAME) $(LDLIBS)
	$(CP) $(PROJECT_BINARY_NAME) vmd-filestore/IEDSERVER.BIN

clean:
	rm -f $(PROJECT_BINARY_NAME)
	rm -f vmd-filestore/IEDSERVER.BIN


