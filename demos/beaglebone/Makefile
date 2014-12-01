LIBIEC_HOME=../..

PROJECT_BINARY_NAME = beagle_demo
PROJECT_SOURCES = beagle_demo.c
PROJECT_SOURCES += beaglebone_leds.c
PROJECT_SOURCES += static_model.c

include $(LIBIEC_HOME)/make/target_system.mk
include $(LIBIEC_HOME)/make/stack_includes.mk

all:	$(PROJECT_BINARY_NAME)

include $(LIBIEC_HOME)/make/common_targets.mk

LDLIBS += -lm

$(PROJECT_BINARY_NAME):	$(PROJECT_SOURCES) $(LIB_NAME)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(PROJECT_BINARY_NAME) $(PROJECT_SOURCES) $(INCLUDES) $(LIB_NAME) $(LDLIBS)
	
beagle_client: beagle_client.c $(LIB_NAME)
	$(CC) $(CFLAGS) $(LDFLAGS) -o beagle_client beagle_client.c $(INCLUDES) $(LIB_NAME) $(LDLIBS)

clean:
	rm -f $(PROJECT_BINARY_NAME) beagle_client


