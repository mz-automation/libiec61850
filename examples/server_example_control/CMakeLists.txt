include_directories(
   .
)

set(server_example_control_SRCS
   server_example_control.c
   static_model.c
)

IF(MSVC)
set_source_files_properties(${server_example_control_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(MSVC)

add_executable(server_example_control
  ${server_example_control_SRCS}
)

target_link_libraries(server_example_control
    iec61850
)
