include_directories(
   .
)

set(server_example_ca_SRCS
   server_example_ca.c
   static_model.c
)

IF(MSVC)
set_source_files_properties(${server_example_ca_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(MSVC)

add_executable(server_example_ca
  ${server_example_ca_SRCS}
)

target_link_libraries(server_example_ca
    iec61850
)
