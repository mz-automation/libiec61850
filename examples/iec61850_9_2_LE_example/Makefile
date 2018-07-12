LIBIEC_HOME=../..

PROJECT_BINARY_NAME = sv_9_2LE_example
PROJECT_SOURCES = iec61850_9_2_LE_example.c
PROJECT_SOURCES += static_model.c

PROJECT_ICD_FILE = sv.icd

include $(LIBIEC_HOME)/make/target_system.mk
include $(LIBIEC_HOME)/make/stack_includes.mk

LDFLAGS += -lm

all:	$(PROJECT_BINARY_NAME)

include $(LIBIEC_HOME)/make/common_targets.mk

model:	$(PROJECT_ICD_FILE)
	java -jar $(LIBIEC_HOME)/tools/model_generator/genmodel.jar $(PROJECT_ICD_FILE)

$(PROJECT_BINARY_NAME):	$(PROJECT_SOURCES) $(LIB_NAME)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(PROJECT_BINARY_NAME) $(PROJECT_SOURCES) $(INCLUDES) $(LIB_NAME) $(LDFLAGS) $(LDLIBS)

clean:
	rm -f $(PROJECT_BINARY_NAME)

