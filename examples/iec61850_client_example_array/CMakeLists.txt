
set(iec61850_client_example_array_SRCS
   client_example_array.c
)

IF(MSVC)
set_source_files_properties(${iec61850_client_example_array_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(MSVC)

add_executable(iec61850_client_example_array
  ${iec61850_client_example_array_SRCS}
)

target_link_libraries(iec61850_client_example_array
    iec61850
)
