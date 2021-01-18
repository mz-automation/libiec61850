include_directories(
   .
)

set(server_example_SRCS
   server_example_simple.c
   static_model.c
)

IF(MSVC)
set_source_files_properties(${server_example_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(MSVC)

add_executable(server_example_simple
  ${server_example_SRCS}
)

target_link_libraries(server_example_simple
    iec61850
)
