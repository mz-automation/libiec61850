
set(r_goose_publisher_example_SRCS
   r_goose_publisher_example.c
)

IF(MSVC)

set_source_files_properties(${r_goose_publisher_example_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(MSVC)                                       
                                       
add_executable(r_goose_publisher_example
  ${r_goose_publisher_example_SRCS}
)

target_link_libraries(r_goose_publisher_example
    iec61850
)


