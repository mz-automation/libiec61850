
set(iec61850_client_no_thread_SRCS
   client_example_no_thread.c
)

IF(MSVC)
set_source_files_properties(${iec61850_client_no_thread_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(MSVC)

add_executable(iec61850_client_no_thread
  ${iec61850_client_no_thread_SRCS}
)

target_link_libraries(iec61850_client_no_thread
    iec61850
)
