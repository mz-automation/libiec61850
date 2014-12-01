
set(iec61850_client_example4_SRCS
   client_example4.c
)

IF(WIN32)
set_source_files_properties(${iec61850_client_example4_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(WIN32)

add_executable(iec61850_client_example4
  ${iec61850_client_example4_SRCS}
)

target_link_libraries(iec61850_client_example4
    iec61850
)
