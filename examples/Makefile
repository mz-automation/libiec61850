
EXAMPLE_DIRS = mms_client_example1
EXAMPLE_DIRS += mms_client_example2
EXAMPLE_DIRS += mms_client_example3 
EXAMPLE_DIRS += mms_client_example4
EXAMPLE_DIRS += mms_client_example5
EXAMPLE_DIRS += iec61850_client_example1
EXAMPLE_DIRS += iec61850_client_example2
EXAMPLE_DIRS += iec61850_client_example3
EXAMPLE_DIRS += iec61850_client_example4
EXAMPLE_DIRS += iec61850_client_example5
EXAMPLE_DIRS += iec61850_client_example_reporting
EXAMPLE_DIRS += server_example1
EXAMPLE_DIRS += server_example2
EXAMPLE_DIRS += server_example3
EXAMPLE_DIRS += server_example4
EXAMPLE_DIRS += server_example5
EXAMPLE_DIRS += server_example_goose
EXAMPLE_DIRS += server_example_control
EXAMPLE_DIRS += server_example_config_file
EXAMPLE_DIRS += server_example_dynamic
EXAMPLE_DIRS += server_example_complex_array
EXAMPLE_DIRS += server_example_61400_25
EXAMPLE_DIRS += server_example_threadless
EXAMPLE_DIRS += server_example_setting_groups
EXAMPLE_DIRS += goose_subscriber
EXAMPLE_DIRS += goose_publisher
EXAMPLE_DIRS += sv_subscriber
EXAMPLE_DIRS += mms_utility

MODEL_DIRS += server_example1
MODEL_DIRS += server_example2
MODEL_DIRS += server_example3
MODEL_DIRS += server_example4
MODEL_DIRS += server_example5
MODEL_DIRS += server_example_goose
MODEL_DIRS += server_example_control
MODEL_DIRS += server_example_config_file
MODEL_DIRS += server_example_complex_array
MODEL_DIRS += server_example_61400_25
MODEL_DIRS += server_example_threadless
MODEL_DIRS += server_example_setting_groups

all:	examples

model:
	-for d in $(MODEL_DIRS); do(cd $$d && $(MAKE) model); done

examples:
	-for d in $(EXAMPLE_DIRS); do(cd $$d && $(MAKE)); done

clean:
	-for d in $(EXAMPLE_DIRS); do(cd $$d && $(MAKE) clean); done
	
