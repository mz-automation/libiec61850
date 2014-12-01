include_directories(
   .
)

set(server_example_goose_SRCS
   server_example_goose.c
   static_model.c
)

IF(WIN32)
set_source_files_properties(${server_example_goose_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(WIN32)

add_executable(server_example_goose
  ${server_example_goose_SRCS}
)

target_link_libraries(server_example_goose
    iec61850
)


