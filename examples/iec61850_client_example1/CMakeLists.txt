
set(iec61850_client_example1_SRCS
   client_example1.c
)

IF(MSVC)
set_source_files_properties(${iec61850_client_example1_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(MSVC)

add_executable(iec61850_client_example1
  ${iec61850_client_example1_SRCS}
)

target_link_libraries(iec61850_client_example1
    iec61850
)
