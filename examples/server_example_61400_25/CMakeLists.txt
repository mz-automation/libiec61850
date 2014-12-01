include_directories(
   .
)

set(server_example_61400_25_SRCS
   server_example_61400_25.c
   static_model.c
)

IF(WIN32)
set_source_files_properties(${server_example_61400_25_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(WIN32)

add_executable(server_example_61400_25
  ${server_example_61400_25_SRCS}
)

target_link_libraries(server_example_61400_25
    iec61850
)
