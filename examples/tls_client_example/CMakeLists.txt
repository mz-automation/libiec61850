
set(example_SRCS
   tls_client_example.c
)

IF(WIN32)
set_source_files_properties(${example_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(WIN32)

add_executable(tls_client_example
  ${example_SRCS}
)

target_link_libraries(tls_client_example
    iec61850
)
