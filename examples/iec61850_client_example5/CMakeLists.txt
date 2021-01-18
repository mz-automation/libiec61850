
set(iec61850_client_example5_SRCS
   client_example5.c
)

IF(MSVC)
set_source_files_properties(${iec61850_client_example5_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(MSVC)

add_executable(iec61850_client_example5
  ${iec61850_client_example5_SRCS}
)

target_link_libraries(iec61850_client_example5
    iec61850
)
