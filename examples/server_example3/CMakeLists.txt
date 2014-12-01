include_directories(
   .
)

set(server_example3_SRCS
   server_example3.c
   static_model.c
)

IF(WIN32)
set_source_files_properties(${server_example3_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(WIN32)

add_executable(server_example3
  ${server_example3_SRCS}
)

target_link_libraries(server_example3
    iec61850
)
