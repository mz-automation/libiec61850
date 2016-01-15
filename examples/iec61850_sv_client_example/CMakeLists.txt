
set(iec61850_sv_client_example_SRCS
   sv_client_example.c
)

IF(WIN32)
set_source_files_properties(${iec61850_sv_client_example_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(WIN32)

add_executable(iec61850_sv_client_example
  ${iec61850_sv_client_example_SRCS}
)

target_link_libraries(iec61850_sv_client_example
    iec61850
)
