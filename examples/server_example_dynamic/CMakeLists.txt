include_directories(
   .
)

set(server_example_dynamic_SRCS
   server_example_dynamic.c
)

IF(WIN32)
set_source_files_properties(${server_example_dynamic_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(WIN32)

add_executable(server_example_dynamic
  ${server_example_dynamic_SRCS}
)

target_link_libraries(server_example_dynamic
    iec61850
)
