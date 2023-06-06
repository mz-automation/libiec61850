
set(goose_subscriber_example_SRCS
   goose_subscriber_example_RT.c
)

IF(MSVC)

set_source_files_properties(${goose_subscriber_example_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(MSVC)
                                       
add_executable(goose_subscriber_example_RT
  ${goose_subscriber_example_SRCS}
)

target_link_libraries(goose_subscriber_example_RT
    iec61850
)


