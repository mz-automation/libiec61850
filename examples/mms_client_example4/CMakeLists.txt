
set(mms_client_example4_SRCS
   mms_client_example4.c
)

IF(WIN32)
set_source_files_properties(${mms_client_example4_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(WIN32)

add_executable(mms_client_example4
  ${mms_client_example4_SRCS}
)

target_link_libraries(mms_client_example4
    iec61850
)
