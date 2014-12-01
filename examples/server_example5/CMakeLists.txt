include_directories(
   .
)

set(server_example5_SRCS
   server_example5.c
   static_model.c
)

IF(WIN32)
set_source_files_properties(${server_example5_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(WIN32)

add_executable(server_example5
  ${server_example5_SRCS}
)

target_link_libraries(server_example5
    iec61850
)
