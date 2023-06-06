
set(sv_subscriber_example_SRCS
   sv_subscriber_example_RT.c
)

IF(MSVC)

set_source_files_properties(${sv_subscriber_example_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(MSVC)
                                       
add_executable(sv_subscriber_example_RT
  ${sv_subscriber_example_SRCS}
)

target_link_libraries(sv_subscriber_example_RT
)



