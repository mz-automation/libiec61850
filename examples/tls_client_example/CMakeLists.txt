
set(example_SRCS
   tls_client_example.c
)

IF(MSVC)
set_source_files_properties(${example_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(MSVC)

configure_file(client_CA1_1.key client_CA1_1.key COPYONLY)
configure_file(client_CA1_1.pem client_CA1_1.pem COPYONLY)
configure_file(root_CA1.pem root_CA1.pem COPYONLY)

add_executable(tls_client_example
  ${example_SRCS}
)

target_link_libraries(tls_client_example
    iec61850
)
