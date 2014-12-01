
set(mms_client_example3_SRCS
   mms_client_example3.c
)

IF(WIN32)
set_source_files_properties(${mms_client_example3_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(WIN32)

add_executable(mms_client_example3
  ${mms_client_example3_SRCS}
)

target_link_libraries(mms_client_example3
    iec61850
)
