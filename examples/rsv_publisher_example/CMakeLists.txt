
set(r_sv_publisher_example_SRCS
   r_sv_publisher_example.c
)

IF(WIN32)

set_source_files_properties(${r_sv_publisher_example_SRCS}
                                       PROPERTIES LANGUAGE CXX)
add_executable(r_sv_publisher_example
  ${r_sv_publisher_example_SRCS}
)

target_link_libraries(r_sv_publisher_example
    iec61850
)

ELSE(WIN32)

add_executable(r_sv_publisher_example
  ${r_sv_publisher_example_SRCS}
)

target_link_libraries(r_sv_publisher_example
    iec61850
)

ENDIF(WIN32)
