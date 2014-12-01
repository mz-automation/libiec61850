include_directories(
   .
)

set(server_example_config_file_SRCS
   server_example_config_file.c
)

IF(WIN32)
set_source_files_properties(${server_example_config_file_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(WIN32)

add_executable(server_example_config_file
  ${server_example_config_file_SRCS}
)

target_link_libraries(server_example_config_file
    iec61850
)
