			
set(iec61850_client_example_ClientGooseControl_SRCS
	client_example_ClientGooseControl.c
)

IF(MSVC)
set_source_files_properties(${iec61850_client_example_ClientGooseControl_SRCS}
										PROPERTIES LANGUAGE CXX)
ENDIF(MSVC)

add_executable(iec61850_client_example_ClientGooseControl
	${iec61850_client_example_ClientGooseControl_SRCS}
)

target_link_libraries(iec61850_client_example_ClientGooseControl
	iec61850
)