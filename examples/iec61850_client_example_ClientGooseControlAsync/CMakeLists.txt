
set(iec61850_client_example_ClientGooseControlAsync_SRCS
	client_example_ClientGooseControlAsync.c
)

IF(MSVC)
set_source_files_properties(${iec61850_client_example_ClientGooseControlAsync_SRCS}
									   PROPERTIES LANGUAGE CXX)
ENDIF(MSVC)

add_executable(iec61850_client_example_ClientGooseControlAsync
  ${iec61850_client_example_ClientGooseControlAsync_SRCS}
)

target_link_libraries(iec61850_client_example_ClientGooseControlAsync
	iec61850
)