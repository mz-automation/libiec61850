
set(sv_subscriber_example_SRCS
   sv_subscriber_example.c
)

IF(WIN32)

IF(WITH_WPCAP)

set_source_files_properties(${sv_subscriber_example_SRCS}
                                       PROPERTIES LANGUAGE CXX)
add_executable(sv_subscriber_example
  ${sv_subscriber_example_SRCS}
)

target_link_libraries(svsubscriber_example
    iec61850
)

ENDIF(WITH_WPCAP)

ELSE(WIN32)

add_executable(sv_subscriber_example
  ${sv_subscriber_example_SRCS}
)

target_link_libraries(sv_subscriber_example
    iec61850
)

ENDIF(WIN32)


