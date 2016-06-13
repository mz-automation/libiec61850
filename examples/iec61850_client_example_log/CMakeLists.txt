
set(iec61850_client_example_log_SRCS
   client_example_log.c
)

IF(MSVC)
set_source_files_properties(${iec61850_client_example_log_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(MSVC)

add_executable(iec61850_client_example_log
  ${iec61850_client_example_log_SRCS}
)

target_link_libraries(iec61850_client_example_log
    iec61850
)
