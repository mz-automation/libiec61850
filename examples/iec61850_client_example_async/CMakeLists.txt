
set(iec61850_client_async_SRCS
   client_example_async.c
)

IF(MSVC)
set_source_files_properties(${iec61850_client_async_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(MSVC)

add_executable(iec61850_client_async
  ${iec61850_client_async_SRCS}
)

target_link_libraries(iec61850_client_async
    iec61850
)
