
set(iec61850_client_example_reporting_SRCS
   client_example_reporting.c
)

IF(MSVC)
set_source_files_properties(${iec61850_client_example_reporting_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(MSVC)

add_executable(iec61850_client_example_reporting
  ${iec61850_client_example_reporting_SRCS}
)

target_link_libraries(iec61850_client_example_reporting
    iec61850
)
