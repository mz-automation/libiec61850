
set(sv_subscriber_example_SRCS
   sv_subscriber_example.c
)

IF(MSVC)

set_source_files_properties(${sv_subscriber_example_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(MSVC)
                                       
add_executable(sv_subscriber_example
  ${sv_subscriber_example_SRCS}
)

target_link_libraries(sv_subscriber_example
    iec61850
)



