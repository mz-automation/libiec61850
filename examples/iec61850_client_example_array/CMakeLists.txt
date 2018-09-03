
set(iec61850_client_example_array_SRCS
   client_example_array.c
)

IF(WIN32)
set_source_files_properties(${iec61850_client_example_array_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(WIN32)

add_executable(iec61850_client_example_array
  ${iec61850_client_example_array_SRCS}
)

target_link_libraries(iec61850_client_example_array
    iec61850
)
