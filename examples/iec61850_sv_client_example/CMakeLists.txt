
set(iec61850_sv_client_example_SRCS
   sv_client_example.c
)

IF(MSVC)
set_source_files_properties(${iec61850_sv_client_example_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(MSVC)

add_executable(iec61850_sv_client_example
  ${iec61850_sv_client_example_SRCS}
)

target_link_libraries(iec61850_sv_client_example
    iec61850
)
