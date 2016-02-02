include_directories(
   .
)

set(sv_9_2_LE_example_SRCS
   iec61850_9_2_LE_example.c
   static_model.c
)

IF(WIN32)
set_source_files_properties(${sv_9_2_LE_example_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(WIN32)

add_executable(sv_9_2_LE_example
  ${sv_9_2_LE_example_SRCS}
)

target_link_libraries(sv_9_2_LE_example
    iec61850
)
