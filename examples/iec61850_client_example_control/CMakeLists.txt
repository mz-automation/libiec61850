
set(iec61850_client_example_control_SRCS
   client_example_control.c
)

IF(MSVC)
set_source_files_properties(${iec61850_client_example_control_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(MSVC)

add_executable(client_example_control
  ${iec61850_client_example_control_SRCS}
)

target_link_libraries(client_example_control
    iec61850
)
