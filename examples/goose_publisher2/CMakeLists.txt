
set(goose_publisher_example_SRCS
   goose_publisher_example2.c
)

IF(MSVC)

set_source_files_properties(${goose_publisher_example_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(MSVC)                                       
                                       
add_executable(goose_publisher_example2
  ${goose_publisher_example_SRCS}
)

target_link_libraries(goose_publisher_example2
    iec61850
)


