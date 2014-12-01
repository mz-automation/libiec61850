
set(iec61850_client_example_reporting_SRCS
   client_example_reporting.c
)

IF(WIN32)
set_source_files_properties(${iec61850_client_example_reporting_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(WIN32)

add_executable(iec61850_client_example_reporting
  ${iec61850_client_example_reporting_SRCS}
)

target_link_libraries(iec61850_client_example_reporting
    iec61850
)
