include_directories(
   .
)

set(example_SRCS
   tls_server_example.c
   static_model.c
)

IF(MSVC)
set_source_files_properties(${example_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(MSVC)

configure_file(server_CA1_1.key server_CA1_1.key COPYONLY)
configure_file(server_CA1_1.pem server_CA1_1.pem COPYONLY)
configure_file(client_CA1_1.pem client_CA1_1.pem COPYONLY)
configure_file(client_CA1_2.pem client_CA1_2.pem COPYONLY)
configure_file(root_CA1.pem root_CA1.pem COPYONLY)

add_executable(tls_server_example
  ${example_SRCS}
)

target_link_libraries(tls_server_example
    iec61850
)
