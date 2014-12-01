include_directories(
   .
)

set(beagle_demo_SRCS
   beagle_demo.c
   static_model.c
)

IF(WIN32)
set_source_files_properties(${beagle_demo_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(WIN32)

add_executable(server_example3
  ${beagle_demo_SRCS}
)

target_link_libraries(server_example3
    iec61850
)
