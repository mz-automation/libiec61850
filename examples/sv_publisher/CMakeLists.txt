
set(sv_publisher_example_SRCS
   sv_publisher_example.c
)

IF(MSVC)

set_source_files_properties(${sv_publisher_example_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(MSVC)
                                       
add_executable(sv_publisher_example
  ${sv_publisher_example_SRCS}
)

target_link_libraries(sv_publisher_example
    iec61850
)



