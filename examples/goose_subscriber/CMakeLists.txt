
set(goose_subscriber_example_SRCS
   goose_subscriber_example.c
)

IF(WIN32)

IF(WITH_WPCAP)

set_source_files_properties(${goose_subscriber_example_SRCS}
                                       PROPERTIES LANGUAGE CXX)
add_executable(goose_subscriber_example
  ${goose_subscriber_example_SRCS}
)

target_link_libraries(goose_subscriber_example
    iec61850
)

ENDIF(WITH_WPCAP)

ELSE(WIN32)

add_executable(goose_subscriber_example
  ${goose_subscriber_example_SRCS}
)

target_link_libraries(goose_subscriber_example
    iec61850
)

ENDIF(WIN32)


