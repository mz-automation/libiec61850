include_directories(
   .
)

set(server_example4_SRCS
   server_example4.c
   static_model.c
)

IF(WIN32)
set_source_files_properties(${server_example4_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(WIN32)

add_executable(server_example4
  ${server_example4_SRCS}
)

target_link_libraries(server_example4
    iec61850
)
