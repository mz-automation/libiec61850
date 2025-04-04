        
set(iec61850_client_example_rcbAsync_SRCS
   client_example_rcbAsync.c
)

IF(MSVC)
set_source_files_properties(${iec61850_client_example_rcbAsync_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(MSVC)

add_executable(iec61850_client_example_rcbAsync
  ${iec61850_client_example_rcbAsync_SRCS}
)

target_link_libraries(iec61850_client_example_rcbAsync
    iec61850
)
