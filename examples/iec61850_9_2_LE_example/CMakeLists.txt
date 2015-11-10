include_directories(
   .
)

set(server_example2_SRCS
   server_example2.c
   static_model.c
)

IF(WIN32)
set_source_files_properties(${server_example2_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(WIN32)

add_executable(server_example2
  ${server_example2_SRCS}
)

target_link_libraries(server_example2
    iec61850
)
