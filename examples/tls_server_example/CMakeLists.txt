include_directories(
   .
)

set(example_SRCS
   tls_server_example.c
   static_model.c
)

IF(WIN32)
set_source_files_properties(${example_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(WIN32)

configure_file(server-key.pem server-key.pem COPYONLY)
configure_file(server.cer server.cer COPYONLY)
configure_file(client1.cer client1.cer COPYONLY)
configure_file(client2.cer client2.cer COPYONLY)
configure_file(root.cer root.cer COPYONLY)

add_executable(tls_server_example
  ${example_SRCS}
)

target_link_libraries(tls_server_example
    iec61850
)
