
set(iec61850_client_example2_SRCS
   client_example2.c
)

IF(MSVC)
set_source_files_properties(${iec61850_client_example2_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(MSVC)

add_executable(iec61850_client_example2
  ${iec61850_client_example2_SRCS}
)

target_link_libraries(iec61850_client_example2
    iec61850
)
