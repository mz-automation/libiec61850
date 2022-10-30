
set(sntp_example_SRCS
   sntp_example.c
)

IF(WIN32)

set_source_files_properties(${sntp_example_SRCS}
                                       PROPERTIES LANGUAGE CXX)
add_executable(sntp_example
  ${sntp_example_SRCS}
)

target_link_libraries(sntp_example
    iec61850
)

ELSE(WIN32)

add_executable(sntp_example
  ${sntp_example_SRCS}
)

target_link_libraries(sntp_example
    iec61850
)

ENDIF(WIN32)


