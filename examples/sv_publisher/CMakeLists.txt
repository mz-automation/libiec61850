
set(sv_publisher_example_SRCS
   sv_publisher_example.c
)

IF(WIN32)

set_source_files_properties(${sv_publisher_example_SRCS}
                                       PROPERTIES LANGUAGE CXX)
add_executable(sv_publisher_example
  ${sv_publisher_example_SRCS}
)

target_link_libraries(sv_publisher_example
    iec61850
)

ELSE(WIN32)

add_executable(sv_publisher_example
  ${sv_publisher_example_SRCS}
)

target_link_libraries(sv_publisher_example
    iec61850
)

ENDIF(WIN32)


