include_directories(
   .
)

set(server_example_threadless_SRCS
   server_example_threadless.c
   static_model.c
)

IF(MSVC)
set_source_files_properties(${server_example_threadless_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(MSVC)

add_executable(server_example_threadless
  ${server_example_threadless_SRCS}
)

target_link_libraries(server_example_threadless
    iec61850
)
