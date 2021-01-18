include_directories(
   .
)

set(server_example_config_file_SRCS
   server_example_config_file.c
)

IF(MSVC)
set_source_files_properties(${server_example_config_file_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(MSVC)

add_executable(server_example_config_file
  ${server_example_config_file_SRCS}
)

target_link_libraries(server_example_config_file
    iec61850
)
