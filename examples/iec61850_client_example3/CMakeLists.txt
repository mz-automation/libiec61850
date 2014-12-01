
set(iec61850_client_example3_SRCS
   client_example3.c
)

IF(WIN32)
set_source_files_properties(${iec61850_client_example3_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(WIN32)

add_executable(iec61850_client_example3
  ${iec61850_client_example3_SRCS}
)

target_link_libraries(iec61850_client_example3
    iec61850
)
