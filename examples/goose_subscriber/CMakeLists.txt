
set(goose_subscriber_example_SRCS
   goose_subscriber_example.c
)

IF(MSVC)

set_source_files_properties(${goose_subscriber_example_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(MSVC)
                                       
add_executable(goose_subscriber_example
  ${goose_subscriber_example_SRCS}
)

target_link_libraries(goose_subscriber_example
    iec61850
)


