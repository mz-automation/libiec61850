
set(iec61850_client_example_files_SRCS
   file-tool.c
)

IF(MSVC)
set_source_files_properties(${iec61850_client_example_files_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(MSVC)

add_executable(file-tool
  ${iec61850_client_example_files_SRCS}
)

target_link_libraries(file-tool
    iec61850
)
