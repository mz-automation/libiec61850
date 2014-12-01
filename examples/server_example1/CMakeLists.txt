include_directories(
   .
)

set(server_example1_SRCS
   server_example1.c
   static_model.c
)

IF(WIN32)
set_source_files_properties(${server_example1_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(WIN32)

add_executable(server_example1
  ${server_example1_SRCS}
)

target_link_libraries(server_example1
    iec61850
)
