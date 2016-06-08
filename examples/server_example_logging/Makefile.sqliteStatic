LIBIEC_HOME=../..

PROJECT_BINARY_NAME = server_example_logging
PROJECT_SOURCES = server_example_logging.c
PROJECT_SOURCES += static_model.c
PROJECT_SOURCES += $(LIBIEC_HOME)/src/logging/drivers/sqlite/log_storage_sqlite.c
PROJECT_SOURCES += $(LIBIEC_HOME)/third_party/sqlite/sqlite3.c

PROJECT_ICD_FILE = simpleIO_direct_control.icd

include $(LIBIEC_HOME)/make/target_system.mk
include $(LIBIEC_HOME)/make/stack_includes.mk

all:	$(PROJECT_BINARY_NAME)

include $(LIBIEC_HOME)/make/common_targets.mk

CFLAGS += -I$(LIBIEC_HOME)/third_party/sqlite
CFLAGS += -DSQLITE_THREADSAFE=0 -DSQLITE_OMIT_LOAD_EXTENSION -DHAVE_USLEEP

LDLIBS += -lm

CP = cp

model:	$(PROJECT_ICD_FILE)
	java -jar $(LIBIEC_HOME)/tools/model_generator/genmodel.jar $(PROJECT_ICD_FILE)

$(PROJECT_BINARY_NAME):	$(PROJECT_SOURCES) $(LIB_NAME)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(PROJECT_BINARY_NAME) $(PROJECT_SOURCES) $(INCLUDES) $(LIB_NAME) $(LDLIBS)

clean:
	rm -f $(PROJECT_BINARY_NAME)


